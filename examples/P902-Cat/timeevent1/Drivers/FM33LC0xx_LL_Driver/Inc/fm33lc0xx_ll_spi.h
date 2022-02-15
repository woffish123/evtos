/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_spi.h
  * @author  FMSH Application Team
  * @brief   Head file of SPI LL Module
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
#ifndef __FM33LC0XX_SPI_H
#define __FM33LC0XX_SPI_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
/** @addtogroup FM33LC0XX_Driver_SPI
  * @{
  */
/* Exported types -------------------------------------------------------------------------------------*/
/** @defgroup SPI_ES_INIT SPI Exported Init structures
  * @{
  */

/**
  * @brief LL SPI Init Sturcture definition
  */
typedef struct
{
    /*! SPI传输方向 接受发送 */
    uint32_t TransferDirection;
    /*! 传输模式 单双工 */ 
    uint32_t TransferMode;
    /*! 主从模式 */
    uint32_t Mode;             
    /*! 数据位宽 */
    uint32_t DataWidth;        
    /*! 时钟极性 */                        
    uint32_t ClockPolarity;    
    /*! 时钟相位 */
    uint32_t ClockPhase;       
    /*! NSS 脚模式 软件或硬件 */
    uint32_t SSN;              
    /*! 通讯速率 */
    uint32_t BaudRate;         
    /*! Bit方向 */
    uint32_t BitOrder;         
                          
} SPI_InitTypeDef;
/**
  * @}
  */
/* Exported constants ---------------------------------------------------------------------------------*/
/** @defgroup SPI_Exported_Constants SPI Exported Constants
  * @{
  */
#define    SPI_SWAP_MOMI_PIN_DISABLE                           (0x0U << SPI_CR1_IOSWAP_Pos)
#define    SPI_SWAP_MOMI_PIN_ENABLE                            (0x1U << SPI_CR1_IOSWAP_Pos)

#define    SPI_SAMP_POSITION_NORMAL                            (0x0U << SPI_CR1_MSPA_Pos)
#define    SPI_SAMP_POSITION_DELAY_HALF_CLK                    (0x1U << SPI_CR1_MSPA_Pos)

#define    SPI_SEND_POSITION_NORMAL                            (0x0U << SPI_CR1_SSPA_Pos)
#define    SPI_SEND_POSITION_ADVANCE_HALF_CLK                  (0x1U << SPI_CR1_SSPA_Pos)

#define    SPI_WORK_MODE_SLAVE                                 (0x0U << SPI_CR1_MM_Pos)
#define    SPI_WORK_MODE_MASTER                                (0x1U << SPI_CR1_MM_Pos)

#define    SPI_SEND_POSITION_SLAVE                             (0x0U << SPI_CR1_MM_Pos)
#define    SPI_SEND_POSITION_MASTER                            (0x1U << SPI_CR1_MM_Pos)

#define    SPI_BAUDRATEPRESCALER_DIV2                          (0x0U << SPI_CR1_BAUD_Pos)
#define    SPI_BAUDRATEPRESCALER_DIV4                          (0x1U << SPI_CR1_BAUD_Pos)
#define    SPI_BAUDRATEPRESCALER_DIV8                          (0x2U << SPI_CR1_BAUD_Pos)
#define    SPI_BAUDRATEPRESCALER_DIV16                         (0x3U << SPI_CR1_BAUD_Pos)
#define    SPI_BAUDRATEPRESCALER_DIV32                         (0x4U << SPI_CR1_BAUD_Pos)
#define    SPI_BAUDRATEPRESCALER_DIV64                         (0x5U << SPI_CR1_BAUD_Pos)
#define    SPI_BAUDRATEPRESCALER_DIV128                        (0x6U << SPI_CR1_BAUD_Pos)
#define    SPI_BAUDRATEPRESCALER_DIV256                        (0x7U << SPI_CR1_BAUD_Pos)

#define    SPI_BIT_ORDER_MSB_FIRST                             (0x0U << SPI_CR1_LSBF_Pos)
#define    SPI_BIT_ORDER_LSB_FIRST                             (0x1U << SPI_CR1_LSBF_Pos)

#define    SPI_SPI_POLARITY_LOW                                (0x0U << SPI_CR1_CPOL_Pos)
#define    SPI_SPI_POLARITY_HIGH                               (0x1U << SPI_CR1_CPOL_Pos)

#define    SPI_SPI_PHASE_1EDGE                                 (0x0U << SPI_CR1_CPHA_Pos)
#define    SPI_SPI_PHASE_2EDGE                                 (0x1U << SPI_CR1_CPHA_Pos)

#define    SPI_DUMMY_CYCLE_DISABLE                             (0x0U << SPI_CR2_DUMMY_EN_Pos)
#define    SPI_DUMMY_CYCLE_ENABLE                              (0x1U << SPI_CR2_DUMMY_EN_Pos)

#define    SPI_RECV_ONLY_DISABLE                               (0x0U << SPI_CR2_RXO_Pos)
#define    SPI_RECV_ONLY_ENABLE                                (0x1U << SPI_CR2_RXO_Pos)

#define    SPI_DATAWIDTH_8BIT                                  (0x0U << SPI_CR2_DLEN_Pos)
#define    SPI_DATAWIDTH_16BIT                                 (0x1U << SPI_CR2_DLEN_Pos)
#define    SPI_DATAWIDTH_24BIT                                 (0x2U << SPI_CR2_DLEN_Pos)
#define    SPI_DATAWIDTH_32BIT                                 (0x3U << SPI_CR2_DLEN_Pos)

#define    SPI_MODE_FULL_DUPLEX                                (0x0U << SPI_CR2_HALFDUPLEX_Pos)
#define    SPI_MODE_SIMPLEX                                    (0x1U << SPI_CR2_HALFDUPLEX_Pos)

#define    SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_TX                (0x0U << SPI_CR2_HD_RW_Pos)
#define    SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_RX                (0x1U << SPI_CR2_HD_RW_Pos)

#define    SPI_SIMPLEX_CMD_LEN_CONFIG_AS_DLEN                  (0x0U << SPI_CR2_CMD8B_Pos)
#define    SPI_SIMPLEX_CMD_LEN_FIX_8BIT                        (0x1U << SPI_CR2_CMD8B_Pos)

#define    SPI_SSN_SEND_MODE_HIGH                              (0x0U << SPI_CR2_SSNM_Pos)
#define    SPI_SSN_SEND_MODE_LOW                               (0x1U << SPI_CR2_SSNM_Pos)

#define    SPI_TXONLY_AUTO_OFF_DISABLE                         (0x0U << SPI_CR2_TXO_AC_Pos)
#define    SPI_TXONLY_AUTO_OFF_ENABLE                          (0x1U << SPI_CR2_TXO_AC_Pos)

#define    SPI_TXONLY_MODE_DISABLE                             (0x0U << SPI_CR2_TXO_Pos)
#define    SPI_TXONLY_MODE_ENABLE                              (0x1U << SPI_CR2_TXO_Pos)

#define    SPI_SSN_MODE_HARDWARE                               (0x0U << SPI_CR2_SSNSEN_Pos)
#define    SPI_SSN_MODE_SOFTWARE                               (0x1U << SPI_CR2_SSNSEN_Pos)

#define    SPI_HALFDUPLE_DCN_CMD                               (0x0U << SPI_ISR_DCN_TX_Pos)
#define    SPI_HALFDUPLE_DCN_DATA                              (0x1U << SPI_ISR_DCN_TX_Pos)
/**
  * @}
  */
/* Exported functions ---------------------------------------------------------------------------------*/
/** @defgroup SPI_Exported_Functions SPI Exported Functions
  * @{
  */

/**
  * @brief    
  * @rmtoll   CR1    IOSWAP    SPI_SetIOPinSwap
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_SWAP_MOMI_PIN_DISABLE
  *           @arg @ref SPI_SWAP_MOMI_PIN_ENABLE
  * @retval   None
  */
__STATIC_INLINE void SPI_SetIOPinSwap(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_IOSWAP_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR1    IOSWAP    SPI_GetIOPinSwap
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SWAP_MOMI_PIN_DISABLE
  *           @arg @ref SPI_SWAP_MOMI_PIN_ENABLE
  */
__STATIC_INLINE uint32_t SPI_GetIOPinSwap(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_IOSWAP_Msk));
}

/**
  * @brief    
  * @rmtoll   CR1    MSPA    SPI_SetSampilingAdjust
  * @param    SPIx SPI instance
  * @param    adjust This parameter can be one of the following values:
  *           @arg @ref SPI_SAMP_POSITION_NORMAL
  *           @arg @ref SPI_SAMP_POSITION_DELAY_HALF_CLK
  * @retval   None
  */
__STATIC_INLINE void SPI_SetSampilingAdjust(SPI_Type* SPIx, uint32_t adjust)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_MSPA_Msk, adjust);
}

/**
  * @brief    
  * @rmtoll   CR1    MSPA    SPI_GetSampilingAdjust
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SAMP_POSITION_NORMAL
  *           @arg @ref SPI_SAMP_POSITION_DELAY_HALF_CLK
  */
__STATIC_INLINE uint32_t SPI_GetSampilingAdjust(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_MSPA_Msk));
}

/**
  * @brief    
  * @rmtoll   CR1    SSPA    SPI_SetSendAdjust
  * @param    SPIx SPI instance
  * @param    adjust This parameter can be one of the following values:
  *           @arg @ref SPI_SEND_POSITION_NORMAL
  *           @arg @ref SPI_SEND_POSITION_ADVANCE_HALF_CLK
  * @retval   None
  */
__STATIC_INLINE void SPI_SetSendAdjust(SPI_Type* SPIx, uint32_t adjust)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_SSPA_Msk, adjust);
}

/**
  * @brief    
  * @rmtoll   CR1    SSPA    SPI_GetSendAdjust
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SEND_POSITION_NORMAL
  *           @arg @ref SPI_SEND_POSITION_ADVANCE_HALF_CLK
  */
__STATIC_INLINE uint32_t SPI_GetSendAdjust(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_SSPA_Msk));
}

/**
  * @brief    
  * @rmtoll   CR1    MM    SPI_SetSPIWorkMode
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_WORK_MODE_SLAVE
  *           @arg @ref SPI_WORK_MODE_MASTER
  * @retval   None
  */
__STATIC_INLINE void SPI_SetSPIWorkMode(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_MM_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR1    MM    SPI_GetSPIWorkMode
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SEND_POSITION_SLAVE
  *           @arg @ref SPI_SEND_POSITION_MASTER
  */
__STATIC_INLINE uint32_t SPI_GetSPIWorkMode(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_MM_Msk));
}

/**
  * @brief    
  * @rmtoll   CR1    WAIT    SPI_SetSendWait
  * @param    SPIx SPI instance
  * @param    wait 
  * @retval   None
  */
__STATIC_INLINE void SPI_SetSendWait(SPI_Type* SPIx, uint32_t wait)
{
    MODIFY_REG(SPIx->CR1, (0x3U << 6U), (wait << 6U));
}

/**
  * @brief    
  * @rmtoll   CR1    WAIT    SPI_GetSendWait
  * @param    SPIx SPI instance
  * @retval   
  */
__STATIC_INLINE uint32_t SPI_GetSendWait(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, 0x3U) >> 6U);
}

/**
  * @brief    
  * @rmtoll   CR1    BAUD    SPI_SetBaudrate
  * @param    SPIx SPI instance
  * @param    baud This parameter can be one of the following values:
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV2
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV4
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV8
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV16
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV32
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV64
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV128
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV256
  * @retval   None
  */
__STATIC_INLINE void SPI_SetBaudrate(SPI_Type* SPIx, uint32_t baud)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_BAUD_Msk, baud);
}

/**
  * @brief    
  * @rmtoll   CR1    BAUD    SPI_GetBaudrate
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV2
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV4
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV8
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV16
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV32
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV64
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV128
  *           @arg @ref SPI_BAUDRATEPRESCALER_DIV256
  */
__STATIC_INLINE uint32_t SPI_GetBaudrate(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_BAUD_Msk));
}

/**
  * @brief    
  * @rmtoll   CR1    LSBF    SPI_SetBitOrder
  * @param    SPIx SPI instance
  * @param    BitOrder This parameter can be one of the following values:
  *           @arg @ref SPI_BIT_ORDER_MSB_FIRST
  *           @arg @ref SPI_BIT_ORDER_LSB_FIRST
  * @retval   None
  */
__STATIC_INLINE void SPI_SetBitOrder(SPI_Type* SPIx, uint32_t BitOrder)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_LSBF_Msk, BitOrder);
}

/**
  * @brief    
  * @rmtoll   CR1    LSBF    SPI_GetBitOrder
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_BIT_ORDER_MSB_FIRST
  *           @arg @ref SPI_BIT_ORDER_LSB_FIRST
  */
__STATIC_INLINE uint32_t SPI_GetBitOrder(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_LSBF_Msk));
}

/**
  * @brief    
  * @rmtoll   CR1    CPOL    SPI_SetClockPolarity
  * @param    SPIx SPI instance
  * @param    ClockPolarity This parameter can be one of the following values:
  *           @arg @ref SPI_SPI_POLARITY_LOW
  *           @arg @ref SPI_SPI_POLARITY_HIGH
  * @retval   None
  */
__STATIC_INLINE void SPI_SetClockPolarity(SPI_Type* SPIx, uint32_t ClockPolarity)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_CPOL_Msk, ClockPolarity);
}

/**
  * @brief    
  * @rmtoll   CR1    CPOL    SPI_GetClockPolarity
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SPI_POLARITY_LOW
  *           @arg @ref SPI_SPI_POLARITY_HIGH
  */
__STATIC_INLINE uint32_t SPI_GetClockPolarity(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_CPOL_Msk));
}

/**
  * @brief    
  * @rmtoll   CR1    CPHA    SPI_SetClockPhase
  * @param    SPIx SPI instance
  * @param    ClockPhase This parameter can be one of the following values:
  *           @arg @ref SPI_SPI_PHASE_1EDGE
  *           @arg @ref SPI_SPI_PHASE_2EDGE
  * @retval   None
  */
__STATIC_INLINE void SPI_SetClockPhase(SPI_Type* SPIx, uint32_t ClockPhase)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_CPHA_Msk, ClockPhase);
}

/**
  * @brief    
  * @rmtoll   CR1    CPHA    SPI_GetClockPhase
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SPI_PHASE_1EDGE
  *           @arg @ref SPI_SPI_PHASE_2EDGE
  */
__STATIC_INLINE uint32_t SPI_GetClockPhase(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_CPHA_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    DUMMY_EN    SPI_SetDummyCycle
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_DUMMY_CYCLE_DISABLE
  *           @arg @ref SPI_DUMMY_CYCLE_ENABLE
  * @retval   None
  */
__STATIC_INLINE void SPI_SetDummyCycle(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_DUMMY_EN_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR2    DUMMY_EN    SPI_GetDummyCycle
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_DUMMY_CYCLE_DISABLE
  *           @arg @ref SPI_DUMMY_CYCLE_ENABLE
  */
__STATIC_INLINE uint32_t SPI_GetDummyCycle(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_DUMMY_EN_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    RXO    SPI_SetRecvModeOnly
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_RECV_ONLY_DISABLE
  *           @arg @ref SPI_RECV_ONLY_ENABLE
  * @retval   None
  */
__STATIC_INLINE void SPI_SetRecvModeOnly(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_RXO_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR2    RXO    SPI_GetRecvModeOnly
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_RECV_ONLY_DISABLE
  *           @arg @ref SPI_RECV_ONLY_ENABLE
  */
__STATIC_INLINE uint32_t SPI_GetRecvModeOnly(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_RXO_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    DLEN    SPI_SetDataWidth
  * @param    SPIx SPI instance
  * @param    DataWidth This parameter can be one of the following values:
  *           @arg @ref SPI_DATAWIDTH_8BIT
  *           @arg @ref SPI_DATAWIDTH_16BIT
  *           @arg @ref SPI_DATAWIDTH_24BIT
  *           @arg @ref SPI_DATAWIDTH_32BIT
  * @retval   None
  */
__STATIC_INLINE void SPI_SetDataWidth(SPI_Type* SPIx, uint32_t DataWidth)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_DLEN_Msk, DataWidth);
}

/**
  * @brief    
  * @rmtoll   CR2    DLEN    SPI_GetDataWidth
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_DATAWIDTH_8BIT
  *           @arg @ref SPI_DATAWIDTH_16BIT
  *           @arg @ref SPI_DATAWIDTH_24BIT
  *           @arg @ref SPI_DATAWIDTH_32BIT
  */
__STATIC_INLINE uint32_t SPI_GetDataWidth(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_DLEN_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    HALFDUPLEX    SPI_SetTransferMode
  * @param    SPIx SPI instance
  * @param    Mode This parameter can be one of the following values:
  *           @arg @ref SPI_MODE_FULL_DUPLEX
  *           @arg @ref SPI_MODE_SIMPLEX
  * @retval   None
  */
__STATIC_INLINE void SPI_SetTransferMode(SPI_Type* SPIx, uint32_t Mode)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_HALFDUPLEX_Msk, Mode);
}

/**
  * @brief    
  * @rmtoll   CR2    HALFDUPLEX    SPI_GetTransferMode
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_MODE_FULL_DUPLEX
  *           @arg @ref SPI_MODE_SIMPLEX
  */
__STATIC_INLINE uint32_t SPI_GetTransferMode(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_HALFDUPLEX_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    HD_RW    SPI_SetTransferDirection
  * @param    SPIx SPI instance
  * @param    TransferDirection This parameter can be one of the following values:
  *           @arg @ref SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_TX
  *           @arg @ref SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_RX
  * @retval   None
  */
__STATIC_INLINE void SPI_SetTransferDirection(SPI_Type* SPIx, uint32_t TransferDirection)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_HD_RW_Msk, TransferDirection);
}

/**
  * @brief    
  * @rmtoll   CR2    HD_RW    SPI_GetTransferDirection
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_TX
  *           @arg @ref SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_RX
  */
__STATIC_INLINE uint32_t SPI_GetTransferDirection(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_HD_RW_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    CMD8B    SPI_SetSimplexCmdlen
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_SIMPLEX_CMD_LEN_CONFIG_AS_DLEN
  *           @arg @ref SPI_SIMPLEX_CMD_LEN_FIX_8BIT
  * @retval   None
  */
__STATIC_INLINE void SPI_SetSimplexCmdlen(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_CMD8B_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR2    CMD8B    SPI_GetSimplexCmdlen
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SIMPLEX_CMD_LEN_CONFIG_AS_DLEN
  *           @arg @ref SPI_SIMPLEX_CMD_LEN_FIX_8BIT
  */
__STATIC_INLINE uint32_t SPI_GetSimplexCmdlen(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_CMD8B_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    SSNM    SPI_SetSSNPinMode
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_SSN_SEND_MODE_HIGH
  *           @arg @ref SPI_SSN_SEND_MODE_LOW
  * @retval   None
  */
__STATIC_INLINE void SPI_SetSSNPinMode(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_SSNM_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR2    SSNM    SPI_GetSSNPinMode
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SSN_SEND_MODE_HIGH
  *           @arg @ref SPI_SSN_SEND_MODE_LOW
  */
__STATIC_INLINE uint32_t SPI_GetSSNPinMode(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_SSNM_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    TXO_AC    SPI_SetTxOnlyModeAutoDisable
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_TXONLY_AUTO_OFF_DISABLE
  *           @arg @ref SPI_TXONLY_AUTO_OFF_ENABLE
  * @retval   None
  */
__STATIC_INLINE void SPI_SetTxOnlyModeAutoDisable(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_TXO_AC_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR2    TXO_AC    SPI_GetTxOnlyModeAutoDisable
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_TXONLY_AUTO_OFF_DISABLE
  *           @arg @ref SPI_TXONLY_AUTO_OFF_ENABLE
  */
__STATIC_INLINE uint32_t SPI_GetTxOnlyModeAutoDisable(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_TXO_AC_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    TXO    SPI_SetTxOnlymode
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_TXONLY_MODE_DISABLE
  *           @arg @ref SPI_TXONLY_MODE_ENABLE
  * @retval   None
  */
__STATIC_INLINE void SPI_SetTxOnlymode(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_TXO_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR2    SSN    SPI_SetSSNPin
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_SetSSNPin(SPI_Type* SPIx)
{
    SET_BIT(SPIx->CR2, SPI_CR2_SSN_Msk); 
}

/**
  * @brief    
  * @rmtoll   CR2    SSN    SPI_ResetSSNPin
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  */
__STATIC_INLINE void SPI_ResetSSNPin(SPI_Type* SPIx)
{
    CLEAR_BIT(SPIx->CR2, SPI_CR2_SSN_Msk);  
}

/**
  * @brief    
  * @rmtoll   CR2    SSNSEN    SPI_SetSNNMode
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_SSN_MODE_HARDWARE
  *           @arg @ref SPI_SSN_MODE_SOFTWARE
  * @retval   None
  */
__STATIC_INLINE void SPI_SetSNNMode(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->CR2, SPI_CR2_SSNSEN_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   CR2    SSNSEN    SPI_GetSNNMode
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_SSN_MODE_HARDWARE
  *           @arg @ref SPI_SSN_MODE_SOFTWARE
  */
__STATIC_INLINE uint32_t SPI_GetSNNMode(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_SSNSEN_Msk));
}

/**
  * @brief    
  * @rmtoll   CR2    SPIEN    SPI_Enable
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_Enable(SPI_Type* SPIx)
{
    SET_BIT(SPIx->CR2, SPI_CR2_SPIEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR2    SPIEN    SPI_IsEnabled
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsEnabled(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR2, SPI_CR2_SPIEN_Msk) == SPI_CR2_SPIEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR2    SPIEN    SPI_Disable
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_Disable(SPI_Type* SPIx)
{
    CLEAR_BIT(SPIx->CR2, SPI_CR2_SPIEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR3    TXBFC    SPI_TxBuffClear
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_TxBuffClear(SPI_Type* SPIx)
{
    WRITE_REG(SPIx->CR3, SPI_CR3_TXBFC_Msk);
}

/**
  * @brief    
  * @rmtoll   CR3    RXBFC    SPI_RxBuffClear
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_RxBuffClear(SPI_Type* SPIx)
{
    WRITE_REG(SPIx->CR3, SPI_CR3_RXBFC_Msk);
}

/**
  * @brief    
  * @rmtoll   CR3    MERRC    SPI_MasterErrorClear
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_MasterErrorClear(SPI_Type* SPIx)
{
    WRITE_REG(SPIx->CR3, SPI_CR3_MERRC_Msk);
}

/**
  * @brief    
  * @rmtoll   CR3    SERRC    SPI_SlaveErrorClear
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_SlaveErrorClear(SPI_Type* SPIx)
{
    WRITE_REG(SPIx->CR3, SPI_CR3_SERRC_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    ERRIE    SPI_DisableIT_Error
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_DisableIT_Error(SPI_Type* SPIx)
{
    CLEAR_BIT(SPIx->IER, SPI_IER_ERRIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    ERRIE    SPI_EnabledIT_Error
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_EnabledIT_Error(SPI_Type* SPIx)
{
    SET_BIT(SPIx->IER, SPI_IER_ERRIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    ERRIE    SPI_IsEnabledIT_Error
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsEnabledIT_Error(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->IER, SPI_IER_ERRIE_Msk) == SPI_IER_ERRIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    TXIE    SPI_DisableIT_Transmitcomplete
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_DisableIT_Transmitcomplete(SPI_Type* SPIx)
{
    CLEAR_BIT(SPIx->IER, SPI_IER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    TXIE    SPI_EnabledIT_Transmitcomplete
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_EnabledIT_Transmitcomplete(SPI_Type* SPIx)
{
    SET_BIT(SPIx->IER, SPI_IER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    TXIE    SPI_IsEnabledIT_Transmitcomplete
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsEnabledIT_Transmitcomplete(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->IER, SPI_IER_TXIE_Msk) == SPI_IER_TXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXIE    SPI_DisableIT_Receivecomplete
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_DisableIT_Receivecomplete(SPI_Type* SPIx)
{
    CLEAR_BIT(SPIx->IER, SPI_IER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXIE    SPI_EnabledIT_Receivecomplete
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_EnabledIT_Receivecomplete(SPI_Type* SPIx)
{
    SET_BIT(SPIx->IER, SPI_IER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   IER    RXIE    SPI_IsEnabledIT_Receivecomplete
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsEnabledIT_Receivecomplete(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->IER, SPI_IER_RXIE_Msk) == SPI_IER_RXIE_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    DCN_TX    SPI_SetDataOrCmdMode
  * @param    SPIx SPI instance
  * @param    mode This parameter can be one of the following values:
  *           @arg @ref SPI_HALFDUPLE_DCN_CMD
  *           @arg @ref SPI_HALFDUPLE_DCN_DATA
  * @retval   None
  */
__STATIC_INLINE void SPI_SetDataOrCmdMode(SPI_Type* SPIx, uint32_t mode)
{
    MODIFY_REG(SPIx->ISR, SPI_ISR_DCN_TX_Msk, mode);
}

/**
  * @brief    
  * @rmtoll   ISR    DCN_TX    SPI_GetDataOrCmdMode
  * @param    SPIx SPI instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref SPI_HALFDUPLE_DCN_CMD
  *           @arg @ref SPI_HALFDUPLE_DCN_DATA
  */
__STATIC_INLINE uint32_t SPI_GetDataOrCmdMode(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->ISR, SPI_ISR_DCN_TX_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    RXCOL    SPI_IsActiveFlag_ReceiveCollosion
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsActiveFlag_ReceiveCollosion(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->ISR, SPI_ISR_RXCOL_Msk) == (SPI_ISR_RXCOL_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    RXCOL    SPI_ClearFlag_RxBuffCollosion
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_ClearFlag_RxBuffCollosion(SPI_Type* SPIx)
{
    WRITE_REG(SPIx->ISR, SPI_ISR_RXCOL_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    TXCOL    SPI_IsActiveFlag_TxBuffCollosion
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsActiveFlag_TxBuffCollosion(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->ISR, SPI_ISR_TXCOL_Msk) == (SPI_ISR_TXCOL_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    TXCOL    SPI_ClearFlag_TxBuffCollosion
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_ClearFlag_TxBuffCollosion(SPI_Type* SPIx)
{
    WRITE_REG(SPIx->ISR, SPI_ISR_TXCOL_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    BUSY    SPI_IsActiveFlag_SPIBusy
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsActiveFlag_SPIBusy(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->ISR, SPI_ISR_BUSY_Msk) == (SPI_ISR_BUSY_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    MERR    SPI_IsActiveFlag_MasterError
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsActiveFlag_MasterError(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->ISR, SPI_ISR_MERR_Msk) == (SPI_ISR_MERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    MERR    SPI_ClearFlag_MasterError
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_ClearFlag_MasterError(SPI_Type* SPIx)
{
    WRITE_REG(SPIx->ISR, SPI_ISR_MERR_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    SERR    SPI_IsActiveFlag_SlaveError
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsActiveFlag_SlaveError(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->ISR, SPI_ISR_SERR_Msk) == (SPI_ISR_SERR_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    SERR    SPI_ClearFlag_SlaveError
  * @param    SPIx SPI instance
  * @retval   None
  */
__STATIC_INLINE void SPI_ClearFlag_SlaveError(SPI_Type* SPIx)
{
    WRITE_REG(SPIx->ISR, SPI_ISR_SERR_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    TXBE    SPI_IsActiveFlag_TxBufffEmpty
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsActiveFlag_TxBufffEmpty(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->ISR, SPI_ISR_TXBE_Msk) == (SPI_ISR_TXBE_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    RXBF    SPI_IsActiveFlag_RxBufffFull
  * @param    SPIx SPI instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t SPI_IsActiveFlag_RxBufffFull(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->ISR, SPI_ISR_RXBF_Msk) == (SPI_ISR_RXBF_Msk));
}
/**
  * @brief    
  * @rmtoll   TXBUF        SPI_WriteTxBuff
  * @param    SPIx SPI instance
  * @param    data 
  * @retval   None
  */
__STATIC_INLINE void SPI_WriteTxBuff(SPI_Type* SPIx, uint32_t data)
{
    MODIFY_REG(SPIx->TXBUF, (0xffffffffU << 0U), (data << 0U));
}

/**
  * @brief    
  * @rmtoll   RXBUF        SPI_ReadRxBuff
  * @param    SPIx SPI instance
  * @retval   
  */
__STATIC_INLINE uint32_t SPI_ReadRxBuff(SPI_Type* SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->RXBUF, 0xffffffffU) >> 0U);
}

/**
  * @}
  */

/** @defgroup SPI_EF_Init Initialization and de-initialization functions
  * @{
  */
ErrorStatus SPI_DeInit(SPI_Type *SPIx);
ErrorStatus SPI_Init(SPI_Type *SPIx,SPI_InitTypeDef *SPI_InitStruct);
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);

/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __FM33LC0XX_SPI_H*/

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-17**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
