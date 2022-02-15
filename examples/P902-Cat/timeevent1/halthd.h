#ifndef __halthd_H 
#define __halthd_H 
#include "hal.h"
#include "evtos.h"
#include "softtimer.h"


// Hal used to deal the hal event and single step cmd event.
typedef struct HalMaster_
{
    ThdBlock super ;
    // the  rtc control  block is timectrl , it will be create  outside.
} HalMaster ;
// init the CmdDealer 
void HalMasterInit(void);
// max event counter for hal thread message entry , it is only used to hold the message   the real buffer size is not this 
#define Max_HalMasterEvt   16
extern volatile HalMaster halmaster ;
extern StdEvt HalMasterEvtLoop[Max_HalMasterEvt];
void HalMaster_initial(LPThdBlock const lpb, StdEvt evt);
#endif // __mac_layer_H
