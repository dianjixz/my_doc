 

# C语言实现try-catch-throw

[![Froser](https://pic3.zhimg.com/v2-811c31aaede8bf1b27eb6ee12ce65228_xs.jpg?source=172ae18b)](https://www.zhihu.com/people/froser)

[Froser](https://www.zhihu.com/people/froser)

简历投递:https://job.toutiao.com/s/eF848tL

## 一、简介

  众所周知，从C++开始才有结构化的异常处理体系(try, catch, throw,  finally)，在C语言中并不存在“异常”这么一说。我们很多时候，处理错误的方式是通过拿errno或者是Windows下的GetLastError()，通过错误码来判断错误处理的流程。在VC系列的编译器中，微软更是支持了结构化异常(SEH)来进行错误的处理。使用结构化异常的程序，如果要移植到其它操作系统下是比较困难的，因为__try, __catch等关键字，是微软创造出来的方言，而并非标准的C++关键字。

  那么，怎么样用C来实现一个简单的try-catch异常处理呢？假设我们有以下代码：

```cpp
void panic()
{
	_throw_(5);
}

int main()
{
	_try_
	{
		_try_
		{
			panic();
		}
		_catch_(x)
		{
			printf("Inner: %d\n", x);
			_throw_(x + 1);
		}
	}
	_catch_(x)
	{
		printf("Outter: %d\n", x);
	}

	return 0;
}
```

  我们的目的是实现_try_, _catch_ 以及 _throw_，并且输出的结果是：

```text
Inner: 5
Outter: 6
```

  之所以输出这样的结果是，panic抛出了一个异常5，仅接着main函数拿到5之后，又抛出了一个5+1=6，所以理论上会得到上面的结果。

  在这里我们简单地认为_throw_就是抛出了一个整数，而并非C++那样抛出了一个std::exception，来简化我们的实现。

## 二、原理

  从原理上来讲，throw其实就是一个跳转，跳转到由try-catch块包围的catch块处。在这里，我们用两个函数来实现这个功能：

```cpp
int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);
```

   setjmp函数记录调用时的当前状态，如IP、ESP等，并且返回0。状态被写在一个jmp_buf对象中，这个对象其实是一个int数组。比较有趣的是longjmp函数，这个函数传入一个jmp_buf以及一个整形。它会立即跳转到当时的setjmp处，并且返回值是longjmp中的第二个参数。也就是说，这个函数可能会被调用两次，从某种程度上来说，有点像fork()。

![img](https://pic2.zhimg.com/80/v2-3615e8b3e1068a1b74cd58dcb5367f81_720w.jpg)longjmp之后，代码跳转到i=setjmp(env)这一行，并且返回2

   在try-catch中，try函数充当着setjmp的功能。当setjmp返回0时（也就是第一次执行），执行try块的代码，当返回非0时，说明有longjmp被调用，此时发生异常，跳入catch快。同时，throw就相当于longjmp，可以跳转到包含它的catch块中。

   我们注意到，longjmp的第一个参数jmp_buf，其实是在try块中创建的。我们怎么来获取到上一个try块中创建的jmp_buf呢？我们可以如同操作系统创建一个运行时栈那样，我们也创建一个try-catch堆栈。在try时，我们把创建的jmp_buf压入，在throw时，我们把jmp_buf弹出。为了线程安全，我们得为每一个线程分配这样一个栈，不过在这个例子中我们不考虑线程安全。

## 三、编码

  首先我们设计简单的数据结构：一个jmp_buf栈，并提供push和pop功能：

```cpp
#define MAX_JUMP 1024
typedef jmp_buf* jmp_stack[MAX_JUMP];
jmp_stack g_js;
int g_jsidx;

void jmp_stack_push(jmp_buf* jb)
{
	g_js[g_jsidx++] = jb;
}

jmp_buf* jmp_stack_pop()
{
	return g_js[--g_jsidx];
}
```

  接下来我们分析_try_应该做一些什么事情。

  首先，它要创建一个jmp_buf对象。

  其次，它要判断setjmp返回值。如果是0，那么说明不是由longjmp跳转而来的，此时将jmp_buf压入栈。

  宏定义如下：

```cpp
#define _try_ \
	jmp_buf __jmp_buf; \
	int __jmp_result = setjmp(__jmp_buf); \
	if (!__jmp_result) jmp_stack_push(&__jmp_buf); \
	if (!__jmp_result)
```

  __jmp_result当为非0时，其实也就是通过longjmp(或者说是throw)返回的错误码。

  再分析_catch_(x)应该做一些什么事情：

  1、首先，获取__jmp_result, 赋值给catch中的参数x。

  2、如果__jmp_result为0，说明不是从throw抛出的，需要主动pop。如果不为0，则执行catch中的语句，不需要pop（因为throw中包含pop操作）。

  宏定义如下：

```text
#define _catch_(x) \
	int x = __jmp_result; \
	if (!x) jmp_stack_pop(); \
	else
```

  最后是throw的行为：

  pop一个jmp_buf，并且longjmp。定义如下：

```cpp
#define _throw_(x) longjmp(*jmp_stack_pop(), x);
```

整个例子完整的代码如下：

```cpp
#include <setjmp.h>

#define _try_ \
	jmp_buf __jmp_buf; \
	int __jmp_result = setjmp(__jmp_buf); \
	if (!__jmp_result) jmp_stack_push(&__jmp_buf); \
	if (!__jmp_result)

#define _catch_(x) \
	int x = __jmp_result; \
	if (!x) jmp_stack_pop(); \
	else

#define _throw_(x) longjmp(*jmp_stack_pop(), x);

#define MAX_JUMP 1024
typedef jmp_buf* jmp_stack[MAX_JUMP];
jmp_stack g_js;
int g_jsidx;

void jmp_stack_push(jmp_buf* jb)
{
	g_js[g_jsidx++] = jb;
}

jmp_buf* jmp_stack_pop()
{
	return g_js[--g_jsidx];
}

/* TEST */
void panic()
{
	_throw_(5);
}

int main()
{
	_try_
	{
		_try_
		{
			panic();
		}
		_catch_(x)
		{
			printf("Inner: %d\n", x);
			_throw_(x + 1);
		}
	}
	_catch_(x)
	{
		printf("Outter: %d\n", x);
	}

	return 0;
}
```

在这里，我们并没有讨论线程安全性，也没有讨论throw一个复合类型，但是功能上已经是实现了的。

## 四、要点总结

1、了解setjmp、longjmp函数的用法

2、知道使用栈来管理try-catch异常信息。

3、知道使用宏来隐藏细节。

欢迎大家在评论中讨论。

发布于 2019-10-14 14:13
