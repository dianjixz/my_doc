 

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
