Apt-clone：备份已安装的软件包并在新的Ubuntu系统上恢复它们

http://www.lanmon.net/Shownews.asp?ID=4452

当我们在基于Ubuntu/Debian的系统上使用apt-clone时，包安装变得更加容易。如果您需要在少数系统上安装相同的软件包，apt-clone将适合您。
如果要在每个系统上手动构建和安装必要的软件包，这是一个耗时的过程。它可以通过多种方式实现，Linux中有许多程序可用。我们过去曾写过一篇关于Aptik的文章。它是允许Ubuntu用户备份和恢复系统设置和数据的程序之一。

什么是apt-clone？
Apt-clone允许您为Debian/Ubuntu系统创建所有已安装软件包的备份，这些软件包可以在新安装的系统（或容器）或目录中恢复。
可以在相同操作系统版本和体系结构的多个系统上还原此备份。

如何安装apt-clone？
apt-clone包可以在官方的Ubuntu/Debian存储库中找到，所以使用apt包管理器或apt-get包管理器安装它。
使用apt包管理器安装apt-clone。

~~~ bash
$ sudo apt install apt-clone
~~~ 

使用apt-get包管理器安装apt-clone。

~~~ bash
$ sudo apt-get install apt-clone
~~~

如何使用apt-clone备份已安装的软件包？
成功安装apt-clone后。只需提供一个保存备份文件的位置即可。我们将已安装的包备份保存在/backup目录中。
Apt-clone将已安装的软件包列表保存到apt-clone-state-Ubuntu18.2daygeek.com.tar.gz。

~~~
$ sudo apt-clone clone ./backup
~~~

我们也可以通过运行ls命令来检查。

~~~
$ ls -lh/backup /总计32K
-rw-r  -  r-- 1 root root 29K Apr 20 19: 06 apt-clone-state-Ubuntu18.2daygeek.com.tar.gz
~~~

执行以下命令，查看备份文件的详细信息。

~~~
$ apt-clone info /backup/apt-clone-state-Ubuntu18.2daygeek.com.tar.gz
主机名: Ubuntu18.2daygeek.com
Arch: amd64
Distro:仿生
元: libunity-scopes-json-def-desktop，ubuntu-desktop
已安装: 1792 pkgs（194自动）
日期:星期六，四月20 19: 06: 43 2019
~~~

根据上面的输出，备份文件中总共有1792个包。

如何恢复使用apt-clone备份的软件包？
您可以使用任何远程复制程序来复制远程服务器上的文件。
~~~
$ scp /backup/apt-clone-state-ubunt-18-04.tar.gz目标服务器:/opt
~~~

复制完成后，使用apt-clone执行还原。
使用以下命令还原。
~~~
$ sudo apt-clone restore /opt/apt-clone-state-Ubuntu18.2daygeek.com.tar.gz
~~~

请注意，还原将覆盖现有的/etc/apt/sources.list和安装/删除包。所以要小心。
如果要将所有程序包还原到文件夹而不是实际还原，可以使用以下命令。.
~~~
$ sudo apt-clone restore /opt/apt-clone-state-Ubuntu18.2daygeek.com.tar.gz --desting
~~~