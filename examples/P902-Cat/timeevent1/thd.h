#ifndef __thd_header_H 
#define __thd_header_H 
#include "softtimer.h"
// blockid  the block index which the evt will be post to . it is not bigger than 0x3f == SftOpt_Mask (defined in softtimer.h)
typedef enum BlockId_
{
    BlcId_Hal = 0 ,        // block id for hal proc  
    BlcId_Net,
    BlcId_CmdPort ,   // block id for cmd port 
    BlcId_CmdDealer,   // block id for CmdDealer .
    BlcId_None = SftOpt_Mask   // used for quick proc . not send to block     
}  BlockId ;





void InitFrame(void);




#endif
