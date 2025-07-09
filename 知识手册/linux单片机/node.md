linux下也能开发单片机，如果工具是通的话，用起来还算不错。

linux下的工具链一般是arm-none-eabi-gcc + gdb-multiarch + pyocd

arm-none-eabi-gcc 用于编译工程，gdb-multiarch 用于调试，pyocd 用于调试烧录固件。

pyocd 是 openocd 的代替版，用这还算舒服，pyocd 的一个优点就是兼容 keil 的 pack 包，如果厂家提供了芯片的 pack 包，那可以直接使用了。很是方便，openocd的话，还需要自己写 .cfg 文件，比较麻烦而且非常痛苦。

下面是pyocd 结合 gdb-multiarch 的使用方法：
```bash
## 调试步骤
### 1. 启动pyOCD的GDB服务器
```bash
pyocd gdbserver --pack /path/to/STM32_DFP.pack --target STM32F4xx
pyocd gdbserver --pack=dist/ArteryTek.AT32F415_DFP.2.1.6.pack  --target _at32f415rct7
```

> 注意：`STM32F4xx`替换为你的具体型号，如`STM32F407VG`。DFP包路径可选，pyOCD会自动下载。
### 2. 在另一个终端启动GDB

```bash
gdb-multiarch your_elf_file.elf
```

### 3. 在GDB中连接并调试
```gdb
(gdb) target extended-remote :3333
(gdb) monitor reset halt
(gdb) load
(gdb) b main
(gdb) continue
```

## 常用命令
- **硬件控制**：  ```gdb
  monitor reset        # 复位芯片  monitor halt         # 暂停CPU
  monitor step         # 单步执行  ```

- **断点管理**：  ```gdb
  b function_name      # 在函数处设置断点  b *0x08000000        # 在地址处设置断点  info break           # 查看断点  ```

- **内存操作**：  ```gdb
  x/10x 0x20000000     # 查看内存  set {int}0x20000000 = 0x1234  # 写内存  ```

```


还有一个简单的在scode调试单片机的lauch.josn文件，可以参考一下：
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "PyOCD Debug",
            "type": "cppdbg",
            "request": "launch",
            "program": "/home/nihao/w2T/github/dianjixz-lib/examples/at32f415_xieyi/dist/at32f415_xieyi",
            "cwd": "/home/nihao/w2T/github/dianjixz-lib/examples/at32f415_xieyi",
            "MIMode": "gdb",
            "miDebuggerPath": "gdb-multiarch",
            "miDebuggerServerAddress": "localhost:3333",
            "serverStarted": "GDB server started",
            "debugServerPath": "pyocd",
            "debugServerArgs": "gdbserver --pack=/home/nihao/w2T/github/dianjixz-lib/examples/at32f415_xieyi/dist/ArteryTek.AT32F415_DFP.2.1.6.pack  --target _at32f415rct7",
            "serverLaunchTimeout": 20000,
            "filterStderr": true,
            "filterStdout": true,
            "targetArchitecture": "arm",
            "hardwareBreakpoints": {
                "require": true,
                "limit": 6
            }
        }
    ]
}
```

裸的gdb调试脚本可以使用：https://github.com/cyrus-and/gdb-dashboard