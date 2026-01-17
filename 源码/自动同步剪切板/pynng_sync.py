import pynng
import time
import threading
import sys
import pyperclip
import logging
import asyncio

# 配置日志
logging.basicConfig(level=logging.DEBUG, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

class GlobData:
    def __init__(self):
        self.configs = {
            '1': {
                'bind': 'tcp://0.0.0.0:6660',
                'connect': []
            },
            '2': {
                'bind': 'tcp://0.0.0.0:6660',
                'connect': ['tcp://192.168.20.76:6660']
            }
        }
        self.clipboard_content = ''
        self.last_clipboard_content = ''
        self.clipboard_lock = threading.Lock()

_Glob = GlobData()

class ClipboardSync:
    def __init__(self, config_key='1'):
        self.config = _Glob.configs[config_key]
        self.bus = None
        self.connect = self.config['connect']
        self.reconnect_event = asyncio.Event()
        
    async def init_bus(self):
        """初始化bus连接"""
        self.bus = pynng.Bus0()
        self.bus.listen(self.config['bind'])
        logger.info(f"Bus listening on {self.config['bind']}")
        
        # 初始连接
        for addr in self.connect:
            try:
                self.bus.dial(addr)
                logger.info(f"Connected to {addr}")
            except Exception as e:
                logger.error(f"Failed to connect to {addr}: {e}")
                self.reconnect_event.set()

    async def reconnect(self):
        """重连协程"""
        while True:
            await self.reconnect_event.wait()
            logger.info("Attempting to reconnect...")
            self.reconnect_event.clear()
            
            for addr in self.connect:
                try:
                    self.bus.dial(addr)
                    logger.info(f"Reconnected to {addr}")
                except Exception as e:
                    logger.error(f"Reconnect failed for {addr}: {e}")
                    self.reconnect_event.set()
                    
            await asyncio.sleep(1)  # 等待1秒后重试

    async def remote_update_clipboard(self, msg):
        """发送剪贴板内容到远程"""
        try:
            await self.bus.asend(msg.encode('utf-8'))
            logger.debug(f"Sent clipboard: {msg[:50]}...")
        except Exception as e:
            logger.error(f"Send error: {e}")
            self.reconnect_event.set()

    def set_clipboard(self, msg):
        """设置本地剪贴板"""
        try:
            pyperclip.copy(msg)
            _Glob.last_clipboard_content = msg
            logger.debug(f"Set clipboard: {msg[:50]}...")
        except Exception as e:
            logger.error(f"Set clipboard error: {e}")


    async def safe_set_clipboard(self, msg, timeout=2.0):
        try:
            await asyncio.wait_for(
                asyncio.to_thread(pyperclip.copy, msg),
                timeout=timeout
            )
            _Glob.last_clipboard_content = msg
            logger.debug(f"Set clipboard: {msg[:50]}...")
        except asyncio.TimeoutError:
            logger.error("Set clipboard 超时！")
        except Exception as e:
            logger.error(f"Set clipboard error: {e}")


    async def safe_paste(self, timeout=2.0):
        try:
            # 在后台线程运行 pyperclip.paste，限制最多等待 timeout 秒
            return await asyncio.wait_for(asyncio.to_thread(pyperclip.paste), timeout=timeout)
        except asyncio.TimeoutError:
            logger.error("pyperclip.paste() 超时，可能剪贴板被阻塞或卡死")
            return None
        except Exception as e:
            logger.error(f"pyperclip.paste() 异常: {e}")
            return None



    async def clipboard_monitor(self):
        """监控本地剪贴板变化"""
        logger.info("Clipboard monitor started")
        while True:
            try:
                _Glob.clipboard_content = await self.safe_paste(timeout=2.0)
                
                if (_Glob.clipboard_content != _Glob.last_clipboard_content 
                    and _Glob.clipboard_content):
                    _Glob.last_clipboard_content = _Glob.clipboard_content
                    logger.info("Local clipboard changed, sending to remote...")
                    await self.remote_update_clipboard(_Glob.clipboard_content)
                    
            except Exception as e:
                logger.error(f"Clipboard monitor error: {e}")
                
            await asyncio.sleep(0.1)

    async def remote_monitor(self):
        """监控远程剪贴板更新"""
        logger.info("Remote monitor started")
        while True:
            try:
                msg = await self.bus.arecv()
                message = msg.decode('utf-8')
                logger.info("Received remote clipboard update")
                await self.safe_set_clipboard(message)
                
            except Exception as e:
                logger.error(f"接收错误: {e}")
                self.reconnect_event.set()
                await asyncio.sleep(1)

    async def run(self):
        """运行主程序"""
        await self.init_bus()
        
        await asyncio.gather(
            self.remote_monitor(),
            self.clipboard_monitor(),
            self.reconnect(),
        )

async def main():
    # 从命令行参数获取配置，默认使用配置1
    config_key = sys.argv[1] if len(sys.argv) > 1 else '1'
    
    if config_key not in _Glob.configs:
        logger.error(f"Invalid config key: {config_key}")
        sys.exit(1)
    
    logger.info(f"Starting clipboard sync with config {config_key}")
    sync = ClipboardSync(config_key)
    
    try:
        await sync.run()
    except KeyboardInterrupt:
        logger.info("Stopping clipboard sync...")
    except Exception as e:
        logger.error(f"Unexpected error: {e}")

if __name__ == "__main__":
    asyncio.run(main())