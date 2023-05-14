 
openwrt 编译环境搭建

binutils bzip2 diff find flex gawk gcc-6+ getopt grep install libc-dev libz-dev make4.1+ perl python3.6+ rsync subversion unzip which
gengetopt
``` bash
git clone https://github.com/openwrt/openwrt.git
sudo apt install rsync subversion unzip -y
./scripts/feeds update -a
./scripts/feeds install -a
```

一、软件包目录及结构（package目录）
概念：OpenWrt 有一个非常好的构建系统，这样我们就可以非常方便地管理数千个软件包和几十个硬件平台。我们也可以非常方便地移植已有的软件到OpenWrt系统中。在前面我们介绍的OpenWrt源码中，你会发现各个软件包目录下一般会有两个文件夹和一个Makefile 文件。
目录结构
我们以dnsmasq软件为例，此软件的目录下会有如下几个文件和目录。

![](https://img-blog.csdnimg.cn/20191014171411975.png)

补丁（patches）目录：是可选的，典型包含缺陷修改或者用于优化可执行程序大小的补丁文件。
files目录：也是可选的，它一般用于保存默认配置文件和初始化启动脚本。
src目录：如果为OpenWrt本身项目所包含的软件模块，因为代码将完全受到自己控制，这时将不会patches 目录存在，而是会有一个 src 目录，代码直接放在src目录下（在下一篇文章中我们自己定义的软件包就有这个目录）。
Makefile：提供下载、编译、安装以及生成 OPKG 安装包的功能，这个文件是必须有的。
二、软件包的Makefile
Makefile提供下载、编译、安装以及生成 OPKG 安装包的功能，这个文件是必须有的。
OpenWrt下的Makefile：从Makefile的内容可以看出，此Makefile与普通的Makefile不同，OpenWrt没有遵守传统的Makefile格式风格，而是将Makefile写成面向对 象格式，这样就简化了多平台移植过程。
Makefile的结构
下面我们以dnsmasq软件的Makefile为例，此Makefile可以编译3个软件包（分别为 dnsmasq、dnsmasq-dhcpv6 和dnsmasq-full）。
①首先是使用“include”指示符来包含顶层目录的rules.mk文件。

![](https://img-blog.csdnimg.cn/20191014171946739.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

②接着是变量定义，它定义了软件包的基本信息，如名称、版本、下载地址、许可协议和编译目录等信息。

![](https://img-blog.csdnimg.cn/20191014172015428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

③在“PKG_*”变量定义完成之后再包含package.mk文件。

![](https://img-blog.csdnimg.cn/20191014172045465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)


④中间部分是软件包的宏定义和一 些编译选项定义（篇幅过大，此处只截取了部分）。

![](https://img-blog.csdnimg.cn/20191014172303103.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20191014172315250.png)

⑤最后是调用BuildPackage。“BuildPackage”是在包含头文件“include/package.mk”中定义的。BuildPackage仅仅需要一个直接参数——要编译的软件包名称。在这个例子中传递了 3 个软件包名称作为参数，分别为 dnsmasq、dnsmasq-dhcpv6 和dnsmasq-full。

![](https://img-blog.csdnimg.cn/20191014172229586.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

rules.mk、package.mk
下面我们还是以dnsmasq软件的Makefile为例。
软件包的Makefile没有太多其他逻辑依赖的工作需要去做，所有的一切都是隐藏在被包含的Makefile（include/package.mk 及 rules.mk）中的。
Makefile文件非常抽象，你只需要按照通用的模板定义变量即可。
rules.mk文件是全局的编译变量定义，在每一个软件包的Makefile文件的第一行均首先包含这个文件。
rules.mk文件中经常使用的变量定义有以下几个：
INCLUDE_DIR：源代码目录下的 include 目录。
BUILD_DIR：代码编译的根目录，通常为“build_dir/target-*”目录。
TARGET_CFLAGS：指定目标平台的 C 语言编译选项。
TARGET_LDFLAGS：指定目标平台的编译链接选项。
INSTALL_DIR：创建目录，并设置目录权限。
INSTALL_DATA：安装数据文件，即复制并设置权限为 0644。
INSTALL_CONF：安装配置文件，即复制并设置权限为 0600。
INSTALL_BIN：安装可执行程序，即复制并增加执行权限，设置权限表示为 0777。
OpenWrt——Makefile变量
OpenWrt 预定义了很多变量，这些变量减少了使用者的开发代价，但需要使用者按照语义进行使用。
变 量	含 义	示 例
PKG_NAME	软件包的名称，可以通过 menuconfig 和 ipkg 查看到	dnsmasq
PKG_VERSION	上游软件的版本号，为 2.73	2.73
PKG_RELEASE	Makefile 的版本号	1
PKG_SOURCE	原始的源代码文件名	 
PKG_SOURCE_URL	用于下载源码的地址（目录）	http://thekelleys.org.uk/dnsmasq
PKG_MD5SUM	软件包的 MD5 值，用于验证下载的文件 是否正确	b8bfe96d22945c8cf4466826ba 9b21bd
PKG_LICENSE	这个软件的许可协议，开源软件的许可证 以 GPL 家族最多	GPL-2.0
PKG_LICENSE_FILES	许可协议文件，是指代码目录下的文件名， 一般均为 COPYING	COPYING
PKG_BUILD_DIR	软件包的编译目录（就是编译过程中的临时目录build_dir）	 
PKG_INSTALL	设置为 1 将调用软件包自己的“make install”，安装目录前缀为 PKG_INSTALL_ DIR	1
PKG_BUILD_PARALLEL	是否可以并行编译	1
PKG_CONFIG_DEPENDS	编译依赖，指定哪些选项依赖本软件包	 
PKG_INSTALL_DIR	当调用原始软件包“make install”时的安 装目录	 
PKG_SOURCE_PROTO	用于下载的传输协议（git、svn），如果为 压缩包则不用指定	 
PKG_SOURCE_SUBDIR	下载目录，如果下载传输协议为“svn”或 “git”时必须指定。例如："PKG_SOURCE_ SUBDIR:=$(PKG_NAME)-$(PKG_VERSI ON)"	 
PKG_SOURCE_VERSION	下载协议为“git”时必须指定，指定的提 交哈希点将会被检出	 
PKG_MAINTAINER	维护者的姓名和邮件地址	 
PKG_BUILD_DEPENDS	软件包编译依赖，即在这个包编译之前编 译，但是在运行时不需要，和 DEPENDS 有相同的语法	 
三、软件包Makefile中的宏定义
软件包定义用于编译前的软件包选择和编译后的IPKG安装包生成。这些设置的参数传递给 buildroot进行交叉编译，buildroot 是交叉编译环境的统称。这些是在menuconfig和生成的IPKG安装包实体中显示的。
Package宏定义
一些宏定义以“Package/”开头，Package开头的定义用于“make menuconfig”选择及编译生成软件包。
但你可以将一个源代码包分割为多个安装包。 你可以增加许多软件安装包“Package/”定义来多次调用BuildPackage，这样就可以从单个 源代码编译出来多个软件安装包。
例如dnsmasq软件的Makefile中就定义了3种软件安装包，即dnsmasq、 dnsmasq-dhcpv6 和 dnsmasq-full。

![](https://img-blog.csdnimg.cn/20191014190244887.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

Package/宏定义下的变量赋值
下面是在软件包“Package/”宏定义下可以赋值的变量。
例如是下图是dnsmasq软件的Makefile中，其中定义了3种软件安装包（dnsmasq、 dnsmasq-dhcpv6 和 dnsmasq-full），每个软件包下有一些变量的定义。
变量	含义
SECTION	软件包的类型，如 network、Sound、Utilities 或 Multimedia等
CATEGORY	在 menuconfig 中显示到菜单分类中
TITLE	标题，是软件包的简短描述
URL	软件包的原始网站地址，可以在这里找到该软件
MAINTAINER	维护者的姓名和邮件地址。一般为这个软件包作者的邮件地址
DEPENDS	（可选）依赖项，需要在本软件包之前编译和安装的软件包

![](https://img-blog.csdnimg.cn/20191014192123809.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

Package/宏定义选项
软件包 Package 选项。
安装包选项	是否必需	含 义
Package/<软件包名称>	是	定义软件包的描述信息，例如网站地址和menuconfig 中的菜单分类等
Package/<软件包名>/Default	可选	软件包的默认选项
Package/<软件包名>/description	是	软件包的详细描述
Package/<软件包名>/install	是	复制文件到 ipkg 目录中，使用$(1)代表 ipkg 的目录，在源代码中使 用相对目录。编译生成的安装文件由$(PKG_INSTALL_ DIR)目录下 复制到 ipkg 的目录下
Package/<软件包名>/config	可选	根据软件包的选择对编译选项进行定义
Package/<软件包名>/conffiles	可选	定义本软件包的运行配置文件列表，一行一个文件
Package/<软件包名>/preinst	可选	这是在安装之前实际执行的脚本，不要忘了包含#!/bin/sh。如果 你需要中止安装就返回 false
Package/<软件包名>/postinst	可选	在安装完成后执行的脚本，例如启动程序。不要忘了包含 #!/bin/sh
Package/<软件包名>/prerm	可选	在删除之前执行的脚本，例如停止程序的执行。不要忘了包含 #!/bin/sh。如果需要中止删除就返回 false
Package/<软件包名>/postrm	可选	在删除之后执行的脚本，不要忘了包含#!/bin/sh。如果需要中止 删除就返回 false
 例如下面是dnsmasq软件包Makefile中的Package/选项。

![](https://img-blog.csdnimg.cn/20191014192243978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

Build宏定义
一些宏定义为“Build/”开头，这些用于代码编译。
通常编译过程都是一样的，只是中间的编译参数有所不同，因此只有一个全局“Build”定义。
例如下面是iproute2软件的Makefile中的Build宏定义。

![](https://img-blog.csdnimg.cn/20191014194126994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

四、构建
软件包模块的编译步骤，通常包含下面4步：
准备（Prepare）。
配置（Configure）。
编译 （Compile）。
安装（Install）。
上面这部分在构建时都是可选的，因为OpenWrt已经制作了通用的模板，适合大多数模块来编译使用。
上面我们介绍的dnsmasq软件就是采用默认的步骤，因此在Makefile中没有看到Build宏定义。但指定了编译选项，例如设置 MAKE_FLAGS 变量指定编译选项，CONFIGURE_ARGS 变量用 于指定配置选项。这些变量都在“package-defaults.mk”文件中定义，我们可以在软件包的 Makefile中修改它。
软件包构建步骤
Build 步骤	是否必需	含 义
Build/Prepare	可选	一组用于解包及打补丁的命令，也可以不使用
Build/Configure	可选	如果源代码不需要configure来生成Makefile或者是通用的configure脚本， 就不需要这部分。否则就需要你自己的命令脚本或者使用"$(call Build/Configure/Default, FOO=bar)"增加额外的参数传递给 configure 脚本
Build/Compile	可选	编译源代码，在大多数情况下应该不用定义而使用默认值。如果你想传递给 make 特定的参数，可以使用“$(call Build/Compile/Default, FOO=bar)”
Build/Install	可选	安装编译后的文件，默认是调用 make install，如果需要传递指定的参 数，使用$(call Build/Install/Default,install install-foo)。注意你需要传递所 有的参数，要增加在“install”参数后面，不要忘了“install”参数
Build/InstallDev	可选	例如静态库和头文件等，但是不需要在目标设备上使用
例如下面是iproute2软件的Makefile中的Build宏定义。

![](https://img-blog.csdnimg.cn/20191014194126994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)


五、准备构建自己的系统软件
下面我们自己以一个自己设计的Hello程序为例，一步一步地构建出自己的软件。
程序实现的功能
我们实现一个在路由器启动后访问指定网站的功能，这样就可以统计路由器的启动次数。网站地址可以在配置文件中进行自定义配置，这个网址通过UCI编程接口读取配置文件来实现。访问指定网站功能通过命令行工具wget来实现。但如果其他人也使用wget来访问服务器，这样就不能区分是路由器行为还是其他应用软件的行为，因此我们修改了代理字符串来作为我们的自定义字符串（使用wget的--user-agent参数，参数如何使用见wget文章），这样就可以和默认的访问行为区分开来。
为了防止某些小区在断电并自动启动后，均立即访问服务器，对服务器产生瞬间流量冲击，因此路由器启动后产生一个随机延迟时间，然后再访问服务器。这个时间可以通过配置文件设置，假如设置为100秒，则访问服务器时间就为1～100秒的随机值。
六、软件目录结构
通常新增一 个模块的主要步骤如下：
在OPenWrt源码的package目录下增加一个目录（例如hello）。
在hello目录下添加src目录和files目录。
src目录存放模块源码和源码编译Makefile。
files目录存放模块的配置文件及启动脚本等。
在hello顶级目录增加Makefile。此Makefile中包含编译脚本和安装脚本。
我们软件的目录为名为hello，放置在OpenWrt源码的package目录下：
files目录包含配置文件和初始化脚本。
files目录下的hello.conf为配置文件，在安装后放在/etc/config/目录下的 hello 文件中。
files目录下hello.init用于控制进程启动的初始化脚本。
Makefile包含编译及安装指令，控制着代码在 OpenWrt 环境下的编译和生成安装包。
src目录保存C语言源代码和一个hello.c文件的Makefile。
src目录下的hello.c为上面的程序C语言代码。
src目录下Makefile为hello.c的Makefile。

![](https://img-blog.csdnimg.cn/20191014195950205.png)

七、src目录设计
这个目录下存放着程序的源代码hello.c与hello.c的Makefile文件。

![](https://img-blog.csdnimg.cn/20191018091828288.png)

![](https://img-blog.csdnimg.cn/20191018091758252.png)


Makefile文件
这个Makefile与hello目录下的Makefile不一样，顶级目录下的Makefile是用于全局编译时使用的，这个Makefile只是用来生成软件使用的。


hello.c代码设计
例如：某公司想了解已售出路由器的使用情况，如路由器的启动次数。我们在每次启 动时将访问指定服务器。Agent 为第一次启动时生成的随机数和指定的代理固定字符串组 合。这样简单修改示例代码的配置文件即可实现。大多数服务器已有记录访问日志功能， 只需统计服务器的访问日志即可实现路由器启动次数的统计。
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <uci.h>

//根据选项来访问服务器。
struct Hello
{
    char agent[50]; //代理字符串
    char url[256];  //访问的url，此处设置为我们在Ubuntu中使用python命令开启的一个HTTPS服务器网址
    int delay;      //启动后延迟多长时间访问
};

int getValue(struct uci_context *ctx, char *key, char*value, int n)
{
    char strKey[100];
    struct uci_ptr ptr;
    snprintf(strKey, sizeof(strKey), "hello.globe.%s",key); //将hello.globe.agent/url/delay 写入strKey中
    //uci_lookup_ptr：分割字符串并查找
	//参数1：UCI上下文环境对象 2：查找的结果保存到ptr中 3：待查找的字符串 4：是否允许扩展查找
	if (uci_lookup_ptr(ctx, &ptr, strKey, true) == UCI_OK) //从/etc/config/hello配置文件中找（备注：默认从/etc/config目录下读取）
    {
        printf("%s\n", ptr.o->v.string);
        strncpy(value, ptr.o->v.string, n-1); //将查找的字符串保存到value中
    }
    return 0;
}

int read_conf( struct Hello *hello)
{
    struct uci_context *ctx = uci_alloc_context(); //分配UCI上下文环境对象
    if (!ctx)
    {
        fprintf(stderr, "No memory\n");
        return 1;
    }
    getValue(ctx, "agent", hello->agent, sizeof(hello->agent)); //将配置文件中agent的值读取到hello->agent中
    getValue(ctx, "url", hello->url, sizeof(hello->url)); //将配置文件中url的值读取到hello->url中
    char delay[20];
    getValue(ctx, "delay", delay, sizeof(delay)); //将配置文件中delay的值读取到hello->delay中
    hello->delay = atoi(delay); //字符串转换为整型
    uci_free_context(ctx); //释放UCI上下文环境对象
    return 0;
}

struct Hello hello;
int main(int argc, char* argv[])
{
    /*
    char agent[50] = "openwrt";
    char url[100] = "http://192.168.43.20:8080";
    */
    
    //从/etc/config/hello配置文件中读取参数，并将读取到的内容设置到struct Hello中
    read_conf(&hello);
    
    //打印一下从配置文件中获取的数据
    printf("agent=%s\n", hello.agent);
    printf("url=%s\n", hello.url);
    printf("delay=%d\n", hello.delay);
    
    char cmd[512] = {0};
    snprintf(cmd, sizeof(cmd), "wget --user-agent=%s %s",hello.agent, hello.url); //将wget命令设置到cmd字符串中
    //system("wget --user-agent=openwrt http://192.168.43.20:8080");
    printf("cmd=%s\n", cmd);
     
    //srand(time(NULL));
    int delay_time = rand() % hello.delay; //延迟时间
    printf("delay_time=%d\n", delay_time);
    sleep(delay_time);
     
    system(cmd);  //执行wge命令
    return 0;
}
八、顶级Makefile设计
此Makefile为hello顶级目录下的Makefile，通过此Makefile来控制编译不是src目录下的。
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk


PKG_NAME:=hello
PKG_RELEASE:=1.0

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
PKG_CONFIG_DEPENDS :=

include $(INCLUDE_DIR)/package.mk


define Package/$(PKG_NAME)
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Hello utility
	DEPENDS:=+libuci
	URL:=https://blog.csdn.net/qq_41453285/article/details/102545618
	MAINTAINER:=1286550014@qq.com
endef

define Package/$(PKG_NAME)/description
	This is Route Hello World OpenWrt.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		CPPFLAGS="$(TARGET_CPPFLAGS)"\ 
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/hello.conf $(1)/etc/config/hello
	
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/hello.init $(1)/etc/init.d/hello
	
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hello $(1)/usr/sbin/hello
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
Makefile解析

![](https://img-blog.csdnimg.cn/20191014200852483.png)

首先包含rules.mk文件，接着将软件包名称定义为“hello”，并设置版本编号为“1.0”，这样编译出来的软件包将包含字符串“hello_1.0”。



在软件包定义中，我们设置软件包分类为“Network”。我们在make menuconfig时，可以在Network分类中找到它。

![](https://img-blog.csdnimg.cn/2019101809215913.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

我们设置依赖变量 DEPENDS 为“+libuci”，因为我们的C语言代码里面调用了UCI的接口函数，否则我们在编译时会遇到“libuci.so”找不到类似的错误信息。依赖是指哪些包必须在这个软件包之前编译和安装。注意，是使用+包名称，“+libuci”表示如果选择本软件包时，libuci软件包也会被自动选择。

![](https://img-blog.csdnimg.cn/20191017123214860.png)

“Build/Prepare”定义了如何准备编译本软件包，这里创建了编译目录，然后将代码复制到编译目录下，编译目录位于build_dir/target-i386_pentium4_musl-1.1.16/hello目录下。

![](https://img-blog.csdnimg.cn/20191018092212463.png)

下面图片是编译之后的显示结果（我把他提到前面来做个演示，演示一下Build/Prepare选项所做的操作）。

![](https://img-blog.csdnimg.cn/20191018092436537.png)

“Package/hello/install”定义了安装本软件包时会发生哪些操作，$(1)表示传入的第一个参数，在安装时，通常为根目录。共有3个文件需要安装，Makefile自动创建了3个目录（如果已存在就不创建），然后将3个文件分别复制到各自的目录下：
首先将配置文件“hello.conf”复制到配置目录“/etc/config”下，并重命名为hello。
接着将启动脚本“hello.init”复制到启动目录“/etc/init.d”下，并重命名为hello。
最后将编译生成的可执行程序hello复制到 “/usr/sbin”目录下。

![](https://img-blog.csdnimg.cn/20191018092219118.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

最后调用BuildPackage，将软件包的名称作为参数传递给BuildPackage。

![](https://img-blog.csdnimg.cn/20191026203312251.png)

九、配置文件设计（hello.conf）
配置文件分析
config globals 'globe'
	option agent 'openwrt'
	option url 'http://192.168.43.20:8080'
	option delay '10'
在hello.c源码中，程序会读取hello配置文件中globe配置节中的agent、url、delay选项的值，这些值用于我们程序的使用。

![](https://img-blog.csdnimg.cn/2019101712182041.png)

选项含义如下：
agent：wget命令执行时的代理字符串。
url：软件启动时，访问的URL。
delay：软件启动延迟机制所设置的时间。
配置文件的格式见文章（https://dongshao.blog.csdn.net/article/details/102527800）中的UCI的统一配置原理。
在Makefile中，hello.conf配置文件会被自动复制到/etc/config目录下，并重命名为hello。
十、软件启动脚本设计（hello.init）
关于软件启动机制与脚本设计，见下篇文章详细介绍：https://blog.csdn.net/qq_41453285/article/details/102545624。
在Makefile中，hello.init启动脚本会被自动复制到/etc/init.d目录下，并重命名为hello。
脚本分析
START、STOP变量：脚本的执行顺序通过START和STOP变量来定义，此处我们分别设置为15和85。
start()、stop()函数：决定软件如何启动和停止服务，在start()函数中我们调用了“hello”命令，因此在系统启动时我们的hello软件就会执行，从而达到开机自启动的效果。
custom()函数：最后是增加扩展命令 custom，仅仅输出扩展命令信息，并没有实际功能。
在这个例子中，这个命令在启动时取代start函数而执行boot函数，如果boot函数没有被重新定义，将执行rc.common中预定义的 boot函数，boot函数再次调用start函数。 如果你不带参数运行命令，将会自动调用help函数输出帮助信息。
#!/bin/sh /etc/rc.common

# hello script

START=15
STOP=85

start() {
    echo "start HelloRoute!"
    hello
}

stop() {
    echo "stop HelloRoute!"
    #hello -t
}

EXTRA_COMMANDS="custom"
EXTRA_HELP="        custom Help for the custom command"

custom() {
    echo "custom command"
}
备注
如果使用opkg命令安装软件时脚本一般均有执行权限，如果是自己手动新增脚本，不要忘记确认脚本是否有执行权限（通过运行chmod +x /etc/init.d/hello命令来增加执行权限）。
另外。我们的配置文件编辑时要采用Linux系统的格式编辑，否则在openwrt中运行这个脚本时会因为换行符的原因导致脚本无法正常执行。

![](https://img-blog.csdnimg.cn/20191018102517525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

十一、单独编译软件
OpenWrt支持编译单个软件包，这样可以非常方便地编译单个软件包来定位问题。
步骤
第一步：将我们的hello目录全部复制到Ubuntu的openwrt源码的package目录下。

![](https://img-blog.csdnimg.cn/20191017123306768.png)


第二步：回到在openwrt源码顶级目录下，输入make menuconfig进行配置，进入Network模块，然后选择hello（输入M将hello设置为M，M代表模块类型编译，“*”代表系统性编译）然后保存退出，保存之后会更新.config配置文件，如果不进行这一步，下面编译时会出错。

![](https://img-blog.csdnimg.cn/20191017125317434.png)

第三步：输入以下命令进行编译。
make package/hello/compile V=s

![](https://img-blog.csdnimg.cn/20191017125630716.png)

之后就可以看到生成安装文件为bin/packages/i386_pentium4/base目录下找到对应的.ipk安装包。

![](https://img-blog.csdnimg.cn/20191017175005368.png)

以后想要更改程序的功能时，删除.ipk再次编译即可。
十二、在平台编译中编译软件
如果我们要加入平台编译过程中，可以在make menuconfig时选择hello模块。
再在隐藏的配置文件“.config”中会增加一项“CONFIG_PACKAGE_hello=y”，这样就可以在编译整个系统时自动编译生成我们的软件模块。
如果遇到编译错误，使用make V=s即可看到详细的编译过程和错误信息。
十三、在openwrt中安装.ipk文件并使用
第一步
将.ipk文件传从Ubuntu中发送到OpenWrt系统中。

![](https://img-blog.csdnimg.cn/20191017175440793.png)

开启Ubuntu，在Ubuntu中开启一个HTTPS服务器，监听端口为8080，等待openwrt路由器连接hello程序的连接。

![](https://img-blog.csdnimg.cn/20191017183828261.png)

第二步
使用openwrt的opkg命令安装.ipk文件，安装程序的时候，配置文件都启动了，因此在第二张图中可以看到我们的Ubuntu服务器接收到了openwrt的连接。

![](https://img-blog.csdnimg.cn/20191017230914510.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20191017230926672.png)


安装完之后可以看到hello软件在/usr/sbin/hello目录下（Makefile脚本设计的），我们可以直接在命令行输入hello运行这个程序。

配置文件在/etc/config目录下，开机自启动的程序在/etc/init.d目录下（Makefile脚本设计的）。

![](https://img-blog.csdnimg.cn/20191017183416814.png)

在/etc.rc.d目录下也创建了我们的开机自启动脚本的软链接。

![](https://img-blog.csdnimg.cn/20191018102151123.png)

这样之后我们的软件就可以开机自启动了
备注：如果想要取消我们的软件在开机自启动，输入（/etc/init.d/hello  disable）即可，同时在/etc/rc.d下的软链接也会消失。
第三步（可选）
如果想要卸载软件，输入下面的命令即可。

![](https://img-blog.csdnimg.cn/20191026214656766.png)

此时再去输入hello命令发现软件已经不存在了，并且配置文件与启动脚本都不存在了。

![](https://img-blog.csdnimg.cn/20191026214745535.png)

备注
本次设计，在openwrt关机重启之后，脚本却没有执行，解决了很久还没有解决，待续......。
十四、构建中出现的问题
问题一
如果编译时出错，请检查如下地方：
①hello顶级目录下的Makefile是否增加了libuci库。
②hello/src目录下的Makefile中gcc命令是否加上了-luci。
③检查Makefile脚本缩进是否正确。
问题二
如果程序运行时遇到libc找不到的错误，是因为编译时的Makefile编写不正确，使用了宿主机的编译指令导致使用宿主机的libc.so.6动态链接库。
问题三
出现下面的问题是因为我们的hello.init文件中换行符不是属于Linux格式的换行符，是换行符引发的问题。

![](https://img-blog.csdnimg.cn/20191017230242980.png)

解决办法是使用notepad编辑hello.init配置文件，改用Linux格式

![](https://img-blog.csdnimg.cn/20191018102517525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNDUzMjg1,size_16,color_FFFFFF,t_70)


问题四
开机之后脚本不执行，查看/etc/init.d/hello脚本是否有可执行权限，没有可执行权限，使用下面方法增加。
chmod +x /etc/init.d/hello

————————————————
版权声明：本文为CSDN博主「董哥的黑板报」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_41453285/article/details/102545618







使用替换rootfs的方式给ARM平台移植OpenWrt系统



为了让不在OpenWrt项目支持列表中的ARM机器快速使用OpenWrt，可以用替换rootfs的简易方法进行移植。

首先，确保你的ARM盒子有能正常使用的Linux系统，并且最好有良好的社区支持，可以去armbian寻找有开源支持的开发板。
以下SoC的开源支持比较良好，多数情况下可以用上主线内核：

    Rockchip

        RK3328/RK3368/RK3399 系列

    amlogic

        s905/s912/s922 系列

    sunxi

        h2/h3/h5 系列

    marvell

        armada a3700 系列

移植OpenWrt的rootfs过程中，需要特别注意的是必须保证原有系统内与内核有关的东西（包括内核模块）不能有任何丢失。多数固件的Linux内核镜像、dtb和uboot的部分变量都是直接存放到rootfs里面的，在替换的时候一定要把这些文件完整保留。

注意：替换rootfs后，机器实际使用的内核与openwrt中由opkg管理的内核（包含内核模块）没有任何关系，因此如果要增删内核模块的话不能使用opkg进行管理。

具体流程：

    准备好Linux环境，可以用虚拟机或实体机，不能用wsl

    下载armvirt的通用rootfs，以OpenWrt 18.06.4为例

        32位arm，适合于Cortex A7/A9/A15等：
        https://downloads.openwrt.org/releases/18.06.4/targets/armvirt/32/openwrt-18.06.4-armvirt-32-default-rootfs.tar.gz

        64位arm，适合于Cortex A53/A72等：
        https://downloads.openwrt.org/releases/18.06.4/targets/armvirt/64/openwrt-18.06.4-armvirt-64-default-rootfs.tar.gz

    判断固件的分区类型

        对于amlogic的固件，一般有2个分区：第一分区为FAT32分区，用于存放内核镜像、dtb、uboot变量和脚本；第二分区为ext4分区，用作真正的rootfs。替换的时候必须保留FAT32分区的所有内容以及ext4分区内的/lib/modules和/lib/firmware

        对于rockchip或sunxi的sd卡固件，一般只有1个ext4分区作为rootfs，此分区内使用/boot文件夹用于存放内核、dtb、uboot变量和脚本。在替换的时候，必须保留此分区内的/lib/modules、/lib/firmware以及/boot。此类固件的bootloader存放在ext4分区前未分配的空间中。

    准备好你要修改的固件，注意修改的固件的rootfs分区必须是可写入的文件系统（如ext4），否则不能操作。

        对于从SD卡启动的机器，将固件用etcher写入SD卡，然后在你的Linux环境下挂载好SD卡，继续下一步操作。

        对于提供emmc刷机固件的机器，需要用loop挂载镜像，具体操作如下：

# 创建挂载目录
mkdir -p /mnt/rootfs

# 查看固件的分区表
sfdisk -J /path/to/firmware
#
# 以amlogic的双分区(FAT32+EXT4)固件为例：
# {
#    "partitiontable": {
#       "label":"dos",
#       "id":"0x1028b956",
#       "device":"/path/to/firmware",
#       "unit":"sectors",
#       "partitions": [
#          {"node":"/path/to/firmware1", "start":8192, "size":262144, "type":"e"},
#          {"node":"/path/to/firmware2", "start":270336, "size":30694112, "type":"83"}
#       ]
#    }
# }
# 可以看到rootfs为第二分区，并且偏移量为270336 blocks，所以偏移的字节数为270336*512

# 带偏移量挂载
sudo mount -o loop,offset=$((270336*512)) /path/to/firmware /mnt/rootfs

    处理/lib/modules
    一般来说Linux发行版的/lib/modules目录结构都是这样的：

/lib/modules/<内核版本号>/
├── kernel
│   ├── arch
│   ├── crypto
│   ├── drivers
│   ├── fs
│   ├── lib
│   ├── mm
│   ├── net
│   ├── security
│   ├── sound
│   └── virt
├── modules.alias
├── modules.alias.bin
├── modules.builtin
├── modules.builtin.bin
├── modules.dep
├── modules.dep.bin
├── modules.devname
├── modules.order
├── modules.softdep
├── modules.symbols
└── modules.symbols.bin

而OpenWrt的/lib/modules/<内核版本号>下面直接存放kernel目录下的所有模块，所以需要我们手工移动一下，具体操作如下：

# root挂载的目录一般需要root权限，可以用sudo或者切换成root用户操作
# 进入rootfs的挂载点
cd /path/to/rootfs

# 进入内核模块目录
cd ./lib/modules/<内核版本号>/

# 删除当前目录下的所有文件，但不删除kernel目录
sudo rm -f * 2>/dev/null

# 找出kernel目录下面的所有ko并移动到当前目录下
sudo mv $(find kernel -type f) .

# 删除空的kernel文件夹
sudo rm -r kernel

    备份文件并解压OpenWrt的rootfs
    如果该镜像的内核镜像、dtb或uboot脚本等文件在rootfs里面的话，需要先备份出来。对于不同板子和不同固件，这一步的操作都不同，下面以armbian为例：

# 创建一个临时目录用于存放备份文件
mkdir -p /tmp/backup

# 进入rootfs的挂载点
cd /path/to/rootfs

# 对于把内核、dtb或uboot变量放在/boot目录的固件，例如sunxi和rockchip，需要将整个/boot目录备份出来
cp -ra ./boot /tmp/backup/boot

# 备份/lib/modules和/lib/firmware
cp -ra ./lib/modules /tmp/backup/
cp -ra ./lib/firmware /tmp/backup/

# 备份后删除当前rootfs下的所有文件
sudo rm -rf *

# 将OpenWrt的rootfs解压出来
sudo tar -xvf /path/to/openwrt/rootfs.tar.gz

# 删除OpenWrt rootfs里自带的/boot目录
sudo rm -rf ./boot

# 恢复刚才备份的目录
sudo mv -f /tmp/backup/boot .
sudo mv -f /tmp/backup/modules/<内核版本号> ./lib/modules
sudo mv -f /tmp/backup/firmware ./lib/firmware

    修改rootfs

    启用串口的getty：对于串口设备名是ttyS0的内核，/etc/inittab里已经包含，所以无需修改，而其它串口设备名就需要手动添加，具体如下：

# 进入rootfs的挂载点
cd /path/to/rootfs

# 对于amlogic的内核，串口设备名为ttyAML0
echo "ttyAML0::askfirst:/usr/libexec/login.sh" |sudo tee -a ./etc/inittab

# 对于rockchip的bsp内核，串口设备名为ttyFIQ0
echo "ttyFIQ0::askfirst:/usr/libexec/login.sh" |sudo tee -a ./etc/inittab

# 对于marvell armada的bsp内核，串口设备名为ttyMV0
echo "ttyMV0::askfirst:/usr/libexec/login.sh" |sudo tee -a ./etc/inittab

    rootfs修改完成，卸载文件系统

sync
cd / && sudo umount /path/to/rootfs

    启动修改后的固件，进入OpenWrt之后检查以下命令能否正常工作：

iptables -L
ip6tables -L
ip route

如果你的内核版本大于4.18，且iptables抛出以下错误：

root@OpenWrt:~# iptables -Liptables v1.6.2: can't initialize iptables table `filter': No child process

那么说明原内核启用了bpfilter，这可能需要重新编译整个内核，请参考这篇文章：https://www.jianshu.com/p/48e2f3e6caeb

文章作者：https://www.jianshu.com/u/70c0d31e3717
本文章由作者：佐须之男 整理编辑，原文地址: 使用替换rootfs的方式给ARM平台移植OpenWrt系统





一、init进程介绍

    init进程是所有系统进程的父进程，它被内核调用起来并负责调用所有其他的进程。 如果任何进程的父进程退出，init进程将成为它的父进程。但是init进程是如何将其他进程调用起来的呢？接下来介绍。

二、OpenWrt软件启动机制


    第一步

        内核启动完成后读取/etc/inittab文件，然后执行inittab中的sysinit所指的脚本（/etc/init.d/rcS）。
        OpenWrt的inittab文件内容如下：

    从init进程逐步到/etc/init.d，整体分析Openwrt的软件启动机制_软件启动机制（init进程）



    第二步

        如果按照通常的简单做法：我们会将每一个待启动的程序启动命令按行放入rcS文件中，并顺序执行。这种实现方法在软件启动进程列表不变时工作得非常好，如果需要动态修改，则不容易以程序来控制（在OpenWrt下，使用ls命令查看不到/etc/init.d/rcS这个文件）。OpenWrt引入了一个便于控制的启动机制，这种机制是在/etc/rc.d目录下创建每个软件的软链接方式，由rcS脚本在该目录读取启动命令的软链接， 然后启动软链接所指向的程序，由于每一个软链接均包含一个数字，这样就可以按照数字顺序读取并进行启动了。
        具体执行流程：执行/etc/init.d/rcS脚本时，给脚本传递两个参数（分别为S何boot），接着rcS脚本通过run_scripts函数来启动软件，将每一个以/etc/rc.d/S开头的脚本按照数字传递boot参数并调用。

        S：表示软件启动模块，是和 K（软件关闭）相对应的。
        boot：表示首次启动。

    从init进程逐步到/etc/init.d，整体分析Openwrt的软件启动机制_/etc/init.d）_02

        例如从/etc/rc.d目录下的脚本可以看出，就是先执行../init.d/sysfixtime，再执行../init.d/boot，以此类推......。几个比较重要的程序如下：

        S10boot：调用uci_apply_defaults执行第1此开机时的UCI配置初始化，该函数执行/etc/uci-defaults/下的所有脚本，执行成功后就删除，因此该目录下的脚本只有第一次开机才会执行。
        S10system：根据UCI配置文件/etc/config/system配置系统，具体可参考该配置文件。
        S11sysctl：根据/etc/sysctl.conf配置系统（[-f  /etc/sysctl.conf]  && sysctl -p -e >&-）。
        S19filewall：启动防火墙fw3，该工具来自openwrt软件包package/network/config/firewal。
        S20network：根据UCI配置文件/etc/config/network，使用守护进程/sbin/netifd来配置网络。

    从init进程逐步到/etc/init.d，整体分析Openwrt的软件启动机制_/etc/inittab_03


三、“/etc/init.d/*”脚本分析

    上面那些最终调用的软件启动shell脚本，包含变量定义和函数定义（start、stop和restart等函数）。
    备注：因为/etc/rc.d/下面文件是/etc/int.d/下脚本的软链接，根据软链接的特性，我们查看软链接文件也就可以访问到/etc/int.d/下面的脚本文件。
    另外使用 opkg 命令安装软件时一般均有执行权限，如果是自己手动新增脚本，不要忘记确认脚本是否有执行权限（通过运行 chmod +x /etc/init.d/hello命令来增加执行权限）。


    脚本分析

    从init进程逐步到/etc/init.d，整体分析Openwrt的软件启动机制_/etc/init.d）_04

        /etc/rc.common：这个脚本没有解析自己的命令行参数，这是通过“/etc/rc.common”脚本回调来完成的。第一行是特殊的注释行，表示使用“/etc/rc.common”来提供一些基本函数，包含主函数及默认功能以及检查脚本执行等。
        START、STOP变量：脚本的执行顺序通过START和 STOP变量来定义。

        如果这两个变量被更改了，再次运行/etc/init.d/hello enable才会再次生效，并且这将删除以前创建的启动链接，然后再根据新的变量定义创建链接（创建的启动链接保存在“/etc/rc.d”目录下）。
        如果多个初始化脚本有相同的启动优先值，则调用顺序取决于启动脚本名称的字母顺序（在上上张图中S10boot与S10system文件中的START变量值相同，但是他们按照脚本名称的字母排序启动）。

        start()、stop()函数：脚本中最重要的函数是 start 和 stop，这两个函数决定如何启动和停止服务。



    rc.common函数含义

        备注：其中的start()、stop()函数实现为空，供应用软件重新实现，相当于C++语言中的虚函数。
        enable、disable 和 enabled函数提供自启动状态的设置和查询。
        help函数提供命令帮助信息，如果你不带参数运行软件命令，将会自动调用help函数输出帮助信息。
        boot函数与start函数的关系：命令在启动时取代 start 函数而执行 boot 函数，如果 boot 函数没 有被重新定义，将执行 rc.common 中预定义的 boot 函数，boot 函数再次调用 start 函数。
        详情见下表：

    函 数
    	

    含 义

    start
    	

    启动服务。相当于 C++语言中的虚函数，通常情况下每一个服务均需重写该函数

    stop
    	

    关闭服务。相当于 C++语言中的虚函数，通常情况下每一个服务均需重写该函数

    restart
    	

    重启服务。调用 stop 函数退出进程，然后再调用 start 函数启动进程

    reload
    	

    重新读取配置，如果读取配置失败则调用 restart 函数重启进程

    enable
    	

    打开服务自启动，即将启动脚本软链接文件放在/etc/rc.d 目录下

    disable
    	

    关闭服务自启动，删除在/etc/rc.d 的软链接文件

    enabled
    	

    提供服务自启动的状态查询

    boot
    	

    调用 start 函数

    shutdown
    	

    调用 stop 函数

    help
    	

    输出帮助信息



    软链接特性

        ①当我们使用enable设置一个软件为开机自启动时，将自动在/etc/rc.d/目录下创建一个软链接指向/etc/init.d/目录下的软件。
        ②当我们使用disable关闭一个软件开机自启动时，软件在/etc/rc.d/目录下的软链接将删除。


四、自定义软件的启动脚本设计案例

    可以参考相关文章：
-----------------------------------
©著作权归作者所有：来自51CTO博客作者董哥的黑板报的原创作品，请联系作者获取转载授权，否则将追究法律责任
从init进程逐步到/etc/init.d，整体分析Openwrt的软件启动机制
https://blog.51cto.com/u_15346415/5224097





 [OpenWRT网络配置](https://www.cnblogs.com/kevinjen/p/16489944.html)

原文连接：Network configuration [Old OpenWrt Wiki]
网络设置

OpenWrt的网络配置文件是/etc/config/network，它负责交换芯片VLAN、网络接口和路由的配置。

此文件在编辑和保存之后需要执行

/etc/init.d/network reload

命令，目的是为了在变更生效前，停止和重启网络。但是，路由器没必要执行重启操作。

    https://dev.openwrt.org/browser/branches/attitude_adjustment/package/base-files/files/etc/config/network
    https://dev.openwrt.org/browser/trunk/package/base-files/files/etc/config/network

可以参考netifd
节

下面是网络配置需要定义的节类型。通常情况下，一台路由器的最简网络配置包括至少2个interfaces(lan和wan)，如果硬件支持，还包括一个switch。
switch

switch节负责交换芯片VLAN的划分。在OpenWrt系统内部，每个VLAN都会有一个独立的interface与它对应，即便它们实际上属于同一个硬件。需要注意的是，不是所有被OpenWrt系统支持的设备都含有可编程的交换芯片，因此这个节在某些平台上不会出现。

现在有2个不同的配置格式在使用，一个是/proc/switch/的API，另一个是新的基于swconfig的switch架构。
/proc/switch

这个变种只出现在Broadcom设备上，比如WRT54GL。

一个典型的配置就像这样：

config 'switch' 'eth0'
        option 'vlan0' '0 1 2 3 5*'
        option 'vlan1' '4 5'

其中，eth0标识符指明这个节对应的交换芯片。VLAN的定义方法是显然的，一般来说，0、1、2、3是路由器LAN口，4是路由器WAN口，5表示CPU，而5*表示这个接口是trunk。更多的信息请参考 switch documentation。
swconfig

新的swconfig框架将取代老式switch配置。现在它正被一些设备如D-Link DIR-300使用。

基于Swconfig的配置是一种全新的格式，即每个VLAN都使用一个节。下面的例子说明了运行在D-Link DIR-300设备上的Kamikaze 8.09分支所使用的标准配置：

config 'switch' 'eth0'
        option 'reset' '1'
        option 'enable_vlan' '1'

config 'switch_vlan' 'eth0_1'
        option 'device' 'eth0'
        option 'vlan' '1'
        option 'ports' '0 1 2 3 5t'

config 'switch_vlan' 'eth0_2'
        option 'device' 'eth0'
        option 'vlan' '2'
        option 'ports' '4 5t'

通用的属性仍然在switch节定义，各VLAN相关属性定义在各自的switch_vlan节中，它们通过指定device属性使系统得悉它使用的交换芯片。完整的格式请参看switch documentation。
Interfaces

interface类型的节声明了逻辑网络接口，可以为这些接口指定IP地址、别名、物理网络接口名称、路由规则及防火墙规则。

一个最简的interface节包含以下行：

config 'interface' 'wan'
        option 'proto' 'dhcp'
        option 'ifname' 'eth0.1'

    wan 是唯一的 逻辑网络接口名称
    dhcp 指定接口协议，这里是dhcp
    eth0.1 是相关联的物理网络接口名称(用ifconfig可看到)

接口协议可以是以下之一：
协议 	描述
static 	静态ip
dhcp 	动态ip
ppp 	PPP 协议 - 拨号MODEM连接
pppoe 	以太网上的PPP协议 - DSL宽带连接
pppoa 	ATM上的PPP协议 - 使用内建MODEM的DSL连接
3g 	使用3G上网卡的CDMA, UMTS 或 GPRS 连接
pptp 	通过PPtP VPN的连接
none 	不指定任何协议

针对指定协议的类型，还需要额外的选项,参看下表。在Required字段标记"yes"的选项必须在interface节中定义，标记"no"选项的可以省略。
所有协议类型的有效选项
名称 	类型 	必需性 	默认 	描述
ifname 	interface name(s) 	yes(*) 	(none) 	物理接口名称。如果type选项被设为bridge，需要指定一个接口列表.\ (*) This option may be empty or missing if only a wireless interface references this network
type 	string 	no 	(none) 	如果设置为"bridge"，将建立一个包含ifname所述接口的网桥
stp 	boolean 	no 	0 	启用生成树协议，只对网桥有效
macaddr 	mac address 	no 	(none) 	指定接口MAC地址
mtu 	number 	no 	(none) 	指定接口最大传输单元(MTU)
auto 	boolean 	no 	0 for proto none, else 1 	指定是否在引导成功后打开这个接口
"static"协议
名称 	类型 	必需性 	默认 	描述
ipaddr 	ip address 	yes, if no ip6addr is set 	(none) 	IP地址
netmask 	netmask 	yes, if no ip6addr is set 	(none) 	子网掩码
gateway 	ip address 	no 	(none) 	默认路由
bcast 	ip address 	no 	(none) 	广播地址 (不设置会自动生成)
ip6addr 	ipv6 address 	yes, if no ipaddr is set 	(none) 	为接口指派给定的IPv6 地址 (CIDR notation)
ip6gw 	ipv6 address 	no 	(none) 	为接口指派给定的IPv6默认网关
dns 	list of ip addresses 	no 	(none) 	DNS服务器（1个或多个）
"dhcp"协议
名称 	类型 	必需性 	默认 	描述
gateway 	string 	no 	(none) 	如果设置为0.0.0.0，将设置DHCP协议获取的默认网关
dns 	list of ip addresses 	no 	(none) 	指定DNS服务器（1个或多个）
"ppp" (Modem上的PPP)协议

要使用PPP，必须安装ppp软件包。
名称 	类型 	必需性 	默认 	描述
device 	file path 	yes 	(none) 	MODEM设备节点
username 	string 	no(?) 	(none) 	用于PAP/CHAP认证的用户名
password 	string 	no(?) 	(none) 	用于PAP/CHAP authentication的密码
connect 	file path 	no 	(none) 	自定义PPP连接脚本路径
disconnect 	file path 	no 	(none) 	自定义PPP断开连接脚本路径
keepalive 	number 	no 	(none) 	Number of connection failures before reconnect
demand 	number 	no 	(none) 	Number of seconds to wait before closing the connection due to inactivity
defaultroute 	boolean 	no 	1 	Replace existing default route on PPP connect
peerdns 	boolean 	no 	1 	Use peer-assigned DNS server(s)
dns 	list of ip addresses 	no 	(none) 	Override peer-assigned DNS server(s)
ipv6 	boolean 	no 	0 	为PPP连接启用IPv6
pppd_options 	string 	no 	(none) 	传递给pppd守护进程的额外命令行参数
"pppoe" (以太网的PPP)协议

要使用PPPoE，必须安装ppp-mod-pppoe软件包.
名称 	类型 	必需性 	默认 	描述
username 	string 	no(?) 	(none) 	用于PAP/CHAP认证的用户名
password 	string 	no(?) 	(none) 	用于PAP/CHAP认证的密码
connect 	file path 	no 	(none) 	自定义PPP连接脚本路径
disconnect 	file path 	no 	(none) 	自定义PPP断开连接脚本路径
keepalive 	number 	no 	(none) 	Number of connection failures before reconnect
demand 	number 	no 	(none) 	Number of seconds to wait before closing the connection due to inactivity
defaultroute 	boolean 	no 	1 	Replace existing default route on PPP connect
peerdns 	boolean 	no 	1 	Use peer-assigned DNS server(s)
dns 	list of ip addresses 	no 	(none) 	Override peer-assigned DNS server(s)
ipv6 	boolean 	no 	0 	为PPP连接启用IPv6
pppd_options 	string 	no 	(none) 	传递给pppd守护进程的额外命令行参数
"pppoa" (ATM上的PPP)协议

要使用PPPoA，必须安装 ppp-mod-pppoa 软件包.
名称 	类型 	必需性 	默认 	描述
unit 	number 	yes 	(none) 	br2684 interface number
vci 	number 	no 	35 	PPPoA VCI
vpi 	number 	no 	8 	PPPoA VPI
atmdev 	number 	no 	(none) 	ATM PVC number
encaps 	string 	no 	llc 	PPPoA encapsulation mode: 'llc' (LLC) or 'vc' (VC)
payload 	string 	no 	bridged 	PPPoA forwarding mode: 'routed' or 'bridged'
username 	string 	no(?) 	(none) 	用于PAP/CHAP认证的用户名
password 	string 	no(?) 	(none) 	用于PAP/CHAP认证的密码
connect 	file path 	no 	(none) 	自定义PPP连接脚本路径
disconnect 	file path 	no 	(none) 	自定义PPP断开连接脚本路径
keepalive 	number 	no 	(none) 	Number of connection failures before reconnect
demand 	number 	no 	(none) 	Number of seconds to wait before closing the connection due to inactivity
defaultroute 	boolean 	no 	1 	Replace existing default route on PPP connect
peerdns 	boolean 	no 	1 	Use peer-assigned DNS server(s)
dns 	list of ip addresses 	no 	(none) 	Override peer-assigned DNS server(s)
ipv6 	boolean 	no 	0 	为PPP连接启用IPv6
pppd_options 	string 	no 	(none) 	传递给pppd守护进程的额外命令行参数
"3g" (PPP over EV-DO, CDMA, UMTS or GRPS)协议

要使用3G，必须安装 comgt 软件包.
名称 	类型 	必需性 	默认 	描述
device 	file path 	yes 	(none) 	Modem设备节点
service 	string 	yes 	gprs 	3G设备类型: evdo, cdma, umts 或 gprs
apn 	string 	yes 	(none) 	Used APN
pincode 	number 	no 	(none) 	解锁SIM卡的PIN码
maxwait 	number 	no 	20 	等待Modem就绪的时间（秒）
username 	string 	no(?) 	(none) 	用于PAP/CHAP认证的用户名
password 	string 	no(?) 	(none) 	用于PAP/CHAP认证的密码
keepalive 	number 	no 	(none) 	Number of connection failures before reconnect
demand 	number 	no 	(none) 	Number of seconds to wait before closing the connection due to inactivity
defaultroute 	boolean 	no 	1 	Replace existing default route on PPP connect
peerdns 	boolean 	no 	1 	Use peer-assigned DNS server(s)
dns 	list of ip addresses 	no 	(none) 	Override peer-assigned DNS server(s)
ipv6 	boolean 	no 	0 	为PPP连接启用IPv6
"pptp" (Point-to-Point Tunneling Protocol)协议

要使用PPtP，必须安装 pptp 软件包.
名称 	类型 	必需性 	默认 	默认
server 	ip address 	yes 	(none) 	远程PPtP服务器
ipproto 	string 	no 	dhcp 	建立PPtP隧道前用来获取IP连通性的协议
username 	string 	no(?) 	(none) 	用于PAP/CHAP认证的用户名
password 	string 	no(?) 	(none) 	用于PAP/CHAP认证的密码
Additionally all options defined for the corresponding ipproto can be specified
别名

Alias sections can be used to define further IPv4 and IPv6 addresses for interfaces.
They also allow combinations like DHCP on the main interface and a static IPv6 address in the alias,
for example to deploy IPv6 on wan while keeping normal internet connectivity.
Each interface can have multiple aliases attached to it.

别名最小的声明包含以下行：

config 'alias'
        option 'interface' 'lan'
        option 'proto' 'static'
        option 'ipaddr' '10.0.0.1'
        option 'netmask' '255.255.255.0'

    lan is the logical interface name of the parent interface
    static is the alias interface protocol
    10.0.0.1 specifies the alias ip address
    255.255.255.0 specifies the alias netmask

At the time of writing, only the static protocol type is allowed for aliases.
Defined options for alias sections are listed below.
名称 	类型 	必需性 	默认 	描述
interface 	string 	yes 	(none) 	Specifies the logical interface name of the parent (or master) interface this alias is belonging to, must refer to one of the defined interface sections
proto 	string 	yes 	(none) 	Specifies the alias interface protocol
ipaddr 	ip address 	yes, if no ip6addr is set 	(none) 	IP address
netmask 	netmask 	yes, if no ip6addr is set 	(none) 	Netmask
gateway 	ip address 	no 	(none) 	Default gateway
bcast 	ip address 	no 	(none) 	Broadcast address (autogenerated if not set)
ip6addr 	ipv6 address 	yes, if no ipaddr is set 	(none) 	IPv6 address (CIDR notation)
ip6gw 	ipv6 address 	no 	(none) 	IPv6 default gateway
dns 	list of ip addresses 	no 	(none) 	DNS server(s)
IPv4 Routes

It is possible to define arbitary IPv4 routes on specific interfaces using route sections. As for aliases, multiple sections can be attached to an interface.

一个minimial的例子类似如下:

config 'route'
        option 'interface' 'lan'
        option 'target' '172.16.123.0'
        option 'netmask' '255.255.255.0'

    lan is the logical interface name of the parent interface
    172.16.123.0 is the network address of the route
    255.255.255.0 specifies the route netmask

Legal options for IPv4 routes are:
名称 	类型 	必需 	默认 	描述
interface 	string 	yes 	(none) 	Specifies the logical interface name of the parent (or master) interface this route is belonging to, must refer to one of the defined interface sections
target 	ip address 	yes 	(none) 	Network address
netmask 	netmask 	no 	(none) 	Route netmask. If ommitted, 255.255.255.255 is assumed which makes target a host address
gateway 	ip address 	no 	(none) 	Network gateway. If ommitted, the gateway from the parent interface is taken, if set to 0.0.0.0 no gateway will be specified for the route
metric 	number 	no 	0 	Specifies the route metric to use
IPv6路由

IPv6 routes可指定定义一个或多个route6的sections.

一个minimial的例子类似如下:

config 'route6'
        option 'interface' 'lan'
        option 'target' '2001:0DB8:100:F00:BA3::1/64'
        option 'gateway' '2001:0DB8:99::1'

    lan is the logical interface name of the parent interface
    2001:0DB8:100:F00:BA3::1/64 is the routed IPv6 subnet in CIDR notation
    2001:0DB8:99::1 specifies the IPv6 gateway for this route

Legal options for IPv6 routes are:
名称 	类型 	必要 	默认 	描述
interface 	string 	yes 	(none) 	Specifies the logical interface name of the parent (or master) interface this route is belonging to, must refer to one of the defined interface sections
target 	ipv6 address 	yes 	(none) 	IPv6 network address
gateway 	ipv6 address 	no 	(none) 	IPv6 gateway. If ommitted, the gateway from the parent interface is taken
metric 	number 	no 	0 	Specifies the route metric to use
例子

下面是特殊的，非标准接口配置的几个例子。
Bridge without IP

config 'interface' 'example'
        option 'type'    'bridge'
        option 'proto'   'none'
        option 'ifname'  'eth0 eth1'
        option 'auto'    '1'

DHCP without default gateway

config 'interface' 'example'
        option 'proto'   'dhcp'
        option 'ifname'  'eth0'
        option 'gateway' '0.0.0.0'

DHCP及IPv6

config 'interface' 'example'
        option 'proto'     'dhcp'
        option 'ifname'    'eth0'

config 'alias'
        option 'interface' 'example'
        option 'proto'     'static'
        option 'ip6addr'   '2001:0DB8:100:F00:BA3::1'

静态IP配置和默认网关与非零十进制

config 'interface' 'example'
        option 'proto'     'static'
        option 'ifname'    'eth0'
        option 'ipaddr'    '192.168.1.200'
        option 'netmask'   '255.255.255.0'
        option 'dns'       '192.168.1.1'

config 'route'
        option 'interface' 'example'
        option 'target'    '0.0.0.0'
        option 'netmask'   '0.0.0.0'
        option 'gateway'   '192.168.1.1'
        option 'metric'    '100'

PPtP-over-PPPoE internet connection

config 'interface' 'wan'
        option 'proto'     'pppoe'
        option 'ifname'    'eth1'
        option 'username'  'user'
        option 'password'  'pass'
        option 'timeout'   '10'

config 'interface' 'vpn'
        option 'proto'     'pptp'
        option 'ifname'    'vpn'
        option 'username'  'vpnuser'
        option 'password'  'vpnpass'
        option 'server'    'vpn.example.org' 

Additionally the "wan" firewall zone must include both interfaces in /etc/config/firewall:

config 'zone'
        option 'name'      'wan'
        option 'network'   'wan vpn'
        option 'input'     'REJECT'
        option 'forward'   'REJECT'
        option 'output'    'ACCEPT'
        option 'masq'      '1' 





