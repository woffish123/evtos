/**
  ****************************************************************************************************
  * @file    fm33lc0xx_ll_uart.c
  * @author  FMSH Application Team
  * @brief   Src file of UART LL Module
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
#include "fm33lc0xx_ll_uart.h"
#include "fm33lc0xx_ll_rcc.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_Driver_UART
  * @{
  */
  
/* Private macros ------------------------------------------------------------*/
/** @addtogroup UART_Private_Macros
  * @{
  */

                                                          
#define         IS_UART_INSTANCE(INSTANCE)              (((INSTANCE) == UART0)||\
                                                         ((INSTANCE) == UART1)||\
                                                         ((INSTANCE) == UART4)||\
                                                         ((INSTANCE) == UART5))
 
#define         IS_UART_CLKSRC(__VALUE__)            (((__VALUE__) == RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1)||\
                                                         ((__VALUE__) == RCC_UART_OPERATION_CLOCK_SOURCE_RCHF)||\
                                                         ((__VALUE__) == RCC_UART_OPERATION_CLOCK_SOURCE_SYSCLK)||\
                                                         ((__VALUE__) == RCC_UART_OPERATION_CLOCK_SOURCE_RC4M_PSC))                                                             
                                                             
#define         IS_UART_BAUDRATE(__VALUE__)            (((__VALUE__) <= 460800))
                                                             
#define         IS_UART_DATAWIDTH(__VALUE__)           (((__VALUE__) == UART_DATAWIDTH_6B)||\
                                                           ((__VALUE__) == UART_DATAWIDTH_7B)||\
                                                           ((__VALUE__) == UART_DATAWIDTH_8B)||\
                                                           ((__VALUE__) == UART_DATAWIDTH_9B))
                                                      
#define         IS_UART_STOPBITS(__VALUE__)            (((__VALUE__) == UART_STOPBITS_1)||\
                                                           ((__VALUE__) == UART_STOPBITS_2))
                                                             
#define         IS_UART_PARITY(__VALUE__)              (((__VALUE__) == UART_PARITY_NONE)||\
                                                           ((__VALUE__) == UART_PARITY_EVEN)||\
                                                           ((__VALUE__) == UART_PARITY_ODD))
                                                             
#define         IS_UART_DIRECTION(__VALUE__)           (((__VALUE__) == UART_DIRECTION_NONE)||\
                                                           ((__VALUE__) == UART_DIRECTION_RX)||\
                                                           ((__VALUE__) == UART_DIRECTION_TX)||\
                                                           ((__VALUE__) == UART_DIRECTION_TX_RX)) 
                                                             
#define         IS_UART_INFRA_MODULATION(__VALUE__)    (((__VALUE__) == DISABLE)||\
                                                           ((__VALUE__) == ENABLE))


#define         IS_UART_INFRARED_POLARITY(__VALUE__)        (((__VALUE__) == UART_INFRARED_POLARITY_POSITIVE)||\
                                                                ((__VALUE__) == UART_INFRARED_POLARITY_NEGATIVE))
                                                             
#define         IS_UART_INFRARED_MODULATION_DUTY(__VALUE__) (((__VALUE__) <= 16))
                                                                
                                                             
#define         IS_UART_INFRARED_MODULATION_FREQ(__VALUE__) (((__VALUE__) > 0))                                                           
/**
  * @}
  */
  
/** @addtogroup UART_EF_Init
  * @{
  */

/**
  * @brief	复位UART 外设寄存器值为复位值
  * @param	外设入口地址
  * @retval	返回错误状态，可能值：
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus UART_DeInit(UART_Type *UARTx)
{
    ErrorStatus status = PASS;

    /* 入口参数合法性断言 */
    assert_param(IS_UART_INSTANCE(UARTx));
	/* 外设复位使能 */
	RCC_EnablePeripheralReset();
    if (UARTx == UART0)
    {
		/*复位UART*/
        RCC_EnableResetAPB1Peripheral(RCC_APB2_PERIPHERAL_RST_UART0);
        RCC_DisableResetAPB1Peripheral(RCC_APB2_PERIPHERAL_RST_UART0);
        /* 外设总线时钟关闭 */
        RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART0);
        /* 外设操作时钟关闭 */
        RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_UART0);  
    }
    else if (UARTx == UART1)
    {
		/*复位UART*/
        RCC_EnableResetAPB2Peripheral(RCC_APB2_PERIPHERAL_RST_UART1);
        RCC_DisableResetAPB2Peripheral(RCC_APB2_PERIPHERAL_RST_UART1);
		/* 外设总线时钟关闭 */
        RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART1);
		/* 外设操作时钟关闭 */
        RCC_Group1_DisableOperationClock(RCC_OPERATION1_CLOCK_UART1);
    }
    else if (UARTx == UART4)
    {
		/*复位UART*/
        RCC_EnableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_UART4);
        RCC_DisableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_UART4);
		/* 外设总线时钟关闭 */
        RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART4);
    }
    else if (UARTx == UART5)
    {
		/*复位UART*/
        RCC_EnableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_UART5);
        RCC_DisableResetAPB2Peripheral(RCC_APB1_PERIPHERAL_RST_UART5);
		/* 外设总线时钟关闭 */
        RCC_Group3_DisableBusClock(RCC_BUS3_CLOCK_UART5);
    }
    else
    {
        status = FAIL;
    }
    /* 锁定外设复位功能 */
    RCC_DisablePeripheralReset();
    return (status);
}

/**
  * @brief	根据需要功能配置UART寄存器 
  *               
  * @param	UARTx  外设入口地址
  * @param 	UART_InitStruct指向一个UART_InitTypeDef类型的结构体，它包含指定UART外设的配置信息	  
  *         
  * @retval	ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS UART配置成功        
  */
ErrorStatus UART_Init(UART_Type *UARTx, UART_InitTypeDef *UART_InitStruct)
{
    ErrorStatus status = FAIL;
    uint32_t Fclk,BaudRate;

	/* 参数合法性检查 */
    assert_param(IS_UART_INSTANCE(UARTx));
    assert_param(IS_UART_CLKSRC(UART_InitStruct->ClockSrc));
    assert_param(IS_UART_DATAWIDTH(UART_InitStruct->DataWidth));
    assert_param(IS_UART_BAUDRATE(UART_InitStruct->BaudRate));
    assert_param(IS_UART_PARITY(UART_InitStruct->Parity));
    assert_param(IS_UART_STOPBITS(UART_InitStruct->StopBits));
    assert_param(IS_UART_DIRECTION(UART_InitStruct->TransferDirection));
    assert_param(IS_UART_INFRA_MODULATION(UART_InitStruct->InfraredModulation));

    if(UARTx == UART0 || UARTx == UART1)
    {
        /*时钟源选择*/
        RCC_SetUARTClockSource(UARTx,UART_InitStruct->ClockSrc);
        /* 根据不同的时钟源计算baudrate 寄存器值，并配置 */
        switch(UART_InitStruct->ClockSrc)
        {
            case RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1:
                Fclk = RCC_GetAPB1ClockFreq(RCC_GetAHBClockFreq(RCC_GetSystemClockFreq()));
                break;    
            case RCC_UART_OPERATION_CLOCK_SOURCE_RCHF:
                Fclk = RCC_GetRCHFClockFreq();
                break;
            case RCC_UART_OPERATION_CLOCK_SOURCE_SYSCLK:
                Fclk = RCC_GetSystemClockFreq();
                break;
            case RCC_UART_OPERATION_CLOCK_SOURCE_RC4M_PSC:
                Fclk = RCC_GetRC4MClockFreq();
                break;
        }
        BaudRate = Fclk/UART_InitStruct->BaudRate - 1;
    }
    if(UARTx == UART0)
    {
        /*总线时钟使能*/
        RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UART0);
		/*操作时钟使能*/
        RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_UART0);  
    }
    else if(UARTx == UART1)
    {
        /*总线时钟使能*/
        RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UART1);
		/*操作时钟使能*/
        RCC_Group1_EnableOperationClock(RCC_OPERATION1_CLOCK_UART1);
    }
    else if(UARTx == UART4)
    {
        /*总线时钟使能*/
        RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UART4);
        Fclk = RCC_GetAPB2ClockFreq(RCC_GetAHBClockFreq(RCC_GetSystemClockFreq()));
        BaudRate = Fclk/UART_InitStruct->BaudRate - 1;
        
    }
    else if(UARTx == UART5)
    {
        /*总线时钟使能*/
        RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UART5);
        Fclk = RCC_GetAPB2ClockFreq(RCC_GetAHBClockFreq(RCC_GetSystemClockFreq()));
        BaudRate = Fclk/UART_InitStruct->BaudRate - 1;
    }
	/*发送接收配置*/
    if(UART_InitStruct->TransferDirection&UART_DIRECTION_TX)
    {
        UART_EnableDirectionTx(UARTx);
    }
    if(UART_InitStruct->TransferDirection&UART_DIRECTION_RX)
    {
        UART_EnableDirectionRx(UARTx);
    }
    if(UART_InitStruct->InfraredModulation == ENABLE)
    {
        /*红外发送使能*/
        UART_Enable_InfraredModulation(UARTx);
    }
    /*配置波特率*/
    UART_SetBaudRate(UARTx,BaudRate);
	/*配置停止位*/
    UART_SetStopBitsLength(UARTx,UART_InitStruct->StopBits);
	/*配置数据位宽*/
    UART_SetDataWidth(UARTx,UART_InitStruct->DataWidth);
	/*配置校验位*/
    UART_SetParity(UARTx,UART_InitStruct->Parity);
	
    status = PASS;
    return status;
}
/**
  * @brief	根据需要功能配置红外调制寄存器 
  *               
  * @param	UARTx  外设入口地址
  *
  * @param 	UART_InitStruct指向一个UART_InitTypeDef类型的结构体，它包含指定UART外设的配置信息	  
  *         
  * @retval	ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS UART配置成功        
  */
ErrorStatus UART_InfraRed_Init(UART_Type *UARTx, UART_InfraRed_InitTypeDef *InfraRed_InitStruct)
{
    ErrorStatus status = FAIL;
    uint32_t  tempTZBRG,tempTH;

	/* 参数合法性检查 */
    assert_param(IS_UART_INSTANCE(UARTx));
    assert_param(IS_UART_INFRARED_POLARITY(InfraRed_InitStruct->Polarity));
    
    RCC_Group3_EnableBusClock(RCC_BUS3_CLOCK_UARTIR);
    /*红外调制极性*/
    UART_SetInfraredPolarity(UART_COMMON,InfraRed_InitStruct->Polarity);
    /*红外调制频率*/
    tempTZBRG = (uint32_t)((RCC_GetAPB1ClockFreq(RCC_GetAHBClockFreq(RCC_GetSystemClockFreq()))*1.0)/InfraRed_InitStruct->ModulationFrequency -1);
    /* 占空比计算 */
    if((tempTZBRG>>4) != 0)
		{
			
			tempTH = (uint32_t)(((float)InfraRed_InitStruct->ModulationDuty/100.0)*((float)(tempTZBRG + 1)/(float)(tempTZBRG>>4)) + 0.5);
		}
		else
		{
			tempTH = (uint32_t)(((float)InfraRed_InitStruct->ModulationDuty/100.0)*(float)(tempTZBRG + 1) + 0.5);
			
		}
    /* 调制频率和占空比配置 */    
    UART_SetInfraredModulationDuty(UART_COMMON,tempTH);
    UART_SetInfraredModulationFrequency(UART_COMMON,tempTZBRG);
    
    status = PASS;
    return status;
}

/**
  * @brief	UART_InitStruct 为默认配置
  * @param 	UART_InitStruct 指向需要将值设置为默认配置的结构体 @ref UART_InitTypeDef structure 
  *         结构体
  * @retval	None
  */
void UART_InfraRed_StructInit(UART_InfraRed_InitTypeDef *InfraRed_InitStruct)
{
	InfraRed_InitStruct->Polarity                  = UART_INFRARED_POLARITY_NEGATIVE;//UART_INFRARED_POLARITY_POSITIVE;
	InfraRed_InitStruct->ModulationDuty            = 50;
	InfraRed_InitStruct->ModulationFrequency       = 38000;
}
/**
  * @brief	UART_InitStruct 为默认配置
  * @param 	UART_InitStruct 指向需要将值设置为默认配置的结构体 @ref UART_InitTypeDef structure 
  *         结构体
  * @retval	None
  */
void UART_StructInit(UART_InitTypeDef *UART_InitStruct)
{
	UART_InitStruct->BaudRate            = 9600;
	UART_InitStruct->DataWidth           = UART_DATAWIDTH_8B;
	UART_InitStruct->StopBits            = UART_STOPBITS_1;
	UART_InitStruct->Parity              = UART_PARITY_NONE ;
	UART_InitStruct->TransferDirection   = UART_DIRECTION_TX_RX;
	UART_InitStruct->InfraredModulation  = DISABLE;
    UART_InitStruct->ClockSrc            = RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
}
/**
  * @}
  */

/**
  * @}
  */

/*************************************************************END OF FILE************************************************************/

