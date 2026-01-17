# UMS (USB Mass Storage)
ums 是uboot的一项usb功能，将emmc在电脑端映射成一个磁盘设备，方便用户操作。
 使用 u-boot 的 UMS（USB Mass Storage）功能（实验性）：

```bash
有拨码开关的请设置为 EMMC 模式。

使用串口在 u-boot 倒计时读秒时使用 `Ctrl^C` 中断以进入 u-boot 命令行，然后输入以下指令：
```
ums 0 mmc 0
```
然后 EMMC 将会以 USB Mass Storage 设备映射在电脑上。

自动进入 ums 功能正在开发，有兴趣的可以一起帮忙开发（超小声）：https://github.com/chainsx/thead-u-boot/tree/emmc-flasher

```