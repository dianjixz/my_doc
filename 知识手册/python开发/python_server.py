from http.server import BaseHTTPRequestHandler, HTTPServer
import mimetypes

class MyRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # 获取请求的文件路径
        file_path = self.path[1:]  # 去掉路径前面的斜杠 "/"

        try:
            # 获取文件的 MIME 类型
            content_type, _ = mimetypes.guess_type(self.path[1:])

            # 设置响应头的 Content-Type
            self.send_response(200)
            self.send_header('Content-Type', content_type)
            self.end_headers()

            # 打开并读取文件内容
            with open(file_path, 'rb') as file:
                self.wfile.write(file.read())
        except FileNotFoundError:
            # 文件不存在时返回 404 错误
            self.send_error(404, 'File Not Found')

# 创建 HTTP 服务器
server_address = ('', 8000)
httpd = HTTPServer(server_address, MyRequestHandler)
print('Server running on http://localhost:8000')

# 启动服务器
httpd.serve_forever()
