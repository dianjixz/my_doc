 

一、前言

这段时间的工作需要研究stb_truetype库，因此本篇文章记录一下该库的基本用法。stb_truetype是一个常见字体加载库, 只有一个头文件, 功能虽没有 FreeType 库强大，但代码量小很多，在Flash非常小的开发板上也可以用，觉得 FreeType 库太大的，可以考虑使用stb_truetype库。

    stb库的GitHub仓库：https://github.com/nothings/stb。

二、stb_truetype解析ttf字体

使用stb_truetype库解析ttf字体的步骤通常如下：
1、加载并初始化ttf字体文件；
2、设置字体大小（字号）并计算缩放比例；
3、获取垂直方向上的度量并根据缩放比例调整，比如字高、行间距等；
4、获取水平方向上的度量，比如字宽、字间距等；
5、获取字符图片的边框（每个字符转化为图像的边界）；
6、调整每个字体图像的宽高（代码中的x、y），并渲染字体；
至此，解析ttf字体已完成，附加步骤为使用stb_image_write库将渲染出来的图像保存为本地图片，下面直接上代码：

    注意：在包含stb_truetype.h头文件的时候需要定义STB_TRUETYPE_IMPLEMENTATION，否则将会无法使用。


**值得注意的是，字符寻址编码是 unicode 类型，日常文件是 utf8 类型。所以使用中文时需要对其进行转换。

~~~ c
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" /* http://nothings.org/stb/stb_image_write.h */

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h" /* http://nothings.org/stb/stb_truetype.h */

int main(int argc, const char *argv[])
{
    /* 加载字体（.ttf）文件 */
    long int size = 0;
    unsigned char *fontBuffer = NULL;

    FILE *fontFile = fopen("font/default.ttf", "rb");
    if (fontFile == NULL)
    {
        printf("Can not open font file!\n");
        return 0;
    }
    fseek(fontFile, 0, SEEK_END); /* 设置文件指针到文件尾，基于文件尾偏移0字节 */
    size = ftell(fontFile);       /* 获取文件大小（文件尾 - 文件头  单位：字节） */
    fseek(fontFile, 0, SEEK_SET); /* 重新设置文件指针到文件头 */
    
    fontBuffer = calloc(size, sizeof(unsigned char));
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);
    
    /* 初始化字体 */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        printf("stb init font failed\n");
    }
    
    /* 创建位图 */
    int bitmap_w = 512; /* 位图的宽 */
    int bitmap_h = 128; /* 位图的高 */
    unsigned char *bitmap = calloc(bitmap_w * bitmap_h, sizeof(unsigned char));
    
    /* "STB"的 unicode 编码 */
    char word[20] = {0x53, 0x54, 0x42};
    
    /* 计算字体缩放 */
    float pixels = 64.0;                                    /* 字体大小（字号） */
    float scale = stbtt_ScaleForPixelHeight(&info, pixels); /* scale = pixels / (ascent - descent) */
    
    /** 
     * 获取垂直方向上的度量 
     * ascent：字体从基线到顶部的高度；
     * descent：基线到底部的高度，通常为负值；
     * lineGap：两个字体之间的间距；
     * 行间距为：ascent - descent + lineGap。
    */
    int ascent = 0;
    int descent = 0;
    int lineGap = 0;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    /* 根据缩放调整字高 */
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    
    int x = 0; /*位图的x*/
    
    /* 循环加载word中每个字符 */
    for (int i = 0; i < strlen(word); ++i)
    {
        /** 
          * 获取水平方向上的度量
          * advanceWidth：字宽；
          * leftSideBearing：左侧位置；
        */
        int advanceWidth = 0;
        int leftSideBearing = 0;
        stbtt_GetCodepointHMetrics(&info, word[i], &advanceWidth, &leftSideBearing);
    
        /* 获取字符的边框（边界） */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
    
        /* 计算位图的y (不同字符的高度不同） */
        int y = ascent + c_y1;
    
        /* 渲染字符 */
        int byteOffset = x + roundf(leftSideBearing * scale) + (y * bitmap_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, bitmap_w, scale, scale, word[i]);
    
        /* 调整x */
        x += roundf(advanceWidth * scale);
    
        /* 调整字距 */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        x += roundf(kern * scale);
    }
    
    /* 将位图数据保存到1通道的png图像中 */
    stbi_write_png("STB.png", bitmap_w, bitmap_h, 1, bitmap, bitmap_w);
    
    free(fontBuffer);
    free(bitmap);
    
    return 0;
}
~~~

运行后可以见STB.png图片，效果如下：
在这里插入图片描述
三、总结

以上便是stb_truetype库的基本用法，可以看出使用过程比较简单，其中需要调整的参数主要是字体大小（字号），使用过程中需要注意以下两点：

1、上面已经提过，这里再提一遍，在包含stb_truetype.h头文件的时候需要定义STB_TRUETYPE_IMPLEMENTATION，否则将会无法使用。

2、调用stb_truetype库函数传入的字符编码必须是unicode编码。
————————————————
版权声明：本文为CSDN博主「天上下橙雨」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_40026797/article/details/112755090


https://blog.csdn.net/ithouse/article/details/70176215?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-70176215-blog-108066748.pc_relevant_paycolumn_v3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-70176215-blog-108066748.pc_relevant_paycolumn_v3&utm_relevant_index=2

https://blog.csdn.net/qq_37868757/article/details/108066748?spm=1001.2101.3001.6650.5&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5-108066748-blog-23750999.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5-108066748-blog-23750999.pc_relevant_default&utm_relevant_index=9