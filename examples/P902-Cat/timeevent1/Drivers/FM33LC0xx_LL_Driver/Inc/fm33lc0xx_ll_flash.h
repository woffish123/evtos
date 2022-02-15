/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_flash.h
  * @author  FMSH Application Team
  * @brief   Head file of FLASH LL Module
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
#ifndef __FM33LC0XX_FLASH_H
#define __FM33LC0XX_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
/** @addtogroup FM33LC0XX_Driver_FLASH
  * @{
  */
/* Exported types -------------------------------------------------------------------------------------*/
/** @defgroup FLASH_ES_INIT FLASH Exported Init structures
  * @{
  */

/**
  * @brief LL FLASH Init Sturcture definition
  */
/**
  * @}
  */
/* Exported constants ---------------------------------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */
#define    FLASH_ERASE_KEY                                    (0x96969696U)
#define    FLASH_CHIP_ERASE_KEY                               (0x7D7D7D7DU)  
#define    FLASH_PGAE_ERASE_KEY                               (0xEAEAEAEAU)
#define    FLASH_SECTOR_ERASE_KEY                             (0x3C3C3C3CU)
#define    FLASH_ERASE_REQUEST                                (0x1234ABCDU)
#define    FLASH_PROGRAM_KEY1                                 (0xA5A5A5A5U)
#define    FLASH_PROGRAM_KEY2                                 (0xF1F1F1F1U)
/*8M*/
#define    FLASH_ERASE_TIMEOUT                                (0x0000FFFFU)
#define    FLASH_ADDRS_ALIGN                                  (0x00000004U)


//#warning    "应该在器件头中"
#define    FLASH_MAX_PAGE_NUM                                 (0x00000200U)
#define    FLASH_MAX_SECTOR_NUM                               (0x00000080U)
#define    FLASH_SECTOR_SIZE_BYTE                             (0x00000800U)
#define    FLASH_PGAE_SIZE_BYTE                               (0x00000200U)
#define    FLASH_ADDR_MAXPROGRAM                              (0x0003FFFFU)


#define    FLASH_INFORMATION1_REGIN                            (0x1U << 17U)
#define    FLASH_INFORMATION2_REGIN                            (0x1U << 18U)
#define    FLASH_BLOCK_0                                       (0x1U << 0U)
#define    FLASH_BLOCK_1                                       (0x1U << 1U)
#define    FLASH_BLOCK_2                                       (0x1U << 2U)
#define    FLASH_BLOCK_3                                       (0x1U << 3U)
#define    FLASH_BLOCK_4                                       (0x1U << 4U)
#define    FLASH_BLOCK_5                                       (0x1U << 5U)
#define    FLASH_BLOCK_6                                       (0x1U << 6U)
#define    FLASH_BLOCK_7                                       (0x1U << 7U)
#define    FLASH_BLOCK_8                                       (0x1U << 8U)
#define    FLASH_BLOCK_9                                       (0x1U << 9U)
#define    FLASH_BLOCK_10                                      (0x1U << 10U)
#define    FLASH_BLOCK_11                                      (0x1U << 11U)
#define    FLASH_BLOCK_12                                      (0x1U << 12U)
#define    FLASH_BLOCK_13                                      (0x1U << 13U)
#define    FLASH_BLOCK_14                                      (0x1U << 14U)
#define    FLASH_BLOCK_15                                      (0x1U << 15U)
                                                                  
#define    FLASH_BOOTSWAP_ENABLE                               (0x2U << FLASH_OPTBR_BTSEN_Pos)
#define    FLASH_BOOTSWAP_DISABLE                              (0x0U << FLASH_OPTBR_BTSEN_Pos)
#define    FLASH_APPCODE_LOCK_ENABLE                           (0x2U << FLASH_OPTBR_ACLOCKEN_Pos)
#define    FLASH_APPCODE_LOCK_DISABLE                          (0x0U << FLASH_OPTBR_ACLOCKEN_Pos)
#define    FLASH_DEBUG_READ_ENABLE                             (0x2U << FLASH_OPTBR_DBRDPEN_Pos)
#define    FLASH_DEBUG_READ_DISABLE                            (0x0U << FLASH_OPTBR_DBRDPEN_Pos)

#define    FLASH_BLOCK_WR_SWD_AND_SOFT                         (0x3U)
#define    FLASH_BLOCK_SWD_WR_SOFT_R                           (0x2U)
#define    FLASH_BLOCK_WR_NONE                                 (0x0U)

#define    FLASH_LATENCY_0                                     (0x0U << FLASH_RDCR_WAIT_Pos)
#define    FLASH_LATENCY_1                                     (0x1U << FLASH_RDCR_WAIT_Pos)
#define    FLASH_LATENCY_2                                     (0x2U << FLASH_RDCR_WAIT_Pos)

#define    FLASH_FLASH_ERTYPE_PAGE                             (0x0U << FLASH_EPCR_ERTYPE_Pos)
#define    FLASH_FLASH_ERTYPE_SECTOR                           (0x1U << FLASH_EPCR_ERTYPE_Pos)
#define    FLASH_FLASH_ERTYPE_CHIP                             (0x2U << FLASH_EPCR_ERTYPE_Pos)

#define    FLASH_FLASH_KEY_STATUS_LOCK                         (0x0U << FLASH_ISR_KEYSTA_Pos)
#define    FLASH_FLASH_KEY_STATUS_CHIP_ERASE                   (0x1U << FLASH_ISR_KEYSTA_Pos)
#define    FLASH_FLASH_KEY_STATUS_SECTOR_ERASE                 (0x2U << FLASH_ISR_KEYSTA_Pos)
#define    FLASH_FLASH_KEY_STATUS_PROGRAM                      (0x3U << FLASH_ISR_KEYSTA_Pos)
#define    FLASH_FLASH_KEY_STATUS_ERROR                        (0x4U << FLASH_ISR_KEYSTA_Pos)

#define    FLASH_BOOT_FLASH_SECTOR_0000H_1FFFH                 (0x0U << FLASH_ISR_BTSTA_Pos)
#define    FLASH_BOOT_FLASH_SECTOR_2000H_3FFFH                 (0x1U << FLASH_ISR_BTSTA_Pos)

/**
  * @}
  */
/* Exported functions ---------------------------------------------------------------------------------*/
/** @defgroup FLASH_Exported_Functions FLASH Exported Functions
  * @{
  */

/**
  * @brief    
  * @rmtoll   RDCR    WAIT    FLASH_SetLatency
  * @param    FLASHx FLASH instance
  * @param    latency This parameter can be one of the following values:
  *           @arg @ref FLASH_LATENCY_0
  *           @arg @ref FLASH_LATENCY_1
  *           @arg @ref FLASH_LATENCY_2
  * @retval   None
  */
__STATIC_INLINE void FLASH_SetLatency(FLASH_Type* FLASHx, uint32_t latency)
{
    MODIFY_REG(FLASHx->RDCR, FLASH_RDCR_WAIT_Msk, latency);
}

/**
  * @brief    
  * @rmtoll   RDCR    WAIT    FLASH_GetLatency
  * @param    FLASHx FLASH instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref FLASH_LATENCY_0
  *           @arg @ref FLASH_LATENCY_1
  *           @arg @ref FLASH_LATENCY_2
  */
__STATIC_INLINE uint32_t FLASH_GetLatency(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->RDCR, FLASH_RDCR_WAIT_Msk));
}

/**
  * @brief    
  * @rmtoll   PFCR    PRFTEN    FLASH_EnablePrefetch
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_EnablePrefetch(FLASH_Type* FLASHx)
{
    SET_BIT(FLASHx->PFCR, FLASH_PFCR_PRFTEN_Msk);
}

/**
  * @brief    
  * @rmtoll   PFCR    PRFTEN    FLASH_IsPrefetchEnabled
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsPrefetchEnabled(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->PFCR, FLASH_PFCR_PRFTEN_Msk) == FLASH_PFCR_PRFTEN_Msk);
}

/**
  * @brief    
  * @rmtoll   PFCR    PRFTEN    FLASH_DisablePrefetch
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_DisablePrefetch(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->PFCR, FLASH_PFCR_PRFTEN_Msk);
}

/**
  * @brief    
  * @rmtoll   OPTBR    IWDTSLP    FLASH_IsEnabledStopIWDT_UnderSleepMode
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnabledStopIWDT_UnderSleepMode(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->OPTBR, FLASH_OPTBR_IWDTSLP_Msk) == FLASH_OPTBR_IWDTSLP_Msk);
}

/**
  * @brief    
  * @rmtoll   OPTBR    IFLOCK    FLASH_IsActiveFlag_InformationLocked
  * @param    FLASHx FLASH instance
  * @param    region This parameter can be one of the following values:
  *           @arg @ref FLASH_INFORMATION1_REGIN
  *           @arg @ref FLASH_INFORMATION2_REGIN
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsActiveFlag_InformationLocked(FLASH_Type* FLASHx, uint32_t region)
{
    return(uint32_t)((READ_BIT(FLASHx->OPTBR, FLASH_OPTBR_IFLOCK_Msk&region) == region));
}

/**
  * @brief    
  * @rmtoll   OPTBR    DFLSEN    FLASH_IsEnabledDataFlash
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnabledDataFlash(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->OPTBR, FLASH_OPTBR_DFLSEN_Msk) == FLASH_OPTBR_DFLSEN_Msk);
}

/**
  * @brief    
  * @rmtoll   OPTBR    BTSEN    FLASH_IsEnabledBootSwap
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnabledBootSwap(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->OPTBR, FLASH_OPTBR_BTSEN_Msk) == FLASH_BOOTSWAP_ENABLE); 
}

/**
  * @brief    
  * @rmtoll   OPTBR    ACLOCKEN    FLASH_IsEnabledFlashLock
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnabledFlashLock(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->OPTBR, FLASH_OPTBR_ACLOCKEN_Msk) == FLASH_APPCODE_LOCK_ENABLE); 
}

/**
  * @brief    
  * @rmtoll   OPTBR    DBRDPEN    FLASH_IsEnabledDebugPortReadProtection
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnabledDebugPortReadProtection(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->OPTBR, FLASH_OPTBR_DBRDPEN_Msk) == FLASH_DEBUG_READ_ENABLE); 
}

/**
  * @brief    
  * @rmtoll   ACLOCK1        FLASH_SetFlashLowRegionLock
  * @param    FLASHx FLASH instance
  * @param    region This parameter can be one of the following values:
  *           @arg @ref FLASH_BLOCK_0  
  *           @arg @ref FLASH_BLOCK_1 
  *           @arg @ref FLASH_BLOCK_2 
  *           @arg @ref FLASH_BLOCK_3 
  *           @arg @ref FLASH_BLOCK_4 
  *           @arg @ref FLASH_BLOCK_5 
  *           @arg @ref FLASH_BLOCK_6 
  *           @arg @ref FLASH_BLOCK_7 
  *           @arg @ref FLASH_BLOCK_8 
  *           @arg @ref FLASH_BLOCK_9 
  *           @arg @ref FLASH_BLOCK_10
  *           @arg @ref FLASH_BLOCK_11
  *           @arg @ref FLASH_BLOCK_12
  *           @arg @ref FLASH_BLOCK_13
  *           @arg @ref FLASH_BLOCK_14
  *           @arg @ref FLASH_BLOCK_15
  * @param    region This parameter can be one of the following values:
  *           @arg @ref FLASH_BLOCK_WR_SWD_AND_SOFT 
  *           @arg @ref FLASH_BLOCK_SWD_WR_SOFT_R  
  *           @arg @ref FLASH_BLOCK_WR_NONE       
  * @retval   None      
  */
__STATIC_INLINE void FLASH_SetFlashLowRegionLock(FLASH_Type* FLASHx, uint32_t region, uint32_t mode)
{
    CLEAR_BIT(FLASHx->ACLOCK1, ((region * region) *(((mode==3)?0:(~mode))&0x03)));
}

/**
  * @brief    
  * @rmtoll   ACLOCK1        FLASH_GetFlashLowRegionLock
  * @param    FLASHx FLASH instance
  * @param    region This parameter can be one of the following values:
  *           @arg @ref FLASH_BLOCK_0  
  *           @arg @ref FLASH_BLOCK_1 
  *           @arg @ref FLASH_BLOCK_2 
  *           @arg @ref FLASH_BLOCK_3 
  *           @arg @ref FLASH_BLOCK_4 
  *           @arg @ref FLASH_BLOCK_5 
  *           @arg @ref FLASH_BLOCK_6 
  *           @arg @ref FLASH_BLOCK_7 
  *           @arg @ref FLASH_BLOCK_8 
  *           @arg @ref FLASH_BLOCK_9 
  *           @arg @ref FLASH_BLOCK_10
  *           @arg @ref FLASH_BLOCK_11
  *           @arg @ref FLASH_BLOCK_12
  *           @arg @ref FLASH_BLOCK_13
  *           @arg @ref FLASH_BLOCK_14
  *           @arg @ref FLASH_BLOCK_15 
  * @retval   Returned value can be one of the following values:
  *           @arg @ref FLASH_BLOCK_WR_SWD_AND_SOFT 
  *           @arg @ref FLASH_BLOCK_SWD_WR_SOFT_R  
  *           @arg @ref FLASH_BLOCK_WR_NONE 
  */
__STATIC_INLINE uint32_t FLASH_GetFlashLowRegionLock(FLASH_Type* FLASHx, uint32_t region)
{
    return (uint32_t)(READ_BIT(FLASHx->ACLOCK1, ((region * region) *0x03)) /(region * region)); 
}

/**
  * @brief    
  * @rmtoll   ACLOCK2        FLASH_SetFlashHighRegionLock
  * @param    FLASHx FLASH instance
  * @param    region This parameter can be one of the following values:
  *           @arg @ref FLASH_BLOCK_0  
  *           @arg @ref FLASH_BLOCK_1 
  *           @arg @ref FLASH_BLOCK_2 
  *           @arg @ref FLASH_BLOCK_3 
  *           @arg @ref FLASH_BLOCK_4 
  *           @arg @ref FLASH_BLOCK_5 
  *           @arg @ref FLASH_BLOCK_6 
  *           @arg @ref FLASH_BLOCK_7 
  *           @arg @ref FLASH_BLOCK_8 
  *           @arg @ref FLASH_BLOCK_9 
  *           @arg @ref FLASH_BLOCK_10
  *           @arg @ref FLASH_BLOCK_11
  *           @arg @ref FLASH_BLOCK_12
  *           @arg @ref FLASH_BLOCK_13
  *           @arg @ref FLASH_BLOCK_14
  *           @arg @ref FLASH_BLOCK_15
  * @param    region This parameter can be one of the following values:
  *           @arg @ref FLASH_BLOCK_WR_SWD_AND_SOFT 
  *           @arg @ref FLASH_BLOCK_SWD_WR_SOFT_R  
  *           @arg @ref FLASH_BLOCK_WR_NONE    
  * @retval   None
  */
__STATIC_INLINE void FLASH_SetFlashHighRegionLock(FLASH_Type* FLASHx, uint32_t region, uint32_t mode)
{
    CLEAR_BIT(FLASHx->ACLOCK2, ((region * region) *(((mode==3)?0:(~mode))&0x03))); 
}

/**
  * @brief    
  * @rmtoll   ACLOCK2        FLASH_GetFlashHighRegionLock
  * @param    FLASHx FLASH instance
  * @param    region This parameter can be one of the following values:
  *           @arg @ref FLASH_BLOCK_0  
  *           @arg @ref FLASH_BLOCK_1 
  *           @arg @ref FLASH_BLOCK_2 
  *           @arg @ref FLASH_BLOCK_3 
  *           @arg @ref FLASH_BLOCK_4 
  *           @arg @ref FLASH_BLOCK_5 
  *           @arg @ref FLASH_BLOCK_6 
  *           @arg @ref FLASH_BLOCK_7 
  *           @arg @ref FLASH_BLOCK_8 
  *           @arg @ref FLASH_BLOCK_9 
  *           @arg @ref FLASH_BLOCK_10
  *           @arg @ref FLASH_BLOCK_11
  *           @arg @ref FLASH_BLOCK_12
  *           @arg @ref FLASH_BLOCK_13
  *           @arg @ref FLASH_BLOCK_14
  *           @arg @ref FLASH_BLOCK_15
  * @retval   Returned value can be one of the following values:
  *           @arg @ref FLASH_BLOCK_SWD_WR_SOFT_R  
  *           @arg @ref FLASH_BLOCK_WR_SWD_AND_SOFT 
  *           @arg @ref FLASH_BLOCK_WR_NONE
  */
__STATIC_INLINE uint32_t FLASH_GetFlashHighRegionLock(FLASH_Type* FLASHx, uint32_t region)
{
    return (uint32_t)(READ_BIT(FLASHx->ACLOCK2, ((region * region) *0x03)) /(region * region)); 
}

/**
  * @brief    
  * @rmtoll   EPCR    ERTYPE    FLASH_SetFlashEraseType
  * @param    FLASHx FLASH instance
  * @param    type This parameter can be one of the following values:
  *           @arg @ref FLASH_FLASH_ERTYPE_PAGE
  *           @arg @ref FLASH_FLASH_ERTYPE_SECTOR
  *           @arg @ref FLASH_FLASH_ERTYPE_CHIP
  * @retval   None
  */
__STATIC_INLINE void FLASH_SetFlashEraseType(FLASH_Type* FLASHx, uint32_t type)
{
    MODIFY_REG(FLASHx->EPCR, FLASH_EPCR_ERTYPE_Msk, type);
}

/**
  * @brief    
  * @rmtoll   EPCR    ERTYPE    FLASH_GetFlashEraseType
  * @param    FLASHx FLASH instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref FLASH_FLASH_ERTYPE_PAGE
  *           @arg @ref FLASH_FLASH_ERTYPE_SECTOR
  *           @arg @ref FLASH_FLASH_ERTYPE_CHIP
  */
__STATIC_INLINE uint32_t FLASH_GetFlashEraseType(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->EPCR, FLASH_EPCR_ERTYPE_Msk));
}

/**
  * @brief    
  * @rmtoll   EPCR    PREQ    FLASH_StartProgram
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_StartProgram(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->EPCR, FLASH_EPCR_EREQ_Msk);
    SET_BIT(FLASHx->EPCR, FLASH_EPCR_PREQ_Msk);
}

/**
  * @brief    
  * @rmtoll   EPCR    EREQ    FLASH_StartErase
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_StartErase(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->EPCR, FLASH_EPCR_PREQ_Msk);
    SET_BIT(FLASHx->EPCR, FLASH_EPCR_EREQ_Msk);
}
/**
  * @brief    
  * @rmtoll   KEY        FLASH_UnlockFlash
  * @param    FLASHx FLASH instance
  * @param    key This parameter can be one of the following values:
  *           @arg @ref FLASH_ERASE_KEY                
  *           @arg @ref FLASH_CHIP_ERASE_KEY           
  *           @arg @ref FLASH_SECTOR_AND_PGAE_ERASE_KEY
  * @retval   None
  */
__STATIC_INLINE void FLASH_UnlockFlash(FLASH_Type* FLASHx, uint32_t key)
{
    WRITE_REG(FLASHx->KEY ,key); 
}
/**
  * @brief    
  * @rmtoll   KEY        FLASH_lockFlash
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_LockFlash(FLASH_Type* FLASHx)
{
    WRITE_REG(FLASHx->KEY ,0xFFFFFFFF); 
}
/**
  * @brief    
  * @rmtoll   IER    OPTIE    FLASH_EnableIT_OPTProgramError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_EnableIT_OPTProgramError(FLASH_Type* FLASHx)
{
    SET_BIT(FLASHx->IER, FLASH_IER_OPTIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    OPTIE    FLASH_DisableIT_OPTProgramError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_DisableIT_OPTProgramError(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->IER, FLASH_IER_OPTIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    OPTIE    FLASH_IsEnbaledIT_OPTProgramError
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnbaledIT_OPTProgramError(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->IER, FLASH_IER_OPTIE_Msk) == FLASH_IER_OPTIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    AUTHIE    FLASH_EnableIT_AuthenticationError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_EnableIT_AuthenticationError(FLASH_Type* FLASHx)
{
    SET_BIT(FLASHx->IER, FLASH_IER_AUTHIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    AUTHIE    FLASH_DisableIT_AuthenticationError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_DisableIT_AuthenticationError(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->IER, FLASH_IER_AUTHIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    AUTHIE    FLASH_IsEnbaledIT_AuthenticationError
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnbaledIT_AuthenticationError(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->IER, FLASH_IER_AUTHIE_Msk) == FLASH_IER_AUTHIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    KEYIE    FLASH_EnableIT_KEYError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_EnableIT_KEYError(FLASH_Type* FLASHx)
{
    SET_BIT(FLASHx->IER, FLASH_IER_KEYIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    KEYIE    FLASH_DisableIT_KEYError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_DisableIT_KEYError(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->IER, FLASH_IER_KEYIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    KEYIE    FLASH_IsEnbaledIT_KEYError
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnbaledIT_KEYError(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->IER, FLASH_IER_KEYIE_Msk) == FLASH_IER_KEYIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    CKIE    FLASH_EnableIT_ClockError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_EnableIT_ClockError(FLASH_Type* FLASHx)
{
    SET_BIT(FLASHx->IER, FLASH_IER_CKIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    CKIE    FLASH_DisableIT_ClockError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_DisableIT_ClockError(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->IER, FLASH_IER_CKIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    CKIE    FLASH_IsEnbaledIT_ClockError
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnbaledIT_ClockError(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->IER, FLASH_IER_CKIE_Msk) == FLASH_IER_CKIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    PRDIE    FLASH_EnableIT_ProgramFinish
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_EnableIT_ProgramFinish(FLASH_Type* FLASHx)
{
    SET_BIT(FLASHx->IER, FLASH_IER_PRDIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    PRDIE    FLASH_DisableIT_ProgramFinish
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_DisableIT_ProgramFinish(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->IER, FLASH_IER_PRDIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    PRDIE    FLASH_IsEnbaledIT_ProgramFinish
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnbaledIT_ProgramFinish(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->IER, FLASH_IER_PRDIE_Msk) == FLASH_IER_PRDIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    ERDIE    FLASH_EnableIT_EraseFinish
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_EnableIT_EraseFinish(FLASH_Type* FLASHx)
{
    SET_BIT(FLASHx->IER, FLASH_IER_ERDIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    ERDIE    FLASH_DisableIT_EraseFinish
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_DisableIT_EraseFinish(FLASH_Type* FLASHx)
{
    CLEAR_BIT(FLASHx->IER, FLASH_IER_ERDIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    ERDIE    FLASH_IsEnbaledIT_EraseFinish
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsEnbaledIT_EraseFinish(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->IER, FLASH_IER_ERDIE_Msk) == FLASH_IER_ERDIE_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    KEYSTA    FLASH_GetFlashLockStatus
  * @param    FLASHx FLASH instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref FLASH_FLASH_KEY_STATUS_LOCK
  *           @arg @ref FLASH_FLASH_KEY_STATUS_CHIP_ERASE
  *           @arg @ref FLASH_FLASH_KEY_STATUS_SECTOR_ERASE
  *           @arg @ref FLASH_FLASH_KEY_STATUS_PROGRAM
  *           @arg @ref FLASH_FLASH_KEY_STATUS_ERROR
  */
__STATIC_INLINE uint32_t FLASH_GetFlashLockStatus(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->ISR, FLASH_ISR_KEYSTA_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    BTSTA    FLASH_GetFlashSwapStatus
  * @param    FLASHx FLASH instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref FLASH_BOOT_FLASH_SECTOR_0000H_1FFFH
  *           @arg @ref FLASH_BOOT_FLASH_SECTOR_2000H_3FFFH
  */
__STATIC_INLINE uint32_t FLASH_GetFlashSwapStatus(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->ISR, FLASH_ISR_BTSTA_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    OPTERR    FLASH_IsActiveFlag_OPTProGramError
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsActiveFlag_OPTProGramError(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->ISR, FLASH_ISR_OPTERR_Msk) == (FLASH_ISR_OPTERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    OPTERR    FLASH_ClearFlag_OPTProGramError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_ClearFlag_OPTProGramError(FLASH_Type* FLASHx)
{
    WRITE_REG(FLASHx->ISR, FLASH_ISR_OPTERR_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    AUTHERR    FLASH_IsActiveFlag_AuthenticationError
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsActiveFlag_AuthenticationError(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->ISR, FLASH_ISR_AUTHERR_Msk) == (FLASH_ISR_AUTHERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    AUTHERR    FLASH_ClearFlag_AuthenticationError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_ClearFlag_AuthenticationError(FLASH_Type* FLASHx)
{
    WRITE_REG(FLASHx->ISR, FLASH_ISR_AUTHERR_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    KEYERR    FLASH_IsActiveFlag_KEYError
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsActiveFlag_KEYError(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->ISR, FLASH_ISR_KEYERR_Msk) == (FLASH_ISR_KEYERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    KEYERR    FLASH_ClearFlag_KEYError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_ClearFlag_KEYError(FLASH_Type* FLASHx)
{
    WRITE_REG(FLASHx->ISR, FLASH_ISR_KEYERR_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    CKERR    FLASH_IsActiveFlag_ClockError
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsActiveFlag_ClockError(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->ISR, FLASH_ISR_CKERR_Msk) == (FLASH_ISR_CKERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    CKERR    FLASH_ClearFlag_ClockError
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_ClearFlag_ClockError(FLASH_Type* FLASHx)
{
    WRITE_REG(FLASHx->ISR, FLASH_ISR_CKERR_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    PRD    FLASH_IsActiveFlag_ProgramFinish
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsActiveFlag_ProgramFinish(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->ISR, FLASH_ISR_PRD_Msk) == (FLASH_ISR_PRD_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    PRD    FLASH_ClearFlag_ProgramFinish
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_ClearFlag_ProgramFinish(FLASH_Type* FLASHx)
{
    WRITE_REG(FLASHx->ISR, FLASH_ISR_PRD_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    ERD    FLASH_IsActiveFlag_EraseFinish
  * @param    FLASHx FLASH instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t FLASH_IsActiveFlag_EraseFinish(FLASH_Type* FLASHx)
{
    return (uint32_t)(READ_BIT(FLASHx->ISR, FLASH_ISR_ERD_Msk) == (FLASH_ISR_ERD_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    ERD    FLASH_ClearFlag_EraseFinish
  * @param    FLASHx FLASH instance
  * @retval   None
  */
__STATIC_INLINE void FLASH_ClearFlag_EraseFinish(FLASH_Type* FLASHx)
{
    WRITE_REG(FLASHx->ISR, FLASH_ISR_ERD_Msk);
}

/**
  * @}
  */

/** @defgroup FLASH_EF_Init Initialization and de-initialization functions
  * @{
  */

ErrorStatus FLASH_PageErase(FLASH_Type* FLASHx, uint32_t Address);
ErrorStatus FLASH_SectorErase(FLASH_Type* FLASHx, uint32_t Address);
ErrorStatus FLASH_Program_Word(FLASH_Type* FLASHx, uint32_t Address, uint32_t data );
ErrorStatus FLASH_Program_Page(FLASH_Type* FLASHx, uint32_t PageNum, uint32_t *data);
ErrorStatus FLASH_Program_Sector(FLASH_Type* FLASHx, uint32_t SectorNum, uint32_t *data);
/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __FM33LC0XX_FLASH_H*/

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-09-23**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
