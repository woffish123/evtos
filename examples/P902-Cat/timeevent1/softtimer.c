#include "softtimer.h"
#include "evtos.h"
#include "hal.h"



// the ccr1header is the index of the first softtimer , the nextid of  this softtimer is a next softtimer index , so it make a list.
// the first softtimer's delay is 0 : the delay value can be calc by  lptim->CCR1 - lptim->CNT ,
// the first softtimer's delay is the delay from the first one overtimed: calc as  : newdelay - (lptim->CCR1 - lptim->CNT) so it must bigger than the first's delay time
// if  not , the new softtime will be pleased at lptime->ccr2 ,  the lptime->ccr2 also hold a array same like CCR1
// so if the new one's delay is short than the ccr2,  the ccr2 need to set the new one to the first on in it's array .
SoftTimeCtrl   SoftTimerCtrlBlock ;

uint8_t GetSoftTimerCnt(void)
{
    return SoftTimerCtrlBlock.usedcnt;
}

void InitSoftTimer(void)
{

    // init ctrl block
    uint8_t temp ;
    for(temp = 0 ; temp < SoftTimerCnt ; temp ++)
    {
        SoftTimerCtrlBlock.ctrl[temp].differ = 0 ;
        SoftTimerCtrlBlock.ctrl[temp].delay = 0 ;
        SoftTimerCtrlBlock.evt[temp] = 0 ;
        SoftTimerCtrlBlock.block[temp] = 0 ;
        SoftTimerCtrlBlock.nextid[temp] = temp+1 ;
    }
    SoftTimerCtrlBlock.nextid[SoftTimerCnt-1] = InvalidId ;
    SoftTimerCtrlBlock.ccr1header = InvalidId ;
    SoftTimerCtrlBlock.ccr2header = InvalidId ;
    SoftTimerCtrlBlock.freeheader = 0 ;
    SoftTimerCtrlBlock.usedcnt = 0;
    InitLpTime();
    SoftTimerCtrlBlock.lpQuickProc = NULL;
}
void SetSoftTimerDefProc( LPSftQuickProc lpQuickProc)
{
     SoftTimerCtrlBlock.lpQuickProc = lpQuickProc;
}
// return a free soft timer ctrol block  from free array,  InvalidId if there is no free one .
uint8_t  GetFreeSoftTimer(void) 
{// just  return the first free id.  return InvalidId ,
    uint8_t res ;
    res = SoftTimerCtrlBlock.freeheader ;
	assert(res	!= InvalidId );
    SoftTimerCtrlBlock.freeheader =  SoftTimerCtrlBlock.nextid[res] ;
    SoftTimerCtrlBlock.usedcnt ++ ;
    return res;
}
// give up set new softtimer ,  it used after call GetFreeSoftTimer();
void  ReleaseFreeSoftTimer(uint8_t res) 
{// put a index to release array.
    assert(res<SoftTimerCnt);
    assert(SoftTimerCtrlBlock.usedcnt);
    SoftTimerCtrlBlock.nextid[res] =SoftTimerCtrlBlock.freeheader ;
    SoftTimerCtrlBlock.freeheader = res ;
    SoftTimerCtrlBlock.usedcnt -- ;
}
LPSoftTimer  GetSoftTimerData(uint8_t index)
{
    assert(index<SoftTimerCnt);
    return &SoftTimerCtrlBlock.ctrl[index] ;
}
// before start should set the delay, evt , repeat , and report lpblock index .
void StartSoftTimer(uint8_t index)
{
    uint32_t temp32 ;
    uint8_t  tempindex ;
    uint8_t  lastindex ;
    assert( index<SoftTimerCnt);
    assert(SoftTimerCtrlBlock.ctrl[index].delay!=0);
    
    if(CheckPeriph(Clock_LPTIM) == 0)
        EnablePeriph(Clock_LPTIM);
    
    SoftTimerCtrlBlock.ctrl[index].differ =  SoftTimerCtrlBlock.ctrl[index].delay;
    INT_DISABLE();
	if((SoftTimerCtrlBlock.block[index] & SftOpt_Repeat) ==0)
	{ // single timer .    
        if(SoftTimerCtrlBlock.ccr1header == InvalidId)
        {//第一个计时器未运行，当前列表空 将当前的延时值作为寄存器1的延时值 设置到寄存器，使能计时器1，
         //  设置当前Nextid为无效， 保存本序列首id为当前id。
            SoftTimerCtrlBlock.ccr1header = index ;
            SoftTimerCtrlBlock.nextid[index] = InvalidId ;
            temp32 = SoftTimerCtrlBlock.ctrl[index].differ ;
            SoftTimerCtrlBlock.ctrl[index].differ = 0 ;
            if(IsLPTIMRun())
              temp32 +=  GetLPTIMCNT();
            SetLPTIMCCR1(temp32 & Sft_OVRFLW_MASK) ;
            SetLPTIMISR(LPTTIM_CCR1_INTERRUPT);
            SetLPTIMIER (GetLPTIMIER()|LPTTIM_CCR1_INTERRUPT); 
            RunLPTIM();   
            INT_ENABLE();
            return;            
        }
        else
        {// 计时器1运行中， 当前列表非空 ， 判断第一个的延时值是否小于新值 ， 如果小于则在列表中循环直到找到第一个延时值大于新值的项
         //   将当前的项插入到这个项前， 否则将当前项加在最后一个项后面。
         // check if the first time is quickly than the new time .
			if(GetLPTIMCCR1() < GetLPTIMCNT())
			{
				temp32 = GetLPTIMCCR1() + Sft_OVRFLW_VALUE ;
				temp32 -= GetLPTIMCNT() ;
			}
			else
			{
				temp32 = GetLPTIMCCR1() - GetLPTIMCNT() ;
			}            
            if(temp32 < SoftTimerCtrlBlock.ctrl[index].differ )
            { // new item bigger than first , try to find the fitable position ,and insert new item to it.
                SoftTimerCtrlBlock.ctrl[index].differ -= temp32 ;
                lastindex = SoftTimerCtrlBlock.ccr1header ;
                tempindex = SoftTimerCtrlBlock.nextid[lastindex];
                /*if(tempindex == InvalidId)
                { // only one item running , and new to the tail .
                   SoftTimerCtrlBlock.nextid[lastindex] = index;
                   SoftTimerCtrlBlock.nextid[index] = InvalidId;
                   INT_ENABLE();
                   return;
                    
                }*/
                // else , there are multiple item .
                while(tempindex != InvalidId )
                {   
                    if(SoftTimerCtrlBlock.ctrl[tempindex].differ< SoftTimerCtrlBlock.ctrl[index].differ )
                    {
                        SoftTimerCtrlBlock.ctrl[index].differ -= SoftTimerCtrlBlock.ctrl[tempindex].differ;
                        lastindex = tempindex ;
                        tempindex = SoftTimerCtrlBlock.nextid[tempindex];
                    }
                    else
                    {// find position ,insert the new one before current one .
                        SoftTimerCtrlBlock.nextid[lastindex] = index;
                        SoftTimerCtrlBlock.nextid[index] = tempindex;
                        SoftTimerCtrlBlock.ctrl[tempindex].differ -= SoftTimerCtrlBlock.ctrl[index].differ;
                    }
                }
                if(tempindex == InvalidId)
                { // only one item running , or has finded all item but no one small that new one  ,put it at the last  
                     SoftTimerCtrlBlock.nextid[lastindex] = index;
                     SoftTimerCtrlBlock.nextid[index] = InvalidId;
                }
                INT_ENABLE();
                return;               
            }                
            else
            { // first is bigger than new item , replace first with new item .
                tempindex = SoftTimerCtrlBlock.ccr1header;
                SoftTimerCtrlBlock.ctrl[tempindex].differ = GetLPTIMCCR1() -temp32 - SoftTimerCtrlBlock.ctrl[index].differ;
                temp32 = GetLPTIMCNT() + SoftTimerCtrlBlock.ctrl[index].differ;
				SetLPTIMCCR1( temp32 & Sft_OVRFLW_MASK );
                SoftTimerCtrlBlock.ccr1header = index ;
                SoftTimerCtrlBlock.nextid[index] =  tempindex ;
                SoftTimerCtrlBlock.ctrl[index].differ = 0 ;
                INT_ENABLE();
                return; 
            }
        }
          
    }
    else
    {// this is a loop item . put it in the second list
         // get the remain delay ticks  . 
         if(IsLPTIMRun())
         {
             if(GetLPTIMCCR2() < GetLPTIMCNT())
             {
                temp32 = GetLPTIMCCR2() + Sft_OVRFLW_VALUE ;
                temp32 -= GetLPTIMCNT() ;
             }else
             {
                temp32 =  GetLPTIMCCR2()- GetLPTIMCNT() ;
             }
         }
         else
         {
             temp32 = 0 ;
         }
     	 // add new one to first position .
		 lastindex = SoftTimerCtrlBlock.ccr2header ;
		 SoftTimerCtrlBlock.nextid[index]= lastindex ;
		 SoftTimerCtrlBlock.ccr2header = index ;	
		 // fresh the differ value .except the first one 
		 while(lastindex != InvalidId)
		 {
            SoftTimerCtrlBlock.ctrl[lastindex].differ += temp32 ;
			lastindex = SoftTimerCtrlBlock.nextid[lastindex]	;
		 }
		 // get the smallest differ .
		 temp32 =  Sft_OVRFLW_VALUE;
		 lastindex = index ;
		 while(lastindex != InvalidId)
		 {
            if(SoftTimerCtrlBlock.ctrl[lastindex].differ <= temp32 )
                temp32 = SoftTimerCtrlBlock.ctrl[lastindex].differ ;
			lastindex = SoftTimerCtrlBlock.nextid[lastindex]	;
		 }
		 // set the new wait value
		 SetLPTIMCCR2((temp32  + GetLPTIMCNT()) & Sft_OVRFLW_MASK);
   	     // fresh the all differ value
		 lastindex = index ;
		 while(lastindex != InvalidId)
		 {
            SoftTimerCtrlBlock.ctrl[lastindex].differ -= temp32 ;
            lastindex = SoftTimerCtrlBlock.nextid[lastindex]	;
		 }	
		 SetLPTIMIER(GetLPTIMIER()|LPTTIM_CCR2_INTERRUPT);
         RunLPTIM();
         INT_ENABLE();
    }//end 

}

// stop a timer  that has been start by  StartSoftTimer.
void StopSoftTimer(uint8_t index)
{// find it in two list . then remove it from the list  and add it's delay to the delay of the one after it  
    uint8_t tempindex ;
    uint8_t lastindex ;
    uint32_t temp32 ;
    assert(index < SoftTimerCnt);
    INT_DISABLE();
    if((SoftTimerCtrlBlock.block[index] & SftOpt_Repeat) ==0)
    {
        if(SoftTimerCtrlBlock.ccr1header == InvalidId )
            goto FailFind ;

        tempindex = SoftTimerCtrlBlock.ccr1header;
        lastindex = SoftTimerCtrlBlock.nextid[tempindex] ;
        if(tempindex == index)
        { // remove the header
            if(lastindex != InvalidId )
            {
                temp32 = GetLPTIMCCR1() +  SoftTimerCtrlBlock.ctrl[lastindex].differ;
				SetLPTIMCCR1 (temp32 & Sft_OVRFLW_MASK);
                SoftTimerCtrlBlock.ccr1header = lastindex ;
                SoftTimerCtrlBlock.ctrl[lastindex].differ = 0 ;
            }
            else
            {   // dsiable the interrupt for this compare . (CCR1)
                SetLPTIMIER(GetLPTIMIER() & (~LPTTIM_CCR1_INTERRUPT) );
                if(GetLPTIMIER() == 0)
                {
                    StopLPTIM() ;
                    DisablePeriph(Clock_LPTIM);
                }                    
                SoftTimerCtrlBlock.ccr1header = InvalidId ;
            }
            goto FreeIndex ;
        }
        else
        {
            while(lastindex != InvalidId)
            {
                if(lastindex == index)
                {
                    SoftTimerCtrlBlock.nextid[tempindex]  =  SoftTimerCtrlBlock.nextid[lastindex] ;
                    lastindex = SoftTimerCtrlBlock.nextid[tempindex] ;
                    if(lastindex != InvalidId)
                    {
                        SoftTimerCtrlBlock.ctrl[lastindex].differ +=  SoftTimerCtrlBlock.ctrl[index].differ ;
                    }
                    // clr the interrupt flage . it may set this time 
                    SetLPTIMISR(LPTTIM_CCR1_INTERRUPT) ;
                    goto FreeIndex ;
                }
                else
                {
                    tempindex = lastindex;
                    lastindex = SoftTimerCtrlBlock.nextid[lastindex] ;
                }
            }
        }
    }
    else
    {//  find in second list . loop list
        if(SoftTimerCtrlBlock.ccr2header == InvalidId )
            goto FailFind ;
     // find it in loop list . just remove it 
        lastindex = SoftTimerCtrlBlock.ccr2header;
        tempindex = SoftTimerCtrlBlock.nextid[lastindex] ;
        if(lastindex == index)
        { // remove the header
			SoftTimerCtrlBlock.ccr2header = tempindex ;
			if(tempindex == InvalidId )
            {
                SetLPTIMIER( GetLPTIMIER() &LPTTIM_CCR1_INTERRUPT);
                if(GetLPTIMIER() == 0)
                {
                    StopLPTIM() ;
                    DisablePeriph(Clock_LPTIM);
                }                
            }
            goto FreeIndex ;
        }
        else
        {
            while(tempindex != InvalidId)
            {
                if(tempindex == index)
                {
                    SoftTimerCtrlBlock.nextid[lastindex]  =  SoftTimerCtrlBlock.nextid[tempindex] ;
                    SetLPTIMISR(LPTTIM_CCR2_INTERRUPT) ;
                    goto FreeIndex ;
                }
                else
                {
                    lastindex = tempindex;
                    tempindex = SoftTimerCtrlBlock.nextid[lastindex] ;
                }
            }
        }
    }
 FailFind :    
    assert(0);
	// not find the index .just return .
    INT_ENABLE();
    return ;
 FreeIndex :    
    // let it float if it has SftOpt_Lock option bit .
    if((SoftTimerCtrlBlock.block[index] & SftOpt_Lock) == 0)
    {
        // put this one to free list .
        SoftTimerCtrlBlock.nextid[index] =SoftTimerCtrlBlock.freeheader ;
        SoftTimerCtrlBlock.freeheader = index ;
        SoftTimerCtrlBlock.usedcnt -- ;
    }
    INT_ENABLE();
}

void ReleaseSoftTimer(uint8_t index)
{
    SoftTimerCtrlBlock.block[index] &= (~SftOpt_Lock);
    StopSoftTimer(index);
}

// the current softtimer should still live . chang the delay time and restart it .
void ResetSoftTimer(uint8_t index,uint32_t delay)
{
    uint8_t preindex,tempindex ;
    INT_DISABLE();

    StopSoftTimer(index);
    // the index is stored at the free list . remove it  normally should be  the first pos of free list. 
    tempindex =SoftTimerCtrlBlock.freeheader ;
    preindex = InvalidId ;
    while(tempindex != InvalidId)
    {
        if(tempindex == index)
        {
            if(preindex == InvalidId)
            {// the index is at the header of the  freelist . remove it .
                SoftTimerCtrlBlock.freeheader = SoftTimerCtrlBlock.nextid[tempindex]  ;
            }
            else
            {//
                SoftTimerCtrlBlock.nextid[preindex] =SoftTimerCtrlBlock.nextid[tempindex] ;
            }
            SoftTimerCtrlBlock.usedcnt ++ ;
            if(delay != RESTARTDELAY ) 
                SoftTimerCtrlBlock.ctrl[index].delay = Ms2Tick(delay) ;            
            StartSoftTimer(index);
            break ;
        }
        else
        {
            preindex = tempindex ;
            tempindex = SoftTimerCtrlBlock.nextid[preindex];
        }
    }
    
    INT_ENABLE();
     
    return ;
}

void ResetSoftTimerEvt(StdEvt evt,uint32_t delay)
{
    uint8_t index ;
    evt  = getevtid(evt);
    for(index = 0 ; index <SoftTimerCnt ; index++)
    {
        if(getevtid(SoftTimerCtrlBlock.evt[index])  == evt)
        {
            ResetSoftTimerEvt(index,delay); 
            return;
        }
    }
}
// just stop the first specail evt .

void ReleaseSoftTimerEvt(StdEvt evt)
{
    uint8_t index ;
    evt  = getevtid(evt);
    for(index = 0 ; index <SoftTimerCnt ; index++)
    {
        if(getevtid(SoftTimerCtrlBlock.evt[index])  == evt)
        {
            ReleaseSoftTimer(index); 
            return;
        }
    }
}


void LPTIM_IRQHandler(void)
{ // driver the motor1-3 , according to the motor Cur mode and data and Wish mode and Data
    uint8_t tempindex ;
    uint8_t lastindex ;
    uint32_t temp32 ;
    uint32_t res = GetLPTIMISR() ;
    SetLPTIMISR( res ); 
    res &= GetLPTIMIER() ;
    
    if(res & (1<<0))
    { // cc1 interrupt.   first is overtime . post it . set next one 
        tempindex = SoftTimerCtrlBlock.ccr1header ;
        
 /*       
        lastindex = SoftTimerCtrlBlock.ccr1header ;
        tempindex = SoftTimerCtrlBlock.nextid[lastindex] ;
        
        if(SoftTimerCtrlBlock.block[lastindex] & SftOpt_Quick)
        {   
            assert(SoftTimerCtrlBlock.lpQuickProc != 0);
            (*SoftTimerCtrlBlock.lpQuickProc)(SoftTimerCtrlBlock.evt[lastindex]);
        }
        else
            postevtbyindex(SoftTimerCtrlBlock.block[lastindex] & SftOpt_Mask,SoftTimerCtrlBlock.evt[lastindex]);
        if((SoftTimerCtrlBlock.block[lastindex] & SftOpt_Lock) == 0)
        {
            // return it to free list .
            SoftTimerCtrlBlock.nextid[lastindex] =SoftTimerCtrlBlock.freeheader ;
            SoftTimerCtrlBlock.freeheader = lastindex ;
            SoftTimerCtrlBlock.usedcnt --;
        }
        // return end. */
        while (tempindex  != InvalidId)
        {// find all delay is 0 item . and post it
            if(SoftTimerCtrlBlock.ctrl[tempindex].differ != 0)
            {// find the other one ,stop post .
                temp32 = GetLPTIMCNT() + SoftTimerCtrlBlock.ctrl[tempindex].differ ;
                SetLPTIMCCR1( temp32 & Sft_OVRFLW_MASK );
                SoftTimerCtrlBlock.ccr1header = tempindex ;
                break ;
            }
            lastindex = tempindex ;
            tempindex = SoftTimerCtrlBlock.nextid[lastindex] ;
            if(SoftTimerCtrlBlock.block[lastindex] & SftOpt_Quick)
            {   
                assert(SoftTimerCtrlBlock.lpQuickProc != 0);
                (*SoftTimerCtrlBlock.lpQuickProc)(SoftTimerCtrlBlock.evt[lastindex]);
            }            
            else
                postevtbyindex(SoftTimerCtrlBlock.block[lastindex] & SftOpt_Mask,SoftTimerCtrlBlock.evt[lastindex]);
            if((SoftTimerCtrlBlock.block[lastindex] & SftOpt_Lock) == 0)
            {
                SoftTimerCtrlBlock.nextid[lastindex] =SoftTimerCtrlBlock.freeheader ;
                SoftTimerCtrlBlock.freeheader = lastindex ;
                SoftTimerCtrlBlock.usedcnt --;
            }
        }
        
		if(tempindex  == InvalidId)
		{ // no item is this list . stop it
            SetLPTIMIER( GetLPTIMIER()&LPTTIM_CCR2_INTERRUPT );  // bit1 maybe is setted
            if(GetLPTIMIER() == 0)
            {
                StopLPTIM() ;
                DisablePeriph(Clock_LPTIM);
            }
            SoftTimerCtrlBlock.ccr1header = InvalidId ;  		
		}
    }
    if(res & (1<<1))
    { // cc2  interrupt.  there maybe item is overtime . post it . set next one 
        lastindex = SoftTimerCtrlBlock.ccr2header ;
		temp32 =  Sft_OVRFLW_VALUE;
        while (lastindex  != InvalidId)
        {// find all delay is 0 item . and get the smallest differ
            if(SoftTimerCtrlBlock.ctrl[lastindex].differ == 0)
            {// find the other one ,stop post .
                if(SoftTimerCtrlBlock.block[lastindex] & SftOpt_Quick)
                {   
                    (*SoftTimerCtrlBlock.lpQuickProc)(SoftTimerCtrlBlock.evt[lastindex]);
                }                                           
                else
                    postevtbyindex(SoftTimerCtrlBlock.block[lastindex] & SftOpt_Mask,SoftTimerCtrlBlock.evt[lastindex]);
				SoftTimerCtrlBlock.ctrl[lastindex].differ = SoftTimerCtrlBlock.ctrl[lastindex].delay ;
			}
			if(SoftTimerCtrlBlock.ctrl[lastindex].differ <= temp32 )
				temp32 = SoftTimerCtrlBlock.ctrl[lastindex].differ ;
			lastindex = SoftTimerCtrlBlock.nextid[lastindex] ;	
        }
      // set the new wait value
      SetLPTIMCCR2 ((temp32  + GetLPTIMCNT()) & Sft_OVRFLW_MASK );

      // fresh the all differ value
      lastindex = SoftTimerCtrlBlock.ccr2header ;
      while(lastindex != InvalidId)
      {
          SoftTimerCtrlBlock.ctrl[lastindex].differ -= temp32 ;
          lastindex = SoftTimerCtrlBlock.nextid[lastindex]	;
      }	
   }

}





