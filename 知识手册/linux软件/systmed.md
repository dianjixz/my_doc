## [Linux systemd启动守护进程，service启动顺序分析及调整service启动顺序](https://blog.csdn.net/xing_huo95/article/details/90246050)

1.systemd相关基础知识，参考以下几篇博客  
http://www.ruanyifeng.com/blog/2016/03/systemd-tutorial-commands.html  
https://www.jianshu.com/p/86adb6d5347b  

2.分析当前系统各项服务启动时间及顺序

a.列出各项启动占用的时间，但由于是并行启动，启动时间不决定启动完成先后

~~~ bash
systemd-analyze blame
~~~
![](https://img-blog.csdnimg.cn/2019051522150665.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hpbmdfaHVvOTU=,size_16,color_FFFFFF,t_70)

b.列出启动矢量图，用浏览器打开boot.svg文件  得到各service启动顺序
~~~ bash
systemd-analyze plot > boot.svg
~~~
![](https://img-blog.csdnimg.cn/20190515222152145.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hpbmdfaHVvOTU=,size_16,color_FFFFFF,t_70)

分析seeed-voicecard.service

~~~ bash
[Unit]
Description=Seeed Voicecard service
After=alsa-restore.service
 
[Service]
Type=oneshot
RemainAfterExit=yes
ExecStart=/usr/bin/seeed-voicecard
User=root
 
[Install]
WantedBy=sysinit.target
~~~
service将在alsa-restore.service之后启动，且属于sysinit.target组，目的在于执行seeed-voicecard 文件
systemd启动顺序约定为，当前服务满足启动条件，则立即创建进程进行并行启动。启动条件指服务的依赖关系（Requires、Wants）及顺序关键字(After、Before)等
c.列出seeed-voicecard.service的全部依赖关系，发现依赖关系异常多、且复杂。但service文件中并未提到如此多的依赖、怀疑依赖有继承关系，删掉After alsa-re... / 仍然有很多依赖。寻找何处定义依赖关系花费了很长时间，也是解决问题的关键。
~~~ bash
systemctl list-dependencies seeed-voicecard.service
~~~

## systemd service 相关知识  
- Unit: 主要用于配置整个服务的详情信息以及服务依赖，用于快速识别该服务的相关情况以及依赖项目  
- Service: 主要用于配置整个服务生命周期的管理行为  
- Install: 用于配置服务的安装级别，当我们设置服务在 Linux 的那种级别下启动或开机自启动时会加载该部分 (典型的当 chkconfig nginx on 或者 systemctl enable|disable nginx.service 时会读取该部分的配置)  

### Unit 块
Unit 块除了可以简单描述整个服务的相关详情外，还有最重要的一点可以用来管理该服务的启动顺序和启动依赖 (必须也得由 systemd 管理)。

比如通常我们有一些服务，必须依赖一些基础环境 (ntpd，rsyslog，network) 才能启动成功，或者必须在某个服务后启动，那我们就可以在这里进行配置。

注意: 在 Unit 快中，每个指令后都可以指定一个以空格分隔的列表

### 服务描述
在 systemd 的 service 配置中，我们通常会使用如下几个配置项来描述该服务的基本信息:

Description: 服务的简单描述Documentation: 指定服务的文档，便于管理员快速追溯，一般可以使用 "http://", "https://", "file:", "info:", "man:" 五种 URI 类型

### 服务依赖管理
systemd 中会有几种服务依赖管理的指令，可能在实际使用过程中会有一些疑惑，有时候功能可能重叠，因此需要根据具体的使用场景进行组合。

- After: 用于指定该服务在那些服务之后启动 ，停止时正好相反
- Before: 用于指定该服务在那些服务之前启动，停止时正好相反
- Requires: 设置该服务必须依赖的其他服务，因此在该服务启动之前，指定的服务列表必须全部在线，否则服务将启动失败或掉线。但如果未设置 After 和 Before 指令时，该服务和依赖的服务将会并行的同时启动。注意：该指令依赖的服务不一定要在整个生命周期都保持一直在线，这取决于其他的检查条件
- Requisite: 和 Requires 类似，区别是在该服务启动时，该指令指定的依赖资源必须全部处于启动成功的状态，否则该服务立马失败，并且不会启动那些失败的依赖服务。因此一般建议 Requisite 和 After 一起联合使用会比较好。
- Wants: Requires 的弱化版，当该服务启动时，尽可能的启动该指令指定的服务，但不会影响该服务的启动
- BindsTo: 和 Requires 类似，但是依赖性更强，这里列出来的任何服务停止运行或崩溃，该服务将立即被停止。考虑服务的启动依赖，一般会和 After 一起使用
- PartOf: 和 Requires 类似，但仅作用于服务的停止和重启，表示该服务是所列服务的一部分，会随指定服务的启动而启动。注意：该指令是单向依赖，服务的主动的重启不会影响其他
![](https://pic4.zhimg.com/80/v2-3c701a3ffac61a0bcd80934d03fff10b_720w.jpg)
### Service 块
毋庸置疑的是，我们通常使用 systemd 最初的想法是用来启动和停止进程，因此，整个 systemd 最核心的部分也就在 Service 块了，因此我们需要对该部分有很熟悉的理解。
#### 基本指令
- Type: 指定进程的启动类型，必须设为 simple, exec, forking, oneshot, dbus, notify, idle 之一。常用的几种如下:
- simple: 一般没有其他指令时，为默认值，表示 ExecStart 后的指令为主进程，主进程启动后服务即启动成功。如果进程需要为其他进程提供服务，需要通过 socket 来进行 
- exec: 同 simple 类似，但是表示该服务的主服务进程执行完成之后，才真正启动成功。
- forking: 标识在使用 ExecStart 后的指令启动程序后，会进行 fork() 的系统调用。传统 Unix 中守护进程的经典做法，如果使用此类型，建议设置 PIDFile= 来指定该服务的主进程。比如 nginx 进程的 daemon 方式，主进程执行完成后，至少有 1 个进程在运行，此时服务状态会变成 active 状态 
- oneshot: 同 simple 类似，但是表示只有在该服务的主服务进程退出之后，才真正执行成功。一般用于执行一次性任务 (配置 RemainAfterExit=yes 设置可停止的一次性服务)
- PIDFile: 指定该服务的 PID 文件路径。systemd 会在启动后读取主服务进程的 PID，并记录在 MAINPID 变量中，在停止服务后，会主动进行删除该文件KillMode: 指定停止服务时，杀死进程的方式，必须设为 control-group, process, mixed, none 之一。(process 表示仅杀死主进程；mixed 表示向主进程发 SIGTERM 信号，然后想服务内 cgroup 的其他进程发 SIGKILL 信号；none 表示仅执行 ExecStop 动作，而不杀死进程)

注意: 建议对长时间持续运行的服务尽可能使用 Type=simple；如果有 master/slave 进程的建议使用 Type=forking
### 环境变量初始化
通常我们的服务主进程可能会有比较多的参数，建议的做法是使用环境变量方式进行维护对应的参数，而在 systemd 中可支持 K/V 和配置文件方式来进行设置。

- Environment: 指定 Key=Value 的环境配置
- EnvironmentFile: 指定包含 key=value 的配置文件

注意: 通常情况下，我们会看到 EnvironmentFile=-filename 和 EnvironmentFile=filename 的写法，前者表示可忽略，即当文件不存在或环境变量加载失败时也不影响后续的处理逻辑。(在 etcd 的高版本里，大部分参数采用了环境变量加载的方式，可见未来通过环境变量来控制服务的启动参数已经是趋势)



进程生命周期管理

整个进程的生命周期是从准备启动进程开始到整个进程结束的过程管理。通常情况下，可以包含为如下几个环节:

    ExecStartPre: 启动前， 该指令可用于在进程启动前加载一些列的基础环境以及初始化检查机制ExecStart: 启动逻辑， 该指令用于启动进程的核心逻辑ExecStartPost: 启动后， 该指令用于在启动进程后进行回调的相关操作ExecReload: 热加载， 该指令用于对进程进行热加载，通常情况用于 配置变更后的热重启 (/bin/kill -s HUP $MAINPID)ExecStop: 停止， 该指令用于对进程执行指定的停止操作，通常情况下用于进行进程的优雅停止 (/bin/kill -s TERM $MAINPID)ExecStopPost: 停止后， 该指令用于在进程停止后，进行一些列的资源释放和等待操作，因为通常情况当进程停止后，相关资源不能立即释放，因为服务不算真正呗停止，如果此时认为进程已停止而强制做一些操作，可能会影响到相关服务处理逻辑TimeoutStopSec: 停止超时时间， 指定服务停止后的超时时间，一般在通知服务是会使用 SIGTERM 信号，待超时时间内资源未释放，将使用 SIGKILL 进行强制杀死

重启策略

    Restart: 当服务进程 正常退出、异常退出、被杀死、超时的时候， 是否重新启动该服务。RestartSec: 多久后重启StartLimitBurst: 启动的最大次数限制，超过后停止继续重启StartLimitInterval: 启动时间的最大间隔

所谓 "服务进程" 是指 ExecStartPre=, ExecStartPost=, ExecStop=, ExecStopPost=, ExecReload= 中设置的进程。 当进程是由于 systemd 的正常操作 (例如 systemctl stop|restart) 而被停止时， 该服务不会被重新启动。 所谓 "超时" 可以是看门狗的 "keep-alive ping" 超时， 也可以是 systemctl start|reload|stop 操作超时.

可选值如下:

    no: 默认值，不会重启on-success: 服务进程正常退出 (退出码为 "0"， 或者进程收到 SIGHUP, SIGINT, SIGTERM, SIGPIPE 信号之一， 并且 退出码符合 SuccessExitStatus= 的设置) 时进行重启on-failure: 仅在服务进程异常退出 (退出码不为 "0"， 或者 进程被强制杀死 (包括 "core dump" 以及收到 SIGHUP, SIGINT, SIGTERM, SIGPIPE 之外的其他信号)， 或者进程由于 看门狗超时 或者 systemd 的操作超时 而被杀死) 时重启always: 无条件重启

资源限制

通常情况下，我们会对目标服务或进程进行一定资源的限制，以防止服务 bug 导致的整个系统资源耗尽。

在 systemd 中，可以使用如下指令进行简单的资源限制:

    LimitCPU: 限制 CPU 使用时长 (秒)，等同 ulimit -tLimitNOFILE: 限制进程使用的文件描述符数量，等同 ulimit -nLimitNPROC: 限制进程的数量，等同于 ulimit -u

注意: 一般为防止服务导致的系统过载，会对进程设置一些资源限制

计算资源调度

    CPUSchedulingPolicy: 设置 CPU 的调度策略可设为 other, batch, idle, fifo, rr 之一CPUSchedulingPriority: 设置 CPU 调度的优先级，取决于调度策略CPUAffinity: 设置 CPU 的亲和性，可以指定 cpu 的编号，比如 0,1

示例

[Service]
EnvironmentFile=/etc/calico/calico.env
ExecStartPre=-/usr/bin/docker pull ${CALICO_IMAGE}
ExecStart=/usr/bin/docker run --net=host --privileged ${CALICO_IMAGE}
ExecStop=-/usr/bin/docker stop calico-node
ExecStopPost=-/usr/bin/docker rm -f -v calico-node
Restart=on-failure
RestartSec=5
StartLimitBurst=3
StartLimitInterval=60s
CPUSchedulingPolicy=fifo
CPUAffinity=0,2
LimitNOFILE=1000
LimitNPROC=3000
LimitCORE=infinity

Install 块

在 systemd 的服务管理中，install 块主要包含了单元的启用信息。只有 systemctl 的 enable/disable 指令会调用该部分内容。

    Alias: 指定服务的别名信息WantedBy/RequiredBy: 设置启用服务的依赖服务

示例

[Install]
Alias=syslog.service
WantedBy=multi-user.target

完整示例

systemd service 完整配置

$ cat  /usr/lib/systemd/system/docker.service
[Unit]
Description=Docker CE Binary Release.
Documentation=https://docs.docker.com
Documentation=https://download.docker.com/linux/static/stable/x86_64/
After=network-online.target firewalld.service
Wants=network-online.target

[Service]
Type=notify
Environment="WELCOME=BGBiao Docker Base Environment."
ExecStartPre=/bin/echo ${WELCOME}
ExecStart=/usr/local/sbin/dockerd  $DOCKER_NETWORK_OPTIONS --live-restore --data-root /opt/data/docker
ExecStartPost=/usr/sbin/iptables -P FORWARD ACCEPT
ExecReload=/bin/kill -s HUP ${MAINPID}
ExecStop=/bin/kill -s TERM ${MAINPID}
ExecStopPost=/usr/bin/sleep 3

LimitNOFILE=100
LimitNPROC=3000
LimitCORE=infinity
TimeoutStartSec=0
Delegate=yes
KillMode=process
Restart=always
StartLimitBurst=3
StartLimitInterval=60s

[Install]
WantedBy=multi-user.target

service 生命周期过程

# 加载配置并重启docker
$ systemctl daemon-reload
$ systemctl restart docker

# 查看整个启动过程信息
$ cat /var/log/messages

Oct 31 14:56:39 host-192-168-0-171 systemd: Reloading.
Oct 31 14:56:43 host-192-168-0-171 systemd: Starting Docker CE Binary Release....
Oct 31 14:56:43 host-192-168-0-171 echo: BGBiao Docker Base Environment.
Oct 31 14:56:43 host-192-168-0-171 dockerd: time="2020-10-31T14:56:43.390806162+08:00" level=info msg="Starting up"
......
Oct 31 14:56:43 host-192-168-0-171 systemd: Started Docker CE Binary Release..


# 停止docker，查看整个过程

$ echo >  /var/log/messages
$ systemctl stop docker
$ cat /var/log/messages

Oct 31 15:01:11 host-192-168-0-171 systemd: Stopping Docker CE Binary Release....
Oct 31 15:01:11 host-192-168-0-171 dockerd: time="2020-10-31T15:01:11.618357841+08:00" level=info msg="Processing signal 'terminated'"
Oct 31 15:01:11 host-192-168-0-171 dockerd: time="2020-10-31T15:01:11.618919303+08:00" level=info msg="Daemon shutdown complete"
Oct 31 15:01:11 host-192-168-0-171 dockerd: time="2020-10-31T15:01:11.618948850+08:00" level=info msg="stopping healthcheck following graceful shutdown" module=libcontainerd
Oct 31 15:01:11 host-192-168-0-171 dockerd: time="2020-10-31T15:01:11.618971913+08:00" level=info msg="stopping event stream following graceful shutdown" error="context canceled" module=libcontainerd namespace=plugins.moby
Oct 31 15:01:11 host-192-168-0-171 dockerd: time="2020-10-31T15:01:11.618952081+08:00" level=info msg="stopping event stream following graceful shutdown" error="context canceled" module=libcontainerd namespace=moby
Oct 31 15:01:11 host-192-168-0-171 dockerd: time="2020-10-31T15:01:11.618954836+08:00" level=info msg="Processing signal 'terminated'"
Oct 31 15:01:15 host-192-168-0-171 systemd: Stopped Docker CE Binary Release..


# 执行
$ systemctl enable docker
Created symlink from /etc/systemd/system/multi-user.target.wants/docker.service to /usr/lib/systemd/system/docker.service.

# 会在多用户下创建服务软链，可保证系统在启动时默认启动服务
$ ls -ld /etc/systemd/system/multi-user.target.wants/docker.service
lrwxrwxrwx 1 root root 38 Oct 31 15:05 /etc/systemd/system/multi-user.target.wants/docker.service -> /usr/lib/systemd/system/docker.service


# 查看资源限制
## 系统的资源限制
$ ulimit -a | egrep  '(files|processes)'
open files                      (-n) 65535
max user processes              (-u) 63457

$ systemctl status docker | grep -i pid
 Main PID: 21926 (dockerd)

## dockerd 进程的资源限制(生产环境需严格设置参数)
$ cat /proc/21926/limits | egrep '(files|processes)'
Max processes             3000                 3000                 processes
Max open files            100                  100                  files


由上述的整个过程可以看到，我们在整个 dockerd 的生命周期中，systemd 如何去管理进程的生命周期的。

Tips

需要注意的是，对于资源限制，Linux 中的 /etc/security/limits.conf 作用域变小了 (只对登录用户的资源进行限制)，可能对 systemd 的 service 的资源限制不生效，因此对于 systemd servie 的资源限制，可以对全局文件进行修改:

$ cat /etc/systemd/system.conf
DefaultLimitNOFILE=100000
DefaultLimitNPROC=65535

[官网文档](https://link.zhihu.com/?target=https%3A//systemd.io/)
[参考文章](https://link.zhihu.com/?target=http%3A//www.jinbuguo.com/systemd/systemd.index.html)

[参考翻译手册](http://www.jinbuguo.com/)