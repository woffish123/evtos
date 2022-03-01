#include "cmdthd.h"
#include "dealerthd.h"
#include "netthd.h"
#include "timealarm.h"
#include "sx1262.h"
// process cmdport   :used to receive uart command .
volatile CmdPort   cmdport ;    
StdEvt CmdPortEvtLoop[Max_CmdPortEvt];


// pre declare end 
void CmdPortInit(void)
{
    cmdport.lplpuartrecv = 0 ;
    cmdport.lplpuartsend = 0 ;

    cmdport.lpuartrecvid = 0 ;

    cmdport.lpuartrecvindex = 0 ;
    cmdport.lpuartsendindex = 0 ;
    cmdport.lpuartsendcnt = 0 ;
}    


// recv lpuart, rf ,rtc  cmd .  check if it is a big cmd ,
// for big cmd defer it for late use. else let is pass. 
void CmdPort_initial(LPThdBlock const lpb, StdEvt evt)
{  
    uint8_t * lprecvbuf ;
    uint16_t evtid = getevtid(evt);
    Var32    var32 ;
    
    lprecvbuf = (uint8_t *)(getevtmem(evt) );
    switch(evtid)
    {
        case Sig_lpUart_Sended :
        case Sig_None :
        {// no buf .stop continue 
            return ;
        }            
        case Sig_Wakeup :
        {  // make the system not goto sleep , for debug use
            WakeFromSleep();
            lprecvbuf[0] |=0x80 ;
            break;
        }
        case Sig_Get_Time :
        {
            *((uint32_t *)(&lprecvbuf[Cmd4ByteIndex])) = RtcGetTime(); 
            break;
        }
        case Sig_Set_Time :
        {
             RtcSetTime(*((uint32_t *)&(lprecvbuf[Cmd4ByteIndex])));
             RestartTimeAlarm();
             break;
        }
        case Sig_Set_RtcEvt :
        {
            SetRtcEvt(&(lprecvbuf[CmdParamIndex]));
            break;
        }
        case Sig_Get_RtcEvt :
        {
            GetRtcEvt(&(lprecvbuf[CmdParamIndex]));
            break;
        }
        case Sig_Store_RtcEvt:
        {
            SaveGlobalData();
            break;
        }
        case Sig_SetMotor1 :
        {
            SetMotor(0,(MotorState)lprecvbuf[CmdParamIndex] , lprecvbuf[CmdParamIndex+1],lprecvbuf[CmdParamIndex+2]);
            break;
        }
        case Sig_SetMotor2 :
        {
            SetMotor(1,(MotorState)lprecvbuf[CmdParamIndex] , lprecvbuf[CmdParamIndex+1],lprecvbuf[CmdParamIndex+2]);
            break;
        }
        case Sig_SetMotor3 :
        {
            SetMotor(2,(MotorState)lprecvbuf[CmdParamIndex] , lprecvbuf[CmdParamIndex+1],lprecvbuf[CmdParamIndex+2]);
            break;
        }  
        case Sig_TestProc0 :
        {
            AddActiveProcById(BlcId_CmdDealer,MotorProc0Id); 
            break;
        }
        case Sig_Rtc_Adjust :
        {
            var32.data32 = stackcheckmark() ;
            lprecvbuf[0] = 'R' ;
            lprecvbuf[1] = 't' ;
            lprecvbuf[2] = 'c' ;
            lprecvbuf[3] = 'A' ;
            lprecvbuf[4] = ' ';
            lprecvbuf[5] = (uint8_t)((RTC->ADJUST>>8) &0xff) ;
            lprecvbuf[6] = (uint8_t)(RTC->ADJUST &0xff) ; 
            lprecvbuf[7] = ' ' ;
            lprecvbuf[8] = var32.data8.data0;
            lprecvbuf[9] = var32.data8.data1;
            lprecvbuf[10] = var32.data8.data2;
            lprecvbuf[11] = var32.data8.data3;   
            AdcStart(ADC_ChanTemp|ADC_ChanVRef);
            break ;                
        } 
        case Sig_50Hz_Set :
            StartCmp250ms(lprecvbuf[CmdParamIndex]);            
            break;
        case Sig_IrDetect :
            Uart0DoIr();
            lprecvbuf[0] |=0x80 ;
            break;
        case Sig_Reset :   // do reset .
            RCC->SOFTRST = 0x5C5CAABB ;
            // set boot the update flage also , to allow the boot listen for one second .
            SetCheckBoot() ;
            break ;

        case Sig_Lora_GetTime :
        {
            AddActiveProcById(BlcId_Net ,RfGetRtcTimeId); 
            break;
        } 
        case Sig_GetRfMode :
        {
            lprecvbuf[0] = 'R' ;
            lprecvbuf[1] = 'F' ;
            lprecvbuf[2] = ' ' ;
            lprecvbuf[3] = 'm' ;
            lprecvbuf[4] = 'o' ;
            lprecvbuf[5] = 'd' ;
            lprecvbuf[6] = ':' ;
            lprecvbuf[7] = '0'  + GetRfMode();
            GetDevState(&lprecvbuf[8]);
 
            break;
        }   
        case Sig_GotoBoot :    
            SetCheckBoot();
        case Sig_DelayReset :
        {
            lprecvbuf[0] = GetFreeSoftTimer();
            SetSoftTimerDelayMs(lprecvbuf[0],2000);
            SoftTimerBlockAndOpt(lprecvbuf[0] ,0,SftOpt_Quick);
            SetSoftTimerEvt(lprecvbuf[0], Sig_DelayReset);
            StartSoftTimer(lprecvbuf[0]);          
            lprecvbuf[0]= 0xee;
            break;
        }
        case Sig_ShutDown :
        {
            lprecvbuf[CmdByteIndex] = 0xAA ; 
            GotoLowPowerMode();
            break;
        }        

      
        default :
            lprecvbuf[CmdByteIndex] = 0xff ; // unkonw cmd recv  send oxff back.
            break;
    }

    LpUart0Send(evt);
    return ;

}




