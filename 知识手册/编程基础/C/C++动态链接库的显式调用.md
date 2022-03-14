C/C++动态链接库的显式调用（动态调用）

C语言中文网推出辅导班啦，包括「C语言辅导班、C++辅导班、算法/数据结构辅导班」，全部都是一对一教学：一对一辅导 + 一对一答疑 + 布置作业 + 项目实践 + 永久学习。QQ在线，随时响应！
《动态链接库的创建和使用》一节给读者详细介绍了如何创建一个动态链接库，以及如何使用它完成程序的链接和运行。但需要指明的是，这只是动态链接库的其中一种用法，本节将讲解它的另外一种用法。

总的来讲，动态链接库的调用方式有 2 种，分别是：

    隐式调用（静态调用）：将动态链接库和其它源程序文件（或者目标文件）一起参与链接；
    显式调用（动态调用）：手动调用动态链接库中包含的资源，同时用完后要手动将资源释放。


显式调用动态链接库的过程，类似于使用 malloc() 和 free()（C++ 中使用 new 和 delete）管理动态内存空间，需要时就申请，不需要时就将占用的资源释放。由此可见，显式调用动态链接库对内存的使用更加合理。

    显式调用动态链接库，更常应用于一些大型项目中。

那么，C/C++ 程序中如何实现显式地调用动态链接库呢？接下来就给大家做详细地讲解。

和隐式调用动态链接库不同，在 C/C++ 程序中显示调用动态链接库时，无需引入和动态链接库相关的头文件。但与此同时，程序中需要引入另一个头文件，即 <dlfcn.h> 头文件，因为要显式调用动态链接库，需要使用该头文件提供的一些函数。
~~~ c
    #include <dlfcn.h>
~~~
该头文件中，以下几个函数是显式调用动态链接库时常用的：

1) 类似于读写文件前必须先打开文件，要想显示调用某个动态链接库提供的资源，首先要做的就是打开该库文件。

    打开库文件，其本质就是将库文件装载到内存中，为后续使用做准备。

打开动态库文件，需要借助 dlopen() 函数，其语法格式为：
~~~ c
void *dlopen (const char *filename, int flag);
~~~
其中，filename 参数用于表明目标库文件的存储位置和库名；flag 参数的值有 2 种：

    RTLD_NOW：将库文件中所有的资源都载入内存；
    RTLD_LAZY：暂时不降库文件中的资源载入内存，使用时才载入。

    值得一提的是，对于 filename 参数，如果用户提供的是以 / 开头，即以绝对路径表示的文件名，则函数会前往该路径下查找库文件；反之，如果用户仅提供文件名，则该函数会依次前往 LD_LIBRARY_PATH 环境变量指定的目录、/etc/ld.so.cache 文件中指定的目录、/usr/lib、/usr/lib64、/lib、/lib64 等默认搜索路径中查找。

2) 借助 dlsym() 函数可以获得指定函数在内存中的位置，其语法格式为：
~~~ c
void *dlsym(void *handle, char *symbol);
~~~
其中，hanle 参数表示指向已打开库文件的指针；symbol 参数用于指定目标函数的函数名。

如果 dlsym() 函数成功找到指定函数，会返回一个指向该函数的指针；反之如果查找失败，函数会返回 NULL。
 
3) 和 dlopen() 相对地，借助 dlclose() 函数可以关闭已打开的动态链接库。该函数的语法格式如下：
~~~ c
int dlclose (void *handle);
~~~
其中，handle 表示已打开的库文件指针。当函数返回 0 时，表示函数操作成功；反之，函数执行失败。

    注意，调用 dlclose() 函数并不一定会将目标库彻底释放，它只会是目标库的引用计数减 1，当引用计数减为 0 时，库文件所占用的资源才会被彻底释放。


4) 借助 dlerror() 函数，我们可以获得最近一次 dlopen()、dlsym() 或者 dlclose() 函数操作失败的错误信息。该函数的语法格式如下：
~~~ c
const char *dlerror(void);
~~~
可以看到，该函数不需要传递任何参数。同时，如果函数返回 NULL，则表明最近一次操作执行成功。

下面通过一个 C 语言项目实例，给大家演示显式调用动态链接库的具体实现过程。这里仍引用前面章节中创建的 demo 项目，其项目结构如下：
~~~
 demo项目
   ├─ headers
   │     └─ test.h
   └─ sources
          ├─ add.c
          ├─ sub.c
          ├─ div.c
          └─ main.c
~~~
项目中各个文件包含的代码如下：
~~~ bash
[root@bogon demo]# cat test.h
#ifndef __TEST_H_
#define __TEST_H_

int add(int a,int b);
int sub(int a,int b);
int div(int a,int b);

#endif
[root@bogon demo]# cat add.c
#include "test.h"
int add(int a,int b)
{
    return a + b;
}
[root@bogon demo]# cat sub.c
#include "test.h"
int sub(int a,int b)
{
    return a - b;
}
[root@bogon demo]# cat div.c
#include "test.h"
int div(int a,int b)
{
    return a / b;
}
~~~
以上这些文件中的代码，和之前一样，没有任何变化。对于 add.c、sub.c 和 div.c 这 3 个源文件，我们可以将它们打包生成一个动态链接库：
~~~ bash
[root@bogon demo]# gcc -fpic -shared add.c sub.c div.c -I ../headers -o libmymath.so
[root@bogon demo]# ls
add.c  div.c  libmymath.so  main.c  sub.c  test.h
~~~
接下来重点分析 main.c 主程序文件的代码：
~~~ bash
[root@bogon demo]# cat main.c
#include <stdio.h>
#include <dlfcn.h>
int main()
{
    int m,n;
    //打开库文件
    void* handler = dlopen("libmymath.so",RTLD_LAZY);
    if(dlerror() != NULL){
        printf("%s",dlerror());
    }
   
    //获取库文件中的 add() 函数
    int(*add)(int,int)=dlsym(handler,"add");
    if(dlerror()!=NULL){
        printf("%s",dlerror());
    }
  
    //获取库文件中的 sub() 函数
    int(*sub)(int,int)=dlsym(handler,"sub");
    if(dlerror()!=NULL){
        printf("%s",dlerror());
    }

    //获取库文件中的 div() 函数
    int(*div)(int,int)=dlsym(handler,"div");
    if(dlerror()!=NULL){
        printf("%s",dlerror());
    }
    //使用库文件中的函数实现相关功能
    printf("Input two numbers: ");
    scanf("%d %d", &m, &n);
    printf("%d+%d=%d\n", m, n, add(m, n));
    printf("%d-%d=%d\n", m, n, sub(m, n));
    printf("%d÷%d=%d\n", m, n, div(m, n));
    //关闭库文件
    dlclose(handler);
    return 0;
}
~~~
首先，该程序中并没有引入 test.h 头文件，因为对于显式调用动态链接库来说，并不需要引入它。与此同时，在使用库文件中的相关函数之前，我们需要先调用 dlopen() 函数打开库文件，然后才能通过 dlsym() 函数找到相关的函数。另外，最后不要忘记调用 dlclose() 函数关闭库文件。

通过执行如下指令，即可生成相应的可执行文件：
~~~ bash
[root@bogon demo]# gcc main.c -ldl -o main.exe
[root@bogon demo]# ls
add.c  div.c  libmymath.so  main.c  main.exe  sub.c  test.h
~~~
注意，这里需要添加 -ldl 选项（该可执行程序需要 libdl.so 动态库的支持）。运行 main.exe，其执行结果为：
~~~ bash
[root@bogon demo]# ./main.exe
Input two numbers: 10 2
10+2=12
10-2=8
10÷2=5

~~~