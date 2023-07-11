 

# [在根文件系统中查看设备树(有助于调试)](https://www.cnblogs.com/zhangshuaifeng/p/10733592.html)

**a. /sys/firmware/fdt**        

进入/sys/firmware目录后便可看到二个文件，一个是devicetree文件夹，另一个是fdt（原始dtb文件，可以用hexdump -C fdt 将其打印出来查看就会发现里面的数据和dtb文件是一致的）。

**b. /sys/firmware/devicetree**

以目录结构呈现的dtb文件。 根节点对应base目录, 每一个节点对应一个目录, 每一个属性对应一个文件

**c. /sys/devices/platform**

系统中所有的platform_device, 有来自设备树的, 也有来有.c文件中注册的
对于来自设备树的platform_device,可以进入 /sys/devices/platform/<设备名>/of_node 查看它的设备树属性（例如进入/sys/devices/platform/led/后若发现该目录下有of_node节点，就表明该platform_device来自设备树）

**d. /proc/device-tree**

是链接文件, 指向 /sys/firmware/devicetree/base



一、设备树编译
有两种方式
1、将设备树文件拷贝到内核源码的arch/*（处理器平台）/boot/dts/*（厂家）/目录下，
   执行make dtbs
2、dtc -I dts -O dtb  *.dts > my.dtb

dtc -I dts -O dtb -o output.dtb file1.dts file2.dts file3.dts


二、设备树反编译
dtc -I dtb -O dts *.dtb > my.dts
————————————————
版权声明：本文为CSDN博主「Alston若水」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/SU3752/article/details/80377596
