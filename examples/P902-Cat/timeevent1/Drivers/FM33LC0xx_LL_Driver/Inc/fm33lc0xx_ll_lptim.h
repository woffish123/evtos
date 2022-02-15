/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_lptim.h
  * @author  FMSH Application Team
  * @brief   Head file of LPTIM LL Module
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


/* Define to prevent recursive inclusion---------------------------------------------------------------*/
#ifndef __FM33LC0XX_LPTIM_H
#define __FM33LC0XX_LPTIM_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
/** @addtogroup FM33LC0XX_Driver_LPTIM
  * @{
  */
/* Exported types -------------------------------------------------------------------------------------*/
/** @defgroup LPTIM_ES_INIT LPTIM Exported Init structures
  * @{
  */

/**
  * @brief LL LPTIM Init Sturcture definition
  */
/* 时基配置 */
typedef struct
{
    /* 内部时钟源*/
	uint32_t ClockSource;
    /* 预分器的时钟源 可选择内部时钟 也可选择外部时钟*/
	uint32_t PrescalerClockSource;   
    /* 分频系数*/
	uint32_t Prescaler;      
    /* 定时器工作模式选择 */
	uint32_t Mode;
    /* 单次计数使能 */   
    uint32_t OneState;
    /* ETR 外部触发计数边沿 */
	uint32_t CountPolarity; 
    /* 外部 触发的时钟极性 */
    uint32_t TriggerPolarity;
    
}LPTIM_TimeInitTypeDef;

/* 输入输出比较配置 */ 
typedef struct
{     
    /* 输入捕获通道 */
	uint32_t ICSource;  
    /* 输入捕获极性 */
	uint32_t ICPolarity;          
	
}TIM_IC_InitTypeDef;

/* 输入输出比较配置 */ 
typedef struct
{           
    /* 自动重装载值 */
    uint32_t OC_Arr;
    /* 比较载值 */
    uint32_t OC_Ccr;    
    /* 比较输出极性*/
	uint32_t OCPolarity;    
	
}TIM_OC_InitTypeDef;

/**
  * @}
  */
/* Exported constants ---------------------------------------------------------------------------------*/
/** @defgroup LPTIM_Exported_Constants LPTIM Exported Constants
  * @{
  */

#define    LPTIM_PRESCALER_INPUT_LPTIM_CLK                     (0x0U << LPTIM_CFGR_PSCSEL_Pos)
#define    LPTIM_PRESCALER_INPUT_LPTIM_INPUT                   (0x1U << LPTIM_CFGR_PSCSEL_Pos)

#define    LPTIM_INPUT_EXTERNAL_INPUT                          (0x0U << LPTIM_CFGR_INSEL_Pos)
#define    LPTIM_INPUT_ADC_EOC                                 (0x1U << LPTIM_CFGR_INSEL_Pos)

#define    LPTIM_CLOCKDIVISION_DIV1                            (0x0U << LPTIM_CFGR_DIVSEL_Pos)
#define    LPTIM_CLOCKDIVISION_DIV2                            (0x1U << LPTIM_CFGR_DIVSEL_Pos)
#define    LPTIM_CLOCKDIVISION_DIV4                            (0x2U << LPTIM_CFGR_DIVSEL_Pos)
#define    LPTIM_CLOCKDIVISION_DIV8                            (0x3U << LPTIM_CFGR_DIVSEL_Pos)
#define    LPTIM_CLOCKDIVISION_DIV16                           (0x4U << LPTIM_CFGR_DIVSEL_Pos)
#define    LPTIM_CLOCKDIVISION_DIV32                           (0x5U << LPTIM_CFGR_DIVSEL_Pos)
#define    LPTIM_CLOCKDIVISION_DIV64                           (0x6U << LPTIM_CFGR_DIVSEL_Pos)
#define    LPTIM_CLOCKDIVISION_DIV128                          (0x7U << LPTIM_CFGR_DIVSEL_Pos)

#define    LPTIM_EXTERNAL_INPUT_COUNT_POLARITY_RISING          (0x0U << LPTIM_CFGR_EDGESEL_Pos)
#define    LPTIM_EXTERNAL_INPUT_COUNT_POLARITY_FALLING         (0x1U << LPTIM_CFGR_EDGESEL_Pos)

#define    LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_RINSING       (0x0U << LPTIM_CFGR_TRIGCFG_Pos)
#define    LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_FALLING       (0x1U << LPTIM_CFGR_TRIGCFG_Pos)
#define    LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_BOTHEDGE      (0x2U << LPTIM_CFGR_TRIGCFG_Pos)

#define    LPTIM_ONE_STATE_TIMER_CONTINUE                      (0x0U << LPTIM_CFGR_ONST_Pos)
#define    LPTIM_ONE_STATE_TIMER_SINGLE                        (0x1U << LPTIM_CFGR_ONST_Pos)

#define    LPTIM_OPERATION_MODE_NORMAL                         (0x0U << LPTIM_CFGR_TMOD_Pos)
#define    LPTIM_OPERATION_MODE_TRIGGER_CNT                    (0x1U << LPTIM_CFGR_TMOD_Pos)
#define    LPTIM_OPERATION_MODE_EXTERNAL_ASYN_PULSE_CNT        (0x2U << LPTIM_CFGR_TMOD_Pos)
#define    LPTIM_OPERATION_MODE_TIMEOUT                        (0x3U << LPTIM_CFGR_TMOD_Pos)

#define    LPTIM_IC1_CAPCTURE_SOURCE_CHANNEL1                  (0x0U << LPTIM_CCSR_CAP1SSEL_Pos)
#define    LPTIM_IC1_CAPCTURE_SOURCE_XTLF                      (0x1U << LPTIM_CCSR_CAP1SSEL_Pos)
#define    LPTIM_IC1_CAPCTURE_SOURCE_RCLP                      (0x2U << LPTIM_CCSR_CAP1SSEL_Pos)
#define    LPTIM_IC1_CAPCTURE_SOURCE_RC4M_PSC                  (0x3U << LPTIM_CCSR_CAP1SSEL_Pos)

#define    LPTIM_IC2_CAPCTURE_SOURCE_GPTIM1_TRGO               (0x0U << LPTIM_CCSR_CAP2SSEL_Pos)
#define    LPTIM_IC2_CAPCTURE_SOURCE_GPTIM2_TRGO               (0x1U << LPTIM_CCSR_CAP2SSEL_Pos)
#define    LPTIM_IC2_CAPCTURE_SOURCE_ATIM_TRGO                 (0x2U << LPTIM_CCSR_CAP2SSEL_Pos)
#define    LPTIM_IC2_CAPCTURE_SOURCE_CHANNEL2                  (0x3U << LPTIM_CCSR_CAP2SSEL_Pos)
/*Functional abstraction*/
#define    LPTIM_CC_MODE_NONE                                 (0x0U)
#define    LPTIM_CC_MODE_CAPTURE                              (0x1U)
#define    LPTIM_CC_MODE_COMPARE                              (0x2U)
#define    LPTIM_IC_POLARITY_EDGE_RISING                      (0x0U)
#define    LPTIM_IC_POLARITY_EDGE_FALLING                     (0x1U)
#define    LPTIM_IC_POLARITY_EDGE_BOTHEDGE                    (0x2U)
#define    LPTIM_OC_POLARITY_LOW_TO_HIGH                      (0x0U)
#define    LPTIM_OC_POLARITY_HIGH_TO_LOW                      (0x1U)
#define    LPTIM_IC_CAPCTURED_EDGE_FALLING                    (0x0U)
#define    LPTIM_IC_CAPCTURED_EDGE_RISING                     (0x1U)

#define    LPTIM_CHANNEL_1                                    (0x0U << 0U)
#define    LPTIM_CHANNEL_2                                    (0x1U << 0U)
/**
  * @}
  */
/* Exported functions ---------------------------------------------------------------------------------*/
/** @defgroup LPTIM_Exported_Functions LPTIM Exported Functions
  * @{
  */

/**
  * @brief    LPTIM Enable
  * @rmtoll   CR    EN    LPTIM_Enable
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_Enable(LPTIM_Type *LPTIMx)
{
    SET_BIT(LPTIMx->CR, LPTIM_CR_EN_Msk);
}

/**
  * @brief    LPTIM Disable
  * @rmtoll   CR    EN    LPTIM_Disable
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_Disable(LPTIM_Type *LPTIMx)
{
    CLEAR_BIT(LPTIMx->CR, LPTIM_CR_EN_Msk);
}

/**
  * @brief    Get LPTIM Enable Status
  * @rmtoll   CR    EN    LPTIM_IsEnabled
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsEnabled(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CR, LPTIM_CR_EN_Msk) == LPTIM_CR_EN_Msk);
}

/**
  * @brief    External Trigger input Analog Filter Enable
  * @rmtoll   CFGR    ETR_AFEN    LPTIM_EnableExternalTriggerAnalogFilter
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_EnableExternalTriggerAnalogFilter(LPTIM_Type *LPTIMx)
{
    SET_BIT(LPTIMx->CFGR, LPTIM_CFGR_ETR_AFEN_Msk);
}

/**
  * @brief    External Trigger input Analog Filter Disable
  * @rmtoll   CFGR    ETR_AFEN    LPTIM_DisableExternalTriggerAnalogFilter
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_DisableExternalTriggerAnalogFilter(LPTIM_Type *LPTIMx)
{
    CLEAR_BIT(LPTIMx->CFGR, LPTIM_CFGR_ETR_AFEN_Msk);
}

/**
  * @brief    Get External Trigger input Analog Filter Enable Status
  * @rmtoll   CFGR    ETR_AFEN    LPTIM_IsEnabledExternalTriggerAnalogFilter
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsEnabledExternalTriggerAnalogFilter(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CFGR, LPTIM_CFGR_ETR_AFEN_Msk) == LPTIM_CFGR_ETR_AFEN_Msk);
}

/**
  * @brief    Set Prescaler input Select
  * @rmtoll   CFGR    PSCSEL    LPTIM_SetPrescalerInput
  * @param    LPTIMx LPTIM instance
  * @param    PrescalerInput This parameter can be one of the following values:
  *           @arg @ref LPTIM_PRESCALER_INPUT_LPTIM_CLK
  *           @arg @ref LPTIM_PRESCALER_INPUT_LPTIM_INPUT
  * @retval   None
  */
__STATIC_INLINE void LPTIM_SetPrescalerInput(LPTIM_Type *LPTIMx, uint32_t PrescalerInput)
{
    MODIFY_REG(LPTIMx->CFGR, LPTIM_CFGR_PSCSEL_Msk, PrescalerInput);
}

/**
  * @brief    Get Prescaler input Select
  * @rmtoll   CFGR    PSCSEL    LPTIM_GetPrescalerInput
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_PRESCALER_INPUT_LPTIM_CLK
  *           @arg @ref LPTIM_PRESCALER_INPUT_LPTIM_INPUT
  */
__STATIC_INLINE uint32_t LPTIM_GetPrescalerInput(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CFGR, LPTIM_CFGR_PSCSEL_Msk));
}

/**
  * @brief    Set External Trigger input source Select
  * @rmtoll   CFGR    INSEL    LPTIM_SetInput
  * @param    LPTIMx LPTIM instance
  * @param    Input This parameter can be one of the following values:
  *           @arg @ref LPTIM_INPUT_EXTERNAL_INPUT
  *           @arg @ref LPTIM_INPUT_ADC_EOC
  * @retval   None
  */
__STATIC_INLINE void LPTIM_SetInput(LPTIM_Type *LPTIMx, uint32_t Input)
{
    MODIFY_REG(LPTIMx->CFGR, LPTIM_CFGR_INSEL_Msk, Input);
}

/**
  * @brief    Get External Trigger input source Select
  * @rmtoll   CFGR    INSEL    LPTIM_GetInput
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_INPUT_EXTERNAL_INPUT
  *           @arg @ref LPTIM_INPUT_ADC_EOC
  */
__STATIC_INLINE uint32_t LPTIM_GetInput(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CFGR, LPTIM_CFGR_INSEL_Msk));
}

/**
  * @brief    Set Counter Clock Divider Select
  * @rmtoll   CFGR    DIVSEL    LPTIM_SetColckDivision
  * @param    LPTIMx LPTIM instance
  * @param    ClockDivision This parameter can be one of the following values:
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV1
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV2
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV4
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV8
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV16
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV32
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV64
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV128
  * @retval   None
  */
__STATIC_INLINE void LPTIM_SetColckDivision(LPTIM_Type *LPTIMx, uint32_t ClockDivision)
{
    MODIFY_REG(LPTIMx->CFGR, LPTIM_CFGR_DIVSEL_Msk, ClockDivision);
}

/**
  * @brief    Get Counter Clock Divider Select
  * @rmtoll   CFGR    DIVSEL    LPTIM_GetColckDivision
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV1
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV2
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV4
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV8
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV16
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV32
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV64
  *           @arg @ref LPTIM_CLOCKDIVISION_DIV128
  */
__STATIC_INLINE uint32_t LPTIM_GetColckDivision(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CFGR, LPTIM_CFGR_DIVSEL_Msk));
}

/**
  * @brief    Set ETR Clock Edge Select
  * @rmtoll   CFGR    EDGESEL    LPTIM_SetPolarityExternalInput
  * @param    LPTIMx LPTIM instance
  * @param    Polarity This parameter can be one of the following values:
  *           @arg @ref LPTIM_EXTERNAL_INPUT_COUNT_POLARITY_RISING
  *           @arg @ref LPTIM_EXTERNAL_INPUT_COUNT_POLARITY_FALLING
  * @retval   None
  */
__STATIC_INLINE void LPTIM_SetPolarityExternalInput(LPTIM_Type *LPTIMx, uint32_t Polarity)
{
    MODIFY_REG(LPTIMx->CFGR, LPTIM_CFGR_EDGESEL_Msk, Polarity);
}

/**
  * @brief    Get ETR Clock Edge Select
  * @rmtoll   CFGR    EDGESEL    LPTIM_GetPolarityExternalInput
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_EXTERNAL_INPUT_COUNT_POLARITY_RISING
  *           @arg @ref LPTIM_EXTERNAL_INPUT_COUNT_POLARITY_FALLING
  */
__STATIC_INLINE uint32_t LPTIM_GetPolarityExternalInput(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CFGR, LPTIM_CFGR_EDGESEL_Msk));
}

/**
  * @brief    Set ETR trigger Configuration
  * @rmtoll   CFGR    TRIGCFG    LPTIM_SetTriggerExternalInput
  * @param    LPTIMx LPTIM instance
  * @param    Trigger This parameter can be one of the following values:
  *           @arg @ref LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_RINSING
  *           @arg @ref LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_FALLING
  *           @arg @ref LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_BOTHEDGE
  * @retval   None
  */
__STATIC_INLINE void LPTIM_SetTriggerExternalInput(LPTIM_Type *LPTIMx, uint32_t Trigger)
{
    MODIFY_REG(LPTIMx->CFGR, LPTIM_CFGR_TRIGCFG_Msk, Trigger);
}

/**
  * @brief    Get ETR trigger Configuration
  * @rmtoll   CFGR    TRIGCFG    LPTIM_GetTriggerExternalInput
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_RINSING
  *           @arg @ref LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_FALLING
  *           @arg @ref LPTIM_EXTERNAL_INPUT_TRIGGER_POLARITY_BOTHEDGE
  */
__STATIC_INLINE uint32_t LPTIM_GetTriggerExternalInput(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CFGR, LPTIM_CFGR_TRIGCFG_Msk));
}

/**
  * @brief    Set One State Timer Mode
  * @rmtoll   CFGR    ONST    LPTIM_Set_OneStateTimer
  * @param    LPTIMx LPTIM instance
  * @param    OneStateTimer This parameter can be one of the following values:
  *           @arg @ref LPTIM_ONE_STATE_TIMER_CONTINUE
  *           @arg @ref LPTIM_ONE_STATE_TIMER_SINGLE
  * @retval   None
  */
__STATIC_INLINE void LPTIM_Set_OneStateTimer(LPTIM_Type *LPTIMx, uint32_t OneStateTimer)
{
    MODIFY_REG(LPTIMx->CFGR, LPTIM_CFGR_ONST_Msk, OneStateTimer);
}

/**
  * @brief    Get One State Timer Mode
  * @rmtoll   CFGR    ONST    LPTIM_Get_OneStateTimer
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_ONE_STATE_TIMER_CONTINUE
  *           @arg @ref LPTIM_ONE_STATE_TIMER_SINGLE
  */
__STATIC_INLINE uint32_t LPTIM_Get_OneStateTimer(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CFGR, LPTIM_CFGR_ONST_Msk));
}

/**
  * @brief    Set Timer operation Mode
  * @rmtoll   CFGR    TMOD    LPTIM_SetOperationMode
  * @param    LPTIMx LPTIM instance
  * @param    Mode This parameter can be one of the following values:
  *           @arg @ref LPTIM_OPERATION_MODE_NORMAL
  *           @arg @ref LPTIM_OPERATION_MODE_TRIGGER_CNT
  *           @arg @ref LPTIM_OPERATION_MODE_EXTERNAL_ASYN_PULSE_CNT
  *           @arg @ref LPTIM_OPERATION_MODE_TIMEOUT
  * @retval   None
  */
__STATIC_INLINE void LPTIM_SetOperationMode(LPTIM_Type *LPTIMx, uint32_t Mode)
{
    MODIFY_REG(LPTIMx->CFGR, LPTIM_CFGR_TMOD_Msk, Mode);
}

/**
  * @brief    Get Timer operation Mode
  * @rmtoll   CFGR    TMOD    LPTIM_GetOperationMode
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_OPERATION_MODE_NORMAL
  *           @arg @ref LPTIM_OPERATION_MODE_TRIGGER_CNT
  *           @arg @ref LPTIM_OPERATION_MODE_EXTERNAL_ASYN_PULSE_CNT
  *           @arg @ref LPTIM_OPERATION_MODE_TIMEOUT
  */
__STATIC_INLINE uint32_t LPTIM_GetOperationMode(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CFGR, LPTIM_CFGR_TMOD_Msk));
}

/**
  * @brief    Set Channel1 Compare Register
  * @rmtoll   CCR1        LPTIM_OC_SetCompareCH1
  * @param    LPTIMx LPTIM instance
  * @param    CompareValue 
  * @retval   None
  */
__STATIC_INLINE void LPTIM_OC_SetCompareCH1(LPTIM_Type *LPTIMx, uint32_t CompareValue)
{
    MODIFY_REG(LPTIMx->CCR1, (0xffffffffU << 0U), (CompareValue << 0U));
}

/**
  * @brief    Get Channel1 Compare Register
  * @rmtoll   CCR1        LPTIM_OC_GetCompareCH1
  * @param    LPTIMx LPTIM instance
  * @retval   
  */
__STATIC_INLINE uint32_t LPTIM_OC_GetCompareCH1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCR1, 0xffffffffU) >> 0U);
}

/**
  * @brief    Set Channel2 Compare Register
  * @rmtoll   CCR2        LPTIM_OC_SetCompareCH2
  * @param    LPTIMx LPTIM instance
  * @param    CompareValue 
  * @retval   None
  */
__STATIC_INLINE void LPTIM_OC_SetCompareCH2(LPTIM_Type *LPTIMx, uint32_t CompareValue)
{
    MODIFY_REG(LPTIMx->CCR2, (0xffffffffU << 0U), (CompareValue << 0U));
}

/**
  * @brief    Get Channel2 Compare Register
  * @rmtoll   CCR2        LPTIM_OC_GetCompareCH2
  * @param    LPTIMx LPTIM instance
  * @retval   
  */
__STATIC_INLINE uint32_t LPTIM_OC_GetCompareCH2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCR2, 0xffffffffU) >> 0U);
}

/**
  * @brief    Get Channel1 Capture Register
  * @rmtoll   CCR1        LPTIM_IC_GetCaptureCH1
  * @param    LPTIMx LPTIM instance
  * @retval   
  */
__STATIC_INLINE uint32_t LPTIM_IC_GetCaptureCH1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCR1, 0xffffffffU) >> 0U);
}

/**
  * @brief    Get Channel2 Capture Register
  * @rmtoll   CCR2        LPTIM_IC_GetCaptureCH2
  * @param    LPTIMx LPTIM instance
  * @retval   
  */
__STATIC_INLINE uint32_t LPTIM_IC_GetCaptureCH2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCR2, 0xffffffffU) >> 0U);
}

/**
  * @brief    Capture/Compare channel 1 Interrupt Enable
  * @rmtoll   IER    CC1IE    LPTIM_EnableIT_Channel1
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_EnableIT_Channel1(LPTIM_Type *LPTIMx)
{
    SET_BIT(LPTIMx->IER, LPTIM_IER_CC1IE_Msk);
}

/**
  * @brief    Capture/Compare channel 2 Interrupt Enable
  * @rmtoll   IER    CC2IE    LPTIM_EnableIT_Channel2
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_EnableIT_Channel2(LPTIM_Type *LPTIMx)
{
    SET_BIT(LPTIMx->IER, LPTIM_IER_CC2IE_Msk);
}

/**
  * @brief    Counter Over-Flow Interrupt Enable
  * @rmtoll   IER    OVIE    LPTIM_EnableIT_CounterOver
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_EnableIT_CounterOver(LPTIM_Type *LPTIMx)
{
    SET_BIT(LPTIMx->IER, LPTIM_IER_OVIE_Msk);
}

/**
  * @brief    External Trigger Interrupt Enable
  * @rmtoll   IER    TRIGIE    LPTIM_EnableIT_ExternalTrigger
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_EnableIT_ExternalTrigger(LPTIM_Type *LPTIMx)
{
    SET_BIT(LPTIMx->IER, LPTIM_IER_TRIGIE_Msk);
}

/**
  * @brief    Channel 1 Over-Capture Interrupt Enable
  * @rmtoll   IER    OVR1IE    LPTIM_EnableIT_CAP1OVR
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_EnableIT_CAP1OVR(LPTIM_Type *LPTIMx)
{
    SET_BIT(LPTIMx->IER, LPTIM_IER_OVR1IE_Msk);
}

/**
  * @brief    Channel 2 Over-Capture Interrupt Enable
  * @rmtoll   IER    OVR2IE    LPTIM_EnableIT_CAP2OVR
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_EnableIT_CAP2OVR(LPTIM_Type *LPTIMx)
{
    SET_BIT(LPTIMx->IER, LPTIM_IER_OVR2IE_Msk);
}

/**
  * @brief    Capture/Compare channel 1 Interrupt Disable
  * @rmtoll   IER    CC1IE    LPTIM_DisableIT_Channel1
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_DisableIT_Channel1(LPTIM_Type *LPTIMx)
{
    CLEAR_BIT(LPTIMx->IER, LPTIM_IER_CC1IE_Msk);
}

/**
  * @brief    Capture/Compare channel 2 Interrupt Disable
  * @rmtoll   IER    CC2IE    LPTIM_DisableIT_Channel2
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_DisableIT_Channel2(LPTIM_Type *LPTIMx)
{
    CLEAR_BIT(LPTIMx->IER, LPTIM_IER_CC2IE_Msk);
}

/**
  * @brief    Counter Over-Flow Interrupt Disable
  * @rmtoll   IER    OVIE    LPTIM_DisableIT_CounterOver
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_DisableIT_CounterOver(LPTIM_Type *LPTIMx)
{
    CLEAR_BIT(LPTIMx->IER, LPTIM_IER_OVIE_Msk);
}

/**
  * @brief    External Trigger Interrupt Disable
  * @rmtoll   IER    TRIGIE    LPTIM_DisableIT_ExternalTrigger
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_DisableIT_ExternalTrigger(LPTIM_Type *LPTIMx)
{
    CLEAR_BIT(LPTIMx->IER, LPTIM_IER_TRIGIE_Msk);
}

/**
  * @brief    Channel 1 Over-Capture Interrupt Disable
  * @rmtoll   IER    OVR1IE    LPTIM_DisableIT_CAP1OVR
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_DisableIT_CAP1OVR(LPTIM_Type *LPTIMx)
{
    CLEAR_BIT(LPTIMx->IER, LPTIM_IER_OVR1IE_Msk);
}

/**
  * @brief    Channel 2 Over-Capture Interrupt Disable
  * @rmtoll   IER    OVR2IE    LPTIM_DisableIT_CAP2OVR
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_DisableIT_CAP2OVR(LPTIM_Type *LPTIMx)
{
    CLEAR_BIT(LPTIMx->IER, LPTIM_IER_OVR2IE_Msk);
}

/**
  * @brief    Get Capture/Compare channel 1 Interrupt Enable Status
  * @rmtoll   IER    CC1IE    LPTIM_IsEnabled_Channel1
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsEnabled_Channel1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->IER, LPTIM_IER_CC1IE_Msk) == LPTIM_IER_CC1IE_Msk);
}

/**
  * @brief    Get Capture/Compare channel 2 Interrupt Enable Status
  * @rmtoll   IER    CC2IE    LPTIM_IsEnabled_Channel2
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsEnabled_Channel2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->IER, LPTIM_IER_CC2IE_Msk) == LPTIM_IER_CC2IE_Msk);
}

/**
  * @brief    Get Counter Over-Flow Interrupt Enable Status
  * @rmtoll   IER    OVIE    LPTIM_IsEnabled_CounterOver
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsEnabled_CounterOver(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->IER, LPTIM_IER_OVIE_Msk) == LPTIM_IER_OVIE_Msk);
}

/**
  * @brief    Get External Trigger Interrupt Enable Status
  * @rmtoll   IER    TRIGIE    LPTIM_IsEnabled_ExternalTrigger
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsEnabled_ExternalTrigger(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->IER, LPTIM_IER_TRIGIE_Msk) == LPTIM_IER_TRIGIE_Msk);
}

/**
  * @brief    Get Channel 1 Over-Capture Interrupt Enable Status
  * @rmtoll   IER    OVR1IE    LPTIM_IsEnabled_CAP1OVR
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsEnabled_CAP1OVR(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->IER, LPTIM_IER_OVR1IE_Msk) == LPTIM_IER_OVR1IE_Msk);
}

/**
  * @brief    Get Channel 2 Over-Capture Interrupt Enable Status
  * @rmtoll   IER    OVR2IE    LPTIM_IsEnabled_CAP2OVR
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsEnabled_CAP2OVR(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->IER, LPTIM_IER_OVR2IE_Msk) == LPTIM_IER_OVR2IE_Msk);
}

/**
  * @brief    Get Capture/Compare channel 1 Interrupt Flag
  * @rmtoll   ISR    CC1IF    LPTIM_IsActiveFlag_Channel1
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsActiveFlag_Channel1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->ISR, LPTIM_ISR_CC1IF_Msk) == (LPTIM_ISR_CC1IF_Msk));
}

/**
  * @brief    Get Capture/Compare channel 2 Interrupt Flag
  * @rmtoll   ISR    CC2IF    LPTIM_IsActiveFlag_Channel2
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsActiveFlag_Channel2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->ISR, LPTIM_ISR_CC2IF_Msk) == (LPTIM_ISR_CC2IF_Msk));
}

/**
  * @brief    Get Counter Over-Flow Interrupt Flag
  * @rmtoll   ISR    OVIF    LPTIM_IsActiveFlag_CounterOver
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsActiveFlag_CounterOver(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->ISR, LPTIM_ISR_OVIF_Msk) == (LPTIM_ISR_OVIF_Msk));
}

/**
  * @brief    Get External Trigger Interrupt Flag
  * @rmtoll   ISR    TRIGIF    LPTIM_IsActiveFlag_ExternalTrigger
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsActiveFlag_ExternalTrigger(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->ISR, LPTIM_ISR_TRIGIF_Msk) == (LPTIM_ISR_TRIGIF_Msk));
}

/**
  * @brief    Get Channel 1 Over-Capture Interrupt Flag
  * @rmtoll   ISR    CAP1OVR    LPTIM_IsActiveFlag_CAP1OVR
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsActiveFlag_CAP1OVR(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->ISR, LPTIM_ISR_CAP1OVR_Msk) == (LPTIM_ISR_CAP1OVR_Msk));
}

/**
  * @brief    Get Channel 2 Over-Capture Interrupt Flag
  * @rmtoll   ISR    CAP2OVR    LPTIM_IsActiveFlag_CAP2OVR
  * @param    LPTIMx LPTIM instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPTIM_IsActiveFlag_CAP2OVR(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->ISR, LPTIM_ISR_CAP2OVR_Msk) == (LPTIM_ISR_CAP2OVR_Msk));
}

/**
  * @brief    Clear Capture/Compare channel 1 Interrupt Flag
  * @rmtoll   ISR    CC1IF    LPTIM_ClearFlag_Channel1
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_ClearFlag_Channel1(LPTIM_Type *LPTIMx)
{
    WRITE_REG(LPTIMx->ISR, LPTIM_ISR_CC1IF_Msk);
}

/**
  * @brief    Clear Capture/Compare channel 2 Interrupt Flag
  * @rmtoll   ISR    CC2IF    LPTIM_ClearFlag_Channel2
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_ClearFlag_Channel2(LPTIM_Type *LPTIMx)
{
    WRITE_REG(LPTIMx->ISR, LPTIM_ISR_CC2IF_Msk);
}

/**
  * @brief    Clear Counter Over-Flow Interrupt Flag
  * @rmtoll   ISR    OVIF    LPTIM_ClearFlag_CounterOver
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_ClearFlag_CounterOver(LPTIM_Type *LPTIMx)
{
    WRITE_REG(LPTIMx->ISR, LPTIM_ISR_OVIF_Msk);
}

/**
  * @brief    Clear External Trigger Interrupt Flag
  * @rmtoll   ISR    TRIGIF    LPTIM_ClearFlag_ExternalTrigger
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_ClearFlag_ExternalTrigger(LPTIM_Type *LPTIMx)
{
    WRITE_REG(LPTIMx->ISR, LPTIM_ISR_TRIGIF_Msk);
}

/**
  * @brief    Clear Channel 1 Over-Capture Interrupt Flag
  * @rmtoll   ISR    CAP1OVR    LPTIM_ClearFlag_CAP1OVR
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_ClearFlag_CAP1OVR(LPTIM_Type *LPTIMx)
{
    WRITE_REG(LPTIMx->ISR, LPTIM_ISR_CAP1OVR_Msk);
}

/**
  * @brief    Clear Channel 2 Over-Capture Interrupt Flag
  * @rmtoll   ISR    CAP2OVR    LPTIM_ClearFlag_CAP2OVR
  * @param    LPTIMx LPTIM instance
  * @retval   None
  */
__STATIC_INLINE void LPTIM_ClearFlag_CAP2OVR(LPTIM_Type *LPTIMx)
{
    WRITE_REG(LPTIMx->ISR, LPTIM_ISR_CAP2OVR_Msk);
}

/**
  * @brief    Set Auto-Reload Register
  * @rmtoll   ARR        LPTIM_SetAutoReload
  * @param    LPTIMx LPTIM instance
  * @param    AutoReload 
  * @retval   None
  */
__STATIC_INLINE void LPTIM_SetAutoReload(LPTIM_Type *LPTIMx, uint32_t AutoReload)
{
    MODIFY_REG(LPTIMx->ARR, (0xffffffffU << 0U), (AutoReload << 0U));
}

/**
  * @brief    Get Auto-Reload Register
  * @rmtoll   ARR        LPTIM_GetAutoReload
  * @param    LPTIMx LPTIM instance
  * @retval   
  */
__STATIC_INLINE uint32_t LPTIM_GetAutoReload(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->ARR, 0xffffffffU) >> 0U);
}

/**
  * @brief    Set Channel 1 Capture edge Config
  * @rmtoll   CCSR    CAPCFG1    LPTIM_IC_SetPolarityChannel1
  * @param    LPTIMx LPTIM instance
  * @param    Polarity This parameter can be one of the following values:
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_RISING
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_FALLING
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_BOTHEDGE
  * @retval   None
  */
__STATIC_INLINE void LPTIM_IC_SetPolarityChannel1(LPTIM_Type *LPTIMx, uint32_t Polarity)
{
    MODIFY_REG(LPTIMx->CCSR, LPTIM_CCSR_CAPCFG1_Msk, Polarity << LPTIM_CCSR_CAPCFG1_Pos);
}

/**
  * @brief    Get Channel 1 Capture edge Config
  * @rmtoll   CCSR    CAPCFG1    LPTIM_IC_GetPolarityChannel1
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_RISING
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_FALLING
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_BOTHEDGE
  */
__STATIC_INLINE uint32_t LPTIM_IC_GetPolarityChannel1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_CAPCFG1_Msk) >> LPTIM_CCSR_CAPCFG1_Pos);
}

/**
  * @brief    Set Channel 2 Capture edge Config
  * @rmtoll   CCSR    CAPCFG2    LPTIM_IC_SetPolarityChannel2
  * @param    LPTIMx LPTIM instance
  * @param    Polarity This parameter can be one of the following values:
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_RISING
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_FALLING
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_BOTHEDGE
  * @retval   None
  */
__STATIC_INLINE void LPTIM_IC_SetPolarityChannel2(LPTIM_Type *LPTIMx, uint32_t Polarity)
{
    MODIFY_REG(LPTIMx->CCSR, LPTIM_CCSR_CAPCFG2_Msk, Polarity<<LPTIM_CCSR_CAPCFG2_Pos);
}

/**
  * @brief    Get Channel 2 Capture edge Config
  * @rmtoll   CCSR    CAPCFG2    LPTIM_IC_GetPolarityChannel2
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_RISING
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_FALLING
  *           @arg @ref LPTIM_IC_POLARITY_EDGE_BOTHEDGE
  */
__STATIC_INLINE uint32_t LPTIM_IC_GetPolarityChannel2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_CAPCFG2_Msk)>>LPTIM_CCSR_CAPCFG2_Pos);
}

/**
  * @brief    Set Channel 1 compare output Polarity
  * @rmtoll   CCSR    POLAR1    LPTIM_OC_SetPolarityChannel1
  * @param    LPTIMx LPTIM instance
  * @param    Polarity This parameter can be one of the following values:
  *           @arg @ref LPTIM_OC_POLARITY_LOW_TO_HIGH
  *           @arg @ref LPTIM_OC_POLARITY_HIGH_TO_LOW
  * @retval   None
  */
__STATIC_INLINE void LPTIM_OC_SetPolarityChannel1(LPTIM_Type *LPTIMx, uint32_t Polarity)
{
    MODIFY_REG(LPTIMx->CCSR, LPTIM_CCSR_POLAR1_Msk, Polarity << LPTIM_CCSR_POLAR1_Pos);
}

/**
  * @brief    Get Channel 1 compare output Polarity
  * @rmtoll   CCSR    POLAR1    LPTIM_OC_GetPolarityChannel1
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_OC_POLARITY_LOW_TO_HIGH
  *           @arg @ref LPTIM_OC_POLARITY_HIGH_TO_LOW
  */
__STATIC_INLINE uint32_t LPTIM_OC_GetPolarityChannel1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_POLAR1_Msk) >>LPTIM_CCSR_POLAR1_Pos );
}

/**
  * @brief    Set Channel 2 compare output Polarity
  * @rmtoll   CCSR    POLAR2    LPTIM_OC_SetPolarityChannel2
  * @param    LPTIMx LPTIM instance
  * @param    Polarity This parameter can be one of the following values:
  *           @arg @ref LPTIM_OC_POLARITY_LOW_TO_HIGH
  *           @arg @ref LPTIM_OC_POLARITY_HIGH_TO_LOW
  * @retval   None
  */
__STATIC_INLINE void LPTIM_OC_SetPolarityChannel2(LPTIM_Type *LPTIMx, uint32_t Polarity)
{
    MODIFY_REG(LPTIMx->CCSR, LPTIM_CCSR_POLAR2_Msk, Polarity << LPTIM_CCSR_POLAR2_Pos);
}

/**
  * @brief    Get Channel 2 compare output Polarity
  * @rmtoll   CCSR    POLAR2    LPTIM_OC_GetPolarityChannel2
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_OC_POLARITY_LOW_TO_HIGH
  *           @arg @ref LPTIM_OC_POLARITY_HIGH_TO_LOW
  */
__STATIC_INLINE uint32_t LPTIM_OC_GetPolarityChannel2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_POLAR2_Msk) >> LPTIM_CCSR_POLAR2_Pos);
}

/**
  * @brief    Set Channel 2 Capture/Compare Select
  * @rmtoll   CCSR    CC2S    LPTIM_CC_Set_ModeChannel2
  * @param    LPTIMx LPTIM instance
  * @param    Mode This parameter can be one of the following values:
  *           @arg @ref LPTIM_CC_MODE_NONE
  *           @arg @ref LPTIM_CC_MODE_CAPTURE
  *           @arg @ref LPTIM_CC_MODE_COMPARE
  * @retval   None
  */
__STATIC_INLINE void LPTIM_CC_Set_ModeChannel2(LPTIM_Type *LPTIMx, uint32_t Mode)
{
    MODIFY_REG(LPTIMx->CCSR, LPTIM_CCSR_CC2S_Msk, Mode << LPTIM_CCSR_CC2S_Pos);
}

/**
  * @brief    Get Channel 2 Capture/Compare Select
  * @rmtoll   CCSR    CC2S    LPTIM_CC_Get_ModeChannel2
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_CC_MODE_NONE
  *           @arg @ref LPTIM_CC_MODE_CAPTURE
  *           @arg @ref LPTIM_CC_MODE_COMPARE
  */
__STATIC_INLINE uint32_t LPTIM_CC_Get_ModeChannel2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_CC2S_Msk) >> LPTIM_CCSR_CC2S_Pos);
}

/**
  * @brief    Set Channel 1 Capture/Compare Select 
  * @rmtoll   CCSR    CC1S    LPTIM_CC_Set_ModeChannel1
  * @param    LPTIMx LPTIM instance
  * @param    Mode This parameter can be one of the following values:
  *           @arg @ref LPTIM_CC_MODE_NONE
  *           @arg @ref LPTIM_CC_MODE_CAPTURE
  *           @arg @ref LPTIM_CC_MODE_COMPARE
  * @retval   None
  */
__STATIC_INLINE void LPTIM_CC_Set_ModeChannel1(LPTIM_Type *LPTIMx, uint32_t Mode)
{
    MODIFY_REG(LPTIMx->CCSR, LPTIM_CCSR_CC1S_Msk, Mode << LPTIM_CCSR_CC1S_Pos );
}

/**
  * @brief    Get Channel 1 Capture/Compare Select 
  * @rmtoll   CCSR    CC1S    LPTIM_CC_Get_ModeChannel1
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_CC_MODE_NONE
  *           @arg @ref LPTIM_CC_MODE_CAPTURE
  *           @arg @ref LPTIM_CC_MODE_COMPARE
  */
__STATIC_INLINE uint32_t LPTIM_CC_Get_ModeChannel1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_CC1S_Msk)>>LPTIM_CCSR_CC1S_Pos);
}

/**
  * @brief    Get Channel 1 Captured Edge
  * @rmtoll   CCSR    CAP1EDGE    LPTIM_IC_GetCapcturedEdgeChannel1
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_IC_CAPCTURED_EDGE_FALLING
  *           @arg @ref LPTIM_IC_CAPCTURED_EDGE_RISING
  */
__STATIC_INLINE uint32_t LPTIM_IC_GetCapcturedEdgeChannel1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_CAP1EDGE_Msk) >> LPTIM_CCSR_CAP1EDGE_Pos);
}

/**
  * @brief    Get Channel2 Captured Edge
  * @rmtoll   CCSR    CAP2EDGE    LPTIM_IC_GetCapcturedEdgeChannel2
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_IC_CAPCTURED_EDGE_FALLING
  *           @arg @ref LPTIM_IC_CAPCTURED_EDGE_RISING
  */
__STATIC_INLINE uint32_t LPTIM_IC_GetCapcturedEdgeChannel2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_CAP2EDGE_Msk) >> LPTIM_CCSR_CAP2EDGE_Pos);
}

/**
  * @brief    Set Channel 1 Capture Source Select
  * @rmtoll   CCSR    CAP1SSEL    LPTIM_IC_SetCapctureSourceChannel1
  * @param    LPTIMx LPTIM instance
  * @param    CaptureSource This parameter can be one of the following values:
  *           @arg @ref LPTIM_IC1_CAPCTURE_SOURCE_CHANNEL1
  *           @arg @ref LPTIM_IC1_CAPCTURE_SOURCE_XTLF
  *           @arg @ref LPTIM_IC1_CAPCTURE_SOURCE_RCLP
  *           @arg @ref LPTIM_IC1_CAPCTURE_SOURCE_RC4M_PSC
  * @retval   None
  */
__STATIC_INLINE void LPTIM_IC_SetCapctureSourceChannel1(LPTIM_Type *LPTIMx, uint32_t CaptureSource)
{
    MODIFY_REG(LPTIMx->CCSR, LPTIM_CCSR_CAP1SSEL_Msk, CaptureSource);
}

/**
  * @brief    Get Channel 1 Capture Source Select
  * @rmtoll   CCSR    CAP1SSEL    LPTIM_IC_GetCapctureSourceChannel1
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_IC1_CAPCTURE_SOURCE_CHANNEL1
  *           @arg @ref LPTIM_IC1_CAPCTURE_SOURCE_XTLF
  *           @arg @ref LPTIM_IC1_CAPCTURE_SOURCE_RCLP
  *           @arg @ref LPTIM_IC1_CAPCTURE_SOURCE_RC4M_PSC
  */
__STATIC_INLINE uint32_t LPTIM_IC_GetCapctureSourceChannel1(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_CAP1SSEL_Msk));
}

/**
  * @brief    Set Channel 2 Capture Source Select
  * @rmtoll   CCSR    CAP2SSEL    LPTIM_IC_SetCapctureSourceChannel2
  * @param    LPTIMx LPTIM instance
  * @param    CaptureSource This parameter can be one of the following values:
  *           @arg @ref LPTIM_IC2_CAPCTURE_SOURCE_GPTIM1_TRGO
  *           @arg @ref LPTIM_IC2_CAPCTURE_SOURCE_GPTIM2_TRGO
  *           @arg @ref LPTIM_IC2_CAPCTURE_SOURCE_ATIM_TRGO
  *           @arg @ref LPTIM_IC2_CAPCTURE_SOURCE_CHANNEL2
  * @retval   None
  */
__STATIC_INLINE void LPTIM_IC_SetCapctureSourceChannel2(LPTIM_Type *LPTIMx, uint32_t CaptureSource)
{
    MODIFY_REG(LPTIMx->CCSR, LPTIM_CCSR_CAP2SSEL_Msk, CaptureSource);
}

/**
  * @brief    Get Channel 2 Capture Source Select
  * @rmtoll   CCSR    CAP2SSEL    LPTIM_IC_GetCapctureSourceChannel2
  * @param    LPTIMx LPTIM instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPTIM_IC2_CAPCTURE_SOURCE_GPTIM1_TRGO
  *           @arg @ref LPTIM_IC2_CAPCTURE_SOURCE_GPTIM2_TRGO
  *           @arg @ref LPTIM_IC2_CAPCTURE_SOURCE_ATIM_TRGO
  *           @arg @ref LPTIM_IC2_CAPCTURE_SOURCE_CHANNEL2
  */
__STATIC_INLINE uint32_t LPTIM_IC_GetCapctureSourceChannel2(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CCSR, LPTIM_CCSR_CAP2SSEL_Msk));
}

/**
  * @brief    Get 32bits-wide Current Counter 
  * @rmtoll   CNT        LPTIM_GetCounter
  * @param    LPTIMx LPTIM instance
  * @retval   
  */
__STATIC_INLINE uint32_t LPTIM_GetCounter(LPTIM_Type *LPTIMx)
{
    return (uint32_t)(READ_BIT(LPTIMx->CNT, 0xffffffffU) >> 0U);
}

/**
  * @}
  */

/** @defgroup LPTIM_EF_Init Initialization and de-initialization functions
  * @{
  */
ErrorStatus LPTIM_DeInit(LPTIM_Type *LPTIMx);
void LPTIM_ICModeStructInit(TIM_IC_InitTypeDef *LPTIM_InitStruct);
void LPTIM_OCModeStructInit(TIM_OC_InitTypeDef *LPTIM_InitStruct);
void LPTIM_TimeModeStructInit(LPTIM_TimeInitTypeDef *LPTIM_InitStruct);
ErrorStatus LPTIM_TimeModeInit(LPTIM_Type *LPTIMx, LPTIM_TimeInitTypeDef *LPTIM_InitStruct);
ErrorStatus LPTIM_IC_ModeInit(LPTIM_Type *LPTIMx,uint32_t Channel, TIM_IC_InitTypeDef *LPTIM_InitStruct);
ErrorStatus LPTIM_OC_ModeInit(LPTIM_Type *LPTIMx,uint32_t Channel, TIM_OC_InitTypeDef *LPTIM_InitStruct);
/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __FM33LC0XX_LPTIM_H*/

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-12-02**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
