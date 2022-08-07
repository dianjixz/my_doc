https://blog.csdn.net/bjbz_cxy/article/details/119913874

目录

前言

一、I2C Drive

1. 开启方法

2. 使用方法

3. 主要API介绍

3.1 i2c_configure

3.2 i2c_transfer

3.3 i2c_recover_bus

3.4 i2c_slave_register

3.5 i2c_slave_unregister

3.6 i2c_slave_driver_register

3.7 i2c_slave_driver_unregister

3.8 i2c_write

3.9 i2c_read

4.0 i2c_write_read

4.1 i2c_burst_read

4.2  i2c_burst_write

4. I2C示例：读写与擦除”FM24V10 FLASH“

4.1 FM24V10 FLASH简介

4.2 着手开发

二、SPI Dirve

1. 开启方法

2. 使用方法

3. 结构体介绍

3.1 结构体介绍

4. 主要API介绍

4.1 spi_write

4.2 spi_read

4.3 spi_transceive

5 简单的读写示例

5.1 写

5.2 读
前言

本文涉及到的I2C与SPI通讯原理可以参考这篇文章：一文详细介绍GPIO、I2C、SPI通讯原理以及物理层原理
一、I2C Drive
1. 开启方法

在当前工程的prj.conf文件中输入以下内容：

CONFIG开头的代表包含指定Drive，格式：CONFIG_Drive名称

CONFIG_I2C=y

编译时Zephry会自动把I2C Drive代码一并编译进来，我们就可以在开发阶段使用I2C的功能了
2. 使用方法

在你代码文件中包含“drivers/i2c.h”即可

#include <drivers/i2c.h>

3. 主要API介绍
3.1 i2c_configure

3.1.1 函数介绍

函数原型
	

作用
	

返回值
int i2c_configure(const struct device *dev, uint32_t dev_config)	配置主机的i2c控制器	0成功，非0失败

3.1.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
dev_config	uint32_t	配置值，要求32位

dev_config可取以下选项

宏定义
	

作用
I2C_SPEED_STANDARD	标准模式：100 kHz
I2C_SPEED_FAST	快速模式：400 kHz
I2C_SPEED_FAST_PLUS	快速模式+：1 MHz
I2C_SPEED_HIGH	高速模式：3.4 MHz
I2C_SPEED_ULTRA	超快速模式：5 MHz
I2C_ADDR_10_BITS	使用10位寻址。不推荐使用-改用I2C_MSG_ADDR_10_BITS 
I2C_MODE_MASTER	使当前控制器作为主控制器
3.2 i2c_transfer

3.2.1 函数介绍

函数原型
	

作用
	

返回值
int i2c_transfer(const struct device *dev, struct i2c_msg *msgs, uint8_t num_msgs, uint16_t addr)	在主模式下执行到另一个I2C设备的数据传输	0成功，非0失败

3.2.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
msgs	struct i2c_msg *	要传输的消息的msgs数组
num_msgs	uint8_t	要传输的msgs数组长度
addr	uint16_t	I2C目标设备的地址
3.3 i2c_recover_bus

3.3.1 函数介绍

函数原型
	

作用
	

返回值
int i2c_recover_bus(const struct device *dev)	恢复I2C总线，在I2C通讯时出现了异常错误导致后续无法正常通讯，可以使用这个API尝试恢复I2C，若不行需要重新断电复位	0成功，非0失败

3.3.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
3.4 i2c_slave_register

3.4.1 函数介绍

函数原型
	

作用
	

返回值
static inline int i2c_slave_register(const struct device *dev,  struct i2c_slave_config *cfg)	将提供的配置注册为控制器的从属设备	0成功，非0失败

3.4.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
cfg	struct i2c_slave_config *	带有I2C驱动程序使用的函数和参数的cfg Config struct
3.5 i2c_slave_unregister

3.5.1 函数介绍

函数原型
	

作用
	

返回值
static inline int i2c_slave_unregister(const struct device *dev, struct i2c_slave_config *cfg)	将提供的配置注销为从属设备	0成功，非0失败

3.5.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
cfg	struct i2c_slave_config *	带有I2C驱动程序使用的函数和参数的cfg Config struct
3.6 i2c_slave_driver_register

3.6.1 函数介绍

函数原型
	

作用
	

返回值
int i2c_slave_driver_register(const struct device *dev)	指示I2C从设备将自身注册到I2C控制器	0成功，非0失败

3.6.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
3.7 i2c_slave_driver_unregister

3.7.1 函数介绍

函数原型
	

作用
	

返回值
int i2c_slave_driver_unregister(const struct device *dev);	指示I2C从设备从I2C控制器注销自身	0成功，非0失败

3.7.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
3.8 i2c_write

3.8.1 函数介绍

函数原型
	

作用
	

返回值
static inline int i2c_write(const struct device *dev,  const uint8_t *buf, uint32_t num_bytes, uint16_t addr)	将数据写入I2C设备	0成功，非0失败

3.8.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
buf	const uint8_t *	传输数据的buf内存池
num_bytes	uint32_t	传输数据的buf大小
addr	uint16_t	I2C设备地址

3.8.3 备注

此函数写入一次会产生起始与终止信号
3.9 i2c_read

3.9.1 函数原型

参数名
	

作用
	

返回值
static inline int i2c_read(const struct device *dev, uint8_t *buf, uint32_t num_bytes, uint16_t addr)	从I2C设备读取一定数量的数据	0成功，非0失败

3.9.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
buf	uint8_t *	存储数据的buf内存池
num_bytes	uint32_t	存储数据的buf大小
addr	uint16_t	I2C设备地址

3.9.3 备注

此函数读取一次会产生起始与终止信号
4.0 i2c_write_read

4.0.1 函数介绍

函数原型
	

作用
	

返回值
static inline int i2c_write_read(const struct device *dev, uint16_t addr, const void *write_buf, size_t num_write, void *read_buf, size_t num_read)	从I2C设备写入然后读取数据	0成功，非0失败

4.0.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
addr	uint16_t	I2C设备地址
write_buf	const void *	指向要写入的数据的指针
num_write	size_t	要写入的字节数
read_buf	void *	指向读取数据的存储器的指针
num_read	size_t	要读取的字节数

4.0.3 备注

此函数只有在读取之后才会产生终止信号
4.1 i2c_burst_read

4.1.1 函数介绍

函数原型
	

作用
	

返回值
static inline int i2c_burst_read(const struct device *dev, uint16_t dev_addr, uint8_t start_addr, uint8_t *buf, uint32_t num_bytes)	从I2C设备的内部地址读取多个字节	0成功，非0失败

4.1.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
dev_addr	uint16_t	I2C设备地址
start_addr	uint8_t	读取的地址
buf	uint8_t *	指向读取数据的存储器的指针
num_bytes	uint32_t	要读取的字节数
4.2  i2c_burst_write

4.2.1 函数介绍

函数原型
	

作用
	

返回值
static inline int i2c_burst_write(const struct device *dev, uint16_t dev_addr, uint8_t start_addr, const uint8_t *buf, uint32_t num_bytes)	将多个字节写入I2C设备的内部地址。	0成功，非0失败

4.2.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向I2C Device的指针
dev_addr	uint16_t	I2C设备地址
start_addr	uint8_t	写入的地址
buf	uint8_t *	指向写入数据的存储器的指针
num_bytes	uint32_t	要写入的字节数
4. I2C示例：读写与擦除”FM24V10 FLASH“
4.1 FM24V10 FLASH简介

FM24V10的FLASH是EEPROM 1MB的铁电工艺存储器，以下是它的特性简介

型号
	

容量
	

FLASH
	

特性
	

掉电存储时间
	

支持通讯协议
	

I2C地址
	

读写次数
	

最大频率
	

工作电压
	

工作温度

FM24V10	1MB	EEPROM	掉电不丢失	十年	I2C/SPI	0x50	100万亿次	3.4MHZ	2.0V-3.6V	-40°C至+ 85°C

FM24V10相当于一个小型MCU，内部有一个FLASH，我们通过与它进行通讯来读写擦除它内部的FLASH，用来存储我们的数据，它支持I2C与SPI，这里我们可以通过I2C来控制。

FM24V10在传输读写时候要求前两个字节是地址位，地址高位与地址的低位

 有些I2C设备有限制，最大一次只能传输256个字节，所以在FM24V10里除去地址位，一次最大只能写入254个。
4.2 着手开发

以下开发时我对Zephry的I2C API进行封装了，为了我能更方便的调用。

4.2.1 基本头文件

    #include <zephyr.h>
    #include <sys/printk.h>
    #include <device.h>
    #include <drivers/i2c.h>
    #include <stdlib.h>
    #include <string.h>

4.2.2 用宏定义将设备信息定义出来

这些信息在FM24V10的介绍里都有提到

最后的宏函数是用于打包高位与低位地址的

    #define FM24V10_FLASH       0x50            //i2c addr
    #define I2C_DRIVE_NAME      "I2C_1"         //zephry i2c drive name
    #define FM24V10_ADDR_NUM    2               //fm24v10 Address byte
    #define ADDR_H              0               //addr height
    #define ADDR_L              0               //addr low
    #define BUF_LEN             8               //buff len
    #define SER_START(startBuf,h,l) startBuf[0]=h;startBuf[1]=l;

 这里说一下如何知道自己I2C的驱动名称是什么

4.2.3 寻找自己的驱动名称

4.2.3.1 build目录下寻找

开启I2C之后并编译之后在build/zephry/zephry.dts里搜索i2c1可以找到如下定义

    i2c1: arduino_i2c: i2c@40005400 {
          compatible = "st,stm32-i2c-v2";
          clock-frequency = < 0x61a80 >;
          #address-cells = < 0x1 >;
          #size-cells = < 0x0 >;
          reg = < 0x40005400 0x400 >;
          clocks = < &rcc 0x2 0x200000 >;
          interrupts = < 0x1f 0x0 >, < 0x20 0x0 >;
          interrupt-names = "event", "error";
          status = "okay";
          label = "I2C_1";
          pinctrl-0 = < &i2c1_scl_pb8 &i2c1_sda_pb9 >;
          bme280@76 {
              compatible = "bosch,bme280";
              status = "okay";
              label = "BME280";
              reg = < 0x76 >;
          };
      };

可以看到其中的label就是”I2C_1“

4.2.3.2 boards目录下寻找

在boards目录下找到你的板子型号，如我的是boards/arm/stm32f746g_disco

在这个文件夹下打开你的dts文件，然后找i2c

    &i2c1 {
        pinctrl-0 = <&i2c1_scl_pb8 &i2c1_sda_pb9>;
        status = "okay";
        clock-frequency = <I2C_BITRATE_FAST>;
        bme280@76 {
            compatible = "bosch,bme280";
            status = "okay";
            label = "BME280";
            reg = <0x76>;
        };
    };

可以看到这个i2c前面有一个”&“引用运算符，就代表这个是引用别的地方的，那么只有在头文件里了，我们可以一步一步分析头文件最终到”dts/arm/st/f7/stm32f745.dtsi“这个文件里

    i2c4: i2c@40006000 {
               compatible = "st,stm32-i2c-v2";
               clock-frequency = <I2C_BITRATE_STANDARD>;
               #address-cells = <1>;
               #size-cells = <0>;
               reg = <0x40006000 0x400>;
               clocks = <&rcc STM32_CLOCK_BUS_APB1 0x01000000>;
               interrupts = <95 0>, <96 0>;
               interrupt-names = "event", "error";
               status = "disabled";
               label = "I2C_4";
           };

就可以看到最终的定义了。

4.2.3.3 添加overlay文件

在你的工程目录下新建目录”boards“，然后在这个目录下新建一个后缀名为”.overlay“的文件，然后在里面引用i2c，并添加label属性就可以了

    &i2c1 {
        label="I2c_1"
    };

4.2.4 定义I2C设备句柄

此代码模式为单列模式，所以定义一个全局的i2c设备句柄

    //par
    const struct device* i2c_dev = NULL;

4.2.5 init函数

这个函数的作用就是实例化i2c_dev设备指针，调用device_get_binding获取zephry i2c的设备驱动指针

成功返回0，否则返回非0

    int fm24v10_init(){
     
        if(i2c_dev != NULL){
     
            return -1;
     
        }
     
        i2c_dev = device_get_binding(I2C_DRIVE_NAME);  if(i2c_dev == NULL) return -2;
     
        return 0;
     
    }

4.2.6 ser_buf

此函数的作用是将高位地址与低位地址还有buff打包成一个新的buff

组合结构为：高位地址->低位地址→DATA 按照此顺序进行打包

    void ser_buf(uint8_t* m_buff,uint8_t h,uint8_t l,uint8_t* buff,size_t len){
     
            memset(m_buff,0,len+FM24V10_ADDR_NUM);
            m_buff[0] = h;
            m_buff[1] = l;
            memcpy(m_buff+2,buff,len);
    }

4.2.7 fm24v10_write

此函数会调用ser_buf进行打包，然后调用i2c_write发送出去，因为fm24v10要求前两位是高低地址位，所以前两个字节进行一次打包。

    int fm24v10_write(uint8_t addr_height,uint8_t addr_low,uint8_t* buff,size_t len){
     
        //check ptr
        if(buff == NULL || i2c_dev == NULL) return -1;
     
        //check ready
        if(!device_is_ready(i2c_dev)) return -2;
     
        //serialize buff
        uint8_t m_buff[BUF_LEN+FM24V10_ADDR_NUM] = {0};
        ser_buf(m_buff,addr_height,addr_low,buff,len);
     
        //write
        if(i2c_write(i2c_dev,m_buff,len+FM24V10_ADDR_NUM,FM24V10_FLASH) !=0) return -4;
     
        return 0;
     
    }

4.2.8 fm24v10_read

此函数的作用是读取数据，在读取时候需要先向FM24V10发送地址位，但是这期间不能有停止位，所以我们需要内部使用i2c_write_read这个函数来实现，它会先写在读取，期间不会产生停止位

    int fm24v10_read(uint8_t addr_height,uint8_t addr_low,uint8_t* buff, size_t len){
     
        //check ptr
        if(buff == NULL || i2c_dev == NULL) return -1;
     
        //check ready
        if(!device_is_ready(i2c_dev)) return -2;
     
        //read
        uint8_t startBuf[FM24V10_ADDR_NUM] = {0};
        SER_START(startBuf,addr_height,addr_low);
        if(i2c_write_read(i2c_dev,FM24V10_FLASH,startBuf,FM24V10_ADDR_NUM,buff,len) != 0){
     
            return -3;
     
        }
     
        return 0;
     
    }

4.2.9 printu

打印函数，因为zephry i2c下用uint8进行通讯，所以写一个函数用来打印，比较方便

    void printu(uint8_t *buff,size_t len){
     
        for(int i = 0; i < len;++i){
     
            printk("%d",buff[i]);
     
        }
     
        printk("\n");
     
    }

4.3 main函数

main函数就是先初始化，然后写入，在读取
```
    void main(void)
    {
        printk("---------- FM240V10 Flash - I2C ----------\n");
     
        if(fm24v10_init() != 0) {
     
            printk("Error fm240v10 init\n");
            return;
        }
     
        uint8_t wrBuf[BUF_LEN] = {1,2,3,4,5,6,7,8};
        uint8_t rdBuf[BUF_LEN] = {0};
     
        if(fm24v10_write(ADDR_H,ADDR_L,wrBuf,BUF_LEN) != 0){
     
            printk("Error write flash\n");
            return;
     
        }
     
        if(fm24v10_read(ADDR_H,ADDR_L,rdBuf,BUF_LEN) != 0){
     
            printk("Error read flash\n");
            return;
     
        }
     
        printk("Write:");
        printu(wrBuf,BUF_LEN);
        printk(" Read:");
        printu(rdBuf,BUF_LEN);
     
        return;
     
    }
```
4.3.1 完整代码
```
    #include <zephyr.h>
    #include <sys/printk.h>
    #include <device.h>
    #include <drivers/i2c.h>
    #include <stdlib.h>
    #include <string.h>
     
    #define FM24V10_FLASH       0x50            //i2c addr
    #define I2C_DRIVE_NAME      "I2C_1"         //zephry i2c drive name
    #define FM24V10_ADDR_NUM    2               //fm24v10 Address byte
    #define ADDR_H              0               //addr height
    #define ADDR_L              0               //addr low
    #define BUF_LEN             8               //buff len
    #define SER_START(startBuf,h,l) startBuf[0]=h;startBuf[1]=l;
    //par
    const struct device* i2c_dev = NULL;
     
    //func
    int fm24v10_init(){                                                                                                    
        if(i2c_dev != NULL){
     
            return -1;
     
        }
     
        i2c_dev = device_get_binding(I2C_DRIVE_NAME);  if(i2c_dev == NULL) return -2;
     
        return 0;
     
    }
     
    void ser_buf(uint8_t* m_buff,uint8_t h,uint8_t l,uint8_t* buff,size_t len){
     
            memset(m_buff,0,len+FM24V10_ADDR_NUM);
            m_buff[0] = h;
            m_buff[1] = l;
            memcpy(m_buff+2,buff,len);
    }
     
    //write
    int fm24v10_write(uint8_t addr_height,uint8_t addr_low,uint8_t* buff,size_t len){
     
        //check ptr
        if(buff == NULL || i2c_dev == NULL) return -1;
     
        //check ready
        if(!device_is_ready(i2c_dev)) return -2;
     
        //serialize buff
        uint8_t m_buff[BUF_LEN+FM24V10_ADDR_NUM] = {0};
        ser_buf(m_buff,addr_height,addr_low,buff,len);
     
        //write
        if(i2c_write(i2c_dev,m_buff,len+FM24V10_ADDR_NUM,FM24V10_FLASH) !=0) return -4;
     
        return 0;
     
    }
     
    //read
    int fm24v10_read(uint8_t addr_height,uint8_t addr_low,uint8_t* buff, size_t len){
     
        //check ptr
        if(buff == NULL || i2c_dev == NULL) return -1;
     
        //check ready
        if(!device_is_ready(i2c_dev)) return -2;
     
        //read
        uint8_t startBuf[FM24V10_ADDR_NUM] = {0};
        SER_START(startBuf,addr_height,addr_low);
        if(i2c_write_read(i2c_dev,FM24V10_FLASH,startBuf,FM24V10_ADDR_NUM,buff,len) != 0){
     
            return -3;
     
        }                 
            return 0;
     
    }
     
    void printu(uint8_t *buff,size_t len){
     
        for(int i = 0; i < len;++i){
     
            printk("%d",buff[i]);
     
        }
     
        printk("\n");
     
    }
     
    void main(void)
    {
        printk("---------- FM240V10 Flash - I2C ----------\n");
     
        if(fm24v10_init() != 0) {
     
            printk("Error fm240v10 init\n");
            return;
        }
     
        uint8_t wrBuf[BUF_LEN] = {1,2,3,4,5,6,7,8};
        uint8_t rdBuf[BUF_LEN] = {0};
     
        if(fm24v10_write(ADDR_H,ADDR_L,wrBuf,BUF_LEN) != 0){
     
            printk("Error write flash\n");
            return;
     
        }
     
        if(fm24v10_read(ADDR_H,ADDR_L,rdBuf,BUF_LEN) != 0){
     
            printk("Error read flash\n");
            return;
     
        }
     
        printk("Write:");
        printu(wrBuf,BUF_LEN);
        printk(" Read:");
        printu(rdBuf,BUF_LEN);
     
        return;
     
    }
```
4.3.2 运行结果

使用west编译并烧录到flash之后使用minicom可以看到如下输出：

    Zephyr OS build zephyr-v2.6.0-1753-g365ff6db9f02  ***
    ---------- FM240V10 Flash - I2C ----------
    Write:12345678
     Read:12345678

即代表正确向flash 0x0的起始地址写入8字节，并成功读取出来，因为是断电保存的，即便你断电后在从这个地址读取依然是这个字节。

4.3.3 擦除

其实擦除也很简单，因为I2C的限制，每次最大只能发送256个，这里我改成128，因为这样字节差上我比较好发送，FM24V10不支持擦除的指令，但是支持随机读写，所以擦除对于它们来说意义不大，其次1MB擦除比较耗时，所以一般情况下不建议写擦除功能，这里我写了一个可以用于参考

是以128字节为单位擦除，此函数会非常耗时，一般不建议写擦除，因为对于随机存储来说，擦除本身就没有意义。

    void erase(){        
        //buff大小为130，前两位为地址位                                                                                                     
        uint8_t buff[130] = {0};
        //长度等于1mb除于128字节
        size_t len = 1048576/128;
        //每次递增时地址+128偏移
        for(uint16_t i = 0;i<len;i+=128){
            //地址从高位到低位写入到
            buff[0]=i >> 8;
            buff[1]=i & 0xff;
            i2c_write(i2c_dev,buff,130,FM24V10_FLASH);
        }
    }

二、SPI Dirve
1. 开启方法

与I2C Dirve一样，在prj.conf文件中添加CONFIG来开启SPI Dirve

CONFIG_SPI=y

2. 使用方法

与I2C一致包含头文件即可

#include <drivers/spi.h>

3. 结构体介绍
3.1 结构体介绍

3.1.1 spi_config

3.1.1.1 结构体定义

    struct spi_config {
        uint32_t        frequency;
        uint16_t        operation;
        uint16_t        slave;
     
        const struct spi_cs_control *cs;
    };

3.1.1.2 成员介绍

成员名
	

类型
	

作用
frequency	uint32_t	通讯频率，即时钟工作频率
operation	uint16_t	工作模式
slave	uint16_t	从设备编号，下标从0开始

3.1.1.2.1 operation的额外介绍

operation比较特殊，SPI是通过设置CPOL和CPHA标志位来确定SPI的工作模式，可以通过设置这个值来控制

Zephry通过对它的BIT位进行设置来控制对于的功能，与GPIO的特殊功能寄存器一样，不同的位代表不同的功能

名称
	

BIT
	

作用
operational mode	0	设置主从，此位决定当前SPI是主还是从，可以取宏：SPI_OP_MODE_MASTER、SPI_OP_MODE_SLAVE
mode	1 : 3	CPOL与CPHA的工作模式
transfer	4	LSB或MSB优先，即大小端优先模式，若设置为MSB则优先传输高位，LSB则优先传输低位，主从必须对应
word_size	5 : 10	数据帧的大小（以位为单位），一次传输多少位，一般与SPI的数据寄存器大小一致
lines	11 : 12	SO线的工作模式，这个模式下面会介绍一下
cs_hold	13	是否需要让CS线处于HOLD状态，即SPI通讯断开后不要断开CS线
lock_on	14	启动锁模式，工作时候不可被打断，占用模式
cs_active_high	15	CS信号的逻辑电平，0代表发送片选CS信号时低电平有效，1即高电平有效

3.1.1.2.2 lines位的介绍

    lines位是告诉zephry当前spi的通讯线是几根，每根线的作用是什么，根据实际情况设置

lines具有如下工作模式

协议
	

数据线数量及功能
	

通讯方式
Single SPI（标准SPI）	1根发送，1根接收	全双工
Dual SPI（双线SPI）	收发共用2根数据线	半双工
Qual SPI（四线SPI）	收发共用4根数据线	半双工
Octal SPI（八线SPI）	收发共用8根数据线	半双工

一般情况下都是默认的，不需要修改这个位

针对这些位的设置，Zephry为了方便我们设置，为我们提供了许多宏，可以直接对这些位进行设置，这样就省去了位操作运算，Zephry已经为我们封装好了

3.1.1.2.3 位设置宏

    operational mode

名称
	

作用
SPI_OP_MODE_MASTER	设置当前SPI为MASTER
SPI_OP_MODE_SLAVE	设置当前SPI为SLAVE
SPI_OP_MODE_GET	获取当前SPI是MASTER还是SLAVE

    mode

名称
	

作用
SPI_MODE_CPOL	设置CPOL为1，若要设置为0需要使用取反运算符!SPI_MODE_CPOL
SPI_MODE_CPHA	设置CPHA为1，若要设置为0需要使用取反运算符!SPI_MODE_CPHA
SPI_MODE_LOOP	此模式仅应用测试，所有发送的数据都会回到DR寄存器，用于测试是否已经发送出去了，且发送数据是否正确，此模式需要控制器支持
SPI_MODE_GET	获取当前工作模式

    transfer

名称
	

作用
SPI_TRANSFER_MSB	高位优先传输
SPI_TRANSFER_LSB	低位优先传输

    word_size

名称
	

作用
SPI_WORD_SIZE_SHIFT	设置每次传输5位
SPI_WORD_SET(_word_size_)	设置自定义每次传输多少位，_word_size_是一个参数
SPI_WORD_SIZE_GET	获取当前传输多少位

    lines

名称
	

作用

名称
	

作用
SPI_LINES_SINGLE	Single SPI（标准SPI）
SPI_LINES_DUAL	Dual SPI（双线SPI）
SPI_LINES_QUAD	Qual SPI（四线SPI）
SPI_LINES_OCTAL	Octal SPI（八线SPI）

    cs_hold

名称
	

作用
SPI_HOLD_ON_CS	使能CS HOLD，若要取消使能使用取反运算符:!SPI_HOLD_ON_CS

    lock_on

名称
	

作用
SPI_LOCK_ON	使能LOCK，若要取消使能使用取反运算符:!SPI_LOCK_ON

    cs_active_high

名称
	

作用
SPI_CS_ACTIVE_HIGH	片选信号为高电平，若要低电平则反:!SPI_CS_ACTIVE_HIGH

有些宏函数是不需要参数的，因为它们的值会根据operation高低位来设置，如SPI_OP_MODE_MASTER是0，刚好在赋值时对应operation的第0位

以上的宏设置需要使用C语言的或运算，比如设置一次传输8BIT，和当前SPI设备为主设备：

operation = SPI_WORD_SET(8) | SPI_OP_MODE_MASTER,		//设置一次传输8BIT，以及当前SPI为主设备

或运算的特性是进行或运算的BIT位有一个为1，则为1，利用这个特性我们可以实现用两个不不同BIT位的值进行或运算就可以设置不同的位了，这里假设第7位对应的是工作模式，第3位对应的是锁功能，我们想将第7位与第3位置1：

state = 64 | 4

底层过程如下：

state是8位的，其BIT位如下：

0 0 0 0 0 0 0 0

64的BIT位如下：

0 1 0 0 0 0 0 0

4的BIT位如下：

0 0 0 0 0 1 0 0

64 | 4

0 1 0 0 0 0 0 0

0 0 0 0 0 1 0 0

=

0 1 0 0 0 1 0 0

然后将这个值赋予给state，那么state的对应的BIT就成功被设置了

3.1.2  spi_buf_set

3.1.2.1 结构体定义

    struct spi_buf_set {
        const struct spi_buf *buffers;
        size_t count;
    };

3.1.2.2 成员介绍

成员名
	

类型
	

作用
buffers	const struct spi_buf	传输时候的buff，需要注意的是这个变量需要在定义时完成初始化，因为它有const关键字
count	size_t	spi_buf指向数组有多少个，这个值比较特殊，spi_buf指向的地址可能是一个队列，连续的，count用于表示spi_buf有几个

3.1.3 spi_buf

3.1.3.1 结构体定义

    struct spi_buf {
        void *buf;
        size_t len;
    };

3.1.3.2 成员介绍

成员名
	

类型
	

作用
buf	void *	要传输的缓冲区指针
len	size_t	缓冲区数据长度
4. 主要API介绍
4.1 spi_write

4.1.1 函数介绍

函数原型
	

作用
	

返回值
static inline int spi_write(const struct device *dev, const struct spi_config *config, const struct spi_buf_set *tx_bufs)	向SPI设备写入数据	0成功，非0失败

4.1.2 参数介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向SPI Device的指针
config	const struct spi_config *	指向spi的配置属性的指针
tx_bufs	const struct spi_buf_set *	指向spi buf的结构指针
4.2 spi_read

4.2.1 函数介绍

函数原型
	

作用
	

返回值
static inline int spi_read(const struct device *dev, const struct spi_config *config, const struct spi_buf_set *rx_bufs)	读取SPI设备内容	0成功，非0失败

4.2.2 参数介绍

参数名
	

类型
	

介绍

参数名
	

类型
	

介绍
dev	const struct device *	指向SPI Device的指针
config	const struct spi_config *	指向spi的配置属性的指针
rx_bufs	const struct spi_buf_set *	指向spi buf的结构指针
4.3 spi_transceive

4.3.1 函数介绍
函数原型	作用	返回值
int spi_transceive(const struct device *dev, const struct spi_config *config, const struct spi_buf_set *tx_bufs,const struct spi_buf_set *rx_bufs)	写入或读取 I2C数据	0成功，非0失败

4.3.2 参数介绍
参数名	类型	介绍
dev	const struct device *	指向SPI Device的指针
config	const struct spi_config *	指向spi的配置属性的指针
tx_bufs	const struct spi_buf_set *	指向spi写入buf的结构指针
rx_bufs	const struct spi_buf_set *	指向spi接收buf的结构指针

4.3.3 备注

这个函数可以实现同时读写功能，spi_write与spi_read都基于这个函数实现，spi_write在调用时候会将rx设置为空，而spi_read调用时会将tx设置为空
5 简单的读写示例

SPI设备一般默认一次只能传输最大到255字节
5.1 写
```
    //包含基本头文件
    #include <zephyr.h>
    #include <sys/printk.h>
    #include <device.h>
    #include <drivers/sensor.h>
    #include <drivers/spi.h>
    #include <stdlib.h>
    #include <string.h>
    //SPI最大传输字节
    #define SPI_MAX_MSG_LEN 255
    //传输时的buff
    static uint8_t txmsg[SPI_MAX_MSG_LEN]={0,0};
    //缓冲区指针
    static struct spi_buf tx = {
        .buf = txmsg,
    };
    //传输结构体
    const static struct spi_buf_set tx_bufs = {
        .buffers = &tx, //缓冲区指针
        .count=1,   //一个缓冲区指针
    };
    //配置项结构体
    static struct spi_config spi_cfg = {
        .operation = SPI_WORD_SET(8) | SPI_OP_MODE_MASTER,		//设置一次传输8BIT，以及当前SPI为主设备
        .slave = 0,												//选择第0个设备
        .frequency = 1000000U,									//设置通讯频率
    };
     
    /*
     * 先判断SPI设备是否正常
     * 然后调用SPI写函数
     *
     */
    void main(){
        //get bem device
        const struct device* dev = device_get_binding("SPI_2");
        if(dev == NULL){
     
            printk("ERR: No Device Drivers!\n");
            return;
     
        }
     
        //check device
        if(!device_is_ready(dev)){
     
            printk("ERR: Device Was Not Ready - %s\n",dev->name);
            return;
     
        }
     
        if(spi_write(dev,&spi_cfg,&tx_bufs) != 0){
            printk("Spi Write Error\n");
            return;
        }
     
        printk("Spi Write Success\n");
        return;
    }
```
5.2 读
```
    //包含基本头文件
    #include <zephyr.h>
    #include <sys/printk.h>
    #include <device.h>
    #include <drivers/sensor.h>
    #include <drivers/spi.h>
    #include <stdlib.h>
    #include <string.h>
    //SPI最大传输字节
    #define SPI_MAX_MSG_LEN 255
    //传输时的buff
    static uint8_t rxmsg[SPI_MAX_MSG_LEN]={0,0};
    //缓冲区指针
    static struct spi_buf rx = {
        .buf = rxmsg,
    };
    //传输结构体
    const static struct spi_buf_set rx_bufs = {
        .buffers = &tx, //缓冲区指针
        .count=1,   //一个缓冲区指针
    };
     
    //配置项结构体
    static struct spi_config spi_cfg = {
        .operation = SPI_WORD_SET(8) | SPI_OP_MODE_MASTER,		//设置一次传输8BIT，以及当前SPI为主设备
        .slave = 0,												//选择第0个设备
        .frequency = 1000000U,									//设置通讯频率
    };
     
    /*
     * 先判断SPI设备是否正常
     * 然后调用SPI写函数
     *
     */
    void main(){
        //get bem device
        const struct device* dev = device_get_binding("SPI_2");
        if(dev == NULL){
     
            printk("ERR: No Device Drivers!\n");
            return;
     
        }
     
        //check device
        if(!device_is_ready(dev)){
     
            printk("ERR: Device Was Not Ready - %s\n",dev->name);
            return;
     
        }
     
        if(spi_write(dev,&spi_cfg,&rx_bufs) != 0){
            printk("Spi Read Error\n");
            return;
        }
     
        printk("Spi Read Success\n");
        return;
    }
```

————————————————
版权声明：本文为CSDN博主「17岁boy想当攻城狮」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/bjbz_cxy/article/details/119913874