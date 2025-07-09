安装
```bash
sudo apt install mbpoll
```


mbpoll [OPTIONS] [设备地址或IP] [从机ID]
-m rtu：使用 Modbus RTU 模式。
-m tcp：使用 Modbus TCP 模式。
-a <从机ID>：指定从机地址（ID）。
-r <寄存器地址>：指定寄存器地址。
-t <类型>：指定寄存器类型，例如 3:hex 表示读取保持寄存器并以十六进制显示。0 线圈，1 离散输入，3 输入寄存器，4 保持寄存器
-c <数量>：指定读取的寄存器数量。
-1：只读取一次（默认持续读取）。
-b <波特率>：设置波特率（RTU模式下需要）。
-P <校验>：设置校验方式（none、even 或 odd）。
-o <超时时间>：设置超时时间，以毫秒为单位。


mbpoll [OPTIONS] [设备地址或IP] [值]
-m rtu：使用 Modbus RTU 模式。
-m tcp：使用 Modbus TCP 模式。
-a <从机ID>：指定从机地址（ID）。
-r <寄存器地址>：指定寄存器地址。
-0：写入单个寄存器。
-1：写入多个寄存器。
-b <波特率>：设置波特率（RTU模式下需要）。
-P <校验>：设置校验方式（none、even 或 odd）。

读线圈：
mbpoll -m rtu -b 115200 -P none -a 1 -r 1 -t 0:hex -c 1 -1 /dev/ttyS3
写线圈：
mbpoll -m rtu -b 115200 -P none -a 1 -r 1 -t 0:hex -1 /dev/ttyS3 0x01

读离散输入：
mbpoll -m rtu -b 115200 -P none -a 1 -r 1 -t 1:hex -c 1 -1 /dev/ttyS3

读输入寄存器：
mbpoll -m rtu -b 115200 -P none -a 1 -r 1 -t 3:hex -c 1 -1 /dev/ttyS3

读保持寄存器：
mbpoll -m rtu -b 115200 -P none -a 1 -r 1 -t 4:hex -c 1 -1 /dev/ttyS3
写保持寄存器：
mbpoll -m rtu -b 115200 -P none -a 1 -r 1 -t 4:hex -1 /dev/ttyS3 0xffaa



mbpoll -a 4 -r 1 -c 2 -0 256 -b 115200 /dev/ttySTM3

mbpoll -a 1 -r 100 -b 115200 /dev/ttySTM3 1

mbpoll -a 4 -t 4:hex -r 1 -c 1 -b 115200 /dev/ttySTM3 -- 125


mbpoll -t4:int -b 115200 /dev/ttySTM3 -- 123 -1568 8974 -12



0x04 0x04 		0xf7 0xff 				0x33 0xdb
03 10 			00 01 		00 02 			11 EA

0x04 0x06 0x00 0x00 0xf8 0x00 0xca 0x5f

mbpoll -m -a 4 -r 1 -c 2 -t4:int /dev/ttySTM3 -- 123 -1568 8974 -12


mbpoll -m rtu -a 4 -r 1 -t 4:hex -1 0x06 -b 115200 /dev/ttySTM3


mbpoll -a 4 -b 115200 -P none -t 4:hex -r 0 -1 -v /dev/ttySTM3 0xf800



mbpoll -a 4 -b 115200 -P none -t 4:hex -r 1 -1 -v /dev/ttySTM3 0xf800

mbpoll -a 4 -b 115200 -P none -t 4:hex -r 2 -1 -v /dev/ttySTM3 0x01




mbpoll -m rtu -b 115200 -P none -a 1 -r 0 -t 3:hex -c 2 /dev/ttyUSB0
-m rtu：使用 Modbus RTU 模式。
-b 115200：波特率设置为 115200。
-P none：无校验。
-a 1：从机地址为 1。
-r 1：从寄存器地址 100 开始读取。
-t 3:hex：读取保持寄存器并以十六进制显示。
-c 2：读取 2 个寄存器。
/dev/ttyUSB0：指定串口设备。