# scons 简单入门

[![img](https://cdn2.jianshu.io/assets/default_avatar/14-0651acff782e7a18653d7530d6b27661.jpg)](https://www.jianshu.com/u/7eb4d5b50d1b)

[辰辰沉沉沉](https://www.jianshu.com/u/7eb4d5b50d1b)

0.5532019.01.20 12:58:44字数 1,201阅读 18,269

### 简单入门

#### hello world

scons由Sconstruct 作为入口，控制如何进行编译操作。Sconstruct 本身是一个python文件，故需要遵循python的语法，以及能使用一些python的方法。(如我们可以用print 来debug）

这有一段很简单的hello.cpp

```c
#include <iostream>

int main() {
    std::cout << "hello world" << std::endl;
}
```

以及一个很简单的Sconstruct

```python
Program("hello.cpp")
```

Program是Scons中的一个编译方法(builder_method)， 告诉Scons 我们想要把hello.cpp 编译成一个可执行文件。

保证Sconstruct 和hello.cpp 在同一个文件夹下，执行scons，就可以完成编译，生成可执行文件hello。

```css
scons: Reading SConscript files ...
scons: done reading SConscript files.
scons: Building targets ...
g++ -o hello.o -c hello.cpp
g++ -o hello hello.o
scons: done building targets.
```

可以看到，我们只指定了一个cpp文件，scons会默认给可执行文件一个名字，以及完成.o文件的生成，非常智能。当然，我们也能指定输出文件的名字，`Program("target_name", hello.cpp")`

另外，除了Program ，还有其他很多builder_method， 如`Object`, `SharedLibrary`，`StaticLibrary`，`LoadableModule`，`StaticObject`，`CFile`

#### 编译多个文件

编译多个文件非常简单

- 直接使用list `Program(["a.cpp", "b.cpp", "c.cpp"])`即可。

- 也可以使用Glob方法

  

```bash
source = Glob("src/*.cpp")
print source # python语法，可以打印出来debug
Program(["hello.cpp"] + source) 
```

- 用这种方式把src下的文件都加进来
- `Program(Split("a.cpp, b.cpp c.cpp")` Split 也是SCons提供的一个方法，顾名思义了。

#### 编译一个库

把Program改为 Library (或者StaticLibrary，这两者是一样的)即可。

```bash
Library("t", Glob("src/*.cpp"))
```

这样就能得到一个静态库了。 如果如要一个动态库，则可以使用SharedLibrary。

#### 链接一个库

上边我们学会如何编译一个库了，那么如何链接呢？也很简单，加个参数即可

```bash
source = Glob("src/*.cpp")
SharedLibrary("t", source)
Program(["hello.cpp"], LIBS=["t"], LIBPATH=".")
```

Program 可以理解为python  的一个方法，很多参数都有默认值，我们要做的只用覆盖其默认值即可。如上，我们指定引入LIBS。同样的，LIBS参数也可以是个str，LIBPATH也可以是个list，放上所有要查找的路径，如['/usr/lib', '/usr/local/lib']，这里就不赘述了。

### 略微高级一点的功能

#### 判断是否需要重新编译

SCons很智能，只会编译需要编译的内容。比如我刚执行完scons，再次执行，则会提示`scons: . is up to date.`。 那么他是如何做到的呢？也不复杂，依赖一个Decider的方法，以及一个`.sconsign.dblite`文件。

默认情况下，如果文件的md5值改变了，才会重新编译。每次编译，SCons都会把md5存起来，再次执行时，如果md5没变，则不需要rebuild。

如果我们不希望使用md5，而是使用文件修改时间呢？很简单，增加`Decider('timestamp-newer')` （默认情况下为md5）。也可以使用`'MD5-timestamp`，则他们一起变了才会rebuild。

我们前面也说到，Decider是一个方法，那很显然，我们也可以自己写个decider方法，详细的可以看[scons-user.pdf](https://scons.org/doc/production/PDF/scons-user.pdf) 6.1.5，这里不细写了。。

#### env

env分为三种

- external enviroment  保存在os.environ 中，和scons其实没太大关系，保存着一些系统定义的环境变量，如PATH之类

- construction environment

  这个比较重要，我们可能希望不同的源文件使用不同的编译参数，这时候我们就能设置不同的编译环境，指定哪个文件用哪种编译。construction environment 中保存了构建相关的一些参数。

  使用方式如下

  

```php
env1 = Environment(CXX = 'gcc') # 创建一个env
print env1["CXX"] # 获取参数
env2 = env1.Clone(CXX = 'g++')    # 复制一个env
env1.Replace(CXX = 'g++') # 修改参数
env1["CXX"] = "clang++"   #再修改参数

env1.MergeFlags("-g") #增加一个flag
```

就可以构建出一个env，除了CXX 被修改以外，其他参数均不变。另外，我们也可以像一个dict一样，去获取env中的内容。

execution enviroment

这其实就是construction environment 中的一个变量ENV

```bash
env = Environment()
print env["ENV"]

env2 = Environment(ENV=os.environ)
env3 = Environment(ENV = {"PATH" : os.environ["PATH"]})
```

- 几个简单的例子，一看就明白了

#### 命令行输入的参数

比如我们想通过一个debug字段来控制是否开启debug模式，怎么做呢？可以通过ARGUMENTS

```dart
env = Environment()
debug = ARGUMENTS.get("debug", 0)
if int(debug):
    print "in debug mode"
```

`scons debug=1`就可以了。

### 小结

上面只是我在看的时候做的一个小结，详细的可以看(文档)[https://scons.org/doc/production/PDF/scons-user.pdf]，很多东西都没写，在日常工作中可以一点一点去体会。





linux下使用Scons构建自己的项目（来源：http://blog.csdn.net/shitangdejiaozi/article/details/51499591）
一、简介：SCons 是一个开放源代码、以 Python 语言编写的下一代的程序建造工具，替代Makefile的功能。
二、安装：sudo apt-get install scons
三、用法
1、SConstruct 文件是scons用来控制编译的文件，类似MakeFile,是一个Python脚本。编译顶层脚本文件名为           SConstruct，输入scons命令后，自动调用该脚本。
2、运行：scons 
scons -c 就是清理你创建的文件，就是make clean 
scons -Q 就是省略自带的一堆废话，只显示编译过程
3、支持的编译类型
Program: 编译成可执行程序
Object :只编译成目标文件，使用这种类型，编译结束后，只会产生目标文件，.o文件。
Library :编译成库文件，scons默认编译的库是指静态链接库
StaticLibrary :显式的编译成静态链接库，与上面的Library效果一样
SharedLibrary :编译成动态库
4、语法
Program('hello',['file.c','file2.c']), //生成可执行文件hello
Program('hello',Glob('*.c')),//使用Glob编译所有匹配的文件
Program('hello',Split("main.c file1.c file2.c")),//使用Split，以空白字符为分割符
Object(‘main’, ’hello.c’)：编译hello.c为目标文件，文件名为main.o
scons的配置文件Sconscript，通过Sconscript()函数导入配置文件。

5、关键字
CCFLAGS 指定编译选项 
LINKFLAGS 指定链接选项 
CPPDEFINES指定预编译器 
LIBS 指定所需要链接的库文件 
LIBPATH 指定库文件（lib)的搜索路径 
CPPPATH 指定[.h,.c,.cpp]等文件的搜素路径
source：指定源文件名
target：指定目标文件名。如：
Program(source = Split(‘hello.c func1.c func2.c’), target = ‘main’)

Program('hello',['hellosons.c','file.c'], 
 		  LIBS = 'pthread', 
    	  LIBPATH = ['/usr/lib','/usr/local/lib'],
     	CCFLAGS = ['-g ','-Wall'],
     	CPPPATH = ['/usr/include','hello/src'])
6、使用Environments
（1）用户的环境变量，他们可以通过os.environ获取
（2）Construction Environment
env = Environment(CC = ‘arm-linux-gcc’) //创建，（name,value）集合
env.Program(‘hello.c’)
for item in env.Dictionary(): 
	print '(%s:%s)' %(item,env[item]) //查看内容 
env['cc'] //查看变量 
env.subst('$CC') //查看变量，如果subset变量不存在时报告错误。
 	//修改环境变量 
env.Clone //拷贝一个环境变量 
env.Replace //替换一个已经存在的环境变量
env.SetDefault //为一个没有被定义的变量设置默认值 
env.Append(CCFLAGS = '-g') //为一个已存在的环境变量增加一个值，感觉这个比较常用啊 
env.Prepend(CCLFAGS = ‘-DFIRST’)//追加到开始位置：Prepend()
env.AppendUnique //为一个环境变量添加一个唯一的值 
flags = {'CCFLAGS':'-option -O3 -O1'} env.MergeFlags(flags) //合并环境变量
7、控制目标文件的路径install
test = env.Program('test.cpp') 
env.Install('bin',test) //放入到bin目录
//将目标放到其他目录下，并修改名字 
test = env.Program('test.cpp') 
env.InstallAs('bin/testapp',test)  //执行文件改为testapp

8、一份源码，要构建它的debug版本，又要构建release版本

//需要为不同的版本指定不同的obj名字，否则会冲突

opt = Environment(CCFLAGS ='-O2')

dbg = Environment(CCFLAGS = '-g')

o = opt.Object('foo-opt','foo.c')  //生成foo-opt.o

opt.Program(o)

d = dbg.Object('foo-dbg','foo.c') //生成foo-dbg
.o
dbg.Program(d)























1. 介绍

Scons是一个开放源码、以Python语言编码的自动化构建工具，类似于makefile，但是他没有像makefile那样晦涩难懂的语法，更加友好。此外scons支持跨平台使用，编写好scons脚本，可以在windows和linux下随意切换使用！
2. 安装scons

先安装python3，之后使用python的pip安装工具安装scons，安装过程中的问题大家可以网上查查或者看错误提示，基本都能解决，以下是linux下安装关键指令

sudo apt install python
pip install scons

3. Scons初步使用

  首先记住，scons是以python语言编码的，所以支持python语法！当然没学过python也没关系，会几个指令也能玩
3.1 编译程序

源代码hello.c

int main()
{
printf("Hello, world!\n");
}

同级目录下创建名字为Sconstruct 的文件，内容如下

Program('hello.c')

然后在当前目录命令窗口输入scons即可编译工程

% scons
scons: Reading SConscript files ...
scons: done reading SConscript files.
scons: Building targets ...
cc -o hello.o -c hello.c
cc -o hello hello.o
scons: done building targets.

在不同的系统下，scons会自动识别系统环境，采用对应系统下的指令进行编译，生成对应系统下可执行的文件，如windows下就会生成.exe文件
3.2 生成Object文件

修改Sconstruct 文件如下

Object('hello.c')

之后在当前目录命令窗口输入scons重新编译工程即可
3.3 清除工程

命令：scons -c
4. Sconstruct文件

    Sconstruct文件是一个python脚本，支持python语法，使用#注释
    使用print 打印
    屏蔽非必要的scons输出 scons -Q

5. 简单的编译工程示例
5.1 指定生成目标文件名字

修改Sconstruct文件为：

Program ('new_hello', 'hello.c')

重新编译工程

% scons -Q
cc -o hello.o -c hello.c
cc -o new_hello hello.o

5.2 编译多个源文件

    在工程中往往有多个源文件一起组合生成一个目标文件，scons这样实现，修改Sconstruct文件如下：

Program(['prog.c', 'file1.c', 'file2.c'])

编译工程

% scons -Q
cc -o file1.o -c file1.c
cc -o file2.o -c file2.c
cc -o prog.o -c prog.c
cc -o prog prog.o file1.o file2.o

    加上指定生成文件名，修改Sconstruct文件如下：

Program('program', ['prog.c', 'file1.c', 'file2.c'])

编译工程

% scons -Q
cc -o file1.o -c file1.c
cc -o file2.o -c file2.c
cc -o prog.o -c prog.c
cc -o program prog.o file1.o file2.o

    如果文件比较多，可以采用Glob函数查找符合条件的文件制作一个文件列表，同时还支持使用shell模式匹配字符* ? [abc] 或 [!abc] ，修改Sconstruct如下：

Program('program', Glob('*.c'))

5.3 指定单个文件和指定多个文件

    指定单个文件
    a) 采用字串方式：Program('hello', 'hello.c')
    b) 采用列表方式Program('hello', ['hello.c'])
    指定多个文件Program('hello', ['file1.c', 'file2.c'])
    尽管python支持采用字符串或者列表指定文件，但是注意不支持混合使用

common_sources = ['file1.c', 'file2.c']# 以下操作错误，因为采用了 字符串+列表 方式
Program('program1', common_sources + 'program1.c')# 以下操作正确，因为采用的是 列表+列表 方式
Program('program2', common_sources + ['program2.c'])

5.4 采用变量让文件列表方便阅读

对源文件使用python列表的一个缺点是，每个文件必须使用引号括起来，当文件名列表非常长的时候，这可能难以阅读，scons和python提供了许多方法使得Sconstruct易于阅读，scons提供了spilt函数，该函数受引用括号括起来的文件名列表，名称用空格或其他空白分隔符分隔，并将其转化为一个单独的文件名列表

Program('program', Split('main.c file1.c file2.c'))

加上变量

src_files = Split('main.c file1.c file2.c')
Program('program', src_files)

加上换行，方便编写

src_files = Split("""	main.cfile1.cfile2.c""")
Program('program', src_files)

注意这里可以使用"""或者 ""或者 " 都可以
5.5 采用关键字

scons允许你对于源文件和输出文件使用pyhton关键字target和source定义，采用关键字之后传入参数的位置顺序可以随便放置

src_files = Split('main.c file1.c file2.c')
Program(target='program', source=src_files)

src_files = Split('main.c file1.c file2.c')
Program(source=src_files, target='program')

5.6 同时编译多个程序

Program('foo.c')
Program('bar', ['bar1.c', 'bar2.c'])

输出

% scons -Q
cc -o bar1.o -c bar1.c
cc -o bar2.o -c bar2.c
cc -o bar bar1.o bar2.o
cc -o foo.o -c foo.c
cc -o foo foo.o

5.7 编译多个程序时的共享文件

如果不处理时会是这样

Program(Split('foo.c common1.c common2.c'))
Program('bar', Split('bar1.c bar2.c common1.c common2.c'))

这样有一个缺点，当需要增加公共文件时，会维护困难，容易遗漏，可以使用下面此方式

common = ['common1.c', 'common2.c']
foo_files = ['foo.c'] + common
bar_files = ['bar1.c', 'bar2.c'] + common
Program('foo', foo_files)
Program('bar', bar_files)

6. 建立和链接库文件
6.1 生成库文件

Library('foo', ['f1.c', 'f2.c', 'f3.c'])

输出

% scons -Q
cc -o f1.o -c f1.c
cc -o f2.o -c f2.c
cc -o f3.o -c f3.c
ar rc libfoo.a f1.o f2.o f3.o
ranlib libfoo.a

默认生成静态库文件，同时scons会根据不同的系统生成对应的库文件，此外scons还会自动给库文件增加前缀和后缀
6.1.1 使用源文件+Object文件生成库文件

Library('foo', ['f1.c', 'f2.o', 'f3.c', 'f4.o'])

输出

% scons -Q
cc -o f1.o -c f1.c
cc -o f3.o -c f3.c
ar rc libfoo.a f1.o f2.o f3.o f4.o
ranlib libfoo.a

6.1.2 指定生成静态库文件StaticLibrary

StaticLibrary('foo', ['f1.c', 'f2.c', 'f3.c'])

6.1.3 指定生成动态库文件SharedLibrary

SharedLibrary('foo', ['f1.c', 'f2.c', 'f3.c'])

6.2 链接库文件

使用$LIBS指定库文件，使用$LIBPATH指定库文件路径

Library('foo', ['f1.c', 'f2.c', 'f3.c'])
Program('prog.c', LIBS=['foo', 'bar'], LIBPATH='.')

注意，在使用库文件的时候，不用为其添加前缀和后缀，如lib .a .so，scons会根据当前的系统自动添加前缀和后缀
6.3 查找库文件：使用$LIBPATH构造变量

链接器默认只会从系统定义的目录中去查找库，scons知道如何在使用$LIBPAH构造变量指定的目录中查找库，$LIBPATH包含一个目录名列表，如下所示：

Program('prog.c', LIBS = 'm',LIBPATH = ['/usr/lib', '/usr/local/lib'])

使用python列表是首选，因为它可以跨系统移植，或者你可以把目录名放在一个字符串中，用系统特定的分隔符分隔，注意分隔符，linux里面使用:，windows里面使用;
linux:LIBPATH = '/usr/lib:/usr/local/lib'
windows:LIBPATH = 'C:\\lib;D:\\lib'
上述Sconstruct执行如下：

% scons -Q
cc -o prog.o -c prog.c
cc -o prog prog.o -L/usr/lib -L/usr/local/lib -lm


Dump(env)











Scons环境搭建和编译原理概述及嵌入式开发常用模板
https://blog.csdn.net/wenbo13579/article/details/126881034


document
https://scons.org/docversions.html




scons解析
https://blog.csdn.net/andyelvis/category_948141.html



# # CFLAGS      # c 的编译定义
# # CCFLAGS     # CC 的编译选项
# # CPPDEFINES  # -D
# # CPPPATH     # -I
# # ASFLAGS     # 汇编的编译定义
# # LINKFLAGS   # 连接选项
# # LIBPATH     # -L
# # LIBS        # -l



参考：

[如何在Scons中使用交叉编译器？](https://www.codenong.com/23898584/)