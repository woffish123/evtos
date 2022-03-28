#include "sx1262.h"
#include "hal.h"
#include "maclayer.h"
#include "netthd.h"
//uint8_t TestFlage ; 
// Bit0 : TxDone ,Bit1 : RxDone ,2 :CAD_TX_DETECT_TRY 3 :CAD_TX_DETECT_TRY_OVERTIME 
//    4 : CAD_TX_Done ,5 CAD_Rx_Done ,6 :TRX_TIMEOUT ,7 Head_ERR . 
// Local param
LoraState RFState = MODE_POWERDOWN;

#define  SPICmdBufCnt     10
static uint8_t       SpiRTxBuf[SPICmdBufCnt];



#define CadDelay    8000      //based on us   when try to recv the cad delay time .
static  uint32_t AirTime  =  200*64 ;     //send packet times  based on 15.625 us .  is used  to check if the send is overtime . cacl according to the current packet count .
static  uint32_t SleepTime = 2*64 ;   // sleep for 2ms
static  uint32_t RecvTime =  1*64 ;   // listen for 1ms 

/*
const uint16_t RFDELAYTIME[(End_type - None)] =
{ 
   1760, 51 ,171, 17 , 26, 43 , 64 ,21 , 32 , 22 , 39 , 9 ,47
};
*/


// poweroff
void RfPowerOff(void )
{
    
    // disable exti interrupt .
    ClearLoraInterrupt();    
    DisableLoraInterrupt();   
    // used PA15  LORA_SW_PORT :LORA_SW_PIN  to set Power up   .  sleep mode  set low  else set high .
    GPIO_ResetOutputPin(LORA_SW_PORT,LORA_SW_PIN);  
    // disable spi connect .
    GPIO_DisablePinInput(SPI_MISO_PORT,SPI_MISO_PIN);   
    GPIO_DisablePinInput(LORA_BUSY_PORT,LORA_BUSY_PIN);    
    GPIO_DisablePinInput(LORA_DIO1_PORT,LORA_DIO1_PIN);  
    GPIO_EnablePinOpendrain(SPI_MOSI_PORT,SPI_MOSI_PIN|SPI_CLK_PIN|SPI_CS_PIN);        
    
    // used PB8   LORA_RESET_PORT :LORA_RESET_PIN to reset  lora .  it should be done at very begainning .  low reset . high or oc  is free 
    GPIO_ResetOutputPin(LORA_RESET_PORT,LORA_RESET_PIN);          
    // used PB14  LORA_EN_PORT: LORA_EN_PIN to enable lora power  Low  : en ,  High: disable . 
    GPIO_SetOutputPin(LORA_EN_PORT,LORA_EN_PIN);   
    RFState = MODE_POWERDOWN ;   
    rflayerdatarelease();
}

void RfPowerOn(void)
{
    // used PB14  LORA_EN_PORT: LORA_EN_PIN to enable lora power  Low  : en   High disable . 
    GPIO_DisablePinOpendrain(LORA_EN_PORT,LORA_EN_PIN);
    GPIO_SetPinMode(LORA_EN_PORT,LORA_EN_PIN,GPIO_MODE_OUTPUT);
    GPIO_ResetOutputPin(LORA_EN_PORT,LORA_EN_PIN);   
    
    // used PB8   LORA_RESET_PORT :LORA_RESET_PIN to reset  lora .  it should be done at very begainning .  low reset . high or oc  is free 
    GPIO_SetPinMode(LORA_RESET_PORT,LORA_RESET_PIN,GPIO_MODE_OUTPUT);
    GPIO_SetOutputPin(LORA_RESET_PORT,LORA_RESET_PIN);      

}
void InitSpi(void)
{
    // used PD6   LORA_BUSY_PORT :LORA_BUSY_PIN to check lora state , Low ready , High :busy
    GPIO_SetPinMode(LORA_BUSY_PORT,LORA_BUSY_PIN,GPIO_MODE_INPUT);
    GPIO_EnablePinInput(LORA_BUSY_PORT,LORA_BUSY_PIN);       
    // used PA10  LORA_DIO1_PORT : LORA_DIO1_PIN to get lora interrupt .  rise eage : there is  interrupt .
    GPIO_SetPinMode(LORA_DIO1_PORT,LORA_DIO1_PIN,GPIO_MODE_INPUT);
    GPIO_EnablePinInput(LORA_DIO1_PORT,LORA_DIO1_PIN);      

    // clear exti flage
    EXTI_ClearFlag(GPIO_COMMON,GPIO_EXTI_LINE_2);    
    // set input edge. disable for default .
    EXTI_SetTrigEdge(GPIO_COMMON,GPIO_EXTI_LINE_2,GPIO_EXTI_TRIGGER_RISING);
    // enable filter
    EXTI_EnableDigitalFilter(GPIO_COMMON,GPIO_EXTI_LINE_2);
    // set exti pin , set PinA10 as input pin .
    GPIO_SetExitLine7(GPIO_COMMON,GPIO_EXTI_LINE_2_PA10);
    // used PA15  LORA_SW_PORT :LORA_SW_PIN  to set Power up   .  sleep mode  set low  else set high .
    GPIO_DisablePinOpendrain(LORA_SW_PORT,LORA_SW_PIN);
    GPIO_SetPinMode(LORA_SW_PORT,LORA_SW_PIN,GPIO_MODE_OUTPUT);
    GPIO_ResetOutputPin(LORA_SW_PORT,LORA_SW_PIN);    
    // used PC9   SPI_MISO_PORT:SPI_MISO_PIN   as spi miso .input .
    GPIO_SetPinMode(SPI_MISO_PORT,SPI_MISO_PIN,GPIO_MODE_DIGITAL);
    GPIO_EnablePinInput(SPI_MISO_PORT,SPI_MISO_PIN);     

    // used PC10  SPI_MOSI_PORT:SPI_MOSI_PIN   as spi mosi . output
    // used PC8   SPI_CLK_PORT: SPI_CLK_PIN    as SPI clk  . output
    // used PC7   SPI_CS_PORT:SPI_CS_PIN       as spi cs .   output low used,
    GPIO_DisablePinOpendrain(SPI_MOSI_PORT,SPI_MOSI_PIN|SPI_CLK_PIN|SPI_CS_PIN);
    GPIO_SetPinMode(SPI_MOSI_PORT,SPI_MOSI_PIN ,GPIO_MODE_DIGITAL);
    GPIO_SetPinMode(SPI_MOSI_PORT,SPI_CLK_PIN ,GPIO_MODE_DIGITAL);
    GPIO_SetPinMode(SPI_MOSI_PORT,SPI_CS_PIN,GPIO_MODE_DIGITAL);
    GPIO_SetOutputPin(SPI_MOSI_PORT,SPI_CS_PIN); 
    // total  : PA10 input ,PA15 output ,   PB8 output , PB14 output  ,  PC7 :output ,PC8 output ,PC9 input PC10 output , PD6 input
    
      
    
    // set spi2   
    EnablePeriph(Clock_SPI2);    
    /* 选择NSS脚控制模式  no NSS used  */
    SPI_SetSNNMode(SPI2,SPI_SSN_MODE_SOFTWARE);
    /* 外设工作主从模式 */
    SPI_SetSPIWorkMode(SPI2,SPI_WORK_MODE_MASTER);
    /* 总线通讯速率 */
    SPI_SetBaudrate(SPI2,SPI_BAUDRATEPRESCALER_DIV2);
    /* 数据bit方向 */
    SPI_SetBitOrder(SPI2,SPI_BIT_ORDER_MSB_FIRST);
    /* 总线数据位宽 */
    SPI_SetDataWidth(SPI2,SPI_DATAWIDTH_8BIT);
    /* 时钟相位 */
    SPI_SetClockPhase(SPI2,SPI_SPI_PHASE_1EDGE);
    /* 传输模式 双工半双工  */
    SPI_SetTransferMode(SPI2,SPI_MODE_FULL_DUPLEX);
    /* 时钟极性 */
    SPI_SetClockPolarity(SPI2,SPI_SPI_POLARITY_LOW);
    SPI_SetSSNPinMode(SPI2 ,SPI_SSN_SEND_MODE_LOW) ;
    SPI_SetSNNMode(SPI2,SPI_SSN_MODE_SOFTWARE);
    SPI_TxBuffClear(SPI2);
    SPI_RxBuffClear(SPI2);
    DisablePeriph(Clock_SPI2);  
    
}
// send and recv cmd . notice  when read data ,should add a 1 byte  after the command byte
//for  send  SpiRTxBuf[0] is command , 1...n  is the data send .
//for  recv  SpiRTxBuf[0] is command , [1]  is fixed as 0 , 2...n  is the data received . 
void SpiInOut(uint8_t len)
{
	uint8_t i ;
    uint8_t *lp ;
    lp = SpiRTxBuf ;
    INT_DISABLE();
    EnablePeriph(Clock_SPI2);  
    SPI_Enable(SPI2);
    SPI2->CR3 |= (SPI_CR3_TXBFC | SPI_CR3_RXBFC) ;
    LowCS();
	for( i = 0; i < len; i++ )
	{
		SPI2->TXBUF = *lp;
        while((SPI2->ISR & 0x3) != 0x03 )
            __nop();
		*lp = SPI2->RXBUF;
        lp++;
	}
    HighCS();
    SPI_Disable(SPI2);
    DisablePeriph(Clock_SPI2); 
    INT_ENABLE();
}
 
void Wakeup( void )
{
    SpiRTxBuf[0] = RADIO_GET_STATUS;
    SpiRTxBuf[1] = 0;
    SpiInOut(2);
    WaitBusy();
}

void CheckReady( void )
{
    if( ( RFState == MODE_SLEEP ) || (RFState == MODE_RX_DC ) )
    {
        Wakeup( );
        RFPowerOn();
    }
    WaitBusy();
}

// set 1 to clear the bit  .
void ClearIrqStatus( uint16_t irq )
{
    SpiRTxBuf[0] = RADIO_CLR_IRQSTATUS ;
    SpiRTxBuf[1] = (uint8_t)((irq>>8) &0xff );
    SpiRTxBuf[2] = (uint8_t)(irq & 0xff );
    SpiInOut(3);
    WaitBusy();
}



void Sleep(SleepMode mode)
{
    RFPowerOff();    
    CheckReady();
    SpiRTxBuf[0] = RADIO_SET_SLEEP;
    SpiRTxBuf[1] = mode;    
    SpiInOut(2);
    RFState =MODE_SLEEP ;
}

void SetDioIrqParams( uint16_t irqMask, uint16_t dio1Mask)
{
    SpiRTxBuf[0] = RADIO_CFG_DIOIRQ;

    SpiRTxBuf[1] = ( uint8_t )( ( irqMask >> 8 ) & 0x00FF );
    SpiRTxBuf[2] = ( uint8_t )( irqMask & 0x00FF );
    SpiRTxBuf[3] = ( uint8_t )( ( dio1Mask >> 8 ) & 0x00FF );
    SpiRTxBuf[4] = ( uint8_t )( dio1Mask & 0x00FF );

    SpiRTxBuf[5] = 0;
    SpiRTxBuf[6] = 0;
    SpiRTxBuf[7] = 0;
    SpiRTxBuf[8] = 0;
    SpiInOut(9);
    WaitBusy(); 
}
// goto sleep mode  use rc clock 
void SetStandby(uint8_t isxosc)
{
    CheckReady();
    RFPowerOff();
    SpiRTxBuf[0] = RADIO_SET_STANDBY;
    SpiRTxBuf[1] = isxosc ? MODE_STDBY_XOSC: MODE_STDBY_RC ;
    SpiInOut(2);
    WaitBusy();
    RFState =MODE_SLEEP ; 
}

void SetFs( void )
{
    CheckReady();
    ClearIrqStatus( IRQ_RADIO_ALL );
    SpiRTxBuf[0] = RADIO_SET_FS ;
    SpiInOut(1);
    WaitBusy();    
    RFState =MODE_FS ;    
}

// timeout  :0x000000 Timeout disable, Tx Single mode,  
//           the device will stay in TX Mode until the packet is transmitted and returns
//           in STBY_RC mode upon completion.
//          : others  ,timeout used , the real Timeout duration = Timeout * 15.625 μs , if timeover and  not send out  a  TIMEOUT interrrupt.
void SetTx( uint32_t timeout )
{
    CheckReady();
    ClearIrqStatus( IRQ_RADIO_ALL );
    SpiRTxBuf[0] = RADIO_SET_TX ;
    SpiRTxBuf[1] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    SpiRTxBuf[2] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    SpiRTxBuf[3] = ( uint8_t )( timeout & 0xFF ); 
    SpiInOut(4);
    WaitBusy();
    RFState =MODE_TX ; 
}
// timeout : 0x0 :    single recv  , no timeout . untill a recv interrupt .
//         : 0xffffff :continue recv  , will recv multi  recv interrupt
//         : others ,  single recv with timeout .  
void SetRx( uint32_t timeout )
{
    CheckReady();
    ClearIrqStatus( IRQ_RADIO_ALL );
    SpiRTxBuf[0] = RADIO_SET_RX ;
    SpiRTxBuf[1] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    SpiRTxBuf[2] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    SpiRTxBuf[3] = ( uint8_t )( timeout & 0xFF ); 
    SpiInOut(4);
    WaitBusy();
    RFState =MODE_RX ;     
}
// do Duty rx with listen for rxTime* 15.625 μs , then stop 500us , then sleep sleepTime * 15.625 μs   then listen again
// untill a  packet is received ,  or  SetStandby is calling .
// the rxTimes and sleepTime should :T preamble + T header ≤ 2 * rxPeriod + sleepPeriod
void SetRxDutyCycle( uint32_t rxTime, uint32_t sleepTime )
{
    CheckReady();
    ClearIrqStatus( IRQ_RADIO_ALL );
    SpiRTxBuf[0] = RADIO_SET_RXDUTYCYCLE ;
    SpiRTxBuf[1] = ( uint8_t )( ( rxTime >> 16 ) & 0xFF );
    SpiRTxBuf[2] = ( uint8_t )( ( rxTime >> 8 ) & 0xFF );
    SpiRTxBuf[3] = ( uint8_t )( rxTime & 0xFF );
    SpiRTxBuf[4] = ( uint8_t )( ( sleepTime >> 16 ) & 0xFF );
    SpiRTxBuf[5] = ( uint8_t )( ( sleepTime >> 8 ) & 0xFF );
    SpiRTxBuf[6] = ( uint8_t )( sleepTime & 0xFF );
    SpiInOut(7);
    WaitBusy();
    RFState = MODE_RX_DC;
}

// goto cad mode , the listen time is set by SetCadParams(...)
void SetCad()
{
    CheckReady();
    SpiRTxBuf[0] = RADIO_SET_CAD ;
    SpiInOut(1);
    WaitBusy();  
    RFState = MODE_CAD;
}
// set cad param
// cadSymbolNum  : how many symbol should be  listen .
// cadDetPeak and cadDetMin define the sensitivity of the LoRa modem when trying to correlate to actual LoRa preamble symbols
// Application note AN1200.48 provides guidance for the selection of these parameters.
// cadExitMode :if detected if goto recv mode 
// cadTimeout  :recv timeout value
void SetCadParams( RadioLoRaCadSymbols_t cadSymbolNum, uint8_t cadDetPeak, uint8_t cadDetMin, RadioCadExitModes_t cadExitMode, uint32_t cadTimeout )
{
    SpiRTxBuf[0] = RADIO_SET_CADPARAMS ;

    SpiRTxBuf[1] = ( uint8_t )cadSymbolNum;
    SpiRTxBuf[2] = cadDetPeak;
    SpiRTxBuf[3] = cadDetMin;
    SpiRTxBuf[4] = ( uint8_t )cadExitMode;
    SpiRTxBuf[5] = ( uint8_t )( ( cadTimeout >> 16 ) & 0xFF );
    SpiRTxBuf[6] = ( uint8_t )( ( cadTimeout >> 8 ) & 0xFF );
    SpiRTxBuf[7] = ( uint8_t )( cadTimeout & 0xFF );
    SpiInOut(8);
    WaitBusy();     
}
// tx continue send mode , for test used .  check the txpower and tx frenquency .
void SetTxContinuousWave( void )
{
    SpiRTxBuf[0] = RADIO_SET_TXCONTINUOUSWAVE ;
    SpiInOut(1);
    WaitBusy();     
    RFState = MODE_TX;
}
//tx continue send preamble , for test used .
void SetTxInfinitePreamble( void )
{
    SpiRTxBuf[0] = RADIO_SET_TXCONTINUOUSPREAMBLE ;
    SpiInOut(1);
    WaitBusy();     
    RFState = MODE_TX;    
}
// set stop detect preamble , if enable  = 1 :the detect preambel is stoped , device go to signle recv mode
void SetStopRxTimerOnPreambleDetect( uint8_t enable )
{
    SpiRTxBuf[0] = RADIO_SET_STOPRXTIMERONPREAMBLE ;
    SpiRTxBuf[1] = enable == 0 ? 0 : 1 ;
    SpiInOut(2);
    WaitBusy();     
  
}
// set the addtional symble bit need to check before to go rx mode .
// defualt the SymbNum is 0 , once detect the right Symb ,the device to go rx mode .
void SetLoRaSymbNumTimeout( uint8_t SymbNum )
{
    SpiRTxBuf[0] = RADIO_SET_LORASYMBTIMEOUT ;
    SpiRTxBuf[1] = SymbNum ;
    SpiInOut(2);
    WaitBusy();     
}
// if used dcdc , 0 used ldo only ,else use dcdc 
// use the dcdc will reduce 1/2 power .
void SetRegulatorMode( uint8_t  usedcdc )
{
    SpiRTxBuf[0] = RADIO_SET_REGULATORMODE ;
    SpiRTxBuf[1] = usedcdc ==0 ? 0 :1  ;
    SpiInOut(2);
    WaitBusy();        
}
// do calibration  for RC64K (bit0)  RC13M (bit1) PLL (bit2)  ADC pulse(bit3) ADC bulkN(bit4) ADC bulkP(bit5) image (bit6)
// only can be used in STDY_RC mode , and this function will used 3.5ms , after the time the busy will be low 
// it cab be see as reset  . both with same delay time and function
void Calibrate( CalibrationParams_t calibParam )
{
    CheckReady();
    SpiRTxBuf[0] = RADIO_CALIBRATE ;
    SpiRTxBuf[1] = calibParam.Value ;
    SpiInOut(2);
    WaitBusy(); 
    RFState = MODE_SLEEP; 

}
// set the rx gain  for  high sensitive :
// max LNA gain, increase current by ~2mA for around ~3dB in sensivity
// it can also set the rx timout value .
void SetRxBoosted( uint8_t highgain ,uint32_t timeout )
{
    SpiRTxBuf[0] = RADIO_WRITE_REGISTER ;
    SpiRTxBuf[1] = (uint8_t)((REG_RX_GAIN >>8) &0xff) ;
    SpiRTxBuf[2] = (uint8_t)( REG_RX_GAIN  &0xff) ;
    SpiRTxBuf[3] =  highgain == 0 ? 0x94 :0x96;

    SpiRTxBuf[4] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    SpiRTxBuf[5] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    SpiRTxBuf[6] = ( uint8_t )( timeout & 0xFF );
    SpiInOut(7);
    WaitBusy(); 
}
// do image calibration , by default  when cold start the device will auto calibrate the 850M  used XTAl ,but is used the TCXO it will fail
// if the used frequency is differ  user should call this function after  calling to SetDIO3AsTcxoCtrl

void CalibrateImage( uint32_t freq )
{
    SpiRTxBuf[0] = RADIO_CALIBRATEIMAGE ;

    if( freq > Band900FreCnt )
    {
        SpiRTxBuf[1] = 0xE1;
        SpiRTxBuf[2] = 0xE9;
    }
    else if( freq > Band850FreCnt )
    {
        SpiRTxBuf[1] = 0xD7;
        SpiRTxBuf[2] = 0xD8;
    }
    else if( freq > Band770FreCnt )
    {
        SpiRTxBuf[1] = 0xC1;
        SpiRTxBuf[2] = 0xC5;
    }
    else if( freq > Band460FreCnt )
    {
        SpiRTxBuf[1] = 0x75;
        SpiRTxBuf[2] = 0x81;
    }
    else if( freq > Band425FreCnt )
    {
        SpiRTxBuf[1] = 0x6B;
        SpiRTxBuf[2] = 0x6F;
    }
    SpiInOut(3);
    WaitBusy(); 
}
// set pa parameter .
void SetPaConfig( uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut )
{
    SpiRTxBuf[0] = RADIO_SET_PACONFIG ;
    SpiRTxBuf[1] = paDutyCycle;
    SpiRTxBuf[2] = hpMax;
    SpiRTxBuf[3] = deviceSel;
    SpiRTxBuf[4] = paLut;
    SpiInOut(5);
    WaitBusy();
}
// set the return mode after rx or tx , it can reduce the mode switch time if there is rx or tx action followed.
// fallbackMode  
void SetRxTxFallbackMode( FallMode mode )
{
    SpiRTxBuf[0] = RADIO_SET_TXFALLBACKMODE ;
    SpiRTxBuf[1] = mode;
    SpiInOut(2);
    WaitBusy();    
}

// set the Sync words as private net .
void SetSyncWords( void)
{
    SpiRTxBuf[0] = RADIO_WRITE_REGISTER ;
    SpiRTxBuf[1] = (uint8_t)((REG_LR_SYNCWORD >>8) &0xff) ;
    SpiRTxBuf[2] = (uint8_t)( REG_LR_SYNCWORD  &0xff) ;
    SpiRTxBuf[3] = (uint8_t)( (LORA_MAC_PRIVATE_SYNCWORD >> 8 ) & 0xFF);

    SpiRTxBuf[4] = ( uint8_t )(LORA_MAC_PRIVATE_SYNCWORD & 0xFF);
    SpiInOut(5);
    WaitBusy();     
}
// read back the interrupt register .
uint16_t GetIrqStatus( void )
{
    uint16_t res ;
    SpiRTxBuf[0] = RADIO_GET_IRQSTATUS;
    SpiRTxBuf[1] = 0 ;
    SpiRTxBuf[2] = 0 ;
    SpiRTxBuf[3] = 0 ;
    SpiInOut(4);
    WaitBusy();  
    res =  SpiRTxBuf[2] ;
    res <<= 8 ;
    res |= SpiRTxBuf[3] ;   
    return res;
}
// set  Dio2 as RF switch
void SetDio2AsRfSwitchCtrl( uint8_t enable )
{
    SpiRTxBuf[0] = RADIO_SET_RFSWITCHMODE;
    SpiRTxBuf[1] = enable ==0 ? 0 : 1;
    SpiInOut(2);
    WaitBusy();       
}
// set di03 as tcxo power ,and set the voltage ,and tcxo stable wait time 
// timeout is base on Delay(23:0) *15.625 μs  -> = n(ms) *64 * 15.625 μs =  n *1000us so n is base on ms 
// call timeout as  n(ms) <<6
void SetDio3AsTcxoCtrl( RadioTcxoCtrlVoltage_t tcxoVoltage, uint32_t timeout )
{
    CheckReady();
    SpiRTxBuf[0] = RADIO_SET_TCXOMODE;
    SpiRTxBuf[1] = tcxoVoltage & 0x07;
    SpiRTxBuf[2] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    SpiRTxBuf[3] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    SpiRTxBuf[4] = ( uint8_t )( timeout & 0xFF );

    SpiInOut(5);
    WaitBusy(); 
}
// freq based on 100KHz  value is  fre(100Khz)<<20  ==   fre<<18 / 10  <<2 
//  ==  Fre<<18/10 <<2    +  Fre<<18%10 <<2   ::  Fre<<18%10 <<2 is [0 9] *4  = [0 -36]  
// frequency is 0 - 9000 , should used 0- 2^14  store it .    
void SetRfFrequency( uint16_t frequency )
{
    QuotRemain qr ;
    qr.quotient = frequency ;
    qr.quotient <<= 18 ;
    qr.remainder = 10 ;
    HardDiv(&qr);
    qr.quotient <<=2 ;
    
    if(qr.remainder == 9  )
        qr.quotient += 4 ;
    else if(qr.remainder >= 7)
        qr.quotient += 3 ;
    else if(qr.remainder >= 4)
        qr.quotient += 2 ;
    else if(qr.remainder >= 2)
        qr.quotient += 1 ;
    

    SpiRTxBuf[0] =RADIO_SET_RFFREQUENCY ;
    SpiRTxBuf[1] = ( uint8_t )( ( qr.quotient >> 24 ) & 0xFF );
    SpiRTxBuf[2] = ( uint8_t )( ( qr.quotient >> 16 ) & 0xFF );
    SpiRTxBuf[3] = ( uint8_t )( ( qr.quotient >> 8 ) & 0xFF );
    SpiRTxBuf[4] = ( uint8_t )( qr.quotient & 0xFF );
    SpiInOut(5);
    WaitBusy();
}

void SetPacketType( )
{
   // set rf mode  0 :GFSK ,1 Lora  here fixed as 1    
    SpiRTxBuf[0] = RADIO_SET_PACKETTYPE;
    SpiRTxBuf[1] =  1;
    SpiInOut(2);
    WaitBusy();    
}
// set tx power and PA ramptime . quick ramp time will case  big noise to circult
void SetTxParams( int8_t power, RadioRampTimes_t rampTime )
{ //sx1261 and 1262 has differrnt parameter .
    
    SetPaConfig( 0x04, 0x07, 0x00, 0x01 );
    if( power > 22 )
        power = 22;
    else if( power < -3 )
        power = -3;

    SpiRTxBuf[0] = RADIO_WRITE_REGISTER ;
    SpiRTxBuf[1] = (uint8_t)((REG_OCP >>8) &0xff) ;
    SpiRTxBuf[2] = (uint8_t)( REG_OCP  &0xff) ;
    SpiRTxBuf[3] =  power >20 ? 0x38 : 0x18;
    SpiInOut(4);
    WaitBusy();
    SpiRTxBuf[0] = RADIO_SET_TXPARAMS ;
    SpiRTxBuf[1] = (uint8_t)power;
    SpiRTxBuf[2] = ( uint8_t )rampTime;
    SpiInOut(3);
    WaitBusy();
}
// set lora param 
// ldo should be 1 if LoRa? symbol time is equal or above 16.38 ms. 
void SetModulationParams( uint8_t sf,uint8_t bw ,uint8_t cr ,uint8_t ldo )
{
    SpiRTxBuf[0] = RADIO_SET_MODULATIONPARAMS ;
    SpiRTxBuf[1] = sf;
    SpiRTxBuf[2] = bw;
    SpiRTxBuf[3] = cr;
    SpiRTxBuf[4] = ldo;
    SpiInOut(5);
    WaitBusy();
    
    SpiRTxBuf[0] = RADIO_READ_REGISTER ;
    SpiRTxBuf[1] = (uint8_t)((REG_BW500_OP >>8) &0xff) ;
    SpiRTxBuf[2] = (uint8_t)( REG_BW500_OP  &0xff) ;
    SpiRTxBuf[3] = 0;
    SpiRTxBuf[4] = 0;
    SpiInOut(5);
    WaitBusy();        
    if(bw == LORA_BW_500)
    {
        SpiRTxBuf[3] = SpiRTxBuf[4] & 0xfb ;
    }else
    {
        SpiRTxBuf[3] = SpiRTxBuf[4] | 0x04 ;
    }
    SpiRTxBuf[0] = RADIO_WRITE_REGISTER ;
    SpiRTxBuf[1] = (uint8_t)((REG_BW500_OP >>8) &0xff) ;
    SpiRTxBuf[2] = (uint8_t)( REG_BW500_OP  &0xff) ;
    SpiInOut(4);
    WaitBusy();         

}
// headertype  : 0  explicit header  1 :  implicit header
// payloadcnt : 0- 0xff 
// crcmode  :  0 : off , 1 :on
// invertiq  0 : normal . 1 : invert IQ 
void SetPacketParams( uint8_t preamblecnt ,uint8_t headertype,uint8_t payloadcnt,uint8_t crcmode,uint8_t invertiq)
{
    SpiRTxBuf[0] = RADIO_SET_PACKETPARAMS ;
    SpiRTxBuf[1] = 0 ; //( PreambleLength >> 8 ) & 0xFF; do not used to long preamble 
    SpiRTxBuf[2] = preamblecnt;
    SpiRTxBuf[3] = headertype;
    SpiRTxBuf[4] = payloadcnt;
    SpiRTxBuf[5] = crcmode ;
    SpiRTxBuf[6] = invertiq;
    SpiInOut(7);
    WaitBusy();
}



void SetBufferBaseAddress( uint8_t txBaseAddress, uint8_t rxBaseAddress )
{
    SpiRTxBuf[0] = RADIO_SET_BUFFERBASEADDRESS ;
    SpiRTxBuf[1] = txBaseAddress;
    SpiRTxBuf[2] = rxBaseAddress;
    SpiInOut(3);
    WaitBusy();
}

RadioStatus_t GetStatus( void )
{
    RadioStatus_t status;

    SpiRTxBuf[0] = RADIO_GET_STATUS ;
    SpiRTxBuf[1] =0;
    SpiRTxBuf[2] =0;
    SpiInOut(3);
    WaitBusy();    
    status.Value = SpiRTxBuf[2];
    return status;
}
// get a rssi of last receive packet .
uint8_t GetRssiInst( void )
{
    SpiRTxBuf[0] = RADIO_GET_RSSIINST ;
    SpiRTxBuf[1] =0;
    SpiRTxBuf[2] =0;
    SpiInOut(3);
    WaitBusy();  
    // low 3 bit is not used
    return (SpiRTxBuf[2]+4) >>3;   
}
// get the new received packet length it will be stored at RecvCnt ,and ,base addr is fixed not chang .
uint8_t GetRxBufferStatus(  )
{
    uint8_t len;

    SpiRTxBuf[0] = RADIO_GET_RXBUFFERSTATUS ;
    SpiRTxBuf[1] =0;
    SpiRTxBuf[2] =0;
    SpiRTxBuf[3] =0;
    SpiInOut(4);
    WaitBusy();
    // In case of LORA fixed header, implict mode  the payloadLength is obtained by reading
    // the register REG_LR_PAYLOADLENGTH
    len = SpiRTxBuf[2];
 #ifdef USE_IMPLICT_HDEADE
    SpiRTxBuf[0] = RADIO_READ_REGISTER ;
    SpiRTxBuf[1] = (uint8_t)((REG_LR_PACKETPARAMS >>8) &0xff) ;
    SpiRTxBuf[2] = (uint8_t)( REG_LR_PACKETPARAMS  &0xff) ;
    SpiRTxBuf[3] =  0 ;
    SpiInOut(5);
    WaitBusy();
    if( SpiRTxBuf[4] & 0x80  )
    {
        SpiRTxBuf[0] = RADIO_READ_REGISTER ;
        SpiRTxBuf[1] = (uint8_t)((REG_LR_PAYLOADLENGTH >>8) &0xff) ;
        SpiRTxBuf[2] = (uint8_t)( REG_LR_PAYLOADLENGTH  &0xff) ;
        SpiRTxBuf[3] =  0 ;
        SpiRTxBuf[4] =  0 ;
        SpiInOut(5);
        WaitBusy();        
        len =  SpiRTxBuf[4];
    }
#endif   
   return len ;
}

RadioError_t GetDeviceErrors( void )
{
    RadioError_t error;
    SpiRTxBuf[0] = RADIO_GET_ERROR ;
    SpiRTxBuf[1] =0;
    SpiRTxBuf[2] =0;
    SpiRTxBuf[3] =0;
    SpiInOut(4);
    WaitBusy();
    error.Value = SpiRTxBuf[2] ;
    error.Value <<= 8 ;
    error.Value |= SpiRTxBuf[3] ;
    return error;
}

void ClearDeviceErrors( void )
{
    SpiRTxBuf[0] = RADIO_CLR_ERROR ;
    SpiRTxBuf[1] =0;
    SpiRTxBuf[2] =0;
    SpiInOut(3);
    WaitBusy();    
}


// send Fifo len is the paylad cnt start at  SpiQueueTxBuf[8] (PACKET_USERID_CNT +4)
void WriteFifo(RfMsg msg)
{   
    uint8_t i ,length,trycnt;  
    LPFrameCtrl lpFrame ;
    uint8_t *lp ;
    lpFrame = (LPFrameCtrl) getrfbuf(msg.value.memid);
    assert(lp !=NULL);
    length = lpFrame->MsgCnt ; 
    lpFrame->NetName = globaldata.cfgdata.NetName  ;
    lpFrame->SendAddr = globaldata.cfgdata.LocalAddr ;
    lpFrame->VarAddr = msg.value.addr ;
    
    ClearIrqStatus( IRQ_RADIO_ALL );
    ClearLoraInterrupt();
    EnableLoraInterrupt();    
    
    SetPacketParams(globaldata.cfgdata.PreambleNum,0,length,1,0);

    EnablePeriph(Clock_SPI2); 
	SPI_Enable(SPI2);
    INT_DISABLE();
    SPI2->CR3 |= (SPI_CR3_TXBFC | SPI_CR3_RXBFC) ;
    LowCS();
	SPI2->TXBUF = RADIO_WRITE_BUFFER;
    while((SPI2->ISR & 0x03)!= 0x03) 
            __nop();    
	SPI2->TXBUF = LORATXBASEADDR;
    while((SPI2->ISR & 0x03)!= 0x03) 
            __nop();    
    
    // set ip return to  buffer id position .
    lp = (uint8_t *) lpFrame;
    lp++ ;  // send from second byte .
	for( i = 0; i < length; i++ )
	{
		SPI2->TXBUF = *lp;
        while((SPI2->ISR & 0x03) != 0x03)
            __nop();
        lp++ ;
	}
    HighCS();

    INT_ENABLE();
    SPI_Disable(SPI2);
    DisablePeriph(Clock_SPI2); 
    WaitBusy();
}
// read Fifo return a evt
// read evt do not fill the sender addr as Signal at evt , it is set as Sig_Rf_Recv_Msg
uint8_t ReadFifo()
{       
    uint8_t msg ;
    uint8_t i ,length ;  
    uint8_t *lp ;
    LPFrameCtrl lpFrame ;
    SetPacketParams(globaldata.cfgdata.PreambleNum,0,0xff,1,0);
    ClearIrqStatus( IRQ_RADIO_ALL );
    ClearLoraInterrupt();
    EnableLoraInterrupt();   
    
    CheckReady();
   
    EnablePeriph(Clock_SPI2); 
	SPI_Enable(SPI2);
    INT_DISABLE();
    SpiRTxBuf[0] = RADIO_GET_RXBUFFERSTATUS ;
    SpiRTxBuf[1] =0;
    SpiRTxBuf[2] =0;
    SpiRTxBuf[3] =0;
    LowCS();
    lp = SpiRTxBuf;
	for( i = 0; i < 4; i++ )
	{
		SPI2->TXBUF = *lp;
        while((SPI2->ISR & 0x3) != 0x03 )
            __nop();
		*lp = SPI2->RXBUF;
        lp++;
	}
    HighCS();
    WaitBusy();
    // In case of LORA fixed header, implict mode  the payloadLength is obtained by reading
    // the register REG_LR_PAYLOADLENGTH
    length = SpiRTxBuf[2];    
    if(length < MACHeaderSize)
    {   
        i= 0xff;
        goto  FailNetName ;
    }
    
    msg = newrfbuf(0);
    lp = (uint8_t *)getrfbuf(msg);
    lpFrame = (LPFrameCtrl)lp ;
    *lp =  length ;
    lp++ ;

    
    LowCS();
    SPI2->CR3 |= (SPI_CR3_TXBFC | SPI_CR3_RXBFC) ;
    
	SPI2->TXBUF = RADIO_READ_BUFFER;
    while((SPI2->ISR & 0x03)!= 0x03) 
            __nop();    
	SPI2->TXBUF = SpiRTxBuf[3]; //LORARXBASEADDR;
    
    while((SPI2->ISR & 0x3) != 0x03 ) 
            __nop();    
 	SPI2->TXBUF = 0;
    while((SPI2->ISR & 0x3) != 0x03 )
            __nop(); 

    SPI2->CR3 |= (SPI_CR3_TXBFC | SPI_CR3_RXBFC) ;

	for( i = 0; i < MACHeaderSize; i++ )
	{
		SPI2->TXBUF = 0x55;
        while((SPI2->ISR & 0x03) != 0x03)
            __nop();
		*lp = SPI2->RXBUF;        
        lp++;
	}    
    if(lpFrame->NetName != globaldata.cfgdata.NetName )
    {   
        i= 0xff;
        goto  FailNetName ;
    }
    
	for( i = MACHeaderSize; i < length; i++ )
	{
		SPI2->TXBUF = 0x55;
        while((SPI2->ISR & 0x03) != 0x03)
            __nop();
		*lp = SPI2->RXBUF;        
        lp++;
	}
FailNetName :    
    HighCS();
    SPI_Disable(SPI2);
    INT_ENABLE();
    DisablePeriph(Clock_SPI2); 
    WaitBusy();
    if(i == 0xff)    
    {
        freerfbuf(msg);
        msg = 0xff ;
    }
    return msg;
}




/*   
//time(ms) need to send one symb  SF12    SF11    SF10    SF9    SF8    SF7
static float SymbTime[3][6] = {{ 32.768, 16.384, 8.192, 4.096, 2.048, 1.024 },  // 125 KHz
                                { 16.384, 8.192,  4.096, 2.048, 1.024, 0.512 },  // 250 KHz
                                { 8.192,  4.096,  2.048, 1.024, 0.512, 0.256 }}; // 500 KHz
*/
// used 1/1000 base and 2^Power fuction 
static uint8_t SymbTimePower[3][6] = {{ 15,     14,     13,    12,    11,     10},
                                 { 14,     13,     12,    11 ,   10,     9} ,
                                 { 13,     12,     11,    10,     9,     8}};
//calc the airtime   based on 15.625us  . and about 2times bigger 
void  CalcAirtime(uint8_t packetlen)
{
    //float ts = SymbTime[globaldata.cfgdata.SignalBw - 4][12 - globaldata.cfgdata.SpreadingFactor];
    // time of preamble
    //double tPreamble = ( globaldata.cfgdata.PreambleNum + 4.25 ) * ts;
    // Symbol length of payload and time
    packetlen += 8  ;   // and the send and recv  userid and group name
    uint32_t tmp = 8 * packetlen - 4 * globaldata.cfgdata.SpreadingFactor +
                         28 + 16 * 1 - 0 ;// 16*1 is the crc mode . it is enabled always  .
                         // used explict header mode .
                         //( ( SX126x.PacketParams.Params.LoRa.HeaderType == LORA_PACKET_FIXED_LENGTH ) ? 20 : 0 ) ) /
                         // ( double )( 4 * ( globaldata.cfgdata.SpreadingFactor -
                         //( ( SX126x.ModulationParams.Params.LoRa.LowDatarateOptimize > 0 ) ? 2 : 0 ) ) ) ) *
    QuotRemain qr ;
    qr.quotient =  tmp;
    qr.remainder = (globaldata.cfgdata.SpreadingFactor<<2) ;
    HardDiv(&qr);   
    if(qr.remainder > 0)
        tmp = qr.quotient +1 ;  
                       //         ( ( SX126x.ModulationParams.Params.LoRa.CodingRate % 4 ) + 4 );
    tmp =              tmp * ( ( globaldata.cfgdata.ErrorCoding &0x03 ) + 4 );
    tmp = 8 + tmp + ( globaldata.cfgdata.PreambleNum + 5 ); // use 5 replace the 4.25

    // Time on air
    //ts = tmp * ts;
    //tmp = (uint32_t)(ts+1);
    tmp <<= SymbTimePower[globaldata.cfgdata.SignalBw - 4][12 - globaldata.cfgdata.SpreadingFactor];
    
    // used 2*CadDelay as  a addtional device delay time .  
    AirTime = CadDelay ; 
    AirTime <<=1 ;
    AirTime += tmp ; 
    // chang airtime based on us to  based on 15.625us  , AirTime = AirTime*64/1000  =   AirTime*64/1024 ==  AirTime>>4
    // and biger that for 2times   AirTime *2 = AirTime*64/1000 *2 ==   AirTime*64/1024 *2 ==  AirTime>>3
    AirTime >>= 3 ;
    //AirTime = 200*64 ;
}

void Initbtim(void )
{
    //the bstim source clock is APBCLK2 , it is 16M .
    EnablePeriph(Clock_BTIM);
     /* 分频系数 */
    BSTIM->PSC  = 15 ; //  div 16 ,so every ticks is 1us .
    
    /* 自动重装载值 */  
    // set ARR will reset the prediv , stop running when interrupt .  soft can't produce interrupt . enable update event
    BSTIM->CR1 = BSTIM_CR1_ARPE | BSTIM_CR1_OPM | BSTIM_CR1_URS ;
    //Clr interrrupt flage 
    BSTIM->ISR = 1 ;
    // disable interrupt .
    BSTIM->IER = 0 ;
    BSTIM->ARR = CadDelay ;  
    NVIC_DisableIRQ(BSTIM_IRQn);
    NVIC_SetPriority(BSTIM_IRQn,1);//中断优先级配置
    NVIC_EnableIRQ(BSTIM_IRQn);     //使能NVIC    
    
    DisablePeriph(Clock_BTIM);
}
// delay  based on   
void WaitDelay() 
{
    EnablePeriph(Clock_BTIM);
    // stop bstim
    BSTIM->CR1 = BSTIM_CR1_ARPE | BSTIM_CR1_OPM | BSTIM_CR1_URS ;    
    //Clr interrrupt flage 
    BSTIM->ISR = 1 ;
    //load data .
    BSTIM->EGR = 1 ;
    //enable interrupt
    BSTIM->IER = 1 ;
    // start delay
    BSTIM->CR1 = BSTIM_CR1_ARPE | BSTIM_CR1_OPM | BSTIM_CR1_URS |BSTIM_CR1_CEN; 
}
void StopDelay()
{
    BSTIM->ISR = 1 ;
    // disable interrupt .
    BSTIM->IER = 0 ;      
    BSTIM->CR1 = BSTIM_CR1_ARPE | BSTIM_CR1_OPM | BSTIM_CR1_URS   ;
    DisablePeriph(Clock_BTIM);
}

void     BSTIM_IRQHandler(void)
{
    //Clr interrrupt flage 
    BSTIM->ISR = 1 ;
    // disable interrupt .
    BSTIM->IER = 0 ;  
    BSTIM->CR1 = BSTIM_CR1_ARPE | BSTIM_CR1_OPM | BSTIM_CR1_URS ; 
    // bcs in oneshut mode , the bstim is stop.     
    SetBufferBaseAddress( LORATXBASEADDR, LORARXBASEADDR ); 
    SetCad();
    DisablePeriph(Clock_BTIM);
}

void  GetDevState(uint8_t *lp)
{
    RadioError_t error = GetDeviceErrors();
    *lp = (uint8_t)((error.Value >>8)&0xff);
    lp++ ;
    *lp = (uint8_t)((error.Value )&0xff);
    lp++ ;
    RadioStatus_t statu = GetStatus();
    *lp = statu.Value ;
    ClearDeviceErrors();
}


void  SetRfParam()
{// set all param before call tx or rx
    uint8_t temp;
    SetLoRaSymbNumTimeout( 5 );     // used the 5

    if( ( ( globaldata.cfgdata.SignalBw <= 7 ) && ( ( globaldata.cfgdata.SpreadingFactor == 11 ) || ( globaldata.cfgdata.SpreadingFactor == 12 ) ) ) ||
    ( ( globaldata.cfgdata.SignalBw <= 8 ) && ( globaldata.cfgdata.SpreadingFactor == 12 ) ) )
    {
        temp= 0x01;
    }
    else
    {
        temp = 0x00;
    }

    if(globaldata.cfgdata.PreambleNum <20)
        globaldata.cfgdata.PreambleNum = 20 ;
    SetModulationParams(  globaldata.cfgdata.SpreadingFactor ,globaldata.cfgdata.SignalBw ,globaldata.cfgdata.ErrorCoding ,temp );
    SetTxParams( globaldata.cfgdata.Power, RADIO_RAMP_200_US );
    SetRfFrequency(globaldata.cfgdata.RFFrequency);
    CalcAirtime(MAXTXCNT);
}

void LoraInit( void )
{
    CalibrationParams_t tem;
    InitSpi();
    Initbtim();
    Wakeup();
    SetRegulatorMode( 1 ); // use dc .
    SetStandby(0); // rc_standby
    // set DIO3 as  Tcxo power and  give 5ms to let it stable .
    // every time the device mode from rc to xoc will need 5ms .
    //SetDio3AsTcxoCtrl( TCXO_CTRL_1_8V, 10 << 6 ); 
#ifdef USE_TCXO
    SetDio3AsTcxoCtrl( TCXO_CTRL_1_8V, RADIO_TCXO_SETUP_TIME << 6 ); // convert from ms to SX126x time base
#endif    
    
    tem.Value = 0x7f  ;  
    Calibrate(tem); // calibrate all  :rc64k ,rc13m ,  pll ,adc ....}
    CalibrateImage(globaldata.cfgdata.RFFrequency); // calibrate the image .for 470M 
    SetDio2AsRfSwitchCtrl(1);
    RFState = MODE_STDBY_RC;    
    SetPacketType(); // lora only.    
    SetSyncWords();
    SetRfParam();
    SetBufferBaseAddress( LORATXBASEADDR, LORARXBASEADDR );
    SetDioIrqParams( IRQ_RADIO_NONE, IRQ_RADIO_NONE );
    SetStopRxTimerOnPreambleDetect(0);
    ClearDeviceErrors();
}

void  OnLoraIqr(void)
{
    RfMsg msg ;
    uint16_t irqRegs = GetIrqStatus( );
    ClearIrqStatus( IRQ_RADIO_ALL );


    if( ( irqRegs & IRQ_RX_DONE ) == IRQ_RX_DONE )
    { 
        // message recved .  check crcerr first then read it  .
        if((irqRegs & (IRQ_CRC_ERROR|IRQ_HEADER_ERROR)) == 0)
        { 
            msg.var.data8.data0 = ReadFifo();
            
            if(msg.var.data8.data0 != InvalidId)
            {
                MacRecvProc(msg.var.data8.data0);
                /*
                #if  Use_Big_Evt  == 0                  
                temp.data16.data1 = makeevt(Sig_Rf_Recv_Data,temp.data8.data0);
                postevtbyindex(BlcId_Net,temp.data16.data1);  
                #else
                temp.data32 = makeevt(Sig_Rf_Recv_Data,temp.data8.data0);
                postevtbyindex(BlcId_Net,temp.data32);    
                #endif                    
                */
            }
            goto TrySend ;
        }
    }
   
    if( ( irqRegs & IRQ_CAD_DONE ) == IRQ_CAD_DONE )
    { // no msg checked . check if there  is msg to send .
 TrySend :        
        msg = getonlinemsg();
        if(msg.value.memid != InvalidId)
        { // there is a msg need to send out .
            WriteFifo(msg);
            SetDioIrqParams( IRQ_TX_DONE|IRQ_RX_TX_TIMEOUT, IRQ_TX_DONE|IRQ_RX_TX_TIMEOUT );
            SetTx(AirTime);
            rflayerdata.rfmode = rf_tx_snd ; 
        }
        else
        { // no msg need to send .  goto RX_DC mode .
            SetDioIrqParams( IRQ_RX_DONE, IRQ_RX_DONE );
            SetRxDutyCycle(RecvTime ,SleepTime);
            rflayerdata.rfmode = rf_rx_dc ;  
        }
    }

    if( irqRegs & (IRQ_RX_TX_TIMEOUT |IRQ_TX_DONE) )
    {
        if(rflayerdata.rfmode == rf_tx_snd )
        { //send finised , goto Rx mode , with overtime .
            SetBufferBaseAddress( LORATXBASEADDR, LORARXBASEADDR ); 
            SetDioIrqParams( IRQ_RX_DONE|IRQ_RX_TX_TIMEOUT, IRQ_RX_DONE|IRQ_RX_TX_TIMEOUT );
            SetRx(AirTime);
            rflayerdata.rfmode = rf_tx_rcv ;          
        }
        else if(rflayerdata.rfmode == rf_tx_rcv )
             goto TrySend ;
    }

}

// start the RF in cad rx mode  , before this the RF should be poweron
void StartRfMonitor() 
{
    rflayerdata.rfmode = rf_cad_detect ;  
    ClearIrqStatus( IRQ_RADIO_ALL );
    ClearLoraInterrupt();
    EnableLoraInterrupt();
    SetPacketParams(globaldata.cfgdata.PreambleNum,0,MAXTXCNT,1,0);
    SetBufferBaseAddress( LORATXBASEADDR, LORARXBASEADDR );      
    // on cad or rx mode  , the interrupt will be create :      IRQ_CAD_DONE|IRQ_HEADER_ERROR|IRQ_RX_DONE|IRQ_RX_TX_TIMEOUT
    // the  IRQ_CRC_ERROR do not create interrrupt .it  will be check before read the data out from Queue .
    // the IRQ_HEADER_ERROR will stop the recv proc .
    // goto cad mode . and enable recv.
    SetDioIrqParams( IRQ_CAD_DONE|IRQ_RX_DONE|IRQ_RX_TX_TIMEOUT, IRQ_CAD_DONE|IRQ_RX_DONE|IRQ_RX_TX_TIMEOUT );
    SetCadParams(CAD_SYMBOL_NUM,CAD_DET_PEAK,CAD_DET_MIN,LORA_CAD_RX,DefOverTime);
    SetCad();  
}

// stop the RF monitor . RF goto sleep mode .
void StopRfMonitor() 
{
    DisableLoraInterrupt();
    ClearLoraInterrupt();
    ClearIrqStatus( IRQ_RADIO_ALL );
    SetStandby(0);  
    rflayerdata.rfmode = rf_init ;    
}

void SendMsg( RfMsg msg)
{
    assert(msg.value.memid != InvalidId);
    // there is must a msg need to send out .
    WriteFifo(msg);
    SetDioIrqParams( IRQ_TX_DONE|IRQ_RX_TX_TIMEOUT, IRQ_TX_DONE|IRQ_RX_TX_TIMEOUT );
    SetTx(AirTime);
    rflayerdata.rfmode = rf_tx_snd ; 

}
void DoSend()
{
    RfMsg  msg ;
    // check wait pin  : high  in sleep mode  . low in rx mode .
    if(IsBusy())
    {
        Wakeup();
    }
    // check if there is data to send . else goto rx dc mode .
    msg = getonlinemsg();

    assert(msg.value.memid != InvalidId);
    // there is must a msg need to send out .
    WriteFifo(msg);
    SetDioIrqParams( IRQ_TX_DONE|IRQ_RX_TX_TIMEOUT, IRQ_TX_DONE|IRQ_RX_TX_TIMEOUT );
    SetTx(AirTime);
    rflayerdata.rfmode = rf_tx_snd ; 
}

