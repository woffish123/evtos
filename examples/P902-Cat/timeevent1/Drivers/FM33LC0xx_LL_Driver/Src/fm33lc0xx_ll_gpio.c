/**
  ****************************************************************************************************
  * @file    fm33lC0xx_ll_gpio.c
  * @author  FMSH Application Team
  * @brief   Src file of GPIO LL Module
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
#include "fm33lc0xx_ll_gpio.h"
#include "fm33lc0xx_ll_rcc.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0XX_Driver_GPIO
  * @{
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup GPIO_Private_Macros
  * @{
  */
  
#define         IS_GPIO_ALL_INSTANCE(INTENCE)              (((INTENCE) == GPIOA)||\
                                                            ((INTENCE) == GPIOB)||\
                                                            ((INTENCE) == GPIOC)||\
                                                            ((INTENCE) == GPIOD))
                                                            
#define         IS_GPIO_PIN(__VALUE__)                  ((((uint32_t)0x00000000U) < (__VALUE__)) &&\
                                                            ((__VALUE__) <=  GPIO_PIN_ALL)))                                                         
                                                             
#define         IS_GPIO_MODE(__VALUE__)                  (((__VALUE__) == GPIO_MODE_ANALOG)||\
                                                             ((__VALUE__) == GPIO_MODE_INPUT)||\
                                                             ((__VALUE__) == GPIO_MODE_OUTPUT)||\
                                                             ((__VALUE__) == GPIO_MODE_DIGITAL))
                                                             
#define         IS_GPIO_OPENDRAIN(__VALUE__)             (((__VALUE__) == GPIO_OUTPUT_OPENDRAIN)||\
                                                             ((__VALUE__) == GPIO_OUTPUT_PUSHPULL))
                                                      
#define         IS_GPIO_PULL_UP(__VALUE__)               (((__VALUE__) == DISABLE)||\
                                                             ((__VALUE__) == ENABLE))
/**
  * @}
  */

/** @addtogroup GPIO_EF_Init
  * @{
  */ 
  
/**
  * @brief  ????????????GPIO???????????????.
  * @param  GPIOx GPIO Port
  * @retval ErrorStatus?????????:
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus GPIO_DeInit(GPIO_Type *GPIOx)
{
    /* ?????????????????? */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    /* ?????????????????????????????? */
    GPIOx->INEN = 0x00000000U;
    GPIOx->PUEN = 0x00000000U;
    GPIOx->ODEN = 0x00000000U;
    GPIOx->FCR  = 0x00000000U;
    GPIOx->DRST = 0x00000000U;
    GPIOx->DSET = 0x00000000U;
    GPIOx->DFS  = 0x00000000U;
    GPIOx->ANEN = 0x00000000U;
    return PASS;
}

/**
  * @brief  ?????? GPIO_InitStruct????????????????????????????????????.
  * @param  GPIOx GPIO Port
  * @param  GPIO_InitStruct ???????????? @ref GPIO_InitTypeDef ?????????
  *         ??????????????????????????????????????????.
  * @retval ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS LPUART????????????  
  */
ErrorStatus GPIO_Init(GPIO_Type *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
    uint32_t pinpos     = 0x00000000U;
    uint32_t currentpin = 0x00000000U;

    /* ?????????????????? */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->Pin));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->Mode));
    assert_param(IS_GPIO_OPENDRAIN(GPIO_InitStruct->OutputType));
    assert_param(IS_GPIO_PULL_UP(GPIO_InitStruct->Pull));
    /* ?????????????????? */
    RCC_Group1_EnableBusClock( RCC_BUS1_CLOCK_GPIO);
    /* ???????????????PIN??????????????????????????????????????? */
    while (((GPIO_InitStruct->Pin) >> pinpos) != 0x00000000U)
    {
        /* ????????????????????????Pin??? */
        currentpin = (GPIO_InitStruct->Pin) & (0x00000001U << pinpos);

        if (currentpin)
        {
					  /* Pin????????????????????? */
            if(GPIO_InitStruct->OutputType == GPIO_OUTPUT_PUSHPULL)
            {
                GPIO_DisablePinOpendrain(GPIOx,currentpin);
            }
            else
            {
                GPIO_EnablePinOpendrain(GPIOx,currentpin);
            }
						
            /* Pin????????????????????? */
            if(GPIO_InitStruct->Pull)
            {
                GPIO_EnablePinPullup(GPIOx, currentpin);
            }
            else
            {
                GPIO_DisablePinPullup(GPIOx, currentpin);
            }

            /* Pin????????????????????? */
            if (GPIO_InitStruct->Mode == GPIO_MODE_ANALOG )
            {
                GPIO_EnablePinAnalog(GPIOx,currentpin);
            }
            /*????????????*/
            if (GPIO_InitStruct->Mode == GPIO_MODE_DIGITAL)
            {
                /*?????????*/
                if(GPIO_InitStruct->RemapPin == ENABLE)
                {
                    GPIO_EnablePinRemap(GPIOx,currentpin);
                }
            }
            /*??????*/
            if(GPIO_InitStruct->Mode == GPIO_MODE_INPUT)
            {
                 GPIO_EnablePinInput(GPIOx,currentpin);   
            }
			
			/* Pin????????????????????? */
            GPIO_SetPinMode(GPIOx, currentpin, GPIO_InitStruct->Mode);
        }
        pinpos++;
    }
    return PASS;
}
/**
  * @brief	?????? GPIO_InitStruct ???????????????
  * @param 	GPIO_InitStruct ??????????????????????????????????????????????????? @ref GPIO_InitTypeDef ?????????  
  *         
  * @retval	None
  */

void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct)
{
  /* ?????????????????? */
  GPIO_InitStruct->Pin        = GPIO_PIN_ALL;
  GPIO_InitStruct->Mode       = GPIO_MODE_INPUT;
  GPIO_InitStruct->OutputType = GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct->Pull       = DISABLE;
  GPIO_InitStruct->RemapPin   = DISABLE;
}
/**
  * @brief  ????????????GPIO???????????????????????????????????????????????????????????????????????????
  *         ???????????????????????????????????????
  * @note   PD8???PD9???????????????
  *
  * @param  None 
  *         
  * @retval	None
  */
void GPIO_ALLPIN_LPM_MODE(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_StructInit(&GPIO_InitStruct);

  GPIO_InitStruct.OutputType = GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;

  GPIO_SetOutputPin(GPIOA, GPIO_PIN_ALL);
  GPIO_SetOutputPin(GPIOB, GPIO_PIN_ALL);
  GPIO_SetOutputPin(GPIOC, GPIO_PIN_ALL);
  GPIO_SetOutputPin(GPIOD, GPIO_PIN_ALL_EXCEPTSWD);
	
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* SWD ON */
  GPIO_InitStruct.Pin  = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_DIGITAL;
  GPIO_InitStruct.Pull = ENABLE;
  GPIO_Init(GPIOD, &GPIO_InitStruct);
}
/**
  * @}
  */

/**
  * @}
  */
/*************************************************************END OF FILE************************************************************/
