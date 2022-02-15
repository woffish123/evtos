/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_comp.h
  * @author  FMSH Application Team
  * @brief   Head file of COMP LL Module
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
#ifndef __FM33LC0XX_COMP_H
#define __FM33LC0XX_COMP_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
/** @addtogroup FM33LC0XX_Driver_COMP
  * @{
  */
/* Exported types -------------------------------------------------------------------------------------*/
/** @defgroup COMP_ES_INIT COMP Exported Init structures
  * @{
  */

/**
  * @brief LL COMP Init Sturcture definition
  */
typedef struct
{
    /*! 比较器正向输入选择 */
    uint32_t PositiveInput;          
    /*! 比较器反向输入选择 */
    uint32_t NegativeInput;
    /*! 结果输出极性 */      
    uint32_t Polarity;
    /*! 中断触发边沿选择 */                                             
    uint32_t Edge;
    /*! 数字滤波器使能 */  
    uint32_t DigitalFilter;  
                                                      
} COMP_InitTypeDef;
/**
  * @}
  */
/* Exported constants ---------------------------------------------------------------------------------*/
/** @defgroup COMP_Exported_Constants COMP Exported Constants
  * @{
  */




#define    COMP_COMPARAROT_OUTPUT_POLARITY_NORMAL              (0x0U << COMP_CR_POL_Pos)
#define    COMP_COMPARAROT_OUTPUT_POLARITY_REVERSE             (0x1U << COMP_CR_POL_Pos)


#define    COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP1            (0x0U << COMP_CR_VPSEL_Pos)
#define    COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP2            (0x1U << COMP_CR_VPSEL_Pos)
#define    COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP3            (0x2U << COMP_CR_VPSEL_Pos)



#define    COMP_COMPARAROT_NEGATIVEVE_INPUT_COMP_INN1          (0x0U << COMP_CR_VNSEL_Pos)
#define    COMP_COMPARAROT_NEGATIVEVE_INPUT_COMP_INN2          (0x1U << COMP_CR_VNSEL_Pos)
#define    COMP_COMPARAROT_NEGATIVEVE_INPUT_VREF               (0x2U << COMP_CR_VNSEL_Pos)
#define    COMP_COMPARAROT_NEGATIVEVE_INPUT_VREF_DIV_2         (0x3U << COMP_CR_VNSEL_Pos)

#define    COMP_COMPARATOR_INTERRUPT_EDGE_BOTH                 (0x0U)
#define    COMP_COMPARATOR_INTERRUPT_EDGE_RISING               (0x1U)
#define    COMP_COMPARATOR_INTERRUPT_EDGE_FALLING              (0x2U)





/**
  * @}
  */
/* Exported functions ---------------------------------------------------------------------------------*/
/** @defgroup COMP_Exported_Functions COMP Exported Functions
  * @{
  */

/**
  * @brief    
  * @rmtoll   CR    CMPO    COMP_GetComparatorOutput
  * @param    COMPx COMP instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t COMP_GetComparatorOutput(COMP_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->CR, COMP_CR_CMPO_Msk) == (COMP_CR_CMPO_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    POL    COMP_SetComparatorPolarity
  * @param    COMPx COMP instance
  * @param    polarity This parameter can be one of the following values:
  *           @arg @ref COMP_COMPARAROT_OUTPUT_POLARITY_NORMAL
  *           @arg @ref COMP_COMPARAROT_OUTPUT_POLARITY_REVERSE
  * @retval   None
  */
__STATIC_INLINE void COMP_SetComparatorPolarity(COMP_Type* COMPx, uint32_t polarity)
{
    MODIFY_REG(COMPx->CR, COMP_CR_POL_Msk, polarity);
}

/**
  * @brief    
  * @rmtoll   CR    POL    COMP_GetComparatorPolarity
  * @param    COMPx COMP instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref COMP_COMPARAROT_OUTPUT_POLARITY_NORMAL
  *           @arg @ref COMP_COMPARAROT_OUTPUT_POLARITY_REVERSE
  */
__STATIC_INLINE uint32_t COMP_GetComparatorPolarity(COMP_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->CR, COMP_CR_POL_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    VPSEL    COMP_SetComparatorPositiveInput
  * @param    COMPx COMP instance
  * @param    Input This parameter can be one of the following values:
  *           @arg @ref COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP1
  *           @arg @ref COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP2
  *           @arg @ref COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP3
  * @retval   None
  */
__STATIC_INLINE void COMP_SetComparatorPositiveInput(COMP_Type* COMPx, uint32_t Input)
{
    MODIFY_REG(COMPx->CR, COMP_CR_VPSEL_Msk, Input);
}

/**
  * @brief    
  * @rmtoll   CR    VPSEL    COMP_GetComparatorPositiveInput
  * @param    COMPx COMP instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP1
  *           @arg @ref COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP2
  *           @arg @ref COMP_COMPARAROT_POSITIVE_INPUT_COMP_INP3
  */
__STATIC_INLINE uint32_t COMP_GetComparatorPositiveInput(COMP_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->CR, COMP_CR_VPSEL_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    VNSEL    COMP_SetComparatorNegativeInput
  * @param    COMPx COMP instance
  * @param    Input This parameter can be one of the following values:
  *           @arg @ref COMP_COMPARAROT_NEGATIVEVE_INPUT_COMP_INN1
  *           @arg @ref COMP_COMPARAROT_NEGATIVEVE_INPUT_VREF
  *           @arg @ref COMP_COMPARAROT_NEGATIVEVE_INPUT_VREF_DIV_2
  * @retval   None
  */
__STATIC_INLINE void COMP_SetComparatorNegativeInput(COMP_Type* COMPx, uint32_t Input)
{
    MODIFY_REG(COMPx->CR, COMP_CR_VNSEL_Msk, Input);
}

/**
  * @brief    
  * @rmtoll   CR    VNSEL    COMP_GetComparatorNegativeInput
  * @param    COMPx COMP instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref COMP_COMPARAROT_NEGATIVEVE_INPUT_COMP_INN1
  *           @arg @ref COMP_COMPARAROT_NEGATIVEVE_INPUT_VREF
  *           @arg @ref COMP_COMPARAROT_NEGATIVEVE_INPUT_VREF_DIV_2
  */
__STATIC_INLINE uint32_t COMP_GetComparatorNegativeInput(COMP_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->CR, COMP_CR_VNSEL_Msk));
}

/**
  * @brief    
  * @rmtoll   CR    CMPEN    COMP_Enable
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_Enable(COMP_Type* COMPx)
{
    SET_BIT(COMPx->CR, COMP_CR_CMPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    CMPEN    COMP_IsEnabled
  * @param    COMPx COMP instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t COMP_IsEnabled(COMP_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->CR, COMP_CR_CMPEN_Msk) == COMP_CR_CMPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   CR    CMPEN    COMP_Disable
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_Disable(COMP_Type* COMPx)
{
    CLEAR_BIT(COMPx->CR, COMP_CR_CMPEN_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP1DF    COMP_EnableDigitalFilter1
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_EnableDigitalFilter1(COMP_COMMON_Type* COMPx)
{
    SET_BIT(COMPx->ICR, COMP_ICR_CMP1DF_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP1DF    COMP_IsEnabledDigitalFilter1
  * @param    COMPx COMP instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t COMP_IsEnabledDigitalFilter1(COMP_COMMON_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->ICR, COMP_ICR_CMP1DF_Msk) == COMP_ICR_CMP1DF_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP1DF    COMP_DisableDigitalFilter1
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_DisableDigitalFilter1(COMP_COMMON_Type* COMPx)
{
    CLEAR_BIT(COMPx->ICR, COMP_ICR_CMP1DF_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP2DF    COMP_EnableDigitalFilter2
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_EnableDigitalFilter2(COMP_COMMON_Type* COMPx)
{
    SET_BIT(COMPx->ICR, COMP_ICR_CMP2DF_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP2DF    COMP_IsEnabledDigitalFilter2
  * @param    COMPx COMP instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t COMP_IsEnabledDigitalFilter2(COMP_COMMON_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->ICR, COMP_ICR_CMP2DF_Msk) == COMP_ICR_CMP2DF_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP2DF    COMP_DisableDigitalFilter2
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_DisableDigitalFilter2(COMP_COMMON_Type* COMPx)
{
    CLEAR_BIT(COMPx->ICR, COMP_ICR_CMP2DF_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP1IE    COMP_EnableITComper1
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_EnableITComper1(COMP_COMMON_Type* COMPx)
{
    SET_BIT(COMPx->ICR, COMP_ICR_CMP1IE_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP1IE    COMP_IsEnabledITComper1
  * @param    COMPx COMP instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t COMP_IsEnabledITComper1(COMP_COMMON_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->ICR, COMP_ICR_CMP1IE_Msk) == COMP_ICR_CMP1IE_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP1IE    COMP_DisableITComper1
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_DisableITComper1(COMP_COMMON_Type* COMPx)
{
    CLEAR_BIT(COMPx->ICR, COMP_ICR_CMP1IE_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP2IE    COMP_EnableITComper2
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_EnableITComper2(COMP_COMMON_Type* COMPx)
{
    SET_BIT(COMPx->ICR, COMP_ICR_CMP2IE_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP2IE    COMP_IsEnabledITComper2
  * @param    COMPx COMP instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t COMP_IsEnabledITComper2(COMP_COMMON_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->ICR, COMP_ICR_CMP2IE_Msk) == COMP_ICR_CMP2IE_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP2IE    COMP_DisableITComper2
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_DisableITComper2(COMP_COMMON_Type* COMPx)
{
    CLEAR_BIT(COMPx->ICR, COMP_ICR_CMP2IE_Msk);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP2SEL    COMP_SetComper2InterruptEdge
  * @param    COMPx COMP instance
  * @param    edge This parameter can be one of the following values:
  *           @arg @ref COMP_COMPARATOR_INTERRUPT_EDGE_BOTH
  *           @arg @ref COMP_COMPARATOR_INTERRUPT_EDGE_RISING
  *           @arg @ref COMP_COMPARATOR_INTERRUPT_EDGE_FALLING
  * @retval   None
  */
__STATIC_INLINE void COMP_SetComper2InterruptEdge(COMP_COMMON_Type* COMPx, uint32_t edge)
{
    MODIFY_REG(COMPx->ICR, COMP_ICR_CMP2SEL_Msk, edge<COMP_ICR_CMP2SEL_Pos);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP2SEL    COMP_GetComper2InterruptEdge
  * @param    COMPx COMP instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref COMP_COMPARATOR_INTERRUPT_EDGE_BOTH
  *           @arg @ref COMP_COMPARATOR_INTERRUPT_EDGE_RISING
  *           @arg @ref COMP_COMPARATOR_INTERRUPT_EDGE_FALLING
  */
__STATIC_INLINE uint32_t COMP_GetComper2InterruptEdge(COMP_COMMON_Type* COMPx)
{
    return (uint32_t)((READ_BIT(COMPx->ICR, COMP_ICR_CMP2SEL_Msk))>>COMP_ICR_CMP2SEL_Pos);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP1SEL    COMP_SetComper1InterruptEdge
  * @param    COMPx COMP instance
  * @param    edge This parameter can be one of the following values:
  *           @arg @ref COMP_COMPARATOR2_INTERRUPT_EDGE_BOTH
  *           @arg @ref COMP_COMPARATOR2_INTERRUPT_EDGE_RISING
  *           @arg @ref COMP_COMPARATOR2_INTERRUPT_EDGE_FALLING
  * @retval   None
  */
__STATIC_INLINE void COMP_SetComper1InterruptEdge(COMP_COMMON_Type* COMPx, uint32_t edge)
{
    MODIFY_REG(COMPx->ICR, COMP_ICR_CMP1SEL_Msk, edge<<COMP_ICR_CMP1SEL_Pos);
}

/**
  * @brief    
  * @rmtoll   ICR    CMP1SEL    COMP_GetComper1InterruptEdge
  * @param    COMPx COMP instance
  * @retval   Returned value can be one of the following values:
  *           @arg @ref COMP_COMPARATOR1_INTERRUPT_EDGE_BOTH
  *           @arg @ref COMP_COMPARATOR1_INTERRUPT_EDGE_RISING
  *           @arg @ref COMP_COMPARATOR1_INTERRUPT_EDGE_FALLING
  */
__STATIC_INLINE uint32_t COMP_GetComper1InterruptEdge(COMP_COMMON_Type* COMPx)
{
    return (uint32_t)((READ_BIT(COMPx->ICR, COMP_ICR_CMP1SEL_Msk))>>COMP_ICR_CMP2SEL_Pos);
}

/**
  * @brief    
  * @rmtoll   ISR    CMP2IF    COMP_IsActiveComparator2
  * @param    COMPx COMP instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t COMP_IsActiveComparator2(COMP_COMMON_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->ISR, COMP_ISR_CMP2IF_Msk) == (COMP_ISR_CMP2IF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    CMP2IF    COMP_ClearFlag_Comparator2
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_ClearFlag_Comparator2(COMP_COMMON_Type* COMPx)
{
    WRITE_REG(COMPx->ISR, COMP_ISR_CMP2IF_Msk);
}

/**
  * @brief    
  * @rmtoll   ISR    CMP1IF    COMP_IsActiveComparator1
  * @param    COMPx COMP instance
  * @retval   State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t COMP_IsActiveComparator1(COMP_COMMON_Type* COMPx)
{
    return (uint32_t)(READ_BIT(COMPx->ISR, COMP_ISR_CMP1IF_Msk) == (COMP_ISR_CMP1IF_Msk));
}

/**
  * @brief    
  * @rmtoll   ISR    CMP1IF    COMP_ClearFlag_Comparator1
  * @param    COMPx COMP instance
  * @retval   None
  */
__STATIC_INLINE void COMP_ClearFlag_Comparator1(COMP_COMMON_Type* COMPx)
{
    WRITE_REG(COMPx->ISR, COMP_ISR_CMP1IF_Msk);
}

/**
  * @}
  */

/** @defgroup COMP_EF_Init Initialization and de-initialization functions
  * @{
  */
ErrorStatus COMP_DeInit(COMP_Type *COMPx);
void COMP_StructInit(COMP_InitTypeDef *COMP_InitStruct);
ErrorStatus COMP_Init(COMP_Type *COMPx, COMP_InitTypeDef *COMP_InitStruct);

/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __FM33LC0XX_COMP_H*/

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2020-04-08**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
