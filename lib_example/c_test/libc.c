char* my_strcpy(char* des, const char* sor)
{
 assert(des && sor);
 这个库函数上面讲了
 char* ret = des;
 while (*des++ = *sor++);
 return ret;
}
//求字符串长度的函数
size_t my_strlen(const char* str)
{
 const char* temp = str;
 保证temp的类型和str的类型一致
 while (*++str);
 为了找到\0
 return str - temp;
 指针相减得到它们直之间的元素的个数
}
//字符串比较函数
//对于返回类型Int,如果string1的字符比string2的字符大就返回小于0的数，相等就返回0，大于就返回大于0的数。
int my_strcmp(const char* str1, const char* str2)`
{
 while (*str1==*str2&&*str1&&*str2)`找到不相同的字符
 {
  str1++;
  str2++;
 }
//出来循环之后有两种情况:
//1字符不相等，
//2.至少有一个字符串访问到了\0.
//不管是哪一种情况，直接相减，即可判断谁大谁小还是相等
 return *str1 - *str2;
}
char* my_strstr(const char* str1, const char* str2)
{
 const char* temp1 = str1;
 const char* temp2 = str2;
 if (*str2=='\0')//字符串长度为0直接返回str1
  return (char*)str1;
 while (*str2)
 {
  while (*str1 != *str2 )
  {
   str1++;
   if (*str1 == '\0')
   return NULL;
  }
  //找到了相同的字符了
  temp1 = str1;
  while (*str1 == *str2)
  {
   str1++;
   str2++;
   if (*str2 == '\0')
    return (char*)temp1;
  }//没有找到，从新进行标记查找
  str2 = temp2;
  str1 = temp1 + 1;
 }
}


char* my_strcat(char* dest, const char* sou)
{
 char* ret = dest;
 while (*dest)找到\0
 {
  dest++;
 }
 while (*dest++ = *sou++)追加
  ;
 return ret;
}


void* my_memcpy(void* dest, const void* src, int count)
{
 void* ret = dest;
 while (count--)
 {
  *(char*)dest = *(char*)src;
  dest = (char*)dest + 1;
  src = (char*)src + 1;
 }
 return ret;
}



void* my_memmove(void* dest, const void* src, int count)
{
 void* ret = dest;
 if (src < dest)
 {
  while (count--)
   *((char*)dest+count) = *((char*)src + count);
 }
 else
 {
  while (count--)
  {
   *(char*)dest = *(char*)src;
   dest = (char*)dest + 1;
   src = (char*)src + 1;
  }
 }
 return ret;
}


int my_atoi(const char* temp)
{
 int sum = 0;
 int a = 1;
 int b = 1;
 while (*temp)
 {
  //开始时排除+ - 和空格
  while (*temp == ' ')
  {
   temp++;
  }
  if (*temp == '-' && a == 1)
  {
   b = -1;
   temp++;
  }
  else if (*temp == '+' && a == 1)
  {
   b = 1;
   temp++;
  }
  //找到数字字符
  while (*temp >= '0' && *temp <= '9')
  {
   if (a == 0)
   {
    sum *= 10;
   }
   sum += *temp - '0';
   a = 0;
   temp++;
  }
  //此处一定不是数字字符
  break;
 }
 return b * sum;
}



char* my_strncat(char* des,const char* sor,int n )
{
 char* ret = des;
 while (*des)
  des++;
 while (n&&*sor)
 {
  *des++ = *sor++;
  n--;
 }
 *des = '\0';
 return ret;
}

char* my_strncpy(char* des, const char* sor, int n)
{
 char* ret = des;
 while (n)
 {
  *des++ = *sor++;
  n--;
  if (*(sor - 1) == '\0')
   break;
 }

 return ret;
}


// 利用 neon 寄存器快速内存复制
#ifdef  __ARM__
static void neon_memcpy(volatile unsigned char *dst, volatile unsigned char *src, int sz)
{
    if (sz & 63)
        sz = (sz & -64) + 64;
    asm volatile (
    "NEONCopyPLD: \n"
            " VLDM %[src]!,{d0-d7} \n"
            " VSTM %[dst]!,{d0-d7} \n"
            " SUBS %[sz],%[sz],#0x40 \n"
            " BGT NEONCopyPLD \n"
    : [dst]"+r"(dst), [src]"+r"(src), [sz]"+r"(sz) : : "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "cc", "memory");
}
#endif




// https://mp.weixin.qq.com/s/vfNLIya2h39lieEOPdWDmA







