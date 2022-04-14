import cgi
import html
import http.server
import mimetypes
import os
import platform
import posixpath
import re
import shutil
import socket
import subprocess
import sys
import threading
import time
import urllib.error
import urllib.parse
import urllib.request
import uuid
from socketserver import ThreadingMixIn

try:
    import numpy as np
except:
    os.system('pip install -i https://pypi.tuna.tsinghua.edu.cn/simple numpy ')
    import numpy as np

try:
    import qrcode
except:
    os.system('pip install -i https://pypi.tuna.tsinghua.edu.cn/simple qrcode ')
    import qrcode

try:
    import PIL
except:
    os.system('pip install -i https://pypi.tuna.tsinghua.edu.cn/simple pillow ')

try:
    from io import StringIO
except ImportError:
    from io import StringIO


class GetWanIp:
    def getip(self):
        import socket
        myname = socket.getfqdn(socket.gethostname())
        myaddr = socket.gethostbyname(myname)
        print(myaddr)
        return myaddr
    
    def visit(self, url):
        opener = urllib.request.urlopen(url, None, 3)
        if url == opener.geturl():
            str = opener.read()
        return re.search(r'(\d+\.){3}\d+', str).group(0)


def showTips():
    print("")
    print('----------------------------------------------------------------------->> ')
    try:
        port = int(sys.argv[1])
    except Exception as e:
        print('-------->> 警告:端口未给出，将使用默认端口: 8080 ')
        print('-------->> 如需使用其他端口，请执行: ')
        print('-------->> python HTTPServerWithUpload.py port ')
        print("-------->> port是一个整数，它的范围是: 1024 < port < 65535 ")
        port = 8080

    if not 1024 < port < 65535:
        port = 8080
    print('-------->> 现在，在监听 ' + str(port) + ' 端口 ...')
    osType = platform.system()
    data = 'http://127.0.0.1:' + str(port)
    if osType == "Windows":
        print('-------->> 您可以访问地址URL:http://' +str( GetWanIp().getip()) + ':' + str(port))
        data = 'http://'+str(GetWanIp().getip()) + ':' + str(port)
    else:
        print('-------->> 您可以访问地址URL:http://127.0.0.1:' + str(port))
    print('-------->> 或扫描此二维码: ')
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=1,
        border=1
    )
    qr.add_data(data)
    qr.make(fit=True)
    img = qr.make_image()
    img2 = np.array(img.convert('L'))
    #img.show()
    d = {255: '■', 0: '  '}
    rows, cols = img2.shape
    for i in range(rows):
        for j in range(cols):
            print(d[img2[i, j]], end='')
        print('')
    print('----------------------------------------------------------------------->> ')
    return ('', port)


serveraddr = showTips()


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
        f.write('<!DOCTYPE html>')
        f.write('<meta name="viewport" content="width=device-width" charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">')
        f.write("<html>\n<title>内网传输</title>\n")
        f.write("<body>\n<h2>目录清单 位于%s</h2>\n" % displaypath)
        f.write("<hr>\n")
        f.write("<form ENCTYPE=\"multipart/form-data\" method=\"post\">")
        f.write("<input name=\"file\" type=\"file\"/>")
        f.write("<input type=\"submit\" value=\"上传\"/>")
        f.write(
            "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp")
        f.write("<input type=\"button\" value=\"主目录\" onClick=\"location='/'\">")
        f.write("</form>\n")
        f.write(
            '<h2 style="color:#FF0000">请先选择完文件再点上传，不这样做的话可能会出现奇怪的情况</h2><hr>\n<ul>\n')
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
            f.write('<table><tr><td width="60%%"><a href="%s">%s</a></td><td width="20%%">%s</td><td width="20%%">%s</td></tr>\n'
                    % (urllib.parse.quote(linkname), colorName,
                        sizeof_fmt(os.path.getsize(filename)), modification_date(filename)))
        f.write("</table>\n<hr>\n</body>\n</html>\n")
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


def test(HandlerClass=SimpleHTTPRequestHandler,
         ServerClass=http.server.HTTPServer):
    http.server.test(HandlerClass, ServerClass)


if __name__ == '__main__':
    srvr = ThreadingServer(serveraddr, SimpleHTTPRequestHandler)

    srvr.serve_forever()


#原作者：bones7456
#改进者：高玩梁

#来源：   https://blog.csdn.net/weixin_43329321/article/details/89061274 

