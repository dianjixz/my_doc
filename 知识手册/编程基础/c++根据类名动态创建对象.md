 

# [C++根据类名动态创建对象](https://www.cnblogs.com/onStateChange/p/6590543.html)

参考的文章：

http://blog.csdn.net/jnu_simba/article/details/9318799

http://blog.csdn.net/xiaoxiaoyusheng2012/article/details/45438899

 

按照网上大部分人的思路，将类名和创建的函数保存成一个map，然后创建对象时根据类型匹配创建函数即可，我自己整理的代码如下：

1、object.h

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 #ifndef OBJECT_H
 2 #define OBJECT_H
 3 
 4 #include <string>
 5 #include <map>
 6 
 7 typedef void* (*Constructor)();
 8 
 9 class CObjectFactory
10 {
11 public:
12     static void registerClass(std::string className, Constructor constructor)
13     {
14         constructors()[className] = constructor;
15     }
16 
17     static void* createObject(const std::string& className)
18     {
19         Constructor constructor = NULL;
20 
21         if(constructors().find(className) != constructors().end())
22             constructor = constructors().find(className)->second;
23 
24         if ( constructor == NULL )
25             return NULL;
26 
27         return (*constructor)();
28     }
29 
30 private:
31     inline static std::map<std::string, Constructor>& constructors()
32     {
33         static std::map<std::string, Constructor> instance;
34         return instance;
35     }
36 };
37 
38 
39 #define REGISTER_CLASS(class_name) \
40 class class_name##Helper { \
41 public: \
42     class_name##Helper() \
43     { \
44         CObjectFactory::registerClass(#class_name, class_name##Helper::creatObjFunc); \
45     } \
46     static void* creatObjFunc() \
47     { \
48         return new class_name; \
49     } \
50 }; \
51 class_name##Helper class_name##helper;
52 
53 
54 #endif
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

主要的设计思路都在这里体现了：

　　a、设计一个工厂类，用于动态创建对象，根据前面的思路，需要实现两个接口，即注册类名称和创建函数的接口、动态创建的接口，且都申明为static；

　　b、没有直接使用类的static变量保存map变量，而是使用了一个static函数，函数内部申明一个static变量map，这样的好处是整个代码都可以在头文件里实现，不需要额外的cpp文件。因为如果采用类的static变量，需要再建一个cpp文件初始化这个变量;

　　c、REGISTER_CLASS宏用于注册，它创建了一个辅助类和一个对应的辅助类对象

2、test.h

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 #ifndef TEST_H
 2 #define TEST_H
 3 
 4 
 5 #include <iostream>
 6 
 7 class CClassTest
 8 {
 9 public:
10     CClassTest()
11     {
12         std::cout<<"CClassTest\n";
13     }
14 };
15 
16 class CClassTest2
17 {
18 public:
19     CClassTest2()
20     {
21         std::cout<<"CClassTest2\n";
22     }
23 };
24 
25 
26 
27 
28 #endif
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

主要用于测试，比较简单

3、test.cpp

```
1 #include "test.h"
2 #include "object.h"
3 
4 REGISTER_CLASS(CClassTest)
5 REGISTER_CLASS(CClassTest2)
```

注册类的宏应放到cpp文件中，如果放到头文件中会报错。

4、main.cpp

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 #include "object.h"
 2 #include "test.h"
 3 #include <iostream>
 4 
 5 int main()
 6 {
 7     CClassTest* test = static_cast<CClassTest*>(CObjectFactory::createObject("CClassTest"));
 8     CClassTest2* test2 = static_cast<CClassTest2*>(CObjectFactory::createObject("CClassTest2"));
 9 
10     delete test;
11     delete test2;
12 
13     return 0;
14 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

直接根据类名称调用接口即可得到对象。
