#!/bin/python3
# ### 方法 1：使用 `pyperclip`（推荐）
# `pyperclip` 是一个跨平台的剪贴板操作库，安装简单，使用方便。

# 1. 首先安装 `pyperclip`：
#    ```bash
#    pip install pyperclip
#    ```

# 2. 获取剪贴板内容：
#    ```python
#    import pyperclip

#    clipboard_content = pyperclip.paste()
#    print("剪贴板内容:", clipboard_content)
# # 设置剪贴板内容
# pyperclip.copy("这是要复制到剪贴板的内容")
#    ```
import zmq
import time
import pyperclip
import threading
last_clipboard_content = ""
lock = threading.Lock()
def publisher():
    global last_clipboard_content
    global lock
    # 创建一个 ZMQ 上下文
    context = zmq.Context()

    # 创建一个发布者套接字
    socket = context.socket(zmq.PUB)

    # 连接到发布者的地址
    publisher_address = "tcp://192.168.20.76:6002"
    socket.connect(publisher_address)
    print(f"Connected to publisher at {publisher_address}")

    # 模拟发布消息
    topic = "example_topic"
    message_id = 0
    last_clipboard_content = ""
    try:
        while True:
            # 发布内容
            with lock:
                clipboard_content = pyperclip.paste()
                if clipboard_content != last_clipboard_content:
                    last_clipboard_content = clipboard_content
                    socket.send_string(f"{last_clipboard_content}")
                    print(f"Published message: {last_clipboard_content}")
            time.sleep(0.1)
    except KeyboardInterrupt:
        print("\nPublisher stopped.")
    finally:
        socket.close()
        context.term()


def subscriber():
    global last_clipboard_content
    global lock
    # 创建一个 ZMQ 上下文
    context = zmq.Context()

    # 创建一个订阅者套接字
    socket = context.socket(zmq.SUB)

    # 连接到发布者的地址
    publisher_address = "tcp://192.168.20.76:6001"
    socket.connect(publisher_address)
    print(f"Connected to subscriber at {publisher_address}")

    # 订阅感兴趣的主题
    socket.setsockopt_string(zmq.SUBSCRIBE, '')
    print(f"Subscribed to topic")

    try:
        while True:
            # 接收消息
            message = socket.recv_string()
            print(f"Received: {message}")
            with lock:
                last_clipboard_content = message
                pyperclip.copy(message)
    except KeyboardInterrupt:
        print("\nSubscriber stopped.")
    finally:
        socket.close()
        context.term()



if __name__ == "__main__":
    producer_thread = threading.Thread(target=publisher)
    consumer_thread = threading.Thread(target=subscriber)
    producer_thread.start()
    consumer_thread.start()
    producer_thread.join()
    consumer_thread.join()
