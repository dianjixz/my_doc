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


import asyncio
import pynng
import pyperclip
import argparse
import sys


class ClipboardSync:
    last_clipboard_content = ""
    interval_seconds = 0.1
    sock = None
    lock = asyncio.Lock()

    def __init__(self, sock):
        self.sock = sock


async def timer(Clipboard):
    count = 0
    while True:
        count += 1
        async with Clipboard.lock:
            clipboard_content = pyperclip.paste()
            if clipboard_content != Clipboard.last_clipboard_content:
                Clipboard.last_clipboard_content = clipboard_content
                print(f"Published message: {clipboard_content}")
                Clipboard.sock.send(f"{clipboard_content}".encode("utf-8"))
            elif count % 100 == 0:
                Clipboard.sock.send(b"")
        await asyncio.sleep(Clipboard.interval_seconds)


async def aio_recv(Clipboard):
    while True:
        try:
            msg = await Clipboard.sock.arecv()
            if msg:
                Clipboard_msg = msg.decode("utf-8")
                async with Clipboard.lock:
                    if Clipboard.last_clipboard_content != Clipboard_msg:
                        Clipboard.last_clipboard_content = Clipboard_msg
                        print("收到异步消息:", Clipboard_msg)
                        pyperclip.copy(Clipboard_msg)
        except:
            pass


async def main(args):
    sock = None
    if args.listen:
        sock = pynng.Bus0(listen=args.listen)
    else:
        sock = pynng.Bus0()
    if args.dial:
        if isinstance(args.dial, list):
            for dial in args.dial:
                sock.dial(dial)
        else:
            sock.dial(args.dial)
    # 一定要在连接之前设置参数
    # sock.reconnect_time = 100
    # sock.max_reconnect_time = 1000
    # 连接到对方（对方必须先listen好）
    print("---------")
    # 并行启动接收和定时器任务
    Clipboard = ClipboardSync(sock)
    recv_task = asyncio.create_task(aio_recv(Clipboard))
    timer_task = asyncio.create_task(timer(Clipboard))
    # 挂起等待，直到其中一个任务出现异常
    await asyncio.gather(recv_task, timer_task)
    # print('clost')
    # sock.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="sync clipboard")
    parser.add_argument(
        "-l", "--listen", type=str, help="listen address, example:tcp://0.0.0.0:6660"
    )
    parser.add_argument(
        "-d", "--dial", type=str, help="dial address, example:tcp://192.168.28.21:6660"
    )
    args = parser.parse_args()
    if not args.listen and not args.dial:
        print("Error: --listen/-l 和 --dial/-d 参数至少指定一个！", file=sys.stderr)
        parser.print_help()
        sys.exit(1)
    asyncio.run(main(args))
