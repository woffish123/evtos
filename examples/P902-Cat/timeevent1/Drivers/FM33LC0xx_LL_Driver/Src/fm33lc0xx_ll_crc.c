/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_crc.c
  * @author  FMSH Application Team
  * @brief   Src file of CRC LL Module
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
#include "fm33lc0xx_ll_rcc.h"
#include "fm33lc0xx_ll_rmu.h"
#include "fm33lc0xx_ll_crc.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_Driver_CRC
  * @{
  */

/** @addtogroup Private_Macros
  * @{
  */
#define         IS_CRC_INSTANCE(INTANCE)                         ((INTANCE) == CRC)

#define         IS_CRC_POLYNOMIAL_WIDTH(__VALUE__)               (((__VALUE__) == CRC_POLYNOMIAL_WIDTH_16BIT)||\
                                                                     ((__VALUE__) == CRC_POLYNOMIAL_WIDTH_32BIT)||\
                                                                     ((__VALUE__) == CRC_POLYNOMIAL_WIDTH_8BIT)||\
                                                                     ((__VALUE__) == CRC_POLYNOMIAL_WIDTH_7BIT))

#define         IS_CRC_DR_WIDTH(__VALUE__)                       (((__VALUE__) == CRC_CRC_DR_WIDTH_BYTE)||\
                                                                    ((__VALUE__) == CRC_CRC_DR_WIDTH_WORD))


#define         IS_CRC_OUPUT_REFLECTE_MODE(__VALUE__)            (((__VALUE__) == CRC_OUPUT_REFLECTE_MODE_NOMAL)||\
                                                                    ((__VALUE__) == CRC_OUPUT_REFLECTE_MODE_REVERSE))
                                                                    
#define         IS_CRC_INPUT_REFLECTE_MODE(__VALUE__)            (((__VALUE__) == CRC_INPUT_REFLECTE_MODE_NOMAL)||\
                                                                    ((__VALUE__) == CRC_INPUT_REFLECTE_MODE_BYTE)||\
                                                                    ((__VALUE__) == CRC_INPUT_REFLECTE_MODE_HALF_WORD)||\
                                                                    ((__VALUE__) == CRC_INPUT_REFLECTE_MODE_WORD))
                                                                    
#define         IS_CRC_CALCULA_MODE(__VALUE__)                   (((__VALUE__) == CRC_CALCULA_MODE_SERIAL)||\
                                                                    ((__VALUE__) == CRC_CALCULA_MODE_PARALLEL))

/**
  * @}
  */
/** @addtogroup CRC_EF_Init
  * @{
  */ 
/**
  * @brief  复位对应CRC寄存器.
  * @param  CRCx 
  * @retval ErrorStatus枚举值:
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus CRC_DeInit(CRC_Type *CRCx)
{
    
    assert_param(IS_CRC_INSTANCE(CRCx));
    
    /* 使能外设复位 */
    RCC_EnablePeripheralReset();
    /* 复位外设寄存器 */
    RCC_EnableResetAPB2Peripheral(RCC_APB2_PERIPHERAL_RST_CRC);
    RCC_DisableResetAPB2Peripheral(RCC_APB2_PERIPHERAL_RST_CRC);
    /* 关闭外设总线始时钟和工作时钟 */
    RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_CRC);
    /* 锁定外设复位 */
    RCC_DisablePeripheralReset();
    return PASS;
}
/**
  * @brief  根据 CRC_InitStruct 的配置信息初始化对应外设入口地址的寄存器值.
  * @param  CRCx CRCx 
  * @param  CRC_InitStruct 指向一个 @ref CRC_InitTypeDef 结构体
  *         其中包含了外设的相关配置信息.
  * @retval ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS 配置成功  
  */
ErrorStatus CRC_Init(CRC_Type *CRCx,CRC_InitTypeDef *CRC_InitStruct)
{
    /* 参数检查 */
    assert_param(IS_CRC_INSTANCE(CRCx));     
    assert_param(IS_CRC_DR_WIDTH(CRC_InitStruct->DataWidth));
    assert_param(IS_CRC_CALCULA_MODE(CRC_InitStruct->CalculatMode));
    assert_param(IS_CRC_POLYNOMIAL_WIDTH(CRC_InitStruct->PolynomialWidth));
    assert_param(IS_CRC_INPUT_REFLECTE_MODE(CRC_InitStruct->ReflectIn)); 
    assert_param(IS_CRC_OUPUT_REFLECTE_MODE(CRC_InitStruct->ReflectOut));
    RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_CRC);
    
    CRC_SetCalcuaMode(CRCx,CRC_InitStruct->CalculatMode);
    CRC_SetInputReflection(CRCx,CRC_InitStruct->ReflectIn);
    CRC_SetOutPutReflection(CRCx,CRC_InitStruct->ReflectOut);
    CRC_SetPolynomialWidth(CRCx,CRC_InitStruct->PolynomialWidth);
    CRC_SetOutPutXORReg(CRCx,CRC_InitStruct->XorReg);
    CRC_SetPolynominalsReg(CRCx,CRC_InitStruct->Polynomial);
    CRC_SetLinearShiftReg(CRCx,CRC_InitStruct->InitVel);
    CRC_SetDataRegWidth(CRCx, CRC_InitStruct->DataWidth);
    if(CRC_InitStruct->XorRegState == ENABLE)
    {
        CRC_Enable_OutPutXOR(CRCx);
    }
    else
    {
        CRC_Disable_OutPutXOR(CRCx);
    }
    
    return PASS;
}  
  
  
/**
  * @brief	设置 CRC_InitStruct 为默认配置
  * @param 	CRC_InitStruct 指向需要将值设置为默认配置的结构体 @ref CRC_InitTypeDef 结构体  
  *         
  * @retval	None
  */

void CRC_StructInit(CRC_InitTypeDef *CRC_InitStruct)
{
    
    CRC_InitStruct->Polynomial      =  0x00000000;
    CRC_InitStruct->PolynomialWidth =  CRC_POLYNOMIAL_WIDTH_16BIT;
    CRC_InitStruct->DataWidth       =  CRC_CRC_DR_WIDTH_BYTE;
    CRC_InitStruct->CalculatMode    =  CRC_CALCULA_MODE_SERIAL;
    CRC_InitStruct->ReflectIn       =  CRC_INPUT_REFLECTE_MODE_NOMAL; 
    CRC_InitStruct->ReflectOut      =  CRC_OUPUT_REFLECTE_MODE_NOMAL;
    CRC_InitStruct->XorReg          =  0x00000000;
    CRC_InitStruct->XorRegState     =  DISABLE;
    
}


/**
  * @}
  */
  
/**
  * @}
  */

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-25**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
