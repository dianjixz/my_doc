 ## C语言socket()函数：建立一个socket通信

相关函数：accept, bind, connect, listen

头文件：#include <sys/types.h>  #include <sys/socket.h>





### 定义函数：int socket(int domain, int type, int protocol);

函数说明：socket()用来建立一个新的socket, 也就是向系统注册, 通知系统建立一通信端口. 参数domain 指定使用何种的地址类型, 完整的定义在/usr/include/bits/socket.h 内, 底下是常见的协议:

PF_UNIX/PF_LOCAL/AF_UNIX/AF_LOCAL UNIX 进程通信协议
  PF_INET?AF_INET Ipv4 网络协议
  PF_INET6/AF_INET6 Ipv6 网络协议
  PF_IPX/AF_IPX IPX-Novell 协议
  PF_NETLINK/AF_NETLINK 核心用户接口装置
  PF_X25/AF_X25 ITU-T X. 25/ISO-8208 协议
  PF_AX25/AF_AX25 业余无线AX. 25 协议
  PF_ATMPVC/AF_ATMPVC 存取原始 ATM PVCs
  PF_APPLETALK/AF_APPLETALK appletalk (DDP)协议
  PF_PACKET/AF_PACKET 初级封包接口

参数 type 有下列几种数值:

1、SOCK_STREAM 提供双向连续且可信赖的数据流, 即TCP. 支持 OOB 机制, 在所有数据传送前必须使用connect()来建立连线状态.
2、SOCK_DGRAM 使用不连续不可信赖的数据包连接
3、SOCK_SEQPACKET 提供连续可信赖的数据包连接
4、SOCK_RAW 提供原始网络协议存取
5、SOCK_RDM 提供可信赖的数据包连接
6、SOCK_PACKET 提供和网络驱动程序直接通信. 

protocol 用来指定socket 所使用的传输协议编号, 通常此参考不用管它, 设为0 即可.

返回值：成功则返回socket 处理代码, 失败返回-1.

错误代码：
1、EPROTONOSUPPORT 参数domain 指定的类型不支持参数type 或protocol 指定的协议
2、ENFILE 核心内存不足, 无法建立新的socket 结构
3、EMFILE 进程文件表溢出, 无法再建立新的socket
4、EACCESS 权限不足, 无法建立type 或protocol 指定的协议
5、ENOBUFS/ENOMEM 内存不足
6、EINVAL 参数domain/type/protocol 不合法



## C语言网络编程：bind函数详解

函数功能

`bind` API能够将套接字文件描述符、端口号和`ip`绑定到一起
注意：
绑定的一定是自己的 ip和和端口，不是对方的；比如对于TCP服务器来说绑定的就是服务器自己的ip和端口

函数头文件 

 #include <sys/types.h>          /* See NOTES */
 #include <sys/socket.h>

函数使用

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

函数参数

- `sockfd` 表示`socket`函数创建的通信文件描述符
- `addrlen` 表示所指定的结构体变量的大小
- `addr` 表示`struct sockaddr`的地址，用于设定要绑定的ip和端口

~~~ c
struct sockaddr {
   sa_family_t sa_family;
   char        sa_data[14];
}
~~~

sa_family 用于指定AF_***表示使用什么协议族的ip
sa_data 存放ip和端口
这里有一个问题，直接向sa_data中写入ip和端口号有点麻烦，内核提供struct sockaddr_in结构体进行写入,通过/usr/include/linux/in.h可以看到结构体原型
使用该结构体时需要包含<netinet/in.h>头文件，且sockaddr_in结构体是专门为tcp/ip协议族使用，其他协议族需要使用其对应的转换结构体,比如“域通信协议族” 使用的是sockaddr_un结构体

~~~ c
struct sockaddr_in {
	  __kernel_sa_family_t  sin_family;     /* Address family               */
	  __be16                sin_port;       /* Port number                  */
	  struct in_addr        sin_addr;       /* Internet address             */
	
	  /* Pad to size of `struct sockaddr'. 设置IP端口号这个成员暂时用不到 */
	  unsigned char         __pad[__SOCK_SIZE__ - sizeof(short int) -
	                        sizeof(unsigned short int) - sizeof(struct in_addr)];
};

/* Internet address.填补相比于struct sockaddr所缺的字节数，保障强制转换不要出错 */
struct in_addr {
        __be32  s_addr; // __be32是32位的unsigned int ，因为ipv4是无符号32位整型 
};

~~~

- 可以看到以上`sockaddr_in`结构体中存放的端口和ip是分开的，所以设置起来非常方便，使用`struct sockaddr_in`设置后，让后将其强制转换为`struct sockaddr`类型，然后传递给bind函数即可

函数举例

~~~ c
struct sockaddr_in addr;
addr.sin_family = AF_INET; //设置tcp协议族
addr.sin_port = htons(6789); //设置端口号
addr.sin_addr.s_addr = inet_addr("192.168.1.105"); //设置ip地址

ret = bind(skfd, (struct sockaddr*)&addr, sizeof(addr));

~~~

如果是跨局域网或者城域网通信，这里设置的ip地址一定为通信设备所在路由器的外网ip地址。

如下c代码

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h> //struct sockadd_in 结构体的头文件
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

void print_err(char *str, int line, int err_no) {
	printf("%d, %s :%s\n",line,str,strerror(err_no));
	_exit(-1);
}

int main()
{
	int skfd = -1;
	skfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( -1 == skfd) {
		print_err("socket failed",__LINE__,errno);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET; //设置tcp协议族
	addr.sin_port = htons(6789); //设置端口号
	addr.sin_addr.s_addr = inet_addr("192.168.102.169"); //设置ip地址

	int ret = bind(skfd, (struct sockaddr*)&addr, sizeof(addr));
	if ( -1 == ret) {
		print_err("bind failed",__LINE__,errno);
	}
	return 0;
}

~~~

为什么需要bind函数

bind函数就是让套接字文件在通信时使用固定的IP和端口号（针对服务器来说）
可以看到如上实现代码，调用socket函数创建的套接字仅仅执行了通信等协议，但是并没有指定通信时所需的ip地址和端口号

ip 是对方设备的唯一标识
端口号 区分同一台计算机上的不同的网络通信进程
如果不调用bind函数指定ip和端口，则会自己指定一个ip和端口，此时违背了TCP通信的可靠性和面向连接的特点。

服务器如何知道客户端的ip和端口号
可以通过上文TCP通信模型中看到，客户端通信时不需要指定ip和端口号，直接创建一个socket套接字文件描述符即可参与通信。
此时当客户端和服务器建立连接的时候，服务器会从客户的数据包中提取出客户端ip和端口，并保存起来，如果是跨网通信，那么记录的就是客户端所在路由器的公网ip

htons函数
#include <arpa/inet.h>
uint16_t htons(uint16_t hostshort); 函数全拼为host to net short
函数功能
a. 将端口从"主机端序" 转为 “网络端序”
b. 如果给定的端口不是short,则转为short
返回值： 函数的调用永远都是成功的，返回转换后的端口号
htons兄弟函数htonl,ntohs,ntohl
htonl与htons唯一的区别时，转换完的端口号为long
ntohs 与htons恰好相反，是从网络字节序转换为主机字节序
ntohl 表示从网络字节转换为主机序，同时转换完的端口号为long
为什么要进行端口的大小端序的转换
大端序：
大端模式，是指数据的高字节保存在内存的低地址中，而数据的低字节保存在内存的高地址中，这样的存储模式有点儿类似于把数据当作字符串顺序处理：地址由小向大增加，而数据从高位往低位放；
小端序：
小端模式，是指数据的高字节保存在内存的高地址中，而数据的低字节保存在内存的低地址中，这种存储模式将地址的高低和数据位权有效地结合起来，高地址部分权值高，低地址部分权值低，和我们的逻辑方法一致。

大小端序是由具体的操作系统来决定的
可以使用如下代码测试系统是大端还是小端：

~~~ c
#include <stdio.h>
int main()
{
    int a = 1;
    char pc = *(char*)(&a);
    if (pc == 1)
        printf("第一个字节为1，小端存储\n");
    else
        printf("第一个字节为0，大端存储\n");

    return 0;

}
~~~

同样，网络通信的时候发送端计算机和接收端计算机可能端序不一致，比如发送者是大端序，接受者是小端序，如果通信时数据的端序处理不好很可能出现乱码，甚至无法接收到数据。
如果发送者和接受者端序一致则也能够正常传输数据，不用htons函数进行转换，不过保证数据序列正确的得进行传输，建议使用htons函数进行端口号的转换

inet_addr函数
<sys/socket.h> <netinet/in.h> <arpa/inet.h>
in_addr_t inet_addr(const char *cp);
函数功能:
a. 将字符串形式的IP "192.168.102.169"转换为IPV4的32位无符号整型数的IP
b. 将无符号整型数的ip，从主机端序转为网络端序
参数：字符串形式的ip
返回值：永远成功，返回网络端序的、32位无符号整型数的ip

```
/**
  * 接受连接
  *
  * @param sockfd 上述listen函数指定的监听socket
  * @param addr   请求连接方（即客户端）地址
  * @param addrlen 客户端地址长度
  * @return 函数执行成功返回一个新的连接socket，失败返回-1
*/
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

服务端工作流程如下：

调用socket函数创建套接字(socket)。

调用bind函数给创建的套接字，分配IP地址和端口(bind)。

调用listen函数进行监听，等待客户端连接(listen)。

等待客户请求到来: 当请求到来后，调用accept函数接受连接请求，返回一个对应于此次连接的新的套接字，做好相互通信准备（accept）。

调用write/read或send/recv进行数据的读写，通过accept返回的套接字和客户端进行通信.

关闭socket(close)。

客户端工作流程如下：

调用socket函数创建套接字(socket)。

调用connect函数连接服务端(connect)。

调用write/read或send/recv进行数据的读写。

关闭socket(close)。

 recv函数

 函数原型：int recv( SOCKET s, char *buf, int len, int flags)

 功能：不论是客户还是服务器应用程序都用recv函数从TCP连接的另一端接收数据。

 参数一：指定接收端套接字描述符；

 参数二：指明一个缓冲区，该缓冲区用来存放recv函数接收到的数据；

 参数三：指明buf的长度；

 参数四 ：一般置为0。

返回读到的字节
