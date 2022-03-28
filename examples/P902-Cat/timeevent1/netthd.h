#ifndef __net_Thread_H 
#define __net_Thread_H 

#include "evtos.h"



//  the sx1262 proc has a recv queue .  a  onlinesendqueue ,and a offline replyqueue .
//  Notice :  the low 16bit of Evt is used to store the the msg short addr , used it to mark a childer
//  recv queue :  when a msg revced , it's hold in this queue , and a msg recv event is send if recv queue is empty
//  online queue : store the msg only send  to tree node , -> it do not need to send at once.
//  offline queue  : store the msg to leaf node , it should be send out when recv a get offline cmd , at once .

// the max evt count ,net thread can hold .
#define Max_NetMasterEvt   8 
// max try to register times , register proc will send register  Max_Rgst_Cnt times , then give up  report register fiald.

#define Max_Rgst_Cnt  10 
// interval period for register proc send ,  for n register times  it delay  n*Rgst_Delay_Base ms 
#define Rgst_Delay_Base   4000   
typedef struct NetMaster_
{
    ThdBlock super ; 
    // the  common evt buffer , for recvsendqueue, onlinesendqueue,and offline queue
 
} NetMaster ;

extern volatile NetMaster netmaster ;
extern StdEvt NetMasterEvtLoop[Max_NetMasterEvt];
void    NetMaster_proc(LPThdBlock const lpb, StdEvt evt);


// init the netmaster
void NetMasterInit(void);


// for orphan node to start register , if it registered ok, it will try to get the rtc time  .
// the rtc time is dealed by default mac proc . we just need to send a rtc cmd out .
uint8_t RfRegisterProc (StdEvt evt,uint8_t dataindex,LPLongProcData lpdata);

// try to send a mac checkonline cmd to father , to get the possible online cmd .
// this is called when the child actived and no data to send to father . -> it must send something to  father , to check online msg and also notify  it's still lived 
// if the child send a data msg to father or root , the online msg is send back also .
uint8_t RfCheckOfflineMsgProc (StdEvt evt,uint8_t dataindex,LPLongProcData lpdata);

uint8_t RfGetRtcTime(StdEvt evt,uint8_t dataindex,LPLongProcData lpdata);


typedef enum NetMasterProcId_
{
    RfRegisterProcId = 0 ,
    RfCheckOfflineMsgProcId,
    RfGetRtcTimeId ,

} NetMasterProcId ;


#endif // __net_layer_H
