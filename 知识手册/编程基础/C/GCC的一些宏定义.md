https://www.cnblogs.com/zl-yang/p/9212569.html

##              [     GCC强制静态库链接未使用的函数、变量      ](https://www.cnblogs.com/zl-yang/p/9212569.html)         

使用cmake构建嵌入式程序工程时遇到的问题，在此记录以下。

工程框架想把芯片相关的功能抽象出一个hal层，向上屏蔽芯片的API。
 每个芯片相关文件会输出一个hal库。
 APP相关代码编译后与hal库链接，生成最终的elf可执行文件。

不同于一般在.S汇编文件中编写中断向量表及中断处理函数，
 我采用的实现为，中断向量表是一个在c文件中定义的数组__isr_vectors，通过gcc的__attribute__(section)以及配置自定义的链接脚本实现代码定位。

由于__isr_vectors以及中断处理函数链接到静态库libhal.a后，在app代码中未被调用，因此最终生成的可执行文件中，这些符号被自动优化掉了。

1. 解决方法
    增加链接选项-Wl,--whole-archive
    在需要保留所有符号的库前增加-Wl,--whole-archive，然后增加-Wl,--no-whole-archive还原
    如：
    修改前:

:arm-none-eabi-gcc main.c -L. -lhal --specs=nosys.specs  -T mem.ld -T sections.ld
 :arm-non-eabi-nm a.out

结果显示没有__isr_vectors符号
 修改后:

arm-none-eabi-gcc main.c -L. -Wl,--whole-archive -lhal -Wl,--no-whole-archive --specs=nosys.specs -T mem.ld -T sections.ld
 arm-non-eabi-nm a.out

可以看到__isr_vectors已经存在

1. 在cmake中添加链接选项

TARGET_LINK_LIBRARIES(app
 -Wl,--whole-archive
 hal
 -Wl,--no-whole-archive
 )

​    https://github.com/pswarfound
