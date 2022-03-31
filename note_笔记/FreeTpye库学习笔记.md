 

## 一、前言

近期工作需要研究 FreeType 库，因此本篇文章记录一下该库的基本用法。

FreeType 是一个免费、开源、可移植且高质量的字体引擎，它有以下优点：

1. 支持多种字体格式文件，并提供了统一的访问接口；
2. 支持单色位图、反走样位图渲染，这使字体显示质量达到 Mac 的水平；
3. 采用面向对象思想设计，用户可以灵活的根据需要裁剪。

但 FreeType 也有缺点，它太大了，即使是裁剪后的阉割版，其代码量也有将近 12 万行，在Flash非常小的开发板可能放不下，此时可以考虑用代码量较小的 stb_truetype 库，具体用法可以参考我之前发的博客：[stb_truetype解析ttf字体并将其保存到图片中](https://link.zhihu.com/?target=https%3A//blog.csdn.net/weixin_40026797/article/details/112755090)。

>  注意：本文采用的 FreeType 库为 AWTK 中的裁剪版，只有一个巨大的头文件，下载链接：[https://github.com/zlgopen/awtk/tree/master/3rd/freetype_single_header](https://link.zhihu.com/?target=https%3A//github.com/zlgopen/awtk/tree/master/3rd/freetype_single_header)。
>  AWTK是 ZLG 开发的开源 GUI 引擎，官网地址：[https://www.zlg.cn/index/pub/awtk.html](https://link.zhihu.com/?target=https%3A//www.zlg.cn/index/pub/awtk.html)。

## 二、FreeType解析矢量（.ttf）字体

使用 FreeType 库将矢量(.ttf)字体解析为位图的步骤详见下文。

### 2.1 包含头文件

此处采用裁剪版的 FreeType 库，需要包含的头文件如下：

```c
#include "freetype.h"
```

如果采用完整版的 FreeType 库，需要包含的头文件如下：

```c
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftpfr.h>
#include <freetype/ftadvanc.h>
```

### 2.2 初始化 FreeType 库

使用 FT_Init_FreeType() 函数初始化一个 FT_Library 句柄，例如:

```c
FT_LIBRARY library;                         //FreeType库的句柄
FT_Error error = FT_Init_FreeType( &library );   
if ( error )
{  /* 初始化失败 */  }
```

### 2.3 加载 FT_Face

FT_Face 是矢量字体的外观对象，用于保存字体外观数、当前外观索引、当前外观所包含字形文件数等相关数据，其中外观可简单理解为常规、斜体、加粗等字体风格，加载 FT_Face 有两种方式，一是通过字体文件名加载，二是从内存中加载，具体如下：

1、通过字体文件名加载FT_Face：

```c
FT_Face face;                        //FT_Face对象的句柄
FT_New_Face( library, "/usr/share/fonts/truetype/arial.ttf", 0, &face);
```

2、从内存中加载 FT_Face：

```c
FT_Face face;                        //FT_Face对象的句柄
FT_New_Memory_Face(font_info.library, font_buf, size, 0, &face);
```

### 2.4 设置字体编码方式

此处设置字体编码方式为Unicode，如果不设置，FreeType默认为 utf-16编码类型：

```c
FT_Select_Charmap(face, FT_ENCODING_UNICODE);
```

### 2.5 设置字体大小

**方法一**：通过物理距离设置字体大小

```c
FT_Set_Char_Size( FT_Face     face,
                  FT_F26Dot6  char_width,        //字符宽度,单位为1/64点
                  FT_F26Dot6  char_height,       //字符高度,单位为1/64点
                  FT_UInt     horz_resolution,   //水平分辨率
                  FT_UInt     vert_resolution ); //垂直分辨率
```

字符宽度和高度以1/64点为单位表示。

点是物理上的距离,一个点代表1/72英寸(2.54cm) 

分辨率以dpi(dots per inch)为单位表示,表示一个英寸有多少个像素 例如:

```c
FT_Set_Char_Size( face, 36 * 64, 0, 96 , 0);    //0表示与另一个尺寸值相等。
```

字符的物理大小：36 * 64 * (1\/64) * (1\/72) 英寸 

字符的像素宽高： 36 * 64 * (1\/64) * (1\/72) * 96

**方法二**：通过像素宽高设置字体大小

```c
FT_Set_Pixel_Sizes( FT_Face  face,
                    FT_UInt  pixel_width,     //像素宽度
                    FT_UInt  pixel_height );  //像素高低
```

例如：

```c
FT_Set_Pixel_Sizes(face, 0 ,16);      //把字符像素设置为16*16像素, 0表示与另一个尺寸值相等。
```

### 2.6 加载字形图像

### 2.6.1 获取字符编码的字形索引

通过 FT_Get_Char_Index() 函数将字符编码转换为一个字形(glyph)索引，例如：

```c
wchar_t char_code = L'A';
FT_UInt glyph_index = FT_Get_Char_Index(face, char_code);     /* 若 glyph_index 为0，表示没找到字形索引 */
```

### 2.6.2 通过索引从face中加载字形

获得字形索引后，接下来可根据索引将字形图像存储到字形槽( FT_GlyphSlot )中。 

**字形槽**：每次只能存储一个字形图像，每个face对象都有一个字形槽，位于face->glyph。

 通过 FT_Load_Glyph() 来加载一个字形图像到字形槽：

```c
FT_Load_Glyph(face, glyph_index, load_flags); /* load_flags：装载标志，一般填FT_LOAD_DEFAULT */
```

并且更新 face->glyph 下的其它成员，比如：

```c
FT_Int            bitmap_left;            //该字形图像的最左边的X值
FT_Int            bitmap_top;             //该字形图像的最上边的Y值
```

### 2.6.3 将字形转化为位图

通过 FT_Render_Glyph() 函数，将字形槽的字形图像转为位图，并存到 face->glyph->bitmap->buffer[]里：

```c
FT_Render_Glyph(face->glyph, render_mode); /* render_mode：渲染模式 */
```

渲染模式 render_mode 通常设为以下2种:

- FT_RENDER_MODE_NORMAL：表示生成位图是RGB888格式
- FT_RENDER_MODE_MONO：表示生成位图每个像素是1位的(黑白图)

并更新face->glyph->bitmap下的其它成员,比如:

```c
int             rows;         //该位图总高度,有多少行
int             width;        //该位图总宽度,有多少列像素点
int             pitch:        //指一行的数据跨度(字节数),比如对于24位(3字节)的24*30汉字,则pitch=24*3
char            pixel_mode    //像素模式，1 指单色的，8 表示反走样灰度值
unsigned char*  buffer        //glyph 的点阵位图内存绶冲区
```

### 2.6.4 一步到位

也可以直接使用 FT_Load_Char() 来代替 FT_Get_Char_Index()、FT_Get_Load_Glyph() 和 FT_Render_Glyph()，一步到位，例如：

```c
FT_Load_Char(face, char_code, FT_LOAD_RENDER);
```

其中 FT_LOAD_RENDER 表示直接将图像转为位图，所以不需要使用 FT_Render_Glyph() 函数。 

该函数默认生成的位图是 FT_RENDER_MODE_NORMAL 类型，即 RGB888。

若想生成 FT_RENDER_MODE_MONO (二值化位图) 类型，操作如下:

```c
FT_Load_Char(face, char_code, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
```

生成出来的二值化位图 face->glyph->bitmap->buffer[] 里的一个字节表示 8 个像素点。

### 2.7 完整示例代码

至此，将矢量字体解析为位图已完成，下面直接上完整的示例代码：

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "freetype.h"

/* 字体数据（ttf） */
typedef struct _ft_fontinfo {
    FT_Face    face;     /* FreeType库句柄对象 */
    FT_Library library;  /* 外观对象（描述了特定字样和风格，比如斜体风格等） */
    int32_t     mono;    /* 是否为二值化模式 */
} ft_fontinfo;

/* 字模格式常量定义 */
typedef enum _glyph_format_t {
    GLYPH_FMT_ALPHA, /* 每个像素占用1个字节 */
    GLYPH_FMT_MONO,  /* 每个像素占用1个比特 */
} glyph_format_t;

/* 字模（位图） */
typedef struct _glyph_t {
    int16_t  x;
    int16_t  y;
    uint16_t w;
    uint16_t h;
    uint16_t advance;  /* 占位宽度 */
    uint8_t  format;   /* 字模格式 */
    uint8_t  pitch;    /* 跨距（每行像素个数 * 单个像素所占字节数） */
    uint8_t  *data;    /* 字模数据：每个像素点占用一个字节 */
    void     *handle;  /* 保存需要释放的句柄 */
} glyph_t;

/* 获取二值化位图上像素点的值 */
uint8_t bitmap_mono_get_pixel(const uint8_t* buff, uint32_t w, uint32_t h, uint32_t x, uint32_t y) {
    /* 计算字节偏移 */
    uint32_t line_length = ((w + 15) >> 4) << 1;
    uint32_t offset = y * line_length + (x >> 3);

    /* 计算位偏移 */
    uint32_t offset_bit = 7 - (x % 8);

    const uint8_t* data = buff + offset;
    if (buff == NULL || (x > w && y > h))
        return 0;
    return (*data >> offset_bit) & 0x1;
}

/* 获取字模 */
static int font_ft_get_glyph(ft_fontinfo *font_info, wchar_t c, float font_size, glyph_t* g) {
    FT_Glyph glyph;
    FT_GlyphSlot glyf;
    FT_Int32 flags = FT_LOAD_DEFAULT | FT_LOAD_RENDER ;

    if (font_info->mono) {
        flags |= FT_LOAD_TARGET_MONO;
    }
    /* 设置字体大小 */
    FT_Set_Char_Size(font_info->face, 0, font_size * 64, 0, 96);
    //FT_Set_Pixel_Sizes(font_info->face, 0, font_size);

    /* 通过编码加载字形并将其转化为位图（保存在face->glyph->bitmap中） */
    if (!FT_Load_Char(font_info->face, c, flags)) {
        glyf = font_info->face->glyph;
        FT_Get_Glyph(glyf, &glyph);

        g->format = GLYPH_FMT_ALPHA;
        g->h = glyf->bitmap.rows;
        g->w = glyf->bitmap.width;
        g->pitch = glyf->bitmap.pitch;
        g->x = glyf->bitmap_left;
        g->y = -glyf->bitmap_top;
        g->data = glyf->bitmap.buffer;
        g->advance = glyf->metrics.horiAdvance / 64;

        if (g->data != NULL) {
            if (glyf->bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
                g->format = GLYPH_FMT_MONO;
            }
            g->handle = glyph;
        }
        else {
            FT_Done_Glyph(glyph);
        }
    }
    return g->data != NULL || c == ' ' ? 1 : 0;
}

int main(int argc, const char *argv[])
{
    ft_fontinfo   font_info;         /* 字库信息 */
    long int      size = 0;          /* 字库文件大小 */
    unsigned char *font_buf = NULL;  /* 字库文件数据 */

    /* 加载字库文件存入font_buf */
    FILE *font_file = fopen("../font/default.ttf", "rb");
    if (font_file == NULL) {
        printf("Can not open font file!\n");
        getchar();
        return 0;
    }
    fseek(font_file, 0, SEEK_END); /* 设置文件指针到文件尾，基于文件尾偏移0字节 */
    size = ftell(font_file);       /* 获取文件大小（文件尾 - 文件头  单位：字节） */
    fseek(font_file, 0, SEEK_SET); /* 重新设置文件指针到文件头 */

    font_buf = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(font_buf, size, 1, font_file);
    fclose(font_file);

    font_info.mono = 1;  /* 设置为二值化模式 */

    /* 初始化FreeType */
    FT_Init_FreeType(&(font_info.library));

    /* 从font_buf中提取外观 */
    FT_New_Memory_Face(font_info.library, font_buf, size, 0, &(font_info.face));

    /* 设置字体编码方式为UNICODE */
    FT_Select_Charmap(font_info.face, FT_ENCODING_UNICODE);


    glyph_t g;
    wchar_t c = L'a';
    float   font_size = 36;  /* 设置字体大小 */
    font_ft_get_glyph(&font_info, c, font_size, &g);  /* 获取字模 */

    /* 打印字模信息 */
    int i = 0, j = 0;
    if (g.format == GLYPH_FMT_MONO) {
        for (j = 0; j < g.h; ++j) {
            for (i = 0; i < g.w; ++i) {
                uint8_t pixel = bitmap_mono_get_pixel(g.data, g.w, g.h, i, j);
                if (pixel) {
                    printf("0");
                }
                else {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }
    else if (g.format == GLYPH_FMT_ALPHA) {
        for (j = 0; j < g.h; ++j) {
            for (i = 0; i < g.w; ++i)
                putchar(" .:ioVM@"[g.data[j*g.w + i] >> 5]);
            putchar('\n');
        }
    }

    /* 释放资源 */
    FT_Done_Glyph(g.handle);
    FT_Done_FreeType(font_info.library);
    free(font_buf);
    getchar();

    return 0;
}
```

输出如下：



![img](https://pic1.zhimg.com/80/v2-78c687c13124be91db92c11c9b64bbd4_720w.jpg)



## 三、总结

以上便是 FreeType 库的基本用法，可以看出使用过程还是很清晰明了的，其中需要调整的参数主要是字符编码、字体大小（字号）以及渲染模式。

更多资料可以访问以下网址：

FreeType 库下载: [https://www.freetype.org/download.html](https://link.zhihu.com/?target=https%3A//www.freetype.org/download.html)

FreeType 英文参考文档下载：[https://www.freetype.org/freetype2/docs/documentation.html](https://link.zhihu.com/?target=https%3A//www.freetype.org/freetype2/docs/documentation.html)

FreeType 中文使用参考:

[http://wenku.baidu.com/view/2d24be10cc7931b765ce155b.html](https://link.zhihu.com/?target=http%3A//wenku.baidu.com/view/2d24be10cc7931b765ce155b.html)

[https://wenku.baidu.com/view/e7149f6748d7c1c708a14574.html](https://link.zhihu.com/?target=https%3A//wenku.baidu.com/view/e7149f6748d7c1c708a14574.html)

希望能对大家有所帮助。





# freetype学习笔记一

[![img](https://upload.jianshu.io/users/upload_avatars/2368622/731128851415.jpg?imageMogr2/auto-orient/strip|imageView2/1/w/96/h/96/format/webp)](https://www.jianshu.com/u/2f9ddadaabfe)

[继续向前冲](https://www.jianshu.com/u/2f9ddadaabfe)

0.072019.04.15 07:53:36字数 668阅读 3,152

# freetype介绍

freetype可以看作是一组组件，每个组件负责一部分任务，它们包括

- 客户应⽤用程序一般会调⽤用freetype⾼高层API，它的功能都在一个组件中，叫做基础层。

- 根据上下⽂文和环境，基础层会调⽤用一个或多个模块进⾏行行⼯工作，⼤大多数情况下，客户应⽤用 程序不不知道使⽤用那个模块。

- 基础层还包含一组例例程来进⾏行行一些共通处理理，例例如内存分配，列列表处理理、io流解析、固 定点计算等等，这些函数可以被模块随意调⽤用，它们形成了了一个底层基础API。

  ![img](https://upload-images.jianshu.io/upload_images/2368622-0b70d4e021991f63.jpeg?imageMogr2/auto-orient/strip|imageView2/2/w/592/format/webp)

  image.jpeg

## 基础类介绍

因为freetype支持多种字体的加载以及渲染，所有分为多个model，

### FT_LibraryRec

freetype工程的library类 此类保存freetype的根数据，用`FT_New_Library()`创建library对象 用`FT_Done_Library()`方法释放对象
 *构造略*

### FT_FaceRec_

freetype工程 root类，一个face 对象代表了一个字体文件。每个Face对象拥有一个`@FT_GlyphSlot` 对象, 以及一个或多个`@FT_Size` 对象。
 num_faces; //字体文件所含外观数 一些字体文件可能包含多个face
 **遗存：**face数量可能对应truetype中的cmap表 最少truetype字体应该是如此。

```c
typedef struct  FT_FaceRec_
  {
    FT_Long           num_faces; //字体文件所含外观数 一些字体文件可能包含多个face    
    FT_Long           face_index;//当前外观索引值
    FT_Long           face_flags;
    FT_Long           style_flags;
    FT_Long           num_glyphs;//当前外观所含字形文件数
    FT_String*        family_name;
    FT_String*        style_name;
    FT_Int            num_fixed_sizes;
    FT_Bitmap_Size*   available_sizes;
    FT_Int            num_charmaps;// 字符表数
    FT_CharMap*       charmaps;//字符表数组
    FT_Generic        generic;
    /*# The following member variables (down to `underline_thickness') */
    /*# are only relevant to scalable outlines; cf. @FT_Bitmap_Size    */
    /*# for bitmap fonts.                                              */
    FT_BBox           bbox;
    FT_UShort         units_per_EM;
    FT_Short          ascender;
    FT_Short          descender;
    FT_Short          height;

    FT_Short          max_advance_width;
    FT_Short          max_advance_height;

    FT_Short          underline_position;
    FT_Short          underline_thickness;

    FT_GlyphSlot      glyph;// 字形槽对象
    FT_Size           size;
    FT_CharMap        charmap;//当前字符表

    /*@private begin */

    FT_Driver         driver;
    FT_Memory         memory;
    FT_Stream         stream;

    FT_ListRec        sizes_list;

    FT_Generic        autohint;
    void*             extensions;

    FT_Face_Internal  internal;

    /*@private end */

  } FT_FaceRec;
```

### FT_CharMapRec_

character map句柄，用于转换character code 到glyph indexes ，
 **注意：**可能一写字体可能存在多个character map

```c
 typedef struct  FT_CharMapRec_
  {
    FT_Face      face;
    FT_Encoding  encoding;
    FT_UShort    platform_id;
    FT_UShort    encoding_id;

  } FT_CharMapRec;
```

### FT_GlyphSlotRec_

存储Glyph对象句柄 一个slot相当于一个容器可以加载任何glyphs内容，每次使用`@FT_Load_Glyph` 或 `@FT_Load_Char` slot容器都会被新的glyph数据重新擦除。glyph数据包括outline或者bitmap数据。**例如：**一个outline glyph用于TrueType 或者Type-1
 通过FT_FaceRec_结构体中的num_glyphs属性可知，不同字体拥有的glyph数量不一样，参看实际字体文件可知，FT_GlyphSlotRec_存储着所有的字形数据，也是我们需要裁剪的重点。

```c
  typedef struct  FT_GlyphSlotRec_
  {
    FT_Library        library;
    FT_Face           face; //指向父对象(FT_Face对象)
    FT_GlyphSlot      next;
    FT_UInt           reserved;       /* retained for binary compatibility */
    FT_Generic        generic;

    FT_Glyph_Metrics  metrics; //字形尺寸对象
    FT_Fixed          linearHoriAdvance;
    FT_Fixed          linearVertAdvance;
    FT_Vector         advance;// 步进对象

    FT_Glyph_Format   format;//字形槽格式

    FT_Bitmap         bitmap;//字形位图
    FT_Int            bitmap_left; //位图左行距离
    FT_Int            bitmap_top; //位图上行距离
      

    FT_Outline        outline;

    FT_UInt           num_subglyphs;
    FT_SubGlyph       subglyphs;

    void*             control_data;
    long              control_len;

    FT_Pos            lsb_delta;
    FT_Pos            rsb_delta;

    void*             other;

    FT_Slot_Internal  internal;

  } FT_GlyphSlotRec;
```

### FT_Slot_InternalRec_

FT_GlyphSlot 的私有数据，GlyphSlot中包含的渲染字体的数据包含在其中

```c
  typedef struct  FT_Slot_InternalRec_
  {
    FT_GlyphLoader  loader;
    FT_UInt         flags;
    FT_Bool         glyph_transformed;
    FT_Matrix       glyph_matrix;
    FT_Vector       glyph_delta;
    void*           glyph_hints;

  } FT_GlyphSlot_InternalRec;
```

### FT_GlyphLoaderRec_

用于装载glyph  FT_GlyphLoaderRec_程序实现不是高级 API 的一部分

```cpp
  typedef struct  FT_GlyphLoaderRec_
  {
    FT_Memory        memory;
    FT_UInt          max_points;
    FT_UInt          max_contours;
    FT_UInt          max_subglyphs;
    FT_Bool          use_extra;

    FT_GlyphLoadRec  base;
    FT_GlyphLoadRec  current;

    void*            other;            /* for possible future extension? */

  } FT_GlyphLoaderRec;
 typedef struct  FT_GlyphLoadRec_
  {
    FT_Outline   outline;       /* outline                   */
    FT_Vector*   extra_points;  /* extra points table        */
    FT_Vector*   extra_points2; /* second extra points table */
    FT_UInt      num_subglyphs; /* number of subglyphs       */
    FT_SubGlyph  subglyphs;     /* subglyphs                 */

  } FT_GlyphLoadRec, *FT_GlyphLoad;
```

## TrueType 字体类介绍

TrueType结构在 ttypes.h 中

## TT_FaceRec_

TrueType Face类 这个对象，这些TrueType字体文件中的类模型与分辨率和与点大小无关的数据。与[TrueType规范](https://links.jianshu.com/go?to=%5Bhttps%3A%2F%2Fdeveloper.apple.com%2Ffonts%2FTrueType-Reference-Manual%2FRM06%2FChap6cmap.html%5D(https%3A%2F%2Fdeveloper.apple.com%2Ffonts%2FTrueType-Reference-Manual%2FRM06%2FChap6cmap.html))相对应。

## 基础方法介绍

```c
    // 1. 初始化freetype2库
    error = FT_Init_FreeType(&library);

    // 2. 创建一个face
    error = FT_New_Face(library, "/Users/qishao/Downloads/Rough cut.ttf", 0, &face);

    // 3. 设置字体尺寸
    error = FT_Set_Char_Size(face, 16*64, 16*64, 96, 96);

    // 4. 获取字符图像索引
    charIdx = FT_Get_Char_Index(face, wch);

    // 5. 加载字符图像
    FT_Load_Glyph(face, charIdx, FT_LOAD_DEFAULT);
    if (face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
    {
        FT_Outline_Embolden(&(face->glyph->outline), 16);    // 加粗轮廓线
    }

    // 6. 获取字符位图
    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
    {
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    }
```

一篇比较全面的介绍性博客合集[《FreeType Glyph Conventions》](https://links.jianshu.com/go?to=%5Bhttps%3A%2F%2Fblog.csdn.net%2Fsloan6%2Farticle%2Fdetails%2F8330793%5D(https%3A%2F%2Fblog.csdn.net%2Fsloan6%2Farticle%2Fdetails%2F8330793))
