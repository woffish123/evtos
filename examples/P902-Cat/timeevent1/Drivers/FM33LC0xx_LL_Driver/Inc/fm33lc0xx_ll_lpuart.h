/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_lpuart.h
  * @author  FMSH Application Team
  * @brief   Head file of LPUART LL Module
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
#ifndef __FM33LC0XX_LPUART_H
#define __FM33LC0XX_LPUART_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
/** @addtogroup FM33LC0XX_Driver_LPUART
  * @{
  */
/* Exported types -------------------------------------------------------------------------------------*/
/** @defgroup LPUART_ES_INIT LPUART Exported Init structures
  * @{
  */

/**
  * @brief LL LPUART Init Sturcture definition
  */
typedef struct
{
    /*! 时钟源选择 */
    uint32_t ClockSrc; 
    /*! 通讯波特率*/
    uint32_t BaudRate;                  
    /*!  数据位宽*/
    uint32_t DataWidth;                 
    /*! 停止位 */
    uint32_t StopBits;                  
    /*! 奇偶校验 */
    uint32_t Parity;                    
    /*! 传输方向 */
    uint32_t TransferDirection;               

} LPUART_InitTypeDef;

/**
  * @}
  */
/* Exported constants ---------------------------------------------------------------------------------*/
/** @defgroup LPUART_Exported_Constants LPUART Exported Constants
  * @{
  */

#define    LPUART_WAKE_UP_DIRECT                               (0x0U << LPUART_CSR_WKBYTECFG_Pos)
#define    LPUART_WAKE_UP_AFTER_CHECK                          (0x1U << LPUART_CSR_WKBYTECFG_Pos)

#define    LPUART_WAKE_UP_EVENT_START                          (0x0U << LPUART_CSR_RXEV_Pos)
#define    LPUART_WAKE_UP_EVENT_RECV_1_BYTE                    (0x1U << LPUART_CSR_RXEV_Pos)
#define    LPUART_WAKE_UP_EVENT_RECV_MATCH_BYTE                (0x2U << LPUART_CSR_RXEV_Pos)
#define    LPUART_WAKE_UP_EVENT_RXD_FALLING                    (0x3U << LPUART_CSR_RXEV_Pos)

#define    LPUART_TXRX_STANDARD                                (0x0U << LPUART_CSR_IOSWAP_Pos)
#define    LPUART_TXRX_SWAPPED                                 (0x1U << LPUART_CSR_IOSWAP_Pos)

#define    LPUART_BITORDER_LSBFIRST                            (0x0U << LPUART_CSR_BITORD_Pos)
#define    LPUART_BITORDER_MSBFIRST                            (0x1U << LPUART_CSR_BITORD_Pos)

#define    LPUART_STOP_1BITS                                   (0x0U << LPUART_CSR_STOPCFG_Pos)
#define    LPUART_STOP_2BITS                                   (0x1U << LPUART_CSR_STOPCFG_Pos)

#define    LPUART_DATAWIDTH_7B                                 (0x0U << LPUART_CSR_PDSEL_Pos)
#define    LPUART_DATAWIDTH_8B                                 (0x1U << LPUART_CSR_PDSEL_Pos)
#define    LPUART_DATAWIDTH_9B                                 (0x2U << LPUART_CSR_PDSEL_Pos)
#define    LPUART_DATAWIDTH_6B                                 (0x3U << LPUART_CSR_PDSEL_Pos)

#define    LPUART_PARITY_NONE                                  (0x0U << LPUART_CSR_PARITY_Pos)
#define    LPUART_PARITY_EVEN                                  (0x1U << LPUART_CSR_PARITY_Pos)
#define    LPUART_PARITY_ODD                                   (0x2U << LPUART_CSR_PARITY_Pos)

#define    LPUART_RX_POLARITY_NORMAL                           (0x0U << LPUART_CSR_RXPOL_Pos)
#define    LPUART_RX_POLARITY_REVERSE                          (0x1U << LPUART_CSR_RXPOL_Pos)

#define    LPUART_TX_POLARITY_NORMAL                           (0x0U << LPUART_CSR_TXPOL_Pos)
#define    LPUART_TX_POLARITY_REVERSE                          (0x1U << LPUART_CSR_TXPOL_Pos)

#define    LPUART_BAUDRATE_9600                                (0x0U << LPUART_BMR_BAUD_Pos)
#define    LPUART_BAUDRATE_4800                                (0x1U << LPUART_BMR_BAUD_Pos)
#define    LPUART_BAUDRATE_2400                                (0x2U << LPUART_BMR_BAUD_Pos)
#define    LPUART_BAUDRATE_1200                                (0x3U << LPUART_BMR_BAUD_Pos)
#define    LPUART_BAUDRATE_600                                 (0x4U << LPUART_BMR_BAUD_Pos)
#define    LPUART_BAUDRATE_300                                 (0x5U << LPUART_BMR_BAUD_Pos)

#define    LPUART_DIRECTION_NONE                               0x00000000U                          
#define    LPUART_DIRECTION_RX                                 LPUART_CSR_RXEN                       
#define    LPUART_DIRECTION_TX                                 LPUART_CSR_TXEN                      
#define    LPUART_DIRECTION_TX_RX                              (LPUART_CSR_RXEN|LPUART_CSR_TXEN)
/**
  * @}
  */
/* Exported functions ---------------------------------------------------------------------------------*/
/** @defgroup LPUART_Exported_Functions LPUART Exported Functions
  * @{
  */

/**
  * @brief    ヨLPUARTBUSY蹇缃璧
  * @rmtoll   CSR    BUSY    LPUART_IsActiveFlag_BUSY
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_BUSY(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_BUSY_Msk) == (LPUART_CSR_BUSY_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    WKBYTECFG    LPUART_SetDataRecvedWakeupMode
  * @param    LPUARTx LPUART instance
  * @param    Mode This parameter can be one of the following values:
  *           @arg @ref LPUART_WAKE_UP_DIRECT
  *           @arg @ref LPUART_WAKE_UP_AFTER_CHECK
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetDataRecvedWakeupMode(LPUART_Type *LPUARTx, uint32_t Mode)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_WKBYTECFG_Msk, Mode);
}

/**
  * @brief    
  * @rmtoll   CSR    WKBYTECFG    LPUART_GetDataRecvedWakeupMode
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_WAKE_UP_DIRECT
  *           @arg @ref LPUART_WAKE_UP_AFTER_CHECK
  */
__STATIC_INLINE uint32_t LPUART_GetDataRecvedWakeupMode(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_WKBYTECFG_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    RXEV    LPUART_SetWakeupEvent
  * @param    LPUARTx LPUART instance
  * @param    Event This parameter can be one of the following values:
  *           @arg @ref LPUART_WAKE_UP_EVENT_START
  *           @arg @ref LPUART_WAKE_UP_EVENT_RECV_1_BYTE
  *           @arg @ref LPUART_WAKE_UP_EVENT_RECV_MATCH_BYTE
  *           @arg @ref LPUART_WAKE_UP_EVENT_RXD_FALLING
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetWakeupEvent(LPUART_Type *LPUARTx, uint32_t Event)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_RXEV_Msk, Event);
}

/**
  * @brief    
  * @rmtoll   CSR    RXEV    LPUART_GetWakeupEvent
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_WAKE_UP_EVENT_START
  *           @arg @ref LPUART_WAKE_UP_EVENT_RECV_1_BYTE
  *           @arg @ref LPUART_WAKE_UP_EVENT_RECV_MATCH_BYTE
  *           @arg @ref LPUART_WAKE_UP_EVENT_RXD_FALLING
  */
__STATIC_INLINE uint32_t LPUART_GetWakeupEvent(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_RXEV_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    IOSWAP    LPUART_SetTXRXSwap
  * @param    LPUARTx LPUART instance
  * @param    SwapConfig This parameter can be one of the following values:
  *           @arg @ref LPUART_TXRX_STANDARD
  *           @arg @ref LPUART_TXRX_SWAPPED
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetTXRXSwap(LPUART_Type *LPUARTx, uint32_t SwapConfig)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_IOSWAP_Msk, SwapConfig);
}

/**
  * @brief    
  * @rmtoll   CSR    IOSWAP    LPUART_GetTXRXSwap
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_TXRX_STANDARD
  *           @arg @ref LPUART_TXRX_SWAPPED
  */
__STATIC_INLINE uint32_t LPUART_GetTXRXSwap(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_IOSWAP_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    DMATXIFCFG    LPUART_EnableDMATxIF
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_EnableDMATxIF(LPUART_Type *LPUARTx)
{
    SET_BIT(LPUARTx->CSR, LPUART_CSR_DMATXIFCFG_Msk);
}

/**
  * @brief    
  * @rmtoll   CSR    DMATXIFCFG    LPUART_DisableDMATxIF
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_DisableDMATxIF(LPUART_Type *LPUARTx)
{
    CLEAR_BIT(LPUARTx->CSR, LPUART_CSR_DMATXIFCFG_Msk);
}

/**
  * @brief    
  * @rmtoll   CSR    DMATXIFCFG    LPUART_IsEnabledIT_DMATxIF
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsEnabledIT_DMATxIF(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_DMATXIFCFG_Msk) == LPUART_CSR_DMATXIFCFG_Msk);
}

/**
  * @brief    
  * @rmtoll   CSR    BITORD    LPUART_SetTransferBitOrder
  * @param    LPUARTx LPUART instance
  * @param    BitOrder This parameter can be one of the following values:
  *           @arg @ref LPUART_BITORDER_LSBFIRST
  *           @arg @ref LPUART_BITORDER_MSBFIRST
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetTransferBitOrder(LPUART_Type *LPUARTx, uint32_t BitOrder)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_BITORD_Msk, BitOrder);
}

/**
  * @brief    
  * @rmtoll   CSR    BITORD    LPUART_GetTransferBitOrder
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_BITORDER_LSBFIRST
  *           @arg @ref LPUART_BITORDER_MSBFIRST
  */
__STATIC_INLINE uint32_t LPUART_GetTransferBitOrder(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_BITORD_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    STOPCFG    LPUART_SetStopBitsLength
  * @param    LPUARTx LPUART instance
  * @param    StopBits This parameter can be one of the following values:
  *           @arg @ref LPUART_STOP_1BITS
  *           @arg @ref LPUART_STOP_2BITS
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetStopBitsLength(LPUART_Type *LPUARTx, uint32_t StopBits)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_STOPCFG_Msk, StopBits);
}

/**
  * @brief    
  * @rmtoll   CSR    STOPCFG    LPUART_GetStopBitsLength
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_STOP_1BITS
  *           @arg @ref LPUART_STOP_2BITS
  */
__STATIC_INLINE uint32_t LPUART_GetStopBitsLength(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_STOPCFG_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    PDSEL    LPUART_SetDataWidth
  * @param    LPUARTx LPUART instance
  * @param    DataWidth This parameter can be one of the following values:
  *           @arg @ref LPUART_DATAWIDTH_7B
  *           @arg @ref LPUART_DATAWIDTH_8B
  *           @arg @ref LPUART_DATAWIDTH_9B
  *           @arg @ref LPUART_DATAWIDTH_6B
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetDataWidth(LPUART_Type *LPUARTx, uint32_t DataWidth)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_PDSEL_Msk, DataWidth);
}

/**
  * @brief    
  * @rmtoll   CSR    PDSEL    LPUART_GetDataWidth
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_DATAWIDTH_7B
  *           @arg @ref LPUART_DATAWIDTH_8B
  *           @arg @ref LPUART_DATAWIDTH_9B
  *           @arg @ref LPUART_DATAWIDTH_6B
  */
__STATIC_INLINE uint32_t LPUART_GetDataWidth(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_PDSEL_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    PARITY    LPUART_SetParity
  * @param    LPUARTx LPUART instance
  * @param    Parity This parameter can be one of the following values:
  *           @arg @ref LPUART_PARITY_NONE
  *           @arg @ref LPUART_PARITY_EVEN
  *           @arg @ref LPUART_PARITY_ODD
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetParity(LPUART_Type *LPUARTx, uint32_t Parity)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_PARITY_Msk, Parity);
}

/**
  * @brief    
  * @rmtoll   CSR    PARITY    LPUART_GetParity
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_PARITY_NONE
  *           @arg @ref LPUART_PARITY_EVEN
  *           @arg @ref LPUART_PARITY_ODD
  */
__STATIC_INLINE uint32_t LPUART_GetParity(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_PARITY_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    RXPOL    LPUART_SetRxPolarity
  * @param    LPUARTx LPUART instance
  * @param    Polarity This parameter can be one of the following values:
  *           @arg @ref LPUART_RX_POLARITY_NORMAL
  *           @arg @ref LPUART_RX_POLARITY_REVERSE
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetRxPolarity(LPUART_Type *LPUARTx, uint32_t Polarity)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_RXPOL_Msk, Polarity);
}

/**
  * @brief    
  * @rmtoll   CSR    RXPOL    LPUART_GetRxPolarity
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_RX_POLARITY_NORMAL
  *           @arg @ref LPUART_RX_POLARITY_REVERSE
  */
__STATIC_INLINE uint32_t LPUART_GetRxPolarity(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_RXPOL_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    TXPOL    LPUART_SetTxPolarity
  * @param    LPUARTx LPUART instance
  * @param    Polarity This parameter can be one of the following values:
  *           @arg @ref LPUART_TX_POLARITY_NORMAL
  *           @arg @ref LPUART_TX_POLARITY_REVERSE
  * @retval   None
  */
  
__STATIC_INLINE void LPUART_SetTxPolarity(LPUART_Type *LPUARTx, uint32_t Polarity)
{
    MODIFY_REG(LPUARTx->CSR, LPUART_CSR_TXPOL_Msk, Polarity);
}
/**
  * @brief    
  * @rmtoll   CSR    TXPOL    LPUART_GetTxPolarity
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_TX_POLARITY_NORMAL
  *           @arg @ref LPUART_TX_POLARITY_REVERSE
  */
__STATIC_INLINE uint32_t LPUART_GetTxPolarity(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_TXPOL_Msk));
}

/**
  * @brief    
  * @rmtoll   CSR    RXEN    LPUART_EnabledRx
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_EnabledRx(LPUART_Type *LPUARTx)
{
    SET_BIT(LPUARTx->CSR, LPUART_CSR_RXEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CSR    RXEN    LPUART_IsEnabledRx
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsEnabledRx(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_RXEN_Msk) == LPUART_CSR_RXEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CSR    RXEN    LPUART_DisableRx
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_DisableRx(LPUART_Type *LPUARTx)
{
    CLEAR_BIT(LPUARTx->CSR, LPUART_CSR_RXEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CSR    TXEN    LPUART_EnabledTx
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_EnabledTx(LPUART_Type *LPUARTx)
{
    SET_BIT(LPUARTx->CSR, LPUART_CSR_TXEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CSR    TXEN    LPUART_IsEnabledTx
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsEnabledTx(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->CSR, LPUART_CSR_TXEN_Msk) == LPUART_CSR_TXEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CSR    TXEN    LPUART_DisableTx
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_DisableTx(LPUART_Type *LPUARTx)
{
    CLEAR_BIT(LPUARTx->CSR, LPUART_CSR_TXEN_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXEV_IE    LPUART_EnabledIT_RXEV
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_EnabledIT_RXEV(LPUART_Type *LPUARTx)
{
    SET_BIT(LPUARTx->IER, LPUART_IER_RXEV_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXEV_IE    LPUART_IsEnabledIT_RXEV
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsEnabledIT_RXEV(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->IER, LPUART_IER_RXEV_IE_Msk) == LPUART_IER_RXEV_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXEV_IE    LPUART_DisableIT_RXEV
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_DisableIT_RXEV(LPUART_Type *LPUARTx)
{
    CLEAR_BIT(LPUARTx->IER, LPUART_IER_RXEV_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXERR_IE    LPUART_EnabledIT_RXERR
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_EnabledIT_RXERR(LPUART_Type *LPUARTx)
{
    SET_BIT(LPUARTx->IER, LPUART_IER_RXERR_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXERR_IE    LPUART_IsEnabledIT_RXERR
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsEnabledIT_RXERR(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->IER, LPUART_IER_RXERR_IE_Msk) == LPUART_IER_RXERR_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXERR_IE    LPUART_DisableIT_RXERR
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_DisableIT_RXERR(LPUART_Type *LPUARTx)
{
    CLEAR_BIT(LPUARTx->IER, LPUART_IER_RXERR_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXBF_IE    LPUART_EnabledIT_RXBF
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_EnabledIT_RXBF(LPUART_Type *LPUARTx)
{
    SET_BIT(LPUARTx->IER, LPUART_IER_RXBF_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXBF_IE    LPUART_IsEnabledIT_RXBF
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsEnabledIT_RXBF(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->IER, LPUART_IER_RXBF_IE_Msk) == LPUART_IER_RXBF_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXBF_IE    LPUART_DisableIT_RXBF
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_DisableIT_RXBF(LPUART_Type *LPUARTx)
{
    CLEAR_BIT(LPUARTx->IER, LPUART_IER_RXBF_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXBE_IE    LPUART_EnabledIT_RXBE
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_EnabledIT_TXBE(LPUART_Type *LPUARTx)
{
    SET_BIT(LPUARTx->IER, LPUART_IER_TXBE_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXBE_IE    LPUART_IsEnabledIT_RXBE
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsEnabledIT_TXBE(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->IER, LPUART_IER_TXBE_IE_Msk) == LPUART_IER_TXBE_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXBE_IE    LPUART_DisableIT_RXBE
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_DisableIT_TXBE(LPUART_Type *LPUARTx)
{
    CLEAR_BIT(LPUARTx->IER, LPUART_IER_TXBE_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    TXSE_IE    LPUART_EnabledIT_TXSE
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_EnabledIT_TXSE(LPUART_Type *LPUARTx)
{
    SET_BIT(LPUARTx->IER, LPUART_IER_TXSE_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    TXSE_IE    LPUART_IsEnabledIT_TXSE
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsEnabledIT_TXSE(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->IER, LPUART_IER_TXSE_IE_Msk) == LPUART_IER_TXSE_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    TXSE_IE    LPUART_DisableIT_TXSE
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_DisableIT_TXSE(LPUART_Type *LPUARTx)
{
    CLEAR_BIT(LPUARTx->IER, LPUART_IER_TXSE_IE_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    RXEVF    LPUART_IsActiveFlag_RXEV
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_RXEV(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->ISR, LPUART_ISR_RXEVF_Msk) == (LPUART_ISR_RXEVF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    RXEVF    LPUART_ClearFlag_RXEV
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_ClearFlag_RXEV(LPUART_Type *LPUARTx)
{
    WRITE_REG(LPUARTx->ISR, LPUART_ISR_RXEVF_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    TXOV    LPUART_IsActiveFlag_TXOV
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_TXOV(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->ISR, LPUART_ISR_TXOV_Msk) == (LPUART_ISR_TXOV_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    TXOV    LPUART_ClearFlag_TXOV
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_ClearFlag_TXOV(LPUART_Type *LPUARTx)
{
    WRITE_REG(LPUARTx->ISR, LPUART_ISR_TXOV_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    PERR    LPUART_IsActiveFlag_PERR
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_PERR(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->ISR, LPUART_ISR_PERR_Msk) == (LPUART_ISR_PERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    PERR    LPUART_ClearFlag_PERR
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_ClearFlag_PERR(LPUART_Type *LPUARTx)
{
    WRITE_REG(LPUARTx->ISR, LPUART_ISR_PERR_Msk);
}



/**
  * @brief    
  * @rmtoll   ISR    FERR    LPUART_IsActiveFlag_FERR
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_FERR(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->ISR, LPUART_ISR_FERR_Msk) == (LPUART_ISR_FERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    FERR    LPUART_ClearFlag_FERR
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_ClearFlag_FERR(LPUART_Type *LPUARTx)
{
    WRITE_REG(LPUARTx->ISR, LPUART_ISR_FERR_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    OERR    LPUART_IsActiveFlag_OERR
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_OERR(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->ISR, LPUART_ISR_OERR_Msk) == (LPUART_ISR_OERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    OERR    LPUART_ClearFlag_OERR
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_ClearFlag_OERR(LPUART_Type *LPUARTx)
{
    WRITE_REG(LPUARTx->ISR, LPUART_ISR_OERR_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    RXBF    LPUART_IsActiveFlag_RXBF
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_RXBF(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->ISR, LPUART_ISR_RXBF_Msk) == (LPUART_ISR_RXBF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    RXBF    LPUART_ClearFlag_RXBF
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_ClearFlag_RXBF(LPUART_Type *LPUARTx)
{
    WRITE_REG(LPUARTx->ISR, LPUART_ISR_RXBF_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    TXBE    LPUART_IsActiveFlag_TXBE
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_TXBE(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->ISR, LPUART_ISR_TXBE_Msk) == (LPUART_ISR_TXBE_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    TXBE    LPUART_ClearFlag_TXBE
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_ClearFlag_TXBE(LPUART_Type *LPUARTx)
{
    WRITE_REG(LPUARTx->ISR, LPUART_ISR_TXBE_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    TXSE    LPUART_IsActiveFlag_TXSE
  * @param    LPUARTx LPUART instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LPUART_IsActiveFlag_TXSE(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->ISR, LPUART_ISR_TXSE_Msk) == (LPUART_ISR_TXSE_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    TXSE    LPUART_ClearFlag_TXSE
  * @param    LPUARTx LPUART instance
  * @retval   None
  */
__STATIC_INLINE void LPUART_ClearFlag_TXSE(LPUART_Type *LPUARTx)
{
    WRITE_REG(LPUARTx->ISR, LPUART_ISR_TXSE_Msk);
}

/**
  * @brief    
  * @rmtoll   BMR    BAUD    LPUART_SetBaudRate
  * @param    LPUARTx LPUART instance
  * @param    BaudRate This parameter can be one of the following values:
  *           @arg @ref LPUART_BAUDRATE_9600
  *           @arg @ref LPUART_BAUDRATE_4800
  *           @arg @ref LPUART_BAUDRATE_2400
  *           @arg @ref LPUART_BAUDRATE_1200
  *           @arg @ref LPUART_BAUDRATE_600
  *           @arg @ref LPUART_BAUDRATE_300
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetBaudRate(LPUART_Type *LPUARTx, uint32_t BaudRate)
{
    MODIFY_REG(LPUARTx->BMR, LPUART_BMR_BAUD_Msk, BaudRate);
}

/**
  * @brief    
  * @rmtoll   BMR    BAUD    LPUART_GetBaudRate
  * @param    LPUARTx LPUART instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref LPUART_BAUDRATE_9600
  *           @arg @ref LPUART_BAUDRATE_4800
  *           @arg @ref LPUART_BAUDRATE_2400
  *           @arg @ref LPUART_BAUDRATE_1200
  *           @arg @ref LPUART_BAUDRATE_600
  *           @arg @ref LPUART_BAUDRATE_300
  */
__STATIC_INLINE uint32_t LPUART_GetBaudRate(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->BMR, LPUART_BMR_BAUD_Msk));
}

/**
  * @brief    
  * @rmtoll   RXBUF        LPUART_ReceiveData
  * @param    LPUARTx LPUART instance
  * @retval   The LPUart received data
  */
__STATIC_INLINE uint32_t LPUART_ReceiveData(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->RXBUF, 0x1ffU) >> 0U);
}

/**
  * @brief    
  * @rmtoll   TXBUF        LPUART_TransmitData
  * @param    LPUARTx LPUART instance
  * @param    Data The data need to transmit through the LPUart
  * @retval   None
  */
__STATIC_INLINE void LPUART_TransmitData(LPUART_Type *LPUARTx, uint32_t Data)
{
    MODIFY_REG(LPUARTx->TXBUF, (0x1ffU << 0U), (Data << 0U));
}

/**
  * @brief    
  * @rmtoll   DMR        LPUART_SetMatchedData
  * @param    LPUARTx LPUART instance
  * @param    Data The value of match under SLEEP MODE
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetMatchedData(LPUART_Type *LPUARTx, uint8_t Data)
{
    MODIFY_REG(LPUARTx->DMR, (0xffU << 0U), (Data << 0U));
}

/**
  * @brief    
  * @rmtoll   DMR        LPUART_GetMatchedData
  * @param    LPUARTx LPUART instance
  * @retval   The value of match data under SLEEP MODE
  */
__STATIC_INLINE uint32_t LPUART_GetMatchedData(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->DMR, 0xffU) >> 0U);
}

/**
  * @brief    
  * @rmtoll   BMR    MCTL    LPUART_SetMCTLVel
  * @param    LPUARTx LPUART instance
  * @param    Data The value of Bit Modulation Control
  * @retval   None
  */
__STATIC_INLINE void LPUART_SetMCTLVel(LPUART_Type *LPUARTx, uint32_t Data)
{
    MODIFY_REG(LPUARTx->BMR, (0xfffU << 16U), (Data << 16U));
}

/**
  * @brief    
  * @rmtoll   BMR    MCTL    LPUART_GetMCTLVel
  * @param    LPUARTx LPUART instance
  * @retval   The value of Bit Modulation Control
  */
__STATIC_INLINE uint16_t LPUART_GetMCTLVel(LPUART_Type *LPUARTx)
{
    return (uint32_t)(READ_BIT(LPUARTx->BMR, 0xfffU) >> 16U);
}

/**
  * @}
  */

/** @defgroup LPUART_EF_Init Initialization and de-initialization functions
  * @{
  */
ErrorStatus LPUART_DeInit(LPUART_Type *LPUARTx);
void LPUART_StructInit(LPUART_InitTypeDef *LPUART_InitStruct);
ErrorStatus LPUART_Init(LPUART_Type *LPUARTx, LPUART_InitTypeDef *LPUART_InitStruct);

/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __FM33LC0XX_LPUART_H*/

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-09-16**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
