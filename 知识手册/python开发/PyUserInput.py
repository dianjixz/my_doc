2.安装：pykeyboard库

    PyMouse 和 PyKeyboard 库都集成到了 PyUserInput库中，直接安装 PyUserInput库即可。

    如果没有安装 pyHook 会报错！

    pip install pykeyboard

3.模拟鼠标键盘操作

    from pymouse import PyMouse			# 模拟鼠标
    from pykeyboard import PyKeyboard	# 模拟键盘
     
    # 定义实例
    mouse = PyMouse()
    keyboard = PyKeyboard()

3.1 鼠标操作

    点击：

        mouse.click(x,y,button,n)
        # x, y: 是坐标位置
        # buttong: 1表示左键，2表示右键
        # n: 点击次数，默认1次，2表示双击

    移动：

        mouse.move(x, y)	# 鼠标移动到坐标(x, y)
        x_dim, y_dim = mouse.screen_size()	# 获得屏幕尺寸

3.2 键盘操作

    输入字符串：不能输入中文！！！如果输入时使用了中文输入法，可能导致输入内容混乱，可以在执行前，将输入法切换为系统自带输入法的英文。

    keyboard.type_string('Hello, World!') 

    点击：

        keyboard.tap_key('H') 						# 点击H键
        keyboard.tap_key('H', n=2, interval=5) 		# 点击H键2次，每次间隔5秒
        keyboard.tap_key(keyboard.numpad_keys[5])   # 点击小键盘5
        keyboard.tap_key(keyboard.function_keys[5]) # 点击功能键F5

    联合点击：

        # 同时按 Alt + Tab 键
        keyboard.press_key(keyboard.alt_key)   # 按住Alt键
        keyboard.tap_key(keyboard.tab_key) 	   # 点击Tab键
        keyboard.release_key(keyboard.alt_key) # 松开Alt键
        # 先按下的后松开

按键属性

    查看所有按键属性：dir()

        print(dir(keyboard))
        # 内容太多，就不展示了

    常用键位：
    A~Z	字母：A~Z
    keyboard.enter_key	回车键：Enter
    keyboard.control_key	Ctrl
    keyboard.alt_key	Alt
    keyboard.tab_key	Tab
    keyboard.delete_key	Delete
    keyboard.function_keys[1]	功能键 F1
    keyboard.numpad_keys[1]	小键盘 1

4.输入中文

    问题：PyUserInput 无法直接输入中文。

    解决方法：利用 pyperclip.copy() 先将中文复制到剪切板，再通过模拟键盘粘贴 。

        import pyperclip
        from pykeyboard import PyKeyboard
        pyperclip.copy('中文')
        keyboard = PyKeyboard()
        keyboard.press_key(keyboard.control_key)	# 按下Ctrl键
        keyboard.tap_key('V')						# 点击V键
        keyboard.release_key(keyboard.control_key)	# 松开Ctrl键
————————————————
版权声明：本文为CSDN博主「小白华」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_51802807/article/details/121179861