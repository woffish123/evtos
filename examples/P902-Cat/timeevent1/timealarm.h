#ifndef  TIME_ALARM_H
#define  TIME_ALARM_H
#include "basictype.h"
#include "maclayer.h"









typedef struct timeevt_
{
	uint32_t  sec :  6 ;    // based on 0
    uint32_t  min :  6 ;    // based on 0
    uint32_t  hour : 5 ;    // based on 0
    uint32_t  week : 3 ;    // based on 0  0-6 week1 - week 7 , 7 : 
    uint32_t  en : 1 ;      // used for mark this evet is enabled or not . bit flage. 
    uint32_t  rsv  : 1 ;    // enabled or not . bit flage
    uint32_t  rpt: 1 ;      // is repeat every week , or just run once .
    uint32_t  evt  :   9 ;   // time event .
}timeevt;
// if a timeevet is a specail event , means , it always relay on the event before it .



#define DaySecValue      0x00017efb     // week = 0 hour=23 10111,min = 59 111011 ,sec = 59 111011   s max for one day based on 0.
#define WeekSecValue     0x000d7efb     // week = 6 hour=23 10111,min = 59 111011 ,sec = 59 111011   s max for one day based on 0.
#define MaxBitCnt        0x001FFFFF     // max value for week,hour,min ,sec



#define TESigMask      0xff800000    //  use high 9 bit as event bits
#define TERptMask      0x00400000    //  bit mask for week repeat 
#define TESvrMask      0x00200000    //  bit mask for svr bit   
#define TEEnMask       0x00100000    //  bit mask for  if this event is enabled or not .

#define TEWeekMask     0x000e0000    //  use low 3 bit as week bits
#define TEHourMask     0x0001f000    //  use low 5 bit as week bits
#define TEMinMask      0x00000fc0    //  use low 6 bit as week bits
#define TESecMask      0x0000003f    //  use low 6 bit as week bits
#define TETickMask     0x000fffff    // all time bit together .
#define TEAlarmMask    0x0001ffff    // sec min hour bit together

#define EvtBitOffset  23
#define getteevt(te)  (uint16_t)((te) >> EvtBitOffset )

#define gettetick(te) ((te)&TETickMask)  

#define WeekBitOffset  17
#define getteweek(te) (uint8_t)(((te)&TEWeekMask)>> WeekBitOffset)  

#define HourBitOffset  12
#define gettehour(te) (uint8_t)(((te)&TEHourMask)>> HourBitOffset) 

#define MinBitOffset   6
#define gettemin(te)  (uint8_t)(((te)&TEMinMask)>> MinBitOffset)  

#define SecBitOffset   0
#define gettesec(te)  (uint8_t)(((te)&TESecMask))  

#define gettealarm(te) ((te)&TEAlarmMask) // get all week hour min sec .

#define setteweek(te,v) te =((te)&(~TEWeekMask))|((v)<< WeekBitOffset)  
#define settehour(te,v) te =((te)&(~TEHourMask))|((v)<< HourBitOffset) 
#define settemin(te,v)  te =((te)&(~TEMinMask)) |((v)<< MinBitOffset)  
#define settesec(te,v)  te =((te)&(~TESecMask)) |(v)  


// chang the evt of a timeevt
#define setteevt(te,evt)   te = (((te)&(TETickMask|TERptMask|TESvrMask|TEEnMask))|((evt)<<23))
// chang the ticks of a timeevt
#define settetick(te,tk)   te = (((te)&(TESigMask|TERptMask|TEDSvrMask|TEEnMask))|(tk))
// make a time evt  without set day and week repeat flage
#define makete(sig,tk)    ((tk)|(((uint32_t)(sig))<<23)) 
// set week repeat flage 
#define setterpt(te)  te =((te)|TERptMask )
#define clrterpt(te)  te =((te)&(~TERptMask ))
#define checkterpt(te)  (((te)&TERptMask) >0 )

// set enable flage 
#define setteen(te)   te =((te)|TEEnMask )
#define clrteen(te)   te =((te) & (~TEEnMask) )
#define checkteen(te)   (((te)&TEEnMask ) >0 )

// check  if the evt of a time evt is same as sig
#define checktesig(te,sig)  (((sig ^ te) & TESigMask) ==0)

// struct to hold the ticks event and param  one uint32_t
typedef union TickBlock_
{
    timeevt value;
	Var32   var;
}TickBlock ,* LPTickBlock; // 4 byte.

// struct to hold the time event and uint32_t  param.
typedef struct TimeBlock_
{
	TickBlock  tb;  //  use the tickblock's var  store the cnt of minute.
    Var32    var;
}TimeBlock, *LPTimeBlock;  // 8 byte



// TimeCtrl used a 2K byte Ram  to store itself and CfgInformation .



#define TotalTmCnt    220   // the buf number based timctrl ,must be times of 8  .it used 200*8 = 1600 byte.
#define TotalTmCnt32  55    // TotalTmCnt based on 4byte : used as uint32_t 
#define InterValueSize (TotalTmCnt32 +4) // weekheader ,weekcur and sqclist size in uint32_t
typedef struct TimeCtrl_
{
    // there are 8 list , every list end with invalidId, every list has a header index and a  cur index ,
    // if the header is InvalidId means the list is NULL .
    // 0-6 : is the week list  , 7 :is the repeat item list.
    uint8_t weekheader[8]  ; // use to store the list item that is only week0 item header  in smallestfirst order,  it is end with InvalidId ;
    uint8_t weekcur[8]  ; // the current one .
 

    // link array ,  store the next index ,
    uint8_t sqclist[TotalTmCnt] ;  // the block next index list .  it contained 8 null item,as 8 lists end flage .
    // current wait time event.
    Var32   alarm;   // waited alarm time  . data32.data0 is sec , data1 is min , data2 is hour ,data3 is week , all in BCD mode . 0xff means invliad vlaue
                    // if week is InvalidId , means the current alarm is invalid , no alarm will be set .
    // follow four param is interused to quick ,action.
    uint8_t curindex ;   // current delay item index .
    uint8_t jumpdaycnt ; // jump to how many days  to the last time .  0 not over day ,  
    uint8_t lastindex ;  // store the before index , that has just be past.
    uint8_t lastweek ;   // store the before week index that has just be pasted. 0-7
    // following is load from flash . the real time event array.
    TimeBlock tmarray[TotalTmCnt];
}TimeCtrl ,* LPTimeCtrl;


#define OFFSETTOTMBLK  ( 24+TotalTmCnt )    
#define TimeCtrlSize   ( OFFSETTOTMBLK + (TotalTmCnt<<3))   // TimeCtrl size . : 24 +220 + 220*8 = 2004
#define CfgRFInfoSize  sizeof(CfgInformation)          //  CfgInformation  RF param size .
    
#define CfgRsvSize  (2048 - TimeCtrlSize -CfgRFInfoSize)                 // CfgInformation reserve buf size.48 for TotalTmCnt =216

// 4×Ö½Ú¶ÔÆë¡£

// the CfgInformation used  totally  40 bytes  . must not chang it size . when load from flash , we used fixed address.



typedef struct GlobalData_
{
    CfgInformation cfgdata;
    TimeCtrl       timedata;
    uint8_t        rsv[CfgRsvSize];
}GlobalData ;
// total ram used is = 2048 byte .

extern GlobalData  globaldata;




// define the start addr for flash to store the data .  here used 2K to store the data
// the flash lock bit for the high sector . 2K flash 
#define FlashLockBit    0xc0000000       // the high 8K area in in this bit.
// the begin addr for data store  in flash  
#define FlashSectorAddr  0x1f800 
// the begin addr for data store  in ram 
#define RamStoreStartAddr    0x20005800
// the total count used for data store . based on uint32_t
#define DataBlockCount       0x200
// the data flash sector num , last one 
#define DataSectorNum       (FlashSectorAddr >>11)


#define RamStoreEndAddr      RamStoreStartAddr + 0x800 - sizeof(TimeBlock)

//get tm addr in ram
#define gettmaddr(index)  ((LPTimeBlock)(RamStoreStartAddr + sizeof(CfgInformation) + OFFSETTOTMBLK+ (((uint16_t)index)<<3) ))
//get tm tb in flash    as a uint32_t
#define getorgtmtb(index)  (*(__IO uint32_t*)(FlashSectorAddr + sizeof(CfgInformation) + OFFSETTOTMBLK+ (((uint16_t)index)<<3)))    
//get tm var in flash   as a uint32_t      
#define getorgtmvar(index)  (*(__IO uint32_t*)(FlashSectorAddr + sizeof(CfgInformation) + OFFSETTOTMBLK + 4+ (((uint16_t)index)<<3)))        
// load the timectrl from flash.
void LoadGlobalData(void) ;
// save the timectrl to  flash.
void SaveGlobalData(void) ;
// make the list for init .  return 0 if no item valid.
uint8_t  PrepareRtcEvt(void);
// find event and start alarm  according to rtc time  . use for first time or the rtc time changed .
void FindCurEvtAndWait(void);

// reset the rtc event . it just set the weekcur to header , and call FindCurEvtAndWait
// this is called when rtc time is changed . but event do not changed by outside .
// if only one item is changed , call enableitemitem or disabletimeitem
// example : 1 event  delay time changed , call disabletimeitem , than chang it , than call enableitemitem
void RestartTimeAlarm(void);
// time is a  RtcTime struct .  this function check the new rtc time  and  addjust the alarm event 
// to least the code .
void ChangRtcTime(uint32_t time);
// do rtc event and set next alarm.
uint8_t OnRtcAlarm(void);
// send down a RtcEvt . store it in ram  buf[1] index ,[2] flage ,[3,4,5,6] tickblock,[7,8,9,10] param. 
#define SetRtcEvtMask  1 
#define SetRtcBitMask  2 
void SetRtcEvt(uint8_t *lp) ;
// read a RtcEvt ,  buf[1] index ,[2] flage ,[3,4,5,6] tickblock,[7,8,9,10] param.
// if the index bigger than cnt return the last one.
void GetRtcEvt(uint8_t *lp)  ;



#endif
