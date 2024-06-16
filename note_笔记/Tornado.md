       机器学习模型训练完成后，要考虑模型部署上线，模型在线预估。一种实现方式时，本地起一个python web服务，把模型文件加载到该服务，外界可以通过请求web服务得到模型预估结果。

       python有很多优秀的web服务框架，本文主要选择其中一种性能较高的tornado框架进行介绍，即通过tornado框架构建服务，本地起服务并访问，读者可以通过下文的示例代码深入理解。


图片

示例代码 | code


import tornado.ioloop
import tornado.web
from tornado.options import define, options, parse_command_line   # 解析服务启动命令行中的端口参数

# 定义默认启动的端口port为8080
define('port', default=8080, type=int)


# 定义执行方法，处理相应的请求方式
class ResHandler(tornado.web.RequestHandler):
    def get(self):   # 针对get请求调用的方法，用浏览器请求 是调用get方法
        name = self.get_arguments('name')   # 接收请求参数
        self.write('<h2>%s极客社群</h2>' % name[0])

    def post(self):   # 针对post请求调用的方法
        name = self.get_arguments('name')
        self.write("hello tornado post user %s" % name)   # 渲染响应(后端返回给前端的)给浏览器的数据

class DaysHandler(tornado.web.RequestHandler):
    def get(self, month, year, day):   # 获取路由参数
        self.write('%s年%s月%s日' % (year, month, day))


# 定义App应用，在应用函数的handlers参数下 配置路由
def make_app():
    return tornado.web.Application(handlers=[
        (r"/", ResHandler),
        (r"/days/(?P<year>\d{4})/(?P<month>\d{2})/(?P<day>\d{2})/", DaysHandler)
        # days路由 接收后面的正则日期参数。指定第一个参数的参数名为year，第二个参数的参数名为month，第三个参数的参数名为day
    ])
    # handlers参数 定义路由匹配列表，即(路由地址， 路由调XXHandler里的get/post等方法 响应页面)


if __name__ == '__main__':
    # 解析服务启动命令参数
    parse_command_line()
    # 启动端口
    app = make_app()
    # 监听端口，从解析的命令参数里获取端口
    app.listen(options.port)

    # 创建一个ioloop，用于监听启动的IO端口
    establish = tornado.ioloop.IOLoop.current()
    establish.start()


上面的示例代码定义了两个不同的访问地址：

A.可以访问

http://127.0.0.1:8080?name=瑞行AI

图片

B.也可以访问

http://127.0.0.1:8080/days/2019/03/11/

图片


图片

响应页面渲染


       简单介绍下，如何对tornado服务的请求响应页面进行渲染，即渲染响应页面的标题、字体等呈现。首先，在当前要起服务的python脚本同级目录 建立文件夹templates，在templates文件夹下 创建渲染页面使用的文件index.html，定义文件内容，如下：

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>
        瑞行AI
    </title>
</head>
<body>
    瑞行AI-tornado框架教学
</body>
</html>

接下来，给出调用渲染文件index.html的起服务的python脚本代码：

import tornado.ioloop
import tornado.web
from tornado.options import define, options, parse_command_line   # 解析服务启动命令行中的端口参数
import os

# 定义默认启动的端口port为8080
define('port', default=8080, type=int)


class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('index.html')   # 要对名字为index.html的页面进行渲染(响应)


# 定义App应用，在应用函数的handlers参数下 配置路由
def make_app(tp_path):
    return tornado.web.Application(
        handlers=[(r"/index/", IndexHandler)],   # handlers参数 定义路由匹配列表，即(路由地址，路由调EntryHandler里的get()函数处理规则)
        template_path=tp_path   # 指定渲染模版index.html文件所在路径
    )


if __name__ == '__main__':
    abs_path = os.path.dirname(os.path.abspath(__file__))  # 当前文件所在目录的绝对路径
    tp_path = os.path.join(abs_path, 'templates')
    print(tp_path)

    # 解析服务启动命令参数
    parse_command_line()
    # 启动端口
    app = make_app(tp_path)
    # 监听端口，从解析的命令参数里获取端口
    app.listen(options.port)

    # 创建一个ioloop，用于监听启动的IO端口
    establish = tornado.ioloop.IOLoop.current()
    establish.start()


运行上面的示例代码后，成功启动服务后即可访问

http://127.0.0.1:8080/index/

图片

        到此为止，关于tornado框架起服务的基本内容就介绍完了，该框架的更多使用细节，推荐访问以下文档：

1.)英文参考文档

https://www.tornadoweb.org/en/stable/

2.)中文参考文档

http://shouce.jb51.net/tornado/