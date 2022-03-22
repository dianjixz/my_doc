 

**1、system（执行shell 命令）**
**相关函数** fork，execve，waitpid，popen
**表头文件** #include<stdlib.h>
**定义函数** int system(const char * string);
**函数说明** system()会调用fork()产生子进程，由子进程来调用/bin/sh-c
string来执行参数string字符串所代表的命令，此命令执行完后随
即返回原调用的进程。在调用system()期间SIGCHLD 信号会被暂时
搁置，SIGINT和SIGQUIT 信号则会被忽略。
返回值 如果system()在调用/bin/sh时失败则返回127，其他失败原因返回-
1。若参数string为空指针(NULL)，则返回非零值。如果system()调
用成功则最后会返回执行shell命令后的返回值，但是此返回值也有
可能为system()调用/bin/sh失败所返回的127，因此最好能再检查
errno 来确认执行成功。
**附加说明** 在编写具有SUID/SGID权限的程序时请勿使用system()，system()会
继承环境变量，通过环境变量可能会造成系统安全的问题。
**范例:**

复制代码 代码如下:

~~~
#include<stdlib.h>
main(){
system(“ls -al /etc/passwd /etc/shadow”);
}
~~~

**2、popen（建立管道I/O）**
**相关函数** pipe，mkfifo，pclose，fork，system，fopen
**表头文件** #include<stdio.h>
**定义函数** FILE * popen( const char * command,const char * type);
**函数说明** popen()会调用fork()产生子进程，然后从子进程中调用/bin/sh -c
来执行参数command的指令。参数type可使用“r”代表读取，“w”
代表写入。依照此type值，popen()会建立管道连到子进程的标准输
出设备或标准输入设备，然后返回一个文件指针。随后进程便可利
用此文件指针来读取子进程的输出设备或是写入到子进程的标准输
入设备中。此外，所有使用文件指针(FILE*)操作的函数也都可以使
用，除了fclose()以外。
**返回值** 若成功则返回文件指针，否则返回NULL，错误原因存于errno中。
错误代码 EINVAL参数type不合法。
**注意事项** 在编写具SUID/SGID权限的程序时请尽量避免使用popen()，popen()
会继承环境变量，通过环境变量可能会造成系统安全的问题。
**范例: 
**

复制代码 代码如下:

~~~
#include<stdio.h> 
main() 
{ 
FILE * fp; 
char buffer[80]; 
fp=popen(“cat /etc/passwd”,”r”); 
fgets(buffer,sizeof(buffer),fp); 
printf(“%s”,buffer); 
pclose(fp); 
}
~~~

执行 root :x:0 0: root: /root: /bin/bash
**3、使用vfork()新建子进程，然后调用exec函数族**

复制代码 代码如下:

~~~
#include<unistd.h>
main()
{
  char * argv[ ]={“ls”,”-al”,”/etc/passwd”,(char*) };

  if(vfork() = =0)
  {
    execv(“/bin/ls”,argv);
  }else{    
    printf(“This is the parent process\n”);
  }
}
~~~