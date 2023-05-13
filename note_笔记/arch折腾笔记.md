Manjaro学习笔记

    自己折腾了个Manjaro来用，记下了一些用过的命令，当个线上笔记吧
    在这里插入图片描述

常用命令
pacman 常用命令
``` bash
# 对整个系统进行更新/升级软件包
$ sudo pacman -Syu  

#安装或者升级单个软件包，或者一列软件包（包含依赖包），使用如下命令：
$ sudo pacman -S package_name1 package_name2 ...
	#与上面命令不同的是，该命令将在同步包数据库后再执行安装
	$ sudo pacman -Sy package_name

# 安装本地包
$ sudo pacman -U local_package_name#其扩展名为pkg.tar.gz或pkg.tar.xz

# 安装一个远程包
$ sudo pacman -U url#不在 pacman 配置的源里面，例：pacman -U http://www.example.com/repo/example.pkg.tar.xz

# 在仓库中搜索含关键字的包
$ sudo pacman -Ss keyword

# 查看已安装软件
$ sudo pacman -Qs keyword

# 删除单个软件包，保留其全部已经安装的依赖关系
$ sudo pacman -R package_name

# 删除指定软件包，及其所有没有被其他已安装软件包使用的依赖关系：
$ sudo pacman -Rs package_name

# 清理软件包缓存
$ sudo pacman -Sc

# 清理所有的缓存文件
$ sudo pacman -Scc

# 清除系统中无用的包
$ sudo pacman -R $(pacman -Qdtq)

```

yay常用命令
``` bash
# 从 AUR 安装软件包#yay 安装命令不需要加 sudo
$ yay -S package 

# yay删除包
$ yay -Rns package 

# 升级所有已安装的包
$ yay -Syu

# 打印系统统计信息
$ yay -Ps

# 检查安装的版本
$ yay -Qi package

```

system入门命令
``` bash
# 帮助调试系统开机启动问题。该命令会重演你的系统启动的所有消息
$ journalctl -b

# 通过命令 journalctl -b -1 可以复审前一次启动，journalctl -b -2 可以复审倒数第 2 次启动，以此类推
$ journalctl _PID=1

# 一个最有用的命令工具之一 systemd-analyze blame，用于帮助查看哪个服务进程启动耗时最长
$ systemd-analyze blame

# 查看启动耗时
$ systemd-analyze blame

# 显示瀑布状的启动过程流
$ systemd-analyze critical-chain

# 显示指定服务的启动流
$ systemd-analyze critical-chain atd.service

```

yaourt常用命令

    这个已经不更新了，但有些软件的安装还能用

``` bash
# 从AUR安装软件包
$ yaourt -S package_name

# 使用关键字搜索软件包
$ yaourt -Ss password

# 从AUR升级本地软件数据库并安装更新
$ yaourt -Syu –aur

# 列出软件包信息
$ yaourt -Si package_name

# 从缓存中清除旧的软件包
$ yaourt -Sc

# 安装AUR中的更新软件包
$ yaourt -Su

# 获取最新的AUR软件包数据库
$ yaourt -Sy

# 清除AUR软件包数据库
$ yaourt -Cd

# 删除软件包
$ yaourt -R package_name

# 获得更多信息
$ yaourt –help
```


安装软件
``` bash
# 测试最新最快的镜像源，选择应用
$ sudo pacman-mirrors -i -c China -m rank

# 更新软件源
$ sudo pacman -Syyu

# 安装基础软件包
$ sudo pacman -Sy base-devel

# 安装AUR助手，建议yay
$ sudo pacman -S yay

# 安装处理器微码(Intel处理器)
$ sudo pacman -S intel-ucode

# 安装处理器微码(AMD处理器)
$ sudo pacman -S amd-ucode

# Vim文本编辑器（个人倾向用用neovim）
$ sudo pacman -S vim
$ sudo pacman -S neovim

# NTFS格式驱动器读写支持
$ sudo pacman -S ntfs-3g

```

Archlinuxcn源添加
``` bash
# 更换源，不推荐Manjaro添加Archlinuxcn源，但添加的方法也写在这里
$ sudo vi /etc/pacman.conf
# 加入以下内容

[archlinuxcn]
SigLevel = Optional TrustedOnly
Server = https://mirrors.ustc.edu.cn/archlinuxcn/$arch

# 然后更新
$ sudo pacman -S archlinuxcn-keyring
$ sduo pacman -Syy

```

yaourt安装

# yaourt已经不再维护了，但是经过测试还是有些软件得用yaourt进行安装
# 最简单的方法就是将yaourt的源添加到 /etc/pacman.conf:
``` bash
[archlinuxfr]
Server = http://repo.archlinux.fr/$arch

# 然后更新
$ sudo pacman -Sy
$ sduo pacman -S yaourt

```

加速yaourt
``` bash
pacman能够调用外部下载工具来代替默认的wget来给pacman提速
比如将/etc/pacman.conf中
XferCommand = /usr/bin/wget –passive-ftp -c -O %o %u
注释掉,同时添加
XferCommand = /usr/bin/axel -o %o %u
即可使用axel多线程下载工具
而要让yaourt也支持多线程,只需修改/etc/makepkg.conf

将’http::/usr/bin/wget -c -t 3 –waitretry=3 -O %o %u’改成了’http::/usr/bin/axel -o %o %u’

```

安装字体
``` bash
# 更新软件源
$ sudo pacman -Syyu

# 安装字体
$ sudo pacman -S adobe-source-han-serif-cn-fonts wqy-zenhei noto-fonts-cjk noto-fonts-emoji noto-fonts-extra

```
安装输入法
``` bash
# 更新软件源
$ sudo pacman -Syyu

# 安装Fcitx5输入引擎、输入法和皮肤
$ sudo pacman -S fcitx5-im fcitx5-chinese-addons fcitx5-qt fcitx5-gtk fcitx5-nord fcitx5-pinyin-zhwiki-rime fcitx5-pinyin-moegirl fcitx5-config-qt

# 加入系统环境变量
$ sudo vim /etc/environment
	# 在最后一行加入下面内容保存
	GTK_IM_MODULE=fcitx
	QT_IM_MODULE=fcitx
	XMODIFIERS=@im=fcitx
	SDL_IM_MODULE=fcitx
	# 最后重启去系统设置输入法配置下即可使用了

# 编辑.xprofile
$ sudo nano ~/.xprofile
	# 添加或修改文件为
	export GTK_IM_MODULE=fcitx5
	export QT_IM_MODULE=fcitx5
	export XMODIFIERS="@im=fcitx5"
	fcitx5 &

# 编辑~/.xinitrc
$ sudo nano ~/.xinitrc
	#注意这里是吧内容添加在exec $(get_session)之前
	export GTK_IM_MODULE=fcitx5
	export XMODIFIERS=@im=fcitx5
	export QT_IM_MODULE=fcitx5

# 把Fcitx5加入开机启动项
$ sudo cp /usr/share/applications/fcitx5.desktop /etc/xdg/autostart/

```

显卡问题

    Manjaro自带非常方便的硬件安装图形化界面，但万一不行还可以自己手动进行安装调试
``` bash
# 此处先卸载再安装
# 查看已经安装的软件包
$ pacman Q
$ sudo pacman -R mhwd mhwd-db mhwd-amdgpu mhwd-nvidia mhwd-nvidia-390xx optimus-manager optimus-manager-qt xf86-video-nouveau xf86-video-amdgpu xf86-video-ati

# 查看现在的硬件信息
$ inxi -G

# 安装 Manjaro mhwd 硬件驱动管理器（请勿在 Arch 下尝试）
$ sudo pacman -S mhwd

# 安装 NVIDIA 闭源驱动
$ sudo mhwd -a pci nonfree 0300

# 重启系统
$ sudo reboot

# 查看独显驱动情况
$ mhwd -li
	# 看到类似下面的输出，现在显卡应该驱动正常
	> Installed PCI configs:
-------------------------------------------------------------------------------
            NAME                   VERSION               FREEDRIVER        TYPE
-------------------------------------------------------------------------------
video-hybrid-intel-nvidia-prime   2021.10.3               true            PCI
```


常用软件安装列表
``` bash
# 更新软件源
$ sudo pacman -Syyu

# timeshift 必装！折腾必备
$ sudo pacman -S timeshift

# neofetch 快速查看系统信息
$ sudo pacman -S neofetch lolcat

# 安装 Telegram
$ sudo pacman -S telegram-desktop

# 安装 Node.js、npm、yarn
$ sudo pacman -S git nodejs npm yarn

# 安装 Gwenview 看图工具和 Ark 解压工具
$ sudo pacman -S gwenview ark

# 火焰截图软件，类似 Windows 下的 Snipaste
$ sudo pacman -S flameshot

# 一个好用的 Dock 栏
$ sudo pacman -S latte-dock

# 视频播放器 mpv 和 vlc
$ sudo pacman -S mpv vlc

# OBS串流工具，录屏很很用
$ sudo pacman -S obs-studio

# 在屏幕上显键盘按键
$ sudo pacman -S screenkey

# 安装图书管理软件 Calibre 
$ sudo pacman -S calibre

# Albert 软件启动神器
$ sudo pacman -S albert

# 安装docker
$ sudo pacman -S docker
$ systemctl start docker
	#docker镜像加速可以登录阿里，获取免费加速服务。
	https://cr.console.aliyun.com/cn-qingdao/instances/


# 刷新 AUR 软件源
$ yay -Syy

# 安装 Chrome 浏览器
$ yay -S google-chrome

# Android-Tools
$ sudo pacman -S android-tools

# imagine 图像优化工具
$ yay -S imagine-git calibre-git 

# 安装 Typora
$ yay -S typora

# 安装 V2raya
$ yay -S v2raya v2ray

# 安装 VScode
$ yay -S visual-studio-code-bin

# 安装 Android Studio
$ yay -S android-studio 

# Jetbrains 全家桶管理工具（包含Android Studio）
$ yay -S jetbrains-toolbox 

# 简单的 Hex 编辑器 
$ yay -S bless

# Etcher U盘镜像写入工具
$ yay -S etcher-bin

# Bitwarden 密码管理器
$ yay -S bitwarden-bin

# 安装网易云音乐
$ yay -S netease-cloud-music

# 安装WPS
$ yay -S wps-office
$ yay -S ttf-wps-fonts
	#避免安装完后打开Wps提示字体不存在，这里建议先安装字体
	#Times new roman字体
	$ yay -S ttf-ms-fonts
	
# 安装Atom
$ yay -S atom

# 安装Notion
$ yay -S notion-app

# 安装Picgo，配合Github搭建图床
$ yay -S picgo-appimage

# 安装Okular，PDF阅读器
$ yay -S okular

# 安装Spotify
$ yay -S spotify

# 安装flame截图
$ yay -S flameshot
	# 设置flameshot：
	$ flameshot config
	
# 安装微信
$ yay -S deepin-wine-wechat

# 安装teams
$ yay -S teams

# 安装blechbit,清理磁盘空间
$ yay -S bleachbit

# 安装深度截图
$ yay -S deepin-screenshot

```

安装Latex
``` bash
# 安装图形化安装界面
$ yay -S tk
# 挂载ISO

$ sudo mount -o loop /path/to/texlive2020.iso /mnt
# 安装,.去除不需要的语言和TeXworks editor，点击安装

$ cd /mnt
$ sudo ./install-tl--gui
# 添加环境变量

$ sudo vim ~/.zshrc
	# 在文件末尾添加
	export MANPATH=/usr/local/texlive/2020/texmf-dist/doc/man:$MANPATH 
	export INFOPATH=/usr/local/texlive/2020/texmf-dist/doc/info:$INFOPATH
	export PATH=/usr/local/texlive/2020/bin/x86_64-linux:$PATH
	
# 测试环境变量
$ tex --version

# 设置清华镜像为更新源
$ sudo tlmgr option repository  https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/tlnet

# 更新tlmgr和宏包
$ sudo tlmgr update --self --all

# 安装字体,复制要安装的字体文件（ttf）到/usr/share/fonts/TTF文件夹:
# 刷新字体:
$ fc-cache -fv

# 更新字体缓存
$ cd /usr/share/fonts/ #字体的位置
$ sudo fc-cache -v -f  #更新缓存

```

安装Virtualbox
``` bash
# 查看内核版本
$ mhwd-kernel -li
	# 比如输出为：
	Currently running: 5.10.42-1-MANJARO (linux510)
	The following kernels are installed in your system:
  	 * linux510
  	 
# 安装 virtualbox，将 510 替换为上一步结果
$ sudo pacman -Syu virtualbox linux510-virtualbox-host-modules

# 查看 Virtualbox 版本
$ vboxmanage --version

	# 比如输出为：
	$ vboxmanage --version
	6.1.22r144080
	
# 安装拓展包，记得选择跟 Virtualbox 版本号一致的
$ yay -Syy && yay virtualbox-ext-oracle

# 将VirtualBox模块添加到内核中，然后重启。
$ sudo vboxreload

```

主题美化
``` bash
# KDE主题网站
https://store.kde.org/

# 安装支持
$ yay -S ocs-url

# 可以选择安装conky
$ sudo pacman -S conky conky-manager
	# 新建文件夹mkdir ~/.conky，将conky配置文件复制到该目录下，我的配置文件在github(内含vim、nvim配置)
	$ git clone https://github.com/Farrell-Z/myvimrc.git
	$ mv myvimrc/myconky ~/.conky/
	# 应用程序打开 conky manager，选择配置文件。

```
KDE主题
``` bash
# latte-dock透明布局
https://store.kde.org/p/1221421

# 资源监控widget
https://store.kde.org/p/998913/

# 使用的图标Papirus

# 时间插件Minimal Clock
https://store.kde.org/p/1173746/

```

fish
``` bash
# 安装fish并修改默认shell：
$ sudo pacman -S fish
$ chsh -c /usr/bin/fish

# 安装oh-my-fish:
curl -L https://get.oh-my.fish | fish

配置fish：fish_config自动打开web浏览器。
# Konsole基础配置：

# 复制前面github上下载的我的配置到nvim目录中：
$ mkdir ~/.config/nvim
$ mv myvimrc/init.vim ~/.config/nvim

# 安装插件管理器：
curl -fLo ~/.local/share/nvim/site/autoload/plug.vim --create-dirs \https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim

# 提供python支持：
$ pip install pynvim jedi

```

ohmyzsh
``` bash
# 查看系统所有shell
$ cat /etc/shells

# 更改默认shell为zsh
$ chsh -s /usr/bin/zsh

# 安装ohmyzsh
$ wget https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | sh

# 安装插件
# zsh-syntax-highlighting：语法高亮
$ git clone https://github.com/zsh-users/zsh-syntax-highlighting.git
${ZSH_CUSTOM}/plugins/zsh-syntax-highlighting

# autosuggestions：记住用过的命令
$ git clone git://github.com/zsh-users/zsh-autosuggestions ~/.oh-my-zsh/custom/plugins/zsh-autosuggestions

# 修改主题
# 这里使用的主题是powerlevel10k，详细信息可从Github找到
$ git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/themes/powerlevel10k

# 修改配置文件
$ sudo vim ~/.zshrc

	# 详情参考以下内容
	#更改ZSH_THEME
	ZSH_THEME="powerlevel10k/powerlevel10k"
	#更改plugins
    plugins=(git
       zsh-autosuggestions
       zsh-syntax-highlighting
     )
 	#刷新配置
 	source ~/.zshrc
 	#打开终端按提示进行配置即可

```

机器学习环境
``` bash
# 安装cuda与cudnn
$ sudo pacman -S cuda cudnn
	#验证安装
	完成之后，我们进入cuda的安装路径，我的路径是/opt/cuda，你可以使用下面的命令将CUDA的示例程序拷贝到你的用户主目录下，之后编译程序
	cp -r /opt/cuda/samples ~
	cd ~/samples
	sudo make
	此时就使用nvcc编译器开始编译CUDA的sample程序，这个花费时间更长，应该在半小时左右，等待编译结束，使用下面的命令验证是否成功,在窗口中查看最后一行的结果是否为pass，如果是则表示CUDA安装成功。
	cd ~/samples/bin/x86_64/linux/release
./deviceQuery

# 安装tensorflow1.13
$ conda install tensorflow

# 安装tensorflow2.0
$ cuda search cuda  # 查看可安装的cuda
$ conda search cuda --info # 查看详细信息
	#找到相关版本的url地址 复制到浏览器下载之后conda file安装，或者直接conda install url 安装，tensorflow 一般比较吃cuda的版本
	
# tensorflow2.0使用 cuda10.0版本
$ conda install cudnn  #自动安装cudnn6，使用conv2D 操作无法初始化cudnn 需要升级使用cudnn7.3.1
$ conda install cudnn==7.3.1

#Pytorch安装
https://pytorch.org/
	#验证
	用  optirun  python 开python交互，，
	import torch
	torch.cuda.is_available()
	
#Anaconda3安装配置
$ yay -Sy miniconda3
	#手动安装
	Miniconda清华源 https://mirrors.tuna.tsinghua.edu.cn/anaconda/miniconda/
	$ sh Miniconda3*.sh
	
	#安装后配置
	# 在 ~/.bashrc 中添加
	export PATH=/opt/anaconda/bin:$PATH
	# 激活
	source /opt/anaconda/bin/activate root
	
	# 安装完之后,如果输入conda找不到命令,需要source ~/.bashrc，如果不行就终端运行如下命令添加内容到.bashrc然后重新source：
	$ echo "[ -f /opt/miniconda3/etc/profile.d/conda.sh ] && source /opt/miniconda3/etc/profile.d/conda.sh" >> ~/.bashrc
	
		# zsh中配置
		$ sudo vim ~/.zshrc
			# 添加下面这条语句：
			export PATH="/opt/anaconda/bin:$PATH"
			# 添加完成并保存退出后，使环境变量生效：
			$ source ~/.zshrc
			
	#配置conda源
		#编辑~/.condarc文件，添加如下内容：
		# 我的添加，注意顺序，越靠上优先级越高
		channels:
 		 - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
 		 - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free
  		 - https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/pytorch/
 		 - https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/fastai/
 		 - https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/bioconda/
 		 - https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/menpo/
 		 - https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/
 		 - defaults   # conda官方的
		show_channel_urls: true
		# 清除索引缓存，保证用的是镜像站提供的索引
		$ conda clean -i
		# 测试一下
		$ conda create -n myenv numpy
		
		# 或者运行命令逐条增加
		$ conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
		$ conda config --set show_channel_urls yes

```

conda使用
``` bash
# 查看当前已创建的所有环境
$ conda env list

# 查看软件或包的安装位置
$ which numpy
$ which numpy

# 查看已安装软件
# conda list

# 创建一个项目环境
$ conda create -n NAME python=3.7

# 激活所需的虚拟环境
$ conda activate ysy # 激活指定环境
$ conda activate base # 激活默认环境

# 退出某个conda虚拟环境:
$ conda deactivate

# 安装需要的包
强烈推荐使用pip安装包，而不是conda
	# 安装某个包
	$ conda install numpy
	# 安装某个指定版本的包
	$ conda install numpy=1.9
	# 同时安装多个包
	$ conda install pytorch torchvision torchaudio cudatoolkit=10.2
	# 安装到指定环境
	$ conda install numpy -n NAME
	# 指定安装路径
	$ conda install numpy --prefix=/path/to/envName
	# 指定channel安装
	$ conda install mingw libpython -c https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/

# 升级conda
$ conda update -n base conda

# 更新软件/包
$ conda update numpy
	# 更新到指定版本
	$ conda update numpy=1.9.3

# 卸载软件/包
$ conda remove numpy

# 重命名某个conda虚拟环境
可以通过clone and remove old来实现重命名
$ conda create -n newName --clone oldName

# 删除原来名称的全部环境
$ conda remove -n oldName --all
```
————————————————
版权声明：本文为CSDN博主「Acfufu」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u014447560/article/details/120631159




修改Terminator终端默认大小

~/.config/terminator/config
``` 
[global_config]
  title_transmit_bg_color = "#4e9a06"
[keybindings]
[layouts]
  [[default]]
    [[[child1]]]
      parent = window0
      profile = default
      type = Terminal
    [[[window0]]]
      parent = ""
      type = Window
      size = 900, 600
[plugins]
[profiles]
  [[default]]
    background_color = "#2d2d2d"
    cursor_color = "#e1e1e1"
    cursor_shape = ibeam
    font = Monospace 15
    foreground_color = "#e1e1e1"
    palette = "#000000:#cc0000:#4e9a06:#c4a000:#3465a4:#75507b:#06989a:#d3d7cf:#555753:#ef2929:#8ae234:#fce94f:#729fcf:#ad7fa8:#34e2e2:#eeeeec"
    use_system_font = False

```

.bashrc
``` bash
# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color|*-256color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# colored GCC warnings and errors
#export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi


```