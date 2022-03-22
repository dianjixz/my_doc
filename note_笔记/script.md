 

# Linux script命令 —— 终端里的记录器 

当你在终端或者控制台工作时，你可能想要记录在终端中所做的一切。这些记录可以用来当作史料，保存终端所发生的一切。比如说，你和一些Linux管理员们同时管理着相同的机器，或者你让某人远程登陆到了你的服务器上，你可能记录想要终端里发生的一切。要实现这个目标，你可以使用script命令。

Usage:
script [options] [file]

Options:
-a, --append 附加输出
-c, --command <command> 运行命令而不是交互式shell
-e, --return 返回子进程的退出代码
-f, --flush 每次写入后运行flush
--force 使用输出文件，即使它是一个链接
-q, --quiet 可以使script命令以静默模式运行
-t, --timing[=<file>] 指明输出录制的时间数据
-V, --version 输出版本信息并退出
-h, --help 显示此帮助并退出

终端会话录制。
script -t 2>demo.time -a demo.his #开始录像，ctrl+d或者exit结束
2> 完整输出
demo.time 对时间线的记录
demo.his 对操作命令等内容的记录

scriptreplay demo.time demo.his #回放

script -t -f -q 2>$USER.time -a $USER.his #记录用户的操作
————————————————
版权声明：本文为CSDN博主「怀揣梦想的大鸡腿」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/dajitui2024/article/details/79396526
