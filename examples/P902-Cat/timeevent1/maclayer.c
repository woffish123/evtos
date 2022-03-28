#include "maclayer.h"
#include "evtos.h"
#include "timealarm.h"
#include "sx1262.h"
RfLayerData rflayerdata ;

#if Nodetype == RootNode
// root node should has a much bigger ram  . to store the NetId to short Addr map  . the max count is  the max Netid we allowed for this net
// so before used the net we need to get the max item cnt (the max Netid) , all netid should small than this one .
// so the Maxnetid will be used as the size of the NetId to Addr array .
#define  MaxNetId   6144
static uint16_t NetIdAddrArray[MaxNetId] ;
__STATIC_INLINE void setnetidtoaddr(uint16_t addr ,uint16_t netid)
{
    if(netid <MaxNetId)
        NetIdAddrArray[netid] = addr ;
}
__STATIC_INLINE uint16_t getaddrbynetid(uint16_t netid)
{
    return NetIdAddrArray[netid] ;
}

#endif

#if Nodetype != LeafNode
#define MaxLeafNodeCnt  63
#define MaxTreeNodeCnt   3
// const 63 leaf node  used 0- 32 byte ,and 3 child node  and 1 father node live cnt  used 33 -34 byte.   every node use half byte as live cnt 
#define NodeCnt      34   // byte 0-32 is for leaf node , 33-34 is used for tree node and father node .
#define TreeNodeOffset  33
static uint8_t ChildNodeLiveCnt[NodeCnt];
// index start from 0 - 62  is leaf node range . 63-65 is tree node range . 67 is father live cnt;
void ResetLeafLiveCnt(uint8_t index)
{
    uint8_t res ;
    if (index >= MaxLeafNodeCnt )
        return ;
    res = index >> 1 ;
    index = index & 1 ;
    if(index )
    { // 1 is high byte  ,start from low byte
       ChildNodeLiveCnt[res] &= 0x0f ;
       ChildNodeLiveCnt[res] |= (MaxNodeLiveCnt <<4) ;
    }
    else
    { // 0 is low byte
       ChildNodeLiveCnt[res] &= 0xf0 ;
       ChildNodeLiveCnt[res] |= MaxNodeLiveCnt  ;   
    }
}

void ResetTreeLiveCnt(uint8_t index)
{
    uint8_t res ;
    if (index >= MaxTreeNodeCnt )
        return ;
    res = index >> 1 ;
    res += TreeNodeOffset ;
    index = index & 1 ;
    if(index )
    { // 1 is high byte  ,start from low byte
       ChildNodeLiveCnt[res] &= 0x0f ;
       ChildNodeLiveCnt[res] |= (MaxNodeLiveCnt <<4) ;
    }
    else
    { // 0 is low byte
       ChildNodeLiveCnt[res] &= 0xf0 ;
       ChildNodeLiveCnt[res] |= MaxNodeLiveCnt  ;   
    }

}
// sub all item 1 , if it's 0 reset the leaf bitmap bit .
void  SubChildNodeLiveCnt(void)
{
    uint32_t bitmap ;
    uint8_t res ,index;
    // sub the live cnt of the local leaf node .
    bitmap = 0x01 ;
    for(index = 0  ; index < 32 ; index++ )
    {
        if(globaldata.cfgdata.LeafBitmapL & bitmap)
        {
            res = ChildNodeLiveCnt[index >>1 ];
            if(index &1)
            { // high byte
                if(res & 0xf0)
                    res -= 0x10 ;
                else
                    globaldata.cfgdata.LeafBitmapL &=  (~bitmap);
            }
            else
            {
                 if(res & 0x0f)
                    res -= 0x1 ;
                else
                    globaldata.cfgdata.LeafBitmapL &=  (~bitmap);           
            
            }
            ChildNodeLiveCnt[index >>1 ] = res ;    
        }
        bitmap <<=1 ;    
    }

    bitmap = 0x01 ;
    for(index = 32  ; index < 63 ; index++ ) // highest bit not used .
    {
        if(globaldata.cfgdata.LeafBitmapH & bitmap)
        {
            res = ChildNodeLiveCnt[index>>1];
            if(index &1)
            { // high byte
                if(res & 0xf0)
                    res -= 0x10 ;
                else
                    globaldata.cfgdata.LeafBitmapH &=  (~bitmap);
            }
            else
            {
                 if(res & 0x0f)
                    res -= 0x1 ;
                else
                    globaldata.cfgdata.LeafBitmapH &=  (~bitmap);           
            
            }
            ChildNodeLiveCnt[index>>1 ] = res ;    
        }
        bitmap <<=1 ;    
    }  
    // sub live cnt of the child tree  , there are max 3 child for a tree node .
    bitmap = 0x10 ;
    for(index = 64  ; index < 67 ; index++ ) // highest bit not used .
    {
        if(globaldata.cfgdata.TreeBitmap & bitmap)
        {
            res = ChildNodeLiveCnt[index >>1 ];
            if(index &1)
            { // high byte
                if(res & 0xf0)
                    res -= 0x10 ;
                else
                    globaldata.cfgdata.TreeBitmap &=  (~bitmap);
            }
            else
            {
                 if(res & 0x0f)
                    res -= 0x1 ;
                 else
                    globaldata.cfgdata.TreeBitmap &=  (~bitmap);           
            }
            ChildNodeLiveCnt[index>>1 ] = res ;    
        }
        bitmap <<=1 ;    
    } 
 }
#else // local is  a leaf node 
#define NodeCnt      1   // byte 0-32 is for leaf node , 33-34 is used for tree node and father node .
static uint8_t ChildNodeLiveCnt[NodeCnt]; 
#endif
 
#if Nodetype != RootNode 
// sub father live cnt , return 1 ,if father live cnt is  0 ,else return  0 
void SubFatherLiveCnt(void)
{
    uint8_t res =  ChildNodeLiveCnt[NodeCnt -1];
    if(res & 0xf0)
    {
        res -= 0x10 ;
        ChildNodeLiveCnt[NodeCnt -1] =  res ;
        return ;
    }
    postevtbyindex(BlcId_Net,Sig_Rf_Discnt_Father);
}
void  ResetFatherLiveCnt(void)
{
    ChildNodeLiveCnt[NodeCnt -1] |=  0xf0 ;
  
}
#endif


static uint8_t NodeActiveCnt = 0;  // used to set max getrtc time cnt .
// check if the local node ,active cnt is over time .
uint8_t SubActiveCnt(void)
{
    if(NodeActiveCnt > 0)
    {
        NodeActiveCnt -- ;
        return 0;
    }
    return 1 ;
}
// set local node as just register ok - max live count
__STATIC_INLINE void ResetActiveCnt(void)
{
    NodeActiveCnt = globaldata.cfgdata.ActivePeriod ;
}


 
// mac layer part  -----------------------------------------------

// init the mac layer event buffer and the live cnt to zero . 
// it is called at node reset .
void rflayerdataInit(void)
{
    uint16_t i ;
    uint32_t * lp32;
    uint8_t *lp8 ;
    // init the rf memory pool .
    lp32 = rflayerdata.memarray;   
    // fill memarray to next id  to make a list . 
    for(i = 0 ; i< (PoolRF_Cnt-1) ; i++)
    {
        *lp32 = i+1 ;
        lp32 += PoolRF_Size;
    }
    // set last as invalid  to mark the list end .
    *lp32 = InvalidId ;
    // fill rflayerbuf to make a list , rflayerbuf hold the rf memarray index  that is used in Mac layer , 
    // there also some memarray is send to Net layer .
    lp32 = (uint32_t *)rflayerdata.msglist ;
    for(i =0 ; i< (Max_RfLayerMem-1)  ; i++)
    {// set msg.memid = 0xff , msg.nextid = i+1;
        *lp32 =0xff00 | (i+1);
        lp32 ++ ;
    }
    *lp32 = 0xffff ;  // set last to invalidid 
    
    // set all index as invalid 
    rflayerdata.freeheader = 0 ;
    rflayerdata.recvheader = InvalidId ;
    rflayerdata.onlineheader = InvalidId ;
    rflayerdata.offlineheader = InvalidId ;
    
    rflayerdata.onlinecur = InvalidId ;    
    rflayerdata.onlineender = InvalidId ;    
    rflayerdata.recvender = InvalidId ;
    
    rflayerdata.netmode = net_init ;
    rflayerdata.rfmode  = rf_init ;
    rflayerdata.memfree = 0 ;
    rflayerdata.rtcfreeday = 0 ;
    // clear the node address to node id array .
    
	lp8 = ChildNodeLiveCnt;
    i = NodeCnt  ;
    while(i)    
    {
        *lp8 = 0x0;
        lp8++ ;
        i-- ;
    }
    NodeActiveCnt =  0;
#if Nodetype == RootNode
    
    if(((uint32_t) NetIdAddrArray) &0x03)
    { // not 4 byte assigned .
        NetIdAddrArray[0] =  0xffff ;
        lp32 = (uint32_t *)(&NetIdAddrArray[1]);
        i = (MaxNetId -1) >>1 ;
    }
    else
    {
        lp32 = (uint32_t *)NetIdAddrArray;
        i = MaxNetId >>1 ;
    }
    while(i)    
    {
        *lp32 = 0xffffffff;
        lp32++ ;
        i-- ;
    }
    // set the last one , it maybe not setted .
    NetIdAddrArray[MaxNetId-1] = 0xffff;
#endif   

}
// release all msg memery stored in  rflayerdata . it is called when RF power off 
// there maybe other rf memory is used by other code ,  not influence them .
void rflayerdatarelease(void)
{
     uint8_t id ;
     uint32_t * lp32 ;
     id = rflayerdata.recvheader ;
     while(id != InvalidId)
     {
        freerfbuf(rflayerdata.msglist[id].value.memid);
        id = rflayerdata.msglist[id].value.nextid; 
     }
     id = rflayerdata.onlineheader ;
     while(id != InvalidId)
     {
        freerfbuf(rflayerdata.msglist[id].value.memid);
        id = rflayerdata.msglist[id].value.nextid; 
     }    
     id = rflayerdata.offlineheader ;
     while(id != InvalidId)
     {
        freerfbuf(rflayerdata.msglist[id].value.memid);
        id = rflayerdata.msglist[id].value.nextid; 
     } 
    lp32 = (uint32_t *)rflayerdata.msglist ;
    for(id =0 ; id< (Max_RfLayerMem-1)  ; id++)
    {// set msg.memid = 0xff , msg.nextid = i+1;
        *lp32 =0xff00 | (id+1);
        lp32 ++ ;
    }
    *lp32 = 0xffff ;  // set last to invalidid 
    
    // set all index as invalid 
    rflayerdata.freeheader = 0 ;
    rflayerdata.recvheader = InvalidId ;
    rflayerdata.onlineheader = InvalidId ;
    rflayerdata.offlineheader = InvalidId ;
    
    rflayerdata.onlinecur = InvalidId ;    
    rflayerdata.onlineender = InvalidId ;    
    rflayerdata.recvender = InvalidId ;    
    
    rflayerdata.netmode = net_init ;
    rflayerdata.rfmode  = rf_init ;
    rflayerdata.memfree = 0 ;
    rflayerdata.rtcfreeday = 0 ;    
}


uint8_t CheckRtcFreeDay(void)  
{
#if  Nodetype !=  RootNode     
    if(rflayerdata.netmode < net_monitor)
        return 0;
    if(rflayerdata.rtcfreeday > Max_RtcFreeCnt)
        return 1 ;
    else
        rflayerdata.rtcfreeday ++; 
#endif   
    return 0;
   
} 

// get a rf buffer , it try to get a new buffer from pool2 :  newevt(addr,PoolId2)
// if there is no buffer , it discard the last item in online queue or offline queue 
uint8_t newrfbuf(uint16_t addr)
{
    RfMsg var  ;
    
    if(rflayerdata.memfree != InvalidId )
    {
        INT_DISABLE();
        var.value.memid = rflayerdata.memfree;
        rflayerdata.memfree = (uint8_t) * ((uint32_t *)(rflayerdata.memarray+ PoolRF_Size * var.value.memid));
        INT_ENABLE();
        return var.value.memid ;
    }
    // no free buffer .
    //try to get one from online , offline queue.
    var = getonlinemsg();
    if(var.value.memid == InvalidId)
    {
        if(rflayerdata.offlineheader != InvalidId)
            var = rmofflineid(rflayerdata.msglist[rflayerdata.offlineheader].value.memid );
    }
    assert(var.value.memid != InvalidId);
    return var.value.memid ;
}

void freerfbuf(uint8_t id)
{
	INT_DISABLE();
	*((uint32_t *)(rflayerdata.memarray+ PoolRF_Size * id)) =  rflayerdata.memfree;
	rflayerdata.memfree = id ;
	INT_ENABLE();
}

// put a evt to recv queue ender.
uint8_t  addrecvmsg(uint8_t msg,uint16_t addr)
{
    uint8_t  id;
    // there must has a free postion.
    assert(rflayerdata.freeheader != InvalidId);
    // get first and next free item  .
    id = rflayerdata.freeheader ;
    rflayerdata.freeheader = rflayerdata.msglist[id].value.nextid;
    
    // store the msg to first free position
    rflayerdata.msglist[id].value.memid = msg ;
    rflayerdata.msglist[id].value.addr = addr ;
    rflayerdata.msglist[id].value.nextid = InvalidId;
    // add it to recv queue end 
    if(rflayerdata.recvender == InvalidId)
    {
        assert(rflayerdata.recvheader == InvalidId);
        rflayerdata.recvheader = id ;
        rflayerdata.recvender = id ; 
        return 1;
    }
    else
    {
        assert(rflayerdata.recvheader != InvalidId);
        rflayerdata.msglist[rflayerdata.recvender].value.nextid = id ;
        rflayerdata.recvender = id ; 
        return 0;
    }
   
}

// put a evt to recv queue header.
uint8_t  insertrecvmsg(uint8_t msg,uint16_t addr)
{
    uint8_t  id;
    // there must has a free postion.
    assert(rflayerdata.freeheader != InvalidId);
    // get first and next free item  .
    id = rflayerdata.freeheader ;
    rflayerdata.freeheader = rflayerdata.msglist[id].value.nextid;
    
    // store the msg to first free position
    rflayerdata.msglist[id].value.memid = msg ;
    rflayerdata.msglist[id].value.addr = addr ;
    rflayerdata.msglist[id].value.nextid = rflayerdata.recvheader;
    rflayerdata.recvheader = id ;
    // add it to recv queue end 
    if(rflayerdata.recvender == InvalidId)
    {
        assert(rflayerdata.recvheader == InvalidId);
        rflayerdata.recvheader = id ;
        return 1;
    }
    return 0;
}
// return the first msg from recv queue , and remove it from  recv queue
RfMsg rmrecvmsg()
{
    RfMsg var ;
    uint8_t id ;
    // get the first msg
    if(rflayerdata.recvheader == InvalidId)
    {
        var.value.memid = InvalidId ;
        return var ;
    }
    // update the recv queue.
    id = rflayerdata.recvheader ;
    var.var.data32 = rflayerdata.msglist[id].var.data32 ;
    rflayerdata.recvheader = var.value.nextid;
    if(rflayerdata.recvender == id)
        rflayerdata.recvender = InvalidId;
    // store the postion to free queue
    rflayerdata.msglist[id].value.nextid = rflayerdata.freeheader ;
    rflayerdata.freeheader = id ;
    return var ;    
}

// try to find a specail msg id in recv list ,it's addr is same as input .
RfMsg findrecvaddr(uint16_t addr)
{
    RfMsg msg ;
    msg.value.memid = InvalidId ;
    msg.value.nextid  = rflayerdata.recvheader ;
    while(msg.value.nextid != InvalidId)
    {
        if(rflayerdata.msglist[msg.value.nextid].value.addr == addr )    
            return msg ;
        msg.value.nextid = rflayerdata.msglist[msg.value.nextid].value.nextid;
    }
    return msg ;
}
// remove all msg with specail addr from queue .
void rmrecvaddr(uint16_t addr)
{
    uint8_t cur ,last ;
    cur  = rflayerdata.recvheader ;
    if(cur == InvalidId)
        return  ;
    // use last to hold the last id .
    last =  InvalidId;
    while(cur != InvalidId)
    {
        if(rflayerdata.msglist[cur].value.addr == addr )    
        { // remove cur .
            freerfbuf(rflayerdata.msglist[cur].value.memid);
            if(last == InvalidId)            
            {// first one .
                last = cur ;
                cur =rflayerdata.msglist[cur].value.nextid ;
                rflayerdata.recvheader = cur ;
                rflayerdata.msglist[last].value.nextid  = rflayerdata.freeheader ;
                rflayerdata.freeheader = last ;
                last = InvalidId ;
            }
            else
            {
                rflayerdata.msglist[last].value.nextid =  rflayerdata.msglist[cur].value.nextid ;      
                rflayerdata.msglist[cur].value.nextid  = rflayerdata.freeheader ;
                rflayerdata.freeheader = cur ; 
                cur  = rflayerdata.msglist[last].value.nextid ;
            }
        }
        else
        {
            // goto next 
            last= cur ;
            cur = rflayerdata.msglist[cur].value.nextid;
        }
    }
    rflayerdata.recvender = last ;
}

// put a msg to direct queue at ender .
void  addonlinemsg(uint8_t msg ,uint16_t addr)
{
    uint8_t id;
    // there must has a free postion.
    assert(rflayerdata.freeheader != InvalidId);
    // get first and next free item  .
    id = rflayerdata.freeheader ;
    rflayerdata.freeheader = rflayerdata.msglist[id].value.nextid;
    rflayerdata.msglivecnt[id] = MsgMaxSendCnt;
    // store the msg to first free position
    rflayerdata.msglist[id].value.memid = msg ;
    rflayerdata.msglist[id].value.addr = addr ;
    
    rflayerdata.msglist[id].value.nextid = InvalidId ;
    // add it to direct queue end 
    if(rflayerdata.onlineheader == InvalidId)
    {
        rflayerdata.onlineheader = id;
        rflayerdata.onlinecur = id ;
    }
    else
    {
        rflayerdata.msglist[rflayerdata.onlineender].value.nextid = id ;
    }
    rflayerdata.onlineender = id ;    
}
// get a msg from online queue . not remove .
// Notice not return the first one ,but current one ,add the current one will be changed.
RfMsg getonlinemsg()
{
    RfMsg var ;
    StdEvt  evt ;
    uint8_t id ;
    var.value.memid = rflayerdata.onlinecur;
    // get the first msg
    if(var.value.memid == InvalidId)
        return var ;
    id = var.value.memid ;
    var.var.data32 = rflayerdata.msglist[id].var.data32 ;
    
    if(id == rflayerdata.onlineender)
        rflayerdata.onlinecur = rflayerdata.onlineheader ;
    else
    {
        rflayerdata.onlinecur = rflayerdata.msglist[id].value.nextid ;
    } 
    if(rflayerdata.msglivecnt[id] > 0)
    {
        rflayerdata.msglivecnt[id]-- ;
        return var ;
    }

    evt = makeevt(Sig_Rf_Overtime,var.value.memid);
    postevtbyindex(BlcId_Net,evt);  
    // remove this one and post it to net layer .
    evt = rflayerdata.onlineheader ;
    if(id == evt)
    {// is the first one .
        rflayerdata.onlineheader  = rflayerdata.msglist[evt].value.nextid ;
        if(rflayerdata.onlineender == id)
        {
            rflayerdata.onlinecur = InvalidId ;
            rflayerdata.onlineender = InvalidId;
            rflayerdata.onlineheader = InvalidId;
        }
    }else
    {// it must can be find .
        while(rflayerdata.msglist[evt].value.nextid != id)
        {
            evt = rflayerdata.msglist[evt].value.nextid ;
        }
        rflayerdata.msglist[evt].value.nextid = rflayerdata.msglist[id].value.nextid ;
        if(rflayerdata.onlineender == id)
        {
            rflayerdata.onlinecur = rflayerdata.onlineheader ;
            rflayerdata.onlineender = evt;
        }        
    }
    // return to free list .
    rflayerdata.msglist[id].value.nextid = rflayerdata.freeheader ;
    rflayerdata.freeheader = id ;        
    // update cur and ender .
    
    return var ;  
}

// remove all msg from queue  that addr is specail.
void rmonlineaddr(uint16_t addr)
{
    uint8_t cur ,last ;
    cur  = rflayerdata.onlineheader ;
    if(cur == InvalidId)
        return  ;
    // use last to hold the last id .
    last =  InvalidId;
    while(cur != InvalidId)
    {
        if(rflayerdata.msglist[cur].value.addr == addr )    
        { // remove cur .
            freerfbuf(rflayerdata.msglist[cur].value.memid);
            if(last == InvalidId)            
            {// first one .
                last = cur ;
                cur =rflayerdata.msglist[cur].value.nextid ;
                rflayerdata.onlineheader = cur ;
                rflayerdata.msglist[last].value.nextid  = rflayerdata.freeheader ;
                rflayerdata.freeheader = last ;
                last = InvalidId ;
            }
            else
            {
                rflayerdata.msglist[last].value.nextid =  rflayerdata.msglist[cur].value.nextid ;   
                rflayerdata.msglist[cur].value.nextid  = rflayerdata.freeheader ;
                rflayerdata.freeheader = cur ; 
                cur  = rflayerdata.msglist[last].value.nextid ;
            }
        }
        else
        {
            // goto next 
            last= cur ;
            cur = rflayerdata.msglist[cur].value.nextid;
        }
    }
    //set cur == first .
    rflayerdata.onlineender = last ;
    rflayerdata.onlinecur = rflayerdata.onlineheader ;

}
// remove the msg the buffer id is specailed . there only one could find .
RfMsg rmonlineid(uint8_t bufid)
{
    RfMsg var ;
    var.value.memid = rflayerdata.onlineheader  ;
    if(var.value.memid == InvalidId)
        return var ;    
    if(rflayerdata.msglist[var.value.memid].value.memid == bufid )
    { // is first one 
        bufid = var.value.memid  ;
        rflayerdata.onlineheader  = rflayerdata.msglist[bufid].value.nextid ;
        var.var.data32 = rflayerdata.msglist[bufid].var.data32;
        // update cur and ender .
        if(rflayerdata.onlineender == bufid)
        {
            rflayerdata.onlinecur = InvalidId ;
            rflayerdata.onlineender = InvalidId;
            rflayerdata.onlineheader = InvalidId;
        }
        else
        {
            if(rflayerdata.onlinecur == bufid)
                 rflayerdata.onlinecur  = rflayerdata.onlineheader;
        }
        // return to free list .
        rflayerdata.msglist[bufid].value.nextid = rflayerdata.freeheader ;
        rflayerdata.freeheader = bufid ;        
        return var ;
    }
    var.value.nextid =  var.value.memid ;
    var.value.memid = rflayerdata.msglist[var.value.memid].value.nextid ;
    while(var.value.memid != InvalidId)
    {
        if(rflayerdata.msglist[var.value.memid].value.memid == bufid )    
        {
            bufid = var.value.memid ;
			rflayerdata.msglist[var.value.nextid].value.nextid = rflayerdata.msglist[var.value.memid].value.nextid ;
            // check if it 's the last one 
            if(bufid == rflayerdata.onlineender)            
            {
                rflayerdata.onlineender = var.value.nextid  ;
                if(bufid == rflayerdata.onlinecur )
                   rflayerdata.onlinecur =  rflayerdata.onlineheader ;
            }
            // put current to free list 
            rflayerdata.msglist[bufid].value.nextid = rflayerdata.freeheader ;
            rflayerdata.freeheader = bufid ;         
            // copy data out .
            var.var.data32 =  rflayerdata.msglist[var.value.memid].var.data32;
            break;
        }
        // goto next 
        var.value.nextid = var.value.memid ;
        var.value.memid = rflayerdata.msglist[var.value.memid].value.nextid;
    }
    return var ;

}
// try to find a specail msg id
RfMsg findonlineaddr(uint16_t addr)
{
    RfMsg msg ;
    msg.value.memid = InvalidId ;
    msg.value.nextid  = rflayerdata.onlineheader ;
    while(msg.value.nextid != InvalidId)
    {
        if(rflayerdata.msglist[msg.value.nextid].value.addr == addr )
            return rflayerdata.msglist[msg.value.nextid] ;
        msg.value.nextid = rflayerdata.msglist[msg.value.nextid].value.nextid;
    }
    return msg ;
}

// put a msg to direct queue .
void  addofflinemsg(uint8_t msg ,uint16_t addr)
{
    uint8_t id;
    // there must has a free postion.
    assert(rflayerdata.freeheader != InvalidId);
    // get first and next free item  .
    id = rflayerdata.freeheader ;
    rflayerdata.freeheader = rflayerdata.msglist[id].value.nextid;
    rflayerdata.msglivecnt[id] = MsgMaxWaitCnt ;
    // store the msg to first free position
    rflayerdata.msglist[id].value.memid = msg ;
    rflayerdata.msglist[id].value.addr = addr ;
    rflayerdata.msglist[id].value.nextid = InvalidId ;
    // add it to direct queue end 
    if(rflayerdata.offlineheader == InvalidId)
    {
        rflayerdata.offlineheader = id;
        return ;
    }
    msg = rflayerdata.offlineheader ;
    while(rflayerdata.msglist[msg].value.nextid != InvalidId)
    {
        msg = rflayerdata.msglist[msg].value.nextid;
    }
    rflayerdata.msglist[msg].value.nextid = id ;
}
// remove a msg from offline queue . it's id is bufid 
RfMsg rmofflineid(uint8_t bufid)
{
    RfMsg var ;
    var.value.memid = rflayerdata.offlineheader  ;
    if(var.value.memid == InvalidId)
        return var ;    
    if(rflayerdata.msglist[var.value.memid].value.memid == bufid )
    { // is first one 
        bufid = var.value.memid  ;
        rflayerdata.offlineheader  = rflayerdata.msglist[bufid].value.nextid ;
        var.var.data32 = rflayerdata.msglist[bufid].var.data32;
        // return to free list .
        rflayerdata.msglist[bufid].value.nextid = rflayerdata.freeheader ;
        rflayerdata.freeheader = bufid ;        
        return var ;
    }
    var.value.nextid =  var.value.memid ;
    var.value.memid = rflayerdata.msglist[var.value.memid].value.nextid ;
    while(var.value.memid != InvalidId)
    {
        if(rflayerdata.msglist[var.value.memid].value.memid == bufid )    
        {
            bufid = var.value.memid  ;
            rflayerdata.msglist[var.value.nextid].value.nextid = rflayerdata.msglist[var.value.memid].value.nextid ;
            var.var.data32 = rflayerdata.msglist[bufid].var.data32;
            // return to free list .
            rflayerdata.msglist[bufid].value.nextid = rflayerdata.freeheader ;
            rflayerdata.freeheader = bufid ;        
            break;
        }
        // goto next 
        var.value.nextid = var.value.memid ;
        var.value.memid = rflayerdata.msglist[var.value.memid].value.nextid;
    }
    return var ;
}
// remove the first RfMsg it's addr is specail
RfMsg getofflineaddr(uint16_t addr)
{
    RfMsg var ;
    var.value.memid = rflayerdata.offlineheader  ;
    if(var.value.memid == InvalidId)
        return var ;    
    if(rflayerdata.msglist[var.value.memid].value.addr == addr )
    { // is first one 
        addr = var.value.memid  ;
        rflayerdata.offlineheader  = rflayerdata.msglist[addr].value.nextid ;
        var.var.data32 = rflayerdata.msglist[addr].var.data32;
        // return to free list .
        rflayerdata.msglist[addr].value.nextid = rflayerdata.freeheader ;
        rflayerdata.freeheader = addr ;        
        return var ;
    }
    var.value.nextid =  var.value.memid ;
    var.value.memid = rflayerdata.msglist[var.value.memid].value.nextid ;
    while(var.value.memid != InvalidId)
    {
        if(rflayerdata.msglist[var.value.memid].value.addr == addr )    
        {
            addr = var.value.memid  ;
            rflayerdata.msglist[var.value.nextid].value.nextid = rflayerdata.msglist[var.value.memid].value.nextid ;
            var.var.data32 = rflayerdata.msglist[addr].var.data32;
            // return to free list .
            rflayerdata.msglist[addr].value.nextid = rflayerdata.freeheader ;
            rflayerdata.freeheader = addr ;        
            break;
        }
        // goto next 
        var.value.nextid = var.value.memid ;
        var.value.memid = rflayerdata.msglist[var.value.memid].value.nextid;
    }
    return var ;

}
// remove all msg from offline queue . it's addr is specail 
void rmofflineaddr(uint16_t addr)
{
    uint8_t cur ,last ;
    cur  = rflayerdata.offlineheader ;
    if(cur == InvalidId)
        return  ;
    // use last to hold the last id .
    last =  InvalidId;
    while(cur != InvalidId)
    {
        if(rflayerdata.msglist[cur].value.addr == addr )    
        { // remove cur .
            freerfbuf(rflayerdata.msglist[cur].value.memid);
            if(last == InvalidId)            
            {// first one .
                last = cur ;
                cur =rflayerdata.msglist[cur].value.nextid ;
                rflayerdata.offlineheader = cur ;
                rflayerdata.msglist[last].value.nextid  = rflayerdata.freeheader ;
                rflayerdata.freeheader = last ;
                last = InvalidId ;
            }
            else
            {
                rflayerdata.msglist[last].value.nextid =  rflayerdata.msglist[cur].value.nextid ;      
                rflayerdata.msglist[cur].value.nextid  = rflayerdata.freeheader ;
                rflayerdata.freeheader = cur ; 
                cur  = rflayerdata.msglist[last].value.nextid ;
            }
        }
        else
        {
            // goto next 
            last= cur ;
            cur = rflayerdata.msglist[cur].value.nextid;
        }
    }
}
//
void subofflinemsglivecnt(void)
{
    uint8_t cur , last;
    StdEvt  evt ;
    cur = rflayerdata.offlineheader ;
    last = InvalidId ;
    while(cur != InvalidId)
    {
        if(rflayerdata.msglivecnt[cur] > 0)
            rflayerdata.msglivecnt[cur]-- ;
        else
        {// remove and post to net layer .
            evt = makeevt(Sig_Rf_Overtime,rflayerdata.msglist[cur].value.memid);
            postevtbyindex(BlcId_Net,evt);             
            if(last == InvalidId)
            {// first one.
                last = cur ;
                cur = rflayerdata.msglist[cur].value.nextid ;
                // return to free list .
                rflayerdata.msglist[last].value.nextid = rflayerdata.freeheader ;
                rflayerdata.freeheader = last ;      
                last = InvalidId;        
            }
            else
            {
                rflayerdata.msglist[last].value.nextid = rflayerdata.msglist[cur].value.nextid ;
                 // return to free list .
                rflayerdata.msglist[cur].value.nextid = rflayerdata.freeheader ;
                rflayerdata.freeheader = cur ;   
                cur =  rflayerdata.msglist[last].value.nextid ;   
            }
        }
    }
}

// router inter function , used to check ,get fitable addr 



// bitmap must be set before anything . it can't be chang by programe .
uint8_t istreenode()
{
	return globaldata.cfgdata.TreeBitmap & 0x80 ? 0 :1 ;
}
//addr is a addr and fix bit cnt  togather data .
void setaddrcfg(uint16_t addr)
{
	globaldata.cfgdata.LocalAddr  = addr ;	
	if((globaldata.cfgdata.TreeBitmap & 0x80) == 0 )
	{// local is a leaf node .
		globaldata.cfgdata.LocalAddr  = addr | 0x3f ; // this address is used by father node . we should remove this bit from leaf bitmap ;
		globaldata.cfgdata.TreeBitmap = (uint8_t)(addr&0x0f);
	}
}

// find a unused localcfg for child tree node .  addr is addr | fix bit cnt .
// return 0x0 if there is no child tree addr .
// this function just find a free addr , but not alloc it out  .
uint16_t findchildaddr()
{
	uint16_t childaddr , temp ;

	uint8_t res;
	// must be a tree node .

	res  = globaldata.cfgdata.TreeBitmap &0x0f ; // fix bit cnt 
	if(res < NetDividor )
		return 0xffff ; 
	// get header fixed bit with var bit as 0 at ender .
	childaddr = globaldata.cfgdata.LocalAddr >>(res+LeafBitCnt) ;
	res-= NetDividor ;
	childaddr <<=2;
	// check unused child tree node .
	if((globaldata.cfgdata.TreeBitmap &0x10) == 0)
	{
		//globaldata.cfgdata.TreeBitmap |= 0x10 ;
        //ResetLiveCnt(ChildTreeOffset);
	}
	else if((globaldata.cfgdata.TreeBitmap &0x20) == 0)
	{
		//globaldata.cfgdata.TreeBitmap |= 0x20 ;
		childaddr +=1;
        ResetLiveCnt(ChildTreeOffset+1);
	}
	else if((globaldata.cfgdata.TreeBitmap &0x40) == 0)
	{
		//globaldata.cfgdata.TreeBitmap |= 0x40 ;
		childaddr +=2;
        //ResetLiveCnt(ChildTreeOffset+2);
	}
	else
		return 0xffff;
	// add the fix bit 
	childaddr <<=res;
	temp = 1<<res;
	temp-- ;
	childaddr |= temp ;
	childaddr <<=LeafBitCnt;
	childaddr |= res ;

	return childaddr ;

}
// mark the specail addr has been alloced
// return 1 : ok ,
// return 0 : failed
uint8_t markaddralloced(uint16_t addr)
{
    uint16_t temp;
    if((addr &0x3f) == 0x3f)
    { // tree addr
        // check if it is a valid addr
        // 1 father should at least one net
        temp = globaldata.cfgdata.TreeBitmap&0x0f ;
        if(temp < NetDividor)
            return 0;
        temp = addr >>(globaldata.cfgdata.TreeBitmap&0x0f+LeafBitCnt-NetDividor);
        temp &= 0x03 ;
        if(temp == 0 )
        {
            if((globaldata.cfgdata.TreeBitmap &0x10) == 0)
            {
                globaldata.cfgdata.TreeBitmap |= 0x10;
                ResetLiveCnt(ChildTreeOffset);
                return 1 ;
            }
        }
        else if (temp == 1 )
        {
            if((globaldata.cfgdata.TreeBitmap &0x20) == 0)
            {
                globaldata.cfgdata.TreeBitmap |= 0x20;
                ResetLiveCnt(ChildTreeOffset+1);
                return 1 ;
            }
        }
        else if (temp == 2 )
        {
            if((globaldata.cfgdata.TreeBitmap &0x40) == 0)
            {
                globaldata.cfgdata.TreeBitmap |= 0x40;
                ResetLiveCnt(ChildTreeOffset+2);
                return 1 ;
            }
        }        
        return 0;
    }
    else
    { // leaf addr 
        // check if it is a valid addr
         if((addr|0x3f)!= globaldata.cfgdata.LocalAddr)
            return 0;
         addr &= 0x3f ;
         if(addr >31)
         {
             if(globaldata.cfgdata.LeafBitmapL & (1<<addr))
                 return 0;
             globaldata.cfgdata.LeafBitmapL |= (1<<addr);
             ResetLiveCnt(addr);
             return 1;
         }
         else if(addr < 63)
         {
              addr -=32 ;
              if(globaldata.cfgdata.LeafBitmapH & (1<<addr))
                 return 0;
              globaldata.cfgdata.LeafBitmapH= (1<<addr);
              ResetLiveCnt(addr);
              return 1;            
         }
         return 0;       
    }
}
uint16_t findleafaddr()
{
	uint32_t temp ;
	uint8_t res ;
	// must be a tree node .
	if(globaldata.cfgdata.LeafBitmapL != 0xffffffff)  
	{
		temp = globaldata.cfgdata.LeafBitmapL ;
		res = 0 ;
		while((temp&1) != 0)
		{
			res++;
			temp>>=1 ;
		}
		temp = 1<<res ;
		//globaldata.cfgdata.LeafBitmapL  |= temp ;
		goto CaclReturn ;
	}
	
	if(globaldata.cfgdata.LeafBitmapH !=  0xffffffff) // // bit31 is not used. it will is father tree addr .
	{
		temp = globaldata.cfgdata.LeafBitmapH<<1 ;
		res =30 ;
		while((temp& 0x80000000) != 0)
		{
			res--;
			temp<<=1 ;
		}
		temp = 1<<res ;
		//globaldata.cfgdata.LeafBitmapH  |= temp ;
		res +=32 ;
		goto CaclReturn ;
	}

	return 0xffff ;
CaclReturn :
    //ResetLiveCnt(res);
	temp = globaldata.cfgdata.LocalAddr & 0xffc0;
	temp |= res ;
	return (uint16_t) temp ;
}
// is current addr has free child  (tree)addr
uint8_t  ishasfreechild()
{
	return (globaldata.cfgdata.TreeBitmap & 0x70) ==0x70 ? 0:1 ;
}
// is current addr has free leaf addr
uint8_t ishasfreeleaf() 
{
    if((globaldata.cfgdata.LeafBitmapH != 0xffffffff) || (globaldata.cfgdata.LeafBitmapL != 0xffffffff))
        return 1 ;
    return 0;
}
// free a used localcfg for child tree node  ,return 1 :removed . 0 : invalid addr
uint8_t  freesubnode(uint16_t childaddr)
{
	uint32_t temp ;
	uint16_t result;
	uint8_t res ;
	// check if it's a local child node .  : the fixed header bit , fill 1 middle bit should be same .
	// get differrnt bit mask 
	res = globaldata.cfgdata.TreeBitmap &0x0f ;
	if(res >= NetDividor)
		temp=0x03<<(res-NetDividor);
	else
		temp = 0 ;

	result = globaldata.cfgdata.LocalAddr ^ childaddr ;
	result >>= LeafBitCnt ;
	result |= (uint16_t)temp ;
	result ^= (uint16_t)temp ;
	if(result)
	{
		return 0 ; //not a valid addr .
	}
    
	//reserve  leaf index .
	res = childaddr & 0x3f ;

	if(res<=0x3e)
	{ // this is leaf node . remove the bitmap 

		if(res >31)
		{
			res -= 32 ;
			temp = 1<< res ;
			temp = ~temp ;
			globaldata.cfgdata.LeafBitmapH &= temp ;
		}
		else
		{
			temp = 1<< res ;
			temp = ~temp ;
			globaldata.cfgdata.LeafBitmapL &= temp ;
		}
	}
	else
	{// this is a tree node .

		res = globaldata.cfgdata.TreeBitmap &0x0f ;
		// get the var bits of the child node . 
		result = childaddr >> (LeafBitCnt - NetDividor +res);
		res =  (uint8_t) (result & NetMask ); 
		// chang as bitmask .
		res = 1<< (res+4) ;
		res = ~res ;

		globaldata.cfgdata.TreeBitmap &= res ;
	}
	return 1;
}
// reset child node live cnt 
void ResetLiveCntByAddr(uint16_t  addr)
{
	uint32_t temp ;
	uint16_t result;
	uint8_t res ;
	// check if it's a local child node .  : the fixed header bit , fill 1 middle bit should be same .
	// get differrnt bit mask 
	res = globaldata.cfgdata.TreeBitmap &0x0f ;
	if(res >= NetDividor)
		temp=0x03<<(res-NetDividor);
	else
		temp = 0 ;

	result = globaldata.cfgdata.LocalAddr ^ addr ;
	result >>= LeafBitCnt ;
	result |= (uint16_t)temp ;
	result ^= (uint16_t)temp ;
	if(result)
	{
		return  ; //not a valid addr .
	}
    
	//reserve  leaf index .
	res = addr & 0x3f ;

	if(res<=0x3e)
	{ // this is leaf node . reset the live cnt .
        ResetLiveCnt(res);
	}
	else
	{// this is a tree node .

		res = globaldata.cfgdata.TreeBitmap &0x0f ;
		// get the var bits of the child node . 
		result = addr >> (LeafBitCnt - NetDividor +res);
		res =  (uint8_t) (result & NetMask ); 
        ResetLiveCnt(res +ChildTreeOffset);
	}
}

// check if srcaddr is sub node of local addr 
// return 1 yes , 0 not  .
uint8_t issubnodeaddr(uint16_t srcaddr)
{
	uint16_t result;
	uint8_t  res ;
	// local node must be a tree node .

	//check if fixed header bits is same with the local node .
	res =  globaldata.cfgdata.TreeBitmap &0x0f ;
	res += LeafBitCnt ;
	result = globaldata.cfgdata.LocalAddr ^ srcaddr ;
	result >>= res ;
	return result ? 0: 1;
}
// get the connected sub node addr according to the  address.
// used for send msg down .
// before call this function , you should call issubnodeaddr to make sure , it's sub node of local node 
uint16_t getsubnodeaddr(uint16_t srcaddr)
{
	uint16_t result,temp ;
	uint8_t  res ;

	res = globaldata.cfgdata.TreeBitmap &0x0f ;
	if(res == 0)
	{ // src is a leaf node .
		return srcaddr ;
	}
	// get the header fixed bit of local node
	res += LeafBitCnt ;
	result =globaldata.cfgdata.LocalAddr >> res;
	result <<= NetDividor ;

	// get the ender fixed bit , it is full of 1 . 
	res -= NetDividor;
	temp = 1<< res ;
	temp-- ;


	// get middle NetDividor bit of the srcaddr
	srcaddr >>= res ;
	srcaddr &= NetMask ;
	// combine header fixed bits , middle NetDividor bit of srcaddr , and end full 1 bits. 
	result |=  srcaddr ;
	result <<= res;
	result |=temp ;
	return result ;
}


// check if srcaddr is up node of local addr 
// return 1 yes , 0 not  .
uint8_t isupnodeaddr(uint16_t srcaddr)
{
	uint16_t temp ;
	//check if fixed header bits is same with the local node .
	temp = ~globaldata.cfgdata.LocalAddr ;
	temp = temp^srcaddr ;
	temp |= srcaddr ;
	temp = ~temp ;
	return temp ==0 ? 1:0 ;
}
// get the connected up node addr according to the src address.
// used for send msg up .
// before call this function , you should call isupnodeaddr to make sure it's up node of local node 
uint16_t getfatheraddr()
{
	uint16_t temp ;
	uint8_t  res ;
	if((globaldata.cfgdata.TreeBitmap & 0x80) == 0x80)
	{// leaf node 
		return globaldata.cfgdata.LocalAddr | 0x3f ;
	}
	else
	{
		res = globaldata.cfgdata.TreeBitmap &0x0f ;
        res += LeafBitCnt  ;
        temp = 0x3<<res ;
		return globaldata.cfgdata.LocalAddr | temp ;
	}
}
// is the addr is same layer as local's father . and not it's father
uint8_t isfatherlayeraddr(uint16_t addr) 
{
    uint16_t mask  ,father;
    uint8_t res ;

    res = globaldata.cfgdata.TreeBitmap &0x0f ;
    res +=  LeafBitCnt ;    

	// get father addr . let  n = (Layerbit + LeafBitCnt)
	// leaf node : father node is [headerbits(16-n-2)]:[fatherGrp:2]:[Layerbit + LeafBitCnt]  
	// tree node : father node is [headerbits(16-n-4)]:[fatherGrp:2]:[NetDividor]:[Layerbit + LeafBitCnt] 
	// if the addr is same layer with father : headerbits is same , [Layerbit + LeafBitCnt] is all 1, fatherGrp is differ and not 11 ,NetDividor is all 1

	father =  globaldata.cfgdata.LocalAddr |0x3f ; 
    if((globaldata.cfgdata.TreeBitmap & 0x80) == 0x0)
    { // it tree
		father |= 0x0003<<res ;
		res += NetDividor ;
    }
	// if the father addr is the  root ?
	if(res >= 16)
		return 0 ;
	mask = 0x0003<<res;
	mask = ~mask ;
    // should not same
	if(father == addr)
		return 0 ;
	// all bits except fatherGrp should same .
	father ^= addr ;
	father &= mask ;
	if(father !=0 )
		return 0;
	// addr fatherGrp should not be 11 
	father = addr >> res ;
	father &= 0x03 ;
	if(father == 0x03)
		return 0 ;
	return 1 ;
}
// if the addr is the addr of the closest father addr
uint8_t isfatheraddr(uint16_t addr)
{
    uint16_t temp ;
    uint8_t res ;
    if(globaldata.cfgdata.TreeBitmap & 0x80)
    {// leaf node
        temp = globaldata.cfgdata.LocalAddr |0x3f ;
    }
    else
    {// get father node .
        res = globaldata.cfgdata.TreeBitmap &0x0f ;
        res += LeafBitCnt  ;
        temp = 0x3<<res ;
        temp |= globaldata.cfgdata.LocalAddr ;
    }
	
    temp ^= addr ;
	return temp ==0 ? 1:0 ;    
}
// if the addr is the addr of the closest child addr . the local addr is also think as the child addr of local addr
uint8_t ischildaddr(uint16_t addr) 
{
    uint16_t temp ;
    uint8_t res ;
    if((addr &0x3f) == 0x3f)
    {// addr is tree node
        res = globaldata.cfgdata.TreeBitmap &0x0f ;
        res += (LeafBitCnt - NetDividor) ;
        temp = 0x3<<res ;
        temp |= addr ;        
    }
    else
    {
        temp = addr|0x3f ;
    }
    temp ^= globaldata.cfgdata.LocalAddr ;
	return temp ==0 ? 1:0 ;    
}


// help function to make mac rf msg . ...........................

void makeackmsg(uint8_t  msg,uint8_t isup)// ,uint16_t toaddr)
{
    LPAckMsg  lpmsg ;
    lpmsg = (LPAckMsg)getrfbuf(msg); 
   
    lpmsg->framectrl.MsgCnt = AckMsgSize;
    lpmsg->framectrl.NetId =  globaldata.cfgdata.NetId ;
    //lpmsg->framectrl.VarAddr = toaddr ;
    lpmsg->framectrl.FrameBit = maccmd_ack ;    
    if(isup)
        SetUpMsg(lpmsg->framectrl.FrameBit);    //  value =0 , need not set .    
}

void makejoinmsg(uint8_t  msg ,uint16_t newaddr) // ,uint16_t toaddr)
{// must be send from father to child
    LPJoinMsg  lpmsg ;
    lpmsg = (LPJoinMsg)getrfbuf(msg); 
   
    lpmsg->framectrl.MsgCnt = JoinMsgSize;
    lpmsg->framectrl.NetId =  globaldata.cfgdata.NetId ;
    //lpmsg->framectrl.VarAddr = toaddr ;
    lpmsg->framectrl.FrameBit = maccmd_join ;    
    //SetDownMsg(lpmsg->framectrl.FrameBit);    //  value =0 , need not set .    
    lpmsg->addr = newaddr ;
}
void analysisjoinmsg(LPFrameCtrl  lpbuf)
{// must be receive from father .
    LPJoinMsg  lpmsg ;
    lpmsg = (LPJoinMsg)lpbuf ; 
    setaddrcfg(lpmsg->addr);
    ResetLiveCnt(FatherTreeOffset);
}
// help function to make a msg .
void makebrcsttreeanswer(uint8_t msg)//,uint16_t toaddr)
{// send from father to child tree.
    LPBroadcastMsg lpmsg = (LPBroadcastMsg)getrfbuf(msg);
    
    lpmsg->framectrl.MsgCnt = BroadcastMsgSize;
    lpmsg->framectrl.NetId =  globaldata.cfgdata.NetId ;
    //lpmsg->framectrl.VarAddr = toaddr ;
    lpmsg->framectrl.FrameBit = maccmd_broadcast ;    
    // SetDownMsg(lpmsg->framectrl.FrameBit);      value =0 , need not set .    
    // get net deep [0-4] ,its net bit /2
    msg = globaldata.cfgdata.TreeBitmap&0x0e ;
    lpmsg->nodeinfo= msg+1 ;  // put in bit[6-4] 
    msg = 0 ;
    // calc local tree free child cnt [0-3]
    if((globaldata.cfgdata.TreeBitmap & 0x10) ==0 )
        msg++;
    if((globaldata.cfgdata.TreeBitmap & 0x20) ==0 )
        msg++;
    if((globaldata.cfgdata.TreeBitmap & 0x40) ==0 )
        msg++;
    lpmsg->nodeinfo *= msg ;    
    lpmsg->recvrssi = GetRssiInst();    
}

void makebrcstleafanswer(uint8_t msg)//,uint16_t toaddr)
{// send from father to leaf .
    LPBroadcastMsg lpmsg = (LPBroadcastMsg)getrfbuf(msg);
    
    lpmsg->framectrl.MsgCnt = BroadcastMsgSize;
    lpmsg->framectrl.NetId =  globaldata.cfgdata.NetId ;
    //lpmsg->framectrl.VarAddr = toaddr ;
    lpmsg->framectrl.FrameBit = maccmd_broadcast ;    
    // SetDownMsg(lpmsg->framectrl.FrameBit);      value =0 , need not set .    
    // get net deep [0-4] ,its net bit /2
    msg = globaldata.cfgdata.TreeBitmap&0x0e ;
    lpmsg->nodeinfo = msg +1;    
    
    // cacl free child count  based on 4 .
    msg = CaclBitCnt32(globaldata.cfgdata.LeafBitmapH);
    // highest bit is not used .
    msg-- ;
    msg+= CaclBitCnt32(globaldata.cfgdata.LeafBitmapL);
    msg>>=3 ;
    
    lpmsg->nodeinfo *= msg ;
    lpmsg->recvrssi = GetRssiInst();   
}
//  recv the braodcast reply msg ,  get the rssi and send addr , check if the new rssi is small than the old one 
//  then use the smallest  replace the old one  ,store them in NearTreeNode1@NearTreeRssi1 
//  
void analysisbrcstreply(uint8_t *lpbuf)
{// receive from father 
    LPBroadcastMsg lpmsg = (LPBroadcastMsg)lpbuf;
    lpmsg->nodeinfo &= 0x7f;
    if(globaldata.cfgdata.NearTreeRssi1 == 0xff)
    {
        globaldata.cfgdata.NearTreeNode1 = lpmsg->framectrl.SendAddr ;
        globaldata.cfgdata.NearTreeRssi1 = lpmsg->recvrssi ;
        globaldata.cfgdata.NearTreeInfor1 = lpmsg->nodeinfo ;
    }
    else if(globaldata.cfgdata.NearTreeRssi2 == 0xff)
    {
        globaldata.cfgdata.NearTreeNode2 = lpmsg->framectrl.SendAddr ;
        globaldata.cfgdata.NearTreeRssi2 = lpmsg->recvrssi ;
        globaldata.cfgdata.NearTreeInfor2 = lpmsg->nodeinfo ;
    }
    else
    {   // replace the small one .
        if(globaldata.cfgdata.NearTreeRssi2 > globaldata.cfgdata.NearTreeRssi1)
        { // 2>1 
            if(globaldata.cfgdata.NearTreeRssi2 < lpmsg->recvrssi )
            {
                globaldata.cfgdata.NearTreeRssi1 = globaldata.cfgdata.NearTreeRssi2 ;
                globaldata.cfgdata.NearTreeNode1 = globaldata.cfgdata.NearTreeNode2 ;  
                globaldata.cfgdata.NearTreeInfor1 = globaldata.cfgdata.NearTreeInfor2;                
                globaldata.cfgdata.NearTreeRssi2 = lpmsg->recvrssi ;
                globaldata.cfgdata.NearTreeNode2 = lpmsg->framectrl.SendAddr ;
                globaldata.cfgdata.NearTreeInfor2 = lpmsg->nodeinfo;
            }
            else if (globaldata.cfgdata.NearTreeRssi2 == lpmsg->recvrssi )
            {
                if(globaldata.cfgdata.NearTreeInfor2 < lpmsg->nodeinfo)
                {
                    globaldata.cfgdata.NearTreeRssi1 = globaldata.cfgdata.NearTreeRssi2 ;
                    globaldata.cfgdata.NearTreeNode1 = globaldata.cfgdata.NearTreeNode2 ;   
                    globaldata.cfgdata.NearTreeInfor1 = globaldata.cfgdata.NearTreeInfor2;                     
                    globaldata.cfgdata.NearTreeRssi2 = lpmsg->recvrssi ;
                    globaldata.cfgdata.NearTreeNode2 = lpmsg->framectrl.SendAddr ;
                    globaldata.cfgdata.NearTreeInfor2 = lpmsg->nodeinfo;
                }
                else
                {
                    globaldata.cfgdata.NearTreeRssi1 = lpmsg->recvrssi ;
                    globaldata.cfgdata.NearTreeNode1 = lpmsg->framectrl.SendAddr ;  
                    globaldata.cfgdata.NearTreeInfor1 = lpmsg->nodeinfo;    
                }
            }
            else if (globaldata.cfgdata.NearTreeRssi1 < lpmsg->recvrssi )
            {
                globaldata.cfgdata.NearTreeRssi1 = lpmsg->recvrssi ;
                globaldata.cfgdata.NearTreeNode1 = lpmsg->framectrl.SendAddr ;
                globaldata.cfgdata.NearTreeInfor1 = lpmsg->nodeinfo;     
            }
            else if (globaldata.cfgdata.NearTreeRssi1 == lpmsg->recvrssi )
            {
                if(globaldata.cfgdata.NearTreeInfor1 < lpmsg->nodeinfo)
                {
                    globaldata.cfgdata.NearTreeRssi1 = globaldata.cfgdata.NearTreeRssi2 ;
                    globaldata.cfgdata.NearTreeNode1 = globaldata.cfgdata.NearTreeNode2 ;  
                    globaldata.cfgdata.NearTreeInfor1 = lpmsg->nodeinfo;    
                }
            }            
        }
        else
        {  // 1>2
            if(globaldata.cfgdata.NearTreeRssi1 < lpmsg->recvrssi )
            {
                globaldata.cfgdata.NearTreeRssi2 = globaldata.cfgdata.NearTreeRssi1 ;
                globaldata.cfgdata.NearTreeNode2 = globaldata.cfgdata.NearTreeNode1 ;
                globaldata.cfgdata.NearTreeInfor2 = globaldata.cfgdata.NearTreeInfor1;                    
                globaldata.cfgdata.NearTreeRssi1 = lpmsg->recvrssi ;
                globaldata.cfgdata.NearTreeNode1 = lpmsg->framectrl.SendAddr ;
                globaldata.cfgdata.NearTreeInfor1 = lpmsg->nodeinfo;    
            }
            else if(globaldata.cfgdata.NearTreeRssi1 == lpmsg->recvrssi )
            {
                if(globaldata.cfgdata.NearTreeInfor1 < lpmsg->recvrssi)
                {
                    globaldata.cfgdata.NearTreeRssi2 = globaldata.cfgdata.NearTreeRssi1 ;
                    globaldata.cfgdata.NearTreeNode2 = globaldata.cfgdata.NearTreeNode1 ;
                    globaldata.cfgdata.NearTreeInfor2 = globaldata.cfgdata.NearTreeInfor1;                    
                    globaldata.cfgdata.NearTreeRssi1 = lpmsg->recvrssi ;
                    globaldata.cfgdata.NearTreeNode1 = lpmsg->framectrl.SendAddr ;
                    globaldata.cfgdata.NearTreeInfor1 = lpmsg->nodeinfo;                
                }
                else
                {
                    globaldata.cfgdata.NearTreeRssi2 = lpmsg->recvrssi ;
                    globaldata.cfgdata.NearTreeNode2 = lpmsg->framectrl.SendAddr ;
                    globaldata.cfgdata.NearTreeInfor2 = lpmsg->nodeinfo;                 
                }
            }                
            else if (globaldata.cfgdata.NearTreeRssi2 <lpmsg->recvrssi )
            {
                    globaldata.cfgdata.NearTreeRssi2 = lpmsg->recvrssi ;
                    globaldata.cfgdata.NearTreeNode2 = lpmsg->framectrl.SendAddr ;
                    globaldata.cfgdata.NearTreeInfor2 = lpmsg->nodeinfo;            
            } 
            else if (globaldata.cfgdata.NearTreeRssi2 ==lpmsg->recvrssi )
            {
                if(globaldata.cfgdata.NearTreeInfor2 < lpmsg->nodeinfo )
                {
                    globaldata.cfgdata.NearTreeRssi2 = lpmsg->recvrssi ;
                    globaldata.cfgdata.NearTreeNode2 = lpmsg->framectrl.SendAddr ;
                    globaldata.cfgdata.NearTreeInfor2 = lpmsg->nodeinfo;            
                }
            }    
        }
    }
}




 //  rf receive a new msg . check it : if the msg is a ack msg , remove the wait queue  if there is . else do mac proc 
 //  this function may create a StdEvt in direct queue  that need to send out directly .
void MacRecvProc(uint8_t memid) 
{
    RfMsg msg ;
    uint8_t   temp8 ;
    LPFrameCtrl  lpframe ;
    lpframe = (LPFrameCtrl)getrfbuf(memid);
    msg.value.memid = GetMacCmd(lpframe->FrameBit) ;
    if(msg.value.memid == maccmd_none )
    {
        temp8 = lpframe->MsgId ;
#if Nodetype != RootNode
        // this is a data msg . check if send to local , discard if not 
        if(IsDownMsg(lpframe->FrameBit)) 
        {// msg is send from root to a child node .  Varaddr is the  node addr   
            if(isfatheraddr(lpframe->SendAddr))
            {
                ResetFatherLiveCnt();
                if(lpframe->VarAddr == globaldata.cfgdata.LocalAddr)
                {   // recv msg to recv list .   send event to net process.
                    if(addrecvmsg(memid,lpframe->SendAddr))
                        postevtbyindex(BlcId_Net,makeevt(Sig_Rf_Recv_Msg,0));  
                    // check if it has ack flage , it also means it has received a message send from local .
                    if(IsAuxAckMsg(lpframe->FrameBit)) 
                         rmonlineid(GetAuxAckMemId(lpframe->FrameBit)) ;      
                    // check if there is a online message need to be send to this node .
                    msg = findonlineaddr(lpframe->SendAddr);
                    if(msg.value.memid == InvalidId)
                    { // no direct msg need to send , send ack.
                        msg.value.memid = newrfbuf(lpframe->SendAddr); 
                        makeackmsg(msg.value.memid,1);
                    }
                    else
                    { // send direct msg ,with aux flage.
                        lpframe = (LPFrameCtrl)getrfbuf(msg.value.memid) ;
                        SetAuxAckMsg(lpframe->FrameBit);
                        SetAuxAckMemId(lpframe->FrameBit,temp8);
                    }
                    //send it out .
                    SendMsg(msg);                    
                }
                else if(issubnodeaddr(lpframe->VarAddr))
                { // msg is send to child node . put is in offline list .
                    
                    addofflinemsg(memid,lpframe->VarAddr);
                    // check if it has ack flage , it also means it has received a message send from local .
                    if(IsAuxAckMsg(lpframe->FrameBit)) 
                         rmonlineid(lpframe->MsgId) ;      
                    // check if there is a online message need to be send to this node .
                    msg = findonlineaddr(lpframe->SendAddr);
                    if(msg.value.memid == InvalidId)
                    { // no direct msg need to send , send ack.
                        msg.value.memid = newrfbuf(lpframe->SendAddr); 
                        makeackmsg(msg.value.memid,1);
                    }
                    else
                    { // send direct msg ,with aux flage.
                        lpframe = (LPFrameCtrl)getrfbuf(msg.value.memid) ;
                        SetAuxAckMsg(lpframe->FrameBit);
                    }
                    //send it out .
                    SendMsg(msg);                    
                }
                else //  should never come here , it means the father send a down msg  but not to it's child
                    freerfbuf(memid);
            }
            else  // discard it
                freerfbuf(msgid);
            return ;
        }
        else
        { // send to root . varaddr is the sender's addr , sender should be child addr
#endif        
            if(ischildaddr(lpframe->SendAddr))
            {
               #if Nodetype  == RootNode   
                    #if  Use_Big_Evt  == 0                  
                    temp.data16.data0 = makeevt(Sig_Rf_Recv_Msg,msg);
                    postevtbyindex(BlcId_Net,temp.data16.data0);  
                    #else
                    temp.data32 = makeevt(Sig_Rf_Recv_Msg,msg);
                    postevtbyindex(BlcId_Net,temp.data32);    
                    #endif    
                    setnetidtoaddr(lpframe->VarAddr,lpframe->NetId)    ;
               #else 
                    ResetLiveCntByAddr(lpframe->SendAddr); 
                    // send this msg to father node .
                    temp.data16.data1 = getfatheraddr();    
                    SendRfMsg(msg,temp.data16.data1);
                    // prepare a reply msg to child.
                    if(IsAuxAckMsg(lpframe->FrameBit)) 
                         rmofflineaddr(lpframe->SendAddr) ;
                    // try to find a cmd need to this child .
                    temp = rmonlineaddr(lpframe->SendAddr);
                    temp.data16.data1 = lpframe->SendAddr ;
                    if(temp.data8.data0 == InvalidId)
                    {
                        temp.data8.data0 = newrfbuf(temp.data16.data1); 
                        makeackmsg(temp.data8.data0,0);
                    }
                    else
                    {
                        lpframe = (LPFrameCtrl)getrfbuf( temp.data8.data0);
                        SetAuxAckMsg(lpframe->FrameBit);
                    }
                    SendRfMsgDirect(temp.data8.data0,temp.data16.data1);                 
               #endif
            
            }
            else  // discard it
                freerfbuf(msg);                
        }
        return ;
    }
    else
    {   // mac cmd .send from father or child node  , varaddr should be aim addr 
        if(temp.data8.data0 == maccmd_broadcast)
        {
            if(IsDownMsg(lpframe->FrameBit))
            { // the msg is  tree reply the braodcast to child .  local is child
              // put the received addr  and rssi to NearTreeNode1/2 and NearTreeRssi1/2 .
              // the netthd will start register proc to send  maccmd_broadcast  with direction is up . and offline 4second
              // the reachable tree will return this message .  here to store the useable tree node .
              // after 4 second , the braodcast proc will use the lowest rssi to send jion message .      
              // for broad cast . node should receive only the tree node , that is at the layer  it's same with it's father. 
                if(rflayerdata.netmode >= net_gettime) 
                { 
                    if(isfatherlayeraddr(lpframe->SendAddr) == 0)
                    {
                        freerfbuf(msg);
                        return ;
                    }
                }                
                analysisbrcstreply((uint8_t *)lpframe);
                freerfbuf(msg);
            }
            else
            { //the msg is child send ,recv by father ,
                if(lpframe->SendAddr == OrphanTreeAddr)
                { //sender is  a tree node 
                    if(ishasfreechild())
                    {
                        makebrcsttreeanswer(msg);
                        SendRfMsgDirect(msg ,lpframe->SendAddr);
                    }
                    else
                        freerfbuf(msg);
                }
                else 
                {// sender is a leaf .
                    if(ishasfreeleaf())
                    {
                        makebrcstleafanswer(msg);
                        SendRfMsgDirect(msg,lpframe->SendAddr);
                    }
                    else
                        freerfbuf(msg);
                }
            }
            return ;
        }
        else 
        { // other mac cmd , the local node should has registered . and the local addr should be not orphan addr
            
            if(lpframe->VarAddr != globaldata.cfgdata.LocalAddr)
            {
                freerfbuf(msg);
                return ;
            }
            if(temp.data8.data0 == maccmd_ack)
            {
                rmofflineaddr(lpframe->SendAddr);
                freerfbuf(msg);
            }
            else if(temp.data8.data0 == maccmd_join)
            { 
                if(IsDownMsg(lpframe->FrameBit))
                { // this is send from tree node . register ok . send  Sig_Rf_Register_OK
                    analysisjoinmsg(lpframe);
                    postevtbyindex(BlcId_Net,Sig_Rf_Register_OK);
                    temp.data16.data1 = lpframe->SendAddr;
                    lpframe = (LPFrameCtrl)getrfbuf( msg);
                    makejoinmsg(msg,0);
                    SetAuxAckMsg(lpframe->FrameBit);
                    SendRfMsgDirect(msg,temp.data16.data1);
                    return ;
                }
                else
                {  // jion is start by  RfRegisterProc  , check if there is free addr ,and return to it .
                    if(lpframe->SendAddr == OrphanTreeAddr)
                    { //sender is  a tree node 
                        temp.data16.data1 = findchildaddr();
                        if(temp.data16.data1!= 0xffff)
                        {
                            makejoinmsg(msg,temp.data16.data1); 
                            SendRfMsgDirect(msg,OrphanTreeAddr);
                        }
                        else
                            freerfbuf(msg);
                    }
                    else if(lpframe->SendAddr == OrphanLeafAddr)
                    {// sender is leaf node
                        temp.data16.data1 = findleafaddr();
                        if(temp.data16.data1!= 0xffff)
                        {
                            makejoinmsg(msg,temp.data16.data1); 
                            SendRfMsgDirect(msg,lpframe->SendAddr);
                        }
                        else
                            freerfbuf(msg);
                    }
                    else 
                    { // receive a reply from child .
                        if(IsAuxAckMsg(lpframe->FrameBit))
                            markaddralloced(lpframe->SendAddr);
                        freerfbuf(msg);
                    }
                    return ;                    
                }
            }
            else if(temp.data8.data0 == maccmd_time)
            { // child need to get time from father .
                if(IsDownMsg(lpframe->FrameBit))
                { // child receive  time sended by father.
                    postevtbyindex(BlcId_Net,Sig_Rf_GetTime_OK);
                    resetrtcfreeday();
                    temp.data32 = *((uint32_t *) (lpframe+1));
                    ChangRtcTime(temp.data32);
                    freerfbuf(msg);
                    ResetActiveCnt();
                }
                else
                {// father reciev get time asking from child .
                    lpframe->FrameBit = maccmd_time ;
                    lpframe->MsgCnt  = RtcMsgSize ;
                    lpframe->VarAddr = lpframe->SendAddr;
                    *((uint32_t *) (lpframe+=1)) =RtcGetTime();
                    SendRfMsgDirect(msg,lpframe->SendAddr);                    
                }
                return ;
            }
            else if(temp.data8.data0 == maccmd_checkoffline)
            { // child need to get msg from father .  this cmd is period send by child to get possible incoming msg .
              // the online msg bit is also been send when there is a msg send to father node .  
                if(IsDownMsg(lpframe->FrameBit))
                { // child receive  reply  sended by father. there is a online msg .
                    ResetActiveCnt();
                    // send to check offline msg proc  , stop it .
                    postevtbyindex(BlcId_Net,makeevt(Sig_Rf_CheckOffline_OK,msg);
                    if(IsAuxAckMsg(lpframe->FrameBit))
                    { // there is a new msg come . send to high layer .
                        postevtbyindex(BlcId_Net,makeevt(Sig_Rf_Recv_Msg,msg));    
                        msg = newrfbuf(lpframe->SendAddr); 
                        makeackmsg(msg,1);
                        SendRfMsgDirect(msg,lpframe->SendAddr);                     
                    }
                    else
                    {
                        postevtbyindex(BlcId_Net,makeevt(Sig_Rf_No_Msg,0)); 
                        freerfbuf(msg);
                    }
                    return ;
                }
                else
                {// father reciev check online asking from child .
                    // return the online msg if there is , else return ack msg .
                    var = getofflineaddr(lpframe->SendAddr);
                    if(var.value.memid == InvalidId)
                    {
                        makeackmsg(var.value.memid,0);
                    }
                    else
                    {
                        setonlinemsg(var.value.memid,var,value.addr);
                        freerfbuf(msg);................
                        lpframe = (LPFrameCtrl)getrfbuf( temp.data8.data0) ;
                        SetAuxAckMsg(lpframe->FrameBit);                          
                    }
                    SendRfMsgDirect(temp.data8.data0,lpframe->SendAddr);                    
                }
                return ;
            }            
            else
            {
                freerfbuf(msg);
                return ;
            }
        }
    }
}
