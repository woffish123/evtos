/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_lcd.c
  * @author  FMSH Application Team
  * @brief   Src file of LCD LL Module
  *******************************************************************************************************
  * @attention
  *
  * Copyright (c) [2019] [Fudan Microelectronics]
  * THIS SOFTWARE is licensed under the Mulan PSL v1.
  * can use this software according to the terms and conditions of the Mulan PSL v1.
  * You may obtain a copy of Mulan PSL v1 at:
  * http://license.coscl.org.cn/MulanPSL
  * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
  * PURPOSE.
  * See the Mulan PSL v1 for more details.
  *
  *******************************************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "fm33lc0xx_ll_rcc.h"
#include "fm33lc0xx_ll_rmu.h"
#include "fm33lc0xx_ll_lcd.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_Driver_LCD
  * @{
  */
  
/** @addtogroup Private_Macros
  * @{
  */

#define         IS_LCD_INSTANCE(INTENCE)             ((INTENCE) == LCD)

#define         IS_LCD_ICCTRL(__VALUE__)             (((__VALUE__) == (LCD_DRV_IC_CTRL_LARGETS))||\
                                                        ((__VALUE__) == (LCD_DRV_IC_CTRL_LARGE))||\
                                                        ((__VALUE__) == (LCD_DRV_IC_CTRL_SMALL))||\
                                                        ((__VALUE__) == (LCD_DRV_IC_CTRL_MININUM)))
                                                        
#define         IS_LCD_ENMODE(__VALUE__)             ((__VALUE__) == (LCD_DRV_ENMODE_INNERRESISTER))

#define         IS_LCD_BIAS(__VALUE__)               (((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_0))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_1))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_2))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_3))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_4))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_5))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_6))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_7))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_8))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_9))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_10))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_11))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_12))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_13))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_14))||\
                                                        ((__VALUE__) == (LCD_GRAY_SCALE_CTRL_LEVEL_15)))

#define         IS_LCD_BIASMD(__VALUE__)             (((__VALUE__) == (LCD_SET_BIASMD_DIV4BIAS))||\
                                                        ((__VALUE__) == (LCD_SET_BIASMD_DIV3BIAS)))
                                                        
#define         IS_LCD_BWFT(__VALUE__)               (((__VALUE__) == (LCD_WAVEFORM_FORMAT_TYPE_A))||\
                                                        ((__VALUE__) == (LCD_WAVEFORM_FORMAT_TYPE_B)))
                                                        
#define         IS_LCD_LMUX(__VALUE__)               (((__VALUE__) == (LCD_SEG_LINE_SELECT_COM_4))||\
                                                        ((__VALUE__) == (LCD_SEG_LINE_SELECT_COM_6))||\
                                                        ((__VALUE__) == (LCD_SEG_LINE_SELECT_COM_8)))


/**
  * @}
  */
/** @addtogroup LCD_EF_Init
  * @{
  */ 

/**
  * @brief  复位对应LCD寄存器.
  * @param  LCDx 
  * @retval ErrorStatus枚举值:
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */  
static uint32_t LCD_DisplayFreq(uint32_t freq)
{
    uint32_t displayFreq;
    uint8_t DivWFT;
	
	switch (LCD_GetWaveform(LCD))
    {
		case LCD_WAVEFORM_FORMAT_TYPE_A: DivWFT = 2; break;
        case LCD_WAVEFORM_FORMAT_TYPE_B: DivWFT = 4; break;
    }
	
	if ((freq > 0) && (freq <= 100))
	{
        switch (LCD_GetCOMxNum(LCD))
        {
            case LCD_SEG_LINE_SELECT_COM_4:
                displayFreq = (uint32_t)(32768.0 / (float)(4 * freq * DivWFT) + 0.5);
                break;
            
            case LCD_SEG_LINE_SELECT_COM_6:
                displayFreq = (uint32_t)(32768.0 / (float)(6 * freq * DivWFT) + 0.5);
                break;
            
            case LCD_SEG_LINE_SELECT_COM_8:
                displayFreq = (uint32_t)(32768.0 / (float)(8 * freq * DivWFT) + 0.5);
                break;
        }
	}
	else
	{
		displayFreq = 32;
	}
    
    return displayFreq & 0xff;
}

/**
  * @brief  复位对应LCD寄存器.
  * @param  LCDx 
  * @retval ErrorStatus枚举值:
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */  
static uint32_t LCD_FlickTime(uint32_t time)
{
    float stepTime;
   
    switch (LCD_GetCOMxNum(LCD))
    {
        case LCD_SEG_LINE_SELECT_COM_4:
            stepTime = (float)(4 * LCD_GetDisplayFrequencyPsc(LCD) * 2 * 16 * 1000) / 32768.0;
            break;
        
        case LCD_SEG_LINE_SELECT_COM_6:
            stepTime = (float)(6 * LCD_GetDisplayFrequencyPsc(LCD) * 2 * 16 * 1000) / 32768.0;
            break;
        
        case LCD_SEG_LINE_SELECT_COM_8:
            stepTime = (float)(8 * LCD_GetDisplayFrequencyPsc(LCD) * 2 * 16 * 1000) / 32768.0;
            break;
    }
    
    return (uint32_t)(time / stepTime + 0.5);
}

/**
  * @brief  复位对应LCD寄存器.

  * @param  LCDx 
  * @retval ErrorStatus枚举值:
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */  
ErrorStatus LCD_DeInit(LCD_Type *LCDx)
{
	assert_param(IS_LCD_INSTANCE(LCDx));
    
    /* 使能外设复位 */
    RCC_EnablePeripheralReset();
    
    /* 复位LCD外设寄存器 */
    RCC_EnableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_LCD);
    RCC_DisableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_LCD);
    /* 关闭外设总线始时钟和工作时钟 */
    RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_LCD);
    
    /* 锁定外设复位 */
    RCC_DisablePeripheralReset();
    return PASS;
}

/**
  * @brief  根据 LCD_InitStruct 的配置信息初始化对应外设入口地址的寄存器值.
  * @param  LCDx LCDx 
  * @param  LCD_InitStruct 指向一个 @ref LCD_InitTypeDef 结构体
  *         其中包含了外设的相关配置信息.
  * @retval ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS LPUART配置成功  
  */
ErrorStatus LCD_Init(LCD_Type *LCDx,LCD_InitTypeDef *LCD_InitStruct)
{
	assert_param(IS_LCD_INSTANCE(LCDx));   
    assert_param(IS_LCD_ICCTRL(LCD_InitStruct->ICCtrl));
    assert_param(IS_LCD_ENMODE(LCD_InitStruct->EnableMode));
    assert_param(IS_LCD_BIAS(LCD_InitStruct->BiasVoltage));
    assert_param(IS_LCD_BIASMD(LCD_InitStruct->BiasMode));
    assert_param(IS_LCD_BWFT(LCD_InitStruct->Waveform));
    assert_param(IS_LCD_LMUX(LCD_InitStruct->COMxNum)); 
    
    /* 外设总线始时钟 */
    RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_LCD);
    
    /* 电流源电流控制 */
    LCD_SetICCtrl(LCD, LCD_InitStruct->ICCtrl);
    /* LCD驱动模式 */
    LCD_SetEnableMode(LCD, LCD_InitStruct->EnableMode);
    /* 偏执电压设置 */
    LCD_SetBiasVoltage(LCD, LCD_InitStruct->BiasVoltage);
    /* 偏执模式选择 */
    LCD_SetBiasMode(LCD, LCD_InitStruct->BiasMode);
    /* 驱动波形设置 */
    LCD_SetWaveform(LCD, LCD_InitStruct->Waveform);
    /* COMx口选择 */
    LCD_SetCOMxNum(LCD, LCD_InitStruct->COMxNum);
    /* 设置工作频率 */
    LCD_SetDisplayFrequencyPsc(LCD, LCD_DisplayFreq(LCD_InitStruct->FreqPsc));
    /* 设置闪烁频率 */
    LCD_SetDispalyOnTime(LCD, LCD_FlickTime(LCD_InitStruct->FlickOnTime));
    LCD_SetDisplayOffTime(LCD, LCD_FlickTime(LCD_InitStruct->FlickOffTime));
    /* 使能外设 */
    LCD_Enable(LCD);
    return PASS;
}

/**
  * @brief	设置 LCD_InitStruct 为默认配置
  * @param 	LCD_InitStruct 指向需要将值设置为默认配置的结构体 @ref LCD_InitTypeDef 结构体  
  *         
  * @retval	None
  */
void LCD_StructInit(LCD_InitTypeDef *LCD_InitStruct)
{
	LCD_InitStruct->ICCtrl              =   LCD_DRV_IC_CTRL_LARGETS;
    LCD_InitStruct->EnableMode          =   LCD_DRV_ENMODE_INNERRESISTER;
    LCD_InitStruct->BiasVoltage         =   LCD_GRAY_SCALE_CTRL_LEVEL_10;
    LCD_InitStruct->BiasMode            =   LCD_SET_BIASMD_DIV4BIAS;
    LCD_InitStruct->Waveform            =   LCD_WAVEFORM_FORMAT_TYPE_A;
    LCD_InitStruct->COMxNum             =   LCD_SEG_LINE_SELECT_COM_6;
    LCD_InitStruct->FreqPsc             =   42;
    LCD_InitStruct->FlickOnTime         =   0;
    LCD_InitStruct->FlickOffTime        =   0;
}



/**
  * @}
  */
/**
  * @}
  */
/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-17**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
