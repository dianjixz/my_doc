#!/bin/env python3
import time
from pymouse import PyMouse			# 模拟鼠标
from pykeyboard import PyKeyboard	# 模拟键盘
    
# 定义实例
k = PyKeyboard()
m = PyMouse()

while True:
    pos = m.position()
    print(pos)
    m.move(pos[0] + 10, pos[1] + 10)
    time.sleep(10)
    pos = m.position()
    print(pos)
    m.move(pos[0] - 10, pos[1] - 10)
    time.sleep(10)

# pip install pyuserinput  -i https://pypi.mirrors.ustc.edu.cn/simple/
# pip install pypiwin32  -i https://pypi.mirrors.ustc.edu.cn/simple/
#m.position()#获取当前坐标的位置
#m.move(x,y)#鼠标移动到xy位置
#m.click(x,y)#移动并且在xy位置点击
#m.click(x,y,1|2)#移动并且在xy位置点击,左右键点击
#mouse.click(x,y,button,n)
# x, y: 是坐标位置
# buttong: 1表示左键，2表示右键
# n: 点击次数，默认1次，2表示双击
#mouse.move(x, y)	# 鼠标移动到坐标(x, y)
#x_dim, y_dim = mouse.screen_size()	# 获得屏幕尺寸
#keyboard.type_string('Hello, World!') 
#keyboard.tap_key('H') 						# 点击H键
#keyboard.tap_key('H', n=2, interval=5) 		# 点击H键2次，每次间隔5秒
#keyboard.tap_key(keyboard.numpad_keys[5])   # 点击小键盘5
#keyboard.tap_key(keyboard.function_keys[5]) # 点击功能键F5
# 同时按 Alt + Tab 键
#keyboard.press_key(keyboard.alt_key)   # 按住Alt键
#keyboard.tap_key(keyboard.tab_key) 	   # 点击Tab键
#keyboard.release_key(keyboard.alt_key) # 松开Alt键
# 先按下的后松开
#print(dir(keyboard))
# 内容太多，就不展示了

# from pykeyboard import PyKeyboard

# k = PyKeyboard()

# #按向下的方向键
# k.press_key(k.down_key)

# #按回车键
# k.press_key(k.enter_key)


'''

import time
from pymouse import PyMouse			# 模拟鼠标
from pykeyboard import PyKeyboard	# 模拟键盘
    
# 定义实例
k = PyKeyboard()
m = PyMouse()

while True:
    k.press_key(k.enter_key)
    time.sleep(0.1)









'''







