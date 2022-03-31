 

# 状态机 | 如何从零开始构建一个QM项目

[![李肖遥](https://pica.zhimg.com/v2-50cf50a007016e9892eca7c2f8861674_xs.jpg?source=172ae18b)](https://www.zhihu.com/people/lixiaoyao2020)

[李肖遥](https://www.zhihu.com/people/lixiaoyao2020)



[状态机 | 如何从零开始构建一个QM项目](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/wzcZ3cKDGnXJI-WiX0Yzow)

[mp.weixin.qq.com/s/wzcZ3cKDGnXJI-WiX0Yzow![img](https://pic4.zhimg.com/v2-7c78abb0f3a6c05d80f60d9b8de09ff7_180x120.jpg)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/wzcZ3cKDGnXJI-WiX0Yzow)

## **QM简介以及打开方式**

QM建模工具可用于两种操作模式：交互式GUI模式和命令行模式。同时也支持windows、Linux、MaxOS操作系统上运行，其两种操作模式都是一样的，我将在windows上演示。

windows上运行QM直接双击桌面快捷方式，启动QM无需任何命令行参数。也可以通过将QM模型文件拖放到QM快捷方式上来启动QM™，我们以一个QM建模和实现简单的“闪烁”应用程序，来学习如何从零开始构建一个QM项目，并且在桌面上执行时将运行结果打印到屏幕上。

## **从头开始创建新模型**

我们先新建一个Model如下图所示，这里我们直接选择默认的qpc框架类型，选择自己的自定义路径，模型模板为None，以便可以从头开始构建模型，然后按确定按钮。

![img](https://pic3.zhimg.com/80/v2-2976ab6f80a084157136ed6302e419c6_720w.jpg)

## **添加模型项**

现在，按照以下步骤可以将项目添加到新模型中。

### **添加包**

添加的第一项是包，UML中的包是一种分组构造，它允许我们将其他模型项组合到更高级别的单元（包）中。程序包最常见的用途是将类分组在一起，但是程序包也可以包含自由属性，自由操作甚至其他程序包。

在`Model Explorer`视图中，右键单击模型项目以获取特定于该项目的弹出菜单，然后选择`Add Package`，在`Property Editor`视图中更改名字如下图：

![img](https://pic4.zhimg.com/80/v2-0687b569adf8a3830482c55718b25a5b_720w.jpg)

### **添加类**

接下来，向新包中添加一个类，因为只有类才能具有行为（即状态机）。

在`Model Explorer`视图中，右键单击`AOs`以获取特定于该项目的弹出菜单，然后选择`Add Class`，在`Property Editor`视图中更改名字如下图：

![img](https://pic3.zhimg.com/80/v2-ee7b73d7ceea4e3d5edc5a99b0da023a_720w.jpg)

### **添加时间事件**

接下来添加时间事件属性，该属性将周期性触发“Blinky”状态机中的闪烁。

在`Model Explorer`视图中，右键单击`Blinky:QActive`以获取特定于该项目的弹出菜单，然后选择`Add Attribute`，在`Property Editor`视图中更改配置如下图：

![img](https://pic1.zhimg.com/80/v2-36f34b3409c48e2e9777f49a79809284_720w.jpg)

### **添加状态机**

在`Model Explorer`视图中，右键单击`Blinky:QActive`以获取特定于该项目的弹出菜单，然后选择`Add State Machine`，双击`SM`，如下图：

![img](https://pic3.zhimg.com/80/v2-f3dfad0146d0971760ba8bf431812c5e_720w.jpg)

## **绘制状态机图**

### **添加状态**

在图工具箱中，单击状态工具，将鼠标移动到图表窗口，在其中放置状态形状的左上角。单击鼠标并将其拖动到状态形状的右下角，释放鼠标。举个例子如下：

![img](https://pic3.zhimg.com/80/v2-286b17181ff955c27920495f51962c2e_720w.jpg)

在`Property Editor`中，将状态名称更改为`off`，然后将`entry`添加到此状态`BSP_ledOff（）;`，添加第二个状态就变为`on`、`BSP_ledOn（）`;。

### **添加初始转换状态**

![img](https://pic4.zhimg.com/v2-edf49c4bcda98cf500f930ceae12ba8f_b.jpg)





之后在`Property Editor`中添加

```text
QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC/2, BSP_TICKS_PER_SEC/2);
```

### **添加转换状态**

![img](https://pic1.zhimg.com/v2-e4568aa95a4c23e4a17ee7f29017a554_b.jpg)





以与之前类似的方式，添加第二个过渡并将其触发器也更改为TIMEOUT，最终效果如下

![img](https://pic1.zhimg.com/80/v2-e17f2d3e35d772c2b609ff1a22983cb8_720w.jpg)

## **生成代码**

与基于状态机的大多数其他图形工具相比，QM将代码生成“`颠倒`”了，通过QM，可以确定生成的代码结构，目录名，文件名以及每个文件中包含的元素，也可以将自己的代码与生成的代码混合，并使用QM生成尽可能多或更少的整体代码。

### **添加目录**

首先创建一个目录，该目录将确定磁盘上生成的文件相对于QM模型文件的位置。

在`Model Explorer`视图中，右键单击模型项，然后在弹出菜单中选择`Add directory`，这是将在其中生成代码的目录。与QM模型文件的目录路径相关联的可以在属性编辑器中进行编辑，代码将在与模型相同的目录中生成。

![img](https://pic3.zhimg.com/80/v2-8cfcc272f506605221d52479f467bc82_720w.jpg)

### **添加文件**

![img](https://pic1.zhimg.com/v2-da0083d80781a1b1fff2e55817dae618_b.jpg)





向目录添加源文件和头文件，这是我们的常规操作。

### **编辑文件及编译工程**

在QM中，我们可以键入自己的代码以及代码生成指令，把下面一段代码复制到文件中。

```text
#include "qpc.h"
#include <stdio.h>
#include <stdlib.h> /* for exit() */
 
Q_DEFINE_THIS_FILE
 
enum { BSP_TICKS_PER_SEC = 100 };
 
void BSP_ledOff(void) {
    printf("LED OFF\n");
}
void BSP_ledOn(void) {
    printf("LED ON\n");
}
void Q_onAssert(char const * const module, int loc) {
    fprintf(stderr, "Assertion failed in %s:%d", module, loc);
    exit(-1);
}
void QF_onStartup(void) {}
void QF_onCleanup(void) {}
void QF_onClockTick(void) {
    QF_TICK_X(0U, (void *)0);  /* perform the QF clock tick processing */
}
 
enum BlinkySignals {
    TIMEOUT_SIG = Q_USER_SIG,
    MAX_SIG
};
 
/*============== ask QM to declare the Blinky class ================*/
$declare${AOs::Blinky}
 
static Blinky l_blinky;
QActive * const AO_Blinky = &l_blinky.super;
 
static void Blinky_ctor(void) {
    Blinky *me = (Blinky *)AO_Blinky;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}
 
int main() {
    /* statically allocate event queue buffer for the Blinky AO */
    static QEvt const *blinky_queueSto[10];
 
    QF_init(); /* initialize the framework */
 
    Blinky_ctor(); /* explicitly call the "constructor" */
    QACTIVE_START(AO_Blinky,
                  1U, /* priority */
                  blinky_queueSto, Q_DIM(blinky_queueSto),
                  (void *)0, 0U, /* no stack */
                  (QEvt *)0);    /* no initialization event */
    return QF_run(); /* run the QF application */
}
 
/*================ ask QM to define the Blinky class ================*/
$define${AOs::Blinky}
```

当然也可以像生成任何其他手写代码一样构建生成的代码，然后开始编译代码如下图所示。

![img](https://pic1.zhimg.com/v2-766a268768f020bf58408b984f2d62e0_b.jpg)





## **下载调试工程**

如果将blinky.c文件设计为在Windows或Linux之类的桌面操作系统上运行，则由于printf（）语句，因此在开头编码了板级支持软件包（BSP）功能。这是如何在Windows命令提示符下生成blinky.exe可执行文件的方法：

![img](https://pic4.zhimg.com/v2-a0fc45b9ffbbf5bc4b90612a7cef564b_b.webp)







> 以上动图都是参考官网的，为了能够帮助我们清晰的理解相关操作，感谢大家支持，原创不易，希望大家三连转发支持，感谢！
