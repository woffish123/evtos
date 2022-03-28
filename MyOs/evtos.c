#include "evtos.h"
#include "hal.h"  // this file used the  LpUart0Send(evt) in ASSERT function .   and gotosleep function serie .


// memery pool address  declaere here .
#if (PoolCnt == 0) || (PoolCnt > 8)
#error PoolCnt should at least 1 , and not bigger than 8 ,else chang the define here .
#endif

#if PoolCnt >= 1
    #if (Pool1_Cnt ==  0) || (Pool1_Size == 0)
    #error Pool1_Cnt and Pool1_Size should not be zero 
    #else
    static uint32_t Pool1[Pool1_Cnt * Pool1_Size];
    #endif
#endif

#if PoolCnt >= 2
    #if (Pool2_Cnt ==  0) || (Pool2_Size == 0)
    #error Pool2_Cnt and Pool2_Size should not be zero 
    #else
    static uint32_t Pool2[Pool2_Cnt * Pool2_Size];
    #endif
#endif

#if PoolCnt >= 3
    #if (Pool3_Cnt ==  0) || (Pool3_Size == 0)
    #error Pool3_Cnt and Pool3_Size should not be zero 
    #else
    static uint32_t Pool3[Pool3_Cnt * Pool3_Size];
    #endif
#endif

#if PoolCnt >= 4
    #if (Pool4_Cnt ==  0) || (Pool4_Size == 0)
    #error Pool4_Cnt and Pool4_Size should not be zero 
    #else
    static uint32_t Pool4[Pool4_Cnt * Pool4_Size];
    #endif
#endif

#if PoolCnt >= 5
    #if (Pool5_Cnt ==  0) || (Pool5_Size == 0)
    #error Pool5_Cnt and Pool5_Size should not be zero 
    #else
    static uint32_t Pool5[Pool5_Cnt * Pool5_Size];
    #endif
#endif

#if PoolCnt >= 6
    #if (Pool6_Cnt ==  0) || (Pool6_Size == 0)
    #error Pool6_Cnt and Pool6_Size should not be zero 
    #else
    static uint32_t Pool6[Pool6_Cnt * Pool6_Size];
    #endif
#endif

#if PoolCnt >= 7
    #if (Pool7_Cnt ==  0) || (Pool7_Size == 0)
    #error Pool7_Cnt and Pool7_Size should not be zero 
    #else
    static uint32_t Pool7[Pool7_Cnt * Pool7_Size];
    #endif
#endif

#if PoolCnt >= 8
    #if (Pool8_Cnt ==  0) || (Pool8_Size == 0)
    #error Pool8_Cnt and Pool8_Size should not be zero 
    #else
    static uint32_t Pool8[Pool8_Cnt * Pool8_Size];
    #endif
#endif



// init a Queue ,  it store  size  StdEvt    in a loop buffer  (lp).  
// StdQueue :  StdQueue handler  that will be init  , it should be declared like   : static StdQueue  QueueName ;
// lp      :  Queue buffer , uint32_t type buffer  ,hold the real buffer to store the Queue item . 
// size    :  buffer total size based on uint32_t  . the real size of the StdEvt  is  4*size / sizeof(StdEvt);
void  InitQueue(StdQueue * lpQueue ,uint32_t *lp, PoolCntType size) 
{
    lpQueue->lppool = (StdEvt*)lp ;
    lpQueue->evtcnt = 0 ;
    lpQueue->headindex = 0 ;
    lpQueue->endindex = 0 ;
#if  Use_Big_Evt   == 0  
    lpQueue->maxcnt = size<<1 ;
#else
    lpQueue->maxcnt = size ;
#endif    
}

StdEvt GetQueue(StdQueue * lpQueue)
{
    StdEvt res ;
    assert(lpQueue!=0) ;
    if(lpQueue->evtcnt == 0)
        return 0x0 ;
    res = lpQueue->lppool[lpQueue->headindex];
    lpQueue->headindex ++ ;
    if(lpQueue->headindex == lpQueue->maxcnt)
        lpQueue->headindex = 0 ;
    lpQueue->evtcnt -- ;
    return res;
}
StdEvt LookQueue(StdQueue * lpQueue) 
{
    assert(lpQueue!=0) ;
    if(lpQueue->evtcnt == 0)
        return 0x0 ;
    return lpQueue->lppool[lpQueue->headindex];
}
uint8_t SetQueue(StdQueue * lpQueue,StdEvt evt)
{
    assert(lpQueue->evtcnt < lpQueue->maxcnt );
    lpQueue->lppool[lpQueue->endindex] = evt;
    lpQueue->endindex ++ ;
    if(lpQueue->endindex == lpQueue->maxcnt)
        lpQueue->endindex = 0 ;
    lpQueue->evtcnt ++ ;
    return lpQueue->evtcnt ;
}
uint8_t GetQueueCnt(StdQueue * lpQueue)
{
    return lpQueue->evtcnt ;
}

void ResetQueue(StdQueue * lpQueue)
{
    lpQueue->evtcnt = 0 ;
    lpQueue->headindex = 0 ;
    lpQueue->endindex = 0 ;    
}





typedef struct EvtFrame_
{
	LPThdBlock lpactor[ActorCnt+1]; // store the activor information
    // a common longproc data
    LongProcData  longprocdata[MAX_LONGPROC_CNT];
    
#if  StaticProcModeQuick == 1     
    // store the real event .
    StdEvt        sigarray[MAX_LONGPROC_CNT*StaticProcWaitEvtCnt];
    // store the next id for evtarray
    uint8_t       sigidmap[MAX_LONGPROC_CNT*StaticProcWaitEvtCnt];
    // store the first id for static proc .
    uint8_t       sigheader[MAX_LONGPROC_CNT];
#endif    

    uint32_t  longprocbit ;
#if Use_Max_Proc == 1     
    uint32_t  longprochbit ;
#endif   
    
    
#if (ActorCnt >16)    
	uint32_t  bitmap;
#elif (ActorCnt >8)    
    uint16_t  bitmap;
#else
    uint8_t   bitmap;    
#endif  

#if  StaticProcModeQuick == 1 
    uint8_t   freesigheader ;
#endif

}EvtFrame;



typedef struct PoolCtor_
{
	  uint32_t*     poolstart[PoolCnt]; // the start addr of the pool.
	  PoolCntType   poolfree[PoolCnt];  // hold the first free item id
	  PoolCntType   poolfreecnt[PoolCnt]; // hold the free item cnt .
}PoolCtor ;

// inter static variables 
static uint8_t   actorcnt;   //total thread counter  inter used 
volatile static EvtFrame  evtframe; 
// current pool cunt,
static uint8_t   poolcnt_;
static PoolCtor  poolctor;


// return the pool id  based on  0;
uint8_t  AddPool(uint32_t * lp , PoolCntType itemcnt,uint8_t itemsize)
{
	uint8_t i ;
	assert(lp!= 0 );
	assert(itemcnt < MaxPoolItemCnt);
	assert(poolcnt_ < PoolCnt );
	  
	poolctor.poolstart[poolcnt_] = lp ;  // hold lp

    // free link
    poolctor.poolfreecnt[poolcnt_] = itemcnt;
    poolctor.poolfree[poolcnt_] = 0 ;
    i = 1 ;
    while(i < itemcnt)
    {
      *lp = i ; // set to next id
      lp += itemsize ;   
      i++;
    }
    poolcnt_++;
	return poolcnt_ -1 ; 
}

// get the mem of the evt.
uint32_t * getevtmem(StdEvt evt)
{
#if  Use_Big_Evt   == 0   
	uint8_t i ;
	int8_t c ;
	uint8_t id = getpoolid(evt);
    assert(id>0);
    i = 0;
    c = id ;
    c -= PoolLen[0];
    while(c > 0)
    {
        i++;
        assert(i < poolcnt_) ;	
        id = c ;
        c-=	PoolLen[i];
    }
    
    
    // i is the pool index , id is the index in this pool based on 1.
	//return  poolctor.poolstart[i]+ poolctor.itemsize[i] * (id-1) ;
    return  poolctor.poolstart[i]+ PoolSize[i] * (id-1) ;
#else
    uint8_t     poolindex ;
    PoolCntType  poolitemindex ;
    poolitemindex = getpoolid(evt);
    assert(poolitemindex >0) ;
    poolindex = getpoolindex(evt);
    assert(poolindex < poolcnt_ );
    //return poolctor.poolstart[poolindex]+ poolctor.itemsize[poolindex] * (poolitemindex-1) ;
    return poolctor.poolstart[poolindex]+ PoolSize[poolindex] * (poolitemindex-1) ;
#endif
}

// release memory pool item to frame
void  freeevt(StdEvt evt)
{ // set the pool index of the evt to the free list .
#if  Use_Big_Evt   == 0       
    int8_t  c ;
    uint8_t i ;
    uint8_t id = getpoolid(evt);
    if (id ==0) return ;
    // get the pool index 	
    i = 0;
    c = id ;
    c-=	PoolLen[0];
    while(c > 0)
    {
        i++;
        assert(i < poolcnt_) ;	
        id = c ;
        c-=	PoolLen[i];
    }
    // i is the pool index , id is the index in this pool based on 1.	
    id -- ;
	INT_DISABLE();
    *(poolctor.poolstart[i]+ PoolSize[i] * id) =  poolctor.poolfree[i];
	poolctor.poolfree[i] = id ;
	poolctor.poolfreecnt[i] ++;
	INT_ENABLE();
#else
    uint8_t     poolindex ;
    PoolCntType  poolitemindex ;
    poolitemindex = getpoolid(evt);
    if(poolitemindex ==0) return ;
    poolindex = getpoolindex(evt);
    assert(poolindex < poolcnt_ );
    
    poolitemindex -- ;
	INT_DISABLE();
	//*(poolctor.poolstart[poolindex]+ poolctor.itemsize[poolindex] * poolitemindex) =  poolctor.poolfree[poolindex];
    *(poolctor.poolstart[poolindex]+ PoolSize[poolindex] * poolitemindex) =  poolctor.poolfree[poolindex];
	poolctor.poolfree[poolindex] = poolitemindex ;
	poolctor.poolfreecnt[poolindex] ++;
	INT_ENABLE();
#endif 
}
// get a memory pool item from frame ,store it to the sig. 
StdEvt newevt(uint16_t sig,uint8_t poolid  )
{
    StdEvt evt;
    PoolCntType id;
	// check if there is a item free in this pool.
	assert(poolctor.poolfreecnt[poolid] >0) ;
	INT_DISABLE();    
    id = poolctor.poolfree[poolid];
    poolctor.poolfree[poolid] = (PoolCntType) *(poolctor.poolstart[poolid]+ PoolSize[poolid] * id);
	poolctor.poolfreecnt[poolid]--;
	INT_ENABLE();     
#if  Use_Big_Evt   == 0      
    evt = 0 ;
    while(poolid >0)
    {
        poolid -- ;
        evt += PoolLen[poolid] ;
    }
    evt += (id+1); 
    evt <<= 9 ;
    evt |= (sig&EvtSigMask);
#else
    evt = poolid ;
    evt <<= 10 ;
    evt |=(id+1) ;
    evt <<=16 ;
    evt |= (sig&EvtSigMask) ;    
#endif    
	return evt;
}

uint8_t freepoolcnt(uint8_t id)
{
    return poolctor.poolfreecnt[id];
}
uint16_t getevtmemlength(StdEvt evt)
{
    uint8_t res ;
#if  Use_Big_Evt  == 0  
    uint8_t c ;
    c = getpoolid(evt);
    res = 0 ;
    while(c > PoolLen[res])
    {
        res++;
        c-=	PoolLen[res];		
    }
#else 
    res =getpoolindex(evt) ;
#endif    
    return PoolSize[res] ;
}
StdEvt getevt(volatile LPThdBlock lpb)
{
    StdEvt res ;
    assert(lpb!=0) ;
    assert(lpb->pool.evtcnt > 0) ;
    res = lpb->pool.lppool[lpb->pool.headindex];
    lpb->pool.headindex ++ ;
    if(lpb->pool.headindex == lpb->pool.maxcnt)
        lpb->pool.headindex = 0 ;
    lpb->pool.evtcnt -- ;
    if(lpb->pool.evtcnt == 0) 
       evtframe.bitmap &= (~(1<<lpb->index))	;
    return res;
}
// remove a special evt from a block 
void removeevt(LPThdBlock lpb,StdEvt evt)
{
    PoolCntType index ;
    assert(lpb!= 0) ;
    freeevt(evt);
    INT_DISABLE();
    index = lpb->pool.headindex;
    while(index != lpb->pool.endindex )
    {
        if(lpb->pool.lppool[index] == evt)
            lpb->pool.lppool[index] = Sig_None;
        index ++ ;
        if(index == lpb->pool.maxcnt)
            index = 0 ;
    }
    INT_ENABLE();
}

// add a event to thread Queue
void postevt(LPThdBlock lpb,StdEvt evt)
{
    assert(lpb->pool.evtcnt < lpb->pool.maxcnt) ;
    INT_DISABLE();
    lpb->pool.lppool[lpb->pool.endindex] = evt;
    lpb->pool.endindex ++ ;
    if(lpb->pool.endindex == lpb->pool.maxcnt)
        lpb->pool.endindex = 0 ;
    lpb->pool.evtcnt ++ ;
    evtframe.bitmap |= (1<<lpb->index);
    INT_ENABLE();
}
void  postevtbyindex(uint8_t index ,StdEvt evt)
{
    postevt(evtframe.lpactor[index],evt);
}

// add a event at the first position of a thread . used for the emergency message .
void insertevt(LPThdBlock lpb,StdEvt evt)
{
    // add a init evt to the first position.
    INT_DISABLE();
    if(lpb->pool.headindex == 0)
        lpb->pool.headindex = lpb->pool.maxcnt -1 ;
    else
        lpb->pool.headindex -- ;
    lpb->pool.lppool[lpb->pool.headindex] = evt;
    if(lpb->pool.evtcnt >= lpb->pool.maxcnt )
    {// overflow, addjust the end index
        if(lpb->pool.endindex == 0)    
            lpb->pool.endindex = lpb->pool.maxcnt -1 ;
        else
            lpb->pool.endindex -- ;
    }
    else
        lpb->pool.evtcnt ++ ;

    evtframe.bitmap |= (1<<lpb->index);
    INT_ENABLE();  
}
void insertevtbyindex(uint8_t index ,StdEvt evt)  
{
    insertevt(evtframe.lpactor[index],evt);
}

void ThdFrameInit(void)
{
    uint16_t i = sizeof(EvtFrame);
    uint16_t *lp = (uint16_t*)&evtframe; 	
    i>>=1 ;
    while(i)
    {
        *lp = 0 ;
        lp++;	 	
        i--;
    }
    actorcnt = 0 ;
   
#if  StaticProcModeQuick == 1    
    
    for(i = 0 ; i< (MAX_LONGPROC_CNT*StaticProcWaitEvtCnt -1) ; i++)
    {
        evtframe.sigidmap[i] = i+1 ;
    }
    evtframe.sigidmap[i] = InvalidId ;
    
    for(i = 0 ; i< MAX_LONGPROC_CNT ; i++)
    {
        evtframe.sigheader[i] = InvalidId ;
    }    
    evtframe.freesigheader = 0 ;
#endif     
  
    
    evtframe.longprocbit = 0xffffffff;
#if Use_Max_Proc == 1     
    evtframe.longprochbit = 0xffffffff;
#endif     
    i = sizeof(poolctor);
    i>>=1 ;
    lp = (uint16_t *)&poolctor;
    while(i)
    {
        *lp = 0 ;
        lp++;	 	
        i--;	 
    }
    poolcnt_ = 0 ;
// Pool1 must exsit   
#if PoolCnt >= 1
    AddPool(Pool1,Pool1_Cnt,Pool1_Size);
#endif    
#if PoolCnt >= 2
    AddPool(Pool2,Pool2_Cnt,Pool2_Size);
#endif     
#if PoolCnt >= 3
    AddPool(Pool3,Pool3_Cnt,Pool3_Size);
#endif  
#if PoolCnt >= 4
    AddPool(Pool4,Pool4_Cnt,Pool4_Size);
#endif  
#if PoolCnt >= 5
    AddPool(Pool5,Pool5_Cnt,Pool5_Size);
#endif  
#if PoolCnt >= 6
    AddPool(Pool6,Pool6_Cnt,Pool6_Size);
#endif  
#if PoolCnt >= 7
    AddPool(Pool7,Pool7_Cnt,Pool7_Size);
#endif  
#if PoolCnt >= 8
    AddPool(Pool8,Pool8_Cnt,Pool8_Size);
#endif  
    stacksetmark() ;
}

// help function for static proc 
#if  StaticProcModeQuick == 1    

// insert a new evt to static proc m after id
// dataindex : lpdata index , sig : the sig waiting for . 
void  addprocsig(uint8_t dataindex,uint16_t sig)
{
    uint8_t next ;
    assert(evtframe.freesigheader != InvalidId);
    next = evtframe.freesigheader ;
    evtframe.freesigheader = evtframe.sigidmap[next];
    evtframe.sigarray[next] = sig ;    
    evtframe.sigidmap[next] = evtframe.sigheader[dataindex] ; 
    evtframe.sigheader[dataindex] = next ;   
}

#endif

// the earlyer init actor has higher piriority
void ThdActorInit(LPThdBlock lpb , LPStdEvt lppool,uint8_t cnt, LPInitFuc lpinit)
{
    assert(lpb!= 0) ;
    assert(lppool!=0) ;
    assert(poolcnt_ !=0) ;
    assert(lpinit != 0) ;

    INT_DISABLE();
    lpb->procbit = 0 ;
#if Use_Max_Proc == 1     
    lpb->prochbit = 0;
#endif      
    lpb->proccnt = 0 ;
    lpb->option = 0 ;

    
    lpb->fuc = NULL ;
    lpb->pool.headindex = 0;
    lpb->pool.evtcnt = 0 ;
    lpb->pool.endindex = 0;
    lpb->pool.maxcnt = cnt;
    lpb->pool.lppool =lppool ;


    lpb->index = actorcnt;
    evtframe.lpactor[actorcnt] = lpb;
    //evtframe.lpinit[actorcnt] = lpinit ; 
    evtframe.lpactor[actorcnt]->lpinit = lpinit ;
    actorcnt ++ ;  
    INT_ENABLE();
}
// 增加当前Block的LongProc处理函数，并将进行一次
// 注意对于同一个Block 不允许同一个LongProc函数被同时多次激活， 否则系统无法判断 结束时应结束哪一个LongProc 
// evt为首次触发的事件， 每一个Proc 应该有一个独立的首次触发事件，以及一个结束事件。 用于使过程的激活可控。
// return  :1  faild to add active proc , 0: success.
uint8_t AddActiveProcById(BlockId thdid,uint8_t procid)
{
    assert(thdid < BlcId_None ) ;
    return AddActiveProc(evtframe.lpactor[thdid],procid);
}

uint8_t  AddActiveProc(LPThdBlock lpb,uint8_t procid) 
{
    uint32_t bit ;
    uint8_t  temp ;
    assert(lpb!=0) ;
    assert(lpb->fuc!=0) ;
    // there must has a free longproc data .
#if Use_Max_Proc != 1     
    assert(evtframe.longprocbit!=0) ;
#else      
    assert((evtframe.longprocbit!=0)|| (evtframe.longprochbit!=0)) ;
#endif
    // check if allow other active porc running 
    if(lpb->option & ThdBlockOption_HardWareLock)
    {
    #if Use_Max_Proc != 1     
        if(lpb->procbit !=0) return 1;
    #else      
        if((lpb->procbit!=0)|| (lpb->prochbit !=0))  return 1;
    #endif    
    }
    // check is allow duplicate longproc running 
    if(lpb->option & ThdBlockOption_NotSameProc)
    { //
        // find free postion .
        if(lpb->procbit !=0)
        {
            bit = 1 ;
            for(temp =0 ; temp <32 ; temp++, bit<<=1 )
            {
                if(lpb->procbit & bit) 
                {// clear the global free bit.
                    if(evtframe.longprocdata[temp].procid == procid)
                         return 1;   
                }
            }
        }
    #if Use_Max_Proc == 1      
        if(lpb->prochbit !=0)
        {// has not find 
            bit = 1 ;
            for(temp =32 ; temp <64 ; temp++, bit<<=1 )
            {
                if(lpb->prochbit & bit) 
                {// clear the global free bit.
                    if(evtframe.longprocdata[temp].procid == procid)
                         return 1;   
                }
            }
        }
    #endif    
    }

    // find free postion .
    if(evtframe.longprocbit!=0)
    {
        bit = 1 ;
        for(temp =0 ; temp <32 ; temp++, bit<<=1 )
        {
            if(evtframe.longprocbit & bit) 
            {   // clear the global free bit.
                evtframe.longprocbit &=  (~bit);
                // store free bit in local .
                lpb->procbit |= bit ;      
                lpb->proccnt ++ ;   
#if Use_Max_Proc == 1                   
                assert(lpb->proccnt <65) ;
#else
                assert(lpb->proccnt <33) ; 
#endif                
                // init the longproc data .
                evtframe.longprocdata[temp].procid =procid ;
                evtframe.longprocdata[temp].procdata.data32 = 0 ;
                evtframe.longprocdata[temp].state16 = 0 ;
                evtframe.longprocdata[temp].state8= InvalidId  ;
                break;
            }
        }
    }
#if Use_Max_Proc == 1      
    else
    {// has not find 
        bit = 1 ;
        for(temp =32 ; temp <64 ; temp++, bit<<=1 )
        {
            if(evtframe.longprochbit & bit) 
            {// clear the global free bit.
                evtframe.longprochbit &=  (~bit);
                // store free bit in local .
                lpb->prochbit |= bit ;      
                lpb->proccnt ++ ;    
                assert(lpb->proccnt <65) ;
                // init the longproc data .

                evtframe.longprocdata[temp].procid =procid ;
                evtframe.longprocdata[temp].procdata.data32 = 0 ;
                evtframe.longprocdata[temp].state16 = 0 ;
                evtframe.longprocdata[temp].state8= InvalidId   ;
                
                break;
            }
        }
    }
#endif    


    // start init part .

    (*(lpb->fuc[procid]))(Sig_None,temp,(LPLongProcData)&evtframe.longprocdata[temp]) ;
    outlog(log_proc_start);
    outlog((LogByte)procid);
    outlog((LogByte)temp) ;

    return 0 ;
}

/* --removed manually calling the DelActiveProc when proc end ,  it will be call automaticlly  according the proc return value 
void DelActiveProcById(BlockId thdid,uint8_t procid)
{
    assert(thdid < BlcId_None ) ;
    DelActiveProc(evtframe.lpactor[thdid],procid);
}
// for one block, never active one longproc  multiplte times
void  DelActiveProc(LPThdBlock lpb,uint8_t procid)
{
    uint32_t bit ;
    uint8_t  temp ;    
    assert(lpb!=0) ;
    assert(lpb->fuc!=0) ;
#if Use_Max_Proc != 1        
    assert(lpb->procbit!=0) ;
#else 
    assert(((lpb->prochbit|lpb->procbit) !=0)) ;
#endif    
    // first find the bit postion for the sepcail procid.
#if Use_Max_Proc == 1       
    if(lpb->procbit != 0)
    {
#endif        
        bit= 1 ;
        for(temp =0; temp < 32 ; temp++,bit<<=1)
        {
            if(lpb->procbit & bit )
            {
                if(evtframe.longprocdata[temp].procid  == procid)
                { // find the longproc bit postion.
                    INT_DISABLE();
                    // Set the global bit .
                    evtframe.longprocbit |= bit ;
                    // clear the local bit .
                    bit = ~bit ;
                    lpb->procbit &= bit ;
                    assert(lpb->proccnt >0) ;
                    lpb->proccnt -- ; 
                    evtframe.longprocdata[temp].state16 = 0 ;
                    evtframe.longprocdata[temp].procdata.data32 =0 ;
                    evtframe.longprocdata[temp].procid = 0 ;
                    evtframe.longprocdata[temp].state8 = 0 ;
                    INT_ENABLE();  
                    return ;
                }
            }
        }
#if Use_Max_Proc == 1 
    }    
    
    bit= 1 ;
    for(temp =32; temp < 64 ; temp++,bit<<=1)
    {
        if(lpb->prochbit & bit )
        {
            if(evtframe.longprocdata[temp].procid  == procid)
            { // find the longproc bit postion.
                INT_DISABLE();
                // Set the global bit .
                evtframe.longprochbit |= bit ;
                // clear the local bit .
                bit = ~bit ;
                lpb->prochbit &= bit ;
                assert(lpb->proccnt >0) ;
                lpb->proccnt -- ; 
                evtframe.longprocdata[temp].state16 = 0 ;
                evtframe.longprocdata[temp].procdata.data32 =0 ;
                evtframe.longprocdata[temp].procid = 0 ;
                evtframe.longprocdata[temp].state8 = 0 ;
                INT_ENABLE();  
                return ;
            }
        }
    }    
#endif  
   
}
*/

// there is a fatal error happend ,  stop the message loop , 
#if _ASSERT  == 1
static    uint8_t  isnotfatal = 1;
#define   MaxLogBuf    16 
static uint8_t logbufhead =0   ;
static uint8_t logbufend =0    ;
uint8_t logbuf[MaxLogBuf] ;

void outlog(LogByte byte)
{
    INT_DISABLE();
    if(!SendLog(byte))  
    {
        SetHalStatu(Hal_Statu_Log); 
        logbuf[logbufend] = byte ;
        logbufend ++ ;
        logbufend &= 0x0f ;  //loop back.
        if(logbufend == logbufhead)
        {
            logbufhead = logbufend+1 ;
            logbufhead &= 0x0f ;  //loop back. 
        }
    }
    INT_ENABLE();
}

LogByte getlog(void)
{
   LogByte res;
   if(logbufend == logbufhead) 
   {
       ClrHalStatu(Hal_Statu_Log);
       return log_nothing ; // 0 
   }
   INT_DISABLE();      
   res = (LogByte)logbuf[logbufhead];
   logbufhead ++ ;
   logbufhead &= 0x0f ;  //loop back.
   if(logbufend == logbufhead) 
   {
       ClrHalStatu(Hal_Statu_Log);
   }   
   INT_ENABLE();
   return res ;     
}

extern uint32_t __initial_sp;
extern uint32_t __stack_base;
//extern uint32_t __heap_base;
//extern uint32_t __heap_limit;

const uint32_t s_magic = 0x43218765;
//const uint32_t h_magic = 0x56781234;

uint32_t stacksetmark(void)
{
		INT_DISABLE();
	
		uint32_t * msp = (uint32_t *) __get_MSP();
		uint32_t * base = & __stack_base;

		if(msp <= base) {
				INT_ENABLE();
				return 0xffffffff;
		}
				
	
		for( ; base != msp; base++)
				*base = s_magic;
		
		INT_ENABLE();
		
		return (uint32_t)msp - (uint32_t)&__stack_base;
}

uint32_t stackcheckmark(void)
{
		INT_DISABLE();
	
		uint32_t * msp = (uint32_t  *)__get_MSP();
		uint32_t * base =  & __stack_base;

		if(msp <= base || *base != s_magic) {
				INT_ENABLE();
				return 0xffffffff;
		}
	
		for( ; base != msp; base++) {
				if(*base != s_magic)
						break;
		}
		
		INT_ENABLE();
		
		return (uint32_t)base - (uint32_t)&__stack_base;
}


void assert_(char * file,uint16_t line, uint8_t b) 
{
    char * lp;
    uint8_t i,j;
    if(b) return ;

    i = 0;
    lp = file ;
    j =0 ;
    while(*lp)
    {
        if(*lp == '\\') 
        {
            i = j+1;
        }
        j++ ;
        lp++ ;
    }
    outlog((LogByte)'E');
    outlog((LogByte)'r');
    outlog((LogByte)' ');
    outlog((LogByte)file[i++]);
    outlog((LogByte)file[i++]);
    outlog((LogByte)file[i++]);
    outlog((LogByte)file[i]);
    outlog((LogByte)' ');
    outlog(((LogByte)(line >>8)));
    outlog(((LogByte)(line)));
    outlog((LogByte)'!');
    ClearLpUart0SendBuffer();
    isnotfatal = 0 ;
}
#endif 

// return the index of the first bit  , it is 1 .
#if (ActorCnt >16) 
  #define MyLOG2(x) CalcLsb32(x)
#elif (ActorCnt > 8) 
#define MyLOG2(x)  CalcLsb16(x)
#else
#define MyLOG2(x) CalcLsb8(x)
#endif

void ThdRun(void)
{
    uint8_t index,flage;
#if  StaticProcModeQuick == 1
    uint8_t lastindex, tempindex ;
#endif
    uint32_t bit, procbit ;
    StdEvt  evt ;
    LPThdBlock lpb;
    LPLongProcData  lpprocdata;
#if _ASSERT  == 1    //test if  there is error
    while(isnotfatal)
#else
    while(1)
#endif
    {
        INT_DISABLE();
        if(evtframe.bitmap != 0)
        {
            index = MyLOG2(evtframe.bitmap);
            lpb = evtframe.lpactor[index] ;
            evt = getevt(lpb);
            INT_ENABLE();
            flage = 0 ;
            procbit = lpb->procbit ;
            if(procbit > 0)
            {// there are static proc waitting event ..   find the lpprocdata of the waiting static proc  .
                while(procbit)
                {
                    
                    index = 31;
                    bit = procbit ;
                    if(bit &0x0000ffff)
                    {
                        index  -= 16 ;
                        bit &= 0x0000ffff;
                    }
                    if(bit &0x00ff00ff)
                    {
                        index  -= 8  ;
                        bit &= 0x00ff00ff;
                    }
                    if(bit &0x0f0f0f0f)
                    {
                        index  -= 4  ;
                        bit &= 0x0f0f0f0f;
                    }
                    if(bit &0x33333333)
                    {
                        index  -= 2  ;
                        bit &= 0x33333333;
                    }
                    if(bit &0x55555555)  index -= 1 ;
                    // remove the finded static proc bit 
                    bit  = (0x00000001<<index) ;
                    procbit ^= bit ;
                    lpprocdata =(LPLongProcData) &(evtframe.longprocdata[index]);
    outlog(log_proccnt);
    outlog((LogByte)(index));                
    outlog((LogByte)((uint8_t)(evt>>8))) ;     
    outlog((LogByte)((uint8_t)(evt))) ;                         
                                     
                    // check the wait signal of the current static proc .
#if  StaticProcModeQuick == 1
                    lastindex = InvalidId;
                    tempindex = evtframe.sigheader[index];
                    //assert(tempindex != InvalidId);
                    while(tempindex != InvalidId)
                    {
#if  Use_Big_Evt  == 0
                        if(CheckEvt(evtframe.sigarray[tempindex],evt))
#else
                        if((evtframe.sigarray[tempindex] ^ getevtid(evt)) == 0)
#endif //Use_Big_Evt
                        {// current evt is waiting signal .  mark finding end .
                            flage =1 ;
                            procbit = 0 ;
                            lpprocdata->state8 --;
                            if(lastindex == InvalidId)
                            {
                                // remove the header .
                                evtframe.sigheader[index] =  evtframe.sigidmap[tempindex];
                            }
                            else
                            {
                                // remove from middle . lastindex store the last valid index , and temp index store the current vailde index
                                evtframe.sigidmap[lastindex] =  evtframe.sigidmap[tempindex];
                            }
                            // removed waiting list item return to  freelist for next time using.
                            evtframe.sigidmap[tempindex] = evtframe.freesigheader;
                            evtframe.freesigheader = tempindex ;
                            
                            if(lpprocdata->state8 == 0)
                            {// waiting condition reached .
                                // release all other waiting sig from map array
                                tempindex = evtframe.sigheader[index];
                                evtframe.sigheader[index] = InvalidId ;
                                while(tempindex != InvalidId)
                                {
                                    lastindex =  evtframe.sigidmap[tempindex] ;
                                    evtframe.sigidmap[tempindex] = evtframe.freesigheader;
                                    evtframe.freesigheader = tempindex;
                                    tempindex = lastindex ;
                                }
                                // do static proc .
                                if((*(lpb->fuc[lpprocdata->procid]))(evt,index,lpprocdata)  == ThdProc_Exit)
                                { // static proc running to the end , reset lpproc data ,and return it to evtframe.
                                    lpb->proccnt -- ;
                                    lpprocdata->state16 = 0 ;
                                    lpprocdata->procdata.data32 =0 ;
                                    lpprocdata->procid = 0 ;
                                    lpprocdata->state8 = InvalidId ;
                                    lpb->procbit ^= bit ;
                                    evtframe.longprocbit |= bit ;
                                }

                            }
                            break;
                        }
                        else
                        {
                            lastindex = tempindex ;
                            tempindex = evtframe.sigidmap[tempindex];
                        }
                    } //while(tempindex != InvalidId)
#else  //   StaticProcModeQuick == 1                       
                    flage=(*(lpb->fuc[lpprocdata->procid]))(evt,index,lpprocdata) ;
                    if(flage > 0)
                    {// stop while loop
                        procbit = 0 ; 
                        if(flage == ThdProc_Exit)
                        {
                            lpb->proccnt -- ;
                            lpprocdata->state16 = 0 ;
                            lpprocdata->procdata.data32 =0 ;
                            lpprocdata->procid = 0 ;
                            lpprocdata->state8 = 0 ;
                            lpb->procbit ^= bit ;
                            evtframe.longprocbit |= bit ;
                        }
                    }
#endif //StaticProcModeQuick                  
                }// end proc bit  while .
            }//if(lpb->proccnt > 0)
#if Use_Max_Proc == 1
            if(flage == 0 )
            {
                if(lpb->prochbit > 0)
                {
                    procbit = lpb->prochbit ;
                    while(procbit)
                    {
                        index = 31;
                        bit = procbit ;
                        if(bit &0x0000ffff)
                        {
                            index  -= 16 ;
                            bit &= 0x0000ffff;
                        }
                        if(bit &0x00ff00ff)
                        {
                            index  -= 8  ;
                            bit &= 0x00ff00ff;
                        }
                        if(bit &0x0f0f0f0f)
                        {
                            index  -= 4  ;
                            bit &= 0x0f0f0f0f;
                        }
                        if(bit &0x33333333)
                        {
                            index  -= 2  ;
                            bit &= 0x33333333;
                        }
                        if(bit &0x55555555)  index -= 1 ;
                        bit = (0x00000001<<index);
                        procbit ^= bit ;
                        index += 32;
                        lpprocdata =(LPLongProcData) &(evtframe.longprocdata[index]);
#if  StaticProcModeQuick == 1
                        lastindex = InvalidId;
                        tempindex = evtframe.sigheader[index];

                        while(tempindex != InvalidId)
                        {
#if  Use_Big_Evt  == 0
                            if(CheckEvt(evtframe.sigarray[tempindex],evt))
#else
                            if((evtframe.sigarray[tempindex] ^ getevtid(evt)) == 0)
#endif //Use_Big_Evt
                            {// current evt is waiting signal .  mark finding end .
                                flage =1 ;
                                procbit = 0 ;
                                lpprocdata->state8 --;
                                if(lastindex == InvalidId)
                                {
                                    // remove the header .
                                    evtframe.sigheader[index] =  evtframe.sigidmap[tempindex];
                                }
                                else
                                {
                                    // remove from middle . lastindex store the last valid index , and temp index store the current vailde index
                                    evtframe.sigidmap[lastindex] =  evtframe.sigidmap[tempindex];
                                }
                                // removed waiting list item return to  freelist for next time using.
                                evtframe.sigidmap[tempindex] = evtframe.freesigheader;
                                evtframe.freesigheader = tempindex ;
                                
                                if(lpprocdata->state8 == 0)
                                {// waiting condition reached .
                                    // release all other waiting sig from map array
                                    tempindex = evtframe.sigheader[index];
                                    evtframe.sigheader[index] = InvalidId ;
                                    while(tempindex != InvalidId)
                                    {
                                        lastindex =  evtframe.sigidmap[tempindex] ;
                                        evtframe.sigidmap[tempindex] = evtframe.freesigheader;
                                        evtframe.freesigheader = tempindex;
                                        tempindex = lastindex ;
                                    }
                                    // do static proc .
                                    if((*(lpb->fuc[lpprocdata->procid]))(evt,index,lpprocdata) == ThdProc_Exit)
                                    { // static proc running to the end , reset lpproc data ,and return it to evtframe.
                                        lpb->proccnt -- ;
                                        lpprocdata->state16 = 0 ;
                                        lpprocdata->procdata.data32 =0 ;
                                        lpprocdata->procid = 0 ;
                                        lpprocdata->state8 = InvalidId ;
                                        lpb->prochbit ^= bit ;
                                        evtframe.longprochbit |= bit ;
                                    }

                                }
                                break;
                            }
                            else
                            {
                                lastindex = tempindex ;
                                tempindex = evtframe.sigidmap[tempindex];
                            }
                        } //while(tempindex != InvalidId)
#else
                        flage= (*(lpb->fuc[lpprocdata->procid]))(evt,index,lpprocdata) ;
                        if(flage > 0 )
                        {
                            procbit = 0 ;
                            if(flage == ThdProc_Exit)
                            {
                                lpb->proccnt -- ;
                                lpprocdata->state16 = 0 ;
                                lpprocdata->procdata.data32 =0 ;
                                lpprocdata->procid = 0 ;
                                lpprocdata->state8 = 0 ;
                                bit = (0x00000001<<(index-32);
                                lpb->prochbit ^= bit ;
                                evtframe.longprochbit |= bit ;
                            }
                        }
#endif   //StaticProcModeQuick             
                    }// while(bit).
                } //if(lpb->prochbit > 0)
            }//  if(flage == 0 )
#endif   //Use_Max_Proc   
            // if the event has not been deal by longproc  , deal it with default proc  .
			if( flage == 0)
				(*lpb->lpinit)(lpb,evt) ;
		}
		else
		{
			INT_ENABLE();
			// delay  a little time to aviod  sleep and wakeup ,in very short period .
			__nop();
			__nop();
			__nop();
			__nop();
			__nop();
			INT_DISABLE();   // disable intterupt  to  go to sleep mode , after wakeup by interrupt do power reset before can goto interrupt .
			GotoSleep();
			INT_ENABLE();

		}
	} // end for loop
#if _ASSERT  == 1    //for test if  there is fatal ,let device stop work here    
	led1on();
	led2on();
	while(1);
#endif
}



