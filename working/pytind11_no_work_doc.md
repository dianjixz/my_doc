# pybind11 不可行报告
21年7月9号，遇到的问题，参考
## pybind11
   pybind11是一个轻量级的只包含头文件的库，它主要是用来在已有的 C++代码的基础上做扩展，它的语法和目标非常像Boost.Python，但Boost.Python为了兼容现有的基本所有的C++编译器而变得非常复杂和庞大，而因此付出的代价是很多晦涩的模板技巧以及很多不必要的对旧版编译器的支持。Pybind11摒弃了这些支持，它只支持python2.7以上以及C++ 11以上的编译器，使得它比Boost.Python更加简洁高效。
### 实现的目标：
* 1、批量化将imlib内的一些图像算法转换成python可调用的算法
* 2、在python端封装imlib让调用和maixpy端保持一致
### 实现目标所需要的工作：
> 方法一：直接将所有对象转换成python对象
* 1、将imlib所用的结构体转换成python对象
* 2、将imlib里面的函数转换成python可调用的函数
* 3、在python端可进行imlib函数的参数输入和参数输出获取
* 4、在python端封pybind11传递上来的参数，使其和maixpy保持一致
> 方法二：在c++端对函数进行一次见简单封装，使图片可以从类中获得，传递给python对象，然后获取返回值
* 1、构建一个c++图形类，将图像等结构体放在类中，然后通过构造函数传递图片解析，直接调用函数获取反回值
* 2、在python端调用pybind11传递上来的函数，解析相关参数，并封装，使其和maixpy保持一致
> 方法三：在c++端对函数进行完全封装，传递类中图片，转义python端传来的参数，转义算法返回的参数
* 1、构建图像大类
* 2、在类中逐个封装图像算法，提供类中图片，转义python端参数，转义算法返回值使符合python对象
* 3、在python端进行简单封装，使其和maixpy保持一致  
### 使用pybind11完成方法一时遇到的问题：
#### 1、使用pybind11转换结构体时无法合理表达出想要的结果。
结构体表达网上例程：
~~~ c++{
//文件名：whjy.h
#include <string>  
using namespace std;  
struct student{  
    string name;  
    int Chinese;  
    int Mathematics;  
    int English;  
    int total;  
    student(string n){  
    this->name = n;  
    }  
    void setName(string stuName){  
        this->name = stuName;  
    }  
};  
void calc(struct student&);

//文件名：func.cpp
#include "whjy.h"  
#include <string>  
void calc(struct student& tyh){  
    tyh.total = tyh.Chinese + tyh.Mathematics + tyh.English;  
}

//文件名：func_wrapper.cpp
#include <pybind11/pybind11.h>  
#include "whjy.h"  
namespace py = pybind11;  
PYBIND11_MODULE(abctest, m){  
    m.doc() = "simple example";  
    py::class_<student>(m, "student")  
        .def(py::init<string>())  
        .def("setName", &student::setName)  
        .def_readonly("name", &student::name)  
        .def_readwrite("Chinese", &student::Chinese)  
        .def_readwrite("Mathematics", &student::Mathematics)  
        .def_readwrite("English", &student::English)  
        .def_readwrite("total", &student::total);  
    m.def("calc", &calc);  
}
}
~~~
对于基本图像结构体  
~~~ c
typedef struct image {
    int w;
    int h;
    int bpp;
    union{
        uint8_t *pixels;
        uint8_t *data;
    };
	uint8_t *pix_ai;	//for MAIX AI speed up
} __attribute__((aligned(8)))image_t;
~~~

- 困难：  

  在该结构体中，联合体pixels和data属于无名联合体，在c语言表达中属于image层级下的一个函数，但在pybind11中无法合理的转化出来。

对于返回参数列表：
~~~ c
typedef struct list
{
    list_lnk_t *head_ptr, *tail_ptr;
    size_t size, data_len;
}
__attribute__((aligned(8)))list_t;

typedef struct find_blobs_list_lnk_data {
    rectangle_t rect;
    uint32_t pixels;
    point_t centroid;
    float rotation;
    uint16_t code, count;
} __attribute__((aligned(8))) find_blobs_list_lnk_data_t;.

list_push_back(out, &lnk_blob);
~~~
首先是结构体转义表达困难，其次转义到python端需要对其进行链表入栈出栈操作来获取算法返回值。在python端进行转义操作是非常复杂而且困难的操作。
参考一个重要算法函数,可以看到在python端转义难度

~~~ c
void imlib_find_blobs(list_t *out, image_t *ptr, rectangle_t *roi, unsigned int x_stride, unsigned int y_stride,
                     list_t *thresholds, bool invert, unsigned int area_threshold, unsigned int pixels_threshold,
                     bool merge, int margin)
~~~

>  `以上只是pybind11遇到的封装输入输出问题 `

### 使用pybind11完成方法二时遇到的问题：

在不考虑简单封装函数工作量的情况下，在这里解决了图片输入的问题，也就是image_t图片的输入输出问题。
- 困难 
以上面所展示的imlib_find_blobs算法函数为例。在不考虑图片输入的前提下，我们需要考虑参数输入和算法输出。
~~~ c
list_t *out				//算法输出列表		可变列表
rectangle_t *roi		//图像感兴趣输入结构体 不可变列表
list_t *thresholds		//算法输入阈值		可变列表
~~~
对于可变列表，输入参数需要`list_push_back`函数进行链表压栈操作，输出参数需要`list_pop_front`函数进行出栈操作。在python端构建输入参数和输出参数比较困难。
相关函数输入输出参数构建困难。pybind11很难进行合理的转换操作。

### 使用pybind11完成方法三时遇到的问题：
当图片和参数全部放在c++端进行处理时，python端需要解析字典，元组和列表操作，c++端需要构建和解析相关输入和输出结构体。
在工作量上比写cpython的工作量还要多，在复杂度上，python调用函数时相关参数需要四次转换，大于cpython的转义次数。


### 使用pybind11传递指针对象时遇到的问题：
网上例程：
~~~ c

//example.cpp
#include <pybind11/pybind11.h>
 
namespace py = pybind11;
 
void swap(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
 
    m.def("swap", [](py::buffer a, py::buffer b) {\
        py::buffer_info a_info = a.request();
        py::buffer_info b_info = b.request();
        swap(static_cast<int*>(a_info.ptr), static_cast<int*>(b_info.ptr));
    });
~~~
测试过程，编译和导入正常。运行时发生错误
~~~ bash
Python 3.8.10 (default, Jun  2 2021, 10:49:15) 
[GCC 9.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import example
>>> a = 3
>>> b=5
>>> ma = example.swap(a,b)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: swap(): incompatible function arguments. The following argument types are supported:
    1. (arg0: buffer, arg1: buffer) -> None

Invoked with: 3, 5
>>> ma = example.swap(3,4)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: swap(): incompatible function arguments. The following argument types are supported:
    1. (arg0: buffer, arg1: buffer) -> None

Invoked with: 3, 4
>>> a = buffer('1')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'buffer' is not defined
>>> 
~~~

