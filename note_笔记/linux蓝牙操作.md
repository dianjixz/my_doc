如果你希望在非交互模式下使用`bluetoothctl`连接蓝牙设备，可以通过在命令行中直接传递命令来实现。以下是一个示例脚本，展示了如何在非交互模式下连接蓝牙设备：

```bash
#!/bin/bash

# 设备的MAC地址
DEVICE_MAC="XX:XX:XX:XX:XX:XX"

# 打开蓝牙功能
bluetoothctl power on

# 开始扫描设备
bluetoothctl scan on &
sleep 5  # 等待一段时间，以确保设备被发现

# 停止扫描
bluetoothctl scan off

# 配对设备
bluetoothctl pair $DEVICE_MAC

# 信任设备
bluetoothctl trust $DEVICE_MAC

# 连接设备
bluetoothctl connect $DEVICE_MAC
```

你可以将上述脚本保存为一个文件（例如 `connect_bluetooth.sh`），然后给予执行权限并运行它：

```bash
chmod +x connect_bluetooth.sh
./connect_bluetooth.sh
```

如果你希望在一行命令中完成所有操作，可以使用以下命令：

```bash
DEVICE_MAC="XX:XX:XX:XX:XX:XX"
bluetoothctl power on && \
bluetoothctl scan on && sleep 5 && \
bluetoothctl scan off && \
bluetoothctl pair $DEVICE_MAC && \
bluetoothctl trust $DEVICE_MAC && \
bluetoothctl connect $DEVICE_MAC
```

请将`XX:XX:XX:XX:XX:XX`替换为你的蓝牙设备的实际MAC地址。这样，你就可以在非交互模式下通过命令行连接蓝牙设备。






`pavucontrol` 是一个图形化的音量控制工具，用于管理 PulseAudio 音频设备和流。虽然 `pavucontrol` 本身是一个 GUI 工具，但你可以通过命令行工具 `pactl` 或 `pacmd` 来调整音频输出设置。

以下是如何在终端中使用 `pactl` 来调整音频输出：

1. **列出所有音频接收器**：
   ```bash
   pactl list sinks short
   ```
   这将显示所有可用的音频输出设备（接收器），并给出每个设备的索引。

2. **更改默认音频输出设备**：
   假设你想将默认音频输出设备更改为特定的蓝牙设备（例如 `bluez_sink.14_2B_2F_A1_2E_DA.a2dp_sink`），可以使用以下命令：

   ```bash
   pactl set-default-sink bluez_sink.14_2B_2F_A1_2E_DA.a2dp_sink
   ```

3. **将当前播放的音频流移动到指定输出设备**：
   首先，列出所有播放中的音频流：
   ```bash
   pactl list sink-inputs short
   ```
   找到你想移动的音频流的 ID，然后使用以下命令将其移动到新的输出设备：

   ```bash
   pactl move-sink-input <stream_id> bluez_sink.14_2B_2F_A1_2E_DA.a2dp_sink
   ```
   替换 `<stream_id>` 为实际流的 ID。

4. **调整音量**：
   可以使用以下命令来调整某个接收器的音量：

   ```bash
   pactl set-sink-volume bluez_sink.14_2B_2F_A1_2E_DA.a2dp_sink 50%
   ```

   这将音量设置为 50%。你也可以用 `+` 或 `-` 来增加或减少音量，例如 `+10%`。

通过这些命令，你可以在终端中控制和调整 PulseAudio 的音频输出设置，无需使用 `pavucontrol` 的 GUI。



```bash

bluetoothctl connect 14:2B:2F:A1:2E:DA
pactl list sinks short
ffmpeg -f pulse "default" -i 123.mp3
mpg123 123.mp3
```



