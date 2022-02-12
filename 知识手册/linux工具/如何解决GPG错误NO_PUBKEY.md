 

https://ubuntuqa.com/article/111.html

问题描述

我使用Software Sources程序添加了一些额外的存储库。但是当我重新加载包数据库时，出现如下错误：

``` bash
W: GPG error: http://ppa.launchpad.net trusty InRelease: The following signatures couldn’t be verified because the public key is not available: NO_PUBKEY 8BAF9A6F
```

根据官方的Ubuntu文档，我知道我可以在终端中使用apt-key修复它。但我希望以图形方式做。有没有办法做到这一点，而不使用终端？

最佳解决方案

到目前为止，最简单的方法是使用Y-PPA-Manager(现在将launchpad-getkeys脚本与图形界面集成在一起)。

``` bash
要安装它，首先添加该程序的webupd8存储库：

sudo add-apt-repository ppa:webupd8team/y-ppa-manager

更新您的软件列表并安装Y-PPA-Manager：

sudo apt-get update
sudo apt-get install y-ppa-manager

运行y-ppa-manager(即键入y-ppa-manager，然后按确认键)。

当出现主y-ppa-manager窗口时，点击”Advanced.”

从高级任务列表中，选择“尝试导入所有缺少的GPG密钥”，然后单击确定。你完成了！正如警告对话框所示，当您开始操作时，根据您拥有的PPA数量和连接速度，可能需要一段时间(大约2分钟)。
```

次佳解决方案

在终端中执行以下命令

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys <PUBKEY>

其中<PUBKEY>是您缺少存储库的公钥，例如8BAF9A6F。

然后更新

sudo apt-get update

备用方法：

sudo gpg --keyserver pgpkeys.mit.edu --recv-key <PUBKEY>
sudo gpg -a --export <PUBKEY> | sudo apt-key add -
sudo apt-get update

请注意，当你输入一个密钥这样使用apt-key你是在告诉你信任你要导入签署您的系统将使用软件密钥的系统。除非您确定密钥真的是包销售商的关键，否则不要这样做。

第三种解决方案

当你没有合适的存储库公钥时会发生这种情况。

要解决这个问题，请使用以下命令：

gpg --keyserver hkp://keyserver.ubuntu.com:80 --recv 9BDB3D89CE49EC21

它从Ubuntu密钥服务器中检索密钥。然后这个：

gpg --export --armor 9BDB3D89CE49EC21 | sudo apt-key add -

它将密钥添加到apt受信任的密钥。

该解决方案可以找到here& here& here。

第四种方案

您需要获取并导入密钥。

要从PPA获取密钥，请访问PPA的Launchpad页面。在Launchpad的每个PPA页面上，点击’关于这个PPA的技术细节'(1)后，你会发现这个链接(2)：

apt,gnupg,ubuntu

按照它并点击密钥ID链接(3)：

apt,gnupg,ubuntu

保存页面，这是你的关键文件。

现在是时候导入它了：

``` bash
Applications > Software Center，

Edit > Software sources...，

输入你的密码，

转到Authentication选项卡并单击Import Key File...，最后

选择保存的密钥文件，然后单击OK。
```

而已。

第五种方案

apt只能处理/etc/apt/trusted.gpg.d中的40个键。 41个密钥，即使您经历了所有添加缺少的密钥的步骤，您也会得到GPG错误“找不到公用密钥”。

请检查您不再使用的ppa(s)文件中是否有未使用的密钥。如果全部正在使用中，请考虑删除一些ppa以及/etc/apt/trusted.gpg.d中的相应密钥文件

此外，使用

sudo apt-key adv

被认为是一种安全风险，不被推荐，因为你“破坏了整个安全概念，因为这不是一个安全的接收密钥的方式，因为各种原因(例如：hkp是一个明文协议，可以伪造短的甚至长的keyid，… )”。 http://ubuntuforums.org/showthread.php?t=2195579

我相信正确的方法来添加丢失的键(例如1ABC2D34EF56GH78)是

gpg --keyserver hkp://keyserver.ubuntu.com:80 --recv 1ABC2D34EF56GH78
gpg --export --armor 1ABC2D34EF56GH78 | sudo apt-key add -

第六种方案

WebUpd8 PPA中包含一个小脚本，我将链接为一个.deb下载文件，因此您不必添加整个PPA，它将自动导入所有缺少的GPG密钥。

下载并安装Launchpad-getkeys(忽略它的版本，它适用于从Karmic到Oneiric的所有Ubuntu版本)。安装完成后，打开终端并输入：

sudo launchpad-getkeys

如果您使用代理服务器，则情况会稍微复杂一些，因此请参阅this获取更多信息

第七种方案

安装Heroku时遇到同样的问题。下面的链接解决了我的问题 –

http://naveenubuntu.blogspot.in/2011/08/fixing-gpg-keys-in-ubuntu.html

修复NO_PUBKEY问题后，仍然存在以下问题

W: GPG error: x[http://toolbelt.heroku.com](http://toolbelt.heroku.com/) ./ Release: The following signatures were invalid: BADSIG C927EBE00F1B0520 Heroku Release Engineering <release@heroku.com>

为了解决它，我在终端中执行了以下命令：

sudo -i
apt-get clean
cd /var/lib/apt
mv lists lists.old
mkdir -p lists/partial
apt-get clean
apt-get update

来源 – Link to solve it

第八种方案

确保您安装了apt-transport-https：

dpkg -s apt-transport-https > /dev/null || bash -c "sudo apt-get update;
sudo apt-get install apt-transport-https -y"

添加存储库：

curl https://repo.skype.com/data/SKYPE-GPG-KEY | sudo apt-key add -
echo "deb [arch=amd64] https://repo.skype.com/deb stable main" | sudo tee /etc/apt/sources.list.d/skype-stable.list

安装Skype for Linux：

sudo apt-get update
sudo apt-get install skypeforlinux -y

来源：https://community.skype.com/t5/Linux/Skype-for-Linux-Beta-signatures-couldn-t-be-verified-because-the/td-p/4645756

第九种方案

更一般地说，以下方法应该适用于每个存储库。首先，在搜索引擎的最终帮助下，搜索程序提供商网站上的文本，如下所示：

-----BEGIN PGP PUBLIC KEY BLOCK-----
Version: GnuPG v1.4.1 (GNU/Linux)
[...]
-----END PGP PUBLIC KEY BLOCK-----

这样的文本例如显示在[http://deb.opera.com上](http://deb.opera.xn--com-x28d/)。复制该段落，将其粘贴到您在桌面上创建的空白文件中。这导致密钥文件。

然后继续输入密钥：

```
应用程序>软件中心

编辑>软件来源…，输入密码

身份验证标签，点击’导入密钥文件…’

选择保存的密钥文件并点击’Ok’。
```

您现在可以删除以前创建的密钥文件。

第十种方案

好！我最终找到了路！

我测试了所有方法来修复GPG错误NO_PUBKEY，并且没有任何工作适合我。

我删除了/etc/apt/trusted.gpg.d文件夹的全部内容

cd /etc/apt/trusted.gpg.d
sudo rm -R *
sudo apt-get update

我使用Y-PPA-Manager方法，因为我懒得手动创建所有的pubkey(太多)：http://www.unixmen.com/fix-w-gpg-error-no_pubkey-ubuntu/

再次运行sudo apt-get更新，最终所有工作都很好！坦克！

基于资料来源：https://bugs.launchpad.net/ubuntu/+source/apt/+bug/1263540 上的＃17后
参考资料

``` base
How do I fix the GPG error “NO_PUBKEY”?
```

本文由Ubuntu问答整理, 博文地址: https://ubuntuqa.com/article/111.html
