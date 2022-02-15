#include "timealarm.h"
#include "basictype.h"
#include "hal.h"
#include "sx1262.h"
#include "cmdthd.h"
#include "halthd.h"

// fix the timectrl at the last 1k Byte of the ram.
// mark this area as  no init area .
// data write to flash use sector erase  , and write to update the stored data .
// 512 byte for every sector.  
GlobalData  globaldata __attribute__((at(RamStoreStartAddr)));




// inter help function  to act on the sqclist of the timectrl 
// the timblock list hold the real delay time and param . 
// the specail event may chang the param and repeat flage.but the delay time is not changed
// this function remove a specail timectrl from the running item list.

// return 1 :sucessed . 0 : param error or it has been enabled
// input the index of the speail item , if it is enabled ,do nothing ,else enable it and insert it to running list .
uint8_t  enabletimeitem( uint8_t index)
{
    uint32_t  temp32 ;
    uint8_t   tempindex ,week ,oldindex;

    // there is a new item enabled , insert it to the 8 list .
    temp32 = globaldata.timedata.tmarray[index].tb.var.data32 ;
	if(checkteen(temp32))
		return 0;
    
    week = getteweek(temp32);
    temp32 = gettealarm(temp32);
    if(temp32 > DaySecValue)
        return 0;        
    
    //find the first one bigger than it .
    tempindex = globaldata.timedata.weekheader[week];
    if(tempindex == InvalidId)
    {
        globaldata.timedata.weekheader[week] = index ;
        globaldata.timedata.weekcur[week] =index ;
        globaldata.timedata.sqclist[index] = InvalidId;
        return 1;
    }
    oldindex = InvalidId ;
    while(tempindex != InvalidId)
    {
        
        if(gettealarm(globaldata.timedata.tmarray[tempindex].tb.var.data32) > temp32)
            break;
        oldindex = tempindex ;
        tempindex = globaldata.timedata.sqclist[tempindex];
    }
    if(oldindex == InvalidId)
    { // set at first one 
        globaldata.timedata.sqclist[index] = globaldata.timedata.weekheader[week] ;
        globaldata.timedata.weekheader[week] = index ;
        return 1;
    }
    // find in running list .
    globaldata.timedata.sqclist[oldindex]  = index ;
    globaldata.timedata.sqclist[index] = tempindex;   
    return 1;
}

// return 1 :sucessed . 0 : param error or it has been disabled
// the item ,must be enabled  it will be disable and remove from the list , else  do nothing , 
uint8_t disabletimeitem(uint8_t index)
{
    uint8_t   tempindex ,week ,oldindex;

	if( ! checkteen(globaldata.timedata.tmarray[index].tb.var.data32))
		return 0;
	clrteen(globaldata.timedata.tmarray[index].tb.var.data32 );
	if(globaldata.timedata.curindex == index)
	{
		RTC_DisableIT_Alarm(RTC);
	}    

    week = getteweek(globaldata.timedata.tmarray[index].tb.var.data32);
    //find the this one and remove from list . 
    tempindex = globaldata.timedata.weekheader[week];
    oldindex = InvalidId ;
    while(tempindex != InvalidId)
    {
        if(tempindex == index )
            break;
        oldindex = tempindex ;
        tempindex = globaldata.timedata.sqclist[tempindex];
    }
    if(tempindex == InvalidId) 
    { // not  find the index
        return 0;
    }
    else
    {//the list is not null and find  it .
        if(oldindex == InvalidId)
        { // first one 
            oldindex = globaldata.timedata.sqclist[globaldata.timedata.weekheader[week]] ;
            globaldata.timedata.weekheader[week] = oldindex;
            if(globaldata.timedata.weekcur[week] == index)
                globaldata.timedata.weekcur[week] = oldindex ;
        }
        else
        {
            tempindex = globaldata.timedata.sqclist[tempindex] ;
            globaldata.timedata.sqclist[oldindex] = tempindex;
            if(globaldata.timedata.weekcur[week] == index)
                globaldata.timedata.weekcur[week] = tempindex ;       
        }

    }
	if(globaldata.timedata.curindex == index)
	{
		FindCurEvtAndWait();
	}
	return 1;
}
// define a defualt time event list . it will open and stopt the motor at specal time . 
const static GlobalData FlashDefaultData __attribute__((at(FlashSectorAddr))) =
{
    .cfgdata.RFFrequency = 4710 ,
    .cfgdata.Power = 2,
    .cfgdata.SignalBw = LORA_BW_500,
    .cfgdata.SpreadingFactor = LORA_SF10,
    .cfgdata.ErrorCoding = LORA_CR_4_6,
    .cfgdata.PreambleNum = 20 ,
    .cfgdata.NetName = DefaultNetName ,
    .cfgdata.Mark = RamValue ,
    .cfgdata.NetId = InvalidNetId ,
#if Nodetype == TreeNode         
    .cfgdata.LocalAddr = OrphanTreeAddr ,
    .cfgdata.TreeBitmap = 0 ,
#elif  Nodetype == LeafNode       
    .cfgdata.LocalAddr = OrphanLeafAddr ,
    .cfgdata.TreeBitmap = 0x80 ,
#elif  Nodetype == RootNode
    .cfgdata.LocalAddr = RootAddr ,    
    .cfgdata.TreeBitmap = RootCfg ,
#endif        
    .cfgdata.LeafBitmapH  = 0x80000000 ,
    .cfgdata.LeafBitmapL  =  0 ,
    .cfgdata.ActivePeriod = MaxChildActivePeriod ,
    .cfgdata.NearTreeNode1 = 0 ,
    .cfgdata.NearTreeNode2 = 0 ,   
    .cfgdata.NearTreeRssi1 = 0 ,  
    .cfgdata.NearTreeInfor1 = 0 ,
    .cfgdata.NearTreeRssi2 = 0 ,
    .cfgdata.NearTreeInfor2 = 0 ,
    .cfgdata.res1 = 0 ,
    .cfgdata.res2 = 0 ,
    .cfgdata.res3 = 0 ,
    //...
    .timedata.weekheader ={0},
    .timedata.weekcur ={0},
    .timedata.sqclist ={0},
    .timedata.alarm.data32 = FlashValue,
    .timedata.curindex =0,
    .timedata.jumpdaycnt =0,
    .timedata.lastindex =0,
    .timedata.lastweek =0,
    .timedata.tmarray =
    {
        {.tb.var.data32 =0xc0de0000 , .var.data32 =0x00000000} ,{.tb.var.data32 =0xc0de0280 , .var.data32 =0x00000001} ,
        {.tb.var.data32 =0xc0de0500 , .var.data32 =0x00000002} ,{.tb.var.data32 =0xc0de0780 , .var.data32 =0x00000003} ,
        {.tb.var.data32 =0xc0de0a00 , .var.data32 =0x00000004} ,{.tb.var.data32 =0xc0de0c80 , .var.data32 =0x00000005} ,
        {.tb.var.data32 =0xc0de1000 , .var.data32 =0x00000006} ,{.tb.var.data32 =0xc0de1280 , .var.data32 =0x00000007} ,
        {.tb.var.data32 =0xc0de1500 , .var.data32 =0x00000008} ,{.tb.var.data32 =0xc0de1780 , .var.data32 =0x00000009} ,
        {.tb.var.data32 =0xc0de1a00 , .var.data32 =0x0000000a} ,{.tb.var.data32 =0xc0de1c80 , .var.data32 =0x0000000b} ,
        {.tb.var.data32 =0xc0de2000 , .var.data32 =0x0000000c} ,{.tb.var.data32 =0xc0de2280 , .var.data32 =0x0000000d} ,
        {.tb.var.data32 =0xc0de2500 , .var.data32 =0x0000000e} ,{.tb.var.data32 =0xc0de2780 , .var.data32 =0x0000000f} ,
        {.tb.var.data32 =0xc0de2a00 , .var.data32 =0x00000010} ,{.tb.var.data32 =0xc0de2c80 , .var.data32 =0x00000011} ,
        {.tb.var.data32 =0xc0de3000 , .var.data32 =0x00000012} ,{.tb.var.data32 =0xc0de3280 , .var.data32 =0x00000013} ,
        {.tb.var.data32 =0xc0de3500 , .var.data32 =0x00000014} ,{.tb.var.data32 =0xc0de3780 , .var.data32 =0x00000015} ,
        {.tb.var.data32 =0xc0de3a00 , .var.data32 =0x00000016} ,{.tb.var.data32 =0xc0de3c80 , .var.data32 =0x00000017} ,
        {.tb.var.data32 =0xc0de4000 , .var.data32 =0x00000018} ,{.tb.var.data32 =0xc0de4280 , .var.data32 =0x00000019} ,
        {.tb.var.data32 =0xc0de4500 , .var.data32 =0x0000001a} ,{.tb.var.data32 =0xc0de4780 , .var.data32 =0x0000001b} ,
        {.tb.var.data32 =0xc0de4a00 , .var.data32 =0x0000001c} ,{.tb.var.data32 =0xc0de4c80 , .var.data32 =0x0000001d} ,
        {.tb.var.data32 =0xc0de5000 , .var.data32 =0x0000001e} ,{.tb.var.data32 =0xc0de5280 , .var.data32 =0x0000001f} ,
        {.tb.var.data32 =0xc0de5500 , .var.data32 =0x00000020} ,{.tb.var.data32 =0xc0de5780 , .var.data32 =0x00000021} ,
        {.tb.var.data32 =0xc0de5a00 , .var.data32 =0x00000022} ,{.tb.var.data32 =0xc0de5c80 , .var.data32 =0x00000023} ,
        {.tb.var.data32 =0xc0de6000 , .var.data32 =0x00000024} ,{.tb.var.data32 =0xc0de6280 , .var.data32 =0x00000025} ,
        {.tb.var.data32 =0xc0de6500 , .var.data32 =0x00000026} ,{.tb.var.data32 =0xc0de6780 , .var.data32 =0x00000027} ,
        {.tb.var.data32 =0xc0de6a00 , .var.data32 =0x00000028} ,{.tb.var.data32 =0xc0de6c80 , .var.data32 =0x00000029} ,
        {.tb.var.data32 =0xc0de7000 , .var.data32 =0x0000002a} ,{.tb.var.data32 =0xc0de7280 , .var.data32 =0x0000002b} ,
        {.tb.var.data32 =0xc0de7500 , .var.data32 =0x0000002c} ,{.tb.var.data32 =0xc0de7780 , .var.data32 =0x0000002d} ,
        {.tb.var.data32 =0xc0de7a00 , .var.data32 =0x0000002e} ,{.tb.var.data32 =0xc0de7c80 , .var.data32 =0x0000002f} ,
        {.tb.var.data32 =0xc0de8000 , .var.data32 =0x00000030} ,{.tb.var.data32 =0xc0de8280 , .var.data32 =0x00000031} ,
        {.tb.var.data32 =0xc0de8500 , .var.data32 =0x00000032} ,{.tb.var.data32 =0xc0de8780 , .var.data32 =0x00000033} ,
        {.tb.var.data32 =0xc0de8a00 , .var.data32 =0x00000034} ,{.tb.var.data32 =0xc0de8c80 , .var.data32 =0x00000035} ,
        {.tb.var.data32 =0xc0de9000 , .var.data32 =0x00000036} ,{.tb.var.data32 =0xc0de9280 , .var.data32 =0x00000037} ,
        {.tb.var.data32 =0xc0de9500 , .var.data32 =0x00000038} ,{.tb.var.data32 =0xc0de9780 , .var.data32 =0x00000039} ,
        {.tb.var.data32 =0xc0de9a00 , .var.data32 =0x0000003a} ,{.tb.var.data32 =0xc0de9c80 , .var.data32 =0x0000003b} ,
        {.tb.var.data32 =0xc0dea000 , .var.data32 =0x0000003c} ,{.tb.var.data32 =0xc0dea280 , .var.data32 =0x0000003d} ,
        {.tb.var.data32 =0xc0dea500 , .var.data32 =0x0000003e} ,{.tb.var.data32 =0xc0dea780 , .var.data32 =0x0000003f} ,
        {.tb.var.data32 =0xc0deaa00 , .var.data32 =0x00000040} ,{.tb.var.data32 =0xc0deac80 , .var.data32 =0x00000041} ,
        {.tb.var.data32 =0xc0deb000 , .var.data32 =0x00000042} ,{.tb.var.data32 =0xc0deb280 , .var.data32 =0x00000043} ,
        {.tb.var.data32 =0xc0deb500 , .var.data32 =0x00000044} ,{.tb.var.data32 =0xc0deb780 , .var.data32 =0x00000045} ,
        {.tb.var.data32 =0xc0deba00 , .var.data32 =0x00000046} ,{.tb.var.data32 =0xc0debc80 , .var.data32 =0x00000047} ,
        {.tb.var.data32 =0xc0dec000 , .var.data32 =0x00000048} ,{.tb.var.data32 =0xc0dec280 , .var.data32 =0x00000049} ,
        {.tb.var.data32 =0xc0dec500 , .var.data32 =0x0000004a} ,{.tb.var.data32 =0xc0dec780 , .var.data32 =0x0000004b} ,
        {.tb.var.data32 =0xc0deca00 , .var.data32 =0x0000004c} ,{.tb.var.data32 =0xc0decc80 , .var.data32 =0x0000004d} ,
        {.tb.var.data32 =0xc0ded000 , .var.data32 =0x0000004e} ,{.tb.var.data32 =0xc0ded280 , .var.data32 =0x0000004f} ,
        {.tb.var.data32 =0xc0ded500 , .var.data32 =0x00000050} ,{.tb.var.data32 =0xc0ded780 , .var.data32 =0x00000051} ,
        {.tb.var.data32 =0xc0deda00 , .var.data32 =0x00000052} ,{.tb.var.data32 =0xc0dedc80 , .var.data32 =0x00000053} ,
        {.tb.var.data32 =0xc0dee000 , .var.data32 =0x00000054} ,{.tb.var.data32 =0xc0dee280 , .var.data32 =0x00000055} ,
        {.tb.var.data32 =0xc0dee500 , .var.data32 =0x00000056} ,{.tb.var.data32 =0xc0dee780 , .var.data32 =0x00000057} ,
        {.tb.var.data32 =0xc0deea00 , .var.data32 =0x00000058} ,{.tb.var.data32 =0xc0deec80 , .var.data32 =0x00000059} ,
        {.tb.var.data32 =0xc0def000 , .var.data32 =0x0000005a} ,{.tb.var.data32 =0xc0def280 , .var.data32 =0x0000005b} ,
        {.tb.var.data32 =0xc0def500 , .var.data32 =0x0000005c} ,{.tb.var.data32 =0xc0def780 , .var.data32 =0x0000005d} ,
        {.tb.var.data32 =0xc0defa00 , .var.data32 =0x0000005e} ,{.tb.var.data32 =0xc0defc80 , .var.data32 =0x0000005f} ,
        {.tb.var.data32 =0xc0df0000 , .var.data32 =0x00000060} ,{.tb.var.data32 =0xc0df0280 , .var.data32 =0x00000061} ,
        {.tb.var.data32 =0xc0df0500 , .var.data32 =0x00000062} ,{.tb.var.data32 =0xc0df0780 , .var.data32 =0x00000063} ,
        {.tb.var.data32 =0xc0df0a00 , .var.data32 =0x00000064} ,{.tb.var.data32 =0xc0df0c80 , .var.data32 =0x00000065} ,
        {.tb.var.data32 =0xc0df1000 , .var.data32 =0x00000066} ,{.tb.var.data32 =0xc0df1280 , .var.data32 =0x00000067} ,
        {.tb.var.data32 =0xc0df1500 , .var.data32 =0x00000068} ,{.tb.var.data32 =0xc0df1780 , .var.data32 =0x00000069} ,
        {.tb.var.data32 =0xc0df1a00 , .var.data32 =0x0000006a} ,{.tb.var.data32 =0xc0df1c80 , .var.data32 =0x0000006b} ,
        {.tb.var.data32 =0xc0df2000 , .var.data32 =0x0000006c} ,{.tb.var.data32 =0xc0df2280 , .var.data32 =0x0000006d} ,
        {.tb.var.data32 =0xc0df2500 , .var.data32 =0x0000006e} ,{.tb.var.data32 =0xc0df2780 , .var.data32 =0x0000006f} ,
        {.tb.var.data32 =0xc0df2a00 , .var.data32 =0x00000070} ,{.tb.var.data32 =0xc0df2c80 , .var.data32 =0x00000071} ,
        {.tb.var.data32 =0xc0df3000 , .var.data32 =0x00000072} ,{.tb.var.data32 =0xc0df3280 , .var.data32 =0x00000073} ,
        {.tb.var.data32 =0xc0df3500 , .var.data32 =0x00000074} ,{.tb.var.data32 =0xc0df3780 , .var.data32 =0x00000075} ,
        {.tb.var.data32 =0xc0df3a00 , .var.data32 =0x00000076} ,{.tb.var.data32 =0xc0df3c80 , .var.data32 =0x00000077} ,
        {.tb.var.data32 =0xc0df4000 , .var.data32 =0x00000078} ,{.tb.var.data32 =0xc0df4280 , .var.data32 =0x00000079} ,
        {.tb.var.data32 =0xc0df4500 , .var.data32 =0x0000007a} ,{.tb.var.data32 =0xc0df4780 , .var.data32 =0x0000007b} ,
        {.tb.var.data32 =0xc0df4a00 , .var.data32 =0x0000007c} ,{.tb.var.data32 =0xc0df4c80 , .var.data32 =0x0000007d} ,
        {.tb.var.data32 =0xc0df5000 , .var.data32 =0x0000007e} ,{.tb.var.data32 =0xc0df5280 , .var.data32 =0x0000007f} ,
        {.tb.var.data32 =0xc0df5500 , .var.data32 =0x00000080} ,{.tb.var.data32 =0xc0df5780 , .var.data32 =0x00000081} ,
        {.tb.var.data32 =0xc0df5a00 , .var.data32 =0x00000082} ,{.tb.var.data32 =0xc0df5c80 , .var.data32 =0x00000083} ,
        {.tb.var.data32 =0xc0df6000 , .var.data32 =0x00000084} ,{.tb.var.data32 =0xc0df6280 , .var.data32 =0x00000085} ,
        {.tb.var.data32 =0xc0df6500 , .var.data32 =0x00000086} ,{.tb.var.data32 =0xc0df6780 , .var.data32 =0x00000087} ,
        {.tb.var.data32 =0xc0df6a00 , .var.data32 =0x00000088} ,{.tb.var.data32 =0xc0df6c80 , .var.data32 =0x00000089} ,
        {.tb.var.data32 =0xc0df7000 , .var.data32 =0x0000008a} ,{.tb.var.data32 =0xc0df7280 , .var.data32 =0x0000008b} ,
        {.tb.var.data32 =0xc0df7500 , .var.data32 =0x0000008c} ,{.tb.var.data32 =0xc0df7780 , .var.data32 =0x0000008d} ,
        {.tb.var.data32 =0xc0df7a00 , .var.data32 =0x0000008e} ,{.tb.var.data32 =0xc0df7c80 , .var.data32 =0x0000008f} 
 },
        
    //...
    .rsv ={0},
};



// load the timectrl from flash.
void LoadGlobalData()
{
    volatile uint32_t temp;
    volatile uint32_t * lpram  ;
    volatile uint32_t * lpflash ;
  
    //step1 : load data from flash to time ctrl.
    lpram= (uint32_t *)&globaldata;
    lpflash = (uint32_t *)FlashSectorAddr ;
    temp = DataBlockCount ;
    while(temp >0)
    {
        *lpram = *lpflash;
        lpflash++;
        lpram++;
        temp--;
    }
    //step2 : check if it's a valid data 
    
    // verify the CfgInformation globaldata.cfgdata
    if(globaldata.cfgdata.Mark != RamValue)
    {
        // set rf configure date to default 
        globaldata.cfgdata.RFFrequency =  4710 ;// Frequncy based on 100K
        globaldata.cfgdata.Power = 2 ;        // Power
        globaldata.cfgdata.SignalBw =    LORA_BW_500;       
        globaldata.cfgdata.SpreadingFactor = LORA_SF10;     
        globaldata.cfgdata.ErrorCoding = LORA_CR_4_6;         // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
        globaldata.cfgdata.PreambleNum = 20;        //PreambleNum,the min preamle is also should bigger than the time , cmu call the proc time .
        globaldata.cfgdata.NetName = DefaultNetName      ;       
        globaldata.cfgdata.Mark = RamValue ;
        globaldata.cfgdata.NetId = InvalidNetId ;
#if Nodetype == TreeNode         
        globaldata.cfgdata.LocalAddr = OrphanTreeAddr ;
        globaldata.cfgdata.TreeBitmap = 0 ;
#elif  Nodetype == LeafNode       
        globaldata.cfgdata.LocalAddr = OrphanLeafAddr ;
        globaldata.cfgdata.TreeBitmap = 0x80 ;
#elif  Nodetype == RootNode
        globaldata.cfgdata.LocalAddr = RootAddr ;    
        globaldata.cfgdata.TreeBitmap = RootCfg ;
#endif        
        globaldata.cfgdata.LeafBitmapH  = 0x80000000 ;
        globaldata.cfgdata.LeafBitmapL  =  0 ;
        
        globaldata.cfgdata.ActivePeriod = MaxChildActivePeriod ;
        //globaldata.cfgdata.NearTreeNode1 = 0 ;
        //globaldata.cfgdata.NearTreeNode2 = 0 ;
        
        outlog(log_cfgdata_error);
        
    }
    if(globaldata.timedata.alarm.data32 == FlashValue)
    {
        if(PrepareRtcEvt())
            FindCurEvtAndWait();
        else
            outlog(log_tmevent_no);
        
    }
    else
    {
         outlog(log_tmdata_error);
    }
    outlog(log_nothing);
}
// make the  the rtc event week list  in smallest first order  , if there is any valid rtc event .
// this function stop the evt alarm. check the item and arrange the sqclist in small order .
// this order is only fitable for at the very beginning of the week .
// normally we need to call  get evt by time , to reorder the  sqclist  
// return 1 , there is valid time event , else  no time event find .
uint8_t  PrepareRtcEvt(void)
{
    uint32_t temp32 ,tick ;
    uint8_t index ,tempindex ,weekindex;
    uint32_t *lp32 ;
    // clr the rtc alarm . 
    RTC_DisableIT_Alarm(RTC)  ;

    // rtc evt maybe restart so value in FlashDefaultData shoud be no used, reset it to invaliid
    // init all the header and cur and sqclist with 0xff . 
    lp32 = (uint32_t *) &globaldata.timedata ;
    for(index = 0 ; index < InterValueSize ; index++)
    {
        *lp32 = 0xffffffff;
        lp32++ ;
    }
    // calc and build the week list . 

    tempindex = InvalidId ;
    lp32 = (uint32_t *) globaldata.timedata.tmarray ;
    weekindex = InvalidId ;
    // find the smallest item 
    for(index = 0 ; index < TotalTmCnt ; index++)
    {
        if(checkteen(*lp32))
        {
            tick = gettetick(*lp32) ;
			temp32 = gettealarm(tick) ;
            // first do check here . 
            if(gettealarm(tick) > DaySecValue)
            {// disable this bit
                clrteen(*lp32);
                continue;
            }
			weekindex = getteweek(tick);
            // put in the week list at small first order
            if(globaldata.timedata.weekheader[weekindex] == InvalidId)
            {
				globaldata.timedata.weekheader[weekindex] = index ;
				
            }
			else
			{
				globaldata.timedata.weekcur[weekindex] = InvalidId ;
				tempindex = globaldata.timedata.weekheader[weekindex] ;
				while(temp32 > gettealarm(globaldata.timedata.tmarray[tempindex].tb.var.data32) )
				{
					globaldata.timedata.weekcur[weekindex] = tempindex ;
					tempindex = globaldata.timedata.sqclist[tempindex] ;
					if(tempindex == InvalidId)
					{
						globaldata.timedata.sqclist[globaldata.timedata.weekcur[weekindex]] = index ;
						break ;
					}
				}
				if(tempindex != InvalidId)
				{
					if(globaldata.timedata.weekcur[weekindex] == InvalidId )
					{// insert  at header 
						globaldata.timedata.sqclist[index]  = tempindex ;
						globaldata.timedata.weekheader[weekindex] = index ;
					}
					else
					{// insert after tempindex .
						globaldata.timedata.sqclist[ globaldata.timedata.weekcur[weekindex] ] = index ;
						globaldata.timedata.sqclist[ index ]  = tempindex ;
					}
				}
			}

        }
        lp32 +=2 ;
    }// end for loop . find the current smallest one .
 /*
    while(1)
    {
        tempindex = InvalidId ;
        temp32 =    MaxBitCnt ;
        lp32 = (uint32_t *) globaldata.timedata.tmarray ;
        weekindex = InvalidId ;
        // find the smallest item 
        for(index = 0 ; index < TotalTmCnt ; index++)
        {
            if(checkteen(*lp32))
            {
                if(globaldata.timedata.sqclist[index] == InvalidId)
                {
                    tick = gettetick(*lp32) ;
                    // first do check here . 
                    if(gettealarm(tick) > DaySecValue)
                    {// disable this bit
                        clrteen(*lp32);
                        continue;
                    }
                    // compare to find the smallest one . don't care the week only check the hour min sec,bcs the alarm can't set week.
                    if(tick < temp32)
                    {
                        temp32 = tick;
                        tempindex = index ;
                        weekindex = getteweek(tick);
                    }
                }
            }
            lp32 +=2 ;
        }// end for loop . find the current smallest one .
        // if the last one 
        if(InvalidId == tempindex)
        {// all checked . break loop
            break ;
        }
        // there is one find . temp set the sqclist of this index  as 0xfe  to show this one  has been checked .
        globaldata.timedata.sqclist[tempindex] = (InvalidId -1);
        

        if(globaldata.timedata.weekheader[weekindex] == InvalidId)
        {
            globaldata.timedata.weekheader[weekindex] = tempindex ;
        }
        else
        {//other .. set it as the last one 's value .
            globaldata.timedata.sqclist[globaldata.timedata.weekcur[weekindex]]   =  tempindex ;     
        }
        globaldata.timedata.weekcur[weekindex] = tempindex ;                

    }// end while
    
*/
    tempindex =0;
    // set the last item of the weeklist that is not empty  is invalidid ,
    for(index = 0 ; index < 8 ;index ++ )
    {
        if(globaldata.timedata.weekcur[index] != InvalidId)
        {
            //globaldata.timedata.sqclist[globaldata.timedata.weekcur[index]]  = InvalidId;
            globaldata.timedata.weekcur[index] = globaldata.timedata.weekheader[index] ;
            tempindex =1 ;
        }
        
    }
    // set the cur index for not used mode .
    globaldata.timedata.curindex = InvalidId;
    globaldata.timedata.lastindex = InvalidId;
    globaldata.timedata.lastweek = InvalidId;
    globaldata.timedata.jumpdaycnt = 0 ;
    return tempindex ;
}

// find the fitable one according to the rtc time .
uint32_t  GetRtcTickTime(void)
{
    uint32_t temp32,bitvalue ;
    uint8_t  flage;
    flage = 0 ;
    if(RTC->SBSCNT & 0x80)
    { // current sec has past at least  half seconds . and it
        bitvalue = Bcd2Int(RTC->BCDSEC) ;
        bitvalue ++ ;
        if(bitvalue == 60 )
        {
            bitvalue = 0 ;
            flage = 1 ;
        }
        temp32 =bitvalue ;
        
		bitvalue = Bcd2Int(RTC->BCDMIN) ;
        if(flage >0 )
        {
            flage = 0 ;
            bitvalue ++ ;    
            if(bitvalue == 60)        
            {
                bitvalue = 0 ;
                flage = 1 ;
            }
		}
        temp32 |= (bitvalue)<<6 ;
        //hour 
        bitvalue = Bcd2Int(RTC->BCDHOUR) ;
        if(flage >0 )
        {
             bitvalue ++ ;    
             if(bitvalue == 24)        
             {
                bitvalue = 0 ;
				flage = 1;
             }
        }
        temp32 |= (bitvalue)<<12 ;   
		//week .
		bitvalue = RTC->BCDWEEK ; // 0-6 bcd and int are same .
		if(flage > 0)
		{
			bitvalue ++;
			if(bitvalue >6 )
				bitvalue = 0 ;
		}
		temp32 |= (bitvalue)<<17 ;  

    }
    else
    {
		temp32 = RTC->BCDWEEK ;
		temp32 <<= 17 ;
        bitvalue = Bcd2Int(RTC->BCDHOUR) ;
        temp32 |= (bitvalue)<<12 ;
        bitvalue = Bcd2Int(RTC->BCDMIN);
        temp32 |= (bitvalue)<<6 ; 
        temp32 |=  Bcd2Int(RTC->BCDSEC) ;
    }
    return temp32 ;
}
// it should be used if there is a rtc time change .
// .... changed.
void  FindCurEvtAndWait(void)
{
    uint32_t temp32 ,tick ;
    uint8_t index,week,id ;
    // continue to find the one first small than rtc time .
    // get rtc time .
    RTC_DisableIT_Alarm(RTC);
    
    // remove current one from sqclist  , if it is not a repeat one : it has been past , it is no used any more.
    
    if(globaldata.timedata.curindex != InvalidId)
    {// not be  first time .
        temp32 = globaldata.timedata.tmarray[globaldata.timedata.curindex].tb.var.data32;
        week = getteweek(temp32);
        if(!checkterpt(temp32))
        { // find the index before current one .
            clrteen(globaldata.timedata.tmarray[globaldata.timedata.curindex].tb.var.data32);
            index = globaldata.timedata.weekheader[week];
            if(globaldata.timedata.lastweek == week)
            {// globaldata.timedata.lastindex  is in current week .  connect the last one to the next one of current index .
                index = globaldata.timedata.sqclist[globaldata.timedata.curindex];
                globaldata.timedata.sqclist[globaldata.timedata.lastindex] = index ; 
            }
            else
            { // find the last index of current one , and remove current one .
                id = InvalidId ;
                while(index != InvalidId)
                {
                    if(index == globaldata.timedata.curindex )
                    {
                        if(id == InvalidId)
                        { // first . it's the first item of this week list. remove header .
                            globaldata.timedata.weekheader[week] =  globaldata.timedata.sqclist[globaldata.timedata.weekheader[week]];
                        }
                        else
                        { // remove current one .
                            globaldata.timedata.sqclist[id] = globaldata.timedata.sqclist[index] ;
                        }
                    }
                    id = index ;
                    index = globaldata.timedata.sqclist[index];
                }
            }
            // the globaldata.timedata.curindex is removed  .set lastindex and week is invalid ,    
            globaldata.timedata.lastweek =InvalidId;
            globaldata.timedata.lastindex=InvalidId;
        }
        else
        {   // update the current index and the last index.
            globaldata.timedata.lastweek =week;
            globaldata.timedata.lastindex=globaldata.timedata.curindex;        
        }
    }
 
	// get local rtc time .
    temp32 = GetRtcTickTime();
    globaldata.timedata.jumpdaycnt = 0 ;
    index = InvalidId ;
	week = (uint8_t)RTC->BCDWEEK ;
	// first find the next one , that is bigger than current rtc time . it maybe in different day .
	// find the current list first .
	while(globaldata.timedata.weekcur[week] != InvalidId)
	{// find in the week list .
		tick  = globaldata.timedata.tmarray[globaldata.timedata.weekcur[week]].tb.var.data32;
		tick  = gettetick(tick);
		// compare the hour, min sec only.
		if(tick  > temp32)
		{
			index = globaldata.timedata.weekcur[week];
			goto CompareNext ;
		}
		globaldata.timedata.weekcur[week] =globaldata.timedata.sqclist[globaldata.timedata.weekcur[week]];
	}
	// not find in current week list ,  reset the cur to header .
	globaldata.timedata.weekcur[week] =globaldata.timedata.weekheader[week];
	
	// find the other week for the first valid item.
	
	while(1)
	{ 
		// jump to next day .
		week++ ;// point to next day.
		globaldata.timedata.jumpdaycnt ++ ;
		if(week == 7)
		{
			week =0 ;
		}
		if(globaldata.timedata.weekheader[week] != InvalidId)
		{// set the new day cur to header . 
			globaldata.timedata.weekcur[week] = globaldata.timedata.weekheader[week];
			//store	id to index		
			index = globaldata.timedata.weekheader[week];
			break;
		} 
		if(week == RTC->BCDWEEK )
		{ // has loop back to current day , and no item find . stop loop.
			break; 
		}
	}
	// index is the closed one index . week is the closed week day. jumpdaycnt is the number of day pasted  
CompareNext :

    // find day repeat list , find the first one bigger than rtc time one .
	// chang to alarm , do not care the week value, for repeat list .
	temp32 = gettealarm(temp32) ;
	id  = InvalidId ;
    while(globaldata.timedata.weekcur[7] != InvalidId)
    {
        tick  = globaldata.timedata.tmarray[globaldata.timedata.weekcur[7]].tb.var.data32;
		tick  = gettealarm(tick );
        // compare the hour, min sec only.
        if(tick  > temp32)
        {
            id = globaldata.timedata.weekcur[7] ;
            
			temp32 = 0 ;
            goto CompareStartLoop;
        }
        // to next one .
        globaldata.timedata.weekcur[7] =globaldata.timedata.sqclist[globaldata.timedata.weekcur[7]];
    }
    if(id == InvalidId)
    {
        if(globaldata.timedata.weekheader[7] != InvalidId)
        {
            globaldata.timedata.weekcur[7] = globaldata.timedata.weekheader[7];
            id = globaldata.timedata.weekheader[7];
			tick = globaldata.timedata.tmarray[id].tb.var.data32;
			tick  = gettealarm(tick );
			temp32  = 1;
        }
    }
CompareStartLoop :
    // index is the closed one index . week is the closed week day. flage is the loop back mark .
	// here  this id store the repeate list closest index  temp32 is next day flage  tick is it's alarm .

	if((index == InvalidId) && (id == InvalidId))
	{// no valid item find , stop
		globaldata.timedata.curindex = InvalidId;
		return ;
	}
	if(index == InvalidId)
	{ // used id as next index 
		index = id ;
		week = (uint8_t)RTC->BCDWEEK;
		if(temp32 >0)
		{ // temp32 is used mark if the data is return to header.
			week ++ ;
			if(week == 7)
				week =0;
            globaldata.timedata.jumpdaycnt = 1; 
		}
	    goto  SetNewAlarm ;
	}
	else if (id == InvalidId)
	{ // used index as next index
	    goto  SetNewAlarm ;
	}
	else
	{ //index and id both is valid . 
		if((globaldata.timedata.jumpdaycnt >0)  && (temp32 >0 ))
		{ //both is loop to other day
			// set week to next day .
			week =(uint8_t)( RTC->BCDWEEK +1 );
			if(week == 7)
				week =0;

			if(globaldata.timedata.jumpdaycnt > 1)
			{ // jump to other day.  repeate list is closest.
                globaldata.timedata.jumpdaycnt = 1;
				index = id;
			}
			else
			{ // both is to the next day . compare to find the closest one .
                globaldata.timedata.jumpdaycnt =1 ;
				goto CompareAlarm ;
			}
		}
		else if( ( globaldata.timedata.jumpdaycnt | temp32 ) >0)
		{// only one is looped
			if( globaldata.timedata.jumpdaycnt >0)
			{// repeat list is in current day. use it 
				index = id ;
				week =(uint8_t) RTC->BCDWEEK ; 
                globaldata.timedata.jumpdaycnt =0 ;
			}// else .index is the right one , need not chang .
		}
		else
		{// both is in current day .  get the small one .
			// week = RTC.BCDWEEK ;  // it need not to chang .
CompareAlarm : 
			temp32 = globaldata.timedata.tmarray[index].tb.var.data32;
			temp32 = gettealarm(temp32);
			
			if(tick < temp32 )
			{
				index = id;
			}		
		
		}
	}

SetNewAlarm :

    globaldata.timedata.curindex = index ;
    tick  = globaldata.timedata.tmarray[index].tb.var.data32;
    
    temp32 = week;
    temp32 <<= 8 ;
    
    id = gettehour(tick);
    temp32 |= Int2Bcd(id);
    temp32 <<= 8 ;
    
    id = gettemin(tick);
    temp32 |= Int2Bcd(id);
    temp32 <<= 8 ;  
    

    id = gettesec(tick);
    temp32 |= Int2Bcd(id);
    
	globaldata.timedata.alarm.data32 =temp32 ;
    RTC->ALARM = temp32 ;

    RTC_EnableIT_Alarm(RTC);   

}
// this function is call in interrupt .
uint8_t  OnRtcAlarm(void)
{

    uint16_t sig ;
    Var32 param ;
    // get current sig and param  first  .
    if(globaldata.timedata.curindex == InvalidId)
        return 0;
    param.data32 = globaldata.timedata.tmarray[globaldata.timedata.curindex].tb.var.data32;
    sig =  getteevt(param.data32);
    param.data32 = globaldata.timedata.tmarray[globaldata.timedata.curindex].var.data32;


    // check if it is  specail cmd. else post evt .
    // nextid will used to hold the next item will be alarm .
    if(sig == Sig_Rtc_Enable)
    {
        if(param.data8.data1)
            sig = enabletimeitem(param.data8.data0);
        else
            sig = disabletimeitem(param.data8.data0);
        if(sig )
        {            
            if( param.data8.data2 ) 
                globaldata.timedata.tmarray[param.data8.data0].var.data32 |= TERptMask ;
            else
                globaldata.timedata.tmarray[param.data8.data0].var.data32 &=  (~TERptMask) ;
        } 
    }
    else if(sig == Sig_Rtc_Stop)
    { // need to stop rtc list .
        for(sig = 0 ; sig < 8 ; sig++)    
        {
           globaldata.timedata.weekcur[sig] = globaldata.timedata.weekheader[sig];
        }
        RTC_DisableIT_Alarm(RTC);
        return 0 ;
    }
    else if(sig == Sig_Rtc_Jump)
    { // need to stop rtc list .
        // set the next item is  param.data8.data0 , and  param.data8.data1 is fix count , param.data8.data2 is jump count . 0xff is first time . 0 is last time
        //check if the specail index  it will be jumped to is valid  .
        if(globaldata.timedata.sqclist[param.data8.data0] == InvalidId )
           return 1 ;
        if( param.data8.data2 == InvalidId )
        {
            param.data8.data2 = param.data8.data1 ;
        }
        
        if( param.data8.data2 > 0 )
        {
             param.data8.data2 -- ;
        }
        else
        {
            param.data8.data2  = InvalidId ;
        }
        // store param to ram  .
        globaldata.timedata.tmarray[globaldata.timedata.curindex].var.data32 = param.data32 ;
        if(param.data8.data2  != InvalidId )
        {// jump to the specail one .
            
            globaldata.timedata.curindex = param.data8.data0 ;
            param.data32  = globaldata.timedata.tmarray[param.data8.data0].tb.var.data32;
            // store the sec and min 
            sig = getteweek ( param.data32 );
            globaldata.timedata.alarm.data32 = sig ;
            globaldata.timedata.alarm.data32 <<= 8 ;

            sig = gettehour ( param.data32 );
            sig = Int2Bcd((uint8_t)sig);
            globaldata.timedata.alarm.data32 |= sig ;
            globaldata.timedata.alarm.data32 <<= 8 ;
            
            sig = gettemin ( param.data32 );
            sig = Int2Bcd((uint8_t)sig);
            globaldata.timedata.alarm.data32 |= sig ;
            globaldata.timedata.alarm.data32 <<= 8 ;
            
            sig = gettesec ( param.data32 );
            sig = Int2Bcd((uint8_t)sig);
            globaldata.timedata.alarm.data32 |= sig ;
            
            RTC->ALARM = globaldata.timedata.alarm.data32 ;

            RTC_EnableIT_Alarm(RTC);          
        }
    }    
    else
    { 
        //analysis cmd and postevt.
        uint32_t *lp ;
        sig= newevt(sig,PoolId1);
        lp=  getevtmem(sig);
        *lp = param.data32;   
        // unfinished . different type of sig will be send to deffernt cmdport .
        if(IsSimpleCmd(sig))
           postevt((LPThdBlock)&(halmaster.super),sig);   
        else 
           postevt((LPThdBlock)&(cmdport.super),sig);                
    }   
    return 1;
}    
void RestartTimeAlarm()
{
    uint8_t temp ;
    for(temp =0 ; temp < 8 ; temp ++)
    {
        globaldata.timedata.weekcur[temp] = globaldata.timedata.weekheader[temp];
    }
    globaldata.timedata.curindex = InvalidId;
    globaldata.timedata.lastindex = InvalidId;
    globaldata.timedata.lastweek = InvalidId;
    globaldata.timedata.jumpdaycnt = 0 ;    
    FindCurEvtAndWait();
}


// save the globaldata.timedata to  flash.
void SaveGlobalData(void) 
{
    volatile uint32_t temp ;

    temp = globaldata.timedata.alarm.data32 ;   
    globaldata.timedata.alarm.data32 = FlashValue ; 
    // the HCLK is used 16M . do not need to change the time value for flash . 
    if(SectorErase(FlashSectorAddr) )
        assert(0);
    // check flash is ready to programe
    if(Program_Sector(DataSectorNum,(uint32_t *)&globaldata))
        assert(0);
    // now write the data back it .
    globaldata.timedata.alarm.data32 = temp ;
}


// send down a RtcEvt . store it in ram  buf[1] index ,[2] flage ,[3,4,5,6] tickblock,[7,8,9,10] param. 
// lp start from 1
void SetRtcEvt(uint8_t *lp) 
{
    uint8_t index;
    index = *lp;   // which index of the tm block changed .
    lp++;
    // the data value is changed . first set new value to it .
    LPTimeBlock lpb ;   
    disabletimeitem(index);    
    lpb = gettmaddr(index);
    lpb->tb.var.data8.data0 = *lp;
    lp++;
    lpb->tb.var.data8.data1 = *lp;
    lp++;
    lpb->tb.var.data8.data2 = *lp;
    lp++;
    lpb->tb.var.data8.data3 = *lp;
    lp++;        
    lpb->var.data8.data0 = *lp ;
    lp++;
    lpb->var.data8.data1 = *lp ;
    lp++;
    lpb->var.data8.data2 = *lp ;
    lp++;
    lpb->var.data8.data3 = *lp ;
    //update the sqc  list . if delay ticks changed .
    if(checkteen(lpb->tb.var.data32))
    {
        enabletimeitem(index);
    }
}

// read a RtcEvt ,  buf[1] index ,[2] flage ,[3,4,5,6] tickblock,[7,8,9,10] param.
// if the index bigger than cnt return the last one.
void GetRtcEvt(uint8_t *lp)  
{
    uint8_t index;
    index = *lp;
    lp+=1 ;    
    LPTimeBlock lpb ;   

    lpb = gettmaddr(index);
    *lp = lpb->tb.var.data8.data0 ;
    lp++;
    *lp = lpb->tb.var.data8.data1 ;
    lp++;
    *lp = lpb->tb.var.data8.data2;
    lp++;
    *lp = lpb->tb.var.data8.data3;
    lp++;        
    *lp = lpb->var.data8.data0  ;
    lp++;
    *lp = lpb->var.data8.data1  ;
    lp++;
    *lp = lpb->var.data8.data2  ;
    lp++;
    *lp = lpb->var.data8.data3  ;
 
}

// time is a  RtcTime struct .  this function check the new rtc time  and  addjust the alarm event 
// to least the code . this function is called by rf time to addjust time .
void ChangRtcTime(uint32_t time)
{
    int32_t temp ;
    temp = (int32_t)(time>>26) ; // get year
    if(temp == Bcd2Int(RTC->BCDYEAR))
    {
        temp = (int32_t)(time>>22) ;
        temp &= 0x0f ; // get month.
        if(temp == Bcd2Int(RTC->BCDMONTH) )
        { // check the day ,hour , min ,and sec  together .
            temp = (int32_t)(time >> 17) ;
            temp &= 0x1f ;
            temp *= 24 ;
            // hour
            temp += ((time>>12)&0x1f);
            temp *= 60 ;
            //min
            temp += ((time>>6)&0x3f);
            temp *= 60 ;
            //sec
            temp += (time&0x3f);

            // sub with the local time .   
            temp -= Bcd2Int(RTC->BCDSEC);
            temp -=(Bcd2Int(RTC->BCDMIN) *60);
            temp -=(Bcd2Int(RTC->BCDHOUR) *3600);
            temp -=(Bcd2Int(RTC->BCDDAY) *3600*24); 
            // check if the differnt is in allowed range .
            if(temp <=3 || temp >= -3)    
                return ;
        }
    }
    //update time 
    RtcSetTime(time);
    // time different , addjust the  event to the closest one.   
    RestartTimeAlarm();

}


