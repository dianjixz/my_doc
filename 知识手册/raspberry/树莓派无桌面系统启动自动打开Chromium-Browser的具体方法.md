

https://blog.csdn.net/sinat_36939362/article/details/95391676

RaspberryPI Lite 自动打开Chromium-Browser
情景：需要用电视机通过网页显示一些数据
需要到的工具:
前期准备步骤：
在RPI Lite安装相应的Package
完善功能
解决Chromium中文乱码的问题
光标隐藏代码
Lite需要满屏显示
写批处理的文档方便系统启动执行
开机自动执行指定的代码
以PI自动登录系统
指定固定的IP地址
情景：需要用电视机通过网页显示一些数据
构想方向
我原来是想用智能电视机或者通过电脑去打开浏览器去自动登录一些网页，从成本看肯定是用智能电视机好，但电视机的可控性很差，比如开机自动打开浏览器等的操作就很难完成 。而且非常多的广告，这也不便于当作生产线的显示板使用。
如果用电脑+电视机的组合也可以，可操控性有了，但成本也上来了，每台电脑成本估计在1千元左右，而且耐用性和管理性并不好。
经过网上收集的资料，最后决定用Raspberry 3B作为主机+电视的组合，成本也低，Linux的稳定性也比Windows好。RaspberryPI有桌面程序，但想做到开机立即可进入页面的效果，通过查询到的网上资料都参支不齐，经过反复的实验，自己整理了些资料，也方便以后自己去查询。

需要到的工具:
SD Card Formatter (新的SD卡需要格式化的工具)
Win32DiskImager (把下载的RPI系统写进SD卡的工具)
Raspbian Buster Lite (RPI Lite 官方的下载地址是: https://www.raspberrypi.org/downloads/raspbian/)
Putty (SSH远端操作工具，这个不是必要，但很多时候使用会方便，特别是直接树莓派键盘的布局与平常的布局是有一点点不同，比如"#"就死搞不出来，在Putty写Script不会有这个问题。
前期准备步骤：
格式化后的SD卡，需要用Win32DiskImager写入下载下来的镜像(这里所用的是Raspbian Buster Lite命令行式的界面，简单讲就是MicroSoft的Dos系统)，过程视乎于SD卡的速度，据网上所讲，RaspbreeyPi的运行速度取决于SD卡的速度，因为所有的读取写入都与SD卡有关。
把SD插入树莓派3B的SD卡槽，接上网络(接入的网络必须可以上网，一般是通过路由器中DHCP分派IP，这样会简单一些。初手不建议在网络环境太复杂中操作，如是则需要上网查如何在树莓派中设定IP地址/网关/DNS等)。
如果有条件可以接入HDMI,直接通过显示器去操作，这样可以省下用SSH去无端操作。但我还是用Putty去远程操作树莓派，所以首先想办法知道DHCP分派了什么IP给树莓派。在这之前有一点非常重要，新版的RPI默认是不允许SSH登录的，需要手动在SD卡的根目录中建立目录并命名为SSH,如果没有这个目录，SSH连接会失败！可以在第1点写完系统入SD卡后随便建立目录
接电开启树莓派
在RPI Lite安装相应的Package
我第一篇参照的文章
链接: Installing Chromium on Raspberry Pi without desktop
```bash
sudo apt-get install chromium-browser 安装Chromium-browser of raspberry
sudo apt-get install xserver-xorg 安装XServer,Chromium应该是在这个基础上运行的
sudo apt-get install xinit 这个是启动Chromium的一个组件
```
安装以上三个包后可以通过以下命令运行Chromium-browser,你会发现系统会先运行Xserver,接着再运行Browser,这个过程并不友好。
```bash
xinit chromium-browser
```
接下来我们安装matchbox-window-manager,具体什么作用我不太清楚，应该是管理窗口。
```bash
sudo apt-get install matchbox-window-manager
```
完善功能
从这里开始关键的问题已经解决，接下来就是优化问题，要解决的问题有以下几点：

解决Chromium中文乱码的问题
打开浏览器后鼠标的图标要隐藏。
Lite需要满屏显示
需要写一个批处理的文档去执行相关的操作
开机运行指定代码
自动登录
在树莓派中如何使用固定的静态IP地址
解决Chromium中文乱码的问题
加装中文字库
```bash
sudo apt-get update​
sudo apt-get upgrade -y
```

*****我认为以上两行代码不一定需要的*****
```bash
sudo apt-get install ttf-wqy-microhei​
sudo dpkg-reconfigure locales
sudo reboot
```

添加zh_CN.UTF-8,设定en_GB.UTF-8为默认语言
完成后需要重启

光标隐藏代码
```bash
sudo apt-get install unclutter​​
```
安装相应的Package后再Sudo Reboot，重启后再执行unclutter

Lite需要满屏显示
修改启动配置文件
sudo nano /boot/config.txt

找到hdmi_mode=1，把注释去掉，即是前面的#号，在这行上面，有着各种模式与分辨率，找到适合自己屏幕的，比如1080P的模式是82，就把1改为82（hdmi_mode=1改为hdmi_mode=82）
这里有个，【链接】关于树莓派连接HDMI显示器及配置方法，内有详细关于不同分辨率的说明。
找到hdmi_group=1，把注销去掉然后把1改为2（1是类似电视的输出，2是显示器输出）
#overscan_left=0
#overscan_right=0
#overscan_top=0
#overscan_bottom=0
把上面的#都删掉
Ctrl + X，Y，Enter保存，sudo Reboot重启系统
写批处理的文档方便系统启动执行
sudo nano start-chromium.sh会在系统默认的路径/home/pi/下生成start-chromium.sh文件
```bash
#禁止显示器休眠
xset dpms 0 0 0
xset s off
##Hide the mouse from the display
unclutter &
##If Chrome crashes (usually due to rebooting), clear the crash flag so we don't have the annoying warning bar
##Linux sed的命令使用可以参考网上的资料，主要用来自动编辑一个或多个文件、简化对文件的反复操作、编写转换程序等。
sed -i 's/"exited_cleanly":false/"exited_cleanly":true/' /home/pi/.config/chromium/Default/Preferences
sed -i 's/"exit_type":"Crashed"/"exit_type":"Normal"/' /home/pi/.config/chromium/Default/Preferences
##删除chrome临时目录
rm -r /home/pi/.config/chromium -f

matchbox-window-manager -use_cursor no -use_titlebar no &
chromium-browser --disable-features=TranslateUI --window-size=1920,1080 --fast --fast-start --disable-popup-blocking --disable-infobars --start-fullscreen --incognito --disable-session-crashed-bubble --allow-running-insecure-content https://yoururl/
```

以下解释一下Chromium-browser的启动参数
--disable-features=TranslateUI不显示翻译此页面的窗口提示
--window-size=1920,1080 满屏的尺寸，这个参数很重要，因为在Lite在显示屏默认不为全屏，这个参数对应启动设定的屏幕尺寸
--fast
--fast-start If this flag is present then this command line is being delegated to an already running chrome process via the fast path, ie: before chrome.dll is loaded. It is useful to tell the difference for tracking purposes.
--disable-popup-blocking禁用弹出拦截
--disable-desktop-notifications 禁用桌面通知，在 Windows 中桌面通知默认是启用的。
--disable-infobars让chorme启动后不提示黄色边框。
--start-fullscreen全屏模式，可以退出
--incognito 让浏览器直接以隐身模式启动。
--disable-session-crashed-bubbleDisables using bubbles for session restore request.
--allow-running-insecure-content 默认情况下，https 页面不允许从 http 链接引用 javascript/css/plug-ins。添加这一参数会放行这些内容。

备选参数
--touch-events=enabled
--kiosk启用kiosk模式。（一种类似于全屏的浏览模式）

开机自动执行指定的代码
试过linux系统常用的在rc.local的exit(0)前写shell命令的方法，但是没有奏效。百度后有种说法是rc.local是在系统启动后、用户登录前运行的，这时显然无法运行start-chromium.sh。另一种方法是写开机脚本，然而笔者没有学过shell脚本，因此用了修改.profile的方法。登录后，系统会自动运行./profile，于是就实现了开机自启动start-chromium.sh脚本代码。
sudo nano .profile
在打开的文档后面增加你要增加一句（nano的用法比vi简单，这里就不赘述了）
export DISPLAY=:0 && sudo xinit /home/pi/start-chromium.sh &
其中/home/pi/file是要启动的文件的路径，加入“&”可以防止阻塞。
注意这里与shell命令不同，不是以往的sudo xinit ./home/pi/start-chromium.sh，这里是没有“.”的。 然后，程序就能开机运行了

以PI自动登录系统
Raspberry PI 旧版本的方法
修改/etc/inittab文件 sudo nano /etc/inittab 向下滚动，找到行 1:2345:respawn:/sbin/getty 115200 tty1 注释掉这一行，修改为 #1:2345:respawn:/sbin/getty 115200 tty1 在该行下增加一行内容1:2345:respawn:/bin/login -f pi tty1 </dev/tty1 >/dev/tty1 2>&1
按Ctrl + X，Y，Enter保存。
原理：linux启动时，会读取inittab文件，执行其中的登录命令，以pi用户名自动登录。

基于新版本Raspberry PI
Raspian-Image (jessie). This one is based on Debian 8 (jessie) where the init-system changed from sysvinit to systemd.
sudo raspi-config
Alternatively, from the Raspbian Jessie desktop, choose Menu → Preferences → Raspberry PiConfiguration.

If you launched raspi-config from command prompt it looks like below:
Scroll down to 3 Boot Options.
Select option B1 Desktop / CLI

Select B2 Console AutoLogin

Save and exit the configuration and reboot.

The Raspberry Pi will now boot to console everytime. You may setup a script to run at startup if you are planning to operate pi headlessly. To do this, edit /etc/profile. Each time a login shell is spawned all commands in /etc/profile are executed.

sudo nano /etc/profile

To execute a script /home/pi/myscript.sh, add the following line
```bash
./home/pi/myscript.sh
```
Now everytime pi is powered on, it will autologin to pi user and execute your startup script, which presumably will take care of everything you it to do in headless mode.

指定固定的IP地址
```bash
vi /etc/dhcpcd.conf

# 使用 vi 编辑文件，增加下列配置项

# 指定接口 eth0
interface eth0
# 指定静态IP，/24表示子网掩码为 255.255.255.0
static ip_address=192.168.1.20/24
# 路由器/网关IP地址
static routers=192.168.1.1
# 手动自定义DNS服务器
static domain_name_servers=114.114.114.114

# 修改完成后，按esc键后输入 :wq 保存。
```
重启树莓派就生效了

sudo reboot
————————————————

版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。

原文链接：https://blog.csdn.net/sinat_36939362/article/details/95391676
