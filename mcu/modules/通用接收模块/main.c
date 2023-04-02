#include <stdio.h>
#include "RxMac.h"

/*
*********************************************************************************************************
*                                      LOCAL  FUNCTION  DECLARE
*********************************************************************************************************
*/

static void onGetData(RxMac sender,uint8_t * pCurChar,uint16_t bytesCnt);
static void onFlushed(RxMac sender,RxMacPtr buf,uint16_t len,RxState state,RxFlag HorU,RxFlag Ender);
static void onGetHeader(RxMac sender,RxFlag flag);
static void onGetHeader2(RxMac sender,RxFlag flag);

/*
*********************************************************************************************************
*                                           LOVAL VARIABLE
*********************************************************************************************************
*/
static RxMac  mac = NULL;
static RXFLAG_STRUCT flags[4];
#define BUF_SIZE  20
static uint8_t buffer[BUF_SIZE];

// 协议示例1：
// 帧头    :HEADER 或者 START
// 强帧尾  :END
// 强特殊串:12345  
// 
static void protocol1_init(void){
  RX_FLAG_INIT(&flags[0],"HEADER",6,FLAG_OPTION_HEADER);
  RX_FLAG_INIT(&flags[1],"START",5,FLAG_OPTION_HEADER);
  RX_FLAG_INIT(&flags[2],"END",3,FLAG_OPTION_STRONG_ENDER);
  RX_FLAG_INIT(&flags[3],"12345",5,FLAG_OPTION_STRONG_UNIQUE);
  mac = RxMac_Create(flags,4,buffer,BUF_SIZE,NULL,onGetHeader,onFlushed);
}
// 协议示例2：
// 帧头    : START
// 帧头后的第1个字符表示后面还要接收多少个字符 1-9,'4'表示4个，如果不是数字或为'0'，则等待帧尾
// 帧尾    : END
// 特殊串:  NOW
// 
static void protocol2_init(void){
  RX_FLAG_INIT(&flags[0],"START",5,FLAG_OPTION_HEADER);
  RX_FLAG_INIT(&flags[1],"END",3,FLAG_OPTION_ENDER);
  RX_FLAG_INIT(&flags[2],"NOW",3,FLAG_OPTION_UNIQUE);
  mac = RxMac_Create(flags,3,buffer,BUF_SIZE,NULL,onGetHeader2,onFlushed);
}
/*
*********************************************************************************************************
*                                           CALLBACK FUNCITON
*********************************************************************************************************
*/

static void onGetData(RxMac sender,uint8_t * pCurChar,uint16_t bytesCnt){
  // 因为发现帧头后才挂载事件，所以下一次回掉正好是说明字符数的那个字符,否则还得根据bytesCnt来判断当前位置
  RxMac_SetOnFeeded(sender,NULL);
  if(*pCurChar > '0' && *pCurChar <= '9' ){
    // bytesCnt是当前收到了多少个字符，所以接收区大小为当前字符数加上还要接收的
    RxMac_SetRxSize(sender,*pCurChar - '0' + bytesCnt);
  }
}
static void onFlushed(RxMac sender,RxMacPtr buf,uint16_t len,RxState state,RxFlag HorU,RxFlag Ender){
  buf[len] = '\0';
  printf("\nFlushed:");
  if(state.headerFound)
    printf("headerFound,");
  if(state.enderFound)
    printf("enderFound,");
  if(state.isFull)
    printf("full,");
  if(state.uniqueFound)
    printf("unique,");
  printf("\nDatas:%s\n",buf);
  RxMac_SetRxSize(sender,BUF_SIZE);
}
static void onGetHeader(RxMac sender,RxFlag flag){
  printf("\nFoundHeader:%s\n",flag->pBuf);
}
static void onGetHeader2(RxMac sender,RxFlag flag){
  printf("\nFoundHeader:%s\n",flag->pBuf);
  RxMac_SetOnFeeded(sender,onGetData);
}
/*
*********************************************************************************************************
*                                           MAIN FUNCTION
*********************************************************************************************************
*/

void main(void) {
  // 选择想要实验的协议来初始化
  protocol1_init();
  // protocol2_init();
  while (1) {
    c = getchar();
    // 回显
    putchar(c);
    RxMac_FeedData(mac,c);
  }
}