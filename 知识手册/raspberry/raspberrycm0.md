# raspberry cm0


串口不可用，需要配置串口：

配置串口，在`/boot/config.txt`中添加：
```txt
enable_uart=1
```

重启后，串口可用。