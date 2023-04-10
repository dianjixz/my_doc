~~~ bash

# 设置Shell
set-option -g default-shell /bin/bash

# 设置复制模式，空格开始选，Enter结束复制
setw -g mode-keys vi 

#开启status-bar uft-8支持
set -g status-utf8 on

# 设定窗口起始
set -g base-index 1

# history
set-option -g history-limit 4096

#urxvt tab like window switching (-n: no prior escape seq)
#ruxvt标签式窗口切换
bind -n S-down new-window
bind -n S-left prev
bind -n S-right next
bind -n C-left swap-window -t -1
bind -n C-right swap-window -t +1

# 切换panes
bind-key C-l last-pane

# 设置鼠标可以操作|滚动设置
#   --> 使用shift键+鼠标键选择文件，Ctrl+Shift+C复制，Ctrl+Shift+V粘贴
# set -g mode-mouse on
# set -g mode-mouse off  # --> let cygwin mouse can be used.
# disable mouse control by default - change 'off' to 'on' to enable by default.
setw -g mode-mouse off
set-option -g mouse-resize-pane off
set-option -g mouse-select-pane off
set-option -g mouse-select-window off
# toggle mouse mode to allow mouse copy/paste
# set mouse on with prefix m
bind m \
    set -g mode-mouse on \;\
    set -g mouse-resize-pane on \;\
    set -g mouse-select-pane on \;\
    set -g mouse-select-window on \;\
    display 'Mouse: ON'
# set mouse off with prefix M
bind M \
    set -g mode-mouse off \;\
    set -g mouse-resize-pane off \;\
    set -g mouse-select-pane off \;\
    set -g mouse-select-window off \;\
    display 'Mouse: OFF'

# 设置窗口可自动调整index
set -g renumber-windows on

# 复制和粘贴模式的快捷键
bind y copy-mode
bind c-y copy-mode
bind p paste-buffer
bind c-p paste-buffer

# Toggle log to file
bind H pipe-pane -o 'cat >>$HOME/#W-tmux.log' '\;' display-message 'Toggled logging to $HOME/#W-tmux.log'

bind h pipe-pane \; display-message 'Ended logging to $HOME/#W-tmux.log'

# 设置窗口名字不能修改
set-option -g allow-rename off

# 设置启动窗口时默认名字
# bind-key c new-window -n 'zsh'

# 设置r键为加载配置文件，并提示信息
bind r source-file ~/.tmux.conf \; display "配置文件已重新加载！" 

# other
setw -g xterm-keys on

#------------------------------------------------------------------------------#
#  *** 基本终终端操作 ***
#  tmux new -s "session_name"       // 创建一个seesion
#  tmux new -s "session_name" -d    // 创建一个seesion, 并在后台运行
#  tmux ls                          // 列出tmux会话列表
#  tmux attach -t session           // 进入指定的会话（可终端共享）
#  tmux kill-session -t "session"   // 删除指定会话
#
#  ** 容易混淆 **
#  bind-key ;   // switch to last pane
#  bind-key l   // switch to last window
#  bind-key L   // switch to last session
#  bind-key C-l // switch to last pane(like: bind-key ;)
#
#  *** 控制台-操作说明 ***
#  Ctrl+b  // 激活控制台
#  ** 系统操作   
#      ?   // 列出所有快捷键；按q返回   
#      d   // 脱离当前会话；
#      D   // 选择要脱离的会话；在同时开启了多个会话时使用   
#      Ctrl+z  // 挂起当前会话   
#      r   // 强制重绘未脱离的会话   
#      s   // 选择并切换会话；在同时开启了多个会话时使用   
#      :   // 进入命令行模式；
#               :new-session         // 创建新会话
#      [   // 进入复制模式；此时的操作与vi/emacs相同，按q/Esc退出   
#      ]   // 进入粘贴模式;
#      ~   // 列出提示信息缓存；其中包含了之前tmux返回的各种提示信息   
#
#  ** 会话操作
#      (   preview session
#      )   next session
#      L   last session
#      s   list session
#      $   rename seesion
#
#  ** 窗口操作
#      c   // 创建新窗口   
#      &   // 关闭当前窗口   
#      [0-9] // 切换至指定窗口   
#      p   // 切换至上一窗口   
#      n   // 切换至下一窗口   
#      l   // 在前后两个窗口间互相切换   
#      w   // 通过窗口列表切换窗口   
#      ,   // 重命名当前窗口；这样便于识别   
#      .   // 修改当前窗口编号；相当于窗口重新排序   
#      f   // 在所有窗口中查找指定文本   
#      i   // 显示tmux当前信息
#
#  ** 面板操作   
#      ”   // 将当前面板平分为上下两块|横向切割 --> 分割成面板
#      %   // 将当前面板平分为左右两块|竖向切割 --> 分割成面板
#      x   // 关闭当前面板   
#      ;   // 切换到最后的pane
#      !   // 将当前面板置于新窗口；即新建一个窗口，其中仅包含当前面板   
#      Ctrl+方向键  // 以1个单元格为单位移动边缘以调整当前面板大小   
#      Alt+方向键   // 以5个单元格为单位移动边缘以调整当前面板大小   
#      Space        // 在预置的面板布局中循环切换;依次包括:
#                       even-horizontal、
#                       even-vertical、
#                       main-horizontal、
#                       main-vertical、
#                       tiled   
#      q   // 显示面板编号   
#      o   // 在当前窗口中选择下一面板   
#      {   // 向前置换当前面板   
#      }   // 向后置换当前面板   
#      方向键   // 移动光标以选择面板   
#      Alt+o    // 逆时针旋转当前窗口的面板   
#      Ctrl+o   // 顺时针旋转当前窗口的面板   


#  *** 参考配置 ***
#
#  ** 配置1 **
#  #设置pan前景色  
#  set -g pane-border-fg green  
#  #设置pane背景色  
#  set -g pane-border-bg black 
#  #设置终端颜色为256色
#  set -g default-terminal "screen-256color"
#  #开启status-bar uft-8支持
#  set -g status-utf8 on
#  #设置pan前景色
#  set -g pane-border-fg green
#  #设置pane背景色
#  set -g pane-border-bg black
#  #设置活跃pane前景色
#  set -g pane-active-border-fg white
#  #设置活跃pane背景色
#  set -g pane-active-border-bg yellow
#  #设置消息前景色
#  set -g message-fg white
#  #设置消息背景色
#  set -g message-bg black
#  #设置消息高亮显示
#  set -g message-attr bright
#  #设置status-bar颜色
#  set -g status-fg white
#  set -g status-bg black
#  #设置窗口列表颜色
#  setw -g window-status-fg cyan
#  setw -g window-status-bg default
#  setw -g window-status-attr dim
#  #设置当前窗口在status bar中的颜色
#  setw -g window-status-current-fg white
#  setw -g window-status-current-bg red
#  setw -g window-status-current-attr bright
#  #设置status bar格式
#  set -g status-left-length 40
#  set -g status-left "#[fg=green]Session: #S #[fg=yellow]#I #[fg=cyan]#P"
#  set -g status-right "#[fg=cyan]%d %b %R"
#  set -g status-interval 60
#  set -g status-justify centre
#  #开启window事件提示
#  setw -g monitor-activity on
#  set -g visual-activity on
#
#  ** 配置2 **
#  # 同一个窗口中的面板操作
#  # up
#  bind-key k select-pane -U 
#  #down
#  bind-key j select-pane -D
#  #left
#  bind-key h select-pane -L
#  #right
#  bind-key l select-pane -R
#  #select last window
#  bind-key C-l select-window -l
#------------------------------------------------------------------------------#

~~~









# tmux简洁教程及config关键配置

[![img](https://cdn2.jianshu.io/assets/default_avatar/12-aeeea4bedf10f2a12c0d50d626951489.jpg)](https://www.jianshu.com/u/57bdea70aa86)

[赤乐君](https://www.jianshu.com/u/57bdea70aa86)关注

0.8682017.04.06 16:08:45字数 1,801阅读 36,253

这个教程的目的是为了更好地使用tmux，作为一个小白，看了网上众多的资料后，感觉资料太多，质量也良莠不齐。在youtube上找了一个很好地系列教程，实际跟着做了一遍后tmux最有用的部分都学会了。有什么不懂得直接查查速查表即可。

本次教程的环境是MAC OS 10.11. 关于如何安装tmux可以参考这两篇文章。

[http://cenalulu.github.io/linux/tmux/](https://link.jianshu.com/?t=http://cenalulu.github.io/linux/tmux/) ： 了解session，window，pane的区别
[http://harttle.com/2015/11/06/tmux-startup.html](https://link.jianshu.com/?t=http://harttle.com/2015/11/06/tmux-startup.html)

这两篇是我个人觉得比较好的文章，可以看完这两篇文章后再来看我的教程。做一个梳理和总结。

这个教程是我跟着视频做完的笔记，视频里有些内容因为时间问题不能用，我也做了相应的改进。建议最好还是把视频跟一遍，然后拿我的笔记用做复习。

本教程参考的视频：[https://www.youtube.com/watch?v=FEfuXRTqINg](https://link.jianshu.com/?t=https://www.youtube.com/watch?v=FEfuXRTqINg)

快捷键速查表：[https://tmuxcheatsheet.com](https://link.jianshu.com/?t=https://tmuxcheatsheet.com)

# 1 Introduction

为什么使用tmux？
因为如果我们用terminal连接remote server。发生一些不可抗力，terminal关了的话，your work is GONE!

但是tmux不一样，即使你关闭了tmux。下次重新attch的时候，你会发现之前的东西都还在。这是因为即使你关闭了tmux，它也还在服务器的后台运行。

- prefix默认指的是ctrl键位和b键位，两个一起press，然后再按其他键位来实现不同的命令。在第4部分，我们会更改这个默认设置为ctrl+a，方便输入。在此之前默认都是ctrl+b

举个栗子：
**prefix + % :水平分割pane**
上面这句话里的`+`号和`:`号可以无视。`:`号之后的内容是我写的注释。
prefix是按下ctrl和b, 然后再按`%`键，这个`%`键就是shift+5。

# 2 Panes

分割pane

- prefix + % :水平分割pane
- prefix + " : 竖直分割pane

退出

- exit ： 退出一个pane，直接在shell里输入即可，这个比快捷键方便

放大一个pane

- prefix + z : 把当前一个pane放大（zoom in)。比如在用ls查看output的时候，因为一个pane可能空间太小，所以把这个pane放大，你可以把注意力全放在这个pane里。回到之前的多pane状态的话只需要重复一遍命令即可(zoom out)

在pane之间switch

- prefix + 上下左右的箭头 :这个说实话还是不方便，之后会有设置的方法来用鼠标选择pane

resize the pane

- prefix + （ctrl）+上下左右箭头 : 与上面命令不同的是，ctrl + b按完之后，不要松开ctrl，一直按着，然后再按箭头来调整。不过因为在mac下ctrl+箭头是切换屏幕，所以还得在偏好设置->键盘->快捷键->Mission Control里把对应的快捷键取消掉。

# 3 Windows

创建window

- prefix + c : 创建一个新的window。最下面会多出window的编号。有*号所在的window就是当前正在操作的window。

在不同的window间移动

- prefix + 数字1，2，3 : 因为能看到不同window的数字编号，所以直接输入想去的window的数字编号即可

关闭window

- prefix + & ： 关闭当前window

重命名window：因为创建新的window后，下面除了数字编号不同外window名称都是一样的。所以为了知道每一个window是什么，最好重命名一下。

- prefix + , (逗号）：更改window名称。但是这里遇到一个问题。更名后，我随便使用ls或cd命令后，window名称会随着目录的不同而变化。google后发现这个是zsh下oh-my-zsh的特性。于是打开~/.zshrc, 讲DISABLE_AUTO_TITLE="true"这一行反注释掉。source ~/.zshrc后，测试更改的名称，发现一切正常。

# 4 Configuration

如果没有配置文件的话先创建: `touch ~/.tmux.conf`
视频中的文件配置



```bash
# Send prefix
set-option -g prefix C-a
unbind-key C-a
bind-key C-a send-prefix

# Use Alt-arrow keys to switch panes
bind -n M-Left select-pane -L
bind -n M-Right select-pane -R
bind -n M-Up select-pane -U
bind -n M-Down select-pane -D

# Shift arrow to switch windows
bind -n S-Left previous-window
bind -n S-Right next-window

# Mouse mode
set -g mode-mouse on
set -g mouse-resize-pane on
set -g mouse-select-pane on
set -g mouse-select-window on

# Set easier window split keys
bind-key v split-window -h
bind-key h split-window -v

# Easy config reload
bind-key r source-file ~/.tmux.conf \; display-message "tmux.conf reloaded"
```

首先，在更改了.tmux.conf后，在tmux里的快捷键没有变化。查找后发现是tmux只有在新建session的时候，才会去找tmux.conf文件。所以说，我之前创建的那些session都没有参考tmux.conf. 所以我就用`tmux lstmux kill-session -a`只保留当前session。再删除当前session `tmux kill-session -t py27`。这下删除了所有创建好的session。

然后再次用`tmux new -s py27`创建一个新的名为`py27`的session。有提示了，但是错误提示显示没有mode-mouse命令。google之发现在2.1之后的tmux版本里，已经废除了这个命令。想要开启mouse mode的话，只需要一个句命令即可`set -g mouse on`。

更新后如下



```bash
# Send prefix
set-option -g prefix C-a
unbind-key C-a
bind-key C-a send-prefix

# Use Alt-arrow keys to switch panes
bind -n M-Left select-pane -L
bind -n M-Right select-pane -R
bind -n M-Up select-pane -U
bind -n M-Down select-pane -D

# Shift arrow to switch windows
bind -n S-Left previous-window
bind -n S-Right next-window

# Mouse mode
set -g mouse on


# Set easier window split keys
bind-key v split-window -h
bind-key h split-window -v

# Easy config reload
bind-key r source-file ~/.tmux.conf \; display-message "tmux.conf reloaded"
```

**Send prefix**
把prefix的ctrl+b变为了ctrl+a，因为这样按起来方便些。基本上用tmux的都改了这个。

**Use Alt-arrow keys to switch panes**
不用按prefix，直接用alt+箭头在pane之间switch。实际用过之后才发现真是太方便了！

**Shift arrow to switch windows**
不用按prefix，直接用shift+箭头在window之间switch。太方便了！

**Mouse mode**
开启鼠标模式。用鼠标就能切换window，pane，还能调整pane的大小，方便！

**Set easier window split keys**
这一部分是用来更方便切分pane的。prefix + v 代表竖着切，prefix + h 代表横着切。比起默认的切割方法不仅直观而且方便。

**Easy config reload**
下一次如果修改了.tmux.conf的设置的话，不用关掉tmux。直接用prefix+r,就能重新加载设置。

# 5 Session

查看所有的session（在terminal输入）

- tmux ls : 这个命令是在terminal里输入的。当前正常运作中的tmux server会显示（attached）。没有的话就是已关闭，tmux server在后台运行。

更名session（tmux状态下输入）

- prefix + $ : 更名后好让自己知道每一个session是用来做什么的。通常一个session对应一个project

创建session的时候直接命名(在terminal输入）

- tmux new -s py35 : 新建一个名为py35的session

断开一个session(detached) （tmux状态下输入）

- prefix + d ：退出session。在只有一个window的状态下，直接输入exit也能退出

重新连接某一个session wich name（在terminal输入）

- tmux a -t py35 : 重新连接py35 session。这里的a是attach的意思

偷懒连接上一个session（在terminal输入）

- tmux a : 如果只有一个session的话，这个是最快的连接方法

删除session（在terminal输入）

- tmux kill-session -a -t py35 : 删除除了py35以外的所有session