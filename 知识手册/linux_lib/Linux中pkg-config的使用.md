1. pkg-config简单介绍

pkg-config在编译应用程序和库的时候作为一个工具来使用。例如你在命令行通过如下命令编译程序时：

# gcc -o test test.c `pkg-config --libs --cflags glib-2.0`

pkg-config可以帮助你插入正确的编译选项，而不需要你通过硬编码的方式来找到glib(或其他库）。

--cflags一般用于指定头文件，--libs一般用于指定库文件。

大家应该都知道一般用第三方库的时候，就少不了要使用到第三方的头文件和库文件。我们在编译、链接的时候，必须要指定这些头文件和库文件的位置。对于一个比较大的第三方库，其头文件和库文件的数量是比较多的，如果我们一个个手动地写，那将是相当的麻烦的。因此，pkg-config就应运而生了。pkg-config能够把这些头文件和库文件的位置指出来，给编译器使用。pkg-config主要提供了下面几个功能：

    检查库的版本号。 如果所需要的库的版本不满足要求，它会打印出错误信息，避免链接错误版本的库文件
    获得编译预处理参数，如宏定义、头文件的位置
    获得链接参数，如库及依赖的其他库的位置，文件名及其他一些链接参数
    自动加入所依赖的其他库的设置

pkg-config命令的基本用法如下：

# pkg-config <options> <library-name>

例如，我们可以通过如下命令来查看当前安装了哪些库：

[root@localhost pkgconfig]# pkg-config --list-all
zlib                      zlib - zlib compression library
gio-unix-2.0              GIO unix specific APIs - unix specific headers for glib I/O library
inputproto                InputProto - Input extension headers
cairo-xcb                 cairo-xcb - XCB surface backend for cairo graphics library
gio-2.0                   GIO - glib I/O library
//后续省略

2. 配置环境变量

事实上，pkg-config只是一个工具，所以不是你安装了一个第三方库，pkg-config就能知道第三方库的头文件和库文件的位置的。为了让pkg-config可以得到一个库的信息，就要求库的提供者提供一个.pc文件。默认情况下，比如执行如下命令：

# pkg-config --libs --cflags glib-2.0

pkg-config会到/usr/lib/pkconfig/目录下去寻找glib-2.0.pc文件。也就是说在此目录下的.pc文件，pkg-config是可以自动找到的。然而假如我们安装了一个库，其生成的.pc文件并不在这个默认目录中的话，pkg-config就找不到了。此时我们需要通过PKG_CONFIG_PATH环境变量来指定pkg-config还应该在哪些地方去寻找.pc文件。

我们可以通过如下命令来设置PKG_CONFIG_PATH环境变量：

# export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig/

这样pkg-config就会在/usr/local/lib/pkgconfig/目录下寻找.pc文件了。我们在文章开头提到的找不到Leptonica 1.74.4的原因也正是因为其找不到lept.pc，因此我们只需要将对应的目录设置到PKG_CONFIG_PATH环境变量中即可。

另外还需要注意的是,上述环境变量的设置只对当前的终端窗口有效。为了让其永久生效，我们可以将上述命令写入到/etc/bash.bashrc等文件中，以方便后续使用。
3. pkg-config与LD_LIBRARY_PATH

pkg-config与LD_LIBRARY_PATH在使用时有些类似，都可以帮助找到对应的库（静态库和共享库）。这里我们重点介绍一下它们两者的区别。我们知道一个程序从源代码，然后编译连接，最后再执行这一基本过程。这里我们列出pkg-config与LD_LIBRARY_PATH的主要工作阶段：

    pkg-config: 编译时、 链接时
    LD_LIBRARY_PATH: 链接时、 运行时

pkg-config主要是在编译时会用到其来查找对应的头文件、链接库等；而LD_LIBRARY_PATH环境变量则在 链接时 和 运行时 会用到。程序编译出来之后，在程序加载执行时也会通过LD_LIBRARY_PATH环境变量来查询所需要的库文件。

下面我们来讲述一下LD_LIBRARY_PATH及ldconfig命令：

库文件在链接（静态库和共享库）和运行（仅限于使用共享库的程序）时被使用，其搜索路径是在系统中进行设置的。一般Linux系统把/lib和/usr/lib这两个目录作为默认的库搜索路径，所以使用这两个目录中的库时不需要进行设置搜索路径即可直接使用。对于处于默认库搜索路径之外的库，需要将库的位置添加到库的搜索路径之中。设置库文件的搜索路径有下列两种方式，可任选其中一种使用：

    在环境变量LD_LIBRARY_PATH中指明库的搜索路径
    在/etc/ld.so.conf文件中添加库的搜索路径

将自己可能存放库文件的路径都加入到/etc/ld.so.conf中是明智的选择。添加方法也及其简单，将库文件的绝对路径直接写进去就OK了，一行一个。比如：

    /usr/X11R6/lib
    /usr/local/lib
    /opt/lib

需要注意的是：第二种搜索路径的设置方式对于程序链接时的库（包括共享库和静态库）的定位已经足够了。但是对于使用了共享库的程序的执行还是不够的，这是因为为了加快程序执行时对共享库的定位速度，避免使用搜索路径查找共享库的低效率，所以是直接读取库列表文件/etc/ld.so.cache的方式从中进行搜索。/etc/ld.so.cache是一个非文本的数据文件，不能直接编辑，它是根据/etc/ld.so.conf中设置的搜索路径由/sbin/ldconfig命令将这些搜索路径下的共享库文件集中在一起而生成的（ldconfig命令要以root权限执行）。因此为了保证程序执行时对库的定位，在/etc/ld.so.conf中进行了库搜索路径的设置之后，还必须要运行/sbin/ldconfig命令更新/etc/ld.so.cache文件之后才可以。

ldconfig，简单的说，它的作用就是将/etc/ld.so.conf列出的路径下的库文件缓存到/etc/ld.so.cache以供使用。因此当安装完一些库文件（例如刚安装好glib)，或者修改ld.so.conf增加新的库路径之后，需要运行一下/sbin/ldconfig使所有的库文件都被缓存到ld.so.cache中。如果没有这样做，即使库文件明明就在/usr/lib下的，也是不会被使用的，结果在编译过程中报错。

在程序链接时，对于库文件（静态库和共享库）的搜索路径，除了上面的设置方式之外，还可以通过-L参数显示指定。因为用-L设置的路径将被优先搜索，所以在链接的时候通常都会以这种方式直接指定要链接的库的路径。

前面已经说明过了，库搜索路径的设置有两种方式：在环境变量 LD_LIBRARY_PATH 中设置以及在 /etc/ld.so.conf 文件中设置。其中，第二种设置方式需要 root 权限，以改变 /etc/ld.so.conf 文件并执行 /sbin/ldconfig 命令。而且，当系统重新启动后，所有的基于 GTK2 的程序在运行时都将使用新安装的 GTK+ 库。不幸的是，由于 GTK+ 版本的改变，这有时会给应用程序带来兼容性的问题，造成某些程序运行不正常。为了避免出现上面的这些情况，在 GTK+ 及其依赖库的安装过程中对于库的搜索路径的设置将采用第一种方式进行。这种设置方式不需要 root 权限，设置也简单：

    # export LD_LIBRARY_PATH=/opt/gtk/lib:$LD_LIBRARY_PATH
     
    # echo $LD_LIBRARY_PATH

4. pc文件书写规范

这里我们首先来看一个例子：

    [root@localhost pkgconfig]# cat libevent.pc 
    #libevent pkg-config source file
     
    prefix=/usr/local
    exec_prefix=${prefix}
    libdir=${exec_prefix}/lib
    includedir=${prefix}/include
     
    Name: libevent
    Description: libevent is an asynchronous notification event loop library
    Version: 2.0.22-stable
    Requires:
    Conflicts:
    Libs: -L${libdir} -levent
    Libs.private: 
    Cflags: -I${includedir}

这是libevent库的一个真实的例子。下面我们简单描述一下pc文件中的用到的一些关键词：

    Name: 一个针对library或package的便于人阅读的名称。这个名称可以是任意的，它并不会影响到pkg-config的使用，pkg-config是采用pc文件名的方式来工作的。
    Description: 对package的简短描述
    URL: 人们可以通过该URL地址来获取package的更多信息或者package的下载地址
    Version: 指定package版本号的字符串
    Requires: 本库所依赖的其他库文件。所依赖的库文件的版本号可以通过使用如下比较操作符指定：=,<,>,<=,>=
    Requires.private: 本库所依赖的一些私有库文件，但是这些私有库文件并不需要暴露给应用程序。这些私有库文件的版本指定方式与Requires中描述的类似。
    Conflicts: 是一个可选字段，其主要用于描述与本package所冲突的其他package。版本号的描述也与Requires中的描述类似。本字段也可以取值为同一个package的多个不同版本实例。例如: Conflicts: bar < 1.2.3, bar >= 1.3.0
    Cflags: 编译器编译本package时所指定的编译选项，和其他并不支持pkg-config的library的一些编译选项值。假如所需要的library支持pkg-config,则它们应该被添加到Requires或者Requires.private中
    Libs: 链接本库时所需要的一些链接选项，和其他一些并不支持pkg-config的library的链接选项值。与Cflags类似
    Libs.private: 本库所需要的一些私有库的链接选项。

5. 示例

如下我们给出一个使用pkg-config的程序例子(test_event.cpp)：

    #include <iostream>  
    #include <event.h>  
    #include <sys/socket.h>  
    #include <sys/types.h>  
    #include <netinet/in.h>  
    #include <string.h>  
    #include <fcntl.h>  
      
    using namespace std;  
      
    struct event_base* main_base;  
      
    static const char MESSAGE[] ="Hello, World!\n";  
      
    void accept_handle(const int sfd, const short event, void *arg)  
    {  
        cout<<"accept handle"<<endl;  
      
        struct sockaddr_in addr;  
      
        socklen_t addrlen = sizeof(addr);  
      
        int fd = accept(sfd, (struct sockaddr *) &addr, &addrlen); //处理连接  
      
        struct bufferevent* buf_ev;  
        buf_ev = bufferevent_new(fd, NULL, NULL, NULL, NULL);  
      
        buf_ev->wm_read.high = 4096;  
      
        cout<<"event write"<<endl;  
        bufferevent_write(buf_ev, MESSAGE, strlen(MESSAGE));  
    }  
      
    int main()  
    {  
        cout<<"hello man!"<<endl;  
      
        // 1. 初始化EVENT  
        main_base = event_init();  
        if(main_base)  
            cout<<"init event ok!"<<endl;  
      
        // 2. 初始化SOCKET  
        int sListen;  
      
        // Create listening socket  
        sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
      
        // Bind  
        struct sockaddr_in server_addr;  
        bzero(&server_addr,sizeof(struct sockaddr_in));  
        server_addr.sin_family=AF_INET;  
        server_addr.sin_addr.s_addr=htonl(INADDR_ANY);  
        int portnumber = 8080;  
        server_addr.sin_port = htons(portnumber);  
      
        /* 捆绑sockfd描述符  */  
        if(bind(sListen,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)  
        {  
            cout<<"error!"<<endl;  
            return -1;  
        }  
      
        // Listen  
        ::listen(sListen, 3);  
        cout<<"Server is listening!\n"<<endl;  
      
        /*将描述符设置为非阻塞*/  
        int flags = ::fcntl(sListen, F_GETFL);  
      
        flags |= O_NONBLOCK;  
      
        fcntl(sListen, F_SETFL, flags);  
      
        // 3. 创建EVENT 事件  
        struct event ev;  
        event_set(&ev, sListen, EV_READ | EV_PERSIST, accept_handle, (void *)&ev);  
      
        // 4. 事件添加与删除  
        event_add(&ev, NULL);  
      
        // 5. 进入事件循环  
        event_base_loop(main_base, 0);  
      
        cout<<"over!"<<endl;  
    }

执行如下命令编译程序：

# gcc -o test_event test_event.cpp -lstdc++ `pkg-config --cflags --libs libevent`

运行程序：

[root@localhost test-src]# ./test_event 
hello man!
init event ok!
Server is listening!

开启另外一个终端，采用nc命令连接test_event服务端程序：

[root@localhost ~]# nc 127.0.0.1 8080
Hello, World!

可以看到运行成功。
6. Linux下链接库的路径顺序
6.1 运行时链接库的搜索顺序

Linux程序在运行时对动态链接库的搜索顺序如下：

1） 在编译目标代码时所传递的动态库搜索路径（注意，这里指的是通过-Wl,rpath=<path1>:<path2>或-R选项传递的运行时动态库搜索路径，而不是通过-L选项传递的）

例如：

# gcc -Wl,-rpath,/home/arc/test,-rpath,/lib/,-rpath,/usr/lib/,-rpath,/usr/local/lib test.c

或者
# gcc -Wl,-rpath=/home/arc/test:/lib/:/usr/lib/:/usr/local/lib test.c

2） 环境变量LD_LIBRARY_PATH指定的动态库搜索路径；

3） 配置文件/etc/ld.so.conf中所指定的动态库搜索路径(更改/etc/ld.so.conf之后，一定要执行命令ldconfig，该命令会将/etc/ld.so.conf文件中所有路径下的库载入内存）;

4） 默认的动态库搜索路径/lib；

5） 默认的动态库搜索路径/usr/lib;
6.2 编译时与运行时动态库查找的比较

下面是对编译时库的查找与运行时库的查找做一个简单的比较：

1) 编译时查找的是静态库或动态库， 而运行时，查找的是动态库；

2） 编译时可以用-L指定查找路径，或者用环境变量LIBRARY_PATH， 而运行时可以用-Wl,rpath或者-R选项，或者修改/etc/ld.so.conf，或者设置环境变量LD_LIBRARY_PATH;

3) 编译时用的链接器是ld，而运行时用的链接器是/lib/ld-linux.so.2

4) 编译时与运行时都会查找默认路径/lib、/usr/lib

5) 编译时还有一个默认路径/usr/local/lib，而运行时不会默认查找该路径；

说明： -Wl,rpath选项虽然是在编译时传递的，但是其实是工作在运行时。其本身其实也不算是gcc的一个选项，而是ld的选项，gcc只不过是一个包装器而已。我们可以执行man ld来进一步了解相关信息

6.3 补充:gcc使用-Wl,-rpath

1) -Wl,-rpath

加上-Wl,-rpath选项的作用就是指定程序运行时的库搜索目录，是一个链接选项，生效于设置的环境变量之前(LD_LIBRARY_PATH)。下面我们通过一个例子来说明：

    // add.h
    int add(int i, int j);
     
    // add.c
    #include "add.h"
     
    int add(int i, int j)
    {
    	return i + j;
    }
     
    // main.c
    #include <stdio.h>
    #include <stdlib.h>
    #include "add.h"
     
    int main(int argc, char *argv[]) 
    {
    	printf("1 + 2 = %d\n", add(1, 2));
    	return 0;
    }

add.h和add.c用于生成一个so库，实现了一个简单的加法，main.c中引用共享库计算1 + 2：

# 编译共享库
gcc add.c -fPIC -shared -o libadd.so
# 编译主程序
gcc main.o -L. -ladd -o app

编译好后运行依赖库：

    # ldd app
    linux-vdso.so.1 (0x00007ffeb23ab000)
    libadd.so => not found
    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007febb7dd0000)
    /lib64/ld-linux-x86-64.so.2 (0x00007febb83d0000
    # ./app
    ./app: error while loading shared libraries: libadd.so: cannot open shared object file: No such file or directory

可以看到， libadd.so这个库没有找到，程序也无法运行，要运行它必须要把当前目录添加到环境变量或者搜索路径中去。但是如果在链接时加上-Wl,rpath选项之后：

    # gcc -c -o main.o main.c
    # gcc -Wl,-rpath=`pwd` main.o -L. -ladd -o app
    # ldd app
    linux-vdso.so.1 (0x00007fff8f4e3000)
    libadd.so => /data/code/c/1-sys/solib/libadd.so (0x00007faef8428000)
    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007faef8030000)
    /lib64/ld-linux-x86-64.so.2 (0x00007faef8838000)
    # ./app
    1 + 2 = 3

依赖库的查找路径就找到了，程序能正常运行。

下面我们再来看一下生成的可执行文件app，执行如下命令：

#  readelf app -d

Dynamic section at offset 0xe08 contains 26 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [libadd.so]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000000f (RPATH)              Library rpath: [/root/test]
 0x000000000000000c (INIT)               0x400578
 0x000000000000000d (FINI)               0x400784
 0x0000000000000019 (INIT_ARRAY)         0x600df0
 0x000000000000001b (INIT_ARRAYSZ)       8 (bytes)
 0x000000000000001a (FINI_ARRAY)         0x600df8
 0x000000000000001c (FINI_ARRAYSZ)       8 (bytes)
 0x000000006ffffef5 (GNU_HASH)           0x400298
 0x0000000000000005 (STRTAB)             0x400408
 0x0000000000000006 (SYMTAB)             0x4002d0
 0x000000000000000a (STRSZ)              189 (bytes)
 0x000000000000000b (SYMENT)             24 (bytes)
 0x0000000000000015 (DEBUG)              0x0
 0x0000000000000003 (PLTGOT)             0x601000
 0x0000000000000002 (PLTRELSZ)           96 (bytes)
 0x0000000000000014 (PLTREL)             RELA
 0x0000000000000017 (JMPREL)             0x400518
 0x0000000000000007 (RELA)               0x400500
 0x0000000000000008 (RELASZ)             24 (bytes)
 0x0000000000000009 (RELAENT)            24 (bytes)
 0x000000006ffffffe (VERNEED)            0x4004e0
 0x000000006fffffff (VERNEEDNUM)         1
 0x000000006ffffff0 (VERSYM)             0x4004c6
 0x0000000000000000 (NULL)               0x0

可以看到是在编译后的程序中包含了库的搜索路径。

2) -Wl,rpath-link

-Wl,rpath-link是设置编译链接时候的顺序，例如app运行依赖libadd.so，但是libadd.so又依赖libadd_ex.so，rpath-link就是指定libadd_ex.so的路径。和-Wl,rpath相比工作的时间不同，一个在链接期间，一个在运行期间。
7. gcc编译头文件查找路径

关于gcc编译时的查找路径，GCC The C Preprocessor: Search Path描述的比较清楚，我在这里将其复制出来：

By default, the preprocessor looks for header files included by the quote form of the directive #include “file” first relative to the directory of the current file, and then in a preconfigured list of standard system directories. For example, if /usr/include/sys/stat.h contains #include “types.h”, GCC looks for types.h first in /usr/include/sys, then in its usual search path.

For the angle-bracket form #include , the preprocessor’s default behavior is to look only in the standard system directories. The exact search directory list depends on the target system, how GCC is configured, and where it is installed. You can find the default search directory list for your version of CPP by invoking it with the -v option. For example,

cpp -v /dev/null -o /dev/null There are a number of command-line options you can use to add additional directories to the search path. The most commonly-used option is -Idir, which causes dir to be searched after the current directory (for the quote form of the directive) and ahead of the standard system directories. You can specify multiple -I options on the command line, in which case the directories are searched in left-to-right order.

If you need separate control over the search paths for the quote and angle-bracket forms of the ‘#include’ directive, you can use the -iquote and/or -isystem options instead of -I. See Invocation, for a detailed description of these options, as well as others that are less generally useful.

If you specify other options on the command line, such as -I, that affect where the preprocessor searches for header files, the directory list printed by the -v option reflects the actual search path used by the preprocessor.

Note that you can also prevent the preprocessor from searching any of the default system header directories with the -nostdinc option. This is useful when you are compiling an operating system kernel or some other program that does not use the standard C library facilities, or the standard C library itself.

除此之外，我们还可以通过相应的环境变量来指定头文件的搜索路径：

export C_INCLUDE_PATH=XXXX:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=XXX:$CPLUS_INCLUDE_PATH

可以将以上代码添加到/etc/profile末尾。


 

[参看]:

    pkg-config官网

    ldconfig命令

    PKG_CONFIG_PATH变量 与 ld.so.conf 文件

    Linux下运行时链接库的路径顺序

    gcc使用-Wl,-rpath解决so库版本冲突

    Linux 中C/C++ search path(头文件搜索路径)
————————————————
版权声明：本文为CSDN博主「几米夜空」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/chen_jianjian/article/details/123890413



configure 使用pkg-config
export CFLAGS=`pkg-config --cflags freetype2`
export CXXFLAGS=`pkg-config --cflags freetype2`

export LDFLAGS=`pkg-config --libs freetype2`



