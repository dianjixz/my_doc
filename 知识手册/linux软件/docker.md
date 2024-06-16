 

# [Docker: 如何修改 Docker 的镜像存储位置](https://www.cnblogs.com/csharpsharper/p/5685263.html)             

我用的阿里云的服务器, 但是系统盘只有20G, 默认 Docker 的镜像文件是安装在/var/lib 目录下的, 这样的话我根本装不了太多的镜像... 这个必须得改改...
 搜了下, 解决方案如下:

#### 方案1,  使用参数-g 来修改 Docker 的镜像存储文件夹.

修改方法如下:
 在 Ubuntu/Debian 系统下:
 编辑 /etc/default/docker 文件, 添加-g 参数的设置, 如下:

```ini
 DOCKER_OPTS="-dns 8.8.8.8 -dns 8.8.4.4 -g /mnt"
```

在 Fedora/Centos 系统下:

编辑 /etc/sysconfig/docker 文件, 添加-g 参数的设置, 如下:

```ini
  other_args="-g /mnt"
```

重启 Docker 服务, 问题就解决了.

#### 方案2 使用链接

1. 停止 Docker: service docker stop.
2. 做个备份 tar -zcC /var/lib/docker > /mnt/var_lib_docker-backup-$(date + %s).tar.gz
3. 迁移/var/lib/docker目录到met 目录下: mv /var/lib/docker /mnt/docker
4. 建个 symlink: ln -s /mnt/docker /var/lib/docker
5. 确认文件夹类型为symlink 类型 ls /var/lib/docker
6. 启动 docker service.



https://www.cnblogs.com/csharpsharper/p/5685263.html

# Docker 导出离线镜像
## 方法一
使用 save 和 load 命令
```bash
docker save -o coredns.tar k8s.gcr.io/coredns:1.3.1
```
```bash
docker load < coredns.tar
```
## 方法二
使用 export 和 import 命令
```bash
docker export eb516548c180 > coredns.tar
```
```bash
docker import /usr/local/coredns.tar
# or
# cat coredns.tar | docker import - k8s.gcr.io/coredns:1.3.1(镜像名自己定义)
```

## 查看镜像是否导入成功
```bash
docker images
```
