#include "proc.h"


//静态线程，1： 静态线程 挂载在某一个block下，借用该block的事件驱动机制，驱动静态线程的运行，
//          2： 静态线程 使用了一个LongProcData 作为他的内部数据的存储点， 当线程结束时释放。 除此数据外，静态线程不能使用其他变量
//
//  因此需要注意 1 : 静态线程可以接收到的事件是有限的， 必须根据其可以接受到的事件的范围选择合适的Block， 或者将静态线程作为一个单独的Block
//          注意这时要求，事件的发送方必须知道事件的接受方是谁。 这往往是比较困难的.
//          2 ：必须设计一个单独的事件用于启动静态线程， 这个事件仅在静态线程未启动时有效， 否则无效。

// this function will do a test motor2. 
// it control the motor2 loop from stop to forward speed up , then speed down, then backword speed up , then , brake, go to forward speed uo then  goto backward speed up.
// start with Sig_Motor2_Test . end report  Sig_MotorProc1_Done
uint8_t MotorProc0(StdEvt evt,LPLongProcData lpdata)
{
    ThdBeginSig(lpdata,evt,Sig_Motor2_Test);
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
    ThdBeginSig(lpdata,evt,Sig_Motor1_Test);
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
    ThdBeginSig(lpdata,evt,Sig_Magnet1_On);
    // init time value .
    lpdata->procdata.data32 = 0xffffffff ;

    // 启动Sig_Magnet_DBOV 超时检查
    lpdata->procdata.data8.data0 = GetFreeSoftTimer();
    SetSoftTimerDelayMs(lpdata->procdata.data8.data0,MagShortOVT);
    SoftTimerBlockAndRepeat(lpdata->procdata.data8.data0 ,BlcId_Hal,SftOpt_Repeat);
    SetSoftTimerEvt(lpdata->procdata.data8.data0, Sig_Magnet_OVTM);
    StartSoftTimer(lpdata->procdata.data8.data0);  
    // 等待 Magnet 2 触发
    ThdWait1of2Sig(lpdata,evt,Sig_Magnet_OVTM,Sig_Magnet2_On);
    if(ThdIsCurSig(lpdata,0))
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
        if(ThdIsCurSig(lpdata,0))
            goto StopOverTime ;
        ResetSoftTimer(lpdata->procdata.data8.data0,RESTARTDELAY);
        
        if(ThdIsCurSig(lpdata,1))
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
        if(ThdIsCurSig(lpdata,0))
            goto StopOverTime ;
        ResetSoftTimer(lpdata->procdata.data8.data0,RESTARTDELAY);
        
        if(ThdIsCurSig(lpdata,1))
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
        if(ThdIsCurSig(lpdata,0))
            goto StopOverTime ;
        ResetSoftTimer(lpdata->procdata.data8.data0,RESTARTDELAY);
        
        if(ThdIsCurSig(lpdata,1))
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
    StopSoftTimer(lpdata->procdata.data8.data0);   
    led1off();
    led2off();
      
    ThdEnd(lpdata);    
}




// end long proc.
