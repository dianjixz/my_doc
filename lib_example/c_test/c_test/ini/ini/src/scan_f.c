#include <stdio.h>
#include <stdint.h>
// #include <cstdlib.h> 
#include <string.h>

#define COLOR_RGB565_TO_R5(pixel) (((pixel) >> 3) & 0x1F)
#define COLOR_RGB565_TO_G6(pixel) \
({ \
    __typeof__ (pixel) _pixel = (pixel); \
    ((_pixel & 0x07) << 3) | (_pixel >> 13); \
})
#define COLOR_RGB565_TO_B5(pixel) (((pixel) >> 8) & 0x1F)
#define COLOR_RGB565_TO_R8(pixel) COLOR_R5_TO_R8(COLOR_RGB565_TO_R5(pixel))
#define COLOR_RGB565_TO_G8(pixel) COLOR_G6_TO_G8(COLOR_RGB565_TO_G6(pixel))
#define COLOR_RGB565_TO_B8(pixel) COLOR_B5_TO_B8(COLOR_RGB565_TO_B5(pixel))

int checkCPU()
{
    union w
    {
        int a;
        char b;
    } c;
    c.a = 1;
    return (c.b == 1); // 小端返回TRUE,大端返回FALSE
}

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
//当系统为大端时，把face_code结构体中的以小端模式存储的数据转换为大端
inline face_code* _code_reverse(face_code* code) {  
    //gcc下用内置函数转换
    for(int i=0;i<CODE_FLOAT_NUM;i++)
        code->element[i]=(float)__builtin_bswap32((unsigned __int32)code->element[i]);
    code->sum=(double)__builtin_bswap64((unsigned __int64)code->sum);
    return code;
}
#define FACE_CODE_CONVERT(code) _code_reverse(code)
#else//小端模式则什么也不做直接返回
#define FACE_CODE_CONVERT(code) code
#endif



typedef struct pixel_s {
    char red;
    char green;
    char blue;
} pixel24_t;

#define pixel24232(_u24_t) \
({\
    __typeof__ (_u24_t) ___u24_t = _u24_t;\
    ((*((uint32_t*)((void*)&___u24_t))) & 0x00ffffff);\
})
#define pixel32224(_u32_t) \
({\
    __typeof__ (_u32_t) __u32_t = _u32_t;\
    (*((pixel24_t*)((void*)&__u32_t)));\
})



int main(int argc,char **argv)
{

    uint16_t rgb565_p = 0x00f8;
    uint8_t r5 = COLOR_RGB565_TO_R5(rgb565_p);
    uint8_t g6 = COLOR_RGB565_TO_G6(rgb565_p);
    uint8_t b5 = COLOR_RGB565_TO_B5(rgb565_p);
    
    printf("r5:%x\n", r5);
    printf("g6:%x\n", g6);
    printf("b5:%x\n", b5);

    if(checkCPU())
    {
        printf("xiao!\n");
    }
    else
    {
        printf("daduan!\n");
    }
    

    char a[11] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b};
    for(int i = 0;i< 11;i++)
    {
        printf("pixel %d:%d\n",i,a[i]);
    }
    pixel24_t *tmp = a + 3;
    printf("pixel:%d,%d,%d\n",tmp->pixel_1, tmp->pixel_2, tmp->pixel_3);

    pixel24_t nihao = *tmp;
    // uint8_t tmp = nihao & 0xff;
    int tmpss = pixel24232(nihao);
    printf("tmpss:%d",tmpss);
    tmpss = 0x111213;
    nihao = pixel32224(tmpss);
    printf("nihao:%d\n",nihao);
    printf("pixel:%d,%d,%d\n",nihao.pixel_1, nihao.pixel_2, nihao.pixel_3);

    return 0;
}

// -EL和-EB