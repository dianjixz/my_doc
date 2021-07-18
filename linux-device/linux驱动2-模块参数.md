

## 给模块传递参数

当我们加载一个模块到Linux内核的时候，Linux内核允许向这个模块传递一些参数。这样设计的好处就是，让我们的模块操作起来更灵活，我们可以通过给它传递不同的参数来完成不同的功能。

例如：我们写一个模块程序，来完成硬件中断的操作。在Linux操作系统中，每个中断都有一个中断号。如果我们在模块里面将中断号写死，那我们的模块只能响应特定的中断了。如果我们把中断号作为参数传递给我们的模块，那么我们的模块就可以完成对不同的中断进行操作。

那怎么向模块传递参数呢？很简单，Linux内核都给我们做好了，我们只需调用相应的接口就可以了。

在模块里面，声明一个变量（全局变量）,用来接收用户加载模块时传递的参数

文件位置如下：

> linux-3.14\include\linux\ Moduleparam.h

**函数原型：**

~~~ c
module_param(name,type,perm)
参数：
  @name用来接收参数的变量名
  @type参数的数据类型
    bool   :布尔类型
    invbool:颠倒了值的bool类型;
    charp  :字符指针类型,内存为用户提供的字符串分配;
    int    :整型
    long   :长整型
    short  :短整型
    uint   :无符号整型
    ulong  :无符号长整型
    ushort :无符号短整型
  @perm 指定参数访问权限。
~~~

每个模块的参数，最后都会表现在sysfs文件系统中，也就是说最后会在系统/sys/module/模块名字/parameters/路径下看到以参数名命名的文件。

 这个文件的权限就是这里指定的权限。如果perm的值为0,则在sysfs文件系统中不会生成参数对应的文件。

**典型使用案列：**

（1）我们可以在模块（test.ko)里面写如下代码，接收用户加载模块时传递的参数

~~~ c
static unsigned int var=0;
module_param(var,uint,0400);
~~~

在加载模块的时候，传递参数：

~~~ bash
insmod test.ko var=100
~~~

最后模块里面的全局变量var的值就为100了。

（2）如果我要传递一个字符串到模块里面，该怎么操作呢？

~~~ c
static char *string;
module_param(string,charp,0400);
~~~

在加载模块的时候，传递参数：

~~~ bash
insmod test.ko string="yikoulinux";
~~~

有人可能会问，这段代码是不是有bug,因为你的string指针是一个野指针。

其实内核会自动给用户传递的字符串分配空间的，然后用string指针保存字符串所在内存的首地址。

（3）让模块内部变量的名字和加载模块时传递的参数名不同

**函数原型：**

~~~ c
module_param_named(name_out,name_in,type,perm);
参数：
  @name_out在加载模块时，参数的名字
  @name_in模块内部变量的名字
  @type 参数类型
  @perm 访问权限
~~~

典型使用案列：

~~~ c
static int var =0;
module_param_named(var_out,var,int,0400);
~~~

在加载模块的时候，传递参数：

~~~ bash
insmod test.ko var_out=100
~~~

var_out就是模块变量var在外部的名字，此时var的值为100

(4)加载模块的时候，传递字符串到模块的一个全局字符数组里面

函数原型：

~~~ c
module_param_string(name,string,len,perm);
参数：
@name在加载模块时，参数的名字
@string模块内部字符数组的名字
@len 模块内部字符数组的大小
@perm 访问权限
~~~

**典型使用案列：**

~~~ c
static int buffer[LEN];
module_param_string(buffer_out,buffer,LEN,0400);
~~~

在加载模块的时候，传递参数：

~~~ bash
insmod test.ko buffer_out="hello word"
~~~

加载模块的时候，内核直接把"hello word"字符串口拷贝到buffer数组中。

(5)加载模块的时候，传递参数到模块的数组中

函数原型：

~~~ c
module_param_arry(name,type,num_point,perm);
参数：
@name 模块的数组名，也是外部指定的参数名
@type模块数组的数据类型
@num_point用来获取用户在加载模块时传递的参数个数，NULL:不关心用户传递的参数个数
@perm 访问权限
~~~

典型使用案列：

~~~ c
static int my_arry[3];
int num;
module_param_arry(my_arry,int,&num,0400);
~~~

在加载模块的时候，传递参数：(多个参数以”，“隔开）

~~~ bash
insmod test.ko my_arr=1,2,3
~~~

注意：以上接口在调用的时候，perm指定的权限不能让普通用户具有写权限，否则编译会报错。

(6)给模块里面每个接收用户参数的变量指定一个描述信息函数原型：

```c
MODULE_PARM_DESC(name,describe);
参数
  @name 变量名
  @describe描述信息的字符串
```

#### 案例演示

说了这么多，下面我们一起来实现一个完整的代码吧：

~~~ c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static int var1 = 0;
module_param(var1,int,0644);
MODULE_PARM_DESC(var1, "Get value from user.\n");

static int var2 = 0;
module_param_named(var2_out,var2,int,0644);
MODULE_PARM_DESC(var2, "Test var2 named var2_out.\n");

static char *string = NULL;
module_param(string,charp,0444);
MODULE_PARM_DESC(string, "Test module param string.\n");

static char buffer[10];
module_param_string(buffer,buffer,sizeof(buffer),0644);
MODULE_PARM_DESC(buffer, "Test module param string.\n");

static int myarray[3];
int num;
module_param_array(myarray,int,&num,0444);
MODULE_PARM_DESC(myarray, "Test module param array.\n");

static int __init hello_init(void)
{
  int i = 0;
  printk("-----------------------------\n");
  printk("var1     : %d\n",var1);
  printk("var2     : %d\n",var2);
  printk("string     : %s\n",string);
  printk("buffer     : %s\n",buffer);

  for(i=0;i<num;i++)
  {
    printk("myarray[%d] : %d\n",i,myarray[i]);
  }
  printk("-----------------------------\n");  
  return 0;
}
static void __exit hello_exit(void)
{
  printk("hello_exit \n");
  return;
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("yikoulinux");
module_init(hello_init);
module_exit(hello_exit);
~~~

命令如下：

~~~ bash
insmod hello.ko var1=100 var2_out=200 string="yikoulinux" buffer="yikoupeng" myarray=100,200,300
~~~

测试结果如下：



同时在目录/sys/module/*hello*/parameters下回生成对应的文件节点，文件权限与代码中定义的是一一对应的：

所有节点的内容可以用cat命令查看

切换到管理员模式，可以通过echo命令修改有W权限的文件：

## 模块符号导出

**(1)什么是符号？**

 这里的符号主要指的是全局变量和函数。

**(2)为什么要导出符号？**

 Linux内核采用的是以模块化形式管理内核代码。内核中的每个模块相互之间是相互独立的，也就是说A模块的全局变量和函数，B模块是无法访问的。

有些时候，我们写一些模块代码的时候，发现部分函数功能别人已经实现了，此时我们就想如果我们可以调用他们已经实现好的函数接口就好了。那如何才能做到这点呢？符号导出了，也就是说你可以把你实现的函数接口和全局变量导出，以供其他模块使用。

  在Linux内核的世界里，如果一个模块已经以静态的方式编译进的内核，那么它导出的符号就会出现在全局的内核符号表中。在Ubuntu 14.04系统中，Linux内核的全局符号表在以下文件中存放：

> /usr/src/linux-headers-3.2.0-29-generic-pae/Module.symvers

如果打开这个文件，可以发现里面的内容就是：

Addr------a符号名------a模块名------a导出符号的宏

(3)如何导出符号？

Linux内核给我们提供了两个宏：

~~~ c
EXPORT_SYMBOL(name);
EXPORT_SYMBOL_GPL(name);
~~~

上面宏定义的任一个使得给定的符号在模块外可用.GPL版本的宏定义只能使符号对GPL许可的模块可用.符号必须在模块文件的全局部分输出，在任何函数之外，因为宏定义扩展成一个特殊用途的并被期望是全局存取的变量的声明。

(4)模块编译时，如何寻找使用的符号？

a.在本模块中符号表中，寻找符号（函数或变量实现）

b.在内核全局符号表中寻找

c.在模块目录下的Module.symvers文件中寻找

如果在这三个地方都没有找到，则编译保存

#### 案例演示

本例实现功能：模块A导出全局变量global_var和函数show两个符号供模块B使用。

模块A代码：

~~~ c
#include <linux/init.h>
#include <linux/module.h>

static int global_var = 100;
static void show(void)
{
  printk("show():  global_var =%d \n",global_var);
}
static int hello_init(void)
{
  printk("module   b :global_var=%d\n",global_var);
  return   0;
}
static void hello_exit(void)
{
  printk("hello_exit   \n");
  return;
}
EXPORT_SYMBOL(global_var);
EXPORT_SYMBOL(show);
MODULE_AUTHOR("PENG");
MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
~~~

模块B

~~~ c
#include <linux/init.h>
#include <linux/module.h>

extern int global_var;
extern    void show(void);
static int hello_init(void)
{
  printk("module   a: global_var= %d\n",global_var);
  show();
  return   0;
}
static void hello_exit(void)
{
  printk("hello_exit   \n");
  return;
}
MODULE_AUTHOR("PENG");
MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
~~~

调试步骤：


1、编译模块A,然后加载模块A,在模块A编译好后，在它的当前目录会看到一个Module.symvers文件，这里存放的就是我们模块A导出的符号。

2、将模块A编译生成的Module.symvers文件拷贝到模块B目录下，然后编译模块B,加载模块B。

3、通过dmesg查看模块打印的信息。

打印信息如下：

