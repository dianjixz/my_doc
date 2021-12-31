 



# 利用gcc的预定义宏和内置函数(Built-in Functions)实现大小端判断和大小端数据转换





对于涉及跨平台开发的项目,就可能会遇到数据大小端的问题，其实就是一个数字在内存中的字节序的问题，判断当前系统是大小端有现成的例子，自己实现转换代码也非常方便,网上有好多不用多说。
但我是个懒人，就算是这么简单的代码，有现成的就不想自己写。
今天要说的是gcc本身已经提供了大小端的判断和数据转换的函数，真的没必要自己写。

gcc预定义宏判断大小端(Endian)
先看看这个文章:查看gcc的默认宏定义命令(点击打开链接)
这个文章告诉你如何获取gcc默认的宏定义,在这些宏定义中就有大小端模式的预定义宏__BYTE_ORDER__
当__BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__时是小端模式
当__BYTE_ORDER__==__ORDER_BIG_ENDIAN__时是小端模式
__BYTE_ORDER__,__ORDER_LITTLE_ENDIAN__,__ORDER_BIG_ENDIAN__都是gcc预定义的宏，在代码中可以直接使用。

gcc内置函数进行大小端转换
gcc提供了不少有用的内置函数(Built-in Functions),这些函数说明可以在gcc的网站上找到

6.58 Other Built-in Functions Provided by GCC(点击打开链接)
这个页面最后面三个函数就是我们需要的:
— Built-in Function: uint16_t __builtin_bswap16 (uint16_t x)
Returns x with the order of the bytes reversed; for example, 0xaabb becomes 0xbbaa. Byte here always means exactly 8 bits.//返回x的反序字节,例如：0xaabb变成0xbbaa,下面类同。
— Built-in Function: uint32_t __builtin_bswap32 (uint32_t x)
Similar to __builtin_bswap16, except the argument and return types are 32 bit.
— Built-in Function: uint64_t __builtin_bswap64 (uint64_t x)
Similar to __builtin_bswap32, except the argument and return types are 64 bit.

_bswap16,_bswap32,_bswap64三个函数分别提供了16位，32位，64位数字的字节反转功能，正好可以用来实现16,32,64位数字的大小端转换。

样例代码
如下写代码中用到了上面提到的函数，对32位的float和64位的double在大端模式下进行转换

#if __BYTE_ORDER__ ==__ORDER_BIG_ENDIAN__
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
————————————————
版权声明：本文为CSDN博主「10km」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/10km/article/details/49021499
