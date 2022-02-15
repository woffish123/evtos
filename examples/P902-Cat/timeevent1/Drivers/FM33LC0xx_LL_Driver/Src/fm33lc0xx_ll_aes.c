/**
  ****************************************************************************************************
  * @file    fm33lc0xx_ll_aes.c
  * @author  FMSH Application Team
  * @brief   Src file of AES LL Module
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
#include "fm33lc0xx_ll_aes.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_Driver_AES
  * @{
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup HDIV_Private_Macros
  * @{
  */ 
#define         IS_AES_INSTANCE(INSTANCE)                (((INSTANCE) == AES))
                                                             
#define         IS_AES_KEYLENTH(__VALUE__)               (((__VALUE__) == AES_KEY_LENTH_IS_128BITS)||\
                                                             ((__VALUE__) == AES_KEY_LENTH_IS_192BITS)||\
                                                             ((__VALUE__) == AES_KEY_LENTH_IS_256BITS))
                                                             
#define         IS_AES_CIPHERMODE(__VALUE__)             (((__VALUE__) == AES_CIPHER_ECB_MODE)||\
                                                             ((__VALUE__) == AES_CIPHER_CBC_MODE)||\
                                                             ((__VALUE__) == AES_CIPHER_CTR_MODE)||\
                                                             ((__VALUE__) == AES_CIPHER_MULT_MODE))
                                                      
#define         IS_AES_OPERATIONMODE(__VALUE__)          (((__VALUE__) == AES_OPERATION_MODE_ENCRYPTION)||\
                                                             ((__VALUE__) == AES_OPERATION_MODE_KEY_EXTENSION)||\
                                                             ((__VALUE__) == AES_OPERATION_MODE_DECRYPT)||\
                                                             ((__VALUE__) == AES_OPERATION_MODE_KEY_EXTENSION_AND_DECRYPT))
      
#define         IS_AES_DATATYPE(__VALUE__)               (((__VALUE__) == AES_DATA_SWAP_RULE_NOT_EXCHANGED)||\
                                                             ((__VALUE__) == AES_DATA_SWAP_RULE_HAIFWORD_EXCHANGE)||\
                                                             ((__VALUE__) == AES_DATA_SWAP_RULE_BYTE_EXCHANGED)||\
                                                             ((__VALUE__) == AES_DATA_SWAP_RULE_BIT_EXCHANGED))
/** 
  *@} 
  */

/** @addtogroup AES_EF_Init
  * @{
  */
  
/**
  * @brief	复位AES 外设寄存器值为复位值
  * @param	外设入口地址
  * @retval	返回错误状态，可能值：
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus AES_DeInit(void)
{
    /* 外设复位使能 */
	 RCC_EnablePeripheralReset();
    
    /* 复位AES */
    RCC_EnableResetAPB2Peripheral( RCC_APB2_PERIPHERAL_RST_AES);
    RCC_DisableResetAPB2Peripheral( RCC_APB2_PERIPHERAL_RST_AES);
    /* 关闭总线时钟 */
    RCC_Group2_DisableBusClock( RCC_BUS2_CLOCK_AES);
    /* 锁定外设复位功能 */
    RCC_DisablePeripheralReset();
    return PASS;
}
/**
  * @brief	根据 AES_InitStructer初始化对应外设入口地址的寄存器值.
  * @param	外设入口地址
  * @retval	返回错误状态，可能值：
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus AES_Init(AES_Type* AESx, AES_InitTypeDef* AES_InitStructer)
{
    /* 入口合法性检查 */
    assert_param(IS_AES_INSTANCE(AESx));
    assert_param(IS_AES_KEYLENTH(AES_InitStructer->KeyLenth));         
    assert_param(IS_AES_CIPHERMODE(AES_InitStructer->CipherMode));
    assert_param(IS_AES_OPERATIONMODE(AES_InitStructer->OperationMode));         
    assert_param(IS_AES_DATATYPE(AES_InitStructer->DataType));
    if(AES_IsEnabled(AESx) == 0)
    {
        
        /* 开启总线时钟 */
        RCC_Group2_EnableBusClock(RCC_BUS2_CLOCK_AES);
        /* key长度 */
        AES_SetBitLenthofKeys(AESx,AES_InitStructer->KeyLenth);
        /* 数据流处理模式 */
        AES_SetCipherMode(AESx,AES_InitStructer->CipherMode);
        /* 操作模式 */
        AES_SetOpertionMode(AESx,AES_InitStructer->OperationMode);
        /* 数据类型 */
        AES_SetDataType(AESx,AES_InitStructer->DataType);
    }
    else
    {
        return FAIL;
    }
    
    
    return PASS;
}
/**
  * @brief	设置 AES_InitStruct 为默认配置
  * @param 	AES_InitStruct 指向需要将值设置为默认配置的结构体 @ref AES_InitTypeDef 结构体  
  *         
  * @retval	None
  */
void AES_StructInit(AES_InitTypeDef* AES_InitStructer)
{
    AES_InitStructer->KeyLenth      = AES_KEY_LENTH_IS_128BITS;

    AES_InitStructer->CipherMode    = AES_CIPHER_ECB_MODE;

    AES_InitStructer->OperationMode = AES_OPERATION_MODE_ENCRYPTION;

    AES_InitStructer->DataType      = AES_DATA_SWAP_RULE_NOT_EXCHANGED; 

}
/** 
  *@} 
  */
/** 
  *@} 
  */
/*********************** (C) COPYRIGHT Fudan Microelectronics *****END OF FILE************************/
