在在ARM Linux上使用上使用OpenGL
  这两天在ARM上跑了一个OpenGL，应该说是OpenGL|ES的软件包，虽然我觉得可能后我不会使用这个软件包，但是还是记录下来以备忘记。
先全局介绍下，首先，什么是OpenGL和OpenGL|ES。OpenGL是一套绘图函数的标准，OpenGL|ES是OpenGL中针对嵌入式系统的一套函数子集。OpenGL|ES的官方是http://www.khronos.org/opengles/， 上面有更多的信息。需要注意的是，OpenGL仅仅是标准，而如果你要使用OpenGL，则需要找到可以实现这些OpenGL标准函数的程序库或源代码。 

目前，在ARM这种一般没有图形加速器的平台上，OpenGL|ES的实现都是靠软件的。主要的OpenGL|ES程序库有：Klimt，Vincent(ogles)，TinyGL等。Google一下，可以看到它们的主页。这些实现基本上都是基于X11、Qt/E这样的窗口平台的。我用的软件包叫做 PicoGL，它是TinyGL的一个分支，不同之处在于PicoGL支持直接在Framebuffer上输出图像。PicoGL是一位台湾同胞写的， 他的主页http://jserv.sayya.org/。但是这个软件包的源码极其难找，我用的是一位很牛的同事搜到的，地址是http://people.openmoko.org/jserv/graphics/picogl-20051108.tar.bz2。

要使用这个软件包还必须有支持软件浮点的交叉编译器，我们一般用的arm-linux-gcc 2.95.3/3.3.2/3.4.1等都不支持软件浮点，这需要我们重新编译一个。编译一个交叉编译器，引用Crosstool作者Dan Kegel的话，used to be a scaryprospect，requiring iron will，days if not weeks of effort……幸好有Crosstool这个工具，如果你的机器好网速快，输入个指令，差不多1个多小时可以编译出来。

crosstool的主页http://kegel.com/crosstool/，下载地址http://kegel.com/crosstool/crosstool-0.43.tar.gz。首先制作交叉编译器，先以root用户登陆，建立交叉编译器安装目录，而且把目录所有者改为你的普通用户

# mkdir /opt/crosstool

# chown /opt/crosstool lxz

# chgrp /opt/crosstool users然后以普通用户登陆，解压缩和安装

# tar xvzf crosstool-0.43.tar.gz# cd crosstool-0.43

# ./demo-arm-softfloat.sh


crosstool会从网上下载需要的源码包，然后编译，后安装。crosstool相关的资料可以看http://kegel.com/crosstool/current/doc/crosstool-howto.html。然后在/opt/crosstool/gcc-3.4.5-glibc-2.3.6/arm-softfloat-linux-gnu里面会有支持软件浮点的交叉编译器了。然后解压缩picogl，修改picogl的代码。首先是picogl的一处bug，在backends/vesafb/tk.c的第一行增加宏#define _FB_TK_然后把backends/vesafb/glx_impl.h里面的struct fb_fix_screeninfo FixedInfo;struct fb_var_screeninfo VarInfo， OrigVarInfo;修改为#ifdef _FB_TK_struct fb_fix_screeninfo FixedInfo;struct fb_var_screeninfo VarInfo， OrigVarInfo;#elseextern struct fb_fix_screeninfo FixedInfo;extern struct fb_var_screeninfo VarInfo， OrigVarInfo;#endif这是因为有好几个文件调用了backends/vesafb/glx_impl.h，如果不这么改，会出现多重定义错误。然后，为了让你的程序使用picogl更方便些，好再改个地方include/GL/glx.h#include GLX_IMPL_HEADER改为你喜欢的方式，指向backends/vesafb/glx_impl.h修改backends/vesafb/tk.c中，initialize_fbdev函数有关VarInfo的设置，改为适合你的LCD的。然后配置# cd PicoGL# CC=/opt/crosstool/gcc-3.4.5-glibc-2.3.6/arm-softfloat-linux-gnu/bin/arm-softfloat-linux-gnu-gcc ./configure –with-backend=vesafb –host=arm-softfloat-linux-gnu –prefix=/home/lxz/builtPicoGL说明下，CC=设置交叉编译器的位置，–with-backend=vesafb指定使用FB作为显示设备，host=arm- softfloat-linux-gnu设定交叉编译，–prefix=/home/lxz/builtPicoGL设定编译后库和示例程序安装位置。然后# make# make install在/home/lxz/builtPicoGL/lib里有PicoGL的库了，把动态库文件拷贝到arm-linux根文件系统的/lib 里面，把/home/lxz/builtPicoGL/bin里的程序拷贝到arm-linux文件系统的任何地方，然后制作和烧写文件系统映像（或者使 用NFS），执行，可以看到结果了。当你编译一个使用PicoGL库的程序（假设叫做hello.c）时，需要输入# arm-softfloat-linux-gnu-gcc -c -I /home/lxz/PicoGL/include -I /usr/include hello.c# arm-softfloat-linux-gnu-gcc -o hello -L /home/lxz/builtPicoGL/lib/libPicoGL.so -L /home/lxz/builtPicoGL/lib/libPicoGLU.so -lm hello.o /home/lxz/builtPicoGL/lib/libPicoGL.a /home/lxz/builtPicoGL/lib/libPicoGLU.a


Google 的 SwiftShader。其实原本不是 Google 的，前身是 TransGaming 的开源项目 swShader（后转为闭源），不过后来由 Google 接盘再开源的。SwiftShader 实现了 Vulkan、OpenGL ES、D3D 9，并且运行效率很不错。不过 SwiftShader 大量使用多线程，显然不适合 ESP32。


Mesa。Mesa 大概是最被广泛使用的 OpenGL/Vulkan 的软件实现了，Mesa 的运行销量也相当不错。但是 Mesa 过于庞大，移植难度非常大。
Vincent（ogles）。Vincent 实现了 OpenGL ES 1.1，由 C++ 编写，本身就是为嵌入式打造的。不过我简单浏览了下，为了优化，Vincent 的很多逻辑都是直接内嵌汇编的，和平台关系过于紧密，移植起来还是有难度的。


TinyGL。TinyGL 是 Fabrice Bellard 开发的 OpenGL 1.1 子集。Fabrice 不用多说，是神仙级程序员。TinyGL 是他开发的一个轻量级 C 语言的 OpenGL 软件实现。TinyGL 的一大优点是，本身实现是纯 C 的，没有用到任何汇编内嵌，而且编译结果按官方说明只有 40K，非常适合移植。
经过评估，我最后选择了 TinyGL 的一个分支实现 PicoGL。PicoGL 基于 TinyGL 4.0，增加了直接写 Linux Framebuffer 的 backend、使用 Makefile 组织项目、增加了定点数运算支持。


















