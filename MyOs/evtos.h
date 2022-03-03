#ifndef  EVNT_OS_H
#define  EVNT_OS_H
// defer the static thread 
#include "basictype.h"
#include "thd.h"
#include "logbyte.h"

// define the return value  .
#define ThdProc_NotFind  0
#define ThdProc_Find     1
#define ThdProc_Exit     2 

// the static thread init .  should  called before static thread start  .
#define ThdInit(lpdata)   (lpdata)->state16= 0 

// if start the thread not need wait a sig
#define ThdBegin(lpdata) {  switch((lpdata)->state16) { case 0:  


// return 2    
#define ThdEnd(lpdata)   } (lpdata)->state16 =0; return ThdProc_Exit ; }  
// 不等待任何值 
#define ThdWait(lpdata)   LC_RETSET(lpdata->state16)     

#if StaticProcModeQuick != 1 
    #define LC_RESUME(s,evt,sig) switch(s) { case 0: if(((evt)^(sig))&EvtSigMask ) { return ThdProc_NotFind;}   
    // if  start thread with waitting a sig 
    #define ThdBeginSig(lpdata,evt,sig) {  LC_RESUME((lpdata)->state16,evt,sig) 
    // if  start thread with waitting a specail cndition .  
    #define ThdBeginCon(lpdata,condi) {  switch((lpdata)->state16) { case 0:  if(!condi) { return ThdProc_NotFind;}
    
    #define LC_RETSET(s) s = __LINE__; return ThdProc_Find; case __LINE__:

// 等待一个非信号的条件完成 
#define ThdWaitCon(lpdata,condition)	        \
  do {						\
         LC_RETSET(lpdata->state16);				\
         if(!(condition) ) { return ThdProc_NotFind;}        \
     } while(0)
#else 
#define LC_RETSET(s) s = __LINE__; return ThdProc_NotFind; case __LINE__:
#endif    

  
     
// thread block hold the tread current state ,the thread event pool .
// it can be used as the base struct to extent to user needed 
typedef struct LongProcData_
{
    uint8_t  procid ; // the local long proc index  start from 0.  Inter Used
    uint8_t  state8 ; // for the long proc used to deal multiple Signal wait .  Inter Used
    uint16_t state16; // for the long proc used to as the jump address.  Inter Used
    Var32    procdata; // hold the local temp data .  User Use。
    
}LongProcData, * LPLongProcData;




#if  StaticProcModeQuick == 1   
// help function for static proc mode 
// insert a new evt to static proc m after id
// input  : dataindex : lpdata index , sig : the sig waiting for . 
// Notice  : the sig will be add at reverse order . 
void  addprocsig(uint8_t dataindex,uint16_t sig) ;

// use the state8  store the event wait cnt , the event is stored in evtarray .
    #define ThdWaitSig(lpdata,index,sig)	        \
      do {						\
             (lpdata)->state8 = 1 ;                     \
             addprocsig(index,sig) ;               \
             LC_RETSET((lpdata)->state16);				\
         } while(0)

    #define ThdWait1of2Sig(lpdata,index,sig1,sig2)	        \
      do {						\
             lpdata->state8 = 1 ;                       \
             addprocsig(index,sig2) ;               \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16);				\
         } while(0)     
         
    #define ThdWait1of3Sig(lpdata,index,sig1,sig2,sig3)	        \
      do {						\
             lpdata->state8 = 1 ;                       \
             addprocsig(index,sig3) ;               \
             addprocsig(index,sig2) ;               \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16);				\
         } while(0)         

    #define ThdWait1of4Sig(lpdata,index,sig1,sig2,sig3,sig4)	        \
      do {						\
             lpdata->state8 = 1 ;                       \
             addprocsig(index,sig4) ;               \
             addprocsig(index,sig3) ;               \
             addprocsig(index,sig2) ;               \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16)	;			\
         } while(0)    

     #define ThdWait1of5Sig(lpdata,index,sig1,sig2,sig3,sig4,sig5)	        \
      do {						\
             lpdata->state8 = 1 ;                       \
             addprocsig(index,sig5) ;             \
             addprocsig(index,sig4) ;               \
             addprocsig(index,sig3) ;             \
             addprocsig(index,sig2) ;               \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16)	;			\
         } while(0)   

     #define ThdWait1of6Sig(lpdata,index,sig1,sig2,sig3,sig4,sig5,sig6)	        \
      do {						\
             lpdata->state8 = 1 ;                       \
             addprocsig(index,sig6) ;               \
             addprocsig(index,sig5) ;            \
             addprocsig(index,sig4) ;              \
             addprocsig(index,sig3) ;              \
             addprocsig(index,sig2) ;               \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16)	;			\
         } while(0)   

     #define ThdWait1of7Sig(lpdata,index,sig1,sig2,sig3,sig4,sig5,sig6,sig7)	        \
      do {						\
             lpdata->state8 = 1 ;                       \
             addprocsig(index,sig7) ;              \
             addprocsig(index,sig6) ;               \
             addprocsig(index,sig5) ;               \
             addprocsig(index,sig4) ;              \
             addprocsig(index,sig3) ;               \
             addprocsig(index,sig2) ;               \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16)	;			\
         } while(0)   

      #define ThdWait1of8Sig(lpdata,index,sig1,sig2,sig3,sig4,sig5,sig6,sig7,sig8)	        \
      do {						\
             lpdata->state8 = 1 ;                       \
             addprocsig(index,sig8) ;              \
             addprocsig(index,sig7) ;              \
             addprocsig(index,sig6) ;               \
             addprocsig(index,sig5) ;               \
             addprocsig(index,sig4) ;              \
             addprocsig(index,sig3) ;               \
             addprocsig(index,sig2) ;               \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16);				\
         } while(0)   
        
    // 用于同时等待多个(最大8个)具体消息的到达 。  注意 最后到达的事件 不清楚是哪一个事件， 之前到达的事情已经被清零 。
    // 因此 所等待的事情 必须为简单事件， 不能包含内存块，否则 这些内存块 没有被释放会产生内存泄漏 。     
    // 例如需要等待 Sig1 和Sig2  同时 ， 则需要设计为 ThdWaitAll2Sig(lpb,sig1 ,Sig2)   
      
    #define ThdWaitAll2Sig(lpdata,index,sig1,sig2)	        \
      do {						\
             lpdata->state8 = 2 ;                       \
             addprocsig(index,sig1) ;                \
             addprocsig(index,sig2) ;                \
             LC_RETSET(lpdata->state16);				\
         } while(0)     
         
    #define ThdWaitAll3Sig(lpdata,index,sig1,sig2,sig3)	        \
      do {						\
             lpdata->state8 = 3 ;                       \
             addprocsig(index,sig3) ;                \
             addprocsig(index,sig2) ;                \
             addprocsig(index,sig1) ;                \
             LC_RETSET(lpdata->state16);				\
         } while(0)         

    #define ThdWaitAll4Sig(lpdata,index,sig1,sig2,sig3,sig4)	        \
      do {						\
             lpdata->state8 = 4 ;                       \
             addprocsig(index,sig4) ;                \
             addprocsig(index,sig3) ;                \
             addprocsig(index,sig2) ;                \
             addprocsig(index,sig1) ;                \
             LC_RETSET(lpdata->state16);				\
         } while(0)    

     #define ThdWaitAll5Sig(lpdata,index,sig1,sig2,sig3,sig4,sig5)	        \
      do {						\
             lpdata->state8 = 5 ;                       \
             addprocsig(index,sig5) ;             \
             addprocsig(index,sig4) ;                \
             addprocsig(index,sig3) ;              \
             addprocsig(index,sig2) ;               \
             addprocsig(index,sig1) ;                \
             LC_RETSET(lpdata->state16);				\
         } while(0)   

     #define ThdWaitAll6Sig(lpdata,index,sig1,sig2,sig3,sig4,sig5,sig6)	        \
      do {						\
             lpdata->state8 = 6 ;                       \
             addprocsig(index,sig6) ;                \
             addprocsig(index,sig5) ;           \
             addprocsig(index,sig4) ;             \
             addprocsig(index,sig3) ;             \
             addprocsig(index,sig2) ;             \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16);				\
         } while(0)   

     #define ThdWaitAll7Sig(lpdata,index,sig1,sig2,sig3,sig4,sig5,sig6,sig7)	        \
      do {						\
             lpdata->state8 = 7 ;                       \
             addprocsig(index,sig7) ;                \
             addprocsig(index,sig6) ;               \
             addprocsig(index,sig5) ;              \
             addprocsig(index,sig4) ;               \
             addprocsig(index,sig3) ;               \
             addprocsig(index,sig2) ;              \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16);				\
         } while(0)   

      #define ThdWaitAll8Sig(lpdata,index,sig1,sig2,sig3,sig4,sig5,sig6,sig7,sig8)	        \
      do {						\
             lpdata->state8 = 8 ;                       \
             addprocsig(index,sig8) ;              \
             addprocsig(index,sig7) ;               \
             addprocsig(index,sig6) ;               \
             addprocsig(index,sig5) ;              \
             addprocsig(index,sig4) ;              \
             addprocsig(index,sig3) ;               \
             addprocsig(index,sig2) ;                \
             addprocsig(index,sig1) ;               \
             LC_RETSET(lpdata->state16);				\
         } while(0)         
         
#else
    //用于等待下一个具体消息的到达。
    #define ThdWaitSig(lpdata,evt,sig)	        \
      do {						\
             LC_RETSET((lpdata)->state16)				\
             if(( (evt)&EvtSigMask ) !=(sig))  { return 0;}         \
         } while(0)


    #define ThdWait1of2Sig(lpdata,evt,sig1,sig2)	        \
      do {						\
             LC_RETSET((lpdata)->state16)				\
             if((((evt)&EvtSigMask)!=(sig1))  && (((evt)&EvtSigMask)!=(sig2))) \
                 {return 0;} \
         } while(0)    
         
    #define ThdWait1of3Sig(lpdata,evt,sig1,sig2,sig3)	        \
      do {						\
             LC_RETSET((lpdata)->state16)				\
             if((((evt)&EvtSigMask)!=(sig1))  && (((evt)&EvtSigMask)!=(sig2))&& \
                (((evt)&EvtSigMask)!=(sig3))   ) \
                 {return 0;} \
         } while(0)      

    #define ThdWait1of4Sig(lpdata,evt,sig1,sig2,sig3,sig4)	        \
      do {						\
             LC_RETSET((lpdata)->state16)				\
             if((((evt)&EvtSigMask)!=(sig1))  && (((evt)&EvtSigMask)!=(sig2)) &&\
                (((evt)&EvtSigMask)!=(sig3))  && (((evt)&EvtSigMask)!=(sig4))) \
                 {return 0;} \
         } while(0)    

     #define ThdWait1of5Sig(lpdata,evt,sig1,sig2,sig3,sig4,sig5)	        \
      do {						\
             LC_RETSET((lpdata)->state16)				\
             if((((evt)&EvtSigMask)!=(sig1))  && (((evt)&EvtSigMask)!=(sig2)) &&\
                (((evt)&EvtSigMask)!=(sig3))  && (((evt)&EvtSigMask)!=(sig4)) &&\
                (((evt)&EvtSigMask)!=(sig5))   ) \
                 {return 0;} \
         } while(0)     

     #define ThdWait1of6Sig(lpdata,evt,sig1,sig2,sig3,sig4,sig5,sig6)	        \
      do {						\
             LC_RETSET((lpdata)->state16)				\
             if((((evt)&EvtSigMask)!=(sig1))  && (((evt)&EvtSigMask)!=(sig2)) &&\
                (((evt)&EvtSigMask)!=(sig3))  && (((evt)&EvtSigMask)!=(sig4)) &&\
                (((evt)&EvtSigMask)!=(sig5))  && (((evt)&EvtSigMask)!=(sig6))) \
                 {return 0;} \
         } while(0)  

     #define ThdWait1of7Sig(lpdata,evt,sig1,sig2,sig3,sig4,sig5,sig6,sig7)	        \
      do {						\
             LC_RETSET((lpdata)->state16)				\
             if((((evt)&EvtSigMask)!=(sig1))  && (((evt)&EvtSigMask)!=(sig2)) &&\
                (((evt)&EvtSigMask)!=(sig3))  && (((evt)&EvtSigMask)!=(sig4)) &&\
                (((evt)&EvtSigMask)!=(sig5))  && (((evt)&EvtSigMask)!=(sig6)) &&\
                (((evt)&EvtSigMask)!=(sig7))  )  \
                 {return 0;} \
         } while(0)  

      #define ThdWait1of8Sig(lpdata,evt,sig1,sig2,sig3,sig4,sig5,sig6,sig7,sig8)	        \
      do {						\
             LC_RETSET((lpdata)->state16)				\
             if((((evt)&EvtSigMask)!=(sig1))  && (((evt)&EvtSigMask)!=(sig2)) &&\
                (((evt)&EvtSigMask)!=(sig3))  && (((evt)&EvtSigMask)!=(sig4)) &&\
                (((evt)&EvtSigMask)!=(sig5))  && (((evt)&EvtSigMask)!=(sig6)) &&\
                (((evt)&EvtSigMask)!=(sig7))  && (((evt)&EvtSigMask)!=(sig7)))  \
                 {return 0;} \
         } while(0) 
        
    // 用于同时等待多个(最大8个)具体消息的到达 。  注意 最后到达的事件 不清楚是哪一个事件， 之前到达的事情已经被清零 。
    // 因此 所等待的事情 必须为简单事件， 不能包含内存块，否则 这些内存块 没有被释放会产生内存泄漏 。     
    // 例如需要等待 Sig1 和Sig2  同时 ， 则需要设计为 ThdWait2Sig(lpb,sig1 ,Sig2)   
      
    #define ThdWaitAll2Sig(lpdata,evt,sig1,sig2)	        \
      do {						\
             lpdata->state8 = 3 ;                       \
             LC_RETSET(lpdata->state16)				\
             if((((evt)&EvtSigMask)==(sig1))  ) { (lpdata)->state8 &=0xfe ;}\
             else if((((evt)&EvtSigMask)==(sig2))  ) { (lpdata)->state8 &=0xfd ;}\
             else { return 0;} \
             if((lpdata)->state8){ return 1 ;}\
         } while(0)

    // wait 3 sig together
    #define ThdWaitAll3Sig(lpdata,evt,sig1,sig2,sig3)	        \
      do {						\
             lpdata->state8 = 7 ;                       \
             LC_RETSET(lpdata->state16)				\
             if((((evt)&EvtSigMask)==(sig1))  ) { (lpdata)->state8 &=0xfe ;}\
             else if((((evt)&EvtSigMask)==(sig2))  ) { (lpdata)->state8 &=0xfd ;}\
             else if((((evt)&EvtSigMask)==(sig3))  ) { (lpdata)->state8 &=0xfb ;}\
             else { return 0;} \
             if((lpdata)->state8){ return 1 ;}\
         } while(0)
         
         
    // wait 4 sig together
    #define ThdWaitAll4Sig(lpdata,evt,sig1,sig2,sig3,sig4)	        \
      do {						\
             lpdata->state8 = 0x0f ;                       \
             LC_RETSET(lpdata->state16)				\
             if((((evt)&EvtSigMask)==(sig1))  ) { (lpdata)->state8 &=0xfe ;}\
             else if((((evt)&EvtSigMask)==(sig2))  ) { (lpdata)->state8 &=0xfd ;}\
             else if((((evt)&EvtSigMask)==(sig3))  ) { (lpdata)->state8 &=0xfb ;}\
             else if((((evt)&EvtSigMask)==(sig4))  ) { (lpdata)->state8 &=0xf7 ;}\
             else { return 0;} \
             if((lpdata)->state8){ return 1 ;}\
         } while(0)   

    // wait 5 sig together
    #define ThdWaitAll5Sig(lpdata,evt,sig1,sig2,sig3,sig4,sig5)	        \
      do {						\
             lpdata->state8 = 0x1f ;                       \
             LC_RETSET(lpdata->state16)				\
             if((((evt)&EvtSigMask)==(sig1))  ) { (lpdata)->state8 &=0xfe ;}\
             else if((((evt)&EvtSigMask)==(sig2))  ) { (lpdata)->state8 &=0xfd ;}\
             else if((((evt)&EvtSigMask)==(sig3))  ) { (lpdata)->state8 &=0xfb ;}\
             else if((((evt)&EvtSigMask)==(sig4))  ) { (lpdata)->state8 &=0xf7 ;}\
             else if((((evt)&EvtSigMask)==(sig5))  ) { (lpdata)->state8 &=0xef ;}\
             else { return 0;} \
             if((lpdata)->state8){ return 1 ;}\
         } while(0)   
     
    // wait 6 sig together
    #define ThdWaitAll6Sig(lpdata,evt,sig1,sig2,sig3,sig4,sig5,sig6)	        \
      do {						\
             lpdata->state8 = 0x3f ;                       \
             LC_RETSET(lpdata->state16)				\
             if((((evt)&EvtSigMask)==(sig1))  ) { (lpdata)->state8 &=0xfe ;}\
             else if((((evt)&EvtSigMask)==(sig2))  ) { (lpdata)->state8 &=0xfd ;}\
             else if((((evt)&EvtSigMask)==(sig3))  ) { (lpdata)->state8 &=0xfb ;}\
             else if((((evt)&EvtSigMask)==(sig4))  ) { (lpdata)->state8 &=0xf7 ;}\
             else if((((evt)&EvtSigMask)==(sig5))  ) { (lpdata)->state8 &=0xef ;}\
             else if((((evt)&EvtSigMask)==(sig6))  ) { (lpdata)->state8 &=0xdf ;}\
             else { return 0;} \
             if((lpdata)->state8){ return 1 ;}\
         } while(0)       

    // wait 7 sig together
    #define ThdWaitAll7Sig(lpdata,evt,sig1,sig2,sig3,sig4,sig5,sig6,sig7)	        \
      do {						\
             lpdata->state8 = 0x7f ;                       \
             LC_RETSET(lpdata->state16)				\
             if((((evt)&EvtSigMask)==(sig1))  ) { (lpdata)->state8 &=0xfe ;}\
             else if((((evt)&EvtSigMask)==(sig2))  ) { (lpdata)->state8 &=0xfd ;}\
             else if((((evt)&EvtSigMask)==(sig3))  ) { (lpdata)->state8 &=0xfb ;}\
             else if((((evt)&EvtSigMask)==(sig4))  ) { (lpdata)->state8 &=0xf7 ;}\
             else if((((evt)&EvtSigMask)==(sig5))  ) { (lpdata)->state8 &=0xef ;}\
             else if((((evt)&EvtSigMask)==(sig6))  ) { (lpdata)->state8 &=0xdf ;}\
             else if((((evt)&EvtSigMask)==(sig7))  ) { (lpdata)->state8 &=0xbf ;}\
             else { return 0;} \
             if((lpdata)->state8){ return 1 ;}\
         } while(0)       
    // wait 8 sig together
    #define ThdWaitAll8Sig(lpdata,evt,sig1,sig2,sig3,sig4,sig5,sig6,sig7)	        \
      do {						\
             lpdata->state8 = 0xff ;                       \
             LC_RETSET(lpdata->state16)				\
             if((((evt)&EvtSigMask)==(sig1))  ) { (lpdata)->state8 &=0xfe ;}\
             else if((((evt)&EvtSigMask)==(sig2))  ) { (lpdata)->state8 &=0xfd ;}\
             else if((((evt)&EvtSigMask)==(sig3))  ) { (lpdata)->state8 &=0xfb ;}\
             else if((((evt)&EvtSigMask)==(sig4))  ) { (lpdata)->state8 &=0xf7 ;}\
             else if((((evt)&EvtSigMask)==(sig5))  ) { (lpdata)->state8 &=0xef ;}\
             else if((((evt)&EvtSigMask)==(sig6))  ) { (lpdata)->state8 &=0xdf ;}\
             else if((((evt)&EvtSigMask)==(sig7))  ) { (lpdata)->state8 &=0xbf ;}\
             else if((((evt)&EvtSigMask)==(sig8))  ) { (lpdata)->state8 &=0x7f ;}\
             else { return 0;} \
             if((lpdata)->state8){ return 1 ;}\
         } while(0)  
         
#endif

// 定义了一个基于Queue的消息队列，用于缓存消息事件 ，  for  Use_Big_Evt max 256*256 item can hold , else  255 time can hold 
typedef struct StdPool_
{
    StdEvt * lppool ;     // point to the pool array .
    PoolCntType  headindex ;  // index to the first valid event .
    PoolCntType  endindex ;   // index to the end valid event .
    PoolCntType  evtcnt   ;   // the total valid event count .
	PoolCntType  maxcnt   ;   // the total event count of the pool
}StdPool ;


//  function to access a StdEvt Queue ,  used to store a series of StdEvt (uint16_t) in Queue mode  .
typedef  StdPool  StdQueue ;
// lpQueue :  the Queue handle , should be define static .
// uint32_t :the buffer address, should be define static .
// size  :  the buffer size , if #ifndef  Use_Big_Evt  size is 2 time of the event count ,else size is same as the event count it can hold .
void  InitQueue(StdQueue * lpQueue ,uint32_t *lp, PoolCntType size) ;
StdEvt GetQueue(StdQueue * lpQueue) ; // out the first event , return 0 if there is no event  , 
StdEvt LookQueue(StdQueue * lpQueue) ; //look first event  dont out it , return 0 if there is no event 
uint8_t SetQueue(StdQueue * lpQueue,StdEvt evt) ; //set a new evt to last position. return the current event count 
void ResetQueue(StdQueue * lpQueue) ; // set item in Queue to zero     
uint8_t GetQueueCnt(StdQueue * lpQueue) ; // get item counti in queue     .
     
// get the mem with in the evt.
uint32_t * getevtmem(StdEvt evt);


// how many pool will be used .  pool must be a 32bit algined! --> item in pool should be n*4byte !! 
// else use the pool item to hold struct maybe failed.
// and the max pool item totally should not bigger than 127 .


// add a mem pool to frame . return a pool id  it is 0 1,2  
// lp: startaddr , itemcnt :dived into itemcnt item. every item has itemsize*4 byte.
     
uint8_t   AddPool(uint32_t * lp , PoolCntType itemcnt,uint8_t itemsize);

// release memory pool item to frame pool
void   freeevt(StdEvt evt);
// alloc a memory pool item from frame pool and set the event as sig .
StdEvt newevt(uint16_t sig,uint8_t poolid );






// define the commonn MAX Long Proc counter 
// there are two value  can be chioced  32 or 64 .  

// if Use_Max_Proc ==1  max 32 longprocdata can be used . else max 64 longprocdata can be used .
// this will limit the max longproc counter wen can used at same time .
// you can define max 255 longproc for every Thread . but only MAX_LONGPROC_CNT longproc can be used at same time for the whole programe .


#if Use_Max_Proc != 1 
#define MAX_LONGPROC_CNT  32
#else
#define MAX_LONGPROC_CNT  64
#endif 
// pre define .
typedef struct  ThdBlock_  * LPThdBlock ;
// define the LongProc function pointer  
// return 1 means the event has been done .  
// if current func is  activeproc and return 0  than call the default proc .
typedef uint8_t  (* LPLongProc)(StdEvt evt,uint8_t dataindex ,LPLongProcData lpdata) ; // 32bit.
// define the default event deal function , normally all event will be deal at this function .
typedef void  (* LPInitFuc)(LPThdBlock lpblock,StdEvt evt) ; 

// follwing is same resource lock bit . it means some resource is not allowed to be shared between static proc .
// the real meanning of the lock bit is different in process.
#define ThdBlockOption_Lock1   BIT0  
#define ThdBlockOption_Lock2   BIT1  
#define ThdBlockOption_Lock3   BIT2  
#define ThdBlockOption_Lock4   BIT3  
#define ThdBlockOption_Lock5   BIT4
#define ThdBlockOption_Lock6   BIT5  
#define ThdBlockOption_Lock_Mask  0x3f
// do not allow any other longproc running , AddActiveProc will check if there is a any proc id is running , if so it will abandon the add .
#define ThdBlockOption_HardWareLock   BIT6
// do not allow  duplicate longproc running , AddActiveProc will check if there is a same proc id is running , if so it will abandon the add .
#define ThdBlockOption_NotSameProc        BIT7 

typedef struct ThdBlock_
{
    LPInitFuc   lpinit ;    //4byte. thread's  default deal function pointer 
    LPLongProc * fuc ;      //4byte. thread's  LongProc 数组指针头  .  it hold the all longproc pointer 
    StdPool   pool;         //8byte. thread's  event Queue .   
    
    uint32_t  procbit ;     //low 32bit to mark which common proc data is used by this thread .   the common proc data used by all block.
#if Use_Max_Proc == 1 
    uint32_t  prochbit ;    // high 32bit to mark which common proc data used .  the common proc data used by all block.
#endif     
    
    
    uint8_t   proccnt;      // temp value , for local use . recorder the item count of fuc array  actived in this block.
    uint8_t   index;        // index in the actor array. 
    uint8_t   option ;      // specail option bit  as ThdBlockOption_xxx
    uint8_t   res ;  // no used
} ThdBlock, *LPThdBlock;


// this should be call at init proc . only set can't be changed
__STATIC_INLINE void SetThdOption(LPThdBlock lpthd , uint8_t opt)
{
    lpthd->option |= opt ;
}
__STATIC_INLINE void SetThdLock(LPThdBlock lpthd , uint8_t lock)
{
    lpthd->option &= (~lock) ;
}

__STATIC_INLINE uint8_t IsNotThdLocked(LPThdBlock lpthd , uint8_t lock)
{// is 1 means lock not used , is 0 means lock is locked.
    return lpthd->option & lock ? 0 : 1 ;
}

__STATIC_INLINE void ClrThdLock(LPThdBlock lpthd , uint8_t lock)
{
    lpthd->option |= lock;
}
// help function for debug.
uint8_t freepoolcnt(uint8_t id) ;
uint16_t getevtmemlength(StdEvt evt);

// 对于一个 Thread  Ctrol Block 其包含了一个 LongProc的指针 fuc ， 一个32/64位的Bitmap变量 procbit ， 以及proccnt ， 这两个变量标识了 当前
// Block 占用了 32个公共LongDataProcData中的那几个， 以及数量是几 。 
// 当Block的消息循环调用时 首先会检查 procbit 是否为空， 如果不为空则， 找到对应的公共LongDataProcData， 调用当前Block的
// 第 procid 个proc 函数进行调用。 因此 对于一个Block 可以有255个 静态过程， 但整个系统同一时刻只能有32/64 个Proc在运行。
// 设置Block当前允许运行的LongProc，Evt为发给Block的初始事件 ，可选 注意 LongProc实际一旦允许会收到当前Block的所有消息，其必须对消息进行过滤，只处理其所关注的消息。

// thdid : the thdblock id ,  procid  : the local static proc id ,    evt : the initail active  evt ,   it is used for the static proc to start when receive  specail evt 
uint8_t  AddActiveProcById(BlockId thdid,uint8_t procid) ;
uint8_t  AddActiveProc(LPThdBlock lpb,uint8_t procid) ;
// 取消某一个LongProc， 这个通常由LongProc在其结束时自行调用。 
//void  DelActiveProcById(BlockId thdid,uint8_t procid);
//void  DelActiveProc(LPThdBlock lpb,uint8_t procid) ;


// 设置Block的Longproc函数组地址，addr为第一个LongProc的地址 这个地址指向一个静态的指针数组， 整个系统中最大允许有32个LongProc同时运行 ，因为系统分配的
// Proc变量LongProcData 为32个， 这32个数据为所有Block共用。
// 单个Block的最大允许函数数量为255 （LongProcData.procid）， 但同时允许运行的数量因LongProcData 为32个而限定为32个。

__STATIC_INLINE  void SetProcAddr(LPThdBlock lpb,  LPLongProc const *  addr)
{
    lpb->fuc = (LPLongProc * ) addr ;
}





//init the frame  should be call first .
void ThdFrameInit(void);

// init the thread struct , add it to the frame,  the thread has not been call. 
// this fuc just put a sig_none event to the shd pool . wait the frame to call it .
// the first call ThdActorInit actor as lowest priority , the last has highest priority.
void ThdActorInit(LPThdBlock lpb , LPStdEvt pool,uint8_t poolcnt, LPInitFuc lpinit);


// post a evt to a actor . this fuc will discard the event if there is no room. and it will wake up the actor sleeped.
void  postevt(LPThdBlock lpb,StdEvt evt);
void  postevtbyindex(uint8_t index ,StdEvt evt) ;

// insert a evet to a actor at the first position, it will discard the last posted event if there is no room 
void insertevt(LPThdBlock lpb,StdEvt evt) ;
void insertevtbyindex(uint8_t index ,StdEvt evt) ;

// replace a special evt to Sig_None  from a block  .
void removeevt(LPThdBlock lpb,StdEvt evt) ;

// the frame check the all actor event pool one by one .untill the pool is empty,every time  if there no event for all actor , frame return 0 else return 1
// it will check the high priority task first .
void ThdRun(void);

#if _ASSERT  == 1
    void assert_(char * file,uint16_t id, uint8_t b) ;
    #define assert(b)  assert_(__FILE__ ,(uint16_t)(__LINE__),(uint8_t)(b))
    void outlog(LogByte byte) ;
    LogByte getlog(void) ;
    //  return current stack free size , and set free size specail data
    uint32_t stacksetmark(void) ;
    //  return current stack free size ,  used after stack_set_guard 
    uint32_t stackcheckmark(void);

#else
	#define assert(b)    ((void)0)
    #define outlog(b) ((void)0)
    #define stacksetmark()   ((void)0)
    #define stackcheckmark()   ((void)0)
    
#endif	


#endif
