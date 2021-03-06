/**
  ****************************************************************************************************
  * @file    fm33lc0xx_ll_svd.c
  * @author  FMSH Application Team
  * @brief   Src file of SVD LL Module
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
#include "fm33lc0xx_ll_svd.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_Driver_SVD
  * @{
  */
  
/* Private macros ------------------------------------------------------------*/
/** @addtogroup UART_Private_Macros
  * @{
  */
#define         IS_SVD_INSTANCE(INSTANCE)               (((INSTANCE) == SVD))

#define         IS_SVD_MODE(__VALUE__)                  (((__VALUE__) == SVD_WORK_MODE_ALWAYS)||\
                                                            ((__VALUE__) == SVD_WORK_MODE_INTERMITTENT))

#define         IS_SVD_INTERVAL(__VALUE__)              (((__VALUE__) == SVD_INTERVEL_ENABLE_PERIOD_62MS)||\
                                                            ((__VALUE__) == SVD_INTERVEL_ENABLE_PERIOD_256MS)||\
                                                            ((__VALUE__) == SVD_INTERVEL_ENABLE_PERIOD_1000MS)||\
                                                            ((__VALUE__) == SVD_INTERVEL_ENABLE_PERIOD_4000MS))

#define         IS_SVD_THRESHOLD(__VALUE__)             (((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP0)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP1)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP2)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP3)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP4)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP5)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP6)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP7)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP8)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP9)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP10)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP11)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP12)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP13)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP14)||\
                                                            ((__VALUE__) == SVD_WARNING_THRESHOLD_LEVEL_STANDARD_SVSBASE))

#define         IS_SVD_SVSCONFIG(__VALUE__)             (((__VALUE__) == DISABLE)||\
                                                            ((__VALUE__) == ENABLE))
                                                             
#define         IS_SVD_VREFSELECT(__VALUE__)            (((__VALUE__) == SVD_REFERENCE_VOLTAGE_1_0_VOLT)||\
                                                            ((__VALUE__) == SVD_REFERENCE_VOLTAGE_1_1_VOLT)||\
                                                            ((__VALUE__) == SVD_REFERENCE_VOLTAGE_1_2_VOLT)) 
                                                            
#define         IS_SVD_DIGITALFILTER(__VALUE__)         (((__VALUE__) == DISABLE)||\
                                                            ((__VALUE__) == ENABLE))                                                             
/** 
  *@} 
  */
/** @addtogroup SVD_EF_Init
  * @{
  */   
/**
  * @brief	??????SVD ??????????????????????????????
  * @param	??????????????????
  * @retval	?????????????????????????????????
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus SVD_DeInit(SVD_Type* SVDx)
{
    ErrorStatus status = PASS;

    /* ??????????????????????????? */
    assert_param(IS_SVD_INSTANCE(SVDx));
	SVDx->CFGR  = 0x00000C08U;
    SVDx->CR    = 0x00000000U; 
    SVDx->IER   = 0x00000000U;
    SVDx->VSR   = 0x00000004U;
    
    return (status);
}


/**
  * @brief	?????? SVD_InitStruct ????????????????????????????????????PMU????????? 
  *         
  * @note 	????????????????????????????????????????????????????????????????????? @ref fm33lc0xx_ll_RCC.h??????????????????
  *      	???????????????????????????????????????????????????????????????????????????      
  * @param	PMUx  ??????????????????
  * @param 	SVD_InitStruct ???????????? @ref SVD_InitTypeDef ????????????????????????????????????LPUART?????????????????????	  
  *         
  * @retval	ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS LPUART????????????        
  */
ErrorStatus SVD_Init(SVD_Type* SVDx, SVD_InitTypeDef *SVD_InitStruct)
{
    ErrorStatus status = FAIL;
	/* ????????????????????? */
    assert_param(IS_SVD_INSTANCE(SVDx));
    assert_param(IS_SVD_MODE(SVD_InitStruct->Mode));
    assert_param(IS_SVD_INTERVAL(SVD_InitStruct->Interval));
    assert_param(IS_SVD_THRESHOLD(SVD_InitStruct->Threshold));
    assert_param(IS_SVD_SVSCONFIG(SVD_InitStruct->SVSChannel));
    assert_param(IS_SVD_VREFSELECT(SVD_InitStruct->VrefSelect));
    assert_param(IS_SVD_DIGITALFILTER(SVD_InitStruct->DigitalFilter));
    
    RCC_Group1_EnableBusClock(RCC_BUS1_CLOCK_ANAC);
    /* ???????????? 1 */
    SVD_SetSVDWorkMode(SVDx,SVD_InitStruct->Mode);
    /* ???????????? */
    SVD_SetSVDThresholdLevel(SVDx,SVD_InitStruct->Threshold);
    /* ?????????????????? */
    SVD_SetSVDInterValEnablePeriod(SVDx,SVD_InitStruct->Interval);
    /* ???????????????????????? */
    SVD_EnableReference(SVD,SVD_InitStruct->VrefSelect);
    /* ?????????????????? */
    if(SVD_InitStruct->SVSChannel == ENABLE)
    {
        SVD_EnableSVDSVSChannel(SVDx);
    }
    else
    {
        SVD_DisableSVDSVDSVSChannel(SVDx);
    }
    /* ?????????????????? */
    if(SVD_InitStruct->DigitalFilter == ENABLE)
    {
        SVD_EnableSVDDigitalFiltering(SVDx);
    }
    else
    {
        SVD_DisableSVDDigitalFiltering(SVDx);
    }
    /* MOD=1 ???????????? 1 */
    if(SVD_InitStruct->Mode == SVD_WORK_MODE_INTERMITTENT)
    {
        if(!SVD_IsEnabledSVDDigitalFiltering(SVDx))
        {
            SVD_EnableSVDDigitalFiltering(SVDx);
        }
    }
    status = PASS;
    return status;
}


/**
  * @brief	SVD_InitStruct ???????????????
  * @param 	SVD_InitStruct ??????????????????????????????????????????????????? @ref SVD_InitTypeDef structure 
  *         ?????????
  * @retval	None
  */

void SVD_StructInit(SVD_InitTypeDef *SVD_InitStruct)
{
    
    SVD_InitStruct->SVSChannel      = DISABLE;
    SVD_InitStruct->DigitalFilter   = ENABLE;
    SVD_InitStruct->VrefSelect      = SVD_REFERENCE_VOLTAGE_1_0_VOLT;
    SVD_InitStruct->Mode            = SVD_WORK_MODE_INTERMITTENT;
    SVD_InitStruct->Interval        = SVD_INTERVEL_ENABLE_PERIOD_62MS;
    SVD_InitStruct->Threshold       = SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP0;

}
/** 
  *@} 
  */
/** 
  *@} 
  */
/*********************** (C) COPYRIGHT Fudan Microelectronics *****END OF FILE************************/

