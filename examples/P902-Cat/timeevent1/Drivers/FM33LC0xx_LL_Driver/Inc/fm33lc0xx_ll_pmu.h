/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_pmu.h
  * @author  FMSH Application Team
  * @brief   Head file of PMU LL Module
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
#ifndef __FM33LC0XX_PMU_H
#define __FM33LC0XX_PMU_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
/** @addtogroup FM33LC0XX_Driver_PMU
  * @{
  */
/* Exported types -------------------------------------------------------------------------------------*/
/** @defgroup PMU_ES_INIT PMU Exported Init structures
  * @{
  */

/**
  * @brief LL PMU Init Sturcture definition
  */
typedef struct
{    
    /*! 低功耗模式配置 */
    uint32_t PowerMode;
    /*! 低功耗模式下内核电压降低与否 */
	FunState CoreVoltageScaling; 
    /*! 睡眠模式配置 */    
	uint32_t DeepSleep;	
    /*! 唤醒后的系统频率 仅对RCHF */
	uint32_t WakeupFrequency;
    /*! 芯片LDO在低功耗模式下是否进入低功耗 */    
	uint32_t LDOLowPowerMode;
    /*! 额外唤醒延时 */    
    uint32_t WakeupDelay;
} PMU_SleepInitTypeDef;
/**
  * @}
  */

/* Exported constants ---------------------------------------------------------------------------------*/
/** @defgroup PMU_Exported_Constants PMU Exported Constants
  * @{
  */
#define    PMU_WKUP0PIN                                        (0x1U << 0U)
#define    PMU_WKUP1PIN                                        (0x1U << 1U)
#define    PMU_WKUP2PIN                                        (0x1U << 2U)
#define    PMU_WKUP3PIN                                        (0x1U << 3U)
#define    PMU_WKUP4PIN                                        (0x1U << 4U)
#define    PMU_WKUP5PIN                                        (0x1U << 5U)
#define    PMU_WKUP6PIN                                        (0x1U << 6U)
#define    PMU_WKUP7PIN                                        (0x1U << 7U)

#define    PMU_LDO_LPM_DISABLE                                 (0x0U << PMU_CR_LDO_LPM_Pos)
#define    PMU_LDO_LPM_ENABLE                                  (0x2U << PMU_CR_LDO_LPM_Pos)

#define    PMU_SLEEP_WAKEUP_FREQ_RCHF_8MHZ                     (0x0U << PMU_CR_WKFSEL_Pos)
#define    PMU_SLEEP_WAKEUP_FREQ_RCHF_16MHZ                    (0x1U << PMU_CR_WKFSEL_Pos)
#define    PMU_SLEEP_WAKEUP_FREQ_RCHF_24MHZ                    (0x2U << PMU_CR_WKFSEL_Pos)

#define    PMU_SLEEP_MODE_NOMAL                                (0x0U << PMU_CR_SLPDP_Pos)
#define    PMU_SLEEP_MODE_DEEP                                 (0x1U << PMU_CR_SLPDP_Pos)

#define    PMU_POWER_MODE_ACTIVE_AND_LPACTIVE                  (0x0U << PMU_CR_PMOD_Pos)
#define    PMU_POWER_MODE_LPRUN_ONLY                           (0x1U << PMU_CR_PMOD_Pos)
#define    PMU_POWER_MODE_SLEEP_AND_DEEPSLEEP                  (0x2U << PMU_CR_PMOD_Pos)


#define    PMU_STOP_CLEAR_MODE_ASYNCHRONOUS                    (0x0U << PMU_WKTR_STPCLR_Pos)
#define    PMU_STOP_CLEAR_MODE_SYNCHRONOUS                     (0x1U << PMU_WKTR_STPCLR_Pos)

#define    PMU_WAKEUP_DELAY_TIME_0US                           (0x0U << PMU_WKTR_T1A_Pos)
#define    PMU_WAKEUP_DELAY_TIME_2US                           (0x1U << PMU_WKTR_T1A_Pos)
#define    PMU_WAKEUP_DELAY_TIME_4US                           (0x2U << PMU_WKTR_T1A_Pos)
#define    PMU_WAKEUP_DELAY_TIME_8US                           (0x3U << PMU_WKTR_T1A_Pos)

/**
  * @}
  */
/* Exported functions ---------------------------------------------------------------------------------*/
/** @defgroup PMU_Exported_Functions PMU Exported Functions
  * @{
  */

/**
  * @brief    
  * @rmtoll   CR    LDO_LPM    PMU_SetLDOLowPowerMode
  * @param    PMUx PMU instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref PMU_LDO_LPM_DISANLE
  *           @arg @ref PMU_LDO_LPM_ENABLE
  * @retval   None
  */
__STATIC_INLINE void PMU_SetLDOLowPowerMode(PMU_Type* PMUx, uint32_t mode)
{
    MODIFY_REG(PMUx->CR, PMU_CR_LDO_LPM_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR    LDO_LPM    PMU_GetLDOLowPowerMode
  * @param    PMUx PMU instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref PMU_LDO_LPM_DISANLE
  *           @arg @ref PMU_LDO_LPM_ENABLE
  */
__STATIC_INLINE uint32_t PMU_GetLDOLowPowerMode(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->CR, PMU_CR_LDO_LPM_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    LDO15EN    PMU_IsEnableLDO15
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsEnableLDO15(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->CR, PMU_CR_LDO15EN_Msk) == PMU_CR_LDO15EN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    WKFSEL    PMU_SetWakeupRCHFFrequency
  * @param    PMUx PMU instance
  * @param    Freq This parameter can be one of the following values:
  *           @arg @ref PMU_SLEEP_WAKEUP_FREQ_RCHF_8MHZ
  *           @arg @ref PMU_SLEEP_WAKEUP_FREQ_RCHF_16MHZ
  *           @arg @ref PMU_SLEEP_WAKEUP_FREQ_RCHF_24MHZ
  * @retval   None
  */
__STATIC_INLINE void PMU_SetWakeupRCHFFrequency(PMU_Type* PMUx, uint32_t Freq)
{
    MODIFY_REG(PMUx->CR, PMU_CR_WKFSEL_Msk, Freq);
}

/**
  * @brief    
  * @rmtoll   CR    WKFSEL    PMU_GetWakeupRCHFFrequency
  * @param    PMUx PMU instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref PMU_SLEEP_WAKEUP_FREQ_RCHF_8MHZ
  *           @arg @ref PMU_SLEEP_WAKEUP_FREQ_RCHF_16MHZ
  *           @arg @ref PMU_SLEEP_WAKEUP_FREQ_RCHF_24MHZ
  */
__STATIC_INLINE uint32_t PMU_GetWakeupRCHFFrequency(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->CR, PMU_CR_WKFSEL_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    SLPDP    PMU_SetSleepMode
  * @param    PMUx PMU instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref PMU_SLEEP_MODE_DEEP
  *           @arg @ref PMU_SLEEP_MODE_NOMAL
  * @retval   None
  */
__STATIC_INLINE void PMU_SetSleepMode(PMU_Type* PMUx, uint32_t mode)
{
    MODIFY_REG(PMUx->CR, PMU_CR_SLPDP_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR    SLPDP    PMU_GetSleepMode
  * @param    PMUx PMU instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref PMU_SLEEP_MODE_DEEP
  *           @arg @ref PMU_SLEEP_MODE_NOMAL
  */
__STATIC_INLINE uint32_t PMU_GetSleepMode(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->CR, PMU_CR_SLPDP_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    CVS    PMU_EnableCoreVoltageScaling
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_EnableCoreVoltageScaling(PMU_Type* PMUx)
{
    SET_BIT(PMUx->CR, PMU_CR_CVS_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    CVS    PMU_IsEnabledCoreVoltageScaling
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsEnabledCoreVoltageScaling(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->CR, PMU_CR_CVS_Msk) == PMU_CR_CVS_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    CVS    PMU_DisableCoreVoltageScaling
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_DisableCoreVoltageScaling(PMU_Type* PMUx)
{
    CLEAR_BIT(PMUx->CR, PMU_CR_CVS_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    PMOD    PMU_SetLowPowerMode
  * @param    PMUx PMU instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref PMU_POWER_MODE_ACTIVE_AND_LPACTIVE
  *           @arg @ref PMU_POWER_MODE_LPRUN_ONLY
  *           @arg @ref PMU_POWER_MODE_SLEEP_AND_DEEPSLEEP
  * @retval   None
  */
__STATIC_INLINE void PMU_SetLowPowerMode(PMU_Type* PMUx, uint32_t mode)
{
    MODIFY_REG(PMUx->CR, PMU_CR_PMOD_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR    PMOD    PMU_GetLowPowerMode
  * @param    PMUx PMU instance
  * @retval   Returned value can be one of the following values:
  */
__STATIC_INLINE uint32_t PMU_GetLowPowerMode(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->CR, PMU_CR_PMOD_Msk));
}


/**
  * @brief    
  * @rmtoll   WKTR    STPCLR    PMU_SetStopSignalClearWay
  * @param    PMUx PMU instance
  * @param    config This parameter can be one of the following values:
  *           @arg @ref PMU_STOP_CLEAR_MODE_ASYNCHRONOUS
  *           @arg @ref PMU_STOP_CLEAR_MODE_SYNCHRONOUS
  * @retval   None
  */
__STATIC_INLINE void PMU_SetStopSignalClearWay(PMU_Type* PMUx, uint32_t config)
{
    MODIFY_REG(PMUx->WKTR, PMU_WKTR_STPCLR_Msk, config);
}

/**
  * @brief    
  * @rmtoll   WKTR    STPCLR    PMU_GetStopSignalClearWay
  * @param    PMUx PMU instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref PMU_STOP_CLEAR_MODE_ASYNCHRONOUS
  *           @arg @ref PMU_STOP_CLEAR_MODE_SYNCHRONOUS
  */
__STATIC_INLINE uint32_t PMU_GetStopSignalClearWay(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKTR, PMU_WKTR_STPCLR_Msk));
}

/**
  * @brief    
  * @rmtoll   WKTR    T1A    PMU_SetWakeupTimeDelay
  * @param    PMUx PMU instance
  * @param    time This parameter can be one of the following values:
  *           @arg @ref PMU_WAKEUP_DELAY_TIME_0US
  *           @arg @ref PMU_WAKEUP_DELAY_TIME_4US
  *           @arg @ref PMU_WAKEUP_DELAY_TIME_8US
  *           @arg @ref PMU_WAKEUP_DELAY_TIME_16US
  * @retval   None
  */
__STATIC_INLINE void PMU_SetWakeupTimeDelay(PMU_Type* PMUx, uint32_t time)
{
    MODIFY_REG(PMUx->WKTR, PMU_WKTR_T1A_Msk, time);
}

/**
  * @brief    
  * @rmtoll   WKTR    T1A    PMU_GetWakeupTimeDelay
  * @param    PMUx PMU instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref PMU_WAKEUP_DELAY_TIME_0US
  *           @arg @ref PMU_WAKEUP_DELAY_TIME_4US
  *           @arg @ref PMU_WAKEUP_DELAY_TIME_8US
  *           @arg @ref PMU_WAKEUP_DELAY_TIME_16US
  */
__STATIC_INLINE uint32_t PMU_GetWakeupTimeDelay(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKTR, PMU_WKTR_T1A_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    ADCWKF    PMU_IsActiveFlag_WakeupADC
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupADC(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_ADCWKF_Msk) == (PMU_WKFR_ADCWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    RTCWKF    PMU_IsActiveFlag_WakeupRTC
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupRTC(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_RTCWKF_Msk) == (PMU_WKFR_RTCWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    SVDWKF    PMU_IsActiveFlag_WakeupSVD
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupSVD(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_SVDWKF_Msk) == (PMU_WKFR_SVDWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    LFDETWKF    PMU_IsActiveFlag_WakeupLFDET
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupLFDET(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_LFDETWKF_Msk) == (PMU_WKFR_LFDETWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    VREFWKF    PMU_IsActiveFlag_WakeupVREF
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupVREF(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_VREFWKF_Msk) == (PMU_WKFR_VREFWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    IOWKF    PMU_IsActiveFlag_WakeupIO
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupIO(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_IOWKF_Msk) == (PMU_WKFR_IOWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    IICWKF    PMU_IsActiveFlag_WakeupIIC
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupIIC(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_I2CWKF_Msk) == (PMU_WKFR_I2CWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    LPU1WKF    PMU_IsActiveFlag_WakeupLPUART1
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupLPUART1(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_LPU1WKF_Msk) == (PMU_WKFR_LPU1WKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    LPU0WKF    PMU_IsActiveFlag_WakeupLPUART0
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupLPUART0(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_LPU0WKF_Msk) == (PMU_WKFR_LPU0WKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    UART1WKF    PMU_IsActiveFlag_WakeupUART1
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupUART1(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_UART1WKF_Msk) == (PMU_WKFR_UART1WKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    UART0WKF    PMU_IsActiveFlag_WakeupUART0
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupUART0(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_UART0WKF_Msk) == (PMU_WKFR_UART0WKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    OPA2WKF    PMU_IsActiveFlag_WakeupOPA2
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupOPA2(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_OPA2WKF_Msk) == (PMU_WKFR_OPA2WKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    OPA1WKF    PMU_IsActiveFlag_WakeupOPA1
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupOPA1(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_OPA1WKF_Msk) == (PMU_WKFR_OPA1WKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    LPTWKF    PMU_IsActiveFlag_WakeupLPT
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupLPT(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_LPTWKF_Msk) == (PMU_WKFR_LPTWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    BSTWKF    PMU_IsActiveFlag_WakeupBST
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupBST(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_BSTWKF_Msk) == (PMU_WKFR_BSTWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    DBGWKF    PMU_IsActiveFlag_WakeupDBG
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupDBG(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_DBGWKF_Msk) == (PMU_WKFR_DBGWKF_Msk));
}

/**
  * @brief    
  * @rmtoll   WKFR    DBGWKF    PMU_ClearFlag_WakeupDBG
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_ClearFlag_WakeupDBG(PMU_Type* PMUx)
{
    WRITE_REG(PMUx->WKFR, PMU_WKFR_DBGWKF_Msk);
}

/**
  * @brief    
  * @rmtoll   WKFR    WKPXF    PMU_IsActiveFlag_WakeupPIN
  * @param    PMUx PMU instance
  * @param    Pin This parameter can be one of the following values:
  *           @arg @ref PMU_WKUP0PIN
  *           @arg @ref PMU_WKUP1PIN
  *           @arg @ref PMU_WKUP2PIN
  *           @arg @ref PMU_WKUP3PIN
  *           @arg @ref PMU_WKUP4PIN
  *           @arg @ref PMU_WKUP5PIN
  *           @arg @ref PMU_WKUP6PIN
  *           @arg @ref PMU_WKUP7PIN
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_WakeupPIN(PMU_Type* PMUx, uint32_t Pin)
{
    return (uint32_t)(READ_BIT(PMUx->WKFR, PMU_WKFR_WKPXF_Msk & Pin) == (Pin));
}

/**
  * @brief    
  * @rmtoll   WKFR    WKPXF    PMU_ClearFlag_WakeupPIN
  * @param    PMUx PMU instance
  * @param    Pin This parameter can be one of the following values:
  *           @arg @ref PMU_WKUP0PIN
  *           @arg @ref PMU_WKUP1PIN
  *           @arg @ref PMU_WKUP2PIN
  *           @arg @ref PMU_WKUP3PIN
  *           @arg @ref PMU_WKUP4PIN
  *           @arg @ref PMU_WKUP5PIN
  *           @arg @ref PMU_WKUP6PIN
  *           @arg @ref PMU_WKUP7PIN
  * @retval   Returned value can be one of the following values:
  */
__STATIC_INLINE void PMU_ClearFlag_WakeupPIN(PMU_Type* PMUx, uint32_t Pin)
{
    WRITE_REG(PMUx->WKFR, Pin);
}

/**
  * @brief    
  * @rmtoll   IER    LPRUNEIE    PMU_EnableITLPRUNError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_EnableITLPRUNError(PMU_Type* PMUx)
{
    SET_BIT(PMUx->IER, PMU_IER_LPRUNEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    LPRUNEIE    PMU_IsEnabledITLPRUNError
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsEnabledITLPRUNError(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->IER, PMU_IER_LPRUNEIE_Msk) == PMU_IER_LPRUNEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    LPRUNEIE    PMU_DisableITLPRUNError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_DisableITLPRUNError(PMU_Type* PMUx)
{
    CLEAR_BIT(PMUx->IER, PMU_IER_LPRUNEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    LPACTEIE    PMU_EnableITLPACTIVEError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_EnableITLPACTIVEError(PMU_Type* PMUx)
{
    SET_BIT(PMUx->IER, PMU_IER_LPACTEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    LPACTEIE    PMU_IsEnabledITLPACTIVEError
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsEnabledITLPACTIVEError(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->IER, PMU_IER_LPACTEIE_Msk) == PMU_IER_LPACTEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    LPACTEIE    PMU_DisableITLPACTIVEError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_DisableITLPACTIVEError(PMU_Type* PMUx)
{
    CLEAR_BIT(PMUx->IER, PMU_IER_LPACTEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    SLPEIE    PMU_EnableITSleepError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_EnableITSleepError(PMU_Type* PMUx)
{
    SET_BIT(PMUx->IER, PMU_IER_SLPEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    SLPEIE    PMU_IsEnabledITSleepError
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsEnabledITSleepError(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->IER, PMU_IER_SLPEIE_Msk) == PMU_IER_SLPEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    SLPEIE    PMU_DisableITSleepError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_DisableITSleepError(PMU_Type* PMUx)
{
    CLEAR_BIT(PMUx->IER, PMU_IER_SLPEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RTCEIE    PMU_EnableITRTCBKPError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_EnableITRTCBKPError(PMU_Type* PMUx)
{
    SET_BIT(PMUx->IER, PMU_IER_RTCEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RTCEIE    PMU_IsEnabledITRTCBKPError
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsEnabledITRTCBKPError(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->IER, PMU_IER_RTCEIE_Msk) == PMU_IER_RTCEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RTCEIE    PMU_DisableITRTCBKPError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_DisableITRTCBKPError(PMU_Type* PMUx)
{
    CLEAR_BIT(PMUx->IER, PMU_IER_RTCEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    SLPUNEIE    PMU_IsActiveFlag_LPRUNError
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_LPRUNError(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->ISR, PMU_ISR_SLPUNEIE_Msk) == (PMU_ISR_SLPUNEIE_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    SLPUNEIE    PMU_ClearFlag_LPRUNError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_ClearFlag_LPRUNError(PMU_Type* PMUx)
{
    WRITE_REG(PMUx->ISR, PMU_ISR_SLPUNEIE_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    LPACTEIF    PMU_IsActiveFlag_LPACTIVEError
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_LPACTIVEError(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->ISR, PMU_ISR_LPACTEIF_Msk) == (PMU_ISR_LPACTEIF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    LPACTEIF    PMU_ClearFlag_LPACTIVEError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_ClearFlag_LPACTIVEError(PMU_Type* PMUx)
{
    WRITE_REG(PMUx->ISR, PMU_ISR_LPACTEIF_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    SLPEIF    PMU_IsActiveFlag_SleepError
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_SleepError(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->ISR, PMU_ISR_SLPEIF_Msk) == (PMU_ISR_SLPEIF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    SLPEIF    PMU_ClearFlag_SleepError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_ClearFlag_SleepError(PMU_Type* PMUx)
{
    WRITE_REG(PMUx->ISR, PMU_ISR_SLPEIF_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    RTCEIF    PMU_IsActiveFlag_RTCError
  * @param    PMUx PMU instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PMU_IsActiveFlag_RTCError(PMU_Type* PMUx)
{
    return (uint32_t)(READ_BIT(PMUx->ISR, PMU_ISR_RTCEIF_Msk) == (PMU_ISR_RTCEIF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    RTCEIF    PMU_ClearFlag_RTCError
  * @param    PMUx PMU instance
  * @retval   None
  */
__STATIC_INLINE void PMU_ClearFlag_RTCError(PMU_Type* PMUx)
{
    WRITE_REG(PMUx->ISR, PMU_ISR_RTCEIF_Msk);
}

/**
  * @}
  */

/** @defgroup PMU_EF_Init Initialization and de-initialization functions
  * @{
  */
ErrorStatus PMU_Sleep_DeInit(PMU_Type* PMUx);
void PMU_StructInit(PMU_SleepInitTypeDef *LPM_InitStruct);
ErrorStatus PMU_Sleep_Init(PMU_Type* PMUx, PMU_SleepInitTypeDef *LPM_InitStruct);
/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __FM33LC0XX_PMU_H*/

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-09-23**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
