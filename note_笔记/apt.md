 

[三种方式给apt设置代理](https://www.cnblogs.com/andrewwang/p/9293031.html)

![img](https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1531283026187&di=b2ff62c0963ab15655143b331fd749b7&imgtype=0&src=http%3A%2F%2Fwww.51xdn.net%2Fuploads%2Fallimg%2F160821%2F140210I29_0.jpg)
为什么设置代理，你懂得。

有很多第三方工具可以用，比如proxychains，非常好用，不过今天这不是正题。因为有可能没有代理，上网你都做不到，更别提下载软件了。想一想方法还是告诉你，免得你万一必须用：`sudo apt install proxychains`

通常有三种办法为apt设置代理，有了代理能上网之后，apt才能为你带来其它软件的帮助。
1.设置环境变量，如果想为所有用户设置，可以编辑/etc/profile或者/etc/environment；如果只是当前用户使用，可以设置/.profile文件，如果使用的不是bash，可能要根据需要设置其它配置文件，具体看使用shell的帮助文件，比如fish是使用：/.config/fish/config.fish。另外不同的脚本设置环境变量的命令是不同的，注意区别使用。
以bash为例，把下面这行脚本写入配置文件。记着把代理的用户名、密码替换成自己的，如果没有密码也可以把@之前的部分包括@都去掉：

```bash
export HTTPS_PROXY=http://username:password@192.168.1.1:8080
```

如果使用的是socks5代理，可以写成这样：

```bash
export HTTPS_PROXY="socks5://192.168.1.1:8088"
#或者
export ALL_PROXY="socks5://192.168.1.1:8088"
```

2.如果不想一直使用代理，只是各别命令行程序有代理需求，可以使用临时的环境变量。方法是，建立一个脚本文件，比如叫proxy.sh，内容为：

```bash
#!/bin/sh
HTTPS_PROXY=http://username:password@192.168.1.1:8080 $*
```

这样，以后需要使用代理执行某个命令的时候，用如下的形式：

```bash
proxy.sh curl https://www.google.com
```

这种方法对于某些子进程没有继承父进程环境变量的情况，会有兼容问题。不如proxychains好用。不过对于apt的测试是可以的。

3.针对apt进行设置，当然就只对apt有效了。其它常用软件git之类的，都有自己的方法，请自行搜索相关设置。设置apt代理的方法是：

```bash
sudo su
vi /etc/apt/apt.conf.d/10proxy   #这个文件正常不存在，会新建一个
#编辑内容为：
Acquire::http::Proxy "http://user:pwd@192.168.1.1:8080";
```

随后执行apt的任何操作，都将使用这个代理了。

参考文献：
https://stackoverflow.com/questions/25322280/how-can-i-configure-a-http-proxy-for-apt-get
