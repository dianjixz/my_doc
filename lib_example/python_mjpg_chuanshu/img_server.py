#!/bin/env python3
'''
使用 http 的 multipart/x-mixed-replace 协议做一个简单的图像传输。可用于一个临时的视频传输,但是不能传输音频.
作者: dianjixz
2022年5月
'''

import http.server
import time

import signal
def handle_signal_z(signum,frame):
    exit(0)
signal.signal(signal.SIGINT,handle_signal_z)

request_headers = "\
Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\n\
Connection: close\n\
Content-Type: multipart/x-mixed-replace;boundary=--boundarydonotcross\n\
Expires: Mon, 1 Jan 2030 00:00:00 GMT\n\
Pragma: no-cache\n\
Access-Control-Allow-Origin: *\n\
"

'''
--boundarydonotcross
X-Timestamp: 591.595504114
Content-Length: 3990
Content-Type: image/jpeg

....
'''

img_headers = "\n\
--boundarydonotcross\n\
X-Timestamp: {}\n\
Content-Length: {}\n\
Content-Type: image/jpeg\n\
"

class RequestHandlerImpl(http.server.BaseHTTPRequestHandler):
    """
    自定义一个 HTTP 请求处理器
    """
    
    def do_GET(self):
        """
        处理 GET 请求, 处理其他请求需实现对应的 do_XXX() 方法
        """
        # print(self.server)                # HTTPServer 实例
        # print(self.client_address)        # 客户端地址和端口: (host, port)
        # print(self.requestline)           # 请求行, 例如: "GET / HTTP/1.1"
        # print(self.command)               # 请求方法, "GET"/"POST"等
        # print(self.path)                  # 请求路径, Host 后面部分
        # print(self.headers)               # 请求头, 通过 headers["header_name"] 获取值
        # self.rfile                        # 请求输入流
        # self.wfile                        # 响应输出流

        # 1. 发送响应code
        self.send_response(200)

        # 2. 发送响应头
        # self.send_header(request_headers.encode("utf-8"))
        # self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Cache-Control", "no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0")
        self.send_header("Connection", "close")
        self.send_header("Content-Type", "multipart/x-mixed-replace;boundary=--boundarydonotcross")
        self.send_header("Expires", "Mon, 1 Jan 2030 00:00:00 GMT")
        self.send_header("Pragma", "no-cache")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        # 3. 发送响应内容（此处流不需要关闭）
        from maix import camera
        import _maix
        while True:
            img = camera.capture()
            jpg = _maix.rgb2jpg(img.convert("RGB").tobytes(), img.width, img.height)
            self.wfile.write(img_headers.format(time.time(), len(jpg)).encode("utf-8"))
            self.wfile.write("\n".encode("utf-8"))
            self.wfile.write(jpg)
            # self.wfile.write("Hello World\n".encode("utf-8"))

    def do_POST(self):
        """
        处理 POST 请求
        """
        # 0. 获取请求Body中的内容（需要指定读取长度, 不指定会阻塞）
        req_body = self.rfile.read(int(self.headers["Content-Length"])).decode()
        print("req_body: " + req_body)

        # 1. 发送响应code
        self.send_response(200)

        # 2. 发送响应头
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.end_headers()

        # 3. 发送响应内容（此处流不需要关闭）
        self.wfile.write(("Hello World: " + req_body + "\n").encode("utf-8"))


# 服务器绑定的地址和端口
server_address = ("", 8000)

# 创建一个 HTTP 服务器（Web服务器）, 指定绑定的地址/端口 和 请求处理器
httpd = http.server.HTTPServer(server_address, RequestHandlerImpl)
# httpd = http.server.ThreadingHTTPServer(server_address, RequestHandlerImpl)

# 循环等待客户端请求
httpd.serve_forever()

# 本地浏览器访问:      http://localhost:8000
# curl命令 GET 访问:  curl http://localhost:8000/hello/world
# curl命令 POST 访问: curl http://localhost:8000/hello/world -d "name=tom&age=25"
