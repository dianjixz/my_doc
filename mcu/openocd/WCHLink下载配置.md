https://blog.csdn.net/michaelchain/article/details/122711082

在OpenOCD配置中添加 WCHLink 支持

在 packages/tool-openocd/scripts/target 目录下, 新增 ch32f1x.cfg 文件, 内容为
```
# CH32F103x

set CPUTAPID 0x2ba01477

cmsis_dap_vid_pid 0x1a86 0x8011

source [find target/stm32f1x.cfg]


```

修改 99-platformio-udev.rules

```
# WCH Link (CMSIS-DAP compatible adapter)
ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="8011", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"


```

保存后执行

```
sudo udevadm control --reload-rules

```

参考

    https://github.com/kaidegit/CMSIS-DAPbyWCH
    https://github.com/pyocd/pyOCD/issues/1121