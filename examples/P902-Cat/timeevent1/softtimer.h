#ifndef COMMON_SOFTTIMER_H
#define COMMON_SOFTTIMER_H
#include "basictype.h"
#include "hal.h"
// Use the LPTIME as the driver to make a soft timer 
#ifndef SoftTimerCnt
#define SoftTimerCnt   2
#endif 
// the usable softtimer bit option 
#define SftOpt_Repeat     0x80   // if set the timer will auto restart when overtime. else it will be remove from soft timer array.
#define SftOpt_Quick      0x40   // is set the timer will not post the event , and the event will be used as a param. 
                                 //user must add the deal function with parm in function SftQuickProc
#define SftOpt_Lock       0x20   // if set the softtimer will not be return to free list when time over , it will be lock for a static proc for next use quickly set
                                 // user must call ReleaseSoftTimer to release it .
#define SftOpt_Mask       0x1f   // use high three bit as softtime option bit .  the other is used to hold the block index .

// if SftOpt_Quick is set  when timer overtime , this function  will be called with param to evt in the softtime interrupt




// so only quicky work should be do here .
typedef void (* LPSftQuickProc )(StdEvt  param) ;



typedef struct SoftTimer_
{
    uint32_t delay  ;      // wished delay ticks. based on  LPTIME_PREDIV
    uint32_t differ ;
    //StdEvt    evt ;         // used set  hold the event will be do .     put it to softtimectrl block to get fix byte align ,so save ram.
}SoftTimer, * LPSoftTimer ;  



    
typedef struct SoftTimeCtrl_
{
    LPSftQuickProc lpQuickProc;
    SoftTimer ctrl[SoftTimerCnt];
    StdEvt    evt[SoftTimerCnt] ;
    uint8_t   block[SoftTimerCnt];        // inter used . the block index based on 0 , when post the evt need which block to send .  the high bit is SftOpt_Repeat , SftOpt_Lock and SftOpt_Lock bit ,used to post by function: postevt(uint8_t  index ,StdEvt evt)
    uint8_t   nextid[SoftTimerCnt];       // inter used . the index  of the next softtimectrl 
    uint8_t   ccr1header;  // the index of the softtime array for  lptime ccr1 register .
    uint8_t   ccr2header;  // the index of the softtime array for  lptime ccr1 register .
    uint8_t   freeheader ; // the index of the first freesoft timer. 
    uint8_t   usedcnt ;  // hold option  . no used now. for test hold the used timer cnt.
}SoftTimeCtrl ; 

extern SoftTimeCtrl   SoftTimerCtrlBlock ;

// set the quick softtimerproc function .
// this function seted by this function was called in the soft timer interrupt proc no delay! ,if  a softertimer is call SoftTimerBlockAndOpt(uint8_t index,uint8_t block ,uint8_t option)
// and the option has SftOpt_Quick set .
void SetSoftTimerDefProc( LPSftQuickProc lpQuickProc) ;

// softtimer param is setted by following function.
__STATIC_INLINE void SetSoftTimerDelayTicks(uint8_t index,uint32_t delaycnt)   
{
    SoftTimerCtrlBlock.ctrl[index].delay = delaycnt ;
}
__STATIC_INLINE void SetSoftTimerDelayMs(uint8_t index,uint32_t delaycnt)   
{
    SoftTimerCtrlBlock.ctrl[index].delay = Ms2Tick(delaycnt) ;
}
// set the soft time option :SftOpt_Repeat ,SftOpt_Quick .  and set which Hal block will used post the evt to 
// if set the SftOpt_Quick option , the block will be no used ,
// block is the block index which the evt will be post to . it is not bigger than 0x3f.
__STATIC_INLINE void SoftTimerBlockAndOpt(uint8_t index,uint8_t block ,uint8_t option)
{
   option &= (~SftOpt_Mask) ;
   SoftTimerCtrlBlock.block[index] = block|option ;
}
__STATIC_INLINE void SetSoftTimerEvt(uint8_t index,StdEvt evt)   
{
    SoftTimerCtrlBlock.evt[index] = evt ;
}
void InitSoftTimer(void);
// return a soft timer ctrol block index,  -1 if there is no free one .

uint8_t  GetFreeSoftTimer(void) ;
// return the specail index softtimer .  set the delay and  param , the delay ticks can be calc by Ms2Tick , it chang ms to ticks
LPSoftTimer  GetSoftTimerData(uint8_t index);
// enable a special softtimer . it should be call after setting the delay, repeat_opt ,evt , block .
void StartSoftTimer(uint8_t index);
// interrupt a special running softtimer.   but not put it to free list if it opt has SftOpt_Lock
uint8_t StopSoftTimer(uint8_t index);


// interrupt a special running softtimer.   
void ReleaseSoftTimer(uint8_t index);
// interrupt a special running softtimer by evt. 
void ReleaseSoftTimerEvt(StdEvt evt);

#define RESTARTDELAY     0
// reset the specail softtimer : the delay time not changed if param :delay is RESTARTDELAY , but it will be restart from now .
// else it  change the delay time based on ms as param delay and restart it .
void ResetSoftTimer(uint8_t index,uint32_t delayms);
// reset the first softtimer it's event is evt .
void ResetSoftTimerEvt(StdEvt evt,uint32_t delay);

uint8_t GetSoftTimerCnt(void);
// example :
/*
void  test(void )
{
    uint8_t temp ;
    LPSoftTimer lpdata ;
    // get the index of a softtimer , it can be used to start and stop this timer 
    temp = GetFreeSoftTimer();
    lpdata = GetSoftTimerData(temp);
    // set delay value on ticks
    //SetSoftTimerDelayTicks(lpdata,4096) ;  
    // set delay time on ms    
    SetSoftTimerDelayMs(lpdata,60000);
    // set as a repeat timer .
    SoftTimerBlockAndOpt(lpdata ,BlcId_Hal,SftOpt_Repeat);
    // set as a single time timer
    //SoftTimerBlockAndOpt(lpdata ,BlcId_Hal,0);
    // set as a locked time for a sepcail static proc ,  that is  the time will not be released when time over .
    //SoftTimerBlockAndOpt(lpdata ,BlcId_Hal,SftOpt_Lock);
    
    // set the evt will be report , it can hold a Queue item to hold expended information .
    SetSoftTimerEvt(lpdata, Sig_Magnet1_Remove);
    StartSoftTimer(temp);
    ...
    ResetSoftTimer(temp,30000)
}
*/
#endif 
