


# 关于爱芯的mmc的系统包重新配置


在替换爱芯的 rootfs 时，如果直接替换 sdk/rootfs 中的文件，系统是无法正常启动的。换一种方法，直接替换爱芯 axp 包内 rootfs，安装后就能正常启动了。


下面是替换的详细步骤。
```bash
# sudo apt install android-sdk-libsparse-utils

axp_pag=$1  
rootfs_pag=$2

mkdir axp_tmp
unzip $axp_pag -d axp_tmp

rm axp_tmp/rootfs_sparse.ext4

img2simg $rootfs_pag axp_tmp/rootfs_sparse.ext4 4096

cd axp_tmp
zip -r ../out.zip .
cd ..

mv out.zip out.zip.axp
rm -r axp_tmp

```


```bash
rm rootfs_sparse.ext4
img2simg /home/nihao/work/img/ax630c_llm/compile-openwrt-21-rootfs-20240819.ext4 rootfs_sparse.ext4 4096
zip -r ../out.zip .
cd .. && mv out.zip out.zip.axp

# 解包simg2img sparse.img raw.img
```
/soc/scripts/usb-adb.sh start && { /usr/local/m5stack/bin/ax_usb_adb_event.sh &> /dev/null & }


还有就是爱芯平台在linux中的下载，本来是想进行协议逆向，但是考虑到工作量，暂时还不想做。




# 关于爱芯对 linux 系统的适配的记录

在移植 rootfs 过程中，遇到了一个爱芯 ddr 打印的问题。准确的来说，这个也不算问题。因为系统ddr的初始化确实需要那样做，但它在第二次启动时打印了太多的log了。
刚开始，只有新做的openwrt系统存在这个问题，爱芯自己的rootfs不存在。我以为爱芯对rootfs做了什么特殊的事情。但我排查梳理的bl、kernel、atf、rootfs时，发现了一些问题。
第一、爱芯对整个系统的适配大多数的时候只做了初始化处理，除非像必须释放资源的设别才会有关闭适配。像poweroff、reboot、这种操作，几乎没有适配。
第二、bl阶段的代码，有很多的log打印函数，没有做log打印管理，或许是我没发现。但是这真的带来的很大的困扰。为此，我强制关闭了ddr初始化代码文件中的所用打印函数。
总的来说，代码质量不算太好。不过无所谓了，几乎不会有人去动这部分代码。但遇到问题后，几乎不存在解决的机会。（对ddr上千个寄存器的初始化操作完全没有兴趣～-～）。


新产品的内存定义到4+32,emmc的储存加大带来了固件的增大。同时烧录时间也被拉长。
为此期望在尽可能的缩小固件的大小来减少固件的烧录时间。
首先尝试了减少 rootfs 大小的方式来减少烧录时间，这个做法确实生效了，预计节约了1分钟所有的烧录时间，但是这样的做法造成了文件系统和分区大小不匹配。由于是根分区，所以没有办法直接进行resize2fs
。只能尝试挂载到其他分区进行resize2fs，然后跳转会来。最快的情况需要大该半分钟，但也存在需要50多秒的尝试时间。所以这样几乎无法完成很好的扩容工作，反而增加了系统的制作难度。
我也尝试单独增加一个分区来实现这个操作，但发现，对于整个系统来讲，几乎无法有效的把整体的烧录时间降下来，反而增加了大量的工作量。
为此决定，只进行rootfs的替换，不再做其他的操作了。
慢速设备中，尝试做整体的烧录时间优化是一个非常痛苦的操作。



