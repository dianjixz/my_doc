
# gun的一些工具记录

### Autotools
GNU Autotools 一般指的是3个 GNU 工具包：Autoconf，Automake 和 Libtool
该工具重要用于生成自动化构建脚本。
在写快跨平台的程序时可能需要用#define来区分不同平台的接口。而configure用来调整这些接口。configure 脚本一般会先检查目前的环境，然后生成一个config.h 文件 (里面带了各种各样的#define) ，同时会生成一个 针对当前平台的 Makefile 文件，之后，make 命令就会使用到这个 Makefile文件。  
另外，GNU的 build 系统还有一些"乱七八糟"的功能，用户在使用 configure 这个脚本的时候，可能会使用到这些功能 (最常见的就是用 --prefix 来指定安装路径，用configure --help来查看说明等等)。

实例：[https://blog.csdn.net/weixin_33985507/article/details/85636537](https://blog.csdn.net/weixin_33985507/article/details/85636537)

auto使用方法：

~~~ bash
nihao@nihao-XiaoXin:~/work/make/two$ls
nihao.c
nihao@nihao-XiaoXin:~/work/make/two$ autoscan 
nihao@nihao-XiaoXin:~/work/make/two$ ls
autoscan.log  configure.scan  nihao.c
nihao@nihao-XiaoXin:~/work/make/two$ mv configure.scan configure.ac
nihao@nihao-XiaoXin:~/work/make/two$ ls
autoscan.log  configure.ac  nihao.c
nihao@nihao-XiaoXin:~/work/make/two$ vi configure.ac
nihao@nihao-XiaoXin:~/work/make/two$ cat configure.ac 
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nihao], [0.0.1], [https://dianjixz.github.io])

AC_CONFIG_SRCDIR([nihao.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AM_INIT_AUTOMAKE(nihao,0.0.1)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
nihao@nihao-XiaoXin:~/work/make/two$ ls
aclocal.m4  autom4te.cache  autoscan.log  configure.ac  nihao.c
nihao@nihao-XiaoXin:~/work/make/two$ vi Makefile.am
nihao@nihao-XiaoXin:~/work/make/two$ cat Makefile.am 
bin_PROGRAMS = nihao
nihao_SOURCES = nihao.c
nihao@nihao-XiaoXin:~/work/make/two$ autoheader 
nihao@nihao-XiaoXin:~/work/make/two$ ls
aclocal.m4      autoscan.log  configure.ac  nihao.c
autom4te.cache  config.h.in   Makefile.am
nihao@nihao-XiaoXin:~/work/make/two$ touch NEWS README AUTHORS ChangeLog
nihao@nihao-XiaoXin:~/work/make/two$ ls
aclocal.m4  autom4te.cache  ChangeLog    configure.ac  NEWS     README
AUTHORS     autoscan.log    config.h.in  Makefile.am   nihao.c
nihao@nihao-XiaoXin:~/work/make/two$ automake -a
configure.ac:20: warning: AM_INIT_AUTOMAKE: two- and three-arguments forms are deprecated.  For more info, see:
configure.ac:20: https://www.gnu.org/software/automake/manual/automake.html#Modernize-AM_005fINIT_005fAUTOMAKE-invocation
configure.ac:11: installing './compile'
configure.ac:20: installing './install-sh'
configure.ac:20: installing './missing'
Makefile.am: installing './INSTALL'
Makefile.am: installing './COPYING' using GNU General Public License v3 file
Makefile.am:     Consider adding the COPYING file to the version control system
Makefile.am:     for your code, to avoid questions about which license your project uses
Makefile.am: installing './depcomp'
nihao@nihao-XiaoXin:~/work/make/two$ ls
aclocal.m4      autoscan.log  config.h.in   depcomp     Makefile.am  NEWS
AUTHORS         ChangeLog     configure.ac  INSTALL     Makefile.in  nihao.c
autom4te.cache  compile       COPYING       install-sh  missing      README
nihao@nihao-XiaoXin:~/work/make/two$ autoconf 
nihao@nihao-XiaoXin:~/work/make/two$ ls
aclocal.m4      ChangeLog    configure.ac  install-sh   NEWS
AUTHORS         compile      COPYING       Makefile.am  nihao.c
autom4te.cache  config.h.in  depcomp       Makefile.in  README
autoscan.log    configure    INSTALL       missing
nihao@nihao-XiaoXin:~/work/make/two$ ./configure 
nihao@nihao-XiaoXin:~/work/make/two$ make






~~~











- Autoconf 用于生成 configure
官方文档链接[http://www.gnu.org/software/autoconf/](http://www.gnu.org/software/automake/)
configure 脚本一般会做以下工作：
1、检查用户的环境是否满足 GNU 程序的 Build 环境 (Autoconf 提供了很多macro来做各种各样的检查)
2、替换模版文件，生成最后 Build 工程所需要的 Makefile，config.h 等文件 (其中 Makefile 的模版文件是 Makefile.in, 而 config.h 的模版文件是 config.h.in)

- Automake 
  官方文档链接[http://www.gnu.org/software/automake/](http://www.gnu.org/software/automake/)

  Automake 的目的就是可以帮助程序员自动生成模版文件 Makefile.in 。需要提供一个简单的 Makefile.am 文件来描述依赖关系就行了。

  例如, 只要在 Makefile.am 中写两行, :

  ~~~ makefile
  bin_PROGRAMS = hello (目标)
  hello_SOURCES = main.c (所依赖的源文件)
  ~~~

  然后 Automake 就可以根据 Makefile.am 的描述自动生成 Makefile.in 模版文件

  ## 



- Libtool

