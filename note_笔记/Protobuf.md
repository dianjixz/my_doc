# protobuf详解

**1 protobuf 简介**

`protobuf` (protocol buffer) 是谷歌内部的混合语言数据标准。通过将结构化的数据进行序列化(串行化)，用于通讯协议、数据存储等领域的语言无关、平台无关、可扩展的序列化结构数据格式。

**我们说的 `protobuf` 通常包括下面三点:**

- 一种二进制数据交换格式。可以将 C++ 中定义的存储类的内容 与 二进制序列串 相互转换，主要用于数据传输或保存
- 定义了一种源文件，扩展名为 `.proto`(类比`.cpp`文件)，使用这种源文件，可以定义存储类的内容
- protobuf有自己的编译器 `protoc`，可以将 `.proto` 编译成`.cc`文件，使之成为一个可以在 C++ 工程中直接使用的类

> **序列化**：将数据结构或对象转换成二进制串的过程。**反序列化**：将在序列化过程中所产生的二进制串转换成数据结构或对象的过程。

## **2 定义proto文件**

### **2.1 `message` 介绍**

`message`：`protobuf`中定义一个消息类型是通过关键字`message`字段指定的，这个关键字类似于C++/Java中的class关键字。使用protobuf编译器将`proto`编译成C++代码之后，每个`message`都会生成一个名字与之对应的C++类，该类公开继承自`google::protobuf::Message`。

### **2.2 `message` 消息定义**

创建`tutorial.person.proto`文件，文件内容如下：

```text
// FileName: tutorial.person.proto 
// 通常文件名建议命名格式为 包名.消息名.proto 

// 表示正在使用proto2命令
syntax = "proto2"; 

//包声明，tutorial 也可以声明为二级类型。
//例如a.b，表示a类别下b子类别
package tutorial; 

//编译器将生成一个名为person的类
//类的字段信息包括姓名name,编号id,邮箱email，
//以及电话号码phones
message Person { 

  required string name = 1;  // (位置1)
  required int32 id = 2;  
  optional string email = 3;  // (位置2)

  enum PhoneType {  //电话类型枚举值 
    MOBILE = 0;  //手机号  
    HOME = 1;    //家庭联系电话
    WORK = 2;    //工作联系电话
  } 
  
  //电话号码phone消息体
  //组成包括号码number、电话类型 type
  message PhoneNumber {
    required string number = 1;    
    optional PhoneType type = 
          2 [default = HOME]; // (位置3)
  }  
  
  repeated PhoneNumber phones = 4; // (位置4)
} 


// 通讯录消息体，包括一个Person类的people
message AddressBook { 
  repeated Person people = 1; 

}
```

### **2.3 字段解释**

### **2.3.1 包声明**

`proto` 文件以`package`声明开头，这有助于防止不同项目之间命名冲突。在C++中，以`package`声明的文件内容生成的类将放在与包名匹配的`namespace`中，上面的`.proto`文件中所有的声明都属于`tutorial`。

### **2.3.2 字段规则**

- `required`:消息体中必填字段，不设置会导致编解码异常。（例如位置1）
- `optional`: 消息体中可选字段，可通过default关键字设置默认值。（例如位置2）
- `repeated`: 消息体中可重复字段，重复的值的顺序会被保留（例如位置3）。其中，proto3默认使用packed方式存储，这样编码方式比较节省内存。

### **2.3.3 标识号**

- `标识号`：在消息体的定义中，每个字段都必须要有一个唯一的标识号，标识号是[0,2^29-1]范围内的一个整数。以Person为例，name=1，id=2, email=3, phones=4 中的1-4就是标识号。

### **2.3.4 数据定义**

许多标准的**简单数据类型**都可以用作`message`字段类型，包括`bool`,`int32`,`float`,`double`和`string`。还可以使用**其他`message`类型作为字段类型**在消息体中添加更多结构。在上面的示例中，`Person`包含`PhoneNumber message`, 而`AddressBook`包含`Person message`。甚至可以**定义嵌套在其他message中的message类型**。例如，上面的`PhoneNumber`定义在`Person`。

### **2.3.5 函数方法**

用`message`关键字声明的的消息体，允许你检查、操作、读、或写整个消息，包括解析二进制字符串，以及序列化二进制字符串。除此之外，也定义了下列方法：

```text
Person:缺省的构造函数。

~Person():缺省的析构函数。

Person(const Person& other):拷贝构造函数。

Person& operator=(const Person& other):
赋值 (Assignment ）操作符。

const UnknownFieldSet& unknown_fields() const:
返回当解析信息时遇到的未知字段的集合。

UnknownFieldSet* mutable_unknown_fields():
返回当前解析信息时遇到的未知字段的集合的一个mutale指针。
```

## **3 编译proto文件**

可以执行以下`protoc`命令对`.proto`文件进行编译，生成对应的c文件。Linux系统通过 `help protoc` 查看`protoc`命令的使用详解。

```text
protoc -I=$SRC_DIR 
--cpp_out=$DST_DIR  xxx.proto 
```

- `$SRC_DIR:proto` 所在的源目录
- `--cpp_out` 生成C++代码
- `$DST_DIR` 生成代码的目标目录
- `xxx.proto`:要针对哪个`proto`文件生成接口，例如 `tutorial.person.proto`

编译完成后，将生成2个文件 `tutorial.pb.h`和`tutorial.pb.c` 其中`tutorial`表示包名，`pb是protobuf`的缩写。

此外，**`protocol buffer` 编译器为`.proto`文件中定义的消息的每个字段生成一套存取器方法**：

对于 `message Person` 中的 `required int32 id = 2`,编译器将生成下列存取器方法：

`bool has_id() const`: 用于判断字段`id`是否存在。如果字段被设置，返回true。

`int32 id() const`: 返回字段`id`的当前值，如果字段没有被设置，返回缺省值。

`void set_id(int32 value)` : 设置字段`id`的值。调用此方法后，`has_id()` 将返回`true`以及`id()` 将返回`value`。

`void clear_id()`:清除字段的值。调用此方法后，`has_id()`将返回`false` 以及`id()`将返回缺省值。

当然，对于其他类型的字段，编译器也会生成不同的存取方法，这里就不一一列举了。

## **4 使用message**

```text
//获取person实例：

tutorial::AddressBook addressBook;
Person *person = 
addressBook.add_people(); 

//写入一个message:

person->set_id(id);
getline(cin,*person->mutable_name());

//读取一个message: 

person.id();
person.name();
person.email(); 
```

**扩展一个message**:

- 你不得更改任何现有字段的字段编号
- 你可以删除`optional`或`repeated`属性的字段
- 你可以添加新的`optional`或`repeated`字段，但必须使用新的标记

## **5 protobuf的优点**

**性能方面**

- 序列化后，数据大小可缩小3倍
- 序列化速度快
- 传输速度快

**使用方面**

- 使用简单：`proto`编译器自动进行序列化和反序列化
- 维护成本低：多平台只需要维护一套对象协议文件，即`.proto`文件
- 可扩展性好：不必破坏旧的数据格式，就能对数据结构进行更新
- 加密性好：http传输内容抓包只能抓到字节数据

**使用范围**

- 跨平台、跨语言、可扩展性强

## **6 安装 protoc 编译工具**

- ubunutu

~~~ bash
apt  install protobuf-compiler
~~~

- other linux

~~~ bash
git clone https://github.com/google/protobuf.git

./autogen.sh

./configure

make

make install
~~~

- Windows

~~~ 
下载地址：
https://github.com/protocolbuffers/protobuf/releases
~~~

![在这里插入图片描述](https://img-blog.csdnimg.cn/8132ba7586d146e2b2a46b3666db0782.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAdml2aXNvbA==,size_20,color_FFFFFF,t_70,g_se,x_16)

安装`protoc-gen-go`

[安装方式一](https://grpc.io/docs/languages/go/quickstart/)

https://grpc.io/docs/languages/go/quickstart/

[安装方式二：](https://go-zero.dev/protoc-install.html)

https://go-zero.dev/protoc-install.html



## **7 总结**

C++ `protocol buffers` 已经做了极大优化，当然它的用途不仅仅是简单的访问器和序列化，如果运用好自己的想象力，可以将`protobuf`应用于更广泛的问题中。

转载自公众号【程序媛的被窝】[protobuf详解](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzkxMzI5NzA3Ng%3D%3D%26mid%3D2247484026%26idx%3D1%26sn%3D989e7b2b4abfcdb406637b6bce78d561%26chksm%3Dc17e99a1f60910b70d16cbee5e0ec2bed4f7a5db7be9da053707ffb8e34e37ad840c7ef87137%26token%3D1931356267%26lang%3Dzh_CN%23rd)