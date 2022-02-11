/*****************************************************************************/
// Headers
/*****************************************************************************/
#include "mf_brd.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "linux/watchdog.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <linux/tty_flags.h>
#include <linux/serial.h>
#include <linux/spi/spidev.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#pragma GCC diagnostic ignored "-Wunused-function"
/*****************************************************************************/
// Macro definitions
/*****************************************************************************/
#define DBG_LINE()                              \
    do                                             \
    {                                              \
        printf("[%s]:LINE%d\r\n", __FILE__, __LINE__); \
    } while(0)

/* 调试用打印 */
#define _DEBUG_INFO(info)                                       \
do                                                              \
{                                                               \
    printf("\033[32m[INFO]:%s LINE:%d \033[0m\r\n", info, __LINE__); \
}while(0)                                                       

#define _DEBUG_ERROR(info)                                      \
do                                                              \
{                                                               \
    printf("\033[31m[ERROR]:%s LINE:%d \033[0m\r\n", info, __LINE__);\
}while(0)
/*****************************************************************************/
// Function definitions
/*****************************************************************************/


/*****************************************************************************/
// Private Var 局部变量
/*****************************************************************************/
static int s_key_fd;	
static uint64_t relay_start_ms = 0;
static uint64_t relay_keep_ms = 0;
static uint8_t relay_thread_flag = 0;

typedef struct
{
    cfg_bl_param_t bl;
    cfg_led_param_t irled;
    cfg_led_param_t wled;
    cfg_relay_param_t relay;
}brd_t;

static brd_t s_brd = {0};
/*****************************************************************************/
// Driver 底层驱动
/*****************************************************************************/

/*****************************************************************************/
// Private Func 局部函数
/*****************************************************************************/
static int _write_file_only(char* path, void* buf, size_t count)
{
    int fd, res;

    fd = open(path, O_WRONLY | O_NONBLOCK);
    if (fd < 0)
        return -1;

    res = write(fd, buf, count);
    if (res < 0)
    {
        close(fd);
        return -2;
    }
        
    res = close(fd);
    if (res < 0)
        return -3;

    return res;
}

static int _read_file_only(char* path, void* buf, size_t count)
{
    int fd, res;

    fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
        return -1;

    res = read(fd, buf, count);
    if (res < 0)
    {
        close(fd);
        return -2;
    }

    res = close(fd);
    if (res < 0)
        return -3;

    return res;
}


/**
 * @brief 读取key状态
 * @details 如果key_idx超出范围则会返回最大idx的按键的状态
 * @param [in] key_idx   按键id(0 按键0,1 按键1)
 * @param [out] state    按键状态值(0 松开，1 按下，2 长按0.5s以上)
 * @retval  
 */
static void _read_key(int32_t key_idx, int* state)
{
    struct input_event event;
    int res;
    static uint8_t key_value[2] = {0};
	
	key_idx = key_idx < sizeof(key_value) ? key_idx : sizeof(key_value) - 1;

    res = read(s_key_fd, &event, sizeof(event));
    if (res == sizeof(event))
    {
        /* 保存value到按键表 */
        if (event.type == EV_KEY)
        {
            switch(event.code)
            {
            case 114:
                key_value[1] = event.value;
                break;
            case 115:
                key_value[0] = event.value;
                break;
            default:break;
            }
        }   
    }

    *state = key_value[key_idx];
}

static int _get_pwm_num(char *pin)
{
    int pwm_num = 0;
    if (strlen(pin) < 3)    return -1;

    if (!strcmp(pin, "PD1") || !strcmp(pin, "PH0"))
    {
        pwm_num = 0;
    }
    else if (!strcmp(pin, "PD2") || !strcmp(pin, "PH1"))
    {
        pwm_num = 1;
    }
    else if (!strcmp(pin, "PD3") || !strcmp(pin, "PH2"))
    {
        pwm_num = 2;
    }
    else if (!strcmp(pin, "PD4") || !strcmp(pin, "PH3"))
    {
        pwm_num = 3;
    }
    else if (!strcmp(pin, "PD5") || !strcmp(pin, "PH4"))
    {
        pwm_num = 4;
    }
    else if (!strcmp(pin, "PD6") || !strcmp(pin, "PH5"))
    {
        pwm_num = 5;
    }
    else if (!strcmp(pin, "PD7") || !strcmp(pin, "PH6"))
    {
        pwm_num = 6;
    }
    else if (!strcmp(pin, "PD8") || !strcmp(pin, "PH7"))
    {
        pwm_num = 7;
    }
    else if (!strcmp(pin, "PD9") || !strcmp(pin, "PH8"))
    {
        pwm_num = 8;
    }
    else if (!strcmp(pin, "PD19") || !strcmp(pin, "PD22") || !strcmp(pin, "PH9"))
    {
        pwm_num = 9;
    }
    else
    {
        pwm_num = -1;
    }
    
    return pwm_num;
}

/**
 * @brief 初始化pwm
 * @details 
 * @param [in] pwm_id   按键id(0 按键0,1 按键1)
 * @param [in] fre      频率
 * @param [in] duty     占空比
 * @retval
 */
static void _pwm_init(char *pin, uint64_t fre, float duty)
{
    int res = 0;
    char path[100];
    char arg[20];
    int pwm_id;
    uint64_t period = 0, duty_cycle = 0;

    pwm_id = _get_pwm_num(pin);
    if (-1 == pwm_id)   return;

    snprintf(arg, sizeof(arg), "%d", pwm_id);
    res = _write_file_only("/sys/class/pwm/pwmchip0/export", arg, strlen(arg));
    if (res < 0)    {return;}

    period = 1000000000 / fre;
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/period", pwm_id);
    snprintf(arg, sizeof(arg), "%lld", period);
    res = _write_file_only(path, arg, strlen(arg));
    if (res < 0)    {return;}

    duty = duty > 1.0 ? 1.0 : duty;
    duty = 1.0 - duty;
    duty_cycle = period * duty;
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", pwm_id);
    snprintf(arg, sizeof(arg), "%lld", duty_cycle);
    res = _write_file_only(path, arg, strlen(arg));
    if (res < 0)    {return;}

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/enable", pwm_id);
    res = _write_file_only(path, "1", 1);
    if (res < 0)    {return;}
}

static void _pwm_deinit(char *pin)
{
    int res = 0;
    char path[100];
    char arg[20];
    int pwm_id;

    pwm_id = _get_pwm_num(pin);
    if (-1 == pwm_id)   return;

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/enable", pwm_id);
    res = _write_file_only(path, "0", 1);
    if (res < 0)    {return;}

    snprintf(arg, sizeof(arg), "%d", pwm_id);
    res = _write_file_only("/sys/class/pwm/pwmchip0/unexport", arg, strlen(arg));
    if (res < 0)    {return;}
}

static void _pwm_set_duty(char * pin, float duty)
{
    int res = 0;
    char path[100];
    char arg[20];
    int pwm_id;
    uint64_t period = 0, duty_cycle = 0;

    pwm_id = _get_pwm_num(pin);
    if (-1 == pwm_id)   return;

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/period", pwm_id);
    res = _read_file_only(path, arg, sizeof(arg));
    if (res < 0)    {return;}
    period = atoi(arg);

    duty = 1.0 - duty;
    duty_cycle = period * duty;
    snprintf(arg, sizeof(arg), "%lld", duty_cycle);
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", pwm_id);
    res = _write_file_only(path, arg, strlen(arg));
    if (res < 0)    {return;}   
}

/**
 * @brief 将字符引脚编号转换为数字引脚编号
 * @param [in] pin   字符引脚编号,格式必须为PAxx~PHxx，且xx的范围为0~31
 * @retval 返回数字引脚编号，如果为-1，则执行失败
 */
static int _get_gpio_num(char* pin)
{
	if (strlen(pin) < 3)
		return -1;
	char c;
	int group_id, group_offset, gpio_num;

	c = pin[0];
	if (c != 'p' && c != 'P')
	{
		return -1;
	}	
		
	c = pin[1];
	if ('a' <= c && c <= 'z')			// 限制a~z
	{
		group_id = c - 'a';
	}
	else if ('A' <= c && c <= 'Z')	    // 限制A~Z
	{
		group_id = c - 'A';
	}
	else
	{
		return -1;
	}

	group_offset = atoi(pin + 2);
	if (group_offset > 31)
	{
		return -1;
	}
	
	gpio_num = (group_id << 5) + group_offset;
	return gpio_num;
}

/**
 * @brief 向export文件注册gpio
 * @param [in] gpio   引脚编号
 * @retval 返回0，成功，小于0，失败
 */
static int gpio_export(uint32_t gpio)  
{  
    int len, res;  
    char buf[10];  

    len = snprintf(buf, sizeof(buf), "%d", gpio);
	if (len > sizeof(buf))
		return -1;

    res = _write_file_only(BRD_GPIO_DEV_PATH "/export", buf, len);
    if (res < 0)
	{
		perror("gpio_export");
	}
        
    return res;  
}

/**
 * @brief 向unexport文件取消注册gpio
 * @param [in] gpio   引脚编号
 * @retval 返回0，成功，小于0，失败
 */
static int gpio_unexport(uint32_t gpio)  
{  
    int len, res;  
    char buf[10];  

    len = snprintf(buf, sizeof(buf), "%d", gpio);
	if (len > sizeof(buf))
		return -1;

    res = _write_file_only(BRD_GPIO_DEV_PATH "/unexport", buf, len);
    if (res < 0)
	{
		perror("gpio unexport");
	}
        
    return res;  
} 

/**
 * @brief 设置gpio输入/输出模式
 * @param [in] gpio   引脚编号
 * @param [in] mode   引脚模式,取值如下:
 *  				0,输入模式
 *  				1,输出模式，默认低电平
 *  				2,输出模式，默认低电平
 *  				3,输出模式，默认高电平 			
 * @retval 返回0，成功，小于0，失败
 */
static int gpio_set_dir(uint32_t gpio, uint32_t mode)  
{  
    int len, res;  
    char path[40];  
   
    len = snprintf(path, sizeof(path), BRD_GPIO_DEV_PATH  "/gpio%d/direction", gpio);  
	if (len > sizeof(path))
		return -1;

    mode = mode > 3 ? 3 : mode;
    switch(mode)
    {
        case 0:res = _write_file_only(path, "in", sizeof("in"));break;
        case 1:res = _write_file_only(path, "out", sizeof("out"));break;
        case 2:res = _write_file_only(path, "low", sizeof("low"));break;
        case 3:res = _write_file_only(path, "high", sizeof("high"));break; 
        default:break;
    } 

    return res;  
}

/**
 * @brief 设置gpio电平
 * @param [in] gpio   引脚编号
 * @param [in] value  引脚电平(1,高电平;0,低电平)		
 * @retval 返回0，成功，小于0，失败
 */
static int gpio_set_value(uint32_t gpio, uint32_t value)  
{  
    int len, res;  
    char path[40]; 
   
    len = snprintf(path, sizeof(path), BRD_GPIO_DEV_PATH  "/gpio%d/value", gpio);  
	if (len > sizeof(path))
		return -1;

	if (value)
		res = _write_file_only(path, "1", 2);
	else
		res = _write_file_only(path, "0", 2);

    return res;  
}

/**
 * @brief 读取gpio电平
 * @param [in] 	gpio   引脚编号
 * @param [out] value  引脚电平(1,高电平;0,低电平)		
 * @retval 返回0，成功，小于0，失败
 */
static int gpio_get_value(uint32_t gpio, uint32_t *value)  
{  
    int len, res;  
    char path[40], state; 
   
    len = snprintf(path, sizeof(path), BRD_GPIO_DEV_PATH  "/gpio%d/value", gpio);  
	if (len > sizeof(path))
		return -1;

	res = _read_file_only(path, &state, 1);
	if (res < 0)
		return res;

	*value = state == '0' ? 0 : 1;

    return res;  
}

/**
 * @brief 初始化gpio
 * @note 还没有限制io_num的范围，需要注意
 * @param [in] dev      设备名
 * @param [in] mode     模式
 * @param [in] state    初始状态值
 * @retval 0 成功 <0 失败
 */
static void* _gpio_init(char* pin, int mode, int state)
{
    int res = -1;

	int io_num = _get_gpio_num(pin);
	if (io_num < 0)
	{
		return (void *)res;		
	}

    /* 向export文件注册一个gpio */
    res = gpio_export(io_num);
	if (res < 0)    return (void *)res;

	/* 设置gpio方向 */
	res = gpio_set_dir(io_num, mode);
	if (res < 0)    return (void *)res;

	if (mode > 0)
	{
		/* 设置gpio电平 */
		res = gpio_set_value(io_num, state);
		if (res < 0)    return (void *)res;

	}


    return (void*)res;
}

/**
 * @brief 初始化gpio
 * @note 还没有限制io_num的范围，需要注意
 * @param [in] handle      设备名
 * @retval 
 */
static void _gpio_deinit(char* pin)
{
    int res;

	int io_num = _get_gpio_num(pin);
	if (io_num < 0) return;
	res = gpio_unexport(io_num);
	if (res < 0) return;
}

/**
 * @brief 读gpio电平，只能在输入模式下调用
 * @note 
 * @param [in]  handle  句柄，用来传入文件描述符
 * @param [out] state 状态值
 * @retval 
 */
static void _gpio_read(char* pin, int* state)
{
	int io_num = _get_gpio_num(pin);
	if (io_num < 0) return;

	gpio_get_value(io_num, (uint32_t *)state);

}

/**
 * @brief 写gpio电平，只能在输出模式下调用
 * @note 
 * @param [in] handle  句柄，用来传入文件描述符
 * @param [in] state   状态值
 * @retval 
 */
static void _gpio_write(char* pin, int state)
{
	int io_num = _get_gpio_num(pin);
	if (io_num < 0) return;		

	gpio_set_value(io_num, state);
}


/**
 * @brief 读取触摸屏触点
 * @note 
 * @param [out] x   坐标x
 * @param [out] y   坐标y
 * @retval 
 */
static void _read_tp(int* x, int* y)
{

}

static void _turn_bl_led(float duty)
{
    int value = 0;
    duty = duty > 1.0 ? 1.0 : duty;
    duty = s_brd.bl.pol ? 1.0 - duty : duty;
    switch (s_brd.bl.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            value = duty ? 1 : 0;
            _gpio_write(s_brd.bl.pin, value);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_set_duty(s_brd.bl.pin, duty);
            break;
        default:break;
    }
}

/**
 * @brief 设置lcd背光灯状态
 * @param [in] state   状态值(1 打开，0 关闭)
 * @retval 
 */
static void _set_bl_led(int state)
{
    int value;

    value = state ? 1 : 0;
    value ^= s_brd.bl.pol;
    switch (s_brd.bl.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_write(s_brd.bl.pin, value);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_set_duty(s_brd.bl.pin, value);
            break;
        default:break;
    }
}


static void _turn_irled(float duty)
{
    int value = 0;
    duty = duty > 1.0 ? 1.0 : duty;
    duty = s_brd.irled.pol ? 1.0 - duty : duty;
    switch (s_brd.irled.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            value = duty ? 1 : 0;
            _gpio_write(s_brd.irled.pin, value);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_set_duty(s_brd.irled.pin, duty);
            break;
        default:break;
    }
}

/**
 * @brief 设置红外灯状态
 * @param [in] state   状态值(1 打开，0 关闭)
 * @retval 
 */
static void _set_irled(int state)
{
    int value;

    value = state ? 1 : 0;
    value ^= s_brd.irled.pol;
    switch (s_brd.irled.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_write(s_brd.irled.pin, value);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_set_duty(s_brd.irled.pin, value);
            break;
        default:break;
    }
}

static void _turn_wled(float duty)
{
    int value = 0;
    duty = duty > 1.0 ? 1.0 : duty;
    duty = s_brd.wled.pol ? 1.0 - duty : duty;
    switch (s_brd.wled.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            value = duty ? 1 : 0;
            _gpio_write(s_brd.wled.pin, value);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_set_duty(s_brd.wled.pin, duty);
            break;
        default:break;
    }
}

/**
 * @brief 设置白灯状态
 * @param [in] state   状态值(1 打开，0 关闭)
 * @retval 
 */
static void _set_wled(int state)
{
    int value;

    value = state ? 1 : 0;
    value ^= s_brd.wled.pol;
    switch (s_brd.wled.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_write(s_brd.wled.pin, value);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_set_duty(s_brd.wled.pin, value);
            break;
        default:break;
    }
}

static void *relay_thread(void *args)
{
    struct timeval time;
    int value = 1;

    value ^= s_brd.relay.pol;
    _gpio_write(s_brd.relay.pin, value);

    while (1)
    {
        gettimeofday(&time, NULL);
        uint64_t curr_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
        
        if (curr_ms - relay_start_ms > relay_keep_ms)
        {
            break;
        }

        usleep(1000 * 100);
    }

    _gpio_write(s_brd.relay.pin, !value);
    relay_thread_flag = 0;

    return NULL;
}

/**
 * @brief 设置继电器状态
 * @param [in] state   状态值(1 打开，0 关闭)
 * @param [in] ms      打开时间
 * @retval 
 */
static void _set_relay(int state, uint32_t ms)
{
    int value;
    struct timeval time;
    static pthread_t tid;

    gettimeofday(&time, NULL);
    
    if (relay_thread_flag)
    {
        if (state)
        {
            relay_start_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
            relay_keep_ms = ms;
        }
        else
        {
            relay_keep_ms = 0;
        }
    }
    else
    {
        if (state)
        {
            relay_start_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
            relay_keep_ms = ms;

            if (0 == pthread_create(&tid, NULL, relay_thread, NULL))
                relay_thread_flag = 1;
            else
                perror("relay thread create error");
        }
        else
        {
            value = state ? 1 : 0;
            value ^= s_brd.relay.pol;
            _gpio_write(s_brd.relay.pin, value);
        }
    }
}

static int _get_baud(int baud)
{
    switch (baud) 
    {
    case 9600:return B9600;
    case 19200:return B19200;
    case 38400:return B38400;
    case 57600:return B57600;
    case 115200:return B115200;
    case 230400:return B230400;
    case 460800:return B460800;
    case 500000:return B500000;
    case 576000:return B576000;
    case 921600:return B921600;
#ifdef B1000000
    case 1000000:return B1000000;
#endif
#ifdef B1152000
    case 1152000:return B1152000;
#endif
#ifdef B1500000
    case 1500000:return B1500000;
#endif
#ifdef B2000000
    case 2000000:return B2000000;
#endif
#ifdef B2500000
    case 2500000:return B2500000;
#endif
#ifdef B3000000
    case 3000000:return B3000000;
#endif
#ifdef B3500000
    case 3500000:return B3500000;
#endif
#ifdef B4000000
    case 4000000:return B4000000;
#endif
    default:return -1;
    }
}


static void clear_custom_speed_flag(int _fd)
{
    struct serial_struct ss;
    if (ioctl(_fd, TIOCGSERIAL, &ss) < 0) {
        // return silently as some devices do not support TIOCGSERIAL
        return;
    }

    if ((ss.flags & ASYNC_SPD_MASK) != ASYNC_SPD_CUST)
        return;

    ss.flags &= ~ASYNC_SPD_MASK;

    if (ioctl(_fd, TIOCSSERIAL, &ss) < 0) {
        perror("TIOCSSERIAL failed");
        exit(1);
    }
}

/**
 * @brief 初始化uart
 * @note 
 * @param [in] dev    设备名
 * @param [in] param  参数
 * @retval 
 */
static void* _uart_init(char* dev, void* param)
{
    int fd;

    mf_brd_uart_t* cfg = (mf_brd_uart_t *)param;

    int baud = _get_baud(cfg->baud);
    int data_bits = cfg->data_bits, stop_bits = cfg->stop_bits;
    char parity = cfg->parity;

    fd = open(dev, O_RDWR | O_NONBLOCK | O_NOCTTY);
    if (fd < 0)
    {
        return (void *)fd;
    }

    struct termios opt;
    memset(&opt, 0, sizeof(opt));

    /* 忽略modem，使能读模式 */
    opt.c_cflag |= CLOCAL | CREAD;

    /* 设置波特率 */
    opt.c_cflag |= baud;

    /* 设置数据位 */
    switch (data_bits)
    {
    case 7:
        opt.c_cflag |= CS7;
        break;
    case 8:
        opt.c_cflag |= CS8;
        break;
    default:break;
    }

    /* 设置奇偶校验位 */
    switch (parity)
    {
    case 'N':
    case 'n':
        opt.c_iflag &= ~INPCK;     
        opt.c_cflag &= ~PARENB;     
        break;
    case 'O':
    case 'o':
        opt.c_iflag |= (INPCK | ISTRIP); 
        opt.c_cflag |= (PARODD | PARENB);
        break;
    case 'E':
    case 'e':
        opt.c_iflag |= (INPCK | ISTRIP); 
        opt.c_cflag |= PARENB;
        opt.c_cflag &= ~PARODD;
        break;
    default:_DEBUG_ERROR("set parity failed.");break;
    }

    /* 设置停止位 */
    switch (stop_bits)
    {
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |= CSTOPB;
        break;
    default:_DEBUG_ERROR("set speed failed.");break;
    }

    /* 设置流控制 */
    opt.c_cflag &= ~CRTSCTS;

    /* 最小字节数与等待时间 */
    opt.c_cc[VMIN] = 1;
    opt.c_cc[VTIME] = 0;

    /* 刷新串口，更新配置 */
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &opt);
   
    clear_custom_speed_flag(fd);

    return (void *)fd;
}

/**
 * @brief 取消初始化uart
 * @note 
 * @param [in] dev    设备名
 * @param [in] param  参数
 * @retval 
 */
static void _uart_deinit(void* handle)
{
    int res;
    int *fd = (int *)handle;

    res = close(*fd);
    if (res < 0)
        fprintf(stderr, "uart close fd(%d) err:%s\n", *fd, strerror(errno));
    else
        *fd = -1;
}

/**
 * @brief uart读数据
 * @note 
 * @param [in] handle       ??
 * @param [in] cnt          长度
 * @param [in] buf          参数
 * @retval 
 */
static int _uart_read(void* handle, int cnt, uint8_t* buf)
{
    int *fd = (int *)handle;

    return read(*fd, buf, cnt);
}

/**
 * @brief uart写数据
 * @note 
 * @param [in] handle       ??
 * @param [in] cnt          长度
 * @param [in] buf          参数
 * @retval 
 */
static int _uart_write(void* handle, int cnt, uint8_t* buf)
{
    int *fd = (int *)handle;

    return write(*fd, buf, cnt);
}

/**
 * @brief 初始化spi
 * @note 
 * @param [in] dev            spi设备名
 * @param [in] param          参数
 * @retval 
 */
static int _spi_init(char* dev, void* param)
{
    int fd, res = 0;
    mf_brd_spi_t *cfg = (mf_brd_spi_t *)param;
    int mode = cfg->mode, bits = cfg->bits, speed = cfg->speed;
    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        return -1;
    }
        

	/*
	 * spi mode
	 */
	res = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
	if (res == -1)
		printf("can't set spi mode");

	res = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
	if (res == -1)
		printf("can't get spi mode");
    
	/*
	 * bits per word
	 */
	res = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (res == -1)
		printf("can't set bits per word");

	res = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (res == -1)
		printf("can't get bits per word");

	/*
	 * max speed hz
	 */
	res = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (res == -1)
		printf("can't set max speed hz");

	res = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (res == -1)
		printf("can't get max speed hz");

	printf("spi mode: 0x%x\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

    return fd;
}

/**
 * @brief 取消初始化spi
 * @note 
 * @param [in] spifd            设备描述符
 * @retval 
 */
static void _spi_deinit(int spifd)
{
    int fd = spifd;

    close(fd);
}

/**
 * @brief spi读写
 * @note 
 * @param [in] spifd         设备描述符
 * @param [in] cnt            长度
 * @param [in] buf_w          写缓存
 * @param [in] buf_r          读缓存
 * @retval 
 */
static void _spi_rw(int spifd, int cnt, uint8_t* buf_w, uint8_t* buf_r)
{
    int fd = spifd;
    int res = -1;
    struct spi_ioc_transfer trans = 
    {
        .tx_buf = (unsigned long)buf_w,     // 接收缓存区
        .rx_buf = (unsigned long)buf_r,     // 发送缓存区
        .len = cnt,
        .delay_usecs = 0                    // 发送时间间隔
    };

    res = ioctl(fd, SPI_IOC_MESSAGE(1), &trans);
	if (res < 1)
		printf("Can't send spi message");
}

/**
 * @brief iic初始化
 * @note 
 * @param [in] dev            设备名
 * @param [in] param          长度
 * @retval 
 */
static int _i2c_init(char* dev, void* param)
{
    int fd, res;
    (void)param;

    fd = open(dev, O_RDWR);
    if (fd < 0)
        return -1;

    res = ioctl(fd, I2C_TENBIT, 0);
    if (res < 0)
        return -1;

    return fd;
}

/**
 * @brief iic取消初始化
 * @note 
 * @param [in] i2cfd         i2c设备文件描述符
 * @retval 
 */
static void _i2c_deinit(int fd)
{
    close(fd);
}

/**
 * @brief iic写数据
 * @note 
 * @param [in] i2cfd         i2c设备文件描述符
 * @param [in] addr          从机地址
 * @param [in] reg_addr      寄存器地址
 * @param [in] cnt           长度
 * @param [in] buff          写缓存
 * @retval 
 */
static void _i2c_write(int i2cfd, uint8_t addr, uint16_t reg_addr, int cnt, uint8_t* buf)
{
    int fd = i2cfd;

    ioctl(fd, I2C_SLAVE, addr);

    uint8_t data[cnt + 1];
    data[0] = reg_addr;
    memcpy(data + 1, buf, cnt);
    write(fd, data, cnt + 1);
}

/**
 * @brief iic读数据
 * @note 
 * @param [in] i2cfd         i2c设备文件描述符
 * @param [in] addr          从机地址
 * @param [in] reg_addr      寄存器地址
 * @param [in] cnt           长度
 * @param [in] buff          读缓存
 * @retval 
 */
static void _i2c_read(int i2cfd, uint8_t addr, uint16_t reg_addr, int cnt, uint8_t* buf)
{
    int fd = i2cfd;

    ioctl(fd, I2C_SLAVE, addr);

    write(fd, &reg_addr, 1);
    read(fd, buf, cnt);
}

/**
 * @brief iic smbus写数据,当前只支持读写1字节和4字节(TODO 扩展读写多字节)
 * @note 
 * @param [in] i2cfd         i2c设备文件描述符
 * @param [in] addr          从机地址
 * @param [in] reg_addr      寄存器地址
 * @param [in] cnt           长度
 * @param [in] buff          写缓存
 * @retval 
 */
static void _i2c_smbus_write(int i2cfd, uint8_t addr, uint16_t reg_addr, int cnt, uint8_t* buf)
{
    int fd = i2cfd;
    union i2c_smbus_data data;
    struct i2c_smbus_ioctl_data ioctl_data = {0};

    if (cnt == 1)
    {
        data.byte = buf[0];
        ioctl_data.size = I2C_SMBUS_BYTE_DATA;
    }
    else if (cnt == 4)
    {
        memcpy(&data.word, buf, cnt);
        ioctl_data.size = I2C_SMBUS_WORD_DATA;
    }
    else
    {
        return;
    }

    ioctl(fd, I2C_SLAVE, addr);
    ioctl_data.read_write = I2C_SMBUS_WRITE;
    ioctl_data.command = reg_addr;
    ioctl_data.data = &data;

    ioctl(fd, I2C_SMBUS, &ioctl_data);
}

/**
 * @brief iic smbus读数据,当前只支持读写1字节和4字节(TODO 扩展读写多字节)
 * @note 
 * @param [in] i2cfd         i2c设备文件描述符
 * @param [in] addr          从机地址
 * @param [in] reg_addr      寄存器地址
 * @param [in] cnt           长度
 * @param [in] buff          读缓存
 * @retval 
 */
static void _i2c_smbus_read(int i2cfd, uint8_t addr, uint16_t reg_addr, int cnt, uint8_t* buf)
{
    int fd = i2cfd;
    union i2c_smbus_data data;
    struct i2c_smbus_ioctl_data ioctl_data = {0};

    if (cnt == 1)
    {
        ioctl_data.size = I2C_SMBUS_BYTE_DATA;
    }
    else if (cnt == 4)
    {
        ioctl_data.size = I2C_SMBUS_WORD_DATA;
    }
    else
    {
        return;
    }

    ioctl(fd, I2C_SLAVE, addr);

    ioctl_data.read_write = I2C_SMBUS_READ;
    ioctl_data.command = reg_addr;
    ioctl_data.data = &data;
    ioctl(fd, I2C_SMBUS, &ioctl_data);

    memcpy(buf, &data.word, cnt);
}

/**
 * @brief 看门狗初始化
 * @details 
 * 目前看门狗超时时间只能设置为1、2、3、4、5、6、8、10、12、14、16秒
 * @param [in] feed_time          喂狗时间，单位s
 * @retval 
*/
static void _watchdog_init(int feed_time)
{
    int res;
    int fd = -1;

    fd = open(BRD_WATCHDOG_DEV, O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "open %s error:%s\n", BRD_WATCHDOG_DEV, strerror(errno));
        return;
    }

    res = ioctl(fd, WDIOC_SETTIMEOUT, &feed_time);
    if (res < 0)
    {
        fprintf(stderr, "watchdog set timeout error\n");
        close(fd);
        return;
    }

    res = ioctl(fd, WDIOC_SETOPTIONS, WDIOS_ENABLECARD);
    if (res < 0)
    {
        fprintf(stderr, "watchdog enable error\n");
        close(fd);
        return;
    }    

    res = close(fd);
    if (res < 0)
    {
        fprintf(stderr, "close %s error:%s\n", BRD_WATCHDOG_DEV, strerror(errno));
        return;   
    }
}

static void _watchdog_feed(void)
{
    int res;
    int fd = -1;

    fd = open(BRD_WATCHDOG_DEV, O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "open %s error:%s\n", BRD_WATCHDOG_DEV, strerror(errno));
        return;
    }

    res = ioctl(fd, WDIOC_KEEPALIVE, 0);
    if (res < 0)
    {
        fprintf(stderr, "watchdog feed error\n");
        close(fd);
        return;
    }

    res = close(fd);
    if (res < 0)
    {
        fprintf(stderr, "close %s error:%s\n", BRD_WATCHDOG_DEV, strerror(errno));
        return;   
    }
}

static void wigand_init(int mode)
{

}

static void wigand_read(int* bitlen, uint64_t* data)
{

}

static void wigand_write(int bitlen, uint64_t data)
{

}

static void mic_init(int fs, int gain)
{

}

static void mic_record(int time, char* file_path, int16_t* buf)
{

}

static void spk_init(int fs, int gain)
{

}

static void _spk_play(int time, char* file_path, int16_t* buf)
{
	char cmd[50];
	int len = 0;

	len = snprintf(cmd, sizeof(cmd), "aplay %s &", file_path);
	if (len > sizeof(cmd))
    {
        return;
    }

	FILE *fstream = NULL;
	fstream = popen(cmd, "r");
	if (NULL == fstream) return;

	pclose(fstream);
}

/**
 * @brief 初始化brd模块
 * @retval MF_ERR_NONE  成功
 */
static mf_err_t _brd_init(void)
{
    mf_err_t err = MF_ERR_NONE;

    if (!mf_cfg.init_flag)  return MF_ERR_MISS_DEPENDENT;
    if (!mf_cfg_param.is_load)  return MF_ERR_MISS_DEPENDENT;

    /* 获取cfg参数 */
    memcpy(&s_brd.bl, &mf_cfg_param.hw.bl, sizeof(cfg_bl_param_t));
    memcpy(&s_brd.irled, &mf_cfg_param.hw.irled, sizeof(cfg_led_param_t));
    memcpy(&s_brd.wled, &mf_cfg_param.hw.wled, sizeof(cfg_led_param_t));
    memcpy(&s_brd.relay, &mf_cfg_param.hw.relay, sizeof(cfg_relay_param_t));

	/* 打开key文件，单独打开文件是为了在非阻塞下能读取event文件 */
	s_key_fd = open(BRD_KEY_DEV, O_RDONLY | O_NONBLOCK);
	if (s_key_fd < 0)
	{
		return MF_ERR_TODO;
	}

    /* 初始化补光灯 */
    float duty = 0.0;
    switch (s_brd.wled.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_init(s_brd.wled.pin, 1, s_brd.wled.pol);
            break;
        case CFG_GPIO_FUNC_PWM:
            duty = (float)s_brd.wled.duty / 100;
            duty = s_brd.wled.pol ? 1.0 - duty : duty;
            _pwm_init(s_brd.wled.pin, 10000, duty);
            break;
        default:
            err = MF_ERR_TODO;
            break;
    }

    /* 初始化红外灯 */
    switch (s_brd.irled.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_init(s_brd.irled.pin, 1, s_brd.irled.pol);
            break;
        case CFG_GPIO_FUNC_PWM:
            duty = (float)s_brd.irled.duty / 100;
            duty = s_brd.irled.pol ? 1.0 - duty : duty;
            _pwm_init(s_brd.irled.pin, 10000, duty);
            break;
        default:
            err = MF_ERR_TODO;
            break;
    }

    /* 初始化lcd背光灯 */
    switch (s_brd.bl.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_init(s_brd.bl.pin, 1, s_brd.bl.pol);
            break;
        case CFG_GPIO_FUNC_PWM:
            duty = (float)s_brd.bl.duty_on / 100;
            duty = s_brd.bl.pol ? 1.0 - duty : duty;
            _pwm_init(s_brd.bl.pin, 10000, 1.0 - (float)s_brd.bl.duty_on / 100);
            break;
        default:
            err = MF_ERR_TODO;
            break;
    }
    _turn_bl_led((float)s_brd.bl.duty_standby / 100); 

    /* 初始化继电器 */
    mf_brd.gpio_init(s_brd.relay.pin, 1, s_brd.relay.pol);

    /* 初始化完成 */
	mf_brd.init_flag = 1;
    return err;
}

/**
 * @brief 初始化brd模块
 * @retval MF_ERR_NONE  成功
 */
static void _brd_deinit(void)
{
	int res = 0;

	/* 关闭key文件 */
	res = close(s_key_fd);
	if (res < 0)    
        fprintf(stderr, "close key fd(%d):%s\n", s_key_fd, strerror(errno));
    else
        s_key_fd = -1;

    /* 取消初始化补光灯 */
    switch (s_brd.wled.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_deinit(s_brd.wled.pin);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_deinit(s_brd.wled.pin);
            break;
        default:break;
    }

    /* 取消初始化红外灯 */
    switch (s_brd.irled.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_deinit(s_brd.irled.pin);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_deinit(s_brd.irled.pin);
            break;
        default:break;
    }

    /* 取消初始化lcd背光灯 */
    switch (s_brd.bl.func)
    {
        case CFG_GPIO_FUNC_SWITCH:
            _gpio_deinit(s_brd.bl.pin);
            break;
        case CFG_GPIO_FUNC_PWM:
            _pwm_deinit(s_brd.bl.pin);
            break;
        default:break;
    }

    /* 取消继电器初始化 */
    mf_brd.gpio_deinit(s_brd.relay.pin);

    /* 取消初始化完成 */
	mf_brd.init_flag = 0;
}

/*****************************************************************************/
// Public Var 全局变量
/*****************************************************************************/
mf_brd_t mf_brd=
{
    .init				= 	_brd_init,
    .deinit             =   _brd_deinit,
    .read_key			= 	_read_key,
    //.read_tp=
    .set_IR_LED        	= 	_set_irled,
    .turn_IR_LED        =   _turn_irled,
    .set_W_LED        	= 	_set_wled,
    .turn_W_LED         =   _turn_wled,  
    .set_BL_LED         =   _set_bl_led,
    .turn_BL_LED        =   _turn_bl_led,
    //.set_RGB=
    .set_relay          =   _set_relay,
    .gpio_init			= 	_gpio_init,
    .gpio_read			=	_gpio_read,
    .gpio_write			=	_gpio_write,
	.gpio_deinit		=	_gpio_deinit,
    .uart_init			=	_uart_init,
    .uart_read			=	_uart_read,
    .uart_write			=	_uart_write,
    .uart_deinit        =   _uart_deinit,
    .spi_init           =   _spi_init,
    .spi_rw             =   _spi_rw,
    .spi_deinit         =   _spi_deinit,
    .i2c_init           =   _i2c_init,
    .i2c_write          =   _i2c_write,
    .i2c_read           =   _i2c_read,
    .i2c_smbus_write    =   _i2c_smbus_write,
    .i2c_smbus_read     =   _i2c_smbus_read,
    .i2c_deinit         =   _i2c_deinit,
    .watchdog_init      =   _watchdog_init,
    .watchdog_feed      =   _watchdog_feed,
    //.wigand_init=
    //.wigand_read=
    //.wigand_write=
    //.mic_init=
    //.mic_record=
    //.spk_init=
    .spk_play			= _spk_play
};

/*****************************************************************************/
// Public Func 全局函数
/*****************************************************************************/

    
