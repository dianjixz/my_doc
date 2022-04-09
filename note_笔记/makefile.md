 

makefile 里如果用echo进行打印的话必须放在标签之后，否则会出现语法错误。

用以下语句在任意地方打印

1，$(info "info here")，输出时不带行号

 

2， $(warning "CFLAGS = $(CFLAGS)")， 带行号

 

3，$(error "CFLAGS = $(CFLAGS)")，带行号，makefile直接停止执行
————————————————
版权声明：本文为CSDN博主「原十」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/tianyexing2008/article/details/99079114

在[上一章节](https://zhuanlan.zhihu.com/p/362911619)中，我们讲解了makefile中.o文件相对于.c文件的自动推导与变量，掌握了这两项，大大方便makefile的编写维护以及扩展。   

在这一章节中，我们继续深入makefile，来讨论另外两种利器：通配符和模式规则。    

## makefile中的通配符

不论是在shell环境中还是在其他脚本中，通配符通常都是支持的，它提供了一种灵活的方法，以使得用户可以对任何带有共性的对象很方便地进行统一处理。   

makefile语法中的主要使用的通配符有 "*","?"，比如：

```text
*.c
```

则表示所有文件名以.c结尾的文件。



## 各个通配符的使用

### "*"

若是要选出通配符中出场率最高之一，* 可谓是当仁不让，* 表示匹配所有任何符合条件的。   

*.o 表示所有的.o文件   

*.c 表示所有的.c文件  

\*  表示所有的文件   



### "?"

"?" 通常在依赖文件列表中使用，匹配所有有更新的目标。  

在第一章节中我们提到，make在编译目标时，会去检查目标的依赖文件列表是是否有文件更新，$? 表示当前依赖列表中已经更新的依赖文件。我们来看以下的示例：

```text
1 main:foo.c bar.c
2     @echo $?
3     touch main
```

第一次运行**make**时，输出

```text
foo.c bar.c
touch main
```

$?  的输出为foo.c,bar.c，因为这两个文件都是第一次编译。  

此时，我们对 foo.c 运行 touch 命令更新 foo.c 的时间戳，再运行 make ：  

输出为：

```text
foo.c
touch main
```

此时，$? 的值就是更新的文件foo.c。  

需要注意的是，在目标没有被生成的时候，make工具会将所有的依赖文件视为已更新的文件，从而重新编译生成目标。      

比如在上述示例中，在第一次执行**make**之后，手动删除生成的**main**文件，$? 的值将会是所有的依赖文件。  

值得注意的是，在shell中，$? 表示上一条指令的执行结果，这里需要做相应区分。  



### 通配符的转义

当我们需要输入真实的字符 * 时，为了避免这些特殊符号被识别为通配符，需要对这些字符进行转义，比如：要操作一个真实的文件名为 *.c 时,不能直接这样输入：

```text
obj = *.c
```

在当前目录下存在.c 文件时，将导致obj被赋值为所有后缀为.c的文件列表，应该使用反斜杠"\"对特殊符号进行转义。应该是这样的：

```text
obj = \*.c
```

这样obj的内容就是*.c了。  

需要注意的是，在windows系统中，文件的路径分隔符是"\"而不是unix下的"/"，尽管windows同时支持windows风格和unix风格的文件分隔符，但是这种支持并不包括通配符的扩展。所以，在windows下执行makefile时需要特别注意这一点。   



## 通配符在赋值时的陷阱

学习了makefile中通配符的概念之后，很多朋友就很容易地写出这样的语句：

```text
OBJ = *.o
```

可能你的本意是：OBJ的值为所有的以.o为后缀的文件名列表，如果没有相应的.o，make就会通过make的隐式推导生成所有对应的.o文件。    

但是事实上并非如此，使用${OBJ}得出的值为 *.o 字符串本身，是的，在这种情况下通配符并不起作用。  

那是不是在所有赋值的行为中，通配符都不起作用呢？博主在官方文档并没有找到相应的解答，但是一试便知。   

在当前目录下执行ls：

```text
main.c foo.c bar.c Makefile
```

Makefile的内容为：

```text
1 OBJ = *.c
2 main:
3     @echo ${OBJ}
```

执行 make 时，结果为：

```text
bar.c foo.c main.c
```

这个结果说明，**从原理上来说，当你在赋值时指定通配符匹配时，如果通配符表达式匹配不到任何合适的对象，通配符语句本身就会被赋值给变量**。所以，在上面的示例中，在当前目录下不存在.o文件时，${OBJ}就被赋值为"*.o"这明显不是我们想要的。而"*.c"因为指定目录下有.c文件而被正常赋值。    



### 通配符函数

make作为一个成熟的工具，既然出现了问题，自然是有对应的解决方案的，我们可以使用通配符函数**wildcard**来实现上面的问题。

```text
OBJ = ${wildcard *.o}
```

OBJ的赋值指定使用通配符的扩展方式，这样即使是没有匹配到任何合适的文件，${OBJ}的内容为空，而并非是错误的"*.o"。  



## 规则中的特殊变量

在makefile的编译规则中，有一些特殊的內建变量，下面就列出一些常用的內建变量： 为了演示方便，这里再回顾一下makefile目标的编译规则：

```text
目标：依赖列表
    命令
```

- **$@**：表示需要被编译的目标 
- **$<**：依赖列表中第一个依赖文件名 
- **$^**：依赖列表中所有文件
- **$?**: 依赖文件列表中所有有更新的文件，以空格分隔
- **~** 或者 **./** ：用户的家目录,如果 **~**后接字符串，表示/home/+字符串,比如~downey,展开为/home/downey/。



## 模式规则

模式规则类似于普通规则。只是在模式规则中，目标名中需要包含有模式字符"%"，包含有模式字符"%"的目标被用来匹配一个文件名，"%" 可以匹配任何非空字符串。规则的依赖文件中同样可以使用"%"，依赖文件中模式字符"%"的取值情况由目标中的"%"来决定。   

例如：对于模式规则"%.o : %.c"，它表示的含义是：所有的.o文件依赖于对应的.c文件。  

下列示例就是一个makefile內建的模式规则，由所有的.c文件生成对应的.o文件：

```text
%.o : %.c
    $(CC) -c $(CFLAGS) $< -o $@
```

根据这个模式规则，makefile提供了隐式推导规则。   

同时，模式规则的依赖可以不包含"%"，当依赖不包含"%"时代表的是所有与模式匹配的目标都依赖于指定的依赖文件。  



### 静态模式规则

静态模式可以更加容易地定义多目标的规则，它的语法是这样的：

```text
目标 ...: 目标模式 ： 依赖的模式
        命令
        ...
```

相对于普通的模式规则，静态模式规则则显得更加地灵活，作为模式规则的一种，仍然使用"%"来进行模式的匹配，我们来看下面一个简单的例子：

当前目录下的文件：foo.c foo.h bar.c bar.h main.c. makefile内容：

```text
1 OBJ = foo.o bar.o
2 main:${OBJ}
3     cc ${OBJ} main.c -o main
4 ${OBJ}:%.o : %.c
5     cc -c $^
```

执行make时的运行log:

```text
cc -c foo.c 
cc -c bar.c 
cc foo.o bar.o main.c -o main
```

make在编译时会将执行的指令打印出来，这一部分就是实际被执行的指令。    

可以看到，在makefile第二行，**main**的依赖文件为${OBJ},即**foo.o**和**bar.o**,make在当前目录中并没有找到这两个文件，所以就需要寻找生成这两个依赖文件的规则。  

第四行就是生成**foo.o**和**bar.o**的规则，所以需要先被执行，这一行使用了静态模式规则，对于存在于${OBJ}中的每个.o文件，使用对应的.c文件作为依赖，调用命令部分，而命令就是生成.o文件。   

可以看到，相对应普通的模式规则，静态模式规则相对来说更加地灵活。  



### 另一种常用的语法

在模式规则时还有另一种常用的语法，是这样的：

```text
${OBJ：pre-pattern=pattern}
```

举个例子：

```text
${OBJ:%.c=%.o}
```

这条语句的作用是：将OBJ中所有.o后缀文件替换成.c后缀文件。   



### 通配符与模式规则区别

乍一看，通配符和模式匹配像是同一个东西， * 和 % 都表示匹配任意的对象。  

当然，这种"乍一看"的印象是错误的。

模式匹配对应的是生成规则，规则对应：目标、依赖和命令，与普通规则不同的是，它并不显示地指定具体的规则，则是自动匹配。   

而通配符对应的是目标，表示寻找所有符合条件的目标，通常代表一个集合。    

一个是针对执行规则，一个是针对目标文件，自然是不同的。  

(模式规则和函数中的模式匹配也是不同的)。  



## 学以致用

在makefile中，通配符与模式规则运用灵活，功能强大，同时也带来的一定的应用难度，合理地运用这些特性可以事半功倍。  

我们来看看下面这个简单的、单目录模式下编译可执行文件的makefile模板：

**环境**：参与编译的文件：

```text
foo.c  foo.h  bar.c  bar.h  common.h main.c
foo.c 依赖 foo.h common.h
bar.c 依赖 bar.h common.h
main.c为主文件，依赖foo.h  bar.h  common.h
```

**目标**：编译可执行文件main.

**makefile**：

```text
1 SRC = ${wildcard *.c}
2 
3 MAIN_SRC = main.c
4 
5 TARGET = main
6 
7 RAW_OBJ = ${patsubst %.c,%.o,${SRC}}
8 
9 OBJ = ${filter-out main% ,${RAW_OBJ}}
10 
11 ${TARGET}:${OBJ}
12     cc $^ ${MAIN_SRC} -o ${TARGET}
13 
14 ${OBJ}:%.o : %.c %.h common.h
15     cc -c $^
16 
17 clean:
18     rm -rf *.o ${TARGET}
```

键入以下指令就可以开始工程的编译：

```text
make
```

### 示例讲解

-  第一行：将所有的.c为后缀的文件名赋值给SRC，以空格分隔。结果为：\${SRC} = main.c foo.c bar.c  
   
-  第三行：赋值MAIN_SRC为main.c  
   
-  第五行：指定编译目标，即可执行文件的变量  
   
-  第七行：使用patsubst()函数将SRC中所有.c后缀文件转换成.o后缀，并赋值给RAW_OBJ，patsubst是一个模式匹配的字符串匹配函数(见后面的章节)。此时\${RAW_OBJ} = main.o foo.o bar.o  
   
-  第九行：因为不需要编译生成main.o,所以需要使用filter-out()函数去掉main.o，filter-out()函数的作用是反选(见后面的章节)，此时${OBJ} =foo.o bar.o   
   
-   第11-12行：这是makefile中的第一个目标，执行make时默认执行这个目标，它依赖\${OBJ},但是目前\${OBJ}中.o文件没有生成，所以需要先调用14行的静态规则语句生成所有的.o文件，然后再与main.c一起编译生成可执行文件main。   
   
-  第14-15行：静态模式，由.c文件生成对应的.o文件，**这里需要特别强调的一点是依赖文件，很多朋友总是会忽略依赖文件的作用，因为依赖文件在编译的时候并不提供任何帮助，但是make需要靠依赖文件来判断文件的更新以至于判断目标是否需要更新，如果忽略依赖文件，仅仅添加%.c的话，\*.h common.h中的修改将不会导致重新编译，这样明显不是用户想要的。** 
   
-  第17-18行：clean目标，将生成的文件全部删除。调用方法是make clean。  
   

### 参考

参考资料：[MAKE 官方文档](https://link.zhihu.com/?target=https%3A//www.gnu.org/software/make/manual/make.html)



# 深入解析Makefile系列(3) -- 函数的使用

[![黄导](https://pic3.zhimg.com/v2-abed1a8c04700ba7d72b45195223e0ff_xs.jpg?source=172ae18b)](https://www.zhihu.com/people/huang-dao-27-40)

[黄导](https://www.zhihu.com/people/huang-dao-27-40)

c程序员

在之前的章节中，我们讲解了编写makefile的基本规则，在这一章节中我们将讨论makefile规则中的函数使用。   

## 语法

函数的使用语法是这样的：

```text
$(function arguments)
```

或者  

```text
${function arguments}
```

参数之间用逗号","分隔，单个参数可以是以空格分隔的列表。  

在makefile中，有一系列的內建函数以适用于各类文件的处理，函数本身就是对操作过程的一种封装，使用官方提供的函数能大大提高我们编写makefile的效率，降低复杂操作的出错率。接下来我们就来看看一些常用的函数。  

对于复杂难以理解的函数，博主将附上详细的注释和示例。对于简单的函数，则不需要添加示例。  

## 文本相关：

### $(subst from,to,text)

### 文本(文本)替换

**函数介绍：** 

- 函数作用：对目标文本(或列表)text执行文本替换，将主文本中的from替换成to，并返回替换后的新文本。  

- 参数：

- - from ： 将要被替换的子文本
  - to   ： 替换到文本text的新子文本
  - text ： 被操作的主文本

- 返回：返回替换之后的新文本

**示例**:

```text
1 TEXT = "hello world" "hello downey"
2 FROM = hello
3 TO   = HELLO
4 RESULT = $(subst $(FROM),$(TO),$(TEXT))
```

${RESULT}结果：

```text
HELLO world HELLO downey
```

可以看到，如果text是文本列表一样支持。  



### $(patsubst pattern,replacement,text)

### 文本(文本)模式替换

**函数介绍:**  

- 函数作用：对目标文本(或列表)text执行文本替换，以模式替换的形式进行替换。  

- 参数：

- - pattern ： 将要被替换的模式匹配方式.
  - replacement ： 替换后的模式匹配方式.
  - text : 被操作的文本

- 返回：返回替换后的新文本

**示例**：

```text
TEXT = foo.c bar.c
RESULT = $(patsubst %.c,%.o,${TEXT})
```

${RESULT}结果：

```text
foo.o bar.o
```

同样的，TEXT可以是列表。  

这个操作可以使用一个更加简单的模式匹规则语法来操作，相当于：

```text
${TEXT : %.c=%.o}
```

结果是一样的。  



### $(strip string)

### 文本精简

**函数介绍：** 

- 函数作用：将文本中前导和尾随的空格删除，在文本中存在多个连续空格时，使用一个空格替换。  

- 参数：

- - string ： 目标文本

- 返回值 ： 精简完的文本

**示例**：(无)  



### $(findstring find,in)

### 在主文本中寻找子文本

**函数介绍：** 

- 函数作用 ： 顾名思义，这是文本查找函数，在in中寻找是否有find文本。

- 参数：

- - find : 子文本
  - in ： 主文本

- 如果in中存在find，返回find，否则返回""(空文本)。

**示例**：(无)



### \$(filter pattern…,text) 和 \$(filter-out pattern…,text)

**函数介绍：** 

- 函数作用 ： 过滤作用，将符合模式规则的text中的文本挑选出来。  

- 参数：

- - pattern  过滤的模式规则
  - text  将要处理的文本

- 返回值 ： 返回符合模式规则的文本

**示例：** 

```text
TEXT := foo.c bar.c foo.h bar.h
RESULT = $(filter %.c,$(TEXT))
```

${RESULT}结果：

```text
foo.c bar.c
```

filter-out()函数的作用与filter相反，运用模式规则进行反选，返回反选的结果。   

值得注意的是，这里的pattern不一定是带 % 的模式匹配，也可以是文件列表。  

filter(filter-out)函数返回的是text文本中符合(不符合)条件的项目，我们再来看下面的示例：

```text
TEXT := foo.c
TEXT_P := foo.c bar.c main.c
RESULT = $(filter $(TEXT_P),$(TEXT))
RESULT_OUT = $(filter-out $(TEXT_P),$(TEXT))
```

${RESULT} 和 $(RESULT_OUT)结果：

```text
foo.c 
(空)
```

从上述示例中看到，从以TEXT_P 去 TEXT 中进行匹配，RESULT的值为foo.c,但是为什么 RESULT_OUT 是空，而不是bar.c main.c呢？    

请看上面本函数的函数作用：**将符合模式的text中的文本挑选出来**，TEXT只有foo.c，但是foo.c被反选出去，所以结果为空。  



### $(sort list)

**函数介绍：** 

- 函数作用 ：将给定的list(通常是以空格分隔的文件列表)按照首字母字典排序。

- 参数：

- - list ： 目标列表

- 返回值 ： 返回排序后的列表。  

**示例：**(无)



### $(word n,text)

**函数介绍：** 

- 函数作用 ：返回text列表中第n个元素，通常来说，这个text为文件或文本列表，元素为单个的文件名或文本

- 参数：    

- - n ： 第n个元素，比较特殊的是，元素列表的计数从1开始。
  - text : 文件列表

- 返回值 ： 返回第n个元素

**示例：**

```text
TEXT := foo.c foo.h bar.c
RESULT := ${word 2,${TEXT}}
```

${RESULT}结果：  

```text
foo.h
```

值得注意的是，当n超出list范围时，返回空文本。当n小于1时，make将报错。  



### $(wordlist s,e,text)

**函数介绍：** 

- 函数作用 ：返回text列表中指定的由s(start)开始由e(end)结尾的列表，s和e均为数字。    

- 参数：

- - s ： 截取开始的地方，s从1开始
  - e ： 截取文本结束的地方。
  - text ： 目标文件列表

- 返回值 ： 返回截取的文本

**示例：**(无)

需要注意的是：  *如果s大于text的最大列表数量，返回空文本*  如果e大与text的最大列表数量，返回从s开始到结尾的列表 * 如果s大于1，返回空。  



### $(words text)

**函数介绍：** 

- 函数作用 ：返回text列表中的元素数量

- 参数：

- - text ： 目标列

- 返回值 ： 返回text列表中的元素数量

**示例：**(无)



### $(firstword names…)  \$(lastword names…)

**函数介绍：** 

- 函数作用 ：返回names列表中的第一个元素

- 参数：

- - names ： 目标列表

- 返回值 ： 返回names列表中的第一个元素

**示例：**(无)

lastword()函数与firstword相反，返回最后一个元素



## 文件与目录操作函数

### $(dir names…) \$(notdir names…)

**函数介绍：** 

- 函数作用 ：截取文件路径中的目录部分，如：/home/downey/file.c，截取/home/downey/，目标可以是列表

- 参数：

- - names ： 目标文件，可以是列表 

- 返回值 ： 返回目录，如果目标是列表，返回以空格分隔的目录。  

**示例：**(无)

**与dir相对的,notdir()仅截取文件名**



### $(suffix names…)  \$(basename names…)

**函数介绍：** 

- 函数作用 ：获取文件列表中的后缀部分。

- 参数：

- - names ： 目标文件，可以是列表 

- 返回值 ： 返回文件列表中的后缀部分。如：".c",".o"

**与suffix相对的,basename()去除后缀名**

**示例：**(无)



### $(addsuffix suffix,names…)

**函数介绍：** 

- 函数作用 ：为目标文件列表添加后缀

- 参数：

- - suffix ： 添加的后缀内容
  - names ： 目标列表

- 返回值 ： 返回添加完后缀的列表

**示例：**

```text
TEXT := foo bar
RESULT := ${addsuffix .o , ${TEXT}}
```

${RESULT}结果:

```text
foo.o bar.o
```

### $(join list1,list2)

**函数介绍：** 

- 函数作用 ：逐个地将list2中的元素链接到list1。  

- 参数：

- - list1 ： 链接后元素在前的列表
  - list2 ： 链接后元素在后的列表

- 返回值 ： 返回链接后的链表

**示例：**

```text
LIST1 := foo bar
LIST2 := .c  .p
RESULT := ${join ${LIST1} , ${LIST2}}
```

${RESULT}结果:

```text
foo.c bar.p
```

有两点需要注意的地方：
  *当list1和list2不等长时，视为与空文本的链接，例如list1为3个元素，list2有两个元素，那么list1的第三个元素对应空文本。*  
  list中的空格将始终被当做一个空格处理。  



### $(realpath names…)

**函数介绍：** 

- 函数作用 ：对names中的每个文件，求其绝对路径，当目标为链接时，将解析链接。  

- 参数：

- - names ：目标文件名(列表) 

- 返回值 ： 目标文件名对应的绝对路径(列表)。 

**示例：**(无)  



### $(abspath names…)

**函数介绍：**  作用与realpath()函数相似，唯一的不同是不解析链接。  



## 其他常用函数

### $(foreach var,list,text)

**函数介绍：** 

- 函数作用 ：对list中的每个var，调用text命令。  

- 参数：

- - var ：被操作的目标元素  
  - list ： 目标元素列表  
  - text ： 对目标元素执行的操作。  

- 返回值 ： 返回执行操作后的文本.

**示例：** 

```text
TEXT := foo.c bar.c
RESULT := ${foreach file,${TEXT},${file}.c}
```

${RESULT}结果:

```text
foo.c.c bar.c.c
```

从结果可以看出，makefile中在${file}后添加了一个.c后缀。    

**值得注意的是：第三个参数中的text是对元素执行的操作，但是！这里的操作并不是指的shell下的操作，不能执行类似于：@echo  ${file}等操作，echo是shell下的函数，而这里是遵循makefile的语法，我们可以使用makefile中允许的函数比如：wildcard()，我们应该分清这一点。** 
 **在示例中，对于TEXT中的每个file(这个可以是任何临时变量)，返回第三个参数执行后的文本，这里是直接将file展开加上.c，整个表达式最后的返回结果就是所有单条语句返回结果的列表集合。** 



### $(file op filename[,text])

**函数介绍：** 

- 函数作用 ：向文件执行文本的输入输出  

- 参数：

- - op ： 要对文件进行的操作，支持：>(覆盖写) >>(追加写)  <(读)
  - filename ： 文件名  
  - text ： 如果op为写，此参数表示要写入的文本。    

- 返回值 ： 返回值无意义   

**示例：** 

```text
TEXT := "hello world"
RESULT := \${file > test,${TEXT}}
```

执行结果：当当前目录下存在test文件时，"hello world"被写入到test中，当不存在test文件时，文件被创建且同时写入"hello world".  



### $(call variable,param,param,…)

**函数介绍：**   

- 函数作用 ：call函数在makefile中是一个特殊的函数，因为它的作用就是创建一个新的函数。你可以使用call函数创建各种实现复杂功能的函数。

- 参数：

- - variable ： 函数名
  - param ： 函数参数,在使用是被传递给自定义的variable函数。
  - param ...   

- 返回值 ： 返回定义函数的结果.  

当call函数被调用时，make将展开这个函数，函数的参数会被赋值给临时参数 \$1,\$2,\$0 则代表函数名本身，参数没有上限也没有下限。     

对于call函数需要注意几点：    
  *当定义的函数名与内建函数名同名时，总是调用内建函数。*
  call函数在分配参数之前扩展它们，意味着对具有特殊扩展规则的内置函数的引用的变量值可能不会正常执行。  

**示例：** 

```text
func = $1.$2
foo = $(call func,a,b)
all:
    @echo $(foo)
```

输出结果：

```text
a.b
```

可以看到，在第一行定义了函数的行为，即返回$1.$2,调用时参数参数a和b，结果为a.b。  

我们还可以使用define来定义更加复杂的函数：

```text
1 define func 
2 $1.$2
3 var = bar.c
4 endef
5 foo = $(call func,a,b)
```

foo的结果为：

```text
a.b
var = bar.c
```

define是定义变量的一种方式，这种定义变量的特殊之处在于它可以定义带有换行的变量，所以它可以定义一系列的命令，在这里就可以定义一个函数。    



### $(value variable)

**函数介绍：** 

- 函数作用 ：获取未展开的变量值。    

- 参数：

- - variable ：目标变量，不需要添加"$"   

- 返回值 ： 返回变量的未展开的值.
   **示例：** 

```text
FOO = \$PATH
all:
    @echo \$(FOO)
    @echo \$(value FOO)
```

执行结果：

```text
ATH
/home/downey/bin:/home/downey/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
```

第一个结果\${FOO}为ATH是因为，make将\$P解析成makefile中的变量，而不是将(PATH)作为一个整体来解析。  

第二个结果得到的就是真实的环境变量值。  

**事实上，博主对这个value函数的定义并不理解，按照官方文档理解，既然是未扩展的值，\$(value FOO)的返回值不应该是"$PATH"这个文本吗？**
 **之后，博主尝试了将PATH换成其他定义的变量，例如：**

```text
TEXT := hello
FOO := $TEXT
all:
    @echo $(value FOO)
```

**输出的结果为"EXT"(\$T+EXT)，而非hello(\${TEXT})，更不是"\$TEXT"这个文本，这个结果就非常奇怪了，明明这个和PATH一样同样是变量，为什么这个结果和$PATH结果完全不一样。** 

**是不是因为PATH是内置变量？那我就换一个内置变量试试：**

```text
FOO = $CURDIR
all:
    @echo $(value FOO)
```

**输出结果为"URDIR",依旧是(\$C+URDIR)，\$(CURDIR)应该是当前目录。** 

**后来查看makefile的内置变量，发现PATH其实是linux shell环境下的全局变量,我接着使用USER(同是shell环境变量，使用export查看)，与PATH一样的效果：** 

```text
FOO = $USER
    all:
        @echo $(value FOO)
```

**输出为"downey"(我的用户名)**
 **其中FOO的赋值只能是 "=" 而不能是 ":="，这一部分确实是和扩展相关的属性。但是博主还是不理解其中的原理。**
 **经过各种实验，我只能将它理解为，value函数可以解析shell中的环境变量，而对于官方的说法：返回变量的未经扩展的值，目前仍然不能理解。** **同时，网上的大部分博客都是复制粘贴，没有什么参考价值。** 
 **希望路过的大神不吝赐教！！**



### $(eval text)

**函数介绍：** 

- 函数作用  ：eval在makefile中是一个非常特别的函数，它允许此函数新定义一个makefile下的结构，包含变量、目标、隐式或者显示的规则。eval函数的参数会被展开，然后再由makeifle进行解析。也就是说，eval函数会被make解析两次，第一次是对eval函数的解析，第二次是make对eval参数的解析。    

- 参数：

- - text ： 一个makefile结构    

- 返回值 ： 返回值无意义.  

**示例：** 

```text
1 define func
2 $1:
3     cc $2 -o $$@
4 endef
5 
6 $(eval $(call func,main,main.c))
```

执行结果：

```text
cc main.c -o main
```

在1-4行定义了一个变量集，我们也可以直接使用call函数，将其作为一个函数调用，返回：

```text
$(1):
    cc $(2) -o $$@
```

返回值依旧是一个makefile规则的表达式，这时候如果需要执行这个规则，那就要使用makefile的规则对其进行再一次地解析，就需要用到eval()函数，eval()函数通常与call()一起使用。   

至于第三行中出现的\$\$,在makefile的语法中，\$是一个特殊字符，通常与其他符号结合表示特定的含义，如果我们单纯的就想打出"\$"字符，我们需要使用"\$\$"表示"\$"符号，就像C语言中的转义符号'\'，如果我们要使用真实的'\'符号，我们就得使用'\'来进行转义。



### $(origin variable)

**函数介绍：** 

- 函数作用 ：这个函数与其他函数的区别在于，它不执行变量操作，而是返回变量的定义信息。  

- 参数：

- - variable ：被操作的目标变量   

- 返回值 ： 返回变量的定义信息。这些定义信息是一些枚举值：

- - undefined ： 变量未定义
  - default   ： 变量被默认定义，就像CC.
  - environment : 传入的环境变量
  - environment override ： 本来在makefile中被定义为环境变量，但是新定义的变量覆盖了环境变量。  
  - file      ： 在makefile中被定义
  - command line ： 在命令行中被定义
  - override   ： 使用override定义的变量
  - automatic  ： 在规则的命令部分被定义的自动变量，也可以理解为临时变量.  

**示例：** (无)  



### $(flavor variable)

**函数介绍：** 

- 函数作用 : 与origin的属性类似，这个函数返回变量的展开方式，在之前章节有提到过，变量展开方式有两种：循环递归扩展和简单扩展。  

- 参数：

- - variable ： 目标变量

- 返回值 ： 返回变量的展开方式，这是一个枚举值：

- - undefined ： 变量未定义
  - recursive ： 循环递归扩展
  - simple ： 简单扩展

**示例：** (无) 



## 控制执行流的函数

### $(error text…)

**函数介绍：** 

- 函数作用 : 生成一个错误，并返回错误信息text。  

- 参数：

- - text ： 将要打印的错误信息。  

- 返回值：返回值无意义，makefile执行终止。  

**需要注意的是：只要调用了这个函数，错误信息就会生成，但是如果将其作为一个循环递归变量的右值，它会被最后扩展的时候才被调用，或者放在规则的命令部分，它将被shell解析，可能结果并不是你想要的。**



### \$(warning text…) \$(info text…)

**函数介绍：**这两个函数属性与error相似，但是这两个函数并不会导致makefile的执行中断，而是警告和提示。  



### $(shell command...)

**函数介绍：** 
  *函数作用 : shell函数与其他函数不一样的地方在于，它将它的参数当做一条shell命令来执行，shell命令是不遵循makefile的语法的，也就是说不由make解析，将传递给shell，这一点和规则中的命令部分是一样的。*  参数：     * command ： 命令部分 * 返回值 ： 返回shell命令执行结果  

**值得注意的是：make会对最后的返回值进行处理，将所有的换行符替换成单个空格。** 

在makefile中可以使用另一种做法来调用shell命令，就是用"`"(键盘左上角ESC下的键)将命令包含。同样的，"!="赋值运算符也是一种方法。    





### 参考

参考资料：[MAKE 官方文档](https://link.zhihu.com/?target=https%3A//www.gnu.org/software/make/manual/make.html)

------
