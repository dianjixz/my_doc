#include <stdio.h>
#include <stdint.h>
#include <iostream>

 typedef enum
{
    LIBMAIX_IMAGE_MODE_INVALID = 0,
    LIBMAIX_IMAGE_MODE_BINARY,
    LIBMAIX_IMAGE_MODE_GRAY  ,
    LIBMAIX_IMAGE_MODE_RGB888,
    LIBMAIX_IMAGE_MODE_RGB565,
    LIBMAIX_IMAGE_MODE_RGBA8888,
    LIBMAIX_IMAGE_MODE_YUV420SP_NV21,
    LIBMAIX_IMAGE_MODE_YUV422_YUYV,
    LIBMAIX_IMAGE_MODE_BGR888,
}libmaix_image_mode_t;
std::string mk[9] = {
    "LIBMAIX_IMAGE_MODE_INVALID",
    "LIBMAIX_IMAGE_MODE_BINARY",
    "LIBMAIX_IMAGE_MODE_GRAY",
    "LIBMAIX_IMAGE_MODE_RGB888",
    "LIBMAIX_IMAGE_MODE_RGB565",
    "LIBMAIX_IMAGE_MODE_RGBA8888",
    "LIBMAIX_IMAGE_MODE_YUV420SP_NV21",
    "LIBMAIX_IMAGE_MODE_YUV422_YUYV",
    "LIBMAIX_IMAGE_MODE_BGR888"};



int main()
{

uint16_t ma = 0;
for(int i = 0;i < 9;i++)
{
    for(int j = 0;j < 9; j++)
    {
        ma = 0;
        ma = i << 8;
        ma |= j;
        std::cout << mk[i] << " -> " << mk[j] << "   :";
        printf(" \t%d \r\n",ma);
    }
	
}





	return 0;
}
