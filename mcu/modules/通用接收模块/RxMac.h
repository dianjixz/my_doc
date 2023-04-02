/*
*******************************************************************************************
*
*
*                                  Universal Receive State Machine
*                                          通用接收状态机
*
* File : RxMac.h
* By   : Lin Shijun(https://blog.csdn.net/lin_strong)
* Date: 2019/03/07
* version: 2.1
* History: 2018/05/29 1.0 the prototype
*          2019/01/23 2.0 modify the type names to more readable ones, though preserve
*                       the old-style for forward compatibility;
*                         change init method to create method which use malloc to alloc
*                       instance, and the corresponding destroy method.
*                         change the internal buffer module from RINGQUEUE to BufferArray,
*                       so user no longer need to know the internal flags management and
*                       allocate the space for it.
*                         add RxMac_FeedDatas method for convenient.
*          2019/03/07 2.1 some modification to the malloc configuration
*
* NOTE(s):  1. the receive process has two basic state
*              A. preRx: when haven't found any header, the RxMac will search for the unique
*                        flag, header and strong-ender. Only when a header is found will come
*                        to next step.
*              B. Rxing: the RxMac will put the successive bytes into the buffer, and search
*                        for the strong-unique, strong-header, ender. 
*           2. the module is drived by the RxMac_FeedData(), user should get the the char 
*              from data stream and pass the data one by one to the RxMac through RxMac_FeedData()
*              or RxMac_FeedDatas().
*           3. each time RxMac find a frame(complete or incomplete),it will call the onFlushed
*              to notify the results; user can judge the frame through the state parameter; 
*               state.headerFound == 1:   find any header, the header is passed by HorU
*               state.enderFound  == 1:   find any ender,  the ender  is passed by Ender
*               state.isFull      == 1:   the buffer is full, maybe you should check headerFound
*                                         to see whether a header has been found.
*               state.uniqueFound == 1:   find any unique flag. In this case, other parameters will
*                                         always be 0 ,the data in the buffer will be the flag,
*                                         and the unique flag is passed by HorU.
*           4. To use this module, for each receive machine:
*              A. define malloc to configure the module, see CONFIGURATION
*              B. allocate the space for buffer and FLAGS.
*                   RXFLAG_STRUCT flags[2];
*                   uint8_t buf[300];
*              C. set the flags according to the protocol, define the callback functions
*                  according to your need.
*                   static void onGetHeader(RxMac sender,RxFlag pFlag){ ...... };
*                   static void onFlushed(RxMac sender,RxMacPtr pBuf,uint16_t len,
*                      RxState state,RxFlag HorU,RxFlag Ender){ ...... };
*                   const uint8_t HeaderFlag[] = "Header";
*                   const uint8_t EnderFlag[] = "\r\n";
*                   RxFlag_Init(flags,HeaderFlag,StrSize(HeaderFlag),RXFLAG_OPTION_HEADER);
*                   RxFlag_Init(&flags[1],EnderFlag,StrSize(EnderFlag),RXFLAG_OPTION_ENDER |
*                                 RXFLAG_OPTION_NOTFILL_ENDER);
*              D. create the receive machine:
*                   RxMac mac;
*                   mac = RxMac_Create(flags,sizeof(flags)/sizeof(flags[0]),buf,300,NULL, 
*                                 onGetHeader, onFlushed );
*              E. feed the receive machine:
*                   while(1){
*                     c = GetNextChar();
*                     RxMac_FeedData(mac,c);
*                   }
*              F. destroy the receive machine if need.
*                  RxMac_Destroy(mac);
*******************************************************************************************
*/

#ifndef RX_MAC_H
#define RX_MAC_H

/*
*******************************************************************************************
*                                       INCLUDES
*******************************************************************************************
*/

#include <stdint.h>

/*
*******************************************************************************************
*                                  CONFIGURATION  配置
*******************************************************************************************
*/
// #define RXMAC_ARGUMENT_CHECK_DISABLE to disable the argument check functions of this module
//#define RXMAC_ARGUMENT_CHECK_DISABLE

// #define RXMAC_NOTFILL_DISABLE to disable the notFill option
//#define RXMAC_NOTFILL_DISABLE

// #define RXMAC_ONFEEDED_DISABLE to disable the onFeeded event.
//#define RXMAC_ONFEEDED_DISABLE

// #define RXMAC_SINGLETON_EN to use singleton pattern,so argument pRxMac of interfaces is 
// useless, and user don't need to allocate space for RX_MAC, but you still need to allocate
// buffer and call init();
//#define RXMAC_SINGLETON_EN

// #define RXMAC_BUF_RPAGE if you want receive machine use the paged array as buffer.
// and don't forget to define CODEWARRIOR malloc
//#define RXMAC_BUF_RPAGE
/*
*******************************************************************************************
*                                 ADDRESSING MODE 寻址模式
*******************************************************************************************
*/

#ifdef RXMAC_BUF_RPAGE
#ifdef CODEWARRIOR
#define RXMAC_BUF_ADDRESSING_MODE  __rptr
#endif
#endif

#ifndef RXMAC_BUF_ADDRESSING_MODE
#define RXMAC_BUF_ADDRESSING_MODE
#endif
typedef uint8_t * RXMAC_BUF_ADDRESSING_MODE RxMacPtr;
 
/*
*********************************************************************************************
*                                    ERROR CODE
*********************************************************************************************
*/

#define RXMAC_ERR_NONE           0
#define RXMAC_ERR_ARGUMENT       1
#define RXMAC_ERR_POINTERNULL    2
#define RXMAC_ERR_UNKNOWN        3
#define RXMAC_ERR_INIT           4

/*
*********************************************************************************************
*                                RECEIVE FLAG STRUCT
*********************************************************************************************
*/

// normal header, RxMac will only check it in Step A
#define RXFLAG_OPTION_HEADER               0x01
// strong header, RxMac will always check it.
#define RXFLAG_OPTION_STRONG_HEADER        0x03
// the header will not be filled into buffer when found.(only valid when is header)
#define RXFLAG_OPTION_NOTFILL_HEADER       0x04
// normal ender, RxMac will only check it in Step B
#define RXFLAG_OPTION_ENDER                0x08
// strong header, RxMac will always check it.
#define RXFLAG_OPTION_STRONG_ENDER         0x18
// the ender will not be filled into buffer when found.(only valid when is ender)
#define RXFLAG_OPTION_NOTFILL_ENDER        0x20
// normal unique, RxMac will only check it in Step A
#define RXFLAG_OPTION_UNIQUE               0x40
// strong unique, RxMac will always check it.
#define RXFLAG_OPTION_STRONG_UNIQUE        0xC0

// receive flag
typedef struct RXFLAG_STRUCT{
   uint8_t const * pBuf;
   uint8_t len;
   uint8_t option;
} RXFLAG_STRUCT;
typedef RXFLAG_STRUCT const * RxFlag;

// void RxFlag_Init(RXFLAG_STRUCT * flag,uint8_t const * buf,uint8_t len,uint8_t opt);
// to  initialize a receive flag
//    flag    point to the target RXFLAG_STRUCT.
//    buf     pointer to the flag buffer
//    size    size of flag 
//    opt     see  RXFLAG_OPTION_XXXXX, bit mode
#define RxFlag_Init(flag,buf,size,opt)     \
{(flag)->pBuf =(buf);(flag)->len =(size);(flag)->option = (opt);}


/*
*********************************************************************************************
*                                   TYPE DEFINITION
*********************************************************************************************
*/

typedef struct RXSTATE_STRUCT{
  unsigned int headerFound: 1;     // 1: have get header
  unsigned int enderFound : 1;     // 1: have get ender
  unsigned int isFull     : 1;     // 1: the buffer is full
  unsigned int uniqueFound: 1;     // 1: this is unique flag.
} RxState;

typedef struct RXMAC_STRUCT * RxMac;

typedef void (* RXMAC_FLUSH_EVENT)(RxMac sender,RxMacPtr buf,uint16_t len,RxState state,
                                   RxFlag HorU,RxFlag Ender);
typedef void (* RXMAC_FLAG_EVENT)(RxMac sender,RxFlag flag);
typedef void (* RXMAC_FILTER)(RxMac sender,uint8_t * pCurChar,uint16_t bytesCnt);

/*
*******************************************************************************************
*                                  FUNCTION DECLARATION
*******************************************************************************************
*/

// to create an instance of RxMac
//  flags       标志字符串结构体的数组
//  flagsCnt    标志字符串结构体的个数
//  buf         用户提供给接收机用的缓冲区 
//  bufLen      缓冲区的大小(起码应该要能放的下最长的标志字符串)
//  onFeeded    在每次被Feed时触发
//  onGetHeader 获得头标志位时触发。
//  onFlushed   收到一帧数据时的回调函数
RxMac RxMac_Create(RXFLAG_STRUCT const flags[], uint8_t flagsCnt, RxMacPtr buf, uint16_t bufLen, RXMAC_FILTER onFeeded, RXMAC_FLAG_EVENT onGetHeader, RXMAC_FLUSH_EVENT onFlushed);
// to destroy the RxMac
void RxMac_Destroy(RxMac mac);
// 向接收机内喂字节
void RxMac_FeedDatas(RxMac mac, uint8_t const * buf, uint16_t len);
void RxMac_FeedData(RxMac mac, uint8_t c);
// 重置接收区长度为最长那个长度
//uint8_t RxMac_ResetRxSize(RxMac mac);
// 设置最大接收到多少个字节
uint8_t RxMac_SetRxSize(RxMac mac, uint16_t size);
// 重置接收机的状态
uint8_t RxMac_ResetState(RxMac mac);
// 强制接收机flush
uint8_t RxMac_Flush(RxMac mac);
// 设置onFeeded
uint8_t RxMac_SetOnFeeded(RxMac mac, RXMAC_FILTER onFeeded);
// 设置onGetHeader
uint8_t RxMac_SetOnGetHeader(RxMac mac, RXMAC_FLAG_EVENT onGetHeader);
// 设置onFlushed
uint8_t RxMac_SetOnFlushed(RxMac mac, RXMAC_FLUSH_EVENT onFlushed);

#include "RxMacPrivate.h"

#endif // of RX_MAC_H