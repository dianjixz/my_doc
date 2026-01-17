# podman
podman 是红帽开发的一个对标 docker 的容器,原本是在研究 bootc 项目的是否发现的这个,bootc 使用 podman 制作启动镜像.


后面发现 podman 的用法要比 docker 还要强大,podman 竟然能替代 chroot ,直接进入系统,进行运行.








代替 chroot 的进行时,直接进入 rootfs 然后操作内部的镜像文件.
```bash
sudo podman run --rm -it --rootfs `pwd`/rootfs /bin/sh
```
