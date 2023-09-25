Python SMTP发送邮件-smtplib模块
https://zhuanlan.zhihu.com/p/318387004


note:
    谷歌邮箱需要添加‘message-id’

``` python
import email.utils as utils
msgRoot['message-id'] = utils.make_msgid(domain=senderAddr.split('@')[1])
```


python3学习课（十六）—发送邮件smtplib，email，邮件头header，定时器schedule，爬虫requests
https://blog.csdn.net/weixin_45239949/article/details/96617183


Python3 SMTP发送邮件
https://www.runoob.com/python3/python3-smtp.html


```python
# import smtplib
# import email.message
# from email.mime.multipart import MIMEMultipart
# from email.mime.text import MIMEText
# from email.mime.base import MIMEBase
# from email import encoders
# import email.utils as utils



# emailss = 'curdeveryday@gmail.com'
# send_from = 'vtraining@m5stack.com'


# smtp_server = 'smtp.qiye.aliyun.com'
# smtp_port = 465
# user_name = send_from
# password = 'Abcd@12345'



# def send_html_email(subject, msg_text, toaddrs=['curdeveryday@gmail.com']):
    

#     fromaddr = 'vtraining@m5stack.com'

#     msg = email.message.Message()
#     msg['message-id'] = utils.make_msgid(domain=fromaddr.split('@')[1])
#     msg['Subject'] = subject
#     msg['From'] = fromaddr
#     msg['To'] = ",".join(toaddrs)
#     msg.add_header('Content-Type', 'text/html')
#     msg.set_payload(msg_text)


#     print(msg.as_string())

#     server = smtplib.SMTP_SSL(host=smtp_server)
#     # server.set_debuglevel(1)
#     server.connect(host=smtp_server, port=smtp_port)
#     server.login(user_name, password)
#     server.sendmail(user_name, [emailss], msg.as_string())
#     server.quit()


# if __name__ == '__main__':
#     send_html_email('Hello!', 'hiii!!!')
```

```python
# #!/usr/bin/python3
 
# import smtplib
# from email.mime.image import MIMEImage
# from email.mime.multipart import MIMEMultipart
# from email.mime.text import MIMEText
# from email.header import Header
 
# sender = 'vtraining@m5stack.com'
# receivers = ['1784651680@qq.com']  # 接收邮件，可设置为你的QQ邮箱或者其他邮箱
 
# smtp_server = 'smtp.qiye.aliyun.com'
# smtp_port = 465
# user_name = sender
# password = 'Abcd@12345'



# msgRoot = MIMEMultipart('related')
# msgRoot['message-id'] = utils.make_msgid(domain=sender.split('@')[1])
# msgRoot['From'] = f'V-Trainer <{sender}>'
# msgRoot['To'] = f'{receivers[0]} <{receivers[0]}>'
# msgRoot['Subject'] = f'[V-Trainer] task id : asdasdasd Online Training Request Finished'


# msgAlternative = MIMEMultipart('alternative')
# msgRoot.attach(msgAlternative)

# msg_content = f'''\
# <p>Hi!</p>
# <br>
# <p>Your training request have been successfully processed, you can download the kmodel & sample program files here: </p>
# <p><a href="https://www.example.com">https://www.example.com</p>
# <br>
# <br>
# <p>Model: Classification MobileNetV1 Alpha: 0.7 Depth: 1</p>
# <p><img decoding="async" src="cid:image1"></p>
# '''

# msgText = MIMEText(msg_content, 'html')
# msgAlternative.attach(msgText)


# with open('/home/nihao/work/k210/NEW_VTrainingService/work_dir/1ad652564ea73d19_dataset/images/1/1.jpg', 'rb') as imageFile:
#     msgImage = MIMEImage(imageFile.read())
#     msgImage.add_header('Content-ID', '<image1>')
#     msgRoot.attach(msgImage)



# server = smtplib.SMTP_SSL(host=smtp_server)
# # server.set_debuglevel(1)
# server.connect(host=smtp_server, port=smtp_port)
# server.login(user_name, password)
# server.sendmail(user_name, receivers, msgRoot.as_string())
# server.quit()









```




















