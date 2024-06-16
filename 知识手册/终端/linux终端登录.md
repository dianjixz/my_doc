 

getty(get teletypewriter)

功能说明：是Unix类操作系统启动时必须的三个步骤之一，用来开启终端，进行终端的初始化，设置终端。

语　　法：getty [-h][-d][-r][-t][-w][终端机编号][连线速率] 或 getty [-c]

补充说明：getty指令是UNIX之类操作系统启动时所必须的3个步骤之一。

getty 命令设置和管理终端线路和端口。getty 命令由 init 命令来运行。getty

命令与终端状态管理员程序相链接。终端状态管理员程序提供了终端控制和登录的复合功能。

注意：getty 命令不在命令行输入。

当作为 getty 命令调用时，终端状态管理员程序提供了通常的端口管理功能，这包括 :

双向使用 允许使用终端线路用来启动和接受连接。

线路速度 设置发送和接收的波特率。

奇偶性 将奇偶性设置为偶数、奇数或无。

延迟 设置回车、制表符、换行以及换页延迟。

字符集映射 为大小写、制表符和换行控制设置字符集映射。

登录器程序 指定用于使用户登录到系统的程序。如果设置了此属性，则禁用安全注意密钥(SAK)的处理功能。如果没有设置此属性，则其缺省值为

/usr/sbin/login 。logger属性包含在对象数据管理员(ODM)数据库中。

字符和行擦除 设置用于字符和行擦除的击键。

回应方式 设置本地或远程回应。

当调用 getty 命令时，会发生下列步骤：

根据 ODM 数据库中的 所有者和保护属性来设置端口保护。如果没有指定这些属性,则其缺省值为 root 用户和

622。

打开由端口名参数指定的端口。如果在端口可以进行载波检测，则直到载波出现或其它的进程已丢失此端口的载波，打开才完成。

指定的端口可能会被锁定。如果 getty 命令带 -u 或 -r

标志运行，它会试图锁定端口。如果锁定了端口，此命令将一直等待端口可用为止，继而退出。如果指定了 -r 标志，getty

命令将等待从端口接收一个字节的数据，然后继续。

根据所指定端口的配置信息设置终端属性。此时能否启用安全注意密钥处理就在于系统的配置如何。

载波消息被写入到指定的端口。

从指定端口读取登录名。如果发生帧错误或中断，则 getty

命令将使用下一组配置的终端属性重复第四和第五步。这是循环调制解调器波特率最常使用的方法。但是可以通过在 ODM

数据库中输入以逗号隔开的值来循环任何 ODM 字段(除了 logmodes和 runmodes)。

按照 runmodes 参数和登录名称复位终端方式。如果登录名称由新行终止，则 getty

命令打开回车符到新行的映射。如果所有的字母字符都是大写，如果可能的话，提示用户使用小写字符登录，并打开从小写到大写的映。

如果程序由登录器参数指定，则会执行它且禁用安全注意密钥处理。否则，“终端状态管理器”程序执行标准系统登录。

注：如果用户在登录时输入了 Sequence Attention Key 序列，则用户登录的是可信的

shell(如果系统经配置，端口安全可信，允许用户从可信路径登录)。

标志

-d 提供调试信息。

-H HeraldString 指定备用预告消息以在端口写入提示的登录名。

消息字符串必须是一个字而且不能包含任何空格。 该字符串将优先于在 /etc/security/login.cfg

文件中定义的预告消息。如果 没有字符串以该选项或在 login.cfg 文件中指定， 则使用来自于消息编目的缺省预告。

-M motdFile 指定到每日文件的备用消息的路径。如果没有指定， 在缺省情况下该值为

/etc/motd。

-N 使 getty 绕过在 /etc/utmp 文件中任何进程标识的检查。这 允许不同于最低登录

shell 的进程来执行 getty。

-r 将端口设为可共享(双向)使用。如果锁定不成功，则 getty

命令一直等待至锁可用为止，然后退出。如果锁定成功, getty 命令将在锁定端口后等待从端口接收一字节的数据。

-u 将端口设为可共享(双向)使用。如果锁定不成功，getty

命令将一直等待锁定为止，然后退出。

-U 除了 getty 不会等待锁可用之外，与 -u

标志相同。这使得端口可用，而无需顾虑锁。

安全性

访问控制：应该如可信计算库中的程序一样安装本程序，任何用户和到根的 setuid 都可执行。

例子

如果想要能够登录 tty0, 请将下一行添加到 /etc/inittab 文件中:

tty0:2:respawn: /usr/sbin/getty

/dev/tty0

此命令初始化端口/dev/tty0 并设置端口特征。

范例2： 开启终端

[root@ubuntu ~]# getty tty7

文件

/usr/sbin/getty 包含 getty 命令。

/etc/locks 包含阻止多重使用通信设备和多重调用远程系统的锁文件。

/usr/sbin/login login 命令。

/etc/security/login.cfg 包含端口登录配置。

/etc/motd 包含登录后显示的日消息。

/usr/bin/setmaps setmaps 命令。

/etc/utmp 包含有关用户登录系统的信息。

相关信息

login 命令、setgroups 命令、shell 命令、su 命令、telinit or init 命令、tsm 命令。

http://www.linuxso.com/command/getty.html
————————————————
版权声明：本文为CSDN博主「weixin_39522927」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_39522927/article/details/112923683





现在的tty的意思逐渐变成了text terminal。一个getty进程只支持一个terminal。在一些系统上，如[Solaris](https://zh.wikipedia.org/wiki/Solaris)，并不用getty而用ttymon。

Alternatives include:

- **mingetty** — A minimal getty which allows automatic logins.



- **fbgetty** — A console getty like mingetty, which supports framebuffers.



- **mgetty** — A versatile program to handle all aspects of a modem under Unix.



## Add additional virtual consoles

Open the file `/etc/systemd/logind.conf` and set the option **NAutoVTs=6** to the number of virtual terminals that you want at boot.

If needed, it is possible to temporarily [start](https://wiki.archlinux.org/title/Start) a `getty@tty*N*.service` service directly.



https://wiki.archlinux.org/title/getty



给其他串口登录的方法:

getty -L ttyS3 115200 vt100
agetty -L ttyUSB0 115200 vt100 -n -l /bin/bash
sudo bash -c "while [ 1 ] ; do getty -L ttyAMA1 115200 vt100 >> /dev/null 2>&1 ; done" &