sudo modprobe uinput
import uinput
events = (
    uinput.REL_X,
    uinput.REL_Y,
    uinput.BTN_LEFT,
    uinput.BTN_RIGHT,
)

device = uinput.Device(events)
device.emit(uinput.REL_X, 30, syn=False)
device.emit(uinput.REL_Y, 50)




sudo  python3 -m pip install pyzmq  -i https://pypi.mirrors.ustc.edu.cn/simple/ --break-system-packages
sudo  python3 -m pip install python-uinput -i https://pypi.mirrors.ustc.edu.cn/simple/ --break-system-packages

import uinput

ALL_KEY_EVENTS = [
    getattr(uinput, attr) for attr in dir(uinput) if attr.startswith("KEY_")
]
device = uinput.Device(ALL_KEY_EVENTS)
device.emit_click(uinput.KEY_A)




