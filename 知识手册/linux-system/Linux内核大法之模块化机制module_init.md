 

内核中的模块化机制

    前言
    裸机代码
    内核代码
    模块化机制
        C语言热身
        module_init深入宏封装
        布局
        段属性定义
        段机制-在内核执行过程部分
        实例
    衍生

前言

本文从module_init展开讲述，Linux中的段机制，以及它带来的好处。在写驱动代码中最常用的就是module_init以及module_exit函数。在Linux系统启动的时候就会自己去执行用module_init定义的函数。
那么为什么Linux内核启动的过程会自动执行，它是如何实现的呢？这个是本文的重点。
裸机代码

在讨论内核驱动代码之前，先看下之前我们写单片机的代码中如何实现一个驱动的。

int main(void)
{ 
    delay_init();   //初始化延时函数
    LED_Init();     //初始化LED端口
    I2C_Init();		//i2c初始化
    LCD_Init();		//LCD初始化
    while(1)
    {
            GPIO_ResetBits(GPIOB,GPIO_Pin_5);  //LED0对应引脚GPIOB.5拉低，亮  等同LED0=0;
            GPIO_SetBits(GPIOE,GPIO_Pin_5);   //LED1对应引脚GPIOE.5拉高，灭 等同LED1=1;
            delay_ms(300);  //延时300ms
            GPIO_SetBits(GPIOB,GPIO_Pin_5);    //LED0对应引脚GPIOB.5拉高，灭  等同LED0=1;
            GPIO_ResetBits(GPIOE,GPIO_Pin_5); //LED1对应引脚GPIOE.5拉低，亮 等同LED1=0;
            delay_ms(300);                     //延时300ms
    }
} 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    
    由此可见每当我们需要添加一个外设比如ADC，DMA都需要在main函数中执行相对应的初始化代码。
    然后在其他地方实现他的功能代码，在代码量小的时候这样做没什么，也能控制，但在内核代码中这样实现就显得不那么明智。

内核代码

我们知道Linux很庞大，驱动-只是它启动过程的一小部分，还有很多如内存管理、调度、算法等等。那么每次需要添加一个设备的驱动就要在启动的main函数中加初始化，就很不灵活。同时内核系统庞大，多人协同不方便，修改内核启动代码容易出错。所以在内核中利用宏来处理我们所定义的初始化代码，然后在Linux内核启动过程中统一 一个地方来调用我们定义的初始化代码，做到灵活，统一可控的代码结构。
模块化机制

Linux中的模块化机制实际上利用宏，把我们的初始化代码统一声明，然后利用编译手段将初始化代码的函数地址统一收集归类，打包进kernel中，在系统启动的时候去特定的地址取出函数地址，并转化为函数指针对其进行调用，达到初始化函数的调用。
C语言热身

在讲段机制之前，先大致了解下基本原理。函数指针

int Func(void);   /*声明一个函数*/
int main(void)
{
	Func();//我们可以这样调用一个函数
}
--------------------------------------
int (*p) (void);  /*定义一个函数指针*/
p = Func;          /*将Func函数的首地址赋给指针变量p*/
p();//也可以通过函数指针来调用一个函数

typedef int (*initcall_t)(void); //利用typedef定义一个函数指针类型 ，Linux就是这样操作的
initcall_t q = Func;
q();

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13

module_init深入宏封装

先看module_init的定义-》/include/linux/module.h
__initcall的定义在-》\kernel\kernel\include\linux\init.h

#define module_init(x)	__initcall(x);
---------------------------------------
特定的宏表示了不一样的优先级，常见的module_init优先级在6
#define __initcall(fn) device_initcall(fn)
#define core_initcall(fn)		__define_initcall(fn, 1)
#define core_initcall_sync(fn)		__define_initcall(fn, 1s)
...
#define arch_initcall(fn)		__define_initcall(fn, 3)
#define arch_initcall_sync(fn)		__define_initcall(fn, 3s)
#define fs_initcall(fn)			__define_initcall(fn, 5)
#define device_initcall(fn)		__define_initcall(fn, 6)
....
#define late_initcall_sync(fn)		__define_initcall(fn, 7s)
-----------------------------------------
#define __define_initcall(fn, id) \
	static initcall_t __initcall_##fn##id __used \
	__attribute__((__section__(".initcall" #id ".init"))) = fn;
------------------------------------
	typedef int (*initcall_t)(void); //定义函数指针类型
	typedef void (*exitcall_t)(void);
	
	1
	2
	3
	4
	5
	6
	7
	8
	9
	10
	11
	12
	13
	14
	15
	16
	17
	18
	19
	20
	
	    可以看到各式各样的宏定义都调用了__define_initcall，后面带有数字，这个数组表达的是有限制，即越小的排在越前面，Linux内核执行的时候也越早执行。
	    __define_initcall也很好理解，定义了一个变量 而这个变量的值就是我们声明初始化函数的地址，而他带有段属性，属于某个段，在这里宏用到极致，随着段id也就是优先级的变化，段属性也在变。
	    而这个段属性_section__有什么用? 这个就会在Linux镜像布局中起作用，意味着我把这个定义的变量 放在哪。标红的变量是同一个变量。

布局

这是我网上摘来的图，里面详细描述了各个段的布局以作参考。
在这里插入图片描述
段属性定义

上面描述的宏只是定义了你的初始化函数指针变量的段属性。而真正确定变量布局是在编译过程，文件在-》arch\arm64\kernel\vmlinux.lds 32位的在arm目录下，注意这个文件是编译后才会生成的。

.init.data : {
...
__dtb_start = .; *(.dtb.init.rodata) __dtb_end = .; 
__setup_start = .; *(.init.setup) __setup_end = .;
  __initcall_start = .; KEEP(*(.initcallearly.init)) 
  __initcall0_start = .;
   KEEP(*(.initcall0.init)) KEEP(*(.initcall0s.init)) 
   __initcall1_start = .; KEEP(*(.initcall1.init)) KEEP(*(.initcall1s.init)) 
   __initcall2_start = .; KEEP(*(.initcall2.init)) KEEP(*(.initcall2s.init)) 
   __initcall3_start = .; KEEP(*(.initcall3.init)) KEEP(*(.initcall3s.init)) 
   __initcall4_start = .; KEEP(*(.initcall4.init)) KEEP(*(.initcall4s.init)) 
   __initcall5_start = .; KEEP(*(.initcall5.init)) KEEP(*(.initcall5s.init))
   __initcallrootfs_start = .; KEEP(*(.initcallrootfs.init)) KEEP(*(.initcallrootfss.init))
   __initcall6_start = .; KEEP(*(.initcall6.init)) KEEP(*(.initcall6s.init))
   __initcall7_start = .; KEEP(*(.initcall7.init)) KEEP(*(.initcall7s.init)) 
   __initcall_end = .;
   ....
 }

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18

段机制-在内核执行过程部分

在上述的lds中得到了各个段的其实地址。XXX_start。
.\kernel\init\main.c

int __init_or_module do_one_initcall(initcall_t fn)
{
....
ret = fn();
}
static void __init do_initcall_level(int level) //执行一个level的initcall
{
	initcall_t *fn;
	...
	for (fn = initcall_levels[level]; fn < initcall_levels[level+1]; fn++)
		do_one_initcall(*fn);
}
static initcall_t *initcall_levels[] __initdata = {
	__initcall0_start,//这里start的值就是lds文件中排列的那些编译链接后就得到了确定的地址。
	__initcall1_start,
	__initcall2_start,
	__initcall3_start,
	__initcall4_start,
	__initcall5_start,
	__initcall6_start,
	__initcall7_start,
	__initcall_end,
};
static void __init do_initcalls(void)
{
	int level;

	for (level = 0; level < ARRAY_SIZE(initcall_levels) - 1; level++)
		do_initcall_level(level);
}
do_initcalls《-do_basic_setup(void)《-kernel_init_freeable(void)《-kernel_init(void *unused)
static noinline void __ref rest_init(void)
{
kernel_thread(kernel_init, NULL, CLONE_FS);//init进程中执行的。
}
rest_init《-void __init start_kernel(void)

    
    
        start_kernel中代码很庞大，倒着梳理会比较好。即由内往外剥离梳理比较合适。
        start_kernel类似于裸机中的main函数，在启动汇编文件中执行调用。各大平台各种实现跳转。
        ./arch/arm/kernel/head-common.S:104: b start_kernel
        ./arch/m68k/68000/head.S:238: jsr start_kernel /* start Linux kernel /
        ./arch/m68k/coldfire/head.S:293: jsr start_kernel / start Linux kernel */

实例

举个例子，除了常用的module_init进行驱动的初始化，还有如下串口的初始化
在这里插入图片描述
文件系统的初始化：文件系统的初始化
衍生

当当uboot的bootargs匹配上时调用解析函数mtdpart_setup
在这里插入图片描述
查看__setup的定义，可以看到这个时候定义的是结构体变量，
在这里插入图片描述

static bool __init obsolete_checksetup(char *line)
{
	const struct obs_kernel_param *p;
	bool had_early_param = false;

	p = __setup_start;
	do {
		int n = strlen(p->str);
		if (parameqn(line, p->str, n)) {
			if (p->early) {
				/* Already done in parse_early_param?
				 * (Needs exact match on param part).
				 * Keep iterating, as we can have early
				 * params and __setups of same names 8( */
				if (line[n] == '\0' || line[n] == '=')
					had_early_param = true;
			} else if (!p->setup_func) {
				pr_warn("Parameter %s is obsolete, ignored\n",
					p->str);
				return true;
			} else if (p->setup_func(line + n))
				return true;
		}
		p++;
	} while (p < __setup_end);
	
	return had_early_param;
}

    
    
        调用关系obsolete_checksetup 《-unknown_bootoption《-start_kernel
        意思也就是说假如bootargs参数中有mtdparts字符是，就会调用mtdpart_setup函数。
————————————————
版权声明：本文为CSDN博主「kalaneryok」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_41884251/article/details/108724568

https://blog.csdn.net/lu_embedded/article/details/51432616
