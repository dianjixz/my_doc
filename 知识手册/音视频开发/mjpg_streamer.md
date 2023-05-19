交叉编译 mjpg_streamer
====================


 jpeg库的移植

MJPG 库下载地址：

http://www.ijg.org/files/，这里我使用的是jpegsrc.v8b.tar.gz

在当前用户的家目录下创建一个mjpg文件夹：

mkdir mjpg

进入刚创建的mjpg目录，将下载好的jpegsrc.v8b.tar.gz压缩包放到mjpg目录，使用下面命令解压

tar -zxvf jpegsrc.v8b.tar.gz

接着在mjpg目录下创建mjpg_install目录，后面编译生成的文件放在次目录。

1.1 配置编译选项

进入 jpeg-8b 文件夹，执行以下命令配置编译选项：

./configure --prefix=/home/cwz/mjpg/mjpg_install --host=arm-linux-gnueabihf

参数说明：
–-prefix=/home/cwz/mjpg/mjpg_install：指定安装的路径要使用绝对路径，这里根据自己的路径自行调整。
–-host=arm-linux-gnueabihf：将来软件要运行所在主机的架构。

image-20221220214331156
1.2 使用make命令编译

make install

    1

image-20221220214639790

然后进入mjpeg_install(安装目录) 文件夹下查看，以下lib和include目录的路径会在后面用到。

image-20221220215242442
1.3 移植编译好的文件到开发板

将mjpeg_install 目录下的lib目录下的库文件拷贝到开发板的/lib/目录下：

我这里PC端和开发板用NFS将PC端上的 /home/workdir/ 挂载到开发板 /mnt/ 目录。所以使用下面将库文件拷贝到开发板。

PC端：

sudo cp *jpeg* /home/workdir/

    1

image-20221220215651362

开发板：

这里先查看下有没有将库文件全部拷贝到开发板的/mnt/目录下

image-20221220220645273

sudo cp /mnt/*jpeg* /lib/

    1

image-20221220220533032
2 mjpg-streamer 的移植

mjpg-streamer 是一个命令行应用程序，可通过网络将 JPEG 文件从网络摄像头流传输到各种类型的查看器，例如浏览器，mplayer 等其他能够接收 MJPG 流的软件。

下载链接接：https://sourceforge.net/p/mjpg-streamer/code/HEAD/tree/

下载命令：

svn checkout https://svn.code.sf.net/p/mjpg-streamer/code/ mjpg-streamer-code 

    1

如果执行以上命令提示如下，

image-20221220222514203

则说明需要先安装软件，输入以下命令安装

sudo apt install subversion

    1

安装完成后，再执行上面的下载命令，等待下载命令执行结束，可以看到mjpg-streamer-code文件夹

image-20221220223054635

进入mjpg-streamer-code/mjpg-streamer/ ,执行 以下命令

make clean

    1

image-20221220223808549
2.1 Makefile中添加编译信息

进入到mjpg-streamer-code/mjpg-streamer/plugins/input_uvc目录，修改Makefile。

image-20221220224446867

vim Makefile

    1

添加以下内容，是上面移植mjpg库时安装目录下生成的include目录和lib目录，根据之前设置的路径自行调整。

image-20221221221826812
2.2 使用make编译mjpg-streamer

返回到mjpg-streamer目录下，执行make命令前，要设置交叉编译器，可以使用下面任一种种方法设置：
方法1：执行以下命令进行编译

make CC=arm-linux-gnueabihf-gcc

    1

出现错误，如下,这里我只记得提示的是未找到convert命令，下图不是当时出错具体信息，大概是这个意思。

image-20221221220913842

解决：

这里是因为Makefile里面使用的convert命令识别不了，安装以下软件，就可以了。

sudo apt install graphicsmagick-imagemagick-compat
sudo apt install imagemagick-6.q16
sudo apt install imagemagick-6.q16hdri

    1
    2
    3

安装好上面的软件，编译前先执行下面命令清除掉之前编译的，

make clean

    1

再使用以下命令重新编译。

make CC=arm-linux-gnueabihf-gcc

    1

方法2 ：配置交叉编译工具：

image-20221221110339840

从上图可以看到Makefile文件里的CC = gcc，这里要更改下。输入以下命令：

find -name "Makefile" -exec sed -i "s/CC = gcc/CC = arm-linux-gnueabihf-gcc/g" {} \;

    1

这行命令是搜索当前目录及其子目录下的所有Makefile文件，并将Makefile里的CC变量设置为arm-linux-gnueabihf-gcc，因为Makefile默认是编译的软件运行在x86平台的。这里要移植到开发板，所以要更改使用交叉编译。

查看是否修改成功

grep "arm-linux-gnueabihf-gcc" * -nR

    1

image-20221221110633058

回到 mjpg-streamer 目录下，执行 make 编译，

make

    1

编译成功后在目录下会生成：

.so 库文件：移植到开发板/lib，提供动态库
mjpg_streamer：移植到开发板/bin，提供可执行命令
www：移植到开发板/opt，摄像头输出的网页

image-20221221112404030
2.3 移植编译好的文件到开发板

PC端：

sudo cp *.so start.sh mjpg_streamer /home/workdir/
sudo cp www /home/workdir/ -rdf

    1
    2

image-20221221115849071

开发板端：

通过NFS，可以看到文件已经从PC端拷贝到开发板/mnt/目录下了。

image-20221221120246579

接下来分别将文件拷贝到开发板的其他目录下。

sudo cp /mnt/*.so /lib/
sudo cp /mnt/mjpg_streamer /bin/
sudo cp /mnt/www /opt/ -rdf

    1
    2
    3

执行完上面的步骤，就完成全部移植了。
2.4 测试是否移植成功

在开发板上运行以下命令测试

 mjpg_streamer --help

    1

开发板上，巴拉巴拉输出一堆内容
在这里插入图片描述

说明成功移植到开发板上。
2.5 mjpg_streamer的运行

这个命令我不知道干嘛的，执行之后报了一堆错。不管了。

mjpg_streamer -i "input_uvc.so -d /dev/video0" -o "output_http.so"

    1

input_uvc.so -d：打开设备节点 /dev/video1

output_http.so：输出信息通过http发送出去

输入以下命令运行mjpg_streamer

``` bash
mjpg_streamer -i "input_uvc.so -d /dev/video0 -f 30 -q 90 -n" -o "output_http.so -w /opt/www"

# web 访问 http://10.254.239.1:8080/stream.html
```

执行命令，出现以下错误，我这里是因为开发板（imx6ull pro）未安装v4l-utils工具

image-20221221152131203
2.5.1 v4l-utils工具安装

sudo apt install v4l-utils

    1

再次进输入上面mjpg_streamer的命令，成功运行如下：

image-20221221154608512

注意：当Ctrl + z,中断进程的时候，再次运行mjpg_streamer，会出现错误，需要先kill这个进程，方可重新运行。

image-20221221161032527
2.5.2 PC端查看运行结果

开发板运行mjpg_streamer成功后，在PC端打开浏览器：输入开发板的IP地址和8080端口号，点击Stream选项会出现图像。

image-20221221154246434
————————————————
版权声明：本文为CSDN博主「colorful-卡勒」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/colortztzztzt/article/details/128402874