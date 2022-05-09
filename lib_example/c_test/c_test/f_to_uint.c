#include<stdio.h>

#define SIGN_BIT 0x80000000
#define EXP_BIT  0x7f800000
#define TAIL_BIT 0x007fffff

int main()
{
        float aFloat;//浮点数
        int aFix = 0;//定点数容器
        int tmp = 0;//浮点数容器
        int exp = 0;//指数大小
        int tail = 0;//尾数位容器
        scanf("%f",&aFloat);
        tmp = *((int*)&aFloat);//置定点数的符号位 
        aFix = tmp & SIGN_BIT;

        //置定点数的整数部分
        exp = ((tmp & EXP_BIT) >> 23) - 127;//指数值
        tail = ((tmp & TAIL_BIT) | 0x00800000);//尾数各位 
        aFix = aFix | ((tail >> (23-exp)) << 16);

        //置定点数的小数部分
        aFix = aFix | ((tail & ~(0xffffffff << (23-exp))) >> (7-exp));
        printf("%d\n",aFix);
}
