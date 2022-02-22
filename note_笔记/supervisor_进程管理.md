目录

- Linux进程管理工具Supervisor的安装配置
  - [简介](https://www.cnblogs.com/xiaoyafei/p/14441875.html#简介)
  - [安装Python包管理工具](https://www.cnblogs.com/xiaoyafei/p/14441875.html#安装python包管理工具)
  - [安装Supervisor](https://www.cnblogs.com/xiaoyafei/p/14441875.html#安装supervisor)
  - [配置](https://www.cnblogs.com/xiaoyafei/p/14441875.html#配置)
  - [配置文件参数说明](https://www.cnblogs.com/xiaoyafei/p/14441875.html#配置文件参数说明)
  - [配置进程管理](https://www.cnblogs.com/xiaoyafei/p/14441875.html#配置进程管理)
  - [启动supervisor](https://www.cnblogs.com/xiaoyafei/p/14441875.html#启动supervisor)
  - 控制进程
    - [交互终端](https://www.cnblogs.com/xiaoyafei/p/14441875.html#交互终端)
    - [bash终端](https://www.cnblogs.com/xiaoyafei/p/14441875.html#bash终端)
    - [web管理界面](https://www.cnblogs.com/xiaoyafei/p/14441875.html#web管理界面)
  - 配置开启自启动supervisor
    - [配置systemctl服务](https://www.cnblogs.com/xiaoyafei/p/14441875.html#配置systemctl服务)
    - [配置service类型](https://www.cnblogs.com/xiaoyafei/p/14441875.html#配置service类型)



# Linux进程管理工具Supervisor的安装配置

## 简介

​	`Supervisor`是用Python开发的一个Client/Server服务，是Linux/Unix系统下的一个进程管理工具，不支持Windows系统。它可以很方便的监听、启动、停止、重启一个或者多个进程。用Supervisor管理的进程，当一个进程意外被杀死，Supervisor监听到进程死亡后，会自动将它重新拉起，很方便的做到进程自动回复的功能，不再需要自己写Shell脚本控制。

​	因为Supervisor是Python开发的，安装前先检查一下系统是否安装Python 2.4以上的版本。下面以CentOS 7环境介绍SuperVisor的安装和配置。

## 安装Python包管理工具

​	`easy_install`是setuptools包里带的一个命令，使用easy_install实际上是在调用setuptools来完成安装模块的工作，所以安装setuptools即可。



```
[root@localhost ~]# wget --no-check-certificate https://bootstrap.pypa.io/ez_setup.py -O - | sudo python
```

## 安装Supervisor



```
[root@VM-0-7-centos ~]# easy_install supervisor
```

​	supervisor安装完成后会生成三个执行程序：`supervisord`、`supervisorctl`、`echo_supervisord_conf`，分别是supervisor的守护进程（用于接收管理命令）、客户端（用于和守护进程通信，发送管理进程的指令）、生成初始化配置文件程序。

## 配置

运行supervisord服务的时候，需要指定supervisor配置文件，如果没有显示指定，默认在以下路径查找：



```
$CWD/supervisord.conf
$CWD/etc/supervisord.conf
/etc/supervisord.conf
/etc/supervisor/supervisord.conf (since Supervisor 3.3.0)
../etc/supervisord.conf (Relative to the executable)
../supervisord.conf (Relative to the executable)
```

`$CWD`表示运行supervisord程序的目录。

可以通过运行`echo_supervisord_conf`程序来生成supervisor的初始化配置文件，如下所示：



```
[root@VM-0-7-centos ~]# mkdir /etc/supervisor
[root@VM-0-7-centos ~]# echo_supervisord_conf > /etc/supervisor/supervisord.con
```

## 配置文件参数说明

supervisor的配置参数较多，下面介绍以下常用的参数配置，详细的配置和说明，请参考官方文档。



```
[unix_http_server]
file=/tmp/supervisor.sock   ; Unix Socker文件，supervisorctl使用
;chmod=0700                 ; socket文件权限
;chown=nobody:nogroup       ; socket文件所属组和所有者
;username=user              ; default is no username (open server)
;password=123               ; default is no password (open server)

;[inet_http_server]         ; HTTP服务器，提供web管理界面
;port=127.0.0.1:9001        ; web管理后台运行的IP和端口
;username=user              ; 登录管理后台的用户名
;password=123               ; 登录管理后台的密码

[supervisord]
logfile=/tmp/supervisord.log ; 主日志文件
logfile_maxbytes=50MB        ; 日志文件大小，超出50M则进行切割
logfile_backups=10           ; 日志文件包保留备份数量默认为10
loglevel=info                ; 日志级别，默认为INFO，其他：debug、warn、trace
pidfile=/tmp/supervisord.pid ; PID文件
nodaemon=false               ; 是否在前台启动，默认是false，即以daemon的方式启动
silent=false                 ; no logs to stdout if true; default false
minfds=1024                  ; 可以打开的文件描述符的最小值。默认1024
minprocs=200                 ; 可以打开的进程数的最小值，默认为200
;umask=022                   ; process file creation umask; default 022
;user=supervisord            ; setuid to this UNIX account at startup; recommended if root
;identifier=supervisor       ; supervisord identifier, default is 'supervisor'
;directory=/tmp              ; default is not to cd during start
;nocleanup=true              ; don't clean up tempfiles at start; default false
;childlogdir=/tmp            ; 'AUTO' child log dir, default $TEMP
;environment=KEY="value"     ; key value pairs to add to environment
;strip_ansi=false            ; strip ansi escape codes in logs; def. fals

[supervisorctl]
serverurl=unix:///tmp/supervisor.sock ; 通过Unix Socket链接supervisord
;serverurl=http://127.0.0.1:9001 ; 通过HTTP的方式链接supervisord
;username=chris              ; should be same as in [*_http_server] if set
;password=123                ; should be same as in [*_http_server] if set

;[program:theprogramname]  [program:xx]是被管理的进程配置参数，xx是进程的名称
;command=/bin/cat              ; the program (relative uses PATH, can take args)
;process_name=%(program_name)s ; process_name expr (default %(program_name)s)
;numprocs=1                    ; number of processes copies to start (def 1)
;directory=/tmp                ; directory to cwd to before exec (def no cwd)
;umask=022                     ; umask for process (default None)
;priority=999                  ; 进程启动优先级，默认999，值小的优先启动
;autostart=true                ; 在supervisor启动的时候也启动
;startsecs=1                   ; 在启动1秒后没有异常退出，就表示进程正常启动了
;startretries=3                ; 启动失败自动重试次数，默认是3
;autorestart=unexpected        ; when to restart if exited after running (def: unexpected)
;exitcodes=0                   ; 'expected' exit codes used with autorestart (default 0)
;stopsignal=QUIT               ; signal used to kill process (default TERM)
;stopwaitsecs=10               ; max num secs to wait b4 SIGKILL (default 10)
;stopasgroup=false             ; 默认为false，进程被杀死时，是否向这个进程组发送stop信号，包括子进程
;killasgroup=false             ; 默认为false，向进程组发送kill信号，包括紫禁城
;user=chrism                   ; 用哪个用户启动进程
;redirect_stderr=true          ; 把stderr重定向到stdout
;stdout_logfile=/a/path        ; stdout log path, NONE for none; default AUTO
;stdout_logfile_maxbytes=1MB   ; stdout日志文件大小，默认50MB
;stdout_logfile_backups=10     ; stdout日志文件备份树，默认是10
;stdout_capture_maxbytes=1MB   ; number of bytes in 'capturemode' (default 0)
;stdout_events_enabled=false   ; emit events on stdout writes (default false)
;stdout_syslog=false           ; send stdout to syslog with process name (default false)
;stderr_logfile=/a/path        ; stderr log path, NONE for none; default AUTO
;stderr_logfile_maxbytes=1MB   ; max # logfile bytes b4 rotation (default 50MB)
;stderr_logfile_backups=10     ; # of stderr logfile backups (0 means none, default 10)
;stderr_capture_maxbytes=1MB   ; number of bytes in 'capturemode' (default 0)
;stderr_events_enabled=false   ; emit events on stderr writes (default false)
;stderr_syslog=false           ; send stderr to syslog with process name (default false)
;environment=A="1",B="2"       ; process environment additions (def no adds)
;serverurl=AUTO                ; override serverurl computation (childutils)

;[include]
;files = relative/directory/*.ini ; 可以指定一个或多个以.ini结束的配置文件
```

## 配置进程管理

进程管理配置参数，不建议全部写在supervisord.conf文件中，应该每个进程写一个配置文件放在include指定的目录下包含进supervisord.conf文件中。

1）创建`/etc/supervisor/config.d`目录，用于存放进程管理的配置文件；

2）修改`/etc/supervisor/supervisord.conf`中的include参数，将`/etc/supercisor/conf.d`目录添加到include中。



```
[include]
files = /etc/supervisor/config.d/*.ini
```

下面时配置Tomcat进程的例子：



```
[program:tomcat]
command=/usr/local/tomcat/bin/catalina.sh run
stdout_logfile=/usr/local/tomcat/logs/catalina.out
autostart=true
autorestart=true
startsecs=5
priority=1
stopasgroup=true
killasgroup=true
```

## 启动supervisor



```
[root@VM-0-7-centos ~]# supervisorctl -c /etc/supervisor/supervisord.conf
```

## 控制进程

### 交互终端

supervisord启动成功后，可以通过supervisorctl客户端控制进程，启动、停止、重启。运行supervisorctl命令，不加参数，会进入supervisor客户端按的交互终端，并会列出当前所管理的所有进程。

[![image-20210224152131703](https://hzxyf.oss-cn-hangzhou.aliyuncs.com/hdlimage-20210224152131703.png)](https://hzxyf.oss-cn-hangzhou.aliyuncs.com/hdlimage-20210224152131703.png)

上图中的`tomcat`就是我们在配置文件中`[program:tomcat]`的名字。

输入help命令可以查看可以执行的命令列表，如果想看某个命令的作用，输入help命令名称，如help stop



```
stop tomcat  # 表示停止Tomcat进程
stop all     # 表示停止所有进程
```

### bash终端



```
supervisorctl reload  # 重启supervisor
supervisorctl status  # 查看状态
supervisorctl start tomcat
supervisorctl restart tomcat
supervisorctl reread
supervisorctl update
```

### web管理界面

如果需要开启web界面，则需要修改配置文件取消注释



```
[root@VM-0-7-centos logs]# vim /etc/supervisor/supervisord.conf
[inet_http_server]         ; inet (TCP) server disabled by default
port=127.0.0.1:9001        ; ip_address:port specifier, *:port for all iface
;username=root              ; default is no username (open server)
;password=123456               ; default is no password (open server)
[root@VM-0-7-centos logs]#  supervisorctl reload
```

## 配置开启自启动supervisor

### 配置systemctl服务

1）进入`/lib/systemd/system`目录，并创建`supervisor.service`文件



```
[Unit]
Description=supervisor
After=network.target

[Service]
Type=forking
ExecStart=/usr/bin/supervisord -c /etc/supervisor/supervisord.conf
ExecStop=/usr/bin/supervisorctl $OPTIONS shutdown
ExecReload=/usr/bin/supervisorctl $OPTIONS reload
KillMode=process
Restart=on-failure
RestartSec=42s

[Install]
WantedBy=multi-user.target
```

2）设置开机启动



```
[root@VM-0-7-centos ~]# systemctl enable supervisor.service
[root@VM-0-7-centos ~]# systemctl daemon-reload
```

3）修改文件权限为766



```
[root@VM-0-7-centos ~]# chmod 766 /lib/systemd/system/supervisor.service
```

### 配置service类型



```
#!/bin/bash
#
# supervisord   This scripts turns supervisord on
#
# Author:       Mike McGrath <mmcgrath@RedHat.com> (based off yumupdatesd)
#
# chkconfig:    - 95 04
#
# description:  supervisor is a process control utility.  It has a web based
#               xmlrpc interface as well as a few other nifty features.
# processname:  supervisord
# config: /etc/supervisor/supervisord.conf
# pidfile: /var/run/supervisord.pid
#

# source function library
. /etc/rc.d/init.d/functions

RETVAL=0

start() {
    echo -n $"Starting supervisord: "
    daemon "supervisord -c /etc/supervisor/supervisord.conf "
    RETVAL=$?
    echo
    [ $RETVAL -eq 0 ] && touch /var/lock/subsys/supervisord
}

stop() {
    echo -n $"Stopping supervisord: "
    killproc supervisord
    echo
    [ $RETVAL -eq 0 ] && rm -f /var/lock/subsys/supervisord
}

restart() {
    stop
    start
}

case "$1" in
  start)
    start
    ;;
  stop) 
    stop
    ;;
  restart|force-reload|reload)
    restart
    ;;
  condrestart)
    [ -f /var/lock/subsys/supervisord ] && restart
    ;;
  status)
    status supervisord
    RETVAL=$?
    ;;
  *)
    echo $"Usage: $0 {start|stop|status|restart|reload|force-reload|condrestart}"
    exit 1
esac

exit $RETVAL
```

将上述脚本内容保存到`/etc/rc.d/rc.local/supervisor`文件中，修改文件权限为755，并设置开机启动



```
[root@VM-0-7-centos ~]# chmod 755 /etc/rc.d/init.d/supervisor
[root@VM-0-7-centos ~]# chkconfig supervisor on
```



__EOF__

![img](https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1600770991721&di=c6a7d47b199a93452c32f9220f0c22a7&imgtype=0&src=http%3A%2F%2Fpic3.zhimg.com%2F50%2Fv2-b6005d639c415b69d7ef2ce6fdfdcaaa_hd.jpg)

本文作者：**[hzxyf](https://www.cnblogs.com/xiaoyafei/p/14441875.html)** 
**本文链接**：https://www.cnblogs.com/xiaoyafei/p/14441875.html
关于博主：评论和私信会在第一时间回复。或者[直接私信](https://msg.cnblogs.com/msg/send/xiaoyafei)我。
版权声明：本博客所有文章除特别声明外，均采用 [BY-NC-SA](https://creativecommons.org/licenses/by-nc-nd/4.0/) 许可协议。转载请注明出处！
声援博主：如果您觉得文章对您有帮助，可以点击文章右下角**【[推荐](javascript:void(0);)】**一下。您的鼓励是博主的最大动力！ 
