linux 终端下一个图片处理软件包，包含多个程序
ImageMagick



sudo apt install imagemagick



 ImageMagick介绍

在Linux下，Gimp的图片处理能力固然很强大，但它是基于图形界面的，用户有时需要在命令行模式处理对一些图片进行处理，这时可就得请求ImageMagick的帮助了！

ImageMagick是一个免费的创建、编辑、合成图片的软件。它可以读取、转换、写入多种格式的图片。图片切割、颜色替换、各种效果的应用，图片的旋转、组合，文本，直线，多边形，椭圆，曲线，附加到图片伸展旋转。其全部源码开放，可以自由使用，复制，修改，发布。它遵守GPL许可协议。它可以运行于大多数的操作系统。最为重要的是，ImageMagick的大多数功能的使用都来源于命令行工具。

ImageMagick的命令行工具包括：
[ convert | identify | mogrify | composite | montage | compare | display | animate | import | conjure ]

ImageMagick擅长令行工具来处理图片，可能你已经习惯每次编辑图片都使用具有图形用户接口(GUI)的编辑工具，就像GIMP和PhotoShop一样。 然而，一个图形用户接口不总是方便的。 假如你想要从一个网页动态地处理一个图像，或者你在不同时间生成一样的或不同的图像，想要对许多图像或重复特定应用相同的操作。 对于操作的这些类型，用程序命令行工具是一个好的选择。

下面对每个命令行工具做一下简短的描述，如果想获得更多详细信息，请阅读它们的帮助文档：

    convert：转换图像格式和大小，模糊，裁剪，驱除污点，抖动，临近，图片上画图片，加入新图片，生成缩略图等。
    identify：描述一个或较多图像文件的格式和特性。
    mogrify：按规定尺寸生成一个图像，模糊，裁剪，抖动等。Mogrify改写最初的图像文件然后写到一个不同的图像文件。
    composite：根据一个图片或多个图片组合生成图片。
    montage：创建一些分开的要素图像。在含有要素图像任意的装饰图片，如边框、结构、图片名称等。
    compare：在算术上和视觉上评估不同的图片，评估其它的改造图片。
    display：如果你拥有一个X server的系统，它可以按次序的显示图片
    animate：利用X server显示动画图片
    import：在X server或任何可见的窗口上输出图片文件。 你可以捕获单一窗口，整个的荧屏或任何荧屏的矩形部分。
    conjure：解释执行 MSL (Magick Scripting Language) 写的脚本。


下面通过一个简单示例对增加一点感性认识：
convert -crop 300x300+0+0 -resize 200x200 -colors 100 +profile "*" 1.jpg 1.png

表示：把1.jpg先从左上角(0,0)为起点裁剪一块300x300的部分，然后把这部分缩小到200x200，然后把颜色缩减到100色，最后生成1.png。
+profile "*": 意味着在图片文件里不存储附加信息，以减小图片体积。
注意，-resize会试图保留原图的视觉比例，因此不一定生成的图片和你指定的尺寸一定匹配，比如，如果2.jpg大小是400x200，所以如果用命令：convert 2.jpg -resize 100x100 2.png，那么生成的图片2.png的实际大小是100x50。

常用的参数有：

    -crop 宽x高+起点横坐标+起点纵坐标：裁剪图
    -resize 宽x高!：改变尺寸，如果使用惊叹号，表示不保留视觉比例，强行改变尺寸匹配给定的宽和高；如果仅给定宽或者高，如“宽x”或“x高”形式的参数(“x高”与“宽x高”的效果是一样的)，则以已知参数为基准按比例改变尺寸。

    -colors 颜色数：设定图片采用的颜色数，如果是生成png或gif图片应指定这个参数
    -quality 质量：设定jpeg图片输出质量，推荐采用80，此命令仅用于输出格式是jpg的情况，不应省略，省略的话默认质量是95，生成图片过大

    +profile "*"：图片中不存储附加信息，必须使用，否则生成图片过大

 

ImageMagick FAQ摘要：

如何让animate在显示图片的过程保持窗口的标题不变？
animate -title "My Image Sequence" images.

import抓得的图片与X服务器上显示的不一致怎么办？
import -descend image.miff

如何动画显示YUV 图片序列?
animate -geometry 352x240 -scene 0-71 yuv3:frame%d

如果只看一个RGB图片的红色通道(red channel)?
display -gamma 1.0,0.0,0.0 image.miff

如何改变PostScript默认纸张大小?
要改变默认的612x792大小，修改magick/image.h中的PSPageGeometry的值并重编译.

如何自左至右无边无框无文字半三幅画连接在一起？
convert +append image1.ppm image2.ppm image3.ppm side_by_side.miff
# 或
convert -size 350x500 xc:black composite.miff
composite -geometry +0+0 composite.miff image1.gif composite.miff
composite -geometry +100+0 composite.miff image2.gif composite.miff
composite -geometry +0+300 composite.miff image3.gif composite.miff
composite -geometry +0+375 composite.miff image4.gif composite.miff

如何生成gif动画？
convert -delay 20 frame*.gif animation.gif
convert -delay 20 frame1.gif -delay 10 frame2.gif -delay 5 frame3.gif animation.gif
convert frame1.gif -page +50+100 frame2.gif -page +0+100 frame3.gif animation.gif
convert -loop 50 frame*.gif animation.gif
convert +adjoin images.* frames%d.gif

如果完整的显示PostScript图片？
display -page letter image.ps

什么是图片目录？如何使用它们？
#图片目录（visual image directory ，VID）的创建：
   montage *.jpg directory.vid
   convert 'vid:*.jpg' directory.vid
#显示图片目录
   display directory.vid
   display vid:movie.mpg

如果带框架保存一个窗口？
xwininfo -frame
import -frame -window ID window.miff

图片以单一颜色（a solid color）显示，是怎么回事？图片被破坏，或含有值为0的matte层，试着:
display +matte image.miff

出现 "???? library is not available ..."是怎么回事?   
没安装相关的函数库。参见安装说明。

如何查看图片不光滑层(matte channel)的值？
convert image.tiff image.matte
display -size 640x480 gray:image.matte

运行"Convert in.gif out.png" 时显示 "Invalid Parameter - out。  
你运行的convert是windows的分区类型转换程序。

如何在图片增加斜角？
#对图片边缘的处理共分四类。
1）增加有色边
convert -bordercolor red -border 25x25 image.jpg image.gif
2）加亮或变暗图片边缘，以增强3D效果
convert -raise 25 image.jpg image.gif
3）在图片周围增加装饰性框架。
convert -mattecolor gray -frame 25x25 image.jpg image.gif
4）在图片边缘增加升、降斜角
convert -mattecolor gray -frame 25x25+0+25 image.jpg image.gif
convert -mattecolor gray -frame 25x25+25+0 image.jpg image.gif

用窗口管理器运行display失败了，怎么办?   
用下列命令之一来确认display运行在一个交互环境中：
display logo:Untitled
display < /dev/console

将PostScript文件转换为其它格式时，如何改善文字的外观？
convert -density 288 -geometry 25% image.ps image.gif

如何用2－3英寸高的文字注释一个图片?   
先检查有无可缩放字体： xlsfonts -fn '*-0-0-0-0-*'　
有则：
convert -font '-*-helvetica-*-*-*--300-300-*-*-*-*-iso8859-1' \
        -fill green -draw 'text 50,300 Magick' image.gif annotated.gif
# If you have the FreeType support built into ImageMagick,
# just increase your pointsize and/or density:
convert -font Helvetica -pointsize 100 -density 300 ...

如何将gif文件分析为一系列的单个图片？
convert animation.gif frame%02d.gif

用NetScape显示的图片，如何去掉周围的背景色？
convert +page alpha.gif beta.gif

如何用web安全的颜色（Web safe colors）创建GIF或PNG图片？
convert -map netscape: alpha.gif beta.gif

为何Adobe Acrobat 2.1不能读ImageMagick's制作的PDF?   
用 Acrobat 3.0或以上版本，来阅读这种经zip压缩的pdf，或制作PDF时不进行压缩。
convert +compress images.tiff image.pdf

如何在图片上增加一个不光滑层(matte channel)？
convert image.gif -matte temp.miff
composite -compose CopyOpacity mask.xbm temp.miff transparent.gif

在NT环境下如何用convert在图片上写字？
convert -font Arial -fill blue -draw "text 10,10 'your text here'" d:\test.tif png:d:\test.png
convert.exe -pointsize 18 -draw "text 0,0 "This is my text!"" C:\blank.gif c:\text.gif

为何GIF文件比期望的大?   
需要在编译源码时加 --enable-lzw 参数，来打开LZW压缩功能。

为何JPEG文件比期望的大?   
可能嵌入Exif或IPTC等"profiles", 或者含有没压缩的thumbnails. 可以用命令行参数 +profile ""或相关的API来将它们去掉。

如何从图片文件中抽取第一张图片？
convert "Image.gif[0]" first.gif

如何创建一张空白图片？
convert -size 800x600 xc:"#ddddff" ltblue.ppm
convert -size 800x600 null:white white.ppm
convert in.png -threshold 100% black.ppm  #<--与in.png同大小

为何GIF文件比期望的大?   
需要在编译源码时加 --enable-lzw 参数，来打开LZW压缩功能。

为何JPEG文件比期望的大?   
可能嵌入Exif或IPTC等"profiles", 或者含有没压缩的thumbnails. 可以用命令行参数 +profile ""或相关的API来将它们去掉。

 

ImageMagick详细补充：

通过ImageMagick，你将会发现，大部分的操作只要在终端下敲动键盘即可，不需要再用鼠标点来点去了。
下面对ImageMagick的主要功能做一下介绍，其中覆盖的大都是人们常用的一些功能，如果你想更加详细的了解，可以阅读它的man手册。

一、convert：顾名思义就是对图像进行转化，它主要用来对图像进行格式的转化，同时还可以做缩放、剪切、模糊、反转等操作。
格式转化
如把 foo.jpg 转化为 foo.png：convert foo.jpg foo.png
如果要想把目录下所有的jpg文件都转化为gif，我们可借助于shell的强大功能：find ./ -name "*.jpg" -exec convert {} {}.gif \;
转化后的gif名称为 *.jpg.gif ，这样看起来不太自然，没关系，我们可以再来一步：rename .jpg.gif .gif *.jpg.gif
或者，你也可用shell script来完成上述的操作：for i in *.jpgdoconvert $i `basename $i .jpg`.gif done
我们还可用mogrify来完成同样的效果：mogrify -format png *.jpg
上面命令将会把目录下面所有的jpg文件转化为png格式。

convert还可以把多张照片转化成pdf格式：convert *.jpg foo.pdf
大小缩放
比如我们要为一个普通大小的图片做一个缩略图，我们可以这样：convert -resize 100x100 foo.jpg thumbnail.jpg
也可以使用百分比，这样显的更为直观：convert -resize 50%x50% foo.jpg thumbnail.jpg
convert会自动地考虑在缩放图像大小时图像的高宽的比例，也就是说着新的图像的高宽比与原图相同。

我们还可以批量生成缩略图：mogrify -sample 80x60 *.jpg
注意，这个命令会覆盖原来的图片，不过你可以在操作前，先把你的图片备份一下。

添加边框
在一张照片的四周加上边框，可以用 -mattecolor 参数。
比如某位同志牺牲了，需要为其做一张黑边框的遗像，可以这样：convert -mattecolor "#000000" -frame 60x60 yourname.jpg rememberyou.png
其中，"#000000"是边框的颜色，边框的大小为60x60
也可以这样加边框：convert -border 60x60 -bordercolor "#000000" yourname.jpg rememberyou.png

添加文字
convert -fill green -pointsize 40 -draw 'text 10,50 "charry.org"' foo.png bar.png
上面的命令在距离图片的左上角10x50的位置，用绿色的字写下charry.org，如果你要指定别的字体，可以用-font参数。

高斯模糊
convert -blur 80 foo.jpg foo.png
-blur参数还可以这样-blur 80x5。后面的那个5表示的是Sigma的值，这个是图像术语，我也不太清楚，总之，它的值对模糊的效果起关键的作用。

上下翻转
convert -flip foo.png bar.png

左右翻转
convert -flop foo.png bar.png

图像反色
convert -negate foo.png bar.png

图像单色
convert -monochrome foo.png bar.png

添加噪声
convert -noise 3 foo.png bar.png

油画效果
我们可用这个功能，把一张普通的图片，变成一张油画，效果非常的逼真：convert -paint 4 foo.png bar.png

图像旋转
把一张图片，旋转一定的角度：convert -rotate 30 foo.png bar.png
上面的30，表示向右旋转30度，如果要向左旋转，度数就是负数。

炭笔效果
convert -charcoal 2 foo.png bar.png
形成炭笔或者说是铅笔画的效果。

毛玻璃效果
convert -spread 30 foo.png bar.png

漩涡效果
以图片的中心作为参照，把图片扭转，形成漩涡的效果：convert -swirl 67 foo.png bar.png

凸起效果
用-raise来创建凸边：convert -raise 5x5 foo.png bar.png
执行后，你会看到，照片的四周会一个5x5的边，如果你要一个凹下去的边，把-raise改为+raise就可以了。其实凸边和凹边看起来区别并不是很大。

二、import：一个用于屏幕截图的组件，下面列出的是我们常用的功能，其他的功能，你参考它的man好了。

截取屏幕的任一矩形区域
import foo.png
在输入上述的命令后，你的鼠标会变成一个十字，这个时候，你只要在想要截取的地方划一个矩形就可以了

截取程序的窗口
import -pause 3 -frame foo.png
回车后，用鼠标在你想截的窗口上点一下即可。参数- frame的作用是告诉import，截图的时候把目标窗口的外框架带上，参数-pause的作用很重要，你可以试着把它去掉，对比一下，你会发现，目标窗口的标题栏是灰色的，pause就是让import稍微延迟一下，等你的目标窗口获得焦点了，才开始截图，这样的图才比较自然。

截取一个倾斜的窗口
如果想让你的截图比较cool，你可以把截取一个倾斜的窗口，方法如下：import -rotate 30 -pause 3 -frame foo.png

截取整个屏幕
import -pause 3 -window root screen.png
注意，暂停了3秒钟，你需要在3秒钟内切换到需要截取的画面噢。

三、display：应该是我们使用的最为频繁的图像处理软件了

显示图片
display foo.png
如果你要显示多个文件，你可以使用*通配符：display *.png

幻灯片
display -delay 5 *
每隔5个百分之秒显示一张图片

一些快捷键

    space(空格): 显示下一张图片
    backspace(回删键):显示上一张图片
    h: 水平翻转
    v: 垂直翻转
    /:顺时针旋转90度
    \:逆时针旋转90度
    >: 放大
    <: 缩小
    F7:模糊图片
    Alt+s:把图片中间的像素旋转
    Ctrl+s:图象另存
    Ctrl+d:删除图片
    q: 退出

其他
ImageMagick还提供有丰富的编程接口，比如，你可以用php来调用它，用ImageMagick来生成验证码图片，效果非常棒。
ImageMagick还有一个小工具identify，它可以用来显示一个图片文件的详悉信息，比如格式、分辨率、大小、色深等等，你都可用它来帮你的忙。
如果你对命令行不太熟悉，你也可以在图片上单击，你会发现，通过鼠标你也可以完成图像的编辑。








