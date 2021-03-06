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
  * @brief  ????????????CRC?????????.
  * @param  CRCx 
  * @retval ErrorStatus?????????:
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus CRC_DeInit(CRC_Type *CRCx)
{
    
    assert_param(IS_CRC_INSTANCE(CRCx));
    
    /* ?????????????????? */
    RCC_EnablePeripheralReset();
    /* ????????????????????? */
    RCC_EnableResetAPB2Peripheral(RCC_APB2_PERIPHERAL_RST_CRC);
    RCC_DisableResetAPB2Peripheral(RCC_APB2_PERIPHERAL_RST_CRC);
    /* ?????????????????????????????????????????? */
    RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_CRC);
    /* ?????????????????? */
    RCC_DisablePeripheralReset();
    return PASS;
}
/**
  * @brief  ?????? CRC_InitStruct ???????????????????????????????????????????????????????????????.
  * @param  CRCx CRCx 
  * @param  CRC_InitStruct ???????????? @ref CRC_InitTypeDef ?????????
  *         ??????????????????????????????????????????.
  * @retval ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS ????????????  
  */
ErrorStatus CRC_Init(CRC_Type *CRCx,CRC_InitTypeDef *CRC_InitStruct)
{
    /* ???????????? */
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
  * @brief	?????? CRC_InitStruct ???????????????
  * @param 	CRC_InitStruct ??????????????????????????????????????????????????? @ref CRC_InitTypeDef ?????????  
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
