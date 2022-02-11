 

# 嵌入式linux内核添加USB模块（U盘）支持

使用menuconfig工具进行配置
 1、Device Drivers->SCSI device support->SCSI disk support此选项必须勾选
 2、Device Drivers->USB support->Support for Host-side USB 此选项选中后会出现子菜单，故必须选中
 3、Device Drivers->USB support->USB Mass Storage support 此选项必须选中，否则/dev/下没有设备
 4、因为我是在DM6467平台下，所以需要Device Drivers->USB support->Inventra USB  Highspeed Dual Role Controller Support选项，并选择其子选项下的DaVinci 644x USB  support下Driver Mode为Host
 编译内核即可，下载内核后成功支持USB的热插拔及其文件系统

