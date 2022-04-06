set fs = createobject("scripting.filesystemobject")

rem -----------------驱动器操作-----------------
rem 驱动器是否存在
rem msgbox(fs.driveexists("c:\"))

rem 获取驱动器对象
rem set drive = fs.getdrive("c:")

rem 驱动器的可用容量大小
rem msgbox((drive.availablespace / 1024 / 1024 / 1024) & " GB")

rem 驱动器的剩余容量大小
rem msgbox((drive.freespace / 1024 / 1024 / 1024) & " GB")

rem 驱动器的总容量大小
rem msgbox((drive.totalsize / 1024 / 1024 / 1024) & " GM")


rem -----------------文件夹操作-----------------
rem 文件夹是否存在
rem msgbox(fs.folderexists("e:\test"))

rem 创建文件夹
rem set folder = fs.createfolder("e:\test2")

rem 获取文件夹对象
rem set folder = fs.getfolder("e:\test2")

rem 移动文件夹
rem fs.movefolder "e:\test2", "e:\test3"
rem folder.move("e:\test3")

rem 复制文件夹
rem fs.copyfolder "e:\test2", "e:\test3"
rem folder.copy("e:\test3")

rem 删除文件夹
rem fs.deletefolder("e:\test3")
rem folder.delete()

rem 获取文件夹名字
rem msgbox(folder.name)

rem 获取文件夹路径
rem msgbox(folder.path)

rem 获取文件夹大小
rem msgbox((folder.size / 1024 / 1024) & " MB")

rem 获取文件夹文件数量
rem msgbox(folder.files.count)

rem 获取文件对象列表
rem for each file in folder.files
rem		msgbox(file.path)
rem next

rem 获取子文件夹数量
rem msgbox(folder.subfolders.count)

rem 获取子文件夹对象列表
rem for each subfolder in folder.subfolders
rem 	msgbox(subfolder.path)
rem next

rem 获取父文件夹对象
rem set parentfolder = folder.parentfolder
rem msgbox(parentfolder.path)


rem -----------------文件操作-----------------
rem 判断文件是否存在
rem msgbox(fs.fileexists("e:\test2\xxx\a.txt"))

rem 获取文件对象
rem set file = fs.getfile("e:\test2\xxx\a.txt")

rem 获取文件路径
rem msgbox(file.path)

rem 获取文件所在文件夹
rem msgbox(file.parentfolder)

rem 获取文件名字和后缀
rem msgbox(fs.getfilename("e:\test2\xxx\a.txt"))
rem msgbox(file.name)

rem 获取文件扩展名(不包.)
rem msgbox(fs.getextensionname("e:\test2\xxx\a.txt"))

rem 获取文件大小
rem msgbox(file.size & " b")

rem 文件移动
rem fs.movefile "e:\test2\xxx\a.txt", "e:\test2\a.txt"
rem file.move("e:\test2\a.txt")

rem 文件复制
rem fs.copyfile "e:\test2\xxx\a.txt", "e:\test2\a.txt"
rem file.copy("e:\test2\a.txt")

rem 文件删除
rem fs.deletefile("e:\test2\a.txt")
rem file.delete()

rem 文件创建
rem set file = fs.createtextfile("e:\test2\a.txt")


rem 文件打开(读)
rem set file = fs.opentextfile("e:\test2\xxx\a.txt", 1, false)

rem 是否到行的末尾
rem msgbox(file.atendofline)

rem 是否到文件末尾
rem msgbox(file.atendofstream)

rem 读取n个字符
rem msgbox(file.read(5))

rem 文件指针在当前文件的列号
rem msgbox(file.column)

rem 读取1行
rem msgbox(file.readline())

rem 文件指针在当前文件的行号
rem msgbox(file.line)

rem 跳过n个字符
rem file.skip(5)

rem 跳过1行
rem file.skipline()

rem 文件打开(写)
rem set file = fs.opentextfile("e:\test2\a.txt", 2, true)

rem 文件打开(追加)
rem set file = fs.opentextfile("e:\test2\a.txt", 8, true)

rem 写入字符串
rem file.write("xiaohei")

rem 写入一行字符串
rem file.writeline("line 1")

rem 写入n行空行
rem file.writeblanklines(5)

rem 文件关闭
file.close()

set fs = nothing





## 前言

当初学VBS主要是为了使用一个脚本文件直接启动可执行jar包，所以就简单学了一下VBS的使用，浅尝辄止，需求达到了也就不再深入学习了。至于为什么不用bat之类的其他脚本语言或者打包成exe，只是当初有需要，刚好看见VBS可以实现功能，就学了。当然各位有更好的推荐，请不吝赐教！

如果您是想尝试学习使用VBS脚本，那么本文章可供参考，写的不好的地方，多多包涵！

## 1、VBS不区分大小写

vbs所有代码均**不区分大小写**。

## 2、语句结束

vbs不可以使用;作为语句结尾，会报错，**直接换行**则表示一条语句。

## 3、代码注释

VBS就一种注释方式，使用**一个英文单引号**进行注释，注释内容为**当前行单引号后的所有内容**

```java
option explicit
dim var1,var2 '多个变量声明用逗号隔开
```

## 4、变量声明

- 显式声明：Dim、Public、Private语句进行声明
- 隐式声明：不声明直接使用，相当于Dim
- 强制声明：Option Explicit语句强制显式声明所有变量

如果没有声明option explicit，vbs在使用变量前不一定声明变量，可直接使用。但这样容易带来一些潜在的问题，不建议这样使用。

**使用建议**：无脑在脚本开头写上**option explicit**即可

## 5、变量赋值

一个语句中可以同时声明多个变量，也可以同时赋值多个变量。但不可以声明变量的同时赋值

```text
option explicit
dim var1,var2 '多个变量声明用逗号隔开
var1=1:var2=2 '多个变量赋值用冒号隔开
'dim var3=3    '声明的同时赋值是错误的
```

**使用建议**：如果觉得难记，没必要合着写，就拆开一个一个声明，一个一个赋值

## 6、字符串的表示方式

字符串必须使用双引号包围，不可使用单引号。而且vbs中单引号不能表示单个字符

## 7、数据拼接

使用'&'可拼接任意类型的数据，有一点值得注意，如果&两端都是数字常量，则不能连续使用，必须存在空格，如:  111 & 777 ；此时才能完成连接，若为111&777会报错。

使用'+'号一样可以起到和'&'类似的效果，区别是：

- 数字 + 数字：表示运算
- 数字 & 数字：表示拼接，且数字和&之间必须有空格
- 数字 + 字符串数字 ：表示运算
- 数字 & 字符串数字：表示拼接，且数字和&之间必须有空格
- 数字和非纯数字字符串拼接时，只能使用 &
- 非纯数字字符串+非纯数字字符串：表示拼接

```text
Option Explicit 
dim Name,age
name=1:age=0
age=age+1
msgbox name & age & "abc" 
'输出11abc
```

## 8、数据计算

```text
Option Explicit 
dim Name,age
name="1":age="1"
age=age+1+"2"+name '对字符串类型会自动转换成数字类型进行计算，如果转换失败则会报错，如包含非数字符号
msgbox name & age  '这里两个数字使用&拼接
'输出15
```

## 9、变量作用域

① **脚本级作用域（声明在最外层的）**

变量在整个脚本文件中都是有效的。声明的变量的作用域就是整个脚本。

② **过程级作用域（声明在过程里的）**

变量在过程或函数中有效。过程、函数之外的其它代码都不能访问过程级变量。

③ **类级作用域（声明在类里的）**

这是一种包含属性和方法的逻辑分组的特殊结构。类定义之外的代码都不能访问类级变量。

有三个关键字可以用于声明变量：Dim、Private、Public

1.对于脚本级变量，不论是哪种类型的声明，作用域都为整个脚本文件

2.dim可用于声明脚本、过程、类级作用域的变量，过程变量必须使用dim，对于类变量，表示局部变量，类级变量dim效果与public相同

3.private可用于声明脚本、类级作用域的变量，对于类级变量，则表示声明了一个私有变量

4.public，与private相对，表示类的公有变量

**使用建议**：一般你也不会在vbs脚本里写多少功能，多半就是个启动时候一些文件检查之类的，**无脑dim**即可。

## 10、数组

```text
option explicit
dim arr() '使用括号表示数组,括号没有填值，只表示一个动态数组变量，并没有指定长度
dim arr2(2) '此时声明arr2时，指定了长度，那么这个长度就不可变化了
dim arr3(3,4) '二维数组
redim arr(3) '可使用redim定义一个数组长度，如果该数组变量在声明时是已经确定长度的，则会报错
redim arr(4) '对于不确定长度的，可多次改变数组长度
'redim arr2(5) 会报错，因为arr2已经在声明时固定了长度
'preserve关键字标识在redim时是否保持原值不变，如果不加该关键字，在重新构建数组时，原值会被清空
redim Preserve arr(3) 'redim 增加一个preserve关键字后可重新定义数组长度的同时保持数值不变，变长则多出来的部分默认为空，变短则截断，即使后面再变长回去数据也不在了
arr(0)="aa" '数组下标从0开始
arr(1)="hh"
msgbox arr(0)'输出数组第一位
```

这里说明一下msgbox，这个就是让系统弹出一个提示框，提示框的内容就是msgbox的第一个参数。

**数组补充说明（不同于其他语言的数组的部分）**：

如dim arr(2)，按照现在最常见的理解，数组长度为2，包含arr(0)和arr(1)两个元素

但事实上arr包含了0，1，2三个下标的元素

声明多元数组使用逗号分割下标dim arr(2,3)，维度从左到右，从低到高(2为一维)

ubound(arr,dim)函数用于获取下标最大值，arr表示数组名，dim表示维度

lbound与ubound对应，是用于获得下标最小值的，但目前看来，始终为0

## 11、if条件判断

**if ... then ... end if**

```text
option explicit
dim a
dim b
a=1
b=2
if a>b then
msgbox "a>b"
end if
```

**if ... then ... else ... end if**

```text
option explicit
dim a
dim b
a=1
b=2
if a>b then
msgbox "a>b"
else
msgbox "a<b"
end if
```

**if ... then ... elseif ... then ... end if**

```text
option explicit
dim a
dim b
a=1
b=1
if a>b then
msgbox "a>b"
elseif a=b then
msgbox "a=b"
end if
```

## 12、select语句

```text
option explicit
dim a
a=1
select case a
case 1
msgbox "a=1"
msgbox "wow"
case 2
msgbox "a=1"
case else
msgbox "I don't know"
end select
```

**使用建议**：开头的select case后填变量，后面的case填变量值，语句执行范围为目标case到下一个case之前，也就是说，当a=1时，执行的就是msgbox "a=1"和msgbox "wow"，执行完就结束select了，不需要break

## 13、do ... loop循环

**do while/until ... loop**

```text
option explicit
dim a
a=1
do while a<10
	a=a+1
	if a=5 then
		exit do
	end if
loop
msgbox a
```

**提示**：将该循环拆成三部分理解。

1、do到loop之间的代码为循环执行的代码。

2、while表示当条件满足继续执行循环。与之对应的是until，表示当条件满足则结束循环。这个根据字面意思应该也能理解。而且while或until可以放在do后面，也可以放在loop后面。放在do后面表示**先判断后执行**，放在loop后面表示**先执行后判断**。

3、exit do表示在该位置结束循环，继续执行循环外的后续代码，要和if等条件判断语句连用，直接放在循环体执行一次就退出了。

## 14、**while...wend循环**

```text
option explicit
dim a
a=1
while a<10
	a=a+1
wend
msgbox a
```

**提示**：while ... wend是不能中途退出的

## 15、for ... next循环

```text
option explicit
dim a
a=0
dim b
b=0
for a=0 to 11
	b=b+1
	if a=5 then
		exit for
	end if
next
msgbox a
msgbox b
```

**提示**：for ... next 中，

1、条件必须是一个变量=val1 to val2这种形式，运作原理就是让这个变量依次等于范围内的值，比如上式中：a=0 to 11，表示先a=0，再a=1,a=2,a=3...a=11。

2、可以使用exit for直接跳出循环

3、在循环体内改变遍历条件对应变量的值会影响到次数，比如我在循环体里直接将a=12，那么就已经超出0~11了，此时就会退出循环。

4、正常退出循环后，依旧会对循环变量加1。但是如果使用exit for结束循环，则不会

**使用建议**：该循环比较适合用在定义一个下标变量去遍历数组

## 16、**for each...next**

```text
option explicit
dim i(5),j
i(0)="a"
i(1)="b"
i(2)="c"
i(3)="d"
i(4)="e"
i(5)="f"
for each j in i
msgbox j 
if j="d" then 
exit for
end if
next
```

**使用建议**：去遍历一个数组或者集合，可以使用exit for退出

## 17、算术运算符

加减乘都和其他语言一样使用，只列举比较特殊的

1.取余：10 mod 5 ；该表达式表示取10/5的余

2.除法：/表示浮点数除法，\表示取整除法

3.指数：2^3;2的三次方

## 18、逻辑运算符

1.相等：=

2.不等：<>

3.大于：>

4.小于：<

比较时的一些细节：当数字比数字时，就正常比较；数字比字符数字时，字符数字会被转换成数字比较；字符串比字符串，按ASCII的值，从第一位依次比较

5.非：not

6.与：and

7.或：or

8.异或：xor

## 19、sub过程

由sub...end sub 进行定义。如果 Sub 过程无任何参数，则 Sub 语句必须包含空括号 ()

不带参数的sub，定义时如果没有参数，可以不加

不带参数的过程

```text
Sub GetName()
    MsgBox "我是Sirrah" '输出字符串
End Sub
```

带参数的过程

```text
Sub GetName(name)
    MsgBox name '输出字符串
End Sub
```

**提示**：

sub的定义可以在脚本文件的任意位置

sub过程是不可以有返回值的

**使用建议**：在定义过程的时候，不管有没有参数，统一都加上括号

## 20、function函数

由function...end function进行定义。如果 function过程无任何参数，则 function语句必须包含空括号 ()，定义时如果没有参数，可以不加。

不带参数的函数

```text
Function GetName()
    MsgBox "我是Sirrah" '输出字符串
End Function
```

带参数的函数

```text
Function GetName(name)
    MsgBox name '输出字符串
End Function
```

function的定义可以才脚本文件的任意位置

function过程是可以有返回值的，返回值的写法为：函数名=值；和其他语言的return不同

```text
Function GetName(name)
    GetName=123 '返回值
End Function
```

**使用建议**：在定义函数的时候，不管有没有参数，统一都加上括号

## 21、调用函数和过程

call：如果无参数，可以不加括号；如果有参数，则必须加括号

```text
Call GetName() '调用Function
Call GetName() '调用Sub过程
```

直接调用：如果没有或只有一个参数，可以加括号也可以不加。如果有两个以上，不可以加括号

```text
GetName '直接调用
```

获取返回值：如果不存在参数，可加括号也可以不加，如果存在在参数，必须加括号；不能使用call来获得返回值

**使用建议**：使用call加括号；直接调用不加括号，需要获取返回值加括号

## 22、类

1.类的定义和声明

```text
Set MyUser = New User '在强制声明的情况下，需要先dim声明变量
Class User
    'Initialize事件相当于构造函数
    Private Sub Class_Initialize  '当这个类被创建时执行
        MsgBox "类开始"
    End Sub
    'Terminate事件相当于析构函数
    Private Sub Class_Terminate  '当类被销毁时执行
        MsgBox "类结束"
        End Sub
End Class
```

2、类结构

```text
Class User
    Private pri_var	'私有属性
    public pub_var 	'公共属性

    Private Sub Class_Initialize	'构造过程
    End Sub

    Private Sub Class_Terminate	'析构过程
    End Sub

    Public Property Get get_xx 	'故有的属性get，与function类似
    End Property  
 
    Public Property Let let_xx	'故有的属性let
    End Property  
 
    Public Sub ToString()		'自定义过程
    End Sub
	
    Private function getString()	'自定义函数
    end function
End Class
```

3、类的实例创建和释放

```text
set obj = new object '声明
set obj =nothing '释放
```

4、当一个对象不被引用时，会自动释放

## 23、文件系统对象**FSO（FileSystemObject）**

1、创建对象

```text
Dim fso
Set fso = wscript.createobject("scripting.filesystemobject")
```

2、常用方法，已声明Set fso = wscript.createobject("scripting.filesystemobject")

**CreateTextFile：**fso.CreateTextFile"C:\test.txt",true  //创建文件，第二个参数表示是否强制覆盖，true为覆盖

**CreateFolder：**Fso.CreateFolder "c:\test"  //创建文件夹，只有一个参数

**DeleteFile：**Fso.DeleteFile "c:\test.txt", true //删除文件，第二个参数用于强制删除只读文件时，true为强制删除

**DeleteFolder：**Fso.DeleteFolder "c:\test",True//删除文件夹，第二个参数用于强制删除只读文件夹，true为强制删除

**FileExists：**Fso.FileExists("c:\test.txt") //是否存在文件，返回布尔值

**FolderExists：**Fso.FolderExists("c:\test") //是否存在文件夹

**CopyFile：**Fso.CopyFile "c:\test.txt","D:\",True //复制文件第一个参数源文件路径，第二个参数目标目录路径，第三个参数，是否强制覆盖

**CopyFolder：**Fso.CopyFolder "c:\test","D:\",False //复制目录第一个参数源目录路径，第二个参数目标目录路径，第三个参数，是否强制覆盖

**MoveFile：**Fso.MoveFile "c:\test.txt","D:\" //移动文件，第一个参数源文件路径，第二个参数目标目录

**MoveFolder：**Fso.MoveFolder "c:\test","D:\" //移动目录，第一个参数源目录路径，第二个参数目标目录

**GetExtensionName：**fso.GetExtensionName("c:\test.txt")//获取后缀名

**GetBaseName：**Fso.GetBaseName("c:\test\test.txt") //获得文件名，不含后缀

**GetParentFolderName：**Fso.GetParentFolderName("c:\test\test.txt")//获取文件当前路径

**OpenTextFile：**Fso.opentextfile("filename"[,iomode[,create[,  format]]])//返回一个textstream对象，第一个参数文件名，第二个参数为io模式，1为只读，2为覆盖写，8为在末尾写，第三个参数表示不存在时是否创建，第四个为编码格式，这里不做赘述，到时候直接查

## 24、使用控制台运行jar包

```text
dim ws
set ws = createobject("Wscript.Shell")
ws.run "cmd /k java -jar .\youApp.jar"
```

## 总结：

至此，VBS的一些简单的语法入门就已经介绍完了。感谢各位的阅读！