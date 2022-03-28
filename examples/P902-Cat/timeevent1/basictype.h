// this file defined some basic common type .
#ifndef  __BASIC_TYPE_H_
#define  __BASIC_TYPE_H_

#include <stdint.h>
#include <fm33lc0xx.h>
#include "logbyte.h"
#define TRUE  1
#define true  1 
#define FALSE 0
#define false 0
#define NULL  0

#define InvalidId        0xff

// don't use the system  SetBit , and GetBit , it is too slowly for time critic use
#define SetRegBit( reg , bit)  (reg) |= (bit)
#define ClrRegBit( reg , bit)  (reg) &= (~(bit))
#define CheckRegBit(reg, bit)  (reg) &  (bit)     

// the MDK use the litte end mode . lower byte stored in lower address 
// so Data8.data0  = var16.data8.data0 .  = var32 [7:0]
typedef struct data16_
{
	uint16_t data0;
	uint16_t data1;
}Data16;

typedef struct data8_
{
	uint8_t data0;
	uint8_t data1;
	uint8_t data2;
	uint8_t data3;
}Data8;


// used to hold the timeevt param.
typedef union  variable32_
{
	uint32_t  data32;
	Data16    data16;
    Data8     data8 ; 
}Var32;

typedef union variable16_
{
    uint16_t data16;
	struct 
    {
        uint8_t data0;
        uint8_t data1;
    } data8; 
}Var16;

typedef uint16_t TickEvt;

// calc how many bit in a item .
__STATIC_INLINE uint8_t CaclBitCnt8(uint8_t value)
{
    value = ( value & 0x55 ) + ( (value >> 1)  & 0x55 ) ; 
    value = ( value & 0x33 ) + ( (value >> 2)  & 0x33 ) ; 
    value = ( value & 0x0f ) + ( (value >> 4)  & 0x0f ) ;
    return value ;
}
__STATIC_INLINE uint8_t CaclBitCnt16(uint16_t value)
{
    value = ( value & 0x5555 ) + ( (value >>1)  & 0x5555 ) ; 
    value = ( value & 0x3333 ) + ( (value >>2)  & 0x3333 ) ; 
    value = ( value & 0x0f0f ) + ( (value >>4)  & 0x0f0f ) ; 
    value = ( value & 0x000f ) + ( (value >>8)  & 0x000f ) ; 
    return (uint8_t )value ;
}

__STATIC_INLINE uint8_t CaclBitCnt32(uint32_t value)
{
    value = ( value & 0x55555555 ) + ( (value >>1)  & 0x55555555 ) ; 
    value = ( value & 0x33333333 ) + ( (value >>2)  & 0x33333333 ) ; 
    value = ( value & 0x0f0f0f0f ) + ( (value >>4)  & 0x0f0f0f0f ) ; 
    value = ( value & 0x00ff00ff ) + ( (value >>8)  & 0x00ff00ff ) ; 
    value = ( value & 0x0000ffff ) + ( (value >>16) & 0x0000ffff ) ; 
    return (uint8_t )value ;
}

// get the first bit 1 in a item based on 1 at low order . 
// return index 1-8,16,32  ,but NOTICE if the input is zero , it return 0.
__STATIC_INLINE uint8_t CalcLsb8(uint8_t data) 
{
    uint8_t res = 7;
    if(data &0x0f)    { res  -= 4  ; data &= 0x0f; }
    if(data &0x33)    { res  -= 2  ; data &= 0x33; }   
    if(data &0x55)     res -= 1 ;
    return res ;
}

__STATIC_INLINE uint8_t CalcLsb16(uint16_t data) 
{
    uint8_t res = 15;
    if(data &0x00ff)    { res  -= 8  ; data &= 0x00ff; }
    if(data &0x0f0f)    { res  -= 4  ; data &= 0x0f0f; }
    if(data &0x3333)    { res  -= 2  ; data &= 0x3333; }   
    if(data &0x5555)     res -= 1 ;
    return res ;
}

__STATIC_INLINE uint8_t CalcLsb32(uint32_t data) 
{
    uint8_t res = 31;
    if(data &0x0000ffff)    { res  -= 16 ; data &= 0x0000ffff; }
    if(data &0x00ff00ff){ res  -= 8  ; data &= 0x00ff00ff; }
    if(data &0x0f0f0f0f){ res  -= 4  ; data &= 0x0f0f0f0f; }
    if(data &0x33333333){ res  -= 2  ; data &= 0x33333333; }   
    if(data &0x55555555)  res -= 1 ;
    return res ;
}

// get the first bit 1 in a item at high order . 
// NOTICE  return 0 if data = 0 
__STATIC_INLINE uint8_t CalcHsb8(uint8_t data) 
{
    uint8_t res = 0;
    if(data &0xf0)    { res  += 4  ; data &= 0xf0; }
    if(data &0xcc)    { res  += 2  ; data &= 0xcc; }   
    if(data &0xaa)     res += 1 ;
    return res ;
}

__STATIC_INLINE uint8_t CalcHsb16(uint16_t data) 
{
    uint8_t res = 0;
    if(data &0xff00)    { res  += 8  ; data &= 0xff00; }
    if(data &0xf0f0)    { res  += 4  ; data &= 0xf0f0; }
    if(data &0xcccc)    { res  += 2  ; data &= 0xcccc; }   
    if(data &0xaaaa)     res += 1 ;
    return res ;
}

__STATIC_INLINE uint8_t CalcHsb32(uint32_t data) 
{
    uint8_t res = 0;
    if(data &0xffff0000)    { res  += 16 ; data &= 0xffff0000; }
    if(data &0xff00ff00){ res  += 8  ; data &= 0xff00ff00; }
    if(data &0xf0f0f0f0){ res  += 4  ; data &= 0xf0f0f0f0; }
    if(data &0xcccccccc){ res  += 2  ; data &= 0xcccccccc; }   
    if(data &0xaaaaaaaa)  res += 1 ;
    return res ;
}

//BIT MAP======================================================
#define BIT0            0x00000001
#define BIT1            0x00000002
#define BIT2            0x00000004
#define BIT3            0x00000008
#define BIT4            0x00000010
#define BIT5            0x00000020
#define BIT6            0x00000040
#define BIT7            0x00000080
#define BIT8            0x00000100
#define BIT9            0x00000200
#define BIT10           0x00000400
#define BIT11           0x00000800
#define BIT12           0x00001000
#define BIT13           0x00002000
#define BIT14           0x00004000
#define BIT15           0x00008000
#define BIT16           0x00010000
#define BIT17           0x00020000
#define BIT18           0x00040000
#define BIT19           0x00080000
#define BIT20           0x00100000
#define BIT21           0x00200000
#define BIT22           0x00400000
#define BIT23           0x00800000
#define BIT24           0x01000000
#define BIT25           0x02000000
#define BIT26           0x04000000
#define BIT27           0x08000000
#define BIT28           0x10000000
#define BIT29           0x20000000
#define BIT30           0x40000000
#define BIT31           0x80000000
#define BITNO           0x00000000

#define B0000_0000 0x00
#define B0000_0001 0x01
#define B0000_0010 0x02
#define B0000_0011 0x03
#define B0000_0100 0x04
#define B0000_0101 0x05
#define B0000_0110 0x06
#define B0000_0111 0x07
#define B0000_1000 0x08
#define B0000_1001 0x09
#define B0000_1010 0x0A
#define B0000_1011 0x0B
#define B0000_1100 0x0C
#define B0000_1101 0x0D
#define B0000_1110 0x0E
#define B0000_1111 0x0F

#define B0001_0000 0x10
#define B0001_0001 0x11
#define B0001_0010 0x12
#define B0001_0011 0x13
#define B0001_0100 0x14
#define B0001_0101 0x15
#define B0001_0110 0x16
#define B0001_0111 0x17
#define B0001_1000 0x18
#define B0001_1001 0x19
#define B0001_1010 0x1A
#define B0001_1011 0x1B
#define B0001_1100 0x1C
#define B0001_1101 0x1D
#define B0001_1110 0x1E
#define B0001_1111 0x1F

#define B0010_0000 0x20
#define B0010_0001 0x21
#define B0010_0010 0x22
#define B0010_0011 0x23
#define B0010_0100 0x24
#define B0010_0101 0x25
#define B0010_0110 0x26
#define B0010_0111 0x27
#define B0010_1000 0x28
#define B0010_1001 0x29
#define B0010_1010 0x2A
#define B0010_1011 0x2B
#define B0010_1100 0x2C
#define B0010_1101 0x2D
#define B0010_1110 0x2E
#define B0010_1111 0x2F

#define B0011_0000 0x30
#define B0011_0001 0x31
#define B0011_0010 0x32
#define B0011_0011 0x33
#define B0011_0100 0x34
#define B0011_0101 0x35
#define B0011_0110 0x36
#define B0011_0111 0x37
#define B0011_1000 0x38
#define B0011_1001 0x39
#define B0011_1010 0x3A
#define B0011_1011 0x3B
#define B0011_1100 0x3C
#define B0011_1101 0x3D
#define B0011_1110 0x3E
#define B0011_1111 0x3F

#define B0100_0000 0x40
#define B0100_0001 0x41
#define B0100_0010 0x42
#define B0100_0011 0x43
#define B0100_0100 0x44
#define B0100_0101 0x45
#define B0100_0110 0x46
#define B0100_0111 0x47
#define B0100_1000 0x48
#define B0100_1001 0x49
#define B0100_1010 0x4A
#define B0100_1011 0x4B
#define B0100_1100 0x4C
#define B0100_1101 0x4D
#define B0100_1110 0x4E
#define B0100_1111 0x4F

#define B0101_0000 0x50
#define B0101_0001 0x51
#define B0101_0010 0x52
#define B0101_0011 0x53
#define B0101_0100 0x54
#define B0101_0101 0x55
#define B0101_0110 0x56
#define B0101_0111 0x57
#define B0101_1000 0x58
#define B0101_1001 0x59
#define B0101_1010 0x5A
#define B0101_1011 0x5B
#define B0101_1100 0x5C
#define B0101_1101 0x5D
#define B0101_1110 0x5E
#define B0101_1111 0x5F

#define B0110_0000 0x60
#define B0110_0001 0x61
#define B0110_0010 0x62
#define B0110_0011 0x63
#define B0110_0100 0x64
#define B0110_0101 0x65
#define B0110_0110 0x66
#define B0110_0111 0x67
#define B0110_1000 0x68
#define B0110_1001 0x69
#define B0110_1010 0x6A
#define B0110_1011 0x6B
#define B0110_1100 0x6C
#define B0110_1101 0x6D
#define B0110_1110 0x6E
#define B0110_1111 0x6F

#define B0111_0000 0x70
#define B0111_0001 0x71
#define B0111_0010 0x72
#define B0111_0011 0x73
#define B0111_0100 0x74
#define B0111_0101 0x75
#define B0111_0110 0x76
#define B0111_0111 0x77
#define B0111_1000 0x78
#define B0111_1001 0x79
#define B0111_1010 0x7A
#define B0111_1011 0x7B
#define B0111_1100 0x7C
#define B0111_1101 0x7D
#define B0111_1110 0x7E
#define B0111_1111 0x7F

#define B1000_0000 0x80
#define B1000_0001 0x81
#define B1000_0010 0x82
#define B1000_0011 0x83
#define B1000_0100 0x84
#define B1000_0101 0x85
#define B1000_0110 0x86
#define B1000_0111 0x87
#define B1000_1000 0x88
#define B1000_1001 0x89
#define B1000_1010 0x8A
#define B1000_1011 0x8B
#define B1000_1100 0x8C
#define B1000_1101 0x8D
#define B1000_1110 0x8E
#define B1000_1111 0x8F

#define B1001_0000 0x90
#define B1001_0001 0x91
#define B1001_0010 0x92
#define B1001_0011 0x93
#define B1001_0100 0x94
#define B1001_0101 0x95
#define B1001_0110 0x96
#define B1001_0111 0x97
#define B1001_1000 0x98
#define B1001_1001 0x99
#define B1001_1010 0x9A
#define B1001_1011 0x9B
#define B1001_1100 0x9C
#define B1001_1101 0x9D
#define B1001_1110 0x9E
#define B1001_1111 0x9F

#define B1010_0000 0xA0
#define B1010_0001 0xA1
#define B1010_0010 0xA2
#define B1010_0011 0xA3
#define B1010_0100 0xA4
#define B1010_0101 0xA5
#define B1010_0110 0xA6
#define B1010_0111 0xA7
#define B1010_1000 0xA8
#define B1010_1001 0xA9
#define B1010_1010 0xAA
#define B1010_1011 0xAB
#define B1010_1100 0xAC
#define B1010_1101 0xAD
#define B1010_1110 0xAE
#define B1010_1111 0xAF

#define B1011_0000 0xB0
#define B1011_0001 0xB1
#define B1011_0010 0xB2
#define B1011_0011 0xB3
#define B1011_0100 0xB4
#define B1011_0101 0xB5
#define B1011_0110 0xB6
#define B1011_0111 0xB7
#define B1011_1000 0xB8
#define B1011_1001 0xB9
#define B1011_1010 0xBA
#define B1011_1011 0xBB
#define B1011_1100 0xBC
#define B1011_1101 0xBD
#define B1011_1110 0xBE
#define B1011_1111 0xBF

#define B1100_0000 0xC0
#define B1100_0001 0xC1
#define B1100_0010 0xC2
#define B1100_0011 0xC3
#define B1100_0100 0xC4
#define B1100_0101 0xC5
#define B1100_0110 0xC6
#define B1100_0111 0xC7
#define B1100_1000 0xC8
#define B1100_1001 0xC9
#define B1100_1010 0xCA
#define B1100_1011 0xCB
#define B1100_1100 0xCC
#define B1100_1101 0xCD
#define B1100_1110 0xCE
#define B1100_1111 0xCF

#define B1101_0000 0xD0
#define B1101_0001 0xD1
#define B1101_0010 0xD2
#define B1101_0011 0xD3
#define B1101_0100 0xD4
#define B1101_0101 0xD5
#define B1101_0110 0xD6
#define B1101_0111 0xD7
#define B1101_1000 0xD8
#define B1101_1001 0xD9
#define B1101_1010 0xDA
#define B1101_1011 0xDB
#define B1101_1100 0xDC
#define B1101_1101 0xDD
#define B1101_1110 0xDE
#define B1101_1111 0xDF

#define B1110_0000 0xE0
#define B1110_0001 0xE1
#define B1110_0010 0xE2
#define B1110_0011 0xE3
#define B1110_0100 0xE4
#define B1110_0101 0xE5
#define B1110_0110 0xE6
#define B1110_0111 0xE7
#define B1110_1000 0xE8
#define B1110_1001 0xE9
#define B1110_1010 0xEA
#define B1110_1011 0xEB
#define B1110_1100 0xEC
#define B1110_1101 0xED
#define B1110_1110 0xEE
#define B1110_1111 0xEF

#define B1111_0000 0xF0
#define B1111_0001 0xF1
#define B1111_0010 0xF2
#define B1111_0011 0xF3
#define B1111_0100 0xF4
#define B1111_0101 0xF5
#define B1111_0110 0xF6
#define B1111_0111 0xF7
#define B1111_1000 0xF8
#define B1111_1001 0xF9
#define B1111_1010 0xFA
#define B1111_1011 0xFB
#define B1111_1100 0xFC
#define B1111_1101 0xFD
#define B1111_1110 0xFE
#define B1111_1111 0xFF

// Boot used 6K Flash ,addr [0  -  (0x1800 -1)]
 
// Enable Boot or not , is enable the soft will set the  SCB->VTOR = Flash_App_Addr
// you Shuold Set the Address a Flash start addr in  "Target ->Option " Menu 
// Becareful  the Flash  last 2K [0x1F800 - 0x20000]  is used to store the Rtc Event . the usable range of the Flash is [0- 0x1F800]
// if used the Boot  the  Flash Range is  [0x1800- 0x1F800] 
// So the ROM1 should be set as addr:  0x1800   count  :  0x1E000
#define  Enable_Boot      1
#if   Enable_Boot  ==  1
#define  Flash_App_Addr   0x1800    
#else
#define  Flash_App_Addr   0x0000    
#endif

// to go  deep sleep or normal sleep mode by Periph enable bit. 
// deep sleep mode will not support the trace.
#define SleepMode_Enable  0

// if use the assert function or debug function
#define _ASSERT   1

// define the commonn MAX Long Proc counter 
// there are two value  can be chioced : 32 or 64 .  
// if Use_Max_Proc ==1  max 64 longprocdata can be used . else max 32 longprocdata can be used .
// this will limit the max longproc counter wen can used at same time .
// you can define max 255 longproc for every Thread . but only MAX_LONGPROC_CNT longproc can be used at same time for the whole programe .
#define Use_Max_Proc    1


// if Use_Big_Evt == 1  StdEvt is  32bit used 16bit to hold sig , and the pool will store in Evt ,it quick the memory  alloc and release 
// max pool cnt is 63 , max pool item is 1K , max pool item size is 1K byte
// but also   it will use 2 times memery to hold the StdEvt . 
// else  StdEvt is  16bit used 9 bit to hold sig , it reduce the memory for StdEvt used, but will a little slow when then is many pool
// max pool cnt is 4 , totally pool item is 127 , max pool item size is 1K byte
#define Use_Big_Evt     0     


/*  对于定义Use_Big_Evt =0   则所有的内存池共用相同的POOID， 这个序号是累加的， 查找当前的序号指得是哪一个那个内存池的那个序号，通过逐一减去个内存池的总数量确定
    因此 所有内存池的总的内存数量不能超过127个 。   
typedef struct StdEvent_
{
     uint16_t   poolid :7 ; // mark the poolid used . 
     uint16_t   evtid :9;   // mark the event  used . 
}StdEvt, * LPStdEvt;       // 16bit.

   对于定义Use_Big_Evt 为1  则 所有内存池 在事件中是分开的的 每个消息包含的内存块 有自己的 池ID , 和序号Id ， 内存池总量 63个， 每个内存池最大容纳 1K个内存块
typedef struct StdEvent_
{
     uint16_t  poolindex : 6 ; // mark the index of the pool
     uint16_t  poolid :10 ;  // mark the pool id used . 
     uint16_t  evtid ;       // mark the event  used . 
}StdEvt, * LPStdEvt;       // 32bit.
   对于内存池内的内存块最大尺寸不超过  255×4 = 1Kbyte 。  且为了对齐 内存池必须是基于uint32_t型的
              内存池数量不超过63个       
*/


#if  Use_Big_Evt  == 0  
    
    #define PoolCntType    uint8_t
    #define MaxPoolItemCnt   0x7f
    #define EvtSigMask   0x1ff
    #define EvtSigBitCnt  9
    #define EvtPoolMask  0xfe00
    #define EvtPoolIndexmask  0
    #define StdEvt     uint16_t 
    #define LPStdEvt   uint16_t *
    
    #define getpoolid(evt) (PoolCntType)(((evt)&EvtPoolMask)>>EvtSigBitCnt)  // based on 1, not 0 
#else
    #define PoolCntType    uint16_t
    #define MaxPoolItemCnt   0x3ff
    #define EvtSigMask   0xffff        // use low 16bit as event range
    #define EvtSigBitCnt  16
    #define EvtPoolMask  0x03ff0000    // use middle 10bit as  pool item index , it begin from 1 , 0 means no pool  
    #define EvtPoolBitCnt 10
    #define EvtPoolIndexmask      0xfc000000    // use high 6 bit as pool index max 63 pool
    #define StdEvt     uint32_t 
    #define LPStdEvt   uint32_t *
    
    #define getpoolid(evt) (PoolCntType)(((evt)&EvtPoolMask)>>EvtSigBitCnt)  // based on 1, not 0 
    #define getpoolindex(evt) (PoolCntType)(((evt)&EvtPoolIndexmask)>>(EvtSigBitCnt + EvtPoolBitCnt))  // based on 0 
    
#endif    
    #define getevtid(evt)  (uint16_t)((evt) &EvtSigMask)
    
    #define setevtid(evt,id)   evt = (((evt)&(EvtPoolMask|EvtPoolIndexmask))|(id))
    #define makeevt(evt,id)    ((((StdEvt)id)<<EvtSigBitCnt)|(evt)) 
     
     
     
#define CheckEvt(evt,sig)  (((sig ^ evt) & EvtSigMask) ==0)


// the static proc mode , there is two kind of way for  static proc to wait some event 
// 1 : wait event is written in code .  : when the static proc running , it will jump to a jump point , check the current input event and the event in code .
//     this way need less  memory , but a little more running time  bcs the check code is running every time  .
// 2 : wait event is stored in ram  .  : when the static proc running , it check the input event with the waiting event array, only jump to  specail jump point when the event checked 
//     this way need more memory ,to store the wait event array , but it is a little quickly .
//     be careful , the static proc is running ONLY there is event registered and there is a same event is send to this block .    
// bcs the static proc is called when any event is input , it's called so frequently , the way 2 is more fitable to used .
//     but
// define StaticProcModeQuick : used way 2 .else used way  1 
#define StaticProcModeQuick       1
#if  StaticProcModeQuick == 1 

// the max wait event cnt is defined , all thd proc  has same count of wait array cnt  MAX Cnt is 4 

#define StaticProcWaitEvtCnt   2
    #if StaticProcWaitEvtCnt > 4
        #error "MAX wait evt Cnt is bigger then 4"
    #endif
#endif 



// the real message memery pool counter defined here.  
// different Pool  hold defferent size message buffer . 
// 确定有多少个内存池以及内存池的大小，一旦 Pooln_Cnt  不为0 则 该内存池被分配，并可以使用
// 最大可以定义63个内存池，缺省可以定义8个，如果超过8个需要在evtos.c 中参照 原有格式添加
// 内存池序号不可跳级 。需要连续定义

// predefin some name 
#define PoolId1        0
#define PoolId2        1
#define PoolId3        2
#define PoolId4        3
#define PoolId5        4
#define PoolId6        5
#define PoolId7        6
#define PoolId8        7
//....
// define how many pool will be used .
#define PoolCnt    1

// define the pool1  cnt  , 
#define Pool1_Cnt    40  // max Pool1_Cnt item.
// define the  pool1 item size based on uint32_t .
#define Pool1_Size   3   // max 3*4 = 12byte .

// define the pool2  cnt  ,   
#define Pool2_Cnt    0  // max Pool2_Cnt item.
// define the  pool3 item size based on uint32_t .
#define Pool2_Size   6  // max 10*4 = 40 byte .

// define the pool3  cnt  ,   
#define Pool3_Cnt    0  // max Pool3_Cnt item.
// define the  pool3 item size based on uint32_t .
#define Pool3_Size   16   // max 16*4 = 64byte .

// define the pool4  cnt  ,   
#define Pool4_Cnt    0  // max Pool3_Cnt item.
// define the  pool3 item size based on uint32_t .
#define Pool4_Size   16   // max 16*4 = 64byte .

// define the pool5  cnt  ,   
#define Pool5_Cnt    0  // max Pool3_Cnt item.
// define the  pool3 item size based on uint32_t .
#define Pool5_Size   16   // max 16*4 = 64byte .

// define the pool6  cnt  ,   
#define Pool6_Cnt    0  // max Pool3_Cnt item.
// define the  pool3 item size based on uint32_t .
#define Pool6_Size   16   // max 16*4 = 64byte .

// define the pool7  cnt  ,   
#define Pool7_Cnt    0  // max Pool3_Cnt item.
// define the  pool3 item size based on uint32_t .
#define Pool7_Size   16   // max 16*4 = 64byte .
//....

static const uint8_t PoolSize[PoolCnt] =
{
    Pool1_Size*4,
#if  Pool2_Cnt >0
    Pool2_Size *4,
    #if  Pool3_Cnt >0
        Pool3_Size *4,
        #if  Pool4_Cnt >0
            Pool4_Size *4,
            #if  Pool5_Cnt >0
                Pool5_Size *4,
                #if  Pool6_Cnt >0
                    Pool6_Size *4,
                    #if  Pool7_Cnt >0
                        Pool7_Size *4,
                        #if  Pool8_Cnt >0
                            Pool8_Size *4,
                        #endif
                    #endif
                #endif
            #endif
       #endif
    #endif      
#endif  
};
static const uint8_t PoolLen[PoolCnt] =
{
    Pool1_Cnt,
#if  Pool2_Cnt >0
    Pool2_Cnt,
    #if  Pool3_Cnt >0
        Pool3_Cnt,
        #if  Pool4_Cnt >0
            Pool4_Cnt,
            #if  Pool5_Cnt >0
                Pool5_Cnt,
                #if  Pool6_Cnt >0
                    Pool6_Cnt,
                    #if  Pool7_Cnt >0
                        Pool7_Cnt,
                        #if  Pool8_Cnt >0
                            Pool8_Cnt,
                        #endif
                    #endif
                #endif
            #endif
       #endif
    #endif      
#endif  
};


// 特殊内存块定义， 部分内存池将用于特殊用途， 这部分内存池单独定义

// define the RF memory  pool  cnt  max cnt i  , 
#define PoolRF_Cnt    120  // max Pool1_Cnt item. 
#if PoolRF_Cnt >= 127
#error Rf Pool count should not bigger than 127 ,bcs for short Evt struct max buffer cnt is 127 
#endif
// define the  pool1 item size based on uint32_t .
#define PoolRF_Size   10   // max 10*4 = 40byte .


// sig define , the max sig is 511. for  Use_Big_Evt = 0 ,else 256*256 sig 

// 每一个串口命令应该定义为一个单独的事件。否则会导致事件驱动过程不好处理。需要进一步分析这些事件。

// 1 前两个bit为 00 ：表示 当前事件来自串口 ， 将其有划分为两部分 大cmd 和小cmd 以第三bit 为分割
// 对于大cmd 表示他的处理过程是一个序列， 需要等待并执行一系列状态， 认为他需要考虑资源独占，禁止其他大cmd
// 同时执行。 其他大Cmd将被阻塞  对于小cmd 认为等同于普通任务

// 2 前两个bit为 01 10 11 ：表示 当前事件来自内部 ， 





// 此外 全零是一个特殊的事件， 任何任务都不处理这个事件， 他的作用是让系统对任务产生一次调用，完成系统的处理初始化

#define Sig_None_Value 0
typedef enum Signal_ 
{
    Sig_None = Sig_None_Value ,   // 0 the task will be call with it  to init start .  0
    // 下面是不同事件区域， 这部分事件 可以携带一个Buffer，用于携带其他参数。 通常用于Cmd 等处理。
    //--------------------------------------------------------------------    
    Sig_lpUart_Sended,      // 1 a cmd  is send out 
    Sig_Get_Time,           //2
    Sig_Set_Time,           //3
    Sig_Get_RtcEvt,  //4 read a RtcEvt ,  buf[1] index ,[2] flage ,[3,4,5,6] tickblock,[7,8,9,10] param.
                    // if the flage bit2  = 1 return the last one.    
    Sig_Set_RtcEvt,  // 5 send down a RtcEvt . store it in ram  buf[1] index ,[2] flage ,[3,4,5,6] tickblock,[7,8,9,10] param.
                    // if flage bit2  =1  : set the max cnt . it should not bigger than current event count
                    // if flage bit2 !=1  : if the index not bigger than cnt , changed it, else add one and put it in the last postion

    Sig_Store_RtcEvt, //6 write the RtcEvt all  to flash .   the system will be reset .  
    
    Sig_SetMotor1,  //7 set the motor1 state . buf[1] mode  , buf[2] ratio
    Sig_SetMotor2,  //8 set the motor2 state . buf[1] mode  , buf[2] ratio
    Sig_SetMotor3,  //9 set the motor3 state . buf[1] mode  , buf[2] ratio
    Sig_TestProc0,  //10 Get test motor1 for test 
    Sig_RF_ConRecv   ,  //11  enable the RF to continue receive. -- no used
    Sig_Rtc_Adjust , //12 addjust the rtc  ADJUST regist  for test .
    Sig_50Hz_Set,    //13 set the 50hz duty. 
    Sig_IrDetect,     //14 do  a Ir send and receive .
    Sig_GetNetMode,    //15 RF mode for test 
    Sig_Wakeup ,      // 0x10  do wakeup , do not goto sleep mode , wait the update programe .
    Sig_DelayReset ,   //0x11 reset after 2 seconds .
    Sig_GotoBoot,      //0x12 reset and goto boot mode ,wait for fresh app . 
    Sig_ShutDown,      //0x13  ,test to goto deep sleep mode    
    Sig_ReadRFReg ,    //0x14 try to read the rf register .
    Sig_Rf_DayOver,    //0x15 a day overed , send to net layer to do node live cnt detect .
    Sig_Lora_GetTime,  //0x16 test. used by other process to start GetTime  proc

    


    // 下面是RF事件区域， MAC 部分上报状态信息 ， 以及上层下发特定的收发以及休眠到MAC层
    //--------------------------------------------------------------------    
    Sig_Rf_Overtime  = 0x80,   // lora detected a  send fail . : it has not receive a ack.
    Sig_Rf_No_Msg,             // for leaf node , there is no msg send to local .
    Sig_Rf_Recv_Data,          // lora received a  block data , that  maybe a msg or a low net msg . 
    Sig_Rf_Recv_Msg,           // lora received a msg , that is send to local node 
    Sig_Rf_Register ,          // ask to start the registe proc 
    Sig_Rf_Rgst_Delay,         // delay  signal used in register proc
    Sig_Rf_Register_OK,        // register ok . the node has jion to the local net .
    Sig_Rf_GetTime,          // used to test the Motor seting     
    Sig_Rf_GetTime_OK,       // receive a rtc time     
    Sig_Rf_CheckOffline,      // ask to start the check wait msg proc.
    Sig_Rf_CheckOffline_OK,   // event received by leaf node : father node received Sig_Rf_CheckUndirect msg , stop check wait msg proc
    Sig_Rf_Discnt_Father,     // dis connect with father .
    
    


    



    
    
    // 带参数事件区域结束  ， 下面是简单事件区域 ， 下面的事件 不能携带参数 。 通常用于底层简单事件
    Sig_Reset = 0x100 ,               // do reset , to do boot or other things , this also the divivor of simple cmd or big cmd

    Sig_Rtc_Stop ,             // no rtc event , stop it .    
    // 下面是小RTC事件区域， 用于系统内的 一步完成的任务
    //--------------------------------------------------------------------    
    
    Sig_Rtc_Enable ,          // used for rtc event , it means the current cmd  enable /disable a timeitem  , 
    // it's param data8.data0 is index, data8.data1 :1 :enable,0 :disable 
                              // data8.data2 1: repeate , 0: single  
                              
    Sig_Rtc_Jump ,            // jump from current rtc event to a specail rtc event . it will jump over lots of event.
    // it's param data8.data0 is index,   data8.data1  is the fix count  
                              // data8.data2 1: repeate , 0: single    
    Sig_Rtc_FixHourInt ,        // hour interrupt created
    
    
    // 下面是内部通用事件区域, 用于单步 非独占任务
    //--------------------------------------------------------------------    

 
    Sig_Key_Down,       // PB13 is pressed.
    Sig_Key_Up,         // PB13 is released
    Sig_Magnet1_On,         // hal1 detected a magnet on it  
    Sig_Magnet1_Remove,     // hal1 detected a magnet on it  
    Sig_Magnet2_On,         // hal2 detected a magnet on it  
    Sig_Magnet2_Remove,     // hal2 detected a magnet on it  
    Sig_Adc_Done,           // Adc simple done .   data is stored at  AdcCfg.Data
    Sig_Motor1_Done,        // motor1 stoped chang it in stabel or stop mode 
    Sig_Motor2_Done,        // motor2 stoped
    Sig_Motor3_Done,        // motor3 stoped
    Sig_Motor2_Test,        // do Motor2 test proc .
    Sig_Motor1_Test,        // do Motor2 test proc .
    Sig_50Hz_Overtime,      // 50Hz period Ovetime .
    Sig_50Hz_Duty,          // 50Hz duty over .
    Sig_Ir_OVT,             // recv ir data overtime
    Sig_Ir_Recv_Right,      // recv ir data right
    Sig_Ir_Recv_Error,      // recv ir data error 
    
    Sig_Motor2_Work ,       // start motor2
    Sig_Motor2_Stop,        // stop motor2
   
    
    Sig_MotorProc0_Done,
    Sig_MotorProc1_Done,
    
    Sig_Magnet_OVTM ,       // wait magnet input overtime , reset the  MagnetSetTimeProc
    
    
    Sig_Lora_Delay,         // wait for RF delay .
    Sig_Time_shark = 0x181,    // test shark led1    
    //--------------------------------------------------------------------      
    MAX_SIG  = 0x200       // the Biggest signal  , Sig should  not bigger than this .
} Signal ;

#define IsSimpleCmd(sig)  ((sig) & Sig_Reset)
// define the max actor cnt. the os use it to define room to hold the actor. the max count is 31.
#define ActorCnt      4
#if ActorCnt > 32
#error max actor cnt is limited by 32
#endif
// define the max soft timer count , 
#define SoftTimerCnt     10 


// net define part 
#define  RootNode     1
#define  TreeNode     2
#define  LeafNode     3
#define  Nodetype     TreeNode
//#define Nodetype    LeafNode
//#define Nodetype  RootNode  

#define MaxNodeLiveCnt       4           //max 15, a counter based on day   a node lived if it not connect with other's  .it will be sub 1 every day.
#define MaxChildActivePeriod (24*(MaxNodeLiveCnt/2) -1)   //max 255 a counter based on hour   every ChildActivePeriod * FixHourCnt hours  the child is wakeup to send mac check online msg.
#if MaxNodeLiveCnt*24 <= MaxChildActivePeriod
#error active period should small than Node Live cnt,it is recommand MaxChildActivePeriod *3 +1 == MaxNodeLiveCnt*24 
#endif
// MaxChildActivePeriod *24 should small than MaxNodeLiveCnt , -> there must once active for a node , else the counter over timed.
// the Node will lose connect with father 

#endif
