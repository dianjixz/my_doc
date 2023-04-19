#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
 
using namespace std;
 
void daemond()
{
    //父进程创建子进程,然后退出,形式上脱离控制终端
    pid_t pid = fork();
    if(pid > 0)
    {
        exit(0);
    }
 
    //子进程创建一个新的会话
    setsid();
    //改变当前目录为根目录
    chdir("/");
    //设置umask掩码,防止从父进程继承的过来的屏蔽字拒绝某些权限
    umask(0);
    //关闭文件描述符,节省系统资源
    //STDIN_FILENO就是标准输入设备（一般是键盘）的文件描述符
    //写入/dev/null的东西会被系统丢掉
    close(STDIN_FILENO);
    //再一次的保护
    open("/dev/null",O_RDWR);
    dup2(0,1);
    dup2(0,2);
    //守护进程的核心逻辑
    //退出
 
}
 
int main(void)
{
 
    daemond();
    while(1);
 
    return 0;
}