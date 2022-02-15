#include "hal.h"
#include "cmdthd.h"
#include "netthd.h"
#include "dealerthd.h"
#include "halthd.h"
#include "thd.h"
#include "sx1262.h"

int32_t main(void)
{
    // init thdframe struct .  disable the interrupt untill everything is ready .
    INT_DISABLE();
    InitFrame();
    
    
    // init periph.... 
    InitPerph();
    InitLed();    
    InitKey();
    InitRtc();
    InitHall();
    // remove lora Init . it will be at shutdown mode  ,init when used it  .
    //LoraInit();
    InitDma();
    // enable the LPUART0 always .  the initlpuarto will not stop it .
    InitLpUart0();
    InitAdc();
    InitMotor();
    InitUart0();
    EnableHall1();
    EnableHall2();
    InitSoftTimer();
    
    //.. init periph end .
    
    
    // init task block.
    CmdPortInit();    
    CmdDealerInit();
    HalMasterInit();
    NetMasterInit();    
    //.. task init end .
    

    
    

    // post event, it used to  start some static proc at start time .
    
    // start a ADC to get temperature , to adjust the Rtc . 
    postevtbyindex(BlcId_Hal,Sig_Rtc_FixHourInt);
    // start a register cmd , to start Rf net register  . 
    // reset the livecnt for RF node is set as zero .  receive Sig_Rf_DayOver will start Rf Net Register proc .
    //postevtbyindex(BlcId_Net,Sig_Rf_DayOver);


    // .. pre work end  , enable interrupt.
    
    // Window dog is used to assert the system should not busy (do event not stop) for more than 250/2 ms . 
    // it means the event loop has been blocked, there must something wrong .
    //EnableWinDog();
    
    // start the message loop .

    INT_ENABLE();
    ThdRun();
}
