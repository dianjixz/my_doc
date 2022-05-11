#!/bin/env python3
import html
import http.server
import mimetypes
import os
import platform
import posixpath
import re
import shutil
import socket
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
from socketserver import ThreadingMixIn
from io import StringIO


class GetWanIp:
    def getip(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))
        ip = s.getsockname()[0]
        s.close()
        return ip

    def visit(self, url):
        opener = urllib.request.urlopen(url, None, 3)
        str = ''
        if url == opener.geturl():
            str = opener.read()
        return re.search(r'(\d+\.){3}\d+', str).group(0)


def showTips():
    print('----------------------------------------------------------------------- ')
    port = ''
    try:
        port = int(sys.argv[1])
    except Exception as e:
        print('未指定监听端口，将使用默认端口: 8080 ')
        print('您可以带上端口号执行本模块以更改监听端口，例: ')
        print('python HTTPServerWithUpload.py port ')
        port = 8080

    if not 1024 < port < 65535:
        print(f'{port}不在有效的端口范围内，有效范围1024~65535，将使用默认端口号8080...')
        port = 8080
    osType = platform.system()
    ip_address = f'http://{GetWanIp().getip()}:{port}'
    if osType == "Windows":
        print(f'现在您可以访问地址URL: {ip_address} 以访问共享目录.')
    else:
        print(f'现在您可以访问地址URL: {ip_address} 以访问共享目录.')

    return ('', port)


def sizeof_fmt(num):
    for x in ['bytes', 'KB', 'MB', 'GB']:
        if num < 1024.0:
            return "%3.1f%s" % (num, x)
        num /= 1024.0
    return "%3.1f%s" % (num, 'TB')


def modification_date(filename):
    return time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(os.path.getmtime(filename)))


class SimpleHTTPRequestHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        f = self.send_head()
        if f:
            for i in f.readlines():
                if isinstance(i, str):
                    self.wfile.write(i.encode("utf-8"))
                else:
                    self.wfile.write(i)
            f.close()

    def do_HEAD(self):
        f = self.send_head()
        if f:
            f.close()

    # 上传结果处理
    def do_POST(self):
        r, info = self.deal_post_data()
        print(r, info, "by: ", self.client_address)
        f = StringIO()
        f.write('<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">')
        f.write('<meta name="viewport" content="width=device-width" charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">')
        f.write("<html>\n<title>上传结果</title>\n")
        f.write("<body>\n<h2>文件上传</h2>")
        if r:
            f.write('<strong style="color:#00FF00">成功</strong>\n')
        else:
            f.write('<strong style="color:#FF0000">失败</strong>\n')
        f.write("<hr>\n")
        f.write(info)
        f.write("</br><a href=\"%s\">点击返回</a>" % self.headers['referer'])
        f.write("<hr><small>Powered By: gaowanliang                       ")
        f.write("</small></body>\n</html>\n")
        length = f.tell()
        f.seek(0)
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Content-Length", str(length))
        self.end_headers()
        if f:
            for i in f.readlines():
                self.wfile.write(i.encode("utf-8"))
            f.close()

    def deal_post_data(self):
        boundary = str(
            self.headers["Content-Type"].split("=")[1]).encode("utf-8")
        remainbytes = int(self.headers['Content-length'])
        line = self.rfile.readline()
        remainbytes -= len(line)
        if not boundary in line:
            return (False, "Content NOT begin with boundary")
        line = self.rfile.readline()
        remainbytes -= len(line)
        fn = re.findall(
            r'Content-Disposition.*name="file"; filename="(.*)"'.encode('utf-8'), line)
        if not fn:
            return (False, "Can't find out file name...")
        path = str(self.translate_path(self.path)).encode('utf-8')
        osType = platform.system()
        try:
            if osType == "Linux":
                fn = os.path.join(path, fn[0].decode('gbk').encode('utf-8'))
            else:
                fn = os.path.join(path, fn[0])
        except Exception as e:
            return (False, "文件名请不要用中文，或者使用IE上传中文名的文件。{}" .format(e))
        while os.path.exists(fn):
            fn += "_".encode("utf-8")
        line = self.rfile.readline()
        remainbytes -= len(line)
        line = self.rfile.readline()
        remainbytes -= len(line)
        try:
            out = open(fn, 'wb')
        except IOError:
            return (False, "Can't create file to write, do you have permission to write?")

        preline = self.rfile.readline()
        remainbytes -= len(preline)
        while remainbytes > 0:
            line = self.rfile.readline()
            remainbytes -= len(line)
            if boundary in line:
                preline = preline[0:-1]
                if preline.endswith('\r'.encode("utf-8")):
                    preline = preline[0:-1]
                out.write(preline)
                out.close()
                return (True, "文件 '%s' 上传成功" % fn)
            else:
                out.write(preline)
                preline = line
        return (False, "Unexpect Ends of data.")

    def send_head(self):
        path = self.translate_path(self.path)
        f = None
        if os.path.isdir(path):
            if not self.path.endswith('/'):
                self.send_response(301)
                self.send_header("Location", self.path + "/")
                self.end_headers()
                return None
            for index in "index.html", "index.htm":
                index = os.path.join(path, index)
                if os.path.exists(index):
                    path = index
                    break
            else:
                return self.list_directory(path)
        ctype = self.guess_type(path)
        try:
            f = open(path, 'rb')
        except IOError:
            self.send_error(404, "File not found")
            return None
        self.send_response(200)
        self.send_header("Content-type", ctype)
        fs = os.fstat(f.fileno())
        self.send_header("Content-Length", str(fs[6]))
        self.send_header("Last-Modified", self.date_time_string(fs.st_mtime))
        self.end_headers()
        return f

    def list_directory(self, path):
        try:
            list = os.listdir(path)
        except os.error:
            self.send_error(404, "No permission to list directory")
            return None
        list.sort(key=lambda a: a.lower())
        f = StringIO()
        displaypath = html.escape(urllib.parse.unquote(self.path))
        dirs = ''
        for name in list:
            fullname = os.path.join(path, name)
            colorName = displayname = linkname = name
            if os.path.isdir(fullname):
                colorName = '<span style="background-color: #CEFFCE;">' + name + '/</span>'
                displayname = name
                linkname = name + "/"
            if os.path.islink(fullname):
                colorName = '<span style="background-color: #FFBFFF;">' + name + '@</span>'
                displayname = name
            filename = os.getcwd() + '/' + displaypath + displayname
            dirs += f"""
                    <tr>
                        <td width='60%'><a href='{urllib.parse.quote(linkname)}'>{colorName}</a>
                        </td><td width='20%'>{sizeof_fmt(os.path.getsize(filename))}</td>
                        <td width='20%'>{modification_date(filename)}</td>
                    </tr>
                """
        script_code = """
        
        """
        content = f"""
                <!DOCTYPE html>
                <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <title>Title</title>
                    </head>
                    <body>
                        <h2>目录清单 位于{displaypath}</h2>
                        <hr>
                        <form ENCTYPE="multipart/form-data" method="post">
                            <input name="file" type="file"/>
                            <input type="submit" value="上传"/>
                            <input type="button" value="主目录" onClick="location='/'">
                        </form>
                        <h2 style="color:#FF0000">请先选择完文件再点上传，不这样做的话可能会出现奇怪的情况</h2>
                        <hr>
                        <table id='wrap'>
                            {dirs}
                        </table>
                        <hr>
                    </body>
                    {script_code}
                </html>
                """
        f.write(content)
        length = f.tell()
        f.seek(0)
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Content-Length", str(length))
        self.end_headers()
        return f

    def translate_path(self, path):
        path = path.split('?', 1)[0]
        path = path.split('#', 1)[0]
        path = posixpath.normpath(urllib.parse.unquote(path))
        words = path.split('/')
        words = [_f for _f in words if _f]
        path = os.getcwd()
        for word in words:
            drive, word = os.path.splitdrive(word)
            head, word = os.path.split(word)
            if word in (os.curdir, os.pardir):
                continue
            path = os.path.join(path, word)
        return path

    def copyfile(self, source, outputfile):
        shutil.copyfileobj(source, outputfile)

    def guess_type(self, path):

        base, ext = posixpath.splitext(path)
        if ext in self.extensions_map:
            return self.extensions_map[ext]
        ext = ext.lower()
        if ext in self.extensions_map:
            return self.extensions_map[ext]
        else:
            return self.extensions_map['']

    if not mimetypes.inited:
        mimetypes.init()  # try to read system mime.types
    extensions_map = mimetypes.types_map.copy()
    extensions_map.update({
        '': 'application/octet-stream',  # Default
        '.py': 'text/plain',
        '.c': 'text/plain',
        '.h': 'text/plain',
    })


class ThreadingServer(ThreadingMixIn, http.server.HTTPServer):
    pass


if __name__ == '__main__':
    serveraddr = showTips()
    srvr = ThreadingServer(serveraddr, SimpleHTTPRequestHandler)
    srvr.serve_forever()


