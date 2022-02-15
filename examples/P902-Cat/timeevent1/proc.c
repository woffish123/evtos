#include "proc.h"


//��̬�̣߳�1�� ��̬�߳� ������ĳһ��block�£����ø�block���¼��������ƣ�������̬�̵߳����У�
//          2�� ��̬�߳� ʹ����һ��LongProcData ��Ϊ�����ڲ����ݵĴ洢�㣬 ���߳̽���ʱ�ͷš� ���������⣬��̬�̲߳���ʹ����������
//
//  �����Ҫע�� 1 : ��̬�߳̿��Խ��յ����¼������޵ģ� �����������Խ��ܵ����¼��ķ�Χѡ����ʵ�Block�� ���߽���̬�߳���Ϊһ��������Block
//          ע����ʱҪ���¼��ķ��ͷ�����֪���¼��Ľ��ܷ���˭�� �������ǱȽ����ѵ�.
//          2 ���������һ���������¼�����������̬�̣߳� ����¼����ھ�̬�߳�δ����ʱ��Ч�� ������Ч��

// this function will do a test motor2. 
// it control the motor2 loop from stop to forward speed up , then speed down, then backword speed up , then , brake, go to forward speed uo then  goto backward speed up.
// start with Sig_Motor2_Test . end report  Sig_MotorProc1_Done
uint8_t MotorProc0(StdEvt evt,LPLongProcData lpdata)
{
    ThdBeginSig(lpdata,evt,Sig_Motor2_Test);
    // ��ʼ����� �� ��������������ʽ��ʼǰ��ϵͳ����һ�ε����Ա�ﵽ��ʽ�ȴ���״̬��
    lpdata->procdata.data32 = 1 ;
    // ��ʼ���������� ��ʼ�ȴ���һ��״̬�� �����ǳ�����ʽ��ʼִ�еĲ��֣� ����Ӧ���źŵ���ʱ��ִ�С�

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
    // �����������ù��̣��������� ��  
    postevt((LPThdBlock)(&cmddealer),Sig_MotorProc0_Done);    
    // ������̽�����ϵͳ�ָ���δ���е��õĳ�ʼ״̬�� �ٴ�ʹ��ǰ�����Ƚ���һ�ε��ã��Ա���ɳ�ʼ����
    ThdEnd(lpdata);
}


// this function will do a test motor2. 
// it control the motor2 loop from stop to forward speed up , then speed down, then backword speed up , then , brake, go to forward speed uo then  goto backward speed up.
// start with Sig_Motor2_Test . end report  Sig_MotorProc1_Done
uint8_t MotorProc1(StdEvt evt,LPLongProcData lpdata)
{
    ThdBeginSig(lpdata,evt,Sig_Motor1_Test);
    // ��ʼ����� �� ��������������ʽ��ʼǰ��ϵͳ����һ�ε����Ա�ﵽ��ʽ�ȴ���״̬��
    lpdata->procdata.data32 = 1 ;
    // ��ʼ���������� ��ʼ�ȴ���һ��״̬�� �����ǳ�����ʽ��ʼִ�еĲ��֣� ����Ӧ���źŵ���ʱ��ִ�С�

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
    // �����������ù��̣��������� ��  
    postevt((LPThdBlock)(&cmddealer),Sig_MotorProc1_Done);    
    // ������̽�����ϵͳ�ָ���δ���е��õĳ�ʼ״̬�� �ٴ�ʹ��ǰ�����Ƚ���һ�ε��ã��Ա���ɳ�ʼ����
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

    // ����Sig_Magnet_DBOV ��ʱ���
    lpdata->procdata.data8.data0 = GetFreeSoftTimer();
    SetSoftTimerDelayMs(lpdata->procdata.data8.data0,MagShortOVT);
    SoftTimerBlockAndRepeat(lpdata->procdata.data8.data0 ,BlcId_Hal,SftOpt_Repeat);
    SetSoftTimerEvt(lpdata->procdata.data8.data0, Sig_Magnet_OVTM);
    StartSoftTimer(lpdata->procdata.data8.data0);  
    // �ȴ� Magnet 2 ����
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
    
    // ������̽�����ϵͳ�ָ���δ���е��õĳ�ʼ״̬�� �ٴ�ʹ��ǰ�����Ƚ���һ�ε��ã��Ա���ɳ�ʼ����
    
    ThdWaitSig(lpdata,evt,Sig_Magnet_OVTM);
    
 StopOverTime :
    StopSoftTimer(lpdata->procdata.data8.data0);   
    led1off();
    led2off();
      
    ThdEnd(lpdata);    
}




// end long proc.
