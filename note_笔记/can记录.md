 

1、#ip link set canX down //关闭can设备；
 2、#ip link set canX up   //开启can设备；
 3、#ip -details link show canX //显示can设备详细信息；
 4、#candump canX  //接收can总线发来的数据；
 5、#ifconfig canX down //关闭can设备，以便配置;
 6、#ip link set canX up type can bitrate 250000 //设置can波特率
 7、#conconfig canX bitrate + 波特率；
 8、#canconfig canX start //启动can设备；
 9、#canconfig canX ctrlmode loopback on //回环测试；
 10、#canconfig canX restart // 重启can设备；
 11、#canconfig canX stop //停止can设备；
 12、#canecho canX //查看can设备总线状态；
 13、#cansend canX --identifier=ID+数据 //发送数据；
 14、#candump canX --filter=ID：mask//使用滤波器接收ID匹配的数据



作者：亚比伦哥
链接：https://www.jianshu.com/p/ebd4e2a06ca6
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。





# canconfig 移植记录

https://blog.csdn.net/weixin_33850890/article/details/85887400

https://zhuanlan.zhihu.com/p/470681140?utm_id=0







    canconfig 配置命令
    
    canconfig can0 restart-ms 1000 bitrate 1000000 ctrlmode triple-sampling on
    canconfig can1 restart-ms 1000 bitrate 1000000 ctrlmode triple-sampling on
    
    canconfig 启动命令
    
    canconfig can1 start
    canconfig can0 start
    
    cansend 发送命令
    
    cansend can0 7ff#112233
    cansend can1 7ff#112233
    
    candump 监控接收命令
    
    candump can0
    candump can1
    
    canconfig 停止命令
    
    canconfig can0 stop
    canconfig can1 stop


ip link set can0 down
ip link set can1 down
sleep 1
ip link set can0 up type can bitrate 400000
ip link set can1 up type can bitrate 400000





http://t.zoukankan.com/chenfulin5-p-7816220.html





wget http://kernel.org/pub/linux/utils/net/iproute2/iproute2-4.0.0.tar.xz

iproute2-4.0.0



编译can工具

http://t.zoukankan.com/idyllcheung-p-10637217.html



https://www.cnblogs.com/chenfulin5/p/6797756.html

一、下载源码

下载canutils和libsocketcan

libsocketcan地址：https://public.pengutronix.de/software/libsocketcan/libsocketcan-0.0.11.tar.bz2 #0.0.11版本

canutils地址：https://public.pengutronix.de/software/socket-can/canutils/v4.0/canutils-4.0.6.tar.bz2 #4.0.6版本

https://git.pengutronix.de/cgit/tools/canutils/ 这个地址git下来的有点问题。

二、先编译libsocketcan

//解压

tar -jxvf libsocketcan-0.0.11.tar.bz2

cd ./libsocketcan-0.0.11

mkdir out

//配置

./configure --host=arm-linux-gnueabhif --prefix=/home/zhangyi/work/psoc_ltp/libsocketcan-0.0.11/out

//编译

make

make install

如果没有新建out文件夹，会报错。

生成的库在out/lib目录下，

三、编译canutils

```
// 先将 libsocketcan 里面的头文件 libsocketcan.h can_netlink.h 复制到 canutils 的 include 里面
```

//解压

tar -jxvf canutils-4.0.6.tar.bz2

cd ./canutils-4.0.6
mkdir out
//配置

./configure --host=arm-linux-gnueabhif --prefix=/home/zhangyi/work/psoc_ltp/canutils-4.0.6/out  libsocketcan_LIBS=-lsocketcan LDFLAGS="-L/home/zhangyi/work/psoc_ltp/libsocketcan-0.0.11/out/lib/" libsocketcan_CFLAGS="-I/home/zhangyi/work/psoc_ltp/libsocketcan-0.0.11/out/include"

//编译

make

make install

四、将libsocketcan和canutils生成的文件拷贝到文件系统中

./libsocketcan-0.0.11/out/lib/ （包含libsocketcan.so.2.3.0和pkgconfig）

./canutils-4.0.6/out/（包含bin lib sbin share）




创建虚拟CAN接口


在Linux上能使用虚拟CAN接口之前，需要在终端执行以下三个步骤：

加载vcan内核模块: sudo modprobe vcan

创建虚拟CAN接口: sudo ip link add dev vcan0 type vcan

将虚拟CAN接口处于在线状态: sudo ip link set up vcan0 或 sudo ip link set dev vcan0 up

将虚拟CAN接口处于离线状态: sudo ip link set down vcan0 或 sudo ip link set dev vcan0 down

删除虚拟CAN接口: sudo ip link del dev vcan0

然后，通过命令ip addr | grep "can" 来验证是否可用并处于在线状态

使用 can-utils 测试CAN通信
```bash
Ubuntu/Debian: sudo apt install can-utils
```
发送CAN信息
```bash
cansend vcan0 123#00FFAA5501020304
```
查看CAN信息
```bash
candump vcan0
```

CAN 的常用操作命令：
```bash
#ifconfig -a //查到当前can网络 can0 can1，包括收发包数量、是否有错误等等
#ip link set vcan0 down //关闭can设备；或使用ifconfig canX down
#ip link set vcan0 up //开启can设备；或使用ifconfig canX up
#ip -details link show vcan0 //显示can设备详细信息；
#ip link set vcan0 up type can bitrate 250000 //设置can波特率
#canconfig vcan0 ctrlmode loopback on //回环测试；
#canconfig vcan0 restart // 重启can设备；
#canconfig vcan0 stop //停止can设备；
#canecho vcan0 //查看can设备总线状态；
#candump vcan0 //接收can总线发来的数据；
#cansend vcan0 --identifier=ID+数据 //发送数据；
#candump vcan0 --filter=ID：mask//使用滤波器接收ID匹配的数据
```



CAN数据读取的代码：
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
int main(void)
{
    struct ifreq ifr = {0};
    struct sockaddr_can can_addr = {0};
    struct can_frame frame = {0};
    int sockfd = -1;
    int i;
    int ret;
 
    /* 打开套接字 */
    sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(0 > sockfd) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
 
    /* 指定can0设备 */
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(sockfd, SIOCGIFINDEX, &ifr);
    can_addr.can_family = AF_CAN;
    can_addr.can_ifindex = ifr.ifr_ifindex;
 
    /* 将can0与套接字进行绑定 */
    ret = bind(sockfd, (struct sockaddr *)&can_addr, sizeof(can_addr));
    if (0 > ret) {
        perror("bind error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
 
    /* 设置过滤规则 */
    //setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
 
    /* 接收数据 */
    for ( ; ; ) {
        if (0 > read(sockfd, &frame, sizeof(struct can_frame))) {
            perror("read error");
            break;
        }
 
        /* 校验是否接收到错误帧 */
        if (frame.can_id & CAN_ERR_FLAG) {
            printf("Error frame!\n");
            break;
        }
 
        /* 校验帧格式 */
        if (frame.can_id & CAN_EFF_FLAG)    //扩展帧
            printf("扩展帧 <0x%08x> ", frame.can_id & CAN_EFF_MASK);
        else        //标准帧
            printf("标准帧 <0x%03x> ", frame.can_id & CAN_SFF_MASK);
 
        /* 校验帧类型：数据帧还是远程帧 */
        if (frame.can_id & CAN_RTR_FLAG) {
            printf("remote request\n");
            continue;
        }
 
        /* 打印数据长度 */
        printf("[%d] ", frame.can_dlc);
 
        /* 打印数据 */
        for (i = 0; i < frame.can_dlc; i++)
            printf("%02x ", frame.data[i]);
        printf("\n");
    }
 
    /* 关闭套接字 */
    close(sockfd);
    exit(EXIT_SUCCESS);
}
```
CAN数据写入的代码：
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
int main(void)
{
    struct ifreq ifr = {0};
    struct sockaddr_can can_addr = {0};
    struct can_frame frame = {0};
    int sockfd = -1;
    int ret;
 
    /* 打开套接字 */
    sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(0 > sockfd) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
 
    /* 指定can0设备 */
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(sockfd, SIOCGIFINDEX, &ifr);
    can_addr.can_family = AF_CAN;
    can_addr.can_ifindex = ifr.ifr_ifindex;
 
    /* 将can0与套接字进行绑定 */
    ret = bind(sockfd, (struct sockaddr *)&can_addr, sizeof(can_addr));
    if (0 > ret) {
        perror("bind error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
 
    /* 设置过滤规则：不接受任何报文、仅发送数据 */
    setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
 
    /* 发送数据 */
    frame.data[0] = 0xA0;
    frame.data[1] = 0xB0;
    frame.data[2] = 0xC0;
    frame.data[3] = 0xD0;
    frame.data[4] = 0xE0;
    frame.data[5] = 0xF0;
    frame.can_dlc = 6;    //一次发送6个字节数据
    frame.can_id = 0x123;//帧ID为0x123,标准帧
 
    for ( ; ; ) {
 
        ret = write(sockfd, &frame, sizeof(frame)); //发送数据
        if(sizeof(frame) != ret) { //如果ret不等于帧长度，就说明发送失败
            perror("write error");
            goto out;
        }
 
        sleep(1);        //一秒钟发送一次
    }
 
out:
    /* 关闭套接字 */
    close(sockfd);
    exit(EXIT_SUCCESS);
}

```


















————————————————
版权声明：本文为CSDN博主「小鱼仙官」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/yuchunhai321/article/details/129494525