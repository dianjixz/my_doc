## [inotify-tools命令使用讲解](https://www.cnblogs.com/wajika/p/6396748.html)

关键字：监控文件 文件监控 监控目录 目录监控


inotify-tools 是为linux下inotify文件监控工具提供的一套c的开发接口库函数，同时还提供了一系列的命令行工具，这些工具可以用来监控文件系统的事件。 inotify-tools是用c编写的，除了要求内核支持inotify外，不依赖于其他。inotify-tools提供两种工具，一是 inotifywait，它是用来监控文件或目录的变化，二是inotifywatch，它是用来统计文件系统访问的次数。现在介绍一下它的使用方法。

安装方法：



```bash
wget http://github.com/downloads/rvoicilas/inotify-tools/inotify-tools-3.14.tar.gz 
tar xzf inotify-tools-3.14.tar.gz ;
cd inotify-tools-3.14
./configure --prefix=/usr && make && su -c 'make install'
```

使用例子：





#### inotifywait

1、实时监控/home的所有事件（包括文件的访问，写入，修改，删除等）

~~~ bash
inotifywait -rm /home
~~~

2、监控/var/log/messeges中有关httpd的日志



~~~ bash
#!/bin/sh
while inotifywait -e modify /var/log/messages; do  
	if tail -n1 /var/log/messages |  grep httpd;  then 
		kdialog --msgbox  "Apache needs love!"  
	fi 
done
~~~

#### inotifywatch

1、统计/home文件系统的事件

 

```
inotifywatch -``v` `-e access -e modify -t 60 -r ``/home
```

 

### 参数说明

#### inotifywait

语法：
inotifywait [-hcmrq] [-e ] [-t ] [--format ] [--timefmt ] [ ... ]
参数：
**-h,–help**
输出帮助信息
**@**
排除不需要监视的文件，可以是相对路径，也可以是绝对路径。
**–fromfile** 
从文件读取需要监视的文件或排除的文件，一个文件一行，排除的文件以@开头。
**-m, –monitor**
接收到一个事情而不退出，无限期地执行。默认的行为是接收到一个事情后立即退出。
**-d, –daemon**
跟–monitor一样，除了是在后台运行，需要指定–outfile把事情输出到一个文件。也意味着使用了–syslog。
**-o, –outfile** 
输出事情到一个文件而不是标准输出。
**-s, –syslog**
输出错误信息到系统日志
**-r, –recursive**
监视一个目录下的所有子目录。
**-q, –quiet**
指定一次，不会输出详细信息，指定二次，除了致命错误，不会输出任何信息。
**–exclude** 
正则匹配需要排除的文件，大小写敏感。
**–excludei** 
正则匹配需要排除的文件，忽略大小写。
**-t , –timeout** 
设置超时时间，如果为0，则无限期地执行下去。
**-e , –event** 
指定监视的事件。
**-c, –csv**
输出csv格式。
**–timefmt** 
指定时间格式，用于–format选项中的%T格式。
**–format** 
指定输出格式。
%w 表示发生事件的目录
%f 表示发生事件的文件
%e 表示发生的事件
%Xe 事件以“X”分隔
%T 使用由–timefmt定义的时间格式

#### inotifywatch

语法：
inotifywatch [-hvzrqf] [-e ] [-t ] [-a ] [-d ] [ ... ]
参数：
**-h, –help**
输出帮助信息
**-v, –verbose**
输出详细信息
**@**
排除不需要监视的文件，可以是相对路径，也可以是绝对路径。
**–fromfile** 
从文件读取需要监视的文件或排除的文件，一个文件一行，排除的文件以@开头。
**-z, –zero**
输出表格的行和列，即使元素为空
**–exclude** 
正则匹配需要排除的文件，大小写敏感。
**–excludei** 
正则匹配需要排除的文件，忽略大小写。
**-r, –recursive**
监视一个目录下的所有子目录。
**-t , –timeout** 
设置超时时间
**-e , –event** 
只监听指定的事件。
**-a , –ascending** 
以指定事件升序排列。
**-d , –descending** 
以指定事件降序排列。

#### 可监听事件

 

| access        | 文件读取                                                     |
| ------------- | ------------------------------------------------------------ |
| modify        | 文件更改。                                                   |
| attrib        | 文件属性更改，如权限，时间戳等。                             |
| close_write   | 以可写模式打开的文件被关闭，不代表此文件一定已经写入数据。   |
| close_nowrite | 以只读模式打开的文件被关闭。                                 |
| close         | 文件被关闭，不管它是如何打开的。                             |
| open          | 文件打开。                                                   |
| moved_to      | 一个文件或目录移动到监听的目录，即使是在同一目录内移动，此事件也触发。 |
| moved_from    | 一个文件或目录移出监听的目录，即使是在同一目录内移动，此事件也触发。 |
| move          | 包括moved_to和 moved_from                                    |
| move_self     | 文件或目录被移除，之后不再监听此文件或目录。                 |
| create        | 文件或目录创建                                               |
| delete        | 文件或目录删除                                               |
| delete_self   | 文件或目录移除，之后不再监听此文件或目录                     |
| unmount       | 文件系统取消挂载，之后不再监听此文件系统。                   |

