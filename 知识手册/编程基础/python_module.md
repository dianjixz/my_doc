一、Python扩展模块
1、Python扩展模块简介

Python与C/C交互的方案有多种，如Python C API，SWIG，SIP，ctypes，cpython，cffi，boost.python等。
Python只是一个语言规范，有很多具体实现，CPython是标准Python，由C编写，Python脚本被编译成CPython字节码，然后由虚拟机解释执行，垃圾回收使用引用计数，Python与C/C混合编程本质是基于CPython解释器。其它的Python实现包括Jython、IronPython、PyPy、Pyston，Jython是Java编写的，使用JVM的垃圾回收，可以与Java混合编程，IronPython面向.NET平台。
Python扩展模块可以用Python编写，也可以用C/C++编译型的语言来写扩展。Python可扩展性具有为语言增加新功能、具有可定制性、代码可以实现复用等优点。
2、Python扩展模块的特点

（1）提升计算能力
Python扩展模块使用C/C编写，其计算性能也是C/C同级别的，其跨语言通信接口上的性能损失小到忽略不计，所以能够提供非常好的性能支持，典型如用于科学计算的Numpy包，其底层调用了第三方的数学计算库，其性能也是同级别的。
（2）使用多核心计算能力
Python扩展模块通过对GIL的控制，可以使用CPU的多核心计算能力，而不会受限于纯Python程序的单核心限制，结合多线程可以定制使用多个核心。
（3）系统组件隔离和模块化
通过把每个C/C++函数提供给Python接口，使得函数之间不共享任何状态，实现了良好的组件隔离，有助于开发和测试。同时由于参数全部通过Python传递，易于打印和中断，可调试性有很大的提高。
（4）使用第三方库
对于不支持Python的第三方库，需要开发者自己编写扩展模块实现系统对接。但现代流行的大型库，很多都有官方的Python扩展模块，使得应用质量有了较大提高，典型如OpenCV和PyCUDA。
二、Python C API扩展
1、Python C API扩展简介

CPython是C语言实现的Python解释器，是Python语言的官方实现，是使用最广泛的Python解释器。
C/C实现Python扩展模块的流程如下：
（1）包含头文件Python.h
（2）C/C模块实现
（3）定义C/C++函数的Python接口映射表
（4）初始化函数
（5）初始化模块
（6）setup.py编写
（7）扩展模块编译安装
2、Python头文件

Python.h头文件包含用于将C/C++模块hook到CPython解析器的CPython API，而且必须将Python.h头文件写在任何标准头文件前，因为Python.h头文件可能定义了一些影响标准头文件的预处理宏。
Python.h文件定义了所有的Python C API，Python C API的方法与变量前缀为Py_和_Py，在代码中尽量不要使用此前缀，避免混乱。
Python.h文件中，Python对象API命名为PyObject_*、内存管理函数API命名为PyMen_*、数值(包括整数和浮点数的运算等)API命名为PyNumber_*、浮点数API命名为PyFloat_*、整数API命名为PyLong_*、序列API命令为PySequence_*、列表API命名为PyList_*、元组API命名为PyTuple_*、字典API命名为PyDict_*、集合API命名为PySet_*、可迭代对象API命名为PyIter_*、字符串API命名为PyUnicode_*、函数参数API命名为PyArg_*、函数API命名为PyFunction_*、文件对象API命名为PyFile_*。
C语言没有bool类型，Python C API的bool类型定义在asdl.h中，形式如下：typedef enum {false, true} bool;，即false=0，true=1。
3、C/C++模块编写

实现一个加法和乘法操作的模块，加法和乘法操作方法如下：

static double add(double a, double b)
{
    return a + b;
}

static double mul(double a, double b)
{
    return a * b;
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.

4、C/C++模块包装

Python C 扩展的函数定义一般如下：

static PyObject *MyFunction( PyObject *self, PyObject *args );
static PyObject *MyFunctionWithKeywords(PyObject *self,  PyObject *args, PyObject *kw);
static PyObject *MyFunctionWithNoArgs( PyObject *self );

    1.
    2.
    3.

Python C扩展模块中的函数是静态函数，名字是任意的，但通常命名为modulename_functionname的形式，返回PyObject类型的指针。如果函数不想返回一个值，Python定义了一个宏Py_RETURN_NONE，等价于在脚本层返回None。
C/C++函数的包装如下：

static PyObject* operator_add(PyObject *self, PyObject *args)
{
    float a, b;
    if(!PyArg_ParseTuple(args, "ff", &a, &b))
    {
        return NULL;
    }
    float result = add(a, b);
    return Py_BuildValue("f", result);
}

static PyObject* operator_mul(PyObject *self, PyObject *args)
{
    float a, b;
    if(!PyArg_ParseTuple(args, "ff", &a, &b))
    {
        return NULL;
    }
    float result = mul(a, b);
    return Py_BuildValue("f", result);
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.
    13.
    14.
    15.
    16.
    17.
    18.
    19.
    20.
    21.

5、Python接口映射表定义

Python接口映射表是PyMethodDef结构的数组，PyMethodDef结构体定义如下：

struct PyMethodDef {
   char *ml_name;
   PyCFunction ml_meth;
   int ml_flags;
   char *ml_doc;
};

    1.
    2.
    3.
    4.
    5.
    6.

ml_name:暴露给Python程序的函数名。
ml_meth: 函数的指针，即函数定义的地方。
ml_flags: 函数签名方式，一般是METH_VARARGS；如果想传入关键字参数，可以与MET_KEYWORDS进行或运算；如果不接受任何参数，可以给其赋值为METH_NOARGS。
ml_doc: 函数的文档字符串，可以直接给其赋值为NULL。
Python接口映射表必须以一个由NULL和0组成的结构体进行结尾，示例如下：

static PyMethodDef operator_methods[] = {
   { "add", (PyCFunction)operator_add, METH_VARARGS, "operator add" },
   { "mul", (PyCFunction)operator_mul, METH_VARARGS, "operator mul" },
   { NULL, NULL, 0, NULL }
};

    1.
    2.
    3.
    4.
    5.

6、扩展模块初始化

扩展模块的初始化函数会在模块被导入时被CPython解析器调用。初始化函数需要从构建的库中导出，因此Python头文件里定义了PyMODINIT_FUNC来进行导出工作，因此需要在定义初始化函数时使用。

PyMODINIT_FUNC initModuleName() {
   Py_InitModule3(ModuleName, module_methods, "docstring...");
}

    1.
    2.
    3.

py_InitModule3函数原型如下：
PyObject* Py_InitModule3(char *name, PyMethodDef *methods, char *doc)
module_name: 被导出的模块名；
module_methods: 模块的方法映射表；
docstring: 模块的注释；
返回值：返回一个新的模块对象
Py_InitModule函数原型如下：
PyObject* Py_InitModule(char *name, PyMethodDef *methods)
name：模块名称
methods：模块函数描述表
返回值：返回一个新的模块对象
operator模块的初始化如下：

PyMODINIT_FUNC initoperator()
{
   Py_InitModule3("operator", operator_methods);
}
operator.c文件如下:
#include <Python.h>

/***************************
* C++语言函数定义
***************************/

static double add(double a, double b)
{
    return a + b;
}

static double mul(double a, double b)
{
    return a * b;
}

/*****************************
* C++语言函数的包装
*****************************/

static PyObject* operator_add(PyObject *self, PyObject *args)
{
    float a, b;
    if(!PyArg_ParseTuple(args, "ff", &a, &b))
    {
        return NULL;
    }
    float result = add(a, b);
    return Py_BuildValue("f", result);
}

static PyObject* operator_mul(PyObject *self, PyObject *args)
{
    float a, b;
    if(!PyArg_ParseTuple(args, "ff", &a, &b))
    {
        return NULL;
    }
    float result = mul(a, b);
    return Py_BuildValue("f", result);
}

static PyMethodDef operator_methods[] = {
   { "add", (PyCFunction)operator_add, METH_VARARGS, "operator add" },
   { "mul", (PyCFunction)operator_mul, METH_VARARGS, "operator mul" },
   { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initoperator()
{
   Py_InitModule3("operator", operator_methods);
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.
    13.
    14.
    15.
    16.
    17.
    18.
    19.
    20.
    21.
    22.
    23.
    24.
    25.
    26.
    27.
    28.
    29.
    30.
    31.
    32.
    33.
    34.
    35.
    36.
    37.
    38.
    39.
    40.
    41.
    42.
    43.
    44.
    45.
    46.
    47.
    48.
    49.
    50.
    51.
    52.
    53.
    54.
    55.
    56.
    57.

7、setup.py编写

setup.py脚本是用于构建扩展模块的配置，通常包含如下实用功能：
（1）提供了更完善的编译参数，比如编译时特定的宏定义，编译器参数等
（2）引用其它第三方库
（3）区分不同平台的编译，对Linux和Mac有所区分
（4）提供更加完善的元信息
setup.py脚本如下：

# !/usr/bin/env python

from distutils.core import setup, Extension

setup(name='operator', ext_modules=[Extension('operator', sources=['operator.c'])])

    1.
    2.
    3.
    4.
    5.

8、Python扩展模块编译安装

编译模块：
python setup.py build
安装模块：
python setup.py install
Python扩展模块将会安装在当前虚拟环境下。
python setup.py build_ext --inplace
–inplace表示在源码处生成模块文件
9、扩展模块使用

import operator


if __name__ == '__main__':
    result = operator.add(1, 9)
    print(result)
    result = operator.mul(100, 1)
    print(result)

# output:
# 10
# 100

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.

10、GIL与多线程

GIL是限制Python使用多核的直接原因，根本原因是Python解释器内部有一些全局变量，如异常处理等，但由于有很多Python API和第三方模块在使用GIL全局变量，使得GIL的改进一直得不到进展。
在Python扩展模块层面是可以释放GIL的，使得CPU控制权交还给Python，而当前C/C++代码也可以继续执行。但任何Python API的调用都必须在GIL控制下进行，因此在执行密集计算的任务前释放GIL，完成计算后，重新申请GIL，再进行返回值和异常处理。
第一种方法如下：

static PyObject *fun(PyObject *self, PyObject *args) 
{
    //....    
    PyThreadState *_save;    
    _save=PyEval_SaveThread();    
    block();    
    PyEval_RestoreThread(_save);
    //... }
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.

第二种方法如下：

Py_BEGIN_ALLOW_THREADS; 
//可能阻塞的操作 
Py_END_ALLOW_THREADS;

    1.
    2.
    3.

使用多核计算的方法是，把任务拆分成多个小份，每个小份都放在一个线程中运行。线程里调用扩展模块中的计算函数，计算函数在实际计算时释放GIL。
11、异常处理

当一个函数失败时，CPython解释器约定会返回一个错误值（NULL）并设置3个全局静态变量，分别对应Python的sys.exec_type, sys.exec_value和sys.exec_traceback。最先检测到异常的函数应该报告并设置全局变量，其它调用函数只是返回异常值。
Python C API定义了一些函数来设置并检查各种异常：
(1)PyErr_SetString(PyObject* type, const char* message)
type是一个预定义的对象，例如PyExc_ZeroDivisionError，C字符串用于说明异常出现的原因。
(2)PyErr_SetObject(PyObject* type, PyObject* value)
type异常类型，value为异常值
(3)PyErr_Occurred()
用来检查是否设置了一个异常
(4)如果要忽略一个异常而不传递给解析器，可以调用PyErr_Clear()函数
(5)所有直接调用malloc()或者realloc()函数失败时，必须要调用PyErr_NoMemory()，并且返回失败标志
常见异常类型如下：
PyExc_ZeroDivisionError ：被0除    
PyExc_IOError ：IO错误    
PyExc_TypeError ：类型错误，如参数类型不对    
PyExc_ValueError ：值的范围错误    
PyExc_RuntimeError ：运行时错误    
PyExc_OSError ：各种与OS交互时的错误
三、Python C API类型转化
1、Python对象构建

Py_BuildValue函数可以用于创建PyObject对象，其函数声明如下：
Python3快速入门（十七）——Python扩展模块开发_Python 扩展模块
2、基础数据类型

Python提供了一系列的函数用于C与Python数据类型的相互转化，相应函数的格式为PyXXX_AsXXX 或者PyXXX_FromXXX，一般带有As的函数是将Python对象转化为C数据类型的，而带有From的函数是将C对象转化为Python，Py的XXX表示的是Python中的数据类型。PyUnicode_AsWideCharString 将Python中的字符串转化为C中宽字符，而 PyUnicode_FromWideChar 是将C++的字符串转化为Python中的字符串。Python3废除了Python2中的普通的字符串，将所有字符串都当做Unicode，所以使用Python3时需要将所有字符串转化为Unicode。

#include <Python.h>
#include <stdio.h>

int main() {

    // 初始化Python环境
    Py_Initialize();

    // 有符号整型
    PyObject* py_ival1 = Py_BuildValue("i", -890);
    PyObject* py_ival2 = PyLong_FromLong(-890);
    int ival1 = PyLong_AsLong(py_ival1);
    int ival2 = PyLong_AsLong(py_ival2);
    printf("ival1 = %d, ival2 = %d\n", ival1, ival2);
    // ival1 = -890, ival2 = -890

    // 无符号整型
    PyObject* py_uval1 = Py_BuildValue("I", 123456789);
    PyObject* py_uval2 = PyLong_FromUnsignedLong(123456789);
    unsigned int uval1 = PyLong_AsUnsignedLong(py_uval1);
    unsigned int uval2 = PyLong_AsUnsignedLong(py_uval2);
    printf("uval1 = %d, uval2 = %d\n", uval1, uval2);
    // uval1 = 123456789, uval2 = 123456789

    // 长整型
    PyObject* py_lval1 = Py_BuildValue("L", 456784567845678);
    PyObject* py_lval2 = PyLong_FromLongLong(456784567845678);
    long long lval1 = PyLong_AsLongLong(py_lval1);
    long long lval2 = PyLong_AsLongLong(py_lval2);
    printf("lval1 = %lld, lval2 = %lld\n", lval1, lval2);
    // lval1 = 456784567845678, lval2 = 456784567845678

    // 浮点类型
    PyObject* py_fval1 = Py_BuildValue("f", 3.1415);
    PyObject* py_fval2 = PyFloat_FromDouble(3.1415);
    double fval1 = PyFloat_AsDouble(py_fval1);
    double fval2 = PyFloat_AsDouble(py_fval2);
    printf("fval1 = %f, fval2 = %f\n", fval1, fval2);
    // fval1 = 3.141500, fval2 = 3.141500

    // 布尔类型
    PyObject* py_bval1 = Py_BuildValue("b", false);
    PyObject* py_bval2 = PyBool_FromLong(true);
    int bval1 = PyLong_AsLong(py_bval1);
    int bval2 = PyLong_AsLong(py_bval2);
    printf("bval1 = %d, bval2 = %d\n", bval1, bval2);
    // bval1 = 0, bval2 = 1

    // 字符串类型
    PyObject* py_sval1 = Py_BuildValue("s", "hello world");
    PyObject* py_sval2 = PyUnicode_FromString("hello world");
    // 将unicode转换为utf8
    PyObject* py_utf1 = PyUnicode_AsUTF8String(py_sval1);
    PyObject* py_utf2 = PyUnicode_AsUTF8String(py_sval2);
    // 将utf8转换为const char*
    char* sval1 = PyBytes_AsString(py_utf1);
    char* sval2 = PyBytes_AsString(py_utf2);
    printf("sval1 = %s, sval2 = %s\n", sval1, sval2);
    // sval1 = hello world, sval2 = hello world

    // 退出Python环境
    Py_Finalize();
    return 0;
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.
    13.
    14.
    15.
    16.
    17.
    18.
    19.
    20.
    21.
    22.
    23.
    24.
    25.
    26.
    27.
    28.
    29.
    30.
    31.
    32.
    33.
    34.
    35.
    36.
    37.
    38.
    39.
    40.
    41.
    42.
    43.
    44.
    45.
    46.
    47.
    48.
    49.
    50.
    51.
    52.
    53.
    54.
    55.
    56.
    57.
    58.
    59.
    60.
    61.
    62.
    63.
    64.

G++编译：
g++ -I/home/user/anaconda3/include/python3.7m -c pycobject.c
链接：
g++ -o main pycobject.o -L/usr/local/lib -lpython3.7 -lrt -lpthread -lutil -ldl
3、Python元组对象

PyObject* PyTuple_New(Py_ssize_t len)
创建一个Python元组对象，必须设置长度，如果设置长度为0，则元组对象是一个空元组。
int PyTuple_Check(PyObject *p)
判断是否是一个元组对象
Py_ssize_t PyTuple_Size(PyObject *p)
获取元组的大小
PyObject* PyTuple_GetItem(PyObject* p, Py_ssize_t pos)
获取元组内指定下标的值
PyObject* PyTuple_GetSlice(PyObject* p, Py_ssize_t low, Py_ssize_t high)
获取分片数据 p[lwo, higt]
int PyTuple_SetItem(PyObject* p, Py_ssize_t pos, PyObject* o)
设置元组指定下标的值
int _PyTuple_Resize(PyObject *p, Py_ssize_t newsize)
改变元组的大小

#include <Python.h>
#include <stdio.h>

int main() {

    // 初始化Python环境
    Py_Initialize();

    PyObject* tuple = PyTuple_New(3);
    PyObject* item0 = Py_BuildValue("i", 123);
    PyTuple_SetItem(tuple, 0, item0);

    PyObject* item1 = Py_BuildValue("s", "hello");
    PyTuple_SetItem(tuple, 1, item1);

    PyObject* item2 = Py_BuildValue("f", 23.98f);
    PyTuple_SetItem(tuple, 2, item2);

    PyObject* py_data = PyTuple_GetItem(tuple, 0);
    int value = PyLong_AsLong(py_data);
    printf("value = %d\n", value);
    // value = 123

    // 退出Python环境
    Py_Finalize();
    return 0;
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.
    13.
    14.
    15.
    16.
    17.
    18.
    19.
    20.
    21.
    22.
    23.
    24.
    25.
    26.
    27.

4、Python字典对象

PyObject* PyDict_New()
创建一个Python字典对象，成功返回新空字典，失败返回NULL。
int PyDict_Check(PyObject *p)
判断对象是不是一个字典
void PyDict_Clear(PyObject *p)
清空Python字典对象的数据
int PyDict_Contains(PyObject* p, PyObject* key)
判断字典内是否存在一个键值数据
PyObject* PyDict_Copy(PyObject* p)
拷贝一个字典的数据，产生一个新的Python字典对象
int PyDict_SetItem(PyObject* p, PyObject* key, PyObject *val)
给Python字典对象设置新的键值数据
int PyDict_SetItemString(PyObject* p, const char key, PyObject *val)
给Python字典对象设置新的键值数据
int PyDict_DelItem(PyObject* p, PyObject* key)
删除Python键值数据
int PyDict_DelItemString(PyObject* p, const char key)
删除Python键值数据
PyObject* PyDict_GetItem(PyObject* p, PyObject *key)
获取Python字典对象的键的值
PyObject* PyDict_GetItemString(PyObject* p, const char *key)
获取Python字典对象的键的值
PyObject* PyDict_SetDefault(PyObject* p, PyObject* key, PyObject* default)
设置Python字典对象的默认值，当获取的Key不存在的时候则返回当前的默认数据
PyObject* PyDict_Items(PyObject* p)
返回一个Python字典对象所有数据的PyListObject
PyObject* PyDict_Keys(PyObject* p)
返回一个Python字典对象的所有Key
PyObject* PyDict_Values(PyObject* p)
返回一个Python字典对象的所有Value数据
Py_ssize_t PyDict_Size(PyObject *p)
获取Python字典的大小 len(dict)
int PyDict_Next(PyObject* p, Py_ssize_t ppos, PyObject* pkey, PyObject* pvalue)
遍历获取Python字典对象的所有数据，

#include <Python.h>
#include <stdio.h>

int main() {

    // 初始化Python环境
    Py_Initialize();

    PyObject* dict = PyDict_New();
    PyObject* key1 = Py_BuildValue("s", "age");
    PyObject* value1 = Py_BuildValue("i", 30);
    PyDict_SetItem(dict, key1, value1);

    PyObject* py_data = PyDict_GetItemString(dict, "age");
    int value = PyLong_AsLong(py_data);
    printf("value = %d\n", value);
    // value = 30

    // 退出Python环境
    Py_Finalize();
    return 0;
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.
    13.
    14.
    15.
    16.
    17.
    18.
    19.
    20.
    21.
    22.

5、Python列表对象

PyObject* PyList_New(Py_ssize_t len)
创建一个列表，成功返回新列表，失败返回NULL
int PyList_Check(PyObject *p)
判断是否是一个Python List（列表）
Py_ssize_t PyList_Size(PyObject *list)
获取列表元素的个数 len(list)
PyObject* PyList_GetItem(PyObject* list, Py_ssize_t index)
从列表里面获取一个元素，计数器不会加1
int PyList_SetItem(PyObject* list, Py_ssize_t index, PyObject* item)
设置别表指定位置的值，下标的所在的位置必须是有值的，并且是有效的
int PyList_Insert(PyObject* list, Py_ssize_t index, PyObject* item)
在列表指定位置插入值
int PyList_Append(PyObject* list, PyObject* item)
在列表尾部追加值
PyObject* PyList_GetSlice(PyObject* list, Py_ssize_t low, Py_ssize_t high)
获取列表里面一段切片数据，一段指定范围的数据 list[low:higt]
int PyList_SetSlice(PyObject* list, Py_ssize_t low, Py_ssize_t high, PyObject* itemlist)
设置列表分片数据，指定列表范围的数据 list[low:higt] = itemlist
int PyList_Sort(PyObject *list)
对列表数据进行排序
int PyList_Reverse(PyObject *list)
把列表里面的所有数据反转
PyObject* PyList_AsTuple(PyObject* list)
将Python列表转为Python元组 tuple(list)

#include <Python.h>
#include <stdio.h>

int main() {

    // 初始化Python环境
    Py_Initialize();

    PyObject* list = PyList_New(5);
    PyObject* item0 = Py_BuildValue("i", 123);
    PyList_SetItem(list, 0, item0);

    PyObject* item1 = Py_BuildValue("s", "hello");
    PyList_SetItem(list, 1, item1);

    PyObject* item2 = Py_BuildValue("f", 23.98f);
    PyList_SetItem(list, 2, item2);

    PyObject* py_data = PyList_GetItem(list, 0);
    int value = PyLong_AsLong(py_data);
    printf("value = %d\n", value);
    // value = 123

    // 退出Python环境
    Py_Finalize();
    return 0;
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.
    13.
    14.
    15.
    16.
    17.
    18.
    19.
    20.
    21.
    22.
    23.
    24.
    25.
    26.
    27.

6、参数提取

Python脚本调用扩展模块的函数时，传入的参数会存在PyObject* args所指向的PyObject对象中，参数的提取使用PyArg_ParseTuple() 和 PyArg_ParseTupleAndKeywords()函数进行解析 。
int PyArg_ParseTuple(PyObject* arg, char* format, ...);
参数 arg 是一个tuple对象，包含Python传递来的参数， format 参数必须是格式化字符串。
int PyArg_ParseTupleAndKeywords(PyObject* arg, PyObject* kwdict, char* format, char* kwlist[],...);
参数 arg 是一个tuple对象，包含Python传递过来的参数，参数 kwdict 是关键字字典，用于接受运行时传来的关键字参数。参数 kwlist 是一个NULL结尾的字符串，定义了可以接受的参数名，并从左到右与format中各个变量对应。如果执行成功 PyArg_ParseTupleAndKeywords() 会返回true，否则返回false并抛出异常。
format参数是一个字符串，通常每个字符代表一种类型；剩下的参数是与format相对应的各个变量的地址，返回值是一个整型，解析成功返回1，解析出错返回0。
无参函数的参数提取：
ok = PyArg_ParseTuple(args, "");
参数为一个字符串的函数的参数提取：
ok = PyArg_ParseTuple(args, "s", &s);
参数为两个长整型与一个字符串的函数的参数提取：
ok = PyArg_ParseTuple(args, "lls", &k, &l, &s);
参数至少有一个字符串，可以另外有一个字符串或整型的函数的参数提取：
ok = PyArg_ParseTuple(args, "s|si", &file, &mode, &bufsize);
参数为两个元组的函数的参数提取：
ok = PyArg_ParseTuple(args, "((ii)(ii))(ii)",&left, &top, &right, &bottom, &h, &v);
参数为一个PyObject对象，可以表示Python中的任意类型。
ok = PyArg_ParseTuple(args, "O", &p);
四、C++调用Python脚本
1、Python C API常用接口

void Py_Initialize()
初始化Python解释器，在C++程序中使用其它Python C API前，必须初始化Python解释器，如果调用失败，将产生一个致命的错误。
int PyRun_SimpleString( const char *command)
执行一段Python代码。
PyObject* PyImport_ImportModule( char *name)
导入一个Python模块，参数name可以是*.py文件的文件名。
PyObject* PyModule_GetDict( PyObject *module)
获取模块名称空间下的字典对象
PyObject* PyRun_String( const char *str, int start, PyObject *globals, PyObject *locals)
执行一段Python代码。
int PyArg_Parse( PyObject *args, char *format, ...)
解析Python数据为C的类型
PyObject* PyObject_GetAttrString( PyObject *o, char *attr_name)
返回模块对象o中的attr_name 属性或函数
PyObject* Py_BuildValue( char *format, ...)
构建一个参数列表，把C类型转换为Python对象，使Python可以使用C类型数据
PyEval_CallObject(PyObject* pfunc, PyObject* pargs)
pfunc是要调用的Python 函数，通常可用PyObject_GetAttrString()获得；pargs是函数的参数列表，通常可用Py_BuildValue()构建
void Py_Finalize()
关闭Python解释器，释放解释器所占用的资源。
2、Python环境初始化

调用Python模块时需要首先包含Python.h头文件，Python.h头文件一般在安装Python目录中的 include文件中。
调用Python脚本前先调用Py_Initialize 函数来初始化Python环境，可以调用Py_IsInitialized来检测Python环境是否初始化成功。
3、调用Python语句

针对简单的Python语句，可以直接调用 PyRun_SimpleString 函数来执行， 接收参数为Python语句的ANSI字符串，返回int型的值。如果为0表示执行成功，否则为失败。
4、调用Python函数

（1）加载Python模块(自定义模块)
加载Python模块需要调用 PyImport_ImportModule 函数，传入模块名称作为参数，模块名称即py文件名称，不能带.py后缀。返回一个Python对象的指针，在C中表示为PyObject，即模块对象的指针。
（2）获取Python函数对象
调用 PyObject_GetAttrString 函数来加载对应的Python模块中的方法，接收两个参数，第一个参数是获取到的对应模块的指针，第二个参数是函数名称的ANSI字符串。返回一个对应Python函数的对象指针。
（3）检查Python函数对象可调用性
调用 PyCallable_Check可以检测Python函数对象是否可以被调用，接收参数为Python函数对象指针，如果能被调用会返回true否则返回false。
（4）参数传递
Python中函数的参数以元组的方式传入，需要先将要传入的参数转化为元组。
（5）Python函数调用
调用 PyObject_CallObject 函数来执行对应的Python函数，接收两个参数，第一个参数Python函数对象的指针，第二个参数是需要传入Python函数中的参数组成的元组。返回Python的元组对象。
（6）解析返回值
获取到返回值（Python元组对象）后使用对应的函数将Python元组转化为C中的变量。
5、释放资源

需要调用 Py_DECREF 来解除Python对象的引用，以便Python的垃圾回收器能正常的回收Python对象的内存。
6、退出Python环境

Py_Finalize();
7、C++调用Python脚本实例

util.py脚本如下：

def add(a, b):
    return a + b


def mul(a, b):
    return a * b


def power(a, b):
    return a ** b

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.

C++调用Python代码如下：

#include <Python.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    // 初始化Python
    Py_Initialize();

    // 检查初始化是否成功
    if (!Py_IsInitialized())
    {
        return -1;
    }
    // 添加当前路径
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    // 载入Python模块
    PyObject* pName = PyUnicode_FromString("util");
    PyObject* pModule = PyImport_Import(pName);
    if (!pModule)
    {
        printf("can't find util.py\n");
        return -1;
    }
    PyObject* pDict = PyModule_GetDict(pModule);
    if ( !pDict )
    {
        return -1;
    }
    PyObject* result;
    int a = 0;
    int b = 0;
    PyObject* pFunc = PyDict_GetItemString(pDict, "add");
    if ( !pFunc || !PyCallable_Check(pFunc) )
    {
        printf("can't find function add\n");
        return -1;
    }

    // 参数进栈
    PyObject* pArgs;
    pArgs = PyTuple_New(2);
    a = 3;
    b = 4;
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("l",a));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("l",b));

    // 调用Python函数
    result = PyObject_CallObject(pFunc, pArgs);
    printf("%d add %d == %d\n", a, b, PyLong_AsLong(result));


    pFunc = PyDict_GetItemString(pDict, "mul");
    if ( !pFunc || !PyCallable_Check(pFunc) )
    {
        printf("can't find function mul\n");
        return -1;
    }

    pArgs = PyTuple_New(2);
    a = 2;
    b = 3;
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("l",a));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("l",b));

    result = PyObject_CallObject(pFunc, pArgs);
    printf("%d mul %d == %d\n", a, b, PyLong_AsLong(result));

    pFunc = PyDict_GetItemString(pDict, "power");
    if ( !pFunc || !PyCallable_Check(pFunc) ) {
        printf("can't find function power\n");
        getchar();
        return -1;
     }
    pArgs = PyTuple_New(2);
    a = 2;
    b = 3;
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("l",a));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("l",b));
    result = PyObject_CallObject(pFunc, pArgs);
    printf("%d power %d == %d\n", a, b, PyLong_AsLong(result));

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);

    // 关闭Python
    Py_Finalize();
    return 0;
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.
    13.
    14.
    15.
    16.
    17.
    18.
    19.
    20.
    21.
    22.
    23.
    24.
    25.
    26.
    27.
    28.
    29.
    30.
    31.
    32.
    33.
    34.
    35.
    36.
    37.
    38.
    39.
    40.
    41.
    42.
    43.
    44.
    45.
    46.
    47.
    48.
    49.
    50.
    51.
    52.
    53.
    54.
    55.
    56.
    57.
    58.
    59.
    60.
    61.
    62.
    63.
    64.
    65.
    66.
    67.
    68.
    69.
    70.
    71.
    72.
    73.
    74.
    75.
    76.
    77.
    78.
    79.
    80.
    81.
    82.
    83.
    84.
    85.
    86.
    87.
    88.
    89.
    90.
    91.

G++编译：
g++ -I/home/user/anaconda3/include/python3.7m -c main.cpp
链接：
g++ -o main main.o -L/usr/local/lib -lpython3.7m -lrt -lpthread -lutil -ldl
五、ctypes扩展
1、ctypes简介

ctypes是Python的一个可以链接C/C的库，可以将C/C函数编译成动态链接库，即window下的.dll文件或者是linux下的.so文件，通过使用cytpes可以直接调用动态连接库的C/C函数，加速代码的运行速度。
ctypes的优点如下：
（1）不要修改动态库的源码
（2）只需要动态库和头文件
（3）使用比较简单，而且目前大部分库都兼容C/C

import platform
from ctypes import *


if __name__ == '__main__':
    if platform.system() == 'Windows':
        libc = cdll.LoadLibrary('msvcrt.dll')
    elif platform.system() == 'Linux':
        libc = cdll.LoadLibrary('libc.so.6')

    libc.printf(bytes('Hello world!\n', 'utf-8'))

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.

ctypes作为连接Python和C的接口,，其对应的数据类型如下：
Python3快速入门（十七）——Python扩展模块开发_Python 扩展模块_02
Python 中的类型，除了 None，int， long， Byte String，Unicode String 作为 C 函数的参数默认提供转换外，其它类型都必须显式提供转换。None：对应 C 中的 NULL，int、long对应 C 中的 int，具体实现时会根据机器字长自动适配。
在python3中，Byte String对应 C 中的一个字符串指针char *，指向一块内存区域，通常字符串前面需要加小b,  b"helloworld"；Unicode String对应 C 中一个宽字符串指针 wchar_t *，指向一块内存区域，Python3中对应的是字符串，如"helloworld"。
2、Python调用C动态连接库

编写C语言函数add.c文件：

#include <stdlib.h>

int add(int a, int b)
{
    return a + b;
}

    1.
    2.
    3.
    4.
    5.
    6.

使用GCC编译C语言文件：
gcc -o libadd.so -shared -fPIC add.c
如果使用g++编译生成C动态库的代码中的函数时，需要使用extern "C"来进行编译。
Python调用C语言动态链接库：

import ctypes


if __name__ == '__main__':
    loader = ctypes.cdll.LoadLibrary
    lib = loader("./libadd.so")
    result = lib.add(1, 2)
    print(result)

# output:
# 3

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.

3、Python调用C++动态连接库

需要extern "C"来辅助，也就是说还是只能调用C函数，不能直接调用方法，但是能解析C方法。如果不用extern “C”，构建后的动态链接库将没有函数的符号表。
编写C语言函数add.cpp文件：

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

class Utils
{
public:
    static void display(const char* name, int age)
    {
        printf("%s %d\n", name, age);
    }
};


extern "C"{
    void display(const char* name, int age)
    {
        Utils::display(name, age);
    }
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.
    13.
    14.
    15.
    16.
    17.
    18.
    19.
    20.
    21.
    22.

G编译：
g++ -o libadd.so -shared -fPIC add.cpp
Python调用C语言动态链接库：

import ctypes


if __name__ == '__main__':
    loader = ctypes.cdll.LoadLibrary
    lib = loader("./libadd.so")
    name = bytes("Bauer", 'utf-8')
    lib.display(name, 28)

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.

4、Python调用C/C++可执行程序

main.cpp文件：

#include <stdio.h>

void display()
{
    printf("Hello CPython\n");
}

int main(int argc, const char* argv[])
{
    display();
    return 0;
}

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.
    9.
    10.
    11.
    12.

G++编译：
g++ -o main main.cpp
Python调用可执行程序：

import os


if __name__ == '__main__':
    os.system("./main")

    1.
    2.
    3.
    4.
    5.

六、SWIG扩展
1、SWIG简介

SWIG（Simplified Wrapper and Interface Generator）是用来为脚本语言调用C和C程序的软件开发工具，实际上是一个编译器，获取C/C的声明和定义，用一个壳封装起来，以便其它脚本语言访问。SWIG 最大的好处就是将脚本语言的开发效率和 C/C++ 的运行效率有机的结合起来。
SWIG安装：
pip install swig
2、编写C/C++模块

utils.h文件如下：

#include <stdio.h>

class utils
{
public:
    static void display();
};

    1.
    2.
    3.
    4.
    5.
    6.
    7.

utils.cpp文件如下：

#include "utils.h"

void utils::display()
{
    printf("Hello SWIG\n");
}

    1.
    2.
    3.
    4.
    5.
    6.

3、编写规则转换接口文件

swig封装需要一个.i后缀文件的封装说明。
%module <name>为封装名称，Python调用包名是<name>。
%{…%}内是附加的函数说明和头文件，源文件以外的部分都要包括在内，包括头文件和宏定义等。
utils.i文件如下：

%module utils
%{
/* Includes the header in the wrapper code */
#include "utils.h"
%}
/* Parse the header file to generate wrappers */
%include "utils.h"

    1.
    2.
    3.
    4.
    5.
    6.
    7.

4、生成封装文件

C++文件命令如下：
swig -python -c++ utils.i
C文件命令如下：
swig -python utils.i
swig会生成两个不同的文件：utils_wrap.cxx（c源码是utils_wrap.c）和python文件utils.py。
5、编译生成模块

编写setup.py文件：

from distutils.core import setup, Extension

utils_module = Extension('_utils', sources=['utils.cpp', 'utils_wrap.cxx'])
setup(name='utils', version='0.1', author="bauer", description="""Simple swig C++/Python example""",
      ext_modules=[utils_module], py_modules=["utils"])

    1.
    2.
    3.
    4.
    5.

可以使用include_dirs指定搜索的头文件路径，library_dirs指定搜索的库路径。
swig生成的扩展模块对象名必须使用python模块名并在前面加上下划线_，通过swig生成的python文件是utils.py，模块对象名必须是’_utils’,否则无法顺利编译。
在当前目录下编译生成Python模块：
python setup.py build_ext --inplace
当前目录下生成模块如下：
_utils.cpython-37m-x86_64-linux-gnu.so
6、Python扩展模块使用

import utils


if __name__ == '__main__':
    utils.utils_display()

# output:
# Hello SWIG

    1.
    2.
    3.
    4.
    5.
    6.
    7.
    8.


-----------------------------------
©著作权归作者所有：来自51CTO博客作者天山老妖S的原创作品，如需转载，请注明出处，否则将追究法律责任
Python3快速入门（十七）——Python扩展模块开发
https://blog.51cto.com/u_9291927/2450914
