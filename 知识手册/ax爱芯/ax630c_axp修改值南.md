


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

还有就是爱芯平台在linux中的下载，本来是想进行协议逆向，但是考虑到工作量，暂时还不想做。