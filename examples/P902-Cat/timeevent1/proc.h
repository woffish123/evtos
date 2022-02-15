#ifndef  __Proc_Long_
#define  __Proc_Long_
//  long proc block , here define the long event proc function .
/*  静态线程 定义在evtos.h中，： 整个系统中定一个32个LongProcData ，作为系统的公用静态线程数据存储区，  因此同时最大允许32个静态线程同时运行
    静态线程必须挂载在一个Block下，借用Block的消息机制运行，  每一个Block最多可以具备255个 静态线程备用。
    因此如果一个Block要挂载静态线程， 首先 需要在Block的初始化过程中（void HalMasterInit(void)）通过 SetProcAddr(LPThdBlock lpb,  LPLongProc const *  addr) 将该Block需要得到已定义的可用静态线程的地址数组设置到Block中， 这个数组定义在
    本文件中 每一个需要挂载的Block可以有一个单独的Block，  这个数组中静态函数的次序为静态函数的id 【0-255】
    
    其次静态函数需要被启动后才可以运行， 启动方式为SetActiveProc  procid 为静态函数的id。 
    // 设置Block当前允许运行的LongProc，Evt为发给Block的初始事件 ，注意 LongProc实际一旦允许会受到当前Block的所有消息，其必须对消息进行过滤，只处理其所关注的消息。
    void  SetActiveProc(LPThdBlock lpb,uint8_t procid, StdEvt evt) ;
    // 取消某一个LongProc， 这个通常由LongProc在其结束时自行调用。 
    void  DelActiveProc(LPThdBlock lpb,uint8_t procid) ;    
    
*/
#include "basictype.h"
#include "evtos.h"


#endif 
