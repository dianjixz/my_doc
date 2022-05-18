


GCC提供了一系列的builtin函数，可以实现一些简单快捷的功能来方便程序编写，另外，很多builtin函数可用来优化编译结果。这些函数以“__builtin_”作为函数名前缀。
很多C标准库函数都有与之对应的GCC builtin函数，例如strcpy()有对应的__builtin_strcpy()内建函数。

下面就介绍一些builtin函数及其作用：

__builtin_ffs(x)：返回x中最后一个为1的位是从后向前的第几位，如__builtin_ffs(0x789)=1, __builtin_ffs(0x78c)=3。于是，__builtin_ffs(x) - 1就是x中最后一个为1的位的位置。



__builtin_popcount(x)：x中1的个数。



__builtin_ctz(x)：x末尾0的个数。x=0时结果未定义。



__builtin_clz(x)：x前导0的个数。x=0时结果未定义。

上面的宏中x都是unsigned int型的，如果传入signed或者是char型，会被强制转换成unsigned int。



__builtin_parity(x)：x中1的奇偶性。



__builtin_return_address(n)：当前函数的第n级调用者的地址，用的最多的就是__builtin_return_address(0)，即获得当前函数的调用者的地址。注意，该函数实现是体系结构相关的，有些体系结构只实现了n=0的返回结果。



uint16_t __builtin_bswap16 (uint16_t x)
uint32_t __builtin_bswap32 (uint32_t x)：按字节翻转x，返回翻转后的结果。



__builtin_prefetch (const void *addr, ...) ：它通过对数据手工预取的方法，在使用地址addr的值之前就将其放到cache中，减少了读取延迟，从而提高了性能，但该函数也需要 CPU 的支持。该函数可接受三个参数，第一个参数addr是要预取的数据的地址，第二个参数可设置为0或1（1表示我对地址addr要进行写操作，0表示要进行读操作），第三个参数可取0-3（0表示不用关心时间局部性，取完addr的值之后便不用留在cache中，而1、2、3表示时间局部性逐渐增强）。

__builtin_constant_p (exp) ：判断exp是否在编译时就可以确定其为常量，如果exp为常量，该函数返回1，否则返回0。如果exp为常量，可以在代码中做一些优化来减少处理exp的复杂度。

__builtin_types_compatible_p(type1, type2) ：判断type1和type2是否是相同的数据类型，相同返回1，否则返回0。该函数不区分const/volatile这样的修饰符，即int和const int被认为是相同的类型。
#define foo(x) 
({
typeof(x) tmp = (x); \
if(__builtin_types_compatible_p(typeof(x), int)) \
//do something... \
else   \
//do something... \
tmp;
})

__builtin_expect (long exp, long c) ：用来引导gcc进行条件分支预测。在一条指令执行时，由于流水线的作用，CPU可以完成下一条指令的取指，这样可以提高CPU的利用率。在执行一条条件分支指令时，CPU也会预取下一条执行，但是如果条件分支跳转到了其他指令，那CPU预取的下一条指令就没用了，这样就降低了流水线的效率。内核中的likely()和unlikely()就是通过__builtin_expect来实现的。
__builtin_expect (long exp, long c)函数可以优化程序编译后的指令序列，使指令尽可能的顺序执行，从而提高CPU预取指令的正确率。该函数的第二个参数c可取0和1，
例如：
if (__builtin_expect (x, 0))
            foo ();
表示x的值大部分情况下可能为0，因此foo()函数得到执行的机会比较少。gcc就不必将foo()函数的汇编指令紧挨着if条件跳转指令。
由于第二个参数只能取整数，所以如果要判断指针或字符串，可以像下面这样写：
if (__builtin_expect (ptr != NULL, 1))
            foo (*ptr);
表示ptr一般不会为NULL，所以foo函数得到执行的概率较大，gcc会将foo函数的汇编指令放在挨着if跳转执行的位置。

void *__builtin_alloca (size_t size):
    该__builtin_alloca函数必须在块范围内调用。该函数在调用函数的堆栈上分配一个对象大小字节。由于__builtin_alloca函数不验证它的参数，它的调用者有责任确保参数不会导致它超过堆栈大小限制。


void *__builtin_alloca_with_align (size_t 大小，size_t 对齐方式):
    该__builtin_alloca_with_align函数必须在块范围内调用。该函数在调用函数的堆栈上分配一个对象大小字节。由于该__builtin_alloca_with_align函数不验证其 大小参数，因此调用者有责任确保该参数不会导致它超出堆栈大小限制。


int __builtin_types_compatible_p ( type1 , type2 ):
    您可以使用内置函数__builtin_types_compatible_p来确定两种类型是否相同。

类型 __builtin_call_with_static_chain ( call_exp , pointer_exp ):
    call_exp表达式必须是函数调用， pointer_exp表达式必须是指针。pointer_exp被传递给目标静态链位置中的 函数调用。builtin 的结果是函数调用的结果。