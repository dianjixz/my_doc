 #!/usr/bin/python3



from maix import camera
import socket
camera.config(size=(224, 224))
ip_port = ('0.0.0.0',8081)    #.........
BUFSIZE=1024
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)  #.........
s.bind(ip_port) #............
s.listen(5) #............

while True:                  #........................,........................
    conn,addr=s.accept()    #...............
    print('............%s.........' %addr[0])
    img = camera.read(video_num=0)              #240*240
    conn.send(img)  #.........,......
    conn.close()
s.close()               #............



# import sys                                
# import traceback                   
# from io import StringIO
 
# import requests
# import tornado
# import tornado.ioloop
# import tornado.web
# from tornado.escape import json_encode
# from PIL import Image
 
# # reload(sys)
# # sys.setdefaultencoding('utf8')
 
            
# class Handler(tornado.web.RequestHandler):
                         
#     def post(self):
#         result = {}
#         image_url = self.get_argument("image_url", default="")
#         print (image_url)
#         if not image_url:
#             result["msg"] = "no image url"
#         else:        
#             result["msg"] = self.process(image_url)
#         self.write(json_encode(result))
                                  
#     def process(self, image_url):
#         """
#         ......image_url........................
 
#         :param image_url: ......url
#         """        
#         try:                                                  
#             response = requests.get(image_url)
#             if response.status_code == requests.codes.ok:
#                 obj = Image.open(StringIO(response.content))
#                 # TODO ......obj.....................
#                 return "ok"                        
#             else:                      
#                 return "get image failed."
#         except Exception as e:   
#             print (traceback.format_exc())
#             return str(e)                      
 
# class ImageServer(object):         
             
#     def __init__(self, port):
#         self.port = port                      
                                                           
#     def process(self):                                      
#         app = tornado.web.Application([(r"/image?", Handler)], )
#         app.listen(self.port)
#         tornado.ioloop.IOLoop.current().start()
                                            
# if __name__ == "__main__":    
#     server_port = "6060"                  
#     server = ImageServer(server_port)
#     print ("begin server")
#     server.process()    




