#!/bin/env python3
import socket
# from PIL.Image import Image
from PIL import Image
import cv2
import numpy


while True:
    phone=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    phone.connect(('10.254.239.1',10000)) #拨通电话
    
    # phone.send('hello'.encode('utf-8')) #发消息
    reace_len = 1
    back_msg = None
    while reace_len:
        if back_msg == None:
            back_msg = phone.recv(230400)
        else:
            back_msg += phone.recv(230400)
        if len(back_msg) == 230400:
            break
    # print(back_msg)
    
    phone.close()

    print(len(back_msg))
    image = Image.frombytes("RGB" , (320,240), back_msg)
    img = cv2.cvtColor(numpy.asarray(image), cv2.COLOR_RGB2BGR)  # 转换代码
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # 转换代码
    cv2.imshow('img', img) 
    cv2.waitKey(1)