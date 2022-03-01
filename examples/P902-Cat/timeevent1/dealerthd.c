#include "dealerthd.h"
#include "thd.h"
#include "timealarm.h"
// process cmddealer  : use to do some long proc cmd dealer 
volatile CmdDealer cmddealer ;
StdEvt CmdDealerEvtLoop[Max_CmdDealerEvt];
static LPLongProc  const DealerProcArray[]=
{
    MotorProc0,
    MotorProc1,
    MagnetSetTimeProc,
    0,
    0
};




void CmdDealerInit(void)
{
    // set longproc function array
    SetThdOption((LPThdBlock)(&cmddealer),ThdBlockOption_NotSameProc);
    SetProcAddr((LPThdBlock)(&cmddealer) ,(LPLongProc const *)DealerProcArray);
    // set a longproc  active 
    //AddActiveProc((LPThdBlock)(&cmddealer),MagnetSetTimeProcId); 
}
// CmdDealer Proc do the longProc calling 
void CmdDealer_initial(LPThdBlock const lpb, StdEvt evt)
{

    uint8_t * lpTx ;
    uint16_t evtid = getevtid(evt);
    // longproc calling cmd , will be deal here 
    switch(evtid)
    {
        case Sig_Motor1_Done :    
        { // for test adc
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'M' ;
            lpTx[1] = 'o' ;
            lpTx[2] = 't' ;
            lpTx[3] = 'o' ;
            lpTx[4] = 'r';
            lpTx[5] = '1' ;
            lpTx[6] = ' ' ;
            lpTx[7] = 'D';
            lpTx[8] = 'o';
            lpTx[9] = 'n'  ;
            lpTx[10] = 'e' ;
            lpTx[11] = 'A' +freepoolcnt(0);   
            LpUart0Send(evt); 
            return;
        }  
        case Sig_Motor2_Done :    
        { // for test adc
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'M' ;
            lpTx[1] = 'o' ;
            lpTx[2] = 't' ;
            lpTx[3] = 'o' ;
            lpTx[4] = 'r';
            lpTx[5] = '2' ;
            lpTx[6] = ' ' ;
            lpTx[7] = 'D';
            lpTx[8] = 'o';
            lpTx[9] = 'n'  ;
            lpTx[10] = 'e' ;
            lpTx[11] = 'A' +freepoolcnt(0);   
            LpUart0Send(evt); 
            return;
        }    
        case Sig_Motor3_Done :    
        { // for test adc
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'M' ;
            lpTx[1] = 'o' ;
            lpTx[2] = 't' ;
            lpTx[3] = 'o' ;
            lpTx[4] = 'r';
            lpTx[5] = '3' ;
            lpTx[6] = ' ' ;
            lpTx[7] = 'D';
            lpTx[8] = 'o';
            lpTx[9] = 'n'  ;
            lpTx[10] = 'e' ;
            lpTx[11] = 'A' +freepoolcnt(0);   
            LpUart0Send(evt); 
            return;
        }
        case Sig_MotorProc0_Done :
//            DelActiveProc((LPThdBlock)(&cmddealer),0);
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'M' ;
            lpTx[1] = 'o' ;
            lpTx[2] = 't' ;
            lpTx[3] = 'o' ;
            lpTx[4] = 'r' ;
            lpTx[5] = 'P' ;
            lpTx[6] = 'r' ;
            lpTx[7] = 'o' ;
            lpTx[8] = 'c' ;
            lpTx[9] = '0'  ;
            lpTx[10] = ' ' ;
            lpTx[11] = '!'  ; 
            LpUart0Send(evt);                
            break;
        case Sig_MotorProc1_Done :
//            DelActiveProc((LPThdBlock)(&cmddealer),1);
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'M' ;
            lpTx[1] = 'o' ;
            lpTx[2] = 't' ;
            lpTx[3] = 'o' ;
            lpTx[4] = 'r' ;
            lpTx[5] = 'P' ;
            lpTx[6] = 'r' ;
            lpTx[7] = 'o' ;
            lpTx[8] = 'c' ;
            lpTx[9] = '2'  ;
            lpTx[10] = ' ' ;
            lpTx[11] = '!'  ; 
            LpUart0Send(evt);                
            break;  
       
        default :
            break;
    }
    return ;
}

// this function will do a test motor2. 
// it control the motor2 loop from stop to forward speed up , then speed down, then backword speed up , then , brake, go to forward speed uo then  goto backward speed up.
// start with Sig_Motor2_Test . end report  Sig_MotorProc1_Done
uint8_t MotorProc0(StdEvt evt,LPLongProcData lpdata)
{
    ThdBegin(lpdata);
    ThdWaitSig(lpdata,evt,Sig_Motor2_Test);
    // 初始化语句 ， 下列语句必须在正式开始前被系统进行一次调用以便达到正式等待的状态。
    lpdata->procdata.data32 = 1 ;
    // 初始化语句结束， 开始等待第一个状态， 这里是程序正式开始执行的部分， 在相应的信号到来时被执行。

    Start :    
    SetMotor(1,Motor_ForStable,20,6);
    ThdWaitSig(lpdata,evt,Sig_Motor2_Done);
    SetMotor(1,Motor_ForStable,60,4);
    ThdWaitSig(lpdata,evt,Sig_Motor2_Done);
    SetMotor(1,Motor_ForStable,100,2);
    ThdWaitSig(lpdata,evt,Sig_Motor2_Done);
    SetMotor(1,Motor_BackStable,20,6);
    ThdWaitSig(lpdata,evt,Sig_Motor2_Done);
    SetMotor(1,Motor_BackStable,60,4);
    ThdWaitSig(lpdata,evt,Sig_Motor2_Done);
    SetMotor(1,Motor_BackStable,100,2);
    ThdWaitSig(lpdata,evt,Sig_Motor2_Done);
    lpdata->procdata.data32 ++ ;
    if(lpdata->procdata.data32 < 20 )
        goto Start ;
    SetMotor(1,Motor_PowerOffBrake,0,0);
    ThdWaitSig(lpdata,evt,Sig_Motor2_Done);    
    // 结束整个调用过程，进行清理活动 。  
    postevt((LPThdBlock)(&cmddealer),Sig_MotorProc0_Done);    
    // 清理过程结束，系统恢复到未进行调用的初始状态， 再次使用前必须先进行一次调用，以便完成初始化。
    ThdEnd(lpdata);
}


// this function will do a test motor2. 
// it control the motor2 loop from stop to forward speed up , then speed down, then backword speed up , then , brake, go to forward speed uo then  goto backward speed up.
// start with Sig_Motor2_Test . end report  Sig_MotorProc1_Done
uint8_t MotorProc1(StdEvt evt,LPLongProcData lpdata)
{
    ThdBegin(lpdata);
    ThdWaitSig(lpdata,evt,Sig_Motor1_Test);
    // 初始化语句 ， 下列语句必须在正式开始前被系统进行一次调用以便达到正式等待的状态。
    lpdata->procdata.data32 = 1 ;
    // 初始化语句结束， 开始等待第一个状态， 这里是程序正式开始执行的部分， 在相应的信号到来时被执行。

    Start :    
    SetMotor(0,Motor_ForStable,20,6);
    ThdWaitSig(lpdata,evt,Sig_Motor1_Done);
    SetMotor(0,Motor_ForStable,60,4);
    ThdWaitSig(lpdata,evt,Sig_Motor1_Done);
    SetMotor(0,Motor_ForStable,100,2);
    ThdWaitSig(lpdata,evt,Sig_Motor1_Done);
    SetMotor(0,Motor_BackStable,20,6);
    ThdWaitSig(lpdata,evt,Sig_Motor1_Done);
    SetMotor(0,Motor_BackStable,60,4);
    ThdWaitSig(lpdata,evt,Sig_Motor1_Done);
    SetMotor(0,Motor_BackStable,100,2);
    ThdWaitSig(lpdata,evt,Sig_Motor1_Done);
    lpdata->procdata.data32 ++ ;
    if(lpdata->procdata.data32 < 20 )
        goto Start ;
    SetMotor(0,Motor_PowerOffBrake,0,0);
    ThdWaitSig(lpdata,evt,Sig_Motor1_Done);    
    // 结束整个调用过程，进行清理活动 。  
    postevt((LPThdBlock)(&cmddealer),Sig_MotorProc1_Done);    
    // 清理过程结束，系统恢复到未进行调用的初始状态， 再次使用前必须先进行一次调用，以便完成初始化。
    ThdEnd(lpdata);
}
// check the magnet input , to do led shark and chang the hour,min ,sec value .
// Mag 1, then  Mag2   shark led 4 times start adjust timer .
//
uint8_t  MagnetSetTimeProc(StdEvt evt,LPLongProcData lpdata)
{
    ThdBegin(lpdata);
    ThdWaitSig(lpdata,evt,Sig_Magnet1_On);    
    // init time value .
    lpdata->procdata.data32 = 0xffffffff ;

    // 启动Sig_Magnet_DBOV 超时检查
    lpdata->procdata.data8.data0 = GetFreeSoftTimer();
    SetSoftTimerDelayMs(lpdata->procdata.data8.data0,MagShortOVT);
    SoftTimerBlockAndOpt(lpdata->procdata.data8.data0 ,BlcId_Hal,SftOpt_Repeat|SftOpt_Lock);
    SetSoftTimerEvt(lpdata->procdata.data8.data0, Sig_Magnet_OVTM);
    StartSoftTimer(lpdata->procdata.data8.data0);  
    // 等待 Magnet 2 触发
    ThdWait1of2Sig(lpdata,evt,Sig_Magnet_OVTM,Sig_Magnet2_On);
    if(CheckEvt(evt,Sig_Magnet_OVTM))
        goto StopOverTime ;
        
    // start set time , shark led 4 times 
    ResetSoftTimer(lpdata->procdata.data8.data0,500);
    led1on();
    led2on();
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1off();
    led2off(); 
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1on();
    led2on();

    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1off();
    led2off();    
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1on();
    led2off();
    // start the over time soft time.    
    ResetSoftTimer(lpdata->procdata.data8.data0,MagLongOVT);

    do
    {
        ThdWait1of3Sig(lpdata,evt,Sig_Magnet_OVTM,Sig_Magnet1_On,Sig_Magnet2_On);
        if(CheckEvt(evt,Sig_Magnet_OVTM))
            goto StopOverTime ;
        ResetSoftTimer(lpdata->procdata.data8.data0,RESTARTDELAY);
        
        if(CheckEvt(evt,Sig_Magnet1_On))
        {// Sig_Magnet1_On
            goto MinSetStep ;
        }   
        led2revers();
        // Sig_Magnet2_ON 
        if(lpdata->procdata.data8.data1 == 0xff)
            lpdata->procdata.data8.data1 = 0 ;
        else
        {
            lpdata->procdata.data8.data1 ++ ;
            if(lpdata->procdata.data8.data1 == 24 )
                lpdata->procdata.data8.data1 = 0 ;
        }
            
        
    }while(1);
    
    
 MinSetStep :
    
    // start set time , shark led 4 times 
    ResetSoftTimer(lpdata->procdata.data8.data0,500);
    led1on();
    led2on();
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1off();
    led2off(); 
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1on();
    led2on();

    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1off();
    led2off();
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1on();
    led2off();
    // start the over time soft time.    
    ResetSoftTimer(lpdata->procdata.data8.data0,MagLongOVT);

    do
    {
        ThdWait1of3Sig(lpdata,evt,Sig_Magnet_OVTM,Sig_Magnet1_On,Sig_Magnet2_On);
        if(CheckEvt(evt,Sig_Magnet_OVTM))
            goto StopOverTime ;
        ResetSoftTimer(lpdata->procdata.data8.data0,RESTARTDELAY);
        
        if(CheckEvt(evt,Sig_Magnet1_On))
        {// Sig_Magnet1_On
            goto SecSetStep ;
        }   
        led2revers();
        // Sig_Magnet2_ON 
        if(lpdata->procdata.data8.data2 == 0xff)
            lpdata->procdata.data8.data2 = 0 ;
        else
        {
            lpdata->procdata.data8.data2 ++ ;
            if(lpdata->procdata.data8.data2 == 59 )
                lpdata->procdata.data8.data2 = 0 ;
        }
    }while(1);
    
 SecSetStep :    
    // start set time , shark led 4 times 
    ResetSoftTimer(lpdata->procdata.data8.data0,500);
    led1on();
    led2on();
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1off();
    led2off(); 
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1on();
    led2on();

    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1off();
    led2off();    
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1on();
    led2off();
    // start the over time soft time.    
    ResetSoftTimer(lpdata->procdata.data8.data0,MagLongOVT);

    do
    {
        ThdWait1of3Sig(lpdata,evt,Sig_Magnet_OVTM,Sig_Magnet1_On,Sig_Magnet2_On);
        if(CheckEvt(evt,Sig_Magnet_OVTM))
            goto StopOverTime ;
        ResetSoftTimer(lpdata->procdata.data8.data0,RESTARTDELAY);
        
        if(CheckEvt(evt,Sig_Magnet1_On))
        {// Sig_Magnet1_On
            goto ShowTimeStep ;
        }   
        led2revers();
        // Sig_Magnet2_ON 
        if(lpdata->procdata.data8.data3 == 0xff)
            lpdata->procdata.data8.data3 = 0 ;
        else
        {
            lpdata->procdata.data8.data3 ++ ;
            if(lpdata->procdata.data8.data3 == 59 )
                lpdata->procdata.data8.data3 = 0 ;
        }
    }while(1);   

 ShowTimeStep :    
    // set the changed time to the Rtc 
    if((lpdata->procdata.data32 & 0xffffff) == 0xffffff)
         goto StopOverTime ;
    NVIC_DisableIRQ(RTC_IRQn);
    RTC->ISR = 0xff ;
    CLEAR_BIT(RTC->IER, RTC_IER_ALARM_IE_Msk|RTC_IER_4HZ_IE_Msk);
    RTC->WER       = 0xACACACACU;   
    if(lpdata->procdata.data8.data1 != 0xff)
        RTC->BCDSEC = Int2Bcd(lpdata->procdata.data8.data1) ;
    if(lpdata->procdata.data8.data2 != 0xff)
        RTC->BCDMIN = Int2Bcd(lpdata->procdata.data8.data2)  ;
    if(lpdata->procdata.data8.data3 != 0xff)
        RTC->BCDHOUR= Int2Bcd(lpdata->procdata.data8.data3)  ;
    //start rtc 
    RTC->WER       = 0;  
    NVIC_EnableIRQ(RTC_IRQn);    
    RestartTimeAlarm();
    // shark led .
    ResetSoftTimer(lpdata->procdata.data8.data0,500);
    led1off();
    led2on();
    

    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1on();
    led2off(); 
    

    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1off();
    led2on();
    

    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    led1on();
    led2off();    
    
    // 清理过程结束，系统恢复到未进行调用的初始状态， 再次使用前必须先进行一次调用，以便完成初始化。
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    
 StopOverTime :
    ReleaseSoftTimer(lpdata->procdata.data8.data0);   
    led1off();
    led2off();
      
    ThdEnd(lpdata);    
}





