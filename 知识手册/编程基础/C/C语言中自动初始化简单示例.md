 

linux下使用readelf -S prog, 可以看到一张elf格式的表。表中以.开头的是一个一个的段。 __attribute((section(name)))的作用就是将被该属性修饰的符号都放到名为name的段中。

示例代码main.c中的_func1与_func2被__section宏修饰，最终都会被放到自定义的名为.app_init_sec段中。
~~~c
    #include <stdio.h>
    #include <string.h>
     
    #define __section __attribute((section(".app_init_sec")))
     
    typedef struct init_t{
            int (*func)(void);
            char *name;
    }_init_t;
     
    _init_t __app_init_start__;
    _init_t __app_init_end__;
     
    static int func1(void)
    {
            printf("call %s\n", __FUNCTION__);
            return 0;
    }
    _init_t _func1 __section = {func1, "func1"};

    static int func2(void)
    {
            printf("call %s\n", __FUNCTION__);
            return 0;
    }
    _init_t _func2 __section = {func2, "func2"};
     
    int main(int argc, char **argv)
    {
            _init_t *p;
     
            for(p = &__app_init_start__; p < &__app_init_end__; p++){
                    printf("==%s\n", p->name);
                    p->func();
            }
     
            return 0;
    }
~~~
也许你已经注意到.app_init_sec段尚未定义，且__app_init_start__与__app_init_end__未做初始化就直接使用了。这是因为这些工作都是在一个.lds格式的文件中完成的。一个简单的.lds文件内容如下所示：
~~~c
    //script.lds
    SECTIONS
    {
        . = ALIGN(16);
        __app_init_start__ = .;
        .app_init_sec : { *(.app_init_sec) }
        __app_init_end__ = .;
    }
    INSERT AFTER .rodata
~~~
为了编译出可执行的文件，需要运行如下的命令：

gcc main.c -T script.lds -g -o prog

 

运行程序prog, 输出如下所示：
~~~
[alan@localhost Test]$ ./prog
==func1
call func1
==func2
call func2
[alan@localhost Test]$
~~~


运行命令readelf -S prog查看elf表,可以在.rodata后找到自定义的.app_init_sec段：
~~~
[14] .text             PROGBITS         00000000004003c0  000003c0
       00000000000001f2  0000000000000000  AX       0     0     16
  [15] .fini             PROGBITS         00000000004005b4  000005b4
       0000000000000009  0000000000000000  AX       0     0     4
  [16] .rodata           PROGBITS         00000000004005c0  000005c0
       0000000000000037  0000000000000000   A       0     0     8
  [17] .app_init_sec     PROGBITS         0000000000400600  00000600
       0000000000000020  0000000000000000  WA       0     0     16
~~~
————————————————
版权声明：本文为CSDN博主「ByteOder」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_38006908/article/details/102504476
