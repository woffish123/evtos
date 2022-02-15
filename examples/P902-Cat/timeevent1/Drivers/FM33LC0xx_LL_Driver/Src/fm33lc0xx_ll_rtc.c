/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_rtc.c
  * @author  FMSH Application Team
  * @brief   Src file of RTC LL Module
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
#include "fm33lc0xx_ll_rtc.h"
#include "fm33_assert.h"  
/** @addtogroup FM33LC0xx_Driver_RTC
  * @{
  */
/** @addtogroup TIM_Private_Macros
  * @{
  */
#define IS_RTC_INSTANCE(RTCx)                     ((RTCx) == RTC)  
  
/**
  * @}
  */

/** @addtogroup RTC_EF_Init
  * @{
  */   
/**
  * @brief  复位对应RTCx寄存器.
  * @param  RTCx 
  * @retval ErrorStatus枚举值:
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus RTC_DeInit(RTC_Type *RTCx)
{
    ErrorStatus result = PASS;
    /* Check the parameters */
    assert_param(IS_RTC_INSTANCE(RTCx));
    RTCx->IER       = 0x00000000U;
    RTCx->WER       = 0x00000000U;
    RTCx->ADJUST    = 0x00000000U;
    RTCx->ADSIGN    = 0x00000000U;
    RTCx->ALARM     = 0x00000000U;
    RTCx->BCDDAY    = 0x00000000U;
    RTCx->BCDHOUR   = 0x00000000U;
    RTCx->BCDMIN    = 0x00000000U;
    RTCx->BCDMONTH  = 0x00000000U;
    RTCx->BCDSEC    = 0x00000000U;
    RTCx->BCDWEEK   = 0x00000000U;
    RTCx->BCDYEAR   = 0x00000000U;
    RTCx->SBSCNT    = 0x00000000U;
    RTCx->TMSEL     = 0x00000000U;
    RTCx->WER       = 0x00000000U;
    return result;
}
/**
  * @brief  配置实时时钟相关.
  * @param  RTCx Timer Instance
  * @param  RTC_InitStruct 指向一个 @ref RTC_InitTypeDef(时基配置结构体)
  * @retval ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS 成功  
  */
ErrorStatus RTC_Init(RTC_Type *RTCx, RTC_InitTypeDef *InitStruct)
{

    /* 参数检查 */
    assert_param(IS_RTC_INSTANCE(RTCx));
    /* 时钟总线使能配置 */ 
    RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_RTC);
    /* 配置时间 */
    RTC_ConfigTime(RTCx,InitStruct);
    return PASS;
}

/**
  * @brief  设置实时时钟
  * @param  RTCx Timer Instance
  * @param  RTC_InitStruct 指向一个 @ref RTC_InitTypeDef(时基配置结构体)
  * @retval ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS 成功  
  */
ErrorStatus RTC_ConfigTime(RTC_Type *RTCx,RTC_InitTypeDef *InitStruct)
{

    /* 使能时间配置 */
    RTC_EnableConfigTime(RTCx);
    /* 配置秒 */
    RTC_SetSecondReg(RTCx,InitStruct->Second);
    /* 配置分钟 */
    RTC_SetMinutesReg(RTCx,InitStruct->Minute);
    /* 配置小时 */
    RTC_SetHoursReg(RTCx,InitStruct->Hour);
    /* 配置日期 */
    RTC_SetDaysReg(RTCx,InitStruct->Day);
    /* 配置周 */
    RTC_SetWeeksReg(RTCx,InitStruct->Week);
    /* 配置月 */
    RTC_SetMonthReg(RTCx,InitStruct->Month);
    /* 配置年 */
    RTC_SetYearsReg(RTCx,InitStruct->Year);
    /* 锁定时间配置 */
    RTC_DisableConfigTime(RTCx);
    
    return PASS;
}
/**
  * @brief  获取实时时间并保存到指定结构体中
  * @param  RTCx Timer Instance
  * @param  RTC_InitStruct 指向一个 @ref RTC_InitTypeDef(时基配置结构体)
  * @retval ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS 成功  
  */
ErrorStatus RTC_GetTime(RTC_Type *RTCx,RTC_InitTypeDef *InitStruct)
{

    /* 配置秒 */
    InitStruct->Second = RTC_GetSecondReg(RTCx);
    /* 配置分钟 */
    InitStruct->Minute  =RTC_GetMinutesReg(RTCx);
    /* 配置小时 */
    InitStruct->Hour = RTC_GetHoursReg(RTCx);
    /* 配置日期 */
    InitStruct->Day = RTC_GetDaysReg(RTCx);
    /* 配置周 */
    InitStruct->Week = RTC_GetWeeksReg(RTCx);
    /* 配置月 */
    InitStruct->Month = RTC_GetMonthReg(RTCx);
    /* 配置年 */
    InitStruct->Year = RTC_GetYearsReg(RTCx);

    return PASS;
}
/**
  * @brief	设置 InitStruct 为默认配置
  * @param 	InitStruct 指向需要将值设置为默认配置的结构体 @ref RTC_InitTypeDef 结构体  
  *         
  * @retval	None
  */

void RTC_StructInit(RTC_InitTypeDef *InitStruct)
{
    /*  */
    InitStruct->Year    = 0x00;
    InitStruct->Month   = 0x00;
    InitStruct->Day     = 0x00;
    InitStruct->Week    = 0x00;
    InitStruct->Hour    = 0x00;
    InitStruct->Minute  = 0x00;
    InitStruct->Second  = 0x00;
}

/**
  * @}
  */


/**
  * @}
  */
/***************************Py_Code_Generator Version: 0.1-0.5- @ 2019-11-07***************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
