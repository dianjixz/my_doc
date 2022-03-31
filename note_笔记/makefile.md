 

makefile 里如果用echo进行打印的话必须放在标签之后，否则会出现语法错误。

用以下语句在任意地方打印

1，$(info "info here")，输出时不带行号

 

2， $(warning "CFLAGS = $(CFLAGS)")， 带行号

 

3，$(error "CFLAGS = $(CFLAGS)")，带行号，makefile直接停止执行
————————————————
版权声明：本文为CSDN博主「原十」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/tianyexing2008/article/details/99079114

