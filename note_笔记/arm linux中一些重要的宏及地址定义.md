[[转载](http://home.eeworld.com.cn/my/space-uid-162102-blogid-64381.html)]于http://biancheng.dnbcw.info/linux/352414.html

**1）TEXT_OFFSET 内核在RAM中的起始位置相对于RAM起始地址偏移。值为0x00008000
**

1. **./arch/arm/Makefile**

   ~~~ makefile
   textofs-y := 0x00008000
   textofs-$(CONFIG_ARCH_CLPS711X) := 0x00028000
   # We don't want the htc bootloader to corrupt kernel during resume
   textofs-$(CONFIG_PM_H1940) := 0x00108000
   # SA1111 DMA bug: we don't want the kernel to live in precious DMA-able memory
   ifeq ($(CONFIG_ARCH_SA1100),y)
   textofs-$(CONFIG_SA1111) := 0x00208000
   endif
   
   # The byte offset of the kernel image in RAM from the start of RAM.
   TEXT_OFFSET := $(textofs-y)
   ~~~

**2）PAGE_OFFSE  内核镜像起始虚拟地址。值为0xC0000000 **

      1. **arch/arm/Kconfig**

~~~ makefile
config PAGE_OFFSET ----这个就是CONFIG_PAGE_OFFSET宏，默认值为0xC0000000
hex
default 0x40000000 if VMSPLIT_1G
default 0x80000000 if VMSPLIT_2G
default 0xC0000000
~~~

**./arch/arm/include/asm/memory.h** 

~~~ c
#define PAGE_OFFSET UL(CONFIG_PAGE_OFFSET)
~~~

**3）PHYS_OFFSET RAM启始物理地址，对应于DDR的物理地址。需要在mach-xxxx/include/mach/memory.h文件中定义这个宏。**

对于2410来说值为0x30000000，RAM接在片选6上
arch/arm/mach-s3c2410/include/mach/memory.h
\#define PHYS_OFFSET  UL(0x30000000)

**4）KERNEL_RAM_VADDR 内核在RAM中的虚拟地址。值为0xC0008000
5）KERNEL_RAM_PADDR 内核在RAM中的物理地址。值为0x30008000**

arch/arm/kernel/head.S

~~~ c
#define KERNEL_RAM_VADDR        (PAGE_OFFSET + TEXT_OFFSET)
#define KERNEL_RAM_PADDR        (PHYS_OFFSET + TEXT_OFFSET)
~~~

**6）swapper_pg_dir 初始页表虚拟地址，值为0xC0004000，**

一般分配KERNEL_RAM_VADDR地址下16K作为页表，因此，我们必须确保正确设置KERNEL_RAM_VADDR。
arch/arm/kernel/head.S

~~~ c
.globl  swapper_pg_dir
.equ    swapper_pg_dir, KERNEL_RAM_VADDR - 0x4000
~~~

**7）VMALLOC_START 0xc4800000
8）VMALLOC_END  0xe0000000
**
**9）MODULES_VADDR 0xbf000000
10）MODULES_END  0xc0000000**

**arch/arm/include/asm/memory.h**

~~~ c
#ifndef CONFIG_THUMB2_KERNEL
#define MODULES_VADDR           (PAGE_OFFSET - 16*1024*1024)
#else
/* smaller range for Thumb-2 symbols relocation (2^24)*/
#define MODULES_VADDR           (PAGE_OFFSET - 8*1024*1024)
#endif

#if TASK_SIZE > MODULES_VADDR
#error Top of user space clashes with start of module space
#endif

/*
 * The highmem pkmap virtual space shares the end of the module area.
 */
#ifdef CONFIG_HIGHMEM
#define MODULES_END             (PAGE_OFFSET - PMD_SIZE)
#else
#define MODULES_END             (PAGE_OFFSET)
#endif
~~~

**11)PKMAP_BASE**   

arch/arm/include/asm/highmem.h:

~~~ c
#define PKMAP_BASE           (PAGE_OFFSET - PMD_SIZE)
~~~

**12)TASK_SIZE**

./arch/arm/include/asm/memory.h 

~~~ c
#define PAGE_OFFSET             UL(CONFIG_PAGE_OFFSET)
#define TASK_SIZE               (UL(CONFIG_PAGE_OFFSET) - UL(0x01000000))
#define TASK_UNMAPPED_BASE      (UL(CONFIG_PAGE_OFFSET) / 3)
~~~

**13）一些不同地址之间转换的宏**

./arch/arm/include/asm/memory.h 

~~~ c
/*
* Physical vs virtual RAM address space conversion.  These are
* private definitions which should NOT be used outside memory.h
* files.  Use virt_to_phys/phys_to_virt/__pa/__va instead.
 */
#ifndef __virt_to_phys
#define __virt_to_phys(x)       ((x) - PAGE_OFFSET + PHYS_OFFSET)//物理地址和虚拟地址
#define __phys_to_virt(x)       ((x) - PHYS_OFFSET + PAGE_OFFSET)//之间转换的宏
#endif

/*
 * Convert a physical address to a Page Frame Number and back
 */
#define __phys_to_pfn(paddr)    ((paddr) >> PAGE_SHIFT) //物理地址和页帧号之间
#define __pfn_to_phys(pfn)      ((pfn) << PAGE_SHIFT)   //转换的宏

/*
* Convert a page to/from a physical address
*/
#define page_to_phys(page)      (__pfn_to_phys(page_to_pfn(page)))//页地址和物理地址
#define phys_to_page(phys)      (pfn_to_page(__phys_to_pfn(phys)))//之间的转换宏依赖于内存模型。

/*
 * Drivers should NOT use these either.
 */
#define __pa(x)                 __virt_to_phys((unsigned long)(x))
#define __va(x)                 ((void *)__phys_to_virt((unsigned long)(x)))
#define pfn_to_kaddr(pfn)       __va((pfn) << PAGE_SHIFT)


/*
* Virtual <-> DMA view memory address translations
* Again, these are *only* valid on the kernel direct mapped RAM
* memory.  Use of these is *deprecated* (and that doesn't mean
* use the __ prefixed forms instead.)  See dma-mapping.h.
*/
#ifndef __virt_to_bus
#define __virt_to_bus   __virt_to_phys
#define __bus_to_virt   __phys_to_virt
#define __pfn_to_bus(x) __pfn_to_phys(x)
#define __bus_to_pfn(x) __phys_to_pfn(x)
#endif
~~~

