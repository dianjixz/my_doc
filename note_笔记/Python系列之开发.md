1. venv介绍

venv是python3.3及以后的版本中新增的虚拟环境管理工具包，用于取代之前的virtualenv，因此不用通过pip install venv再去安装。
2. venv基本操作

    2.1 首先，创建虚拟环境

在我的账号家目录下，创建空的文件夹，然后在新建文件夹下，利用venv创建项目根目录env。可以发现在env目录下，有3个文件夹和1一个文件。

其中bin目录用于保存项目运行相关的启动脚本，lib目录用于保存本项目依赖的系统库和第三方库，include用于保护项目依赖的c头文件（一般为空），剩下的pyenv.cfg顾名思义就是虚拟环境的配置文件。

wen.cao@MBC02XD15UJGH7 ~ % mkdir DemoPython
wen.cao@MBC02XD15UJGH7 ~ % cd DemoPython 
wen.cao@MBC02XD15UJGH7 DemoPython % 
wen.cao@MBC02XD15UJGH7 DemoPython % python3 -m venv env
wen.cao@MBC02XD15UJGH7 DemoPython % ls -l
total 0
drwxr-xr-x  6 wen.cao  656741719  192  7 24 22:41 env
wen.cao@MBC02XD15UJGH7 DemoPython % ls -l env 
total 8
drwxr-xr-x  13 wen.cao  656741719  416  7 24 22:41 bin
drwxr-xr-x   2 wen.cao  656741719   64  7 24 22:41 include
drwxr-xr-x   3 wen.cao  656741719   96  7 24 22:41 lib
-rw-r--r--   1 wen.cao  656741719  104  7 24 22:41 pyvenv.cfg

    2.2 紧接着，我们需要激活虚拟环境

在激活以后，前面就出现了（env）提示符，表示激活成功。另外，激活后的python解释器和pip的位置就变成虚拟环境的了，而不是操作系统默认的路径。

wen.cao@MBC02XD15UJGH7 DemoPython % source env/bin/activate
(env) wen.cao@MBC02XD15UJGH7 DemoPython %
(env) wen.cao@MBC02XD15UJGH7 DemoPython % which python
/Users/wen.cao/DemoPython/env/bin/python
(env) wen.cao@MBC02XD15UJGH7 DemoPython % which python3
/Users/wen.cao/DemoPython/env/bin/python3
(env) wen.cao@MBC02XD15UJGH7 DemoPython % which pip 
/Users/wen.cao/DemoPython/env/bin/pip
(env) wen.cao@MBC02XD15UJGH7 DemoPython % which pip3
/Users/wen.cao/DemoPython/env/bin/pip3

    2.3 虚拟环境的包管理

可以发现，正常激活后，操作系统下的python包，并没有带进虚拟环境，这一点对于我们很有用，因为任何一个python项目依赖的包其实是不一样的，这也是venv存在的意义。

当然，我们可以在虚拟环境安装自己的包，此时的包是不会影响到操作系统或者其他虚拟环境的。

(env) wen.cao@MBC02XD15UJGH7 DemoPython % pip list
Package    Version
---------- -------
pip        19.2.3 
setuptools 41.2.0 
WARNING: You are using pip version 19.2.3, however version 21.1.3 is available.
You should consider upgrading via the 'pip install --upgrade pip' command.
(sample) wen.cao@MBC02XD15UJGH7 DemoPython % pip install --upgrade pip
Collecting pip
  Using cached https://files.pythonhosted.org/packages/47/ca/f0d790b6e18b3a6f3bd5e80c2ee4edbb5807286c21cdd0862ca933f751dd/pip-21.1.3-py3-none-any.whl
Installing collected packages: pip
  Found existing installation: pip 19.2.3
    Uninstalling pip-19.2.3:
      Successfully uninstalled pip-19.2.3
Successfully installed pip-21.1.3
(env) wen.cao@MBC02XD15UJGH7 DemoPython % 
(env) wen.cao@MBC02XD15UJGH7 DemoPython % pip list
Package    Version
---------- -------
pip        21.1.3
setuptools 41.2.0

    2.4 开始开发项目

我们以一个test.py的脚本来代替一个完整项目源码。

(env) wen.cao@MBC02XD15UJGH7 DemoPython % echo "print(\"hello venv\")\n" > test.py
(sample) wen.cao@MBC02XD15UJGH7 DemoPython % cat test.py                             
print("hello venv")

(env) wen.cao@MBC02XD15UJGH7 DemoPython % ls
env	test.py

    2.5 运行调试项目

可以看到，我们是用venv/bin中的python解释器来运行我们的项目的。

(env) wen.cao@MBC02XD15UJGH7 DemoPython % which python
/Users/wen.cao/DemoPython/env/bin/python
(env) wen.cao@MBC02XD15UJGH7 DemoPython % python test.py
hello venv

    2.6 退出虚拟环境

当项目开发完毕以后，就可以退出虚拟环境，回到操作系统的python环境里。

(env) wen.cao@MBC02XD15UJGH7 DemoPython % deactivate
wen.cao@MBC02XD15UJGH7 DemoPython % 
wen.cao@MBC02XD15UJGH7 DemoPython % which python
/usr/bin/python




python3 -m venv env
source env/bin/activate
deactivate