#!/bin/env python3


import serial

# 配置串口参数
port = 'COM3'  # 串口号，Windows 上通常是 COM1, COM2 等，Linux 上通常是 /dev/ttyUSB0, /dev/ttyS0 等
baudrate = 9600  # 波特率
timeout = 1  # 超时时间（秒）

# 打开串口
try:
    ser = serial.Serial(port, baudrate, timeout=timeout)
    print(f"串口 {port} 已打开")

    # 读取数据
    while True:
        data = ser.readline()  # 读取一行数据
        if data:
            print(f"收到数据: {data.decode('utf-8').strip()}")  # 解码并打印数据

except serial.SerialException as e:
    print(f"打开串口时出错: {e}")

finally:
    # 关闭串口
    if 'ser' in locals() or 'ser' in globals():
        ser.close()
        print(f"串口 {port} 已关闭")