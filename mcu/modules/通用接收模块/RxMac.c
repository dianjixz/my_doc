/*
*******************************************************************************************
*
*
*                         Implementation of the Universal Receive State Machine
*                                          通用接收状态机
*
* File : RxMac.c
* By   : Lin Shijun(https://blog.csdn.net/lin_strong)
* Date: 2019/03/07
* version: 2.1
* History: 2018/05/29 1.0 the prototype
*          2019/01/23 2.0 In addition to the content in RxMac.h:
*                           abstract the flag management part as RxFlagMgr and the
*                          corresponding methods.
*                           refactor the code.
*          2019/03/07 2.1 some modification to the malloc configuration
* NOTE(s): 
*
*******************************************************************************************
*/

/*
*******************************************************************************************
*                                       INCLUDES
*******************************************************************************************
*/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "RxMac.h"
#include "BufferMallocArray.h"

/*
*******************************************************************************************
*                                   RECEIVE FLAGS MANAGER
*******************************************************************************************
*/

typedef struct RXFLAGMGR_STRUCT {
  // buffer to hold the pre-data which hasn't matched any flag.
  BufferUINT8Indexed BufForFlag;
  // the flag array to be matched.
  RxFlag   Flags;
  // count of flags.
  uint8_t  FlagsCnt;
  // current state, in which headerFound will influence the match behavior. 
  // controlled by the child class.
  RxState  state;
}RXFLAGMGR_STRUCT;

static RxFlag _RxFlagMgr_GetNextMatchedAtThisState(RxMac mac,uint8_t nextByte);
static BOOL _RxFlagMgr_Init(RxMac mac,RxFlag flags,uint8_t flagsCnt,uint8_t maxLen);
static void _RxFlagMgr_Destroy(RxMac mac);
static void _RxFlagMgr_Reset(RxMac mac);

/*
*******************************************************************************************
*                                   STRUCT DIFINITION
*******************************************************************************************
*/

typedef struct RXMAC_STRUCT{
  // manage the flag matches.
  RXFLAGMGR_STRUCT FlagMgr;
  // record the Header or Unique flag.
  RxFlag   pHorU;
  // internal buffer to hold data.
  RxMacPtr pRxBuf;
  // length of the internal buffer
  uint16_t RxBufSize;
  // Count of the bytes in the internal buffer/ the index for next feeded byte
  uint16_t RxCnt;
  RXMAC_FILTER onFeeded;
  RXMAC_FLAG_EVENT onGetHeader;
  RXMAC_FLUSH_EVENT onFlushed;
} RXMAC_STRUCT;

/*
*******************************************************************************************
*                          LOCAL  FUNCITON  DECLARATION
*******************************************************************************************
*/
#ifndef RXMAC_SINGLETON_EN
  #define _pMac          mac
  #define _BufForFlag   (_pMac->FlagMgr.BufForFlag)
  #define _Flags        (_pMac->FlagMgr.Flags)
  #define _FlagsCnt     (_pMac->FlagMgr.FlagsCnt)
  #define _state        (_pMac->FlagMgr.state)
  #define _pHorU        (_pMac->pHorU)
  #define _pRxBuf       (_pMac->pRxBuf)
  #define _RxBufSize    (_pMac->RxBufSize)
  #define _RxCnt        (_pMac->RxCnt)
  #define _fonFeeded    (_pMac->onFeeded)
  #define _fonGetHeader (_pMac->onGetHeader)
  #define _fonFlushed   (_pMac->onFlushed)
  #define _RxMac_Destroy()    (free(mac))
#else
  static RXMAC_STRUCT _mac;
  // 单例模式中，这个指针用于标识是否单例已初始化过
  static RxMac _pMac = NULL;
  #define _BufForFlag   (_mac.FlagMgr.BufForFlag)
  #define _Flags        (_mac.FlagMgr.Flags)
  #define _FlagsCnt     (_mac.FlagMgr.FlagsCnt)
  #define _state        (_mac.FlagMgr.state)
  #define _pHorU        (_mac.pHorU)
  #define _pRxBuf       (_mac.pRxBuf)
  #define _RxBufSize    (_mac.RxBufSize)
  #define _RxCnt        (_mac.RxCnt)
  #define _fonFeeded    (_mac.onFeeded)
  #define _fonGetHeader (_mac.onGetHeader)
  #define _fonFlushed   (_mac.onFlushed)
  #define _RxMac_Destroy()  (_pMac = NULL)
#endif

#define _stateByte      (*(uint8_t *)(&_state))
#define _isRxBufFull()  (_RxCnt >= _RxBufSize)

#ifndef RXMAC_ONFEEDED_DISABLE
  #define _onFeeded(pChar,cnt)    if(_fonFeeded != NULL)  _fonFeeded(_pMac,pChar,cnt);
#else
  #define _onFeeded(pChar,cnt)
#endif
#define  _onGetHeader(headerFlag)       if(_fonGetHeader != NULL) _fonGetHeader(_pMac,headerFlag);

#undef _DONT_CHECK_MAC
#ifdef RXMAC_ARGUMENT_CHECK_DISABLE
#define _DONT_CHECK_MAC
#endif
#ifdef RXMAC_SINGLETON_EN
#define _DONT_CHECK_MAC
#endif

#ifdef _DONT_CHECK_MAC
  #define _checkMacNotNull()
  #define _checkMacNotNull_void()
#else
  #define _checkMacNotNull()       if(_pMac == NULL)  return RXMAC_ERR_POINTERNULL;
  #define _checkMacNotNull_void()  if(_pMac == NULL)  return;
#endif


#ifdef RXMAC_BUF_RPAGE
#ifdef CODEWARRIOR
static RxMacPtr _memcpy_internal(RxMacPtr dest, RxMacPtr src, size_t n);
#define memcpy(dest,src,n)   _memcpy_internal(dest,src,n)
#endif
#endif
// 冲刷缓冲区
static void _flush(RxMac mac,RxFlag ender);
// 往接收机缓冲区内放数据
static void _BufIn(RxMac mac,RxMacPtr buf,uint16_t len);

static void _RxMac_FlushIfFull(RxMac mac);
static void _RxMac_RecordAndFlushPreBytesIfGotHeaderOrUnique(RxMac mac,RxFlag flagJustGot);
static void _RxMac_GetHeaderProcess(RxMac mac,RxFlag flagJustGot);
static void _RxMac_GetUniqueProcess(RxMac mac,RxFlag flagJustGot);
static void _RxMac_GetEnderProcess(RxMac mac,RxFlag flagJustGot);
/*
*******************************************************************************************
*                                      RxMac_Create()
*
* Description : To create a receive machine instance.    创建一个接收机实例
*
* Arguments   : flags      pointer to the flags(an array); 指向标志串(数组)的指针
*               flagsCnt   the count of the flags;         有多少个标志串;
*               buf        pointer to the buffer provided to the RxMac;提供给接收机使用的缓存
*               bufLen     the size of the buffer.         缓存的大小
*               onFeeded   the callback func that will be called everytime feeded, you
*                          can modify the feeded byte in this callback.
*                          每次被feed时会调用的回调函数，如改变对应数据值会影响标志位的判断
*               onGetHeader the callback func that will be called when find a header.
*                          当发现帧头时会调用的回调函数
*               onFlushed  the callback func that will be called when flushed.
*                          当Flush时会调用的回调函数
*
* Return      : Pointer to the created instance.
*               NULL      if any error.
*
* Note(s)     : 1. size of buffer should bigger than the longest flag, or the flag will be 
*               useless.
*               2. if flagsCnt > 0, flags can't point to NULL.
*               3. you must provide a buffer.
*               4. if you enable the RXMAC_SINGLETON_EN, multi-create will pointer to the
*                  same instance initialized as the last create.
*
*               void onGetHeader(RxMac sender,RxFlag flag):
*                sender   the pointer to the RxMac which call this function
*                flag     the header matched
*
*               void onFeeded(RxMac sender,uint8_t * pCurChar,uint16_t bytesCnt):
*                sender    the pointer to the RxMac which call this function
*                pCurChar  point to the byte just received, you can change it before any other process.
*                bytesCnt  the number of bytes in the buffer including the char just feeded.
*
*               void onFlushed(RxMac sender,RxMacPtr pBuf,uint16_t len,RxState state,RxFlag HorU,
*                  RxFlag Ender);
*                sender    the pointer to the RxMac which call this function
*                buf       the pointer to the frame.
*                len       the length of frame.
*                state     the state of frame.
*                HorU      point to the header flag if state.headerFound == 1, or unique flag if 
*                          state.uniqueFound == 1.
*                Ender     point to the ender flag if state.enderFound == 1.
*******************************************************************************************
*/
RxMac RxMac_Create(RXFLAG_STRUCT const flags[],uint8_t flagsCnt,RxMacPtr buf,uint16_t bufLen,RXMAC_FILTER onFeeded,RXMAC_FLAG_EVENT onGetHeader,RXMAC_FLUSH_EVENT onFlushed){
  uint8_t i, maxLen = 0;
#ifndef RXMAC_SINGLETON_EN
  RxMac mac;
#endif
#ifndef RXMAC_ARGUMENT_CHECK_DISABLE
  if((flags == NULL && flagsCnt > 0 ) || buf == NULL)
    return NULL;
#endif
  // find out the max length of flags.
  for(i = 0; i < flagsCnt; i++){
    if(flags[i].len > maxLen)
      maxLen = flags[i].len;
  }
#ifndef RXMAC_ARGUMENT_CHECK_DISABLE
  if(bufLen < maxLen){
    return NULL;
  }
#endif
#ifdef RXMAC_SINGLETON_EN
  if(_pMac != NULL)            // if have created one instance, free the previous FlagBuf
    _RxFlagMgr_Destroy(&_mac);
  else
    _pMac = &_mac;
#else
  if((mac = (RxMac)malloc(sizeof(RXMAC_STRUCT))) == NULL){
    return NULL;
  }
#endif
  if(!_RxFlagMgr_Init(_pMac,flags,flagsCnt,maxLen)){
    _RxMac_Destroy();
    return NULL;
  }
  _pHorU = NULL;
  _pRxBuf = buf;
  _RxCnt = 0;
  _RxBufSize = bufLen;
  _fonFeeded = onFeeded;
  _fonGetHeader = onGetHeader;
  _fonFlushed = onFlushed;
  return _pMac;
}
/*
*******************************************************************************************
*                                        RxMac_Destroy()
*
* Description : Destroy a receive machine instance.
*
* Arguments   : mac     the target receive machine.     目标接收机
*
* Return      : 
*
* Note(s)     : 
*
*******************************************************************************************
*/
void RxMac_Destroy(RxMac mac){
  if(_pMac == NULL)
    return;
  _RxFlagMgr_Destroy(mac);
  _RxMac_Destroy();
}
/*
*******************************************************************************************
*                                        RxMac_FeedData(s)
*
* Description : To feed RxMac the next char(s).    用于给接收机下一个字符
*
* Arguments   : mac    the target receive machine.     目标接收机
*               c      the char to feed;               下一个字符
*
* Return      : 
*
* Note(s)     : 
*******************************************************************************************
*/

void RxMac_FeedDatas(RxMac mac, uint8_t const * buf, uint16_t len){
  uint16_t i;
  for(i = 0; i < len; i++)
    RxMac_FeedData(mac,buf[i]);
}

void RxMac_FeedData(RxMac mac,uint8_t c){
  RxFlag curFlag;
  _checkMacNotNull_void();
  _onFeeded(&c,_RxCnt + 1);
  _pRxBuf[_RxCnt++] = c;
  curFlag = _RxFlagMgr_GetNextMatchedAtThisState(mac,c);
  if(curFlag == NULL){        // if no flag match
    _RxMac_FlushIfFull(mac);
    return;
  }
  _RxMac_RecordAndFlushPreBytesIfGotHeaderOrUnique(mac,curFlag);
  if(_RxFlag_isHeader(curFlag)){
    _RxMac_GetHeaderProcess(mac,curFlag);
  }else if(_RxFlag_isUnique(curFlag)){
    _RxMac_GetUniqueProcess(mac,curFlag);
  }else{   // if(_RxFlag_isEnder(curFlag))
    _RxMac_GetEnderProcess(mac,curFlag);
  }
}
/*
*******************************************************************************************
*                                        RxMac_SetRxSize()
*
* Description : set the size of RxBuf.   设置接收缓冲区的大小
*
* Arguments   : mac     the target receive machine.     目标接收机
*               size    the size to set;
*
* Return      : RXMAC_ERR_NONE            if Success;
*               RXMAC_ERR_POINTERNULL     if mac == NULL
*               RXMAC_ERR_ARGUMENT        if size is wrong.
* Note(s)     : the size should bigger than the current number of chars in the RxBuf.
*               
*******************************************************************************************
*/
uint8_t RxMac_SetRxSize(RxMac mac, uint16_t size){
  _checkMacNotNull();
#ifndef RXMAC_ARGUMENT_CHECK_DISABLE
  if(size <= _RxCnt)
    return RXMAC_ERR_ARGUMENT;
#endif
  _RxBufSize = size;
  return RXMAC_ERR_NONE;
}
/*
*******************************************************************************************
*                                        RxMac_ResetState()
*
* Description : reset the state of receive machine.   重置接收机的状态
*
* Arguments   : mac    the target receive machine.    目标接收机
*
* Return      : RXMAC_ERR_NONE            if Success;
*               RXMAC_ERR_POINTERNULL     if mac == NULL
* Note(s)     : it will not trigger call-back of onFlush.
*******************************************************************************************
*/
uint8_t RxMac_ResetState(RxMac mac){
  _checkMacNotNull();
  // 复位接收机
  Buffer_Cleanup((Buffer)_BufForFlag);
  _RxCnt = 0;
  _stateByte = 0;
  _pHorU = NULL;
  return RXMAC_ERR_NONE;
}
/*
*******************************************************************************************
*                                        RxMac_Flush()
*
* Description : force receive machine to flush.
*
* Arguments   : mac     the target receive machine.     目标接收机
*
* Return      : RXMAC_ERR_NONE            if Success;
*               RXMAC_ERR_POINTERNULL     if mac == NULL
*
* Note(s)     : it will force receive machine to flush, if there is any data in the RxBuffer,
*
*******************************************************************************************
*/

uint8_t RxMac_Flush(RxMac mac){
  _checkMacNotNull();
  _flush(_pMac,NULL);
  return RXMAC_ERR_NONE;
}

/*
******************************************************************************************
*                                        RxMac_SetOnFeeded()
*
* Description : set the onFeeded callback function.
*
* Arguments   : mac       the target receive machine.     目标接收机
*               onFeeded  the callback function to set;   要设置的回调函数
*
* Return      : RXMAC_ERR_NONE            if Success;
*               RXMAC_ERR_POINTERNULL     if mac == NULL
*
* Note(s)     :
*
******************************************************************************************
*/
uint8_t RxMac_SetOnFeeded(RxMac mac,RXMAC_FILTER onFeeded){
  _checkMacNotNull();
  _fonFeeded = onFeeded;
  return RXMAC_ERR_NONE;
}
/*
******************************************************************************************
*                                        RxMac_SetOnGetHeader()
*
* Description : set the onGetHeader callback function.
*
* Arguments   : mac          the target receive machine.     目标接收机
*               onGetHeader  the callback function to set;   要设置的回调函数
*
* Return      : RXMAC_ERR_NONE            if Success;
*               RXMAC_ERR_POINTERNULL     if mac == NULL
*
* Note(s)     :
*
******************************************************************************************
*/
uint8_t RxMac_SetOnGetHeader(RxMac mac,RXMAC_FLAG_EVENT onGetHeader){
  _checkMacNotNull();
  _fonGetHeader = onGetHeader;
  return RXMAC_ERR_NONE;
}
/*
******************************************************************************************
*                                        RxMac_SetOnFlushed()
*
* Description : set the onFlushed callback function.
*
* Arguments   : mac          the target receive machine.     目标接收机
*               onFlushed    the callback function to set;   要设置的回调函数
*
* Return      : RXMAC_ERR_NONE            if Success;
*               RXMAC_ERR_POINTERNULL     if pRxMac == NULL
*
* Note(s)     :
*
******************************************************************************************
*/
uint8_t RxMac_SetOnFlushed(RxMac mac,RXMAC_FLUSH_EVENT onFlushed){
  _checkMacNotNull();
  _fonFlushed = onFlushed;
  return RXMAC_ERR_NONE;
}
/*
******************************************************************************************
*                                        _RxMac_printBuffer()
*
* Description : print the internal buffer, just for developer.
*               给开发者用于查看内部缓存的
*
* Arguments   : mac          the target receive machine.     目标接收机
*               onFlushed    the callback function to set;   要设置的回调函数
*
* Return      : the len of buffer printed.
*
* Note(s)     :
*
******************************************************************************************
*/

uint16_t _RxMac_printBuffer(RxMac mac,uint8_t * buf){
  memcpy(buf,_pRxBuf,_RxCnt);
  return _RxCnt;
}

/*
******************************************************************************************
*                                   LOCAL  FUNCITON 
******************************************************************************************
*/
static RxMacPtr _memcpy_internal(RxMacPtr dest, RxMacPtr src, size_t n){
  RxMacPtr p = dest;
  while(n-- > 0)
    *p++ = *src++;
  return dest;
}

static void _BufIn(RxMac mac,RxMacPtr buf,uint16_t len){
  memcpy(_pRxBuf+_RxCnt,buf,len);
  _RxCnt += len;
}

static void _flush(RxMac mac,RxFlag ender){
  // 触发回调
  if((_RxCnt > 0 || ender != NULL) && _fonFlushed != NULL)
    _fonFlushed(_pMac,_pRxBuf,_RxCnt,_state,_pHorU,ender);
  // 复位接收机
  _RxCnt = 0;
  _stateByte = 0;
  _pHorU = NULL;
}

BOOL BufferUINT8Indexed_BackMatch(BufferUINT8Indexed buf,uint8_t const *toMatch,uint16_t len){
  uint16_t cnt = _Buffer_getCount(buf);
  if(len > cnt) 
    return FALSE;
  while(len > 0){
    if(_BufferUINT8Indexed_get(buf,--cnt) != toMatch[--len])
      return FALSE;
  }
  return TRUE;
}


static void _RxMac_FlushIfFull(RxMac mac){
  if(_isRxBufFull()){
    _state.isFull = 1;
    _flush(_pMac,NULL);
  }
}

static void _RxMac_RecordAndFlushPreBytesIfGotHeaderOrUnique(RxMac mac,RxFlag flagJustGot){
  if(flagJustGot->option & RXFLAGMASK_USUHSH){
    if(_RxCnt > flagJustGot->len){
      _RxCnt -= flagJustGot->len;
      _flush(_pMac,NULL);
    }else{
      _RxCnt = 0;
    }
    _pHorU = flagJustGot;
  }
}

static void _RxMac_GetHeaderProcess(RxMac mac,RxFlag flagJustGot){
#ifndef RXMAC_NOTFILL_DISABLE
  if(!_RxFlag_dontFillHeader(flagJustGot))
#endif
    _BufIn(_pMac,(RxMacPtr)flagJustGot->pBuf,flagJustGot->len);
  _state.headerFound = 1;
  _onGetHeader(flagJustGot);
  _RxMac_FlushIfFull(mac);
}

static void _RxMac_GetUniqueProcess(RxMac mac,RxFlag flagJustGot){
  _state.uniqueFound = 1;
  _BufIn(_pMac,(RxMacPtr)flagJustGot->pBuf,flagJustGot->len);
  _flush(_pMac,NULL);
}

static void _RxMac_GetEnderProcess(RxMac mac,RxFlag flagJustGot){
  _state.enderFound = 1;
  if(_RxCnt < flagJustGot->len){          // if part of the flag has been manually flushed.
    _RxCnt = 0;                           // restore the buffer.
    _BufIn(_pMac,(RxMacPtr)flagJustGot->pBuf,flagJustGot->len); 
  }
#ifndef RXMAC_NOTFILL_DISABLE
  if(_RxFlag_dontFillEnder(flagJustGot))
    if(_RxCnt > flagJustGot->len)
      _RxCnt -= flagJustGot->len;
    else
      _RxCnt = 0;
#endif
  _flush(_pMac,flagJustGot);
}

static RxFlag _RxFlagMgr_GetNextMatchedAtThisState(RxMac mac,uint8_t nextByte){
  uint8_t i,mask;
  if(_Buffer_isFull(_BufForFlag))
    BufferUINT8_FrontOut((BufferUINT8)_BufForFlag);
  BufferUINT8_BackIn((BufferUINT8)_BufForFlag,nextByte);
  // mask to identify possible flag
  mask = (_state.headerFound)?STATEMASK_STEPB:STATEMASK_STEPA;
  for(i = 0; i < _FlagsCnt; i++){
    if((_Flags[i].option & mask) && 
      BufferUINT8Indexed_BackMatch(_BufForFlag,_Flags[i].pBuf,_Flags[i].len)){
        Buffer_Cleanup((Buffer)_BufForFlag);
        return &_Flags[i];
    }
  }
  return NULL;
}

static BOOL _RxFlagMgr_Init(RxMac mac,RxFlag flags,uint8_t flagsCnt,uint8_t maxLen){
  if(_BufForFlag = (BufferIndexed)BufferUINT8MallocArray_Create(maxLen)){
    _Flags = flags;
    _FlagsCnt = flagsCnt;
    _stateByte = 0;
  }
  return _BufForFlag != NULL;
}

static void _RxFlagMgr_Destroy(RxMac mac){
  Buffer_Destroy(_BufForFlag);
}

static void _RxFlagMgr_Reset(RxMac mac){
  Buffer_Cleanup((Buffer)_BufForFlag);
  }