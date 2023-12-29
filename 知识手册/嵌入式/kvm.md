kvm在线调整内存磁盘，以及如何迁移




KVM支持在线调整虚拟机的内存和磁盘大小,以及虚拟机的迁移。这里对KVM的这些功能进行说明:
在线调整内存:

    登录KVM主机,查看虚拟机的XML配置文件:
    bash

virsh edit <vm_name>

    1

    找到<memory>标签,修改内存大小,单位为KB。例如:
    xml

<memory unit='KiB'>4194304</memory>   # 4GB

    1

    保存退出,重启虚拟机使配置生效:virsh reboot <vm_name>。
    登录虚拟机确认内存大小变化。内存热添加成功。
    在线调整磁盘:
    登录KVM主机,使用qemu-img resize将磁盘镜像文件大小调整至新大小。例如:
    bash

qemu-img resize /path/to/disk.qcow2 +10G # 增加10G

    1

    找到虚拟机使用此磁盘的XML定义,更新<disk>标签中的capacity值。
    将新大小同步至虚拟机。方法1:
    bash

virsh blockresize <vm_name> <disk_name> <new_size>   # 新大小KB为单位

    1

方法2:重启虚拟机。
4. 登录虚拟机确认磁盘空间变化。磁盘热扩展成功。

虚拟机迁移:
一、
使用virsh migrate命令可以在KVM服务器间迁移虚拟机,可以实现虚拟机的负载均衡和高可用。
迁移命令格式:virsh migrate <vm_name> <dest_host> [–live]

    –live表示使用live migration进行在线迁移。
    源和目标KVM服务器需要开启libvirtd服务并建立可信连接。
    虚拟机磁盘需要使用共享存储,NFS、iSCSI等。本地磁盘的虚拟机只能停机迁移。
    二、
    拷贝xml文件 迁移虚拟机
    在KVM中,我们可以通过拷贝虚拟机的XML配置文件来迁移虚拟机。具体步骤如下:

    在源KVM主机上找到虚拟机的配置文件。路径为/etc/libvirt/qemu/<vm_name>.xml。
    将此XML文件拷贝到目标KVM主机上的同一路径下。
    在两台主机上定义虚拟网络,并确保虚拟机绑定的虚拟网络在目标主机上也存在。或者网络使用bridge模式,则确保桥接的物理接口在两主机存在。
    将虚拟机磁盘文件拷贝到目标主机。如果使用的是本地磁盘,则需要停机拷贝;如果使用共享存储如NFS,则跳过此步骤。
    在源主机上将虚拟机停止:virsh shutdown <vm_name>
    在目标主机上定义虚拟机:
    bash

virsh define /etc/libvirt/qemu/<vm_name>.xml

    1

    启动虚拟机:virsh start <vm_name>
    登录虚拟机确认迁移成功。
    通过以上步骤,我们实现了通过拷贝XML配置文件的方式在KVM间迁移虚拟机。与virsh migrate命令相比,这种方式适用于本地磁盘的虚拟机,且虚拟机需要短暂停机。但可以在网络不通的情况下实现虚拟机迁移。
    注意:

    确保两主机上的虚拟机名称和UUID唯一。
    网络模式需要一致,或在目标主机上也存在。
    迁移前必须停止虚拟机。
    确保存储的虚拟磁盘能在目标主机访问。
————————————————
版权声明：本文为CSDN博主「蜗牛去旅行吧」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_54104864/article/details/130842127