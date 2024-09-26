# lora_wan 服务搭建记录

## 目录
- LoRa

- LoRaWAN

- sx1303

- packet_forwarder

- chirpstack



## LoRa

LoRa 是一种长距离、低功耗的物联网 (IoT) 无线平台,一般情况下泛指使用LoRa技术的射频芯片.主要特点如下：

- LoRa（long range 的缩写）采用的扩频调制技术源于啁啾扩频 (CSS)  技术,是远距离无线传输技术和LPWAN通信技术中的一种.扩频技术用带宽换取灵敏度的技术,Wi-Fi,ZigBee等都使用了扩频技术,但LoRa调制的特点是接近香农定理的极限,最大效率地提高灵敏度.相比于传统FSK技术,在相同的通信速率下,LoRa比FSK灵敏度好8~12dBm.目前,LoRa 主要在Sub-GHz的ISM频段运行,
- LoRa技术融合了数字扩频、数字信号处理和前向纠错编码等技术使得在长距离通信性能层面大幅度提高,LoRa的链路预算优于其他任何标准化的通信技术,链路预算是指给定的环境中决定距离的主要因素.
- LoRa射频芯片主要有 SX127X系列，SX126X系列，SX130X系列，其中SX127X，SX126X系列用于LoRa节点，SX130X用于LoRa网关

## LoRaWAN

- [LoRaWAN](https://lora-alliance.org/about-lorawan/) 是一种建立在 LoRa  无线电调制技术之上的低功耗广域网开放协议。旨在将电池供电的“事物”无线连接到区域、国家或全球网络中的互联网，并针对关键的物联网 (IoT)  要求，例如双向定向通信、端到端安全、移动性和本地化服务。其中节点无线连接到互联网有入网认证，相当于建立节点和服务器间的加密通信信道，LoRaWAN协议层次下图所示。

  - MAC层中的Class A/B/C 三类节点设备基本覆盖了物联网所有的应用场景，三者之间不同点在于节点收发的时隙不同
  - 
  - - Modulation层中EU868，AS430等表明不同国家使用频段参数不同，地区参数请点击参考[链接](https://lora-alliance.org/wp-content/uploads/2019/11/rp_2-1.0.0_final_release.pdf)

  [![SX1262-LoRa-HAT-021.png](https://www.waveshare.net/w/upload/thumb/9/9d/SX1262-LoRa-HAT-021.png/800px-SX1262-LoRa-HAT-021.png)](https://www.waveshare.net/wiki/文件:SX1262-LoRa-HAT-021.png)

- 实现LoRaWAN网络覆盖城市或其它区域需要由节点（LoRa节点射频芯片）、网关（或称基站,LoRa网关射频芯片）、Server和云四部分组成，如下图所示
  - DEVICE（节点设备）需先发起入网请求数据包到GATEWAY（网关）再到服务器，认证通过后才可以正常和服务器收发应用数据
  - GATEWAY（网关）可通过有线网络，3/4/5G无线网络与服务器进行通信
  - 服务器端主要运营商有[TTN](https://eu1.cloud.thethings.network/console/)等，自行搭建云端服务请参考[lorawan-stack](https://github.com/TheThingsNetwork/lorawan-stack)，[chirpstack](https://www.chirpstack.io/)

![](https://www.waveshare.net/w/upload/thumb/c/c2/SX1268-LoRa-HAT-021.png/900px-SX1268-LoRa-HAT-021.png)

- 树莓派 Pico 和 Pico-LoRa-SX1262 通过 LoRaWAN 接入互联网有两种方式,一是  OTAA(Over-The-Air-Activation),二是 ABP(Activation By Personalization) ,  本文通过方式一 OTAA 入网, 入网过程参照下图所示 , 详细文档参考[链接1](https://lora-developers.semtech.com/documentation/tech-papers-and-guides/lora-and-lorawan/) [链接2](https://www.thethingsnetwork.org/docs/lorawan/end-device-activation/) 和[源码](https://github.com/Lora-net/LoRaMac-node)

  - 步骤1.终端设备将Join-Request消息发送到将要加入的网络,注意加入过程始终由终端设备发起,Join-Request消息可以使用任何数据速率并使用特定于区域的加入通道之一来传输.例如,在欧洲,终端设备可以通过在 868.10 MHz、868.30 MHz 或 868.50 MHz  中随机选择来发送Join-Request消息.Join-Request消息通过一个或多个网关到达网络服务器.另外注意按照当地无线电管理规定选择适用频段,频段分布具体表格点击[链接](https://lora-alliance.org/wp-content/uploads/2019/11/rp_2-1.0.0_final_release.pdf)查看或访问[LoRa Alliance](https://lora-alliance.org/)搜索, Join-Request消息由以下字段组成,而AppEUI,DevEUI由服务器端注册生成,请参考下面说明.
    - AppEUI: IEEE EUI64 地址空间中的 64 位全局唯一应用标识符，唯一标识能够处理 Join-Request 帧的实体。
    - DevEUI: IEEE EUI64 地址空间中唯一标识终端设备的 64 位全局唯一设备标识符。
    - DevNonce: 由终端设备生成的唯一的随机的 2 字节值.网络服务器使用每个终端设备的 DevNonce  来跟踪它们的加入请求.如果终端设备使用先前使用的 DevNonce 发送加入请求(这种情况称为replay  attack),网络服务器会拒绝加入请求并且不允许该终端设备向网络注册.
  - 步骤2.网络服务器处理Join-Request-Message.如果允许终端设备加入网络,网络服务器将生成两个会话密钥（NwkSKey 和  AppSKey）和 Join-accept 消息.然后使用 AppKey 对加入接受消息本身进行加密.网络服务器使用 ECB 模式下的 AES  解密操作来加密 Join-accept 消息.
  - 步骤3.网络服务器将加密的加入接受消息作为正常下行链路发送回终端设备.
  - 步骤4.终端设备使用AES解密Join-Accept.并使用 AppKey 和 AppNonce 生成两个会话密钥 NwkSKey 和  AppSKey用于后续和 Networking 服务器通信. Network Server 同样保存了 kSKey ,Join服务器分发  AppSKey 给 Application Server.

  ![](https://www.waveshare.net/w/upload/9/9a/Pico-LoRa-SX1262-868M_016.png)



## sx1303

本产品是一款使用新一代 SX1302/3 基带芯片的 LoRa 网关模组，具有传输距离远，节点容量大，和接收灵敏度高等特点，其中 SX1303  支持并发通信。SX1302/3 在低能耗方面的出色表现，使得网关可以简化自身的热设计，降低了材料成本和精简了尺寸。

- 板载 Semtech SX1302/3 基带处理器和 SX1250 射频前端芯片组
- 板载 PA 和 LNA，具有 +26dBm 发射功率和 -141dBm 接收高灵敏度
- SX1303 支持精细时间戳，可基于计算到达时间差 (TDOA) 进行网络定位
- 52 个金手指引脚的 Mini-PCIe 外形封装，方便集成到各种嵌入式系统
- 板载 4 个 LED 指示灯，方便查看模块运行状态
- 提供完善的配套资料手册 (C 示例程序和使用手册等)

![](https://www.waveshare.net/w/upload/thumb/0/03/SX1302_LoRaWAN_Gateway_HAT_008.jpg/800px-SX1302_LoRaWAN_Gateway_HAT_008.jpg)

微雪的sx1303设备存在多个版本，不同版本之间金手指可能存在接出不良的现象。

模块的引脚并没有完全使用，下面是启动摸块必备的引脚：

- SDA
- SCL
- TX
- RX
- D22
- D13
- D23
- D18
- CE0
- CE1
- MOSI
- MISO
- SCK

## packet_forwarder

packet forwarder 是一个开源的 LoRa 网关软件，用于将 LoRa 信息发送到 LoRa 服务器上。

```bash
git clone https://github.com/siuwahzhong/sx1302_hal.git
cd sx1302_hal
git checkout ws-dev
make clean all
make all
cp tools/reset_lgw.sh util_chip_id/
cp tools/reset_lgw.sh packet_forwarder/
```

注意：

- 修改 tools/reset_lgw.sh 配置与引脚相适配
- 修改 sx1302_hal/packet_forwarder/global_conf.json.sx1250.EU868 配置文件，使网关指向服务器。

## chirpstack 

LoRa 开源服务器，用于接受 LoRa 网关传上来的消息，对外提供 grpc api 和 mqtt 消息订阅。 chirpstack 的安装有两种方式，一种是 docker 安装，一种是手动安装。在 Core135 上采用的是手动安装。

chirpstack 有两个版本，V3 和 V4。最新的是 V4 版本，本次安装的也是 V4 版本。

V3 和 V4 版本有两点不同：

- V3 的组件更加分散，需要单独安装配置各自的组件。V4 集成度更高。
- V4 相对 V3 缺少了 REST API 。在V4 上以 grpc 方式提供 api 服务。

chirpstack 安装：

```bash
# 安装依赖组件
sudo apt install \
    mosquitto \
    mosquitto-clients \
    redis-server \
    redis-tools \
    postgresql

# 配置 postgres，数据库表
sudo -u postgres psql
```bash
-- create role for authentication
create role chirpstack with login password 'chirpstack';

-- create database
create database chirpstack with owner chirpstack;

-- change to chirpstack database
\c chirpstack

-- create pg_trgm extension
create extension pg_trgm;

-- exit psql
\q
```
# 安装 chirpstack 网关
sudo apt install apt-transport-https dirmngr

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1CE2AFD36DBCCA00

sudo echo "deb https://artifacts.chirpstack.io/packages/4.x/deb stable main" | sudo tee /etc/apt/sources.list.d/chirpstack.list

sudo apt update

sudo apt install chirpstack-gateway-bridge

# 配置 chirpstack
# 编辑 /etc/chirpstack-gateway-bridge/chirpstack-gateway-bridge.toml
# 配置 mqtt 话题
[integration.mqtt]
event_topic_template="eu868/gateway/{{ .GatewayID }}/event/{{ .EventType }}"
command_topic_template="eu868/gateway/{{ .GatewayID }}/command/#"
# 配置 mqtt 服务器


# 安装 ChirpStack
sudo apt install chirpstack
# 配置/etc/chirpstack/chirpstack.toml
# 配置 mqtt 服务器
```

使用：

 创建网关，创建设备配置文件，创建应用，在应用中添加设备。

将上述创建出的 eui 分别填入网关配置和设备 OTTA 配置中，然后上电就能在  ChirpStack 中看到设备上穿的信息。

## 问题

LoRaWAN 帧计数问题，当 LoRa 重发消息时，LoRa 服务器会拒绝 LoRa 的消息，从而导致 LoRa 设备重发失败。在服务器中会有重发帧警告，在设备端会持续的重发失败。

该现象是由设备发送 LoRaWAN 帧后没有收到服务器的确认帧，从而进行消息重发。服务器接收到重发消息时发现帧计数重复，从而拒绝接受此次的 LoRaWAN 帧。

解决方法有三种：

- 设备端不进行消息确认
- 设备端不进行消息重发
- 服务端禁用帧计数验证

![](../image/2024-03-25 15-07-19.png)


## NOTE
RAK2287 usb版
https://docs.rakwireless.com.cn/Product-Categories/WisLink/RAK2287/Quickstart/#rak2287-usb-spi-raspberry-pi






1、找到并打开Mosquitto服务器的配置文件 /etc/mosquitto/mosquitto.conf

2、将其中的配置选项allow_anonymous改为 false，禁止匿名登录

allow_anonymous false

3、将密码配置选项配置如下：

password_file /etc/mosquitto/pwfile

4、如果该目录下没有该文件，则进入该目录，并拷贝一份，命令如下：  

cp pwfile.example pwfile

5、添加用户信息。在终端执行以下代码，应用mosquitto_passwd命令创建用户名

mosquitto_passwd -c /etc/mosquitto/pwfile username

  执行以后会提示输入密码，重复2次输入之后，用户名密码配置完成。

6、重新启动mosquitto服务之后，用户名密码生效

mosquitto -c /etc/mosquitto/mosquitto.conf
————————————————

                            版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
                        
原文链接：https://blog.csdn.net/tuzirou/article/details/135480278






/etc/chirpstack-gateway-bridge/chirpstack-gateway-bridge.toml
``` txt
# This configuration provides a Semtech UDP packet-forwarder backend and
# integrates with a MQTT broker. Many options and defaults have been omitted
# for simplicity.
#
# See https://www.chirpstack.io/gateway-bridge/install/config/ for a full
# configuration example and documentation.


# Gateway backend configuration.
[backend]
# Backend type.
type="semtech_udp"

  # Semtech UDP packet-forwarder backend.
  [backend.semtech_udp]

  # ip:port to bind the UDP listener to
  #
  # Example: 0.0.0.0:1700 to listen on port 1700 for all network interfaces.
  # This is the listener to which the packet-forwarder forwards its data
  # so make sure the 'serv_port_up' and 'serv_port_down' from your
  # packet-forwarder matches this port.
  udp_bind = "0.0.0.0:1700"


# Integration configuration.
[integration]
# Payload marshaler.
#
# This defines how the MQTT payloads are encoded. Valid options are:
# * protobuf:  Protobuf encoding
# * json:      JSON encoding (easier for debugging, but less compact than 'protobuf')
marshaler="protobuf"

  # MQTT integration configuration.
  [integration.mqtt]
  # Event topic template.
  event_topic_template="eu868/gateway/{{ .GatewayID }}/event/{{ .EventType }}"

  # Command topic template.
  command_topic_template="eu868/gateway/{{ .GatewayID }}/command/#"

  # MQTT authentication.
  [integration.mqtt.auth]
  # Type defines the MQTT authentication type to use.
  #
  # Set this to the name of one of the sections below.
  type="generic"

    # Generic MQTT authentication.
    [integration.mqtt.auth.generic]
    # MQTT server (e.g. scheme://host:port where scheme is tcp, ssl or ws)
    server="tcp://127.0.0.1:1883"

    # Connect with the given username (optional)
    username="chirp"

    # Connect with the given password (optional)
    password="chirp"


```

/etc/chirpstack/chirpstack.toml
```txt
# Logging.
[logging]

  # Log level.
  #
  # Options are: trace, debug, info, warn error.
  level="info"


# PostgreSQL configuration.
[postgresql]

  # PostgreSQL DSN.
  #
  # Format example: postgres://<USERNAME>:<PASSWORD>@<HOSTNAME>/<DATABASE>?sslmode=<SSLMODE>.
  #
  # SSL mode options:
  #  * disable - no SSL
  #  * require - Always SSL (skip verification)
  #  * verify-ca - Always SSL (verify that the certificate presented by the server was signed by a trusted CA)
  #  * verify-full - Always SSL (verify that the certification presented by the server was signed by a trusted CA and the server host name matches the one in the certificate)
  dsn="postgres://chirpstack:chirpstack@localhost/chirpstack?sslmode=disable"

  # Max open connections.
  #
  # This sets the max. number of open connections that are allowed in the
  # PostgreSQL connection pool.
  max_open_connections=10

  # Min idle connections.
  #
  # This sets the min. number of idle connections in the PostgreSQL connection
  # pool (0 = equal to max_open_connections).
  min_idle_connections=0


# Redis configuration.
[redis]

  # Server address or addresses.
  #
  # Set multiple addresses when connecting to a cluster.
  servers=[
    "redis://localhost/",
  ]

  # Redis Cluster.
  #
  # Set this to true when the provided URLs are pointing to a Redis Cluster
  # instance.
  cluster=false


# Network related configuration.
[network]

  # Network identifier (NetID, 3 bytes) encoded as HEX (e.g. 010203).
  net_id="000000"

  # Enabled regions.
  #
  # Multiple regions can be enabled simultaneously. Each region must match
  # the 'name' parameter of the region configuration in '[[regions]]'.
  enabled_regions=["eu868"]
#  enabled_regions=[
#    "as923",
#    "as923_2",
#    "as923_3",
#    "as923_4",
#    "au915_0",
#    "cn470_10",
#    "cn779",
#    "eu433",
#    "eu868",
#    "in865",
#    "ism2400",
#    "kr920",
#    "ru864",
#    "us915_0",
#    "us915_1",
#  ]


# API interface configuration.
[api]

  # interface:port to bind the API interface to.
  bind="0.0.0.0:8080"

  # Secret.
  #
  # This secret is used for generating login and API tokens, make sure this
  # is never exposed. Changing this secret will invalidate all login and API
  # tokens. The following command can be used to generate a random secret:
  #   openssl rand -base64 32
  # secret="you-must-replace-this"
  secret="A+IQny8oHpsuFXpuocQISSkQKB+rI23TUxmpVx/65AE="


[integration]
  enabled=["mqtt"]

  [integration.mqtt]
    server="tcp://127.0.0.1:1883"
    json=true
    # Connect with the given username (optional)
    username="chirp"

    # Connect with the given password (optional)
    password="chirp"

```


/etc/mosquitto/mosquitto.conf
```txt
# Place your local configuration in /etc/mosquitto/conf.d/
#
# A full description of the configuration file is at
# /usr/share/doc/mosquitto/examples/mosquitto.conf.example

pid_file /run/mosquitto/mosquitto.pid

persistence true
persistence_location /var/lib/mosquitto/

log_dest file /var/log/mosquitto/mosquitto.log

include_dir /etc/mosquitto/conf.d
bind_address 0.0.0.0
password_file /etc/mosquitto/pwfile
```




# chirpstack 的 mqtt 接口进行数据发送与接收
2024年 06月 19日 星期三 17:43:43 CST  

应用从 chirpstack 中收发 lora 消息的主要方式是通过 MQTT 的方式，根据当前互联网中的教程，是很容易从 chirpstack 的收发主题中订阅到设备的上行消息，但是当前所有的下行教程，包括官网的下行 mqtt 消息都是有错误的。这导致几乎无法正常通过 mqtt 的方式向 loraWAN 设备发送消息，这是一个很大的坑，在之前为了解决这个问题，是通过使用 chirpstack 原生的 grpc 方式进行下发消息调用的， grpc 可以通过 python c++ 等方式进行编程，在交插编译 grpc 是一个很大工作量，同样的 python 在设备中运行也是一个不小的开销。为了在交插编译 grpc 并在设备上跑 grpc C++ 程序，我花了很大的功夫。在解决完交叉编译 grpc 的问题后，我决定看看为什么 mqtt 无法向 chirpstack 中发送消息的问题。  
先是在互联网中搜索相关的教程，但是都无法使用，甚至 chirpstack 官网的教程都无法使用，我有点怀疑 chirpstack 到底实现了这个功能了没有，所以只能去查看 chirpstack 的源码。
检查完源码后，发现 mqtt 的 enquene 消息体和官方描述的不一致：  
官方的 enquene 消息体：
```json
// https://www.chirpstack.io/application-server/integrations/mqtt/
{
    "confirmed": true,                        // whether the payload must be sent as confirmed data down or not
    "fPort": 10,                              // FPort to use (must be > 0)
    "data": "...."                            // base64 encoded data (plaintext, will be encrypted by ChirpStack Network Server)
    "object": {                               // decoded object (when application coded has been configured)
        "temperatureSensor": {"1": 25},       // when providing the 'object', you can omit 'data'
        "humiditySensor": {"1": 32}
    }
}
```
源码中的 enquene 消息体：  
```
{
    "id": "",                                 // UUID
    "dev_eui": "",                            // Device EUI (EUI64).
    "confirmed": true,                        // whether the payload must be sent as confirmed data down or not
    "fPort": 10,                              // FPort to use (must be > 0)
    "data": "...."                            // base64 encoded data (plaintext, will be encrypted by ChirpStack Network Server)
    "object": {                               // decoded object (when application coded has been configured)
        "temperatureSensor": {"1": 25},       // when providing the 'object', you can omit 'data'
        "humiditySensor": {"1": 32}
    }
}
```
对，明显就看出来源码中接受的消息体多了 id、dev_eui 两项数据。其实只是少了两项，如果代码中忽略这两项的话，也是能正常使用的。测试时我发送的消息中就没有 id、object 两项，它也能够正常解析且不报错的，但是新的 rust 代码中加了 dev_eui 判断，判断消息体中的 dev_eui 和主题中的 dev_eui 值是否一致，如果不一致就认为消息错误，不给 enquene 了。排除这样做的目的，修改后没有在文档中说明，导致这个接口根本无法使用，这就让超级恶心到使用者了。  
```rust
async fn message_callback(
    application_id: String,
    dev_eui: String,
    command: String,
    json: bool,
    p: Publish,
) {
    let topic = String::from_utf8_lossy(&p.topic);

    info!(topic = %topic, qos = ?p.qos, "Command received for device");

    let err = || -> Result<()> {
        match command.as_ref() {
            "down" => {
                let cmd: integration::DownlinkCommand = match json {
                    true => serde_json::from_slice(&p.payload)?,
                    false => integration::DownlinkCommand::decode(&mut Cursor::new(&p.payload))?,
                };
                if dev_eui != cmd.dev_eui {
                    return Err(anyhow!(
                        "Payload dev_eui {} does not match topic dev_eui {}",
                        cmd.dev_eui,
                        dev_eui
                    ));
                }
                tokio::spawn(super::handle_down_command(application_id, cmd));
            }
            _ => {
                return Err(anyhow!("Unknown command type"));
            }
        }

        Ok(())
    }()
    .err();

    if err.is_some() {
        warn!(
            topic = %topic,
            qos = ?p.qos,
            "Processing command error: {}",
            err.as_ref().unwrap()
        );
    }
}
```
作为完全不懂 rust 的我，意识到 dev_eui != cmd.dev_eui 这个问题时是几分钟后的事情了。当意识到是这里导致 mqtt 的消息下发失效后，我有些想爆粗口。太坑人了～～～～～   
在 mqtt 消息中添加了 dev_eui 条目后，我终于在 enquene 中看到了 mqtt 发送的消息了，顿时泪流满面，果然开源的世界中是无法完全信任别人的。
新的 mqtt 消息体：
```json
{
    "dev_eui": "4bee530d29435bef",
    "confirmed": false,
    "fPort": 10,
    "data": "qrvM"
}
```

chirpstack 源码在两年前使用 rust 语言重写了，所以 mqtt 之前的实现不得而知，但是关于 mqtt command_cmd 部分在第二次的 mqtt.rs 提交中就存在了。也就是说，chirpstack 在使用 rust 重写后，这个消息体就被改变了，但互联网中几乎没有看到关于这一点的描述，真的是坑人～～～

至此，chirpstack mqtt 的基本通信问题被解决，再也不用十分别扭的方式从 chirpstack 向 lora 设备发送消息了。


## mqtt 通信接口

1、 订阅应用中所有消息（注意全部小写）  
topic：  
```
application/{application id}/#

如 : application/1/#
```
  
2、订阅指定上行消息（注意全部小写）  
topic：  
```
application/[ApplicationID]/device/[DevEUI]/event/[EventType]

如 : application/1/device/70b3d57ed0056427/event/up
```
3、给 LoRa 节点发消息（注意全部小写）  
topic :   
```
application/[ApplicationID]/device/[DevEUI]/command/down
如 : application/94d9ff1f-6e2e-4e70-bdf0-2cadbdbb4ef0/device/4bee530d29435bef/command/down
```
消息体：  
```json
{
    "id": "",
    "dev_eui": "",
    "confirmed": false,  
    "fPort": 10,  
    "data": "qrvM" 
}


```
如：  
```json
{
    "dev_eui": "4bee530d29435bef",
    "confirmed": false,  
    "fPort": 10,  
    "data": "qrvM" 
}
```
注意：dev_eui 一定要和主题中的 [DevEUI] 一致，data 是二进制的 base64 编码字符串。  