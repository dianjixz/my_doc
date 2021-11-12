~~~ bash
nihao@nihao-V:~/work/emdebian/mindb$ sudo multistrap -a armhf -f multistrap_mindb.conf
multistrap 2.2.11 using multistrap_mindb.conf
multistrap 2.2.11 using multistrap_mindb.conf
Using foreign architecture: armhf
multistrap building armhf multistrap on 'amd64'
Getting package lists: APT_CONFIG=/tmp/multistrap.3FWzsu apt-get  -o Apt::Architecture=armhf -o Dir::Etc::TrustedParts=/home/nihao/work/emdebian/mindb/target-rootfs/etc/apt/trusted.gpg.d -o Dir::Etc::Trusted=/home/nihao/work/emdebian/mindb/target-rootfs/etc/apt/trusted.gpg -o Apt::Get::AllowUnauthenticated=true -o Apt::Get::Download-Only=true -o Apt::Install-Recommends=false -o Dir=/home/nihao/work/emdebian/mindb/target-rootfs/ -o Dir::Etc=/home/nihao/work/emdebian/mindb/target-rootfs/etc/apt/ -o Dir::Etc::Parts=/home/nihao/work/emdebian/mindb/target-rootfs/etc/apt/apt.conf.d/ -o Dir::Etc::PreferencesParts=/home/nihao/work/emdebian/mindb/target-rootfs/etc/apt/preferences.d/ -o APT::Default-Release='*' -o Dir::State=/home/nihao/work/emdebian/mindb/target-rootfs/var/lib/apt/ -o Dir::State::Status=/home/nihao/work/emdebian/mindb/target-rootfs/var/lib/dpkg/status -o Dir::Cache=/home/nihao/work/emdebian/mindb/target-rootfs/var/cache/apt/ update
忽略:1 http://mirrors.tuna.tsinghua.edu.cn/debian stretch InRelease
获取:2 http://mirrors.tuna.tsinghua.edu.cn/debian stretch Release [118 kB]
获取:3 http://mirrors.tuna.tsinghua.edu.cn/debian stretch Release.gpg [3,177 B]
忽略:3 http://mirrors.tuna.tsinghua.edu.cn/debian stretch Release.gpg
正在读取软件包列表... 完成
W: GPG 错误：http://mirrors.tuna.tsinghua.edu.cn/debian stretch Release: 由于没有公钥，无法验证下列签名： NO_PUBKEY 04EE7237B7D453EC NO_PUBKEY 648ACFD622F3D138 NO_PUBKEY 0E98404D386FA1D9 NO_PUBKEY EF0F382A1A7B6500
E: 仓库 “http://ftp2.cn.debian.org/debian stretch Release” 没有数字签名。
N: 无法安全地用该源进行更新，所以默认禁用该源。
N: 参见 apt-secure(8) 手册以了解仓库创建和用户配置方面的细节。
apt update failed. Exit value: 100
nihao@nihao-V:~/work/emdebian/mindb$ apt-key list
Warning: apt-key is deprecated. Manage keyring files in trusted.gpg.d instead (see apt-key(8)).
/etc/apt/trusted.gpg
--------------------
pub   rsa4096 2019-04-14 [SC] [有效至：2027-04-12]
      80D1 5823 B7FD 1561 F9F7  BCDD DC30 D7C2 3CBB ABEE
uid           [ 未知 ] Debian Archive Automatic Signing Key (10/buster) <ftpmaster@debian.org>
sub   rsa4096 2019-04-14 [S] [有效至：2027-04-12]

/etc/apt/trusted.gpg.d/debian-ports-archive-2019.gpg
----------------------------------------------------
pub   rsa4096 2017-12-30 [SC] [已过期：2020-01-31]
      6657 1731 B5A7 1F91 C501  F3FD DA1B 2CEA 81DC BC61
uid           [已过期] Debian Ports Archive Automatic Signing Key (2019) <ftpmaster@ports-master.debian.org>

/etc/apt/trusted.gpg.d/debian-ports-archive-2021.gpg
----------------------------------------------------
pub   rsa4096 2019-11-05 [SC] [有效至：2022-01-31]
      0581 65A2 348E 0226 6CC1  B73A 5A88 D659 DCB8 11BB
uid           [ 未知 ] Debian Ports Archive Automatic Signing Key (2021) <ftpmaster@ports-master.debian.org>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2012-cdimage.gpg
------------------------------------------------------
pub   rsa4096 2012-05-11 [SC]
      8439 38DF 228D 22F7 B374  2BC0 D94A A3F0 EFE2 1092
uid           [ 未知 ] Ubuntu CD Image Automatic Signing Key (2012) <cdimage@ubuntu.com>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2018-archive.gpg
------------------------------------------------------
pub   rsa4096 2018-09-17 [SC]
      F6EC B376 2474 EDA9 D21B  7022 8719 20D1 991B C93C
uid           [ 未知 ] Ubuntu Archive Automatic Signing Key (2018) <ftpmaster@ubuntu.com>

nihao@nihao-V:~/work/emdebian/mindb$ rm /etc/apt/trusted.gpg
rm：是否删除有写保护的普通文件 '/etc/apt/trusted.gpg'？ ^C
nihao@nihao-V:~/work/emdebian/mindb$ sudo rm /etc/apt/trusted.gpg
nihao@nihao-V:~/work/emdebian/mindb$ gpg --keyserver keyserver.ubuntu.com --recv-keys 04EE7237B7D453EC
gpg: 密钥 E0B11894F66AEC98：“Debian Archive Automatic Signing Key (9/stretch) <ftpmaster@debian.org>”未改变
gpg: 合计被处理的数量：1
gpg:              未改变：1
nihao@nihao-V:~/work/emdebian/mindb$ gpg --keyserver keyserver.ubuntu.com --recv-keys 648ACFD622F3D138
gpg: 密钥 DC30D7C23CBBABEE：“Debian Archive Automatic Signing Key (10/buster) <ftpmaster@debian.org>”未改变
gpg: 合计被处理的数量：1
gpg:              未改变：1
nihao@nihao-V:~/work/emdebian/mindb$ gpg --keyserver keyserver.ubuntu.com --recv-keys 0E98404D386FA1D9
gpg: 密钥 73A4F27B8DD47936：公钥“Debian Archive Automatic Signing Key (11/bullseye) <ftpmaster@debian.org>”已导入
gpg: 合计被处理的数量：1
gpg:               已导入：1
nihao@nihao-V:~/work/emdebian/mindb$ gpg --keyserver keyserver.ubuntu.com --recv-keys EF0F382A1A7B6500
gpg: 密钥 EF0F382A1A7B6500：公钥“Debian Stable Release Key (9/stretch) <debian-release@lists.debian.org>”已导入
gpg: 合计被处理的数量：1
gpg:               已导入：1
nihao@nihao-V:~/work/emdebian/mindb$ apt-key list
Warning: apt-key is deprecated. Manage keyring files in trusted.gpg.d instead (see apt-key(8)).
/etc/apt/trusted.gpg.d/debian-ports-archive-2019.gpg
----------------------------------------------------
pub   rsa4096 2017-12-30 [SC] [已过期：2020-01-31]
      6657 1731 B5A7 1F91 C501  F3FD DA1B 2CEA 81DC BC61
uid           [已过期] Debian Ports Archive Automatic Signing Key (2019) <ftpmaster@ports-master.debian.org>

/etc/apt/trusted.gpg.d/debian-ports-archive-2021.gpg
----------------------------------------------------
pub   rsa4096 2019-11-05 [SC] [有效至：2022-01-31]
      0581 65A2 348E 0226 6CC1  B73A 5A88 D659 DCB8 11BB
uid           [ 未知 ] Debian Ports Archive Automatic Signing Key (2021) <ftpmaster@ports-master.debian.org>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2012-cdimage.gpg
------------------------------------------------------
pub   rsa4096 2012-05-11 [SC]
      8439 38DF 228D 22F7 B374  2BC0 D94A A3F0 EFE2 1092
uid           [ 未知 ] Ubuntu CD Image Automatic Signing Key (2012) <cdimage@ubuntu.com>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2018-archive.gpg
------------------------------------------------------
pub   rsa4096 2018-09-17 [SC]
      F6EC B376 2474 EDA9 D21B  7022 8719 20D1 991B C93C
uid           [ 未知 ] Ubuntu Archive Automatic Signing Key (2018) <ftpmaster@ubuntu.com>

nihao@nihao-V:~/work/emdebian/mindb$ gpg --armor --export 04EE7237B7D453EC | sudo apt-key add -
Warning: apt-key is deprecated. Manage keyring files in trusted.gpg.d instead (see apt-key(8)).
OK
nihao@nihao-V:~/work/emdebian/mindb$ gpg --armor --export 648ACFD622F3D138 | sudo apt-key add -
Warning: apt-key is deprecated. Manage keyring files in trusted.gpg.d instead (see apt-key(8)).
OK
nihao@nihao-V:~/work/emdebian/mindb$ gpg --armor --export 0E98404D386FA1D9 | sudo apt-key add -
Warning: apt-key is deprecated. Manage keyring files in trusted.gpg.d instead (see apt-key(8)).
OK
nihao@nihao-V:~/work/emdebian/mindb$ gpg --armor --export EF0F382A1A7B6500 | sudo apt-key add -
Warning: apt-key is deprecated. Manage keyring files in trusted.gpg.d instead (see apt-key(8)).
OK
nihao@nihao-V:~/work/emdebian/mindb$ apt-key list
Warning: apt-key is deprecated. Manage keyring files in trusted.gpg.d instead (see apt-key(8)).
/etc/apt/trusted.gpg
--------------------
pub   rsa4096 2017-05-22 [SC] [有效至：2025-05-20]
      E1CF 20DD FFE4 B89E 8026  58F1 E0B1 1894 F66A EC98
uid           [ 未知 ] Debian Archive Automatic Signing Key (9/stretch) <ftpmaster@debian.org>
sub   rsa4096 2017-05-22 [S] [有效至：2025-05-20]

pub   rsa4096 2019-04-14 [SC] [有效至：2027-04-12]
      80D1 5823 B7FD 1561 F9F7  BCDD DC30 D7C2 3CBB ABEE
uid           [ 未知 ] Debian Archive Automatic Signing Key (10/buster) <ftpmaster@debian.org>
sub   rsa4096 2019-04-14 [S] [有效至：2027-04-12]

pub   rsa4096 2021-01-17 [SC] [有效至：2029-01-15]
      1F89 983E 0081 FDE0 18F3  CC96 73A4 F27B 8DD4 7936
uid           [ 未知 ] Debian Archive Automatic Signing Key (11/bullseye) <ftpmaster@debian.org>
sub   rsa4096 2021-01-17 [S] [有效至：2029-01-15]

pub   rsa4096 2017-05-20 [SC] [有效至：2025-05-18]
      067E 3C45 6BAE 240A CEE8  8F6F EF0F 382A 1A7B 6500
uid           [ 未知 ] Debian Stable Release Key (9/stretch) <debian-release@lists.debian.org>

/etc/apt/trusted.gpg.d/debian-ports-archive-2019.gpg
----------------------------------------------------
pub   rsa4096 2017-12-30 [SC] [已过期：2020-01-31]
      6657 1731 B5A7 1F91 C501  F3FD DA1B 2CEA 81DC BC61
uid           [已过期] Debian Ports Archive Automatic Signing Key (2019) <ftpmaster@ports-master.debian.org>

/etc/apt/trusted.gpg.d/debian-ports-archive-2021.gpg
----------------------------------------------------
pub   rsa4096 2019-11-05 [SC] [有效至：2022-01-31]
      0581 65A2 348E 0226 6CC1  B73A 5A88 D659 DCB8 11BB
uid           [ 未知 ] Debian Ports Archive Automatic Signing Key (2021) <ftpmaster@ports-master.debian.org>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2012-cdimage.gpg
------------------------------------------------------
pub   rsa4096 2012-05-11 [SC]
      8439 38DF 228D 22F7 B374  2BC0 D94A A3F0 EFE2 1092
uid           [ 未知 ] Ubuntu CD Image Automatic Signing Key (2012) <cdimage@ubuntu.com>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2018-archive.gpg
------------------------------------------------------
pub   rsa4096 2018-09-17 [SC]
      F6EC B376 2474 EDA9 D21B  7022 8719 20D1 991B C93C
uid           [ 未知 ] Ubuntu Archive Automatic Signing Key (2018) <ftpmaster@ubuntu.com>

nihao@nihao-V:~/work/emdebian/mindb$ ls
multistrap_mindb.conf  target-rootfs
nihao@nihao-V:~/work/emdebian/mindb$ cd target-rootfs/
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs$ ls
dev  etc  lib64  usr  var
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs$ cd etc/
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs/etc$ ls
apt  network  shells
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs/etc$ cd apt/
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs/etc/apt$ ls
apt.conf.d  preferences.d  sources.list.d  trusted.gpg.d
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs/etc/apt$ sudo cp /etc/apt/trusted.gpg .
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs/etc/apt$ ls
apt.conf.d  preferences.d  sources.list.d  trusted.gpg  trusted.gpg.d
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs/etc/apt$ cd ..
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs/etc$ slcd
Command 'slcd' not found, did you mean:
  command 'slc' from deb heimdal-multidev (7.7.0+dfsg-2)
  command 'nslcd' from deb nslcd (0.9.11-1)
Try: sudo apt install <deb name>
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs/etc$ cd ..
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs$ ls
dev  etc  lib64  usr  var
nihao@nihao-V:~/work/emdebian/mindb/target-rootfs$ cd ..
nihao@nihao-V:~/work/emdebian/mindb$ ls
multistrap_mindb.conf  target-rootfs
nihao@nihao-V:~/work/emdebian/mindb$ sudo multistrap -a armhf -f multistrap_mindb.conf
multistrap 2.2.11 using multistrap_mindb.conf
multistrap 2.2.11 using multistrap_mindb.conf
Using foreign architecture: armhf
multistrap building armhf multistrap on 'amd64'
Getting package lists: APT_CONFIG=/tmp/multistrap.nRaGAw apt-get  -o Apt::Architecture=armhf -o Dir::Etc::TrustedParts=/home/nihao/work/emdebian/mindb/target-rootfs/etc/apt/trusted.gpg.d -o Dir::Etc::Trusted=/home/nihao/work/emdebian/mindb/target-rootfs/etc/apt/trusted.gpg -o Apt::Get::AllowUnauthenticated=true -o Apt::Get::Download-





~~~

密钥位置/home/nihao/.gnupg



# [由于没有公钥，无法验证下列签名](https://www.cnblogs.com/jack123/p/12956016.html)

报错：

W: GPG 错误：http://ftp.de.debian.org/debian sid InRelease: 由于没有公钥，无法验证下列签名： NO_PUBKEY 04EE7237B7D453EC NO_PUBKEY 648ACFD622F3D138
W: 仓库 “http://ftp.de.debian.org/debian sid InRelease” 没有数字签名。
N: 无法认证来自该源的数据，所以使用它会带来潜在风险。
N: 参见 apt-secure(8) 手册以了解仓库创建和用户配置方面的细节。

下载：

apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 648ACFD622F3D138

报错：

gpg: 下载密钥‘22F3D138’，从 hkp 服务器 keyserver.ubuntu.com
gpg: 密钥 3CBBABEE：公钥“Debian Archive Automatic Signing Key (10/buster) <ftpmaster@debian.org>”已导入
gpg: 合计被处理的数量：1
gpg: 已导入：1 (RSA: 1)
gpg: 找不到可写的钥匙环：eof
gpg: 读取‘[stdin]’时出错：常规错误
gpg: import from `[stdin]' failed: 常规错误
gpg: 合计被处理的数量：0

 

使用：

sudo apt-get update -o Acquire::diffs=false
sudo apt-get dist-upgrade
sudo apt-get update

遇到选择点回车。

终端输入代码：

gpg --keyserver subkeys.pgp.net --recv-keys 04EE7237B7D453EC

这个代码是下载密钥

完成后要导入密钥，输入以下代码：

gpg --armor --export 04EE7237B7D453EC | apt-key add -

上面如果不行的话，换个清华大学的镜像试试。





W: GPG error: http://mirrors.163.com jessie-updates InRelease: The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 8B48AD6246925553 NO_PUBKEY 7638D0442B90D010
这个提示看起来是缺少公钥。
\1. 去官网的服务器获取相应的公钥。

```
gpg --keyserver keyserver.ubuntu.com --recv-keys 8B48AD6246925553
```

\2. 将获得的公钥添加到系统密钥列表。

```
gpg -a --export 8B48AD6246925553 | sudo apt-key add -
```

\3. 几个可以获得公钥的地址：
（不同网络的限制不一样，大不了一个一个尝试）

```
keyring.debian.org
keyserver.ubuntu.com
pgp.mit.edu
subkeys.pgp.net
www.gpg-keyserver.de
```