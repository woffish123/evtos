#ifndef __dealer_thd_H 
#define __dealer_thd_H 
#include "hal.h"
#include "evtos.h"
#include "softtimer.h"


typedef struct CmdDealer_
{
    ThdBlock super ;
  
} CmdDealer ;
// init the CmdDealer
void CmdDealerInit(void);

// max event counter for CmdDealer thread message entry , it is only used to hold the message   the real buffer size is not this 
#define Max_CmdDealerEvt   12
extern volatile CmdDealer cmddealer ;
extern StdEvt CmdDealerEvtLoop[Max_CmdDealerEvt];
void CmdDealer_initial(LPThdBlock const lpb, StdEvt evt);


#define MagShortOVT       2000       // 2s for magnet double click over time 
#define MagLongOVT        6000      // 6s over time 


uint8_t MotorProc0(StdEvt evt,LPLongProcData lpdata);
uint8_t MotorProc1(StdEvt evt,LPLongProcData lpdata);
uint8_t MagnetSetTimeProc(StdEvt evt,LPLongProcData lpdata);


// define Id for Hal Proc  for simply using  .
typedef enum DealerProcId_
{
    MotorProc0Id = 0 ,
    MotorProc1Id,
    MagnetSetTimeProcId
} HalProcId ;



#endif // __mac_layer_H
