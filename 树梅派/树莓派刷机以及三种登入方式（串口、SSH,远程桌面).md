 

https://leo-max.blog.csdn.net/article/details/113782096?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1.no_search_link&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1.no_search_link





刷机
镜像（3B和4B的镜像不一样，不够也有通用的镜像）
SD卡（一般16G以上）
读卡器
Win32DiskImager（将镜像烧到SD卡的软件）；这个软件可能会让自己的SD卡容量变小，方便可以再安装个内存卡修复软件。
        现在一般都买树莓派都会将镜像直接烧再SD卡上了，但如果需要换镜像的则需要以上几个步骤了。



串口登录
安装驱动
        用USB转串口模块需要事先安装驱动 CH340_341


USB-TTL转换模块（USB转串口模块）


USB-TTL连接树莓派串口
将模块的TXD线接到树莓派RXD针脚 （树莓派靠外侧第5个针脚），将模块RXD接到TXD针脚 （树莓派靠外侧第4个针脚）


修改系统配置，启用串口登录树莓派
1.打开SD卡根目录的"config.txt"文件，将以下内容添加在最后并且保存。（这样就停止了蓝牙，解除了对串口的占用）

dtoverlay=pi3-miniuart-bt
1
2.然后再修改根目录的"cmdline.txt"，将里面的内容全部替换成以下内容，以防万一，请先备份好这个文件的原内容。

dwc_otg.lpm_enable=0 console=tty1 console=serial0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline fsck.repair=yes rootwait
1
启动树莓派，并用相关软件进行串口登录（SecureCRT）

默认账号：pi

默认密码：raspberry

SSH登录
在串口登录后：

配置网络
输入指令：sudo nano /etc/wpa_supplicant/wpa_supplicant.conf

开始配置WIFI：
赋值下面指令到打开的编辑器里

 network={

        ssid="wifi名字"
    
        psk="密码"
        }
1
2
3
4
5
6
ctrl+o 保存（之后按下enter）
ctrl+x 退出 nano文档编辑器（进入到里面就可以编写）
重启 (sudo reboot)
输入 ifconfig 查看是否有IP地址
固定IP地址 输入 sudo nano /etc/rc.local

打开SSH功能 输入： sudo raspi-config
方向键上下选择第5个，回车确定：

方向键上下选择第2个，回车确定
方向键左右选择Yes，回车确定

在SecurCRT页面选择“闪电”快速登录，
协议选择SSH
主机名输入IP地址
用户名：pi


设置完SSH后，也可以选择Xshell登录，按自己喜好。

远程桌面连接
树莓派安装xrdp
输入:sudo apt-get install xrdp 安装xrdp环境

win+r打开命令窗口输入mstsc 开启远程桌面服务

然后输入树莓派ip即可进入树莓派界面。

输入用户名：pi 密码raspberry

进入桌面
————————————————
版权声明：本文为CSDN博主「LEO-max」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/zouchengzhi1021/article/details/113782096









