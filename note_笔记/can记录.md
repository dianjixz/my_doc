 

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
