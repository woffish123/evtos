/**
  ****************************************************************************************************
  * @file    fm33lc0xx_ll_wwdt.c
  * @author  FMSH Application Team
  * @brief   Src file of WWDT LL Module
  ****************************************************************************************************
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
  ****************************************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "fm33lc0xx_ll_rcc.h"
#include "fm33lc0xx_ll_wwdt.h"
#include "fm33_assert.h"
/** @addtogroup fm33lC0xx_Driver_WWDT
  * @{
  */


/** @addtogroup Private_Macros
  * @{
  */
#define         IS_WWDT_INSTANCE(INTANCE)                ((INTANCE) == WWDT)

#define         IS_WWDT_OVERFLOWPERIOD(__VALUE__)            (((__VALUE__) == WWDT_OVERFLOW_PERIOD_X1)||\
                                                                 ((__VALUE__) == WWDT_OVERFLOW_PERIOD_X4)||\
                                                                 ((__VALUE__) == WWDT_OVERFLOW_PERIOD_X16)||\
                                                                 ((__VALUE__) == WWDT_OVERFLOW_PERIOD_X64)||\
                                                                 ((__VALUE__) == WWDT_OVERFLOW_PERIOD_X128)||\
                                                                 ((__VALUE__) == WWDT_OVERFLOW_PERIOD_X256)||\
                                                                 ((__VALUE__) == WWDT_OVERFLOW_PERIOD_X512)||\
                                                                 ((__VALUE__) == WWDT_OVERFLOW_PERIOD_X1024))
/**
  * @}
  */
  
/** @addtogroup WWDT_EF_Init
  * @{
  */ 
  
/**
  * @brief	关闭WWDT外设总线时钟
  *
  * @param	外设入口地址
  *
  * @retval	返回错误状态，可能值：
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus WWDT_DeInit(WWDT_Type* WWDTx)
{
    assert_param(IS_WWDT_INSTANCE(WWDTx));
   
    return PASS;
}
/**
  * @brief	根据 WWDT_InitStruct初始化对应外设入口地址的寄存器值. 
  *         
  * @note 	WWTD使能后将无法关闭，直到下一次芯片复位
  *      	      
  * @param	WWDTx  外设入口地址
  *
  * @param 	WWDT_InitTypeDef结构体，它包含指定WWDT外设的配置信息	  
  *         
  * @retval	ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS LPUART配置成功        
  */
ErrorStatus WWDT_Init(WWDT_Type* WWDTx, WWDT_InitTypeDef *WWDT_InitStruct)
{
    ErrorStatus status = PASS;

    /* 入口参数检查 */
    assert_param(IS_WWDT_INSTANCE(WWDTx));
    assert_param(IS_WWDT_OVERFLOWPERIOD(WWDT_InitStruct->OverflowPeriod));
    /* 开启总线时钟 */
    RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_WWDT);
    /* 配置独立看门狗溢出周期 */
    WWDT_SetCounterPeriod(WWDTx,WWDT_InitStruct->OverflowPeriod);
    /* 启动看门狗 */
    WWDT_EnableCounter(WWDTx);
    
    return status;
}
/**
  * @brief	设置 WWDT_InitStruct 为默认配置
  *
  * @param 	WWDT_InitStruct 指向需要将值设置为默认配置的结构体 @ref WWDT_InitTypeDef 结构体 
  *         
  * @retval	None
  */
void WWDT_StructInit(WWDT_InitTypeDef *WWDT_InitStruct)
{
    /* 默认最长溢出周期 */
    WWDT_InitStruct->OverflowPeriod = WWDT_OVERFLOW_PERIOD_X1024;
}
/** 
  *@} 
  */
  
/** 
  *@} 
  */
/*************************************************************END OF FILE************************************************************/
