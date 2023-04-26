# pybind11 库的使用笔记

## 安装

[pybind11](https://github.com/pybind/pybind11)
![](https://upload-images.jianshu.io/upload_images/11478104-7204921406d809d2.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)
![](https://upload-images.jianshu.io/upload_images/11478104-619f976bc22d377f.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)
![](https://upload-images.jianshu.io/upload_images/11478104-a1affaa0fe4fdf20.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)
![](https://upload-images.jianshu.io/upload_images/11478104-5ccb2e72bc738a30.png?imageMogr2/auto-orient/strip|imageView2/2/w/1153/format/webp)

### 下载pybind11
~~~ shell
git clone https://github.com/pybind/pybind11.git
~~~

### 安装pytest
~~~ shell
pip3 install pytest
~~~

### 编译安装
~~~ shell
mkdir build
cd build
cmake ..
cmake --build . --config Release --target check
~~~
## 使用指南

### 一些函数示例
- 头文件和命名空间
~~~ c++
#include <pybind11/pybind11.h>
namespace py = pybind11;
~~~
- 绑定简单函数
~~~ c++
#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i, int j)
{
    return i + j;
}

PYBIND11_MODULE(example, m)
{
    m.doc() = "pybind11 example plugin"; // 可选的模块说明

    m.def("add", &add, "A function which adds two numbers");
}
~~~
PYBIND11_MODULE()宏函数将会创建一个函数，在由Python发起import语句时该函数将会被调用。模块名字“example”，由宏的第一个参数指定（千万不能出现引号）。第二个参数"m"，定义了一个py::module的变量。函数py::module::def()生成绑定代码，将add()函数暴露给Python。
`简单编译:`

> c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` example.cpp -o example `python3-config --extension-suffix`   
> g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` example.cpp -o example`python3-config --extension-suffix`



python简单使用
~~~ python
import example
example.add(1, 2)
~~~
- 关键字参数
~~~ c++
m.def("add", &add, "A function which adds two numbers", py::arg("i"), py::arg("j"));
~~~
py::arg是众多特殊标签之一，它们能将元数据（metadata）传递给py::module::def()。通过这个简单的修改，我们可以利用“keyword arguments”来调用函数了。
~~~ python
import example
# 参数的名字也将出现在文档的函数签名中。
help(example)
example.add(i=1, j=2)
~~~
还可以用较短的命名参数表示法：
~~~ c++
using namespace pybind11::literals;
m.def("add2", &add, "i"_a, "j"_a);
~~~
_a后缀是一个C++11的字面字，与py::arg是等价的。
- 默认参数


不幸的是，pybind11不能自动地提取这些参数，因为他们（默认参数）不是函数的类型信息。不过可以利用py::arg扩展来很很简单的实现这些特性。下面例子将展示pybind11对默认参数的支持：
~~~ c++
PYBIND11_MODULE(example, m)
{
    m.doc() = "pybind11 example plugin"; // 可选的模块说明
    // default argument
    m.def("add3", &add, "A function which adds two numbers", py::arg("i") = 1, py::arg("j") = 2);
}
~~~
现在在python中使用带默认参数的add函数:
~~~ python
>>> import example
>>> help(example.add3)
Help on built-in function add3 in module example:

add3(...) method of builtins.PyCapsule instance
    add3(i: int = 1, j: int = 2) -> int

    A function which adds two numbers

>>> example.add3()
3
>>> example.add3(3)
5
>>> example.add3(j=3)
4
>>>
~~~
- 导出变量

在pybind11通过py::module::attr() 函数实现从C++中导出变量到Python中。内建的类型和泛型对象在被指定为属性（attribute）时将会被自动的转换，同事也可以使用函数py::module::cast进行显示的转换。
~~~ c++
PYBIND11_MODULE(example, m)
{
    m.attr("the_answer") = 42;
    py::object world = py::cast("World");
    m.attr("what") = world;
}
~~~
~~~ python
>>> import example
>>> example.the_answer
42
>>> example.what
'World'
>>>
>>> example.the_answer = 100
>>> example.the_answer
100
>>>
~~~
- 支持数据类型
大量可以开箱即用的数据类型，可以无缝地用作函数参数、返回值或者被py::cast用来转换。这部分内容将在类型转换章节（Type conversions）进行介绍。
|  python类型  |  C++_pyton类型  |  C++原生类型  |
|  ---  |  ---   |  ---  |
| list   |  py::list  |  std::vector<int>  |





### pybind11重要函数
- py::module::attr() 
该函数实现从C++中导出变量到Python中.

- py::module::cast
该函数进行显示的转换



### c++ 扩展demo1 hello world
~~~ c++
#include<pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(example, m) {

    m.doc() = "pybind11 example module";

    // Add bindings here
    m.def("foo", []() {
        return "Hello, World!";
    });

}
~~~
~~~ python
#文件名：setup.py
from setuptools import setup, Extension  
  
functions_module = Extension(  
    name = 'functions',  
    sources = ['add.cpp', 'sub.cpp', 'mul.cpp', 'div.cpp', 'functions_wrapper.cpp'],  
    include_dirs = [r'D:\software\pybind11-master\include', 
                    r'D:\software\Anaconda\include']  
)  
setup(ext_modules = [functions_module])
~~~







### c++ 扩展demo2 更多的调用程序
~~~ c++
#include<pybind11/pybind11.h>

namespace py = pybind11;


PYBIND11_MODULE(example, m) {

    m.doc() = "pybind11 example module";

    // Add bindings here
    m.def("foo", []() {
        return "Hello, World!";
    });

    m.def("foo2", []() {
        return "This is foo2!\n";
    });

    m.def("add", [](int a, int b) {
        return a + b;
    });

    m.def("sub", [](int a, int b) {
        return a - b;
    });

    m.def("mul", [](int a, int b) {
        return a * b;
    });

    m.def("div", [](int a, int b) {
        return static_cast<float>(a) / b;
    });

}
~~~

### c++ 扩展demo3 绑定命名空间外的函数

~~~ c++
#include<iostream>
#include<pybind11/pybind11.h>

namespace py = pybind11;


/*
file:///D:/pybind11-master/docs/.build/html/basics.html
*/

# if 1

int add(int a, int b) {
    return a + b;
}

int add2(int a, int b) {
    return a + b;
}

int add3(int a, int b) {
    return a + b;
}


PYBIND11_MODULE(demo2, m) {
    
    m.doc() = "example module";
    //函数名称， 函数指针， 描述
    m.def("add", &add, "A function which adds two numbers");
    
    // keyword arguments
    //py::arg("a")
    m.def("add2", &add2, "A function which adds two numbers", py::arg("a"), py::arg("b"));

    //default arguments
    m.def("add3", &add3, "A function which adds two numbers", py::arg("a") = 10, py::arg("b") = 5);

    //Exporting variables
    m.attr("num1") = 100;
    py::object world = py::cast("World");
    m.attr("what") = world;

}

#endif
~~~

### c++ 扩展demo4 类扩展1
编写python扩展
~~~ c++
class Student {

public:
    std::string name;
    std::string gender;
    int age;
    
public:
    Student() {}
    Student(std::string name, std::string gender, int age) {
        this->name = name;
        this->gender = gender;
        this->age = age;
    }
    ~Student() {};

public:
    //setter
    void setName(std::string name) {
        this->name = name;
    }

    void setGender(std::string gender) {
        this->gender = gender;
    }

    void setAge(int age) {
        this->age = age;
    }

    //getter
    std::string getName() {
        return this->name;
    }

    std::string getGender() {
        return this->gender;
    }

    int getAge() {
        return this->age;
    }

public:
    //声明静态成员
    static std::string university;

    // static method
    static std::string getUniversity() {
        return university;
    }

};

// 定义并且初始化static变量
std::string Student::university = "Xidian University";

PYBIND11_MODULE(example, m) {
    //py::class_<Pet>(m, "Pet")
    //  .def(py::init<const std::string &>())
    //  .def("setName", &Pet::setName)
    //  .def("getName", &Pet::getName);

    // Studnet
    py::class_<Student>(m, "Student")
        .def(py::init<std::string, std::string, int>())
        .def("setName", &Student::setName)
        .def("setGender", &Student::setGender)
        .def("setAge", &Student::setAge)
        .def("getName", &Student::getName)
        .def("getGender", &Student::getGender)
        .def("getAge", &Student::getAge)
        .def_static("getUniversity", &Student::getUniversity);  //def_static() 静态方法

}

~~~
### c++ 扩展demo4 类扩展2
~~~ c++

#include<iostream>
#include<pybind11/pybind11.h>

namespace py = pybind11;

class Person
{

public:
    std::string habbit;             //兴趣爱好

private:
    std::string name;               //姓名
    std::string gender;             //性别
    int age;                        //年龄
    std::string idCardNum;          //身份证号码
    std::string address;            //住址

public:
    void setName(std::string name) {
        this->name = name;
    }

    void setAge(int age) {
        this->age = age;
    }

    void setGender(std::string gender) {
        this->gender = gender;
    }

    void setAddress(std::string address) {
        this->address = address;
    }

    void setIdCardNum(std::string idCardNum) {
        this->idCardNum = idCardNum;
    }

    std::string getName() {
        return this->name;
    }

    std::string getGender() {
        return this->gender;
    }

    int getAge() {
        return this->age;
    }

    std::string getAddress() {
        return this->address;
    }

    std::string getIdCardNum() {
        return this->idCardNum;
    }


public:
    //
    static std::string university;
    static std::string getUniversity() {
        return university;
    }

public:
    Person() {};
    Person(std::string name, std::string gender, int age) {
        this->name = name;
        this->gender = gender;
        this->age = age;
    }
    ~Person() {};

};

std::string Person::university = "xidian University";


PYBIND11_MODULE(demo3, m) {

    m.doc() = "Simple Class";
    
    py::class_<Person>(m, "Person")
        .def(py::init())
        .def(py::init<std::string, std::string, int>())
        .def("setName", &Person::setName)
        .def("setAge", &Person::setAge)
        .def("setGender", &Person::setGender)
        .def("setAddress", &Person::setAddress)
        .def("getName", &Person::getName)
        .def("getAge", &Person::getAge)
        .def("getGender", &Person::getGender)
        .def("getAddress", &Person::getAddress)
        .def_static("getUniversity", &Person::getUniversity)
        .def_readonly_static("university", &Person::university)
        .def_readwrite("habbit", &Person::habbit)
        .def_property("name", &Person::getName, &Person::setName)
        .def_property("age", &Person::getAge, &Person::setAge)
        .def_property("gender", &Person::getGender, &Person::setGender)
        .def_property("address", &Person::getAddress, &Person::setAddress)
        .def_property("idCardNum", &Person::getIdCardNum, &Person::setIdCardNum);
}
~~~
### c++ 扩展demo5
~~~ c++
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

/*
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};


/*
module: examplelib 
target: examplelib

cpp: example.cpp
*/
PYBIND11_MODULE(examplelib, m)
{
    // optional module docstring
    m.doc() = "pybind11 example plugin";

    // FUNCTIONS
    // expose add function, and add keyword arguments and default arguments
    m.def("add", &add, "A function which adds two numbers", py::arg("i") = 1, py::arg("j") = 2);

    // DATA
    // exporting variables
    m.attr("the_answer") = 42;
    py::object world = py::cast("World");
    m.attr("what") = world;


    // CLASSES
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);

    /*
    python3 
    > help(examplelib)
    */
}
~~~
~~~ cmake
cmake_minimum_required (VERSION 2.6)

project (pybind)
enable_language(C)
enable_language(CXX)

find_package(pybind11 CONFIG REQUIRED)
include_directories(${pybind11_INCLUDE_DIRS})
message([MAIN] "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIRS}")

MESSAGE( [Main] " pybind11_INCLUDE_DIRS = ${pybind11_INCLUDE_DIRS}")
MESSAGE( [Main] " pybind11_LIBRARIES = ${pybind11_LIBRARIES}")

#
#   # Create an extension module
#   add_library(mylib MODULE main.cpp)
#   target_link_libraries(mylib pybind11::module)
#
#   # Or embed the Python interpreter into an executable
#   add_executable(myexe main.cpp)
#   target_link_libraries(myexe pybind11::embed)

# method (1): generate `examplelib.pyd`
pybind11_add_module(examplelib example.cpp)

# method (2): generate `examplelib.dll` rename to `examplelib.pyd`
#add_library(examplelib MODULE example.cpp)
#target_link_libraries(examplelib pybind11::module)

MESSAGE( [Main] " pybind11_INCLUDE_DIRS = ${pybind11_INCLUDE_DIRS}")
MESSAGE( [Main] " pybind11_LIBRARIES = ${pybind11_LIBRARIES}")

#add_executable(cpp_use_python cpp_use_python.cpp)
#target_link_libraries(cpp_use_python PRIVATE pybind11::embed)
~~~
### c++ 扩展demo6 c++调用python
~~~ c++
#include <pybind11/embed.h>  
#include <iostream>  

namespace py = pybind11;

int main() {
    py::scoped_interpreter python;

    /*
    import sys
    print sys.path
    print "Hello,World!"
    */
    py::module sys = py::module::import("sys");
    py::print(sys.attr("path"));
    py::print("Hello, World!"); // use the Python API

    /*
    import example
    n = example.add(1,2)
    */
    py::module example = py::module::import("example");
    py::object result = example.attr("add")(1, 2);
    int n = result.cast<int>();
    assert(n == 3);
    std::cout << "result from example.add(1,2) = " << n << std::endl;

    /*
    from example import MyMath
    obj = MyMath("v0")
    obj.my_add(1,2)
    */
    py::object MyMath = py::module::import("example").attr("MyMath"); // class
    py::object obj = MyMath("v0"); // class object
    py::object my_add = obj.attr("my_add");// object method
    py::object result2 = my_add(1, 2); // result
    int n2 = result2.cast<int>(); // cast from python type to c++ type
    assert(n2 == 3);
    std::cout << "result from obj.my_add(1,2) = " << n2 << std::endl;

    /*
    from example import MyMath
    obj = MyMath("v0")
    obj.my_strcon("abc","123");
    */

    py::object my_strcon = obj.attr("my_strcon"); // object method
    py::object result3 = my_strcon("abc", "123");
    std::string str3 = result3.cast<std::string>();
    std::cout << "result from obj.my_strcon(abc,123) = " << str3 << std::endl;

    return 0;
}
~~~
~~~ cmake
cmake_minimum_required (VERSION 2.6)

project (pybind)
enable_language(C)
enable_language(CXX)

find_package(pybind11 CONFIG REQUIRED)
include_directories(${pybind11_INCLUDE_DIRS})

MESSAGE( [Main] " pybind11_INCLUDE_DIRS = ${pybind11_INCLUDE_DIRS}")
MESSAGE( [Main] " pybind11_LIBRARIES = ${pybind11_LIBRARIES}")

add_executable(cpp_use_python cpp_use_python.cpp)
target_link_libraries(cpp_use_python PRIVATE pybind11::embed)
~~~

### c++ 扩展demo7 c++调用python
~~~ c++
//main.cpp
#include<pybind11/embed.h>
#include<iostream>
namespace py=pybind11;
int main() {
py::scoped_interpreter python;
//查看系统路径
py::module sys=py::module::import("sys");
py::print(sys.attr("path"));
//用import函数导入python模块
auto module=py::module::import("calc");
//调用函数时要用attr（）进行类型转换
module.attr("add")(1,2);
return 0;
}
~~~
~~~ python
//calc.py
def add(i,j)：
print(“Hi”)
return i+j
~~~
~~~ cmake
//CMakelists.txt
cmake_minimum_required (VERSION 2 .8)
project(pf_test)
SET(CMAKE_CXX_FLAGS "-std=c++11")
#指定自己的python库路径
link_directories(~/anaconda/anaconda3/lib)
#将自己的python头文件包含进来
INCLUDE_DIRECTORIES(~/anaconda/anaconda3/include/python3.7m)
#将自己pybind11头文件路径包含进来
INCLUDE_DIRECTORIES(~/pybind/pybind11-2.3.0/include/pybind11)
#连接到两个动态库文件
link_libraries(/home/wangzq/anaconda/ anaconda3/pkgs/python-3.7.4-h265db76_1/lib/lib/python3.7m.so)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/lib/pybind11_cross_ _module_tests.cpython-37m-x86_64-1inux-gnu.so)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/1ib/pybind11_tests. cpython-37m-x86_64-1inux-gnu.so)
add_executable(pf_test main.cpp calc.py)
#  g++ `python3 -m pybind11 --includes` `python3-config --cflags` `python3-config --ldflags` main.cpp -o example -lpython3.10
~~~
### c++ 扩展demo9 c++调用python类
~~~ c++
//mian.cpp
#include <pybind11/ embed. h>
#include <iostream>
#include <pybind11/st1.h>
namespace py = pybind11;
int main()
{
    py::scoped_ interpreter python;
    py::print("hello world");
    //用py::object导入python类模块并进行类的初始化
    py::object Dog = py : : module : : import("c_ test").attr("Dog");
    py::object obj = Dog("miao", "sit");
    // function_ test
    py::object add = obj.attr("add");
    py::object result2 = add(2, 4);
    //注：python返回值到c++中需要类型转换，用cast<数据类型>()来进行转换
    int n = result2.cast<int>();
    std::cout << n << std : : endl;
    //list_ test  集合类型
    py::object type_ test = obj.attr("type_ test");
    py::object result3 = type_ test(2);
    py::list v[5] = result3.cast < py : : list > ();
    std::cout << v[2] << std : : endl;
    .std::cout << v << std : : endl;
    //dict_ test   字典类型
    py::object dict_ test = obj.attr("dict_ test");
    py::object result4 = dict_ test(1);
    py::dict dd = result4.cast < py : : dict > ();
    std::cout << dd << std : : endl;
    //set_ test  列表类型
    py::object set_ test = obj.attr("set_ test");
    py::object result5 = set_ test(3);
    py::set ss = result5.cast < py : : set > ();
    std::cout << ss << std : : endl;
    //tuple_ test  元组类型测试
    py::object tup1e_test = obj.attr("tuple_test");
    py::object result 6 = tuple_test(1);
    Py::tuple tt = result6.cast < py : : tuple > ();
    std::cout << tt << std : : endl;
    return 0;
}

~~~
~~~ python
# //c_test.py
class Dog:
    def __init__(self, name, action):
        self._name = name
        self.action = action
    def get_name(self):
        return self._name
    def get_action(self):
        return self.action
    def add(self, i, j):
        print("Hi ")
        return i+j
    def type_test(self, a):
        n = list()
        if a == 1:
            n = {1, 2, 3}
        else:
            n = {5, 5, 5}
        print("type test")
        return n
    def dict_test(self, b):
        d = dict()
        if b == 1:
            d = {"wo": 2, "ni": 3}
        else:
            d = {"hei": 3, "ha": 5}
        return d
    def set_test(self, c):
        e = set()
        if c == 1:
            e = {1, 2, 3, 3}
        else:
            e = {2, 3, 3, 3}
        return e
    def tuple_test(self, d):
        f = tup1e()
        if d == 1:
            f = (5, 6, 7)
        else:
            f = (7, 8, 9)
        return f
# //注意换行 格式乱了。。。

~~~

~~~ C++
  int my_test(py::object objx)
  {
    // auto mk = py::type(objx);
    // auto mk = py::int(3);
    // py::type(std::move(objx))
    // py::print(py::str(objx));
    // py::print(py::str(py::type(objx)));
    // py::scoped_interpreter python;

    py::print("nihao");
    auto ty = objx.get_type();
    cout << ty<< endl;
    // cout << mk<< endl;
    // if(py::isinstance<py::bytes>(objx))
    // {
    //   cout << "---bytes!"<<endl;
    // }
    // // py::int a(3);
    // for(int i =0;i<objx.cast<int>();i++)
    // {
    //   cout <<"zhuanhuan!"<<endl;
    // }
  // py::scoped_interpreter python;
    auto mk=py::module::import("PIL.Image").attr("Image");
    py::print(mk);
    // auto ma = py::type(mk);
    if(py::isinstance(objx,mk))
    {
      cout << "PIL.Image.Image"<<endl;
      auto mdd = objx.attr("size").cast<vector<int>>();
      cout << mdd[0]  << mdd[1]<< endl;
      // auto md = mk.attr("tobytes");
      // auto img_bytes = md(objx);
    // cout << objx.size<<endl;
    


      // py::print(img_bytes);
    }
~~~

