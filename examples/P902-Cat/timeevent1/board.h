#ifndef Board_CONFIG_
#define Board_CONFIG_


//#define Uart_SCH_Define 
#ifdef  Uart_SCH_Define
// defualt uart0 and lpuart pin map mode .
// lpuart0 tx
#define LPUART_TX_PORT  GPIOA
#define LPUART_TX_PIN   GPIO_PIN_14

#define LPUART_RX_PORT  GPIOA
#define LPUART_RX_PIN   GPIO_PIN_13

// uart rx
#define UART_RX_PORT  GPIOA
#define UART_RX_PIN   GPIO_PIN_2

#define UART_TX_PORT  GPIOA
#define UART_TX_PIN   GPIO_PIN_3

#else 
// reverse  uart0 and  lpuart0 pin map .  

// lpuart0 tx
#define LPUART_TX_PORT  GPIOA
#define LPUART_TX_PIN   GPIO_PIN_3

#define LPUART_RX_PORT  GPIOA
#define LPUART_RX_PIN   GPIO_PIN_2

// uart rx
#define UART_RX_PORT  GPIOA
#define UART_RX_PIN   GPIO_PIN_13

#define UART_TX_PORT  GPIOA
#define UART_TX_PIN   GPIO_PIN_14
#endif 



#define LORA_SW_PORT  GPIOA
#define LORA_SW_PIN   GPIO_PIN_15

//remove control for hal1 detect the interrupt is same with UART0 RXD
#define HAL1_IN_PORT  GPIOA
#define HAL1_IN_PIN   GPIO_PIN_0
// high power enable , 1 enable.
#define MDPWR_CTRL_PORT  GPIOA
#define MDPWR_CTRL_PIN   GPIO_PIN_1

//compare2 in +
#define CMP2_INN_PORT  GPIOA
#define CMP2_INN_PIN   GPIO_PIN_4

#define PWM3_IN2_PORT  GPIOA
#define PWM3_IN2_PIN  GPIO_PIN_5
// 20mA 3.3V out ,1: stop, 0:out
#define VOUT_EN_PORT  GPIOA
#define VOUT_EN_PIN   GPIO_PIN_6
//opa 1 + 2 
#define OPA_INP2_PORT  GPIOA
#define OPA_INP2_PIN   GPIO_PIN_7

#define CMP2_INP_PORT  GPIOA
#define CMP2_INP_PIN   GPIO_PIN_8
// OC  disable ,  0 enable.
#define VREF_EN_PORT  GPIOA
#define VREF_EN_PIN   GPIO_PIN_9

#define LORA_DIO1_PORT  GPIOA
#define LORA_DIO1_PIN   GPIO_PIN_10

#define LED_2_PORT  GPIOB
#define LED_2_PIN   GPIO_PIN_2
// hal2 input 2
#define HAL2_IN_PORT  GPIOB
#define HAL2_IN_PIN   GPIO_PIN_3

#define PWM3_IN1_PORT  GPIOB
#define PWM3_IN1_PIN   GPIO_PIN_4

#define PWM2_IN2_PORT  GPIOB
#define PWM2_IN2_PIN   GPIO_PIN_5

#define LORA_RESET_PORT  GPIOB
#define LORA_RESET_PIN   GPIO_PIN_8

#define LED_1_PORT  GPIOB
#define LED_1_PIN   GPIO_PIN_9

#define PWM2_IN1_PORT  GPIOB
#define PWM2_IN1_PIN   GPIO_PIN_10

#define OPA_INP1_PORT  GPIOB
#define OPA_INP1_PIN   GPIO_PIN_11
// 1 default 0: pressed
#define KEY_1_PORT  GPIOB
#define KEY_1_PIN   GPIO_PIN_12
// 1 default 0: pressed
#define KEY_2_PORT  GPIOB
#define KEY_2_PIN   GPIO_PIN_13
// 0 : enable lora power .
#define LORA_EN_PORT  GPIOB
#define LORA_EN_PIN   GPIO_PIN_14

#define PWM1_IN2_PORT  GPIOC
#define PWM1_IN2_PIN   GPIO_PIN_1

#define PWM1_IN1_PORT  GPIOC
#define PWM1_IN1_PIN   GPIO_PIN_0

#define HAL1_EN_PORT  GPIOC
#define HAL1_EN_PIN   GPIO_PIN_4

#define HAL2_EN_PORT  GPIOC
#define HAL2_EN_PIN   GPIO_PIN_5

#define VBAT_ADC_PORT  GPIOC
#define VBAT_ADC_PIN   GPIO_PIN_6

#define SPI_CS_PORT  GPIOC
#define SPI_CS_PIN   GPIO_PIN_7

#define SPI_CLK_PORT  GPIOC
#define SPI_CLK_PIN   GPIO_PIN_8

#define SPI_MISO_PORT  GPIOC
#define SPI_MISO_PIN   GPIO_PIN_9

#define SPI_MOSI_PORT  GPIOC
#define SPI_MOSI_PIN   GPIO_PIN_10

#define ISEN2_ADC_PORT  GPIOD
#define ISEN2_ADC_PIN   GPIO_PIN_11

#define ISEN1_ADC_PORT  GPIOD
#define ISEN1_ADC_PIN   GPIO_PIN_0

#define ISEN3_ADC_PORT  GPIOD
#define ISEN3_ADC_PIN   GPIO_PIN_1

#define LORA_BUSY_PORT  GPIOD
#define LORA_BUSY_PIN   GPIO_PIN_6


#define SW_DATA_PORT  GPIOD
#define SW_DATA_PIN   GPIO_PIN_8

#define SW_CLK_PORT  GPIOD
#define SW_CLK_PIN   GPIO_PIN_7

// the interrupt pin is 
//1 : Key input  PB12 PB13 ,  only one is used , used PB13  with digital filter
//2 : HAL input  PA0   PB3 , both is used . with digital filter
//3 : Lora  DIO  PA10    no digital filter  .

#endif 

