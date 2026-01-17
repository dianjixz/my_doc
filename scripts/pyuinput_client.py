import keyboard
import zmq
# 要监听的按键列表
keys_to_listen = ['w', 'a', 's', 'd', 'q', 'e']

print("请按下 'w', 'a', 's', 'd', 'q', 'e' 键（按 esc 退出程序）")



# push_client.py
import zmq
import time

def main():
    context = zmq.Context()
    socket = context.socket(zmq.PUSH)
    socket.connect("tcp://192.168.28.115:5555")  # 连接到服务端IP和端口

    # for i in range(10):
    #     message = f"消息 {i}"
    #     print(f"发送: {message}")
    #     socket.send_string(message)
    #     time.sleep(1)
    while True:
        event = keyboard.read_event()  # 读取一个按键事件（阻塞）
        if event.event_type == keyboard.KEY_DOWN:  # 按下才响应
            if event.name in keys_to_listen:
                print(f"按键 {event.name} 被按下")
                socket.send_string(event.name)
            elif event.name == 'esc':
                print("退出程序")
                break

if __name__ == "__main__":
    main()