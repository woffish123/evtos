/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_gptim.c
  * @author  FMSH Application Team
  * @brief   Src file of GPTIM LL Module
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
/* Includes ------------------------------------------------------------------*/
#include "fm33lc0xx_ll_rcc.h"
#include "fm33lc0xx_ll_rmu.h"
#include "fm33lc0xx_ll_gptim.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_Driver_GPTIM
  * @{
  */
  
  
/** @addtogroup TIM_Private_Macros
  * @{
  */


#define IS_GPTIM_INSTANCE(TIMx)                     (((TIMx) == GPTIM0) \
                                                        ||((TIMx) == GPTIM1))

#define IS_GPTIM_COUNTERMODE(__VALUE__)          (((__VALUE__) == GPTIM_COUNTER_DIRECT_UP) \
                                                       || ((__VALUE__) == GPTIM_COUNTER_DIRECT_DOWN) \
                                                       || ((__VALUE__) == GPTIM_COUNTER_ALIGNED_CENTER_DOWN   ) \
                                                       || ((__VALUE__) == GPTIM_COUNTER_ALIGNED_CENTER_UP     ) \
                                                       || ((__VALUE__) == GPTIM_COUNTER_ALIGNED_CENTER_UP_DOWN))

#define IS_GPTIM_CLOCKDIVISION(__VALUE__)        (((__VALUE__) == GPTIM_CLOCKDIVISION_DIV1) \
                                                     || ((__VALUE__) == GPTIM_CLOCKDIVISION_DIV2) \
                                                     || ((__VALUE__) == GPTIM_CLOCKDIVISION_DIV4))


#define IS_GPTIM_CC_MODE(__VALUE__)               (((__VALUE__) ==   GPTIM_CC_MODE_OUTPUT) \
                                                       ||((__VALUE__) == GPTIM_CC1_MODE_INPUT_TI1) \
                                                       || ((__VALUE__) == GPTIM_CC1_MODE_INPUT_TI2) \
                                                       || ((__VALUE__) == GPTIM_CC2_MODE_INPUT_TI2) \
                                                       || ((__VALUE__) == GPTIM_CC2_MODE_INPUT_TI1) \
                                                       || ((__VALUE__) == GPTIM_CC3_MODE_INPUT_TI3) \
                                                       || ((__VALUE__) == GPTIM_CC3_MODE_INPUT_TI4) \
                                                       || ((__VALUE__) == GPTIM_CC4_MODE_INPUT_TI3) \
                                                       || ((__VALUE__) == GPTIM_CC4_MODE_INPUT_TI4) \
                                                       || ((__VALUE__) == GPTIM_CC_MODE_INPUT_TRC))
                                      
#define IS_GPTIM_IC_FILTER(__VALUE__)            (((__VALUE__) == GPTIM_IC_FILTER_FDIV1) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV1_N2) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV1_N4) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV1_N8) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV2_N6) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV2_N8) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV4_N6) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV4_N8) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV8_N6) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV8_N8) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV16_N5) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV16_N6) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV16_N8) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV32_N5) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV32_N6) \
                                                     || ((__VALUE__) == GPTIM_IC_FILTER_FDIV32_N8))

#define IS_GPTIM_CHANNEL(__VALUE__)              (((__VALUE__) == GPTIM_CHANNEL_1)\
                                                    || ((__VALUE__) == GPTIM_CHANNEL_2)\
                                                    || ((__VALUE__) == GPTIM_CHANNEL_3)\
                                                    || ((__VALUE__) == GPTIM_CHANNEL_4))
                                        
                                        
                                        
#define IS_GPTIM_SLAVE_MODE(__VALUE__)           (((__VALUE__) == GPTIM_SLAVE_MODE_PROHIBITED)\
                                                    || ((__VALUE__) == GPTIM_SLAVE_MODE_TI2FP2_TI1FP1_CNT)\
                                                    || ((__VALUE__) == GPTIM_SLAVE_MODE_TI1FP1_TI2FP2_CNT)\
                                                    || ((__VALUE__) == GPTIM_SLAVE_MODE_TI2FP2_CNT_TI1FP1_CNT)\
                                                    || ((__VALUE__) == GPTIM_SLAVE_MODE_TRGI_RISE_RST)\
                                                    || ((__VALUE__) == GPTIM_SLAVE_MODE_TRGI_HIGH_RUN)\
                                                    || ((__VALUE__) == GPTIM_SLAVE_MODE_TRGI_RISE_RUN)\
                                                    || ((__VALUE__) == GPTIM_SLAVE_MODE_TRGI_RISE_CNT))


#define IS_GPTIM_TRIGGER_SRC(__VALUE__)          (((__VALUE__) ==GPTIM_TIM_TS_ITR0 )\
                                                        ||((__VALUE__) ==GPTIM_TIM_TS_ITR1 )\
                                                        ||((__VALUE__) ==GPTIM_TIM_TS_ITR2)\
                                                        ||((__VALUE__) ==GPTIM_TIM_TS_ITR3)\
                                                        ||((__VALUE__) ==GPTIM_TIM_TS_TI1F_ED)\
                                                        ||((__VALUE__) ==GPTIM_TIM_TS_TI1FP1)\
                                                        ||((__VALUE__) ==GPTIM_TIM_TS_TI2FP2)\
                                                        ||((__VALUE__) ==GPTIM_TIM_TS_ETRF))    



#define IS_GPTIM_ETP_FILTER(__VALUE__)           (((__VALUE__) == GPTIM_ETR_FILTER_FDIV1) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV1_N2) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV1_N4) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV1_N8) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV2_N6) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV2_N8) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV4_N6) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV4_N8) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV8_N6) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV8_N8) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV16_N5) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV16_N6) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV16_N8) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV32_N5) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV32_N6) \
                                                     || ((__VALUE__) == GPTIM_ETR_FILTER_FDIV32_N8))


#define IS_GPTIM_ETR_PSC(__VALUE__)               (((__VALUE__) == GPTIM_ETR_PRESCALER_DIV1) \
                                                     || ((__VALUE__) == GPTIM_ETR_PRESCALER_DIV2) \
                                                     || ((__VALUE__) == GPTIM_ETR_PRESCALER_DIV4) \
                                                     || ((__VALUE__) == GPTIM_ETR_PRESCALER_DIV8))
                                 
                                 

#define IS_GPTIM_ETR_POLARITY(__VALUE__)         (((__VALUE__) == GPTIM_ETR_POLARITY_NONINVERTED) \
                                                     || ((__VALUE__) == GPTIM_ETR_POLARITY_NINVERTED)) 

                                   
#define IS_GPTIM_OCPOLARITY(__VALUE__)           (((__VALUE__) == GPTIM_OC_POLARITY_NOMAL) \
                                                    || ((__VALUE__) == GPTIM_OC_POLARITY_REVERSE))
                                                    
#define IS_GPTIM_IC_POLARITY(__VALUE__)          (((__VALUE__) == GPTIM_IC_POLARITY_RISING) \
                                                       || ((__VALUE__) == GPTIM_IC_POLARITY_FALLING))
    
                                       
 
#define IS_GPTIM_IC_PRESCALER(__VALUE__)         (((__VALUE__) == GPTIM_IC_PRESCALER_DIV_1) \
                                                     || ((__VALUE__) == GPTIM_IC_PRESCALER_DIV_2) \
                                                     || ((__VALUE__) == GPTIM_IC_PRESCALER_DIV_4) \
                                                     || ((__VALUE__) == GPTIM_IC_PRESCALER_DIV_8))


#define IS_GPTIM_IC_ACTIVEINPUT(__VALUE__)          (((__VALUE__) == GPTIM_CC1_MODE_INPUT_TI1) \
                                                       || ((__VALUE__) == GPTIM_CC1_MODE_INPUT_TI2) \
                                                       || ((__VALUE__) == GPTIM_CC2_MODE_INPUT_TI2) \
                                                       || ((__VALUE__) == GPTIM_CC2_MODE_INPUT_TI1) \
                                                       || ((__VALUE__) == GPTIM_CC3_MODE_INPUT_TI3) \
                                                       || ((__VALUE__) == GPTIM_CC3_MODE_INPUT_TI4) \
                                                       || ((__VALUE__) == GPTIM_CC4_MODE_INPUT_TI3) \
                                                       || ((__VALUE__) == GPTIM_CC4_MODE_INPUT_TI4) \
                                                       || ((__VALUE__) == GPTIM_CC_MODE_INPUT_TRC))
                                                       
#define IS_GPTIM_OC_POLARITY(__VALUE__)          (((__VALUE__) == GPTIM_OC_POLARITY_NOMAL) \
                                                    || ((__VALUE__) == GPTIM_OC_POLARITY_REVERSE))



#define IS_GPTIM_OC_MODE(__VALUE__)               (((__VALUE__) ==   GPTIM_OC_MODE_FROZEN) \
                                                      || ((__VALUE__) == GPTIM_OC_MODE_ACTIVE) \
                                                      || ((__VALUE__) == GPTIM_OC_MODE_INACTIVE) \
                                                      || ((__VALUE__) == GPTIM_OC_MODE_TOGGLE) \
                                                      || ((__VALUE__) == GPTIM_OC_MODE_FORCED_INACTIVE) \
                                                      || ((__VALUE__) == GPTIM_OC_MODE_FORCED_ACTIVE) \
                                                      || ((__VALUE__) == GPTIM_OC_MODE_PWM1) \
                                                      || ((__VALUE__) == GPTIM_OC_MODE_PWM2))
                                                      
#define IS_GPTIM_OC_FASTMODE(__VALUE__)          (((__VALUE__) == ENABLE) \
                                                    || ((__VALUE__) == DISABLE))

#define IS_GPTIM_OC_PRELOAD(__VALUE__)           (((__VALUE__) == ENABLE) \
                                                    || ((__VALUE__) == DISABLE))
                                                    
#define IS_GPTIM_OC_ETR_CLEARN(__VALUE__)        (((__VALUE__) == ENABLE) \
                                                    || ((__VALUE__) == DISABLE)) 
     

#define IS_GPTIM_TRIGGER_DELAY(__VALUE__)       (((__VALUE__) == DISABLE) \
                                                    || ((__VALUE__) == ENABLE))
                                                        
                                                        
#define IS_GPTIM_IC_CAPTURE_STATE(__VALUE__)    (((__VALUE__) == DISABLE) \
                                                     || ((__VALUE__) == ENABLE))                                                        
/**
  * @}
  */


/* Private function prototypes -----------------------------------------------*/
/** @defgroup TIM_Private_Functions TIM Private Functions
  * @{
  */
static ErrorStatus  OCConfig(GPTIM_Type *TIMx, uint32_t Channel, GPTIM_OC_InitTypeDef *TIM_OC_InitStruct );
static ErrorStatus  SetITRxInputSrc(GPTIM_Type *TIMx,uint32_t ITRx,uint32_t ITRx_Src);
/**
  * @}
  */

/** @addtogroup TIM_EF_Init
  * @{
  */

/**
  * @brief  ????????????GPTIMx?????????.
  * @param  GPTIMx 
  * @retval ErrorStatus?????????:
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus GPTIM_DeInit(GPTIM_Type *TIMx)
{
    ErrorStatus result = PASS;

    /* Check the parameters */
    assert_param(IS_GPTIM_INSTANCE(TIMx));
    /* ?????????????????? */
    RCC_EnablePeripheralReset();
    if(TIMx == GPTIM0)
    {
        /* ?????????????????? */
        RCC_EnableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_GTTIM0);
        RCC_DisableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_GTTIM0);
        /* ?????????????????? */
        RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_GTIM0);
    }
    else if(TIMx == GPTIM1)
    {
        /* ?????????????????? */
        RCC_EnableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_GTTIM1);
        RCC_DisableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_GTTIM1);
        /* ?????????????????? */
        RCC_Group4_DisableBusClock(RCC_BUS4_CLOCK_GTIM1);
    }
    else
    {
        result = FAIL;
    }
    /* ?????????????????? */
    RCC_DisablePeripheralReset();
    return result;
}
/**
  * @brief  ??????????????????????????????????????????????????????.
  * @param  TIMx Timer Instance
  * @param  TIM_InitStruct ???????????? @ref GPTIM_InitTypeDef(?????????????????????)
  * @retval ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS ??????  
  */
ErrorStatus GPTIM_Init(GPTIM_Type *TIMx, GPTIM_InitTypeDef *TIM_InitStruct)
{

    /* ???????????? */
    assert_param(IS_GPTIM_INSTANCE(TIMx));
    assert_param(IS_GPTIM_COUNTERMODE(TIM_InitStruct->CounterMode));
    assert_param(IS_GPTIM_CLOCKDIVISION(TIM_InitStruct->ClockDivision));
    /* ???????????????????????? */
    if(TIMx == GPTIM0)
    {
        RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM0);
    }
    else
    {
        RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM1);
    }
    /* ??????????????????????????? */
    switch(TIM_InitStruct->CounterMode)
    {
        /* ?????????????????? */
        case GPTIM_COUNTER_ALIGNED_CENTER_DOWN   :   
        case GPTIM_COUNTER_ALIGNED_CENTER_UP     : 
        case GPTIM_COUNTER_ALIGNED_CENTER_UP_DOWN:
        GPTIM_SetCounterAlignedMode(TIMx,TIM_InitStruct->CounterMode);
        break;
        default:
        /* ???????????? */
        GPTIM_SetCounterDirection(TIMx,TIM_InitStruct->CounterMode);
        GPTIM_SetCounterAlignedMode(TIMx,GPTIM_COUNTER_ALIGNED_EDGE);
        break;
    }
    /* ?????????????????? */
    GPTIM_SetAutoReload(TIMx,TIM_InitStruct->Autoreload );
    /* ???????????????????????????????????????????????????????????????????????? */
    GPTIM_SetClockDivision(TIMx,TIM_InitStruct->ClockDivision);
    /* ???????????? */
    GPTIM_SetPrescaler(TIMx,TIM_InitStruct->Prescaler);
    /* ??????????????? */
    if(TIM_InitStruct->AutoreloadState == ENABLE)
    {
        GPTIM_EnableARRPreload(TIMx);
    }
    else
    {
        GPTIM_DisableARRPreload(TIMx);
    }
    /* ????????????????????????????????????????????? */
    GPTIM_GenerateEvent_UPDATE(TIMx);
    
    return PASS;
}

/**
  * @brief	?????? GPTIM_InitTypeDef ???????????????
  * @param 	TIM_InitStruct ??????????????????????????????????????????????????? @ref GPTIM_InitTypeDef ?????????  
  *         
  * @retval	None
  */
void GPTIM_StructInit(GPTIM_InitTypeDef *TIM_InitStruct)
{
  /* Set the default configuration */
  TIM_InitStruct->Prescaler         = (uint16_t)0x0000;
  TIM_InitStruct->AutoreloadState   = DISABLE;
  TIM_InitStruct->CounterMode       = GPTIM_COUNTER_DIRECT_UP;
  TIM_InitStruct->Autoreload        = 0xFFFFU;
  TIM_InitStruct->ClockDivision     = GPTIM_CLOCKDIVISION_DIV1;
}

/**
  * @brief  ??????????????????????????????????????????????????????????????????.
  * @param  TIMx Timer Instance
  * @param  TIM_InitStruct ???????????? @ref GPTIM_SlaveInitTypeDef ????????? 
  * @retval ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS ??????  
  */
ErrorStatus GPTIM_SlaveMode_Init(GPTIM_Type *TIMx, GPTIM_SlaveInitTypeDef *TIM_InitStruct)
{

    
    /* ???????????? */
    assert_param(IS_GPTIM_INSTANCE(TIMx)); 
    assert_param(IS_GPTIM_SLAVE_MODE(TIM_InitStruct->SlaveMode));
    assert_param(IS_GPTIM_TRIGGER_SRC(TIM_InitStruct->TriggerSrc));
    assert_param(IS_GPTIM_TRIGGER_DELAY(TIM_InitStruct->TriggerDelay));
    /* ???????????????????????? */
    if(TIMx == GPTIM0)
    {
        RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM0);
    }
    else
    {
        RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM1);
    }
    /* ???????????????????????? */
    GPTIM_DisableMasterSlaveMode(TIMx);
    /* ???????????????????????????TS */  
    GPTIM_SetSlaveMode(TIMx,0);
    /* ???????????????????????? */
    GPTIM_SetTriggerInput(TIMx,TIM_InitStruct->TriggerSrc);
    /* ITRx ??????????????? */
    if(TIM_InitStruct->TriggerSrc <= GPTIM_TIM_TS_ITR3)
    {
        /* ????????????ITRx????????? */
        SetITRxInputSrc(TIMx,TIM_InitStruct->TriggerSrc,TIM_InitStruct->ITRxSrc);
    }
    /* ??????????????? */
    GPTIM_SetSlaveMode(TIMx,TIM_InitStruct->SlaveMode);
    /* ???????????????????????? */
    if(TIM_InitStruct->TriggerDelay == ENABLE)
    {
        GPTIM_EnableMasterSlaveMode(TIMx);
    }
    /* ????????????????????????????????????????????? */
    GPTIM_GenerateEvent_UPDATE(TIMx);
    return PASS;
}
/**
  * @brief	?????? ???????????????????????????
  * @param  TIMx Timer Instance
  * @param 	ITRx ?????????????????????  
  * @param 	ITRx_?????????????????????ITRx?????????????????????????????????????????????.
  * @retval	None
  *			-FAIL ????????????????????????        
  *			-PASS ??????  
  */
static ErrorStatus SetITRxInputSrc(GPTIM_Type *TIMx,uint32_t ITRx,uint32_t ITRx_Src)
{
    ErrorStatus result = PASS;
    /* ?????????ITRx?????????????????????????????????????????????*/
    if(TIMx == GPTIM0)
    {
        /* ??????ITRx??? */
        switch(ITRx)
        {
            case GPTIM_TIM_TS_ITR0:
                GPTIM_SetGPTIM0ITR0Input(TIMx,ITRx_Src);
                break;
            case GPTIM_TIM_TS_ITR1:
                GPTIM_SetGPTIM0ITR1Input(TIMx,ITRx_Src);
                break;
            case GPTIM_TIM_TS_ITR2:
                GPTIM_SetGPTIM0ITR2Input(TIMx,ITRx_Src);
                break;
            case GPTIM_TIM_TS_ITR3:
                GPTIM_SetGPTIM0ITR3Input(TIMx,ITRx_Src);
                break;
            default:
                result = FAIL;
                break;
        }
    }
    else
    {
        /* ??????ITRx??? */
        switch(ITRx)
        {
            case GPTIM_TIM_TS_ITR0:
                GPTIM_SetGPTIM1ITR0Input(TIMx,ITRx_Src);
                break;
            case GPTIM_TIM_TS_ITR1:
                GPTIM_SetGPTIM1ITR1Input(TIMx,ITRx_Src);
                break;
            case GPTIM_TIM_TS_ITR2:
                GPTIM_SetGPTIM1ITR2Input(TIMx,ITRx_Src);
                break;
            case GPTIM_TIM_TS_ITR3:
                GPTIM_SetGPTIM1ITR3Input(TIMx,ITRx_Src);
                break;
            default:
                result = FAIL;
                break;
        }    
    }
   return  result;   
}
/**
  * @brief	?????? GPTIM_SlaveInitTypeDef ???????????????
  * @param 	TIM_InitStruct ??????????????????????????????????????????????????? @ref GPTIM_SlaveInitTypeDef ?????????  
  *         
  * @retval	None
  */
void GPTIM_SlaveModeStructInit(GPTIM_SlaveInitTypeDef *TIM_InitStruct)
{
    TIM_InitStruct->ITRxSrc       = 0;
    TIM_InitStruct->SlaveMode     = GPTIM_SLAVE_MODE_PROHIBITED;     
    TIM_InitStruct->TriggerSrc    = GPTIM_TIM_TS_TI1FP1;
    TIM_InitStruct->TriggerDelay  = DISABLE;

}
/**
  * @brief  ??????TIM????????????????????????ETR.
  * @param  TIMx Timer Instance
  * @param  ETPolarity   ??????
  * @param  ETPrescaler  ??????
  * @param  ETR_Filter   ??????
  * @param  TIM_IC_InitStruct ???????????? @ref GPTIM_IC_InitTypeDef ?????????
  * @retval  TIM_OC_InitStruct pointer to a @ref GPTIM_OC_InitTypeDef structure 
  *			-FAIL ????????????????????????        
  *			-PASS ??????  
  */
ErrorStatus  GPTIM_ETR_Init(GPTIM_Type *TIMx, GPTIM_ETR_InitTypeDef* TIM_InitStruct)
{
  
    assert_param(IS_GPTIM_ETP_FILTER(TIM_InitStruct->ETRFilter));
    assert_param(IS_GPTIM_ETR_PSC(TIM_InitStruct->ETRClockDivision));                                                      
    assert_param(IS_GPTIM_ETR_POLARITY(TIM_InitStruct->ETRPolarity));

    /* ???????????????????????? */
    if(TIMx == GPTIM0)
    {
        RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM0);
    }
    else
    {
        RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM1);
    }
    /* ?????????????????? */
    GPTIM_SetETRPolarity(TIMx,TIM_InitStruct->ETRPolarity);
    /* ?????????????????? */
    GPTIM_SetETRExtrenalTriggerFilter(TIMx,TIM_InitStruct->ETRFilter);
    /* ?????????????????? */
    GPTIM_SetETRExtrenalTriggerPsc(TIMx,TIM_InitStruct->ETRClockDivision);
    if(TIM_InitStruct->AsExternal == ENABLE)
    {
        GPTIM_EnableExternalClock(TIMx);
    }
    else
    {
        GPTIM_DisableExternalClock(TIMx);
    }
    /* ????????????????????????????????????????????? */
    GPTIM_GenerateEvent_UPDATE(TIMx);
    return PASS;
}

void GPTIM_ETRStructInit(GPTIM_ETR_InitTypeDef *TIM_InitStruct)
{
    TIM_InitStruct->AsExternal          = DISABLE;
    TIM_InitStruct->ETRFilter           = GPTIM_ETR_FILTER_FDIV1;       
    TIM_InitStruct->ETRPolarity         = GPTIM_ETR_POLARITY_NONINVERTED;     
    TIM_InitStruct->ETRClockDivision    = GPTIM_ETR_PRESCALER_DIV1;
}

/**
  * @brief  ??????TIM?????????????????????.
  * @param  TIMx Timer Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref GPTIM_CHANNEL_0
  *         @arg @ref GPTIM_CHANNEL_1
  *         @arg @ref GPTIM_CHANNEL_2
  *         @arg @ref GPTIM_CHANNEL_3
  * @retval TIM_OC_InitStruct pointer to a @ref GPTIM_OC_InitTypeDef structure 
  *			-FAIL ????????????????????????        
  *			-PASS ??????  
  */
ErrorStatus GPTIM_OC_Init(GPTIM_Type *TIMx, uint32_t Channel, GPTIM_OC_InitTypeDef *TIM_OC_InitStruct)
{
    ErrorStatus result = PASS;
    /* ???????????? */
    assert_param(IS_GPTIM_INSTANCE(TIMx)); 
    assert_param(IS_GPTIM_OC_MODE(TIM_OC_InitStruct->OCMode));
    assert_param(IS_GPTIM_OC_PRELOAD(TIM_OC_InitStruct->OCPreload));
    assert_param(IS_GPTIM_OC_POLARITY(TIM_OC_InitStruct->OCPolarity));
    assert_param(IS_GPTIM_OC_FASTMODE(TIM_OC_InitStruct->OCFastMode));
    assert_param(IS_GPTIM_OC_ETR_CLEARN(TIM_OC_InitStruct->OCETRFStatus));  
    /* ???????????? */
    GPTIM_DisableNormalChannelOutput(TIMx,Channel);
    /* ???????????? */
    GPTIM_SetOCPolartyChannel(TIMx,TIM_OC_InitStruct->OCPolarity,Channel);
    /* ??????????????????????????? */
    GPTIM_SetChannelMode(TIMx,GPTIM_CC_MODE_OUTPUT,Channel);
    /* ????????????????????????????????? */
    OCConfig(TIMx,Channel,TIM_OC_InitStruct);
    /* ???????????? */
    GPTIM_EnableNormalChannelOutput(TIMx,Channel);
    /* ????????????????????????????????????????????? */
    GPTIM_GenerateEvent_UPDATE(TIMx);
    return result;
}
/**
  * @brief	??????
  * @param  TIMx Timer Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref GPTIM_CHANNEL_0
  *         @arg @ref GPTIM_CHANNEL_1
  *         @arg @ref GPTIM_CHANNEL_2
  *         @arg @ref GPTIM_CHANNEL_3 
  * @param 	TIM_ICInitStruct ??????????????????????????????????????????????????? @ref GPTIM_IC_InitTypeDef ???????????????????????????.
  * @retval	None
  *			-FAIL ????????????????????????        
  *			-PASS ??????  
  */
static ErrorStatus OCConfig(GPTIM_Type *TIMx, uint32_t Channel,GPTIM_OC_InitTypeDef *TIM_OC_InitStruct)
{
    ErrorStatus result = PASS;
    
    /* ?????????????????????????????? */
    GPTIM_SetOCMode(TIMx,TIM_OC_InitStruct->OCMode,Channel);
    /* ??????TRF???????????? */
    if(TIM_OC_InitStruct->OCETRFStatus == ENABLE)
    {
        GPTIM_Enable_Clear(TIMx,Channel);
    }
    /* ?????????????????????????????? */
    if(TIM_OC_InitStruct->OCFastMode == ENABLE )
    {
        GPTIM_Enable_FastCompare(TIMx,Channel);
    }
    /* ?????????????????????????????? */
    if(TIM_OC_InitStruct->OCPreload == ENABLE)
    {
        GPTIM_Enable_Preload(TIMx,Channel);
    }
    /* ??????????????? */
    switch(Channel)
    {
        case GPTIM_CHANNEL_1:
            GPTIM_SetCompareCH1(TIMx,TIM_OC_InitStruct->CompareValue);
            break;
        case GPTIM_CHANNEL_2:
            GPTIM_SetCompareCH2(TIMx,TIM_OC_InitStruct->CompareValue);
            break;
        case GPTIM_CHANNEL_3:
            GPTIM_SetCompareCH3(TIMx,TIM_OC_InitStruct->CompareValue);
            break;
        case GPTIM_CHANNEL_4:
            GPTIM_SetCompareCH4(TIMx,TIM_OC_InitStruct->CompareValue);
            break;
        default :
            result = FAIL;
            break;

    }
    return result;
}
/**
  * @brief	?????? GPTIM_OC_InitTypeDef ???????????????
  * @param 	TIM_OC_InitStruct ??????????????????????????????????????????????????? @ref GPTIM_OC_InitTypeDef ?????????  
  *         
  * @retval	None
  */
void GPTIM_OC_StructInit(GPTIM_OC_InitTypeDef *TIM_OC_InitStruct)
{
  /* Set the default configuration */
  TIM_OC_InitStruct->OCMode       = GPTIM_OC_MODE_FROZEN;
  TIM_OC_InitStruct->OCETRFStatus = DISABLE;
  TIM_OC_InitStruct->OCFastMode   = DISABLE;
  TIM_OC_InitStruct->CompareValue = 0x00000000U;
  TIM_OC_InitStruct->OCPolarity   = GPTIM_OC_POLARITY_NOMAL;
  TIM_OC_InitStruct->OCPreload    = DISABLE;

}
/**
  * @brief  ??????TIM?????????????????????.
  * @param  TIMx Timer Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref GPTIM_CHANNEL_0
  *         @arg @ref GPTIM_CHANNEL_1
  *         @arg @ref GPTIM_CHANNEL_2
  *         @arg @ref GPTIM_CHANNEL_3
  * @param  TIM_IC_InitStruct ???????????? @ref GPTIM_IC_InitTypeDef ?????????
  * @retval  TIM_OC_InitStruct pointer to a @ref GPTIM_OC_InitTypeDef structure 
  *			-FAIL ????????????????????????        
  *			-PASS ??????  
  */
ErrorStatus GPTIM_IC_Init(GPTIM_Type *TIMx, uint32_t Channel, GPTIM_IC_InitTypeDef* InitStruct)
{
    ErrorStatus result = PASS;
    
    /* ???????????? */
    assert_param(IS_GPTIM_CHANNEL(Channel));
    assert_param(IS_GPTIM_IC_CAPTURE_STATE(InitStruct->CaptureState));
    assert_param(IS_GPTIM_IC_POLARITY(InitStruct->ICPolarity));
    assert_param(IS_GPTIM_IC_ACTIVEINPUT(InitStruct->ICActiveInput));                             
    assert_param(IS_GPTIM_IC_PRESCALER(InitStruct->ICPrescaler));
    assert_param(IS_GPTIM_IC_FILTER(InitStruct->ICFilter));
    /* ???????????????????????? */
    if(TIMx == GPTIM0)
    {
        RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM0);
    }
    else
    {
        RCC_Group4_EnableBusClock(RCC_BUS4_CLOCK_GTIM1);
    }  
    /* ???????????? */
    GPTIM_DisableNormalChannelOutput(TIMx,Channel);
    /*???????????? */
    GPTIM_SetICPolartyChannel(TIMx,InitStruct->ICPolarity,Channel);
    /* ?????????????????? */
    GPTIM_SetChannelMode(TIMx,InitStruct->ICActiveInput,Channel);
    /* ??????????????? */
    GPTIM_SetICModePrescaler(TIMx,InitStruct->ICPrescaler,Channel);
    /* ??????????????? */
    GPTIM_SetICModeFilter(TIMx,InitStruct->ICFilter,Channel);
    if(InitStruct->CaptureState == ENABLE)
    {
        GPTIM_EnableChannelCapture(TIMx,Channel);
    }
    /* ????????????????????????????????????????????? */
    GPTIM_GenerateEvent_UPDATE(TIMx);
    return result;
}

/**
  * @brief	?????? GPTIM_IC_InitTypeDef ???????????????
  * @param 	TIM_ICInitStruct ??????????????????????????????????????????????????? @ref GPTIM_IC_InitTypeDef ?????????  
  *         
  * @retval	None
  */
void GPTIM_IC_StructInit(GPTIM_IC_InitTypeDef *TIM_ICInitStruct)
{
  /* Set the default configuration */  
  TIM_ICInitStruct->ICPolarity    = GPTIM_IC_POLARITY_RISING;
  TIM_ICInitStruct->ICActiveInput = GPTIM_CC1_MODE_INPUT_TI1;
  TIM_ICInitStruct->ICPrescaler   = GPTIM_IC_PRESCALER_DIV_1;
  TIM_ICInitStruct->ICFilter      = GPTIM_IC_FILTER_FDIV1;
  TIM_ICInitStruct->CaptureState  = DISABLE;
}
/**
  * @}
  */
/**
* @}
*/
/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-21**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
