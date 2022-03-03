#include "hal.h"
#include "basictype.h"
#include "halthd.h"
#include "cmdthd.h"
#include "dealerthd.h"
#include "timealarm.h"
#include "thd.h"


// interrupt enable count .
volatile int8_t _IntCnt_ = 0 ;
// global Halstatu 
volatile uint32_t      HalStatu ;
volatile PerphClkCtrl  periphctrl ;
volatile static  ADCCfg  AdcCfg;
// made the Queue for lpuart to send message 



//lpuart buffer .
static StdQueue  LpUartSendQueue ;
#define LpUartSendQueueBufSize   4    
// the StdQueue store uint16_t  size is 4*2
static uint32_t LpUartSendQueueBuf[LpUartSendQueueBufSize] ;   


// using the RTC count as time stamp to filter hall input 
static uint8_t HallTimestamp1;
static uint8_t HallTimestamp2;

static MotorData  MotorCtrl[3] ;

// check if the periph is enabled
uint8_t CheckPeriph(PerphClk periph)
{
    if(periphctrl.bitmap &(1<<periph))
        return 1;
    else
        return 0;
}

void EnablePeriph(PerphClk periph)
{
    uint8_t isfirst = 0;
    uint8_t temp = periphctrl.count[periph>>1];
    if(periph & 1)
    {
        if((temp &0xf0)!= 0xf0)
            temp = temp + 0x10 ;
        if((temp &0xf0) == 0x10)
            isfirst = 1 ;
    }
    else
    {
        if((temp & 0x0f) != 0x0f)
            temp = temp +1 ;
        if((temp & 0x0f) == 1)
            isfirst = 1 ;
    }
    periphctrl.count[periph>>1] = temp ;    
    if(! isfirst )
        return ;
    periphctrl.bitmap |=  (1<<periph) ;
    switch(periph)
    {
        case Clock_LPTIM :
            // enable the bus clock first    
            RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_LPTIM);  
            //RCC->PCLKCR1 |= RCC_BUS1_CLOCK_LPTIM;
            // enable the periph work clock , if needed.
            RCC_Group2_EnableOperationClock(RCC_OPERATION2_CLOCK_LPTIM);
            //RCC->OPCCR2 |= RCC_OPERATION2_CLOCK_LPTIM;
            break;
        case Clock_RTC :
            RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_RTC);
            //RCC->PCLKCR1 |= RCC_BUS1_CLOCK_RTC ;
            break;
        case Clock_PMU :
            RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_PMU);
            //RCC->PCLKCR1 |= RCC_BUS1_CLOCK_PMU ;
            break;
        case Clock_IWDT :
            RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_IWDT);
            //RCC->PCLKCR1 |= RCC_BUS1_CLOCK_IWDT;
            break;
        case Clock_GPIO :
            RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_GPIO);
            //RCC->PCLKCR1 |= RCC_BUS1_CLOCK_GPIO;
            RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_EXTI);
            //RCC->OPCCR1  |= RCC_OPERATION1_CLOCK_EXTI ;
            break; 
        case Clock_DCU :
            RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_DCU);
            //RCC->PCLKCR1 |= RCC_BUS1_CLOCK_DCU;
            break;
        // group 2
        case Clock_CRC :
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_CRC);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_CRC;
            break;
        case Clock_RNG :
            RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_ANAC);
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_RNG);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_RNG;
            RCC_Group2_EnableOperationClock(RCC_OPERATION2_CLOCK_RNG);
            //RCC->OPCCR2  |= RCC_OPERATION2_CLOCK_RNG;
            break;
        case Clock_AES :
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_AES);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_AES;
            break;
        case Clock_LCD :
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_LCD);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_LCD;
            break;
        case Clock_DMA :
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_DMA);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_DMA;
            break;
        case Clock_FLASH :
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_FLASH);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_FLASH;
            RCC_Group2_EnableOperationClock(RCC_OPERATION2_CLOCK_FLASH_ERASN_PROGRAM);
            //RCC->OPCCR2 |= RCC_OPERATION2_CLOCK_FLASH_ERASN_PROGRAM;
            break;
        case Clock_WWDT :
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_WWDT);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_WWDT;
            break;
        case Clock_ADC :
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_ADC);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_ADC;
            RCC_Group2_EnableOperationClock(RCC_OPERATION2_CLOCK_ADC);
            //RCC->OPCCR2 |= RCC_OPERATION2_CLOCK_ADC;
            break;        
         case Clock_HDIV :
            RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_HDIV);
            //RCC->PCLKCR2 |= RCC_BUS2_CLOCK_HDIV;
            break;  
        // group 3 
        case Clock_SPI1 :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_SPI1);
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_SPI1;
            break;  
        case Clock_SPI2 :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_SPI2);
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_SPI2;
            break;  
        case Clock_UART0 :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UART0);
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_UART0;
            RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_UART0);
            //RCC->OPCCR1  |= RCC_OPERATION1_CLOCK_UART0;
            break;  
        case Clock_UART1 :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UART1);
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_UART1;
            RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_UART1);
            //RCC->OPCCR1 |= RCC_OPERATION1_CLOCK_UART1 ;
            break;  
        case Clock_UART4 :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UART4);
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_UART4;
            break;  
        case Clock_UART5 :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UART5);
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_UART5;
            break; 
        case Clock_LPUART0 :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_LPUART0);
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_LPUART0;
            RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_LPUART0);
            //RCC->OPCCR1 |= RCC_OPERATION1_CLOCK_LPUART0;
            break;  
        case Clock_LPUART1 :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_LPUART1);
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_LPUART1;
            RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_LPUART1);
            //RCC->OPCCR1 |= RCC_OPERATION1_CLOCK_LPUART1;
            break;  
        case Clock_I2C :
            RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_I2C); 
            //RCC->PCLKCR3 |= RCC_BUS3_CLOCK_I2C;
            RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_I2C);
            //RCC->OPCCR1 |= RCC_OPERATION1_CLOCK_I2C;
            break;
        // group 4
        case Clock_BTIM :
            RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_BTIM);
            //RCC->PCLKCR4 |= RCC_BUS4_CLOCK_BTIM;
            RCC_Group2_EnableOperationClock(RCC_OPERATION2_CLOCK_BSTIM);
            //RCC->OPCCR2 |= RCC_OPERATION2_CLOCK_BSTIM;
            RCC_SetBSTIMClockSource(RCC_BSTIM_OPERATION_CLK_SOURCE_LSCLK);
            break;          
        case Clock_GTIM0 :
            RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM0);
            //RCC->PCLKCR4 |= RCC_BUS4_CLOCK_GTIM0;
            break;          
        case Clock_GTIM1 :
            RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM1);
            //RCC->PCLKCR4 |= RCC_BUS4_CLOCK_GTIM1;
            break;          
        case Clock_ATIM :
            RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_ATIM);
            //RCC->PCLKCR4 |= RCC_BUS4_CLOCK_ATIM;
            RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_ATIM);
            //RCC->OPCCR1 |= RCC_OPERATION1_CLOCK_ATIM;
            break;          
    }
}
void DisablePeriph(PerphClk periph)
{
    uint8_t islast = 0;
    uint8_t temp = periphctrl.count[periph>>1];
    if(periph & 1)
    {
        if(temp > 0x10)
            temp = temp - 0x10 ;
        else
        {
            islast = 1 ;
            temp &= 0x0f ;
        }
    }
    else
    {
        if((temp & 0x0f) >0x1)
            temp = temp -1 ;
        else
        {
            islast = 1;
            temp &= 0xf0 ;           
        }
    }
    periphctrl.count[periph>>1] = temp ;
    if(! islast)
        return ;
    periphctrl.bitmap &=  (~(1<<periph)) ;
    switch(periph)
    {
        case Clock_LPTIM :
            // enable the bus clock first    
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_LPTIM);   
            // enable the periph work clock , if needed.
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_LPTIM);
            break;
        case Clock_RTC :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_RTC);
            break;
        case Clock_PMU :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_PMU);
            break;
        case Clock_IWDT :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_IWDT);
            break;
        case Clock_GPIO :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_GPIO);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_EXTI);
            break; 
        case Clock_DCU :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_DCU);
            break;
        // group 2
        case Clock_CRC :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_CRC);
            break;
        case Clock_RNG :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_ANAC);
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_RNG);
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_RNG);
            break;
        case Clock_AES :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_AES);
            break;
        case Clock_LCD :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_LCD);
            break;
        case Clock_DMA :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_DMA);
            break;
        case Clock_FLASH :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_FLASH);
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_FLASH_ERASN_PROGRAM);
            break;
        case Clock_WWDT :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_WWDT);
            break;
        case Clock_ADC :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_ADC);
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_ADC);
            break;        
         case Clock_HDIV :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_HDIV);
            break;  
        // group 3 
        case Clock_SPI1 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_SPI1);
        
            break;  
        case Clock_SPI2 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_SPI2);
            break;  
        case Clock_UART0 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART0);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_UART0);
            break;  
        case Clock_UART1 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART1);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_UART1);
            break;  
        case Clock_UART4 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART4);
            break;  
        case Clock_UART5 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART5);
            break; 
        case Clock_LPUART0 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_LPUART0);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_LPUART0);
            break;  
        case Clock_LPUART1 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_LPUART1);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_LPUART1);
            break;  
        case Clock_I2C :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_I2C); 
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_I2C);
            break;
        // group 4
        case Clock_BTIM :
            RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_BTIM);
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_BSTIM);
            break;          
        case Clock_GTIM0 :
            RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_GTIM0);
            break;          
        case Clock_GTIM1 :
            RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_GTIM1);
            break;          
        case Clock_ATIM :
            RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_ATIM);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_ATIM);
            break;          
         
    }    
}

void StopPeriph(PerphClk periph)
{
    uint8_t temp = periphctrl.count[periph>>1];
    if(periph & 1)
    {
         temp &= 0x0f ;
    }
    else
    {
         temp &= 0xf0 ;           
    }
    periphctrl.count[periph>>1] = temp ;
    
    periphctrl.bitmap &=  (~(1<<periph)) ;
    switch(periph)
    {
        case Clock_LPTIM :
            // enable the bus clock first    
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_LPTIM);   
            // enable the periph work clock , if needed.
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_LPTIM);
            break;
        case Clock_RTC :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_RTC);
            break;
        case Clock_PMU :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_PMU);
            break;
        case Clock_IWDT :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_IWDT);
            break;
        case Clock_GPIO :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_GPIO);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_EXTI);
            break; 
        case Clock_DCU :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_DCU);
            break;
        // group 2
        case Clock_CRC :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_CRC);
            break;
        case Clock_RNG :
            RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_ANAC);
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_RNG);
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_RNG);
            break;
        case Clock_AES :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_AES);
            break;
        case Clock_LCD :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_LCD);
            break;
        case Clock_DMA :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_DMA);
            break;
        case Clock_FLASH :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_FLASH);
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_FLASH_ERASN_PROGRAM);
            break;
        case Clock_WWDT :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_WWDT);
            break;
        case Clock_ADC :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_ADC);
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_ADC);
            break;        
         case Clock_HDIV :
            RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_HDIV);
            break;  
        // group 3 
        case Clock_SPI1 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_SPI1);
        
            break;  
        case Clock_SPI2 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_SPI2);
            break;  
        case Clock_UART0 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART0);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_UART0);
            break;  
        case Clock_UART1 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART1);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_UART1);
            break;  
        case Clock_UART4 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART4);
            break;  
        case Clock_UART5 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART5);
            break; 
        case Clock_LPUART0 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_LPUART0);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_LPUART0);
            break;  
        case Clock_LPUART1 :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_LPUART1);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_LPUART1);
            break;  
        case Clock_I2C :
            RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_I2C); 
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_I2C);
            break;
        // group 4
        case Clock_BTIM :
            RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_BTIM);
            RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_BSTIM);
            break;          
        case Clock_GTIM0 :
            RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_GTIM0);
            break;          
        case Clock_GTIM1 :
            RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_GTIM1);
            break;          
        case Clock_ATIM :
            RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_ATIM);
            RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_ATIM);
            break;          
         
    }    
}

void EnableWinDog(void)
{
    EnablePeriph(Clock_WWDT);
    WWDT->IER = 0  ;
    WWDT->ISR = 1 ;
    WWDT->CFGR =  7  ;  // about 250ms for 16M ,
    WWDT->CR = 0x5a ;
}

void InitPerph(void)
{
    uint8_t res ;
    HalStatu = 0 ;
    //periph enable control , every periph  has max 15 counter to mark the user.
    uint8_t * p =(uint8_t *) (&periphctrl );
    for(res = 0 ; res < sizeof(periphctrl) ;res++)
    {
        *p = 0 ;
        p++;
    }

    INT_DISABLE();
    
    RCC_SetAHBMasterPriority(RCC_AHB_MASTER_PRIORITY_CPU_FIRST);

    EnablePeriph(Clock_GPIO);  
    //RTC模块时钟打开   has been opened in SystemInit just after reset.
    
    // set SWD enable . it has been enabled in SystemInit
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin  = GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_DIGITAL;
    GPIO_InitStruct.Pull = ENABLE;
    GPIO_Init(GPIOD, &GPIO_InitStruct); 
    
    //GPIO_DisablePinOpendrain(GPIOD,GPIO_PIN_7|GPIO_PIN_8);
    //GPIO_SetPinMode(GPIOD,LED_1_PIN,GPIO_MODE_DIGITAL);
    //GPIO_SetPinMode(GPIOD,LED_2_PIN,GPIO_MODE_DIGITAL);
    //GPIO_EnablePinPullup(GPIOD,GPIO_PIN_7|GPIO_PIN_8);
    
    // stop power down detect.
    RMU_EnablePowerDownReset(RMU);
    RMU_DisableBORPowerDownReset(RMU);
    INT_ENABLE();
}


//  following periph can work if the high speed clock stoped 
//  LPUART0-1  LPTIM RTC  GPIO, Comp BTIM
// XTL RESET RCL POR/BOR LVD  do not need clock.
// in the  PERI_CLKEN  it is  0x107a820c  , the reverse is 0xef056df3 (bit 12, and 23 no used)
void GotoSleep(void) 
{
 //   FeedWinDog();
    if((periphctrl.bitmap & SleepMode_CheckMask) != 0 )
    {  // high speed periph is running   . continue ..

        __WFI();  
        return ;
    }
#if  SleepMode_Enable  == 1      
    else
    {   // can go to sleep mode . 
  
        if((periphctrl.bitmap & DeepSleepMode_CheckMask) != 0)
        {  //sleep mode .
            
 #if  RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_8MHZ
            PMU->CR  = 0xA0102;
 #elif RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_16MHZ
            PMU->CR  = 0xA0502;
 #elif RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_24MHZ
            PMU->CR  = 0xA0902;
 #endif             
            
        }
        else
        {
            // deepsleep mode .

            //stop inter 32k rc .
            RCC_SetSleepModeRCLPWorkMode(RCC_RCLP_UNDER_SLEEP_CLOSE);
            //SCB->SCR =  0x04; 
 #if  RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_8MHZ
            PMU->CR  = 0xA0302;
 #elif RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_16MHZ
            PMU->CR  = 0xA0702;
 #elif RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_24MHZ
            PMU->CR  = 0xA0B02;
 #endif 
        }
    }
#endif
    __WFI();  
    // return to nomal state.
    //SCB->SCR =  0;    
    // enable ldo to normal mode .
    PMU->CR  = 0x20400;
    // enable inter 32K lp osc for xtal fail use. 
    RCC_SetSleepModeRCLPWorkMode(RCC_RCLP_UNDER_SLEEP_OPEN);
}


// stop all periph , goto deep sleep .
void GotoLowPowerMode(void)
{
    // put all pin as oc , high ->  float . it will disable all outer periph power .
    GPIOA->FCR = 0x55555555;
    GPIOA->DO =  0xFFFD;
    GPIOA->ODEN =  0xFFFF;   
    GPIOB->FCR = 0x55555555;
    GPIOB->DO =  0xFFDB;;  
    GPIOB->ODEN =  0xFFFF;
    GPIOC->FCR = 0x55555555;
    GPIOC->DO =  0xFFFF;
    GPIOC->ODEN =  0xFFFF;
    //  D7 and D8  as   SWD
    GPIOD->FCR = 0x557E9555; //GPIO_PIN_ALL_EXCEPTSWD and XTAL pin .
    GPIOD->DO =  0xF87F;   //GPIO_PIN_ALL_EXCEPTSWD and XTAL pin .
    GPIOD->ODEN =  0xF87F; //GPIO_PIN_ALL_EXCEPTSWD
    GPIOD->PUEN =  0x0180;  
    // disable all inter periph .
    RCC->PCLKCR1=0;
    RCC->OPCCR2=0;
    RCC->PCLKCR2 =0;
    RCC->OPCCR1=0;
    RCC->PCLKCR3 =0;
    RCC->PCLKCR4 =0 ;    
    // disable all interrupt .
    NVIC->ICER[0U] = 0xffffffff;
    // goto deep sleep
    PMU->CR  = 0xA0202;
    __WFI();  
}


uint8_t Int2Bcd(int8_t bcd)
{
    uint8_t res  = 0 ;
    while(bcd >=10)
    {
        res ++ ;
        bcd -= 10;
    }
    res <<=4 ;
    res |= bcd;
    return res ;
}

uint32_t GetRandValue(void)
{
    uint32_t  value ;
    EnablePeriph(Clock_RNG);
    RNG_ClearFlag_RandomFail(RNG);
    RNG_Enable(RNGCTL);
    __NOP();
    __NOP();
    __NOP();
    RNG_Disable(RNGCTL);
    value = RNG_Read(RNG);
    if(RNG_IsActiveFlag_RandomFail(RNG))
    {
        RNG_ClearFlag_RandomFail(RNG);
        value = 0x12345678;
    }
    DisablePeriph(Clock_RNG);
    return value;   
}
void InitKey(void)
{

    // key1  and key 2 has same port.   set as input . use key 1 only. 
    GPIO_SetPinMode(KEY_1_PORT,KEY_1_PIN,GPIO_MODE_INPUT);
    GPIO_SetPinMode(KEY_1_PORT,KEY_2_PIN,GPIO_MODE_INPUT);
    GPIO_EnablePinInput(KEY_1_PORT,LED_1_PIN | LED_2_PIN);
    

    // set exti sel
    
    // set input edge.
    EXTI_SetTrigEdge(GPIO_COMMON,GPIO_EXTI_LINE_7,GPIO_EXTI_TRIGGER_BOTHEDGE);
    // enable filter
    EXTI_EnableDigitalFilter(GPIO_COMMON,GPIO_EXTI_LINE_7);
    // set exti pin , set Key_2_Pin as input pin .
    GPIO_SetExitLine7(GPIO_COMMON,GPIO_EXTI_LINE_7_PB13);
    // clear exti flage
    EXTI_ClearFlag(GPIO_COMMON,GPIO_EXTI_LINE_7);
    // set exti poriority
    NVIC_SetPriority(GPIO_IRQn,2) ;
    // enable exti 
    NVIC_ClearPendingIRQ(GPIO_IRQn);
    NVIC_EnableIRQ(GPIO_IRQn);
}
extern void  OnLoraIqr(void);
void GPIO_IRQHandler(void)
{
    // clear exti flage .
    uint32_t flage = GPIO_COMMON->EXTIISR;
    GPIO_COMMON->EXTIISR = flage ;
    if(flage & (1<<7))
    { // key inputed
        if(GPIO_COMMON->EXTIDI &(1<<7))
        { // key up .
            ClrHalStatu(Hal_Statu_KeyDown);
            postevt((LPThdBlock)&(halmaster.super),Sig_Key_Up); 
        }
        else
        {
            SetHalStatu(Hal_Statu_KeyDown);
            postevt((LPThdBlock)&(halmaster.super),Sig_Key_Down);
        }
    }
    if(flage & (1<<4))
    {   // Hal2 inputed valid only when  hal2  enabled :PC5 is high
        if(GPIOC->DO & (1<<5))
        {
            if(GPIO_COMMON->EXTIDI &(1<<4))
            { // high  no magnet.
                ClrHalStatu(Hal_Statu_Magnet2On);
                postevt((LPThdBlock)&(halmaster.super),Sig_Magnet2_Remove); 
            }
            else
            {
                // using the RTC count as time stamp to filter hall input 
                SetHalStatu(Hal_Statu_Magnet2On);
                HallTimestamp2 -= RTC->SBSCNT ; 
                if(HallTimestamp2 > 80)
                {
                    postevt((LPThdBlock)&(halmaster.super),Sig_Magnet2_On);
                    HallTimestamp2 = RTC->SBSCNT ; 
                }
            }
        }
    }
    
    if(flage & (1<<0))
    { // Hal1 inputed only PC4 is high 
        if(GPIOC->DO & (1<<4))
        {
            if(GPIO_COMMON->EXTIDI &(1<<0))
            { // magnet removed  down edge detected .
                ClrHalStatu(Hal_Statu_Magnet1On);
                postevt((LPThdBlock)&(halmaster.super),Sig_Magnet1_Remove); 
            }
            else
            {
                SetHalStatu(Hal_Statu_Magnet1On);
                
                HallTimestamp1 -= RTC->SBSCNT ; 
                if(HallTimestamp1 > 80)
                {
                    postevt((LPThdBlock)&(halmaster.super),Sig_Magnet1_On);;
                    HallTimestamp1 = RTC->SBSCNT ; 
                }                
            }
        }
    }  
  
    if(flage & (1<<2))
    { // lora  inputed only PA15 is high 
        if(GPIOA->DO & (1<<15))
        {//call lora interrupt .
           OnLoraIqr(); 
        }
    }     
}
void InitLed(void)
{
    // led 1 and led1 has same port.
    GPIO_DisablePinOpendrain(LED_1_PORT,LED_1_PIN|LED_2_PIN);
    GPIO_SetPinMode(LED_1_PORT,LED_1_PIN,GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(LED_1_PORT,LED_2_PIN,GPIO_MODE_OUTPUT);
    GPIO_ResetOutputPin(LED_1_PORT,LED_1_PIN | LED_2_PIN);
    
    // init the power out gpio
    GPIO_SetPinMode(VOUT_EN_PORT,VOUT_EN_PIN,GPIO_MODE_OUTPUT);
    DisPowerOut();
}


/*
// the temperature cause the rtc delayed  value in ppm :  K*(Toffset * Toffset) (K can be get for datasheet, = -0.035)  
so  we can get the addjust value in ppm accroding to Toffset  , it is Cureent temperature - CRYSTAL_CENTER_TEMP
ticks from ppm fromat is :  ppm*32/30.517 = ppm*1.0485958646000589835173837533178

==>  the FM33LC rtc is adjusted by 32s . so the addjust value is ROUND( crystalCompensationTable[i]*K*32*32768/1000000 )
==>  according to test set hte k is  0.016 , and to get round (ROUND,0) 
==>  the rang for device to addjust is [-511  ,511] , so remove the value bigger than it , the leng is limited to 51 it means [-25  - 75] degree.
==>  get new table  :
*/
#define CRYSTAL_CENTER_TEMP                250  // the center of the curve. based on 0.1c
#define CRYSTAL_CENTER_OFFSET              0    // the center point offset of the curve in degree，it's the offset  the curve above the zero line at middle point.
#define CRYSTAL_COMPENSATION_TABLE_LENGTH  50   // data count 
#define CRYSTAL_ADDJUST_OFFSET             21    // addjust quickly value .  

// the real cal value is data in  crystalCompensationTable - CRYSTAL_CENTER_OFFSET

const uint8_t crystalCompensationTable[50]    = 
{// 1   2  3  4  5  6  7  8  9  10 11 12
    0,  0, 1, 1, 1, 1, 2, 2, 3, 3, 4, 5,
    6,  7, 8, 9,10,11,12,14,15,17,18,20,
    22,23,25,27,29,31,33,36,38,40,43,45,
    48,51,53,56,59,62,65,68,71,75,78,81,
    85,89
};
// temp :  temperature ,based on 0.1C
// addjust rtc adjust register by temperature .
void RtcTempAdjust(int16_t temp)
{
	// according to the temperature to adjust the offset , inorder to get precise time .
    // Get the abs differ between current temp and CRYSTAL_NOMINAL_TEMP.
    QuotRemain  qr;
    if(temp >CRYSTAL_CENTER_TEMP)
         qr.quotient = temp - CRYSTAL_CENTER_TEMP;
    else
         qr.quotient = CRYSTAL_CENTER_TEMP -temp;
    qr.remainder = 10 ;
    HardDiv(&qr);     
    if (qr.remainder >5)
       qr.quotient++ ;
    // Checking value . should not bigger than 50 
    if (qr.quotient > CRYSTAL_COMPENSATION_TABLE_LENGTH - 1)
    { 
        qr.quotient = CRYSTAL_COMPENSATION_TABLE_LENGTH - 1;
    }
   
    temp =  crystalCompensationTable[qr.quotient] + CRYSTAL_ADDJUST_OFFSET;  
    // Get additional accumulator value from lookup table using temperature value
    // It is assumed that the temperature was the same from now to the next time of doing adjust .
#if CRYSTAL_CENTER_OFFSET > 0    
    if(qr.quotient  >= CRYSTAL_CENTER_OFFSET)
    {// sub the 1s count  to quick the rtc time. 
        RtcAdjust =  RtcAdjQuick | (temp - CRYSTAL_CENTER_OFFSET ) ;
        //RTC->ADJUST = temp - CRYSTAL_CENTER_OFFSET ;
        //RTC->ADSIGN = 1;
    }
    else
    { // add the 1s  count  to slow the rtc time.
        //RTC->ADJUST =  CRYSTAL_CENTER_OFFSET - temp ;
        //RTC->ADSIGN = 0;
        RtcAdjust =  RtcAdjSlow | (CRYSTAL_CENTER_OFFSET - temp  ) ;
    }
#else
    RtcAdjust =  RtcAdjQuick | temp ;    
#endif 
    
    RTC->ISR = RTC_IER_SEC_IE;  
    RTC->IER |= RTC_IER_SEC_IE ;
      
    
    //RTC->WER       = 0x00000000U;
//    DisablePeriph(Clock_RTC);
 }
void InitRtc(void)
{
    EnablePeriph(Clock_RTC);   // do not stop it .
    //RTC_EnableIT_Hour(RTC);
    RtcAdjust = 0 ;
    RTC->IER = RTC_IER_HOUR_IE_Msk ;
    RTC->ISR = 0xffff;
//    DisablePeriph(Clock_RTC);    
    // set exti poriority
    NVIC_SetPriority(RTC_IRQn,1) ;
    // enable exti 
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);   
  
}


// predecalre the MotorProc .
void MotorProc(uint8_t res );
void RTC_IRQHandler(void)
{
    static uint8_t HourCnt= FixHourCnt;
// rtc tick and rtc alarm.
    uint8_t  temp ;
    uint32_t res = RTC->ISR ;
    RTC->ISR  = res ; 
    res &= RTC->IER ;
    if(res & RTC_IER_SEC_IE)
    { // do a addjust .
        RTC->IER &=  (~RTC_IER_SEC_IE); 
        if((RtcAdjust & 0xffff0000) == RtcAdjSlow ) 
        {// add rtc count for 1s , it slowly the rtc
           RTC->ADJUST = RtcAdjust &0x1ff;
           RTC->ADSIGN = 1; 
        }
        else if ((RtcAdjust & 0xffff0000) == RtcAdjQuick ) 
        { // sub rtc count for 1s , it quickly the rtc
           RTC->ADJUST = RtcAdjust &0x1ff;
           RTC->ADSIGN = 0; 
        }

    }   
    if(res & RTC_ISR_HOUR_IF)
    { // post a HourEvent .
        postevtbyindex(BlcId_Net,Sig_Rtc_FixHourInt);
        if(HourCnt >1) 
        {   // addjust rtc every hour  once time .
            HourCnt --;
            RTC->ISR = RTC_IER_SEC_IE;  
            RTC->IER |= RTC_IER_SEC_IE ;            
        }
        else
        {   // get new addjust value ,and set it  .
            HourCnt= FixHourCnt ;
            postevt((LPThdBlock)&(halmaster.super),Sig_Rtc_FixHourInt);
        }
    }   
    // 64 HZ  motor driver .
    if(res & Motor_RTC_Int_Mask)
    {
        for (temp = 0 ; temp <3 ; temp++)
        {
            MotorProc(temp);
        }
        temp =MotorCtrl[0].Mode | MotorCtrl[1].Mode | MotorCtrl[2].Mode ;
        if((temp&0xf0) == 0)
        {// disable driver clock.
            RTC->IER &= (~ Motor_RTC_Int_Mask);
            if(temp==0)
            { //all motor is in stop mode .
                DisVRefOut();
                DisMotorPower() ;           
            }
        }           
    
    }   
    // time alarm 
    if(res & RTC_ISR_ALARM_IF_Msk)
    {// we should check the week bit , it maybe not the week for today
        if(globaldata.timedata.alarm.data8.data3 ==  RTC->BCDWEEK )
        {
            if(OnRtcAlarm()) 
                FindCurEvtAndWait();
            outlog(log_rtc_int);
        }
    }
    if(res & RTC_IER_DAY_IE_Msk)
    {
        postevtbyindex(BlcId_Net,Sig_Rf_DayOver);
    }

 }

// read time  formate in bcd code .  like this : s ,m ,h,m,y,w
uint32_t RtcGetTime(void)
{
    uint32_t time =0;
    uint32_t temp ;
//    EnablePeriph(Clock_RTC);  
    while(1) 
    {
        //read to buffer first 
        time = Bcd2Int((uint8_t )RTC->BCDSEC);

        temp = Bcd2Int((uint8_t )RTC->BCDMIN);
        temp <<=6 ;
        time |= temp;    
        
        temp = Bcd2Int((uint8_t ) RTC->BCDHOUR) ;
        temp <<=12 ;
        time |= temp;
        
        temp = Bcd2Int((uint8_t ) RTC->BCDDAY) ;
        temp <<=17 ;
        time |= temp;     
        
        temp = Bcd2Int((uint8_t ) RTC->BCDMONTH) ;
        temp <<=22 ;
        time |= temp; 

        temp = Bcd2Int((uint8_t ) RTC->BCDYEAR) ;
        temp <<=26 ;
        time |= temp; 

        // check if it changed 
        temp = Bcd2Int((uint8_t )RTC->BCDSEC);
        temp = temp ^time ;
        if((temp & 0x3f) ==0 ) 
            return time;
    }
//   DisablePeriph(Clock_RTC);
}    
// set time formate in bcd code .  like this : s ,m ,h,m,y,w
void RtcSetTime(uint32_t time)
{
    // stop rtc first .
//    EnablePeriph(Clock_RTC);
    NVIC_DisableIRQ(RTC_IRQn);
    RTC->ISR = 0xff ;
    CLEAR_BIT(RTC->IER, RTC_IER_ALARM_IE_Msk|RTC_IER_4HZ_IE_Msk);
    RTC->WER       = 0xACACACACU;   
    
    RTC->BCDSEC = Int2Bcd(time&0x3f) ;
    time>>= 6 ;
    RTC->BCDMIN = Int2Bcd(time&0x3f)  ;
    time>>= 6 ;
    RTC->BCDHOUR= Int2Bcd(time&0x1f)  ;
    time>>= 5 ;
    RTC->BCDDAY = Int2Bcd(time&0x1f)  ;
    time>>= 5 ;        
    RTC->BCDMONTH = Int2Bcd(time&0x0f)  ;
    time>>= 4 ;
    RTC->BCDYEAR = Int2Bcd(time&0x3f)  ;
    //start rtc 
    RTC->WER       = 0;  
    NVIC_EnableIRQ(RTC_IRQn);
//    DisablePeriph(Clock_RTC);
}    




// return 0   successed . return 1  divzero find .
uint8_t HardDiv(QuotRemain * lpvalue)
{
    if(lpvalue->remainder == 0)
        return 1 ;
    EnablePeriph(Clock_HDIV);
    HDIV ->END = lpvalue->quotient;
    HDIV ->SOR  = lpvalue->remainder;
    while(HDIV->SR & HDIV_SR_BUSY_Msk) {};
    lpvalue->quotient =  HDIV->QUOT;
    lpvalue->remainder = HDIV ->REMD;
    DisablePeriph(Clock_HDIV);
    return 0 ;
}

// init the register, and then diable the uart pin  and register , and the lptim.
// bault is  9600 .
void InitLpUart0(void)
{
    // enable the LPUART0 always .
    EnablePeriph(Clock_GPIO);
    EnablePeriph(Clock_LPUART0);

    GPIO_DisablePinOpendrain(LPUART_TX_PORT,LPUART_RX_PIN|LPUART_TX_PIN);
    GPIO_SetPinMode(LPUART_TX_PORT,LPUART_RX_PIN,GPIO_MODE_DIGITAL);
    GPIO_SetPinMode(LPUART_TX_PORT,LPUART_TX_PIN,GPIO_MODE_DIGITAL);
    GPIO_EnablePinPullup(LPUART_TX_PORT,LPUART_RX_PIN);   

    #ifndef  Uart_SCH_Define
       SetRegBit(LPUART_TX_PORT->DFS , LPUART_RX_PIN|LPUART_TX_PIN);
    #endif
    
    LPUART_SetDataRecvedWakeupMode(LPUART0,LPUART_WAKE_UP_AFTER_CHECK);
    LPUART_SetWakeupEvent(LPUART0,LPUART_WAKE_UP_EVENT_RECV_MATCH_BYTE);
    LPUART_SetTransferBitOrder(LPUART0,LPUART_BITORDER_LSBFIRST);
    
    LPUART_SetStopBitsLength(LPUART0,LPUART_STOP_1BITS);
    LPUART_SetDataWidth(LPUART0,LPUART_DATAWIDTH_8B);
    LPUART_SetParity(LPUART0,LPUART_PARITY_NONE);
    LPUART_SetRxPolarity(LPUART0,LPUART_RX_POLARITY_NORMAL);
    LPUART_SetTxPolarity(LPUART0,LPUART_TX_POLARITY_NORMAL);
    
    
    LPUART_SetBaudRate(LPUART0,LPUART_BAUDRATE_9600);
    LPUART_SetMCTLVel(LPUART0,0x0552); // 9600

    LPUART_SetMatchedData(LPUART0,CmdHeader);
    LPUART0->IER = LPUART_IER_RXEV_IE_Msk ;
   
    //start recv
    ClrHalStatu(Hal_Statu_LpUartSnd);
    ClrHalStatu(Hal_Statu_Log); 
    cmdport.lplpuartrecv = NULL ;
    LPUART_EnabledRx(LPUART0);
    
    InitQueue(&LpUartSendQueue ,LpUartSendQueueBuf,LpUartSendQueueBufSize);
    
    NVIC_DisableIRQ(LPUART0_IRQn);
    NVIC_SetPriority(LPUART0_IRQn,2);//中断优先级配置
    NVIC_EnableIRQ(LPUART0_IRQn);     //使能NVIC
}



void LPUART0_IRQHandler(void)
{

	StdEvt evt; 
    uint32_t res = LPUART0->ISR ;
    LPUART0->ISR  = res ; 
	//接收中断处理
	if((res & LPUART_ISR_RXBF_Msk) &&(LPUART0->IER & LPUART_IER_RXBF_IE))//接收数据中断
	{ // during recv .
		cmdport.lplpuartrecv[cmdport.lpuartrecvindex] = LPUART_ReceiveData(LPUART0);   
        cmdport.lpuartrecvindex ++;
        if(cmdport.lpuartrecvindex == LPUARTBUFLEN )
        { 
            ClrRegBit(LPUART0->IER , (LPUART_IER_RXBF_IE|LPUART_IER_RXERR_IE));
            SetRegBit(LPUART0->IER , LPUART_IER_RXEV_IE); 


            evt = makeevt(cmdport.lplpuartrecv[0],cmdport.lpuartrecvid);
            postevt((LPThdBlock)&(cmdport.super),evt);
            // buffer will be remalloc when receive the header cmd byte.
            cmdport.lplpuartrecv = NULL ;
            
        }        
	}
	if((res & LPUART_ISR_RXEVF_Msk) &&(LPUART0->IER & LPUART_IER_RXEV_IE))//接收数据匹配中断
	{ // start recv .
        ClrRegBit(LPUART0->IER , LPUART_IER_RXEV_IE);
        SetRegBit(LPUART0->IER , (LPUART_IER_RXBF_IE|LPUART_IER_RXERR_IE));
        // remalloc the recv buf .
        if(cmdport.lplpuartrecv == NULL )
        {
            evt = newevt(Sig_None,PoolId1);
            cmdport.lpuartrecvid = getpoolid(evt);
            cmdport.lplpuartrecv   = (uint8_t *)getevtmem(evt);        
        }
        cmdport.lpuartrecvindex =  0; 

	}
	if((res & (LPUART_ISR_PERR|LPUART_ISR_FERR)) &&(LPUART0->IER & LPUART_IER_RXERR_IE))
	{ // re enable recv .
        ClrRegBit(LPUART0->IER , (LPUART_IER_RXBF_IE|LPUART_IER_RXERR_IE));
        SetRegBit(LPUART0->IER , LPUART_IER_RXEV_IE);
        cmdport.lpuartrecvindex =  0;     
        //ClrHalStatu(Hal_Statu_LpUartRcv);
	}

	//发送中断处理
	if((res & LPUART_IER_TXSE_IE_Msk) &&(LPUART0->IER & LPUART_IER_TXSE_IE))
	{
#if _ASSERT  == 1
        if( CheckHalStatu(Hal_Statu_Log) )
        {
                // no event is sendding
            LPUART0->TXBUF = getlog();
        }
        else
#endif        
		{
            // last byte is send over . 
            ClrRegBit(LPUART0->IER , LPUART_IER_TXSE_IE);
            // check if there is  data in Queue
            evt = LookQueue(&LpUartSendQueue);        
            if(evt != 0)
            {
                cmdport.lplpuartsend   = (uint8_t *)getevtmem(evt);    
                cmdport.lpuartsendcnt  = getevtmemlength(evt);
                cmdport.lpuartsendindex = 0 ;
                LPUART0->TXBUF =CmdHeader;                        
                SetRegBit(LPUART0->IER , LPUART_IER_TXBE_IE); 
                // clear the LPUART_ISR_TXBE_IE
                SetRegBit(LPUART0->ISR , LPUART_IER_TXBE_IE);
                return ;
            }
            else
            {
                LPUART_DisableTx(LPUART0);
                ClrHalStatu(Hal_Statu_LpUartSnd);
            }
        }
	}

 	if((res & LPUART_IER_TXBE_IE_Msk) &&(LPUART0->IER & LPUART_IER_TXBE_IE))
	{
		// during send .
        if(cmdport.lpuartsendindex == cmdport.lpuartsendcnt)
        { // last one  . 
            evt = GetQueue(&LpUartSendQueue);
            freeevt(evt);
            SetRegBit(LPUART0->IER , LPUART_IER_TXSE_IE); 
            ClrRegBit(LPUART0->IER , LPUART_IER_TXBE_IE);            
            LPUART0->TXBUF =  CmdEnder;            
            cmdport.lpuartsendcnt  = 0;
        }
        else
        {
            LPUART0->TXBUF =  cmdport.lplpuartsend[cmdport.lpuartsendindex];
            cmdport.lpuartsendindex++ ;        
        }
	} 


}
// enable the register  and pin to send  and will stop register when send over .
void LpUart0Send(StdEvt evt)
{
    if(SetQueue(&LpUartSendQueue,evt) >1)
    { // there is more than one msg wait send : there is evt being send now . just return .
        return ;
    }
#if _ASSERT    == 1
    if(CheckHalStatu(Hal_Statu_LpUartSnd))
        return ;
#endif    
    // there is no evt being send now . do send 
    SetHalStatu(Hal_Statu_LpUartSnd);
    cmdport.lplpuartsend   = (uint8_t *)getevtmem(evt);     
    cmdport.lpuartsendindex = 0 ;
    cmdport.lpuartsendcnt = getevtmemlength(evt);
    LPUART0->TXBUF =CmdHeader;
    LPUART_ClearFlag_TXBE(LPUART0);
    LPUART_EnabledIT_TXBE(LPUART0);
    LPUART_EnabledTx(LPUART0); 
}
void ClearLpUart0SendBuffer(void)
{
    ResetQueue(&LpUartSendQueue);
}
// try to send the Log data out . send when there is no evt is sending 
uint8_t SendLog(uint8_t data) 
{
    // there is no evt being send now . do send 
    if(CheckHalStatu(Hal_Statu_LpUartSnd))
        return 0;
    SetHalStatu(Hal_Statu_LpUartSnd); 
    LPUART0->TXBUF =data;
    SetRegBit(LPUART0->IER , LPUART_IER_TXSE_IE); 
    ClrRegBit(LPUART0->IER , LPUART_IER_TXBE_IE);    
    LPUART_EnabledTx(LPUART0);  
    return 1 ;    

}
// uart0 part 
// init the Uart0 . used to receive cmd ,and reset to go to boot mode .
void InitUart0(void)
{

    EnablePeriph(Clock_UART0);
    //  set txd and rxd as gpio output OC mode  .
    GPIO_EnablePinOpendrain(UART_TX_PORT,UART_RX_PIN|UART_TX_PIN);
    GPIO_DisablePinPullup(UART_TX_PORT,UART_RX_PIN|UART_TX_PIN); 
    GPIO_SetOutputPin(UART_TX_PORT,UART_RX_PIN|UART_TX_PIN);    
    GPIO_SetPinMode(UART_RX_PORT,UART_RX_PIN,GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(UART_TX_PORT,UART_TX_PIN,GPIO_MODE_OUTPUT);    

    #ifndef  Uart_SCH_Define
       ClrRegBit(UART_TX_PORT->DFS , UART_RX_PIN|UART_TX_PIN); 
    #endif
    //set default value .
    //UART0->CSR = 0 ; 
    // enable read over time , rxd down edge wakeup(made a interrupt).
    
    //UART_Enable_NegtiveEdgeWakeup(UART0);
    //enable DMA send last byte interrupt .

    UART_SetTransferBitOrder(UART0,UART_DATA_BITORDER_LSBFIRST);
    UART_SetStopBitsLength(UART0,UART_STOPBITS_1);
    UART_SetDataWidth(UART0,UART_DATAWIDTH_8B);
    UART_SetParity(UART0,UART_PARITY_NONE);
    
    // set default value :
    UART0->IER = 0 ;
    // only enable the wakeup. for receive mode start .
    //UART_EnableIT_NegtiveEdgeWakeup(UART0);
    // clear interrupt flage.
    UART0->ISR = 0xffff;
    // set send dealy and recv overtime .
    //UART0->TODR =  UartRecvOvtm ;
    //UART_Enable_ReceiveTimeOut(UART0);
    // for safe we used 9600 .
    UART0->BGR = UART0_Baud ;
    // enable ir .
    UART_COMMON->IRCR = (IRTH<<11) | IRBRG ;
    UART_Enable_InfraredModulation(UART0);
    NVIC_DisableIRQ(UART0_IRQn);
    NVIC_SetPriority(UART0_IRQn,2);//中断优先级配置
    NVIC_EnableIRQ(UART0_IRQn);     //使能NVIC   
    DisablePeriph(Clock_UART0);    
}




// do fix data send . it enable recv , and send data sout 
void Uart0DoIr(void)
{
    uint8_t  temp = 255;
    // enable the 3.3V  out .
    EnPowerOut();
    // delay a short time for the ir receiver to power up .
    while(temp >0)
    {
        __NOP();
        __NOP();
        __NOP();    
        temp -- ;
    }
    GPIO_DisablePinOpendrain(UART_TX_PORT,UART_RX_PIN|UART_TX_PIN);
    GPIO_SetPinMode(UART_RX_PORT,UART_RX_PIN,GPIO_MODE_DIGITAL);
    GPIO_SetPinMode(UART_TX_PORT,UART_TX_PIN,GPIO_MODE_DIGITAL);
    GPIO_EnablePinPullup(UART_TX_PORT,UART_RX_PIN); 
    EnablePeriph(Clock_UART0);
    // enbale ir clock
    //RCC->PCLKCR3 |= 1<<14 ;
    // send one byte ,try to receive it .
    UART0->TXBUF =0xff;
    UART0->ISR = 0xFFFF ;
    UART0->IER  = UART_IER_RXTOIE | UART_IER_RXBFIE ;
    SetRegBit(UART0->CSR, UART_CSR_RXEN_Msk|UART_CSR_TXEN_Msk);//|UART_CSR_RXTOEN_Msk); 
    
    // start a  detect over time softtimer.
    // get the index of a softtimer , it can be used to start and stop this timer 
    temp = GetFreeSoftTimer();
    SetSoftTimerDelayMs(temp,20);
    SoftTimerBlockAndOpt(temp,BlcId_None ,SftOpt_Quick);
    SetSoftTimerEvt(temp, Sig_Ir_OVT);
    StartSoftTimer(temp);    
}
void Uart0Stop(void)
{
    UART0->IER = 0 ; 
    ClrRegBit(UART0->CSR, UART_CSR_RXEN_Msk|UART_CSR_TXEN_Msk); 
    // enbale ir clock
    RCC->PCLKCR3 &= (~(1<<14)) ;    
    StopPeriph(Clock_UART0);       
    GPIO_EnablePinOpendrain(UART_TX_PORT,UART_RX_PIN|UART_TX_PIN);
    GPIO_DisablePinPullup(UART_TX_PORT,UART_RX_PIN|UART_TX_PIN); 
    GPIO_SetOutputPin(UART_TX_PORT,UART_RX_PIN|UART_TX_PIN);    
    GPIO_SetPinMode(UART_RX_PORT,UART_RX_PIN,GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(UART_TX_PORT,UART_TX_PIN,GPIO_MODE_OUTPUT);  
}
void UART0_IRQHandler(void)
{
    uint32_t res = UART0->ISR ;
    // clear all interrupt flage but not receive error flage . it will be read when all data received  in dma interrupt.
    UART0->ISR  = 0xffff ; 

    if((res & UART_ISR_RXBF) && (UART0->IER&UART_IER_RXBFIE))
    {
        if(UART0->RXBUF == 0xff)
        {
            postevt((LPThdBlock)&(halmaster.super),Sig_Ir_Recv_Right); 
        }
        else
        {
            postevt((LPThdBlock)&(halmaster.super),Sig_Ir_Recv_Error);
        }           
        Uart0Stop(); 
        DisPowerOut();        
    }

 }
 
void InitHall(void)
{
    // hal1 and hal2 power pin has same port.
    GPIO_DisablePinOpendrain(HAL2_EN_PORT,HAL2_EN_PIN|HAL1_EN_PIN);
    GPIO_SetPinMode(HAL1_EN_PORT,HAL1_EN_PIN,GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(HAL2_EN_PORT,HAL2_EN_PIN,GPIO_MODE_OUTPUT);
    GPIO_ResetOutputPin(HAL2_EN_PORT,HAL2_EN_PIN|HAL1_EN_PIN);

    // hal1 input and hal 2  input has pulled up.. 
    GPIO_SetPinMode(HAL1_IN_PORT,HAL1_IN_PIN,GPIO_MODE_INPUT);
    GPIO_EnablePinInput(HAL1_IN_PORT,HAL1_IN_PIN );
    GPIO_SetPinMode(HAL2_IN_PORT,HAL2_IN_PIN,GPIO_MODE_INPUT);
    GPIO_EnablePinInput(HAL2_IN_PORT,HAL2_IN_PIN );

    // set HAL1  PA0 exti sel
    
    // set input edge.
    EXTI_SetTrigEdge(GPIO_COMMON,GPIO_EXTI_LINE_0,GPIO_EXTI_TRIGGER_BOTHEDGE);
    // enable filter
    EXTI_EnableDigitalFilter(GPIO_COMMON,GPIO_EXTI_LINE_0);
    // set exti pin , set Hal_1_Pin as input pin .
    GPIO_SetExitLine0(GPIO_COMMON,GPIO_EXTI_LINE_0_PA0);
    // clear exti flage
    EXTI_ClearFlag(GPIO_COMMON,GPIO_EXTI_LINE_0);

    // set HAL2  PB3 exti sel
    HallTimestamp1 = RTC->SBSCNT ;
    HallTimestamp2 = RTC->SBSCNT ;
    // set input edge.
    EXTI_SetTrigEdge(GPIO_COMMON,GPIO_EXTI_LINE_4,GPIO_EXTI_TRIGGER_BOTHEDGE);
    // enable filter
    EXTI_EnableDigitalFilter(GPIO_COMMON,GPIO_EXTI_LINE_4);
    // set exti pin , set Hal_2_Pin as input pin .
    GPIO_SetExitLine4(GPIO_COMMON,GPIO_EXTI_LINE_4_PB3);
    // clear exti flage
    EXTI_ClearFlag(GPIO_COMMON,GPIO_EXTI_LINE_4);
}




void InitAdc(void)
{

	//配置引脚为模拟功能
	// ADC ADC_11 引脚 PC6  , slowly mode  pin . 
    // VBAT_ADC_PORT  GPIOC
    // VBAT_ADC_PIN   GPIO_PIN_6
	GPIO_DisablePinOpendrain(VBAT_ADC_PORT,VBAT_ADC_PIN);
    GPIO_EnablePinAnalog(VBAT_ADC_PORT,VBAT_ADC_PIN);
    GPIO_SetPinMode(VBAT_ADC_PORT,VBAT_ADC_PIN,GPIO_MODE_ANALOG);

    // ADC ADC_2 引脚 PD11  ,    quckly mode pin .
    // ISEN2_ADC_PORT  GPIOD  
    // ISEN2_ADC_PIN   GPIO_PIN_11   
	GPIO_DisablePinOpendrain(ISEN2_ADC_PORT,ISEN2_ADC_PIN);
    GPIO_EnablePinAnalog(ISEN2_ADC_PORT,ISEN2_ADC_PIN);
    GPIO_SetPinMode(ISEN2_ADC_PORT,ISEN2_ADC_PIN,GPIO_MODE_ANALOG);    

    // ADC ADC_3 引脚 PD0  ,    quckly mode pin .    
    //  ISEN1_ADC_PORT  GPIOD  quckly mode pin .
    //  ISEN1_ADC_PIN   GPIO_PIN_0  
	GPIO_DisablePinOpendrain(ISEN1_ADC_PORT,ISEN1_ADC_PIN);
    GPIO_EnablePinAnalog(ISEN1_ADC_PORT,ISEN1_ADC_PIN);
    GPIO_SetPinMode(ISEN1_ADC_PORT,ISEN1_ADC_PIN,GPIO_MODE_ANALOG); 

    // ADC ADC_4 引脚 PD1  ,    quckly mode pin .
    // ISEN3_ADC_PORT  GPIOD    quckly mode pin .
    // ISEN3_ADC_PIN   GPIO_PIN_1 
	GPIO_DisablePinOpendrain(ISEN3_ADC_PORT,ISEN3_ADC_PIN);
    GPIO_EnablePinAnalog(ISEN3_ADC_PORT,ISEN3_ADC_PIN);
    GPIO_SetPinMode(ISEN3_ADC_PORT,ISEN3_ADC_PIN,GPIO_MODE_ANALOG);     


    //ADC 时钟设置   :  RCHF   16M   2分频   ->  8M 
    EnablePeriph(Clock_ADC);
	
	//ADC 寄存器设置
    ADC->CR = 0; // stop adc . normal mode  .
    ADC->CFGR =  (2<<20) |(1<<17) |(1<<16) |(1<<10) | 1;  // moniter all out  channel [2,3,4,11] ,Result >>2 , OverSampling for 4 .  enable OverSampeling ,Wait mode , Enable DMA
    ADC->CHER = 0  ;  // select 0 channel  for default .  it will be set in StartAdc
    ADC->HLTR = (Adc_AWD_Limit_High <<16 ) | Adc_AWD_Limit_Low ;
    ADC->ISR = 0xffff  ;   // clear all 
    ADC->IER = (1<<6) ;  // enable  AWD High interrupt .
    // the ADC Sample time is calced as  9.704061 *(1000 + Rin)*12.8 ps  ADC clock is  1/8M s per clock : 125ns
    ADC->SMTR = (9<<4) |  3 ;  // set low speed prepare time 10us = 80 used 9(128clock)  the quicky speed prepare time 250ns = 2clock  used 3 (10 AdcClock) , 
    DisablePeriph(Clock_ADC);
    AdcCfg.ChanSel =  0;
    //VREF->CR = 3 ;
    NVIC_DisableIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn,2);//中断优先级配置
    NVIC_EnableIRQ(ADC_IRQn);     //使
}
// input the channel  group . the channel sel and the ADCenable will be disabled when mcu deal the adc result .
// return 0 if there is already a adc is running ,or no channel selected.
uint8_t  AdcStart(uint16_t chan)
{
    uint32_t  sel = 0 ;
    if(AdcCfg.ChanSel)
        return 0 ;
    chan &= (ADC_Chan2|ADC_Chan3 |ADC_Chan4 |ADC_Chan11 | ADC_ChanTemp |ADC_ChanVRef |ADC_ChanOp1);
    if(chan == 0 )
        return 0 ;
    EnablePeriph(Clock_ADC);
    EnablePeriph(Clock_DMA);
    DMA->CH0CR = 0 ;
    AdcCfg.ChanSel = chan ;
    sel = chan & (ADC_ChanTemp |ADC_ChanVRef |ADC_ChanOp1);
    sel <<=4 ;
    sel |= (chan&0x0fff);
    if(chan & (ADC_Chan2|ADC_Chan3 |ADC_Chan4 ))
        ADC->CFGR =  (1<<24) | (2<<20) |(1<<17) |(1<<16) |(1<<10) | 1;  // moniter all out  channel [2,3,4,11] ,Result >>2 , OverSampling for 4 .  enable OverSampeling ,Wait mode , Enable DMA
    else
        ADC->CFGR =  (2<<20) |(1<<17) |(1<<16) |(1<<10) | 1;  // moniter all out  channel [2,3,4,11] ,Result >>2 , OverSampling for 4 .  enable OverSampeling ,Wait mode , Enable DMA
    ADC->ISR = 0xffff;//清标志	    
    ADC->CHER = sel ;
    if(chan & ADC_ChanTemp) 
    {
        // open the Vtemp. and Vref 
        VREF->CR = 3; 
        VREF->BUFCR = ((1<<2) |(1<<0)) ;
    }    
    else if(chan & ADC_ChanVRef) 
    { // Vref only
        VREF->CR =1; 
        VREF->BUFCR = (1<<0);
    }
    if(chan & (ADC_ChanTemp|ADC_ChanVRef)) 
    { // should wait the Vref stable .
        while((VREF->SR &2) == 0) __nop();
        // delay a little time , do not use the timer set delay time , to stop current CPU work,let current stable
        for (sel = 0 ; sel <0xfff ; sel++ )        
        {
               __NOP();
        }        
    }
    
    if(chan & ADC_Chan11) 
    { // vbat need to open the motor power .
        EnMotorPower();
    }    
    
    // set dma channel 0 .    
    // calc chan bit = 1 count 
    for (sel = 0 ; chan > 0 ; chan &= (chan-1) )        
    {
           sel++;
    }
    sel -- ;
    sel <<= 16 ;
    sel |= DMA_CHAN0_CFG;
    DMA->CH0MAD = (uint32_t)&AdcCfg.Data[0];
    DMA->CH0CR = sel ;
    // clear isr flage for channel 4 .
    DMA->ISR = (1<<0 |1<<8) ;
    DMA->GCR = 1 ;
     // start ADC . 

    ADC->CR = 1 ; 
    // start change .
    ADC->CR = 3 ; 
    return 1;
}
void ADC_IRQHandler(void)
{
    uint32_t res = ADC->ISR ;
    ADC->ISR  = res ; 
    res &=  ADC->IER ;
    if(res & (1<<6))
    { // high limit interrupt detect .
    
    
    }
    if(res & (1<<5))
    { // low limit interrupt detect .
    
    
    }
}


void InitDma(void)
{// if use the DMA the DMA clock will not be stoped, bcs we test find , the from the DMA clock 
    EnablePeriph(Clock_DMA);
    DMA->GCR = 0 ;
    DMA->CH0CR = 0 ;
    DMA->CH1CR = 0 ;
    DMA->CH2CR = 0 ;
    DMA->CH3CR = 0 ;
    DMA->CH4CR = 0 ;
    DMA->CH5CR = 0 ;
    DMA->CH6CR = 0 ;
    DMA->CH7CR = 0 ;
    DisablePeriph(Clock_DMA);   
    
    NVIC_DisableIRQ(DMA_IRQn);
    NVIC_SetPriority(DMA_IRQn,0);//中断优先级配置
    NVIC_EnableIRQ(DMA_IRQn);     //使    
}


void DMA_IRQHandler(void)
{
    uint32_t res = DMA->ISR ;
    DMA->ISR  = res ; 
    if(res & (1<<8))
    { // Channel 0 complete 
        // clear the channel sel .
        // stop vtemp and vref/Vtemp buf.
        VREF->CR = 0 ; 
        VREF->BUFCR = 0;
        // stop DMA channel 0 and adc .
        ADC->CR = 0 ;
        DMA->CH0CR = 0 ;
        DisablePeriph(Clock_DMA);        
        DisablePeriph(Clock_ADC);
        postevt((LPThdBlock)&(halmaster.super),Sig_Adc_Done);
    }
 
}
uint16_t  GetAdcResult(uint8_t chan)
{
    return AdcCfg.Result[chan];
}
uint16_t GetAdcValue(uint8_t i)
{
    return AdcCfg.Data[i];
}
void  CalcAdcResult(void)
{
    // Vchannel = ADC_VREFValue * VchanneData*3000/(VRefData ×4095) =  (ADC_VREFValue * VchanneData*3000/ VRefData) >>12 = ((ADC_VREFValue *3000)*  VchanneData / VRefData) >>12 =  (((ADC_VREFValue *3000)/ VRefData )   *  VchanneData >>12) 
    //   (((ADC_VREFValue *3000)/ VRefData   is the VDD value in mv ,  a small value , we hold it at AdcCfg.Result[chan]
    // or  use the VDD =3.3  to quickly get result : (AdcCfg.Data[res2]*3300)>>12 ;
    QuotRemain qr;
    uint8_t  res2 ;
    //find the index for current chan

    // if ADC_ChanVRef is checked  , use it to calc others to get precise result .
    if(AdcCfg.ChanSel & ADC_ChanVRef)
    { // calc (((ADC_VREFValue *3)/ VRefData
       res2 = CaclBitCnt16(AdcCfg.ChanSel & ADC_ChanVRefOffsetMask) ;
       qr.quotient = ADC_VREFValue*3000 ;
       qr.remainder = AdcCfg.Data[res2]  ;  
       HardDiv(&qr); 
       qr.remainder <<=1  ;
       if(qr.remainder > AdcCfg.Data[res2])
           qr.quotient ++ ;
       AdcCfg.Result[IndVRef]= (uint16_t)qr.quotient ;        
    }
    else
       AdcCfg.Result[IndVRef]= 3300 ;
    
    res2 = 0 ;
    if(AdcCfg.ChanSel & ADC_Chan2)
    {
       qr.quotient = AdcCfg.Data[res2]*AdcCfg.Result[IndVRef] ;
       AdcCfg.Result[IndISen2]= qr.quotient>>12 ;
       res2++; 
    }
    if(AdcCfg.ChanSel & ADC_Chan3)
    {
       qr.quotient = AdcCfg.Data[res2]*AdcCfg.Result[IndVRef] ;
       AdcCfg.Result[IndISen1]= qr.quotient>>12 ;        
       res2++; 
    }
    if(AdcCfg.ChanSel & ADC_Chan4)
    {
       qr.quotient = AdcCfg.Data[res2]*AdcCfg.Result[IndVRef] ;
       AdcCfg.Result[IndISen3]= qr.quotient>>12 ;        
        
       res2++; 
    }
    if(AdcCfg.ChanSel & ADC_Chan11)
    {
       // stop power
       qr.quotient = MotorCtrl[0].Mode | MotorCtrl[1].Mode | MotorCtrl[2].Mode ;
       if((qr.quotient & 0xf0) ==0)
           DisMotorPower();     
       // the  real value is divide as 33/84 .
       qr.quotient = (AdcCfg.Data[res2]*AdcCfg.Result[IndVRef])>>12 ;
       qr.quotient *=840 ; 
       qr.remainder = 33 ; 
       HardDiv(&qr);
       if(qr.remainder > 16)     
           qr.quotient++ ;
       // result is stored voltage based on 0.001V  .
       AdcCfg.Result[IndVbat] = (uint16_t)qr.quotient;
       res2++; 
    }
    if(AdcCfg.ChanSel & ADC_ChanTemp)
    { //1 : Vtemp = AdcCfg.Data[res2]*AdcCfg.Result[IndVRef] /4095 (in mv).    2 : Temp = (Vtemp-VCal30)/Scolpe +30 ,VCal = VFlash*3.0/4095, 3: Scolpe =3.06
      //so :Temp =  (AdcCfg.Data[res2]*AdcCfg.Result[IndVRef] - VFlash*3000) /4095/3.06  +30 .
      //         =  ((AdcCfg.Data[res2]*AdcCfg.Result[IndVRef]/(4096) - VFlash*3000/(4095))/3.06  +30 . 
       qr.quotient = AdcCfg.Data[res2]*AdcCfg.Result[IndVRef] ;
       AdcCfg.Result[IndTSens] =  (uint16_t)(qr.quotient>>12);
       qr.quotient = ADC_TempValue *3000; 
       qr.remainder =  (uint16_t)(qr.quotient >>12);
       if(AdcCfg.Result[IndTSens]  > qr.remainder)
       {   // biger than 30 .  
           qr.quotient = (AdcCfg.Result[IndTSens] - qr.remainder)*1000 ;  // chang to 10 times /306
           qr.remainder = 306 ;
           HardDiv(&qr);
           if(qr.remainder > 153)
               qr.quotient ++ ;
           AdcCfg.Result[IndTSens] = (uint16_t)qr.quotient +300;
       }
       else
       {   // small than 30
           qr.quotient = (qr.remainder - AdcCfg.Result[IndTSens])*1000 ;
           qr.remainder = 306 ;
           HardDiv(&qr);
           if(qr.remainder > 153)
               qr.quotient ++ ;           
           AdcCfg.Result[IndTSens] = 300 -(uint16_t)qr.quotient ;
       }
       RtcTempAdjust(AdcCfg.Result[IndTSens]);
       res2++; 
    }
    if(AdcCfg.ChanSel & ADC_ChanVRef)
    { 
       res2++; 
    }
    if(AdcCfg.ChanSel & ADC_ChanOp1)
    {
       qr.quotient = AdcCfg.Data[res2]*AdcCfg.Result[IndVRef] ;
       AdcCfg.Result[IndOpa1]= qr.quotient>>12 ;         
    }
    
    //clear flage . enable to next adc proc .
    AdcCfg.ChanSel = 0 ;

}

void InitMotor(void)
{
    uint8_t temp ;
    uint8_t * lp ;
    lp =(uint8_t* )&MotorCtrl;
    for (temp = 0 ; temp <sizeof(MotorData) ;temp++)
    {
        *lp = 0 ;
        lp++ ;
    }
    // init the Vref out gpio
    GPIO_SetPinMode(VREF_EN_PORT,VREF_EN_PIN,GPIO_MODE_OUTPUT); 
    DisVRefOut();
    
    // init the Motor power ctrol gpio
    GPIO_DisablePinOpendrain(MDPWR_CTRL_PORT,MDPWR_CTRL_PIN);
    GPIO_SetPinMode(MDPWR_CTRL_PORT,MDPWR_CTRL_PIN,GPIO_MODE_OUTPUT); 
    DisMotorPower() ;

    // init the Motor 1 PWM+ and PWM- Signal .
    // it used the GPT1  CH1 PC0 and Ch2  PC1  
    GPIO_DisablePinOpendrain(PWM1_IN1_PORT,PWM1_IN1_PIN|PWM1_IN2_PIN);
    GPIO_SetPinMode(PWM1_IN1_PORT,PWM1_IN1_PIN,GPIO_MODE_OUTPUT);   
    GPIO_SetPinMode(PWM1_IN2_PORT,PWM1_IN2_PIN,GPIO_MODE_OUTPUT); 
    // set the default output as 0 , 
    GPIOC->DRST =   PWM1_IN1_PIN|PWM1_IN2_PIN ;   
    // init the Motor 2 PWM+ and PWM- Signal .
    // it used the GPT0 Ch1 PB10 and ATIM1  CH2  PB5
    GPIO_DisablePinOpendrain(PWM2_IN1_PORT,PWM2_IN1_PIN|PWM2_IN2_PIN);
    GPIO_SetPinMode(PWM2_IN1_PORT,PWM2_IN1_PIN,GPIO_MODE_OUTPUT);   
    GPIO_SetPinMode(PWM2_IN2_PORT,PWM2_IN2_PIN,GPIO_MODE_OUTPUT); 
    GPIOB->DRST = PWM2_IN1_PIN|PWM2_IN2_PIN;
    
    // specail set for PB10 .
    GPIOB->DFS = (1<<10);
    // init the Motor 2 PWM+ and PWM- Signal .
    // it used the ATIM1  CH1 PB4 and GPT1 Ch4 PA5
    GPIO_DisablePinOpendrain(PWM3_IN1_PORT,PWM3_IN1_PIN);
    GPIO_DisablePinOpendrain(PWM3_IN2_PORT,PWM3_IN2_PIN);
    GPIO_SetPinMode(PWM3_IN1_PORT,PWM3_IN1_PIN,GPIO_MODE_OUTPUT);   
    GPIO_SetPinMode(PWM3_IN2_PORT,PWM3_IN2_PIN,GPIO_MODE_OUTPUT);  
    GPIOB->DRST = PWM3_IN1_PIN ;
    GPIOA->DRST = PWM3_IN2_PIN ;
    // config content : 1 GPT1 CH1 and CH2 CH4 , GPT0 Ch1 ,   ATM1 CH1 and CH2  output PWM 40K .
    //                  2 GPT1 CH1 and CH2 CH4 , GPT0 Ch1 ,   ATM1 CH1 and CH2  CCR regist changed by DMA , to speedup and SpeedDown .
    //                  3 GPT0,1, ATIM  source by RCHF , 16Mhz.  div by 4 = 4M  chang frequncy set as 40K , 4M /40K  = 100
    //
    
    // ATIM init part .
 
    EnablePeriph(Clock_ATIM);
    // enable  ARR preload . set URS 1 .
    ATIM->CR1 = (1<<7) ;
    // UEV active DMA ,
    //ATIM->CR2 = (1<<3)  ;
    // set repeat times 0 .
    ATIM->RCR = 0 ;
    //set frenquncy 
    ATIM->ARR = PWMPERIOD;
    //set prescale 
    ATIM->PSC = PWMDIV;
    ATIM_GenerateEvent_UPDATE(ATIM);

    ATIM->CCER  = 0x00 ; // disable all channel enable  CcxE = 0. set ch1 and ch2 active mode output high. CcxP =0

    //Bit[0-2 ] = 0  , set chan 1 output , bit2=0 : do't care input trig ,  bit3=1 enable CCR preload. bit6:4 = 110 pwm1 mode bit7=0 : 
    ATIM->CCMR1 = 0x68 | 0x6800 ; 
    // clear the interrupt flage.
    ATIM->ISR = 0xffff ;

    ATIM->CCR1 = 0 ; // set no active for default
    ATIM->CCR2 = 0 ; // set no active for default
    
    // bit10 :0  OSSI: MOE = 0 OC is not output  .  need to test . bit11:0 OSSR: MOE =1  CCE =0 , OC is not output.
    ATIM->BDTR = 0 ; // disable channel output .
    // Disable interrupt .
    ATIM->DIER = 0 ;
    DisablePeriph(Clock_ATIM); 

    // gptim1 init  , use the GPT1  CH1 PC0 and Ch2  PC1   and Ch4 for PWM out.
    EnablePeriph(Clock_GTIM1);
    // enable  ARR preload . set URS 1 .
    GPTIM1->CR1 = (1<<7) ;
    // no dma enabled , no interrupt enable .
    GPTIM1->CR2 = 0  ;
    GPTIM1->DIER = 0 ;
    // enable ch1 CCR preload, ch1 pwm1 mode  . ch2 CCR preload, ch4 pwm1 mode    
    GPTIM1->CCMR1 = 0x68 | 0x6800 ;
    GPTIM1->CCMR2 = 0x6800 ;
    //disable all out , and set default is high
    GPTIM1->CCER = 0 ;
    
    GPTIM1->CCR1 = 0 ;
    GPTIM1->CCR2 = 0 ;
    GPTIM1->CCR4 = 0 ;
    //set frenquncy 
    GPTIM1->ARR = PWMPERIOD;
    //set prescale 
    GPTIM1->PSC = PWMDIV;    
    DisablePeriph(Clock_GTIM1);
   
    // gptim0 init  , use the GPT1  CH1 PC0 and Ch2  PC1  for DMA[5] PWM out, and Ch4 for PWM out.
    EnablePeriph(Clock_GTIM0);
    // enable  ARR preload . set URS 1 .
    GPTIM0->CR1 = (1<<7) ;
    // no dma enabled , no interrupt enable .
    GPTIM0->CR2 = 0  ;
    GPTIM0->DIER = 0 ;
    // enable ch1 CCR preload, ch1 pwm1 mode  . ch2 CCR preload, ch2 pwm1 mode    
    GPTIM0->CCMR1 = (1<<3)|0x60 ;
    //disable all out , and set default is high
    GPTIM0->CCER = 0 ;
    
    GPTIM0->CCR1 = 0 ;
    //set frenquncy 
    GPTIM0->ARR = PWMPERIOD;
    //set prescale 
    GPTIM0->PSC = PWMDIV;    
    DisablePeriph(Clock_GTIM0);
   
}
__STATIC_INLINE void SetMotor1Forward(void)
{
    if(CheckPeriph(Clock_GTIM1)== 0 )
        EnablePeriph(Clock_GTIM1);
    // set gpio mode
    GPIOC->DRST = PWM1_IN2_PIN |PWM1_IN1_PIN  ;
    GPIO_SetPinMode(PWM1_IN2_PORT,PWM1_IN2_PIN,GPIO_MODE_OUTPUT);  
    //GPIO_SetPinMode(PWM3_IN2_PORT,PWM3_IN2_PIN,GPIO_MODE_DIGITAL);      
    // set duty
    GPTIM1->CCR1 = MotorCtrl[0].CurRatio ;
    // set load duty .
    // GPTIM1->EGR = 1 ;
    // enable output 
    GPTIM1->CCER &= 0xff00;
    GPTIM1->CCER |= 1<<0  ;  //1<<4   1<<12  active output high.
    // enable  running 
    GPTIM1->CR1 = (1<<7)|(1<<0) ;    
    GPIO_SetPinMode(PWM1_IN1_PORT,PWM1_IN1_PIN,GPIO_MODE_DIGITAL); 
}
   
__STATIC_INLINE void SetMotor1Backward(void)
{
    if(CheckPeriph(Clock_GTIM1)== 0 )
        EnablePeriph(Clock_GTIM1);
    // set gpio mode
    GPIOC->DRST = PWM1_IN2_PIN |PWM1_IN1_PIN  ;
    GPIO_SetPinMode(PWM1_IN1_PORT,PWM1_IN1_PIN,GPIO_MODE_OUTPUT);  
    //GPIO_SetPinMode(PWM3_IN2_PORT,PWM3_IN2_PIN,GPIO_MODE_DIGITAL);      
    // set duty
    GPTIM1->CCR2 = MotorCtrl[0].CurRatio  ;
    // set load duty .
    // GPTIM1->EGR = 1 ;
    // enable output 
    GPTIM1->CCER &= 0xff00;
    GPTIM1->CCER |= (1<<4)  ;  //1<<4   1<<12  active output high.
    // enable  running 
    GPTIM1->CR1 = (1<<7)|(1<<0) ;    
    GPIO_SetPinMode(PWM1_IN2_PORT,PWM1_IN2_PIN,GPIO_MODE_DIGITAL); 

}

__STATIC_INLINE void SetMotor1Stop(uint8_t isbrake)
{
    if(isbrake)
        GPIOC->DSET =   PWM1_IN1_PIN|PWM1_IN2_PIN ; 
    else
        GPIOC->DRST =   PWM1_IN1_PIN|PWM1_IN2_PIN ;
    GPIO_SetPinMode(PWM1_IN1_PORT,PWM1_IN1_PIN,GPIO_MODE_OUTPUT);   
    GPIO_SetPinMode(PWM1_IN2_PORT,PWM1_IN2_PIN,GPIO_MODE_OUTPUT);                     
    // try to close the gptim1    
    GPTIM1->CCER &= (1<<12) ; 
    if( GPTIM1->CCER == 0)
    {
        GPTIM1->CR1 =0 ;
        DisablePeriph(Clock_GTIM1);
    }                
}
__STATIC_INLINE void SetMotor2Forward(void)
{
    if(CheckPeriph(Clock_GTIM0)== 0 )
        EnablePeriph(Clock_GTIM0);                            
    GPIOB->DRST = PWM2_IN2_PIN |PWM2_IN1_PIN;
    GPIO_SetPinMode(PWM2_IN2_PORT,PWM2_IN2_PIN,GPIO_MODE_OUTPUT);         
    GPTIM0->CCR1 = MotorCtrl[1].CurRatio  ;
    GPTIM0->CCER = 1<<0 ;
    GPTIM0->CR1 = (1<<7)|(1<<0) ;
    GPIO_SetPinMode(PWM2_IN1_PORT,PWM2_IN1_PIN,GPIO_MODE_DIGITAL); 
}
__STATIC_INLINE void SetMotor2Backward(void)
{
    if(CheckPeriph(Clock_ATIM)== 0 )
        EnablePeriph(Clock_ATIM);   
    GPIOB->DRST = PWM2_IN2_PIN |PWM2_IN1_PIN;    
    GPIO_SetPinMode(PWM2_IN1_PORT,PWM2_IN1_PIN,GPIO_MODE_OUTPUT);
    ATIM->CCR2 = MotorCtrl[1].CurRatio ;
    ATIM->CCER |= (1<<4) ;
    ATIM->CR1 = (1<<7)|1 ;
    ATIM->BDTR = 1<<15 ;
    GPIO_SetPinMode(PWM2_IN2_PORT,PWM2_IN2_PIN,GPIO_MODE_DIGITAL); 
}
__STATIC_INLINE void SetMotor2Stop(uint8_t isbrake)
{
    if(isbrake)
        GPIOB->DSET =   PWM2_IN1_PIN|PWM2_IN2_PIN ; 
    else
        GPIOB->DRST =   PWM2_IN1_PIN|PWM2_IN2_PIN ;
    GPIO_SetPinMode(PWM2_IN1_PORT,PWM2_IN1_PIN,GPIO_MODE_OUTPUT);   
    GPIO_SetPinMode(PWM2_IN2_PORT,PWM2_IN2_PIN,GPIO_MODE_OUTPUT); 
    // try to close the gptim0    
    GPTIM0->CCER = 0 ; ; 
    GPTIM0->CR1 =0 ;
    DisablePeriph(Clock_GTIM0);
    // close atim ch2 . ch1 maybe running.
    if((ATIM->CCER & 1)== 0)
    {
        ATIM->CR1 =0 ;
        ATIM->BDTR = 0 ;
        ATIM->CCER = 0 ;
        DisablePeriph(Clock_ATIM);
    }
    else
        ATIM->CCER = 1 ;      
}
__STATIC_INLINE void SetMotor3Forward(void)
{
    if(CheckPeriph(Clock_ATIM)== 0 )
        EnablePeriph(Clock_ATIM);
    GPIOB->DRST  = PWM3_IN1_PIN;
    GPIOA->DRST  = PWM3_IN2_PIN; 
    GPIO_SetPinMode(PWM3_IN2_PORT,PWM3_IN2_PIN,GPIO_MODE_OUTPUT);
    ATIM->CCR1 = MotorCtrl[2].CurRatio  ;
    ATIM->CCER |= (1<<0)  ;
    ATIM->CR1 = (1<<7)|1 ;
    ATIM->BDTR = 1<<15 ;   
    GPIO_SetPinMode(PWM3_IN1_PORT,PWM3_IN1_PIN,GPIO_MODE_DIGITAL);   
}
__STATIC_INLINE void SetMotor3Backward(void)
{
    if(CheckPeriph(Clock_GTIM1)== 0 )
        EnablePeriph(Clock_GTIM1);   
    GPIOB->DRST  = PWM3_IN1_PIN;
    GPIOA->DRST  = PWM3_IN2_PIN;    
    GPIO_SetPinMode(PWM3_IN1_PORT,PWM3_IN1_PIN,GPIO_MODE_OUTPUT);
    GPTIM1->CCR4 = MotorCtrl[2].CurRatio  ;
    GPTIM1->CCER |= 1<<12  ;
    GPTIM1->CR1 = (1<<7)|(1<<0) ;  
    GPIO_SetPinMode(PWM3_IN2_PORT,PWM3_IN2_PIN,GPIO_MODE_DIGITAL);      
}
__STATIC_INLINE void SetMotor3Stop(uint8_t isbrake)
{
    if(isbrake)
    {
        GPIOB->DSET  = PWM3_IN1_PIN;
        GPIOA->DSET  = PWM3_IN2_PIN;  
    }
    else
    {
        GPIOB->DRST  = PWM3_IN1_PIN;
        GPIOA->DRST  = PWM3_IN2_PIN;  
    }
    GPIO_SetPinMode(PWM3_IN1_PORT,PWM3_IN1_PIN,GPIO_MODE_OUTPUT);   
    GPIO_SetPinMode(PWM3_IN2_PORT,PWM3_IN2_PIN,GPIO_MODE_OUTPUT);                     

    // try to close the Atim1 ch1 , ch2 maybe running 
    if((ATIM->CCER & (1<<4))== 0)
    {
        ATIM->CR1 =0 ;
        ATIM->BDTR = 0 ;
        ATIM->CCER = 0 ;
        DisablePeriph(Clock_ATIM);
    }
    else
        ATIM->CCER = 1<<4 ;
    // try to close gptim1 ch4 , ch1 and ch2 maybe running
    if((GPTIM1->CCER & 0xff)== 0)
    {
        GPTIM1->CR1 =0 ;
        GPTIM1->CCER = 0 ;
        DisablePeriph(Clock_GTIM1);
    }
    else
        ATIM->CCER &= 0xff ;    
}
__STATIC_INLINE void SetMotorStop(uint8_t index,uint8_t isbrake)
{
    if(index== 0)
    {
        SetMotor1Stop(isbrake);
    }
    else if(index == 1)
    {
        SetMotor2Stop(isbrake);    
    }
    else
    {
        SetMotor3Stop(isbrake);      
    }
}
__STATIC_INLINE void SetMotorFor(uint8_t index)
{
    if(index== 0)
    {
        SetMotor1Forward();
    }
    else if(index == 1)
    {
        SetMotor2Forward();    
    }
    else
    {
        SetMotor3Forward();      
    }
}
__STATIC_INLINE void SetMotorBack(uint8_t index)
{
    if(index== 0)
    {
        SetMotor1Backward();
    }
    else if(index == 1)
    {
        SetMotor2Backward();    
    }
    else
    {
        SetMotor3Backward();      
    }
}

__STATIC_INLINE void UpdateMotorFor(uint8_t index)
{
    if(index== 0)
    {
        GPTIM1->CCR1 = MotorCtrl[0].CurRatio ;
    }
    else if(index == 1)
    {
        GPTIM0->CCR1 = MotorCtrl[1].CurRatio  ;  
    }
    else
    {
        ATIM->CCR1 = MotorCtrl[2].CurRatio  ;      
    }
}
__STATIC_INLINE void UpdateMotorBack(uint8_t index)
{
    if(index== 0)
    {
        GPTIM1->CCR2 = MotorCtrl[0].CurRatio  ;;
    }
    else if(index == 1)
    {
        ATIM->CCR2 = MotorCtrl[1].CurRatio ;  
    }
    else
    {
        GPTIM1->CCR4 = MotorCtrl[2].CurRatio  ;     
    }
}
__STATIC_INLINE void MotorPostDone(uint8_t index)
{
    if(index== 0)
    {
       postevt((LPThdBlock)&(cmddealer.super),Sig_Motor1_Done);;
    }
    else if(index == 1)
    {
       postevt((LPThdBlock)&(cmddealer.super),Sig_Motor2_Done);;  
    }
    else
    {
       postevt((LPThdBlock)&(cmddealer.super),Sig_Motor3_Done);  ;     
    }
}
 
void MotorProc(uint8_t index)
{
    uint8_t mode ;
    assert(index<3);
    LPMotorData lpmotor = &MotorCtrl[index];
    // get wish mode .
    mode = (lpmotor->Mode >>4)&0x0f ;
    switch(mode)
    {
        case Motor_PowerOffFree :
        {   // get current mode.
            mode = lpmotor->Mode&0x0f ;
            switch(mode)
            {
                case Motor_PowerOff :
                {
                    lpmotor->WishRatio= 0 ;
                    lpmotor->Mode = 0 ;
                    MotorPostDone(index);
                    break;
                }
                case Motor_WakeUp :
                {
                    SetMotorStop(index,0);
                    lpmotor->Mode = (Motor_PowerOffFree<<4 ) | Motor_PowerOff;
                    break;
                }
                case Motor_ForChang :
                case Motor_ForStable :
                case Motor_ForBrake :    
                {
                    if(lpmotor->CurRatio > FreeStep)
                    {
                        lpmotor->CurRatio -= FreeStep ;
                        lpmotor->Mode = (Motor_PowerOffFree<<4)|Motor_ForFree ;
                    }
                    else
                    {
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_PowerOffFree<<4)|Motor_PowerOff ;
                    }
                    SetMotorStop(index,0);
                    break;
                }
                case Motor_BackStable :
                case Motor_BackChang :
                case Motor_BackBrake  :  
                {
                    if(lpmotor->CurRatio > FreeStep)
                    {
                        lpmotor->CurRatio -= FreeStep ;
                        lpmotor->Mode = (Motor_PowerOffFree<<4)|Motor_BackFree ;
                    }
                    else
                    {
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_PowerOffFree<<4)|Motor_PowerOff ;
                    }
                    SetMotorStop(index,0);    
                    break;
                }
                case Motor_ForFree :
                case Motor_BackFree :                    
                {
                    if(lpmotor->CurRatio > FreeStep)
                    {
                        lpmotor->CurRatio -= FreeStep ;
                    }
                    else
                    {
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_PowerOffFree<<4)|Motor_PowerOff ;
                    }
                    break;                
                }
             }
            break;            
        }
        case Motor_PowerOffBrake :
        {
            mode = lpmotor->Mode&0x0f ;
            switch(mode)
            {
                case Motor_PowerOff :
                {
                    lpmotor->WishRatio= 0 ;
                    lpmotor->Mode = 0 ;
                    MotorPostDone(index);
                    break;
                }
                case Motor_WakeUp :
                {
                    SetMotorStop(index,0);
                    lpmotor->Mode = (Motor_PowerOffBrake<<4 ) | Motor_PowerOff;
                    break;
                }
                case Motor_ForChang :
                case Motor_ForStable :
                case Motor_ForFree :    
                {
                    if(lpmotor->CurRatio > BrakeStep)
                    {
                        lpmotor->CurRatio -= BrakeStep ;
                        lpmotor->Mode = (Motor_PowerOffBrake<<4)|Motor_ForBrake ;
                        SetMotorStop(index,1);
                    }
                    else
                    {
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_PowerOffBrake<<4)|Motor_PowerOff ;
                        SetMotorStop(index,0);    
                    }
                    break;
                }
                case Motor_BackStable :
                case Motor_BackChang :
                case Motor_BackFree  :  
                {
                    if(lpmotor->CurRatio > BrakeStep)
                    {
                        lpmotor->CurRatio -= BrakeStep ;
                        lpmotor->Mode = (Motor_PowerOffBrake<<4)|Motor_BackBrake ;
                        SetMotorStop(index,1);
                    }
                    else
                    {
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_PowerOffBrake<<4)|Motor_PowerOff ;
                        SetMotorStop(index,0);    
                    }
                    break;
                }
                case Motor_ForBrake :
                case Motor_BackBrake :                    
                {
                    if(lpmotor->CurRatio > BrakeStep)
                    {
                        lpmotor->CurRatio -= BrakeStep ;
                    }
                    else
                    {
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_PowerOffBrake<<4)|Motor_PowerOff ;
                        SetMotorStop(index,0);    
                    }
                    break;                
                }
             }
            break;   
        }
        case Motor_ForStable :
        {
            mode = lpmotor->Mode&0x0f ;
            switch(mode)
            {
                case Motor_PowerOff:
                { // wakeup motor , the motor need to high in,for 20us 
                    lpmotor->Mode = (Motor_ForStable <<4)|Motor_WakeUp ;
                    SetMotorStop(index,1);
                    break;
                }
                case Motor_WakeUp :
                {// enable from MinPWMStep
                    if(lpmotor->WishRatio >BestPWMDuty)
                        lpmotor->CurRatio = BestPWMDuty ;
                    else
                        lpmotor->CurRatio = lpmotor->WishRatio ;
                    lpmotor->Mode = (Motor_ForStable <<4)|Motor_ForChang ;
                    SetMotorFor(index);
                    break;
                }
                
                case Motor_BackStable :
                case Motor_BackChang :
                case Motor_BackFree :                      
                {// go brake first . BrakeStep
                    if(lpmotor->CurRatio < BrakeStep)
                    {// slowly . to go for next time
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_ForStable <<4)|Motor_WakeUp ;
                    }
                    else
                    {
                        lpmotor->CurRatio -= BrakeStep ;
                        lpmotor->Mode = (Motor_ForStable <<4)|Motor_BackBrake ;    
                    }
                    SetMotorStop(index,1);
                    break;
                }
                case Motor_ForStable :    // maybe deffer ratio is set .  check it .             
                case Motor_ForChang :
                {
                    lpmotor->Mode = (Motor_ForStable<<4)|Motor_ForChang;
                    if(lpmotor->CurRatio < (lpmotor->WishRatio - lpmotor->Step ))
                        lpmotor->CurRatio += lpmotor->Step;
                    else if(lpmotor->CurRatio > (lpmotor->WishRatio + lpmotor->Step ))
                        lpmotor->CurRatio -= lpmotor->Step;
                    else
                    {
                        lpmotor->CurRatio = lpmotor->WishRatio;
                        lpmotor->Mode = Motor_ForStable ;  // do not need gointo  MotorProc
                        MotorPostDone(index);
                    }
                    UpdateMotorFor(index);
                    break;
                }
                case Motor_ForFree :
                case Motor_ForBrake :
                {
                    if(lpmotor->CurRatio < (lpmotor->WishRatio - lpmotor->Step ))
                    {
                        lpmotor->CurRatio += MinPWMDuty ;
                    }
                    else if(lpmotor->CurRatio > (lpmotor->WishRatio + lpmotor->Step ))
                    {
                        lpmotor->CurRatio -= MinPWMDuty ;
                    }
                    lpmotor->Mode = (Motor_ForStable <<4)|Motor_ForChang ;
                    SetMotorFor(index);
                    break;
                }
              
                case Motor_BackBrake :
                {
                    if(lpmotor->CurRatio < BrakeStep)
                    {// slowly . to go for next time
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_ForStable <<4)|Motor_WakeUp ;
                    }
                    else
                    {
                        lpmotor->CurRatio -= BrakeStep ;
                    }                
                    break;
                }
            }
            break;            
        }
        case Motor_BackStable :
        {
            mode = lpmotor->Mode&0x0f ;
            switch(mode)
            {
                case Motor_PowerOff:
                { // wakeup motor , the motor need to high in,for 20us 
                    lpmotor->Mode = (Motor_BackStable <<4)|Motor_WakeUp ;
                    SetMotorStop(index,1);
                    break;
                }
                case Motor_WakeUp :
                {// enable from MinPWMStep
                    if(lpmotor->WishRatio >BestPWMDuty)
                        lpmotor->CurRatio = BestPWMDuty ;
                    else
                        lpmotor->CurRatio = lpmotor->WishRatio ;
                    lpmotor->Mode = (Motor_BackStable <<4)|Motor_BackChang ;
                    SetMotorBack(index);
                    break;
                }
                case Motor_ForStable :    // maybe deffer ratio is set .  check it .             
                case Motor_ForChang :
                case Motor_ForFree :                    
                {// go brake first . BrakeStep
                    if(lpmotor->CurRatio < BrakeStep)
                    {// slowly . to go for next time
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_BackStable <<4)|Motor_WakeUp ;
                    }
                    else
                    {
                        lpmotor->CurRatio -= BrakeStep ;
                        lpmotor->Mode = (Motor_BackStable <<4)|Motor_ForBrake ;    
                    }
                    SetMotorStop(index,1);
                    break;
                }
                case Motor_BackStable :
                case Motor_BackChang :    
                {
                    lpmotor->Mode = (Motor_BackStable<<4)|Motor_BackChang;
                    if(lpmotor->CurRatio < (lpmotor->WishRatio - lpmotor->Step ))
                        lpmotor->CurRatio += lpmotor->Step;
                    else if(lpmotor->CurRatio > (lpmotor->WishRatio + lpmotor->Step ))
                        lpmotor->CurRatio -= lpmotor->Step;
                    else
                    {
                        lpmotor->CurRatio = lpmotor->WishRatio;
                        lpmotor->Mode = Motor_BackStable ;  // do not need gointo  MotorProc
                        MotorPostDone(index);
                    }
                    UpdateMotorBack(index);
                    break;
                }
                case Motor_BackFree :                
                case Motor_BackBrake :                
                {
                    if(lpmotor->CurRatio < (lpmotor->WishRatio - lpmotor->Step ))
                    {
                        lpmotor->CurRatio += MinPWMDuty ;
                    }
                    else if(lpmotor->CurRatio > (lpmotor->WishRatio + lpmotor->Step ))
                    {
                        lpmotor->CurRatio -= MinPWMDuty ;
                    }
                    lpmotor->Mode = (Motor_BackStable <<4)|Motor_BackChang ;
                    SetMotorBack(index);
                    break;
                }
                case Motor_ForBrake :
                {
                    if(lpmotor->CurRatio < BrakeStep)
                    {// slowly . to go for next time
                        lpmotor->CurRatio = 0 ;
                        lpmotor->Mode = (Motor_BackStable <<4)|Motor_WakeUp ;
                    }
                    else
                    {
                        lpmotor->CurRatio -= BrakeStep ;
                    }                
                    break;
                }
            }
            break;     
        }            

    }        
}

// used the LPTIM as a common delay timer array source . 
void InitLpTime(void)
{
    //LPTIM init // use the lptim as a step driver for motor 1,2,3    
    EnablePeriph(Clock_LPTIM);
    //stop running 
    LPTIM->CR = 0 ;
    // clock 32768, predive is 16 , timer mode 
    LPTIM->CFGR = LPTIME_PREDIV <<10 ;
    LPTIM->ARR =  Sft_OVRFLW_VALUE ; 
    // enable compare .
    LPTIM->CCSR = 0x0A ; 
    // enable CC1 ,CC2 ,//overflow interrupt . .
    LPTIM->IER = 0; 
    //clear if
    LPTIM->ISR = 0xfffff;
    //Set compare value 
 //   LPTIM->CCR1 = MOTOR2DIV;
 //   LPTIM->CCR2 = MOTOR3DIV;
    DisablePeriph(Clock_LPTIM);
    NVIC_DisableIRQ(LPTIM_IRQn);
    NVIC_SetPriority(LPTIM_IRQn,3);//中断优先级配置
    NVIC_EnableIRQ(LPTIM_IRQn);     //使        
}


// index : 0-2 
// mode  : Motor_PowerOffFree = slow stop . Motor_PowerOffBrake = quickly stop .
//         Motor_ForStable = forward at ratio by step .
//         Motor_BackStable = backward  at ratio by step .
// step  : chang slowly or quickly , can be [FreeStep - BrakeStep] (2,3,4,5,6)
void SetMotor(uint8_t index,MotorState mode , uint8_t ratio,uint8_t step)
{
    assert(index<3);
    if(step > BrakeStep ) step = BrakeStep;
    else if(step < FreeStep ) step = FreeStep;
    if(mode < Motor_PowerOffFree  || mode > Motor_BackStable)
        return ;
    if(ratio < MinPWMDuty ) ratio = MinPWMDuty ;
    else if(ratio >100) ratio = 100 ;
    
    EnMotorPower();
    EnVRefOut();      
    INT_DISABLE();	
    MotorCtrl[index].WishRatio = ratio ;    
    MotorCtrl[index].Step = step;
    // chang the wish mode  to new mode .
    step = mode <<4;
    ratio = MotorCtrl[index].Mode ;
    ratio &= 0x0f ;
    MotorCtrl[index].Mode = step | ratio;    
    MotorProc(index);
    INT_ENABLE();	
    step =MotorCtrl[0].Mode | MotorCtrl[1].Mode | MotorCtrl[2].Mode ;
    if( step &0xf0 )
    { // need the lptim to chang motor state
        RTC->IER |= Motor_RTC_Int_Mask ;
    }
    else
    {
        RTC->ISR = Motor_RTC_Int_Mask ;
        RTC->IER &= (~Motor_RTC_Int_Mask) ;
        if(step ==0 )
        {
            DisMotorPower();
            DisVRefOut();  
        }           
    }    
    
}



static uint8_t Percnt ;
void  StartCmp250ms(uint8_t percnt )
{   // set Cmp output period is 50ms , duty is percnt 
    /*
    输入PWM 频率为 50HZ，脉宽为2ms(10%占空比)，电调上电，会听到“BB”两声；
    调整脉宽为1ms（5%占空比），会听到“BBB”三声，解锁完毕。
    电调解锁完毕后，只需要在1ms ~ 2ms（5% - 10%占空比）之间，调整脉宽，即可调整无刷电机转速,1ms为停止，2ms为最大转速。
    */
    uint8_t temp ;
    
    // set the CMP2_INN_ pin  pa4 output .
    GPIO_DisablePinOpendrain(CMP2_INN_PORT,CMP2_INN_PIN);
    GPIO_SetPinMode(CMP2_INN_PORT,CMP2_INN_PIN,GPIO_MODE_OUTPUT);
    GPIO_SetOutputPin(CMP2_INN_PORT,CMP2_INN_PIN);  
    Percnt = percnt; 
    ReleaseSoftTimerEvt(Sig_50Hz_Overtime);
    ReleaseSoftTimerEvt(Sig_50Hz_Duty);
    temp = GetSoftTimerCnt();
    assert(temp == 0 );
    if(percnt ==0 )
    {
        GPIO_ResetOutputPin(CMP2_INN_PORT,CMP2_INN_PIN);
        return ;
    }
    // get the index of a softtimer , it can be used to start and stop this timer 
    temp = GetFreeSoftTimer();
    SetSoftTimerDelayMs(temp,20);
    SoftTimerBlockAndOpt(temp,BlcId_Hal ,SftOpt_Repeat);
    SetSoftTimerEvt(temp, Sig_50Hz_Overtime);
    StartSoftTimer(temp);
    temp = GetFreeSoftTimer();
    SetSoftTimerDelayTicks(temp,percnt);
    SoftTimerBlockAndOpt(temp,BlcId_Hal ,SftOpt_Quick);
    SetSoftTimerEvt(temp, Sig_50Hz_Duty);
    StartSoftTimer(temp);    
    
}
void On50HzOvertime(void)
{
    uint8_t temp ;
    GPIO_SetOutputPin(CMP2_INN_PORT,CMP2_INN_PIN); 
    if(Percnt ==0)
    {
        GPIO_ResetOutputPin(CMP2_INN_PORT,CMP2_INN_PIN); 
        return ;
    }
    temp = GetFreeSoftTimer();
    SetSoftTimerDelayTicks(temp,Percnt);
    SoftTimerBlockAndOpt(temp,BlcId_Hal ,SftOpt_Quick);
    SetSoftTimerEvt(temp, Sig_50Hz_Duty);
    StartSoftTimer(temp);     
}



uint8_t SectorErase(uint32_t Address)
{
    uint32_t    Timeout = 0;
    uint32_t    Primask;
    uint8_t     ret = 0;
    /* 入口参数检查 */

    /*时钟使能*/
    RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_FLASH);
    if(Address & (FLASH_ADDRS_ALIGN-1))
    {
        /*地址未对齐*/
        return 1;
    }
    if(FLASH_GetFlashLockStatus(FLASH) == FLASH_FLASH_KEY_STATUS_ERROR)
    {
        /*Flash 已经锁定，复位前无法操作*/
        return 1;
    }
    RCC_Group2_EnableOperationClock(RCC_OPERATION2_CLOCK_FLASH_ERASN_PROGRAM);
    /*配置擦写类型*/
    FLASH_SetFlashEraseType(FLASH,FLASH_FLASH_ERTYPE_SECTOR);
    /* 开始擦除扇区*/
    FLASH_StartErase(FLASH);
    
    if(FLASH_GetFlashLockStatus(FLASH) != FLASH_FLASH_KEY_STATUS_SECTOR_ERASE)
    {/* Key 序列*/
        Primask = __get_PRIMASK();
        INT_DISABLE();
        __set_PRIMASK(0xffff);
        FLASH_UnlockFlash(FLASH,FLASH_ERASE_KEY);
        FLASH_UnlockFlash(FLASH,FLASH_SECTOR_ERASE_KEY);
        __set_PRIMASK(Primask);
        INT_ENABLE();
    }
    FLASH_ClearFlag_EraseFinish(FLASH);
    /* 擦请求 */
    *((uint32_t*)Address) =  FLASH_ERASE_REQUEST;
    while(1)
    {
        Timeout++;
        if((Timeout > FLASH_ERASE_TIMEOUT)\
           ||(FLASH_IsActiveFlag_ClockError(FLASH))\
           ||(FLASH_IsActiveFlag_KEYError(FLASH))\
           ||(FLASH_IsActiveFlag_AuthenticationError(FLASH)))
        {
            /* 超时或出现错误 */
            ret = 1;
            break;
        }
        else if(FLASH_IsActiveFlag_EraseFinish(FLASH))
        {
            /*编程成功*/
            FLASH_ClearFlag_EraseFinish(FLASH);
            ret =  0;
            break;
        }
    }
    FLASH_LockFlash(FLASH);
    RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_FLASH_ERASN_PROGRAM);
    return ret;
}

uint8_t Program_Sector(uint32_t SectorNum, uint32_t *data)
{
    uint32_t     Cnt;
    uint32_t     Primask;
    uint32_t     Address;
    uint32_t     Timeout;
    uint8_t  ret; 

    Address      = SectorNum*FLASH_SECTOR_SIZE_BYTE;
    /* 页对齐*/
    if(Address & (FLASH_SECTOR_SIZE_BYTE-1))
    {
        /*地址未对齐*/
        return 1;
    }
    RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_FLASH);
    RCC_Group2_EnableOperationClock(RCC_OPERATION2_CLOCK_FLASH_ERASN_PROGRAM);
    FLASH_StartProgram(FLASH);
    if(FLASH_GetFlashLockStatus(FLASH) != FLASH_FLASH_KEY_STATUS_PROGRAM)
    {/* Key 序列*/
        Primask = __get_PRIMASK();
        INT_DISABLE();
        __set_PRIMASK(0xffff);
        FLASH_UnlockFlash(FLASH,FLASH_PROGRAM_KEY1);
        FLASH_UnlockFlash(FLASH,FLASH_PROGRAM_KEY2);
        __set_PRIMASK(Primask);
        INT_ENABLE();
    }
    for(Cnt = 0;Cnt< FLASH_SECTOR_SIZE_BYTE;Cnt+=4)
    {
        Timeout = 0;
        FLASH_StartProgram(FLASH);
        *((uint32_t*)Address) =  *(data++);
        Address+=4;
        while(1)
        {
            Timeout++;
            if((Timeout > FLASH_ERASE_TIMEOUT)\
               ||(FLASH_IsActiveFlag_ClockError(FLASH))\
               ||(FLASH_IsActiveFlag_KEYError(FLASH))\
               ||(FLASH_IsActiveFlag_AuthenticationError(FLASH)))
            {
                /* 超时或出现错误 */
                ret = 1;
                break;
            }
            if(FLASH_IsActiveFlag_ProgramFinish(FLASH))
            {
                /*编程成功*/  
                FLASH_ClearFlag_ProgramFinish(FLASH);
                ret = 0;
                break;
            }
        }        
    }
    FLASH_LockFlash(FLASH);
    RCC_Group2_DisableOperationClock(RCC_OPERATION2_CLOCK_FLASH_ERASN_PROGRAM);
    return ret;
}


