#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#define PAGE_SHIFT  12
#define PAGE_MASK       (~((1 << PAGE_SHIFT) - 1)) 

#include "fbtools.h"

#define TRUE 1
#define FALSE 0
#define MAX (x, y)((x) > (y) ? (x) : (y))
#define MIN (x, y)((x) < (y) ? (x) : (y))

/* open & init a frame buffer */
int fb_open(PFBDEV pFbdev)
{
    pFbdev->fb = open(pFbdev-> dev, O_RDWR);
    if (pFbdev->fb < 0)
    {
        printf("Error opening %s: %m. Check kernel config/n", pFbdev->dev);
        return FALSE;
    }
    if (-1 == ioctl(pFbdev->fb, FBIOGET_VSCREENINFO, &(pFbdev->fb_var)))
    {
        printf("ioctl FBIOGET_VSCREENINFO/n");
        return FALSE;
    }
    if (-1 == ioctl(pFbdev->fb, FBIOGET_FSCREENINFO, &(pFbdev->fb_fix)))
    {
        printf("ioctl FBIOGET_FSCREENINFO/n");
        return FALSE;
    }

    /*map physics address to virtual address */
    pFbdev->fb_mem_offset = (unsigned long)(pFbdev->fb_fix.smem_start) & (~PAGE_MASK);
    pFbdev->fb_mem = (unsigned long int)mmap(NULL, pFbdev->fb_fix.smem_len + pFbdev->fb_mem_offset,
                                                PROT_READ | PROT_WRITE, MAP_SHARED, pFbdev->fb, 0);
    if (-1L == (long)pFbdev->fb_mem)
    {
        printf("mmap error! mem:%d offset:%d/n", pFbdev->fb_mem,
               pFbdev->fb_mem_offset);
        return FALSE;
    }

    return TRUE;
}

/* close frame buffer */
int fb_close(PFBDEV pFbdev)
{
    close(pFbdev->fb);
    pFbdev->fb = -1;
}

/* get display depth */
int get_display_depth(PFBDEV pFbdev)
{
    if (pFbdev->fb <= 0)
    {
        printf("fb device not open, open it first/n");
        return FALSE;
    }
    return pFbdev->fb_var.bits_per_pixel;
}

/* full screen clear */
void fb_memset(void *addr, int c, size_t len)
{
    memset(addr, c, len);
}

/* use by test */
// #define DEBUG
#ifdef DEBUG
main()
{
    FBDEV fbdev;
    memset(&fbdev, 0, sizeof(FBDEV));
    strcpy(fbdev.dev, "/dev/fb0");
    if (fb_open(&fbdev) == FALSE)
    {
        printf("open frame buffer error/n");
        return;
    }

    fb_memset(fbdev.fb_mem + fbdev.fb_mem_offset, 0, fbdev.fb_fix.smem_len);

    fb_close(&fbdev);
}
#endif