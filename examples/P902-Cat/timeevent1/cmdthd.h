#ifndef __CmdPort_H 
#define __CmdPort_H 
#include "hal.h"
#include "evtos.h"
#include "softtimer.h"

// control the lpuart send and recv , rf send and recv ,timealarm recv.
typedef struct CmdPort_
{
    ThdBlock super ;
    // output the recv and send buffer to sub proc .
//    uint8_t * lpuartrecv; // store the recv buffer start address for uart0
//    uint8_t * lpuartsend; // store the send buffer start address for uart0
    uint8_t * lplpuartrecv; // store the recv buffer start address for lpuart0
    uint8_t * lplpuartsend; // store the send buffer start address for lpuart0    

//    PoolCntType uartrecvid;  // store the recv uart buf id , used by uart0
    PoolCntType lpuartrecvid;  // store the recv uart buf id , used by uart0
    
    uint8_t lpuartsendindex ;  // current lpuart 0  buffer  send  index .
    uint8_t lpuartsendcnt   ;  // current lpuart 0  buffer  cnt .
    
    uint8_t lpuartrecvindex ;  // receive cmd  is fixed cnt : LPUARTBUFLEN

    
} CmdPort ;
// init the CmdPort struct.
// CmdPort is used to recv the big cmd ,it will have multi step to finish .
void CmdPortInit(void) ;

// max event buffer count for Cmd port thread message entry , it is only used to hold the message   the real buffer size is not this  
#define Max_CmdPortEvt     20
extern volatile  CmdPort   cmdport ;
extern StdEvt CmdPortEvtLoop[Max_CmdPortEvt];
void CmdPort_initial(LPThdBlock const lpb, StdEvt evt);


#endif // __mac_layer_H
