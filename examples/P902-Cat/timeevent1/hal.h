#ifndef hal_h_20200723
#define hal_h_20200723

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fm33lc0xx_ll_rcc.h"
#include "fm33lc0xx_ll_lptim.h"
#include "fm33lc0xx_ll_lpuart.h"
#include "fm33lc0xx_ll_gpio.h"
#include "fm33lc0xx_ll_uart.h"
#include "fm33lc0xx_ll_vref.h"
#include "fm33lc0xx_ll_iwdt.h"
#include "fm33lc0xx_ll_wwdt.h"
#include "fm33lc0xx_ll_pmu.h"
#include "fm33lc0xx_ll_flash.h"
#include "fm33lc0xx_ll_svd.h"
#include "fm33lc0xx_ll_aes.h"
#include "fm33lc0xx_ll_rmu.h"
#include "fm33lc0xx_ll_rng.h"
#include "fm33lc0xx_ll_opa.h"
#include "fm33lc0xx_ll_comp.h"
#include "fm33lc0xx_ll_hdiv.h"
#include "fm33lc0xx_ll_i2c.h"
#include "fm33lc0xx_ll_spi.h"
#include "fm33lc0xx_ll_u7816.h"
#include "fm33lc0xx_ll_bstim.h"
#include "fm33lc0xx_ll_gptim.h"
#include "fm33lc0xx_ll_atim.h"
#include "fm33lc0xx_ll_crc.h"
#include "fm33lc0xx_ll_dma.h"
#include "fm33lc0xx_ll_rtc.h"
#include "fm33lc0xx_ll_lcd.h"
#include "fm33lc0xx_ll_adc.h"

#include "board.h"
#include "basictype.h"

//#define RCHF_CLOCK  RCC_RCHF_FREQUENCY_8MHZ
#define RCHF_CLOCK   RCC_RCHF_FREQUENCY_16MHZ
//#define RCHF_CLOCK   RCC_RCHF_FREQUENCY_24MHZ

 
extern volatile  int8_t _IntCnt_ ;
__STATIC_INLINE void INT_DISABLE(void)    
{
    _IntCnt_++ ; 
    if(_IntCnt_ >0)
        __disable_irq();
}
__STATIC_INLINE void INT_ENABLE(void)
{
   _IntCnt_-- ; 
   if(_IntCnt_ <=0)
       __enable_irq();
}
__STATIC_INLINE uint8_t INT_CHECK(void)
{
    return _IntCnt_ == 0 ? 1:0 ;
}
void InitPerph(void);
void InitKey(void);
void GPIO_IRQHandler(void);

void InitLed(void);

#define led1off()     GPIO_ResetOutputPin(LED_1_PORT, LED_1_PIN)
#define led1on()    GPIO_SetOutputPin(LED_1_PORT, LED_1_PIN)
#define led1revers() GPIO_ToggleOutputPin(LED_1_PORT,LED_1_PIN)

#define led2off()     GPIO_ResetOutputPin(LED_2_PORT, LED_2_PIN)
#define led2on()    GPIO_SetOutputPin(LED_2_PORT, LED_2_PIN)
#define led2revers() GPIO_ToggleOutputPin(LED_2_PORT,LED_2_PIN)
// used to store inter status .
extern volatile uint32_t HalStatu;
#define IsNoHalStatu()   (HalStatu==0) 
#define SetHalStatu(bit)  HalStatu |=(bit)
#define ClrHalStatu(bit)  HalStatu &= ~(bit)
#define CheckHalStatu(bit) ((HalStatu & (bit)) != 0)

// key pin is down.
#define Hal_Statu_KeyDown       (0x1U <<0)
// uart0 is in recv mode  DMA is used.
#define Hal_Statu_UartRcv     (0x1U  <<1)
// uart0 is in send mode .DMA is used  not used
#define Hal_Statu_LpUartSnd     (0x1U <<2)
// ADC is running  DMA is used 
#define Hal_Statu_ADC            (0x1U <<3)  
// hal1 detected magnet  
#define Hal_Statu_Magnet1On     (0x1U <<4)
// hal1 detected magnet  
#define Hal_Statu_Magnet2On     (0x1U <<5)




#define Hal_Statu_PwrdnReset    (0x1U <<31)
// used to mark hal event happened

// gpio used to ctrol the 3v3 output .
#define EnPowerOut()     GPIO_ResetOutputPin(VOUT_EN_PORT, VOUT_EN_PIN)
#define DisPowerOut()    GPIO_SetOutputPin(VOUT_EN_PORT, VOUT_EN_PIN)
// gpio used to ctrol the Vref output .
#define EnVRefOut()     GPIO_ResetOutputPin(VREF_EN_PORT, VREF_EN_PIN)
#define DisVRefOut()    GPIO_SetOutputPin(VREF_EN_PORT, VREF_EN_PIN)
// gpio used to ctrol the motor power .
#define EnMotorPower()     GPIO_SetOutputPin(MDPWR_CTRL_PORT, MDPWR_CTRL_PIN)
#define DisMotorPower()    GPIO_ResetOutputPin(MDPWR_CTRL_PORT, MDPWR_CTRL_PIN)


// a strcut to record the perph used count  to control perph clock gate enable only , to reduce power 
// the periph  clock select should be done by periph init function . so  call init function before enaable the gate clock.
// max 15 used record, for every periph , when the record is zero , the clock for this periph will be daiabled
typedef enum PerphClk_
{
    // Clock_LPTIM  to  Clock_DCU  is in bus1 group.   reg RCC_PCLKCR1 
    Clock_LPTIM =0,  //  from LSCLK  , APB1CLK .sleep mode 
    //Clock_USB ,   // no used .
    Clock_RTC ,   //APB1CLK   sleep mode 
    Clock_PMU ,   //AHBCLK
    //Clock_SCU ,   // System controller APB bus clock . no used
    Clock_IWDT ,  // APB1CLK
    //Clock_ANAC ,  // Analog controller APB bus clock . no used 
    Clock_GPIO ,  // AHBCLK   sleep mode 
    Clock_DCU ,   // Debug Control Unit APB bus clock
    //   Clock_CRC to Clock_HDIV is in  bus2 group  reg RCC_PCLKCR2
    Clock_CRC,    //APB2CLK
    Clock_RNG,    //APB2CLK
    Clock_AES,    //APB2CLK
    Clock_LCD,    //APB1CLK
    Clock_DMA,    //AHBCLK
    Clock_FLASH,  // AHBCLK
    //Clock_RAMBIST, //AHBCLK
    Clock_WWDT, // APB1CLK
    Clock_ADC,  //APB2CLK    sleep mode 
    Clock_HDIV,  //AHBCLK
    // group 3   reg  RCC_PCLKCR3 
    Clock_SPI1,    //APB1CLK
    Clock_SPI2,    //APB1CLK
    Clock_UART0,   // APB1CLK
    Clock_UART1,   // APB1CLK
    Clock_UART4,   // APB2CLK
    Clock_UART5,   // APB2CLK //no used
    //Clock_UARTIR, //no used
    Clock_LPUART0,  //APB1CLK   sleep mode 
    //Clock_U7816,    // APB1CLK
    Clock_LPUART1,  //APB2CLK   sleep mode 
    Clock_I2C  ,    //APB1CLK
    // group 4  reg  RCC_PCLKCR4 
    Clock_BTIM,   // APB2CLK
    Clock_GTIM0,  // APB1CLK
    Clock_GTIM1,  // APB1CLK
    Clock_ATIM    //APB2CLK
}PerphClk ;
#define SizeOfPeriph    28 
typedef struct PerphClkCtrl_ {
    uint32_t bitmap ;
    uint8_t count[SizeOfPeriph/2];
}PerphClkCtrl;
// define periph it can be running in sleep mode and deep sleep mode .
#define DeepSleepMode_Mask  ((1<<Clock_LPTIM) | (1<<Clock_RTC)  | (1<<Clock_GPIO) |(1<<Clock_LPUART0) | (1<<Clock_LPUART1) ) 
#define SleepMode_Mask     ( DeepSleepMode_Mask|(1<< Clock_BTIM ))
#define SleepMode_CheckMask     (0xffffffff ^SleepMode_Mask)
#define DeepSleepMode_CheckMask     (0xffffffff ^DeepSleepMode_Mask)



void EnablePeriph(PerphClk periph);
void DisablePeriph(PerphClk periph);
/// it will force stop the Periph clock. 
void StopPeriph(PerphClk periph);
// check if the periph is enabled
uint8_t CheckPeriph(PerphClk periph);
// stop all periph , goto deep sleep .
void GotoLowPowerMode(void);
__STATIC_INLINE void  WakeFromSleep(void) 
{
    EnablePeriph(Clock_UART4);
} 

void EnableWinDog(void);
__STATIC_INLINE void  FeedWinDog(void)
{
    if(WWDT->CNT & 0x200) WWDT->CR = 0xac;
}
uint32_t GetRandValue(void);

void GotoSleep(void) ;


#define  RTC_Default_OffSet   0x000000000U
#define  FixHourCnt    4 
//  no used.
//#define RemainSec    RTC->BKR0
//  no used.
//#define RemainTick   RTC->BKR1

// use the RTC  BKR2 store cur Rtc Adjust value and directions :   bit0-15 is Adjust
// high 2 byte is 0xACAC means directtion is 1 , else  if is ox5D5D meanse direction is 0
#define RtcAdjust    RTC->BKR2
#define RtcAdjSlow    0xACAC0000
#define RtcAdjQuick    0x5D5D0000 

#define BootEnableRegister   RTC->BKR7
#define BootEnableMask    0x5D5DACAC
#define SetCheckBoot()    BootEnableRegister = BootEnableMask
#define ClrCheckBoot()    BootEnableRegister = 0
#define CheckBoot()       BootEnableRegister == BootEnableMask


// read time  ,time data in uint8_t buffer formate in bcd code .  like this : s ,m ,h,m,y,w, sigh,sigl.
uint32_t RtcGetTime(void) ;
// set time 
void RtcSetTime(uint32_t time) ;

/*
__STATIC_INLINE uint8_t Bcd2Int(uint8_t bcd)
{
    uint8_t res  = bcd &0xf0 ;
    bcd = bcd & 0x0f ;
    bcd += (res>>3) ;
    bcd += (res>>1) ;
    return bcd ;
}*/
#define Bcd2Int(bcd)   (( ((bcd)>>4)*10)+((bcd) &0xf) )
    
uint8_t Int2Bcd(int8_t bcd);


//#define    Int2Bcd(val)   ((((val)/10)<<4) | ((val)%10))
void InitRtc(void) ;

void RTC_IRQHandler(void);
// hard div :[sign ] 32bit /16bit = [sign ] quotient  : remainder   +  div_zero error.
// 为了简化除法处理流程， harddiv 只处理 无符号数的除法  有符号数需要自行记录符号
typedef struct quotremain_
{
    uint32_t quotient;    // input : the digital will be div , output  the result
    uint16_t remainder;   // input : the  digital used to div , output the reamian 
}QuotRemain ;
// return 0   successed . return 1  divzero find .
uint8_t HardDiv(QuotRemain * value);

#define LPUARTBUFLEN 12


// in sleep mode Uart0  the first cmd byte will not be received, and the second will be received , but the content is error .
// so the real valued data is stored from CmdByteIndex to LPUARTBUFLEN-1. only LPUARTBUFLEN-1 byte is used.
#define CmdByteIndex  0
#define CmdParamIndex 1      
#define Cmd4ByteIndex 4     //for used as uint32_t assigne .


// init the register, and then diable the uart pin  and register 
void InitLpUart0(void);
// enable the register  and pin to send  and will stop register when send over .
void LpUart0Send(StdEvt evt);
void ClearUart0Send(void);

uint8_t  SendLog(uint8_t data) ;
#if RCHF_CLOCK == RCC_RCHF_FREQUENCY_8MHZ
// divder used to divide the APBCLK to get the ir frequncy.
// here APBCLK is  same as RCHF_CLOCK,  ir frequncy is 38K
#define IRBRG  210
// use the ir 1:3 duty  . calc as  (1/3 *(IRBRG + 1))/(IRBRG>>4)) + 0.5);
#define IRTH   5

#define Baud_115200     68
#define Baud_38400      207
#define Baud_9600       832

#elif RCHF_CLOCK == RCC_RCHF_FREQUENCY_16MHZ
// divder used to divide the APBCLK to get the ir frequncy.
// here APBCLK is  same as RCHF_CLOCK,  ir frequncy is 38K
#define IRBRG       420 
// use the ir 1:3 duty
#define IRTH        5   

#define Baud_115200     138 
#define Baud_38400      416
#define Baud_9600       1666
#else
// divder used to divide the APBCLK to get the ir frequncy.
// here APBCLK is  same as RCHF_CLOCK,  ir frequncy is 38K
#define IRBRG        630 
// use the ir 1:3 duty
#define IRTH       5

#define Baud_115200     207
#define Baud_38400      624
#define Baud_9600       2500
#endif

#define UART0_Baud      Baud_9600 

// delay based on baut between to byte .
#define UartSendDelay   4
// overtime value  based on baut  100 .
#define UartRecvOvtm    60

// init the Uart0 . used to receive cmd ,and reset to go to boot mode .
void InitUart0(void);

// do data send .
void Uart0DoIr(void) ;
// stop use  uart0 set pin as OC mode  float .
void Uart0Stop(void);
//hall  init seting . just enable the gpio  output .
void InitHall(void);

#define  EnableHall1()  GPIO_SetOutputPin(HAL1_EN_PORT, HAL1_EN_PIN)
#define  DisableHall1()  GPIO_ResetOutputPin(HAL1_EN_PORT, HAL1_EN_PIN)
#define  EnableHall2()  GPIO_SetOutputPin(HAL2_EN_PORT, HAL2_EN_PIN)
#define  DisableHall2()  GPIO_ResetOutputPin(HAL2_EN_PORT, HAL2_EN_PIN)



// ADC  part  .  ADC  check  ADC11  : VBat slowly,  ADC2,3,4  quicky when used the PWM mode  . inter op1 output - needed .  
//  temper senser  : . slowly   .    Vref  every time  .
// 			Get122VSample = GetVref1p22Sample();
//			GetVSample=GetVoltageSample();
//			GetV =  (GetVSample *3000*(ADC_VREF))/(Get122VSample*4095); 	
//a signed  byte to  store the current Rtc LTBC value . it will be used to coreect the rtc .

#define ADC_VREFValue    (*((uint16_t *)(0x1FFFFB08)))   // 30℃ vref1.22采样值 VCC=3V
//#define ADC_VREFVCC      (*((uint16_t *)(0x1FFFFB0c)))   // 30℃ vref1.22*10000 
#define ADC_TempValue    (*((uint16_t *)(0x1FFFFa92)))   // 30℃ vtemp 采样值 VCC =3V it is about 1000mv  -> 2~12 /3 = 1365

#define Adc_AWD_Limit_High    0x20 
#define Adc_AWD_Limit_Low     0x08 

// ADC part  the Adc checked channel is ADC_Chan2,ADC_Chan3,ADC_Chan4 ,ADC_Chan11 and ADC_ChanTemp ,ADC_ChanVRef,ADC_ChanOp1
// totally seven channle , when do adc ,the adc will do like following order .
#define ADCChanCnt      7 
#define ADC_Chan0   0x001      // -no used
#define ADC_Chan1   0x002      // -no used
#define ADC_Chan2   0x004      // motor 1     0
#define ADC_Chan3   0x008      // motor 2     1
#define ADC_Chan4   0x010      // motor 3     2 
#define ADC_Chan5   0x020
#define ADC_Chan6   0x040
#define ADC_Chan7   0x080
#define ADC_Chan8   0x100
#define ADC_Chan9   0x200
#define ADC_Chan10   0x400
#define ADC_Chan11   0x800     // vbat         3 
#define ADC_ChanTemp 0x1000    // temp         4
#define ADC_ChanVRef 0x2000    // vref1.2V     5
#define ADC_ChanOp1   0x4000   // opt1         6
#define ADC_ChanOp2   0x8000
#define ADC_ChanVRefOffsetMask   (ADC_Chan2|ADC_Chan3|ADC_Chan4|ADC_Chan11|ADC_ChanTemp)
// local data to hold the current adc result and the channel selected.currenty.
typedef struct ADCCfg_
{
    uint16_t  ChanSel ;
    uint16_t  Data[ADCChanCnt];
    int16_t  Result[ADCChanCnt];
}ADCCfg ;

void InitAdc(void);
// input the channel  group . the channel sel and the ADCenable will be disabled when mcu deal the adc result .
// return 0 if there is already a adc is running ,or no channel selected.
uint8_t  AdcStart(uint16_t chan) ;

void CalcAdcResult(void);

// index is 0-6

#define IndISen2 0
#define IndISen1 1
#define IndISen3 2
#define IndVbat  3
#define IndTSens 4
#define IndVRef  5
#define IndOpa1  6
// input : IndXXX , Output ADC voltage result .
uint16_t GetAdcResult(uint8_t index)  ;
uint16_t GetAdcValue(uint8_t i);


void InitDma(void);
// used by adc . copy from periph to ram. size uint16 ,increase address. enable complete interrupt .  enable channel .
#define  DMA_CHAN0_CFG   (1<<11 |1<<4|1<<2|1<<0)

// used by uart0 recv. copy the recv data from  periph to ram   count LPUARTBUFLEN  size uint8 , increase address . enable complete interrupt . enabe channel
#define  DMA_CHAN1_CFG   ((LPUARTBUFLEN-1)<<16 | 1<<11 |1<<2 |1<<0)

// used by uart0 send . copy the send data from ram to periph . count LPUARTBUFLEN size uint8 , increase address . not enable complete interrupt . enabe channel
#define  DMA_CHAN2_CFG   ((LPUARTBUFLEN -1)<<16 | 1<<11 |1<<8 |1 <<6 |1<<0)


void InitMotor(void);
// Motor Used the RTC 16Hz or 64Hz as the mode chang clock .
//#define Motor_Quick_Clock  1
#ifdef  Motor_Quick_Clock 
#define  Motor_RTC_Int_Mask  RTC_ISR_64HZ_IF
#else
#define  Motor_RTC_Int_Mask  RTC_ISR_16HZ_IF
#endif 

//LPTIME Frequncy is : 32768  prediv is  16 .min step is 1/1024/2 = 0.49ms .  max step is 24 day .
#define LPTIME_PREDIV    4    // 5 ->0b 101  : 32   if use the 4 :0b100  = 16  the min step is 0.49ms ,maxday is 24 day. 
#define Sft_OVRFLW_VALUE 0x80000000
#define Sft_OVRFLW_MASK  0x7fffffff   // the max delay time is  half the uint32_t , to simple the overflow case .
#define SoftTimeTickPerSec     (32768>>LPTIME_PREDIV) 
#define SoftTimeTickPerMs (32768>>(LPTIME_PREDIV+10))     // about 2 tick per ms , for  LPTIME_PREDIV =4 . 
//#define Ms2Tick(mscnt)    (((uint32_t)mscnt)*SoftTimeTickPerSec/1000)
#define Ms2Tick(mscnt)    (((uint32_t)mscnt)<<1)

void InitLpTime(void);
#define LPTTIM_CCR1_INTERRUPT    1

#define LPTTIM_CCR2_INTERRUPT    2
#define SetLPTIMCCR1(data)  LPTIM->CCR1 = data 
#define GetLPTIMCCR1()    LPTIM->CCR1
#define SetLPTIMCCR2(data)  LPTIM->CCR2 = data 
#define GetLPTIMCCR2()    LPTIM->CCR2
#define GetLPTIMCNT()       LPTIM->CNT
#define SetLPTIMIER(data)    LPTIM->IER = data 
#define GetLPTIMIER()     LPTIM->IER 
#define SetLPTIMISR(data)    LPTIM->ISR = data 
#define GetLPTIMISR()     LPTIM->ISR 
#define RunLPTIM()  LPTIM->CR = 1 
#define StopLPTIM() LPTIM->CR = 0
#define IsLPTIMRun() ((LPTIM->CR & 1) == 1)
// PWM input clock is 16M , it's  div by PWMDIV  so the Timer clock is 125KHz.
#define PWMDIV   (128-1)
// PWM Frequncy is  PWM Clock /PWMPERIOD =125K/100 = 1.25Khz, period is 800us. 
#define PWMPERIOD  100
// The AT8870 allow max frequency is 100Khz ,  so the min positive  is 10us . that is  1.25percent . so the  min pwm value is2
#define MinPWMDuty 2    // the min Duty allowed
#define BestPWMDuty 10  // the Duty it can let motor speed up ,with small currency. 

#define BrakeStep  6    // the motor in brake mode , the ratio chang with this step.
#define FreeStep   2    // the motor in free mode , the ratio chang with this step.
typedef enum MotorState_
{
    Motor_PowerOff = 0 , // just start it need two pin as high for 2us. and wait for 50us for AT8870 wakeup .
                         // this mode will never be see at timer interrupt .  
    
    Motor_WakeUp,        // from now it can receive pwm wave .
    
    // following two state  is for  hope only .
    Motor_PowerOffFree, // wish  :motor stop at free mode . 
    Motor_PowerOffBrake, // wish  :motor stop at free mode .     
    Motor_ForStable,    // in pwm forward mode .and ratio is not chang .
    Motor_BackStable,    // in pwm backward mode .and ratio is not chang .
    Motor_ForChang,   // rotate forward. and ratio is changing .
    Motor_BackChang,   // rotate backward. and ratio is changing .
    Motor_ForBrake, // brake forward,and ratio is changing .
    Motor_BackBrake, // brake backward,and ratio is changing .
    Motor_ForFree , //  free forward,and ratio is changing .  
    Motor_BackFree , //  free backward,and ratio is changing .  
    
} MotorState, * LPMotorState ;



// you need not to tell the motor to do what next step , just told it what you want it do . it will chang these mode .
// like:  wish it stop a motor  or  wish it forward/backward rotate at a wished pwm ratate and chang step
// the motor hope mode should only be follow , other mode is a temp state can't be stable . 
// Motor_PowerOffFree : stop motor slowly,
// Motor_PowerOffBrake : stop motor slowly,
// Motor_ForStable : forward with power.
// Motor_BackStable : backward with power.
//
typedef struct MotorData_
{
    uint8_t Step  ;      // wished chang step.
    uint8_t Mode ;   // high 4bit is wish mode  , Low 4bit is Current mode . if high 4bit is Motor_PowerOff  means the current motor do not need chang. if low 4bit also is  Motor_PowerOff means the motor not work.
    uint8_t CurRatio ;  // current pwm ratio 
    uint8_t WishRatio ; // wished ratio 
}MotorData, * LPMotorData ;  

void SetMotor(uint8_t index,MotorState mode , uint8_t ratio ,uint8_t step);
void  StartCmp250ms(uint8_t percnt );
void On50HzOvertime(void);

uint8_t SectorErase(uint32_t Address);
uint8_t Program_Sector(uint32_t SectorNum, uint32_t *data);

#endif //hal_h_20200723
