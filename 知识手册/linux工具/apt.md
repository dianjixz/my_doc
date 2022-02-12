apt-get 命令适用于 deb 包管理式的 Linux 操作系统（Debian、Ubuntu等），主要用于自动从互联网软件仓库中搜索、下载、安装、升级、卸载软件或操作系统。

Ubuntu 软件包查找: [https://packages.ubuntu.com/](https://links.jianshu.com/go?to=https%3A%2F%2Fpackages.ubuntu.com%2F)

### 1. 安装 软件包

~~~ bash
apt-get install PackageName                  # 普通安装
apt-get install PackageName=VersionName     # 安装指定包的指定版本
apt-get --reinstall install PackageName     # 重新安装

apt-get build-dep PackageName               # 安装源码包所需要的编译环境
apt-get -f install                          # 修复依赖关系

apt-get source PackageName                  # 下载软件包的源码
~~~

### 2. 卸载 软件包

~~~ bash
apt-get remove PackageName              // 删除软件包, 保留配置文件

apt-get --purge remove PackageName      // 删除软件包, 同时删除配置文件
 apt-get purge PackageName               // 删除软件包, 同时删除配置文件

apt-get autoremove PackageName          // 删除软件包, 同时删除为满足依赖
 // 而自动安装且不再使用的软件包

apt-get --purge autoremove PackageName  // 删除软件包, 删除配置文件,
 // 删除不再使用的依赖包

apt-get clean && apt-get autoclean      // 清除 已下载的软件包 和 旧软件包
~~~

### 3. 更新 软件包

~~~ bash
apt-get update                               // 更新安装源（Source）
 apt-get upgrade                             // 更新已安装的软件包
 apt-get dist-upgrade                      // 更新已安装的软件包（识别并处理依赖关系的改变）
~~~

### 4. 查询 软件包

~~~ bash
dpkg -l                                                  // 列出已安装的所有软件包

apt-cache search PackageName         // 搜索软件包
 apt-cache show PackageName            // 获取软件包的相关信息, 如说明、大小、版本等

apt-cache depends PackageName       // 查看该软件包需要哪些依赖包
 apt-cache rdepends PackageName      // 查看该软件包被哪些包依赖
 apt-get check                                         // 检查是否有损坏的依赖

~~~

### 5. apt-get --help

执行命令: apt-get --help，输出:

apt 1.0.1ubuntu2，用于 amd64 构架，编译于 Dec  8 2016 16:23:38
 用法： apt-get [选项] 命令
 　　 apt-get [选项] install|remove 软件包1 [软件包2 ...]
 　　 apt-get [选项] source 软件包1 [软件包2 ...]

apt-get 是一个用于下载和安装软件包的简易命令行界面。
 最常用命令是 update 和 install。

### 命令：

~~~ bash
update - 取回更新的软件包列表信息
 upgrade - 进行一次升级
 install - 安装新的软件包(注：软件包名称是 libc6 而非 libc6.deb)
 remove - 卸载软件包
 autoremove - 卸载所有自动安装且不再使用的软件包
 purge - 卸载并清除软件包的配置
 source - 下载源码包文件
 build-dep - 为源码包配置所需的编译依赖关系
 dist-upgrade - 发布版升级，见 apt-get(8)
 dselect-upgrade - 根据 dselect 的选择来进行升级
 clean - 删除所有已下载的包文件
 autoclean - 删除已下载的旧包文件
 check - 核对以确认系统的依赖关系的完整性
 changelog - 下载指定软件包，并显示其changelog
 download - 下载指定的二进制包到当前目录
~~~

### 选项：

~~~ bash
-h  本帮助文档。
 -q  让输出可作为日志 - 不显示进度
 -qq 除了错误外，什么都不输出
 -d  仅仅下载 - 【不】安装或解开包文件
 -s  不作实际操作。只是依次模拟执行命令
 -y  对所有询问都回答是(Yes)，同时不作任何提示
 -f  当出现破损的依赖关系时，程序将尝试修正系统
 -m  当有包文件无法找到时，程序仍尝试继续执行
 -u  显示已升级的软件包列表
 -b  在下载完源码包后，编译生成相应的软件包
 -V  显示详尽的版本号
 -c=? 读取指定配置文件
 -o=? 设置任意指定的配置选项，例如 -o dir::cache=/tmp
 请查阅 apt-get(8)、sources.list(5) 和 apt.conf(5)的参考手册
 以获取更多信息和选项。
~~~



# apt问题指南

## 添加源
方式一:
找到各大软件源,按官网指南添加

方式二:
手动编辑 /etc/apt/source.conf 设置源

添加源后使用 `sudo apt update` 进行更新!

## 由于没有公钥，无法验证下列签名

相关报错:

~~~ bash
W: GPG 错误：http://ftp.de.debian.org/debian sid InRelease: 由于没有公钥，无法验证下列签名： NO_PUBKEY 04EE7237B7D453EC NO_PUBKEY 648ACFD622F3D138
W: 仓库 “http://ftp.de.debian.org/debian sid InRelease” 没有数字签名。
N: 无法认证来自该源的数据，所以使用它会带来潜在风险。
N: 参见 apt-secure(8) 手册以了解仓库创建和用户配置方面的细节。
~~~

### 方法一:

~~~ bash
apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 648ACFD622F3D138

~~~

### 方法二:
~~~ bash
gpg --keyserver subkeys.pgp.net --recv-keys 04EE7237B7D453EC
gpg --armor --export 04EE7237B7D453EC | apt-key add -
~~~

几个可以获得公钥的地址：
（不同网络的限制不一样，大不了一个一个尝试）
~~~ bash
keyring.debian.org
keyserver.ubuntu.com
pgp.mit.edu
subkeys.pgp.net
www.gpg-keyserver.de
~~~
 当 apt 密钥无法验证,又没有其他办法时,可以使用下面的操作,注意忽略签名验证会带来安全问题,你要清楚你在做什么!
~~~ bash
sudo apt -o Acquire::AllowInsecureRepositories=true -o Acquire::AllowDowngradeToInsecureRepositories=true update
# 更新仓库时忽略签名验证

sudo apt -o APT::Get::AllowUnauthenticated=true install repo-keyring-pkgname
# 忽略软件包的签名验证
~~~

注意:

apt只能处理/etc/apt/trusted.gpg.d中的40个键。 如果是41个密钥，即使您经历了所有添加缺少的密钥的步骤，您也会得到GPG错误“找不到公用密钥”。

请检查您不再使用的ppa(s)文件中是否有未使用的密钥。如果全部正在使用中，请考虑删除一些ppa以及/etc/apt/trusted.gpg.d中的相应密钥文件

### 方案三:

WebUpd8 PPA中包含一个小脚本，我将链接为一个.deb下载文件，因此您不必添加整个PPA，它将自动导入所有缺少的GPG密钥。

下载并安装Launchpad-getkeys(忽略它的版本，它适用于从Karmic到Oneiric的所有Ubuntu版本)。安装完成后，打开终端并输入：

sudo launchpad-getkeys

如果您使用代理服务器，则情况会稍微复杂一些，因此请参阅this获取更多信息

## 设置代理
### 方式一
使用 proxychains 软件代理.

### 方式二
使用全局代理:
~~~ bash
export HTTPS_PROXY=http://username:password@192.168.1.1:8080
export HTTPS_PROXY="socks5://192.168.1.1:8088"
#或者
# export ALL_PROXY="socks5://192.168.1.1:8088"
~~~

### 方式三
针对 apt 的代理
~~~ bash
sudo su
vi /etc/apt/apt.conf.d/10proxy   #这个文件正常不存在，会新建一个
#编辑内容为：
Acquire::http::Proxy "http://user:pwd@192.168.1.1:8080";
~~~
随后执行apt的任何操作，都将使用这个代理了。
参考:

https://ubuntuqa.com/article/111.html

https://www.cnblogs.com/andrewwang/p/9293031.html
https://stackoverflow.com/questions/25322280/how-can-i-configure-a-http-proxy-for-apt-get

# apt 相关内容
multistrap 可用于建立基本的 debian 系文件系统.
debootstrap 可用于建立基本的 debian 系文件系统.
