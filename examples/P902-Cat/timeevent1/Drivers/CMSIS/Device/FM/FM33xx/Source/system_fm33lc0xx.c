/**************************************************************************//**
 * @file     system_<Device>.c
 * @brief    CMSIS Cortex-M# Device Peripheral Access Layer Source File for
 *           Device <Device>
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#include "fm33lc0xx.h"
#include "hal.h"
/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */

#define RCHF8M_NVR_TRIM     (*(uint32_t *)0x1FFFFB40)	// RC8M 常温校准值
#define RCHF16M_NVR_TRIM 	(*(uint32_t *)0x1FFFFB3C)	// RC16M 常温校准值
#define RCHF24M_NVR_TRIM 	(*(uint32_t *)0x1FFFFB38)	// RC24M 常温校准值



#if  RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_8MHZ
    #define  RCHF_NVR_TRIM RCHF8M_NVR_TRIM
#elif RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_16MHZ
    #define  RCHF_NVR_TRIM RCHF16M_NVR_TRIM
#elif RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_24MHZ
    #define  RCHF_NVR_TRIM RCHF24M_NVR_TRIM
#endif 



void SystemInit (void)
{
    uint32_t temp;
    // reset all periph . rtc is not inside.
    RCC_EnablePeripheralReset();   
    RCC_EnableResetAHBPeripheral(0xFFFFFFFF);    
    RCC_EnableResetAPB1Peripheral(0xFFFFFFFF);
    RCC_EnableResetAPB2Peripheral(0xFFFFFFFF);
    
    RCC_DisableResetAHBPeripheral(0x0);  
    RCC_DisableResetAPB1Peripheral(0x0);
    RCC_DisableResetAPB2Peripheral(0x0);    
    RCC_DisablePeripheralReset();
    // enable the VRef 
    // VREF->CR = 1 ;
    // set all periph  with  selectable work clock.
    RCC_SetEXTIClockSource(RCC_EXTERNAL_INTERRUPT_SAMPLING_CLOCK_SELECT_LSCLK);
    RCC_SetLPUARTClockSource(LPUART0,RCC_LPUART_OPERTION_CLOCK_SOURCE_LSCLK);
    RCC_SetLPUARTClockSource(LPUART1,RCC_LPUART_OPERTION_CLOCK_SOURCE_LSCLK);
    RCC_SetI2CClockSource(RCC_I2C_OPERATION_CLOCK_SOURCE_APBCLK1);
    // ATIM used APBCLK2  .  RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_16MHZ  ,RCC_APB2_DIV_1  so it's clock is 16M
    RCC_SetATIMClockSource(RCC_ATIME_OPERATION_CLOCK_SOURCE_APBCLK2);
    RCC_SetUARTClockSource(UART0,RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1);
    RCC_SetUARTClockSource(UART1,RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1);
    RCC_SetADCClockSource(RCC_ADC_OPERATION_CLOCK_PRESCALLER_RCHF);
    // lptim   used  32.768K as clock . it can be used in sleep mode .
    RCC_SetLPTIMClockSource(RCC_LPTIM_OPERATION_CLOCK_SOURCE_LSCLK);
    // bstim   used APB2CLK  the  RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_16MHZ  ,RCC_APB2_DIV_1  so it's clock is 16M
    RCC_SetBSTIMClockSource(RCC_BSTIM_OPERATION_CLK_SOURCE_APBCLK2);
    
#if RCHF_CLOCK   == RCC_RCHF_FREQUENCY_8MHZ
    RCC_SetRNGPrescaler(RCC_RNG_OPERATION_CLOCK_PRESCALER_DIV2);
    RCC_SetADCPrescaler(RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV1);    
#elif RCHF_CLOCK   == RCC_RCHF_FREQUENCY_16MHZ
    RCC_SetRNGPrescaler(RCC_RNG_OPERATION_CLOCK_PRESCALER_DIV4); 
    RCC_SetADCPrescaler(RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV2);
#elif RCHF_CLOCK   == RCC_RCHF_FREQUENCY_24MHZ    
    RCC_SetRNGPrescaler(RCC_RNG_OPERATION_CLOCK_PRESCALER_DIV8); 
    RCC_SetADCPrescaler(RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV8);
#endif    
    SCB->VTOR = Flash_App_Addr ;
    GPIO_COMMON->PINWKEN = 0x80000000 ;
    /*trim*/ 
    temp = RCHF_NVR_TRIM;
   
    if(!((temp>>16)&temp))
    {
        RCC->RCHFTR = (0x7F&temp);
    }
    RCC_SetRCHFFrequency(RCHF_CLOCK);
    // set rcmf  4Mhz , not opened . 
    temp = *(uint32_t*)(0x1FFFFB44U);
    if((((temp>>16)^temp)&0xFF) == 0xFF)
    {
        RCC->RCMFTR = (0x7F&temp);
    }
// save the RCHF clock for wakeup from sleep with same clock.       
#if  RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_8MHZ    
    PMU->CR = PMU_SLEEP_WAKEUP_FREQ_RCHF_8MHZ | PMU_CR_CVS_Msk ;
#elif  RCHF_CLOCK ==  RCC_RCHF_FREQUENCY_16MHZ    
    PMU->CR = PMU_SLEEP_WAKEUP_FREQ_RCHF_16MHZ | PMU_CR_CVS_Msk ;
#else
    PMU->CR = PMU_SLEEP_WAKEUP_FREQ_RCHF_24MHZ | PMU_CR_CVS_Msk ;
#endif     
    // enable power down reset .
    RMU->PDRCR |= 0x01;    
    // disable bor 
    RMU->BORCR =0x0;
    // set delay 2 us for HRC clock stable 
    PMU->WKTR = 0x01 ;
    
    // DEBUG IWDT WWDT //
    DBG->CR =0x03;
    
    //disable pll 
    RCC->PLLCR = (uint32_t)0x00000000U;
    // set default sysclk  :enable auto lpclk switch, enable exti when sleep ,   RCHF , div
    RCC->SYSCLKCR = (uint32_t)( 0x0A000000 | RCC_SYSTEM_CLKSOURCE_RCHF|RCC_SYSCLK_DIV_1| RCC_APB1_DIV_1 |RCC_APB2_DIV_1 );

    // enable gpio clock . PAD RCC 
    RCC->PCLKCR1 |=  (0x1U << 7U);
    
    // put all pin as oc , high ->  float .  but MDPWR_CTRL_PIN   GPIO_PIN_1 = 0
    GPIOA->FCR = 0x55555555;
    GPIOA->DO =  0xFFFD;
    GPIOA->ODEN =  0xFFFF;   
    GPIOB->FCR = 0x55555555;    //LED_2_PIN   GPIO_PIN_2 = 0    GPIO_PIN_9 = 0
    GPIOB->DO =  0xFFDB;  
    GPIOB->ODEN =  0xFFFF;
    GPIOC->FCR = 0x55555555;
    GPIOC->DO =  0xFFFF;
    GPIOC->ODEN =  0xFFFF;
    //  D7 and D8  as   SWD
    GPIOD->FCR = 0x557E9555; //GPIO_PIN_ALL_EXCEPTSWD and XTAL pin .
    GPIOD->DO =  0xF87F;   //GPIO_PIN_ALL_EXCEPTSWD and XTAL pin .
    GPIOD->ODEN =  0xF87F; //GPIO_PIN_ALL_EXCEPTSWD
    GPIOD->PUEN =  0x0180;    
    
    #ifdef USE_LSCLK_CLOCK_SRC_XTLF   
    // set portd  pin9 and pin10 as periph pin
    //GPIOD->FCR |= 0x3C0000; 
    // start XTLF
    //RCC->XTLFCR  = (uint32_t)(0x00000000U);
    RCC->XTLFCR  |= (uint32_t)(0x00000005U<<8);
    // delay a long time (1-s)  to let debug running .
    // and Becareful !! the LFXTAL need 1s to stable,
#if   Enable_Boot != 1   
    temp = 1  ;    
    while(temp <= 0x1fffff )
    {
       temp ++ ;
    } 
#else
    temp = 1  ;    
    while(temp <= 0x1ff )
    {
       temp ++ ;
    } 
#endif    
    // reduce the xtal currency . as 0.3uA
    RCC->XTLFCR  = (uint32_t)(0x00000003U);
    // set lsclk as xtlf
    RCC->LSCLKSEL = 0xAA;
    /* LSCXTLF*/
    RCC->SYSCLKCR |= 0x8000000U;
    #else
    RCC->SYSCLKCR &= 0x7FFFFFFU;
    RCC->LSCLKSEL = 0x55;
    #endif     
    RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_RTC);
    if(RCC->RSTFR & 0x300 )
    { //power on reset or powerdown reset.
        RCC->RSTFR = 0xffff;
        SetHalStatu(Hal_Statu_PwrdnReset);
        // enable  xtal , and set lsclk as xtal.
        RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_RTC);
        RTC->WER       = 0xACACACACU;
        RTC->IER       = 0; //RTC_IER_ALARM_IE | RTC_IER_4HZ_IE;
        RTC->ADJUST    = RTC_Default_OffSet;  // 0.954ppm per ticks   max is 0x1FF
        RTC->ADSIGN    = 0x00000000U;
        RTC->ALARM     = 0x00000000U;
        RTC->BCDDAY    = 0x00000000U;
        RTC->BCDHOUR   = 0x00000000U;
        RTC->BCDMIN    = 0x00000000U;
        RTC->BCDMONTH  = 0x00000000U;
        RTC->BCDSEC    = 0x00000000U;
        RTC->BCDWEEK   = 0x00000000U;
        RTC->BCDYEAR   = 0x00000000U;
        RTC->SBSCNT    = 0x00000000U;
        RTC->TMSEL     = 0x00000000U;
        RTC->ISR       = 0x00000000U; 
        RTC->WER       = 0x00000000U;
        RCC_Group1_DisableBusClock(RCC_BUS1_CLOCK_RTC); 
    }

    
    //  reset lora . removed , it will be done when needed .    
    // used PB14  LORA_EN_PORT: LORA_EN_PIN to enable lora power  Low  : Power   High close .   PB14 is alway open , and init at start
    //GPIO_DisablePinOpendrain(LORA_EN_PORT,LORA_EN_PIN);
    //GPIO_SetPinMode(LORA_EN_PORT,LORA_EN_PIN,GPIO_MODE_OUTPUT);
    //GPIO_ResetOutputPin(LORA_EN_PORT,LORA_EN_PIN);    
    // used PB8   LORA_RESET_PORT :LORA_RESET_PIN to reset  lora .  it should be done at very begainning .  low reset . high or oc  is free 
    //GPIO_SetPinMode(LORA_RESET_PORT,LORA_RESET_PIN,GPIO_MODE_OUTPUT);
    //GPIO_ResetOutputPin(LORA_RESET_PORT,LORA_RESET_PIN);   
    // delay 100us 
    //temp = 1  ;
    //while(temp <= 0x350 )
    //{
    //    temp ++ ;
    //}
    //GPIO_SetOutputPin(LORA_RESET_PORT,LORA_RESET_PIN);
    ClrCheckBoot();
} 

void NMI_Handler(void)
{
    led1on();
    led2on();
    return;
}
// hard error happed . reset
void HardFault_Handler(void)
{
    led1on();
    led2on();    
     RCC->SOFTRST = 0x5C5CAABB ; // do reset
}
//用于创造对特权操作系统代码的调用。SVC是用于呼叫操作系统所提供API的正道。用户程序只需知道传递给操作系统的参数，而不必知道各API函数的地址
void SVC_Handler(void)
{
    led1on();
    led2on();    
    return;
}

//某IRQ正在活动，并且被SysTick抢占，它将触发一个PendSV异常，以便缓期执行上下文切换//
void PendSV_Handler(void)
{
        led1on();
    led2on();
    return;
}


