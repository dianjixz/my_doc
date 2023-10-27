- Linux GPIO 驱动

使用 gpiolib 进行开发。


I2C驱动有4个重要的东西，I2C总线、I2C驱动、I2C设备、I2C设备器

    I2C总线：维护着两个链表（I2C驱动、I2C设备），管理I2C设备和I2C驱动的匹配和删除等
    I2C驱动：对应的就是I2C设备的驱动程序
    I2C设备：是具体硬件设备的一个抽象
    I2C设配器：用于I2C驱动和I2C设备间的通用，是SOC上I2C控制器的一个抽象
————————————————
版权声明：本文为CSDN博主「JT同学」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_42462202/article/details/91128914

驱动程序一般调用i2c_transfer来发送信息


spi的通信有两种，一种是同步，另一种是异步。
2.4. 数据传输流程

整体的数据传输流程大致上是这样的:

定义一个spi_message结构;  
用spi_message_init函数初始化spi_message;  
定义一个或数个spi_transfer结构，初始化并为数据准备缓冲区并赋值给spi_transfer相应的字段(tx_buf，rx_buf等);   
通过spi_message_init函数把这些spi_transfer挂在spi_message结构下;  
如果使用同步方式，调用spi_sync()，如果使用异步方式，调用spi_async();(我调试外设时，只使用过spi_sync  

同步spi_sync

异步spi_async



linux 平台设备框架  
linux将驱动和设备分离，为了达到这个效果，最终演变成了平台总线、平台设备、平台驱动三大块。  
- 平台总线： 比如i2c总线，spi总线等等。
- 平台设备：比如设备树上定义的设备。
- 平台驱动：比如实际的驱动程序。

实际上这些框架的划分能极大的让设备和驱动做到高内聚，底耦合。

所以开发驱动变成了，找到相关的总线，在总线上注册驱动，在总线上注册设备。当设备和驱动匹配时，就能 linux 世界中的一个设备正常工作了。






Linux platform平台总线、平台设备、平台驱动



BUILD自己的KERNEL HEADER

1、主要思路参考ubuntu系统中自带的kernel header

2、大致操作步骤如下：

    a、下载kernel代码

    b、配置OK后编译

    c、find . -name *.o | xargs rm -f 删除所有的.o和vmlinux*

    d、删除除script目录外所有的.c文件

    e、删除除arch/和include下的.h文件，script目录除外

    f、至此，基本的kernel header完成。可用于build内核ko模块。