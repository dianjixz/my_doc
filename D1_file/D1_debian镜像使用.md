# D1 Debian镜像使用说明
2021年4月28日编写。    
D1 Debain环境是由官方sdk和debian rootfs环境构建而成，本文档只讲解debian镜像的使用环境，及使用方法，debian的rootfs请转移到debian rootfs构建文档！  
## 镜像整体结构与流程
目前由于一些不可控因素，使得当前的D1 debian环境是外挂在sdk环境中的，我们会在以后的版本中构建出原生的D1 debian环境。  
D1 debian镜像环境主要分为两部分。  
第一部分是原厂SDK部分，这部分存放于开发板的flash中，用于linux环境的启动。  
第二部分是debian的rootfs环境，这部分存放于sd卡中，用于debian环境的使用。  
系统的启动流程是首先从flash中引导SDK环境的启动，linux启动完成后运行开机脚本，将挂载到/mnt目录下的sipeed目录。然后挂载相应设备到该根目录下，最后执行chroot命令，完成根目录切换，并初始化debian环境。  
## 镜像各部分的详细介绍  
### SDK镜像
sdk镜像主要负责linux环境的启动，添加了chroot_debian_home脚本，并设定开机启动。在sdk镜像中包含两种调试方式。
第一种是通过串口进行调试。usb转串口接入到D1开发板的DEBUG端口。开机会打印出内核信息，开机完成后能够直接登陆串口终端，对D1的linux系统进行操作。 
在linux下调试可以使用picocom串口终端软件，也可以根据自己喜好使用其他软件。  
> #ubunutu下picocom的安装  
> sudo apt install picocom -y
>     
> #picocom的使用  
> sudo picocom /dev/tty* -b 115200  

串口进入串口终端启动完成后按回车键登陆后显示信息如下： 
~~~
************************
BusyBox v1.27.2 () built-in shell (ash)

 _____  _              __     _
|_   _||_| ___  _ _   |  |   |_| ___  _ _  _ _
  | |   _ |   ||   |  |  |__ | ||   || | ||_'_|
  | |  | || | || _ |  |_____||_||_|_||___||_,_|
  |_|  |_||_|_||_|_|  Tina is Based on OpenWrt!
 ----------------------------------------------
 Tina Linux (Neptune, 5C1C9C53)
 ----------------------------------------------
root@TinaLinux:/# 
~~~

> #note` picocom的退出命令，先按下ctrl键，然后依次按A键松开，再按B键松开就可以退出串口了。详细使用方法请百度picocom`


第二种是通过adb调试的方式进入:  
在系统启动完成之后输入以下命令，即可完成串口终端的登陆。  
> adb shell  

如果出现“未发现设备”，那么稍微等待一会，确认linux系统是否正常启动。   

用于出初始化debian环境的脚本是/etc/init.d/chroot_debian_home,这脚本的功能主要是用于挂载sd卡到/mnt/debian根目录，然后挂系统文件到该目录下，最后切换根目录并初始化debain用户环境。  
### debian环境  
debian环境构建于riscv64环境。
在使用debian环境时主要的初始化脚本是/sbin/os_init。这个脚本主要用于开启ssh，开启X windows服务，以及开启一个vnc用于远程连接。用户可以通过，ssh和vnc来访问使用debian环境。