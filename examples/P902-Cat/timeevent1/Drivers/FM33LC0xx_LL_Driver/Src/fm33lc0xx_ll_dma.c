/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_dma.c
  * @author  FMSH Application Team
  * @brief   Src file of DMA LL Module
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
#include "fm33lc0xx_ll_dma.h"
#include "fm33_assert.h"
    
/** @addtogroup FM33LC0xx_Driver_DMA
  * @{
  */
/** @addtogroup Private_Macros
  * @{
  */
#define         IS_DMA_INSTANCE(INTANCE)                         ((INTANCE) == DMA)

#define         IS_DMA_PRIORITY(__VALUE__)                       (((__VALUE__) == DMA_CHANNEL_PRIORITY_LOW)||\
                                                                     ((__VALUE__) == DMA_CHANNEL_PRIORITY_MEDIUM)||\
                                                                     ((__VALUE__) == DMA_CHANNEL_PRIORITY_HIGH)||\
                                                                     ((__VALUE__) == DMA_CHANNEL_PRIORITY_VERYHIGH))

#define         IS_DMA_CIRC_MODE(__VALUE__)                      (((__VALUE__) == DISABLE)||\
                                                                     ((__VALUE__) == ENABLE))


#define         IS_DMA_DIRECION(__VALUE__)                       (((__VALUE__) == DMA_DIR_PERIPHERAL_TO_RAM)||\
                                                                     ((__VALUE__) == DMA_DIR_FLASH_TO_RAM)||\
                                                                     ((__VALUE__) == DMA_DIR_RAM_TO_FLASH)||\
                                                                    ((__VALUE__) == DMA_DIR_RAM_TO_PERIPHERAL))

                                                                    
#define         IS_DMA_DATA_SIZE(__VALUE__)                      (((__VALUE__) == DMA_BAND_WIDTH_BYTE)||\
                                                                    ((__VALUE__) == DMA_BAND_WIDTH_WORD)||\
                                                                    ((__VALUE__) == DMA_BAND_WIDTH_HALF_WORD))

#define         IS_DMA_INCMODE(__VALUE__)                       (((__VALUE__) == DMA_INCREMENTAL_INCREASE)||\
                                                                    ((__VALUE__) == DMA_INCREMENTAL_REDUCE) ||\
                                                                    ((__VALUE__) == DMA_CH7_RAM_ADDR_INCREASE)||\
                                                                    ((__VALUE__) == DMA_CH7_RAM_ADDR_REDUCE))
                                                                    
#define         IS_DMA_PERIPH(__VALUE__)                        (((__VALUE__) == DMA_PERIPHERAL_FUNCTION1)||\
                                                                    ((__VALUE__) == DMA_PERIPHERAL_FUNCTION2)||\
                                                                    ((__VALUE__) == DMA_PERIPHERAL_FUNCTION3)||\
                                                                    ((__VALUE__) == DMA_PERIPHERAL_FUNCTION4)||\
                                                                    ((__VALUE__) == DMA_PERIPHERAL_FUNCTION5)||\
                                                                    ((__VALUE__) == DMA_PERIPHERAL_FUNCTION6)||\
                                                                    ((__VALUE__) == DMA_PERIPHERAL_FUNCTION7)||\
                                                                    ((__VALUE__) == DMA_PERIPHERAL_FUNCTION8))
/**
  * @}
  */ 
  
/** @addtogroup DMA_EF_Init
  * @{
  */
  
/**
  * @brief  ????????????DMA?????????.
  * @param  DMAx 
  * @retval ErrorStatus?????????:
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus DMA_DeInit(DMA_Type *DMAx)
{
    
    assert_param(IS_DMA_INSTANCE(DMAx));
    
    /* ?????????????????? */
    RCC_EnablePeripheralReset();
    /* ????????????????????? */
    RCC_EnableResetAHBPeripheral(RCC_AHB_PERIPHERAL_RST_DMA);
    RCC_DisableResetAHBPeripheral(RCC_AHB_PERIPHERAL_RST_DMA);
    /* ?????????????????????????????????????????? */
    RCC_Group2_DisableBusClock(RCC_BUS2_CLOCK_DMA);
    /* ?????????????????? */
    RCC_DisablePeripheralReset();
    return PASS;
}
/**
  * @brief  ?????? DMA_InitStruct ???????????????????????????????????????????????????????????????.
  * @param  DMAx DMAx 
  * @param  DMA_InitStruct ???????????? @ref DMA_InitTypeDef ?????????
  *         ??????????????????????????????????????????.
  * @param  Channel This parameter can be one of the following values:
  *           @arg @ref DMA_CHANNEL_0
  *           @arg @ref DMA_CHANNEL_1
  *           @arg @ref DMA_CHANNEL_2
  *           @arg @ref DMA_CHANNEL_3
  *           @arg @ref DMA_CHANNEL_4
  *           @arg @ref DMA_CHANNEL_5
  *           @arg @ref DMA_CHANNEL_6
  *           @arg @ref DMA_CHANNEL_7
  * @retval ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS ????????????  
  */
ErrorStatus DMA_Init(DMA_Type *DMAx,DMA_InitTypeDef *DMA_InitStruct,uint32_t Channel)
{
    /* ???????????? */
    assert_param(IS_DMA_INSTANCE(DMAx));
    assert_param(IS_DMA_PRIORITY(DMA_InitStruct->Priority));
    assert_param(IS_DMA_CIRC_MODE (DMA_InitStruct->CircMode));
    assert_param(IS_DMA_DIRECION(DMA_InitStruct->Direction));  
    assert_param(IS_DMA_DATA_SIZE(DMA_InitStruct->DataSize)); 
    assert_param(IS_DMA_INCMODE (DMA_InitStruct->MemoryAddressIncMode));
    /* ???????????? */
    RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_DMA);
    /* ????????????????????? */
    DMA_SetChannelPriority(DMAx,DMA_InitStruct->Priority,Channel);
    /* RAM???????????? */
    DMA_SetChannelAddrIncremental(DMAx,DMA_InitStruct->MemoryAddressIncMode,Channel);
    /* ???????????? */
    DMA_SetChannelTransDirection(DMAx,DMA_InitStruct->Direction,Channel);
    /* ???????????? */
    DMA_SetChannelBandWidth(DMAx,DMA_InitStruct->DataSize,Channel);
    /* ???????????? */
    if(DMA_InitStruct->CircMode == ENABLE)
    {
        DMA_Enable_CircularMode(DMAx,Channel);
        if(Channel == DMA_CHANNEL_7)
        {
            return FAIL;
        }
    }
    else
    {
        DMA_Disable_CircularMode(DMAx,Channel);
    }
    
    /* ?????????????????? */
    DMA_SetTransmissionSize(DMAx,DMA_InitStruct->NbData,Channel);
    
    /* ???????????????7 ????????????????????????FLASH?????????????????????????????????7?????????????????? */
    if(Channel != DMA_CHANNEL_7)
    {
        assert_param(IS_DMA_PERIPH(DMA_InitStruct->PeriphAddress));
        DMA_SetChannelPeripheralMap(DMAx,DMA_InitStruct->PeriphAddress,Channel);
    }
    else
    {
        DMA_SetChannelFlashAddr(DMAx,DMA_InitStruct->PeriphAddress,Channel);
        DMA_SetFlash_AddrIncremental (DMAx,DMA_InitStruct->FlashAddressIncMode);
    }   
    DMA_SetChannelMemAddr(DMAx,DMA_InitStruct->MemoryAddress,Channel);
    
    return PASS;
}  
/**
  * @brief	?????? CRC_InitStruct ???????????????
  * @param 	CRC_InitStruct ??????????????????????????????????????????????????? @ref CRC_InitTypeDef ?????????  
  *         
  * @retval	None
  */

void DMA_StructInit(DMA_InitTypeDef *InitStruct)
{
    
    InitStruct->CircMode                = DISABLE;
    InitStruct->DataSize                = DMA_BAND_WIDTH_BYTE;
    InitStruct->Direction               = DMA_DIR_PERIPHERAL_TO_RAM;
    InitStruct->PeriphAddress           = DMA_PERIPHERAL_FUNCTION1;
    InitStruct->NbData                  = 0;
    InitStruct->Priority                = DMA_CHANNEL_PRIORITY_LOW;
    InitStruct->MemoryAddress           = 0x0U;
    InitStruct->MemoryAddressIncMode    = DMA_INCREMENTAL_REDUCE;
    
}

/**
  * @brief  ????????????DMA??????.
  * @param  DMAx DMAx 
  * @param  DMA_InitStruct ???????????? @ref DMA_InitTypeDef ?????????
  *         ??????????????????????????????????????????.
  * @param  Channel This parameter can be one of the following values:
  *           @arg @ref DMA_CHANNEL_0
  *           @arg @ref DMA_CHANNEL_1
  *           @arg @ref DMA_CHANNEL_2
  *           @arg @ref DMA_CHANNEL_3
  *           @arg @ref DMA_CHANNEL_4
  *           @arg @ref DMA_CHANNEL_5
  *           @arg @ref DMA_CHANNEL_6
  *           @arg @ref DMA_CHANNEL_7
  * @retval ErrorStatus?????????	
  *			-FAIL ?????????????????????????????????????????????????????????        
  *			-PASS ??????  
  */
ErrorStatus StartOnceDMATransmion(DMA_Type *DMAx,DMA_InitTypeDef *DMA_InitStruct ,uint32_t Channel)
{
    uint32_t   TimeOut = 0xFFFF;
   
    /* ?????????????????????????????????????????????DMA */
    DMA_Init(DMAx,DMA_InitStruct,Channel);
    /* ??????DMA??????????????? */
    DMA_Enable_DMA(DMAx);
    /* ??????DMA?????????????????? */
    DMA_Enable_Channel(DMAx,Channel);
    while(1)
    {
        TimeOut--;
        if(TimeOut == 0 || DMA_IsActiveFlag_Finished(DMAx,Channel)||\
             DMA_IsActiveFlag_ADDRERR(DMAx))
        {
            /* ????????????????????? */
            break;
        }    
    }
    /* ??????DMA??????????????? */
    DMA_Disable_DMA(DMAx);
    /* ??????DMA?????????????????? */
    DMA_Disable_Channel(DMAx,Channel);
    if(!TimeOut)
    {
        /* ???????????? */
        return FAIL;
    }
    if(DMA_IsActiveFlag_ADDRERR(DMAx))
    {
        /* ?????????????????? */
        return FAIL;
    }
    DMA_ClearFlag_Finished(DMAx,Channel);
    return PASS;
}  
/**
  * @}
  */
/**
  * @}
  */

/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-25**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
