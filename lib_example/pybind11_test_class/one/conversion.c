// #include "imlib.h"
#include "conversion.h"
#include <stdint.h>

#define DEBUG(da) \
    do            \
    {             \
    } while (0)

#define UpAlign4(n) (((n) + 3) & ~3)
#define UpAlign8(n) (((n) + 7) & ~7)






int rgb565_to_rgb888(const void *psrc, int w, int h, void *pdst)
{
    int srclinesize = UpAlign4(w * 2);
    int dstlinesize = UpAlign4(w * 3);

    const unsigned char *psrcline;
    const unsigned short *psrcdot;
    unsigned char *pdstline;
    unsigned char *pdstdot;

    int i, j;
    // int k = 0;

    if (!psrc || !pdst || w <= 0 || h <= 0)
    {
        DEBUG("rgb565_to_rgb888 : parameter error\n");
        return -1;
    }
    psrcline = (const unsigned char *)psrc;
    pdstline = (unsigned char *)pdst;
    DEBUG("bushi 1\r\n");
    for (i = 0; i < h; i++)
    {
        psrcdot = (const unsigned short *)psrcline;
        pdstdot = pdstline;
        for (j = 0; j < w; j++)
        {
            //565 b|g|r -> 888 r|g|b
            // DEBUG("进行到x:%d,y:%d\r\n",i,j);
            *pdstdot++ = (unsigned char)(((*psrcdot) >> 11) << 3);      
            *pdstdot++ = (unsigned char)(((*psrcdot) >> 5) << 2);
            *pdstdot++ = (unsigned char)(((*psrcdot) >> 0) << 3);
            psrcdot++;
            // k++;
        }
        psrcline += srclinesize;
        pdstline += dstlinesize;
    }
    // printf("转换了:%d\r\n",k);
    return 0;
}

int rgb888_to_rgb565(const void *psrc, int w, int h, void *pdst)
{
    int srclinesize = UpAlign4(w * 3);
    int dstlinesize = UpAlign4(w * 2);

    const unsigned char *psrcline;
    const unsigned char *psrcdot;
    unsigned char *pdstline;
    unsigned short *pdstdot;

    int i, j;

    if (!psrc || !pdst || w <= 0 || h <= 0)
    {
        DEBUG("rgb888_to_rgb565 : parameter error\n");
        return -1;
    }

    psrcline = (const unsigned char *)psrc;
    pdstline = (unsigned char *)pdst;
    for (i = 0; i < h; i++)
    {
        psrcdot = psrcline;
        pdstdot = (unsigned short *)pdstline;
        for (j = 0; j < w; j++)
        {
            //888 r|g|b -> 565 r|g|b
            *pdstdot = (((psrcdot[2] >> 3) & 0x1F) << 0)     //R

                       | (((psrcdot[1] >> 2) & 0x3F) << 5)   //G

                       | (((psrcdot[0] >> 3) & 0x1F) << 11); //B
            psrcdot += 3;
            pdstdot++;
        }
        psrcline += srclinesize;
        pdstline += dstlinesize;
    }

    return 0;
}
#define RGB888_RED 0x00ff0000
#define RGB888_GREEN 0x0000ff00
#define RGB888_BLUE 0x000000ff

#define RGB565_RED 0xf800
#define RGB565_GREEN 0x07e0
#define RGB565_BLUE 0x001f

unsigned short RGB888ToRGB565(unsigned int n888Color)
{
    unsigned short n565Color = 0;

    // 获取RGB单色，并截取高位
    unsigned char cRed = (n888Color & RGB888_RED) >> 19;
    unsigned char cGreen = (n888Color & RGB888_GREEN) >> 10;
    unsigned char cBlue = (n888Color & RGB888_BLUE) >> 3;

    // 连接
    n565Color = (cRed << 11) + (cGreen << 5) + (cBlue << 0);
    return n565Color;
}

unsigned int RGB565ToRGB888(unsigned short n565Color)
{
    unsigned int n888Color = 0;

    // 获取RGB单色，并填充低位
    unsigned char cRed = (n565Color & RGB565_RED) >> 8;
    unsigned char cGreen = (n565Color & RGB565_GREEN) >> 3;
    unsigned char cBlue = (n565Color & RGB565_BLUE) << 3;

    // 连接
    n888Color = (cRed << 16) + (cGreen << 8) + (cBlue << 0);
    return n888Color;
}




void rgb888_to_rgb565_img(image_t *img_in,image_t *img_out)
{
	uint8_t cha;
	uint8_t *nihao;
	img_out->w = img_in->w;
	img_out->h = img_in->h;
	img_out->bpp = img_in->h;
    rgb888_to_rgb565(img_in->data,img_in->w,img_in->h,img_out->data);
    nihao = (uint8_t *)img_out->data;
    // for (int i = 0;i<img_in->w * img_in->h * 2;i = i + 2)
    // {
    //     cha = nihao[i];
    //     nihao[i] = nihao[i + 1];
    //     nihao[i + 1] = cha;
    // }
}

int rgb565_to_rgb888_img(image_t *img_in,image_t *img_out)
{
    uint8_t cha;
	uint8_t *nihao;
	img_out->w = img_in->w;
	img_out->h = img_in->h;
	img_out->bpp = img_in->h;
    nihao = (uint8_t *)img_out->data;
    // for (int i = 0;i<img_in->w * img_in->h * 2;i = i + 2)
    // {
    //     cha = nihao[i];
    //     nihao[i] = nihao[i + 1];
    //     nihao[i + 1] = cha;
    // }
    rgb565_to_rgb888(img_in->data,img_in->w,img_in->h,img_out->data);
    return 0;

}






// #include <stdint.h>

// //RGB2Lab Lab2RGB
// const float param_13 = 1.0f / 3.0f;
// const float param_16116 = 16.0f / 116.0f;
// const float Xn = 0.950456f;
// const float Yn = 1.0f;
// const float Zn = 1.088754f;

// float gamma(float x)
// {
//     return x > 0.04045 ? powf((x + 0.055f) / 1.055f, 2.4f) : (x / 12.92);
// };

// void RGB2XYZ(unsigned char R, unsigned char G, unsigned char B, float *X, float *Y, float *Z)
// {
//     float RR = gamma(R / 255.0);
//     float GG = gamma(G / 255.0);
//     float BB = gamma(B / 255.0);

//     *X = 0.4124564f * RR + 0.3575761f * GG + 0.1804375f * BB;
//     *Y = 0.2126729f * RR + 0.7151522f * GG + 0.0721750f * BB;
//     *Z = 0.0193339f * RR + 0.1191920f * GG + 0.9503041f * BB;
// }

// void XYZ2Lab(float X, float Y, float Z, float *L, float *a, float *b)
// {
//     float fX, fY, fZ;

//     X /= (Xn);
//     Y /= (Yn);
//     Z /= (Zn);

//     if (Y > 0.008856f)
//         fY = pow(Y, param_13);
//     else
//         fY = 7.787f * Y + param_16116;

//     if (X > 0.008856f)
//         fX = pow(X, param_13);
//     else
//         fX = 7.787f * X + param_16116;

//     if (Z > 0.008856)
//         fZ = pow(Z, param_13);
//     else
//         fZ = 7.787f * Z + param_16116;

//     *L = 116.0f * fY - 16.0f;
//     *L = *L > 0.0f ? *L : 0.0f;
//     *a = 500.0f * (fX - fY);
//     *b = 200.0f * (fY - fZ);
// }

// void rgb_to_lab(unsigned char R, unsigned char G, unsigned char B, float *L, float *a, float *b)
// {
//     float X, Y, Z;
//     RGB2XYZ(R, G, B, &X, &Y, &Z);
//     XYZ2Lab(X, Y, Z, L, a, b);
// }

// extern const float param_13;
// extern const float param_16116;
// extern const float Xn;
// extern const float Yn;
// extern const float Zn;

// float gamma_XYZ2RGB(float x)
// {
//     return x > 0.0031308 ? (1.055f * powf(x, (1 / 2.4f)) - 0.055) : (x * 12.92);
// };

// void XYZ2RGB(float X, float Y, float Z, unsigned char *R, unsigned char *G, unsigned char *B)
// {
//     float RR, GG, BB;
//     RR = 3.2404542f * X - 1.5371385f * Y - 0.4985314f * Z;
//     GG = -0.9692660f * X + 1.8760108f * Y + 0.0415560f * Z;
//     BB = 0.0556434f * X - 0.2040259f * Y + 1.0572252f * Z;

//     RR = gamma_XYZ2RGB(RR);
//     GG = gamma_XYZ2RGB(GG);
//     BB = gamma_XYZ2RGB(BB);

//     RR = CLIP255(RR * 255.0 + 0.5);
//     GG = CLIP255(GG * 255.0 + 0.5);
//     BB = CLIP255(BB * 255.0 + 0.5);

//     *R = (unsigned char)RR;
//     *G = (unsigned char)GG;
//     *B = (unsigned char)BB;
// }

// void Lab2XYZ(float L, float a, float b, float *X, float *Y, float *Z)
// {
//     float fX, fY, fZ;

//     fY = (L + 16.0f) / 116.0;
//     fX = a / 500.0f + fY;
//     fZ = fY - b / 200.0f;

//     if (powf(fY, 3.0) > 0.008856)
//         *Y = powf(fY, 3.0);
//     else
//         *Y = (fY - param_16116) / 7.787f;

//     if (powf(fX, 3) > 0.008856)
//         *X = fX * fX * fX;
//     else
//         *X = (fX - param_16116) / 7.787f;

//     if (powf(fZ, 3.0) > 0.008856)
//         *Z = fZ * fZ * fZ;
//     else
//         *Z = (fZ - param_16116) / 7.787f;

//     (*X) *= (Xn);
//     (*Y) *= (Yn);
//     (*Z) *= (Zn);
// }

// void lab_to_rgb(float L, float a, float b, unsigned char *R, unsigned char *G, unsigned char *B)
// {
//     float X, Y, Z;
//     Lab2XYZ(L, a, b, &X, &Y, &Z);
//     XYZ2RGB(X, Y, Z, R, G, B);
// }

// // int RGB2Lab(IMAGE_TYPE *bmp_img,float *lab_img)
// // {
// // 	DWORD width,height,index;
// // 	WORD  biBitCount;
// // 	T_U8 *dst,*bmp,R,G,B;
// // 	float X,Y,Z,L,a,b;

// // 	T_U32 line_byte;
// // 	T_U16 i,j;

// // 	BITMAPFILEHEADER bf;
// // 	BITMAPINFOHEADER bi;

// // 	memset(&bf, 0, sizeof(bf));
// // 	memset(&bi, 0, sizeof(bi));

// // 	bmp = bmp_img;
// // 	memcpy(&bf,bmp,14);
// // 	memcpy(&bi,&bmp[14],40);

// // 	height = bi.biHeight;
// // 	width  = bi.biWidth;
// // 	biBitCount = bi.biBitCount;//每一个像素由24 bits表示，即RGB分量每一个分量用8 bits表示
// // 	line_byte = WIDTHBYTES(width*bi.biBitCount);

// // 	dst = bmp_img+BMPHEADSIZE;

// // 	for (i = 0; i <height;i++)
// // 	{
// // 		for (j = 0;j < width;j++)
// // 		{
// // 			index = i*line_byte+3*j;
// // 			B = dst[index];
// // 			G = dst[index+1];
// // 			R = dst[index+2];

// // 			RGB2XYZ(R,G,B,&X,&Y,&Z);
// // 			XYZ2Lab(X,Y,Z,&L,&a,&b);

// // 			lab_img[index] = L;
// // 			lab_img[index+1] = a;
// // 			lab_img[index+2] = b;
// // 		}
// // 	}

// // 	return 0;

// // }
// // int Lab2RGB(IMAGE_TYPE *bmp_img,float *lab_img)
// // {
// // 	DWORD width,height,index;
// // 	WORD  biBitCount;
// // 	T_U8 *bmp,R,G,B,*Lab2BMP;
// // 	float X,Y,Z,L,a,b;

// // 	T_U32 line_byte;
// // 	T_U16 i,j;

// // 	BITMAPFILEHEADER bf;
// // 	BITMAPINFOHEADER bi;

// // 	FILE *Lab2BMP_fp = fopen("Lab2BMP.bmp","wb");

// // 	if(NULL == Lab2BMP_fp)
// // 	{
// // 		printf("Can't open Lab2BMP.bmp\n");
// // 		return -1;
// // 	}

// // 	memset(&bf, 0, sizeof(bf));
// // 	memset(&bi, 0, sizeof(bi));

// // 	bmp = bmp_img;
// // 	memcpy(&bf,bmp,14);
// // 	memcpy(&bi,&bmp[14],40);

// // 	height = bi.biHeight;
// // 	width  = bi.biWidth;
// // 	biBitCount = bi.biBitCount;//每一个像素由24 bits表示，即RGB分量每一个分量用8 bits表示
// // 	line_byte = WIDTHBYTES(width*bi.biBitCount);

// // 	fwrite(&bf,sizeof(BITMAPFILEHEADER),1,Lab2BMP_fp);
// // 	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,Lab2BMP_fp);

// // 	Lab2BMP = (T_U8*)malloc(height*line_byte);
// // 	if (Lab2BMP == NULL)
// // 	{
// // 		printf("Can't malloc LabBMP image.\n");
// // 		return 0;
// // 	}
// // 	memset(Lab2BMP,0,height*line_byte);

// // 	for (i = 0; i <height;i++)
// // 	{
// // 		for (j = 0;j < width;j++)
// // 		{
// // 			index = i*line_byte+3*j;
// // 			L = lab_img[index];
// // 			a = lab_img[index+1];
// // 			b = lab_img[index+2];

// // 			Lab2XYZ(L,a,b,&X,&Y,&Z);
// // 			XYZ2RGB(X,Y,Z,&R,&G,&B);

// // 			Lab2BMP[index] = B;
// // 			Lab2BMP[index+1] = G;
// // 			Lab2BMP[index+2] = R;
// // 		}
// // 	}

// // 	fwrite(Lab2BMP, line_byte*height, 1, Lab2BMP_fp);
// // 	fclose(Lab2BMP_fp);
// //     free(Lab2BMP);

// // 	return 0;

// // }
/*
*参数：psrc
*	rgb565函数指针
*参数：pdst
*	rgb888函数指针
*/
// int rgb565_to_rgb888(const void *psrc, int w, int h, void *pdst)
// {
//     int srclinesize = UpAlign4(w * 2);
//     int dstlinesize = UpAlign4(w * 3);

//     const unsigned char *psrcline;
//     const unsigned short *psrcdot;
//     unsigned char *pdstline;
//     unsigned char *pdstdot;

//     int i, j;

//     if (!psrc || !pdst || w <= 0 || h <= 0)
//     {
//         DEBUG("rgb565_to_rgb888 : parameter error\n");
//         return -1;
//     }
//     psrcline = (const unsigned char *)psrc;
//     pdstline = (unsigned char *)pdst;
//     DEBUG("bushi 1\r\n");
//     for (i = 0; i < h; i++)
//     {
//         psrcdot = (const unsigned short *)psrcline;
//         pdstdot = pdstline;
//         for (j = 0; j < w; j++)
//         {
//             //565 b|g|r -> 888 r|g|b
//             // DEBUG("进行到x:%d,y:%d\r\n",i,j);
//             *pdstdot++ = (unsigned char)(((*psrcdot) >> 0) << 3);
//             *pdstdot++ = (unsigned char)(((*psrcdot) >> 5) << 2);
//             *pdstdot++ = (unsigned char)(((*psrcdot) >> 11) << 3);
//             psrcdot++;
//         }
//         psrcline += srclinesize;
//         pdstline += dstlinesize;
//     }

//     return 0;
// }
/*
*参数：psrc
*	rgb888函数指针
*参数：pdst
*	rgb565函数指针
*/
// int rgb888_to_rgb565(const void *psrc, int w, int h, void *pdst)
// {
//     int srclinesize = UpAlign4(w * 3);
//     int dstlinesize = UpAlign4(w * 2);

//     const unsigned char *psrcline;
//     const unsigned char *psrcdot;
//     unsigned char *pdstline;
//     unsigned short *pdstdot;

//     int i, j;

//     if (!psrc || !pdst || w <= 0 || h <= 0)
//     {
//         DEBUG("rgb888_to_rgb565 : parameter error\n");
//         return -1;
//     }

//     psrcline = (const unsigned char *)psrc;
//     pdstline = (unsigned char *)pdst;
//     for (i = 0; i < h; i++)
//     {
//         psrcdot = psrcline;
//         pdstdot = (unsigned short *)pdstline;
//         for (j = 0; j < w; j++)
//         {
//             //888 r|g|b -> 565 b|g|r
//             *pdstdot = (((psrcdot[0] >> 3) & 0x1F) << 0)     //r
//                        | (((psrcdot[1] >> 2) & 0x3F) << 5)   //g
//                        | (((psrcdot[2] >> 3) & 0x1F) << 11); //b
//             psrcdot += 3;
//             pdstdot++;
//         }
//         psrcline += srclinesize;
//         pdstline += dstlinesize;
//     }

//     return 0;
// }
