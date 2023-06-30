memtool 是一个命令行工具，用于读取和写入系统内存和外设寄存器。它提供了多种功能，包括内存显示（md）、内存写入（mw）、寄存器显示（mdr）、寄存器写入（mwr）等。memtool 可以用于嵌入式系统开发和调试，方便地查看和修改内存数据，与外设进行通信。

以下是 memtool 命令的 md 格式输出示例：

``` bash

Usage: memtool md [-bwlqsx] REGION

Display (hex dump) a memory region.

Options:
  -b        byte access
  -w        word access (16 bit)
  -l        long access (32 bit)
  -q        quad access (64 bit)
  -s <FILE> display file (default /dev/mem)
  -x        swap bytes at output

Memory regions can be specified in two different forms: START+SIZE
or START-END, If START is omitted it defaults to 0x100
Sizes can be specified as decimal, or if prefixed with 0x as hexadecimal.
An optional suffix of k, M or G is for kbytes, Megabytes or Gigabytes.
```
以上是 memtool 的 md 命令的用法说明，通过指定合适的选项和内存区域，可以以十六进制格式显示指定内存区域的内容。

源码地址：https://github.com/pengutronix/memtool.git


memtool md -l 0x50003024+4

./memtool mw -l 0x50003024 0xd00e0







devmem2 是一个命令行工具，用于读取和写入系统内存和外设寄存器。它提供了简单而直接的方式来访问和修改特定内存地址的内容。

devmem2 的使用格式如下：

``` bash

Usage: devmem2 ADDRESS [TYPE [DATA]]

ADDRESS: 内存地址，可以是十进制或十六进制格式
TYPE: 数据类型，可选参数，包括 b (byte), h (halfword), w (word), l (longword)，默认为 byte
DATA: 写入数据，可选参数，必须与指定的数据类型相匹配

通过指定内存地址、数据类型和数据，可以读取或写入指定内存地址的内容。devmem2 提供了对内存的直接访问，可用于嵌入式系统的调试和开发过程中。

例如，要读取内存地址 0x12345678 处的字节数据，可以使用以下命令：

devmem2 0x12345678

要向内存地址 0x12345678 处写入字节数据 0xAB，可以使用以下命令：

css

devmem2 0x12345678 b 0xAB
```
通过 devmem2 工具，可以方便地读取和写入系统内存和外设寄存器，以便进行嵌入式系统的调试和调整。请注意，在使用 devmem2 工具时，务必小心操作，避免对关键系统区域造成意外的影响。

源码地址：https://github.com/VCTLabs/devmem2.git


./devmem2 0x50003024 w 

./devmem2 0x50003024 w 0xd00e0