#include "thd.h"
#include "halthd.h"
#include "netthd.h"
#include "cmdthd.h"
#include "dealerthd.h"


// define a default softtimer funtction . 
// this function is called , if  a softertimer is call SoftTimerBlockAndOpt(uint8_t index,uint8_t block ,uint8_t option)
// and the option has SftOpt_Quick set . then when time over  this function is called in the softtimer interrupt no delay
void SftQuickProc(StdEvt  param)
{
    if(param == Sig_50Hz_Duty)
    {
        GPIO_ResetOutputPin(CMP2_INN_PORT,CMP2_INN_PIN);  
    }
    if(param == Sig_Ir_OVT)
    {
        postevt((LPThdBlock)&(halmaster.super),Sig_Ir_OVT);   
        Uart0Stop();      
        DisPowerOut();  
    }
    if(param == Sig_DelayReset)
    {
        RCC->SOFTRST = 0x5C5CAABB ; 
    }    
    
    return ;
}

// init the frame before call any other function. 
// the earlyer init actor has higher piriority
void InitFrame()
{
    ThdFrameInit();
    SetSoftTimerDefProc(SftQuickProc);
    // hal has highest priority 
    ThdActorInit((LPThdBlock)&halmaster,HalMasterEvtLoop,Max_HalMasterEvt,HalMaster_initial);
    // mac has lower priority
    ThdActorInit((LPThdBlock)&netmaster,NetMasterEvtLoop,Max_NetMasterEvt,NetMaster_proc);
    // these uart lower priority
    ThdActorInit((LPThdBlock)&cmdport,CmdPortEvtLoop,Max_CmdPortEvt,CmdPort_initial);

    // big cmd has lowest priority
    ThdActorInit((LPThdBlock)&cmddealer,CmdDealerEvtLoop,Max_CmdDealerEvt,CmdDealer_initial);
}
















