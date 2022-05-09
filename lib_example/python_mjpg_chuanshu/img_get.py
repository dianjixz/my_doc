import tornado.ioloop
import tornado.web
# import matplotlib
from PIL import Image
# matplotlib.use('Agg')
# import matplotlib.pyplot as plt
# import numpy as np
import io
import time
from maix import camera
http_hand = {
        'Cache-Control':'no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0',
        'Connection': 'close',
        'Content-Type': 'multipart/x-mixed-replace;boundary=--boundarydonotcross' ,
        'Expires': 'Mon, 1 Jan 2030 00:00:00 GMT',
        'Pragma': 'no-cache',
        'Access-Control-Allow-Origin': '*'  # CORS
}

# def genImage(freq):
#     t = np.linspace(0, 10, 500)
#     y = np.sin(t * freq * 2 * 3.141)
#     fig1 = plt.figure()
#     plt.plot(t, y)
#     plt.xlabel('Time [s]')
#     memdata = io.BytesIO()
#     plt.grid(True)
#     plt.savefig(memdata, format='png')
#     image = memdata.getvalue()
#     return image


class MainHandler(tornado.web.RequestHandler):
    @tornado.web.asynchronous
    def get(self):
        for i in range(10):
            self.write("Hello, world")
            time.sleep(1)
            self.finish()
        self.write('<img src="dss.png" />')#哪里写test.png了？
        # time.sleep(1)


class ImageHandler(tornado.web.RequestHandler):
    def get(self):
        
        # self.set_header('Content-type', 'image/jpg')
        # self.set_header('Content-length', len(image))
        # for obj in http_hand:
        #     self.set_header(obj,http_hand[obj])

        # self.set_header('Cache-Control','no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0')
        # self.set_header('Connection', 'close')
        # self.set_header('Content-type', 'multipart/x-mixed-replace;boundary=--boundarydonotcross')
        # # self.set_header('Expires','Mon, 1 Jan 2030 00:00:00 GMT')
        # self.set_header('Pragma', 'no-cache')
        # self.set_header('Access-Control-Allow-Origin', '*')

 
        img = camera.capture()
        # image = genImage(np.random.random(1)[0])
        
        # img = Image.frombytes("", size, data)
        imgByteArr = io.BytesIO()
        img.save(imgByteArr, format='JPEG')
        imgByteArr = imgByteArr.getvalue()


        # image
        self.write(imgByteArr)
        # self.finish()
        # self.write(imgByteArr)

        self.set_header("Content-type", "image/jpg")
        # self.write("can you see me?")
        # time.sleep(1)


application = tornado.web.Application([
    (r"/", MainHandler),
    (r"/dss.png", ImageHandler),
])

if __name__ == "__main__":
    print('Starting server')
    application.listen(9999)
    tornado.ioloop.IOLoop.instance().start()



# import urllib3
# import os
# #PIL图像处理标准库
# from PIL import Image
# from io import BytesIO

# http = urllib3.PoolManager()
# response = http.request('GET','f.hiphotos.baidu.com/image/pic/item/8d5494eef01f3a29f863534d9725bc315d607c8e.jpg')
# result = response.data

# #将bytes结果转化为字节流
# bytes_stream = BytesIO(result)
# #读取到图片
# roiimg = Image.open(bytes_stream)

# # roiimg.show()  #展示图片
# #print(type(result))
# #print(response.status)

# imgByteArr = BytesIO()    #初始化一个空字节流
# roiimg.save(imgByteArr,format('PNG'))     #把我们得图片以‘PNG’保存到空字节流
# imgByteArr = imgByteArr.getvalue()    #无视指针，获取全部内容，类型由io流变成bytes。
# # dir_name = os.mkdir('baiduimg')
# img_name = '1.jpg'
# with open(os.path.join('baiduimg',img_name),'wb') as f:
#     f.write(imgByteArr)

