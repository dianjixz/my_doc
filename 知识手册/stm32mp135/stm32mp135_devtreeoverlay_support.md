# CoreMP135 Overlay Device Trees 支持

在 Linux 中，Overlay Device Trees（DTBO）是一种特殊的设备树，用于在不修改主设备树（DTB）的情况下动态添加或修改硬件配置。这对于某些需要运行时硬件配置更改的应用非常有用，如根据不同的外设需求调整系统。
DTBO 的支持可分为两个阶段，第一个阶段是 uboot 引导阶段，uboot 直接将 dtbo 覆盖到设备树中，在启动内核后，这便是一个已经调整过的设备，内核对这种调整是没有感知的，属于不可更改的调整。第二个阶段就是 linux 启动后的调整，linux 启动后在 dtbo 的帮助下可以动态的调整设备树，从而控制固定设备的开启与关闭，和相关运行参数的调整。

## DTBO 语法
设备树插件是在设备树基础上增加的内容，我们之前讲解的设备树语法完全适用， 甚至我们可以直接将之前编写的设备树节点复制到设备树插件里。具体使用方法介绍如下。
### 设备树插件格式
设备树插件拥有相对固定的格式，甚至可以认为它只是把设备节点加了一个“壳”编译后内核能够动态加载它。 格式如下，具体节点省略。
```dts
/dts-v1/;
/plugin/;

 / {
        fragment@0 {
            target-path = "/";
            __overlay__ {
                /*在此添加要插入的节点*/
                .......
            };
        };

        fragment@1 {
            target = <&XXXXX>;
            __overlay__ {
                /*在此添加要插入的节点*/
                .......
            };
        };
    .......
 };
```
- 第1行： 用于指定dts的版本。
- 第2行： 表示允许使用未定义的引用并记录它们，设备树插件中可以引用主设备树中的节点，而这些“引用的节点”对于设备树插件来说就是未定义的，所以设备树插件应该加上“/plugin/”。
- 第6行： 指定设备树插件的加载位置，默认我们加载到根节点下，既“target-path =“/”,或者使用target = <&XXXXX>，增加节点或者属性到某个节点下。
- 第7-8行： 我们要插入的设备及节点或者要引用(追加)的设备树节点放在__overlay__ {…}内，你可以增加、修改或者覆盖主设备树的节点。

另外一种设备树插件格式：
```dts
/dts-v1/;
/plugin/;

&{/} {
    /*此处在根节点"/"下,添加要插入的节点或者属性*/
};

&XXXXX {
    /*此处在节点"XXXXX"下,添加要插入的节点或者属性*/
};
```
这种插件的书写方式和正式的 dts 格式相同，唯一的区别是在第二行声明了这个是以插件的形式应用。

以CoreMP135 设备为例，在设备树中添加一个 led 设备：
```dts
// file: led-overlay.dts
/dts-v1/;
/plugin/;

/ {
    fragment@0 {
        target-path = "/";
        __overlay__ {
            leds {
                compatible = "gpio-leds";

                led-blue {
                    function = "heartbeat"; // LED_FUNCTION_HEARTBEAT
                    color = <3>;            // LED_COLOR_ID_BLUE
                    gpios = <&gpioc 13 1>;  // &gpioc 13 GPIO_ACTIVE_LOW
                    linux,default-trigger = "heartbeat";
                    default-state = "off";
                };
            };
        };
    };
};
```
编译设备树：
```bash
root@M5Core135:~# dtc -I dts -O dtb -o led-overlay.dtbo led-overlay.dts
root@M5Core135:~# ls
led-overlay.dtbo  led-overlay.dts
```

## DTBO 在 uboot 中的使用
在 CoreMP135 根文件系统的 /boot 目录下存放的引导文件，其中 /boot/extlinux/extlinux.conf 是 uboot 的开机引导脚本。通过该脚本，可以设置开机叠加的设备树。
```bash
root@M5Core135:/boot/extlinux# cat /boot/extlinux/extlinux.conf 
label stm32mp135f-core135-buildroot
  kernel /boot/zImage
  devicetree /boot/stm32mp135f-core135.dtb
  append root=/dev/mmcblk0p5 rw panic=5 quiet rootwait
```
- label : 启动标签，可以有多个启动标签，uboot 在开机时可以根据启动标签进行选择。
- kernel ：要启动的内核文件
- devicetree ： 要启动内核设备树
- append ： 添加的 kernel 启动参数
更多参数请参考 uboot 的 pxe 菜单。

我们需要关注的是，fdtoverlays 参数，该参数用于定义开启覆盖的 dtbo 设备树文件。

具体操作如下：
```bash
# 编译出 dtbo 设备树文件
root@M5Core135:~# ls
led-overlay.dtbo  led-overlay.dts

# 将设备树文件放在 /boot 引导目录下。
root@M5Core135:~# cd /boot
root@M5Core135:/boot# cp /root/led-overlay.dtbo .
root@M5Core135:/boot# ls
extlinux	   stm32mp135f-coremp135.dtb        led-overlay.dtbo  
stm32mp135f-core135.dtb  zImage

# 在引导文件中设置 dtbo 参数
root@M5Core135:/boot/extlinux# cat extlinux.conf 
label stm32mp135f-core135-buildroot
  kernel /boot/zImage
  devicetree /boot/stm32mp135f-core135.dtb
  fdtoverlays /boot/led-overlay.dtbo
  append root=/dev/mmcblk0p5 rw panic=5 quiet rootwait

```
重启设备，我们就能在 /sys/class/leds 目录下看到我们添加的 led 设备了。  
同时观察设备 bus 上的 PC13 引脚是否有电平变化。

## DTBO 在 linux 中的使用
linux 在运行时也能够对设备树进行动态的调整，linux 对设备树的调整需要 dtbocfg.ko 模块的支持。  
首先在 kernel 编译时开启 CONFIG_OF_OVERLAY 的支持。  
然后编译 git@github.com:m5stack/dtbocfg.git 内核项目，生成 dtbocfg.ko 内核模块。  
加载 dtbocfg.ko 模块后，将编译好的 led-overlay.dtbo 导入到 /sys/kernel/config/device-tree/overlays/xx/dtb0 中。  
向 /sys/kernel/config/device-tree/overlays/xx/status 写入 1 开启设备树的使用。  
具体操作如下：
```bash
# 挂载 configs 文件系统，如果系统没有自动挂载，就进行手动挂载。
# mount -t configfs none /sys/kernel/config

# 加载内核模块
root@M5Core135:~# modprobe dtbocfg

# 创建加载目录
root@M5Core135:~# mkdir /sys/kernel/config/device-tree/overlays/leds

# 填充 dtbo
root@M5Core135:~# cat led-overlay.dtbo > /sys/kernel/config/device-tree/overlays/leds/dtbo

# 启动 dtbo
root@M5Core135:~# echo 1 > /sys/kernel/config/device-tree/overlays/leds/status
```
同时观察设备 bus 上的 PC13 引脚是否有电平变化。

注意：
    此时的设备树加载后就开始起作用了。 









