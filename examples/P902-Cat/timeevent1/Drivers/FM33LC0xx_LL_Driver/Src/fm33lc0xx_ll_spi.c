/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_spi.c
  * @author  FMSH Application Team
  * @brief   Src file of SPI LL Module
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
#include "fm33lc0xx_ll_spi.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_Driver_SPI
  * @{
  */
  
/** @addtogroup Private_Macros
  * @{
  */
#define         IS_SPI_INSTANCE(INTANCE)                     (((INTANCE) == SPI1)||\
                                                                 ((INTANCE) == SPI2))

#define         IS_SPI_MODE(__VALUE__)                       (((__VALUE__) == SPI_WORK_MODE_SLAVE)||\
                                                                 ((__VALUE__) == SPI_WORK_MODE_MASTER))
                                                                 
#define         IS_SPI_NSS_MODE(__VALUE__)                   (((__VALUE__) == SPI_SSN_MODE_HARDWARE)||\
                                                                 ((__VALUE__) == SPI_SSN_MODE_SOFTWARE))

#define         IS_SPI_BIT_ORDER(__VALUE__)                  (((__VALUE__) == SPI_BIT_ORDER_MSB_FIRST)||\
                                                                 ((__VALUE__) == SPI_BIT_ORDER_LSB_FIRST))
                                                                 
#define         IS_SPI_DATAWIDT(__VALUE__)                   (((__VALUE__) == SPI_DATAWIDTH_8BIT)||\
                                                                 ((__VALUE__) == SPI_DATAWIDTH_16BIT)||\
                                                                 ((__VALUE__) == SPI_DATAWIDTH_24BIT)||\
                                                                 ((__VALUE__) == SPI_DATAWIDTH_32BIT))

#define         IS_SPI_CLOCK_PHASE(__VALUE__)                (((__VALUE__) == SPI_SPI_PHASE_1EDGE)||\
                                                                 ((__VALUE__) == SPI_SPI_PHASE_2EDGE))
                                                                 
#define         IS_SPI_CLOCK_POLARITY(__VALUE__)             (((__VALUE__) == SPI_SPI_POLARITY_LOW)||\
                                                                 ((__VALUE__) == SPI_SPI_POLARITY_HIGH))

#define         IS_SPI_BAUDRATE(__VALUE__)                   (((__VALUE__) == SPI_BAUDRATEPRESCALER_DIV2)||\
                                                                 ((__VALUE__) == SPI_BAUDRATEPRESCALER_DIV4)||\
                                                                 ((__VALUE__) == SPI_BAUDRATEPRESCALER_DIV8)||\
                                                                 ((__VALUE__) == SPI_BAUDRATEPRESCALER_DIV16)||\
                                                                 ((__VALUE__) == SPI_BAUDRATEPRESCALER_DIV32)||\
                                                                 ((__VALUE__) == SPI_BAUDRATEPRESCALER_DIV64)||\
                                                                 ((__VALUE__) == SPI_BAUDRATEPRESCALER_DIV128)||\
                                                                 ((__VALUE__) == SPI_BAUDRATEPRESCALER_DIV256))


#define         IS_SPI_TANSFERMODE(__VALUE__)                (((__VALUE__) == SPI_MODE_FULL_DUPLEX)||\
                                                                 ((__VALUE__) == SPI_MODE_SIMPLEX))
                                                                 
                                                                 
#define         IS_SPI_TANSFER_DIRECT(__VALUE__)             (((__VALUE__) == SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_TX)||\
                                                                 ((__VALUE__) == SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_RX))
                                                                 
/**
  * @}
  */
/** @addtogroup SPI_EF_Init
  * @{
  */ 
/**
  * @brief  复位对应SPI寄存器.
  * @param  SPIx 
  * @retval ErrorStatus枚举值:
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus SPI_DeInit(SPI_Type *SPIx)
{
    
    assert_param(IS_SPI_INSTANCE(SPIx));
    
    /* 使能外设复位 */
    RCC_EnablePeripheralReset();
    if(SPIx == SPI1)
    {
        /* 复位IIC外设寄存器 */
        RCC_EnableResetAPB2Peripheral(RCC_APB2_PERIPHERAL_RST_SPI1);
        RCC_DisableResetAPB2Peripheral(RCC_APB2_PERIPHERAL_RST_SPI1);
        /* 关闭外设总线始时钟和工作时钟 */
        RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_SPI1);
        //RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_I2C);
    }
    else
    {
        /* 复位IIC外设寄存器 */
        RCC_EnableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_SPI2);
        RCC_DisableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_SPI2);
        /* 关闭外设总线始时钟和工作时钟 */
        RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_SPI2);
        //RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_I2C);
    }
    
    /* 锁定外设复位 */
    RCC_DisablePeripheralReset();
    return PASS;
}
/**
  * @brief  根据 SPI_InitStruct 的配置信息初始化对应外设入口地址的寄存器值.
  * @param  SPIx SPIx 
  * @param  SPI_InitStruct 指向一个 @ref SPI_InitTypeDef 结构体
  *         其中包含了外设的相关配置信息.
  * @retval ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS LPUART配置成功  
  */
ErrorStatus SPI_Init(SPI_Type *SPIx,SPI_InitTypeDef *SPI_InitStruct)
{
    
    assert_param(IS_SPI_INSTANCE(SPIx));                                          
    assert_param(IS_SPI_MODE(SPI_InitStruct->Mode));                                                                            
    assert_param(IS_SPI_NSS_MODE(SPI_InitStruct->SSN));                                         
    assert_param(IS_SPI_BIT_ORDER(SPI_InitStruct->BitOrder));                                                                       
    assert_param(IS_SPI_DATAWIDT(SPI_InitStruct->DataWidth));
    assert_param(IS_SPI_BAUDRATE(SPI_InitStruct->BaudRate));    
    assert_param(IS_SPI_CLOCK_PHASE(SPI_InitStruct->ClockPhase));                                                                      
    assert_param(IS_SPI_CLOCK_POLARITY(SPI_InitStruct->ClockPolarity));
    
    if(SPIx == SPI1)
    {
        /* 外设总线始时钟 */
        RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_SPI1);
    }
    else
    {
        /* 外设总线始时钟 */
        RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_SPI2);
    }
    /* 选择NSS脚控制模式 */
    SPI_SetSNNMode(SPIx,SPI_InitStruct->SSN);
    /* 外设工作主从模式 */
    SPI_SetSPIWorkMode(SPIx,SPI_InitStruct->Mode);
    /* 总线通讯速率 */
    SPI_SetBaudrate(SPIx,SPI_InitStruct->BaudRate);
    /* 数据bit方向 */
    SPI_SetBitOrder(SPIx,SPI_InitStruct->BitOrder);
    /* 总线数据位宽 */
    SPI_SetDataWidth(SPIx,SPI_InitStruct->DataWidth);
    /* 时钟相位 */
    SPI_SetClockPhase(SPIx,SPI_InitStruct->ClockPhase);
    /* 传输模式 双工半双工  */
    SPI_SetTransferMode(SPIx,SPI_InitStruct->TransferMode);
    /* 时钟极性 */
    SPI_SetClockPolarity(SPIx,SPI_InitStruct->ClockPolarity);
    /* 传输方向 仅接收 或仅发送 在半双工模式时使用线始时钟 */
    SPI_SetTransferDirection(SPIx,SPI_InitStruct->TransferDirection);
    /* 使能 外设*/
    SPI_Enable(SPIx);
    return PASS;
}
/**
  * @brief	设置 SPI_InitStruct 为默认配置
  * @param 	SPI_InitStruct 指向需要将值设置为默认配置的结构体 @ref SPI_InitTypeDef 结构体  
  *         
  * @retval	None
  */

void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct)
{
    SPI_InitStruct->SSN                 =   SPI_SSN_MODE_SOFTWARE;
    SPI_InitStruct->Mode                =   SPI_WORK_MODE_MASTER;
    SPI_InitStruct->BaudRate            =   SPI_BAUDRATEPRESCALER_DIV8;
    SPI_InitStruct->BitOrder            =   SPI_BIT_ORDER_MSB_FIRST;
    SPI_InitStruct->DataWidth           =   SPI_DATAWIDTH_8BIT;
    SPI_InitStruct->ClockPhase          =   SPI_SPI_PHASE_1EDGE;
    SPI_InitStruct->TransferMode        =   SPI_MODE_FULL_DUPLEX;
    SPI_InitStruct->ClockPolarity       =   SPI_SPI_POLARITY_LOW;
    SPI_InitStruct->TransferDirection   =   SPI_SIMPLEX_DIRECTION_HALF_DUPLEX_TX;
    
}
/**
  * @}
  */
/**
  * @}
  */
/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-17**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
