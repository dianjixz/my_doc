/*
*******************************************************************************************
*
*
*                       Private Declarations for Universal Receive State Machine
*
* File : RxMacPrivate.h
* By   : Lin Shijun(https://blog.csdn.net/lin_strong)
* Date: 2019/03/07
* version: 2.1
* History: 
* NOTE(s): 
*******************************************************************************************
*/


/*
*******************************************************************************************
*                                  FUNCTION DECLARATION
*******************************************************************************************
*/
// 打印内部缓冲区，返回缓冲区的长度
uint16_t _RxMac_printBuffer(RxMac mac,uint8_t * buf);

/*
*******************************************************************************************
*                                      RECEIVE FLAG STRUCT
*******************************************************************************************
*/

// struct of RXFLAG_STRUCT.option 
/*typedef struct RXFLAG_OPTION{
  unsigned int isHeader : 1;  // 1: the flag is the head of the frame
  unsigned int strong_H : 1;  // 1: strong-header, RxMac will 
  unsigned int notfill_H: 1;  // 0: fill the flag into the buffer when found as header
  unsigned int isEnder  : 1;  // 1: the flag is the end of the frame
  unsigned int strong_E : 1;  // 
  unsigned int notfill_E: 1;  // 0: fill the flag into the buffer when found as ender
  unsigned int isUnique : 1;  // 1: the flag is a unique flag which is treated as single frame.
  unsigned int strong_U : 1;  // 0: when receiving a frame, RxMac will not 
}; //*/
// normal header, RxMac will only check it in Step A
#define RXFLAG_OPTBIT_HEADER               0x01
// strong header, RxMac will always check it.
#define RXFLAG_OPTBIT_STRONG_HEADER        0x02
// the header will not be filled into buffer when found.(only valid when is header)
#define RXFLAG_OPTBIT_NOTFILL_HEADER       0x04
// normal ender, RxMac will only check it in Step B
#define RXFLAG_OPTBIT_ENDER                0x08
// strong header, RxMac will always check it.
#define RXFLAG_OPTBIT_STRONG_ENDER         0x10
// the ender will not be filled into buffer when found.(only valid when is ender)
#define RXFLAG_OPTBIT_NOTFILL_ENDER        0x20
// normal unique, RxMac will only check it in Step A
#define RXFLAG_OPTBIT_UNIQUE               0x40
// strong unique, RxMac will always check it.
#define RXFLAG_OPTBIT_STRONG_UNIQUE        0x80

#define STATEMASK_STEPA   \
  (RXFLAG_OPTBIT_HEADER | RXFLAG_OPTBIT_UNIQUE | RXFLAG_OPTBIT_STRONG_ENDER)
#define STATEMASK_STEPB   \
  (RXFLAG_OPTBIT_STRONG_UNIQUE | RXFLAG_OPTBIT_ENDER | RXFLAG_OPTBIT_STRONG_HEADER)
#define RXFLAGMASK_USUHSH   \
  (RXFLAG_OPTBIT_HEADER | RXFLAG_OPTBIT_STRONG_HEADER | \
  RXFLAG_OPTBIT_UNIQUE | RXFLAG_OPTBIT_STRONG_UNIQUE)

// BOOL _RxFlag_isHeader(RxFlag flag);
#define _RxFlag_isHeader(flag)   \
  ((flag)->option & (RXFLAG_OPTION_STRONG_HEADER | RXFLAG_OPTION_HEADER))
// BOOL _RxFlag_isEnder(RxFlag flag);
#define _RxFlag_isEnder(flag)    \
  ((flag)->option & (RXFLAG_OPTION_STRONG_ENDER  | RXFLAG_OPTION_ENDER))
// BOOL _RxFlag_isUnique(RxFlag flag);
#define _RxFlag_isUnique(flag)   \
  ((flag)->option & (RXFLAG_OPTION_STRONG_UNIQUE | RXFLAG_OPTION_UNIQUE))
// BOOL _RxFlag_dontFillHeader(RxFlag flag);
#define _RxFlag_dontFillHeader(flag) \
  ((flag)->option & RXFLAG_OPTBIT_NOTFILL_HEADER)
// BOOL _RxFlag_dontFillEnder(RxFlag flag);
#define _RxFlag_dontFillEnder(flag) \
  ((flag)->option & RXFLAG_OPTBIT_NOTFILL_ENDER)

/*
*******************************************************************************************
*                                    FORWARD COMPATIBILITY
*******************************************************************************************
*/
// 以下仅为前向兼容
typedef RxMacPtr          pRB_BYTE;
typedef RXFLAG_STRUCT     RX_FLAG,*pRX_FLAG;
typedef RxMac             pRX_MAC;
typedef RxState           RX_STATE;
#define FLAG_OPTION_HEADER             RXFLAG_OPTION_HEADER
#define FLAG_OPTION_STRONG_HEADER      RXFLAG_OPTION_STRONG_HEADER
#define FLAG_OPTION_NOTFILL_HEADER     RXFLAG_OPTION_NOTFILL_HEADER
#define FLAG_OPTION_ENDER              RXFLAG_OPTION_ENDER
#define FLAG_OPTION_STRONG_ENDER       RXFLAG_OPTION_STRONG_ENDER
#define FLAG_OPTION_NOTFILL_ENDER      RXFLAG_OPTION_NOTFILL_ENDER
#define FLAG_OPTION_UNIQUE             RXFLAG_OPTION_UNIQUE
#define FLAG_OPTION_STRONG_UNIQUE      RXFLAG_OPTION_STRONG_UNIQUE
#define RX_FLAG_INIT                   RxFlag_Init