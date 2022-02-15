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
    if(netid < MaxNetId)    return NetIdAddrArray[netid] ;
}

#endif

// const 63 leaf node ,and 3 child node  and 1 father node live cnt .  63+3 =66  =33byte .
#define ChildNodeCnt      34   // byte 32-33 is used for tree node and father node .
#define ChildTreeOffset   64   // byte 0-31 used for leaf node .  
#define FatherTreeOffset  67   // byte 33 high byte ,used for father live cnt

static uint8_t ChildNodeLiveCnt[ChildNodeCnt];
static uint8_t NodeActiveCnt = 0;
// check if the local node ,active cnt is over time .
uint8_t SubActiveCnt(void)
{
    if(NodeActiveCnt > 0)
    {
        NodeActiveCnt -- ;
        return 0;
    }
    NodeActiveCnt = globaldata.cfgdata.ActivePeriod ;
    return 1 ;
}
// set local node as just register ok - max live count
__STATIC_INLINE void ResetActiveCnt(void)
{
    NodeActiveCnt = globaldata.cfgdata.ActivePeriod ;
}
// index start from 0 - 62  is leaf node range . 64-66 is tree node range . 67 is father live cnt;
void ResetLiveCnt(uint8_t index)
{
    uint8_t res ;
    if (index >= ChildNodeCnt*2 )
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
// sub father live cnt , return 1 ,if father live cnt is  0 ,else return  0 
uint8_t  SubFatherLiveCnt(void)
{
#if Nodetype    != RootNode
    uint8_t res =  ChildNodeLiveCnt[33];
    if(res & 0xf0)
    {
        res -= 0x10 ;
        ChildNodeLiveCnt[33] =  res ;
        return 0;
    }
    return 1 ;
#else
    return 0;    
#endif    
        
}
// sub all item 1 , if it's 0 reset the fitable bitmap bit .
void  SubLiveCnt(void)
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



// get a rf buffer , it try to get a new buffer from pool2 :  newevt(addr,PoolId2)
// if there is no buffer , it discard the last item in undirect queue or wait queue 
uint8_t newrfbuf(uint16_t addr)
{
    Var32 var  ;
    
    if(rflayerdata.freecnt >0 )
    {
        INT_DISABLE();
        var.data8.data0 = rflayerdata.idfree;
        rflayerdata.idfree =  * (rflayerdata.memarray+ PoolRF_Size * var.data8.data0);
        rflayerdata.freecnt -- ;
        INT_ENABLE();
        return var.data8.data0 ;
    }
    // no free buffer .
    //try to get one from undirect , wait queue.
    var = getundirectmsg();
    if(var.data8.data0 == InvalidId)
    {
        var = getwaitmsg();
    }
    assert(var.data8.data0 != InvalidId);
    return var.data8.data0 ;
}

void freerfbuf(uint8_t id)
{
	INT_DISABLE();
	*(rflayerdata.memarray+ PoolRF_Size * id) =  rflayerdata.idfree;
	rflayerdata.idfree = id ;
	rflayerdata.freecnt  ++;
	INT_ENABLE();
}

// put a evt to direct queue ender.
void  setdirectmsg(uint8_t msg,uint16_t addr)
{
    uint8_t  temp ,id;
    // there must has a free postion.
    assert(rflayerdata.freeheader != InvalidId);
    // get first and next free item  .
    id = rflayerdata.freeheader ;
    rflayerdata.freeheader = rflayerdata.nextid[id];
    
    // store the msg to first free position
    rflayerdata.rflayerbuf[id] = msg ;
    rflayerdata.rflayeraddr[id] = addr ;
    // add it to direct queue end 
    temp = rflayerdata.directheader ;
    while(rflayerdata.nextid[temp] != InvalidId)
    {
        temp = rflayerdata.nextid[temp];
    }
    rflayerdata.nextid[temp] = id ;
    rflayerdata.nextid[id] = InvalidId ;
}
// get a msg from direct queue 
Var32 getdirectmsg()
{
    Var32 var ;
    uint8_t id ;
    // get the first msg
    if(rflayerdata.directheader == InvalidId)
    {
        var.data8.data0 = InvalidId ;
        return var ;
    }
    // update the direct queue.
    id = rflayerdata.directheader ;
    var.data8.data0 =  rflayerdata.rflayerbuf[id] ;
    var.data16.data1 = rflayerdata.rflayeraddr[id] ;
    rflayerdata.directheader = rflayerdata.nextid[id];
    // store the postion to free queue
    rflayerdata.nextid[id] = rflayerdata.freeheader ;
    rflayerdata.freeheader = id ;
    return var ;    
}

// try to find a specail msg id
uint8_t finddirectaddr(uint16_t addr)
{
    uint8_t id ;
    id  = rflayerdata.directheader ;
    while(id != InvalidId)
    {
        if(rflayerdata.rflayeraddr[id] == addr )    
            return id ;
        id = rflayerdata.nextid[id];
    }
    return InvalidId ;
}
// remove msg from queue .
Var32 rmdirectaddr(uint16_t addr)
{
    Var32 var ;
    var.data8.data0 = InvalidId ;
    var.data8.data1  = rflayerdata.directheader ;
    if(var.data8.data1 == InvalidId)
    {
        return var ;
    }
    // find the addr ,store it at id ;
    var.data8.data0  = var.data8.data1 ;
    if(rflayerdata.rflayeraddr[var.data8.data1] == addr )
    {
        rflayerdata.directheader = rflayerdata.nextid[var.data8.data1] ;
    }
    else
    {   
        var.data8.data1  =  rflayerdata.nextid[var.data8.data1];   
        while(var.data8.data1 != InvalidId)
        {
            if(rflayerdata.rflayeraddr[var.data8.data1] == addr )    
            {
                rflayerdata.nextid[var.data8.data0] = rflayerdata.nextid[var.data8.data1] ;
                break;
            }
            else
            {
                var.data8.data0 = var.data8.data1;
                var.data8.data1 = rflayerdata.nextid[var.data8.data1] ;
            }
        }
        if(var.data8.data1 == InvalidId)
        {
            var.data8.data0 = InvalidId;
            return var;
        }
    }
    // put id to free queue .
    var.data8.data0  = var.data8.data1 ; 
    rflayerdata.nextid[var.data8.data0] = rflayerdata.freeheader ;
    rflayerdata.freeheader = var.data8.data0 ;
    var.data16.data1 = rflayerdata.rflayeraddr[var.data8.data0];
    return var ;
}

// put a msg to direct queue at ender .
void  setundirectmsg(uint8_t msg ,uint16_t addr)
{
    uint8_t  temp ,id;
    // there must has a free postion.
    assert(rflayerdata.freeheader != InvalidId);
    // get first and next free item  .
    id = rflayerdata.freeheader ;
    rflayerdata.freeheader = rflayerdata.nextid[id];
    
    // store the msg to first free position
    rflayerdata.rflayerbuf[id] = msg ;
    rflayerdata.rflayeraddr[id] = addr ;
    
    rflayerdata.nextid[id] = InvalidId ;
    // add it to direct queue end 
    if(rflayerdata.undirectheader == InvalidId)
    {
        rflayerdata.undirectheader = id;
        return ;
    }
    temp = rflayerdata.undirectheader ;
    while(rflayerdata.nextid[temp] != InvalidId)
    {
        temp = rflayerdata.nextid[temp];
    }
    rflayerdata.nextid[temp] = id ;

}
// get a msg from undirect queue header 
Var32 getundirectmsg()
{
    Var32 var ;
    uint8_t id ;
    // get the first msg
    if(rflayerdata.undirectheader == InvalidId)
    {
        var.data8.data0 = InvalidId ;
        return var ;
    }
    // update the direct queue.
    id = rflayerdata.undirectheader ;
    var.data8.data0 =  rflayerdata.rflayerbuf[id] ;
    var.data16.data1 = rflayerdata.rflayeraddr[id] ;
    rflayerdata.undirectheader = rflayerdata.nextid[id];
    // store the postion to free queue
    rflayerdata.nextid[id] = rflayerdata.freeheader ;
    rflayerdata.freeheader = id ;
    return var ;    
}

// try to find a specail msg id
uint8_t findundirectaddr(uint16_t addr)
{
    uint8_t id ;
    id  = rflayerdata.undirectheader ;
    while(id != InvalidId)
    {
        if(rflayerdata.rflayeraddr[id] == addr )    
            return id ;
        id = rflayerdata.nextid[id];
    }
    return InvalidId ;
}
// remove msg from queue .
Var32 rmundirectaddr(uint16_t addr)
{
    Var32 var ;
    var.data8.data0 = InvalidId ;
    var.data8.data1  = rflayerdata.undirectheader ;
    if(var.data8.data1 == InvalidId)
    {
        return var ;
    }
    // find the addr ,store it at id ;
    var.data8.data0  = var.data8.data1 ;
    if(rflayerdata.rflayeraddr[var.data8.data1] == addr )
    {
        rflayerdata.undirectheader = rflayerdata.nextid[var.data8.data1] ;
    }
    else
    {   
        var.data8.data1  =  rflayerdata.nextid[var.data8.data1];   
        while(var.data8.data1 != InvalidId)
        {
            if(rflayerdata.rflayeraddr[var.data8.data1] == addr )    
            {
                rflayerdata.nextid[var.data8.data0] = rflayerdata.nextid[var.data8.data1] ;
                break;
            }
            else
            {
                var.data8.data0 = var.data8.data1;
                var.data8.data1 = rflayerdata.nextid[var.data8.data1] ;
            }
        }
        if(var.data8.data1 == InvalidId)
        {
            var.data8.data0 = InvalidId;
            return var;
        }
    }
    // put id to free queue .
    var.data8.data0  = var.data8.data1 ; 
    rflayerdata.nextid[var.data8.data0] = rflayerdata.freeheader ;
    rflayerdata.freeheader = var.data8.data0 ;
    var.data16.data1 = rflayerdata.rflayeraddr[var.data8.data0];
    return var ;
}
// put a msg to direct queue .
void  setwaitmsg(uint8_t msg ,uint16_t addr)
{
    uint8_t  temp ,id;
    // there must has a free postion.
    assert(rflayerdata.freeheader != InvalidId);
    // get first and next free item  .
    id = rflayerdata.freeheader ;
    rflayerdata.freeheader = rflayerdata.nextid[id];
    // store the msg to first free position
    rflayerdata.rflayerbuf[id] = msg ;
    rflayerdata.rflayeraddr[id] = addr ;
    rflayerdata.nextid[id] = InvalidId ;
    // add it to direct queue end 
    if(rflayerdata.waitheader == InvalidId)
    {
        rflayerdata.waitheader = id;
        return ;
    }
    temp = rflayerdata.waitheader ;
    while(rflayerdata.nextid[temp] != InvalidId)
    {
        temp = rflayerdata.nextid[temp];
    }
    rflayerdata.nextid[temp] = id ;
}
// get a msg from wait queue . remove it if the trycnt <=1 , 
Var32 getwaitmsg()
{
    uint8_t * lp;
    Var32 var ;
    // get the first msg
    if(rflayerdata.waitheader == InvalidId)
    {
        var.data8.data0 = InvalidId ;
        return var ;
    }
    // update the direct queue.
    var.data8.data1 = rflayerdata.waitheader ;
    var.data8.data0 =  rflayerdata.rflayerbuf[var.data8.data1] ;
    var.data16.data1 = rflayerdata.rflayeraddr[var.data8.data1] ;
    lp = (uint8_t *) getrfbuf( var.data8.data0);
    if(*lp < 2)
    {
        rflayerdata.waitheader = rflayerdata.nextid[var.data8.data1];
        // store the postion to free queue
        rflayerdata.nextid[var.data8.data1] = rflayerdata.freeheader ;
        rflayerdata.freeheader = var.data8.data1;
    }
    return var ;
}


// try to find a specail msg id
uint8_t findwaitaddr(uint16_t addr)
{
    uint8_t id ;
    id  = rflayerdata.waitheader ;
    while(id != InvalidId)
    {
        if(rflayerdata.rflayeraddr[id] == addr )    
            return id ;
        id = rflayerdata.nextid[id];
    }
    return InvalidId ;
}
Var32 rmwaitaddr(uint16_t addr)
{
    Var32 var ;
    var.data8.data0 = InvalidId ;
    var.data8.data1  = rflayerdata.waitheader ;
    if(var.data8.data1 == InvalidId)
    {
        return var ;
    }
    // find the addr ,store it at id ;
    var.data8.data0  = var.data8.data1 ;
    if(rflayerdata.rflayeraddr[var.data8.data1] == addr )
    {
        rflayerdata.waitheader = rflayerdata.nextid[var.data8.data1] ;
    }
    else
    {   
        var.data8.data1  =  rflayerdata.nextid[var.data8.data1];   
        while(var.data8.data1 != InvalidId)
        {
            if(rflayerdata.rflayeraddr[var.data8.data1] == addr )    
            {
                rflayerdata.nextid[var.data8.data0] = rflayerdata.nextid[var.data8.data1] ;
                break;
            }
            else
            {
                var.data8.data0 = var.data8.data1;
                var.data8.data1 = rflayerdata.nextid[var.data8.data1] ;
            }
        }
        if(var.data8.data1 == InvalidId)
        {
            var.data8.data0 = InvalidId;
            return var;
        }
    }
    // put id to free queue .
    var.data8.data0  = var.data8.data1 ; 
    rflayerdata.nextid[var.data8.data0] = rflayerdata.freeheader ;
    rflayerdata.freeheader = var.data8.data0 ;
    var.data16.data1 = rflayerdata.rflayeraddr[var.data8.data0];
    return var ;
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
	{

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


// mac layer part  -----------------------------------------------

// init the mac layer event buffer and the live cnt to zero . 
// it is called at node reset .
void rflayerdataInit(void)
{
    uint16_t i ;
    uint32_t * lp32;
    uint8_t *lp8 ;
    // init the rf memory pool .
    rflayerdata.idfree  =0;
    rflayerdata.freecnt =PoolRF_Cnt;
    lp32 = rflayerdata.memarray;    
    for(i = 0 ; i< (PoolRF_Cnt -1) ; i++)
    {
        *lp32 = i+1 ;
        lp32 += PoolRF_Size;
    }
    *lp32 = InvalidId ;
    
    for(i =0 ; i< Max_RfLayerMem -1 ; i++)
    {
        rflayerdata.nextid[i] = i+1;
    }
    rflayerdata.nextid[i] = InvalidId;
    rflayerdata.freeheader = 0 ;
    
    rflayerdata.directheader = InvalidId ;
    rflayerdata.undirectheader = InvalidId ;
    rflayerdata.waitheader = InvalidId ;
    //rflayerdata.waitender = InvalidId;
    rflayerdata.cadcnt = 32 ;
    rflayerdata.delaycnt = Max_Delay;

    lp8 = ChildNodeLiveCnt;
    i = ChildNodeCnt  ;
    while(i)    
    {
        *lp8 = 0x0;
        lp8++ ;
        i-- ;
    }
    NodeActiveCnt =  0;
#if Nodetype == RootNode
    
    if(NetIdAddrArray &0x03)
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
     uint8_t id ,temp;
     
     id = rflayerdata.directheader ;
     while(id != InvalidId)
     {
        freerfbuf(id);
        temp = id ; 
        id = rflayerdata.nextid[id];
        rflayerdata.nextid[temp] =  rflayerdata.freeheader;
        rflayerdata.freeheader =temp; 
     }
     id = rflayerdata.undirectheader ;
     while(id != InvalidId)
     {
        freerfbuf(id); 
        temp = id ; 
        id = rflayerdata.nextid[id];
        rflayerdata.nextid[temp] =  rflayerdata.freeheader;
        rflayerdata.freeheader =temp; 
     }    
     id = rflayerdata.waitheader ;
     while(id != InvalidId)
     {
        freerfbuf(id); 
        temp = id ; 
        id = rflayerdata.nextid[id];
        rflayerdata.nextid[temp] =  rflayerdata.freeheader;
        rflayerdata.freeheader =temp; 
     } 
    rflayerdata.directheader = InvalidId ;
    rflayerdata.undirectheader = InvalidId ;
    rflayerdata.waitheader = InvalidId ;     
}


uint8_t CheckRtcFreeDay(void)  
{
    if(rflayerdata.rfmode < rf_monitor)
        return 0;
    if(rflayerdata.rtcfreeday > Max_RtcFreeCnt)
        return 1 ;
    else
        rflayerdata.rtcfreeday ++; 
    return 0;
}

 //  rf receive a new msg . check it : if the msg is a ack msg , remove the wait queue  if there is . else do mac proc 
 //  this function may create a StdEvt in direct queue  that need to send out directly .
void MacRecvProc(uint8_t msg) 
{
    Var32 temp ;
    LPFrameCtrl  lpframe ;
    lpframe = (LPFrameCtrl)getrfbuf(msg);
    temp.data8.data0 = GetMacCmd(lpframe->FrameBit) ;
    if(temp.data8.data0 == maccmd_none )
    {   // this is a data msg . check if send to local , discard if not 
        if(IsDownMsg(lpframe->FrameBit)) 
        {// msg is send from root to a child node .  Varaddr is the  node addr   
            if(isfatheraddr(lpframe->SendAddr))
            {
                ResetLiveCnt(FatherTreeOffset);
                if(lpframe->VarAddr == globaldata.cfgdata.LocalAddr)
                {   // recv msg to local .   send it to net process.
                    postevtbyindex(BlcId_Net,makeevt(Sig_Rf_Recv_Msg,msg));  
                    // check if it has ack flage , it also means it has received a message send from local .
                    if(IsAuxAckMsg(lpframe->FrameBit)) 
                         rmwaitaddr(lpframe->SendAddr) ;      
                    // check if there is a direct message need to be send to this node .
                    temp = rmundirectaddr(lpframe->SendAddr);
                    temp.data16.data1 = lpframe->SendAddr ;
                    if(temp.data8.data0 == InvalidId)
                    { // no direct msg need to send , send ack.
                        temp.data8.data0 = newrfbuf(lpframe->SendAddr); 
                        makeackmsg(temp.data8.data0,1);
                    }
                    else
                    { // send direct msg ,with aux flage.
                        temp.data8.data0 = msg ;
                        lpframe = (LPFrameCtrl)getrfbuf(temp.data8.data0) ;
                        SetAuxAckMsg(lpframe->FrameBit);
                    }
                    SendRfMsgDirect(temp.data8.data0,temp.data16.data1);                     
                }
                else if(issubnodeaddr(lpframe->VarAddr))
                { // msg is send to child node .
                    // get next connected  child addr for the  addr , then put it to normal msg queue.
                    temp.data8.data0 = msg ;
                    temp.data16.data1 = getsubnodeaddr(lpframe->VarAddr);
                    SendRfMsg(temp.data8.data0,temp.data16.data1);
                    // we need return a ack , or undirect msg to sender 
                    temp = rmundirectaddr(lpframe->SendAddr);
                    temp.data16.data1 = lpframe->VarAddr;
                    if(temp.data8.data0 == InvalidId)
                    {
                        temp.data8.data0 = newrfbuf(temp.data16.data1); 
                        makeackmsg(temp.data8.data0,0);
                    }
                    else
                    {
                        temp.data8.data0 = msg ;
                        lpframe = (LPFrameCtrl)getrfbuf( temp.data8.data0) ;
                        SetAuxAckMsg(lpframe->FrameBit);                        
                    }
                    SendRfMsgDirect(temp.data8.data0,temp.data16.data1);                        
                }
                else //  should never come here , it means the father send a down msg  but not to it's child
                    freerfbuf(msg);
            }
            else  // discard it
                freerfbuf(msg);
            return ;
        }
        else
        { // send to root . varaddr is the sender's addr , sender should be child addr
            if(ischildaddr(lpframe->SendAddr))
            {
               #if Nodetype  == RootNode   
                    evt = makeevt(Sig_Rf_Recv_Msg,msg);
                    postevtbyindex(BlcId_Net,evt);  
                    setnetidtoaddr(lpframe->VarAddr,lpframe->NetId)    ;
               #else 
                    ResetLiveCntByAddr(lpframe->SendAddr); 
                    // send this msg to father node .
                    temp.data16.data1 = getfatheraddr();    
                    SendRfMsg(msg,temp.data16.data1);
                    // prepare a reply msg to child.
                    if(IsAuxAckMsg(lpframe->FrameBit)) 
                         rmwaitaddr(lpframe->SendAddr) ;
                    // try to find a cmd need to this child .
                    temp = rmundirectaddr(lpframe->SendAddr);
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
              // the netthd will start register proc to send  maccmd_broadcast  with direction is up . and wait 4second
              // the reachable tree will return this message .  here to store the useable tree node .
              // after 4 second , the braodcast proc will use the lowest rssi to send jion message .      
              // for broad cast . node should receive only the tree node , that is at the layer  it's same with it's father. 
                if(rflayerdata.rfmode >= rf_gettime) 
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
                rmwaitaddr(lpframe->SendAddr);
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
            else if(temp.data8.data0 == maccmd_checkundirect)
            { // child need to get time from father .  this cmd is period send by child to get possible incoming msg .
              // the undirect msg is also been send when there is a msg send to father node .  
                if(IsDownMsg(lpframe->FrameBit))
                { // child receive  reply  sended by father. there is a undirect msg .
                    ResetActiveCnt();
                    postevtbyindex(BlcId_Net,Sig_Rf_CheckUndirect_OK);
                    if(IsAuxAckMsg(lpframe->FrameBit))
                    {
                        rmundirectaddr(lpframe->SendAddr);
                        freerfbuf(msg);
                    }
                    else
                    {
                        ResetLiveCnt(FatherTreeOffset);
                        postevtbyindex(BlcId_Net,makeevt(Sig_Rf_Recv_Msg,msg));    
                        msg = newrfbuf(lpframe->SendAddr); 
                        makeackmsg(msg,1);
                        SendRfMsgDirect(msg,lpframe->SendAddr);                     
                    }
                    return ;
                }
                else
                {// father reciev check undirect asking from child .
                    // return the undirect msg if there is , else return ack msg .
                    temp.data8.data0 = findundirectaddr(lpframe->SendAddr);
                    if(temp.data8.data0 == InvalidId)
                    {
                        temp.data8.data0 = msg ;
                        makeackmsg(temp.data8.data0,0);
                    }
                    else
                    {
                        freerfbuf(msg);
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
