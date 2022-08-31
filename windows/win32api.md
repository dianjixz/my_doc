windows多线程加锁


HANDLE mutex = CreateMutex(NULL, false, NULL);

WaitForSingleObject(mutex, INFINITE);

ReleaseMutex(mutex);

CloseHandle(mutex);


aardio创建多线程

thread.create(fun, paream)

https://bbs.aardio.com/forum.php


aardio中获取网络图片经GDI处理后保存到本地

import process; 
import fsys.stream;
import gdip.bitmap;
import inet.http;

//获取一个网络图片
var pngData = inet.http().get("http://www.baidu.com/img/baidu_logo.gif");

//转换为GDI对象处理
var bitmap = gdip.bitmap( pngData );

//转换为文件流
var stream = fsys.stream();
bitmap.saveToStream(stream,"*.png");

//获取图片字符串 保存到本地
var str  = stream.readAll();
string.save("/test.png",str );

process.explore_select("/test.png");


arrdio学习笔记——C语言交互之调用dll

https://zhuanlan.zhihu.com/p/375223254

Windows核心编程-CreateFile详解

https://blog.csdn.net/bxsec/article/details/76566011


COMMTIMEOUTS主要用于串口超时参数设置
https://blog.csdn.net/liuzhuomju/article/details/7479507

Windows-COM
https://gitee.com/zytq/Windows-COM/

windows结束线程的三种方式
https://blog.csdn.net/Kwansy/article/details/106479627
https://blog.csdn.net/renlonggg/article/details/120672053

Windows下线程的使用

https://zhuanlan.zhihu.com/p/433982373

创建完使用CloseHandle(mutex);关闭线程资源


windows串口编程（2）

https://blog.csdn.net/ljsant/article/details/51890256

串口get_CommEvent函数返回值枚举

https://blog.csdn.net/linxi8693/article/details/89454604

windows串口通信函数API

https://blog.csdn.net/weixin_39100510/article/details/124292893

使用Win32API实现Windows下异步串口通讯 

https://www.iteye.com/blog/nwi887nj-1351691

Win10 串口通信 —— 同步异步

https://blog.csdn.net/u011218356/article/details/107995518





windows c编写串口通信

https://blog.csdn.net/u010835747/article/details/117357403?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-117357403-blog-107995518.pc_relevant_antiscanv2&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-117357403-blog-107995518.pc_relevant_antiscanv2&utm_relevant_index=2


对ReadFile堵塞进行异步处理

https://blog.csdn.net/bangtanhui/article/details/122122560

异步读写(ReadFileEx和ReadFile)之overlapped

https://blog.csdn.net/weixin_34087307/article/details/94472265?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-2-94472265-blog-122122560.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-2-94472265-blog-122122560.pc_relevant_default&utm_relevant_index=5

CreateFile的OVERLAPPED 与 FILE_FLAG_OVERLAPPED

https://blog.csdn.net/duanbeibei/article/details/49795425?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-1-49795425-blog-94472265.pc_relevant_without_ctrlist_v3&spm=1001.2101.3001.4242.2&utm_relevant_index=4

Windows核心编程-CreateFile详解

https://blog.csdn.net/bxsec/article/details/76566011

CreateFile函数详解

https://blog.csdn.net/qq_32619837/article/details/89327250