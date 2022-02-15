#ifndef __net_Thread_H 
#define __net_Thread_H 

#include "evtos.h"



//  the sx1262 proc has a directsendqueue .  a  undirectsendqueue ,and a waitreplyqueue .
//  Notice :  the low 16bit of Evt is used to store the the msg short addr , used it to mark a childer
//  directsendqueue :  when a msg need to sendout , it's hold in this queue first untill it is send out 
//                     once send out , if it need ack it will be stored in waitackqueue
//  undirectsendqueue : store the msg only send when there is a ask msg received .
//  waitreplyqueue  : store the msg need ack/reply , but not been received , it will be send again when the rf proc in idle .

// the max evt count ,net thread can hold .
#define Max_NetMasterEvt   8 

// we define a lora lock , to not allow the resource been share by different proc 
#define RfHardWareLock   ThdBlockOption_Lock1

typedef struct NetMaster_
{
    ThdBlock super ; 
    // the  common evt buffer , for directsendqueue, undirectsendqueue,and waitreplyqueue
 
} NetMaster ;

extern volatile NetMaster netmaster ;
extern StdEvt NetMasterEvtLoop[Max_NetMasterEvt];
void    NetMaster_proc(LPThdBlock const lpb, StdEvt evt);


// init the netmaster
void NetMasterInit(void);


// for orphan node to start register , if it registered ok, it will try to get the rtc time  .
// the rtc time is dealed by default mac proc . we just need to send a rtc cmd out .
uint8_t RfRegisterProc (StdEvt evt,LPLongProcData lpdata);

// try to send a mac checkundirect cmd to father , to get the possible undirect cmd .
// this is called when the child actived and no data to send to father . -> it must send something to  father , to check undirect msg and also notify  it's still lived 
// if the child send a data msg to father or root , the undirect msg is send back also .
uint8_t RfCheckUndirectMsgProc (StdEvt evt,LPLongProcData lpdata);

uint8_t RfGetRtcTime(StdEvt evt,LPLongProcData lpdata);


typedef enum NetMasterProcId_
{
    RfRegisterProcId = 0 ,
    RfCheckUndirectMsgProcId,
    RfGetRtcTimeId ,

} NetMasterProcId ;
static LPLongProc  const NetMasterProcArray[]=
{
    RfRegisterProc,
    RfCheckUndirectMsgProc,
    RfGetRtcTime   ,
    0,
};

#endif // __net_layer_H
