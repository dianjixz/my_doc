
https://www.cnblogs.com/liangliangge/p/12214305.html

Uboot常用命令使用

1、前言

Uboot启动后，会进入到一个倒计时，在倒计时结束之前，如果此时我们按下键盘的回车键，将进入到uboot的命令行模式，有点类似Linux系统终端模式，如果没有按下回车键的话，将直接启动Linux内核，本篇文章将介绍uboot中的一些常用命令，熟悉这些命令后，以后在适配调试uboot的时候会得心应手。

如下所示，倒计时的时候按下回车键后，进入到uboot的命令行模式：

在行的前面多了"=>"字符串，说明此时已经进入uboot的命令行模式了，接下来就可以进行一些命令输入。



bootargs linux启动参数变量


bootdelay
	
定义执行自动启动的等候秒数
baudrate
	
定义串口控制台的波特率
netmask
	
定义以太网接口的掩码
ethaddr
	
定义以太网接口的MAC地址
bootfile
	
定义缺省的下载文件
bootargs
	
定义传递给Linux内核的命令行参数
bootcmd
	
定义自动启动时执行的几条命令
serverip
	
定义tftp服务器端的IP地址
ipaddr
	
定义本地的IP地址
stdin
	
定义标准输入设备，一般是串口
stdout
	
定义标准输出设备，一般是串口
stderr
	
定义标准出错信息输出设备，一般是串口


bootcmd是自动启动时默认执行的一些命令，因此你可以在当前环境中定义各种不同配置，不同环境的参数设置，然后设置bootcmd为你经常使用的那种参数，而且在bootcmd中可以使用调用的方式，方便修改。

bootargs是环境变量中的重中之重，甚至可以说整个环境变量都是围绕着bootargs来设置的。bootargs的种类非常非常的多，我们平常只是使用了几种而已。bootargs非常的灵活，内核和文件系统的不同搭配就会有不同的设置方法，甚至你也可以不设置 bootargs,而直接将其写到内核中去（在配置内核的选项中可以进行这样的设置），正是这些原因导致了bootargs使用上的困难。
下面介绍一下bootargs常用参数，bootargs的种类非常的多，而且随着kernel的发展会出现一些新的参数，使得设置会更加灵活多样。









 

2、查看uboot支持的命令

在使用uboot命令之前，我们先来看看当前的uboot版本所支持的命令有哪些，可以在当前的命令函数输入：

=> help
or
=> ?

该命令用于查看当前uboot版本所支持的所有命令，输入后按回车即可，在我当前使用的uboot版本中所支持的命令如下：
复制代码

?       - alias for 'help'
base    - print or set address offset
bdinfo  - print Board Info structure
bmode   - sd1|sd2|qspi1|normal|usb|sata|ecspi1:0|ecspi1:1|ecspi1:2|ecspi1:3|esdhc1|esdhc2|esdhc3|esdhc4 [noreset]
bmp     - manipulate BMP image data
boot    - boot default, i.e., run 'bootcmd'
bootd   - boot default, i.e., run 'bootcmd'
bootelf - Boot from an ELF image in memory
bootm   - boot application image from memory
bootp   - boot image via network using BOOTP/TFTP protocol
bootvx  - Boot vxWorks from an ELF image
bootz   - boot Linux zImage image from memory
clocks  - display clocks
clrlogo - fill the boot logo area with black
cmp     - memory compare
coninfo - print console devices and information
cp      - memory copy
crc32   - checksum calculation
dcache  - enable or disable data cache
dhcp    - boot image via network using DHCP/TFTP protocol
dm      - Driver model low level access
echo    - echo args to console
editenv - edit environment variable
env     - environment handling commands
erase   - erase FLASH memory
exit    - exit script
ext2load- load binary file from a Ext2 filesystem
ext2ls  - list files in a directory (default /)
ext4load- load binary file from a Ext4 filesystem
ext4ls  - list files in a directory (default /)
ext4size- determine a file's size
ext4write- create a file in the root directory
false   - do nothing, unsuccessfully
fatinfo - print information about filesystem
fatload - load binary file from a dos filesystem
fatls   - list files in a directory (default /)
fatsize - determine a file's size
fdt     - flattened device tree utility commands
flinfo  - print FLASH memory information
fstype  - Look up a filesystem type
fuse    - Fuse sub-system
go      - start application at address 'addr'
gpio    - query and control gpio pins
help    - print command description/usage
i2c     - I2C sub-system
icache  - enable or disable instruction cache
iminfo  - print header information for application image
imxtract- extract a part of a multi-image
itest   - return true/false on integer compare
load    - load binary file from a filesystem
loadb   - load binary file over serial line (kermit mode)
loads   - load S-Record file over serial line
loadx   - load binary file over serial line (xmodem mode)
loady   - load binary file over serial line (ymodem mode)
loop    - infinite loop on address range
ls      - list files in a directory (default /)
md      - memory display
mm      - memory modify (auto-incrementing address)
mmc     - MMC sub system
mmcinfo - display MMC info
mw      - memory write (fill)
nand    - NAND sub-system
nboot   - boot from NAND device
nfs     - boot image via network using NFS protocol
nm      - memory modify (constant address)
ping    - send ICMP ECHO_REQUEST to network host
pmic    - PMIC
printenv- print environment variables
protect - enable or disable FLASH write protection
reset   - Perform RESET of the CPU
run     - run commands in an environment variable
save    - save file to a filesystem
saveenv - save environment variables to persistent storage
setenv  - set environment variables
setexpr - set environment variable as the result of eval expression
showvar - print local hushshell variables
size    - determine a file's size
sleep   - delay execution for some time
source  - run script from memory
test    - minimal test like /bin/sh
tftpboot- boot image via network using TFTP protocol
true    - do nothing, successfully
usb     - USB sub-system
usbboot - boot from USB device
version - print monitor, compiler and linker version

复制代码

对于上面的命令列表中的命令，并不是所有都能运行的，如果该命令并没有在板级文件中使能配置的话，哪么直接在命令行中输入后按回车键，将会直接提示运行不了，也就是说上面列出的命令，并没有全部都使能了。

上面列出的命令，后面都描述了命令的功能，例如，最后一个version命令，它的功能就是用来输出uboot编译和链接的相关版本信息，除此之外，如果我们想查看某个命令的详细用法的话，可以输入：

=> help command_name
or
=> ? command_name

例如，我们查看bootz命令的用法，输入如下，然后回车：

=> help bootz
or
=> ? bootz

回车后，输出如下：

从输出的结果可以看到，bootz命令的用法被详细的列出来了，该命令就是用来在内存中启动Linux内核的，对于其它命令的详细用法，我们也可以使用这个方式进行命令的详细使用方式进行查询，接下来，我们了解一些常用的uboot命令用法。

 

3、信息查询相关命令

在uboot的命令中，常用的信息查询命令主要有3个，分别是bdinfo、printenv和version命令。

首先是bdinfo命令，该命令用来查询当前板子的相关信息，在命令行模式下，直接输入bdinfo，然后回车即可，输出如下：

如上所示，通过该命令可以查看板子的DRAM的大小以及DRAM的起始地址、当前使用的网络接口以及IP地址、波特率和uboot偏移地址以及偏移量等相关信息。

接下来是printenv命令，该命令用于查询当前板子的一些相关环境变量，同样，在命令行模式下输入printenv并回车，输出如下：

从上图可以看到，命令输入后，将会输出一堆的环境变量，例如：当前串口的波特率baudrate、启动参数bootargs以及启动命令bootcmd等，这些环境变量都是字符串，能对其进行修改。

最后则是version命令，该命令用于查询uboot版本和交叉编译工具的相关信息，在命令模式下输入version后并回车，输出如下：

从输出的信息可以看到，当前的uboot版本号为2016.03，后面接的是编译时间，交叉编译工具链为arm-linux-gnueabihf-gcc，版本号为4.9.4。

 

4、环境变量相关命令

环境变量常用的命令主要有两个，分别是setenv和saveenv，setenv命令用来设置或者修改当前环境变量的值，saveenv用来保存环境变量的值，一般环境变量是存放在外部的Flash中的，例如Nand Flash中，当uboot启动的时候，会将环境变量读取的DRAM中，当我们使用setenv修改了环境变量的值后，需要使用saveenv命令将修改后的环境变量的值保存到Flash中，否则修改无效。

setenv命令的格式如下：

=> setenv env value

查看当前的bootdelay环境变量值为：

=> print bootdelay

输出如下所示：

当前的bootdelay为3，我们要将bootdelay环境变量的值设置为5，可以使用下面的命令：

=> setenv bootdelay 5
=> saveenv

输出如下：

从输出信息可以看到，新设置的环境变量已经被保存到Nand Flash中了，这时候重启系统，将会发现启动倒计时变成了5秒，重新输出bootdelay环境，如下：

需要注意的是，当我们修改环境变量有空格值的时候，例如bootcmd或者bootargs等，这个时候的环境变量值需要使用单引号括起来。

此外，setenv命令也可以用来新建环境变量，比如我们要新建一个author的环境变量并查看，可以使用下面的命令：

=> setenv author huangly
=> saveenv
=> print author

设置成功后，输出如下：

setenv命令可以用来新建环境变量，也可以用来删除环境变量，如下命令可以删除author环境变量：

=> setenv author
=> saveenv
=> print

设置后，输出如下：

从输出可以看到，author环境变量已经被成功删除了。

 

5、内存操作相关命令

内存操作的相关命令用于对DRAM内存进行读写操作，常用的内存操作命令有md、nm、mm、mw、cp、cmp。

（1）md命令

md命令用于显示内存值，该命令的用法如下所示：

命令用法中的[.b, .w, .l]对应着byte、word、long，分别以1个字节、2个字节、4个字节来进行内存值显示，address表示要查看的内存起始地址，[# of objects]表示要查看的数据长度，和显示的数据格式有关，并且需要注意的是uboot命令中的数字都是十六进制的。

例如，当我们想要查看从地址0x8000000开始的20个字节的内存值，可以使用下面的命令显示：

=> md.b 80000000 14
or
=> md.b 0x80000000 0x14

显示结果如下：

（2）nm命令

nm命令用于修改指定地址的内存值，该命令的用法如下：

nm命令同样是使用[.b, .w, .l]来指定内存的操作格式，例如，想要使用修改0x80000000地址的数据为0x77，可使用下面的命令：

=> nm.b 80000000
or
=> nm.b 0x80000000

在上述的命令中，b表示修改为1个字节，80000000表示要修改的内存地址，修改如下所示：

对内存的值修改完成后，可以使用md命令重新显示新修改的内存值。

（3）mm命令

mm命令也是可以用来修改内存值，但是使用mm命令修改内存值的时候，地址值将会自增，使用nm命令时，地址值将不会自增。

例如，使用.b格式修改0x80000000开始的连续4个字节数据为0x12345678，使用命令如下：

=> mm.b 80000000

修改的过程中，地址会自增，修改完成后显示如下：

修改后，要使用md命令查看修改的值，判断是否修改成功。

（4）mw命令

mw命令用来使用一个指定的数据填充一段内存，命令的使用方法如下：

该命令同样使用[.b, .w, .l]来指定操作格式，address表示要填充的内存起始地址，value表示要填充的数据，count是要填充的长度。

例如，使用.b格式将以0x80000000为起始地址的0x14个内存块填充为0x33，命令如下：

=> mw.b 80000000 33 14

命令执行后如下所示：

使用mw修改完成后，要使用md命令显示修改后的内存值，判断是否修改成功。

（5）cp命令

cp命令是数据拷贝命令，用于将DRAM中的数据从一段内存中拷贝到另一段内存中，命令的使用格式如下所示：

该命令同样使用[.b, .w, .l]来指定操作格式，source表示为内存源地址，target表示为目标地址，count为拷贝的长度。

例如，使用.b格式将0x80000000开始地址处的0x14个字节拷贝到0x80000100地址处，命令如下所示：

=> cp.b 80000000 80000100 14
or
=> cp.b 0x80000000 0x80000100 14

命令执行后输出如下所示：

内存修改后，要使用md命令检查内存值是否修改成功。

（6）cmp命令

cmp命令用于比较两段内存的数据是否相等，命令的使用格式如下所示：

该命令同样使用[.b, .w, .l]来指定操作格式，addr1为第一段内存首地址，addr2为第二段内存首地址，count表示要比较的长度。

例如，使用.b格式来比较0x80000000和0x80000100两个地址的数据是否相等，比较的长度为0x14个字节，命令如下：

=> cmp.b 80000000 80000100 14

命令执行后显示如下：

从上面的比较输出结果可以看到，两个地址处的数据是相同的。

 

6、emmc和sd卡相关操作命令

对于uboot来说是支持emmc或者sd卡，因此也需要提供给用户emmc和sd卡的相关操作命令，uboot中常用于操作mmc设备的命令为"mmc"，mmc是一系列的命令，它的后面可以跟不同的参数，在uboot命令行中，输入下面的命令可以查看详细的用法：

=> ? mmc

回车后，输出如下所示：

从图中可以看到，mmc后跟不同的参数可以实现不同的功能，如下：
命令 	功能
mmc info 	输出mmc设备的信息
mmc read 	读取mmc设备中的数据
mmc write 	向mmc设备中写入数据
mmc rescan 	扫描当前存在的mmc设备
mmc part 	列出mmc设备的分区
mmc dev 	切换mmc设备
mmc list 	列出当前的mmc设备
mmc hwpartition 	设置mmc设备的分区
mmc bootbus 	设置指定mmc设备的BOOT_BUS_WIDTH的值
mmc bootpart 	设置指定mmc设备的boot和RPMB分区大小
mmc partconf 	设置指定mmc设备的PARTITION_CONFIG的值
mmc rst 	mmc设备复位
mmc setdsr 	设置mmc设备DSR寄存器的值

接下来，依次看一下这些命令的使用。

（1）mmc info

 mmc info命令用来显示当前选中的mmc设备的信息，直接输入下面的命令即可：

=> mmc info

在我当前的目标板中，接入了一张Mico SD卡，其输出信息如下：

从输出信息可以知道，SD卡的速率为25MHz，容量为7.4GB（SD卡为8GB卡），总线的宽度为4bit。

（2）mmc rescan

mmc rescan命令用于扫描当前目标板上所有的mmc设备，包括eMMC和SD卡，命令输入如下：

=> mmc rescan

（3）mmc list

mmc list命令可用于查看当前目标板共有多少个mmc设备，输入命令如下：

=> mmc list

回车后，将会列出mmc设备，我当前的输出如下：

从输出可以知道，当前默认的设备为SD卡。

（4）mmc dev

mmc dev命令可以用来切换当前的mmc设备，命令格式如下：

mmc dev [dev] [part]

其中[dev]表示要切换的mmc设备号，[part]是mmc设备的分区号，如果不写分区号，则默认为分区0。

例如，使用命令切换到eMMC设备：

#当0为SD卡，1为eMMC时
=> mmc dev 1

（5）mmc part

mmc part命令可以用来查看当前mmc设备的分区，例如查看当前sd卡中的分区：

=> mmc dev 0
=> mmc part

输出结果如下：

可以看到，当前只有一个分区，开始的扇区为2048。

（6）mmc read

 mmc read命令可用于读取mmc设备的数据，它的使用格式如下所示：

mmc read addr blk# cnt

其中addr是将数据读取到DRAM中的地址，blk是要读取的块起始地址，一块为512字节，cnt则是要读取的块的数量。

例如，从当前的SD卡设备的第2048块开始，读取20个块数据到DRAM的0x80000100地址处，该命令如下：

=> mmc list
=> mmc dev 0
=> mmc read 80000100 800 14

读取成功后，如下所示：

（7）mmc write

mmc write命令可以将DRAM中的数据写入到mmc设备里面，其命令的格式如下所示：

mmc write addr blk# cnt

其中addr是要写入到mmc设备中的数据在DRAM中的起始地址，blk是要写入mmc的块起始地址，cnt是要写入的块数量，一个块的大小为512字节。

例如，将DRAM地址0x80000100开始的数据，从mmc设备的2048个块开始烧写，烧写20个块，命令如下：

=> mmc list
=> mmc part
=> mmc write 80000100 2048 14

命令执行后，数据写入成功如下：

（8）mmc erase

mmc erase可以用来擦除mmc设备中指定的块，其使用命令格式如下：

mmc erase blk# cnt

其中blk是要擦除的起始块，cnt是要擦除的块数量。

例如，可以使用下面的命令擦除mmc设备从2048个块开始的20个块：

=> mmc list
=> mmc part
=> mmc erase 800 14

块擦除成功后，如下所示：

需要注意的是，不要随便使用该命令擦除mmc设备的前两个块，它里面保存着分区表。

 

7、FAT格式文件系统相关操作命令

当我们需要在uboot中对SD卡或者eMMC设备中存储的文件进行操作时，这个时候就需要用到uboot中文件系统的操作命令，对于FAT格式的文件系统操作相关的命令有fatinfo、fatls、fstype、fatload、fatwrite等，接下来，依次看看这些命令的用法。

（1）fatinfo命令

fatinfo命令用来查询mmc设备中指定分区的文件系统信息，该命令的用法如下：

该命令的用法中，<interface> 表示要查看的接口，例如mmc，[<dev[:part]>]中的dev表示要查询的设备号，part则表示要查询的分区。

例如，当前我的目标板有个sd卡设备，查看sd卡中分区1的文件系统信息，可以使用下面的命令：

=> mmc list
=> mmc part
=> fatinfo mmc 0:1

命令输入回车后，输出如下所示：

从输出的结果中可以看到该mmc设备的分区1的文件系统格式为FAT32的。

（2）fatls命令

fatls命令可以用于查询FAT格式文件系统的目录和文件信息，该命令的用法如下：

在该命令的用法中，<interface>表示要查询的接口，[<dev[:part]>]中dev表示要查询的设备号，part表示要查询分区，[directory]表示要查询的目录，如果该参数不输入的话，默认为根目录。

例如，查询我当前sd卡中分区1中的目录和文件，可以输入下面命令：

=> fatls mmc 0:1

命令输入回车后，将会列举根目录下所有的目录和文件，输出如下：

当我们再想查看img/目录下的目录或者文件时，可以使用下面的命令：

=> fatls mmc 0:1 img/

命令输入回车后，输出如下：

从输出中可以看到，该目录下存在着1个文件和两个目录，其中.和../是隐藏的目录。

（3）fstype命令

fstype命令可以用于查看mmc设备中某个分区的文件系统格式，该命令的用法如下所示：

从上面的输出可以看到，fstyp命令具有两个用法，第一个用来查看mmc设备分区中的文件系统类型，第二个则是用来设置文件系统类型的环境变量，对于第一个命令用法，<interface>表示接口，例如mmc，<dev>:<part>中dev则表示要查询的设备号，part则是设备的分区。

例如，查看我当前目标板中sd设备的第一个分区的文件系统类型，可以使用下面命令：

=> fstype mmc 0:1

命令输入后回车，输出如下所示：

可以看到当前sd卡的第一个分区文件系统格式为fat类型的。

（4）fatload命令

fatload命令用来将指定的文件读取到DRAM内存中，该命令的使用格式如下：

该命令的用法中，<interface>表示设备接口，例如mmc，[<dev[:part]>中的dev表示设备号，part表示mmc设备的分区，<addr>则是文件读取到DRAM中的起始地址，<filename>则是要读取的文件的名字，bytes表示要读取多少字节的数据，如果该值为0或者未使用，则表示将要读取整个文件，pos表示要读的文件相对于文件首地址的偏移，如果为0或者未使用，则表示从文件首地址开始读取。

例如，在我当前的目标帮中，将sd卡中第一个分区中的img/u-boot-imx6ul14x14evk_nand.imx文件读取到DRAM中0x80000100起始地址中，可以使用下面的命令：

=> fatls mmc 0:1 img/
=> fatload mmc 0:1 80000100 img/u-boot-imx6ul14x14evk_nand.imx

命令输入后回车，输出如下：

从输出结果可以看到，已经将该文件读取到了DRAM中，读取了470016个字节，速度未6.9MiB/s。

（5）fatwrite命令

需要注意的是，当在uboot的板级配置文件中定义了#define CONFIG_CMD_FAT宏，fatinfo、fatls、fatload命令才会出现在uboot命令中，而fatwrite命令则需要定义#define CONFIG_FAT_WRITE宏才会出现，因此，如果想要在uboot中使用fatwrite命令，则需要定义宏CONFIG_FAT_WRITE。 

fatwrite命令可以用于将DRAM中的数据写入到mmc设备中去，该命令的使用格式如下：

命令格式中，<interface>表示为接口，例如mmc，<dev[:part]>中dev表示为设备号，part表示为mmc设备的分区，<addr>则为要写入的数据在DRAM中的起始地址，<filename>表示写入的数据文件的名字，<bytes>表示要写入的字节数。

例如，在我的目标板系统中从DRAM地址0x80000100开始写20个字节，在sd卡设备的分区1中生成test.bin文件，可以使用下面命令：

=> mmc list
=> fatls mmc 0:1
=> fatwrite mmc 0:1 80000100 test.bin 14
=> fatls mmc 0:1

命令输入回车后，输出如下：

从输出结果可以看到，test.bin文件成功写入到sd卡，并且大小为20字节，在Linux嵌入式调试中，可以使用该命令进行uboot或者zImage文件的更新。

 

8、EXT格式文件系统相关操作命令

uboot中除了有FAT格式文件系统的相关操作命令外，还有EXT格式文件系统的相关操作命令，这些命令和实现的功能如下所示：
命令 	功能
ext2load 	从Ext2文件系统中加载二进制文件到DRAM中
ext2ls 	列举目录中的文件
ext4load 	从Ext4文件系统中加载二进制文件到DRAM中
ext4ls 	列举目录中的文件
ext4size 	修改文件大小
ext4write 	在root目录下新创建文件

以上命令的使用方式和FAT格式文件系统相关命令的使用方式相同。

例如，当前我目标板上的sd卡设备中的分区2是ext2文件系统，可以使用下面命令查询分区2中的目录和文件：

=> mmc list
=> mmc part
=> ext2ls mmc 0:2

命令输入后回车，输出如下所示：

从上图看到，列出了3个目录和1个文件，其中lost+found目录是在创建ext2文件系统的时候生成的，part2是测试的一个文件。

 

9、Nand Flash相关操作命令

uboot中除了有emmc子系统外，还具有nand子系统，所有uboot也是支持Nand Flash的，在这里，我使用的是盈鹏飞公司的CoM-P6UL核心板，板载了256MB的DRAM和256MB的Nand Flash，支持从Nand Flash中启动，Nand Flash的分区如下：
Name 	Offset 	Size 	说明
uboot 	0x0 	0x400000 	4MB
kernel 	0x400000 	0x1000000 	16MB
dtb 	0x1400000 	0x80000 	512KB
rootfs 	0x1480000 	0xeb80000 	235.5MB

在uboot中输入下面的命令可以查看Nand子系统所支持的所有命令：

=> ? nand

将输出如下所示：

从上面可以看到，uboot中对于Nand Flash的命令是非常多的，接下来，对一些常使用的命令进行分析。

（1）nand info命令

nand info命令可以用来打印目标板上Nand Flash的相关信息，直接输入下面命令回车即可：

=> nand info

在我当前目标板上，输出如下所示：

从输出可以看到，它打印输出了当前Nand Flash的页面大小和00B大小等相关信息。

（2）nand device命令

nand device命令能用于显示Nand Flash的信息，也能用于切换目标板上的Nand Flash，如果目标板支持多块Nand Flash的话，可以使用该命令进行切换。

（3）nand erase命令

nand erase命令可以用于擦除Nand Flash，在对Nand Flash中写入数据之前，必须要先对写的区域进行擦除，然后才能保证数据能写入进擦除的区域内，nand erase命令的形式有3种，如下：

第一种形式如下：

nand erase[.spread] [clean] off size 

off表示Nand Flash的偏移地址，也就是要擦除区域的起始地址，size表示要擦除的区域大小。

例如，可以使用下面的命令将Nand Flash存储Linux内核镜像zImage的区域进行擦除：

=> nand erase 0x400000 0x1000000

命令输入回车后，输出如下：

第二种形式如下：

nand erase.part [clean] partition

表示擦除指定的区域。

第三种形式如下：

nand erase.chip [clean]

该形式将会将整个Nand Flash进行擦除，nand erase命令一般是配后nand write命令进行配后使用。

（4）nand read命令

nand read命令可以用于从Nand Flash中指定的地址读取指定大小的数据到DRAM中，该命令的使用格式如下：

nand read - addr off|partition size

命令使用格式中，addr表示DRAM的地址，off表示要读取的Nand Flash的区域的起始地址，size表示要读取的数据大小。

例如，可以使用下面的命令读取Linux内核镜像zImage文件到DRAM的0x80800000地址处：

=> nand read 0x80800000 0x400000 0x1000000

命令输入并回车，输出如下：

（5）nand write命令

nand write命令可以用于向Nand Flash中指定的地址写入指定大小的数据，一般和nand erase命令结合使用，还可以用来更新Nand Flash中的uboot、kernel和dtb等文件，该命令的使用格式如下：

nand write - addr off|partition size

和nand read命令类似，addr表示要写入的数据的开始地址，off表示Nand Flash中写入的起始地址，size表示要写入的数据大小。

例如，使用该命令并结合fatload命令进行Linux内核镜像zImage和dtb文件更新，将我们需要更新的文件放入到SD卡中，使用fatload命令将需要更新的zImage和dtb写入到DRAM中，然后使用nand write命令更新到Nand Flash中，如下：

将需要更新的zImage镜像文件读入到DRAM中：

=> fatload mmc 0:1 0x80800000 img/zImage

擦除Nand Flash中存储zImage的区域，并将DRAM中的zImage更新到Nand Flash中：

=> nand erase 0x400000 0x1000000
=> nand write 0x80800000 0x400000 0x1000000

使用nand write更新zImage后，输出如下：

接下来，按照类似的方法去更新dtb：

将需要更新的dtb设备树文件读入到DRAM中：

=> fatload mmc 0:1 0x83000000 img/imx6ul-14x14-evk.dtb

擦除Nand Flash中存储dtb设备树的区域，并将DRAM中的dtb文件更新到Nand Flash中：

=> nand erase 0x1400000 0x80000
=> nand write 0x83000000 0x1400000 0x80000

使用nand write更新dtb后，输出如下：

另外，还可以使用bootz命令启动Linux内核，使用下面命令即可：

=> bootz 0x80800000 - 0x83000000

启动log如下：

到此，uboot中Nand子系统的常用命令就描述完毕了，更多的命令可以自行探索。

 

10、BOOT启动相关操作命令

我们都知道uboot最主要的工作就是引导启动Linux系统，因此uboot中肯定是有相关的boot启动命令的，和boot启动常用相关的命令有boot、bootm和bootz，接下来，我们了解一下这些命令的使用。

（1）boot命令

boot命令是用来启动Linux系统的，该命令的用法如下：

可以看到到，该命令将会运行bootcmd，也就是boot命令将会读取bootcmd这个环境变量，并运行这个环境变量中的命令，来看一下当前我目标板中的bootcmd环境变量内容，使用下面命令查看：

=> print bootcmd

输出如下：

可以看到，该环境变量就是定义了一些启动引导的命令集合，先使用nand read命令从Nand Flash中读取kernel和dtb在DRAM内存地址中，然后使用bootz命令启动Linux系统。

（2）bootm命令

bootm命令用于启动uImage镜像，该命令的用法如下：

其中addr就是uImage镜像文件在DRAM的起始地址，另外它还有一些参数，如果要使用dtb或initrd的话，则就是在后面添加对应的DRAM地址。

（3）bootz命令

bootz命令也是用来启动Linux系统，只不过启动的是Linux zImage镜像文件，该命令用法如下：

命令中的addr是DRAM内存Linux镜像文件的起始地址，initrd是initrd文件在DRAM中的地址，fdt是设备树在DRAM中的地址，如果没有使用到initrd文件的话，则使用'-'进行替代，对于Linux镜像和设备树文件，我们可以通过读取eMMC或Nand Flash到DRAM中，当然，如果在网络接口可以使用的情况下，也可以使用NFS或TFTP服务将镜像下载到DRAM中，启动的原理是一样的。

例如，当Nand Flash中存储着我们需要启动的Linux系统镜像和设备树文件的话，可以使用下面命令进行启动：

先读取Linux zImage镜像到DRAM的0x80800000处：

=> nand read 0x80800000 0x400000 0x1000000

然后读取设备树文件到DRAM的0x83000000处：

=> nand read 0x83000000 0x1400000 0x80000

使用bootz启动引导Linux系统：

=> bootz 0x80800000 - 0x83000000

命令输入回车后，系统启动log如下：

可以看到，使用bootz命令能启动Linux系统了。

 

11、其它常用uboot命令

除了上面提及到的一些常用命令以外，uboot中还有一些另外常用的命令，例如reset、go、run和mtest等，接下来看看这些命令的使用。

（1）reset命令

reset命令能用来复位CPU，其用法如下：

直接输入下面命令并回车，将会复位CPU：

=> reset

（2）go命令

go命令能用于跳转到指定的内存地址处执行应用程序，该命令的使用格式如下：

命令用法中的addr就是内存的地址，将应用程序下载到该内存地址后，用go跳转到该地址处，将会执行该应用程序。

例如，编写一个简单的GPIO电平控制汇编程序gpioctrl.S，代码如下：
复制代码

.global _start

_start:
/* 1、使能IO时钟 */
ldr r0, =0x020c406c     /* 将寄存器地址CCM_CCGR1写入到r0 */
ldr r1, =0xffffffff     /* 将gpio1时钟使能 */
str r1,[r0]

/* 2、设置GPIO1_IO08引脚IO复用为GPIO1_IO08 */
ldr r0, =0x020e007c /* 将寄存器IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO08写入r0 */
ldr r1, =0x5    /* 设置IO引脚复用模式为GPIO1_IO08 */
str r1,[r0]

/* 3、配置GPIO1_IO08引脚电气属性 
 * bit [16]: 0 关闭HYS
 * bit [15:14]: 00 默认下拉
 * bit [13]: 0 keeper
 * bit [12]: 1 pull/keeper使能
 * bit [11]: 0 禁止开路输出
 * bit [7:6]: 10 速度为100MHz
 * bit [5:3]: 110 驱动能力为R0/6
 * bit [0]: 0 低摆率
 */
ldr r0, =0x020e0308   /* 将寄存器IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO08写入r0 */
ldr r1, =0x10b0 /* 设置GPIO1_IO08引脚电气属性 */
str r1,[r0]

/* 4、配置GPIO1_IO08引脚方向为输出 */
ldr r0, =0x0209c004     /* 将寄存器GPIO1_GDIR地址写入r0 */
ldr r1, =0x00000100     /* 将GPIO1_IO08方向设置为输出 */
str r1,[r0]

/* 5、控制GPIO1_IO08引脚电平高低 */
ldr r0, =0x0209c000     /* 将寄存器GPIO1_DR地址写入r0 */
ldr r1, =0x00000100     /* 将GPIO1_IO08引脚设置为高电平 */ 
str r1,[r0]

复制代码

编写对应的Makefile编译脚本，如下：
复制代码

all:gpioctrl.S
    arm-linux-gnueabihf-gcc -g -c gpioctrl.S -o gpioctrl.o
    arm-linux-gnueabihf-ld -Ttext 0x87800000 gpioctrl.o -o gpioctrl.elf
    arm-linux-gnueabihf-objcopy -O binary -S -g gpioctrl.elf gpioctrl.bin
    arm-linux-gnueabihf-objdump -D gpioctrl.elf > gpioctrl.dis

clean:
    rm -rf *.o gpioctrl.bin gpioctrl.elf gpioctrl.dis

复制代码

编译产生gpioctrl.bin文件后，将.bin文件拷贝到SDCard中，在uboot中使用fatload命令加载到内存0x87800000地址处，并使用go命令执行程序：

=> fatload mmc 0:1 0x87800000 gpioctrl.bin
=> go 0x87800000

程序运行后，对应的GPIO电平将得到控制。

（3）run命令

run命令能用来运行环境变量中定义的命令，例如通过run bootcmd来运行bootcmd中定义的启动命令，能将Linux系统进行启动，该命令能运行我们自己定义的环境变量，该用法如下：

用法中的var就是定义好的环境变量。

接下来，举例说明该命令如何使用，例如，当前目标板是Nand Flash启动的，我们想要通过SDCard去更新Nand Flash中存储的kernel和dtb，该怎么操作呢？可以将需要更新的固件保存到SDCard里面，然后通过fatload命令将需要更新的固件读入到DRAM里面，再通过nand write命令更新到Nand Flash里面，命令非常多，我们可以将这些使用到的命令定义为一个环境变量，然后在uboot中run var即可，在调试Linux系统的时候将会非常方便，接下来，看看怎么操作。

SDCard的分区1里面保存了需要更新的固件kernel和dtb，如下：

接下来，在uboot中使用下面命令，创建updatecmd环境变量并将环境变量进行保存：

=> setenv updatecmd 'fatload mmc 0:1 0x80800000 zImage;nand erase 0x400000 0x1000000;nand write 0x80800000 0x400000 0x1000000;fatload mmc 0:1 0x83000000 imx6ul-14x14-evk.dtb;nand erase 0x1400000 0x80000;nand write 0x83000000 0x1400000 0x80000'
=> saveenv

使用print命令查看updatecmd环境变量是否创建成功：

=> print updatecmd

输出如下，则创建成功：

接下来，将保存了需要更新固件的SDCard接入到目标板中，并使用run命令进行固件更新：

=> run updatecmd

命令运行后，固件更新成功，输出如下：

使用该命令能很方便地在uboot中完成了kernel和dtb固件的更新。

（4）mtest命令

mtest命令能用于进行内存读写测试，例如可以用来测试目标板DDR的稳定性，该命令的用法如下：

命令用法中的start是DRAM内存的起始地址，end是内存的结束地址，例如我们向测试0x80000000到0x87800000这段内存，可以使用下面命令：

=> mtest 80000000 87800000

测试结果如下：

测试的时候，如果想退出测试的话，可以使用键盘上的"Ctrl+C"组合键。



解决 Warning - bad CRC, using default environment警告

```bash
=> env default -a
## Resetting to default environment

=> saveenv 
Saving Environment to MMC...
Writing to MMC(0)... done



```

