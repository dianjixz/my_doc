1. 原因

    这里只考虑有 MMU 的芯片，Linux 为了实现进程虚拟地址空间，在启用 MMU 后，在内核中操作的都是虚拟地址，内核访问不到物理地址。
    如果在驱动里直接访问物理地址，等于访问了一个非法地址，会导致内核崩溃，下面会有一个相关的小实验。
    通过 ioremap() 将物理地址映射为虚拟地址后，内核就能通过 ioremap() 返回的虚拟地址，以 虚拟地址->mmu页表映射-> 物理地址 的形式正确地访问到物理地址了。
    ARM Linux 引入设备树特性后，一些支持设备树的设备驱动不再使用直接 ioremap()，改用 drivers/of/address.c/of_iomap()，of_iomap() 的内部仍然会调用 ioremap()，例如：
~~~
clk-rk3288.c (drivers\clk\rockchip)

static void rk3288_clk_init(struct device_node *np) {
    rk3288_cru_base = of_iomap(np， 0);
    [...]
}
~~~
2. ioremap() 实验

实验环境：
~~~
    Linux-4.14 + Allwinner/H3。
~~~
实验代码：
~~~
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm/io.h>

#define USE_IOREMAP

#define H3_GPIO_BASE (0x01C20800)

static volatile unsigned long *gpio_regs = NULL;

static int __init ioremap_mod_init(void)
{

    int i = 0;
    printk(KERN_INFO "ioremap_mod init\n");

#ifdef USE_IOREMAP
    gpio_regs = (volatile unsigned long *)ioremap(H3_GPIO_BASE， 1024);
#else
    gpio_regs = (volatile unsigned long *)H3_GPIO_BASE;
#endif

    for (i=0; i<3; i++)
        printk(KERN_INFO "reg[%d] = %lx\n"， i， gpio_regs[i]);

    return 0;
}
module_init(ioremap_mod_init);

static void __exit ioremap_mod_exit(void)
{
    printk(KERN_INFO "ioremap_mod exit\n ");

#ifdef USE_IOREMAP
    iounmap(gpio_regs);
#endif 
}

module_exit(ioremap_mod_exit);

MODULE_AUTHOR("es-hacker");
MODULE_LICENSE("GPL v2");
~~~
实验结果：

使用了 ioremap()
~~~
$ insmod ioremap

ioremap_mod init
reg[0] = 71227722
reg[1] = 33322177
reg[2] = 773373
~~~
未使用 ioremap():
~~~
$ insmod ioremap_mod.ko

Unable to handle kernel paging request at virtual address 01c20800
pgd = c9ece7c0
[01c20800] *pgd=6ddd7003， *pmd=00000000
Internal error: Oops: 206 [#1] SMP ARM
CPU: 1 PID: 1253 Comm: insmod Tainted: G           O    4.14.111 #116
Hardware name: sun8i
task: ef15d140 task.stack: edc50000
PC is at ioremap_mod_init+0x3c/0x1000 [ioremap_mod]
LR is at ioremap_mod_init+0x14/0x1000 [ioremap_mod]
pc : [<bf5d903c>]    lr : [<bf5d9014>]    psr: 600e0013
sp : edc51df8  ip : 00000007  fp : 118fa95c
r10: 00000001  r9 : ee7056c0  r8 : bf5d6048
r7 : bf5d6000  r6 : 00000000  r5 : bf5d6200  r4 : 00000000
r3 : 01c20800  r2 : 01c20800  r1 : 00000000  r0 : bf5d5048
Flags: nZCv  IRQs on  FIQs on  Mode SVC_32  ISA ARM  Segment user
Control: 30c5387d  Table: 49ece7c0  DAC: b106c794
Process insmod (pid: 1253， stack limit = 0xedc50210)

[<bf5d903c>] (ioremap_mod_init [ioremap_mod]) from [<c0201a70>] (do_one_initcall+0x40/0x16c)
[<c0201a70>] (do_one_initcall) from [<c02b20c8>] (do_init_module+0x60/0x1f0)
[<c02b20c8>] (do_init_module) from [<c02b1214>] (load_module+0x1b48/0x2250)
[<c02b1214>] (load_module) from [<c02b1ad8>] (SyS_finit_module+0x8c/0x9c)
[<c02b1ad8>] (SyS_finit_module) from [<c0221f80>] (ret_fast_syscall+0x0/0x4c)
Code: e5953000 e3050048 e1a01004 e34b0f5d (e7932104) 
---[ end trace 928c64a33a054308 ]---
Segmentation fault
~~~
3. ioremap() 的实现内幕

ioremap() 的实现内幕会涉及到比较多的内存管理的知识，这里我们抛开代码细节简单了解一下原理就好。

    ioremap() 将 vmalloc 区的某段虚拟内存块映射到 io memory，其实现原理与vmalloc() 类似，都是通过在 vmalloc 区分配虚拟地址块，然后修改内核页表的方式将其映射到设备的 I/O 地址空间。
    与 vmalloc() 不同的是，ioremap() 并不需要通过伙伴系统去分配物理页，因为ioremap() 要映射的目标地址是 io memory，不是物理内存 (RAM)。

函数调用流程：

总结一下：

    相关检查;
    分配一个 vm_struct 结构体，内核在管理虚拟内存中的 vmalloc 区时，内核必须跟踪哪些子区域被使用、哪些是空闲的，对应的数据结构就是 vm_strcut。
    初始化 vm_struct;
    建立页表;

4. 相关参考

    深入理解 Linux 内核 / 8.3.2
    深入 Linux 内核架构 / 3.5.7
    深入理解Linux设备驱动程序内核机制 / 3.5.3
    https://blog.csdn.net/njuitjf/article/details/40745227

5. 参与讨论

关于嵌入式软件软件(应用/驱动)开发，大家都遇到过哪些经典的面试题呢？快抛出来一起讨论吧~

你和我各有一个苹果，如果我们交换苹果的话，我们还是只有一个苹果。但当你和我各有一个想法，我们交换想法的话，我们就都有两个想法了。

对 嵌入式系统 (Linux、RTOS、OpenWrt、Android) 和 开源软件 感兴趣，关注公众号：嵌入式Hacker。



Linux I/O 映射(ioremap)和writel/readl

在裸奔代码中，如果要控制gpio，直接控制gpio寄存器地址即可；

在linux系统中，所有操作的地址都是虚拟地址，都是由linux内核去管理，所以需要将物理地址转换成内核可识别的虚拟地址。

~~~
1、ioremap 和 iounmap

// 功能：将物理地址映射为虚拟地址

// 参数1：需要映射的物理地址

// 参数2：需要映射的地址长度

// 返回值：映射后的虚拟地址（例：void*reg_base = ioremap(0x12345678, 4);）

void *ioremap(cookie, size);


// 功能：取消映射

// 参数：需要取消的虚拟地址

void iounmap(cookie);


2、虚拟地址操作

// 功能：将虚拟地址中的值读出来

// 参数：需要读取的虚拟地址

__raw_readl(addr)；        等同于      readl(addr);


 

// 功能：将数据写入虚拟地址

// 参数1：数据

// 参数2：需要写入的虚拟地址

__raw_writel(data, addr)；        等同于      writel(data, addr);


// 从虚拟地址读取4个字节数据

unsigned int readl(unsigned int addr);

// 往虚拟地址写入4个字节数据

void writel(unsigned int data, unsigned int addr);


// 从虚拟地址读取2个字节数据

unsigned short readw(unsigned int addr);

 

// 往虚拟地址写入2个字节数据

void writew(unsigned short data, unsigned int addr);


// 从虚拟地址读取1个字节数据

unsigned char readb(unsigned int addr);

 

// 往虚拟地址写入1个字节数据

void writeb(unsigned char data, unsigned int addr);


 

void led_on(void)
{
	void *reg_base;
	unsigned int temp; 
	reg_base = ioremap(0x12345678, 4);

	temp = __raw_readl(reg_base); 
	temp &= ~(0xff<<12);
	temp |= (0x11<<12);

	__raw_writel(temp, reg_base);
}

~~~