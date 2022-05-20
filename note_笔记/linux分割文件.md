1. 文件切割 - split

[1] 命令语法

``` bash
# -a: 指定输出文件名的后缀长度(默认为2个:aa,ab...)
# -d: 指定输出文件名的后缀用数字代替
# -l: 行数分割模式(指定每多少行切成一个小文件;默认行数是1000行)
# -b: 二进制分割模式(支持单位:k/m)
# -C: 文件大小分割模式(切割时尽量维持每行的完整性)
split [-a] [-d] [-l <行数>] [-b <字节>] [-C <字节>] [要切割的文件] [输出文件名]
```

[2] 使用实例

``` bash
# 行切割文件
$ split -l 300000 users.sql /data/users_
# 使用数字后缀
$ split -d -l 300000 users.sql /data/users_
# 按字节大小分割
$ split -d -b 100m users.sql /data/users_
```


2. 文件合并 - cat

[1] 命令语法

``` bash
# -n: 显示行号
# -e: 以$字符作为每行的结尾
# -t: 显示TAB字符(^I)
cat [-n] [-e] [-t] [输出文件名]
```

[2] 使用实例

``` bash
# 合并文件
$ cat /data/users_* > users.sql
```