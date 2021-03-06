/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_opa.h
  * @author  FMSH Application Team
  * @brief   Head file of OPA LL Module
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
#ifndef __FM33LC0XX_OPA_H
#define __FM33LC0XX_OPA_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
/** @addtogroup FM33LC0XX_Driver_OPA
  * @{
  */
/* Exported types -------------------------------------------------------------------------------------*/
/** @defgroup OPA_ES_INIT OPA Exported Init structures
  * @{
  */

/**
  * @brief LL OPA Init Sturcture definition
  */
typedef struct
{
    /*! 正向输入选择 */  
    uint32_t INP;          
    /*! 反向输入选择*/
    uint32_t INN;
    /*!  OPA 工作模式*/      
    uint32_t Mode;
    /*! 数字滤波器使能 */                                             
    uint32_t DigitalFilter;
    /*! PGA反向输入到选择由Pin输入 */  
    uint32_t NegtiveToPin; 
    /*! PGA模式下的放大倍数*/
    uint32_t Gain;
    /*! 低功耗模式使能 */  
    uint32_t LowPowermode;  
                                                      
} OPA_InitTypeDef;
/**
  * @}
  */
/* Exported constants ---------------------------------------------------------------------------------*/
/** @defgroup OPA_Exported_Constants OPA Exported Constants
  * @{
  */

#define    OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN1                  (0x0U << OPA_CR_VNSEL_Pos)
#define    OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN2                  (0x1U << OPA_CR_VNSEL_Pos)
#define    OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF                  (0x2U << OPA_CR_VNSEL_Pos)
#define    OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_THREE_QUARTERS   (0x3U << OPA_CR_VNSEL_Pos)
#define    OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_HALF             (0x4U << OPA_CR_VNSEL_Pos)
#define    OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_ONE_QUARTER      (0x5U << OPA_CR_VNSEL_Pos)
#define    OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_ONE_EIGHTH       (0x6U << OPA_CR_VNSEL_Pos)

#define    OPA_POSITIVE_INPUT_CHANNEL_OPA_INP1                 (0x0U << OPA_CR_VPSEL_Pos)
#define    OPA_POSITIVE_INPUT_CHANNEL_OPA_INP2                 (0x1U << OPA_CR_VPSEL_Pos)

#define    OPA_GAIN_MUL_2                                      (0x0U << OPA_CR_PGA_GAIN_Pos)
#define    OPA_GAIN_MUL_4                                      (0x1U << OPA_CR_PGA_GAIN_Pos)
#define    OPA_GAIN_MUL_8                                      (0x2U << OPA_CR_PGA_GAIN_Pos)
#define    OPA_GAIN_MUL_16                                     (0x3U << OPA_CR_PGA_GAIN_Pos)

#define    OPA_MODE_STANDLONE                                  (0x0U << OPA_CR_MOD_Pos)
#define    OPA_MODE_COMPARISON                                 (0x1U << OPA_CR_MOD_Pos)
#define    OPA_MODE_PGA                                        (0x2U << OPA_CR_MOD_Pos)
#define    OPA_MODE_BUFF                                       (0x3U << OPA_CR_MOD_Pos)

#define    OPA_NEGATIVE_TRM_AMPLITUDE_DECREASE_HIGH            (0xfU << OPA_CALR_NCAL_Pos)
#define    OPA_NEGATIVE_TRM_AMPLITUDE_DECREASE_LOW             (0x1U << OPA_CALR_NCAL_Pos)
#define    OPA_NEGATIVE_TRM_AMPLITUDE_CONSTANT                 (0x0U << OPA_CALR_NCAL_Pos)
#define    OPA_NEGATIVE_TRM_AMPLITUDE_INCREASE_LOW             (0x11U << OPA_CALR_NCAL_Pos)
#define    OPA_NEGATIVE_TRM_AMPLITUDE_INCREASE_HIGH            (0x1fU << OPA_CALR_NCAL_Pos)

#define    OPA_POSITIVE_TRM_AMPLITUDE_DECREASE_HIGH            (0xfU << OPA_CALR_PCAL_Pos)
#define    OPA_POSITIVE_TRM_AMPLITUDE_DECREASE_LOW             (0x1U << OPA_CALR_PCAL_Pos)
#define    OPA_POSITIVE_TRM_AMPLITUDE_CONSTANT                 (0x0U << OPA_CALR_PCAL_Pos)
#define    OPA_POSITIVE_TRM_AMPLITUDE_INCREASE_LOW             (0x11U << OPA_CALR_PCAL_Pos)
#define    OPA_POSITIVE_TRM_AMPLITUDE_INCREASE_HIGH            (0x1fU << OPA_CALR_PCAL_Pos)

/**
  * @}
  */
/* Exported functions ---------------------------------------------------------------------------------*/
/** @defgroup OPA_Exported_Functions OPA Exported Functions
  * @{
  */

/**
  * @brief    
  * @rmtoll   CR    BUFFEN    OPA_EnableVrefBuffer
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnableVrefBuffer(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CR, OPA_CR_BUFFEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    BUFFEN    OPA_IsEnabledVrefBuffer
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledVrefBuffer(OPA_Type* OPAx)
{
    return (uint32_t)(!(READ_BIT(OPAx->CR, OPA_CR_BUFFEN_Msk) == OPA_CR_BUFFEN_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    BUFFEN    OPA_DisableVrefBuffer
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisableVrefBuffer(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CR, OPA_CR_BUFFEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    BUFBYP    OPA_EnableByPassVrefBuffer
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnableByPassVrefBuffer(OPA_Type* OPAx)
{
    SET_BIT(OPAx->CR, OPA_CR_BUFBYP_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    BUFBYP    OPA_IsEnabledByPassVrefBuffer
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledByPassVrefBuffer(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_BUFBYP_Msk) == OPA_CR_BUFBYP_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    BUFBYP    OPA_DisableByPassVrefBuffer
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisableByPassVrefBuffer(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CR, OPA_CR_BUFBYP_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    VNSEL    OPA_SetNegtiveInputChannel
  * @param    OPAx OPA instance
  * @param    Channel This parameter can be one of the following values:
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN1
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN2
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_THREE_QUARTERS
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_HALF
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_ONE_QUARTER
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_ONE_EIGHTH
  * @retval   None
  */
__STATIC_INLINE void OPA_SetNegtiveInputChannel(OPA_Type* OPAx, uint32_t Channel)
{
    MODIFY_REG(OPAx->CR, OPA_CR_VNSEL_Msk, Channel);
}

/**
  * @brief    
  * @rmtoll   CR    VNSEL    OPA_GetNegtiveInputChannel
  * @param    OPAx OPA instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN1
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN2
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_THREE_QUARTERS
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_HALF
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_ONE_QUARTER
  *           @arg @ref OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_ONE_EIGHTH
  */
__STATIC_INLINE uint32_t OPA_GetNegtiveInputChannel(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_VNSEL_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    VPSEL    OPA_SetPositiveInputChannel
  * @param    OPAx OPA instance
  * @param    channel This parameter can be one of the following values:
  *           @arg @ref OPA_POSITIVE_INPUT_CHANNEL_OPA_INP1
  *           @arg @ref OPA_POSITIVE_INPUT_CHANNEL_OPA_INP2
  * @retval   None
  */
__STATIC_INLINE void OPA_SetPositiveInputChannel(OPA_Type* OPAx,uint32_t	channel)
{
    MODIFY_REG(OPAx->CR, OPA_CR_VPSEL_Msk, channel);
}

/**
  * @brief    
  * @rmtoll   CR    VPSEL    OPA_GetPositiveInputChannel
  * @param    OPAx OPA instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref OPA_POSITIVE_INPUT_CHANNEL_OPA_INP1
  *           @arg @ref OPA_POSITIVE_INPUT_CHANNEL_OPA_INP2
  */
__STATIC_INLINE uint32_t OPA_GetPositiveInputChannel(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_VPSEL_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    DF    OPA_EnableComparModeDigitalFilter
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnableComparModeDigitalFilter(OPA_Type* OPAx)
{
    SET_BIT(OPAx->CR, OPA_CR_DF_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    DF    OPA_IsEnabledComparModeDigitalFilter
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledComparModeDigitalFilter(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_DF_Msk) == OPA_CR_DF_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    DF    OPA_DisableComparModeDigitalFilter
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisableComparModeDigitalFilter(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CR, OPA_CR_DF_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    VN_EXC    OPA_EnableNegtiveConnetToPIN
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnableNegtiveConnetToPIN(OPA_Type* OPAx)
{
    SET_BIT(OPAx->CR, OPA_CR_VN_EXC_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    VN_EXC    OPA_IsEnabledNegtiveConnetToPIN
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledNegtiveConnetToPIN(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_VN_EXC_Msk) == OPA_CR_VN_EXC_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    VN_EXC    OPA_DisableNegtiveConnetToPIN
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisableNegtiveConnetToPIN(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CR, OPA_CR_VN_EXC_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    PGA_GAIN    OPA_SetGain
  * @param    OPAx OPA instance
  * @param    vel This parameter can be one of the following values:
  *           @arg @ref OPA_GAIN_MUL_2
  *           @arg @ref OPA_GAIN_MUL_4
  *           @arg @ref OPA_GAIN_MUL_8
  *           @arg @ref OPA_GAIN_MUL_16
  * @retval   None
  */
__STATIC_INLINE void OPA_SetGain(OPA_Type* OPAx, uint32_t vel)
{
    MODIFY_REG(OPAx->CR, OPA_CR_PGA_GAIN_Msk, vel);
}

/**
  * @brief    
  * @rmtoll   CR    PGA_GAIN    OPA_GetGain
  * @param    OPAx OPA instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref OPA_GAIN_MUL_2
  *           @arg @ref OPA_GAIN_MUL_4
  *           @arg @ref OPA_GAIN_MUL_8
  *           @arg @ref OPA_GAIN_MUL_16
  */
__STATIC_INLINE uint32_t OPA_GetGain(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_PGA_GAIN_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    MOD    OPA_SetOPAMode
  * @param    OPAx OPA instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref OPA_MODE_STANDLONE
  *           @arg @ref OPA_MODE_COMPARISON
  *           @arg @ref OPA_MODE_PGA
  *           @arg @ref OPA_MODE_BUFF
  * @retval   None
  */
__STATIC_INLINE void OPA_SetOPAMode(OPA_Type* OPAx, uint32_t mode)
{
    MODIFY_REG(OPAx->CR, OPA_CR_MOD_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR    MOD    OPA_GetOPAMode
  * @param    OPAx OPA instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref OPA_MODE_STANDLONE
  *           @arg @ref OPA_MODE_COMPARISON
  *           @arg @ref OPA_MODE_PGA
  *           @arg @ref OPA_MODE_BUFF
  */
__STATIC_INLINE uint32_t OPA_GetOPAMode(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_MOD_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    LPM    OPA_EnableLowpowerMode
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnableLowpowerMode(OPA_Type* OPAx)
{
    SET_BIT(OPAx->CR, OPA_CR_LPM_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    LPM    OPA_IsEnabledLowpowerMode
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledLowpowerMode(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_LPM_Msk) == OPA_CR_LPM_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    LPM    OPA_DisableLowpowerMode
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisableLowpowerMode(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CR, OPA_CR_LPM_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    EN    OPA_Enable
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_Enable(OPA_Type* OPAx)
{
    SET_BIT(OPAx->CR, OPA_CR_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    EN    OPA_IsEnabled
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabled(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CR, OPA_CR_EN_Msk) == OPA_CR_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    EN    OPA_Disable
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_Disable(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CR, OPA_CR_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CALR    NCAL_EN    OPA_EnableNegativeinputCalibration
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnableNegativeinputCalibration(OPA_Type* OPAx)
{
    SET_BIT(OPAx->CALR, OPA_CALR_NCAL_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CALR    NCAL_EN    OPA_IsEnabledNegativeinputCalibration
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledNegativeinputCalibration(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CALR, OPA_CALR_NCAL_EN_Msk) == OPA_CALR_NCAL_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CALR    NCAL_EN    OPA_DisableNegativeinputCalibration
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisableNegativeinputCalibration(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CALR, OPA_CALR_NCAL_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CALR    NCAL    OPA_SetNegativeTrim
  * @param    OPAx OPA instance
  * @param    trim_vel This parameter can be one of the following values:
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_DECREASE_HIGH
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_DECREASE_LOW
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_CONSTANT
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_INCREASE_LOW
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_INCREASE_HIGH
  * @retval   None
  */
__STATIC_INLINE void OPA_SetNegativeTrim(OPA_Type* OPAx, uint32_t trim_vel)
{
    MODIFY_REG(OPAx->CALR, OPA_CALR_NCAL_Msk, trim_vel);
}

/**
  * @brief    
  * @rmtoll   CALR    NCAL    OPA_GetNegativeTrim
  * @param    OPAx OPA instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_DECREASE_HIGH
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_DECREASE_LOW
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_CONSTANT
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_INCREASE_LOW
  *           @arg @ref OPA_NEGATIVE_TRM_AMPLITUDE_INCREASE_HIGH
  */
__STATIC_INLINE uint32_t OPA_GetNegativeTrim(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CALR, OPA_CALR_NCAL_Msk));
}

/**
  * @brief    
  * @rmtoll   CALR    PCAL_EN    OPA_EnablePositiveinputCalibration
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnablePositiveinputCalibration(OPA_Type* OPAx)
{
    SET_BIT(OPAx->CALR, OPA_CALR_PCAL_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CALR    PCAL_EN    OPA_IsEnabledPositiveinputCalibration
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledPositiveinputCalibration(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CALR, OPA_CALR_PCAL_EN_Msk) == OPA_CALR_PCAL_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CALR    PCAL_EN    OPA_DisablePositiveinputCalibration
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisablePositiveinputCalibration(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->CALR, OPA_CALR_PCAL_EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CALR    PCAL    OPA_SetPositiveTrim
  * @param    OPAx OPA instance
  * @param    trim_vel This parameter can be one of the following values:
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_DECREASE_HIGH
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_DECREASE_LOW
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_CONSTANT
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_INCREASE_LOW
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_INCREASE_HIGH
  * @retval   None
  */
__STATIC_INLINE void OPA_SetPositiveTrim(OPA_Type* OPAx, uint32_t trim_vel)
{
    MODIFY_REG(OPAx->CALR, OPA_CALR_PCAL_Msk, trim_vel);
}

/**
  * @brief    
  * @rmtoll   CALR    PCAL    OPA_GetPositiveTrim
  * @param    OPAx OPA instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_DECREASE_HIGH
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_DECREASE_LOW
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_CONSTANT
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_INCREASE_LOW
  *           @arg @ref OPA_POSITIVE_TRM_AMPLITUDE_INCREASE_HIGH
  */
__STATIC_INLINE uint32_t OPA_GetPositiveTrim(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->CALR, OPA_CALR_PCAL_Msk));
}

/**
  * @brief    
  * @rmtoll   IER    FIE    OPA_EnableITOutputFall
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnableITOutputFall(OPA_Type* OPAx)
{
    SET_BIT(OPAx->IER, OPA_IER_FIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    FIE    OPA_IsEnabledOutputFall
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledOutputFall(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->IER, OPA_IER_FIE_Msk) == OPA_IER_FIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    FIE    OPA_DisableOutputFall
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisableOutputFall(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->IER, OPA_IER_FIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RIE    OPA_EnableITOutputRise
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_EnableITOutputRise(OPA_Type* OPAx)
{
    SET_BIT(OPAx->IER, OPA_IER_RIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RIE    OPA_IsEnabledOutputRise
  * @param    OPAx OPA instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsEnabledOutputRise(OPA_Type* OPAx)
{
    return (uint32_t)(READ_BIT(OPAx->IER, OPA_IER_RIE_Msk) == OPA_IER_RIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RIE    OPA_DisableOutputRise
  * @param    OPAx OPA instance
  * @retval   None
  */
__STATIC_INLINE void OPA_DisableOutputRise(OPA_Type* OPAx)
{
    CLEAR_BIT(OPAx->IER, OPA_IER_RIE_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    FIF    OPA_IsActiveFlag_OutPutFall
  * @param    RNGx RNG instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsActiveFlag_OutPutFall(OPA_Type* RNGx)
{
    return (uint32_t)(READ_BIT(RNGx->ISR, OPA_ISR_FIF_Msk) == (OPA_ISR_FIF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    FIF    OPA_ClearFlag_OutPutFall
  * @param    RNGx RNG instance
  * @retval   None
  */
__STATIC_INLINE void OPA_ClearFlag_OutPutFall(OPA_Type* RNGx)
{
    WRITE_REG(RNGx->ISR, OPA_ISR_FIF_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    RIF    OPA_IsActiveFlag_OutPutRise
  * @param    RNGx RNG instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_IsActiveFlag_OutPutRise(OPA_Type* RNGx)
{
    return (uint32_t)(READ_BIT(RNGx->ISR, OPA_ISR_RIF_Msk) == (OPA_ISR_RIF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    RIF    OPA_ClearFlag_OutPutRise
  * @param    RNGx RNG instance
  * @retval   None
  */
__STATIC_INLINE void OPA_ClearFlag_OutPutRise(OPA_Type* RNGx)
{
    WRITE_REG(RNGx->ISR, OPA_ISR_RIF_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    OUT    OPA_GetOPAOutputLevel
  * @param    RNGx RNG instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t OPA_GetOPAOutputLevel(OPA_Type* RNGx)
{
    return (uint32_t)(READ_BIT(RNGx->ISR, OPA_ISR_OUT_Msk) == (OPA_ISR_OUT_Msk));
}

/**
  * @}
  */

/** @defgroup OPA_EF_Init Initialization and de-initialization functions
  * @{
  */
ErrorStatus OPA_DeInit(OPA_Type *OPAx);
void OPA_StructInit(OPA_InitTypeDef *OPA_InitStruct);
ErrorStatus OPA_Init(OPA_Type *OPAx, OPA_InitTypeDef *OPA_InitStruct);

/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __FM33LC0XX_OPA_H*/

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-09-25**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
