主要内容

1. 什么是Lora？

2. LoRa的主要特点？

3. LoRa的系统架构？

4. LoRa终端节点的网络连接方式？

5. LoRa终端节点的三种工作模式？

6. LoRa的协议栈？
1. 什么是LoRa
![](https://s2.51cto.com/images/blog/202107/26/da1da5695d6c51a29db0327d77ba7723.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_lora

LoRa是semtech公司创建的低功耗、远距离、无线、广域网的标准。

LoRa的名字是远距离（Long Range）的简称。

低功耗一般很难覆盖远距离，远距离一般功耗高，要想马儿不吃草还要跑得远，好像难以办到。而LoRa的最大特点就是在同样的功耗条件下比其他无线方式传播的距离更远，实现了低功耗和远距离的统一，它在同样的功耗下比传统的无线射频通信距离扩大3-5倍。与之对应的无线广域网通信技术有：GPRS、NB IoT、Sigfox；与之对应的无线局域网通信技术有：RFID、蓝牙、ZigeBee、WiFi。
2. LoRa的主要特点

低速率（带宽）：低频谱带宽，传输速率从几百到几十Kbps，速率越低传输距离越长，这很像一个人挑东西，挑的多走不太远，少了可以走远。

低功耗（功率）：信号的发送功率超低，电池寿命长达10年，若采用蜂窝网络则需频繁更换电池，这在很多恶劣环境下很难实现。

远距离（空间）：传输距离远，城镇可达2-5 Km ， 郊区可达15 Km 。

大连接（数量）：LoRa网关可以连接上千上万个LoRa节点。

非频发（时间）：适合不需要连续发送数据包的的场合，

高延时（时间）：数据的传输延时较大。

低费用（金钱）：采用免费的频谱资源、单次较低数据量的数据传输、较低的传输频次，使得LoRa的运营成本较低。目前蜂窝网络为高带宽设计，采用蜂窝网络要占用网络和码号资源，还会产生包月流量费用。

安全性：采用AES128加密。

工作频率：ISM 频段 包括433、868、915 MH等。

调制方式：基于semtech公司私有专利的扩频技术，线性调制扩频（CSS）的一个变种，具有前向纠错（FEC）能力，这是实现低功耗远距离的关键。

标准：IEEE 802.15.4g。
3. LoRa的系统架构

如下是LoRa在通用的物联网架构中的位置：
![](https://s2.51cto.com/images/blog/202107/26/32876b7462ae98bee04c6acb3606fcc9.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)

物联网LoRa系列-2：LoRa系统架构与协议栈详解_2/3/4/5g_02

LoRa：是无线接入技术，在物联网架构中，LoRa处于物联网的无线终端感知层和网络传输层。因此LoRa本身并不是物联网的全部，只是物联网的一部分。

LoRa终端：通过传感器采集物体的信息，并通过LoRa无线空口技术，把数据传输给LoRa无线基站。

LoRa无线基站：提供对LoRa终端的无线接入和汇聚，LoRa无线基站通过以太局域网或2G/4G/5G的公共移动通信网，连接到LoRaWAN广域网服务器。

LoRaWAN服务器：可以管理多个LoRa无线基站，同时充当LoRa终端连接物联网云平台的网关。

 

如下是LoRa联盟推荐的的LoRa网络架构：
![](https://s2.51cto.com/images/blog/202107/26/edcd6167da1523232a88088e4431e560.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_2/3/4/5g_03

LoRa Node：LoRa终端节点

Concentrator：实现对LoRa终端节点提供接入和汇聚功能个的LoRa基站。

Network Server：LoRa的核心网，用于管理LoRa网络中所有的LoRa节点。

Application Server：由不同业务领域的服务器组成，并通过Web或手机接入的方式向用户提供业务服务。

与通用物联网架构的区别是，在此架构中，没有一个显式的、支持各种物联网无线接入的、通用的物联网云平台层。该云平台可以从Application Server中分离出来，处于Network Server和Application Server之间。可以与Network Server一起部署。
4. LoRa终端节点的网络连接方式：

（1）点对点通信。
![](https://s2.51cto.com/images/blog/202107/26/9bce245069ad09af61dc795674fc87f6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
一点对一点通信，A点发起，B点接收，可以回复也可以不回复确认，多组之间的频点建议分开。如下午图所示：

物联网LoRa系列-2：LoRa系统架构与协议栈详解_lora_04

这种连接方式单纯利用LoRa射频低功耗、远距离传输的功能，目前主要针对特定应用和试验性质的项目。

优点在于最简单，

缺点在于不存在组网。

 

（2）星状网轮询。
![](https://s2.51cto.com/images/blog/202107/26/b468b98ea41a578c7bb51adf6d1b4a94.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
一点对多点通信，N个“从节点”轮流与“中心节点”通信，从节点上传，等待中心点收到后返回确认，然后下一个节点再开始上传，直到所有N个节点全部完成，一个循环周期结束。如下图所示。

物联网LoRa系列-2：LoRa系统架构与协议栈详解_物联网_05

该结构本质上还属于点对点通信，但是加入了分时处理，N个从节点之间的频点可以分开，也可重复使用。

优势在于单项目成本低，

不足之处是仅适合从节点数量不大和网络实时性要求不高的应用。

 

（3）星状网并发。

如图下图，一点对多点通信：
![](https://s2.51cto.com/images/blog/202107/26/b94e0a8515326e756c1299bfb04879b5.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_2/3/4/5g_06

这里：网关、LoRa Ap 、LoRa基站是同一个概念。

多个从节点可同时与中心点通信，从节点可随机上报数据，节点可以根据外界环境和信道阻塞自动采取跳频和速率自适应技术，逻辑上，网关/可以接收不同速率和不同频点的信号组合，物理上网关可以同时接收8路、16路、32路甚至更多路数据，减少了大量节点上行时冲突的概率。

该系统具有极大的延拓性，可单独建网，可交叉组网，这是LoRa目前最主要、最主流的组网形态！

 ![](https://s2.51cto.com/images/blog/202107/26/193fe4a34d673ed22a18b77264c89af3.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)

物联网LoRa系列-2：LoRa系统架构与协议栈详解_物联网_07
5. LoRa终端节点的三种工作模式：用好LoRa终端的关键！

为了更好的适应物联网低功耗的需求，设计了三种LoRa终端的工作模式： Class A、Class B、Class C.

Class A的终端，长期处于休眠状态，最省电。只能在唤醒后且发送数据后的2个接收时隙，接收服务器的指示和响应，因此不能实时响应服务器的控制。

Class B的终端，间隙性的休眠，间隙性的唤醒，唤醒后，并不一定发送数据，但打开在特定的时隙打开接收窗口，接收来自于服务器的指令指示。

Class C的终端，实时在线，在每个接收时隙周期里都能够接收来自服务器的指示。因此这种模式的终端最耗电。

 
Class A：功耗最低的模式

Class A 上下行时隙图：
![](https://s2.51cto.com/images/blog/202107/26/8f408970cc7895c37c3abc5b0a59706b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_lora_08

终端的接收窗口RX1，一般是在终端的上行发送Transmit后1秒开始，

终端的接收窗口RX2, 一般是在终端的上行发送Transmit后2秒开始。

也就是，终端接收服务器的数据窗口，是在终端有数据上报时，才顺便看一下是否有服务器下发来的指令。其他时间，终端都处于休眠状态，无法接收服务器的指令。

 

终端与基站的通信过程：
![](https://s2.51cto.com/images/blog/202107/26/49f403be53b642a1af77a1d761908f21.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_2/3/4/5g_09

类A设备实现一个双向的通信，即每个端设备的上行传输后跟着两个短小下行接收窗口。

端设备安排好的传输时隙是基于它自己的通信需要的，该需要是有一个基于随机时间基的小变化的。

类A操作是应用的最低功耗选择，在端设备已经发送一个上行传输后该应用立刻只从服务器获取下行通信。在任意其他时间的来自服务器的下行通信必须等待下次安排好的上行通信。类A涵盖了绝大多数用例，是LoRa功率最高效的模式。
 

 
Class B：应用最广泛的模式

Class B 上下行时隙图：
![](https://s2.51cto.com/images/blog/202107/26/88e99882fa933bb765d9607da75debdb.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_2/3/4/5g_10

lass B 的时隙则复杂一些，它有一个同步时隙beacon，还有一个固定周期的接收窗口ping时隙。

如这个示例中，beacon周期为128秒，ping周期为32秒。

看起来复杂，但实际上就跟NB-IoT的eDRX模式差不多，隔几十秒打开接收窗口收一个数据。其他时候都在休眠。

这种工作模式，终端功耗低、服务器下发数据延时也不大。既不用接电，也能够“还算及时”的控制设备。因此适合定位器、开关等场景。

 

终端与基站的通信过程：
![](https://s2.51cto.com/images/blog/202107/26/4b3a4e78c2cae0a2f8e2f2b52bf0d573.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_2/3/4/5g_11

LoRaWAN类B选项的终端，周期性的唤醒：

每128s唤醒后与基站同步一次，称为beacon周期。

每32s打开接收窗口，在周期性的时隙期间，它允许设备在一个可预知的时隙上，打开一个简短额外的接收窗口（称为“ping slot”），接收来自于网关服务器的消息。

 
Class C：响应最及时的模式

Class C 上下行时隙图：
![](https://s2.51cto.com/images/blog/202107/26/340ab6802fef90bff7c0994c7e9f0fc5.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_lora_12

Class C 和 A 基本是相同的，

几乎随时都可以接收数据。只干活不睡觉，功耗自然就高了。考虑到10mA级别的接收电流，不是长供电的设备最好别用这个模式。

 

终端与基站的通信过程：
![](https://s2.51cto.com/images/blog/202107/26/31d8076dfe40845f3b1fe2ae4c8bb349.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_物联网_13

该模式的LoRa终端，不休眠，除了发送数据时，处于发送状态不能接收数据外，其他时间都处于接收状态，在任何时隙都能够接收来自于服务器的消息和命令指示。实时性最高，最耗电。

 
汇总比较：
![](https://s2.51cto.com/images/blog/202107/26/049527b450516020a3130d03d4d5dd77.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_lora_14

 
6. LoRa的协议栈

（1）LoRa终端的协议栈：
![](https://s2.51cto.com/images/blog/202107/26/5e31125d0782ed58e743cc53a7bb72f7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_物联网_15

RF层：最底层RF射频层（Regional ISM, 区域性免费频段)，支持的频段有欧洲的US868、EU433; 美国的US915, 亚洲的AS430；这里的数字表示频率。 这层的协议通常是由semtech公司的RFIC芯片实现的。

L1层：LoRa Modulation（模式调试与编码），实现对数字信号的无线编码调制。包括扩频编码调制与移频键控编码调制FSK。 这层的协议通常是由semtech公司的RFIC芯片实现的。

L2层：LoRa MAC（MAC层）：实现LoRa终端的无线链路管理，包括上述描述的终端的三种工作模式的管理。同时还定义了MAC层的数据包封装格式。

后续会详细介绍RF层, L1层, L2层的协议细节。

 

（2）LoRa系统各个节点的协议栈
![](https://s2.51cto.com/images/blog/202107/26/b9c630c4ba6d9d1853d7068bf3862ab6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)
物联网LoRa系列-2：LoRa系统架构与协议栈详解_2/3/4/5g_16

物联网平台层与物联网应用层不在此文中讨论。
-----------------------------------
©著作权归作者所有：来自51CTO博客作者文火冰糖的硅基工坊的原创作品，请联系作者获取转载授权，否则将追究法律责任
物联网LoRa系列-2：LoRa系统架构与协议栈详解
https://blog.51cto.com/u_11299290/3196319






node:
ChirpStack 默认帐户密码是:
admin
admin