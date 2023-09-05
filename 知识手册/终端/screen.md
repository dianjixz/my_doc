screen
======


``` bash
# 启动一个会话  快捷键 Ctrl a+c
screen
# 启动一个带名字的会话
screen -S nihao 
# 分离会话 快捷键 Ctrl a+d
screen -d
# 重新连接会话 
screen -r [name]
# 切换会话 Ctrl a+n 切换至下一个 Screen 会话 Ctrl a+p 切换至上一个 Screen 会话

# 列出所有会话
screen -ls







#!/bin/bash
RC_NAME=speedtest
cmd="/bin/bash /root/screenjob/001_speedtest.sh"
SCREEN=/usr/bin/screen
$SCREEN -dmS $RC_NAME
$SCREEN -x -S $RC_NAME -p 0 -X stuff "$cmd"
$SCREEN -x -S $RC_NAME -p 0 -X stuff "\n"



```

Linux screen设置缓冲区大小/历史记录长度
https://blog.csdn.net/DLW__/article/details/119483273



在~/.screenrc中添加：

```bash
defscrollback 10000
termcapinfo xterm ti@:te@

```

常用配置
```bash
# 缓冲区行数
defscrollback 10000

# 支持鼠标滚轮
termcapinfo xterm ti@:te@

# 不显示欢迎屏幕
startup_message off

# 关闭闪屏
vbell off

# 状态栏
hardstatus alwayslastline
hardstatus string '%{= kg}[%{G}%H%{-}][ %{C}%-Lw%{+b g}[%{r}%n %{Y}%t%{-b g}]%{C}%+Lw%= %{G}][ %{C}S:%{b}(F1)[] (F2)< (F3)> (F4)<> %{C}V:%{b}(F5)| (F6)- (F7)> (F8)x %{kg}][ %Y/%m/%d %{G}%c %{g}]'

# 快捷键
# 窗口
bindkey -k k1 title
bindkey -k k2 prev
bindkey -k k3 next
bindkey -k k4 other
# 视图拆分
bindkey -k k5 split -v
bindkey -k k6 split
bindkey -k k7 focus
bindkey -k k8 remove
# 备用
#bindkey -k k9 other
#bindkey -k k; other
#bindkey -k F1 other
#bindkey -k F2 other

# 默认窗口名
#sessionname trainer

```





