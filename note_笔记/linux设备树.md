

sudo apt install device-tree-compiler

# 反编译dtb
dtc -I dtb -O dts am335x-evm2.dtb -o mytest.dts
本机没有这个命令，所以需要在docker中使用。


update_dtb /dev/mmcblk0 sipeed.dtb

导出dtb是需要进行对齐

- dd if=./image/sunxi.dtb of=sipeed.dtb bs=72k count=1 conv=sync




docker cp 75e3b57ac014:/home/nihao/sdk/tina-d1-open_new/out/d1-nezha/image/sunxi.dtb .

dtc -I dtb -O dts sunxi.dtb -o sunxi.dts








