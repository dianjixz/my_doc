libffi-dev

apt-get install libbz2-dev libncurses5-dev libgdbm-dev liblzma-dev sqlite3 libsqlite3-dev openssl libssl-dev tcl8.6-dev tk8.6-dev libreadline-dev zlib1g-dev libffi-dev





在编译的时候，Python出现了下面的提示信息：

```ruby
Python build finished, but the necessary bits to build these modules were not found:



   _bsddb             _sqlite3           _ssl



   _tkinter           bsddb185           bz2



   dbm                gdbm               readline



   sunaudiodev



To find the necessary bits, look in setup.py in detect_modules() for the module's name.
```

当然，每个机器上因为不同的配置，编译报错信息也会不同，比如网上有人的报错就是下面这样的：

```ruby
Python build finished, but the necessary bits to build these modules were not found:



  _bsddb				 _curses				_curses_panel	



  _sqlite3			  _ssl					_tkinter		  



  bsddb185			  bz2					 dbm				 



  dl					  gdbm					imageop			



  readline			  sunaudiodev		  zlib				



To find the necessary bits, look in setup.py in detect_modules() for the module's name.
```

无论报错信息如何，意思很明确，我们编译的时候，系统没有办法找到对应的模块信息，为了解决这些报错，我们就需要提前安装依赖包，这些依赖包对应列表如下（不一定完全）：

| 模块          | 依赖               | 说明                                                         |
| ------------- | ------------------ | ------------------------------------------------------------ |
| _bsddb        | bsddb              | Interface to Berkeley DB library。Berkeley数据库的接口       |
| _curses       | ncurses            | Terminal handling for character-cell displays。              |
| _curses_panel | ncurses            | A panel stack extension for curses。                         |
| _sqlite3      | sqlite             | DB-API 2.0 interface for SQLite databases。SqlLite，CentOS可以安装sqlite-devel |
| _ssl          | openssl-devel.i686 | TLS/SSL wrapper for socket objects。                         |
| _tkinter      | N/A                | a thin object-oriented layer on top of Tcl/Tk。如果不使用桌面程序可以忽略TKinter |
| bsddb185      | old bsddb module   | 老的bsddb模块，可忽略。                                      |
| bz2           | bzip2-devel.i686   | Compression compatible with bzip2。bzip2-devel               |
| dbm           | bsddb              | Simple “database” interface。                                |
| dl            | N/A                | Call C functions in shared objects.Python2.6开始，已经弃用。 |
| gdbm          | gdbm-devel.i686    | GNU’s reinterpretation of dbm                                |
| imageop       | N/A                | Manipulate raw image data。已经弃用。                        |
| readline      | readline-devel     | GNU readline interface                                       |
| sunaudiodev   | N/A                | Access to Sun audio hardware。这个是针对Sun平台的，CentOS下可以忽略 |
| zlib          | Zlib               | Compression compatible with gzip                             |

在CentOS下，可以安装这些依赖包：readline-devel，sqlite-devel，bzip2-devel.i686，openssl-devel.i686，gdbm-devel.i686，libdbi-devel.i686，ncurses-libs，zlib-devel.i686。完成这些安装之后，可以再次编译，上表中指定为弃用或者忽略的模块错误可以忽略。



 在编译完成之后，就可以接着上面的第六步安装[Python](https://so.csdn.net/so/search?from=pc_blog_highlight&q=Python)到指定目录下。安装完成之后，我们可以到安装目录下查看Python是否正常安装。

```ruby
$ pushd /usr/local/python-2.7.6/bin



$ python --version



$ sudo mv /usr/bin/python /usr/bin/python.old



$ sudo ln -s python /usr/bin/python



$ popd



$ python -V



$ sudo sed -i "s/#\!\/usr\/bin\/python/#\!\/usr\/bin\/python.old/" /usr/bin/yum
```

\1. 我们进入到安装路径下的bin目录， 并查看安装的版本（版本是2.7.6）

\2. 将现有的python版本重命名备份

\3. 创建新版本Python到 `/usr/bin` 目录下替代原有python

\4. 测试系统Python版本（应该是新的2.7.6）

\5. 此时如果yum不能正常工作，可以简单的通过sed将里面的Python引用替换成原来的Python路径即可。

至此，完成Python的更新编译安装工作。如果你想完成自动安装，可以将前面的代码保存为Shell脚本文件——需要注意的是，你可能需要添加各个模块的安装脚本，本篇的是通过yum来安装的。也可以通过参考2中的文章，文中包含自动安装的脚本文件。 

```bash
./configure --prefix=/usr/local/python
```



../configure --host=aarch64-linux-gnu --build=aarch64 ac_cv_file__dev_ptmx=no ac_cv_file__dev_ptc=no --disable-ipv6 --prefix=/home/nihao/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/python3


aPy5W9x.

export LD_LIBRARY_PATH=”$LD_LIBRARY_PATH:/home/nihao/host/lib” 

../configure CC=aarch64-linux-gnu-gcc LD=aarch64-linux-gnu-ld --host=aarch64-linux-gnu --prefix=/home/nihao/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/aarch64-linux-gnu  --enable-shared --enable-static


_PYTHON_SYSCONFIGDATA_NAME