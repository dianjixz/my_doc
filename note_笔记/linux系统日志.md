https://www.cnblogs.com/wxp100/p/18195458

“/var/log”是Linux系统登录文件放置的地方,里面就是记录点日志，可以删除，不过为了句柄安全，最好删除后重启xenserver（就是重启虚拟机）。

以下是位于 /var/log/ 目录下的 不同的日志文件。其中一些日志文件是特定于发行版的。例如，您会在基于 Debian 的系统上看到 dpkg.log（例如，在 Ubuntu 上）。
系统日志

常用系统日志：

    系统报错日志:/var/log/messages – 包含全局系统消息，包括系统启动期间记录的消息。/var/log/messages 中记录了几件事情，包括 mail、cron、daemon、kern、auth 等。
    核心启动日志:/var/log/dmesg – 包含内核环形缓冲区信息。当系统启动时，它会在屏幕上打印一些消息，这些消息显示内核在启动过程中检测到的硬件设备的信息。这些消息在内核环形缓冲区中可用，每当新消息到来时，旧消息就会被覆盖。您还可以使用dmesg 命令查看此文件的内容。
    邮件系统日志:/var/log/maillog或/var/log/mail.log – 包含来自系统上运行的邮件服务器的日志信息。例如，sendmail 将有关所有已发送项目的信息记录到此文件
    安全、登录与网络日志:/var/log/secure – 包含与身份验证和授权权限相关的信息。例如，sshd 在此处记录所有消息，包括登录失败。
    登录和身份验证日志：/var/log/auth.log或/var/log/audit/ – 包含系统授权信息，包括使用的用户登录和身份验证机制。
    登录记录:/var/log/wtmp或/var/log/utmp – 是一个二进制文件，记录每个用户的登录次数和持续时间等信息。包含登录记录。last命令就通过访问这个文件获得这些信息，并以反序从后向前显示用户的登录记录，last也能根据用户、终端tty或时间显示相应的记录。。who 命令使用这个文件来显示信息。
    失败登录日志：/var/log/btmp – 此文件包含有关登录尝试失败的信息。使用最后一条命令查看 btmp 文件。例如，“last -f /var/log/btmp | 更多的”
    最近登录日志：/var/log/lastlog – 显示所有用户的最近登录信息。这不是一个 ascii 文件。您应该使用 lastlog 命令查看此文件的内容。
    定时任务日志:/var/log/cron – 每当cron 守护程序（或anacron）启动 cron 作业时，它都会在此文件中记录有关 cron 作业的信息

其他系统日志：

    /var/log/anaconda.log – 安装 Linux 时，所有与安装相关的消息都存储在此日志文件中
    /var/log/boot.log – 包含系统引导时记录的信息
    /var/log/kern.log – 包含内核记录的信息。有助于您对定制内核进行故障排除。
    /var/log/daemon.log – 包含系统上运行的各种后台守护程序记录的信息
    /var/log/dpkg.log – 包含使用 dpkg 命令安装或删除软件包时记录的信息
    /var/log/yum.log – 包含使用 yum 安装软件包时记录的信息
    /var/log/user.log – 包含有关所有用户级别日志的信息
    /var/log/Xorg.x.log – 来自 X 的日志消息
    /var/log/alternatives.log – update-alternatives 的信息记录到此日志文件中。在 Ubuntu 上，update-alternatives 维护确定默认命令的符号链接。
    /var/log/cups – 所有打印机和打印相关的日志消息

应用程序日志

    /var/log/httpd/（或）/var/log/apache2 – 包含 apache Web 服务器 access_log 和 error_log
    /var/log/lighttpd/ – 包含轻量 HTTPD access_log 和 error_log
    /var/log/conman/ – ConMan 客户端的日志文件。conman 连接由 conmand 守护进程管理的远程控制台。
    /var/log/mail/ - 此子目录包含来自您的邮件服务器的其他日志。例如，sendmail 将收集到的邮件统计信息存储在 /var/log/mail/statistics 文件中
    /var/log/prelink/ - prelink 程序修改共享库和链接二进制文件以加快启动过程。/var/log/prelink/prelink.log 包含有关被预链接修改的 .so 文件的信息。
    /var/log/audit/ - 包含 Linux 审计守护进程 (auditd) 存储的日志信息。
    /var/log/setroubleshoot/ - SELinux 使用 setroubleshootd（SE 疑难解答守护程序）来通知文件安全上下文中的问题，并将这些信息记录在此日志文件中。
    /var/log/samba/ - 包含 samba 存储的日志信息，用于将 Windows 连接到 Linux。
    /var/log/sa/ - 包含由sysstat 包收集的每日 sar 文件。
    /var/log/sssd/ – 由管理对远程目录和身份验证机制的访问的系统安全服务守护进程使用。

