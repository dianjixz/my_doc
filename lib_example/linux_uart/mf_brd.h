#ifndef __MF_BRD_H
#define __MF_BRD_H

/*****************************************************************************/
// Headers
/*****************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mf_cfg.h"
#include "mf_cfg_param.h"
#include "linux/spi/spidev.h"

/*****************************************************************************/
// Enums & Macro
/*****************************************************************************/
#define BRD_KEY_DEV				"/dev/input/event0"				/* 按键设备路径 */
#define BRD_GPIO_DEV_PATH   	"/sys/class/gpio"				/* GPIO设备路径 */
#define BRD_UARTP_DEV			"/dev/ttyS3"					/* 协议串口设备 */
#define BRD_KEY_DEV				"/dev/input/event0"				/* 按键设备路径 */
#define BRD_IIC_DEV				"/dev/iic-0"					/* IIC设备路径 */
#define BRD_WATCHDOG_DEV		"/dev/watchdog"					/* 看门狗设备路径 */
#define BRD_SPK_FAIL_FILE		"/root/assets/fail.wav"			/* 识别失败音频文件 */
#define BRD_SPK_RECORD_OK_FILE	"/root/assets/record_ok.wav"	/* 录入成功音频文件 */
#define BRD_SPK_SUCCESS_FILE	"/root/assets/success.wav"		/* 识别成功音频文件 */
typedef enum 
{
	MF_BOARD_MF7_V		= 0,	/*7寸竖屏标案*/
	MF_BOARD_MF7_H		= 1,	/*7寸横屏标案*/
	MF_BOARD_MF2_V		= 2,	/*伟创2.8寸*/

	MF_BOARD_NONE,		/* Invalid */
}mf_brd_type_t;

typedef enum{
	KEY_RELEASE=0,
	KEY_PRESS=1,
	KEY_LONGPRESS=2,
	KEY_DBL=3
} key_state_t;


typedef struct{
	int baud;
	int data_bits;
	int stop_bits;
	char parity;
}mf_brd_uart_t;

typedef struct{
	uint32_t address;
}mf_brd_iic_t;

typedef struct{
	uint32_t mode;			/* 工作模式 */
	uint32_t speed;			/* 频率 */
	uint8_t bits;			/* 发送/接收数据位 */
}mf_brd_spi_t;

typedef struct
{
	int init_flag;

	mf_err_t (*init)(void);
	void (*deinit)(void);
	void (*read_key)(int32_t key_idx, int* state);
	void (*read_tp)(int* x, int* y);		
	void (*set_IR_LED)(int state);
	void (*turn_IR_LED)(float duty);
	void (*set_W_LED)(int state);
	void (*turn_W_LED)(float duty);
	void (*set_BL_LED)(int state);
	void (*turn_BL_LED)(float duty);
	void (*set_RGB)(int state); //b0:R, b1:G, b2:B
	void (*set_relay)(int state, uint32_t ms); //设置继电器打开ms，或者关闭
	void* (*gpio_init)(char* pin, int mode, int state);
	void (*gpio_read)(char* pin, int* state);
	void (*gpio_write)(char* pin, int state);
	void (*gpio_deinit)(char* pin);
	void* (*uart_init)(char* dev, void* param);
	int (*uart_read)(void* handle, int cnt, uint8_t* buf);
	int (*uart_write)(void* handle, int cnt, uint8_t* buf);
	void (*uart_deinit)(void* handle);
	int (*spi_init)(char* dev, void* param);
	void (*spi_rw)(int spifd, int cnt, uint8_t* buf_w, uint8_t *buf_r);
	void (*spi_deinit)(int spifd);
	int (*i2c_init)(char* dev, void* param);
	void (*i2c_write)(int i2cfd, uint8_t addr, uint16_t reg_addr, int cnt, uint8_t* buf);
	void (*i2c_read)(int i2cfd, uint8_t addr, uint16_t reg_addr, int cnt, uint8_t* buf);
    void (*i2c_smbus_write)(int i2cfd, uint8_t addr, uint16_t reg_addr, int cnt, uint8_t* buf);
    void (*i2c_smbus_read)(int i2cfd, uint8_t addr, uint16_t reg_addr, int cnt, uint8_t* buf);
    void (*i2c_deinit)(int i2cfd);
	void (*watchdog_init)(int feed_time);	//初始化看门狗，以及喂狗时间，内部新建线程喂狗
	void (*watchdog_feed)(void);
	void (*wigand_init)(int mode);	//韦根信号初始化，设定模式 接收0/发送1
	void (*wigand_read)(int* bitlen, uint64_t* data);	//读取数据
	void (*wigand_write)(int bitlen, uint64_t data);	//写数据
	void (*mic_init)(int fs, int gain);	//初始化采样率,增益，仅支持S16_little格式
	void (*mic_record)(int time, char* file_path, int16_t* buf);	//录制音频到文件或者buf
	void (*spk_init)(int fs, int gain);	//初始化采样率，增益，仅支持S16_little格式
	void (*spk_play)(int time, char* file_path, int16_t* buf); //播放音频文件或者buf
} mf_brd_t;


/*****************************************************************************/
// Functions
/*****************************************************************************/


/*****************************************************************************/
// Vars
/*****************************************************************************/
extern mf_brd_t mf_brd;




#endif

