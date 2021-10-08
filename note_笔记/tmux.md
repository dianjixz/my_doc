~~~ bash

# 设置Shell
set-option -g default-shell /bin/zsh

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