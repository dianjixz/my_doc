用gcc编译时序代码时需要给指定函数指定优化等级，不然容易被gcc优化掉时序
// 首先用__attribute__声明函数
int add(int a, int b) __attribute__((optimize("O0")));
// 然后再定义函数，声明和定义必须分开，否则编译错误
int add(int a, int b)
{
}
int sub(int a, int b) __attribute__((optimize("O2")));
int sub(int a, int b)
{
}
————————————————
版权声明：本文为CSDN博主「半斗米」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/zoomdy/article/details/79251021
、gcc中指定优化级别的参数有：-O0、-O1、-O2、-O3、-Og、-Os、-Ofast。


https://zhuanlan.zhihu.com/p/151219726
gcc编译指南


假设main.cpp,hello.h,hello.cpp,其中main.cpp调用了hello类中的方法

1 生成hello.so

  g++ -shared hello.cpp -o libhello.so

2 编译main.cpp,并链接,并指定运行时libhello.so的位置

　　g++ main.cpp -lhello -L./ -Wl,-rpath=./ -o main
　　
makefile标准写法　　
https://blog.csdn.net/xy010902100449/article/details/50328975

下面我们要介绍的函数主要是处理文件名的。每个函数的参数字符串都会被当做一个或是一系列的文件名来对待。
$(dir ) 名称： 取目录函数——dir。功能：从文件名序列中取出目录部分。目录部分是指最后一个反斜杠（“/”）之前的部分。如果没有反斜杠，那么返回“./”。返回：返回文件名序列的目录部分。示例： $(dir src/foo.c hacks)返回值是“src/ ./”。
$(notdir ) 名称： 取文件函数——notdir。功能：从文件名序列中取出非目录部分。非目录部分是指最后一个反斜杠（“/”）之后的部分。返回：返回文件名序列的非目录部分。示例： $(notdir src/foo.c hacks)返回值是“foo.c hacks”。
$(suffix ) 名称： 取后缀函数——suffix。功能：从文件名序列中取出各个文件名的后缀。返回：返回文件名序列的后缀序列，如果文件没有后缀，则返回空字串。示例：$(suffix src/foo.c src-1.0/bar.c hacks)返回值是“.c .c”。
$(basename )名称： 取前缀函数——basename。功能：从文件名序列中取出各个文件名的前缀部分。返回：返回文件名序列的前缀序列，如果文件没有前缀，则返回空字串。示例：$(basename src/foo.c src-1.0/bar.c hacks)返回值是“src/foo src-1.0/bar hacks”。
$(addsuffix , ) 名称： 加后缀函数——addsuffix。功能：把后缀加到中的每个单词后面。返回：返回加过后缀的文件名序列。示例：$(addsuffix .c,foo bar)返回值是“foo.c bar.c”。
$(addprefix , ) 名称： 加前缀函数——addprefix。功能：把前缀加到中的每个单词后面。返回：返回加过前缀的文件名序列。示例：$(addprefix src/,foo bar)返回值是“src/foo src/bar”。
$(join , )名称： 连接函数——join。功能：把中的单词对应地加到的单词后面。如果的单词个数要比的多，那么，中的多出来的单词将保持原样。如果的单词个数要比多，那么，多出来的单词将被复制到中。返回：返回连接过后的字符串。示例：$(join aaa bbb , 111 222 333)返回值是“aaa111 bbb222 333”。 
————————————————
版权声明：本文为CSDN博主「狂奔的乌龟」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/xy010902100449/article/details/50328975


一般我们可以使用“$(wildcard *.c)”来获取工作目录下的所有的.c文件列表。复杂一些用法；可以使用“$(patsubst %.c,%.o,$(wildcard *.c))”，首先使用“wildcard”函数获取工作目录下的.c文件列表；之后将列表中所有文件名的后缀.c替换为.o。这样我们就可以得到在当前目录可生成的.o文件列表。因此在一个目录下可以使用如下内容的Makefile来将工作目录下的所有的.c文件进行编译并最后连接成为一个可执行文件：
————————————————
版权声明：本文为CSDN博主「狂奔的乌龟」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/xy010902100449/article/details/50328975


 $* 　　不包含扩展名的目标文件名称。

$+ 　　所有的依赖文件，以空格分开，并以出现的先后为序，可能包含重复的依赖文件。

$< 　　第一个依赖文件的名称。

$? 　　所有的依赖文件，以空格分开，这些依赖文件的修改日期比目标的创建日期晚。

$@ 　 目标的完整名称。

$^ 　　所有的依赖文件，以空格分开，不包含重复的依赖文件。

$% 如果目标是归档成员，则该变量表示目标的归档成员名称。


1. 安装 python工具 gdbgui
    $pip install gdbgui

2. 命令行启动调试命令

   $gdbgui --host 0.0.0.0 你的可执行程序命令
 这个事情，浏览器会被自动启动，自动链接：127.0.0.1:5000, 后面的就在图形界面下工作吧，如：
    set args
    n
    s
 这些都在浏览器窗口输入，可以看变量的值，设置值，设置端点，等等。
 总之，很爽！





gcc交叉编译时设置了"–sysroot"会产生哪些影响
文章目录
gcc交叉编译时设置了"--sysroot"会产生哪些影响
gcc选项`--sysroot`对`-I`的影响
gcc选项`--sysroot`对`-L`的影响
使用CMAKE进行交叉编译时的建议
1. CMAKE设置`--sysroot`通过`CMAKE_SYSROOT()`配置
2. CMAKE中用来添加头文件搜索路径的宏`INCLUDE_DIRECTORIES() `并不会受到`--sysroot`选项影响
3. 解决链接时无法找到动态库依赖嵌套问题
在做交叉编译的时候，常常涉及到一个gcc编译选项--sysroot,这个选项是用来设置目标平台根目录的。--sysroot选项的官方说明如下

--sysroot=dir
Use dir as the logical root directory for headers and libraries. For example, if the compiler normally searches for headers in /usr/include and libraries in /usr/lib, it instead searches dir/usr/include and dir/usr/lib.

If you use both this option and the -isysroot option, then the --sysroot option applies to libraries, but the -isysroot option applies to header files.

The GNU linker (beginning with version 2.16) has the necessary support for this option. If your linker does not support this option, the header file aspect of --sysroot still works, but the library aspect does not.
从--sysroot的说明可以看出，其会对编译和链接过程中，查找头文件和链接库造成影响。

例如:
原本默认会从/usr/include目录中搜索头文件、从/usr/lib中搜索依赖库，
当设置了--sysroot=dir后则会从dir/usr/include搜索头文件、从dir/usr/lib中搜索依赖库。

未设置--sysroot	设置了--sysroot=dir后
头文件搜索路径	/usr/include	dir/usr/include
依赖库搜索路径	/usr/lib	dir/usr/lib
通过gcc -print-search-dirs查看默认动态库搜索路径

$ ./aarch64-linux-gnu-gcc --sysroot=/home/admin/tx2-rootfs -print-search-dirs |
 grep libraries | sed 's/libraries: =//g' | tr ':' '\n' | xargs readlink -f
/usr/local/lib/linaro-7.3.1/lib/gcc/aarch64-linux-gnu/7.3.1
/usr/local/lib/linaro-7.3.1/lib/gcc/aarch64-linux-gnu
/usr/local/lib/linaro-7.3.1/lib/gcc
/usr/local/lib/linaro-7.3.1/aarch64-linux-gnu/lib/aarch64-linux-gnu
/usr/local/lib/linaro-7.3.1/aarch64-linux-gnu/lib64
/home/admin/tx2-rootfs/lib/aarch64-linux-gnu/7.3.1
/home/admin/tx2-rootfs/lib/aarch64-linux-gnu
/home/admin/tx2-rootfs/lib64
/home/admin/tx2-rootfs/usr/lib/aarch64-linux-gnu/7.3.1
/home/admin/tx2-rootfs/usr/lib/aarch64-linux-gnu
/home/admin/tx2-rootfs/usr/lib64
/usr/local/lib/linaro-7.3.1/aarch64-linux-gnu/lib
/home/admin/tx2-rootfs/lib
/home/admin/tx2-rootfs/usr/lib
gcc选项--sysroot对-I的影响
gcc官方文档关于-I依赖库搜索路径的介绍如下：

-I dir
-iquote dir
-isystem dir
-idirafter dir
Add the directory dir to the list of directories to be searched for header files during preprocessing. If dir begins with ‘=’ or $SYSROOT, then the ‘=’ or $SYSROOT is replaced by the sysroot prefix; see --sysroot and -isysroot.

Directories specified with -iquote apply only to the quote form of the directive, #include "file". Directories specified with -I, -isystem, or -idirafter apply to lookup for both the #include "file" and #include <file> directives.

You can specify any number or combination of these options on the command line to search for header files in several directories. The lookup order is as follows:

    1. For the quote form of the include directive, the directory of the current file is searched first.
    2. For the quote form of the include directive, the directories specified by -iquote options are searched in left-to-right order, as they appear on the command line.
    3. Directories specified with -I options are scanned in left-to-right order.
    4. Directories specified with -isystem options are scanned in left-to-right order.
    5. Standard system directories are scanned.
    6. Directories specified with -idirafter options are scanned in left-to-right order.
You can use -I to override a system header file, substituting your own version, since these directories are searched before the standard system header file directories. However, you should not use this option to add directories that contain vendor-supplied system header files; use -isystem for that.

The -isystem and -idirafter options also mark the directory as a system directory, so that it gets the same special treatment that is applied to the standard system directories.

If a standard system include directory, or a directory specified with -isystem, is also specified with -I, the -I option is ignored. The directory is still searched but as a system directory at its normal position in the system include chain. This is to ensure that GCC’s procedure to fix buggy system headers and the ordering for the #include_next directive are not inadvertently changed. If you really need to change the search order for system directories, use the -nostdinc and/or -isystem options.
经过测试和验证发现，
-Idir编译器只会从dir路径下搜索头文件；
-I=dir或-I$SYSROOT/dir则会受--sysroot影响。

gcc选项--sysroot对-L的影响
gcc官方文档关于-L依赖库搜索路径的介绍比较简单

-Ldir
Add directory dir to the list of directories to be searched for -l.
1
2
经过测试和验证，发现结果和-I选项类似
-Ldir编译器只会从dir路径下搜索依赖库；
-L=dir或-I$SYSROOT/dir则会受--sysroot影响。

使用CMAKE进行交叉编译时的建议
1. CMAKE设置--sysroot通过CMAKE_SYSROOT()配置
CMAKE官方文档推荐在工具链文件中设置，如用来交叉编译tx2的工具链文件tx2.cmake内容如下

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(SYSROOT_PATH  /home/admin/tx2-rootfs)
set(CMAKE_SYSROOT "${SYSROOT_PATH}")
message(STATUS  "Using sysroot path as ${SYSROOT_PATH}")

set(CMAKE_STAGING_PREFIX /home/admin/workspace/staging/)
set(CMAKE_INSTALL_PREFIX /usr/local)

set(TOOLCHAIN_PATH /usr/local/lib/linaro-7.3.1)
set(TOOLCHAIN_HOST ${TOOLCHAIN_PATH}/bin/aarch64-linux-gnu)

set(TOOLCHAIN_CC "${TOOLCHAIN_HOST}-gcc")
set(TOOLCHAIN_CXX "${TOOLCHAIN_HOST}-g++")

set(CMAKE_C_COMPILER ${TOOLCHAIN_CC})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_CXX})

add_link_options("LINKER:-rpath-link,/home/admin/tx2-rootfs/lib/aarch64-linux-gnu:/home/admin/tx2-rootfs/usr/lib/aarch64-linux-gnu")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
在执行cmake的时候指定该工具链文件，假如工具链文件的路径为/home/admin/workspace/tx2.cmake
$ cd /home/admin/workspace/myprj/build
$ cmake -DCMAKE_TOOLCHAIN_FILE=/home/admin/workspace/tx2.cmake ..
或者在你CMakeLists.txt定义project(myprj)前添加include(tx2.cmake)亦可
cmake_minimum_required (VERSION 3.19)
include(tx2.cmake)
project (myprj CXX)

2. CMAKE中用来添加头文件搜索路径的宏INCLUDE_DIRECTORIES()并不会受到--sysroot选项影响
这是因为如果想要--sysroot产生作用，则需要-I=include_dir或-I$SYSROOT/include_dir，而如果通过INCLUDE_DIRECTORIES(“$SYSROOT/include_dir”)则CMAKE将这个$SYSROOT/include_dir识别为一个相对于当前工程的相对路径，并且会在编译阶段将路径补全成相对于当前工程目录的绝对路径,例如当前工程路径为/home/admin/workspace/myprj在编译阶段传递到gcc的参数就变成了-I/home/admin/workspace/myprj/\$SYSROOT/include_dir而不是真正想要的-I$SYSROOT/include_dir

推荐做法
直接通过CMAKE本身的包管理工具find_package(pkg)然后将搜索到的包导出的头文件目录列表添加到include_directories(${pkg_INCLUDE_DIR})。

find_package(pkg)
include_directories(${pkg_INCLUDE_DIR})
注意：示例中查找的包名为pkg，则头文件路径会保存在名为pkg_INCLUDE_DIR的变量中,大小写敏感

3. 解决链接时无法找到动态库依赖嵌套问题
如果所编译的可执行程序所依赖的动态库也需要依赖其他动态库，但连接器没有发现，就会报下面错误。

warning: lib*.so.1, needed by lib*.so, not found (try using -rpath or -rpath-link)
1
解决此类问题的关键点

修复sysroot目录中/usr/lib/aarch64-linux-gnu中错误的动态库符号链接
添加链接选项-Wl,-rpath-link
原理分析

以找不到libm.so.6为例，到/home/admin/tx2-rootfs/usr/lib/aarch64-linux-gnu下查看libm.so

$ ls -la 
lrwxrwxrwx  1 admin admin       32 Jun  5 01:25 libm.so -> /lib/aarch64-linux-gnu/libm.so.6
发现libm.so指向了一个无效的链接，类似的无效符号链接有很多，为了保证以后链接不会出问题，需要：

修复这些符号链接到正确的路径
创建/lib/aarch64-linux-gnu符号链接
可是即使修复了指向无效动态库链接的问题，这个链接警告和报错还是没有实际解决。这时就要借助
-rpath和-rpath-link选项了
在CMAKE官方文档中，说可以通过设置CMAKE_INSTALL_RPATH和CMAKE_BUILD_WITH_INSTALL_RPATH这两个变量来向gcc增加-Wl,rpath,选项

set(CMAKE_INSTALL_RPATH "dir")
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
可是实际操作发现，只能设置-rpath而不能设置-rpath-link。
为了支持-rpath-link，推荐使用add_link_options增加-rpath-link选项

 add_link_options("LINKER:-rpath-link,/home/admin/tx2-rootfs/lib/aarch64-linux-gnu:/home/admin/tx2-rootfs/usr/lib/aarch64-linux-gnu")
1
add_link_option 宏在cmake 3.13版本以后才支持的
注意：由于--sysroot选项不会作用于-rpath-link，所以要填写完整路径名
————————————————
版权声明：本文为CSDN博主「AgileTortoise」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/zvvzxzko2006/article/details/110467542


介绍GCC在编译阶段和程序运行阶段用到的环境变量。
GCC编译时用到的环境变量

GCC编译时用到的变量。
C_INCLUDE_PATH  

GCC编译时查找头文件的目录列表。比如:

echo $C_INCLUDE_PATH
# outputs
## /usr/include:/usr/local/include

CPLUS_INCLUDE_PATH

类似C_INCLUDE_PATH，适用于g++。
LIBRARY_PATH

gcc和g++在编译的链接(link)阶段查找库文件的目录列表，比如:

echo $LIBRARY_PATH
# outputs
## /usr/lib:/usr/lib64:/usr/local/lib:/usr/local/lib64

程序运行时用到的环境变量

程序运行阶段用到的变量。
LD_LIBRARY_PATH
export LD_LIBRARY_PATH=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`


程序运行时查找动态链接库(.so文件)的目录列表。比如:

echo $LD_LIBRARY_PATH
# outputs
## /usr/lib:/usr/lib64:/usr/local/lib:/usr/local/lib64

LD_PRELOAD

在LD_PRELOAD(参考man ld.so的LD_PRELOAD部分)中定义的动态链接库会在其他动态链接库之前被加载，因此会覆盖其他链接库里定义的同名符号（函数变量等），完整的例子可参考test-ld-preload。需要注意的是，在C++中覆盖C函数库中的函数时，应使用extern "C"阻止Name Mangling。

su
LD_PRELOAD=/usr/lib/libtsocks.so apt-get update

注意，LD_PRELOAD无法在sudo命令里使用。1