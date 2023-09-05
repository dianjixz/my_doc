使用的 stm32mp1 的一些笔记
=======================





stm32mp1 启动是从特定地址的分区启动的，烧写系统时需要对 sd 卡进行分区。
``` bash
sudo sgdisk --resize-table=128 -a 1 -n 1:34:545 -c 1:fsbl1 -n 2:546:1057 -c 2:fsbl2 -n 3:1058:5153 -c 3:ssbl -n 4:5154:136225 -c 4:bootfs -n 5:136226 -c 5:rootfs -A 4:set:2 -p /dev/sdb -g

```

```bash
sudo sgdisk --resize-table=128 -a 1 \
-n 1:34:545             -c 1:fsbl1 \
-n 2:546:1057           -c 2:fsbl2 \
-n 3:1058:1569          -c 3:fsbl3 \
-n 4:1570:2081          -c 4:fsbl4 \
-n 5:2082:10273         -c 5:fsbl5 \
-n 6:10274:18465        -c 6:fsbl6 \
-n 7:18466:19489        -c 7:fsbl7 \
-n 8:19490:150561       -c 8:boot \
-n 9:150562:183329      -c 9:vendorfs \
-n 10:183330:1690657    -c 10:rootfs \
-g -P /dev/sdb





-n 1:34:545 -c 1:fsbl1 
-n 2:546:1057 -c 2:fsbl2 
-n 3:1058:1569 -c 3:ssb3 
-n 4:1570:2081 -c 4:bootfs 
-n 5:136226 -c 5:rootfs 
-A 4:set:2 -p /dev/sdb -g




```
sgdisk：

保存分区表：
sudo sgdisk --backup=partition_table_backup.bin /dev/sda

恢复的分区表：
sudo sgdisk --load-backup=partition_table_backup.bin /dev/sda


sfdisk：
保存分区表：
sudo sfdisk -d /dev/sdX > partition_table_backup.txt
or
sudo parted /dev/sdX print > partition_table_backup.txt






恢复的分区表：
sudo sfdisk /dev/sdX < partition_table_backup.txt
or
sudo parted /dev/sdX --script < partition_table_backup.txt




Disk /dev/sdb：14.84 GiB，15931539456 字节，31116288 个扇区
Disk model: Mass-Storage    
单元：扇区 / 1 * 512 = 512 字节
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节
磁盘标签类型：gpt
磁盘标识符：05E429DD-ADD8-425C-8212-E6E5CAE8B5CA

设备          起点    末尾    扇区   大小 类型
/dev/sdb1       34     545     512   256K Linux 保留            fsbl1
/dev/sdb2      546    1057     512   256K Linux 保留            fsbl2
/dev/sdb3     1058    1569     512   256K Linux 保留            metadata1
/dev/sdb4     1570    2081     512   256K Linux 保留            metadata2
/dev/sdb5     2082   10273    8192     4M 未知                  fipa
/dev/sdb6    10274   18465    8192     4M 未知                  fipb
/dev/sdb7    18466   19489    1024   512K Linux 保留            u-boot-env
/dev/sdb8    19490  150561  131072    64M Linux 文件系统         bootfs
/dev/sdb9   150562  183329   32768    16M Linux 文件系统         vendorfs
/dev/sdb10  183330 1690657 1507328   736M Linux 文件系统         rootfs
/dev/sdb11 1690658 3145694 1455037 710.5M Linux 文件系统         userfs


















|   fsbl1  |  fsbl2  |  metadata1  |  metadata2  |  fipa   |  fipb  |  u-boot-env   |  bootfs  |  vendorfs  |   rootfs  |  userfs |
|   -----  |  -----  |  ---------  |  ---------  |  ----   |  ----  |  ----------   |  ------  |  --------  |   ------  |  ------ |
|   256K   |  256K   |  256K       |  256K       |  4M     |  4M    |  512K         |   64M    |    16M     |   736M    |  710.5M |


``` bash
dd if=tf-a-sdcard.stm32 of=/dev/mmcblk0p1 conv=fdatasync
dd if=tf-a-sdcard.stm32 of=/dev/mmcblk0p2 conv=fdatasync
dd if=metadata.bin of=/dev/mmcblk0p3 conv=fdatasync
dd if=metadata.bin of=/dev/mmcblk0p4 conv=fdatasync
dd if=fip.bin of=/dev/mmcblk0p5 conv=fdatasync
dd if=/dev/zero of=/dev/mmcblk0p7 conv=fdatasync

```





stm32mp135 sdk 的 linux包编译时出错解决:
``` bash
cc1: error: cannot load plugin ./scripts/gcc-plugins/arm_ssp_per_task_plugin.so: ./scripts/gcc-plugins/arm_ssp_per_task_plugin.so: undefined symbol: _ZN8opt_pass14set_pass_paramEjb


Okay. I've got a dirty solution. Open .config (kernel configuration), find
CONFIG_HAVE_GCC_PLUGINS=y
CONFIG_GCC_PLUGINS=y
and change value to "n"
then try to make again
```





``` bash
#rarch=armautoload=nobaudrate=115200board=stm32mp1board_id=0x1635board_name=stm32mp135f-dkboard_rev=0x000bboot_a_script=load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script}; 
source ${scriptaddr}boot_device=mmcboot_efi_binary=load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} efi/boot/bootarm.efi; 
if fdt addr ${fdt_addr_r}; 
then bootefi ${kernel_addr_r} ${fdt_addr_r};
else bootefi ${kernel_addr_r} ${fdtcontroladdr};
fiboot_efi_bootmgr=if fdt addr ${fdt_addr_r}; 
then bootefi bootmgr ${fdt_addr_r};else bootefi bootmgr;fiboot_extlinux=sysboot ${devtype} ${devnum}:${distro_bootpart} any ${scriptaddr} ${prefix}${boot_syslinux_conf}boot_instance=0boot_net_usb_start=trueboot_prefixes=/ /boot/boot_script_dhcp=boot.scr.uimgboot_scripts=boot.scr.uimg boot.scrboot_syslinux_conf=extlinux/extlinux.confboot_targets=mmc1 ubifs0 mmc0 usb0 pxe bootcmd=run bootcmd_stm32mpbootcmd_mmc0=devnum=0; run mmc_bootbootcmd_mmc1=devnum=1; run mmc_bootbootcmd_pxe=run boot_net_usb_start; dhcp; if pxe get; then pxe boot; fibootcmd_stm32mp=echo "Boot over ${boot_device}${boot_instance}!";if test ${boot_device} = serial || test ${boot_device} = usb;then stm32prog ${boot_device} ${boot_instance}; else run env_check;if test ${boot_device} = mmc;then env set boot_targets "mmc${boot_instance}"; fi;if test ${boot_device} = nand || test ${boot_device} = spi-nand ;then env set boot_targets ubifs0; fi;if test ${boot_device} = nor;then env set boot_targets mmc0; fi;run distro_bootcmd;fi;bootcmd_ubifs0=devnum=0; run ubifs_bootbootcmd_usb0=devnum=0; run usb_bootbootdelay=1
console=ttySTM0
cpu=armv7distro_bootcmd=for target in ${boot_targets}; do run bootcmd_${target}; doneefi_dtb_prefixes=/ /dtb/ /dtb/current/env_check=if env info -p -d -q; then env save; fieth1addr=10:e7:7a:e3:40:14ethaddr=10:e7:7a:e3:40:13fdt_addr_r=0xc4000000fdtcontroladdr=dabf0e10fdtfile=stm32mp135f-dk.dtb
fdtoverlay_addr_r=0xc4300000
kernel_addr_r=0xc2000000
load_efi_dtb=load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${prefix}${efi_fdtfile}loadaddr=0xc2000000mmc_boot=if mmc dev ${devnum}; then devtype=mmc; run scan_dev_for_boot_part; fipxefile_addr_r=0xc4200000ramdisk_addr_r=0xc4400000scan_dev_for_boot=echo Scanning ${devtype} ${devnum}:${distro_bootpart}...; for prefix in ${boot_prefixes}; do run scan_dev_for_extlinux; run scan_dev_for_scripts; done;run scan_dev_for_efi;scan_dev_for_boot_part=part list ${devtype} ${devnum} -bootable devplist; env exists devplist || setenv devplist 1; for distro_bootpart in ${devplist}; do if fstype ${devtype} ${devnum}:${distro_bootpart} bootfstype; then run scan_dev_for_boot; fi; done; setenv devplistscan_dev_for_efi=setenv efi_fdtfile ${fdtfile}; if test -z "${fdtfile}" -a -n "${soc}"; then setenv efi_fdtfile ${soc}-${board}${boardver}.dtb; fi; for prefix in ${efi_dtb_prefixes}; do if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${efi_fdtfile}; then run load_efi_dtb; fi;done;run boot_efi_bootmgr;if test -e ${devtype} ${devnum}:${distro_bootpart} efi/boot/bootarm.efi; then echo Found EFI removable media binary efi/boot/bootarm.efi; run boot_efi_binary; echo EFI LOAD FAILED: continuing...; fi; setenv efi_fdtfilescan_dev_for_extlinux=if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${boot_syslinux_conf}; then echo Found ${prefix}${boot_syslinux_conf}; run boot_extlinux; echo SCRIPT FAILED: continuing...; fiscan_dev_for_scripts=for script in ${boot_scripts}; do if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${script}; then echo Found U-Boot script ${prefix}${script}; run boot_a_script; echo SCRIPT FAILED: continuing...; fi; donescriptaddr=0xc4100000serial#=801D000B3031510A30333432serverip=192.168.1.1soc=stm32mpubifs_boot=env exists bootubipart || env set bootubipart UBI; env exists bootubivol || env set bootubivol boot; if ubi part ${bootubipart} && ubifsmount ubi${devnum}:${bootubivol}; then devtype=ubi; run scan_dev_for_boot; fiusb_boot=usb start; if usb dev ${devnum}; then devtype=usb; run scan_dev_for_boot_part; fiusb_pgood_delay=1000vendor=st\arch=armautoload=no
baudrate=115200
board=stm32mp1board_id=0x1635board_name=stm32mp135f-dkboard_rev=0x000bboot_a_script=load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script}; source ${scriptaddr}boot_device=mmcboot_efi_binary=load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} efi/boot/bootarm.efi; if fdt addr ${fdt_addr_r}; then bootefi ${kernel_addr_r} ${fdt_addr_r};else bootefi ${kernel_addr_r} ${fdtcontroladdr};fiboot_efi_bootmgr=if fdt addr ${fdt_addr_r}; then bootefi bootmgr ${fdt_addr_r};else bootefi bootmgr;fiboot_extlinux=run scan_m4fw;run scan_overlays; sysboot ${devtype} ${devnum}:${distro_bootpart} any ${scriptaddr} ${prefix}${boot_syslinux_conf}boot_instance=0boot_m4fw=rproc init; rproc load 0 ${m4fw_addr} ${filesize}; rproc start 0boot_net_usb_start=trueboot_prefixes=/mmc0_boot_script_dhcp=boot.scr.uimgboot_scripts=boot.scr.uimg boot.scrboot_syslinux_conf=extlinux/stm32mp135f-dk_extlinux.confboot_targets=mmc0bootcmd=run bootcmd_stm32mpbootcmd_mmc0=devnum=0; run mmc_bootbootcmd_mmc1=devnum=1; run mmc_bootbootcmd_pxe=run boot_net_usb_start; dhcp; if pxe get; then pxe boot; fibootcmd_stm32mp=echo "Boot over ${boot_device}${boot_instance}!";if test ${boot_device} = serial || test ${boot_device} = usb;then stm32prog ${boot_device} ${boot_instance}; else run env_check;if test ${boot_device} = mmc;then env set boot_targets "mmc${boot_instance}"; fi;if test ${boot_device} = nand || test ${boot_device} = spi-nand ;then env set boot_targets ubifs0; fi;if test ${boot_device} = nor;then env set boot_targets mmc0; fi;run distro_bootcmd;fi;bootcmd_ubifs0=devnum=0; run ubifs_bootbootcmd_usb0=devnum=0; run usb_bootbootdelay=1bootfstype=ext4console=ttySTM0cpu=armv7devplist=8distro_bootcmd=for target in ${boot_targets}; do run bootcmd_${target}; doneefi_dtb_prefixes=/ /dtb/ /dtb/current/env_check=if env info -p -d -q; then env save; fieth1addr=10:e7:7a:e3:40:14ethaddr=10:e7:7a:e3:40:13fdt_addr_r=0xc4000000fdtcontroladdr=dabf0e10fdtfile=stm32mp135f-dk.dtbfdtoverlay_addr_r=0xc4300000fileaddr=c4100000filesize=efdkernel_addr_r=0xc2000000load_efi_dtb=load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${prefix}${efi_fdtfile}loadaddr=0xc2000000m4fw_addr=0xc2000000m4fw_name=rproc-m4-fw.elfmmc_boot=if mmc dev ${devnum}; then devtype=mmc; run scan_dev_for_boot_part; fiov_apply=test -n ${fdtovaddr} && test -n ${overlay} && for ov in ${overlay}; do echo overlaying ${ov}...; load ${devtype} ${devnum}:${distro_bootpart} ${fdtovaddr} /overlays/${ov}.dtbo && fdt resize ${filesize} && fdt apply ${fdtovaddr}; doneov_init=load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${fdtfile} && env set fdt_addr ${fdt_addr_r} && fdt addr ${fdt_addr} && setexpr fdtovaddr ${fdt_addr} + C0000pxefile_addr_r=0xc4200000ramdisk_addr_r=0xc4400000scan_dev_for_boot=echo Scanning ${devtype} ${devnum}:${distro_bootpart}...; for prefix in ${boot_prefixes}; do run scan_dev_for_extlinux; run scan_dev_for_scripts; done;run scan_dev_for_efi;scan_dev_for_boot_part=part list ${devtype} ${devnum} -bootable devplist; env exists devplist || setenv devplist 1; for distro_bootpart in ${devplist}; do if fstype ${devtype} ${devnum}:${distro_bootpart} bootfstype; then run scan_dev_for_boot; fi; done; setenv devplistscan_dev_for_efi=setenv efi_fdtfile ${fdtfile}; if test -z "${fdtfile}" -a -n "${soc}"; then setenv efi_fdtfile ${soc}-${board}${boardver}.dtb; fi; for prefix in ${efi_dtb_prefixes}; do if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${efi_fdtfile}; then run load_efi_dtb; fi;done;run boot_efi_bootmgr;if test -e ${devtype} ${devnum}:${distro_bootpart} efi/boot/bootarm.efi; then echo Found EFI removable media binary efi/boot/bootarm.efi; run boot_efi_binary; echo EFI LOAD FAILED: continuing...; fi; setenv efi_fdtfilescan_dev_for_extlinux=if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${boot_syslinux_conf}; then echo Found ${prefix}${boot_syslinux_conf}; run boot_extlinux; echo SCRIPT FAILED: continuing...; fiscan_dev_for_scripts=for script in ${boot_scripts}; do if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${script}; then echo Found U-Boot script ${prefix}${script}; run boot_a_script; echo SCRIPT FAILED: continuing...; fi; donescan_m4fw=if test -e ${devtype} ${devnum}:${distro_bootpart} ${m4fw_name};then echo Found M4 FW $m4fw_name; if load ${devtype} ${devnum}:${distro_bootpart} ${m4fw_addr} ${m4fw_name}; then run boot_m4fw; fi; fi;scan_overlays=if test -e ${devtype} ${devnum}:${distro_bootpart} /overlays/overlays.txt && load ${devtype} ${devnum}:${distro_bootpart} ${loadaddr} /overlays/overlays.txt && env import -t ${loadaddr} ${filesize}; then echo loaded overlay.txt: ${overlay}; run ov_init; run ov_apply; fiscriptaddr=0xc4100000serial#=801D000B3031510A30333432serverip=192.168.1.1soc=stm32mpubifs_boot=env exists bootubipart || env set bootubipart UBI; env exists bootubivol || env set bootubivol boot; if ubi part ${bootubipart} && ubifsmount ubi${devnum}:${bootubivol}; then devtype=ubi; run scan_dev_for_boot; fiusb_boot=usb start; if usb dev ${devnum}; then devtype=usb; run scan_dev_for_boot_part; fiusb_pgood_delay=1000vendor=st
```


https://blog.csdn.net/qq_36769014/article/details/128126435
stm32mp1系列使用了 drm 显示框架，

# 测试屏幕
modetest -M stm -s 32:480x272


modetest -M stm -s 32:#0




stm32mp1 的系统制作。  
系统制作需要调整的不多，主要是 uboot ， linux 内核，和 rootfs 。
stm32mp1 系列官方提供了 gcc 工具链，直接加载编译环境就能有很方便的使用了。简单记录一下 uboot 和 linux 的编译过程。

## uboot

``` bash
tar xf u-boot-stm32mp-v2021.10-stm32mp1-r1-r0.tar.xz
cd u-boot-stm32mp-v2021.10-stm32mp1-r1
for p in `ls -1 ../*.patch`; do patch -p1 < $p; done

make stm32mp13_defconfig
make DEVICE_TREE=stm32mp135f-dk all

```

## linux

``` bash
tar xfJ linux-5.15.24.tar.xz
cd linux-5.15.24
for p in `ls -1 ../*.patch`; do patch -p1 < $p; done

make multi_v7_defconfig fragment*.config
make uImage vmlinux dtbs LOADADDR=0xC2000040


make INSTALL_MOD_PATH="../build/install_artifact" modules_install

# arch/arm/boot/uImage
# cp $PWD/../build/arch/arm/boot/uImage $PWD/../build/install_artifact/boot/
# cp $PWD/../build/arch/arm/boot/dts/st*.dtb $PWD/../build/install_artifact/boot/
```




stm32mp135 的 boot 文件是合成的：
/dev/sdb1    tf-a
/dev/sdb2    tf-a
/dev/sdb3    tf-a-metadata
/dev/sdb4    tf-a-metadata
/dev/sdb5    optee-os + u-boot
/dev/sdb6    optee-os + u-boot
/dev/sdb7    u-boot-env
/dev/sdb8    bootfs
/dev/sdb9    vendorfs
/dev/sdb10   rootfs
/dev/sdb11   userfs



root_dir = /home/nihao/work/stm32mp/nihso/STM32MP1Dev/stm32mp1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sources/arm-ostl-linux-gnueabi/optee-os-stm32mp-3.16.0-stm32mp1-r1-r0/optee-os-stm32mp-3.16.0-stm32mp1-r1



fiptool create \
--fw-config ${root_dir}/../../FIP_artifacts/arm-trusted-firmware/fwconfig/stm32mp135f-dk-fw-config-optee.dtb \
--hw-config ${root_dir}/../../FIP_artifacts/u-boot/u-boot-stm32mp135f-dk-trusted.dtb \

--nt-fw ${root_dir}/../../FIP_artifacts/u-boot/u-boot-nodtb-stm32mp13.bin \

--tos-fw ${root_dir}/../deploy/tee-header_v2-stm32mp135f-dk.bin \
--tos-fw-extra1 ${root_dir}/../deploy/tee-pager_v2-stm32mp135f-dk.bin \
--tos-fw-extra2 ${root_dir}/../deploy/tee-pageable_v2-stm32mp135f-dk.bin \

${root_dir}/../../FIP_artifacts/fip/fip-stm32mp135f-dk-optee.bin



stm32mp135的rtc功能会影响到 GPIO PI1 引脚上，会导致改引脚有异常的 32khz 的时钟信号