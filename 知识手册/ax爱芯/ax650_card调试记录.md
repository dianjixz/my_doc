# AX650_card
AX650_card 是爱芯的一种加速卡的形式，通过m.2的pcie进行通信,为主机设备提供硬件加速服务，包括编解码，NPU推理等等。 

|              | 描述                                                    |
| ------------ | ------------------------------------------------------- |
| 处理器       | Octa-corre Cortex-A55@1.7GHz X8                         |
| 内存         | 8GiB，64bit LPDDR4x                                     |
| 存储         | 16MiB，Nor Flash                                        |
| NPU          | 18TOPs@Int8，72TOPs@Int4                                |
| 模型支持     | 支持 CNN、Transformer 模型部署，支持 LLM、VLM 部署      |
| 视频编码     | H.264/H.265，16路 1080@30fps 编码                       |
| 视频解码     | H.264/H.265，32路 1080@30fps 解码                       |
| Host 适配    | 支持 Intel、AMD、NXP、Xilinx、Raspberry Pi、Rockchip 等 |
| Host 系统    | Ubuntu、Debian、CentOS、Kylin、UOS                      |
| 外形尺寸     | M.2 2280，M Key                                         |
| 工作电压     | 3.3 V                                                   |
| 整体系统功耗 | ＜8 w                                                   |

上面是友商的描述，实际我们使用的AX8850，官方数据来源NPU算力24TOPs@INT8，按照经验值计算最大为96TOPs@INT4，算力还算可观，专门为LLM大模型做了优化。







AX650_card 的主型态只有一颗ax650N+2个DDR+1个nor flash。flash用于启动uboot，将AX650N设置为PCIE从机状态，启动后，axcl 驱动将会为加速卡提供需要运行的系统，AX650启动后通过PCIE，使用axcl架构为主机提供加速服务。

2025年 07月 01日 星期二 11:44:04 CST  
AX650_R1.25.1301_AX650_SDK_V3.0.2 的 AX650_card 主工程编译失败，爱芯FAE表示，需要更新系统。

2025年 07月 02日 星期三 12:08:01 CST
AX650_R1.25.2201_AX650_SDK_V3.6.2_M5Stack 的 AX650_card 主工程编译成功，该project的输出主要分两部分：
- AX650_card.axp 系统包部分，里面主要是引导区的文件，用于提供一个可引导的环境。
- axcl_host.deb 工具包部分，该部分中主要是axcl工具，开发库，以及加速卡需要的一个firewire固件包，该固件包用于加速卡的启动。

按照规划，加速卡出厂刷入AX650_card.axp系统。在插入设备后，axcl工具链会通过PCIE引导加速卡启动，然后执行对应的任务。

 







