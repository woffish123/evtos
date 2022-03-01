
#ifndef __SX1276_H
#define __SX1276_H

//=================================================================================== include hear file

#include "predef.h"
#include "em_i2c.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "em_leuart.h"
#include "em_idac.h"
#include "em_timer.h"
#include "em_adc.h"
#include "LeUart.h"
#include "RTC.h"

/*   内存模式说明 
1 ：RegFifiTxBaseAddr ，RegFifiRxBaseAddr	  ： 收发FIFO栈底地址，用于确定内存的收发占用的空间
2 ：RegFifoAddrPtr ：SPi 读写FiFO时的起始地址，读写前必须先设置这个地址。 随读写操作自动增加
3 ：RegRxNbBytes ：接收时写入的数据块大小， 可能包含多个包， 需要迅速读取
4 ：RegPayLoadLength ： 发送时将要被发送的数据块的大小。
5 ：RegFifoRxCurrentAddr：上一次接收到的包的起始位置。
 收发最大包长度 120 byte

*/

#define NOISE_ABSOLUTE_ZERO                         -174.0
#define NOISE_FIGURE_LF                                4.0
#define NOISE_FIGURE_HF                                6.0 

//=================================================================================== Register Address Define
//------SX1276 Internal registers Address-----
#define REG_LR_FIFO                                 0x00 
// Common settings
#define REG_LR_OPMODE                               0x01 
#define REG_LR_BANDSETTING                          0x04
#define REG_LR_FRFMSB                               0x06 
#define REG_LR_FRFMID                               0x07
#define REG_LR_FRFLSB                               0x08 
// Tx settings
#define REG_LR_PACONFIG                             0x09 
#define REG_LR_PARAMP                               0x0A 
#define REG_LR_OCP                                  0x0B 
// Rx settings
#define REG_LR_LNA                                  0x0C 
// LoRa registers
#define REG_LR_FIFOADDRPTR                          0x0D   //SPI操作起始地址
#define REG_LR_FIFOTXBASEADDR                       0x0E   //Tx接受开始地址，在连续发射状态下，仅第一个包从这个地址开始，其他后续包起始地址将顺延
#define REG_LR_FIFORXBASEADDR                       0x0F   //Rx接受开始地址，在连续接受状态下，仅第一个包从这个地址开始，其他后续包起始地址将顺延
#define REG_LR_FIFORXCURRENTADDR                    0x10   //最后一次接收到的数据包的起始地址 。
#define REG_LR_IRQFLAGSMASK                         0x11 
#define REG_LR_IRQFLAGS                             0x12 
#define REG_LR_NBRXBYTES                            0x13   //最后一次接受到的数据包的字节总数。
#define REG_LR_RXHEADERCNTVALUEMSB                  0x14 
#define REG_LR_RXHEADERCNTVALUELSB                  0x15 
#define REG_LR_RXPACKETCNTVALUEMSB                  0x16 
#define REG_LR_RXPACKETCNTVALUELSB                  0x17 
#define REG_LR_MODEMSTAT                            0x18 
#define REG_LR_PKTSNRVALUE                          0x19 
#define REG_LR_PKTRSSIVALUE                         0x1A 
#define REG_LR_RSSIVALUE                            0x1B 
#define REG_LR_HOPCHANNEL                           0x1C 
#define REG_LR_MODEMCONFIG1                         0x1D 
#define REG_LR_MODEMCONFIG2                         0x1E 
#define REG_LR_SYMBTIMEOUTLSB                       0x1F 
#define REG_LR_PREAMBLEMSB                          0x20 
#define REG_LR_PREAMBLELSB                          0x21 
#define REG_LR_PAYLOADLENGTH                        0x22 
#define REG_LR_PAYLOADMAXLENGTH                     0x23 
#define REG_LR_HOPPERIOD                            0x24 
#define REG_LR_FIFORXBYTEADDR                       0x25    // 最后一次接受写入的最后一个字节的地址
#define REG_LR_MODEMCONFIG3                         0x26
// end of documented register in datasheet
#define REG_LP_IMAGECAL                             0x3b
#define REG_LP_TEMP		                            0x3c
#define REG_LP_LOWBAT                             0x3d 
// I/O settings
#define REG_LR_DIOMAPPING1                          0x40
#define REG_LR_DIOMAPPING2                          0x41
// Version
#define REG_LR_VERSION                              0x42
// Additional settings
#define REG_LR_PLLHOP                               0x44
#define REG_LR_TCXO                                 0x4B
#define REG_LR_PADAC                                0x4D
#define REG_LR_FORMERTEMP                           0x5B
#define REG_LR_BITRATEFRAC                          0x5D
#define REG_LR_AGCREF                               0x61
#define REG_LR_AGCTHRESH1                           0x62
#define REG_LR_AGCTHRESH2                           0x63
#define REG_LR_AGCTHRESH3                           0x64

//=================================================================================== Const









//------Pin Para Define-----

//  PC15  SPI-CLK , PC14  SPI_CS
//  PD7  SPI_MOSI   PD6   SPI_MISO 
//  DIO0 PC10 ->Pe10 , DIO1 PC11 ->Pe11  ,DIO2 PF5 ->PE12 ,DIO3 PF4 ->Pe13  , DIO4 PF3  -> PA1  DIO5  PA9   

// reset   PC8 ->PF2. txctrl  pa8 -> PC13 ,rxctrl pA10 ->PA2 
//  

#define AntRxMode()          {GPIO->P[0].DOUTSET =(1<<2); GPIO->P[2].DOUTCLR = (1<<13);}
#define AntTxMode()          {GPIO->P[0].DOUTCLR =(1<<2); GPIO->P[2].DOUTSET =(1<<13);}
#define AntSleepMode()       {GPIO->P[0].DOUTCLR =(1<<2); GPIO->P[2].DOUTCLR =(1<<13);}

// CADDetedct  check and Dio4  pf3.
#define GetCadDetect()       ( GPIO->P[0].DIN &  (1<<1) )

#define IntEnable()    GPIO_IntEnable( 1<<13 | 1<<10 | 1<<11 )
#define IntDisable()   GPIO_IntDisable(1<<13 | 1<<10 | 1<<11 )
#define IntClr()       GPIO_IntClear(1<<10 |1<<11 |1<<13)
// reset 

#define LoRaResetLow()            GPIO->P[5].DOUTCLR = 1<<2
#define LoRaResetHigh()           GPIO->P[5].DOUTSET = 1<<2
// cs 
#define LowCS()	      GPIO->P[2].DOUTCLR = (1<<14) 
#define HighCS()      GPIO->P[2].DOUTSET = (1<<14) 
// clk

// enable spi clk , enable spi , and low cs pin 
#define SpiEnable()			   {SetChipSPI(); LowCS();}
// Disable spi clk ,Disable spi , and high cs pin 

#define SpiDisable()			   {ResetChipSPI(); HighCS();}
// before the Spi is used . we should enable the GPIO first .








#define FREQ_STEP          61.03515625

#define PktMaxLen          120
#define FIFORXBASEADDR    0x0
#define FIFOTXBASEADDR    0x80 
            
//------RegOpMode-----
#define RFLR_OPMODE_LONGRANGEMODE_MASK              0x7F 
#define RFLR_OPMODE_LONGRANGEMODE_OFF               0x00 // Default
#define RFLR_OPMODE_LONGRANGEMODE_ON                0x80 

#define RFLR_OPMODE_ACCESSSHAREDREG_MASK            0xBF 
#define RFLR_OPMODE_ACCESSSHAREDREG_ENABLE          0x40 
#define RFLR_OPMODE_ACCESSSHAREDREG_DISABLE         0x00 // Default

#define RFLR_OPMODE_FREQMODE_ACCESS_MASK            0xF7
#define RFLR_OPMODE_FREQMODE_ACCESS_LF              0x08 // Default
#define RFLR_OPMODE_FREQMODE_ACCESS_HF              0x00 

#define RFLR_OPMODE_MASK                            0x07 
#define RFLR_OPMODE_SLEEP                           0x00 
#define RFLR_OPMODE_STANDBY                         0x01 // Default
#define RFLR_OPMODE_SYNTHESIZER_TX                  0x02 
#define RFLR_OPMODE_TRANSMITTER                     0x03 
#define RFLR_OPMODE_SYNTHESIZER_RX                  0x04 
#define RFLR_OPMODE_RECEIVER_CONTINUE               0x05 
// LoRa specific modes
#define RFLR_OPMODE_RECEIVER_SINGLE                 0x06 
#define RFLR_OPMODE_CAD                             0x07 

//------RegBandSetting-----
#define RFLR_BANDSETTING_MASK                    0x3F 
#define RFLR_BANDSETTING_AUTO                    0x00 // Default
#define RFLR_BANDSETTING_DIV_BY_1                0x40
#define RFLR_BANDSETTING_DIV_BY_2                0x80
#define RFLR_BANDSETTING_DIV_BY_6                0xC0

/*!
 * RegPaConfig
 */
#define RFLR_PACONFIG_PASELECT_MASK                 0x7F 
#define RFLR_PACONFIG_PASELECT_PABOOST              0x80 
#define RFLR_PACONFIG_PASELECT_RFO                  0x00 // Default

#define RFLR_PACONFIG_MAX_POWER_MASK                0x8F

#define RFLR_PACONFIG_OUTPUTPOWER_MASK              0xF0 
 
/*!
 * RegPaRamp
 */
#define RFLR_PARAMP_TXBANDFORCE_MASK                0xEF 
#define RFLR_PARAMP_TXBANDFORCE_BAND_SEL            0x10 
#define RFLR_PARAMP_TXBANDFORCE_AUTO                0x00 // Default

#define RFLR_PARAMP_MASK                            0xF0 
#define RFLR_PARAMP_3400_US                         0x00 
#define RFLR_PARAMP_2000_US                         0x01 
#define RFLR_PARAMP_1000_US                         0x02
#define RFLR_PARAMP_0500_US                         0x03 
#define RFLR_PARAMP_0250_US                         0x04 
#define RFLR_PARAMP_0125_US                         0x05 
#define RFLR_PARAMP_0100_US                         0x06 
#define RFLR_PARAMP_0062_US                         0x07 
#define RFLR_PARAMP_0050_US                         0x08 
#define RFLR_PARAMP_0040_US                         0x09 // Default
#define RFLR_PARAMP_0031_US                         0x0A 
#define RFLR_PARAMP_0025_US                         0x0B 
#define RFLR_PARAMP_0020_US                         0x0C 
#define RFLR_PARAMP_0015_US                         0x0D 
#define RFLR_PARAMP_0012_US                         0x0E 
#define RFLR_PARAMP_0010_US                         0x0F 

/*!
 * RegOcp
 */
#define RFLR_OCP_MASK                               0xDF 
#define RFLR_OCP_ON                                 0x20 // Default
#define RFLR_OCP_OFF                                0x00   

#define RFLR_OCP_TRIM_MASK                          0xE0
#define RFLR_OCP_TRIM_045_MA                        0x00
#define RFLR_OCP_TRIM_050_MA                        0x01   
#define RFLR_OCP_TRIM_055_MA                        0x02 
#define RFLR_OCP_TRIM_060_MA                        0x03 
#define RFLR_OCP_TRIM_065_MA                        0x04 
#define RFLR_OCP_TRIM_070_MA                        0x05 
#define RFLR_OCP_TRIM_075_MA                        0x06 
#define RFLR_OCP_TRIM_080_MA                        0x07  
#define RFLR_OCP_TRIM_085_MA                        0x08
#define RFLR_OCP_TRIM_090_MA                        0x09 
#define RFLR_OCP_TRIM_095_MA                        0x0A 
#define RFLR_OCP_TRIM_100_MA                        0x0B  // Default
#define RFLR_OCP_TRIM_105_MA                        0x0C 
#define RFLR_OCP_TRIM_110_MA                        0x0D 
#define RFLR_OCP_TRIM_115_MA                        0x0E 
#define RFLR_OCP_TRIM_120_MA                        0x0F 
#define RFLR_OCP_TRIM_130_MA                        0x10
#define RFLR_OCP_TRIM_140_MA                        0x11   
#define RFLR_OCP_TRIM_150_MA                        0x12 
#define RFLR_OCP_TRIM_160_MA                        0x13 
#define RFLR_OCP_TRIM_170_MA                        0x14 
#define RFLR_OCP_TRIM_180_MA                        0x15 
#define RFLR_OCP_TRIM_190_MA                        0x16 
#define RFLR_OCP_TRIM_200_MA                        0x17  
#define RFLR_OCP_TRIM_210_MA                        0x18
#define RFLR_OCP_TRIM_220_MA                        0x19 
#define RFLR_OCP_TRIM_230_MA                        0x1A 
#define RFLR_OCP_TRIM_240_MA                        0x1B
//------RegLna-----
#define RFLR_LNA_GAIN_MASK                          0x1F 
#define RFLR_LNA_GAIN_G1                            0x20 // Default max
#define RFLR_LNA_GAIN_G2                            0x40 //  
#define RFLR_LNA_GAIN_G3                            0x60 
#define RFLR_LNA_GAIN_G4                            0x80 
#define RFLR_LNA_GAIN_G5                            0xA0 
#define RFLR_LNA_GAIN_G6                            0xC0 // min

#define RFLR_LNA_BOOST_LF_MASK                      0xE7 
#define RFLR_LNA_BOOST_LF_DEFAULT                   0x00 // Default
#define RFLR_LNA_BOOST_LF_GAIN                      0x08 
#define RFLR_LNA_BOOST_LF_IP3                       0x10 
#define RFLR_LNA_BOOST_LF_BOOST                     0x18 

#define RFLR_LNA_RXBANDFORCE_MASK                   0xFB 
#define RFLR_LNA_RXBANDFORCE_BAND_SEL               0x04
#define RFLR_LNA_RXBANDFORCE_AUTO                   0x00 // Default

#define RFLR_LNA_BOOST_HF_MASK                      0xFC 
#define RFLR_LNA_BOOST_HF_OFF                       0x00 // Default
#define RFLR_LNA_BOOST_HF_ON                        0x03 

//------RegModemConfig2-----
#define RFLR_MODEMCONFIG2_SF_MASK                   0x0F 
#define RFLR_MODEMCONFIG2_SF_6                      0x60 
#define RFLR_MODEMCONFIG2_SF_7                      0x70 // Default
#define RFLR_MODEMCONFIG2_SF_8                      0x80 
#define RFLR_MODEMCONFIG2_SF_9                      0x90 
#define RFLR_MODEMCONFIG2_SF_10                     0xA0 
#define RFLR_MODEMCONFIG2_SF_11                     0xB0 
#define RFLR_MODEMCONFIG2_SF_12                     0xC0 

#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_MASK     0xF7 
#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_ON       0x08 
#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_OFF      0x00 

#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK         0xFB 
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON           0x04 

  
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_OFF          0x00 // Default
 
#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK       0xFC 
#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB            0x00 // Default

   
//------RegModemConfig1-----
#define RFLR_MODEMCONFIG1_BW_MASK                   0x0F 

#define RFLR_MODEMCONFIG1_BW_7_81_KHZ               0x00 
#define RFLR_MODEMCONFIG1_BW_10_41_KHZ              0x10 
#define RFLR_MODEMCONFIG1_BW_15_62_KHZ              0x20 
#define RFLR_MODEMCONFIG1_BW_20_83_KHZ              0x30 
#define RFLR_MODEMCONFIG1_BW_31_25_KHZ              0x40 
#define RFLR_MODEMCONFIG1_BW_41_66_KHZ              0x50 
#define RFLR_MODEMCONFIG1_BW_62_50_KHZ              0x60 
#define RFLR_MODEMCONFIG1_BW_125_KHZ                0x70 // Default
#define RFLR_MODEMCONFIG1_BW_250_KHZ                0x80 
#define RFLR_MODEMCONFIG1_BW_500_KHZ                0x90 
                                                    
#define RFLR_MODEMCONFIG1_CODINGRATE_MASK           0xF1 
#define RFLR_MODEMCONFIG1_CODINGRATE_4_5            0x02
#define RFLR_MODEMCONFIG1_CODINGRATE_4_6            0x04 // Default
#define RFLR_MODEMCONFIG1_CODINGRATE_4_7            0x06 
#define RFLR_MODEMCONFIG1_CODINGRATE_4_8            0x08 
                                                    
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK       0xFE 
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_ON         0x01 
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF        0x00 // Default

//------RegModemConfig3-----
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK  0xF7 
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON    0x08 
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_OFF   0x00 // Default

#define RFLR_MODEMCONFIG3_AGCAUTO_MASK              0xFB 
#define RFLR_MODEMCONFIG3_AGCAUTO_ON                0x04 // Default 
#define RFLR_MODEMCONFIG3_AGCAUTO_OFF               0x00 

//------RegPaConfig-----
#define RFLR_PACONFIG_PASELECT_MASK                 0x7F 
#define RFLR_PACONFIG_PASELECT_PABOOST              0x80 
#define RFLR_PACONFIG_PASELECT_RFO                  0x00 // Default

#define RFLR_PACONFIG_MAX_POWER_MASK                0x8F

#define RFLR_PACONFIG_OUTPUTPOWER_MASK              0xF0 

//------RegIrqFlags-----
#define RFLR_IRQFLAGS_RXTIMEOUT                     0x80 
#define RFLR_IRQFLAGS_RXDONE                        0x40 
#define RFLR_IRQFLAGS_PAYLOADCRCERROR               0x20 
#define RFLR_IRQFLAGS_VALIDHEADER                   0x10
#define RFLR_IRQFLAGS_TXDONE                        0x08 
#define RFLR_IRQFLAGS_CADDONE                       0x04 
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL            0x02 
#define RFLR_IRQFLAGS_CADDETECTED                   0x01 
   
//------RegHopChannel (Read Only)----- 
#define RFLR_HOPCHANNEL_PLL_LOCK_TIMEOUT_MASK       0x7F 
#define RFLR_HOPCHANNEL_PLL_LOCK_FAIL               0x80 
#define RFLR_HOPCHANNEL_PLL_LOCK_SUCCEED            0x00 // Default
                                                    
#define RFLR_HOPCHANNEL_PAYLOAD_CRC16_MASK          0xBF
#define RFLR_HOPCHANNEL_PAYLOAD_CRC16_ON            0x40
#define RFLR_HOPCHANNEL_PAYLOAD_CRC16_OFF           0x00 // Default

#define RFLR_HOPCHANNEL_CHANNEL_MASK                0x3F 

/*!
 * RegDioMapping1
 */
#define RFLR_DIOMAPPING1_DIO0_MASK                  0x3F
#define RFLR_DIOMAPPING1_DIO0_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO0_01                    0x40
#define RFLR_DIOMAPPING1_DIO0_10                    0x80
#define RFLR_DIOMAPPING1_DIO0_11                    0xC0

#define RFLR_DIOMAPPING1_DIO1_MASK                  0xCF
#define RFLR_DIOMAPPING1_DIO1_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO1_01                    0x10
#define RFLR_DIOMAPPING1_DIO1_10                    0x20
#define RFLR_DIOMAPPING1_DIO1_11                    0x30

#define RFLR_DIOMAPPING1_DIO2_MASK                  0xF3
#define RFLR_DIOMAPPING1_DIO2_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO2_01                    0x04
#define RFLR_DIOMAPPING1_DIO2_10                    0x08
#define RFLR_DIOMAPPING1_DIO2_11                    0x0C

#define RFLR_DIOMAPPING1_DIO3_MASK                  0xFC
#define RFLR_DIOMAPPING1_DIO3_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO3_01                    0x01
#define RFLR_DIOMAPPING1_DIO3_10                    0x02
#define RFLR_DIOMAPPING1_DIO3_11                    0x03

/*!
 * RegDioMapping2
 */
#define RFLR_DIOMAPPING2_DIO4_MASK                  0x3F
#define RFLR_DIOMAPPING2_DIO4_00                    0x00  // Default
#define RFLR_DIOMAPPING2_DIO4_01                    0x40
#define RFLR_DIOMAPPING2_DIO4_10                    0x80
#define RFLR_DIOMAPPING2_DIO4_11                    0xC0

#define RFLR_DIOMAPPING2_DIO5_MASK                  0xCF
#define RFLR_DIOMAPPING2_DIO5_00                    0x00  // Default
#define RFLR_DIOMAPPING2_DIO5_01                    0x10
#define RFLR_DIOMAPPING2_DIO5_10                    0x20
#define RFLR_DIOMAPPING2_DIO5_11                    0x30   
   
//=================================================================================== Data Type Define


typedef enum
{
    RFLR_STATE_IDLE,
    RFLR_STATE_STANDBY,
    RFLR_STATE_RX_SINGLE_RUNNING,
    RFLR_STATE_RX_CONTINUE_RUNNING,
    RFLR_STATE_RX_SINGLE_VALIDHEADER,
    RFLR_STATE_RX_CONTINUE_VALIDHEADER,
    
//    RFLR_STATE_RX_SINGLE_DONE,
//    RFLR_STATE_RX_SINGLE_TIMEOUT,
//    RFLR_STATE_RX_CONTINUE_DONE,
//    RFLR_STATE_RX_CONTINUE_TIMEOUT,   
    
//    RFLR_STATE_TX_INIT,
    RFLR_STATE_TX_RUNNING,
    RFLR_STATE_TX_DONE,
    RFLR_STATE_TX_TIMEOUT,
    RFLR_STATE_CAD_INIT,
    RFLR_STATE_CAD_TX_RUNNING,
    RFLR_STATE_CAD_RX_RUNNING

}tRFLRStates;





//===================================================================================

// outer function .
//  the dma rx and send buffer can use same location  
#define  PACKETLEN          65
#define  SPICmdBufLen       5
#define  RFFixLen          10    // len +UserID[PACKET_USERID_CNT]+LocalID+ RemoteID +Cmd  , len is not send to peer, but used to set fifo addr .
#define  RFBUFLEN      (PACKETLEN - RFFixLen ) // 53byte .

#define  DefRcvTimeOut    30
#define  MaxRcvTimeOut    0x3ff
#define  RcvTimeOutMask   0xfc00
#define  InfinitRcv       0x3f
#define  InfinitMode      0xfc00
#define  SendMaxCadCnt     5


void OnLoraDelay(void);
void RF_StandBy (void);
void RfAck();

typedef struct _MESSAGE_
{
    uint8_t len ;  // total message length .  before send user should set this value , after read  user can get message len from here .
    uint8_t UserID[PACKET_USERID_CNT];   //wlan name , this will used to show if the node is same WLAN net  be checked during reiceiving . must same .  this value is set and check  anto by driver .
    uint16_t LocalID;                    //it is the local id, means the where the message coming from . while be set auto by driver
    //uint16_t MessageID ;                 // the message Counter  will be set auto by driver ,it will add by 1 every time there is a message succused received or send ,when received , if the MessageId in the Node is bigger than this one , the message is no used . it maybe a repeat message.  
    uint16_t RemoteID ;                  // it is the remote id , means where the message will be send to .   it is stored in globle data , and auto set by driver , if wanto chang it should chang before send .
    uint8_t  Cmd;  // cmd  same as usrt cmd .
    uint8_t  buf[1];
} MESSAGE ; 

extern  uint8_t        SpiFifoRxBuf[PACKETLEN];
extern  uint8_t        SpiFifoTxBuf[PACKETLEN];
//message used from RFSendBuf or RFRecvBuf  , the UserId and LocalId will be check when receiving.

//#define  lpRxMessage ((MESSAGE *)(SpiFifoRxBuf))
//#define  lpTxMessage ((MESSAGE *)(SpiFifoTxBuf))

     
#define  lpRxBuf           ((uint8_t *) &SpiFifoRxBuf[10])
#define  lpTxBuf           ((uint8_t *) &SpiFifoTxBuf[10])


__STATIC_INLINE void SetTxCmd(uint8_t cmd) 
{  SpiFifoTxBuf[9] =cmd; } 
#define GetTxCmd()   SpiFifoTxBuf[9] 

__STATIC_INLINE uint8_t GetRxCmd(void) 
{ return SpiFifoRxBuf[9]; } 

__STATIC_INLINE void SetTxLen(uint8_t len) 
{  SpiFifoTxBuf[0] = len + RFFixLen; } 

__STATIC_INLINE uint8_t GetRxLen(void) 
{ return SpiFifoRxBuf[0] - RFFixLen+1; } 



#define MessageIdLen  (PACKET_USERID_CNT +4)






// outer function  


// the Send and Recv buf struct is  [PACKET_USERID_CNT] +[LOCAL_ID_CNT]+Cmd[1]+buf


// initial the Spi interface , it  should be  call ,before calling   SX1276SetLoRaInit.  there should be a long delay  (bigger then 6ms )between these two function .
void SX1276Init( void );
// acording the LoRaSettings to init the Lora moderm .
uint8_t SX1276SetLoRaInit(void);
//当射频参数 通过修改 LoRaSettings中的值修改之后，需要调用这个函数将更新后的值配置到寄存器中。
void ChangeRfConfig(void);



// disable RF  . main should call it to reduce the power consume ,when RF is  no used  
void RF_Sleep( void );




// 配置进入接受状态， 参数为小于等于MaxRcvTimeOut 则进入单次接受状态， 等待时间为 timeout*Ts 参数为大于 MaxRcvTimeOut 小于 InfinitMode 则进入多次尝试接收状态。  尝试次数为MaxRcvTimeOut >>12 最大 63次 ， 参数为 InfinitMode 则进入持续接受状态 ，一直等待 直到收到数据 ，进入接受状态后进一步信息将以任务字方式 反馈到主程序
// 当接收到数据后，SpiFifoRxBuf[0]为接受到的数据长度 +PACKET_START_ADDR, SpiFifoRxBuf[PACKET_START_ADDR]开始地址为有效数据包  

// 当进入连续接受模式 ， 整个Fifo被用来存储数据，此时数据会自动累加 在FIFO中循环，需要根据
// RegRxNbBytes 得到上次包的大小 ，RegFifoRxCurrentAddr 上次包存储的位置， 进行读取 。
// 当单次接受模式时 ， 收到的数据必然是从FIFORXBASEADDR开始存储的 ，从这里接受数据即可。

//调用后最终通过HAL EVENT方式反馈发送结果， 连续接受和单次接收返回的状态不同

//1 ：HAL_LORA_RX_DONE 接受到数据包 。 
//2 ：HAL_LORA_RX_TIMEOUT 未接收到数据包 ，信道有不正确的打包方式的发送在进行，但无法正确解析， 仅出现在单次接受情况下
//3 ：HAL_LORA_INVALIIDE_USERID ，接收到包头格式正确，但用户名不一致的包，中断当前的接受 。
//4 ：HAL_LORA_INVALIIDE_CRC  接收到包头正确 ， 用户名正确，但CRC校验失败的包， 这种情况意味着，有为止的发送者所发的包被干扰，或信噪比较低，如果可能应降低发射速率，或采用更高的CRC校验方式，以提高信噪比加大接受成功率。
// 连续接收状态 ：
//5 ：HAL_LORA_RX_CAD_OVR  信道一直为空未 接受到数据包 。  仅出现在多次接受情况下

void EnterRcvMode(uint16_t timeout);

// 衍生的接受后进入立即发送状态 函数， 用于产生应答  所发送的内容的命令字节 将被标识为Answer ，数据区 内容如有需要，如果此时发送区长度域为0 则只发送0字节 否则认为需要返回其他数据，此时数据长度必须提前放入发送缓冲中，参数意义同EnterRxMode（）
// 返回状态值为 
//1 ：HAL_LORA_RX_DONE 接受到数据包 。 
//2 ：HAL_LORA_RX_TIMEOUT 未接收到数据包 ，信道有不正确的打包方式的发送在进行，但无法正确解析， 仅出现在单次接受情况下
//3 ：HAL_LORA_INVALIIDE_USERID ，接收到包头格式正确，但用户名不一致的包，中断当前的接受 。
//4 ：HAL_LORA_INVALIIDE_CRC  接收到包头正确 ， 用户名正确，但CRC校验失败的包， 这种情况意味着，有为止的发送者所发的包被干扰，或信噪比较低，如果可能应降低发射速率，或采用更高的CRC校验方式，以提高信噪比加大接受成功率。
// 连续接收状态 ：
//5 ：HAL_LORA_RX_CAD_OVR  信道一直为空未 接受到数据包 。  仅出现在多次接受情况下
//6 ：：HAL_LORA_TX_DONE ： 信息已发出。
#define  RcvSendMode( timeout) {EnterRcvMode(timeout); status_Set(STATUS_LORA_PINPANG_MODE);}






//检测信道情况尝试发送数据, 调用之前应该将需要发送的数据 填充到 SpiFifoTxBuf【PACKET_START_ADDR】 开始的数组中，SpiFifoTxBuf【0】 应该设置为 发送数据数据长度 +PACKET_START_ADDR 
//可能会失败 ， 发送尝试次数为 timeout ， 最大尝试次数为 63 次 ， 当尝试次数为 0 则不检测信道， 直接发送 。多次尝试仍失败 或者尝试次数为0则直接发送， 尝试的时间由LORA的带宽 扩频因子，Crc，参数综合确定 基本为2倍 扩频因子为最大12时发送5个字节的空中时间 。
//调用后最终通过HAL EVENT方式反馈发送结果， 有1种可能 1 ：HAL_LORA_TX_DONE 发送成功 。。

void EnterTXMode(uint16_t timeout);

// 衍生的发送后进入接受状态 函数， 用于产生应答，所发送数据 必须以，提前放入发送缓冲中 , 参数意义同EnterRxMode（）
// 尝试SendMaxCadCnt次发送， 然后配置进入接受状态， 参数为小于等于MaxRcvTimeOut 则进入单次接受状态， 等待时间为 timeout*Ts 参数为大于 MaxRcvTimeOut 小于 InfinitMode 则进入多次尝试接收状态。  尝试次数为MaxRcvTimeOut >>12 最大 63次 ， 参数为 InfinitMode 则进入持续接受状态 ，一直等待 直到收到数据 ，进入接受状态后进一步信息将以任务字方式 反馈到主程序
// 当接收到数据后，SpiFifoRxBuf[0]为接受到的数据长度 +PACKET_START_ADDR, SpiFifoRxBuf[PACKET_START_ADDR]开始地址为有效数据包  

// 当进入连续接受模式 ， 整个Fifo被用来存储数据，此时数据会自动累加 在FIFO中循环，需要根据
// RegRxNbBytes 得到上次包的大小 ，RegFifoRxCurrentAddr 上次包存储的位置， 进行读取 。
// 当单次接受模式时 ， 收到的数据必然是从FIFORXBASEADDR开始存储的 ，从这里接受数据即可。

//调用后最终通过HAL EVENT方式反馈发送结果， 连续接受和单次接收返回的状态不同

//1 ：HAL_LORA_RX_DONE 接受到数据包 。 
//2 ：HAL_LORA_RX_TIMEOUT 未接收到数据包 ，信道有不正确的打包方式的发送在进行，但无法正确解析， 仅出现在单次接受情况下
//3 ：HAL_LORA_INVALIIDE_USERID ，接收到包头格式正确，但用户名不一致的包，中断当前的接受 。
//4 ：HAL_LORA_INVALIIDE_CRC  接收到包头正确 ， 用户名正确，但CRC校验失败的包， 这种情况意味着，有为止的发送者所发的包被干扰，或信噪比较低，如果可能应降低发射速率，或采用更高的CRC校验方式，以提高信噪比加大接受成功率。
// 连续接收状态 ：
//5 ：HAL_LORA_RX_CAD_OVR  信道一直为空未 接受到数据包 。  仅出现在多次接受情况下
#define  SendRcvMode( timeout) {status_Set(STATUS_LORA_PINPANG_MODE); EnterTXMode(timeout);}
// check if rf is working now ;
#define IsRFWorking()    CheckStatus(STATUS_RF_WORKING)
// 用于简单评估当前信号状态  ，用于 一定量的数据发送或接受过程之后 。
bool IsChannelFree(void); 
// 得到 上一次接受的信号强度
extern uint8_t  Rssi ;
#define GetPacketRssi() ( Rssi )




//===================================================================================


#endif //__SX1276_H
