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



