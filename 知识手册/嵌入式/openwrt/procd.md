`procd` 是 OpenWrt 项目中的一个进程管理守护进程，主要用于管理系统服务和守护进程。它的设计主要是为了替代传统的 `init` 系统，并且更适合嵌入式设备的需求。

严格来说，`procd` 本身并不是专门用来检测内核事件的工具。它的主要功能包括：

1. 启动和停止系统服务。
2. 监控服务的运行状态并在必要时重新启动它们。
3. 管理 UCI（Unified Configuration Interface）配置文件。
4. 处理热插拔事件（例如 USB 设备插拔）。
5. 管理系统的引导过程。

然而，`procd` 可以通过使用 `ubus`（OpenWrt 的消息总线系统）来与其他系统组件通信，而这些组件可能负责检测和处理内核事件。例如，`procd` 可以通过 `ubus` 接口与 `netifd`（网络接口守护进程）通信，后者可以处理和响应网络接口的状态变化，这些变化通常是由内核事件触发的。

因此，虽然 `procd` 本身不是直接用于检测内核事件的工具，但它可以通过与其他系统组件的交互来间接处理由内核事件引发的系统状态变化。

如果你需要专门检测和处理内核事件，可能需要使用其他工具或机制，比如 `udev`、`inotify` 或者直接编写内核模块来捕捉特定的内核事件。




```sh
# touch /etc/hotplug.d/block/10-block
#!/bin/sh

LOGGER=/usr/bin/logger

case "$ACTION" in
    add)
        # 处理设备插入事件
        $LOGGER "Block device added: $DEVNAME"
        if echo "$DEVNAME" | grep -qE '^mmcblk[0-9]+p[0-9]+$'; then
            # 如果设备名称符合 mmcblkxpx 的格式
            $LOGGER "Device $DEVNAME is of mmcblkxpx type"
            # 挂载设备到指定目录，例如 /mnt/sdcard
            mkdir -p /mnt/$DEVNAME
            mount /dev/$DEVNAME /mnt/$DEVNAME
            if [ $? -eq 0 ]; then
                $LOGGER "Successfully mounted /dev/$DEVNAME to /mnt/$DEVNAME"
            else
                $LOGGER "Failed to mount /dev/$DEVNAME"
            fi
        else
            $LOGGER "Device $DEVNAME is not of mmcblkxpx type"
        fi
        ;;
    remove)
        # 处理设备拔出事件
        $LOGGER "Block device removed: $DEVNAME"
        if echo "$DEVNAME" | grep -qE '^mmcblk[0-9]+p[0-9]+$'; then
            # 如果设备名称符合 mmcblkxpx 的格式
            $LOGGER "Device $DEVNAME is of mmcblkxpx type"
            # 卸载设备并删除挂载点
            umount /mnt/$DEVNAME
            if [ $? -eq 0 ]; then
                $LOGGER "Successfully unmounted /mnt/$DEVNAME"
                rmdir /mnt/$DEVNAME
            else
                $LOGGER "Failed to unmount /mnt/$DEVNAME"
            fi
        else
            $LOGGER "Device $DEVNAME is not of mmcblkxpx type"
        fi
        ;;
    *)
        $LOGGER "Unhandled action: $ACTION"
        ;;
esac


```



