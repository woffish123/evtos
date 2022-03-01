

#include "SX1276.h"
#include "Hal.h"



static tRFLRStates RFLRState = RFLR_STATE_IDLE;
// Local param
uint8_t        SpiRxBuf[SPICmdBufLen];
uint8_t        SpiTxBuf[SPICmdBufLen];
uint8_t        Rssi ;

static uint8_t        RtyTimes ;
static uint16_t       SendRtyTimes ;


// SpiFifoRxBuf and SpiFifoTxBuf is used only when  call recv  or send function , else it will no changed 
// so  we  can set both read ,and send together , for example : set  RxBuffer  not changed , set Txbuffer as respones data 
// call recv mode fisrt , and then call send immediaterly.

// Notice!!   the buffer struct is :  length , UserID , data  .
// SpiFifoRxBuf[0] and SpiFifoTxBuf[0] is fixed to store the data length +1+ UserIDCnt ,
// you can set the SpiFifoTxBuf[0] as data lenght +1+UserIDCnt ,then call send
// you can get the length of received data  as SpiFifoRxBuf[0] =lenght +1+UserIDCnt ,after a successful receive. 
// to simple the recv and send data is alwith started at  PACKET_START_ADDR  it equal to  PACKET_USERID_CNT+1
// ie : for( int i = PACKET_START_ADDR ; i < SpiFifoTxBuf[0] ; i++ ) ....
uint8_t        SpiFifoRxBuf[PACKETLEN];
uint8_t        SpiFifoTxBuf[PACKETLEN];


const float SignalBw[] =
{
    7.8,10.4 ,15.6 , 20.8 , 31.2 , 41.6 , 62.5, 125,250,500

};


void RfAck()
{
    SetTxCmd(0);
    SetTxLen(2);
    lpTxBuf[0] = 'O' ;
    lpTxBuf[1] = 'K' ;

}

/****************************************************************************
* ??    3?￡ovoid SX1276InitIo( void )
* 1|    ?ü￡o3?ê??ˉó?é??μD????àá′?óμ?1ü?? ,ò??°ía2?μ?EventIO??￡?μ??D??μè????
* è??ú2?êy￡o?T
* 3??ú2?êy￡o?T
* ?μ    ?÷￡o????DIO0-5 ×÷?a?D??ê?è??? ￡?EVENT0-6×÷?aê?3???￡?
* μ÷ó?·?·¨￡o?T 
****************************************************************************/ 
void SX1276Init( void )
{

        
        SetChipSPI();
        SetChipGPIO();
        SetChipGPIO();
        // init GPIO .
        //  PC15  SPI-CLK , PC14  SPI_CS
        //  PD7  SPI_MOSI   PD6   SPI_MISO 
        //  DIO0 Pe10 ,DIO1 PE11 , DIO2 PE12 ,DIO3 Pe13 ,DIO4 PA1
        // Reset PF2 , TxCtrl PC13 ,rxctrl Pa2
        

        // PF  reset PF2 
        LoRaResetLow();
        GPIO->P[5].CTRL = _GPIO_P_CTRL_DRIVEMODE_STANDARD ; // set Port C as 6ma 
        GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
        
        //PC  ,pc13 out, pc14 out ,PC15 out ,
        // for simple use  the DIO0 and DIO1  and DIO3 used as interrupt ,,and  dio2,4,5 reserved ,not used 
        GPIO->P[2].DOUTCLR = 1<<15 ;  //SPI_CLK  default as low .
        HighCS(); 
        GPIO->P[2].CTRL = _GPIO_P_CTRL_DRIVEMODE_STANDARD ; // set Port C as 6ma 
        GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE13_PUSHPULL |GPIO_P_MODEH_MODE14_PUSHPULL |GPIO_P_MODEH_MODE15_PUSHPULL;
        
        
        // PA  2 out  1 : input
        GPIO->P[0].CTRL = _GPIO_P_CTRL_DRIVEMODE_STANDARD ; // set Port A as 6ma 
        GPIO->P[0].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL |GPIO_P_MODEL_MODE1_INPUT ;
       
        //PD  MISO and MOSI 
        GPIO->P[3].CTRL = _GPIO_P_CTRL_DRIVEMODE_STANDARD ; // set Port A as 6ma 
        GPIO->P[3].MODEL |= GPIO_P_MODEL_MODE7_PUSHPULL | GPIO_P_MODEL_MODE6_INPUT;
        GPIO->P[3].DOUTSET = 1<<7 ; // default as High 
        
        //PE  10 ,11,12,13
        GPIO->P[4].MODEH |= GPIO_P_MODEH_MODE10_INPUT|GPIO_P_MODEH_MODE11_INPUT|GPIO_P_MODEH_MODE12_INPUT|GPIO_P_MODEH_MODE13_INPUT;
        
        
        GPIO_IntConfig(gpioPortE, 13, true, false, false);
        
        GPIO_IntConfig(gpioPortE, 10, true, false, false);
        //DIO1
        GPIO_IntConfig(gpioPortE, 11, true, false, false);
        
        
        AntSleepMode() ;

         
        
        // spi initial and enable  
        USART1->ROUTE = USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_LOCATION_LOC3; ;  //enable  all pin ,but cs .and  used location 3
        // clk  the Uart1 clk is fHFPERCLK /(2 x (1 + USARTn_CLKDIV/256))  -> USARTn_CLKDIV = 256 x (f HFPERCLK /(2 x brdesired) - 1)  , and the the max clk for SX1276 is 10M, the Mian clk for mierochip is 16M or 21M
        // we should se the clk higher to reduce the time used for communicate , so set uartn_clkdiv as 256, uartclk is   fHFPERCLK /(2 x2
        USART1->CLKDIV = 256 ;
        USART1->FRAME  = 5 ;
        USART1->CTRL =  1<<0 | 1<<10 ;
        USART1->CMD  =  1<<1 | 1<<3  ;  // Disable tx and rx .
        
        while(USART1->STATUS & (1<<1)) ; // wait tx disabled
        USART1->CMD  =  1<<4 | 1<<10 |1<<11 ; //set spi in mazter mode 
        USART1->CMD  =  1<<0 | 1<<2 ;  // enable tx and rx
       


 
        SendRtyTimes =0;
        RtyTimes =0;
        status_Reset(STATUS_LORA_PINPANG_MODE);
        // init default seting
     
        globalData.cfginfor.Crc = DefaultCfgInfor.Crc ;
        globalData.cfginfor.Mode = DefaultCfgInfor.Mode ;
        globalData.cfginfor.ErrorCoding = DefaultCfgInfor.ErrorCoding ;
        globalData.cfginfor.LocalID = DefaultCfgInfor.LocalID ;
        globalData.cfginfor.Power = DefaultCfgInfor.Power ;
        globalData.cfginfor.PreambleNum = DefaultCfgInfor.PreambleNum ;
        globalData.cfginfor.RemoteID = DefaultCfgInfor.RemoteID ;
        globalData.cfginfor.RFFrequency = DefaultCfgInfor.RFFrequency ;
        globalData.cfginfor.SignalBw = DefaultCfgInfor.SignalBw ;
        globalData.cfginfor.SpreadingFactor = DefaultCfgInfor.SpreadingFactor ;
        globalData.cfginfor.UserID[0] = DefaultCfgInfor.UserID[0] ;
        globalData.cfginfor.UserID[1] = DefaultCfgInfor.UserID[1] ;
        globalData.cfginfor.UserID[2] = DefaultCfgInfor.UserID[2] ;
        globalData.cfginfor.UserID[3] = DefaultCfgInfor.UserID[3] ;
        
        
        NVIC_SetPriority(GPIO_ODD_IRQn, 2);
        NVIC_SetPriority(GPIO_EVEN_IRQn,3);
        NVIC_EnableIRQ(GPIO_ODD_IRQn);
        NVIC_EnableIRQ(GPIO_EVEN_IRQn);          
        
        
        ResetChipSPI();	
        LoRaResetHigh();
	// lora reset over 



}

void SndRcvBuf(uint8_t len)
{
	uint8_t i ;
	
	SpiEnable();
	for( i = 0; i < len; i++ )
	{
		USART_Tx(USART1, SpiTxBuf[i]);
		SpiRxBuf[i] = USART_Rx(USART1);
	}
	SpiDisable();

}
void WriteFifo()
{     
  // send length stored at  SpiFifoTxBuf[0]   
    uint8_t i ,len;  
    
    for(i =0 ; i  <PACKET_USERID_CNT ; i ++)
      SpiFifoTxBuf[i+1] = globalData.cfginfor.UserID[i];
    // i ==5 
    SpiFifoTxBuf[5] = (unsigned char) ((globalData.cfginfor.LocalID>>8) &0xff);

    SpiFifoTxBuf[6] = (unsigned char) (globalData.cfginfor.LocalID &0xff);
    // next is the sender 's  Remoter ID , it should be same with receiver's local id  or as a braodcast addr. 
    SpiFifoTxBuf[7] = (unsigned char) ((globalData.cfginfor.RemoteID>>8) &0xff);
    SpiFifoTxBuf[8] = (unsigned char) (globalData.cfginfor.RemoteID &0xff);


    // cmd and buf will be write be caller . len will be set by SetTxLen    
    len = SpiFifoTxBuf[0]+1 ;

	SpiEnable();
	SpiFifoTxBuf[0] =   0x80;   
	for( i = 0; i < len; i++ )
	{
		USART_Tx(USART1, SpiFifoTxBuf[i]);
	}
    SpiFifoTxBuf[0] = len ;
    SpiDisable();



}

bool ReadFifo(bool bBufOnly)
{    // read length stored at  SpiFifoRxBuf[0]        
    uint8_t i ,len;  
    len = SpiFifoRxBuf[0] ;

	SpiEnable();

	for( i = 0; i < len; i++ )
	{
		USART_Tx(USART1, 0 );
		SpiFifoRxBuf[i] = USART_Rx(USART1);
	}
    SpiFifoRxBuf[0] = len ; 
	SpiDisable();
    if( bBufOnly ) 
      return true ;
    
    
    for( i =0 ; i  < PACKET_USERID_CNT ; i ++)
         if(SpiFifoRxBuf[i+1] != globalData.cfginfor.UserID[i] )
                    break;
    if(i != PACKET_USERID_CNT)
         return false ;

    // store cur remote id
    
    globalData.cfginfor.RemoteID = SpiFifoRxBuf[5] ;
    globalData.cfginfor.RemoteID <<= 8;
    globalData.cfginfor.RemoteID |= SpiFifoRxBuf[6] ;
    

    if(globalData.cfginfor.Mode & SysMode_IDCheck)
    {
        if(SpiFifoRxBuf[7]  == BDCT_ADDRH  && SpiFifoRxBuf[8] ==  BDCT_ADDRL)
           return true ;      
        
        // sender's remote id should be same with receiver's localID .
        len= (unsigned char)((globalData.cfginfor.LocalID>>8) & 0xff);
        if (SpiFifoRxBuf[7] != len)
           return false ;

        len= (unsigned char)(globalData.cfginfor.LocalID & 0xff);
        if (SpiFifoRxBuf[8] != len)
           return false ;
    }
    
    return true ;
}


/****************************************************************************
* ??    3?￡ovoid SX1276LoRaSetOpMode( uint8_t opMode )
* 1|    ?ü￡oDT??é??μD???1¤×÷×′ì??￡ê?
* è??ú2?êy￡o?T
* 3??ú2?êy￡o?T
* ?μ    ?÷￡o
* μ÷ó?·?·¨￡o?T 
****************************************************************************/ 
void SX1276LoRaSetOpMode( uint8_t opMode )
{
 

    uint8_t temp;
	temp = opMode & RFLR_OPMODE_MASK ;
	if(temp == RFLR_OPMODE_SLEEP)
	{
        AntSleepMode();
	 	// ento sleep mode   first .
   		SpiTxBuf[0] = REG_LR_OPMODE |0x80 ;
		SpiTxBuf[1] = RFLR_OPMODE_SLEEP ;
		SndRcvBuf(2);	
        IntDisable();
        return ;
	}

 
	
	if(temp == RFLR_OPMODE_TRANSMITTER  ||temp ==  RFLR_OPMODE_SYNTHESIZER_TX )
	{
		AntTxMode() ;
	}
	else 
	{
		AntRxMode( );
	}

	SpiTxBuf[0] = REG_LR_OPMODE |0x80 ;
    SpiTxBuf[1]= temp |0x80 ;
	SndRcvBuf(2);
 
}


//设置工作频率	checked.. 频率设置原则为 freq (0- 2^24) *2^6 Hz
void SX1276LoRaSetRFFrequency( uint32_t freq )
{
    globalData.cfginfor.RFFrequency = freq;

   	// freq = ( uint32_t )( ( double )freq / ( double )FREQ_STEP );
 	SpiTxBuf[0] = REG_LR_FRFMSB|0x80;
	SpiTxBuf[1] = ( uint8_t )( ( freq >> 16 ) & 0xFF );
	SpiTxBuf[2] = ( uint8_t )( ( freq >> 8 ) & 0xFF );
	SpiTxBuf[3] = ( uint8_t )(  freq  & 0xFF );
	SndRcvBuf(4);
}
// checked ..
void SX1276LoRaSetNbTrigPeaks( uint8_t value )
{
    
 	SpiTxBuf[0] = 0x31;
	SndRcvBuf(2);
    SpiTxBuf[1]= ( SpiRxBuf[1] & 0xF8 ) | value;
    SpiTxBuf[0] = 0x31 |0x80 ;
	SndRcvBuf(2);
}

//设置扩频因子SF  checked..
void SX1276LoRaSetSpreadingFactor( uint8_t factor )
{

    if( factor > 12 )
    {
        factor = 12;
    }
    else if( factor < 6 )
    {
        factor = 6;
    }

    if( factor == 6 )
    {
        SX1276LoRaSetNbTrigPeaks( 5 );
    }
    else
    {
        SX1276LoRaSetNbTrigPeaks( 3 );
    }


	SpiTxBuf[0] = REG_LR_MODEMCONFIG2;	 
	SndRcvBuf(2);

       SpiTxBuf[1] = ( SpiRxBuf[1] & RFLR_MODEMCONFIG2_SF_MASK ) | ( factor << 4 );

        SpiTxBuf[0] = REG_LR_MODEMCONFIG2 |0x80;
	SndRcvBuf(2);

    globalData.cfginfor.SpreadingFactor = factor;
}

//设置纠错编码率	checked..
void SX1276LoRaSetErrorCoding( uint8_t value )
{
   	 value = value & 0x07;
    
	SpiTxBuf[0] = REG_LR_MODEMCONFIG1;	 
	SndRcvBuf(2);

    	SpiTxBuf[1] = ( SpiRxBuf[1] & RFLR_MODEMCONFIG1_CODINGRATE_MASK ) | ( value << 1 );
	SpiTxBuf[0] = REG_LR_MODEMCONFIG1 | 0x80;
	SndRcvBuf(2);

    globalData.cfginfor.ErrorCoding = value;
}

//设置RxPayLoad Header CRC是否打开
void SX1276LoRaSetPacketCrcOn( uint8_t enable )
{
    if(enable != 0)
    {
        enable = 1;
    }
    SpiTxBuf[0] = REG_LR_MODEMCONFIG2;	 
    SndRcvBuf(2);

    SpiTxBuf[1] = ( SpiRxBuf[1] & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) | ( enable << 2 );

    SpiTxBuf[0] = REG_LR_MODEMCONFIG2 |0x80;
    SndRcvBuf(2);
   // globalData.cfginfor.CrcOn = enable;
}

//设置信号带宽	checked..
void SX1276LoRaSetSignalBandwidth( uint8_t bw )
{
    bw = bw & 0x0F;

	SpiTxBuf[0] = REG_LR_MODEMCONFIG1;	 
	SndRcvBuf(2);
    SpiTxBuf[1] = ( SpiRxBuf[1] & RFLR_MODEMCONFIG1_BW_MASK ) | ( bw << 4 );
    SpiTxBuf[0] = REG_LR_MODEMCONFIG1 |0x80;
	SndRcvBuf(2);
    globalData.cfginfor.SignalBw = bw;
}

//设置是否隐含帧头： 1->隐藏 0->包含	checked..
void SX1276LoRaSetImplicitHeaderOn( uint8_t enable )
{
        if(enable != 0)
            enable = 1;

	SpiTxBuf[0] = REG_LR_MODEMCONFIG1;	 
	SndRcvBuf(2);
    	SpiTxBuf[1] = ( SpiRxBuf[1] & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) | enable;
    	SpiTxBuf[0] = REG_LR_MODEMCONFIG1 |0x80;
	SndRcvBuf(2);
    	//globalData.cfginfor.ImplicitHeaderOn = enable;
}
void SX1276LoRaSetHopPeriod(uint8_t value)
{
	SpiTxBuf[0] = REG_LR_HOPPERIOD|0x80;	 
	SpiTxBuf[1] = value ; 
	SndRcvBuf(2);
	//globalData.cfginfor.HopPeriod  = value  ;
}

//设置一次接收超时的时间	 .. checked .
void SX1276LoRaSetSymbTimeout( uint16_t value )
{
 	SpiTxBuf[0] = REG_LR_MODEMCONFIG2;	 
	SndRcvBuf(3);
    	SpiTxBuf[1] = ( SpiRxBuf[1] & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) | ( ( value >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK );
    	SpiTxBuf[2] =  value & 0xFF ;
    	SpiTxBuf[0] = REG_LR_MODEMCONFIG2 |0x80;
	SndRcvBuf(3);
}

//设置固定帧长时，PayLoad的长度		 checked..
void SX1276LoRaSetPayloadLength( uint8_t value )
{

	SpiTxBuf[0] = REG_LR_PAYLOADLENGTH |0x80 ;
	SpiTxBuf[1] = value ;
	SndRcvBuf(2);
    	//globalData.cfginfor.PayloadLength = value;
}

//设置是否进行低速率优化   checked .
void SX1276LoRaSetLowDatarateOptimize( uint8_t enable )
{
	    if(enable != 0)
	    {
	        enable = 1;
	    }
	SpiTxBuf[0] = REG_LR_MODEMCONFIG3  ;
	SndRcvBuf(2);

    SpiTxBuf[1] = ( SpiRxBuf[1] & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK ) | ( enable << 3 );
	SpiTxBuf[0] = REG_LR_MODEMCONFIG3|0x80   ;
    SndRcvBuf(2);
}

//设置发送PA设置，选择PA输出管脚RFO pin（最大发送功率+14dBm），PA_BOOST pin（最大发送功率+20dBm）
// checked .
void SX1276LoRaSetPAOutput( uint8_t outputPin )
{
	SpiTxBuf[0] = REG_LR_PACONFIG  ;
	SndRcvBuf(2);
    
    	SpiTxBuf[1] = (SpiRxBuf[1] & RFLR_PACONFIG_PASELECT_MASK ) | outputPin;
	SpiTxBuf[0] = REG_LR_PACONFIG|0x80  ;
	SndRcvBuf(2);
    
}
//????
void SX1276LoRaSetPaRamp(uint8_t value)
{
	
 	SpiTxBuf[0] = REG_LR_PARAMP  ;
	SndRcvBuf(2);
    
    	SpiTxBuf[1] = (SpiRxBuf[1] & RFLR_PARAMP_MASK ) |(value & ~RFLR_PARAMP_MASK);
	SpiTxBuf[0] = REG_LR_PARAMP|0x80  ;
	SndRcvBuf(2);

}

//使能 PA_BOOST管脚的+20dBm输出。TRUE 输出+20dB, FASLE输出 默认
// checked ..
void SX1276LoRaSetPa20dBm( uint8_t enale )
{
/*	uint8_t temp ;

	SpiTxBuf[0] = REG_LR_PADAC  ;
	SndRcvBuf(2);
	temp = SpiRxBuf[1];
  */
	SpiTxBuf[0] = REG_LR_PACONFIG ;
	SndRcvBuf(2);

    if( ( SpiRxBuf[1] & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
    {    
	    if( enale != 0 )
            SpiTxBuf[1] = 0x87;
	}
   	 else
   	{
   	     SpiTxBuf[1] = 0x84;
    }
   	SpiTxBuf[0] = REG_LR_PADAC |0x80 ;
	SndRcvBuf(2);
}

//设置发送功率	   checked..
void SX1276LoRaSetRFPower( int8_t power )
{
	uint8_t temp ;

	SpiTxBuf[0] = REG_LR_PADAC  ;
	SndRcvBuf(2);
	temp = SpiRxBuf[1];

	SpiTxBuf[0] = REG_LR_PACONFIG ;
	SndRcvBuf(2);
	  
   
	    if( ( SpiRxBuf[1] & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
	    {
	        if( ( temp & 0x87 ) == 0x87 )
	        {
	            if( power < 5 )
	            {
	                power = 5;
	            }
	            if( power > 20 )
	            {
	                power = 20;
	            }
	            temp = ( SpiRxBuf[1] & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
	            SpiTxBuf[1] = ( temp & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
	        }
	        else
	        {
	            if( power < 2 )
	            {
	                power = 2;
	            }
	            if( power > 17 )
	            {
	                power = 17;
	            }
	            temp = ( SpiRxBuf[1] & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
	            SpiTxBuf[1] = ( temp & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
	        }
	    }
	    else
	    {
	        if( power < -1 )
	        {
	            power = -1;
	        }
	        if( power > 14 )
	        {
	            power = 14;
	        }
	        temp = ( SpiRxBuf[1] & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
	        SpiTxBuf[1] = ( temp & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power + 1 ) & 0x0F );
	    }


	SpiTxBuf[0] = REG_LR_PACONFIG|0x80 ;
	SndRcvBuf(2);
//   	globalData.cfginfor.Power = power;
}

// 设置Preamble长度	   checked..
void SX1276LoRaSetPreambleLength( uint16_t value )
{
 

	SpiTxBuf[0] = REG_LR_PREAMBLEMSB|0x80 ;
	SpiTxBuf[1] = ( value >> 8 ) & 0x00FF;
	SpiTxBuf[2] =  value & 0xFF;
	SndRcvBuf(3);
}

uint8_t SX1276LoraReadRssi(void)
{

	SpiTxBuf[0] = REG_LR_RSSIVALUE ;
	SndRcvBuf(2);
        return  SpiRxBuf[1];
 

}
static uint8_t  NewPacketLocation =0 ;


// try to check the User name ,  maybe the ceceive the too quickly  that will receive all data finished . before call on loraDelay the second times
void OnLoraDelay(void)
{
    uint8_t temp ;

    //  there are 3 kind of status ,to call this function 
    if((RFLRState == RFLR_STATE_RX_SINGLE_VALIDHEADER))
    {//ID_Assert  delay over timr : recv mode , ,check the current RegFifoRxByteAddr is big than last time .
        
  
        // store newpacket start address
        SpiTxBuf[0] = REG_LR_FIFORXBYTEADDR  ;
        SndRcvBuf(2);
        temp = NewPacketLocation+MessageIdLen ;
            
        if(SpiRxBuf[1] < temp )
        {// failed  try again ., here will continue try to get UserID  count byte , untill get  enough byte or  the recv overed ,
            SetLoraDelay(ID_Assert) ;                
            return ;
        }

        // recved the username , check it .
        SpiTxBuf[0] = REG_LR_FIFOADDRPTR |0x80 ;
        SpiTxBuf[1] = NewPacketLocation;
        SndRcvBuf(2);	
                                 
        //read data . only  userid and Local ID , Remote ID . for first byte return is no used ,the read buf len should add 1.
        SpiFifoRxBuf[0] =MessageIdLen;
        if(ReadFifo(false))
        { 
            status_Set(STATUS_LORA_VALID_USERID);            
        }
        else
        {    // invalide  packet received .
            
            status_Reset(STATUS_LORA_VALID_USERID |STATUS_RF_WORKING);
            SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
            RFLRState = RFLR_STATE_IDLE;
            event_Set(HAL_LORA_INVALIIDE_USERID);
            
        }
        return ;
    }
        // case :CAD time Over 
    else if( (RFLRState == RFLR_STATE_CAD_TX_RUNNING || RFLRState == RFLR_STATE_CAD_RX_RUNNING ))
    { // CAD_Send_Delay , or  CAD_recv_Delay over time  , do cad again .
        SX1276LoRaSetOpMode( RFLR_OPMODE_CAD );
    }
 
}


void ChangeRfConfig(void)
{
   float v ; //TSyn
   float t ;
   uint16_t d;
   DBG_Fill(32);
    // calc Tsyn in ms   
    v =  1<<globalData.cfginfor.SpreadingFactor ;
    v /= SignalBw[globalData.cfginfor.SignalBw] ;
    
    SX1276LoRaSetRFFrequency( globalData.cfginfor.RFFrequency ); //435MHz
    // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    SX1276LoRaSetSpreadingFactor( globalData.cfginfor.SpreadingFactor ); //SF6
    SX1276LoRaSetErrorCoding( globalData.cfginfor.ErrorCoding );
    SX1276LoRaSetSignalBandwidth( globalData.cfginfor.SignalBw );
    //SX1276LoRaSetImplicitHeaderOn( globalData.cfginfor.ImplicitHeaderOn );
    
    
    if(v >16 )
    {
        SX1276LoRaSetLowDatarateOptimize( 1 );
        d =1 ;
    }
    else
    {
        SX1276LoRaSetLowDatarateOptimize( 0 );
        d =0 ;
    }
    //set the length of the preamble
    
    SX1276LoRaSetPreambleLength( globalData.cfginfor.PreambleNum ); 
    

    //  for Tim0 each time0 clk is time0step/MainClk   ms
    // so Ts is V/(tem0step/mainclk)  step   = V*MainClk/tem0step   time0 steps  
    // v * timer0 clock (Frc / div)*1.1 = timer0  count .
    v= v* MainClk   /(1<<TIMER0_PREDIV) ;
    
    //calc on air time for preamble 
    t =  globalData.cfginfor.PreambleNum +4.25 ;
    t *= v ;
    t+=0.5 ;
    t/=2   ;  // use half preamble tim as cad check time
    // the Snd Delay can also be used as a almost packet send time , to calc the RTC time adjust .
    SetRecvCADDelay((uint16_t)(t)) ;
    
     //calc on air for a total cmd ,  44 is the inter header length, and we assume the content of the cmd is 15 byte  normally.

    t =  globalData.cfginfor.PreambleNum +4.25 + 44 + 15*8;
    t *= v ;
    t+=0.5 ;
    t/=2   ;  // use half preamble tim as cad check time
    // the Snd Delay can also be used as a almost packet send time , to calc the RTC time adjust .
    SetSendCADDelay((uint16_t)(t)) ;
   
    
    
    SX1276LoRaSetSymbTimeout(globalData.cfginfor.PreambleNum+5) ;

    
    
    t =   8* PACKET_USERID_CNT+44 ;
    t /= (4*(globalData.cfginfor.SpreadingFactor - 2* d)) ;
    t += 0.99;
    d = (uint16_t) t ;
    d *=  (4+globalData.cfginfor.ErrorCoding);
    d += globalData.cfginfor.PreambleNum;
    
    // get differ for PACKET_USERID_CNT byte on air  time  for these is a ceil function , to reduce the influence of it add a 0.5 *(4+globalData.cfginfor.ErrorCoding)
        
    SetASSERTDelay( (uint16_t)(d*v) );
    
    
    SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_PABOOST );
    if(globalData.cfginfor.Power >17)
        SX1276LoRaSetPa20dBm( 1 );
    else
   		SX1276LoRaSetPa20dBm( 0 );
    SX1276LoRaSetRFPower( globalData.cfginfor.Power );    
}

void GPIO_EVEN_IRQHandler(void)
{

    uint8_t temp ;
    uint16_t  IFflage   = GPIO->IF&0x5555 ;
    GPIO->IFC = IFflage ;
    //DBG_Fill(31);    
    IFflage &= GPIO->IEN ; 
    if(IFflage & (1<<10))
    {// DIO 0 inerrupt 
        
    // case 1 : after a valid header is rcved . 
      
      if( (RFLRState == RFLR_STATE_RX_SINGLE_VALIDHEADER)  )
      { // RxDone happed. there is a valid packet Recved before . stop recv  store the recv data and set flage .
        // do some prepare work .
        SpiTxBuf[0] =  REG_LR_PKTRSSIVALUE ;
        SndRcvBuf(2);
        Rssi = SpiRxBuf[1];                            
       
        SpiTxBuf[0] = REG_LR_IRQFLAGS  ;
        SndRcvBuf(2);	
        if((SpiRxBuf[1] & RFLR_IRQFLAGS_PAYLOADCRCERROR )!= 0)  
        {
           event_Set(HAL_LORA_INVALIIDE_CRC);
        }
        else
        {
 
          //store the next start addr 
          temp = NewPacketLocation;
          SpiTxBuf[0] = REG_LR_FIFOADDRPTR ;
          SndRcvBuf(2);	
          NewPacketLocation = SpiRxBuf[1];
          
          // write REG_LR_FIFOADDRPTR as REG_LR_FIFORXCURRENTADDR;
          SpiTxBuf[0] = REG_LR_FIFOADDRPTR |0x80 ;
          SpiTxBuf[1] = temp;
          SndRcvBuf(2);	

          // get packet len and recv it to fifo buf .
          SpiTxBuf[0] = REG_LR_NBRXBYTES ;
          SndRcvBuf(2);
           //read data .
          SpiFifoRxBuf[0] = SpiRxBuf[1]++ ;
          if(ReadFifo(CheckStatus(STATUS_LORA_VALID_USERID)))
          {
                event_Set(HAL_LORA_RX_DONE);
                if(CheckStatus(STATUS_LORA_PINPANG_MODE))
                {
                   status_Reset(STATUS_LORA_PINPANG_MODE);
                   EnterTXMode(SendMaxCadCnt);
                
                }
                else
                {
                    SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
                    RFLRState = RFLR_STATE_IDLE;
                    status_Reset(STATUS_RF_WORKING); 
                    return ;
                }              
          }
          else
          {
              event_Set(HAL_LORA_INVALIIDE_USERID);
              SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
              RFLRState = RFLR_STATE_IDLE;              
              status_Reset(STATUS_LORA_PINPANG_MODE);
              status_Reset(STATUS_RF_WORKING); 
              return ;
          }

        }
          
        SpiTxBuf[1]=  RFLR_IRQFLAGS_PAYLOADCRCERROR | RFLR_IRQFLAGS_RXDONE ;
        SpiTxBuf[0] = REG_LR_IRQFLAGS |0x80 ;
        SndRcvBuf(2);

       }
       // case 2 : send finished .
       else if( RFLRState == RFLR_STATE_TX_RUNNING)
        {        
           if(CheckStatus(STATUS_LORA_PINPANG_MODE))
            {// the peer lora should receiv the send cmd and turn to send mode also , so the time is not need to care.
                status_Reset(STATUS_LORA_PINPANG_MODE);
               // set in single dac recv mode .
               // EnterRcvMode(SendRtyTimes);
                RtyTimes = (SendRtyTimes&RcvTimeOutMask)>>10 ;
                SendRtyTimes = SendRtyTimes&0x3ff ;
                if(SendRtyTimes == 0)
                    SendRtyTimes = globalData.cfginfor.PreambleNum ;
               
                SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGSMASK;
                // to simple interrupt proc coding 
                SpiTxBuf[1] =	//RFLR_IRQFLAGS_RXTIMEOUT |
                                    //RFLR_IRQFLAGS_RXDONE |
                                    //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                    //RFLR_IRQFLAGS_VALIDHEADER |
                                    RFLR_IRQFLAGS_TXDONE |
                                    //RFLR_IRQFLAGS_CADDONE |
                                    RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL ;
                                    //RFLR_IRQFLAGS_CADDETECTED

                    
                SndRcvBuf(2);   
                // clr all interrupt .
                SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGS;
                SpiTxBuf[1] = 0xff ;
                SndRcvBuf(2); 
                SpiTxBuf[0] = 0x80 | REG_LR_DIOMAPPING1;
                if(RtyTimes == 0)
                {
                    SpiTxBuf[1] = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 |RFLR_DIOMAPPING1_DIO2_00 |RFLR_DIOMAPPING1_DIO3_01;//0x0d ;
                 }
                 else
                { //cad mode
                    SpiTxBuf[1] = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 |RFLR_DIOMAPPING1_DIO2_00 |RFLR_DIOMAPPING1_DIO3_00;//0x0d ;
                }    
                SpiTxBuf[2] = RFLR_DIOMAPPING2_DIO4_00 | RFLR_DIOMAPPING2_DIO5_00 ;//0xf0 ; 
                SndRcvBuf(3);
                //设置RxFiFO的指针起始位置
                SpiTxBuf[0] = REG_LR_FIFOADDRPTR|0x80 ;
                SpiTxBuf[1] = FIFORXBASEADDR;
                SndRcvBuf(2);
                SX1276LoRaSetSymbTimeout(SendRtyTimes);
                NewPacketLocation  = FIFORXBASEADDR ;
                status_Reset(STATUS_LORA_VALID_USERID);
                if(RtyTimes == 0)
                {
                     RFLRState = RFLR_STATE_RX_SINGLE_RUNNING;        
                     SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_SINGLE );
                      //SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_CONTINUE);
                }
                else
                {// cad mode .
                     RFLRState = RFLR_STATE_CAD_RX_RUNNING; 
                     SX1276LoRaSetOpMode( RFLR_OPMODE_CAD );
                 }               
            
            }
            else
            {
               SpiTxBuf[1] = RFLR_IRQFLAGS_TXDONE ;
               SpiTxBuf[0] = REG_LR_IRQFLAGS |0x80 ;
               SndRcvBuf(2);
               SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
               RFLRState = RFLR_STATE_IDLE;
               status_Reset(STATUS_RF_WORKING);
               event_Set(HAL_LORA_TX_DONE);
               return ;
            }          

        }
 
    }



}

 
  void GPIO_ODD_IRQHandler(void)
 {
        uint8_t  temp ;
	    uint16_t  IFflage   = GPIO->IF&0xAAAA ;
        GPIO->IFC = IFflage ;
        IFflage &= GPIO->IEN ;   
        
        temp =0;

        if(IFflage & (1<< 11))
        {
            // SX1276 DIO 1  interrupt .
            
            // Rx Timeout ..
             if( RFLRState == RFLR_STATE_RX_SINGLE_RUNNING)
             { // time out hanpped. there is no valid packet heared .
                   //check if the CAD mode is finised. if not ,return to cad mode .
                   if(RtyTimes != 0)
                   {
                        SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGSMASK;
  
                        SpiTxBuf[1] =  RFLR_IRQFLAGS_RXTIMEOUT |
                                RFLR_IRQFLAGS_RXDONE |
                                RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                RFLR_IRQFLAGS_VALIDHEADER |
                               RFLR_IRQFLAGS_TXDONE |
                               //RFLR_IRQFLAGS_CADDONE |
                               RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL 
                               //RFLR_IRQFLAGS_CADDETECTED
                                 ;
    
        
                         SndRcvBuf(2);   
                         
                         // clr all interrupt .
                         SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGS;
                         SpiTxBuf[1] = 0xff ;
                         SndRcvBuf(2); 
                         SpiTxBuf[0] = 0x80 | REG_LR_DIOMAPPING1;
                         SpiTxBuf[1] = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 |RFLR_DIOMAPPING1_DIO2_00 |RFLR_DIOMAPPING1_DIO3_00;//0x0d ;
                         SpiTxBuf[2] = RFLR_DIOMAPPING2_DIO4_00 | RFLR_DIOMAPPING2_DIO5_00 ;//0xf0 ; 
                         SndRcvBuf(3);
                         RFLRState = RFLR_STATE_CAD_RX_RUNNING; 
                         SX1276LoRaSetOpMode( RFLR_OPMODE_CAD );
                 
                   }
                   else
                   {
                        SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
                        RFLRState = RFLR_STATE_IDLE;
                        event_Set(HAL_LORA_RX_TIMEOUT);
                        status_Reset(STATUS_RF_WORKING);
                        status_Reset(STATUS_LORA_PINPANG_MODE);
                   }
                    return ;
              }
             temp = RFLR_IRQFLAGS_RXTIMEOUT;

        }
        
        
       	if(IFflage & (1<< 13))
	    {// DIO 3 inerrupt . 

            // case 1 : recv mode , the validHeader interrupt recved . set Rtc delay to check the user id .
              if( RFLRState == RFLR_STATE_RX_SINGLE_RUNNING )
              {
                    temp = RFLR_IRQFLAGS_VALIDHEADER;
                    RFLRState =RFLR_STATE_RX_SINGLE_VALIDHEADER;
                    //if(PACKET_USERID_CNT > 0)
                    SetLoraDelay(ID_Assert) ; 
              }                
            // case 2 : CAD tx mode .      
              else if(RFLRState == RFLR_STATE_CAD_TX_RUNNING)
              {	// cad detected .
                
                    RtyTimes -- ;
                    if(GetCadDetect() && (RtyTimes > 0))
                    { // there is trafic in channel ,delay  and try again .
                          SetLoraDelay(CAD_Send_Delay);
                    }
                    else 
                    {   // no tranfic the channel is free , or  time out .send  now .
                        RFLRState = RFLR_STATE_TX_RUNNING;
                        SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );                           
                     }
                    temp = RFLR_IRQFLAGS_CADDETECTED|RFLR_IRQFLAGS_CADDONE;
               }
     		else if(RFLRState == RFLR_STATE_CAD_RX_RUNNING)
            {
                // check if cad detected check failed .
                if(GetCadDetect())
                {// try to rcv  .

                     SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGSMASK;
             
                     SpiTxBuf[1] =	//RFLR_IRQFLAGS_RXTIMEOUT |
                               //RFLR_IRQFLAGS_RXDONE |
                               //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                               //RFLR_IRQFLAGS_VALIDHEADER |
                               RFLR_IRQFLAGS_TXDONE |
                               RFLR_IRQFLAGS_CADDONE |
                               RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                               RFLR_IRQFLAGS_CADDETECTED;
 

                     SndRcvBuf(2);                     
                     // enable interrupt mask   
                     SpiTxBuf[0] = 0x80 | REG_LR_DIOMAPPING1;
                     SpiTxBuf[1] = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 |RFLR_DIOMAPPING1_DIO2_00 |RFLR_DIOMAPPING1_DIO3_01;//0x0d ;
                     SndRcvBuf(2);                                
                     RFLRState = RFLR_STATE_RX_SINGLE_RUNNING;        
                     SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_SINGLE ); 
      
                }
                else
                {
                    // there is no trafic in channel ,try to again.  .
                     
                     if(RtyTimes == 0)
                     {
                          event_Set(HAL_LORA_RX_CAD_OVR);
                          SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
                          RFLRState = RFLR_STATE_IDLE;
                          status_Reset(STATUS_RF_WORKING);
                          status_Reset(STATUS_LORA_PINPANG_MODE);
                          return ;
                          
                     }
                     else
                          SetLoraDelay(CAD_Recv_Delay);    
                     if(RtyTimes != InfinitRcv)
                          RtyTimes -- ;

                 }
                temp = RFLR_IRQFLAGS_CADDETECTED|RFLR_IRQFLAGS_CADDONE;
            }
	    }
        if(temp != 0)
        SpiTxBuf[1] = temp;
        SpiTxBuf[0] = REG_LR_IRQFLAGS |0x80 ;
        SndRcvBuf(2); 		         
 
 }



/****************************************************************************
* 名    称：void SX1276SetLoRaInit( void )
* 功    能：射频芯片初始化LoRa模式
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
uint8_t  SX1276SetLoRaInit(void)
{
    // Initialize LoRa registers structure

    //SX1276Init();
    //set the LoRa Mode , this must be done  to init the lora mode .
    // disable  interrupt 
    
    SpiEnable();
    RF_Sleep();    
    // check if the lora chip ok 
    AntSleepMode();
    
    SpiTxBuf[0] = 0x42 ;
    SpiTxBuf[1] = 0;
    SndRcvBuf(2);	
    if(SpiRxBuf[1] != 0x12)
    {
      IntDisable();
      return 0;
    }
    // set in lora  mode .
    SpiTxBuf[0] = REG_LR_OPMODE |0x80 ;
    SpiTxBuf[1] = RFLR_OPMODE_SLEEP|0x80 ;
    SndRcvBuf(2);	
    


    
  
    // mask all interrupt .
    SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGSMASK;	
    SpiTxBuf[1] =   RFLR_IRQFLAGS_RXTIMEOUT |
     		        RFLR_IRQFLAGS_RXDONE |
                    RFLR_IRQFLAGS_PAYLOADCRCERROR |
                    RFLR_IRQFLAGS_VALIDHEADER |
                    RFLR_IRQFLAGS_TXDONE |
                    RFLR_IRQFLAGS_CADDONE |
                    RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                    RFLR_IRQFLAGS_CADDETECTED;
    SndRcvBuf(2);
    // clr all interrupt .
    SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGS;
    SpiTxBuf[1] = 0xff ;
    SndRcvBuf(2);
    //Digital IO Pin Mapping
    // Mapping1    7-6 DIO0 , 5-4 DIO1 3-2 DIO2  1-0 DIO3 , Mapping2 7-6 Dio4 ,5-4 DIO5 ,3-0 reserved 0
    // DIOx Mapping |   DIO5    |     DIO4    |       DIO3      |       DIO2        |   DIO1            |  DIO0
    //     00       | ModeReady | CadDetected | CadDone         | FhssChangeChannel | RxTimeout         | RxDone
    //     01       | ClkOut    | PllLock     | ValidHeader     | FhssChangeChannel | FhssChangeChannel | TxDone
    //     10       | ClkOut    | PllLock     | PayloadCrcError | FhssChangeChannel | CadDetected       | CadDone
    //     11       | ----      | ----        | ----            | ----              | ----              | ----
    SpiTxBuf[0] = 0x80 | REG_LR_DIOMAPPING1;
    SpiTxBuf[1] = 0x00 ;
    SpiTxBuf[2] = 0x00 ;
    SndRcvBuf(3);
    
  
    
    
    //LNA gain setting : G1 = highest gain
    SpiTxBuf[0] = 0x80 | REG_LR_LNA;
    SpiTxBuf[1] = RFLR_LNA_GAIN_G1 ;
    SndRcvBuf(2);
   
    // set the RF settings
    SX1276LoRaSetImplicitHeaderOn( 0 );

    SX1276LoRaSetPacketCrcOn( 1);
 
    //设置跳频时间间隔为0，即不跳频
    SX1276LoRaSetHopPeriod(0);

    ChangeRfConfig();   
 
    // set default fifo base address.
    SpiTxBuf[0] = REG_LR_FIFOTXBASEADDR|0x80 ;
    SpiTxBuf[1] = FIFOTXBASEADDR;
    SpiTxBuf[2] = FIFORXBASEADDR;
    SndRcvBuf(3);
    return 1;
 
}

//切换为接收状态,设置为单次接受状态 。
void EnterRcvMode(uint16_t timeout  )
{
    // first should stop current running .
    if(CheckStatus(STATUS_RF_WORKING))
    {
        IntDisable();
        IntClr();
    }
    else
        status_Set(STATUS_RF_WORKING );
    status_Reset(STATUS_LORA_PINPANG_MODE);
    RF_StandBy();
    // timeout <
    RtyTimes = (timeout&RcvTimeOutMask)>>10 ;
    timeout = timeout&0x3ff ;
    if(timeout == 0)
      timeout = globalData.cfginfor.PreambleNum ;
    //设置中断掩码，设置为1的在中断寄存器中不产生中断标志
    SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGSMASK;
    /*
    if(RtyTimes == 0)
    {
            
            SpiTxBuf[1] =	//RFLR_IRQFLAGS_RXTIMEOUT |
                               //RFLR_IRQFLAGS_RXDONE |
                               //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                               //RFLR_IRQFLAGS_VALIDHEADER |
                               RFLR_IRQFLAGS_TXDONE |
                               RFLR_IRQFLAGS_CADDONE |
                               RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                               RFLR_IRQFLAGS_CADDETECTED;
 
    }
    else
    {// cad mode
             SpiTxBuf[1] =      RFLR_IRQFLAGS_RXTIMEOUT |
                                RFLR_IRQFLAGS_RXDONE |
                                RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                RFLR_IRQFLAGS_VALIDHEADER |
                               RFLR_IRQFLAGS_TXDONE |
                               //RFLR_IRQFLAGS_CADDONE |
                               RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL 
                               //RFLR_IRQFLAGS_CADDETECTED
                                 ;
    
    }
    */
    
    // to simple interrupt proc coding 
    SpiTxBuf[1] =	//RFLR_IRQFLAGS_RXTIMEOUT |
                    //RFLR_IRQFLAGS_RXDONE |
                    //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                    //RFLR_IRQFLAGS_VALIDHEADER |
                    RFLR_IRQFLAGS_TXDONE |
                    //RFLR_IRQFLAGS_CADDONE |
                    RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL ;
                    //RFLR_IRQFLAGS_CADDETECTED

    
    SndRcvBuf(2);   
    // clr all interrupt .
    SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGS;
    SpiTxBuf[1] = 0xff ;
    SndRcvBuf(2); 
    // removed to init proc to reduce time
    // no hop .
    //SpiTxBuf[0] = 0x80 | REG_LR_HOPPERIOD;
    //SpiTxBuf[1] = 0;	
    //SndRcvBuf(2);   
    
    //
    //SX1276LoRaSetSymbTimeout(GetRxTimeout());    
    //Digital IO Pin Mapping
    // Mapping1    7-6 DIO0 , 5-4 DIO1 3-2 DIO2  1-0 DIO3 , Mapping2 7-6 Dio4 ,5-4 DIO5 ,3-0 reserved 0
    // DIOx Mapping |   DIO5    |     DIO4    |       DIO3      |       DIO2        |   DIO1            |  DIO0
    //     00       | ModeReady | CadDetected | CadDone         | FhssChangeChannel | RxTimeout         | RxDone
    //     01       | ClkOut    | PllLock     | ValidHeader     | FhssChangeChannel | FhssChangeChannel | TxDone
    //     10       | ClkOut    | PllLock     | PayloadCrcError | FhssChangeChannel | CadDetected       | CadDone
    //     11       | ----      | ----        | ----            | ----              | ----              | ----
    // enable DIO0  RxDone , DIO1  RxTimeOut ,DIO3 ValidHeader interrupt .
    SpiTxBuf[0] = 0x80 | REG_LR_DIOMAPPING1;
    if(RtyTimes == 0)
    {
        SpiTxBuf[1] = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 |RFLR_DIOMAPPING1_DIO2_00 |RFLR_DIOMAPPING1_DIO3_01;//0x0d ;
    }
    else
    { //cad mode
        SpiTxBuf[1] = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 |RFLR_DIOMAPPING1_DIO2_00 |RFLR_DIOMAPPING1_DIO3_00;//0x0d ;
    }    
    SpiTxBuf[2] = RFLR_DIOMAPPING2_DIO4_00 | RFLR_DIOMAPPING2_DIO5_00 ;//0xf0 ; 
    SndRcvBuf(3);
    

        
    //设置RxFiFO的指针起始位置
    SpiTxBuf[0] = REG_LR_FIFOADDRPTR|0x80 ;
    SpiTxBuf[1] = FIFORXBASEADDR;
    SndRcvBuf(2);
    
    SX1276LoRaSetSymbTimeout(timeout);
    NewPacketLocation  = FIFORXBASEADDR ;
    status_Reset(STATUS_LORA_VALID_USERID);
    if(RtyTimes == 0)
    {
        RFLRState = RFLR_STATE_RX_SINGLE_RUNNING;        
        SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_SINGLE );
        //SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_CONTINUE);
    }
    else
    {// cad mode .
        RFLRState = RFLR_STATE_CAD_RX_RUNNING; 
        SX1276LoRaSetOpMode( RFLR_OPMODE_CAD );
    }
    // next there are four statue  1,DIO1  RxTImerOut , 2 DIO0 RxDone 3 DIO3 ValidHeader 4 :PayloadCrcError
    // the order of them is 1,DIO1  RxTImerOut ,2 DIO3 ValidHeader ,3 DIO0 RxDone ,4 :PayloadCrcError
}



//切换为休眠状态 stop current  running.
void RF_Sleep( void )
{
    SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
    RFLRState = RFLR_STATE_IDLE;
    IntDisable();
    status_Reset(STATUS_RF_WORKING);

}

//切换为StandBy状态
void RF_StandBy( void )
{

    SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
    RFLRState = RFLR_STATE_STANDBY;
    IntEnable();
}



/****************************************************************************
 * ??    3?￡ouint8_t GetCCA(void)
 * 1|    ?ü￡o??è?μ±?°D?μà×′ì?
 * è??ú2?êy￡o?T
 * 3??ú2?êy￡o
 CHANNEL_IDLE￡o±íê?D?μà???D
 CHANNEL_BUSY￡o±íê?D?μà?|
 * ?μ    ?÷￡o
 * μ÷ó?·?·¨￡o
 ****************************************************************************/
bool IsChannelFree(void)
{
	uint16_t sum_rssi;
	uint8_t  i;

	sum_rssi = 0;
	for(i=0; i<RSSI_SUM_NUM; i++)
	{
		sum_rssi += SX1276LoraReadRssi();
	}
	sum_rssi = sum_rssi / RSSI_SUM_NUM;
    return sum_rssi<63 ? true : false ;
}
// CAD mode try to tx,  when tx finished , into single recv mode .
void EnterTXMode( uint16_t timeout )
{
    //uint8_t temp ;
    if(CheckStatus(STATUS_RF_WORKING))
    {
        IntDisable();
        IntClr();
    }
    else
        status_Set(STATUS_RF_WORKING );
    
    RF_StandBy();
    if(CheckStatus(STATUS_LORA_PINPANG_MODE))
    {
        SendRtyTimes = timeout ;
        RtyTimes = SendMaxCadCnt ;
    }
    else
    {
        SendRtyTimes = 0 ;
        RtyTimes = timeout & InfinitRcv ;    
    }
    // clr all interrupt .
    SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGS;
    SpiTxBuf[1] = 0xff ;
    SndRcvBuf(2);

	SpiTxBuf[0] = 0x80 | REG_LR_IRQFLAGSMASK;
    if(RtyTimes)
    {
        SpiTxBuf[1] =   RFLR_IRQFLAGS_RXTIMEOUT |
	                    RFLR_IRQFLAGS_RXDONE |
	                    RFLR_IRQFLAGS_PAYLOADCRCERROR |
	                    RFLR_IRQFLAGS_VALIDHEADER |
	                    //RFLR_IRQFLAGS_TXDONE |
	                    //RFLR_IRQFLAGS_CADDONE |
	                    RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL 
	                    //RFLR_IRQFLAGS_CADDETECTED
                          ;
    }
    else
    {
        SpiTxBuf[1] =   RFLR_IRQFLAGS_RXTIMEOUT |
	                    RFLR_IRQFLAGS_RXDONE |
	                    RFLR_IRQFLAGS_PAYLOADCRCERROR |
	                    RFLR_IRQFLAGS_VALIDHEADER |
	                    //RFLR_IRQFLAGS_TXDONE |
	                    RFLR_IRQFLAGS_CADDONE |
	                    RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL  |
	                    RFLR_IRQFLAGS_CADDETECTED;   
    }
											;	
    SndRcvBuf(2); 
    // do not minus 1 , for we need to write the addr first ,it take the len position
	SX1276LoRaSetPayloadLength(SpiFifoTxBuf[0]);

	//set REG_LR_FIFOTXBASEADDR and  REG_LR_FIFOADDRPTR to zero  
 	SpiTxBuf[0] = REG_LR_FIFOADDRPTR|0x80 ;
	SpiTxBuf[1] = FIFOTXBASEADDR;
	SpiTxBuf[2] = FIFOTXBASEADDR;
	SndRcvBuf(3);

    //Digital IO Pin Mapping
 // DIOx Mapping |   DIO5    |     DIO4    |       DIO3      |       DIO2        |   DIO1            |  DIO0
 //     00       | ModeReady | CadDetected | CadDone         | FhssChangeChannel | RxTimeout         | RxDone
 //     01       | ClkOut    | PllLock     | ValidHeader     | FhssChangeChannel | FhssChangeChannel | TxDone
 //     10       | ClkOut    | PllLock     | PayloadCrcError | FhssChangeChannel | CadDetected       | CadDone
 //     11       | ----      | ----        | ----            | ----              | ----              | ----
 
        // set Dio3 CadDone , , Dio0 Txdnoe.
    SpiTxBuf[0] = 0x80 | REG_LR_DIOMAPPING1;
    
    SpiTxBuf[1] = RFLR_DIOMAPPING1_DIO0_01 | RFLR_DIOMAPPING1_DIO1_00 |RFLR_DIOMAPPING1_DIO2_00 |RFLR_DIOMAPPING1_DIO3_00;//0x0d ;
    SpiTxBuf[2] = RFLR_DIOMAPPING2_DIO4_00 | RFLR_DIOMAPPING2_DIO5_00 |1 ;//0xf0 ;    
    SndRcvBuf(3); 
	

    WriteFifo();
    
    if(RtyTimes)
    {
        RFLRState = RFLR_STATE_CAD_TX_RUNNING;   
        SX1276LoRaSetOpMode( RFLR_OPMODE_CAD );
    }
    else
    {
        RFLRState = RFLR_STATE_TX_RUNNING;
        SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );     
    }
    // next there are three statue  Dio3 CadDone , Dio1 CadDetected , Dio0 Txdnoe.
    // the order of them is 1,Dio3 CadDone ,Dio1 CadDetected ,3 DDio0 Txdnoe.  the   CadDetected  will be read when ,caddone .     
}