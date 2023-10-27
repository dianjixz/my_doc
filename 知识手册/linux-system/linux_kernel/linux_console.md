- [配置linux，关闭LCD的console控制台输出。](https://www.cnblogs.com/Rainingday/p/12627285.html)

  内核是nxp官方提供的，基本修改下设备文件，就烧写了。但是运行Qt的时候，会发现左下角有光标闪烁。所以想怎么关闭控制台在LCD输出。取消就完事了。

![](/home/nihao/work/my_doc/image/framebuffer_console.jpg)



- linux下光标消失的问题

问题描述:在开发中遇到需要使用屏幕显示图片的需求,但是内核已经开启了屏幕终端,应为屏幕终端在编译内核的时候就已经开启了,所以暂时没办法去更改内核开关.这个时候可以通过控制终端的光标方法来实现隐藏光标的功能.

~~~ shell
echo -e "\033[?25l" 隐藏光标

echo -e "\033[?25h" 显示光标
#实际使用的命令
echo -e "\033[?25l" > /dev/tty1
~~~

    echo -e "\033[41;36m 红底绿字\033[0m"
    其中41代表字背景色, 36代表字体的颜色
     
    字背景颜色范围:40----47
    40:黑
    41:深红
    42:绿
    43:黄色
    44:蓝色
    45:紫色
    46:深绿
    47:白色
     
    字颜色:30-----------37
    30:黑
    31:红
    32:绿
    33:黄
    34:蓝色
    35:紫色
    36:深绿
    37:白色
     
    ANSI控制码的说明
    \33[0m 关闭所有属性,设置成默认属性
    \33[1m 设置高亮度
    \33[4m 下划线
    \33[5m 闪烁
    \33[7m 反显
    \33[8m 消隐
    \33[30m -- \33[37m 设置前景色
    \33[40m -- \33[47m 设置背景色
    \33[nA 光标上移n行
    \33[nB 光标下移n行
    \33[nC 光标右移n行
    \33[nD 光标左移n行
    \33[y;xH设置光标位置
    \33[2J 清屏
    \33[K 清除从光标到行尾的内容
    \33[s 保存光标位置
    \33[u 恢复光标位置
    \33[?25l 隐藏光标
    \33[?25h 显示光标

> echo 0 > /sys/class/graphics/fbcon/cursor_blink
> #关光标

切换控制台
sudo chvt 2

屏蔽内核所有打印
echo 0 4 0 7 > /proc/sys/kernel/printk


嵌入式终端屏幕显示及输入



由于之前一直通过类似超级终端的串口工具来显示终端设备的信息，有时在使用上存在局限性，所以希望通过终端自身来显示并可输入操作
1. 在uboot里，将CONFIG_BOOTARGS的define加入
console=tty0 console=ttyS1,57600n8
如果不在uboot中的启动参数中键入tty0，则在内核中的输出信息将不显示在屏幕上
2. 在/etc/inittab文件内加入tty0::askfirst:-/bin/sh后，在LCD console上将出现“Please Press Enter to activate this console.”
3. 在2.6.xx Linux内核中，配置(make menuconfig)如下
Device Drivers---->
        Graphics support---->
                <*>Support for frame buffer devices---->
                        <*>Virtual Frame Buffer support(ONLY FOR TESTING!)
                Console display driver support---->
                        <*>Framebuffer Console support
                         ......
然后在系统启动后，执行命令“sh > /dev/tty0”，即进入终端输入模式
如果想返回到串口模式下，则执行命令“sh > /dev/tty”
(其中/dev下tty tty0文件的建立，mknod /dev/tty c 5 0, mknod /dev/tty0 c 4 0)
 
 参考：http://hi.baidu.com/leoxeon/blog/item/0c7326115f9d6dcfa6ef3f4f.html
4. disable console. avoid console blank automatically（否则运行大概10分钟没有按键中断时就会黑屏）
echo -e "\033[9;0]" > /dev/tty0
或通过setterm命令来设置blankinterval：setterm -blank 0
参考：http://blogold.chinaunix.net/u2/66024/showart_2074961.html

另：
tty、串口、console等的区别 
tty是Teletype的缩写
终端是一种字符型设备，它有多种类型，通常使用tty来简称各种类型的终端设备。tty是Teletype的缩写。Teletype是最早出现的一种终端设备，很象电传打字机（或者说就是?），是由Teletype公司生产的。设备名放在特殊文件目录/dev/下，终端特殊设备文件一般有以下几种：
1.串行端口终端（/dev/ttySn）
串行端口终端（Serial Port Terminal）是使用计算机串行端口连接的终端设备。计算机把每个串行端口都看作是一个字符设备。有段时间这些串行端口设备通常被称为终端设备，因为那时它的最大用途就是用来连接终端。这些串行端口所对应的设备名称是/dev/tts/0（或/dev/ttyS0）、/dev/tts/1（或/dev/ttyS1）等，设备号分别是（4,0?）、（4,1?）等，分别对应于DOS系统下的COM1、COM2等。若要向一个端口发送数据，可以在命令行上把标准输出重定向到这些特殊文件名上即可。例如，在命令行提示符下键入：echo test > /dev/ttyS1会把单词”test”发送到连接在ttyS1（COM2）端口的设备上。
2.伪终端（/dev/pty/）
伪终端（Pseudo Terminal）是成对的逻辑终端设备，例如/dev/ptyp3和/dev/ttyp3（或着在设备文件系统中分别是/dev/pty/m3和/dev/pty/s3）。它们与实际物理设备并不直接相关。如果一个程序把ttyp3看作是一个串行端口设备，则它对该端口的读/写操作会反映在该逻辑终端设备对的另一个上面（ttyp3）。而ttyp3则是另一个程序用于读写操作的逻辑设备。这样，两个程序就可以通过这种逻辑设备进行互相交流，而其中一个使用ttyp3的程序则认为自己正在与一个串行端口进行通信。这很象是逻辑设备对之间的管道操作。
对于ttyp3（s3），任何设计成使用一个串行端口设备的程序都可以使用该逻辑设备。但对于使用ptyp3的程序，则需要专门设计来使用ptyp3（m3）逻辑设备。
例如，如果某人在网上使用telnet程序连接到你的计算机上，则telnet程序就可能会开始连接到设备ptyp2（m2）上（一个伪终端端口上）。此时一个getty程序就应该运行在对应的ttyp2（s2）端口上。当telnet从远端获取了一个字符时，该字符就会通过m2、s2传递给getty程序，而getty程序就会通过s2、m2和telnet程序往网络上返回”login:”字符串信息。这样，登录程序与telnet程序就通过“伪终端”进行通信。通过使用适当的软件，就可以把两个甚至多个伪终端设备连接到同一个物理串行端口上。
在使用设备文件系统（device filesystem）之前，为了得到大量的伪终端设备特殊文件，HP-UX AIX等使用了比较复杂的文件名命名方式。
3.控制终端（/dev/tty）？
如果当前进程有控制终端（Controlling Terminal）的话，那么/dev/tty就是当前进程的控制终端的设备特殊文件。可以使用命令”ps –ax”来查看进程与哪个控制终端相连。对于你登录的shell，/dev/tty就是你使用的终端，设备号是（5,0）。使用命令”tty”可以查看它具体对应哪个实际终端设备。/dev/tty有些类似于到实际所使用终端设备的一个联接。
4.虚拟控制台（/dev/ttyn）, /dev/console
在UNIX系统中，计算机显示器通常被称为控制台终端（Console）。它仿真了类型为Linux的一种终端（TERM=Linux），并且有一些设备特殊文件与之相关联：tty0、tty1、tty2等。当你在控制台上登录时，使用的是tty1。使用Alt [F1—F6]组合键时，我们就可以切换到tty2、tty3等上面去。tty1 –tty6等称为虚拟终端，而tty0则是当前所使用虚拟终端的一个别名，系统所产生的信息会发送到该终端上。因此不管当前正在使用哪个虚拟终端，系统信息都会发送到控制台终端上。
你可以登录到不同的虚拟终端上去，因而可以让系统同时有几个不同的会话期存在。只有系统或超级用户root可以向/dev/tty0进行写操作。tty# 是系统视频监视器上全屏显示的终端，用于在不能使用framebuffer设备的机器上存取视频卡，而/dev/console由系统内核管理，接收系统消息。
5.其它类型
还针对很多不同的字符设备存在有很多其它种类的终端设备特殊文件。例如针对ISDN设备的/dev/ttyIn终端设备等。这里不再赘述。
-----------------------------------------------------------------------------------------
PC redhat下
tty           [5,0]     root
console  [5,1]      root
tty#         [4,#]      root
ttyS#    [4,64+#]  uucp 与上文不一致
ttyp#       [3,#]     tty
ptyp#      [2,#]     tty
 
转载：http://blog.csdn.net/bianhonglei/archive/2009/12/03/4932583.aspx
http://blog.csdn.net/liaoxinmeng/archive/2009/12/14/5004743.aspx



http://blog.chinaunix.net/uid-10034083-id-3245122.html