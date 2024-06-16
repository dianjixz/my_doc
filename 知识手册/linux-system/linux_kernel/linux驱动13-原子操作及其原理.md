# 原子操作原理分析

## 1. 概念

原子操作是指不被打断的操作，即它是最小的执行单位。最简单的原子操作就是一条条的汇编指令(不包括一些伪指令，伪指令会被汇编器解释成多条汇编指令)。在 linux 中原子操作对应的数据结构为 atomic_t，定义如下：

```
typedef struct {
	int counter;} atomic_t;
```

本质上就是一个整型变量，之所以定义这么一个数据类型，是为了让原子操作函数只接受 atomic_t 类型的操作数，如果传入的不是 atomic_t 类型数据，在程序编译阶段就不会通过；另一个原因就是确保编译器不会对相应的值进行访问优化，确保对它的访问都是对内存的访问，而不是对寄存器的访问。

## 2. 赋值操作

ARM 处理器有直接对内存地址进行赋值的指令(STR)。

```
#define atomic_set(v,i)	(((v)->counter) = (i))
```

## 3. 读操作

用 volatile 来防止编译器对变量访问的优化，确保是对内存的访问，而不是对寄存器的访问。

```
#define atomic_read(v)	(*(volatile int *)&(v)->counter)
```

## 4. 加操作

使用独占指令完成累加操作。

```
static inline void atomic_add(int i, atomic_t *v){
	unsigned long tmp;
	int result;
	// 使用独占指令读取，然后执行加操作，独占写失败时就重新执行	__asm__ __volatile__("@ atomic_add\n""1:	ldrex	%0, [%3]\n""	add	%0, %0, %4\n""	strex	%1, %0, [%3]\n""	teq	%1, #0\n""	bne	1b"
	: "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)
	: "r" (&v->counter), "Ir" (i)
	: "cc");}
```

## 5. 减操作

对比*加操作*和*减操作*的代码可以看出，它们非常的相似，其实不同的地方就一句，所以现在最新的内核源码中已经使用宏定义 ATOMIC_OP(op, c_op, asm_op) 来重写了这部分代码。

```
static inline void atomic_sub(int i, atomic_t *v){
	unsigned long tmp;
	int result;
	// 使用独占指令读取，然后执行减操作，独占写失败时就重新执行	__asm__ __volatile__("@ atomic_sub\n""1:	ldrex	%0, [%3]\n""	sub	%0, %0, %4\n""	strex	%1, %0, [%3]\n""	teq	%1, #0\n""	bne	1b"
	: "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)
	: "r" (&v->counter), "Ir" (i)
	: "cc");}
```

## 6. 其他操作

类似的原子操作函数还有一些，比如 atomic_XXX_return、atomic_cmpxchg、atomic_clear_mask，以及在此基础上实现的 atomic_inc、atomic_dec、atomic_XXX_and_test、atomic_XXX_return等。以上代码都是针对 SMP 处理器的实现方式，针对非 SMP 处理器，由于不存在其他核心的抢占，所以只需要防止其他进程抢占即可实现原子操作，例如*加操作*：

```
static inline int atomic_sub_return(int i, atomic_t *v){
	unsigned long flags;
	int val;
	// 通过关闭中断防止其他进程打断代码的执行	raw_local_irq_save(flags);
	val = v->counter;
	v->counter = val -= i;
	// 恢复中断原始的状态	raw_local_irq_restore(flags);

	return val;}
```

\7. ldrex 和 strex 使用说明



在Armv6开始支持多核，通过ldrex与strex指令来保证数据操作的原子性，比如自旋锁的上锁操作、原子变量操作等。在Armv6之前，都是单核，为保证数据的原子性，需要进行关中断操作。对于多核平台，关中断操作只能关闭本核中断，要想对数据进行原子操作，必须借助ldrex指令与strex。



独占加载和存储寄存器。

**语法**

LDREX{*cond*} *Rt*, [*Rn* {, #*offset*}]

STREX{*cond*} *Rd*, *Rt*, [*Rn* {, #*offset*}]

LDREXB{*cond*} *Rt*, [*Rn*]     字节加载

STREXB{*cond*} *Rd*, *Rt*, [*Rn*]   字节存储

LDREXH{*cond*} *Rt*, [*Rn*]    半字加载

STREXH{*cond*} *Rd*, *Rt*, [*Rn*]   半字存储

LDREXD{*cond*} *Rt*, *Rt2*, [*Rn*]   双字加载

STREXD{*cond*} *Rd*, *Rt*, *Rt2*, [*Rn*]  双字存储

其中：

*cond*

是一个可选的条件代码（请参阅*条件执行*）。

*Rd*

是存放返回状态的目标寄存器。

*Rt*

是要加载或存储的寄存器。

*Rt2*

为进行双字加载或存储时要用到的第二个寄存器。

*Rn*

是内存地址所基于的寄存器。

*offset*

为应用于 *Rn* 中的值的可选偏移量。*offset* 只可用于 Thumb-2 指令中。如果省略*offset*，则认为偏移量为 0。

**LDREX**

LDREX 可从内存加载数据。

- 如果物理地址有共享 TLB 属性，则 LDREX 会将该物理地址标记为由当前处理器独占访问，并且会清除该处理器对其他任何物理地址的任何独占访问标记。
- 否则，会标记：执行处理器已经标记了一个物理地址，但访问尚未完毕。

**STREX**

STREX 可在一定条件下向内存存储数据。条件具体如下：

- 如果物理地址没有共享 TLB 属性，且执行处理器有一个已标记但尚未访问完毕的物理地址，那么将**会**进行存储，清除该标记，并在*Rd* 中返回值 0。
- 如果物理地址没有共享 TLB 属性，且执行处理器也没有已标记但尚未访问完毕的物理地址，那么将**不会**进行存储，而会在*Rd* 中返回值 1。
- 如果物理地址有共享 TLB 属性，且已被标记为由执行处理器独占访问，那么将进行存储，清除该标记，并在*Rd* 中返回值 0。
- 如果物理地址有共享 TLB 属性，但没有标记为由执行处理器独占访问，那么**不会**进行存储，且会在*Rd* 中返回值 1。

**限制**

r15 不可用于 *Rd*、*Rt*、*Rt2* 或 *Rn* 中的任何一个。

对于 STREX，*Rd* 一定不能与*Rt*、*Rt2* 或*Rn* 为同一寄存器。

对于 ARM 指令：

- *Rt* 必须是一个编号为偶数的寄存器，且不能为 r14
- *Rt2* 必须为*R(t+1)*
- 不允许使用 *offset*。

对于 Thumb 指令：

- r13 不可用于 *Rd*、*Rt* 或*Rt2* 中的任何一个
- 对于 LDREXD，*Rt* 和*Rt2* 不可为同一个寄存器
- *offset* 的值可为 0-1020 范围内 4 的任何倍数。

**用法**

利用 LDREX 和 STREX 可在多个处理器和共享内存系统之前实现进程间通信。

出于性能方面的考虑，请将相应 LDREX 指令和 STREX 指令间的指令数控制到最少。

**Note**

STREX 指令中所用的地址必须要与近期执行次数最多的 LDREX 指令所用的地址相同。如果使用不同的地址，则STREX 指令的执行结果将不可预知。

**体系结构**

ARM LDREX 和 STREX 可用于 ARMv6 及更高版本中。

ARM LDREXB、LDREXH、LDREXD、STREXB、STREXD 和STREXH 可用于 ARMv6K 及更高版本中。

所有这些 32 位 Thumb 指令均可用于 ARMv6T2 及更高版本，但 LDREXD 和 STREXD 在 ARMv7-M 架构中不可用。

这些指令均无 16 位版本。

**示例**

~~~ c
 MOV r1, #0x1   ; load the ‘lock taken’ value
try
 LDREX r0, [LockAddr]   ; load the lock value
 CMP r0, #0         ; is the lock free?
 STREXEQ r0, r1, [LockAddr]  ; try and claim the lock
 CMPEQ r0, #0        ; did this succeed?
 BNE try          ; no – try again
    ....                                        ; yes – we have the lock
~~~


**8. ldrex使用问题**

ldrex与strex指令的结合确实强大，对于一些古老的多核处理器，需要锁总线来保证数据原子操作(如x86)，这样会导致访问效率降低。而ldrex和strex没有进行锁总线操作，并且在两条指令之间可以对变量进行复杂的操作，不仅仅是加1减1操作。但是也需要注意ldrex与strex指令要想正常工作，也是有前提的，笔者在开发ti 66AH2H（4核cortex-A15）平台BSP时，就遇到了这个问题，先看下ARM手册对于ldrex指令的相关说明：

![微信图片_20210607223649](../../../image\微信图片_20210607223649.jpg)

![微信图片_20210607223701](../../../image\微信图片_20210607223701.jpg)

上述内容来源于ARM官方文档（DDI0438I_cortex_a15_r4p0_trm.pdf），主要表达意思就是对于ldrex和strex指令的支持需要global monitor，而global monitor的实现由两种，一种是内部实现，需要开启cache。另一种是外部实现（和芯片有关系，有的芯片没有实现），通过总线监听的方式。当内部和外部都实现时，优先使用内部global monitor。笔者在开发ti 66AH2H（4核cortex-A15）平台BSP时，就在没有开启cache时使用了ldrex指令，系统工作异常，这也说明了66AH2H没有实现外部global monitor。同样是ti cortex-A15(AM5728)，就实现了外部global monitor，zynq 7000双核cortex-A9(Armv7-A)也可以在不开启cache时使用ldrex指令，外部global monitor是否实现与芯片有关系，为安全起见，尽量不在开启cache前使用ldrex和strex指令。

