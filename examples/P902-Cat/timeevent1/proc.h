#ifndef  __Proc_Long_
#define  __Proc_Long_
//  long proc block , here define the long event proc function .
/*  ��̬�߳� ������evtos.h�У��� ����ϵͳ�ж�һ��32��LongProcData ����Ϊϵͳ�Ĺ��þ�̬�߳����ݴ洢����  ���ͬʱ�������32����̬�߳�ͬʱ����
    ��̬�̱߳��������һ��Block�£�����Block����Ϣ�������У�  ÿһ��Block�����Ծ߱�255�� ��̬�̱߳��á�
    ������һ��BlockҪ���ؾ�̬�̣߳� ���� ��Ҫ��Block�ĳ�ʼ�������У�void HalMasterInit(void)��ͨ�� SetProcAddr(LPThdBlock lpb,  LPLongProc const *  addr) ����Block��Ҫ�õ��Ѷ���Ŀ��þ�̬�̵߳ĵ�ַ�������õ�Block�У� ������鶨����
    ���ļ��� ÿһ����Ҫ���ص�Block������һ��������Block��  ��������о�̬�����Ĵ���Ϊ��̬������id ��0-255��
    
    ��ξ�̬������Ҫ��������ſ������У� ������ʽΪSetActiveProc  procid Ϊ��̬������id�� 
    // ����Block��ǰ�������е�LongProc��EvtΪ����Block�ĳ�ʼ�¼� ��ע�� LongProcʵ��һ��������ܵ���ǰBlock��������Ϣ����������Ϣ���й��ˣ�ֻ����������ע����Ϣ��
    void  SetActiveProc(LPThdBlock lpb,uint8_t procid, StdEvt evt) ;
    // ȡ��ĳһ��LongProc�� ���ͨ����LongProc�������ʱ���е��á� 
    void  DelActiveProc(LPThdBlock lpb,uint8_t procid) ;    
    
*/
#include "basictype.h"
#include "evtos.h"


#endif 
