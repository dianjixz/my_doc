文章目录

                1、WSGI协议
                2、gunicorn介绍
                3、gunicorn安装
                4、gunicorn使用
                    4.1 基于Flask创建python服务
                    4.2 配置参数、启动应用服务
                        4.2.1 命令行配置gunicorn参数
                        4.2.2 文件配置gunicorn参数
                5、访问web服务器
                6、结束gunicorn服务进程

1、WSGI协议

Web框架致力于如何生成HTML代码，而Web服务器用于处理和响应HTTP请求。Web框架和Web服务器之间的通信，需要一套双方都遵守的接口协议。WSGI协议就是用来统一这两者的接口的。
2、gunicorn介绍

我们知道，python是单线程的语言，当进程阻塞时，后续请求将排队处理。

gunicorn是一个python WSGI http server，其优势在于它使用了pre-fork worker模式。gunicorn在启动时，会在主进程中预先fork出指定数量的worker进程来处理请求，依靠操作系统来提供负载均衡。
3、gunicorn安装

gunicorn安装非常简单，使用命令pip install gunicorn即可：
```
pip install gunicorn
```

一般使用它，主要是为使用其异步的worker模型，还需要安装对应的异步模块：
```
pip install greenlet # 使用异步必须安装
pip install eventlet # 使用eventlet workers
pip install gevent   # 使用gevent workers
```

4、gunicorn使用
4.1 基于Flask创建python服务

这里使用gunicorn来部署一个flask项目举例，此处flask框架的使用不过多阐述，不是本文的重点。

如下例子，保存为app.py
```
from flask import Flask
app = Flask(__name__)

@app.route("/hello")
def hello():
    return "Hello World!"

@app.route("/")
def nihao():
    return "nihao!"

if __name__ == '__main__':
    app.run()
```

注: 建议将应用程序运行所需的所有Python文件都放在顶层文件夹apps中。
将主入口程序命名为 app.py，将脚本中创建的Flask对象命名为 app (对应于部署命令 gunicorn -c gunicorn/config.py app:app 中的 app:app )。
```
FlaskApp  
    └── apps
        └── < .py files>
    └── gunicorn
        └── config.py
    ├── app.py

```

4.2 配置参数、启动应用服务

gunicorn通常使用的参数如下：
```
1)-c CONFIG,--config=CONFIG
指定一个配置文件（py文件）

2)-b BIND,--bind=BIND
设定服务需要绑定的端口。建议使用HOST:PORT。

3)-D,--daemon
后台进程方式运行gunicorn进程

4)-w WORKERS,--workers=WORKERS
工作进程的数量

5)-k WORKERCLASS,--worker-class=WORKERCLASS
工作进程类型，包括sync（默认）,eventlet,gevent,tornado,gthread,gaiohttp

6)--backlog INT
最大挂起的连接数

7)--log-level LEVEL
日志输出等级

8)--access-logfile FILE
访问日志输出文件

9)--error-logfile FILE
错误日志输出文件
```


4.2.1 命令行配置gunicorn参数

当所需配置的参数较少时，我们可以直接在shell中输入启动配置，比如：
```
gunicorn -w 3 -b 127.0.0.1:5000 app:app
```

此处app:app中，第一个app为flask项目实例所在的包，第二个app为生成的flask项目实例。
4.2.2 文件配置gunicorn参数

当参数较多或者不方便使用命令行时，我们也可以将参数写在配置文件中，下面举列说明配置文件 gunicorn/config.py 的写法。
```
bind = "127.0.0.1:5000"
workers = 2
```

运行以下命令：
```
gunicorn -c gunicorn/config.py app:app
```

看到如下信息表明运行成功:
```
xxx@xxx:~/projects/project_name$ gunicorn -c gunicorn/config.py app:app
[2021-10-13 17:19:41 +0800] [19950] [INFO] Starting gunicorn 20.1.0
[2021-10-13 17:19:41 +0800] [19950] [INFO] Listening at: http://127.0.0.1:5000 (19950)
[2021-10-13 17:19:41 +0800] [19950] [INFO] Using worker: sync
[2021-10-13 17:19:41 +0800] [19968] [INFO] Booting worker with pid: 19968
```

也可以使用 nohup 使应用始终保持后台运行，并记录相应的日志信息：
```
nohup gunicorn -c  $(pwd)/config.py app:app &> ./logs/uwsgi.log &
```

运行结果和使用命令行参数结果一样。

gunicorn配置文件是一个python文件，因此可以实现更复杂的逻辑，如下：
```
# gunicorn.conf.py
import logging
import logging.handlers
from logging.handlers import WatchedFileHandler
import os
import multiprocessing

bind = '127.0.0.1:5000'  #绑定ip和端口号

backlog = 512 #监听队列
timeout = 30  # 响应耗时（单位为秒）

worker_class = 'gevent' #使用gevent模式，还可以使用sync 模式，默认的是sync模式
workers = multiprocessing.cpu_count() * 2 + 1    #进程数
threads = 2 #指定每个进程开启的线程数

loglevel = 'info' #日志级别，这个日志级别指的是错误日志的级别，而访问日志的级别无法设置
access_log_format = '%(t)s %(p)s %(h)s "%(r)s" %(s)s %(L)s %(b)s %(f)s" "%(a)s"'    #设置gunicorn访问日志格式，错误日志无法设置

"""
其每个选项的含义如下：
h          remote address
l          '-'
u          currently '-', may be user name in future releases
t          date of the request
r          status line (e.g. ``GET / HTTP/1.1``)
s          status
b          response length or '-'
f          referer
a          user agent
T          request time in seconds
D          request time in microseconds
L          request time in decimal seconds
p          process ID
"""
accesslog = "/log/access.log"      #访问日志文件
errorlog = "/log/error.log"        #错误日志文件
```


5、访问web服务器

访问地址：127.0.0.1:5000/hello
查看相关页面可查看是否启用成功。
在这里插入图片描述
6、结束gunicorn服务进程

使用 ps -ef | grep gunicorn 命令找出gunicorn所有进程。
```
[root@VM_0_12_centos ~] ps -ef|grep gunicorn
xxx   19950 17074  0 17:13 pts/2    00:00:00 /home/xxx/anaconda3/bin/python /home/xxx/anaconda3/bin/gunicorn -c gunicorn/config.py app:app
xxx   19968 19950  0 17:13 pts/2    00:00:00 /home/xxx/anaconda3/bin/python /home/xxx/anaconda3/bin/gunicorn -c gunicorn/config.py app:app
xxx   20264 20141  0 17:13 pts/1    00:00:00 grep --color=auto gunicorn

```

然后使用 kill -9 进程ID 命令来杀掉相关进程
```
[root@VM_0_12_centos ~] kill -9 19950
[root@VM_0_12_centos ~] kill -9 19968
[root@VM_0_12_centos ~] ps -ef | grep gunicorn

```

杀掉进程后，稍等几秒，再使用ps -ef | grep gunicorn查看，发现gunicorn服务进程已全部杀掉。

【参考博客】：

    https://zhuanlan.zhihu.com/p/88422780
    https://www.cnblogs.com/shijingjing07/p/9110619.html
————————————————

                            版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
                        
原文链接：https://blog.csdn.net/u012856866/article/details/119384985