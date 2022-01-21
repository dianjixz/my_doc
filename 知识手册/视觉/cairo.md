 

### 关于Cairo

Cairo是非常流行的开源2D图形渲染引擎库，它支持包括X-Windos，Win32，图像，pdf在内的各种输出设备。目前，Cairo已被广泛的使用在多个平台上来渲染图形界面，包括Firefox/Webkit-EFL/GTK+/Poppler等等。



### 编译安装Cairo


我们可以直接从Ubuntu的上下载cairo的稳定版：

**[plain]** [view plain](http://blog.csdn.net/flexwang_/article/details/38000401#) [copy](http://blog.csdn.net/flexwang_/article/details/38000401#)

1. sudo apt-get install libcairo2-dev 

这里，为了使用Cairo的最新版，我们选择从git上下载源代码，并编译安装。

**[plain]** [view plain](http://blog.csdn.net/flexwang_/article/details/38000401#) [copy](http://blog.csdn.net/flexwang_/article/details/38000401#)

1. git clone git://anongit.freedesktop.org/git/cairo 
2. cd cairo 
3. mkdir cairobuild 
4. ./autogen.sh #这里可以加上配置选项 
5. make  
6. sudo make install 

### Cairo的基本绘图模型与概念

在使用Cairo编程前，我们应该对于Cairo的基本绘图模型做一了解。这些模包括：表面（surfac），源（source)，遮盖(mask)，路径(path),上下文(context)和函数(verb)。

#### 表面(surface)

Surface是Cair绘图的目标区域，在Cairo中使用cairo_surface_t表示。前面讲到Cair支持多种输出设备，因此我们绘图的目标区域可能是一张png图象也可能是一个pdf文件。不同目标的绘制的底层实现各不相同，而surfac对这些绘图的目标进行了一个抽象。因此，我们在创建了相应的surface后，只需要调用统一的函数对surface进行绘制，而不需要关心其后端(backend)的具体实现。

#### 源(source)

Source指的是我们绘图是的具体的材料与格式，它包括画笔的粗细、颜色等等。在Cairo中，source不光可以是简的颜色，也可以是一种图案（patter）比如渐变色，甚至可以是一个表面（surface)。

#### 遮盖（mask）

Mask相当于我们在绘图过程，用一张挖空了某些部分的纸遮挡在画布上。这样，在绘图过程中，只有挖空的部分会被我们所使用的源影响到，其余部分不受影响。

#### 路径（path）

Path是指Cairo的绘制表面上一些虚拟的路径，它可能是一条线段，一个闭合的四边形，也可能是更加复杂的曲线。Path可以由Cairo的函数（在Cairo中被称为verb）所创建。但是，由于Path只是虚拟的路径，所以对Path的创建并不代表对表面绘制。接下来，我们还需要使用绘制函数（Cairo里称为drawing verb)进行绘制。比如，我们可以通过cairo_rectangle函数创建一个闭合的长方形的路径，然后通过cairo_fill函数填充这个长方形。

#### 上下文(context)

Context是Cairo的核心结构，在Cairo中使用cairo_t来表示。它记录了当前状态下，与绘制有关的各种信息，包括之前介绍过的表面、源、遮盖、字体等等。 在任何绘制之前，我们都必须先创建一个cair_t结构，同时将它绑定到一个绘制表面上(surface)。下面的代码段创建了一个cairo_t，并将其绑定到一个640x480的png图象上。

**[cpp]** [view plain](http://blog.csdn.net/flexwang_/article/details/38000401#) [copy](http://blog.csdn.net/flexwang_/article/details/38000401#)

1. cairo_surface_t *surface; 
2. cairo_t *cr; 
3.  
4.  
5. surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 640, 480); 
6. cr = cairo_create (surface); 

#### 函数(verb)

Cairo中与绘制相关的函数被称为verb。目前Cairo支持五种drawing verb，分别是stroke(画线)， fill(填充)，text（文字），paint（滤镜），mask（遮盖）。其中，paint相当于是整个源进行了一次操作，比如cairo_paint_with_alpha函数可以设置一个alpha值，来对整个图象进行灰度的减少。

#### 变换(transformation)

Cairo还提供类似OpenGL的坐标变换操作。变换操作包括：平移(cairo_translate)，伸缩(cairo_scale)，旋转(cairo_rotate)。我们也可以通过cairo_transform函数来指定一个复杂的变换。

### 例子

#### 绘制一个矩形到rectangle.png图片上。

**[cpp]** [view plain](http://blog.csdn.net/flexwang_/article/details/38000401#) [copy](http://blog.csdn.net/flexwang_/article/details/38000401#)

1. \#include <cairo.h> 
2.  
3. **int** 
4. main (**int** argc, **char** *argv[]) 
5. { 
6.   cairo_surface_t *surface; 
7.   cairo_t *cr; 
8.  
9.   **int** width = 640; 
10.   **int** height = 480; 
11.   surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height); 
12.   cr = cairo_create (surface); 
13.  
14.   /* Drawing code goes here */ 
15.   cairo_set_line_width (cr, 10); 
16.   cairo_set_source_rgb (cr, 0, 0, 0); 
17.   cairo_rectangle (cr, width/4, height/4, width/2, height/2); 
18.   cairo_stroke (cr); 
19.  
20.   /* Write output and clean up */ 
21.   cairo_surface_write_to_png (surface, "rectangle.png"); 
22.   cairo_destroy (cr); 
23.   cairo_surface_destroy (surface); 
24.  
25.   **return** 0; 
26. }  

#### 绘制helloworld到helloworld.pdf上。

**[cpp]** [view plain](http://blog.csdn.net/flexwang_/article/details/38000401#) [copy](http://blog.csdn.net/flexwang_/article/details/38000401#)

1. \#include <cairo.h> 
2. \#include <cairo-pdf.h> 
3. **int** 
4. main (**int** argc, **char** *argv[]) 
5. { 
6.   cairo_surface_t *surface; 
7.   cairo_t *cr; 
8.   cairo_text_extents_t te; 
9.  
10.  
11.   /* Prepare drawing area */ 
12.   **int** width = 200; 
13.   **int** height = 120; 
14. ​    
15.   surface = cairo_pdf_surface_create ("helloworld.pdf", width, height); 
16.   cr = cairo_create (surface); 
17.  
18.  
19.   /* Drawing code goes here */ 
20.   cairo_set_source_rgb (cr, 0.0, 0.0, 0.0); 
21.   cairo_select_font_face (cr, "Georgia", 
22. ​    CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD); 
23.   cairo_set_font_size (cr, 20); 
24.   cairo_text_extents (cr, "helloworld", &te); 
25.   cairo_move_to (cr, width/2 - te.width / 2 - te.x_bearing, 
26. ​     height/2 - te.height / 2 - te.y_bearing); 
27.   cairo_show_text (cr, "helloworld"); 
28.  
29.  
30.   cairo_destroy (cr); 
31.   cairo_surface_destroy (surface); 
32.  
33.  
34.   **return** 0; 
35. } 



#### gcc编译

使用gcc编译调用了cairo的源文件时，应指定cairo的头文件目录和库文件所在目录。假设源文件文件名为cairotest.c，cairo.h等头文件所在目录为/usr/include/cairo，cairo库文件所在目录为/usr/local/lib，则可以使用以下命令编译：

**[plain]** [view plain](http://blog.csdn.net/flexwang_/article/details/38000401#) [copy](http://blog.csdn.net/flexwang_/article/details/38000401#)

1. gcc cairotest.c -o cairotest -I/usr/include/cairo -L/usr/local/lib/ -lcairo 

我们也可以使用pkg-config来配置cairo的目录，然后使用如下命令来编译：

**[plain]** [view plain](http://blog.csdn.net/flexwang_/article/details/38000401#) [copy](http://blog.csdn.net/flexwang_/article/details/38000401#)

1. gcc -o cairotest $(pkg-config --cflags --libs cairo) cairotest.c  
