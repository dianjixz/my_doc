 # C++编程基础


## 类使用
~~~ C++
#include <iostream>
 
using namespace std;
 
class Line
{
   public:
      void setLength( double len );
      double getLength( void );
      Line();   // 这是构造函数声明
      ~Line();  // 这是析构函数声明
 
   private:
      double length;
};
 
// 成员函数定义，包括构造函数
Line::Line(void)
{
    cout << "Object is being created" << endl;
}
Line::~Line(void)
{
    cout << "Object is being deleted" << endl;
}
 
void Line::setLength( double len )
{
    length = len;
}
 
double Line::getLength( void )
{
    return length;
}
// 程序的主函数
int main( )
{
   Line line;
 
   // 设置长度
   line.setLength(6.0); 
   cout << "Length of line : " << line.getLength() <<endl;
 
   return 0;
}
~~~


## STL使用笔记

### Vector的创建与初始化方法
- 方法一
~~~ c++
vector<int> list1;
//默认初始化，vector 为空， size 为0。
~~~
- 方法二
~~~ c++
vector<int> list2(list);

vector<int> list2 = list;
//两种方式等价 ，list2 初始化为 list 的拷贝。
~~~
- 方法三
~~~ c++
vector<int> list = {1,2,3.0,4,5,6,7};
 
vector<int> list3 {1,2,3.0,4,5,6,7};
//list 初始化为列表中元素的拷贝，列表中元素必须与 list 的元素类型相容。
~~~
- 方法四
~~~ c++
vector<int> list3(list.begin() + 2, list.end() - 1);
//默认值初始化，list 中将包含7个元素，每个元素进行缺省的值初始化。
~~~
- 方法五
~~~ c++
vector<int> ilist4(7);
//默认值初始化，list 中将包含7个元素，每个元素进行缺省的值初始化。
~~~
- 方法六
~~~ c++
vector<int> ilist5(7, 3)
//指定值初始化，ilist5被初始化为包含7个值为3的int。
~~~



## c++11随记：std::bind及 std::placeholders

一 使用场景
先将可调用的对象保存起来，在需要的时候再调用，是一种延迟计算的思想。不论是普通函数、函数对象、还是成员函数，成员变量都可以绑定，其中成员函数都可以绑定是相当灵活的。

二 头文件
定义于头文件

~~~ c++
#include <functional>
~~~

三 bind原型
~~~ c++
a.）

template< class F, class... Args >
/*unspecified*/ bind( F&& f, Args&&... args );

b.)

template< class R, class F, class... Args >
/*unspecified*/ bind( F&& f, Args&&... args );
~~~ 
四 绑定普通函数
~~~ c++
// 头文件

#include <functional>
#include <iostream>

// 定义函数

int TestFunc(int a, char c, float f)
{
    std::cout << a << std::endl;
    std::cout << c << std::endl;
    std::cout << f << std::endl;
    return a;
}
~~~
绑定函数：
~~~ c++
    auto fun1 = std::bind(TestFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    auto fun2 = std::bind(TestFunc, std::placeholders::_2, std::placeholders::_3, std::placeholders::_1);
    auto fun3 = std::bind(TestFunc, std::placeholders::_1, std::placeholders::_2, 98.77);
~~~
使用：
~~~ c++
  fun1(30, 'C',100.1);
  fun2(100.1, 30, 'C');
  fun3(30,'C');
~~~
说明：
fun1说明)
占位符->第一个参数和函数第一个参数匹配（int）,第二个参数和第二个参数匹配（char）,第三个参数和第三个参数匹配
fun2说明)
显然，可以通过占位符调整顺序，fun2绑定说明
占位符->第二个参数和函数第一个参数匹配（int）,第三个参数和第二个参数匹配（char）,第一个参数和第三个参数匹配
fun3说明：
占位符->第一个参数和函数第一个参数匹配（int）,第二个参数和第二个参数匹配（char）,第三个参数默认为98.77
如果第三个参数也要填的话，会被忽略掉
~~~ c++
  fun3(30,'C',8.9);
~~~
上面的参数8.9将会忽略掉

五 绑定成员函数
定义一个测试类：
~~~ c++
class TestClass
{
public:
    int ClassMember(int a) { return 55+a; }
    int ClassMember2(int a,char ch,float f)
    {
        std::cout <<ch <<" "<< f << " "<<a<<std::endl;
        return 55+a;
    }
    static int StaticMember(int a) { return 66+a; }
};

~~~ 
先看一个简单的绑定
~~~ c++
    TestClass test;
    auto fun4 = std::bind(&TestClass::ClassMember,test,std::placeholders::_1);
    fun4(4);
~~~
通过对象test绑定，和绑定普通函数一样，用一个占位符占用绑定位置，当有多个参数时：
~~~ c++
 auto fun5 = std::bind(&TestClass::ClassMember2,test,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
 fun5(1,'A',3.1);
~~~
绑定静态成员函数就更简单了
~~~ c++
    auto fun6 = &TestClass::StaticMember;
    fun6(3);
~~~
auto的类型
~~~ c++
    std::function<int(int)> fun7 = &TestClass::StaticMember;
~~~
六 绑定成员变量
定义一个有public成员变量的测试类
~~~ c++
class TestClass2
{
public:
    TestClass2()
        :m_a(100)
    {}
public:
    int m_a;
};
~~~
绑定成员变量
~~~ c++
 TestClass2 test2;
 auto fun8 = std::bind(&TestClass2::m_a,std::placeholders::_1);
~~~
使用
~~~ c++
  int var =fun8(test2);
  std::cout<<var<<std::endl;

~~~
七 绑定仿函数
定义测试类
~~~ c++
class TestClass3
{
public:
    TestClass3()=default;
    TestClass3(const TestClass3& obj)
    {
        std::cout<<"TestClass3 copy construct."<<std::endl;
    }
    void operator()(int a)
    {
        std::cout<<a<<std::endl;
    }
};
~~~
绑定使用
~~~ c++
 TestClass3 test3;
 auto fun9 = test3;
 fun9(2018);
~~~
这里多定义了一个拷贝构造函数，多验证了一个东西，即绑定时调用了拷贝构造
即用test3拷贝构造了一个新的对象，并绑定这个对象，所以fun9是新的对象，跟test3没关系了。
实际上其他的类成员函数也是一样的，拷贝构造完再绑定

八 绑定成员函数，是拷贝构造新对象再绑定
修改下TestClass
~~~ c++
class TestClass
{
public:
    TestClass(int a):m_a(a){}
    TestClass(const TestClass& obj)
    {
        m_a = obj.m_a+100;
        std::cout<<"copy construct."<<std::endl;
    }
    int ClassMember(int a) { std::cout<<" this:"<<this<<" :"<<&m_a<<" "<<m_a<<std::endl;return 55+a; }
    int ClassMember2(int a,char ch,float f)
    {
        std::cout <<ch <<" "<< f << " "<<a<<std::endl;
        return 55+a;
    }
    static int StaticMember(int a) {return 66+a; }
public:
    int m_a;
};;

~~~
测试：
~~~ c++
    std::cout<<"------"<<std::endl;
    TestClass test(67);
    std::cout<<"&test "<<&test<<" "<<test.m_a<<" &test.m_a "<<&test.m_a<<std::endl;
    auto fun4 = std::bind(&TestClass::ClassMember,test,std::placeholders::_1);
    fun4(4);
    fun4(4);
    std::cout<<"------"<<std::endl;
~~~
输出
~~~ bash
------
&test 0x63fde4 67 &test.m_a 0x63fde4
copy construct.
 this:0x63fde0 :0x63fde0 167
 this:0x63fde0 :0x63fde0 167
------
~~~
显然调用了拷贝构造，指针地址也都不一样。
绑定静态成员函数就是直接绑定，没用拷贝构造
~~~ c++
   std::cout<<"-----"<<std::endl;
   auto fun6 = &TestClass::StaticMember;
   fun6(3);
   std::cout<<"-----"<<std::endl;

~~~
输出
~~~ bash
-----
-----
~~~

同样的，绑定成员对象也没有拷贝构造
~~~ c++
   std::cout<<"------"<<std::endl;
   auto fun44 = std::bind(&TestClass::m_a,std::placeholders::_1);
   std::cout<<"TT:"<<fun44(test)<<" "<<test.m_a<<std::endl;
   std::cout<<"------"<<std::endl;
~~~
输出
~~~ bash
------
TT:67 67
------
~~~

九 通过指针，取消拷贝构造
先定义一个对象，再利用std::function保存成员函数，在调用时候把test对象传进去即可，测试代码：
~~~ c++
  TestClass test(67);
  std::cout<<"############"<<std::endl;
  std::function<int(TestClass*,int)> fg = &TestClass::ClassMember;
  fg(&test,5);
  std::cout<<"#################"<<std::endl;
// 通过引用保存，取消拷贝构造
  std::function<int(TestClass&,int)> hj = &TestClass::ClassMember;
  TestClass tt(8);
  std::cout<<&tt<<" "<<&tt.m_a<<std::endl;
  hj(tt,45);
~~~
输出：
~~~ bash
0x62fde8 0x62fde8
 this:0x62fde8 :0x62fde8 8
~~~
看到this和m_a的地址是一样的，不写了
————————————————
版权声明：本文为CSDN博主「但行技术」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_43333380/article/details/82935291



未知类型 std::any，代替 C 的 void*

某种类型 std::optional，代替 C 的类型指针

回调函数 std::function，代替 C 的函数指针

某些类型 std::variant，代替 union



~~~ c++
#include<iostream>
#include<any>
#include<vector>
#include<set>
using namespace std;
int main() {
	any Int = 69,		//整型
		Double = 8.8,	//浮点型
		CStr = "hello",			//字符数组
		Str = string("world!");		//string类
	vector<any> anys = { Int,Double,CStr,Str };	//可直接放入序列容器中
	//set < any > any_set{ Int,Double,CStr,Str };	//Error:不能直接放入关联容器内,需要提供operator<，但 any 很难比较
	cout << *Int._Cast<int>() << endl;	//T* _Cast<typenampe T>():返回any对象的"值"，但需要一个模板参数确定对象值的类型
	cout << *Double._Cast<double>() << endl;
	cout << *CStr._Cast<char const *>() << endl;
	cout << *Str._Cast<string>() << endl;
	return 0;
}
//OutPut:
//69
//8.8
//hello
//world!

————————————————
版权声明：本文为CSDN博主「Italink」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_40946921/article/details/90646022
~~~

工厂模式




类构造
```c++
// 类构造
class String
{
public:
    String(const char *str = NULL);	// 构造函数
    String(const String &str);		// 拷贝构造函数
    ~String();						// 析构函数

    String operator+(const String &str) const;	// 重载+
    String& operator=(const String &str);		// 重载=
    String& operator+=(const String &str);		// 重载+=
    bool operator==(const String &str) const;	// 重载==
    char& operator[](int n) const;				// 重载[]

    int getLength() const;		//获取长度

    friend istream& operator>>(istream &is, String &str); // 输入
    friend ostream& operator<<(ostream &os, String &str); // 输出

private:
    char *data;		//字符串
};



```
```c++
#include <iostream>
#include <cstring>

using namespace std;

// 实现 String 类
class String
{
public:
    // 普通构造函数
    String(const char *str = NULL) {
    if (str == NULL) {
        m_data = new char[1];
        m_data = '\0';
    } else {
        int length = strlen(str);

        m_data = new char[length + 1];

        strcpy(m_data, str);
    }			
    }

    // 拷贝构造函数,深复制
    String(const String &other) {
    if (!other.m_data) {
        m_data = NULL;
    }

    m_data = new char[strlen(other.m_data) + 1];

    strcpy(m_data, other.m_data);
    }

    // 析构函数
    ~String() {
    if (m_data) {
        delete[] m_data;
        m_data = NULL;
    }
    }

    // 重载 + 字符串连接, 不能返回引用
    String operator+(const String &other) const {
    String newString;

    delete[] newString.m_data;

    if (!other.m_data) {
        newString = *this;	
    } else if (!m_data) {
        newString = other;
    } else {
        newString.m_data = new char[strlen(m_data) + strlen(other.m_data) + 1];
        strcpy(newString.m_data, m_data);
        strcat(newString.m_data, other.m_data);
    }

    return newString;
    }

    // 重载 = 赋值
    String& operator=(const String &other) {
    if (this != &other) {
        delete[] m_data;

        if (!other.m_data) {
        m_data = NULL;
        } else {
        m_data = new char[strlen(other.m_data) + 1];
        strcpy(m_data, other.m_data);
        }
    }

    return *this;
    }

    // 重载 ==
    bool operator==(const String &other) const {
    if (strlen(m_data) != strlen(other.m_data)) {
        return false;
    } else {
            return strcmp(m_data, other.m_data) ? false : true;
    }
    }

    // 重载 +=
    String& operator+=(const String &other) {
        char *newData = new char[strlen(m_data) + strlen(other.m_data) + 1];

    strcpy(newData, m_data);
    strcat(newData, other.m_data);

    delete[] m_data;

    m_data = newData;

    return *this;
    }

    // 重载 []
    char& operator[](int n) const {
    if (n >= strlen(m_data)) {
        return m_data[strlen(m_data) - 1];
    }

    return m_data[n];
    }

    // 获取长度
    int getLength() const {
    return strlen(m_data);
    }

    // 输入, 重载输入操作，需要先申请一块内存，用于存放输入字符串
    friend istream& operator>>(istream &is, String &str) {
    char strTemp[100];

    memset(strTemp, 0, sizeof(strTemp));

    is >> strTemp;

    str.m_data = new char[strlen(strTemp) + 1];

    strcpy(str.m_data, strTemp);

    return is;
    }

    // 输出
    friend ostream& operator<<(ostream &os, String &str) {
    os << str.m_data;

    return os;
    }

private:
    char *m_data;
};

// 测试
void test(){
    String s;

    cin >> s;

    cout << s << " : " << s.getLength() << endl;

    
    String s1(s);
    
    cout << s1 << " : " << s1.getLength() << endl;

    const char *str = "Hello";

    String s2(str);

    cout << s2 << " : " << s2.getLength() << endl;


    char str1[] = "world!";

    String s3(str1);

    cout << s3 << " : " << s3.getLength() << endl;

    String s4 = s3;

    if (s4 == s3) {
    cout << "s3 == s4" << endl;
    } else {
    cout << "s3 != s4" << endl;
    }

    String s5 = s3 + s4;

    cout << s5 << " : " << s5.getLength() << endl;
    
    s3 += s4;

    if (s5 == s3) {
    cout << "s5 == s3" << endl;
    } else {
    cout << "s5 != s3" << endl;
    }

    cout << s5[5] << endl;
}

int main()
{
    test();
    return 0;
}

```






















