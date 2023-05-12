``` c
// int master, slave;
// char *slavename;
// char *cc;
// master = posix_openpt(O_RDWR);
// grantpt(master);
// unlockpt(master);
// slavename = ptsname(master);
// slave = open(slavename, O_RDWR);
// printf("master: %dn", master);
// printf("slavename: %sn", slavename);
#if defined(__linux__) || defined(__GLIBC__) || defined(__GNU__)
#define _GNU_SOURCE			/* GNU glibc grantpt() prototypes */
#endif
// #include
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <pty.h>  /* for openpty and forkpty */
#include <utmp.h> /* for login_tty */
#include <time.h>


#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int master;
    char slavename[50], window[64], buf[64];
    FILE *slave;
    master = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    int md = grantpt(master);
    int ck = unlockpt(master);
    // slavename = ptsname(master);
    int sad = ptsname_r(master, slavename, 50);

    printf("master: %d\r\n", master);
    printf("slavename: %s\r\n", slavename);
    snprintf(buf, sizeof(buf), "-S%s/%d", strrchr(slavename, '/') + 1, master);
    printf("niha:%s\r\n", buf);
    if (!fork())
    {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave = fopen(slavename, "r+");
    fgets(window, sizeof(window), slave);
    printf("window: %sn", window);
    fputs("say something: ", slave);
    fgets(buf, sizeof(buf), slave);
    fprintf(slave, "you said %snexiting in 3 seconds...n", buf);
    sleep(3);
    return 0;
}
```


伪终端(pseudoterminal): ptmx, pts (伪终端master和伪终端slave).

/dev/ptmx用于创建伪终端主从设备对.当我们用open打开/dev/ptmx设备后, 返回主设备的文件描述符,

并且在/dev/pts/目录下创建一个伪终端从设备.

在成功打开/dev/ptmx后, 可以通过函数ptsname()来获取从设备的path.

想要打开open伪终端从设备,必须先使用主设备的文件描述符调用grantpt(), unlockpt().

当主设备和从设备都打开后, 在主设备写入的数据可以在从设备上读取, 在从设备上写入的数据可以在主设备读取.

Data written to the slave is presented on the master descriptor as input.  Data written to the master is presented to the slave as input.

伪终端应用:

当需要调用调用exece等函数执行系统命令时,

通过fork函数创建子进程, 在子进程中调用exece执行系统命令, 并且把子进程的stdout和stderr通过dup2重定向

到伪终端的从设备;而在父进程中通过读取伪终端的主设备来获取子进程支持系统命令的输出信息.




三、伪终端编程接口

　　从2.6.4版本内核开始，BSD风格伪终端被认为过时，不再被新的应用使用，可以在配置内核时去除对BSD伪终端的支持。

老式的BSD伪终端的使用是先依次对每个master文件执行open操作直到成功，然后从应用程序打开相应的slave并执行读写操作。

UNIX 98伪终端的一般使用流程如下（具体参考linux man手册）：

    使用posix_openpt打开master；
    使用grantpt设置调用进程为slave的属主并允许其对slave进行读写操作；
    使用unlockpt对slave解锁；
    使用ptsname返回slave的设备名；
    使用open打开slave设备并进行读写操作。

　　上述步骤中的函数都来自glibc库，函数原型分别说明如下：

函数名
	

posix_openpt

功能
	

打开一个未被使用的伪终端设备

参数
	

flags
	

int
	

设备操作标记，可以是0或以下两项的之一（或组合）

O_RDWR —— 允许对设备同时进行读写操作，此标记通常需要指定；

O_NOCTTY —— 不将设备作为进程的控制终端。

返回值
	

int，如果成功，返回master设备的文件描述符，否则-1

说明
	

需要包含stdlib.h头文件，Glibc2.2.1及其后续版本支持，一些UNIX系统没有该函数，可以自行实现如下。

int posix_openpt(int flags)

{

    return open("/dev/ptmx", flags);

}

 

函数名
	

grantpt

功能
	

改变指定master对应从设备的属主与访问权限

参数
	

fd
	

int
	

主设备文件描述符

返回值
	

int，如果成功，返回0，否则-1

说明
	

需要包含stdlib.h头文件，Glibc2.1及其后续版本支持。

 

函数名
	

unlockpt

功能
	

解锁指定master对应的从设备

参数
	

fd
	

int
	

主设备文件描述符

返回值
	

int，如果成功，返回0，否则-1

说明
	

需要包含stdlib.h头文件，Glibc2.1及其后续版本支持。

 

函数名
	

ptsname

功能
	

获取指定master对应的从设备的名称

参数
	

fd
	

int
	

主设备文件描述符

返回值
	

char *，如果成功，在一个静态存储区存放设备名并返回其地址，否则NULL。

说明
	

需要包含stdlib.h头文件，Glibc2.1及其后续版本支持。

返回指针不能被调用进程释放。

此函数的可重入版本如下：

int ptsname_r(int fd, char *buf, size_t buflen)

该函数将返回的从设备名存放在buf指向的缓冲区，buflen为缓冲区大小。如果成功，ptsname_r返回0，否则返回非0值。

 

　　伪终端编程更常使用的API是openpty，其直接实现了上述流程的所有步骤，函数说明如下：

函数名
	

openpty

功能
	

获取一对可用的伪终端

参数
	

amaster
	

int *
	

指向主设备文件描述符

aslave
	

int *
	

指向从设备文件描述符

name
	

char *
	

如果输入参数不为空，存放返回的从设备名称

termp
	

struct

termios *termp
	

传入的从设备终端参数，通常设置为NULL

winp
	

struct

winsize *winp
	

传入的从设备窗口大小，通常设置为NULL

返回值
	

int，如果成功，返回0，否则-1。

说明
	

需要包含pty.h头文件，glibc2与libc5及后续版本支持，不遵循posix标准。

从glibc2.8开始对入参termp和winp增加const修饰符。

glibc2.0.92之前版本返回的是BSD伪终端对，2.0.92及后续版本返回UNIX 98伪终端对。

由于从设备名长度不可预知，如果通过传入非空的name参数来获取slave设备名，会有缓冲区溢出的危险。

 

　　login_tty函数用于实现在指定的终端上启动登录会话，函数说明如下：

函数名
	

login_tty

功能
	

为指定终端上的登录会话作准备

参数
	

fd
	

int *
	

指定终端设备的文件描述符

返回值
	

int，如果成功，返回0，否则-1。

说明
	

需要包含utmp.h头文件，glibc2与libc5及后续版本支持，不遵循posix标准。

 

　　forkpty函数整合了openpty,fork和 login_tty，在网络服务程序可用于为新登录用户打开一对伪终端，并创建相应的会话子进程。

函数名
	

forkpty

功能
	

为新会话打开一对伪终端并创建处理进程

参数
	

amaster
	

int *
	

指向主设备文件描述符

name
	

char *
	

如果输入参数不为空，存放返回的从设备名称

termp
	

struct

termios *termp
	

传入的从设备终端参数，通常设置为NULL

winp
	

struct

winsize *winp
	

传入的从设备窗口大小，通常设置为NULL

返回值
	

int，如果失败，返回-1，否则，子进程返回0，父进程返回子进程的PID。

说明
	

需要包含pty.h头文件，glibc2与libc5及后续版本支持，不遵循posix标准。

从glibc2.8开始对入参termp和winp增加const修饰符。

由于从设备名长度不可预知，如果通过传入非空的name参数来获取slave设备名，会有缓冲区溢出的危险。

 

　　注意：使用opentty，login_pty和forkpty需要链接util库。

 

四、伪终端编程示例

　　以下程序启动后打开一对伪终端，不断打印从主设备上读取的数据。


``` c
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 #include <sys/types.h>
 #include <linux/limits.h>
 #include <pty.h> /* for openpty and forkpty */
 #include <utmp.h> /* for login_tty */
 #include <time.h>
 
 #define SLAVE_DEV_NAME_MAX_LEN       100
 #define PTY_BUFF_MAX_LEN             100
 
 
 /* 
  * call opentty
  * print any data read from ptmx
  */
 int main(int argc, char *argv[])
 {
     int mpty = 0;
     int spty = 0;
     char spty_name[SLAVE_DEV_NAME_MAX_LEN] = {'/0'};
     char *pname = NULL;
     
     int rv = 0;
     int namelen = 0;
 
     int n = 0;
     char buf[PTY_BUFF_MAX_LEN] = {'/0'};
 
     fd_set rdfdset;
 
     rv = openpty(&mpty, &spty, spty_name, NULL, NULL);
 
     if (-1 == rv)
     {
         perror("Failed to get a pty");
         goto ERROR;
     }
 
     printf("Get a pty pair, FD -- master[%d] slave[%d]\n", mpty, spty);
     printf("Slave name is:%s\n", spty_name);
 
     /* Monitoring the pty master for reading */
     FD_ZERO(&rdfdset);
     FD_SET(mpty, &rdfdset);
 
     while (1)
     {
         rv = select(mpty + 1, &rdfdset, NULL, NULL, NULL);
         
         if (0 > rv)
         {
             perror("Failed to select");
             goto ERROR;
         }
 
         if (FD_ISSET(mpty, &rdfdset))
         {
             /* Now data can be read from the pty master */
             n = read(mpty, buf, PTY_BUFF_MAX_LEN);
             if (0 < n)
             {
                 int ii = 0;
                 
                 memset(buf + n, 0, PTY_BUFF_MAX_LEN - n);
 
                 printf("-----------------------------------\n");
                 printf("Message from slave:\n");
                 printf("%s\n", buf);
                 printf("------%d bytes------\n\n", n);
 
             }
             else if (0 == n)
             {
                 printf("No byte is read from the master\n");
             }
             else
             {
                 perror("Failed to read the master");
                 goto ERROR;
             }
         }
         else
         {
             printf("The master isn't readable!\n");
             goto ERROR;
         }
     }
 
 
 ERROR:
 
     if (0 < mpty)
     {
         close(mpty);
     }
 
     if (0 < spty)
     {
         close(spty);
     }
 
     return -1;
         
 }
```

 　　

　　启动一个终端窗口进入ptytest.c存放目录，输入如下命令编译代码。

XXX@ubuntu:~$ gcc -o ptytest -g ptytest.c –lutil  

　　输入./ptytest指令运行程序，终端窗口运行结果如下：

Get a pty pair, FD -- master[3] slave[4]

Slave name is:/dev/pts/23

　　打开另一个终端窗口，键入指令“echo hello, world  > /dev/pts/23”，ptytest程序运行窗口输出结果如下：
复制代码

Get a pty pair, FD -- master[3] slave[4]
Slave name is:/dev/pts/23
-----------------------------------
Message from slave:
hello, world

------14 bytes------

复制代码


