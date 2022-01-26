#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#define FB_DEV_PATH	"/dev/fb0"

int main ()   
{  
    int fp=0;  
    struct fb_var_screeninfo vinfo;  
    struct fb_fix_screeninfo finfo;  
    long screensize=0;  
    char *fbp = NULL, *test_fbp=NULL;    

    /* 读/写的方式打开framebuffer的设备文件 */
    fp = open(FB_DEV_PATH, O_RDWR);  
    if(fp < 0) {
        printf("Error : Can not open framebuffer device/n");
        exit(1);  
    }  

    /* 获取硬件决定的显示参数（这部分参数不能被修改） */
    if(ioctl(fp, FBIOGET_FSCREENINFO, &finfo)){  
        printf("Error reading fixed information/n");  
        exit(2);  
    }  

    /* 获取软件决定的显示参数（这部分参数可以由用户修改） */
    if(ioctl(fp, FBIOGET_VSCREENINFO, &vinfo)){  
        printf("Error reading variable information/n");  
        exit(3);  
    }  

    /* 获取一些显示参数 */
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;  
    printf("The phy mem = 0x%x, total size = %d(byte)\n", finfo.smem_start, finfo.smem_len);  
    printf("xres =  %d, yres =  %d, bits_per_pixel = %d， line_length = %d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.line_length);  
    printf("So the screensize = %d(byte), using %d frame\n", screensize, finfo.smem_len/screensize);
    printf("vinfo.xoffset = %d, vinfo.yoffset = %d\n", vinfo.xoffset, vinfo.yoffset);  
    printf("vinfo.vmode is :%d\n", vinfo.vmode);  
    printf("finfo.ypanstep is :%d\n", finfo.ypanstep);  
    printf("vinfo.red.offset=0x%x\n", vinfo.red.offset);
    printf("vinfo.red.length=0x%x\n", vinfo.red.length);
    printf("vinfo.green.offset=0x%x\n", vinfo.green.offset);
    printf("vinfo.green.length=0x%x\n", vinfo.green.length);
    printf("vinfo.blue.offset=0x%x\n", vinfo.blue.offset);
    printf("vinfo.blue.length=0x%x\n", vinfo.blue.length);
    printf("vinfo.transp.offset=0x%x\n", vinfo.transp.offset);
    printf("vinfo.transp.length=0x%x\n", vinfo.transp.length);
    
    /* 将显示内存映射出来，从这里开始，就可以通过操作fbp指向的内存来操作显示屏了 */
    fbp =(char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);  
    if ((int)fbp == -1)  
    {    
        printf ("Error: failed to map framebuffer device to memory./n");  
        exit (4);  
    }
    printf("Get virt mem = %p\n", fbp);  

    /* 让屏幕全部填充红色 */
    for (int i = 0; i < screensize; i += 4)
    {
        fbp[i]  = 0x00;     // B
        fbp[i + 1] = 0x00;  // G
        fbp[i + 2] = 0xFF;  // R
    }

    /* 刷新显示（有些framebuffer实现了自刷新，就可以不调用这个api） */
    ioctl(fp, FBIOPAN_DISPLAY, &vinfo);

	/* 解除映射 */
    munmap(fbp, screensize);
    
    /* 关闭fb驱动设备 */
    close (fp);
    return 0;
}  
