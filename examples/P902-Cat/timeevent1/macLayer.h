#ifndef __mac_layer_H 
#define __mac_layer_H 

#include "basictype.h"
#include "evtos.h"
// define a init value to flash data  .  in flash the value should be fixed as it
#define FlashValue  0xcdcd    // if the Mark is this means the cfginformation is just read from flash
#define RamValue    0xdcdc    // if the Mark is this means the cfginformation is changed by app. it's valid data.

#define DefaultNetName   0x34ef  //the default net name .
#define InvalidNetId     0xffff  //invalid net id 
// CfgInformation is stored at fixed ram address . it is limited to 44 byte . and it must assign as 32Bit .
typedef  struct _CfgInformation_
{
    // uint32_t  1
    uint16_t RFFrequency;  // frequncy based on 100KHz
    int8_t   Power;
    uint8_t  SignalBw; // LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                             // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved] below 6 is limited for use .  
     // uint32_t  2   
    uint8_t  SpreadingFactor;  // LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    uint8_t  ErrorCoding;      // LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    uint16_t NetId;      // hold the address from father node .
    
    
    // uint32_t   3 
    uint16_t NetName ;     // fixed name to mark this net .
    uint8_t  PreambleNum ; 
    uint8_t  ActivePeriod ;   //max hour cnt ,for  child to send msg to father ,check if there is msg wait send down.
    // next is router data. that is not read from flash . default is all zero
    
    
    
    // uint32_t  4
    uint32_t LeafBitmapH ;  // for tree node to mark the assigned leaf node 1-31 , bit 0 is not used
                            // a LeafNodeLiveCnt connect with this 
    // uint32_t  5
    uint32_t LeafBitmapL ;  // for tree node to mark the assigned leaf node 32-62 , bit 31 is not used
                            // a LeafNodeLiveCnt connect with this 
    // uint32_t  6 
    uint16_t LocalAddr ;     // for tree node addr .
    uint16_t Mark;           // no used to mark if the struct is valid ,
    
    // uint32_t  7   
    uint16_t NearTreeNode1 ; // two  the closed (most bigger Rssi) tree node addr ,used to as a reserve father tree node . 
    uint16_t NearTreeNode2 ; 
    
    // uint32_t  8
    uint8_t  NearTreeRssi1  ;// rssi low 2bit is not used .
    uint8_t  NearTreeInfor1 ;// net layer deep cnt(TreeBitmap&f /2)*     free child(/4 leaf when bit7 is 1 ,tree when bit7 is 0) count,
    uint8_t  NearTreeRssi2  ;// rssi low 2bit is not used .
    uint8_t  NearTreeInfor2 ;// net layer deep cnt(TreeBitmap&f /2)*     free child(/4 leaf when bit7 is 1 ,tree when bit7 is 0) count,
    // uint32_t  9

    uint8_t  TreeBitmap ;   //bit7 1 :is leaf ,0 is tree, bit3-0 : is tree freebit cnt ,  bit6-4 is child tree node bitmap ;
                            //there is a uint8_t ChildNodeLiveCnt [3] connect with it .     
    uint8_t  res1 ;
    uint8_t  res2;
    uint8_t  res3 ;
    // uint32_t  10
    // uint32_t  11

} CfgInformation ,* LPCfgInformation ;  

// sub leaf live cnt
#if Nodetype != LeafNode
void  ResetLeafLiveCnt(uint8_t index);
void  ResetTreeLiveCnt(uint8_t index);
void  SubChildNodeLiveCnt(void);
#endif
#if Nodetype != RootNode
// sub father live cnt , return 1 ,if father live cnt is not 0 ,else return 0 
void  SubFatherLiveCnt(void) ;
void  ResetFatherLiveCnt(void);
#endif

// sub active cnt ,if it return 1 -> active period is overtimed.
uint8_t SubActiveCnt(void);
void ResetActiveCnt(void) ;    


// the net is a tree - leaf net .    tree node connect each other to make a net . and the leaf node connected with tree node only
// there is also a specail node . it is the root node , all msg should be send to root node . 
// tree node  addr  is only TreeAddrMask bits used is must start from 1. the LeafAddrMask is fixed as 0
// for leaf node  addr ,  the TreeAddrMask  bit is same as  the addr of the tree node  it connected , the LeafAddrMask is free
// for root node it's addr is fixed as 0 



// notice the last addr for all node is to root node . so we do not need to mark it 
// the recvaddr and the send addr is used to jump from tree node to root  

// for tree  node the  default addr (orphan addr) is TreeAddrMask , it is tree node but the TreeAddrMask addr will never be assign to any tree node
// when a tree node  send msg use default addr it means it want to jion the net , the msg will be send to root node , it will assign a addr to it .    

// for leaf  node the  default addr (orphan addr) is LeafAddrMask  ,it is not belong to any tree node 
// when a leaf node send msg use default addr it means it want to jion the net , the msg will be answered by the tree node , it will assign a addr to it .    

// there maybe more than 1 root in a net , and if so these roots must have quickly way to connect with each other to aviod assign same tree node addr to different node .

#define  RootAddr   0xffff
#define  RootCfg    0x0A 
#define  NetDividor 2         //every net has 3 subber child tree node .
#define  NetMask    0x03     
#define  LeafBitCnt  6        //every net has 63 leaf node
#define  OrphanTreeAddr          0x97bf    // default value when start .  this is  a tree  addr that never used .
#define  OrphanLeafAddr          0x9770    // default value when start .  this is  a leaf  addr that never used .



// return if current node is a tree node , 1 :yes , 0 : no .
// bitmap must be set before anything . it can't be chang by programe .
uint8_t istreenode(void);
// set the localaddress and bitmap from addr alloced by father .
//addr is a addr and fix bit cnt  togather data .
void setaddrcfg(uint16_t addr);

// alloc a unused localcfg for child tree node .  addr is addr | fix bit cnt .
// return 0xffff if there is no child tree addr .
uint16_t allocchildaddr(void);

// return a leaf addr of this node 
// return 0xffff if there is no free leaf address or the current node  is not a tree node .
uint16_t allocleafaddr(void);

// is current addr has free child addr
uint8_t  ishasfreechild(void);

// free a used localcfg for child tree node  ,return 1 :removed . 0 : invalid addr
uint8_t  freesubnode(uint16_t childaddr);

// check if srcaddr is sub node of local addr 
// return 1 yes , 0 not  .
uint8_t issubnodeaddr(uint16_t srcaddr);

// get the connected sub node addr according to the arm address.
// used for send msg down .
// before call this function , you should call issubnodeaddr to make sure , it's sub node of local node 
uint16_t getsubnodeaddr(uint16_t srcaddr);
// check if srcaddr is up node of local addr 
// return 1 yes , 0 not  .
uint8_t isupnodeaddr(uint16_t srcaddr);

// get the connected up node addr according to the src address.
// used for send msg up .
uint16_t getfatheraddr(void);

// if the addr is the same layer with local's father node
uint8_t isfatherlayeraddr(uint16_t addr) ;
// if the addr is the addr of the closest father addr
uint8_t isfatheraddr(uint16_t addr) ;
// if the addr is the addr of the closest child addr 
uint8_t ischildaddr(uint16_t addr) ;
#if Nodetype == TreeNode 
__STATIC_INLINE uint8_t isorphannode(uint16_t addr) 
{
    return addr == OrphanTreeAddr ? 1 : 0 ;
}

#elif Nodetype == LeafNode

__STATIC_INLINE uint8_t IsOrphanNode(uint16_t addr) 
{
    return addr == OrphanLeafAddr ? 1 : 0 ;
}
#endif

// BIT 0- 7 is the mem id ,from the sender . it 's valid when AuxAckMask is set,

// BIT8 - Bit11    is used for Mac layer ,: if FrameBit & MacMsgMask != 0  -> this is a mac msg .
#define  MacMsgMask       (BIT8|BIT9|BIT10|BIT11)   // the leaf node look for the tree node  to jion the net,

#define  DirectionMask    BIT12   // 1: up the msg is Send to root ,0 : Down the msg is send from root.
#define  AuxAckMask       BIT13   // the sender waiting a ack msg . and there also a online msg to it . the online msg also means a ack.


typedef enum MacMsgType_
{
    maccmd_none = 0 ,  // this is not a mac cmd . it must is sent to local .
    maccmd_broadcast,  // if DirectionMask=1 up orphan tree/leaf braodcast to find closest tree node . addr must be OrphanTreeAddr or OrphanLeafAddr
                       // if DirectionMask=0 down  tree reply the braodcast to child . BroadcastMsg
    maccmd_join ,      // orphan tree/leaf ask jion tree node . addr must be LeafAddrMask.
    maccmd_ack   ,     // this is a ack . 
    maccmd_time ,      // ask to ge time. 
    maccmd_checkoffline ,  // send by child ,check if there is a online msg offlineing to send 
    
}MacMsgType ;
#define GetMacCmd(framebit) ((uint8_t) ((framebit)&MacMsgMask))
#define IsMacMsg(framebit) (((framebit)& MacMsgMask) !=0)

// mark msg is send from father node to child node .
#define IsDownMsg(framebit)(((framebit)&DirectionMask)==0)
#define SetUpMsg(framebit) (framebit)|=DirectionMask 
#define SetDownMsg(framebit) (framebit)&=(~DirectionMask) 

// mark this msg is also a ack msg for lastest sended msg .   it means  1 : the lastest msg send has received . 2 there is a new msg come .
// when it set the lower byte of framebit is the memid of the last sended msg .
#define IsAuxAckMsg(framebit) (((framebit)&AuxAckMask)==AuxAckMask)
#define SetAuxAckMsg(framebit) (framebit)|=AuxAckMask
#define GetAuxAckMemId(framebit) ((uint8_t)framebit)
#define SetAuxAckMemId(framebit,id) (framebit) &= 0xff00 ,(framebit) |= id

// the start struct of the rf msg .  this struct is used mainly at mac layer.
// TryCnt must at 0 offset ,and MsgCnt must at 1offset . 
// the WriteFifo and getofflineevt  used absolute address of this .
// total 12 byte used .

//TryCnt and MsgCnt is used inter , not be sendout 
#define MsgOffset    1 
typedef struct FrameCtrl_   
{
    uint8_t  MsgCnt   ;    // the msg length , not include the SendCnt and MsgCnt .
    uint8_t  MsgId    ;    // a inter counter used to give a id for Send a msg , the return ack msg would has same id  to notify which cmd received . it 's used for data msg only.
    uint16_t NetName  ;    // mark the net . fixed. stored in    autoset .
    uint16_t NetId    ;    // mark the real Id of the node ;
    uint16_t SendAddr ;    // the local addr the msg is send from .     autoset .
    uint16_t VarAddr  ;    // the real addr the msg  is from (DirectionMask = 0 ),   or send to (DirectionMask =1 )    
    uint16_t FrameBit ;    // a serial of bit  mark the detail information about the msg

}FrameCtrl ,* LPFrameCtrl ;

#define NetNameOffset 2
#define NetIdOffset  4 
#define FrameBitOffset 6
#define VarAddrOffset 8

#define MACHeaderSize   sizeof(FrameCtrl)
 

// all item in RtcTime is based on 0 , except the year is based on 2000
typedef struct RtcTime_
{
    uint32_t sec :6 ;
    uint32_t min :6 ;
    uint32_t hour : 5 ;
    uint32_t day  : 5 ;
    uint32_t month : 4 ;
    uint32_t year : 6 ;
}RtcTime ;
#


typedef struct AckMsg_
{
    FrameCtrl framectrl ;

}AckMsg ,* LPAckMsg ;
#define AckMsgSize  (sizeof(AckMsg) -MsgOffset)
    
typedef struct RtcMsg_
{
    FrameCtrl framectrl ;
    uint32_t   time ;    // marked as RtcTime    
}RtckMsg ,* LPRtcMsg ;
#define RtcMsgSize  (sizeof(RtckMsg) -MsgOffset)

typedef struct JoinMsg_
{
    FrameCtrl framectrl ;
    uint16_t  addr ;    // the addr from father node .
    uint8_t   istree ;  // the addr is for tree node or leaf node .
}JoinMsg ,* LPJoinMsg ;
#define JoinMsgSize  (sizeof(JoinMsg) -MsgOffset)

// rssi differ small than RSSI_OFFSET , will be think as same level.
#define RSSI_OFFSET   5 
// the BroadcastMsg return the father's addr ,rssi, net layer deep cnt and free child cnt 
// the child will compare the rssi, net layer deep cnt and free child cnt to select a fitable one as it's father node 
// the select ruler is  : good rssi (it maybe div by recv frequncy )> more free child > less net layer deep cnt is better than other
typedef struct BroadcastMsg_
{
    FrameCtrl framectrl ;
    uint8_t   nodeinfo ;      //net layer deep cnt(TreeBitmap&f /2),  *free child(/4 leaf when bit7 is 1 ,tree when bit7 is 0) count,
    uint8_t   recvrssi ;        // the rssi tree node reiceived .
}BroadcastMsg ,* LPBroadcastMsg ;
#define BroadcastMsgSize  (sizeof(BroadcastMsg) -MsgOffset)

//Mac is drived by Rf Interrupt .  the data and inter function is defined here 
// max rf msg  mac can hold  , for safe use cnt based 4 byte. it should small then the PoolRF_Cnt
#define  Max_RfLayerMem     PoolRF_Cnt

// max register delay cnt   when register the node will set delay time as n*4 seconds max delay time is  Max_RegisterCnt*4 second
#define  Max_RegisterCnt   0x50

// max RTC free running day cnt   when it reached , update rtc time by rf .
#define  Max_RtcFreeCnt    0x10

typedef enum NetMODE_
{
    net_init = 0 ,    // rf just init . 
    net_register ,   // rf is trying to jion to the net .
    net_gettime ,    // node has geted time , it trying to get rtc time .
    net_monitor ,    // node has finished  jion to net  , it's try to  send or recv msg
    net_idle,        // node has finished  jion to net  , it's in sleep mode ,stop recv or send msg .
}NetMode ;

typedef enum RfMODE_
{
    rf_init = 0 ,    // rf just init . 
    rf_cad_detect ,   // rf is trying to start work.
    rf_tx_snd ,    // node has geted time , it trying to get rtc time .
    rf_rx_dc ,    // node has finished  jion to net  , it's try to  send or recv msg
    rf_tx_rcv,        // node has finished  jion to net  , it's in sleep mode ,stop recv or send msg .
}RfMode ;

// memarray is the all usable rf buffer , it's count is PoolRF_Cnt , length if PoolRF_Size *4 
typedef  struct RfValue_
{
    uint8_t  nextid ; // store the index of next item .
    uint8_t  memid   ;   // store the index  in memarray
    uint16_t addr ;   // store the msg send to addr     
} RfValue;

typedef union RfMsg_
{
    RfValue value;
    Var32   var ;
}RfMsg ;

// msg in online is use for tree node and Root node conminicate , will try to send MsgMaxSendCnt times , if no ack received , discard it , and post a  Sig_Rf_Overtime event to net layer .
// if received a  ack , this msg will be take out from online list .
// msg that need to send to leaf node , should be put in offline list , offline the leaf send msg to local , than local can send msg in offline list to it .

#define MsgMaxSendCnt   4      // max send times for online msg .
#define MsgMaxWaitCnt   48     // max wait hours 
// RfLayerData hold the all rf buffer , and three msg list
// 1 : the recv send msg list ,    it is used as mac msg , don't need to wait the replay .
// 2 : the online send msg list ,  it is used as data msg , it need to check ack msg should be received from pear to make sure it's received by pear 
// 3 : the offline list , it is the the offline send msg(for leaf node) , that has been received by pear , and the pear should return a return msg
typedef struct RfLayerData_
{
    // total RF buffer pool
    uint32_t memarray[PoolRF_Cnt*PoolRF_Size];
    // total local msg .
    RfMsg   msglist[Max_RfLayerMem] ;
    uint8_t msglivecnt[Max_RfLayerMem];
    
    volatile uint8_t  memfree;   // hold the first free item id in memarray    
    volatile uint8_t  freeheader ;   // hold first  unused item 
    volatile uint8_t  recvheader ; // hold first  recv item in recv send list
    volatile uint8_t  onlineheader ; // hold first online item in online send list
    
    volatile uint8_t  offlineheader ;    // hold first Wait Ack item in Wait Ack list
    // when send msg , it will try to send item in loop mode . give all msg fair send time ,to avoid a msg 's dead recver block send and receiv 
    volatile uint8_t  onlinecur ; // hold cur online item in online send list
    volatile uint8_t  onlineender ; // hold last online item in online send list   
    volatile uint8_t  recvender ; // hold last recv item in online send list   


	volatile RfMode   rfmode ;  // hold the lower hardware work mode .
    volatile NetMode  netmode  ;    // hold the Net Mode.
    uint8_t  rtcfreeday;        // hold the rtc running day count from last time get rtc from rf.
    // hold the rf param here .  used when
} RfLayerData ;

extern  RfLayerData rflayerdata ;

uint8_t CheckRtcFreeDay(void)  ;

__STATIC_INLINE RfMsg getrfmsg(uint8_t rfvalueid)  
{
    assert(rfvalueid != InvalidId);
    return rflayerdata.msglist[rfvalueid];
}



__STATIC_INLINE void resetrtcfreeday()  
{
    rflayerdata.rtcfreeday = 0; 
}


__STATIC_INLINE NetMode GetNetMode()  
{
    return rflayerdata.netmode; 
}

// on stop hardware , chang netmode .
__STATIC_INLINE void OnStopRf()  
{
    rflayerdata.rfmode = rf_init ;
    if(rflayerdata.netmode == net_monitor)
        rflayerdata.netmode = net_idle ;
}
// on start hardware , chang netmode .
__STATIC_INLINE void OnStartRf()  
{
    rflayerdata.rfmode = rf_init ;
    if(rflayerdata.netmode == net_idle)
        rflayerdata.netmode = net_monitor ;
}
// detect if the hardware is running .
__STATIC_INLINE uint8_t IsRfStop()  
{
    return (rflayerdata.rfmode == rf_init) ? 1: 0;
}


// add a msg at recv ender .  return 1 if there is only one item in recv list .
uint8_t  addrecvmsg(uint8_t id ,uint16_t addr);
// add a msg at recv header . used for msg from leaf . return 1, if there is only one item in recv list .
uint8_t  insertrecvmsg(uint8_t id ,uint16_t addr);
// get a msg from recv queue header. and remove it from list .
RfMsg rmrecvmsg(void) ;

// find a special receiver addr at recv queue . just return id 
RfMsg findrecvaddr(uint16_t addr);
// remove all msg in recv list , it's arm addr is specail .
void rmrecvaddr(uint16_t addr);


// put a new msg (root or tree node send msg to each other) .
void  addonlinemsg(uint8_t id, uint16_t addr) ;
// get a msg , this will sub the live cnt of the msg . if live cnt == 0 , it create  Sig_Rf_Overtime with msg id send to net layer .
RfMsg getonlinemsg(void);
// find first msg with special addr, return msgid ,  sub live cnt by 1 , if live cnt is 0 remove it from online list .
RfMsg findonlineaddr(uint16_t addr);
// remove all msg with special addr 
void rmonlineaddr(uint16_t addr);
// remove msg index is specail .
RfMsg rmonlineid(uint8_t bufid);



// insert a msg to offline queue . 
void  addofflinemsg(uint8_t id ,uint16_t addr);
// get a msg from offline queue header ,  it's id is specail  will remove it .
RfMsg rmofflineid(uint8_t bufid);

// remove all msg with specail addr .
void rmofflineaddr(uint16_t addr);
// get first msg in offline list . and sub it's live cnt by 1
RfMsg getofflineaddr(uint16_t addr);
// sub all live cnt by 1 , and remove the msg it's live cnt = 0 , the msg will be post to net layer .
void subofflinemsglivecnt(void);


__STATIC_INLINE uint8_t isofflineempty(void)
{
    return rflayerdata.offlineheader == InvalidId ? 1 :0 ;
}


// get a rf buffer , it try to get a new buffer from poolrf :  newevt(addr,PoolId2)
// if there is no buffer , it discard the last item in online queue or offline queue 
uint8_t  newrfbuf(uint16_t addr); 
void     freerfbuf(uint8_t memid);
__STATIC_INLINE uint32_t * getrfbuf(uint8_t memid)
{
    return (rflayerdata.memarray+ PoolRF_Size * memid) ;
}



// init the rf layer data 
void rflayerdataInit(void);
// release  the rflayer data .
void rflayerdatarelease(void);
// Mac layer to check the msg . and deal it .  
// this function may create a StdEvt that need to send out directly.
void  MacRecvProc(uint8_t msg) ;

// all msg infor include [0] (msg send try time) ,[1] (msg length) , FrameBit ,RecvAddr , msgcnt  should be set before call SendRf

// before call send function caller should set the evt 's VarAddr , and NetId to localaddr and local netid .
// and you should call StartRfMonitor to start the rf .
#define SendOnlineMsg( id, addr)  setonlinemsg(id,addr); DoSend()
//#define SendOfflineMsg(id,addr) setdirectmsg(id,addr) ; DoSend()
/*
uint8_t OnLeafWakeup( msg) 
{ 
    if(GetMacCmd == maccmd_checkonline 
        findofflineaddr
        insertrecvmsg
        DoSend()
*/
#endif

