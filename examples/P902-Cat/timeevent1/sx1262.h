#ifndef _SX1262_Driver_H
#define _SX1262_Driver_H
#include "timealarm.h"
#include "hal.h"
// include  , to use the fifo to store the send and receive message .
#include "evtos.h"

//   FreCnt *32M /2^25   = Frequncy 
//   so FreCnt  = Frequncy *2^25 /32M   =  Frequncy(based on Mhz) * 2^20  
//  so the Frequncy is stored as a uint16_t value , it is based on 100KHz


#define Band900FreCnt  0x38400000        // 900 <<20 
#define Band850FreCnt  0x35200000        // 850 <<20
#define Band770FreCnt  0x30200000        // 770 <<20
#define Band460FreCnt  0x1CC00000        // 460 <<20
#define Band425FreCnt  0x1A900000        // 425 <<20

#define CurRfFrequncy  4710       

//there is 256 byte  Queue in sx126X , define the half used for tx and half used for ex
#define LORATXBASEADDR   0x00
#define LORARXBASEADDR   0x80

#define MAXTXCNT         127   // must usable tx /rx ability 
//#define USE_TCXO    1
#ifdef USE_TCXO
    /*!
     * Radio complete Wake-up Time with TCXO stabilisation time
     */
    #define RADIO_TCXO_SETUP_TIME                       5 // [ms]
#else
    /*!
     * Radio complete Wake-up Time with TCXO stabilisation time
     */
    #define RADIO_TCXO_SETUP_TIME                       0 // No Used
#endif

/*!
 * Radio complete Wake-up Time with margin for temperature compensation
 */
#define RADIO_WAKEUP_TIME                               3 // [ms]

/*!
 * \brief Compensation delay for SetAutoTx/Rx functions in 15.625 microseconds
 */
#define AUTO_RX_TX_OFFSET                           2

/*!
 * \brief LFSR initial value to compute IBM type CRC
 */
#define CRC_IBM_SEED                                0xFFFF

/*!
 * \brief LFSR initial value to compute CCIT type CRC
 */
#define CRC_CCITT_SEED                              0x1D0F

/*!
 * \brief Polynomial used to compute IBM CRC
 */
#define CRC_POLYNOMIAL_IBM                          0x8005

/*!
 * \brief Polynomial used to compute CCIT CRC
 */
#define CRC_POLYNOMIAL_CCITT                        0x1021

/*!
 * \brief The address of the register holding the first byte defining the CRC seed
 *
 */
#define REG_LR_CRCSEEDBASEADDR                      0x06BC

/*!
 * \brief The address of the register holding the first byte defining the CRC polynomial
 */
#define REG_LR_CRCPOLYBASEADDR                      0x06BE

/*!
 * \brief The address of the register holding the first byte defining the whitening seed
 */
#define REG_LR_WHITSEEDBASEADDR_MSB                 0x06B8
#define REG_LR_WHITSEEDBASEADDR_LSB                 0x06B9

/*!
 * \brief The address of the register holding the packet configuration
 */
#define REG_LR_PACKETPARAMS                         0x0704

/*!
 * \brief The address of the register holding the payload size
 */
#define REG_LR_PAYLOADLENGTH                        0x0702

/*!
 * \brief The addresses of the registers holding SyncWords values
 */
#define REG_LR_SYNCWORDBASEADDRESS                  0x06C0

/*!
 * \brief The addresses of the register holding LoRa Modem SyncWord value
 */
#define REG_LR_SYNCWORD                             0x0740

/*!
 * Syncword for Private LoRa networks
 */
#define LORA_MAC_PRIVATE_SYNCWORD                   0x1424

/*!
 * Syncword for Public LoRa networks
 */
#define LORA_MAC_PUBLIC_SYNCWORD                    0x3444

/*!
 * The address of the register giving a 4 bytes random number
 */
#define RANDOM_NUMBER_GENERATORBASEADDR             0x0819

/*!
 * The address of the register holding RX Gain value (0x94: power saving, 0x96: rx boosted)
 */
#define REG_RX_GAIN                                 0x08AC

/*!
 * Change the value on the device internal trimming capacitor
 */
#define REG_XTA_TRIM                                0x0911

/*!
 * Set the current max value in the over current protection
 */
#define REG_OCP                                     0x08E7

// bit2 =0 used to option the work when BW =500 ,else bit2 = 1 
#define REG_BW500_OP                                 0x0889

// the SX1262 delay time 
// btime function  . used  as speail delay fucntion for RF .
// it will save the source and  delay a fixed time  then call specail rf function when over time .

// sx1262 need some time to switch from one mode to another . and after that the IO Busy will be  low
typedef  enum RFDelay_
{
    None = 0 ,
    Reset_Cal  ,  // cold start  from sleep to STBY_RC . it is a long time  3500us . it will be use at  very beginning 
    Sleep_STBY_RC , // warm start
    STBYRC_STBY_XOCS ,  
    STBY_RC_FS  ,
    STBY_RC_RX ,
    STBY_RC_TX ,
    STBY_XOSC_TX,
    STBY_XOSC_RX,
    FS_RX ,
    FS_TX ,
    RX_FS ,
    RX_TX ,
    End_type
} RfDelay_Type ;
// based on 2us +2us
  
/*
void Initbtim(void );
// wait a cad time . the time is fixed as CadDelay
void WaitDelay(void);
void StopDelay(void);
*/
typedef enum
{
    MODE_POWERDOWN  =  0x00,
    MODE_SLEEP,                                             //! The radio is in sleep mode
    MODE_CALIB,                                             //  the radio in calibration mode ,for 3.5ms
    MODE_STDBY_RC,                                          //! The radio is in standby mode with RC oscillator
    MODE_STDBY_XOSC,                                        //! The radio is in standby mode with XOSC oscillator
    MODE_FS,                                                //! The radio is in frequency synthesis mode
    MODE_TX,                                                //! The radio is in transmit mode
    MODE_RX,                                                //! The radio is in receive mode
    MODE_RX_DC,                                             //! The radio is in receive duty cycle mode
    MODE_CAD,                                             //! The radio is in channel activity detection mode
}LoraState;



typedef enum
{
    IRQ_RADIO_NONE                          = 0x0000,
    IRQ_TX_DONE                             = 0x0001,
    IRQ_RX_DONE                             = 0x0002,
    IRQ_PREAMBLE_DETECTED                   = 0x0004,
    IRQ_SYNCWORD_VALID                      = 0x0008,
    IRQ_HEADER_VALID                        = 0x0010,
    IRQ_HEADER_ERROR                        = 0x0020,
    IRQ_CRC_ERROR                           = 0x0040,
    IRQ_CAD_DONE                            = 0x0080,
    IRQ_CAD_ACTIVITY_DETECTED               = 0x0100,
    IRQ_RX_TX_TIMEOUT                       = 0x0200,
    IRQ_RADIO_ALL                           = 0xFFFF,
}RadioIrqMasks_t;

/*!
 * \brief Represents all possible opcode understood by the radio
 */
typedef enum RadioCommands_e
{
    RADIO_GET_STATUS                        = 0xC0,
    RADIO_WRITE_REGISTER                    = 0x0D,
    RADIO_READ_REGISTER                     = 0x1D,
    RADIO_WRITE_BUFFER                      = 0x0E,
    RADIO_READ_BUFFER                       = 0x1E,
    RADIO_SET_SLEEP                         = 0x84,
    RADIO_SET_STANDBY                       = 0x80,
    RADIO_SET_FS                            = 0xC1,
    RADIO_SET_TX                            = 0x83,
    RADIO_SET_RX                            = 0x82,
    RADIO_SET_RXDUTYCYCLE                   = 0x94,
    RADIO_SET_CAD                           = 0xC5,
    RADIO_SET_TXCONTINUOUSWAVE              = 0xD1,
    RADIO_SET_TXCONTINUOUSPREAMBLE          = 0xD2,
    RADIO_SET_PACKETTYPE                    = 0x8A,
    RADIO_GET_PACKETTYPE                    = 0x11,
    RADIO_SET_RFFREQUENCY                   = 0x86,
    RADIO_SET_TXPARAMS                      = 0x8E,
    RADIO_SET_PACONFIG                      = 0x95,
    RADIO_SET_CADPARAMS                     = 0x88,
    RADIO_SET_BUFFERBASEADDRESS             = 0x8F,
    RADIO_SET_MODULATIONPARAMS              = 0x8B,
    RADIO_SET_PACKETPARAMS                  = 0x8C,
    RADIO_GET_RXBUFFERSTATUS                = 0x13,
    RADIO_GET_PACKETSTATUS                  = 0x14,
    RADIO_GET_RSSIINST                      = 0x15,
    RADIO_GET_STATS                         = 0x10,
    RADIO_RESET_STATS                       = 0x00,
    RADIO_CFG_DIOIRQ                        = 0x08,
    RADIO_GET_IRQSTATUS                     = 0x12,
    RADIO_CLR_IRQSTATUS                     = 0x02,
    RADIO_CALIBRATE                         = 0x89,
    RADIO_CALIBRATEIMAGE                    = 0x98,
    RADIO_SET_REGULATORMODE                 = 0x96,
    RADIO_GET_ERROR                         = 0x17,
    RADIO_CLR_ERROR                         = 0x07,
    RADIO_SET_TCXOMODE                      = 0x97,
    RADIO_SET_TXFALLBACKMODE                = 0x93,
    RADIO_SET_RFSWITCHMODE                  = 0x9D,
    RADIO_SET_STOPRXTIMERONPREAMBLE         = 0x9F,
    RADIO_SET_LORASYMBTIMEOUT               = 0xA0,
}RadioCommands_t;

typedef enum
{
    RADIO_RAMP_10_US                        = 0x00,
    RADIO_RAMP_20_US                        = 0x01,
    RADIO_RAMP_40_US                        = 0x02,
    RADIO_RAMP_80_US                        = 0x03,
    RADIO_RAMP_200_US                       = 0x04,
    RADIO_RAMP_800_US                       = 0x05,
    RADIO_RAMP_1700_US                      = 0x06,
    RADIO_RAMP_3400_US                      = 0x07,
}RadioRampTimes_t;

typedef enum
{
    LORA_CAD_01_SYMBOL                      = 0x00,
    LORA_CAD_02_SYMBOL                      = 0x01,
    LORA_CAD_04_SYMBOL                      = 0x02,
    LORA_CAD_08_SYMBOL                      = 0x03,
    LORA_CAD_16_SYMBOL                      = 0x04,
}RadioLoRaCadSymbols_t;

#define CAD_SYMBOL_NUM          LORA_CAD_02_SYMBOL
#define CAD_DET_PEAK            22
#define CAD_DET_MIN             10
#define CAD_TIMEOUT_MS          2000



typedef enum
{
    LORA_SF5                                = 0x05,
    LORA_SF6                                = 0x06,
    LORA_SF7                                = 0x07,
    LORA_SF8                                = 0x08,
    LORA_SF9                                = 0x09,
    LORA_SF10                               = 0x0A,
    LORA_SF11                               = 0x0B,
    LORA_SF12                               = 0x0C,
}RadioLoRaSpreadingFactors_t;
typedef enum
{
    LORA_BW_500                             = 6,
    LORA_BW_250                             = 5,
    LORA_BW_125                             = 4,
    LORA_BW_062                             = 3,
    LORA_BW_041                             = 10,
    LORA_BW_031                             = 2,
    LORA_BW_020                             = 9,
    LORA_BW_015                             = 1,
    LORA_BW_010                             = 8,
    LORA_BW_007                             = 0,
}RadioLoRaBandwidths_t;
typedef enum
{
    LORA_CR_4_5                             = 0x01,
    LORA_CR_4_6                             = 0x02,
    LORA_CR_4_7                             = 0x03,
    LORA_CR_4_8                             = 0x04,
}RadioLoRaCodingRates_t;




typedef enum
{
    LORA_CAD_ONLY                           = 0x00,
    LORA_CAD_RX                             = 0x01,
    LORA_CAD_LBT                            = 0x10,
}RadioCadExitModes_t;

typedef union
{
    struct
    {
        uint8_t RC64KEnable    : 1;                             //!< Calibrate RC64K clock
        uint8_t RC13MEnable    : 1;                             //!< Calibrate RC13M clock
        uint8_t PLLEnable      : 1;                             //!< Calibrate PLL
        uint8_t ADCPulseEnable : 1;                             //!< Calibrate ADC Pulse
        uint8_t ADCBulkNEnable : 1;                             //!< Calibrate ADC bulkN
        uint8_t ADCBulkPEnable : 1;                             //!< Calibrate ADC bulkP
        uint8_t ImgEnable      : 1;
        uint8_t                : 1;
    }Fields;
    uint8_t Value;
}CalibrationParams_t;

typedef enum FallMode_
{
    FallToFs = 0x40,
    FallToRC = 0x20,
    FallToXOC = 0x30,
}FallMode ;
typedef enum
{
    TCXO_CTRL_1_6V                          = 0x00,
    TCXO_CTRL_1_7V                          = 0x01,
    TCXO_CTRL_1_8V                          = 0x02,
    TCXO_CTRL_2_2V                          = 0x03,
    TCXO_CTRL_2_4V                          = 0x04,
    TCXO_CTRL_2_7V                          = 0x05,
    TCXO_CTRL_3_0V                          = 0x06,
    TCXO_CTRL_3_3V                          = 0x07,
}RadioTcxoCtrlVoltage_t;


typedef union RadioStatus_u
{
    uint8_t Value;
    struct
    {   //bit order is lsb -> msb
        uint8_t Reserved  : 1;  //!< Reserved
        uint8_t CmdStatus : 3;  //!< Command status
        uint8_t ChipMode  : 3;  //!< Chip mode
        uint8_t CpuBusy   : 1;  //!< Flag for CPU radio busy
    }Fields;
}RadioStatus_t;

typedef union
{
    struct
    {
        uint8_t Rc64kCalib              : 1;                    //!< RC 64kHz oscillator calibration failed
        uint8_t Rc13mCalib              : 1;                    //!< RC 13MHz oscillator calibration failed
        uint8_t PllCalib                : 1;                    //!< PLL calibration failed
        uint8_t AdcCalib                : 1;                    //!< ADC calibration failed
        uint8_t ImgCalib                : 1;                    //!< Image calibration failed
        uint8_t XoscStart               : 1;                    //!< XOSC oscillator failed to start
        uint8_t PllLock                 : 1;                    //!< PLL lock failed
        uint8_t BuckStart               : 1;                    //!< Buck converter failed to start
        uint8_t PaRamp                  : 1;                    //!< PA ramp failed
        uint8_t                         : 7;                    //!< Reserved
    }Fields;
    uint16_t Value;
}RadioError_t;


//#define  USE_IMPLICT_HDEADE    1
#define XTAL_FREQ                                   ( double )32000000
#define FREQ_DIV                                    ( double )pow( 2.0, 25.0 )
#define FREQ_STEP                                   ( double )( XTAL_FREQ / FREQ_DIV )
// FreDiv =     Fre(basedonMHz)*2**20 


void InitSpi(void);

//check if PD6 is low 
#define IsBusy() (GPIOD->DIN & (1<<6))
// wait the busy to low 
#define WaitBusy()   while(GPIOD->DIN & (1<<6)) 
    
//power down the chip RF Switch ,set chip in sleep mode . PA15
#define RFPowerOff() GPIOA->DRST = 1<<15 
//power on the chip 
#define RFPowerOn()  GPIOA->DSET = 1<<15
// cs control
#define LowCS()	      SPI_ResetSSNPin(SPI2) //GPIOC->DRST = 1<<7
#define HighCS()      SPI_SetSSNPin(SPI2)  //GPIOC->DSET = 1<<7


//Enable  DIO1 interrupt  
#define EnableLoraInterrupt()  {EXTI_ClearFlag(GPIO_COMMON,GPIO_EXTI_LINE_2);  EXTI_SetTrigEdge(GPIO_COMMON,GPIO_EXTI_LINE_2,GPIO_EXTI_TRIGGER_RISING);}
//Disable DIO1 interrupt  
#define DisableLoraInterrupt() EXTI_SetTrigEdge(GPIO_COMMON,GPIO_EXTI_LINE_2,GPIO_EXTI_TRIGGER_NONE)
//Clear DIO1 interrupt
#define ClearLoraInterrupt()   EXTI_ClearFlag(GPIO_COMMON,GPIO_EXTI_LINE_2)

typedef enum SleepMode_
{
    Shutdown = 0 ,
    WarmStart= 0x04 ,
    WarmStartWithRtc = 0x05
} SleepMode ;
#define WARMSTART   1<<2 
#define RTCWakeUP   1<<0



// get a rssi of last receive packet .
uint8_t GetRssiInst( void );

void  OnLoraIqr(void) ;


#define DefOverTime  512000     //0.8s   based on 15.625 us  ( 1/64 ms) used as tx/rx ovetime times

// initial the interface to lora , it  should be  call ,before calling   SX1276SetLoRaInit.  
// before call this function we should call poweron lora first  .
void PowerOn(void);
void LoraInit( void );
//  deintial the interface to lora . and shutdown the lora power  .
void PowerOff(void );
//当射频参数 通过修改 LoRaSettings中的值修改之后，需要调用这个函数将更新后的值配置到寄存器中。
void  SetRfParam(void );



// get device information 
void GetDevState(uint8_t *lp);
extern LoraState  RFState;
//
__STATIC_INLINE LoraState  GetRfState(void)  
{
    return RFState;
}
// power the rf , before call LoraInit , we should wait 20ms for rf chip to init itself .
void RfPowerOn(void);
// shutdown the rf .
void RfPowerOff(void );

void StartRfMonitor(void) ;
// stop the RF monitor . RF goto sleep mode .
void StopRfMonitor(void) ;
// stop rx_dc mode check if there is online msg and  goto send mode .
void DoSend(void) ;

void SendMsg(RfMsg msg);



#endif
