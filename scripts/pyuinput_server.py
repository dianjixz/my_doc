# sudo modprobe uinput
import uinput
events = (
    uinput.REL_X,
    uinput.REL_Y,
    uinput.BTN_LEFT,
    uinput.BTN_RIGHT,
)

device = uinput.Device(events)
# device.emit(uinput.REL_X, 30, syn=False)
# device.emit(uinput.REL_Y, 50)
# pull_server.py
import zmq

def main():
    context = zmq.Context()
    socket = context.socket(zmq.PULL)
    socket.bind("tcp://*:5555")  # 监听所有接口的5555端口

    print("服务端(PULL)已启动，等待消息...")
    while True:
        message = socket.recv_string()
        # print(f"收到消息: {message}")
        if message == "q":
            device.emit(uinput.BTN_LEFT, 1)
            device.emit(uinput.BTN_LEFT, 0)
        elif message == "e":
            device.emit(uinput.BTN_RIGHT, 1)
            device.emit(uinput.BTN_RIGHT, 0)
        elif message == "w":
            device.emit(uinput.REL_Y, -5)
        elif message == "s":
            device.emit(uinput.REL_Y, 5)
        elif message == "a":
            device.emit(uinput.REL_X, -5)
        elif message == "d":
            device.emit(uinput.REL_X, 5)

if __name__ == "__main__":
    main()