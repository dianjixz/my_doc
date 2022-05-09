<!DOCTYPE html>
<html lang="">
  <head>
    <meta charset="utf-8">
    <title></title>
  </head>
  <body>
    <header></header>
    <main></main>
    <footer></footer>
  </body>
</html>
# 开源轻量级显示框架LVGL简介

https://www.xyhtml5.com/22598.html

http://lvgl.100ask.net/8.2/index.html



## #01 LVGL是什么

LVGL的作者是来自匈牙利的Gabor Kiss-Vamosikisvegabor，LVGL用C语言编写，以实现最大的兼容性（与C  ++兼容），模拟器可在没有嵌入式硬件的PC上启动嵌入式GUI设计，同时LVGL作为一个图形库，它自带着接近三十多种小工具可以供开发者使用。这些强大的构建块按钮搭配上带有非常丝滑的动画以及可以做到平滑滚动的高级图形，同时兼具着不高的配置要求以及开源属性，显著的优势使得LVGL蔚然成风，成为广大开发者在选择GUI时的第一选择。



![img](https://pic2.zhimg.com/v2-bb12a7245082819271ac3f2e8fc4aaf1_b.webp)







### **主要特性**



- 强大的构建块，如按钮，图表，列表，滑块，图像等。
- 高级图形动画，抗锯齿，不透明度，平滑滚动
- 各种输入设备，如触摸板、鼠标、键盘、编码器等
- 多语言支持与UTF-8编码
- 多显示器支持，即使用更多的TFT，单色显示器同时
- 完全可定制的图形元素与css类样式
- 硬件独立与任何微控制器或显示器使用
- 可扩展，使用少量内存(64kb Flash, 16kb RAM)
- 支持操作系统、外部内存和GPU，但不是必需的
- 单帧缓冲操作，甚至与高级图形效果
- 用C编写的最大兼容性(c++兼容)
- 模拟器在没有嵌入式硬件的PC上开始嵌入式GUI设计
- 绑定到MicroPython
- 教程，例子，快速GUI设计的主题
- 文档可以在线和PDF格式获取
- 麻省理工学院许可下的免费和开源

![img](https://pic2.zhimg.com/80/v2-2efb5779c292d5fa4d49b6e0002b6759_720w.jpg)LVGL主要特性



### **配置要求**

基本上，每个能够驱动显示器的现代控制器都适合运行 LVGL。最低要求是：

- 16、32 或 64 位微控制器或处理器
- 建议使用 16 MHz 时钟速度
- 闪存/ROM：> 64 kB 用于非常重要的组件 (> 建议使用 180 kB)
- RAM:
  静态 RAM 使用量：0~2 kB，取决于使用的功能和对象类型
  堆: > 2kB (> 建议使用 8 kB)
  动态数据（堆）: > 2 KB (> 如果使用多个对象，建议使用 16 kB). 在 lv_conf.h 文件中配置 LV_MEM_SIZE 生效。
  显示缓冲区：> “水平分辨率”像素（推荐> 10 × 10×“水平分辨率”）
  MCU 或外部显示控制器中的一个帧缓冲器
- C99 或更新的编译器
- 具备基本的 C（或 C++）知识

一块能驱动显示屏且Flash大于64KB，RAM大于20KB的单片机都可以支持运行LVGL。这样也就说明只需要是我们经常用于开发的单片机几乎都可以支持(16bit及以上)LVGL，LVGL能够同时被这么多平台支持的主要原因是LVGL对硬件的要求并不算太高。



### **系统框架**

LVGL本身是一个图形库。

我们的应用程序通过调用LVGL库来创建GUI。它包含一个HAL（硬件抽象层）接口，用于注册显示和输入设备驱动程序。驱动程序除特定的驱动程序外，它还有其他的功能，可驱动显示器GPU、读取触摸板或按钮的输入。

MCU有两种典型的硬件设置。一个带有内置LCD/TFT驱动器的外围设备，而另一种是没有内置LCD/TFT驱动器的外围设备。相同的是，这两种情况都需要一个帧缓冲区来存储屏幕的当前图像。

集成了TFT/LCD驱动器的MCU如果MCU集成了TFT/LCD驱动器外围设备，则可以直接通过RGB接口连接显示器。在这种情况下，帧缓冲区可以位于内部RAM（如果MCU有足够的RAM）中，也可以位于外部RAM（如果MCU具有存储器接口）中。

如果MCU没有集成TFT/LCD驱动程序接口，则必须使用外部显示控制器（例如SSD1963、SSD1306、ILI9341 ）。在这种情况下，MCU可以通过并行端口，SPI或通过I2C与显示控制器进行通信。帧缓冲区通常位于显示控制器中，从而为MCU节省了大量RAM。

## LVGL源码布局

-  **./lvgl** 库本身 仓库地址：https://github.com/lvgl/lvgl。
-  **./lv_drivers** 显示和输入设备驱动程序 GitHub 仓库地址：https://github.com/lvgl/lv_drivers
-  **./lv_examples** 示例和演示
- lvgl官方文档网站(https://docs.lvgl.io)
- lvgl官方博客博客站点(https://blog.lvgl.io)
- sim在线模拟器网站(https://sim.lvgl.io)
- lv_sim _… 适用于各种 IDE 和平台的模拟器项目
- lv_port _… 移植到其他开发板
- lv_binding _… 绑定到其他语言
- lv _…移植到其他平台



# 移植过程

## 1, LVGL配置文件

上面的三个库中有一个类似名为 **lv_conf_template.h** 的配置头文件(template就是模板的意思)。通过它可以设置库的基本行为，裁剪不需要模块和功能，在编译时调整内存缓冲区的大小等等。

1. 将 **lvgl/lv_conf_template.h** 复制到 lvgl 同级目录下，并将其重命名为 `lv_drv_conf.h` 。打开文件并将开头的 `#if 0` 更改为 `#if 1` 以使能其内容。
2. 将 **lv_drivers/lv_drv_conf_template.h** 复制到 lv_drivers 同级目录下，并将其重命名为 `lv_conf.h` 。打开文件并将开头的 `#if 0` 更改为 `#if 1` 以使能其内容。
3. (可选)将 **lv_examples/lv_ex_conf_template.h** 复制到 lv_examples 同级目录下，并将其重命名为 `lv_ex_conf.h` 。打开文件并将开头的 `#if 0` 更改为 `#if 1` 以使能其内容。

使能配置文件

lv_conf.h 也可以复制到其他位置，但是应该在编译器选项中添加 `LV_CONF_INCLUDE_SIMPLE` 定义 (例如，对于 gcc 编译器为 `-DLV_CONF_INCLUDE_SIMPLE` ) 并手动设置包含路径。

在配置文件中，注释说明了各个选项的含义。我们在移植时至少要检查以下三个配置选项，其他配置根据具体的需要进行修改：

-  `LV_HOR_RES_MAX` 显示器的水平分辨率。
-  `LV_VER_RES_MAX` 显示器的垂直分辨率。
-  `LV_COLOR_DEPTH` 颜色深度，其可以是：

> - 8 – RG332
> - 16 – RGB565
> - 32 – (RGB888和ARGB8888)

## 2, 初始化lvgl

准备好这三个库：lvgl、lv_drivers、lv_examples 后，我们就要开始使用lvgl带给我们的功能了。使用 lvgl 图形库之前，我们还必须初始化 lvlg 以及相关其他组件。初始化的顺序为：

1. 调用 lv_init() 初始化 lvgl 库;
2. 初始化驱动程序；
3. 在 LVGL 中注册显示和输入设备驱动程序；
4. 在中断中每隔 `x毫秒` 调用 `lv_tick_inc(x)` 用以告知 lvgl 经过的时间；
5. 每隔 `x毫秒` 定期调用 `lv_task_handler()` 用以处理与 lvgl 相关的任务。

### Windows初始化示例(Cdoe::Blocks)

~~~ c
#if WIN32
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
{
/*Initialize LittlevGL*/
lv_init();
/*Initialize the HAL for LittlevGL*/
hal_init();
/*Check the themes too*/
lv_disp_set_default(lv_windows_disp);
/*Run your APP here */
#if WIN32
while(!lv_win_exit_flag) {
#else
while(1) {
#endif // WIN32
/* Periodically call the lv_task handler.
* It could be done in a timer interrupt or an OS task too.*/
lv_task_handler();
usleep(5*1000);       /*Just to let the system breath*/
lv_tick_inc(5*1000)
}
return 0;
}
~~~

### Linux初始化示例

~~~ c
int main(void)
{
/* LittlevGL init */
lv_init();
/* Linux frame buffer device init */
fbdev_init();
/* A small buffer for LittlevGL to draw the screen's content */
static lv_color_t buf[DISP_BUF_SIZE];
/* Initialize a descriptor for the buffer */
static lv_disp_buf_t disp_buf;
lv_disp_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);
/* Initialize and register a display driver */
lv_disp_drv_t disp_drv;
lv_disp_drv_init(&disp_drv);
disp_drv.buffer = &disp_buf;
disp_drv.flush_cb = fbdev_flush;
lv_disp_drv_register(&disp_drv);
//hal_init
lv_disp_set_default lv_windows_disp
/* Linux input device init */
evdev_init();
/* Initialize and register a display input driver */
lv_indev_drv_t indev_drv;
lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
indev_drv.type = LV_INDEV_TYPE_POINTER;
indev_drv.read_cb = evdev_read;   //lv_gesture_dir_t lv_indev_get_gesture_dir(const lv_indev_t * indev)
lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
/*Run your APP here */
/*Handle LitlevGL tasks (tickless mode)*/
while(1) {
lv_task_handler();
usleep(5000);
lv_tick_inc(5*1000);
}
return 0;
}
~~~

## LVGL 显示接口

要设置显示，必须初始化 `lv_disp_buf_t` 和 `lv_disp_drv_t` 变量。

-  **lv_disp_buf_t** 保存显示缓冲区信息的结构体
-  **lv_disp_drv_t** HAL要注册的显示驱动程序、与显示交互并处理与图形相关的结构体、回调函数。

### LVGL显示缓冲区

`lv_disp_buf_t` 初始化示例：

~~~ c
/*A static or global variable to store the buffers*/
static lv_disp_buf_t disp_buf;
/*Static or global buffer(s). The second buffer is optional*/
static lv_color_t buf_1[MY_DISP_HOR_RES * 10];
static lv_color_t buf_2[MY_DISP_HOR_RES * 10];
/*Initialize `disp_buf` with the buffer(s) */
lv_disp_buf_init(&disp_buf, buf_1, buf_2, MY_DISP_HOR_RES*10);
~~~

关于缓冲区大小，有 3 种情况：

1.  **一个缓冲区** LVGL将屏幕的内保存到缓冲区中并将其发送到显示器。缓冲区可以小于屏幕。在这种情况下，较大的区域将被重画成多个部分。如果只有很小的区域发生变化(例如按下按钮)，则只会刷新该部分的区域。
2.  **两个非屏幕大小的缓冲区** 具有两个缓冲区的 LVGL 可以将其中一个作为显示缓冲区，而另一缓冲区的内容发送到后台显示。应该使用 DMA 或其他硬件将数据传输到显示器，以让CPU同时绘图。这样，渲染和刷新并行处理。与 **一个缓冲区** 的情况类似，如果缓冲区小于要刷新的区域，LVGL将按块绘制显示内容
3.  **两个屏幕大小的缓冲区** 与两个非屏幕大小的缓冲区相反，LVGL将始终提供整个屏幕的内容，而不仅仅是块。这样，驱动程序可以简单地将帧缓冲区的地址更改为从 LVGL 接收的缓冲区。因此，当MCU具有 LCD/TFT 接口且帧缓冲区只是 RAM 中的一个位置时，这种方法的效果很好。

## LVGL显示驱动器

一旦缓冲区初始化准备就绪，就需要初始化显示驱动程序。在最简单的情况下，仅需要设置 `lv_disp_drv_t` 的以下两个字段：

-  **buffer** 指向已初始化的 `lv_disp_buf_t` 变量的指针。
-  **flush_cb** 回调函数，用于将缓冲区的内容复制到显示的特定区域。刷新准备就绪后，需要调用lv_disp_flush_ready()。  LVGL可能会以多个块呈现屏幕，因此多次调用flush_cb。使用 lv_disp_flush_is_last() 可以查看哪块是最后渲染的。

其中，有一些可选的数据字段：

-  **hor_res** 显示器的水平分辨率。(默认为 lv_conf.h 中的 `LV_HOR_RES_MAX` )
-  **ver_res** 显示器的垂直分辨率。 (默认为 lv_conf.h 中的 `LV_VER_RES_MAX` )
-  **color_chroma_key** 在 chrome 键控图像上将被绘制为透明的颜色。(默认为 lv_conf.h 中的 `LV_COLOR_TRANSP` )
-  **user_data** 驱动程序的自定义用户数据。可以在 lv_conf.h 中修改其类型。
-  **anti-aliasing** 使用抗锯齿(anti-aliasing)(边缘平滑)。缺省情况下默认为 lv_conf.h 中的 `LV_ANTIALIAS` 。
-  **rotated** 如果 `1` 交换 `hor_res` 和 `ver_res` 。两种情况下 LVGL 的绘制方向相同(从上到下的线条)，因此还需要重新配置驱动程序以更改显示器的填充方向。
-  **screen_transp** 如果为 `1` ，则屏幕可以具有透明或不透明的样式。需要在 lv_conf.h 中启用 `LV_COLOR_SCREEN_TRANSP` 。

要使用GPU，可以使用以下回调：

-  **gpu_fill_cb** 用颜色填充内存中的区域。
-  **gpu_blend_cb** 使用不透明度混合两个内存缓冲区。
-  **gpu_wait_cb** 如果在 GPU 仍在运行 LVGL 的情况下返回了任何 GPU 函数，则在需要确保GPU渲染就绪时将使用此函数。

>  注意，这些功能需要绘制到内存(RAM)中，而不是直接显示在屏幕上。

其他一些可选的回调，使单色、灰度或其他非标准RGB显示一起使用时更轻松、优化：

-  **rounder_cb** 四舍五入要重绘的区域的坐标。例如。 2×2像素可以转换为2×8。如果显示控制器只能刷新特定高度或宽度的区域(对于单色显示器，通常为8 px高)，则可以使用它。
-  **set_px_cb** 编写显示缓冲区的自定义函数。如果显示器具有特殊的颜色格式，则可用于更紧凑地存储像素。  (例如1位单色，2位灰度等)。这样，lv_disp_buf_t中使用的缓冲区可以较小，以仅保留给定区域大小所需的位数。  set_px_cb不能与两个屏幕大小的缓冲区一起显示缓冲区配置。
-  **monitor_cb** 回调函数告诉在多少时间内刷新了多少像素。
-  **clean_dcache_cb** 清除与显示相关的所有缓存的回调

要设置 lv_disp_drv_t 变量的字段，需要使用 lv_disp_drv_init(＆disp_drv) 进行初始化。最后，要为 LVGL 注册显示设备，需要调用lv_disp_drv_register(＆disp_drv)。

代码示例：

~~~ c
lv_disp_drv_t disp_drv;                 /*A variable to hold the drivers. Can be local variable*/
lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
disp_drv.buffer = &disp_buf;            /*Set an initialized buffer*/
disp_drv.flush_cb = my_flush_cb;        /*Set a flush callback to draw to the display*/
lv_disp_t * disp;
disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
~~~

回调的一些简单示例：

~~~ c
void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
/*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
int32_t x, y;
for(y = area->y1; y <= area->y2; y++) {
for(x = area->x1; x <= area->x2; x++) {
put_px(x, y, *color_p)
color_p++;
}
}
/* IMPORTANT!!!
* Inform the graphics library that you are ready with the flushing*/
lv_disp_flush_ready(disp);
}
void my_gpu_fill_cb(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, const lv_area_t * dest_area, const lv_area_t * fill_area, lv_color_t color);
{
/*It's an example code which should be done by your GPU*/
uint32_t x, y;
dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
for(y = fill_area->y1; y < fill_area->y2; y++) {
for(x = fill_area->x1; x < fill_area->x2; x++) {
dest_buf[x] = color;
}
dest_buf+=dest_width;    /*Go to the next line*/
}
}
void my_gpu_blend_cb(lv_disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa)
{
/*It's an example code which should be done by your GPU*/
uint32_t i;
for(i = 0; i < length; i++) {
dest[i] = lv_color_mix(dest[i], src[i], opa);
}
}
void my_rounder_cb(lv_disp_drv_t * disp_drv, lv_area_t * area)
{
/* Update the areas as needed. Can be only larger.
* For example to always have lines 8 px height:*/
area->y1 = area->y1 & 0x07;
area->y2 = (area->y2 & 0x07) + 8;
}
void my_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa)
{
/* Write to the buffer as required for the display.
* Write only 1-bit for monochrome displays mapped vertically:*/
buf += buf_w * (y >> 3) + x;
if(lv_color_brightness(color) > 128) (*buf) |= (1 << (y % 8));
else (*buf) &= ~(1 << (y % 8));
}
void my_monitor_cb(lv_disp_drv_t * disp_drv, uint32_t time, uint32_t px)
{
printf("%d px refreshed in %d ms\n", time, ms);
}
void my_clean_dcache_cb(lv_disp_drv_t * disp_drv, uint32)
{
/* Example for Cortex-M (CMSIS) */
SCB_CleanInvalidateDCache();
}
~~~

## LVGL输入设备的类型

要设置输入设备，必须初始化 `lv_indev_drv_t` 变量：

~~~ c
lv_indev_drv_t indev_drv;
lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
indev_drv.type =...                 /*See below.*/
indev_drv.read_cb =...              /*See below.*/
/*Register the driver in LVGL and save the created input device object*/
lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
~~~

**类型** (indev_drv.type)可以是：

-  **LV_INDEV_TYPE_POINTER** 触摸板或鼠标
-  **LV_INDEV_TYPE_KEYPAD** 键盘或小键盘
-  **LV_INDEV_TYPE_ENCODER** 带有左，右，推动选项的编码器
-  **LV_INDEV_TYPE_BUTTON** 外部按钮按下屏幕

**read_cb** (indev_drv.read_cb)是一个函数指针，将定期调用该函数指针以报告输入设备的当前状态。它还可以缓冲数据并在没有更多数据要读取时返回 `false` ，或者在缓冲区不为空时返回 `true` 。

进一步了解有关 LVGL输入设备 的更多信息。

LVGL触摸板，鼠标或任何指针

可以单击屏幕点的输入设备属于此类别。

```c
indev_drv.type = LV_INDEV_TYPE_POINTER;
indev_drv.read_cb = my_input_read;
...
bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
data->point.x = touchpad_x;
data->point.y = touchpad_y;
data->state = LV_INDEV_STATE_PR or LV_INDEV_STATE_REL;
return false; /*No buffering now so no more data read*/
}
```

即使状态为 LV_INDEV_STATE_REL ，触摸板驱动程序也必须返回最后的 X/Y 坐标。

要设置鼠标光标，请使用 `lv_indev_set_cursor(my_indev,&img_cursor)` 。( `my_indev` 是 `lv_indev_drv_register` 的返回值)键盘或键盘

## LVGL触摸板或键盘

带有所有字母的完整键盘或带有一些导航按钮的简单键盘均属于此处。

要使用键盘/触摸板：

- 注册具有 LV_INDEV_TYPE_KEYPAD 类型的 read_cb 函数。
- 在 lv_conf.h 中启用 LV_USE_GROUP
- 必须创建一个对象组：lv_group_t * g = lv_group_create()，并且必须使用 lv_group_add_obj(g，obj) 向其中添加对象
- 必须将创建的组分配给输入设备：lv_indev_set_group(my_indev，g)( my_indev 是 lv_indev_drv_register 的返回值)
- 使用 LV_KEY _… 在组中的对象之间导航。有关可用的密钥，请参见 lv_core/lv_group.h。

~~~ c
indev_drv.type = LV_INDEV_TYPE_KEYPAD;
indev_drv.read_cb = keyboard_read;
...
bool keyboard_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
data->key = last_key();            /*Get the last pressed or released key*/
if(key_pressed()) data->state = LV_INDEV_STATE_PR;
else data->state = LV_INDEV_STATE_REL;
return false; /*No buffering now so no more data read*/
}
~~~

## LVGL编码器

可以通过下面四种方式使用编码器：

1. 按下按钮
2. 长按其按钮
3. 转左
4. 右转

简而言之，编码器输入设备的工作方式如下：

- 通过旋转编码器，可以专注于下一个/上一个对象。
- 在简单对象(如按钮)上按下编码器时，将单击它。
- 如果将编码器按在复杂的对象(如列表，消息框等)上，则该对象将进入编辑模式，从而转动编码器即可在对象内部导航。
- 长按按钮，退出编辑模式。

要使用编码器(类似于键盘)，应将对象添加到组中。

~~~ c
indev_drv.type = LV_INDEV_TYPE_ENCODER;
indev_drv.read_cb = encoder_read;
...
bool encoder_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
data->enc_diff = enc_get_new_moves();
if(enc_pressed()) data->state = LV_INDEV_STATE_PR;
else data->state = LV_INDEV_STATE_REL;
return false; /*No buffering now so no more data read*/
}
~~~

### 使用带有编码器逻辑的按钮

除了标准的编码器行为外，您还可以利用其逻辑来使用按钮导航(聚焦)和编辑小部件。如果只有几个按钮可用，或者除编码器滚轮外还想使用其他按钮，这将特别方便。

需要有3个可用的按钮：

-  **LV_KEY_ENTER** 将模拟按下或推动编码器按钮
-  **LV_KEY_LEFT** 将向左模拟转向编码器
-  **LV_KEY_RIGHT** 将正确模拟转向编码器
- 其他键将传递给焦点小部件

如果按住这些键，它将模拟indev_drv.long_press_rep_time中指定的时间段内的编码器单击。

```c
indev_drv.type = LV_INDEV_TYPE_ENCODER;
indev_drv.read_cb = encoder_with_keys_read;
...
bool encoder_with_keys_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
data->key = last_key();            /*Get the last pressed or released key*/
/* use LV_KEY_ENTER for encoder press */
if(key_pressed()) data->state = LV_INDEV_STATE_PR;
else {
data->state = LV_INDEV_STATE_REL;
/* Optionally you can also use enc_diff, if you have encoder*/
data->enc_diff = enc_get_new_moves();
}
return false; /*No buffering now so no more data read*/
}
```

## LVGL按键

按钮是指屏幕旁边的外部“硬件”按钮，它们被分配给屏幕的特定坐标。如果按下按钮，它将模拟在指定坐标上的按下。 (类似于触摸板)

使用 lv_indev_set_button_points(my_indev, points_array)  将按钮分配给坐标。points_array应该看起来像const lv_point_t points_array [] =  {{12,30}，{60,90}，…}

points_array不能超出范围。将其声明为全局变量或函数内部的静态变量。

```
indev_drv.type = LV_INDEV_TYPE_BUTTON;
indev_drv.read_cb = button_read;
...
bool button_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
static uint32_t last_btn = 0;   /*Store the last pressed button*/
int btn_pr = my_btn_read();     /*Get the ID (0,1,2...) of the pressed button*/
if(btn_pr >= 0) {               /*Is there a button press? (E.g. -1 indicated no button was pressed)*/
last_btn = btn_pr;           /*Save the ID of the pressed button*/
data->state = LV_INDEV_STATE_PR;  /*Set the pressed state*/
} else {
data->state = LV_INDEV_STATE_REL; /*Set the released state*/
}
data->btn = last_btn;            /*Save the last button*/
return false;                    /*No buffering now so no more data read*/
}
```

## LVGL其它功能

除了 read_cb 之外，还可以在 lv_indev_drv_t 中指定 feedback_cb 回调。输入设备发送任何类型的事件时，都会调用feedback_cb。 (独立于其类型)。它允许为用户提供反馈，例如在LV_EVENT_CLICK上播放声音。

可以在lv_conf.h中设置以下参数的默认值，但可以在lv_indev_drv_t中覆盖默认值：

-  **拖拽限制(drag_limit)** 实际拖动对象之前要滑动的像素数 drag_throw 拖曳速度降低[％]。更高的价值意味着更快的减速
-  **(drag_throw)** 拖曳速度降低[％]。更高的价值意味着更快的减速
-  **(long_press_time)** 按下时间发送 LV_EVENT_LONG_PRESSED (以毫秒为单位)
-  **(long_press_rep_time)** 发送 LV_EVENT_LONG_PRESSED_REPEAT 的时间间隔(以毫秒为单位)
-  **(read_task)** 指向读取输入设备的lv_task的指针。可以通过 `lv_task_...()` 函数更改其参数

每个输入设备都与一个显示器关联。默认情况下，新的输入设备将添加到最后创建的或显式选择的显示设备(使用lv_disp_set_default())。相关的显示已存储，并且可以在驱动程序的显示字段中更改。

## [LVGL 心跳](https://www.xyhtml5.com/22593.html)

LVGL心跳，LVGL 需要系统滴答声才能知道动画和其他任务的经过时间。

为此我们需要定期调用 lv_tick_inc(tick_period) 函数，并以毫秒为单位告知调用周期。例如， lv_tick_inc(1) 用于每毫秒调用一次。

为了精确地知道经过的毫秒数，lv_tick_inc 应该在比 lv_task_handler() 更高优先级的例程中被调用(例如在中断中)，即使 lv_task_handler 的执行花费较长时间。

使用 FreeRTOS 时，可以在 vApplicationTickHook 中调用 lv_tick_inc 。

在基于 Linux 的设备上(例如在 Raspberry Pi 上)， lv_tick_inc 可以在如下所示的线程中调用，比如：

```
void * tick_thread (void *args)
{
while(1) {
usleep(5*1000);   /*Sleep for 5 millisecond*/
lv_tick_inc(5);      /*Tell LVGL that 5 milliseconds were elapsed*/
}
}
```

# [LVGL 输入设备接口](https://www.xyhtml5.com/22594.html)

## LVGL输入设备的类型

要设置输入设备，必须初始化 `lv_indev_drv_t` 变量：

```
lv_indev_drv_t indev_drv;
lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
indev_drv.type =...                 /*See below.*/
indev_drv.read_cb =...              /*See below.*/
/*Register the driver in LVGL and save the created input device object*/
lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
```

**类型** (indev_drv.type)可以是：

-  **LV_INDEV_TYPE_POINTER** 触摸板或鼠标
-  **LV_INDEV_TYPE_KEYPAD** 键盘或小键盘
-  **LV_INDEV_TYPE_ENCODER** 带有左，右，推动选项的编码器
-  **LV_INDEV_TYPE_BUTTON** 外部按钮按下屏幕

**read_cb** (indev_drv.read_cb)是一个函数指针，将定期调用该函数指针以报告输入设备的当前状态。它还可以缓冲数据并在没有更多数据要读取时返回 `false` ，或者在缓冲区不为空时返回 `true` 。

进一步了解有关 LVGL输入设备 的更多信息。

## LVGL触摸板，鼠标或任何指针

可以单击屏幕点的输入设备属于此类别。

```
indev_drv.type = LV_INDEV_TYPE_POINTER;
indev_drv.read_cb = my_input_read;
...
bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
data->point.x = touchpad_x;
data->point.y = touchpad_y;
data->state = LV_INDEV_STATE_PR or LV_INDEV_STATE_REL;
return false; /*No buffering now so no more data read*/
}
```

即使状态为 LV_INDEV_STATE_REL ，触摸板驱动程序也必须返回最后的 X/Y 坐标。

要设置鼠标光标，请使用 `lv_indev_set_cursor(my_indev,&img_cursor)` 。( `my_indev` 是 `lv_indev_drv_register` 的返回值)键盘或键盘

## LVGL触摸板或键盘

带有所有字母的完整键盘或带有一些导航按钮的简单键盘均属于此处。

要使用键盘/触摸板：

- 注册具有 LV_INDEV_TYPE_KEYPAD 类型的 read_cb 函数。
- 在 lv_conf.h 中启用 LV_USE_GROUP
- 必须创建一个对象组：lv_group_t * g = lv_group_create()，并且必须使用 lv_group_add_obj(g，obj) 向其中添加对象
- 必须将创建的组分配给输入设备：lv_indev_set_group(my_indev，g)( my_indev 是 lv_indev_drv_register 的返回值)
- 使用 LV_KEY _… 在组中的对象之间导航。有关可用的密钥，请参见 lv_core/lv_group.h。

```
indev_drv.type = LV_INDEV_TYPE_KEYPAD;
indev_drv.read_cb = keyboard_read;
...
bool keyboard_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
data->key = last_key();            /*Get the last pressed or released key*/
if(key_pressed()) data->state = LV_INDEV_STATE_PR;
else data->state = LV_INDEV_STATE_REL;
return false; /*No buffering now so no more data read*/
}
```

## LVGL编码器

可以通过下面四种方式使用编码器：

1. 按下按钮
2. 长按其按钮
3. 转左
4. 右转

简而言之，编码器输入设备的工作方式如下：

- 通过旋转编码器，可以专注于下一个/上一个对象。
- 在简单对象(如按钮)上按下编码器时，将单击它。
- 如果将编码器按在复杂的对象(如列表，消息框等)上，则该对象将进入编辑模式，从而转动编码器即可在对象内部导航。
- 长按按钮，退出编辑模式。

要使用编码器(类似于键盘)，应将对象添加到组中。

```
indev_drv.type = LV_INDEV_TYPE_ENCODER;
indev_drv.read_cb = encoder_read;
...
bool encoder_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
data->enc_diff = enc_get_new_moves();
if(enc_pressed()) data->state = LV_INDEV_STATE_PR;
else data->state = LV_INDEV_STATE_REL;
return false; /*No buffering now so no more data read*/
}
```

### 使用带有编码器逻辑的按钮

除了标准的编码器行为外，您还可以利用其逻辑来使用按钮导航(聚焦)和编辑小部件。如果只有几个按钮可用，或者除编码器滚轮外还想使用其他按钮，这将特别方便。

需要有3个可用的按钮：

-  **LV_KEY_ENTER** 将模拟按下或推动编码器按钮
-  **LV_KEY_LEFT** 将向左模拟转向编码器
-  **LV_KEY_RIGHT** 将正确模拟转向编码器
- 其他键将传递给焦点小部件

如果按住这些键，它将模拟indev_drv.long_press_rep_time中指定的时间段内的编码器单击。

```
indev_drv.type = LV_INDEV_TYPE_ENCODER;
indev_drv.read_cb = encoder_with_keys_read;
...
bool encoder_with_keys_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
data->key = last_key();            /*Get the last pressed or released key*/
/* use LV_KEY_ENTER for encoder press */
if(key_pressed()) data->state = LV_INDEV_STATE_PR;
else {
data->state = LV_INDEV_STATE_REL;
/* Optionally you can also use enc_diff, if you have encoder*/
data->enc_diff = enc_get_new_moves();
}
return false; /*No buffering now so no more data read*/
}
```

## LVGL按键

按钮是指屏幕旁边的外部“硬件”按钮，它们被分配给屏幕的特定坐标。如果按下按钮，它将模拟在指定坐标上的按下。 (类似于触摸板)

使用 lv_indev_set_button_points(my_indev, points_array)  将按钮分配给坐标。points_array应该看起来像const lv_point_t points_array [] =  {{12,30}，{60,90}，…}

points_array不能超出范围。将其声明为全局变量或函数内部的静态变量。

```
indev_drv.type = LV_INDEV_TYPE_BUTTON;
indev_drv.read_cb = button_read;
...
bool button_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
static uint32_t last_btn = 0;   /*Store the last pressed button*/
int btn_pr = my_btn_read();     /*Get the ID (0,1,2...) of the pressed button*/
if(btn_pr >= 0) {               /*Is there a button press? (E.g. -1 indicated no button was pressed)*/
last_btn = btn_pr;           /*Save the ID of the pressed button*/
data->state = LV_INDEV_STATE_PR;  /*Set the pressed state*/
} else {
data->state = LV_INDEV_STATE_REL; /*Set the released state*/
}
data->btn = last_btn;            /*Save the last button*/
return false;                    /*No buffering now so no more data read*/
}
```

## LVGL其它功能

除了 read_cb 之外，还可以在 lv_indev_drv_t 中指定 feedback_cb 回调。输入设备发送任何类型的事件时，都会调用feedback_cb。 (独立于其类型)。它允许为用户提供反馈，例如在LV_EVENT_CLICK上播放声音。

可以在lv_conf.h中设置以下参数的默认值，但可以在lv_indev_drv_t中覆盖默认值：

-  **拖拽限制(drag_limit)** 实际拖动对象之前要滑动的像素数 drag_throw 拖曳速度降低[％]。更高的价值意味着更快的减速
-  **(drag_throw)** 拖曳速度降低[％]。更高的价值意味着更快的减速
-  **(long_press_time)** 按下时间发送 LV_EVENT_LONG_PRESSED (以毫秒为单位)
-  **(long_press_rep_time)** 发送 LV_EVENT_LONG_PRESSED_REPEAT 的时间间隔(以毫秒为单位)
-  **(read_task)** 指向读取输入设备的lv_task的指针。可以通过 `lv_task_...()` 函数更改其参数

每个输入设备都与一个显示器关联。默认情况下，新的输入设备将添加到最后创建的或显式选择的显示设备(使用lv_disp_set_default())。相关的显示已存储，并且可以在驱动程序的显示字段中更改。

# [LVGL 任务处理器](https://www.xyhtml5.com/22592.html)

**任务处理器(Task Handler)**要处理 LVGL 的任务，我们需要定期通过以下方式之一调用 lv_task_handler() ：

- mian 函数中设置 while(1) 调用
- 定期定时中断(低优先级然后是 lv_tick_inc()) 中调用
- 定期执行的 OS 任务中调用

计时并不严格，但应保持大约5毫秒以保持系统响应。

范例：

```
while(1) {
lv_task_handler();
my_delay_ms(5);
}
```

要了解有关任务的更多信息，请参阅本教程的LVGL 任务 部分。

# [LVGL 睡眠管理](https://www.xyhtml5.com/22591.html)

**LVGL睡眠管理**，没有用户输入时，MCU 可以进入睡眠状态。在这种情况下，mian 函数中的 while(1) 应该看起来像这样：

```
while(1) {
/*Normal operation (no sleep) in < 1 sec inactivity*/
if(lv_disp_get_inactive_time(NULL) < 1000) {
lv_task_handler();
}
/*Sleep after 1 sec inactivity*/
else {
timer_stop();   /*Stop the timer where lv_tick_inc() is called*/
sleep();                  /*Sleep the MCU*/
}
my_delay_ms(5);
}
```

如果发生唤醒(按，触摸或单击等)，还应该在输入设备读取功能中添加以下几行：

```
lv_tick_inc(LV_DISP_DEF_REFR_PERIOD);  /*Force task execution on wake-up*/
timer_start();                         /*Restart the timer where lv_tick_inc() is called*/
lv_task_handler();                     /*Call `lv_task_handler()` manually to process the wake-up event*/
```

除了 lv_disp_get_inactive_time() 外，还可以调用 lv_anim_count_running() 来查看每个动画是否完成。

# [LVGL 操作系统和中断](https://www.xyhtml5.com/22590.html)

LVGL默认情况下 **不是线程安全的** 。

但是，在以下情况中，调用 LVGL 相关函数是有效的：

- 在事件 (Events) 中。在 “事件” 中了解更多信息。
- 在 (lv_tasks) 中。在 “任务” 中了解更多信息。

## 任务和线程

如果需要使用实际的任务或线程，则需要一个互斥锁，该互斥锁应在调用 lv_task_handler 之前被调用，并在其之后释放。同样，必须在与每个LVGL(lv  _…)相关的函数调用和代码周围的其他任务和线程中使用相同的互斥锁。这样，就可以在真正的多任务环境中使用LVGL。只需使用互斥锁(mutex)即可避免同时调用 LVGL 函数。

## 中断

避免从中断中调用 LVGL 函数( lv_tick_inc() 和 lv_disp_flush_ready() 除外)。但是，如果需要执行此操作，则必须在 lv_task_handler 运行时禁用 LVGL  函数的中断。设置标志或某个值并在 lv_task 中定期检查是一种不错的方法。

## LVGL 的控件

~~~ c
// 控件添加示例 标签添加

lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);
lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK);     /*Break the long lines*/
lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);       /*Center aligned lines*/
lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label "
"and  wrap long text automatically.");
lv_obj_set_width(label1, 150);
lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, -30);
lv_obj_t * label2 = lv_label_create(lv_scr_act(), NULL);
lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC);     /*Circular scroll*/
lv_obj_set_width(label2, 150);
lv_label_set_text(label2, "It is a circularly scrolling text. ");
lv_obj_align(label2, NULL, LV_ALIGN_CENTER, 0, 30);
~~~





# 使用踩坑

1,gui的风格,比如字体,字体大小等,都是由  lv_style_t 控制的. gui中,所有都是控件.



资源:


(LVGL —— Ubuntu20环境建立vscode模拟器)[https://blog.csdn.net/weixin_45652444/article/details/119756079]

(基于LVGL的手表UI架构设计)[https://blog.csdn.net/StarEmbedded/article/details/117631636]