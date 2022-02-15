/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_i2c.h
  * @author  FMSH Application Team
  * @brief   Head file of I2C LL Module
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
#ifndef __FM33LC0XX_I2C_H
#define __FM33LC0XX_I2C_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
/** @addtogroup FM33LC0XX_Driver_I2C
  * @{
  */
/* Exported types -------------------------------------------------------------------------------------*/
/** @defgroup I2C_ES_INIT I2C Exported Init structures
  * @{
  */

/**
  * @brief LL I2C Init Sturcture definition
  */
typedef struct
{
    /*! 主机时使用的时钟源*/
    uint32_t ClockSource;          
    /*! I2C通讯速率*/
    uint32_t BaudRate;
                                                                                                      
} I2C_MasterMode_InitTypeDef;
/**
  * @brief LL I2C Slavemode Init Sturcture definition
  */
typedef struct
{
    /*! 从模式从机地址 */
    uint32_t OwnAddr;
    /*! 从模式自动回应ACK */  
    uint32_t ACK;
    /*! 从机模式从机地址位宽 */
    uint32_t OwnAddrSize10bit;
    
		uint32_t SCLSEN;
	
} I2C_SlaveMode_InitTypeDef;
/**
  * @}
  */
/* Exported constants ---------------------------------------------------------------------------------*/
/** @defgroup I2C_Exported_Constants I2C Exported Constants
  * @{
  */

#define    I2C_MSP_DATA_DIRECTION_SLAVE_TO_MASTER              (0x0U << I2C_MSPSR_RW_Pos)
#define    I2C_MSP_DATA_DIRECTION_MASTER_TO_SLAVE              (0x1U << I2C_MSPSR_RW_Pos)

#define    I2C_MSP_DATA_BUFF_STATUS_FULL                       (0x0U << I2C_MSPSR_BF_Pos)
#define    I2C_MSP_DATA_BUFF_STATUS_EMPTY                      (0x1U << I2C_MSPSR_BF_Pos)

#define    I2C_SSP_MASTER_RESPOND_ACK                          (0x0U << I2C_MSPSR_ACKMO_Pos)
#define    I2C_SSP_MASTER_RESPOND_NACK                         (0x1U << I2C_MSPSR_ACKMO_Pos)

#define    I2C_SSP_DATA_DIRECTION_SLAVE_TO_MASTER              (0x1U << I2C_SSPSR_RW_Pos)
#define    I2C_SSP_DATA_DIRECTION_MASTER_TO_SLAVE              (0x0U << I2C_SSPSR_RW_Pos)

#define    I2C_SSP_DATA_TYPE_DATA                              (0x1U << I2C_SSPSR_DA_Pos)
#define    I2C_SSP_DATA_TYPE_ADDR                              (0x0U << I2C_SSPSR_DA_Pos)

#define    I2C_SSP_DATA_BUFF_STATUS_FULL                       (0x1U << I2C_SSPSR_BF_Pos)
#define    I2C_SSP_DATA_BUFF_STATUS_EMPTY                      (0x0U << I2C_SSPSR_BF_Pos)

/**
  * @}
  */
/* Exported functions ---------------------------------------------------------------------------------*/
/** @defgroup I2C_Exported_Functions I2C Exported Functions
  * @{
  */

/**
  * @brief    
  * @rmtoll   MSPCFGR    MSPEN    I2C_MasterMode_Enable
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Enable(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_MSPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    MSPEN    I2C_MasterModeIsEnabled
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterModeIsEnabled(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_MSPEN_Msk) == I2C_MSPCFGR_MSPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    MSPEN    I2C_MasterMode_Disable
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Disable(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_MSPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    TOEN    I2C_MasterMode_Enable_TimeOut
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Enable_TimeOut(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_TOEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    TOEN    I2C_MasterMode_IsEnabled_TimeOut
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabled_TimeOut(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_TOEN_Msk) == I2C_MSPCFGR_TOEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    TOEN    I2C_MasterMode_Disable_TimeOut
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Disable_TimeOut(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_TOEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    MSP_DMAEN    I2C_MasterModeEnableDMA
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterModeEnableDMA(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_MSP_DMAEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    MSP_DMAEN    I2C_MasterMode_IsEnabledDMA
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledDMA(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_MSP_DMAEN_Msk) == I2C_MSPCFGR_MSP_DMAEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    MSP_DMAEN    I2C_MasterMode_DisableDMA
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableDMA(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_MSP_DMAEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    AUTOEND    I2C_MasterMode_Enable_AutoStop
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Enable_AutoStop(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_AUTOEND_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    AUTOEND    I2C_MasterMode_IsEnabled_AutoStop
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabled_AutoStop(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_AUTOEND_Msk) == I2C_MSPCFGR_AUTOEND_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCFGR    AUTOEND    I2C_MasterMode_Disable_AutoStop
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Disable_AutoStop(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPCFGR, I2C_MSPCFGR_AUTOEND_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCR    RCEN    I2C_MasterMode_EnableReceive
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_EnableReceive(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPCR, I2C_MSPCR_RCEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCR    RCEN    I2C_MasterMode_IsEnabledReceive
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledReceive(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPCR, I2C_MSPCR_RCEN_Msk) == I2C_MSPCR_RCEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCR    RCEN    I2C_MasterMode_DisableReceive
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableReceive(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPCR, I2C_MSPCR_RCEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCR    PEN    I2C_MasterMode_Enable_I2C_Stop
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Enable_I2C_Stop(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPCR, I2C_MSPCR_PEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCR    RSEN    I2C_MasterMode_Enable_I2C_Rstart
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Enable_I2C_Rstart(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPCR, I2C_MSPCR_RSEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPCR    SEN    I2C_MasterMode_Enable_I2C_Start
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Enable_I2C_Start(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPCR, I2C_MSPCR_SEN_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    WCOLE    I2C_MasterMode_EnabledIT_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_EnabledIT_WriteConflict(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPIER, I2C_MSPIER_WCOLE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    WCOLE    I2C_MasterMode_IsEnabledIT_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledIT_WriteConflict(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPIER, I2C_MSPIER_WCOLE_Msk) == I2C_MSPIER_WCOLE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    WCOLE    I2C_MasterMode_DisableIT_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableIT_WriteConflict(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPIER, I2C_MSPIER_WCOLE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    OVTE    I2C_MasterMode_EnabledIT_TimeOut
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_EnabledIT_TimeOut(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPIER, I2C_MSPIER_OVTE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    OVTE    I2C_MasterMode_IsEnabledIT_TimeOut
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledIT_TimeOut(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPIER, I2C_MSPIER_OVTE_Msk) == I2C_MSPIER_OVTE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    OVTE    I2C_MasterMode_DisableIT_TimeOut
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableIT_TimeOut(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPIER, I2C_MSPIER_OVTE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    SE    I2C_MasterMode_EnabledIT_Start
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_EnabledIT_Start(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPIER, I2C_MSPIER_SE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    SE    I2C_MasterMode_IsEnabledIT_Start
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledIT_Start(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPIER, I2C_MSPIER_SE_Msk) == I2C_MSPIER_SE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    SE    I2C_MasterMode_DisableIT_Start
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableIT_Start(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPIER, I2C_MSPIER_SE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    PE    I2C_MasterMode_EnabledIT_Stop
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_EnabledIT_Stop(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPIER, I2C_MSPIER_PE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    PE    I2C_MasterMode_IsEnabledIT_Stop
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledIT_Stop(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPIER, I2C_MSPIER_PE_Msk) == I2C_MSPIER_PE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    PE    I2C_MasterMode_DisableIT_Stop
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableIT_Stop(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPIER, I2C_MSPIER_PE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    NACKE    I2C_MasterMode_EnabledIT_Nack
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_EnabledIT_Nack(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPIER, I2C_MSPIER_NACKE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    NACKE    I2C_MasterMode_IsEnabledIT_Nack
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledIT_Nack(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPIER, I2C_MSPIER_NACKE_Msk) == I2C_MSPIER_NACKE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    NACKE    I2C_MasterMode_DisableIT_Nack
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableIT_Nack(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPIER, I2C_MSPIER_NACKE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    TXIE    I2C_MasterMode_EnabledIT_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_EnabledIT_TransmitCompleted(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPIER, I2C_MSPIER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    TXIE    I2C_MasterMode_IsEnabledIT_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledIT_TransmitCompleted(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPIER, I2C_MSPIER_TXIE_Msk) == I2C_MSPIER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    TXIE    I2C_MasterMode_DisableIT_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableIT_TransmitCompleted(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPIER, I2C_MSPIER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    RXIE    I2C_MasterMode_EnabledIT_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_EnabledIT_ReceiveCompleted(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->MSPIER, I2C_MSPIER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    RXIE    I2C_MasterMode_IsEnabledIT_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsEnabledIT_ReceiveCompleted(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPIER, I2C_MSPIER_RXIE_Msk) == I2C_MSPIER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPIER    RXIE    I2C_MasterMode_DisableIT_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_DisableIT_ReceiveCompleted(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->MSPIER, I2C_MSPIER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPISR    WCOL    I2C_MasterMode_IsActiveFlag_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsActiveFlag_WriteConflict(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPISR, I2C_MSPISR_WCOL_Msk) == (I2C_MSPISR_WCOL_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPISR    WCOL    I2C_MasterMode_ClearFlag_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_ClearFlag_WriteConflict(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->MSPISR, I2C_MSPISR_WCOL_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPISR    OVT    I2C_MasterMode_IsActiveFlag_TimeOut
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsActiveFlag_TimeOut(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPISR, I2C_MSPISR_OVT_Msk) == (I2C_MSPISR_OVT_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPISR    OVT    I2C_MasterMode_ClearFlag_TimeOut
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_ClearFlag_TimeOut(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->MSPISR, I2C_MSPISR_OVT_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPISR    S    I2C_MasterMode_IsActiveFlag_Start
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsActiveFlag_Start(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPISR, I2C_MSPISR_S_Msk) == (I2C_MSPISR_S_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPISR    S    I2C_MasterMode_ClearFlag_Start
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_ClearFlag_Start(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->MSPISR, I2C_MSPISR_S_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPISR    P    I2C_MasterMode_IsActiveFlag_Stop
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsActiveFlag_Stop(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPISR, I2C_MSPISR_P_Msk) == (I2C_MSPISR_P_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPISR    P    I2C_MasterMode_ClearFlag_Stop
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_ClearFlag_Stop(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->MSPISR, I2C_MSPISR_P_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPISR    NACK    I2C_MasterMode_IsActiveFlag_Nack
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsActiveFlag_Nack(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPISR, I2C_MSPISR_NACK_Msk) == (I2C_MSPISR_NACK_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPISR    NACK    I2C_MasterMode_ClearFlag_Nack
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_ClearFlag_Nack(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->MSPISR, I2C_MSPISR_NACK_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPISR    TXIF    I2C_MasterMode_IsActiveFlag_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsActiveFlag_TransmitCompleted(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPISR, I2C_MSPISR_TXIF_Msk) == (I2C_MSPISR_TXIF_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPISR    TXIF    I2C_MasterMode_ClearFlag_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_ClearFlag_TransmitCompleted(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->MSPISR, I2C_MSPISR_TXIF_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPISR    RXIF    I2C_MasterMode_IsActiveFlag_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsActiveFlag_ReceiveCompleted(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPISR, I2C_MSPISR_RXIF_Msk) == (I2C_MSPISR_RXIF_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPISR    RXIF    I2C_MasterMode_ClearFlag_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_ClearFlag_ReceiveCompleted(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->MSPISR, I2C_MSPISR_RXIF_Msk);
}

/**
  * @brief    
  * @rmtoll   MSPSR    BUSY    I2C_MasterMode_IsActiveFlag_Busy
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_MasterMode_IsActiveFlag_Busy(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPSR, I2C_MSPSR_BUSY_Msk) == (I2C_MSPSR_BUSY_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPSR    RW    I2C_MasterMode_Get_DataDirection
  * @param    I2Cx I2C instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref I2C_MSP_DATA_DIRECTION_SLAVE_TO_MASTER
  *           @arg @ref I2C_MSP_DATA_DIRECTION_MASTER_TO_SLAVE
  */
__STATIC_INLINE uint32_t I2C_MasterMode_Get_DataDirection(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPSR, I2C_MSPSR_RW_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPSR    BF    I2C_MasterMode_GetDataDataBuffStatus
  * @param    I2Cx I2C instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref I2C_MSP_DATA_BUFF_STATUS_FULL
  *           @arg @ref I2C_MSP_DATA_BUFF_STATUS_EMPTY
  */
__STATIC_INLINE uint32_t I2C_MasterMode_GetDataDataBuffStatus(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPSR, I2C_MSPSR_BF_Msk));
}

/**
  * @brief    
  * @rmtoll   MSPSR    ACKMO    I2C_MasterMode_SetMasterRespond
  * @param    I2Cx I2C instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref I2C_SSP_MASTER_RESPOND_ACK
  *           @arg @ref I2C_SSP_MASTER_RESPOND_NACK
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_SetMasterRespond(I2C_Type* I2Cx, uint32_t mode)
{
    MODIFY_REG(I2Cx->MSPSR, I2C_MSPSR_ACKMO_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   MSPBGR    MSPBGRH    I2C_MasterMode_Set_SCL_HighWidth
  * @param    I2Cx I2C instance
  * @param    times 
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Set_SCL_HighWidth(I2C_Type* I2Cx, uint32_t times)
{
    MODIFY_REG(I2Cx->MSPBGR, (0x1ffU << 16U), (times << 16U));
}

/**
  * @brief    
  * @rmtoll   MSPBGR    MSPBGRH    I2C_MasterMode_Get_SCL_HighWidth
  * @param    I2Cx I2C instance
  * @retval   
  */
__STATIC_INLINE uint32_t I2C_MasterMode_Get_SCL_HighWidth(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPBGR, 0x1ffU) >> 16U);
}

/**
  * @brief    
  * @rmtoll   MSPBGR    MSPBGRL    I2C_MasterMode_Set_SCL_LowWidth
  * @param    I2Cx I2C instance
  * @param    times 
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Set_SCL_LowWidth(I2C_Type* I2Cx, uint32_t times)
{
    MODIFY_REG(I2Cx->MSPBGR, (0x1ffU << 0U), (times << 0U));
}

/**
  * @brief    
  * @rmtoll   MSPBGR    MSPBGRL    I2C_MasterMode_Get_SCL_LowWidth
  * @param    I2Cx I2C instance
  * @retval   
  */
__STATIC_INLINE uint32_t I2C_MasterMode_Get_SCL_LowWidth(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPBGR, 0x1ffU) >> 0U);
}

/**
  * @brief    
  * @rmtoll   MSPBUF    MSPBUFF    I2C_MasterMode_WriteDataBuff
  * @param    I2Cx I2C instance
  * @param    data 
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_WriteDataBuff(I2C_Type* I2Cx, uint32_t data)
{
    MODIFY_REG(I2Cx->MSPBUF, (0xffU << 0U), (data << 0U));
}

/**
  * @brief    
  * @rmtoll   MSPBUF    MSPBUFF    I2C_MasterMode_ReadDataBuff
  * @param    I2Cx I2C instance
  * @retval   
  */
__STATIC_INLINE uint32_t I2C_MasterMode_ReadDataBuff(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPBUF, 0xffU) >> 0U);
}

/**
  * @brief    
  * @rmtoll   MSPTCR    SDAHD    I2C_MasterMode_Set_SDA_HoldTime
  * @param    I2Cx I2C instance
  * @param    times 
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Set_SDA_HoldTime(I2C_Type* I2Cx, uint32_t times)
{
    MODIFY_REG(I2Cx->MSPTCR, (0x1ffU << 0U), (times << 0U));
}

/**
  * @brief    
  * @rmtoll   MSPTCR    SDAHD    I2C_MasterMode_Get_SDA_HoldTime
  * @param    I2Cx I2C instance
  * @retval   
  */
__STATIC_INLINE uint32_t I2C_MasterMode_Get_SDA_HoldTime(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPTCR, 0x1ffU) >> 0U);
}

/**
  * @brief    
  * @rmtoll   MSPTOR    TIMEOUT    I2C_MasterMode_Set_SlaveSCL_TimeOut
  * @param    I2Cx I2C instance
  * @param    times 
  * @retval   None
  */
__STATIC_INLINE void I2C_MasterMode_Set_SlaveSCL_TimeOut(I2C_Type* I2Cx, uint32_t times)
{
    MODIFY_REG(I2Cx->MSPTOR, (0xfffU << 0U), (times << 0U));
}

/**
  * @brief    
  * @rmtoll   MSPTOR    TIMEOUT    I2C_MasterMode_Get_SlaveSCL_TimeOut
  * @param    I2Cx I2C instance
  * @retval   
  */
__STATIC_INLINE uint32_t I2C_MasterMode_Get_SlaveSCL_TimeOut(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->MSPTOR, 0xfffU) >> 0U);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SCLSEN    I2C_SlaveMode_EnableSCLStretching
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnableSCLStretching(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPCR, I2C_SSPCR_SCLSEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SCLSEN    I2C_SlaveMode_IsEnabled_SCLStretching
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabled_SCLStretching(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPCR, I2C_SSPCR_SCLSEN_Msk) == I2C_SSPCR_SCLSEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SCLSEN    I2C_SlaveMode_Disable_SCLStretching
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Disable_SCLStretching(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPCR, I2C_SSPCR_SCLSEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SSP_DMAEN    I2C_SlaveMode_Enable_DAM
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Enable_DAM(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPCR, I2C_SSPCR_SSP_DMAEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SSP_DMAEN    I2C_SlaveMode_IsEnabled_DAM
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabled_DAM(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPCR, I2C_SSPCR_SSP_DMAEN_Msk) == I2C_SSPCR_SSP_DMAEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SSP_DMAEN    I2C_SlaveMode_Disable_DAM
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Disable_DAM(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPCR, I2C_SSPCR_SSP_DMAEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    ACKEN    I2C_SlaveMode_Enable_ACK_Respond
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Enable_ACK_Respond(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPCR, I2C_SSPCR_ACKEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    ACKEN    I2C_SlaveMode_IsEnabled_ACK_Respond
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabled_ACK_Respond(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPCR, I2C_SSPCR_ACKEN_Msk) == I2C_SSPCR_ACKEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    ACKEN    I2C_SlaveMode_Disable_ACK_Respond
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Disable_ACK_Respond(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPCR, I2C_SSPCR_ACKEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SDAO_DLYEN    I2C_SlaveMode_Enable_SDAStretching
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Enable_SDAStretching(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPCR, I2C_SSPCR_SDAO_DLYEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SDAO_DLYEN    I2C_SlaveMode_IsEnabled_SDAStretching
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabled_SDAStretching(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPCR, I2C_SSPCR_SDAO_DLYEN_Msk) == I2C_SSPCR_SDAO_DLYEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SDAO_DLYEN    I2C_SlaveMode_Disable_SDAStretching
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Disable_SDAStretching(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPCR, I2C_SSPCR_SDAO_DLYEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SCLI_ANFEN    I2C_SlaveMode_Enable_SCLAnalogFilter
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Enable_SCLAnalogFilter(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPCR, I2C_SSPCR_SCLI_ANFEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SCLI_ANFEN    I2C_SlaveMode_IsEnabled_SCLAnalogFilter
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabled_SCLAnalogFilter(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPCR, I2C_SSPCR_SCLI_ANFEN_Msk) == I2C_SSPCR_SCLI_ANFEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SCLI_ANFEN    I2C_SlaveMode_Disable_SCLAnalogFilter
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Disable_SCLAnalogFilter(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPCR, I2C_SSPCR_SCLI_ANFEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    A10EN    I2C_SlaveMode_Enable_ADDR10Bit
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Enable_ADDR10Bit(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPCR, I2C_SSPCR_A10EN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    A10EN    I2C_SlaveMode_IsEnabled_ADDR10Bit
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabled_ADDR10Bit(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPCR, I2C_SSPCR_A10EN_Msk) == I2C_SSPCR_A10EN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    A10EN    I2C_SlaveMode_Disable_ADDR10Bit
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Disable_ADDR10Bit(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPCR, I2C_SSPCR_A10EN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SSPEN    I2C_SlaveMode_Enable
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Enable(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPCR, I2C_SSPCR_SSPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SSPEN    I2C_SlaveMode_IsEnabled
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabled(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPCR, I2C_SSPCR_SSPEN_Msk) == I2C_SSPCR_SSPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPCR    SSPEN    I2C_SlaveMode_Disable
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Disable(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPCR, I2C_SSPCR_SSPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    ADEE    I2C_SlaveMode_EnabledIT_AddrError
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnabledIT_AddrError(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPIER, I2C_SSPIER_ADEE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    ADEE    I2C_SlaveMode_IsEnabledIT_AddrError
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabledIT_AddrError(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPIER, I2C_SSPIER_ADEE_Msk) == I2C_SSPIER_ADEE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    ADEE    I2C_SlaveMode_DisableIT_AddrError
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_DisableIT_AddrError(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPIER, I2C_SSPIER_ADEE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    SE    I2C_SlaveMode_EnabledIT_Start
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnabledIT_Start(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPIER, I2C_SSPIER_SE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    SE    I2C_SlaveMode_IsEnabledIT_Start
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabledIT_Start(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPIER, I2C_SSPIER_SE_Msk) == I2C_SSPIER_SE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    SE    I2C_SlaveMode_DisableIT_Start
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_DisableIT_Start(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPIER, I2C_SSPIER_SE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    PE    I2C_SlaveMode_EnabledIT_Stop
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnabledIT_Stop(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPIER, I2C_SSPIER_PE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    PE    I2C_SlaveMode_IsEnabledIT_Stop
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabledIT_Stop(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPIER, I2C_SSPIER_PE_Msk) == I2C_SSPIER_PE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    PE    I2C_SlaveMode_DisableIT_Stop
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_DisableIT_Stop(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPIER, I2C_SSPIER_PE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    WCOLE    I2C_SlaveMode_EnabledIT_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnabledIT_WriteConflict(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPIER, I2C_SSPIER_WCOLE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    WCOLE    I2C_SlaveMode_IsEnabledIT_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabledIT_WriteConflict(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPIER, I2C_SSPIER_WCOLE_Msk) == I2C_SSPIER_WCOLE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    WCOLE    I2C_SlaveMode_DisableIT_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_DisableIT_WriteConflict(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPIER, I2C_SSPIER_WCOLE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    SSPOVE    I2C_SlaveMode_EnabledIT_BuffOverFlow
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnabledIT_BuffOverFlow(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPIER, I2C_SSPIER_SSPOVE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    SSPOVE    I2C_SlaveMode_IsEnabledIT_BuffOverFlow
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabledIT_BuffOverFlow(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPIER, I2C_SSPIER_SSPOVE_Msk) == I2C_SSPIER_SSPOVE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    SSPOVE    I2C_SlaveMode_DisableIT_BuffOverFlow
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_DisableIT_BuffOverFlow(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPIER, I2C_SSPIER_SSPOVE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    ADME    I2C_SlaveMode_EnabledIT_AddrMatched
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnabledIT_AddrMatched(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPIER, I2C_SSPIER_ADME_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    ADME    I2C_SlaveMode_IsEnabledIT_AddrMatched
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabledIT_AddrMatched(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPIER, I2C_SSPIER_ADME_Msk) == I2C_SSPIER_ADME_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    ADME    I2C_SlaveMode_DisableIT_AddrMatched
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_DisableIT_AddrMatched(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPIER, I2C_SSPIER_ADME_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    TXIE    I2C_SlaveMode_EnabledIT_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnabledIT_TransmitCompleted(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPIER, I2C_SSPIER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    TXIE    I2C_SlaveMode_IsEnabledIT_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabledIT_TransmitCompleted(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPIER, I2C_SSPIER_TXIE_Msk) == I2C_SSPIER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    TXIE    I2C_SlaveMode_DisableIT_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_DisableIT_TransmitCompleted(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPIER, I2C_SSPIER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    RXIE    I2C_SlaveMode_EnabledIT_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_EnabledIT_ReceiveCompleted(I2C_Type* I2Cx)
{
    SET_BIT(I2Cx->SSPIER, I2C_SSPIER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    RXIE    I2C_SlaveMode_IsEnabledIT_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsEnabledIT_ReceiveCompleted(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPIER, I2C_SSPIER_RXIE_Msk) == I2C_SSPIER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPIER    RXIE    I2C_SlaveMode_DisableIT_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_DisableIT_ReceiveCompleted(I2C_Type* I2Cx)
{
    CLEAR_BIT(I2Cx->SSPIER, I2C_SSPIER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPISR    ADE    I2C_SlaveMode_IsActiveFlag_AddrError
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_AddrError(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPISR, I2C_SSPISR_ADE_Msk) == (I2C_SSPISR_ADE_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPISR    ADE    I2C_SlaveMode_ClearFlag_AddrError
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_ClearFlag_AddrError(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->SSPISR, I2C_SSPISR_ADE_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPISR    S    I2C_SlaveMode_IsActiveFlag_Start
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_Start(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPISR, I2C_SSPISR_S_Msk) == (I2C_SSPISR_S_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPISR    S    I2C_SlaveMode_ClearFlag_Start
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_ClearFlag_Start(I2C_Type* I2Cx)
{
    READ_REG(I2Cx->SSPISR); 
}

/**
  * @brief    
  * @rmtoll   SSPISR    P    I2C_SlaveMode_IsActiveFlag_Stop
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_Stop(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPISR, I2C_SSPISR_P_Msk) == (I2C_SSPISR_P_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPISR    P    I2C_SlaveMode_ClearFlag_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_ClearFlag_WriteConflict(I2C_Type* I2Cx)
{
    READ_REG(I2Cx->SSPISR); 
}

/**
  * @brief    
  * @rmtoll   SSPISR    WCOL    I2C_SlaveMode_IsActiveFlag_WriteConflict
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_WriteConflict(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPISR, I2C_SSPISR_WCOL_Msk) == (I2C_SSPISR_WCOL_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPISR    WCOL    I2C_SlaveMode_ClearFlag_BuffOverFlow
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_ClearFlag_BuffOverFlow(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->SSPISR, I2C_SSPISR_WCOL_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPISR    SSPOV    I2C_SlaveMode_IsActiveFlag_BuffOverFlow
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_BuffOverFlow(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPISR, I2C_SSPISR_SSPOV_Msk) == (I2C_SSPISR_SSPOV_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPISR    ADM    I2C_SlaveMode_IsActiveFlag_AddrMatched
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_AddrMatched(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPISR, I2C_SSPISR_ADM_Msk) == (I2C_SSPISR_ADM_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPISR    ADM    I2C_SlaveMode_ClearFlag_AddrMatched
  * @param    I2Cx I2C instance
  * @retval   Returned value can be one of the following values:
  */
__STATIC_INLINE void I2C_SlaveMode_ClearFlag_AddrMatched(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->SSPISR, I2C_SSPISR_ADM_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPISR    TXI    I2C_SlaveMode_IsActiveFlag_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_TransmitCompleted(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPISR, I2C_SSPISR_TXI_Msk) == (I2C_SSPISR_TXI_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPISR    TXI    I2C_SlaveMode_ClearFlag_TransmitCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_ClearFlag_TransmitCompleted(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->SSPISR, I2C_SSPISR_TXI_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPISR    RXI    I2C_SlaveMode_IsActiveFlag_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_ReceiveCompleted(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPISR, I2C_SSPISR_RXI_Msk) == (I2C_SSPISR_RXI_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPISR    RXI    I2C_SlaveMode_ClearFlag_ReceiveCompleted
  * @param    I2Cx I2C instance
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_ClearFlag_ReceiveCompleted(I2C_Type* I2Cx)
{
    WRITE_REG(I2Cx->SSPISR, I2C_SSPISR_RXI_Msk);
}

/**
  * @brief    
  * @rmtoll   SSPSR    BUSY    I2C_SlaveMode_IsActiveFlag_Busy
  * @param    I2Cx I2C instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_IsActiveFlag_Busy(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPSR, I2C_SSPSR_BUSY_Msk) == (I2C_SSPSR_BUSY_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPSR    RW    I2C_SlaveMode_Get_DataDirection
  * @param    I2Cx I2C instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref I2C_SSP_DATA_DIRECTION_SLAVE_TO_MASTER
  *           @arg @ref I2C_SSP_DATA_DIRECTION_MASTER_TO_SLAVE
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_Get_DataDirection(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPSR, I2C_SSPSR_RW_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPSR    DA    I2C_SlaveMode_Get_DataType
  * @param    I2Cx I2C instance
  * @param    I2Cx I2C instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref I2C_SSP_DATA_TYPE_DATA
  *           @arg @ref I2C_SSP_DATA_TYPE_ADDR
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_Get_DataType(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPSR, I2C_SSPSR_DA_Msk));
}

/**
  * @brief    
  * @rmtoll   SSPBUF        I2C_SlaveMode_WriteDataBuff
  * @param    I2Cx I2C instance
  * @param    data 
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_WriteDataBuff(I2C_Type* I2Cx, uint32_t data)
{
    MODIFY_REG(I2Cx->SSPBUF, (0xffU << 0U), (data << 0U));
}

/**
  * @brief    
  * @rmtoll   SSPBUF        I2C_SlaveMode_ReadDataBuff
  * @param    I2Cx I2C instance
  * @retval   
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_ReadDataBuff(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPBUF, 0xffU) >> 0U);
}

/**
  * @brief    
  * @rmtoll   SSPADR        I2C_SlaveMode_Set_SlaveAddr
  * @param    I2Cx I2C instance
  * @param    addr 
  * @retval   None
  */
__STATIC_INLINE void I2C_SlaveMode_Set_SlaveAddr(I2C_Type* I2Cx, uint32_t addr)
{
    MODIFY_REG(I2Cx->SSPADR, (0x3ffU << 0U), (addr << 0U));
}

/**
  * @brief    
  * @rmtoll   SSPADR        I2C_SlaveMode_Get_SlaveAddr
  * @param    I2Cx I2C instance
  * @retval   
  */
__STATIC_INLINE uint32_t I2C_SlaveMode_Get_SlaveAddr(I2C_Type* I2Cx)
{
    return (uint32_t)(READ_BIT(I2Cx->SSPADR, 0x3ffU) >> 0U);
}

/**
  * @}
  */

/** @defgroup I2C_EF_Init Initialization and de-initialization functions
  * @{
  */
ErrorStatus I2C_DeInit(I2C_Type *I2Cx);
void I2C_SlaveMode_StructInit(I2C_SlaveMode_InitTypeDef *I2C_InitStruct);
void I2C_MasterMode_StructInit(I2C_MasterMode_InitTypeDef *I2C_InitStruct);
ErrorStatus I2C_SlaveMode_Init(I2C_Type *I2cx, I2C_SlaveMode_InitTypeDef *I2C_InitStruct);
ErrorStatus I2C_MasterMode_Init(I2C_Type *I2Cx, I2C_MasterMode_InitTypeDef *I2C_InitStruct);

/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __FM33LC0XX_I2C_H*/

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-15**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
