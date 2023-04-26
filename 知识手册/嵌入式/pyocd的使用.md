pyocd 使用记录
============

背景：遇到 platform 后我就感觉这个开发平台很好用，刚开始开发 arduino 的时候使用的很开心，因为使用的都是arduino之类的，或者使用串口下载，这个时候使用还很开心。但是自从开始使用
openocd 后，折磨的我情况就来了。各种开发板不匹配，各种 daplink usb不识别，每次都要在下载调试上被折磨了很久。  
其实在之前就听见朋友使用 pyocd。但是一直都没有特别在意。直到前两天去查了查 pyocd 的用法，突然发现这个工具实在是太开心了，完美的替代了 openocd 的使用，从此在也不用担心我调试器
的问题。  

pyocd [官网](https://pypi.org/project/pyocd/)  

pyocd [使用指南](https://www.cnblogs.com/halin/p/15169658.html)  

pyocd 的命令比较多，但其实常用的命令不多。  

- pyocd load -t stm32f103c8 target.bin/target.hex    给 stm32f103c8 系列板子烧写固件，可以用 stlink 或者 daplink。  
- pyocd gdb -t stm32f103c8                           开启 gdbserver 服务，可用于 gdb 进行调试。  
- pyocd pack -i stm32f103                            安装 stm32f103 的支持包，可以离线装，后面后会介绍。  



在 platformio 中的使用示列：
```
[env:genericSTM32F103C8]
platform = ststm32
board = genericSTM32F103C8
framework = arduino
upload_protocol = custom
upload_command = pyocd load -t stm32f103c8 $SOURCE
debug_tool = custom
debug_server = 
    pyocd 
    gdb 
    -t 
    stm32f103c8
```
这样就可以完美的替代调令人抓狂的 openocd 了。不管怎么说，使用这个都会让你少掉几根头发。  
在 platformio 使用 pyocd 的时需要在环境中安装 pyocd
``` bash
# linux
source ~/.platformio/penv/bin/activate
pip install pyocd -i https://mirrors.ustc.edu.cn/pypi/web/simple
pip install libusb -i https://mirrors.ustc.edu.cn/pypi/web/simple

wget https://keilpack.azureedge.net/pack/Keil.STM32F1xx_DFP.2.4.0.pack
mkdir -p ~/.local/share/cmsis-pack-manager/Keil/STM32F1xx_DFP
mv Keil.STM32F1xx_DFP.2.4.0.pack ~/.local/share/cmsis-pack-manager/Keil/STM32F1xx_DFP/2.4.0.pack
pyocd pack -s
```

只要你拥有 keil 的 pack，就能拥有 pyocd 的调试功能。

keil pack：  
https://www.keil.com/dd2/pack/
