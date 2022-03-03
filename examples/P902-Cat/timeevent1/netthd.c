#include "netthd.h"
#include "thd.h"
#include "hal.h"
#include "softtimer.h"
#include "sx1262.h"
#include "maclayer.h"
// process Net : used to dealer the RF event . 
// it receive the RF message from higher layer , and do the send and receive of RF message .
volatile NetMaster netmaster ;
StdEvt NetMasterEvtLoop[Max_NetMasterEvt];
static LPLongProc  const NetMasterProcArray[]=
{
    RfRegisterProc,
    RfCheckUndirectMsgProc,
    RfGetRtcTime   ,
    0,
};
// we need to use big buffer for rf work.


// net layer part  -----------------------------------------------
void NetMasterInit(void)
{
#if Nodetype == RootNode       
    uint16_t  temp ;
#endif    
    SetThdOption((LPThdBlock)(&netmaster),ThdBlockOption_HardWareLock);
    rflayerdataInit();
#if Nodetype == RootNode      
        RfPowerOn();
        // delay 10ms . allow the lora power up  .
        for(temp = 0 ; temp < 0x555; temp ++)
            __NOP();
        LoraInit();  
        StartRfMonitor(INFINITE_CAD);
#else    
    RfPowerOff();
#endif    
    SetProcAddr((LPThdBlock)(&netmaster) ,(LPLongProc const *)NetMasterProcArray);
}


void    NetMaster_proc(LPThdBlock const lpb, StdEvt evt)
{
    uint8_t * lpTx ;

    uint16_t evtid = getevtid(evt);
    // longproc calling cmd , will be deal here 
    switch(evtid)
    {
        case  Sig_Rf_Recv_Data :
        {// receive a lower data .
            outlog(log_rf_recv_data);
            MacRecvProc(getpoolid(evt));
            break;
        }
        case  Sig_Rtc_FixHourInt :
        { // every hour ,send this , to sub active cnt 
            if(SubLocalLiveCnt())
            {
                if(GetRfMode() < rf_gettime) 
                {
                    AddActiveProcById(BlcId_Net ,RfRegisterProcId);
                }
                else if(GetRfMode() <= rf_monitor)
                    AddActiveProcById(BlcId_Net ,RfGetRtcTimeId);
#if    Nodetype ==  LeafNode                
                else
                    AddActiveProcById(BlcId_Net ,RfCheckUndirectMsgProcId);
#endif                
            }
            return ;
        }
        case  Sig_Rf_DayOver :
        { // send by rtc day alarm . no buffer with it .
          // evert day to sub all child or lead live count ,  to remove the node lose connectting a defined day count .  
            SubChildLiveCnt();

            if(SubFatherLiveCnt())
            { // the father has long time lose connected with local . 
              // start register again, but not clear the local data.
                AddActiveProcById(BlcId_Net ,RfRegisterProcId);
            }
            else if(CheckRtcFreeDay())
            {
                AddActiveProcById(BlcId_Net ,RfGetRtcTimeId);
            }
            return ;
        }
        case     Sig_Rf_Recv_Msg :
        {  // send by rf proc  .  recv a rf msg .
            // rf buffer
            evtid = getpoolid(evt);
            //lpTx = (uint8_t * )getrfbuf(evtid);  
            // do something .
            
            // send a uart msg .
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);   
            lpTx[0] = 'R' ;
            lpTx[1] = 'F' ;
            lpTx[2] = ' ' ;
            lpTx[3] = 'R' ;
            lpTx[4] = 'e' ;
            lpTx[5] = 'c' ;
            lpTx[6] = 'v' ;
            lpTx[7] = ' ' ;
            lpTx[8] =  (uint8_t)evtid;
            lpTx[9] =  (uint8_t)(evtid>>8)  ;
            lpTx[10] = 's' ;
            lpTx[11] = 'g' ;   
            LpUart0Send(evt);
            freerfbuf(evtid);    
            return ;                
        }

        case     Sig_Rf_Overtime :
        { // send by rf proc  , rf monitor times overed.  rf stop work , go to idle state. 
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);   
            lpTx[0] = 'R' ;
            lpTx[1] = 'F' ;
            lpTx[2] = ' ' ;
            lpTx[3] = 'S' ;
            lpTx[4] = 't' ;
            lpTx[5] = 'o' ;
            lpTx[6] = 'p' ;
            lpTx[7] = ' ' ;
            lpTx[8] = 'm' ;
            lpTx[9] = 'n'  ;
            lpTx[10] = 't' ;
            lpTx[11] = 'r' ;   
            LpUart0Send(evt); 
            return ;                
        }        
        default :
            break;        
    }        
}
#define  ProcLoopCnt  lpdata->procdata.data8.data0 
#define  ProcSftIndex lpdata->procdata.data8.data1
#define  ProcLcTemp1   lpdata->procdata.data8.data2
#define  ProcLcTemp2   lpdata->procdata.data8.data3
uint8_t RfRegisterProc (StdEvt evt,uint8_t dataindex,LPLongProcData lpdata)
{
    // Notice the local param is used only between two ThdWait block .
    uint16_t temp ;
    LPFrameCtrl lpbuf ; 
    ThdBegin(lpdata);
    // lpdata->procdata is valid for the while proc time .
    // alloc a timer for this static proc .
    ProcSftIndex = GetFreeSoftTimer();
    SoftTimerBlockAndOpt(ProcSftIndex ,BlcId_Net,SftOpt_Lock);
    SetSoftTimerEvt(ProcSftIndex, Sig_Rf_Rgst_Delay);    
    
    if(MODE_POWERDOWN == GetRfState())
    {
        RfPowerOn();
        // delay 10ms . allow the lora power up  .
        SetSoftTimerDelayMs(ProcSftIndex,10); 
        StartSoftTimer(ProcSftIndex);  
        ThdWaitSig(lpdata,dataindex,Sig_Rf_Rgst_Delay);
        // do rf initial .
        LoraInit();
        outlog(log_rf_coldup);  
    }
    if(rflayerdata.rfmode > rf_monitor)
        goto StartCheckOldInfor;

    
    
    ProcLoopCnt = Max_Rgst_Cnt ; //store the loop times  and also decide the delay between next try.
    StartRfMonitor(INFINITE_CAD);   
   

    //reset the inter data to invalid data .
    rflayerdata.rfmode =  rf_init; 
    rflayerdata.rtcfreeday = 0 ;
    globaldata.cfgdata.NearTreeNode1 = OrphanLeafAddr ;
    globaldata.cfgdata.NearTreeNode2 = OrphanLeafAddr ;
    globaldata.cfgdata.NearTreeRssi1 = 0xff ;
    globaldata.cfgdata.NearTreeRssi2 = 0xff ;     
StartSendBoradcast:        
    outlog(log_rf_register); 
    // make a broad cast msg .
    evt = newrfbuf(globaldata.cfgdata.LocalAddr);    
    lpbuf  = (LPFrameCtrl)getrfbuf(evt) ;
    lpbuf->MsgCnt = (sizeof(FrameCtrl) -2) ;
    lpbuf->FrameBit = maccmd_broadcast  | DirectionMask ;
    lpbuf->NetId =  globaldata.cfgdata.NetId ;    
    // send to a unknow node , the send to addr is no used,
    SendRfMsgDirect(evt,globaldata.cfgdata.LocalAddr) ;

    // start a wait timer for 4s to get the node reply .    
    SetSoftTimerDelayMs(ProcSftIndex,Rgst_Delay_Base * (Max_Rgst_Cnt +1 -ProcLoopCnt)); 
    StartSoftTimer(ProcSftIndex);  
     
  
    ThdWaitSig(lpdata,dataindex,Sig_Rf_Rgst_Delay);
   
    ProcLcTemp1 = 0xff ;
    // check if we find broad cast msg . the NearTreeNode1/2 will be set .
    if(globaldata.cfgdata.NearTreeRssi1 != 0xff)
    {
        if(globaldata.cfgdata.NearTreeRssi1 > globaldata.cfgdata.NearTreeRssi2)
        { // use 1 to send broadcast reply.
            ProcLcTemp1 = 2 ;
            globaldata.cfgdata.NearTreeRssi2 = 0xff ;
        }
        else 
        {
            ProcLcTemp1 = 1 ;
            globaldata.cfgdata.NearTreeRssi1 = 0xff ;
        }
    }
    else
    {
        if(globaldata.cfgdata.NearTreeRssi2 != 0xff)
        {
            ProcLcTemp1 = 2 ;
            globaldata.cfgdata.NearTreeRssi2 = 0xff ;        
        }
    }
    if(ProcLcTemp1 == 0xff) // not find.
    {
        outlog(log_rf_regist_no_reply);
        ProcLoopCnt -- ;
        if(ProcLoopCnt > 0)
            goto StartSendBoradcast ;
        else
            goto StopRegister ;
    }
    // finished receive nearby tree node broadcast .  trying to register .
    ProcLoopCnt =4 ;
    rflayerdata.rfmode =  rf_register; 
StartCheckOldInfor:     

    // have find a useable tree node , it's addr is temp .
    // send jion cmd to register .  if register ok . we will receive a Sig_Rf_Register_OK evt . to stop register . else restart register.
    // make a join msg .
    evt = newrfbuf(temp);    
    lpbuf  = (LPFrameCtrl)getrfbuf(evt)  ;
    lpbuf->MsgCnt = (sizeof(FrameCtrl) -2) ;
    lpbuf->NetId =  globaldata.cfgdata.NetId ;  
    if(ProcLcTemp1 ==1 )
        lpbuf->VarAddr = globaldata.cfgdata.NearTreeNode1 ;
    else
        lpbuf->VarAddr = globaldata.cfgdata.NearTreeNode2 ;     
    lpbuf->FrameBit = maccmd_join  | DirectionMask ;
    SendRfMsgDirect(evt,temp) ;
    outlog(log_rf_jioning);
    //wait 2s to get result
    // start a wait timer for 4s to get the node reply .    
    SetSoftTimerDelayMs(ProcSftIndex,2000); 
    StartSoftTimer(ProcSftIndex);      
    
    ThdWait1of2Sig(lpdata,dataindex,Sig_Rf_Rgst_Delay,Sig_Rf_Register_OK);
    if(CheckEvt(evt,Sig_Rf_Rgst_Delay))
    { // faild ,retry
        outlog(log_rf_jionfail);
        ProcLoopCnt -- ;
        if(ProcLoopCnt > 0)
            goto StartCheckOldInfor ;
        else
            goto StopRegister ;
    }
    else
    {
        StopSoftTimer(ProcSftIndex);
    }      
    // register ok . try to get rtc time .
    rflayerdata.rfmode =  rf_gettime; 
    ProcLoopCnt  = 3 ;
StartSendGetTime:     
    // send a get rtc time 
    temp = getfatheraddr();
    evt = newrfbuf(temp);    
    lpbuf  = (LPFrameCtrl)getrfbuf(evt) ;
    lpbuf->MsgCnt = RtcMsgSize ;
    lpbuf->FrameBit = maccmd_time  ;
    lpbuf->NetId =  globaldata.cfgdata.NetId ; 
    lpbuf->VarAddr= temp;    
    SendRfMsgDirect(evt,temp) ;

    outlog(log_rf_asktime);
    SetSoftTimerDelayMs(ProcSftIndex,1500); 
    StartSoftTimer(ProcSftIndex);  
    
    ThdWait1of2Sig(lpdata,dataindex,Sig_Rf_Rgst_Delay,Sig_Rf_GetTime_OK);
    if(CheckEvt(evt,Sig_Rf_Rgst_Delay))
    { // faild ,retry
        ProcLoopCnt -- ;
        if(ProcLoopCnt > 0)
            goto StartSendGetTime ;
    }    
    else
    {
        outlog(log_rf_asktimefail);
        ReleaseSoftTimer(ProcSftIndex);
    }  
StopRegister:   
    
#if  Nodetype  ==    LeafNode    
    // for leaf node : set rf to standby mode . to reduce the power .  and send a Sig_Rf_Overtime msg . 
    // user can RfPowerOff(); to shutdown the rf power .
    rflayerdata.rfmode =  rf_idle; 
    StopRfMonitor();
    postevt((LPThdBlock)&(netmaster.super),Sig_Rf_Overtime);
#else
    rflayerdata.rfmode =  rf_monitor;     
#endif    
//    DelActiveProc((LPThdBlock)(&netmaster),RfRegisterProcId);
    ThdEnd(lpdata);    
}
// send maccmd_checkundirect to get undirect msg .
uint8_t RfCheckUndirectMsgProc (StdEvt evt,uint8_t dataindex,LPLongProcData lpdata)
{
    // Notice the local param is used only between two ThdWait block .
    uint16_t temp ;
    LPFrameCtrl lpbuf ; 
    ThdBegin(lpdata);
    // check the if the net has finished register and gettime .
    if(rflayerdata.rfmode <  rf_gettime) 
        goto StopProc ;
    // lpdata->procdata is valid for the while proc time .
    ProcLoopCnt = 0x2 ; //store the loop times  and also decide the delay between next try.
    ProcSftIndex = GetFreeSoftTimer();
    SoftTimerBlockAndOpt(ProcSftIndex ,BlcId_Net,SftOpt_Lock);
    SetSoftTimerEvt(ProcSftIndex, Sig_Rf_Rgst_Delay);    
    
    if(MODE_POWERDOWN == GetRfState())
    {
        RfPowerOn();
        // delay 10ms . allow the lora power up  .
        SetSoftTimerDelayMs(ProcSftIndex,10); 
        StartSoftTimer(ProcSftIndex);  
        ThdWaitSig(lpdata,dataindex,Sig_Rf_Rgst_Delay);
        // do rf initial .
        LoraInit();
    }
    if(IsRfStop())
    {
        OnStartRf();
        StartRfMonitor(INFINITE_CAD);    
    }
StartCheckUndirect:
    // make a check undirect msg .
    temp = getfatheraddr();
    evt = newrfbuf(temp);    
    lpbuf  = (LPFrameCtrl)getrfbuf(evt);
    lpbuf->MsgCnt = (sizeof(FrameCtrl) -2) ;
    lpbuf->FrameBit = maccmd_checkundirect  | DirectionMask ;
    lpbuf->NetId =  globaldata.cfgdata.NetId ;    
    SendRfMsgDirect(evt,temp) ;

    // start a wait timer for 1.5s to get the node reply .    
    SetSoftTimerDelayMs(ProcSftIndex,1500); 
    StartSoftTimer(ProcSftIndex);  
  
    ThdWait1of2Sig(lpdata,dataindex,Sig_Rf_Rgst_Delay,Sig_Rf_CheckUndirect_OK);
    if(CheckEvt(evt,Sig_Rf_Rgst_Delay))
    { // faild ,retry
        ProcLoopCnt -- ;
        if(ProcLoopCnt > 0)
            goto StartCheckUndirect  ;
    }
    
    ReleaseSoftTimer(ProcSftIndex);
   
#if  Nodetype  ==    LeafNode    
    // for leaf node : set rf to standby mode . to reduce the power .  and send a Sig_Rf_Overtime msg . 
    // user can RfPowerOff(); to shutdown the rf power .
    StopRfMonitor();
    OnStopRf();
    postevt((LPThdBlock)&(netmaster.super),Sig_Rf_Overtime);
#endif 
StopProc :    
//    DelActiveProc((LPThdBlock)(&netmaster),RfCheckUndirectMsgProcId);
    __NOP();
    ThdEnd(lpdata);   
}

// Open the RF ， Send Sig_Get_Time event ,try to get time  . continue for about 3 times , inter time is 10s .
uint8_t  RfGetRtcTime(StdEvt evt,uint8_t dataindex,LPLongProcData lpdata)
{
    uint16_t temp ;
    LPFrameCtrl lpbuf ; // Notice the local param is used only between two ThdWait block .
    // rf process do not allow proc running  together .
    ThdBegin(lpdata);
 
    ProcSftIndex = GetFreeSoftTimer();
    SoftTimerBlockAndOpt(ProcSftIndex ,BlcId_Net,SftOpt_Lock);
    SetSoftTimerEvt(ProcSftIndex, Sig_Rf_Rgst_Delay);    
    // 初始化语句 ， 下列语句必须在正式开始前被系统进行一次调用以便达到正式等待的状态。
    if(rflayerdata.rfmode <  rf_register) 
        goto StopProc ;
    // 初始化语句结束， 开始等待第一个状态， 这里是程序正式开始执行的部分， 在相应的信号到来时被执行。
    ProcLoopCnt = 0x2 ; //store the loop times  and also decide the delay between next try.
    if(MODE_POWERDOWN == GetRfState())
    {
        RfPowerOn();
        // delay 10ms . allow the lora power up  .
        SetSoftTimerDelayMs(ProcSftIndex,10);
        StartSoftTimer(ProcSftIndex);  
    
        ThdWaitSig(lpdata,dataindex,Sig_Rf_Rgst_Delay);
        // do initial .
        LoraInit();
    }
     
    if(IsRfStop())
    {
        OnStartRf();
        StartRfMonitor(INFINITE_CAD);    
    }  
StartGetRtcTime:    
    // prepare send cmd  . 
    temp = getfatheraddr();
    evt = newrfbuf(temp);    
    lpbuf  = (LPFrameCtrl)getrfbuf(evt) ;
    lpbuf->VarAddr = temp;    
    lpbuf->MsgCnt = RtcMsgSize ;
    lpbuf->FrameBit = maccmd_time ;
    lpbuf->NetId =  globaldata.cfgdata.NetId ; 
    SendRfMsgDirect(evt,temp) ;

    // delay 1.5s .
    SetSoftTimerDelayMs(ProcSftIndex,1500); //
    StartSoftTimer(ProcSftIndex);  
    ThdWait1of2Sig(lpdata,dataindex,Sig_Rf_Rgst_Delay,Sig_Rf_GetTime_OK);
    if(CheckEvt(evt,Sig_Rf_Rgst_Delay))
    { // faild ,retry
        ProcLoopCnt -- ;
        if(ProcLoopCnt > 0)
            goto StartGetRtcTime ;
    }

    ReleaseSoftTimer(ProcSftIndex);
 #if  Nodetype  ==    LeafNode    
    // for leaf node : set rf to standby mode . to reduce the power .  and send a Sig_Rf_Overtime msg . 
    // user can RfPowerOff(); to shutdown the rf power .
    StopRfMonitor();
    OnStopRf();
    postevt((LPThdBlock)&(netmaster.super),Sig_Rf_Overtime);
#endif 
 StopProc :
    // 结束整个调用过程，进行清理活动 。  
//    DelActiveProc((LPThdBlock)(&netmaster),RfGetRtcTimeId);
    //add a nop for goto no code warnning 
    __NOP();
    ThdEnd(lpdata);
}
// mac layer end ----------------------------------------------------
