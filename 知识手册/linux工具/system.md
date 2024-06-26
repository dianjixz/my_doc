 

**systemd的特性有：**

支持并行化任务

同时采用socket式与D-Bus总线式激活服务；

按需启动守护进程（daemon）；

利用Linux的cgroups监视进程；

支持快照和系统恢复；

维护挂载点和自动挂载点；

各服务间基于依赖关系进行精密控制。

**一、由来**

历史上Linux的启动一直采用init进程，下面的[命令](https://www.linuxcool.com/)用来启动服务。

```
$ sudo /etc/init.d/apache2 start
#或者
$ service apache2 start
```

这种方法有两个缺点:

一是启动时间长。init进程是串行启动，只有前一个进程启动完，才会启动下一个进程。
二是启动[脚本](https://www.linuxcool.com/)复杂。init进程只是执行启动[脚本](https://www.linuxcool.com/)，不管其他事情。脚本需要自己处理各种情况，这往往使得脚本变得很长。

**二、Systemd概述**

Systemd就是为了解决这些问题而诞生的。它的设计目标是，为系统的启动和管理提供一套完整的解决方案，根据Linux惯例，字母d是守护进程（daemon）的缩写，Systemd这个名字的含义，就是它要守护整个系统。

![Systemd入门教程：命令篇Systemd入门教程：命令篇](https://www.linuxprobe.com/wp-content/uploads/2016/05/systemd_author_Lennart-Poettering.jpg)（上图为Systemd作者Lennart Poettering）

使用了Systemd，就不需要再用init了。Systemd取代了initd，成为系统的第一个进程（PID 等于 1），其他进程都是它的子进程。

```
$ systemctl --version  #命令查看Systemd的版本。
```

Systemd的优点是功能强大，使用方便，缺点是体系庞大，非常复杂。事实上，现在还有很多人反对使用Systemd，理由就是它过于复杂，与操作系统的其他部分强耦合，违反”keep simple, keep stupid”的Unix哲学。

![Systemd入门教程：命令篇Systemd入门教程：命令篇](https://www.linuxprobe.com/wp-content/uploads/2016/05/systemd_framework.png)（上图为Systemd架构图）

**三、系统管理**

Systemd并不是一个命令，而是一组命令，涉及到系统管理的方方面面。

**3.1 systemctl 是Systemd的主命令，用于管理系统**

```
#重启系统
$ sudo systemctl reboot

#关闭系统，切断电源
$ sudo systemctl poweroff

#CPU停止工作
$ sudo systemctl halt

#暂停系统
$ sudo systemctl suspend

#让系统进入冬眠状态
$ sudo systemctl hibernate

#让系统进入交互式休眠状态
$ sudo systemctl hybrid-sleep

#启动进入救援状态（单用户状态）
$ sudo systemctl rescue
```

**3.2 systemd-analyze 命令用于查看启动耗时**

```
#查看启动耗时
systemd-analyze blame
$ systemd-analyze                                                                                       

#查看每个服务的启动耗时
$ systemd-analyze blame

#显示瀑布状的启动过程流
$ systemd-analyze critical-chain

#显示指定服务的启动流
$ systemd-analyze critical-chain atd.service
```

**3.3 hostnamectl 命令用于查看当前主机的信息**

```
#显示当前主机的信息
$ hostnamectl

#设置主机名。
$ sudo hostnamectl set-hostname rhel7
```

**3.4 localectl 命令用于查看本地化设置**

```
#查看本地化设置
$ localectl

#设置本地化参数。
$ sudo localectl set-locale LANG=en_GB.utf8
$ sudo localectl set-keymap en_GB
```

**3.5 timedatectl 命令用于查看当前时区设置**

```
#查看当前时区设置
$ timedatectl

#显示所有可用的时区
$ timedatectl list-timezones                                                                                   

#设置当前时区
$ sudo timedatectl set-timezone America/New_York
$ sudo timedatectl set-time YYYY-MM-DD
$ sudo timedatectl set-time HH:MM:SS
```

**3.6 loginctl 命令用于查看当前登录的用户**

```
#列出当前session
$ loginctl list-sessions

#列出当前登录用户
$ loginctl list-users

#列出显示指定用户的信息
$ loginctl show-user ruanyf
```

**四、Unit**

**4.1 含义**

Systemd可以管理所有系统资源，不同的资源统称为 Unit（单位）,Unit一共分成以下12种。

> Service unit：系统服务
> Target unit：多个Unit构成的一个组
> Device Unit：硬件设备
> Mount Unit：文件系统的挂载点
> Automount Unit：自动挂载点
> Path Unit：文件或路径
> Scope Unit：不是由Systemd启动的外部进程
> Slice Unit：进程组
> Snapshot Unit：Systemd快照，可以切回某个快照
> Socket Unit：进程间通信的socket
> Swap Unit：swap文件
> Timer Unit：定时器

systemctl list-units命令可以查看当前系统的所有Unit。

```
#列出正在运行的Unit
$ systemctl list-units

#列出所有Unit，包括没有找到配置文件的或者启动失败的
$ systemctl list-units --all

#列出所有没有运行的Unit
$ systemctl list-units --all --state=inactive

#列出所有加载失败的Unit
$ systemctl list-units --failed

#列出所有正在运行的、类型为service的Unit
$ systemctl list-units --type=service
```

**4.2 Unit的状态**

systemctl status命令用于查看系统状态和单个Unit的状态。

```
#显示系统状态
$ systemctl status

#显示单个Unit的状态
$ sysystemctl status bluetooth.service

#显示远程主机的某个Unit的状态
$ systemctl -H root@rhel7.example.com status httpd.service
```

除了status命令，systemctl还提供了三个查询状态的简单方法，主要供脚本内部的判断语句使用。

```
#显示某个Unit是否正在运行
$ systemctl is-active application.service

#显示某个Unit是否处于启动失败状态
$ systemctl is-failed application.service

#显示某个Unit服务是否建立了启动链接
$ systemctl is-enabled application.service
```

**4.3 Unit管理**

对于用户来说，最常用的是下面这些命令，用于启动和停止Unit（主要是service）。

```
#立即启动一个服务
$ sudo systemctl start apache.service

#立即停止一个服务
$ sudo systemctl stop apache.service

#重启一个服务
$ sudo systemctl restart apache.service

#杀死一个服务的所有子进程
$ sudo systemctl kill apache.service

#重新加载一个服务的配置文件
$ sudo systemctl reload apache.service

#重载所有修改过的配置文件
$ sudo systemctl daemon-reload

#显示某个 Unit 的所有底层参数
$ systemctl show httpd.service

#显示某个 Unit 的指定属性的值
$ systemctl show -p CPUShares httpd.service

#设置某个 Unit 的指定属性
$ sudo systemctl set-property httpd.service CPUShares=500

#使服务开机启动
sudo sudo systemctl enable apache.service

#关闭服务开机启动
sudo sudo systemctl disable apache.service
```

**4.4 依赖关系**

Unit之间存在依赖关系：A依赖于B，就意味着Systemd在启动A的时候，同时会去启动B。
systemctl list-dependencies命令列出一个Unit的所有依赖。

```
$ systemctl list-dependencies nginx.service
```

上面命令的输出结果之中，有些依赖是Target类型（详见下文），默认不会展开显示。如果要展开Target，就需要使用--all参数。

```
$ systemctl list-dependencies --all nginx.service
```

**五、Unit的配置文件**

**5.1 概述**

每一个Unit都有一个配置文件，告诉Systemd怎么启动这个Unit。
Systemd默认从目录/etc/systemd/system/读取配置文件。但是里面存放的大部分文件都是符号链接，指向目录/usr/lib/systemd/system/，真正的配置文件存放在那个目录。

systemctl enable命令用于在上面两个目录之间，建立符号链接关系。

```
$ sudo systemctl enable clamd@scan.service
#等同于
$ sudo ln -s '/usr/lib/systemd/system/clamd@scan.service' '/etc/systemd/system/multi-user.target.wants/clamd@scan.service'
```

如果配置文件里面设置了开机启动，systemctl enable命令相当于激活开机启动，与之对应的，systemctl disable命令用于在两个目录之间，撤销符号链接关系，相当于撤销开机启动。

```
$ sudo systemctl disable clamd@scan.service
```

配置文件的后缀名，就是该Unit的种类，比如sshd.socket；如果省略，Systemd默认后缀名为.service，所以sshd会被理解成sshd.service。

**5.2 配置文件的状态**

systemctl list-unit-files命令用于列出所有配置文件。

```
#列出所有配置文件
$ systemctl list-unit-files

#列出指定类型的配置文件
$ systemctl list-unit-files --type=service
```

这个命令会输出一个列表。

```
$ systemctl list-unit-files

UNIT FILE              STATE
chronyd.service        enabled
clamd@.service         static
clamd@scan.service     disabled
```

这个列表显示每个配置文件的状态，一共有四种。

> enabled：已建立启动链接
> disabled：没建立启动链接
> static：该配置文件没有[Install]部分（无法执行），只能作为其他配置文件的依赖
> masked：该配置文件被禁止建立启动链接

注意，从配置文件的状态无法看出，该 Unit 是否正在运行。这必须执行前面提到的systemctl status命令。

```
$ systemctl status bluetooth.service
```

一旦修改配置文件，就要让 SystemD 重新加载配置文件，然后重新启动，否则修改不会生效。

```
$ sudo systemctl daemon-reload
$ sudo systemctl restart httpd.service
```

**5.3 配置文件的格式**

配置文件就是普通的文本文件，可以用文本编辑器打开。

```
systemctl cat命令可以查看配置文件的内容。
$ systemctl cat atd.service
[Unit]
Description=ATD daemon

[Service]
Type=forking
ExecStart=/usr/bin/atd

[Install]
WantedBy=multi-user.target
```

从上面的输出可以看到，配置文件分成几个区块。每个区块的第一行，是用方括号表示的区别名，比如[Unit]。注意，配置文件的区块名和字段名，都是大小写敏感的。

每个区块内部是一些等号连接的键值对。

```
[Section]
Directive1=value
Directive2=value
```

注意，键值对的等号两侧不能有空格。

**5.4 配置文件的区块**

**[Unit]区块通常是配置文件的第一个区块，用来定义 Unit 的元数据，以及配置与其他 Unit 的关系。它的主要字段如下。**

> Description：简短描述
> Documentation：文档地址
> Requires：当前Unit依赖的其他Unit，如果它们没有运行，当前Unit会启动失败
> Wants：与当前Unit配合的其他Unit，如果它们没有运行，当前Unit不会启动失败
> BindsTo：与Requires类似，它指定的 Unit 如果退出，会导致当前Unit停止运行
> Before：如果该字段指定的Unit也要启动，那么必须在当前Unit之后启动
> After：如果该字段指定的Unit也要启动，那么必须在当前Unit之前启动
> Conflicts：这里指定的Unit 不能与当前Unit同时运行
> Condition...：当前Unit运行必须满足的条件，否则不会运行
> Assert...：当前Unit运行必须满足的条件，否则会报启动失败

**Install]通常是配置文件的最后一个区块，用来定义如何启动，以及是否开机启动。它的主要字段如下。**

> WantedBy：它的值是一个或多个Target，当前Unit激活时（enable）符号链接会放入/etc/systemd/system目录下面以Target名+.wants后缀构成的子目录中
> RequiredBy：它的值是一个或多个Target，当前Unit激活时，符号链接会放入/etc/systemd/system目录下面以Target 名 + .required后缀构成的子目录中
> Alias：当前Unit 可用于启动的别名
> Also：当前Unit激活（enable）时，会被同时激活的其他Unit

**[Service]区块用来Service的配置，只有Service类型的Unit才有这个区块。它的主要字段如下。**

> Type：定义启动时的进程行为。它有以下几种值。
> Type=simple：默认值，执行ExecStart指定的命令，启动主进程
> Type=forking：以fork方式从父进程创建子进程，创建后父进程会立即退出
> Type=oneshot：一次性进程，Systemd会等当前服务退出，再继续往下执行
> Type=dbus：当前服务通过D-Bus启动
> Type=notify：当前服务启动完毕，会通知Systemd，再继续往下执行
> Type=idle：若有其他任务执行完毕，当前服务才会运行
> ExecStart：启动当前服务的命令
> ExecStartPre：启动当前服务之前执行的命令
> ExecStartPost：启动当前服务之后执行的命令
> ExecReload：重启当前服务时执行的命令
> ExecStop：停止当前服务时执行的命令
> ExecStopPost：停止当其服务之后执行的命令
> RestartSec：自动重启当前服务间隔的秒数
> Restart：定义何种情况Systemd会自动重启当前服务，可能的值包括always（总是重启）、on-success、on-failure、on-abnormal、on-abort、on-watchdog
> TimeoutSec：定义Systemd停止当前服务之前等待的秒数
> Environment：指定环境变量

Unit配置文件的完整字段清单，请参考[官方文档](https://www.freedesktop.org/software/systemd/man/systemd.unit.html)。

**六、Target**

启动计算机的时候，需要启动大量的Unit。如果每一次启动，都要一一写明本次启动需要哪些Unit，显然非常不方便。Systemd的解决方案就是Target。

简单说，Target就是一个Unit组，包含许多相关的Unit。启动某个Target的时候，Systemd就会启动里面所有的Unit。从这个意义上说，Target这个概念类似于“状态点”，启动某个Target就好比启动到某种状态。

传统的init启动模式里面，有RunLevel的概念，跟Target的作用很类似。不同的是，RunLevel是互斥的，不可能多个RunLevel同时启动，但是多个Target可以同时启动。

```
#查看当前系统的所有Target
$ systemctl list-unit-files --type=target

#查看一个 Target 包含的所有 Unit
$ systemctl list-dependencies multi-user.target

#查看启动时的默认 Target
$ systemctl get-default

#设置启动时的默认Target
$ sudo systemctl set-default multi-user.target

#切换Target时，默认不关闭前一个Target启动的进程，
# systemctl isolate命令改变这种行为，
#关闭前一个Target里面所有不属于后一个Target的进程
$ sudo systemctl isolate multi-user.target
```

Target与传统RunLevel的对应关系如下。

```
Traditional runlevel      New target name     Symbolically linked to...

Runlevel 0           |    runlevel0.target -> poweroff.target
Runlevel 1           |    runlevel1.target -> rescue.target
Runlevel 2           |    runlevel2.target -> multi-user.target
Runlevel 3           |    runlevel3.target -> multi-user.target
Runlevel 4           |    runlevel4.target -> multi-user.target
Runlevel 5           |    runlevel5.target -> graphical.target
Runlevel 6           |    runlevel6.target -> reboot.target
```
``` bash
# 删除原来的模式
rm /etc/systemd/system/default.target
# 启动到文本模式 命令行模式
ln -sf /lib/systemd/system/multi-user.target /etc/systemd/system/default.target
# 启动到图形模式
ln -sf /lib/systemd/system/graphical.target /etc/systemd/system/default.target
```




**它与init进程的主要差别如下。**
（1）默认的RunLevel（在/etc/inittab文件设置）现在被默认的Target取代，位置是/etc/systemd/system/default.target，通常符号链接到graphical.target（图形界面）或者multi-user.target（多用户命令行）。

（2）启动脚本的位置，以前是/etc/init.d目录，符号链接到不同的RunLevel目录（比如/etc/rc3.d、/etc/rc5.d等），现在则存放在/lib/systemd/system和/etc/systemd/system目录。

（3）配置文件的位置，以前init进程的配置文件是/etc/inittab，各种服务的配置文件存放在/etc/sysconfig目录。现在的配置文件主要存放在/lib/systemd目录，在/etc/systemd目录里面的修改可以覆盖原始设置。

**七、日志管理**

Systemd统一管理所有Unit的启动日志。带来的好处就是，可以只用journalctl一个命令，查看所有日志（内核日志和应用日志）。日志的配置文件是/etc/systemd/journald.conf。

journalctl功能强大，用法非常多。

```
#查看所有日志（默认情况下 ，只保存本次启动的日志）
$ sudo journalctl

#查看内核日志（不显示应用日志）
$ sudo journalctl -k

#查看系统本次启动的日志
$ sudo journalctl -b
$ sudo journalctl -b -0

#查看上一次启动的日志（需更改设置）
$ sudo journalctl -b -1

#查看指定时间的日志
$ sudo journalctl --since="2012-10-30 18:17:16"
$ sudo journalctl --since "20 min ago"
$ sudo journalctl --since yesterday
$ sudo journalctl --since "2015-01-10" --until "2015-01-11 03:00"
$ sudo journalctl --since 09:00 --until "1 hour ago"

#显示尾部的最新10行日志
$ sudo journalctl -n

#显示尾部指定行数的日志
$ sudo journalctl -n 20

#实时滚动显示最新日志
$ sudo journalctl -f

#查看指定服务的日志
$ sudo journalctl /usr/lib/systemd/systemd

#查看指定进程的日志
$ sudo journalctl _PID=1

#查看某个路径的脚本的日志
$ sudo journalctl /usr/bin/bash

#查看指定用户的日志
$ sudo journalctl _UID=33 --since today

#查看某个 Unit 的日志
$ sudo journalctl -u nginx.service
$ sudo journalctl -u nginx.service --since today

#实时滚动显示某个 Unit 的最新日志
$ sudo journalctl -u nginx.service -f

#合并显示多个 Unit 的日志
$ journalctl -u nginx.service -u php-fpm.service --since today

#查看指定优先级（及其以上级别）的日志，共有8级
# 0: emerg
# 1: alert
# 2: crit
# 3: err
# 4: warning
# 5: notice
# 6: info
# 7: debug
$ sudo journalctl -p err -b

#日志默认分页输出，--no-pager 改为正常的标准输出
$ sudo journalctl --no-pager

#以 JSON 格式（单行）输出
$ sudo journalctl -b -u nginx.service -o json

#以 JSON 格式（多行）输出，可读性更好
$ sudo journalctl -b -u nginx.serviceqq
 -o json-pretty

#显示日志占据的硬盘空间
$ sudo journalctl --disk-usage

#指定日志文件占据的最大空间
$ sudo journalctl --vacuum-size=1G

#指定日志文件保存多久
$ sudo journalctl --vacuum-time=1years
```

> 原文来自：http://www.ruanyifeng.com/blog/2016/03/systemd-tutorial-commands.htm
>
> 转载地址：http://www.codeceo.com/article/systemd-command.html
>
> 本文地址：https://www.linuxprobe.com/systemd-command.html编辑：岳永，审核员：冯振华

本文原创地址：https://www.linuxprobe.com/systemd-command.html编辑：public，审核员：暂无





# systemd系统开机运行rc.local

在使用较新版本的`ubuntu`系统中发现，原来写入`/etc/rc.local`文件中的开机命令不执行了?经过一番研究，发现原来`ubuntu`自`16.０４`版本后就不再使用`initd`管理系统，而改用`systemd`

修复问题
默认的service文件都是存在与/etc/systemd/system目录下，有点像某种服务的配置文件。注意到/lib/systemd/system下也有个rc-local.service，我们借用这个模板来进行修改，当然你也可以从头开始编写

sudo cp /lib/systemd/system/rc-local.service /etc/systemd/system/rc-local.service
1
修改内容如下，主要是添加Install字段信息
~~~ bash
#  SPDX-License-Identifier: LGPL-2.1+
#
#  This file is part of systemd.
#
#  systemd is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.

# This unit gets pulled automatically into multi-user.target by
# systemd-rc-local-generator if /etc/rc.local is executable.
[Unit]
Description=/etc/rc.local Compatibility
Documentation=man:systemd-rc-local-generator(8)
ConditionFileIsExecutable=/etc/rc.local
After=network.target

[Service]
Type=forking
ExecStart=/etc/rc.local start
TimeoutSec=0
RemainAfterExit=yes
GuessMainPID=no

[Install]
WantedBy=multi-user.target
~~~
其中Unit字段主要描述服务的启动顺序以及依赖关系，Service字段主要描述如何启动，Install字段描述如何安装这个服务。ubuntu 18.04系统默认已经将/etc/rc.local文件移除了，因此，我们需要手动创建一个，并将需要开机执行的命令写入到文件中，如
~~~ bash
#!/bin/bash

/usr/bin/sslocal -c /home/xugaoxiang/Tools/ss/ss.json &
~~~
同样的，别忘了，给/etc/rc.local加上可执行的权限
~~~ bash
sudo chmod a+x /etc/rc.local
然后执行
​~~~ bash
xugaoxiang@ubuntu:~$ sudo systemctl enable rc-local
Created symlink /etc/systemd/system/multi-user.target.wants/rc-local.service → /etc/systemd/system/rc-local.service.
接着启动这个服务并查看它的状态

sudo systemctl start rc-local.service
sudo systemctl status rc-local.service
~~~
命令输出如下
~~~ bash
● rc-local.service - /etc/rc.local Compatibility
   Loaded: loaded (/etc/systemd/system/rc-local.service; enabled; vendor preset: enabled)
  Drop-In: /lib/systemd/system/rc-local.service.d
           └─debian.conf
   Active: active (running) since Thu 2018-11-01 13:17:08 CST; 2s ago
     Docs: man:systemd-rc-local-generator(8)
  Process: 10810 ExecStart=/etc/rc.local start (code=exited, status=0/SUCCESS)
    Tasks: 1 (limit: 4915)
   CGroup: /system.slice/rc-local.service
           └─10811 /usr/bin/python /usr/bin/sslocal -c /home/xugaoxiang/Tools/ss/ss.json

11月 01 13:17:08 ubuntu systemd[1]: Starting /etc/rc.local Compatibility...
11月 01 13:17:08 ubuntu systemd[1]: Started /etc/rc.local Compatibility.
11月 01 13:17:08 ubuntu rc.local[10810]: INFO: loading config from /home/xugaoxiang/Tools/ss/ss.json
11月 01 13:17:08 ubuntu rc.local[10810]: 2018-11-01 13:17:08 INFO     loading libcrypto from libcrypto.so.1.1
11月 01 13:17:08 ubuntu rc.local[10810]: 2018-11-01 13:17:08 INFO     starting local at 127.0.0.1:1080
~~~
可以看到rc.local中的脚本已经被正确执行了。
————————————————
版权声明：本文为CSDN博主「迷途小书童的Note」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/djstavaV/article/details/88166805





systemd path：实时监控文件和目录的变动
systemd path工具提供了监控文件、目录变化并触发执行指定操作的功能。

有时候这种监控功能是非常实用的，比如监控到/etc/nginx/nginx.conf或/etc/nginx/conf.d/发生变化后，立即reload nginx。虽然，用户也可以使用inotify类的工具来监控，但远不如systemd path更方便、更简单且更易于观察监控效果和调试。

其实，systemd path的底层使用的是inotify，所以受限于inotify的缺陷，systemd path只能监控本地文件系统，而无法监控网络文件系统。

systemd path能监控哪些操作
systemd path暴露的监控功能并不多，它能监控的动作包括：



这些指令监控的路径必须是绝对路径。

可以多次使用这些指令，且同一个指令也可以使用多次，这样就能够同时监控多个文件或目录，它们将共用事件触发后执行的操作。如果想要对不同监控目录执行不同操作，那只能定义多个systemd path的监控实例。

如果监控某路径时发现权限不足，则一直等待，直到有权监控。

如果在启动Path Unit时(systemctl start xxx.path)，指定的路径已经存在(对于PathExists与PathExistsGlob来说)或者指定的目录非空(对于DirectoryNotEmpty来说)，将会立即触发并执行对应操作。不过，对于PathChanged与PathModified来说，并不遵守这个规则。

systemd path使用示例
要使用systemd path的功能，需至少编写两个文件，一个.path文件和一个.service文件，这两个文件的前缀名称通常保持一致，但并非必须。这两个文件可以位于以下路径：

/usr/lib/systemd/system/
/etc/systemd/system/
~/.config/systemd/user/：用户级监控，只在该用户登录后才监控，该用户所有会话都退出后停止监控
例如：

~~~
/usr/lib/systemd/system/test.path
/usr/lib/systemd/system/test.service


/etc/systemd/system/test.path
/etc/systemd/system/test.service

~/.config/systemd/user/test.path
~/.config/systemd/user/test.service

~~~



例如，有以下监控需求：

监控/tmp/foo目录下的所有文件修改、创建、删除等操作
如果被监控目录/tmp/foo不存在，则创建
监控/tmp/a.log文件的更改
监控/tmp/file.lock锁文件是否存在
为了简化，这些监控触发的事件都执行同一个操作：向/tmp/path.log中写入一行信息。

此处将path_test.path文件和path_test.service文件放在/etc/systemd/system/目录下。

path_test.path内容如下：


~~~
$ cat /etc/systemd/system/path_test.path
[Unit]
Description = monitor some files

[Path]
PathChanged = /tmp/foo
PathModified = /tmp/a.log
PathExists = /tmp/file.lock
MakeDirectory = yes
Unit = path_test.service

# 如果不需要开机后就自动启动监控的话，可省略下面这段
# 如果开机就监控，则加上这段，并执行systemctl enable path_test.path
[Install]
WantedBy = multi-user.target


~~~

其中MakeDirectory指令默认为no，当设置为yes时表示如果监控的目录不存在，则自动创建目录，但该指令对PathExists指令无效。

Unit指令表示该sysmted path实例监控到符合条件的事件时启动的服务单元，即要执行的对应操作。通常省略该指令，这时启动的服务名称和path实例的名称一致(除了后缀)，例如path_test.path默认启动的是path_test.service服务。

path_test.service内容如下：


~~~
$ cat /etc/systemd/system/path_test.service
[Unit]
Description = path_test.service

[Service]
ExecStart = /bin/bash -c 'echo file changed >>/tmp/path.log'
然后执行如下操作启动该systemd path实例：


~~~


~~~
systemctl daemon-reload
systemctl start path_test.path


~~~

使用如下命令可以列出当前已启动的所有systemd path实例：


~~~
$ systemctl --type=path list-units --no-pager
UNIT                               LOAD   ACTIVE SUB     DESCRIPTION                              
systemd-ask-password-console.path  loaded active waiting Dispatch Password Requests to Console
systemd-ask-password-wall.path     loaded active waiting Forward Password Requests to Wall Dir
path_test.path                     loaded active waiting monitor some files


~~~
然后测试该systemd path能否如愿工作。


~~~
$ touch /tmp/foo/a
$ touch /tmp/foo/a
$ touch /tmp/a.log
$ echo 'hello world' >>/tmp/a.log
$ rm -rf /tmp/a.log

~~~
...
如果想观察触发情况，可使用journalctl。例如：


~~~
$ journalctl -u path_test.service
Jul 05 16:09:43 junmajinlong.com systemd[1]: Started path_test.service.
Jul 05 16:09:45 junmajinlong.com systemd[1]: Started path_test.service.
Jul 05 16:09:47 junmajinlong.com systemd[1]: Started path_test.service.
Jul 05 16:09:49 junmajinlong.com systemd[1]: Started path_test.service.
Jul 05 16:09:51 junmajinlong.com systemd[1]: Started path_test.service.
Jul 05 16:09:55 junmajinlong.com systemd[1]: Started path_test.service.

~~~
systemd path临时监控
使用systemd-run命令可以临时监控路径。


~~~
$ systemd-run --path-property=PathModified=/tmp/b.log echo 'file changed'
Running path as unit: run-rb6f67e732fb243c7b530673cac867582.path
Will run service as unit: run-rb6f67e732fb243c7b530673cac867582.service


~~~
可以查看当前已启动的systemd path实例，包括临时监控实例：


~~~
$ systemctl --type=path list-units --no-pager

~~~
如果需要停止，使用run-xxxxxx名称即可：


~~~
systemctl stop run-rb6f67e732fb243c7b530673cac867582.path

~~~
systemd path资源控制
systemd path触发的任务可能会消耗大量资源，比如执行rsync的定时任务、执行数据库备份的定时任务，等等，它们可能会消耗网络带宽，消耗IO带宽，消耗CPU等资源。

想要控制这些定时任务的资源使用量也非常简单，因为真正执行任务的是.service，而Service配置文件中可以轻松地配置一些资源控制指令或直接使用Slice定义的CGroup。这些资源控制类的指令可参考man systemd.resource-control。

例如，直接在[Service]中定义资源控制指令：


~~~
[Service]
Type=simple
MemoryLimit=20M
ExecStart=/usr/bin/backup.sh

~~~
又或者让Service使用定义好的Slice：


~~~
[Service]
ExecStart=/usr/bin/backup.sh
Slice=backup.slice

~~~
其中backup.slice的内容为：


~~~
$ cat /usr/lib/systemd/system/backup.slice
[Unit]
Description=Limited resources Slice
DefaultDependencies=no
Before=slices.target

[Slice]
CPUQuota=50%
MemoryLimit=20M
systemd path的『Bug』
systemd path监控路径上所产生的事件是需要时间的，如果两个事件发生时的时间间隔太短，systemd path可能会丢失第二个甚至后续第三个第四个等等事件。

~~~
例如，使用PathChanged或PathModified监控路径/tmp/foo目录时，执行以下操作触发事件：


~~~
$ touch /tmp/foo/a && rm -rf /tmp/foo/a

~~~
期待的是systemd path能够捕获这两个事件并执行两次对应的操作，但实际上只会执行一次对应操作。换句话说，systemd path丢失了一次事件。

之所以会丢失事件，是因为touch产生的事件被systemd path捕获，systemd path立即启动对应.service服务做出对应操作，在本次操作还未执行完时，rm又立即产生了新的事件，于是systemd path再次启动服务，但此时服务尚未退出，所以本次启动服务实际上什么事也不做。

所以，从结果上看去就像是systemd path丢失了事件，但实际上是因为服务尚未退出的情况下再次启动服务不会做任何事情。

可以加上一点休眠时间来耽搁一会：


~~~
$ touch /tmp/foo/a && sleep 0.1 && rm -rf /tmp/foo/a

~~~
上面的命令会成功执行两次对应操作。

再比如，将.service文件中的ExecStart设置为/usr/bin/sleep 5，那么在5秒内的所有操作，除了第一次触发的事件外，其它都会丢失。

systemd path的这个『bug』也有好处，因为可以让瞬间产生的多个有关联关系的事件只执行单次任务，从而避免了中间过程产生的事件也重复触发相关操作。



作者: 骏马金龙
链接: https://www.junmajinlong.com/linux/systemd/systemd_path/
来源: 骏马金龙
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



# 另一个开机启动的设置方式
设置自启动Service

1、创建启动脚本
使用任何文本编辑器，在/etc/init.d目录下创建一个新的启动脚本，假设命名为your_script_name，以下是示例脚本的参考内容：
```bash
#!/bin/bash

### BEGIN INIT INFO
# Provides:          your_service_name
# Required-Start:    $all
# Required-Stop:     
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start your_service_name at boot time
# Description:       Enable service provided by your_service_name
### END INIT INFO

/path/to/your/program &

exit 0
```

2、设置启动脚本具有可执行权限
```bash
sudo chmod +x /etc/init.d/your_script_name
```
3、使用update-rc.d命令将脚本添加到系统的启动项中
```bash
sudo update-rc.d your_script_name defaults

```
4、使用systemctl命令启用自启动
```bash
sudo systemctl enable your_script_name
```
5、重启开发板验证自启动服务程序是否运行正常
```bash
root@ubuntu:~# systemctl status your_script_name.service 
● your_script_name.service - LSB: Start your_service_name at boot time
    Loaded: loaded (/etc/init.d/your_script_name; generated)
    Active: active (exited) since Wed 2023-04-19 15:01:12 CST; 57s ago
    Docs: man:systemd-sysv-generator(8)
    Process: 2768 ExecStart=/etc/init.d/your_script_name start (code=exited, status=0/SUCCESS)
```



```txt
[Unit]
Description=My Application

[Service]
ExecStart=/path/to/your/application
Restart=always
RestartSec=30
User=your_username

[Install]
WantedBy=multi-user.target

```