一.MQTT是什么

MQTT（Message Queuing Telemetry Transport，消息队列遥测传输协议），是一种基于发布/订阅（publish/subscribe）模式的"轻量级"通讯协议，该协议构建于TCP/IP协议上，由IBM在1999年发布。MQTT最大优点在于，可以以极少的代码和有限的带宽，为连接远程设备提供实时可靠的消息服务。作为一种低开销、低带宽占用的即时通讯协议，使其在物联网、小型设备、移动应用等方面有较广泛的应用。

MQTT是一个基于客户端-服务器的消息发布/订阅传输协议。MQTT协议是轻量、简单、开放和易于实现的，这些特点使它适用范围非常广泛。在很多情况下，包括受限的环境中，如：机器与机器（M2M）通信和物联网（IoT）。其在，通过卫星链路通信传感器、偶尔拨号的医疗设备、智能家居、及一些小型化设备中已广泛使用。在这里插入图片描述
MQTT简介：https://www.runoob.com/w3cnote/mqtt-intro.html
二.mosquitto库

mosquitto是一款实现了消息推送协议 MQTT v3.1 的开源消息代理软件，提供轻量级的，支持可发布/可订阅的的消息推送模式，使设备对设备之间的短消息通信变得简单。
在实验中使用mosquitto库函数来实现订阅与发布。
mosquitto库函数源码：https://mosquitto.org/api/files/mosquitto-h.html
1.Ubuntu下安装mosquitto

1.wget下载源码包

wget http://mosquitto.org/files/source/mosquitto-1.5.5.tar.gz

    1

2.解压

tar -xzvf mosquitto-1.5.5.tar.gz

    1

3.进入目录

cd mosquitto-1.5.5/

    1

4.编译

make

    1

5.运行安装

sudo make install

    1

可能出现问题及解决方法

【1】编译找不到openssl/ssl.hsudo apt-get install libssl-dev

【2】编译过程g++命令未找到：sudo apt-get install g++

【3】编译过程找不到ares.hsudo apt-get install libc-ares-dev

【4】编译过程找不到uuid/uuid.hsudo apt-get install uuid-dev

【5】使用过程中找不到libmosquitto.so.1 error while loading shared libraries: libmosquitto.so.1: cannot open shared object file: No such file or directory
【解决方法】——修改libmosquitto.so位置
创建链接sudo ln -s /usr/local/lib/libmosquitto.so.1 /usr/lib/libmosquitto.so.1
更新动态链接库sudo ldconfig
参考：https://blog.csdn.net/caijiwyj/article/details/86671211
2.测试mosquitto订阅与发布

1.mosquitto broker配置
查看mosquitto配置文件在mosquitto文件夹下vim mosquitto.conf
mosquitto配置文件默认即可。如有需求可按需修改。
2.启动
mosquitto [-c config file] [ -d | --daemon ] [-p port number] [-v]
参数说明：
-c 后面跟的是启动mosquitto可以调整的参数，比如是否开启基本认证，端口是什么，SSL单向和双向的认证配置等等。
-d 表示MQTT mosquitto将在后台运行。
-p 代表当前的mosquitto服务实例启动以后，其监听端口号，这个配置的覆盖[-c config file] 指定的配置文件中的端口。
-v 代码调试模式（verbose）在终端输出更多的信息 。
3.发布

mosquitto_sub 命令参数说明
1. -c  设定‘clean session’为无效状态，这样一直保持订阅状态，即便是已经失去连接，如果再次连接仍旧能够接收的断开期间发送的消息。
2. -d  打印debug信息
3. -h  指定要连接的域名  默认为localhost
4. -i 指定clientId
5. -I 指定clientId前缀
6. -k keepalive 每隔一段时间，发PING消息通知broker，仍处于连接状态。 默认为60秒。
7. -q 指定希望接收到QoS为什么的消息  默认QoS为0
8. -R 不显示陈旧的消息
9. -t 订阅topic
10. -v 打印消息
11. --will-payload  指定一个消息，该消息当客户端与broker意外断开连接时发出。该参数需要与--will-topic一起使用
12. --will-qos  Will的QoS值。该参数需要与--will-topic一起使用
13. --will-retain 指定Will消息被当做一个retain消息（即消息被广播后，该消息被保留起来）。该参数需要与--will-topic一起使用
14. --will-topic  用户发送Will消息的topic

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16

4.订阅

mosquitto_pub 命令参数说明
1. -d  打印debug信息
2. -f  将指定文件的内容作为发送消息的内容
3. -h  指定要连接的域名  默认为localhost
4. -i  指定要给哪个clientId的用户发送消息
5. -I  指定给哪个clientId前缀的用户发送消息
6. -m  消息内容
7. -n  发送一个空（null）消息
8. -p  连接端口号
9. -q  指定QoS的值（0,1,2）
10. -t  指定topic
11. -u  指定broker访问用户
12. -P  指定broker访问密码
13. -V  指定MQTT协议版本
14. --will-payload  指定一个消息，该消息当客户端与broker意外断开连接时发出。该参数需要与--will-topic一起使用
15. --will-qos  Will的QoS值。该参数需要与--will-topic一起使用
16. --will-retain 指定Will消息被当做一个retain消息（即消息被广播后，该消息被保留起来）。该参数需要与--will-topic一起使用
17. --will-topic  用户发送Will消息的topic

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19

5.关闭

# 查看mosquitto服务进程ID
ps -aux | grep mosquitto
 
# 杀掉服务进程
# PID 为上一步查找的ID
kill -9 PID

    1
    2
    3
    4
    5
    6
    7

3.使用mosquitto库函数实现上述订阅与发布

mosquitto库的API：https://mosquitto.org/api/files/mosquitto-h.html
下面介绍常用的一些函数，剩下的API读者可依据上述链接自行学习。

int mosquitto_lib_init(void)
//功能：使用mosquitto库函数前，要先初始化，使用之后就要清除。清除函数；int mosquitto_lib_cleanup();
//返回值：MOSQ_ERR_SUCCESS 总是

    1
    2
    3
    4

int mosquitto_lib_cleanup（void）
//功能：使用完mosquitto函数之后，要做清除工作。
//返回值： MOSQ_ERR_SUCCESS 总是

    1
    2
    3

struct mosquitto *mosquitto_new( const char * id, bool clean_session, void * obj )

/*功能：创建一个新的mosquitto客户端实例，新建客户端。

参数：①id ：用作客户端ID的字符串。如果为NULL，将生成一个随机客户端ID。如果id为NULL，clean_session必须为true。

②clean_session：设置为true以指示代理在断开连接时清除所有消息和订阅，设置为false以指示其保留它们，客户端将永远不会在断开连接时丢弃自己的传出消息。调用mosquitto_connect或mosquitto_reconnect将导致重新发送消息。使mosquitto_reinitialise将客户端重置为其原始状态。如果id参数为NULL，则必须将其设置为true。
简言之：就是断开后是否保留订阅信息true/false

③obj： 用户指针，将作为参数传递给指定的任何回调，（回调参数）

返回：成功时返回结构mosquitto的指针，失败时返回NULL，询问errno以确定失败的原因：
ENOMEM内存不足。
EINVAL输入参数无效。

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15

void mosquitto_destroy( struct mosquitto * mosq )

/*功能：释放客户端

参数：mosq： struct mosquitto指针

    1
    2
    3
    4
    5
    6

void mosquitto_connect_callback_set(struct mosquitto * mosq, void (*on_connect)(struct mosquitto *mosq, void *obj, int rc) )

/*功能：连接确认回调函数，当代理发送CONNACK消息以响应连接时，将调用此方法。

参数：mosq： struct mosquitto指针
void (*on_connect)(struct mosquitto *mosq , void *obj, int rc)  回调函数 （参数：
mosq： struct mosquitto指针
obj：mosquitto_new中提供的用户数据
rc： 连接响应的返回码，其中有：
0-成功
1-连接被拒绝（协议版本不可接受）
2-连接被拒绝（标识符被拒绝）
3-连接被拒绝（经纪人不可用）
4-255-保留供将来使用
）

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16

void mosquitto_disconnect_callback_set( struct mosquitto *mosq,void (*on_disconnect)( struct mosquitto *mosq,void *obj, int rc) );

/*功能：断开连接回调函数，当代理收到DISCONNECT命令并断开与客户端的连接，将调用此方法。
参数：mosq： struct mosquitto指针
void (*on_connect)(struct mosquitto *mosq , void *obj, int rc)  回调函数 （参数：
mosq： struct mosquitto指针
obj：mosquitto_new中提供的用户数据
rc：0表示客户端已经调用mosquitto_disconnect，任何其他值，表示断开连接时意外的。）

    1
    2
    3
    4
    5
    6
    7
    8
    9

int mosquitto_connect( struct mosquitto * mosq, const char * host, int port, int keepalive )

/*功能: 连接到MQTT代理/服务器（主题订阅要在连接服务器之后进行）
参数：①mosq ： 有效的mosquitto实例，mosquitto_new（）返回的mosq.
②host : 服务器ip地址
③port：服务器的端口号
④keepalive：保持连接的时间间隔， 单位秒。如果在这段时间内没有其他消息交换，则代理应该将PING消息发送到客户端的秒数。
返回：MOSQ_ERR_SUCCESS 成功。
MOSQ_ERR_INVAL 如果输入参数无效。
MOSQ_ERR_ERRNO 如果系统调用返回错误。变量errno包含错误代码

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11

int mosquitto_disconnect( struct mosquitto * mosq )

/*功能：断开与代理/服务器的连接。
返回：
MOSQ_ERR_SUCCESS 成功。
MOSQ_ERR_INVAL 如果输入参数无效。
MOSQ_ERR_NO_CONN 如果客户端未连接到代理。

    1
    2
    3
    4
    5
    6
    7
    8

int mosquitto_publish( struct mosquitto * mosq, int * mid, const char * topic, int payloadlen, const void * payload, int qos, bool retain )

/*功能：主题发布的函数
参数：①mosq：有效的mosquitto实例，客户端
②mid：指向int的指针。如果不为NULL，则函数会将其设置为该特定消息的消息ID。然后可以将其与发布回调一起使用，以确定何时发送消息。请注意，尽管MQTT协议不对QoS = 0的消息使用消息ID，但libmosquitto为其分配了消息ID，以便可以使用此参数对其进行跟踪。
③topic：要发布的主题，以null结尾的字符串
④payloadlen：有效负载的大小（字节），有效值在0到268，435，455之间；主题消息的内容长度
⑤payload： 主题消息的内容，指向要发送的数据的指针，如果payloadlen >0，则它必须时有效的存储位置。
⑥qos：整数值0、1、2指示要用于消息的服务质量。
⑦retain：设置为true以保留消息。
返回:
MOSQ_ERR_SUCCESS 成功。
MOSQ_ERR_INVAL 如果输入参数无效。
MOSQ_ERR_NOMEM 如果发生内存不足的情况。
MOSQ_ERR_NO_CONN 如果客户端未连接到代理。
MOSQ_ERR_PROTOCOL 与代理进行通信时是否存在协议错误。
MOSQ_ERR_PAYLOAD_SIZE 如果payloadlen太大。
MOSQ_ERR_MALFORMED_UTF8 如果主题无效，则为UTF-8
MOSQ_ERR_QOS_NOT_SUPPORTED 如果QoS大于代理支持的QoS。
MOSQ_ERR_OVERSIZE_PACKET 如果结果包大于代理支持的包。

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21

int mosquitto_subscribe( struct mosquitto * mosq, int * mid, const char * sub, int qos )
/*功能：订阅主题函数
参数：①mosq：有效的mosquitto实例，客户端
②mid： 指向int的指针。如果不为NULL，则函数会将其设置为该特定消息的消息ID。然后可以将其与订阅回调一起使用，以确定何时发送消息。；主题的消息ID
③sub： 主题名称，订阅模式。
④qos : 此订阅请求的服务质量。
返回值：
MOSQ_ERR_SUCCESS 成功。
MOSQ_ERR_INVAL 如果输入参数无效。
MOSQ_ERR_NOMEM 如果发生内存不足的情况。
MOSQ_ERR_NO_CONN 如果客户端未连接到代理。
MOSQ_ERR_MALFORMED_UTF8 如果主题无效，则为UTF-8
MOSQ_ERR_OVERSIZE_PACKET 如果结果包大于代理支持的包。

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14

void mosquitto_message_callback_set( struct mosquitto * mosq, void (*on_message)(struct mosquitto *, void *, const struct mosquitto_message *) )

/*功能：消息回调函数，收到订阅的消息后调用。

参数：①mosq： 有效的mosquitto实例，客户端。

②on_message 回调函数，格式如下：void callback（struct mosquitto * mosq，void * obj，const struct mosquitto_message * message）
回调的参数：
①mosq：进行回调的mosquitto实例

②obj： mosquitto_new中提供的用户数据

③message: 消息数据，回调完成后，库将释放此变量和关联的内存，客户应复制其所需要的任何数据。

struct mosquitto_message{
int mid;//消息序号ID
char *topic; //主题
void *payload; //主题内容 ，MQTT 中有效载荷
int payloadlen; //消息的长度，单位是字节
int qos; //服务质量
bool retain; //是否保留消息
};

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23

int mosquitto_loop_forever( struct mosquitto * mosq, int timeout, int max_packets )

/*功能：此函数在无限阻塞循环中为你调用loop（），对于只想在程序中运行MQTT客户端循环的情况，这很有用，如果服务器连接丢失，它将处理重新连接，如果在回调中调用mosqitto_disconnect（）它将返回。

参数：①mosq: 有效的mosquitto实例，客户端

②timeout： 超时之前，在select（）调用中等待网络活动的最大毫秒数，设置为0以立即返回，设置为负可使用默认值为1000ms。

③max_packets： 该参数当前未使用，应设为为1，以备来兼容

返回值：
MOSQ_ERR_SUCCESS 成功。
MOSQ_ERR_INVAL 如果输入参数无效。
MOSQ_ERR_NOMEM 如果发生内存不足的情况。
MOSQ_ERR_NO_CONN 如果客户端未连接到代理。
MOSQ_ERR_CONN_LOST 如果与代理的连接丢失。
MOSQ_ERR_PROTOCOL 与代理进行通信时是否存在协议错误。
MOSQ_ERR_ERRNO 如果系统调用返回错误。变量errno包含错误代码

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19

int mosquitto_loop_stop( struct mosquitto * mosq, bool force )

/*功能:网络事件阻塞回收结束处理函数，这是线程客户端接口的一部分。调用一次可停止先前使用mosquitto_loop_start创建的网络线程。该调用将一直阻塞，直到网络线程结束。为了使网络线程结束，您必须事先调用mosquitto_disconnect或将force参数设置为true。

参数：①mosq :有效的mosquitto实例

②force：设置为true强制取消线程。如果为false，则必须已经调用mosquitto_disconnect。

返回：
MOSQ_ERR_SUCCESS 成功。
MOSQ_ERR_INVAL 如果输入参数无效。
MOSQ_ERR_NOT_SUPPORTED 如果没有线程支持。

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13

int mosquitto_loop_start( struct mosquitto * mosq )
/*功能：网络事件循环处理函数，通过创建新的线程不断调用mosquitto_loop() 函数处理网络事件，不阻塞

返回：
MOSQ_ERR_SUCCESS 成功。
MOSQ_ERR_INVAL 如果输入参数无效。
MOSQ_ERR_NOT_SUPPORTED 如果没有线程支持。

    1
    2
    3
    4
    5
    6
    7
    8

在这里插入图片描述
发布端源码：

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mosquitto.h"
#include <errno.h>

#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512


static int running = 1;


void my_connect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function: my_connect_callback.\n");
}

void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function: my_disconnect_callback.\n");
    running = 0;

}


void my_publish_callback(struct mosquitto *mosq,void *obj,int mid)
{
    printf("Call the function: my_publish_callback.\n");

}

int main (int argc, char **argv)
{

    int rv;
    struct mosquitto *mosq;
    char buff[MSG_MAX_SIZE];

    mosquitto_lib_init();

    mosq = mosquitto_new("pub_test",true,NULL);
    if(mosq == NULL)
    {
        printf("New pub_test error: %s\n",strerror(errno));
        mosquitto_lib_cleanup();
        return -1;
    }

    mosquitto_connect_callback_set(mosq,my_connect_callback);
    mosquitto_disconnect_callback_set(mosq,my_disconnect_callback);
    mosquitto_publish_callback_set(mosq,my_publish_callback);

    rv = mosquitto_connect(mosq,HOST,PORT,KEEP_ALIVE);

    if(rv)
    {
        printf("Connect server error: %s\n",strerror(errno));
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return -1;
    }

    printf("Start!\n");

    int loop = mosquitto_loop_start(mosq);
    if(loop)
    {

        printf("mosquitto loop error: %s\n",strerror(errno));
        return 1;
    }


    while(fgets(buff,MSG_MAX_SIZE,stdin)!=NULL)
    {
        mosquitto_publish(mosq,NULL,"test",strlen(buff)+1,buff,0,0);
        memset(buff,0,sizeof(buff));
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    printf("End!\n");

    return 0;
} 


    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27
    28
    29
    30
    31
    32
    33
    34
    35
    36
    37
    38
    39
    40
    41
    42
    43
    44
    45
    46
    47
    48
    49
    50
    51
    52
    53
    54
    55
    56
    57
    58
    59
    60
    61
    62
    63
    64
    65
    66
    67
    68
    69
    70
    71
    72
    73
    74
    75
    76
    77
    78
    79
    80
    81
    82
    83
    84
    85
    86
    87
    88
    89
    90
    91

订阅端源码：


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mosquitto.h"


#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512

static int running = 1;

void my_connect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function:on_connect\n");

    if(rc)
    {
        printf("on_connect error!\n");
        exit(1);
    }
    else
    {
        if(mosquitto_subscribe(mosq,NULL,"test",2))
        {
            printf("Set the topic error!\n");
            exit(1);
        }
        
    }
}


void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function: my_disconnect_callback\n");
    running = 0;
}

void my_subscribe_callback(struct mosquitto *mosq,void *obj,int mid,int qos_count,const int *granted_qos)
{
    printf("Call the function: on_subscribe\n");
}

void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    printf("Call the function: on_message\n");
    printf("Recieve a message of %s: %s\n.",(char *)msg->topic,(char *)msg->payload);
    if(0 == strcmp(msg->payload,"quit")){
    
        mosquitto_disconnect(mosq);
    }
}




int main (int argc, char **argv)
{
    int ret;
    struct mosquitto *mosq;

    ret = mosquitto_lib_init();

    mosq = mosquitto_new("sub_test",true,NULL);
    if(mosq == NULL)
    {
        printf("New sub_test error!\n");
        mosquitto_lib_cleanup();
        return -1;
    }

    
    mosquitto_connect_callback_set(mosq,my_connect_callback);
    mosquitto_disconnect_callback_set(mosq,my_disconnect_callback);
    mosquitto_subscribe_callback_set(mosq,my_subscribe_callback);
    mosquitto_message_callback_set(mosq,my_message_callback);

    ret = mosquitto_connect(mosq,HOST,PORT,KEEP_ALIVE);

    if(ret)
    {
        printf("Connect server error!\n");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return -1;
    }

    printf("Start!\n");

    while(running)
    {
        mosquitto_loop(mosq,-1,1);
        //mosquitto_loop_start(mosq);


    }


    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();


    printf("End!\n");

    return 0;
} 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27
    28
    29
    30
    31
    32
    33
    34
    35
    36
    37
    38
    39
    40
    41
    42
    43
    44
    45
    46
    47
    48
    49
    50
    51
    52
    53
    54
    55
    56
    57
    58
    59
    60
    61
    62
    63
    64
    65
    66
    67
    68
    69
    70
    71
    72
    73
    74
    75
    76
    77
    78
    79
    80
    81
    82
    83
    84
    85
    86
    87
    88
    89
    90
    91
    92
    93
    94
    95
    96
    97
    98
    99
    100
    101
    102
    103
    104
    105
    106
    107
    108
    109
    110

makefie：

all:
    gcc -o sub sub.c -lmosquitto
    gcc -o pub pub.c -lmosquitto
sub:
    gcc -o sub sub.c -lmosquitto 
pub:
    gcc -o pub pub.c -lmosquitto 
clean:
    -rm sub pub

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10

三.mosquitto库运行实例

启动mosquitto服务器

mosquitto -v

    1

运行结果：
在这里插入图片描述
————————————————
版权声明：本文为CSDN博主「Frank-Hu」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/hyz529900/article/details/124687845