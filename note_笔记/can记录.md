 

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
