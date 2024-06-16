import requests
from PIL import Image
from io import BytesIO

url = 'http://192.168.8.106:8889/stream'  # 替换为实际的流地址

# 发起请求
response = requests.get(url, stream=True)

# 检查响应是否成功
if response.status_code == 200:
    # 使用 BytesIO 对象缓存数据
    buffer = BytesIO()
    mk = 0
    data_buf = b''
    # 逐一处理流中的每个部分
    for part in response.iter_content(chunk_size=1024):
        data_buf += part
        if mk == 0:
            boundary = data_buf.find(b'--boundarydonotcross')
            if boundary != -1:
                data_buf = data_buf[boundary+2:]
                mk = 1
        elif mk == 1:
            boundary = data_buf.find(b'------------boundarydonotcross')
            if boundary != -1:
                data_buf = data_buf[:boundary]
                print(data_buf)
                data_buf = b''
                mk = 0




        # boundary = data_buf.find(b'------------boundarydonotcross')
        # if boundary != -1:
        #     data_buf = data_buf[boundary:]
        #     mkasd = data_buf.find(b'\n\n')
        #     if mkasd != -1:
        #         head_flage = data_buf[:mkasd+2]
        #         lines = head_flage.split(b'\n')
        #         size_str = lines[2].decode('ascii', errors='ignore')
        #         size = int(size_str.split(' ')[1])


        #         print(size)


        #         data_buf = data_buf[mkasd+2:]

                
        #         # ascii_string = head_flage.decode('ascii', errors='ignore')
                


        #         # print(lines)



            
        #     data_buf = b''





            # lines = data_buf.split(b'\n')
            # for line in lines:
            #     if line == b'------------boundarydonotcross':
            #         print(line)



        # buffer.write(part)
        # line1 = buffer.readline()
        # if line1:
        #     print(line1)







        # boundary = part.find(b'------------boundarydonotcross')
        # if boundary != -1:
        #     pass




        # # print(len(part))
        # # # 寻找部分的边界，这通常以\r\n开头
        # boundary = part.find(b'------------boundarydonotcross')
        # if boundary != -1:
        #     print(part)
        # #     # 将部分写入缓冲区
        # #     buffer.write(part[boundary + 4:])

        # #     # 处理缓冲区中的数据（这里假设数据是图像）
        # #     image_data = buffer.getvalue()
        # #     print("get ones imgge", mk)
        # #     mk += 1
        # #     # image = Image.open(BytesIO(image_data))

        # #     # 在这里处理图像，比如显示或保存
        # #     # image.show()

        # #     # 清空缓冲区，准备接收下一个部分的数据
        #     buffer.seek(0)
        #     buffer.truncate()

        # else:
        #     buffer.write(part)

else:
    print(f"Failed to fetch the stream. Status code: {response.status_code}")
