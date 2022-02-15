#include "halthd.h"
#include "timealarm.h"

//process halmaster  used to receive the hal event ,
volatile HalMaster halmaster ;
StdEvt HalMasterEvtLoop[Max_HalMasterEvt];


// pre declare end 
void HalMasterInit(void)
{
    // load global config data. 
    LoadGlobalData();

}

void HalMaster_initial(LPThdBlock const lpb, StdEvt evt)
{ 
    uint16_t temp ;
    uint8_t *lpTx ;
    uint16_t evtid = getevtid(evt);
    switch (evtid) 
    {
        case Sig_Key_Up: 
        {
            return;
        }  
        case Sig_Key_Down :
        {
            return;
        }
        
        case Sig_Motor2_Work :
        {
            SetMotor(0,Motor_BackStable,100,4);
            break;
        }
        case Sig_Motor2_Stop :
        {
            SetMotor(0,Motor_PowerOffFree,0,0);
            break;
        }
        case Sig_Magnet1_Remove :
        {  // do rtc state report .
            return  ;          
        }
        case Sig_Magnet1_On :
        {
            //AdcStart(ADC_ChanTemp|ADC_ChanVRef);
            //postevt(lpb,Sig_Motor2_Test);
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);   
            lpTx[0] = 'S' ;
            lpTx[1] = 'i' ;
            lpTx[2] = 'g' ;
            lpTx[3] = ' ' ;
            lpTx[4] = 'M' ;
            lpTx[5] = '1' ;
            lpTx[6] = ' ' ;
            lpTx[7] = 'O' ;
            lpTx[8] = 'N' ;
            lpTx[9] = '!'  ;
            lpTx[10] = '.' ;
            lpTx[11] = 'A' +freepoolcnt(0);   
            LpUart0Send(evt );  

            return   ;          
        }        
        case Sig_Magnet2_Remove :
        {
             return  ;          
        } 
        case Sig_Magnet2_On :
        {  // no buffer
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);   
            lpTx[0] = 'S' ;
            lpTx[1] = 'i' ;
            lpTx[2] = 'g' ;
            lpTx[3] = ' ' ;
            lpTx[4] = 'M' ;
            lpTx[5] = '2' ;
            lpTx[6] = ' ' ;
            lpTx[7] = 'O' ;
            lpTx[8] = 'N' ;
            lpTx[9] = '!'  ;
            lpTx[10] = '.' ;
            lpTx[11] = 'A' +freepoolcnt(0);   
            LpUart0Send(evt);  

            return  ;          
        }        
        case Sig_Time_shark :
        { // has buf .
           
            lpTx   = (uint8_t *) getevtmem(evt);
            lpTx[3] = '0' + lpTx[0];
            lpTx[0] = 'S' ;
            lpTx[1] = 'i' ;
            lpTx[2] = 'g' ;
            
            lpTx[4] = 'I' ;
            lpTx[5] = 'A' + freepoolcnt(0);
            lpTx[6] = '0'; 
            lpTx[7] = RTC->BCDDAY ;
            lpTx[8] = RTC->BCDHOUR;  //hour
            lpTx[9] = RTC->BCDMIN ; //min
            lpTx[10] = RTC->BCDSEC  ;//sec
            lpTx[11] = '#' ;  
            LpUart0Send(evt);    
            return ;

        }
        case Sig_Rtc_FixHourInt :
        { // for test
            AdcStart(ADC_ChanTemp|ADC_ChanVRef);
            return;
        }        
        case Sig_Adc_Done :
        { // for test adc
            CalcAdcResult();
            if(GetAdcResult(IndVbat) < 66)
            { // vbat low power board go to  stop mode  TESTTTJ -remove
              // BCS £º open the power to short time . there is big capitor need to be charg , it has not been charged enough.  
                //GotoLowPowerMode();
            }
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'A' ;
            lpTx[1] = 'd' ;
            lpTx[2] = 'c' ;
            lpTx[3] = RTC->ADJUST ;
            temp = GetAdcResult(IndTSens);
            lpTx[4] =(uint8_t)temp ;
            temp >>= 8 ;
            lpTx[5] = (uint8_t)temp ;
            lpTx[6] = ' ' ;
            temp = GetAdcValue(0);
            lpTx[7] =(uint8_t)temp ;
            temp >>= 8 ;
            lpTx[8] = (uint8_t)temp ;
            
            temp = GetAdcResult(IndVRef);
            lpTx[9] =(uint8_t)temp ;
            temp >>= 8 ;
            lpTx[10] = (uint8_t)temp ;
            lpTx[11] = 'A' +freepoolcnt(0);   
            LpUart0Send(evt); 
            return;
        }  

        case Sig_50Hz_Overtime :
           On50HzOvertime();     
            return;
        case Sig_Ir_OVT :    
        { // for test adc
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'I' ;
            lpTx[1] = 'r' ;
            lpTx[2] = 'd' ;
            lpTx[3] = 'e' ;
            lpTx[4] = 't' ;
            lpTx[5] = 'e' ;
            lpTx[6] = ' ' ;
            lpTx[7] = 'O';
            lpTx[8] = 'V';
            lpTx[9] = 'T'  ;
            lpTx[10] = 'M' ;
            lpTx[11] = ' ' ;  
            LpUart0Send(evt); 
            return;
        }
        case Sig_Ir_Recv_Right :    
        { // for test adc
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'I' ;
            lpTx[1] = 'r' ;
            lpTx[2] = 'd' ;
            lpTx[3] = 'e' ;
            lpTx[4] = 't';
            lpTx[5] = 'e' ;
            lpTx[6] = ' ' ;
            lpTx[7] = 'G';
            lpTx[8] = 'E';
            lpTx[9] = 'T'  ;
            lpTx[10] = 'O' ;
            lpTx[11] = 'K' ;   
            LpUart0Send(evt); 
            return;
        }        
        case Sig_Ir_Recv_Error :    
        { // for test adc
            evt = newevt(Sig_None,PoolId1);
            lpTx = (uint8_t * )getevtmem( evt);
            lpTx[0] = 'I' ;
            lpTx[1] = 'r' ;
            lpTx[2] = 'd' ;
            lpTx[3] = 'e' ;
            lpTx[4] = 't';
            lpTx[5] = 'e' ;
            lpTx[6] = ' ' ;
            lpTx[7] = 'G';
            lpTx[8] = 'E';
            lpTx[9] = 'T'  ;
            lpTx[10] = 'E' ;
            lpTx[11] = 'r' ;   
            LpUart0Send(evt); 
            return;
        } 

        
    }// end switch
    freeevt(evt);
}


