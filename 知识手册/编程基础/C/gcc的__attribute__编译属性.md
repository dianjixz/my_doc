 

要了解Linux  Kernel代码的分段信息，需要了解一下gcc的__attribute__的编绎属性，__attribute__主要用于改变所声明或定义的函数或数据的特性，它有很多子项，用于改变作用对象的特性。比如对函数，noline将禁止进行内联扩展、noreturn表示没有返回值、pure表明函数除返回值外，不会通过其它（如全局变量、指针）对函数外部产生任何影响。但这里我们比较感兴趣的是对代码段起作用子项section。 

__attribute__的section子项的使用格式为： 

  __attribute__((section("section_name"))) 

  其作用是将作用的函数或数据放入指定名为"section_name"输入段。 

https://blog.csdn.net/Fate_Dream/article/details/53809550





# 指定函数、变量到某个特定的section段

在[函数声明](https://so.csdn.net/so/search?q=函数声明&spm=1001.2101.3001.7020)时：

char func()__attribute__((section(".my_section")));

链接脚本：

.my_section : {undefined

*.o (.my_section)
 }>ram





# attribute 用法 section 部分

https://blog.csdn.net/weixin_34205826/article/details/92752848?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2.pc_relevant_paycolumn_v3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2.pc_relevant_paycolumn_v3&utm_relevant_index=5
