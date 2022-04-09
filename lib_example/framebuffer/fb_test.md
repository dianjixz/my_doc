 

~~~ c
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
static int fd_fb;
static struct fb_var_screeninfo var;	/* Current var */
static struct fb_fix_screeninfo  finfo;
static int screen_size;
static unsigned char *fb_base;
static unsigned int line_width;
static unsigned int pixel_width;

/**********************************************************************
 * 函数名称： lcd_put_pixel
 * 功能描述： 在LCD指定位置上输出指定颜色（描点）
 * 输入参数： x坐标，y坐标，颜色
 * 输出参数： 无
 * 返 回 值： 会
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2020/05/12	     V1.0	  zh(angenao)	      创建
 ***********************************************************************/ 
void lcd_put_pixel(int x, int y, unsigned int color)
{
	unsigned char *pen_8 = fb_base+y*line_width+x*pixel_width;
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (var.bits_per_pixel)
	{
		case 8:
		{
			*pen_8 = color;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue  = (color >> 0) & 0xff;
			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = color;
			break;
		}
		case 24:
		{
			*pen_32=color;
			break;
		}
		case 32:
		{
			*pen_32 = color;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", var.bits_per_pixel);
			break;
		}
	}
}

int main(int argc, char **argv)
{
	int i,j;
	
	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}
	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var\n");
		return -1;
	}
    if (ioctl(fd_fb,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information\n");
        exit(-1);
	}
     //显示结构体信息
  	 printf("The mem is :%d\n",finfo.smem_len);  
   	 printf("The line_length is :%d\n",finfo.line_length);  
   	 printf("The xres is :%d\n",var.xres);  
   	 printf("The yres is :%d\n",var.yres);  
   	 printf("bits_per_pixel is :%d\n",var.bits_per_pixel); 
	line_width  = var.xres * var.bits_per_pixel / 8;

	pixel_width = var.bits_per_pixel / 8;
  	  //计算显存大小
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;


  	  /*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/
	fb_base = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fb_base == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}

	/* 清屏: 全部设为白色 */
	memset(fb_base, 0xff, screen_size);


	
	for(i = 0 ; i < var.xres/2 ; i++)
		for(j = 0 ; j < var.yres/2 ; j++)
			lcd_put_pixel(i,j, 0xFF0000); //设为红色

	for(i = var.xres/2 ; i < var.xres ; i++)
		for(j = var.yres/2 ; j < var.yres ; j++)
			lcd_put_pixel(i,j, 0xFF0000); //设为红色


	munmap(fb_base , screen_size);/*解除映射*/
	close(fd_fb); /*关闭文件*/
	
	return 0;	
}
~~~

