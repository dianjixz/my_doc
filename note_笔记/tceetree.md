 

作者：知乎用户
链接：https://www.zhihu.com/question/59227720/answer/215384169
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



[amboar/tceetree](https://link.zhihu.com/?target=https%3A//github.com/amboar/tceetree)

用法： 

```text
#列出cscope需要分析的文件
find . -name "*.[c|h]" > cscope.files
#生成标签
cscope -Rcbkq
#分析标签，生成dot
tceetree -f -o filename.dot  # f选项带文件名
#生成图形 png svg pdf等，函数调用不建议png，放不下
dot -Tpdf -O filename.dot -o filename.pdf
```

下图是tceetree自己的函数调用（红蓝线是我自己修改**dot**加的）

![img](https://pic1.zhimg.com/50/v2-16ab0a07289c6e9272c96fffbd549777_720w.jpg?source=1940ef5c)![img](https://pic1.zhimg.com/80/v2-16ab0a07289c6e9272c96fffbd549777_720w.jpg?source=1940ef5c)

列一个复杂点的，git，是不是有点晕，你对力量一无所知:-) 你完全可以生成内核的，也可以只生成某几个文件的，修改cscope.files就好



![img](https://pica.zhimg.com/50/v2-c14a41e35d94dc72ec9441cd58f50ddb_720w.jpg?source=1940ef5c)![img](https://pica.zhimg.com/80/v2-c14a41e35d94dc72ec9441cd58f50ddb_720w.jpg?source=1940ef5c)

稍微放大点的

![img](https://pic1.zhimg.com/50/v2-4e7425f3a97fe2083725f2b9e0d254f3_720w.jpg?source=1940ef5c)![img](https://pic1.zhimg.com/80/v2-4e7425f3a97fe2083725f2b9e0d254f3_720w.jpg?source=1940ef5c)
