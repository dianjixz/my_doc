 CMakeLists 中常用的预定义变量            

**常用路径变量**

PROJECT_NAME：通过PROJECT指定的项目名称

```
    project(Demo)
复制代码
```

PROJECT_SOURCE_DIR：工程的根目录，上图中的Demo目录

PROJECT_BINARY_DIR：执行cmake命令的目录，一般是在build目录，在此目录执行cmake ..

CMAKE_CURRENT_SOURCE_DIR： 当前CMakeLists.txt文件所在目录

CMAKE_CURRENT_BINARY_DIR： target编译目录，可使用ADD_SUBDIRECTORY来修改此变量

```
# 添加cmake执行子目录
ADD_SUBDIRECTORY(example)
复制代码
```

EXECUTABLE_OUTPUT_PATH：重新定义目标链接库文件的输出位置

```
# 设置可执行文件的输出路径为 build/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
复制代码
```

LIBRARY_OUTPUT_PATH：库文件输出位置

```
    set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
复制代码
```

**常用系统信息变量**

CMAKE_MAJOR_VERSION：cmake的主版本号cmake version 3.11.2中的3

CMAKE_MINOR_VERSION：cmake的次版本号cmake version 3.11.2中的11

CMAKE_PATCH_VERSION：cmake的补丁等级cmake version 3.11.2中的2

 ![在这里插入图片描述](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/6258c5a227ec467bb2cb5b72d64df6ca~tplv-k3u1fbpfcp-zoom-in-crop-mark:1304:0:0:0.awebp)

CMAKE_SYSTEM：系统名称，带版本号

CMAKE_SYSTEM_NAME：系统名称，不带版本号

CMAKE_SYSTEM_VERSION：系统版本号

CMAKE_SYSTEM_PROCESSOR：处理器名称

**编译选项：**

BUILD_SHARED_LIBS：默认的库编译方式(shared or static)，默认为static，一般在ADD_LIBRARY时直接指定编译库的类型

CMAKE_C_FLAGS：设置C编译选项

CMAKE_CXX_FLAGS：设置C++编译选项

CMAKE_CXX_FLAGS_DEBUG：设置编译类型为Debug时的编译选项

CMAKE_CXX_FLAGS_RELEASE：设置编译类型为Release时的编译选项

CMAKE_CXX_COMPILER：设置C++编译器

```cpp
# 设置C++编译器为g++
set(CMAKE_CXX_COMPILER "g++")
# 设置标准库版本为c++17 并开启警告
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
# 设置Debug模式下，不开启优化，开启调试，生成更详细的gdb调试信息
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")
# 设置Release模式下，开启最高级优化
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
复制代码
```

到此 CMakeLists 中常用的预定义变量介绍完成。


作者：地表最强菜鸡
链接：https://juejin.cn/post/6939387279513223175
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

#### 基本语法

一个最基本的CmakeLists.txt文件最少需要包含以下三行：

```
cmake_minimum_required (VERSION 2.6)
project (Tutorial)
add_executable(Tutorial tutorial.cxx)
复制代码
```

注意：cmake的语法支持大小、小写和大小写混合上边的代码中我们使用的cmake语法是小写的.

```
cmake_minimum_required
CMAKE_MINIMUM_REQUIRED
cmake_MINUMUM_required
复制代码
```

上面三种写法是相同的，注意，只有系统指令是不区分大小写的，但是变量和字符串是区分大小写的。

创建一个tutorial.cxx文件，用来计算一个数字的平方根，内容如下:

```
// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main (int argc, char *argv[])
{
  if (argc < 2)
    {
    fprintf(stdout,"Usage: %s number\n",argv[0]);
    return 1;
    }
  double inputValue = atof(argv[1]);
  double outputValue = sqrt(inputValue);
  fprintf(stdout,"The square root of %g is %g\n",
          inputValue, outputValue);
  return 0;
}
复制代码
```

这样就完成一个最简单的cmake程序。

#### 构建程序

用cmake来编译这段代码，进入命令行执行内部构建命令（后边会讲外部构建）：

```
cmake .
复制代码
```

这是输出一系列的log信息

```
-- The C compiler identification is AppleClang 9.0.0.9000039
-- The CXX compiler identification is AppleClang 9.0.0.9000039
-- Check for working C compiler: /Library/Developer/CommandLineTools/usr/bin/cc
-- Check for working C compiler: /Library/Developer/CommandLineTools/usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /Library/Developer/CommandLineTools/usr/bin/c++
-- Check for working CXX compiler: /Library/Developer/CommandLineTools/usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/saka/Desktop/Tutorial/Step1
复制代码
```

同时生成了三个文件`CMakeCache.txt`、`Makefile`、`cmake_install.cmake`和一个文件夹`CmakeFiles`,然后执行

```
make 
复制代码
```

即可生成可执行程序`Tutorial`。在ubuntu或者centos上可能会提示找不到`math.h`文件,这时候我们需要在cmakeLists.txt文件中最后添加

```
target_link_libraries(Tutorial apue.a)
复制代码
```

然后重新编译即可。需要删除刚才生成的额外的文件。

#### 添加版本号

下面讲解如何为程序添加版本号和带有使用版本号的头文件。

`set(KEY VALUE)`接受两个参数，用来声明变量。在camke语法中使用`KEY`并不能直接取到`VALUE`,必须使用`${KEY}`这种写法来取到`VALUE`。

```
cmake_minimum_required (VERSION 2.6)
project (Tutorial)
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable
add_executable(Tutorial tutorial.cxx)
复制代码
```

配置文件将会被写入到可执行文件目录下，所以我们的项目必须包含这个文件夹来使用这些配置头文件。我们需要在工程目录下新建一个`TutorialConfig.h.in`,内容如下：

```
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
复制代码
```

上面的代码中的`@Tutorial_VERSION_MAJOR@`和`@Tutorial_VERSION_MINOR@`将会被替换为`CmakeLists.txt`中的1和0。 然后修改`Tutorial.cxx`文件如下，用来在不输入额外参数的情况下输出版本信息：

```
// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TutorialConfig.h"
 
int main (int argc, char *argv[])
{
  if (argc < 2)
    {
    fprintf(stdout,"%s Version %d.%d\n",
            argv[0],
            Tutorial_VERSION_MAJOR,
            Tutorial_VERSION_MINOR);
    fprintf(stdout,"Usage: %s number\n",argv[0]);
    return 1;
    }
  double inputValue = atof(argv[1]);
  double outputValue = sqrt(inputValue);
  fprintf(stdout,"The square root of %g is %g\n",
          inputValue, outputValue);
  return 0;
}
复制代码
```

然后执行

```
cmake .
make
./Tutorial
复制代码
```

即可看到输出内容:

![img](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2018/1/29/161425f0d0feccdb~tplv-t2oaga2asx-zoom-in-crop-mark:1304:0:0:0.awebp)


作者：saka
链接：https://juejin.cn/post/6844903557183832078
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

#### 构建自己的库

这个库将包含我们自己计算一个数字的平方根的计算方法。生成的程序可以使用这个库，而不是由编译器提供的标准平方根函数(`math.h`)。

在本教程中，我们将把库放到一个名为`mathfunction`的子目录中,在工程目录下新建`mathfunction`文件夹。这个文件夹中新建CMakeLists.txt文件，包含以下一行代码:

```
add_library(MathFunctions mysqrt.cxx)
复制代码
```

然后在这个文件夹中创建源文件`mysqrt.cxx`，它只有一个名为`mysqrt`的函数，与编译器的sqrt函数提供了类似的功能。

为了利用新库，我们在工程根目录下的`CMakeLists.txt`中添加`add_subdirectory()`来构建我们自己的库。我们还添加了另一个include目录,以便`MathFunctions / MathFunctions.h`可以为函数原型找到头文件,该文件代码如下：

```
double mysqrt(double x);
复制代码
```

然后创建`mysqrt.cxx`文件，内容如下

```
#include "MathFunctions.h"
#include <stdio.h>

// a hack square root calculation using simple operations
double mysqrt(double x)
{
  if (x <= 0) {
    return 0;
  }

  double result;
  double delta;
  result = x;

  // do ten iterations
  int i;
  for (i = 0; i < 10; ++i) {
    if (result <= 0) {
      result = 0.1;
    }
    delta = x - (result * result);
    result = result + 0.5 * delta / result;
    fprintf(stdout, "Computing sqrt of %g to be %g\n", x, result);
  }
  return result;
}
复制代码
```

最后一个更改是将新库添加到可执行文件。根目录下CMakeLists.txt的最后添加以下代码

```
include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
add_subdirectory (MathFunctions) 
 
# add the executable
add_executable (Tutorial tutorial.cxx)
target_link_libraries (Tutorial MathFunctions)
复制代码
```

现在文件目录如下

```
.
├── CMakeLists.txt
├── MathFunctions
│   ├── CMakeLists.txt
│   ├── MathFunctions.h
│   └── mysqrt.cxx
├── TutorialConfig.h.in
└── tutorial.cxx
复制代码
```

#### 构建可选选项

`MathFunctions`是我们自己构建的库，有时候我们需要控制这个库是否应该使用，那么可以为使用这个库添加一个开关，在构建大型项目时非常有用。

在项目根目录下的`CMakeLists.txt`文件中添加如下代码：

```
# should we use our own math functions?
option (USE_MYMATH 
        "Use tutorial provided math implementation" ON)
复制代码
```

假如你使用的是CMake GUI，`USE_MYMATH`默认值是用户可以根据需要更改。该设置将存储在缓存中，以便用户在每次运行CMake时生成默认配置。然后我们就可以选择性的构建和使用mathfunction库。修改根目录下CMakeLists.txt:

```
# add the MathFunctions library?
#
if (USE_MYMATH)
  include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
  add_subdirectory (MathFunctions)
  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)
 
# add the executable
add_executable (Tutorial tutorial.cxx)
target_link_libraries (Tutorial  ${EXTRA_LIBS})
复制代码
```

这将使用`USE_MYMATH`的设置来确定是否应该编译和使用mathfunction库。注意，使用一个变量(在本例中是EXTRA_LIBS)来设置可选的库，然后将它们链接到可执行文件中。这是一种常见的方法，用于保持较大的项目具有许多可选组件。 首先在Configure.h.in文件中添加以下内容：

```
#cmakedefine USE_MYMATH
复制代码
```

然后我们就可以使用`USE_MYMATH`这个变量了，最后修改`Tutorial.cxx`源代码如下：

```
// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TutorialConfig.h"
#ifdef USE_MYMATH
#include "MathFunctions.h"
#endif
 
int main (int argc, char *argv[])
{
  if (argc < 2)
    {
    fprintf(stdout,"%s Version %d.%d\n", argv[0],
            Tutorial_VERSION_MAJOR,
            Tutorial_VERSION_MINOR);
    fprintf(stdout,"Usage: %s number\n",argv[0]);
    return 1;
    }
 
  double inputValue = atof(argv[1]);
 
#ifdef USE_MYMATH
  double outputValue = mysqrt(inputValue);
#else
  double outputValue = sqrt(inputValue);
#endif
 
  fprintf(stdout,"The square root of %g is %g\n",
          inputValue, outputValue);
  return 0;
}
复制代码
```

我们编译执行看以下结果

1. 使用自定义的库(USE_MYMATH=ON)

```
 ~/Desktop/Tutorial/Step2/ ./Tutorial 4
Computing sqrt of 4 to be 2.5
Computing sqrt of 4 to be 2.05
Computing sqrt of 4 to be 2.00061
Computing sqrt of 4 to be 2
Computing sqrt of 4 to be 2
Computing sqrt of 4 to be 2
Computing sqrt of 4 to be 2
Computing sqrt of 4 to be 2
Computing sqrt of 4 to be 2
Computing sqrt of 4 to be 2
The square root of 4 is 2
复制代码
```

1. 不适用自定义的库(USE_MYMATH=OFF)

```
 ~/Desktop/Tutorial/Step2/ ./Tutorial 4
The square root of 4 is 2
复制代码
```

可以看到，这个开关达到了我们需要的效果。


作者：saka
链接：https://juejin.cn/post/6844903557196414989
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

#### 设置安装规则

安装规则相当简单。对于mathfunction库，我们设添加了这个库，通过将以下两行添加到mathfunction的CMakeLists.txt中来安装头文件和静态库:

```
install (TARGETS MathFunctions DESTINATION bin)
install (FILES MathFunctions.h DESTINATION include)
复制代码
```

然后根目录下的CMakeLusts.txt文件中添加如下行,用来安装可执行文件和配置文件：

```
# add the install targets
install (TARGETS Tutorial DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"        
         DESTINATION include)
复制代码
```

注意上边install的第一个参数和第三个参数。

`TARGETS`包含六种形式：`ARCHIVE`, `LIBRARY`, `RUNTIME`, `OBJECTS`, `FRAMEWORK`,  `BUNDLE`。注意Mathfunction安装的是`LIBRARY`，而根目录下的可执行文件是`RUNTIME`类型。

`FILE` 将给定的文件复制到指定目录。如果没有给定权限参数，则由该表单安装的文件默认为`OWNER_WRITE`、`OWNER_READ`、`GROUP_READ`和`WORLD_READ`。

TARGETS和FILE可指定为相对目录和绝对目录。

`DESTINATION`在这里是一个相对路径，取默认值。在unix系统中指向 `/usr/local` 在windows上`c:/Program Files/${PROJECT_NAME}`。

也可以通过设置`CMAKE_INSTALL_PREFIX`这个变量来设置安装的路径，那么安装位置不指向`/usr/local`，而指向你所指定的目录。

```
cmake .
make
makeinstall
复制代码
```

执行完毕后即安装了软件。

示例中指向的安装地址如下表所示:

| 文件/库          | 安装位置            |
| ---------------- | ------------------- |
| MathFunctions    | /usr/local/bin/     |
| MathFunctions.h  | /usr/local/include/ |
| Tutorial         | /usr/local/bin/     |
| TutorialConfig.h | /usr/local/include  |

#### 添加测试

添加测试也非常简单，在根目录下的CMakeLists.txt文件最后添加如下代码来测试输入参数后产生的结果是否正确。

```
include(CTest)

# does the application run
add_test (TutorialRuns Tutorial 25)
# does it sqrt of 25
add_test (TutorialComp25 Tutorial 25)
set_tests_properties (TutorialComp25 PROPERTIES PASS_REGULAR_EXPRESSION "25 is 5")
# does it handle negative numbers
add_test (TutorialNegative Tutorial -25)
set_tests_properties (TutorialNegative PROPERTIES PASS_REGULAR_EXPRESSION "-25 is 0")
# does it handle small numbers
add_test (TutorialSmall Tutorial 0.0001)
set_tests_properties (TutorialSmall PROPERTIES PASS_REGULAR_EXPRESSION "0.0001 is 0.01")
# does the usage message work?
add_test (TutorialUsage Tutorial)
set_tests_properties (TutorialUsage PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number")
复制代码
```

构建项目完成后，可以运行`ctest`命令行工具来运行测试。第一个测试只是验证应用程序是否正常运行，没有发生崩溃，并且返回值是0。这是CTest测试的基本形式。接下来的几个测试都使用`PASS_REGULAR_EXPRESSION`测试属性来验证测试的输出是否包含特定的字符串。

如果您想要添加许多测试来测试不同的输入值，您可以考虑创建如下的宏(相当于函数):

```
#define a macro to simplify adding tests, then use it
macro (do_test arg result)
  add_test (TutorialComp${arg} Tutorial ${arg})
  set_tests_properties (TutorialComp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)
 
# do a bunch of result based tests
do_test (25 "25 is 5")
do_test (-25 "-25 is 0")
复制代码
```

构建项目后执行`ctest`可得到如下结果：

```
 ~/Desktop/Tutorial/Step3/build/ ctest
Test project /Users/saka/Desktop/Tutorial/Step3/build
    Start 1: TutorialRuns
1/5 Test #1: TutorialRuns .....................   Passed    0.00 sec
    Start 2: TutorialComp25
2/5 Test #2: TutorialComp25 ...................   Passed    0.01 sec
    Start 3: TutorialNegative
3/5 Test #3: TutorialNegative .................   Passed    0.00 sec
    Start 4: TutorialSmall
4/5 Test #4: TutorialSmall ....................   Passed    0.00 sec
    Start 5: TutorialUsage
5/5 Test #5: TutorialUsage ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 5

Total Test time (real) =   0.03 sec
复制代码
```

可以看到，测试全部通过。

#### 添加系统自检

有时候我们可能会为多种平台开发程序，而有的平台包含某个库，有的平台不包含这个库，那么我们就可以通过系统自检来判断是使用平台系统提供的库还是自己编写的库。

下边我们将添加一些依赖于目标平台是否具有log和exp函数的代码。当然，几乎每个平台都有这些功能，但本教程假设它们不太常见。如果平台有log库，那么我们将使用它来计算平方根，而不适用mysqrt中的函数。

首先使用CheckFunctionExists.cmake宏来测试这些函数的是否存在。在顶级CMakeLists.txt文件中编写代码如下:

```
# does this system provide the log and exp functions?
include (CheckFunctionExists)
check_function_exists (log HAVE_LOG)
check_function_exists (exp HAVE_EXP)
复制代码
```

然后在`TutorialConfig.h.in`中添加使用上边定义的变量的代码：

```
// does the platform provide exp and log functions?
#cmakedefine HAVE_LOG
#cmakedefine HAVE_EXP
复制代码
```

有一点需要注意：log和exp的测试代码一定要在`TutorialConfig.h`的`configure_file`命令之前。`configure.file`命令会立即使用CMake中的当前设置配置文件。最后，在mysqrt函数中，我们可以提供一个基于log和exp的替代实现，如果它们在系统上可用以下代码可用:

```
// if we have both log and exp then use them
#if defined (HAVE_LOG) && defined (HAVE_EXP)
  result = exp(log(x)*0.5);
#else // otherwise use an iterative approach
  ...
```


作者：saka
链接：https://juejin.cn/post/6844903558102401031
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

有时候我们的文件不是一开始就编写好的，而是通过在编译过程中来生成文件比如某个日志，构建时间等，等文件生成后我们需要将这个文件再添加到应用程序的构建过程中。

下边，我们将创建一个预先计算的平方根表作为构建过程的一部分，程序可以通过查询这个表来输出对应的数值，然后将该表编译到我们的应用程序中。为了实现这一点，我们首先需要一个生成该表的程序。在`mathfunction`子目录中，新建一个文件，名称为MakeTable.cxx，内容如下：

```
// A simple program that builds a sqrt table 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int main (int argc, char *argv[])
{
  int i;
  double result;
 
  // make sure we have enough arguments
  if (argc < 2)
    {
    return 1;
    }
  
  // open the output file
  FILE *fout = fopen(argv[1],"w");
  if (!fout)
    {
    return 1;
    }
  
  // create a source file with a table of square roots
  fprintf(fout,"double sqrtTable[] = {\n");
  for (i = 0; i < 10; ++i)
    {
    result = sqrt(static_cast<double>(i));
    fprintf(fout,"%g,\n",result);
    }
 
  // close the table with a zero
  fprintf(fout,"0};\n");
  fclose(fout);
  return 0;
}
复制代码
```

注意，该表是作为有效的c++代码生成的，输出的文件名称是以参数形式传入的。下一步是将合适的命令添加到`mathfunction`的CMakeLists.txt文件中来构建MakeTable的可执行文件，然后将其作为构建过程的一部分运行。添加如下命令：

```
# first we add the executable that generates the table
add_executable(MakeTable MakeTable.cxx)
 
# add the command to generate the source code
add_custom_command (
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  )
 
# add the binary tree directory to the search path for 
# include files
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
 
# add the main library
add_library(MathFunctions mysqrt.cxx ${CMAKE_CURRENT_BINARY_DIR}/Table.h  )
复制代码
```

首先，`add_executable(MakeTable MakeTable.cxx)`添加了`MakeTable`这个可执行文件。然后我们添加一个自定义命令来指定通过运行`MakeTable`来生成平方根表(Table.h)，注意`add_custom_command`的第二个参数`COMMAND`,相当于执行MakeTable并传入参数`${CMAKE_CURRENT_BINARY_DIR}/Table.h`。接下来我们要让CMake知道mysqrt.cxx依赖于生成的文件表(Table.h)。这是通过添加生成的Table.h文件到MathFunctions库来实现的。我们还必须将当前的二进制目录添加到包含目录的列表中，因为生成的表在二进制目录中，这样库就可以找到并包含在mysqrt.cxx中。当这个项目被构建时，它将首先构建`MakeTable`可执行文件。然后它将运行`MakeTable`命令生成`Table.h`文件。最后，它将编译mysqrt.cxx和table.h生成mathfunction库。

根目录下的的CMakeLists.txt文件如下:

```
cmake_minimum_required (VERSION 2.6)
project (Tutorial)
include(CTest)
 
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)
 
# does this system provide the log and exp functions?
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
 
check_function_exists (log HAVE_LOG)
check_function_exists (exp HAVE_EXP)
 
# should we use our own math functions
option(USE_MYMATH 
  "Use tutorial provided math implementation" ON)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories ("${PROJECT_BINARY_DIR}")
 
# add the MathFunctions library?
if (USE_MYMATH)
  include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
  add_subdirectory (MathFunctions)
  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)
 
# add the executable
add_executable (Tutorial tutorial.cxx)
target_link_libraries (Tutorial  ${EXTRA_LIBS})
 
# add the install targets
install (TARGETS Tutorial DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"        
         DESTINATION include)
 
# does the application run
add_test (TutorialRuns Tutorial 25)
 
# does the usage message work?
add_test (TutorialUsage Tutorial)
set_tests_properties (TutorialUsage
  PROPERTIES 
  PASS_REGULAR_EXPRESSION "Usage:.*number"
  )
 
 
#define a macro to simplify adding tests
macro (do_test arg result)
  add_test (TutorialComp${arg} Tutorial ${arg})
  set_tests_properties (TutorialComp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endmacro (do_test)
 
# do a bunch of result based tests
do_test (4 "4 is 2")
do_test (9 "9 is 3")
do_test (5 "5 is 2.236")
do_test (7 "7 is 2.645")
do_test (25 "25 is 5")
do_test (-25 "-25 is 0")
do_test (0.0001 "0.0001 is 0.01")
复制代码
```

TutorialConfig.h.in文件如下：

```
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
#cmakedefine USE_MYMATH
 
// does the platform provide exp and log functions?
#cmakedefine HAVE_LOG
#cmakedefine HAVE_EXP
复制代码
```

MathFunction文件夹下的CMakeLists.txt文件内容如下:

```
# first we add the executable that generates the table
add_executable(MakeTable MakeTable.cxx)
# add the command to generate the source code
add_custom_command (
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  )
# add the binary tree directory to the search path 
# for include files
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
 
# add the main library
add_library(MathFunctions mysqrt.cxx ${CMAKE_CURRENT_BINARY_DIR}/Table.h)
 
install (TARGETS MathFunctions DESTINATION bin)
install (FILES MathFunctions.h DESTINATION include)

复制代码
```

完整的文件结构

```
 ~/Desktop/Tutorial/Step5/ tree -L 2
.
├── CMakeLists.txt
├── MathFunctions
│   ├── CMakeLists.txt
│   ├── MakeTable.cxx
│   ├── MathFunctions.h
│   └── mysqrt.cxx
├── TutorialConfig.h.in
├── build
└── tutorial.cxx
复制代码
```

然后执行外部构建命令(第六个教程里有说明如何外部构建)：

```
mkdir build
cd build
cmake ..
make
复制代码
```

构建成功后，我们预期会在二进制文件夹中的MathFunction中生成一个Table.h文件，并且文件内容是一个简单的平方根表。

来看一下结果：

```
 ~/Desktop/Tutorial/Step5/ tree -L 3 -I CMakeFiles
.
├── CMakeLists.txt
├── c
│   ├── CMakeLists.txt
│   ├── MakeTable.cxx
│   ├── MathFunctions.h
│   └── mysqrt.cxx
├── TutorialConfig.h.in
├── build
│   ├── CMakeCache.txt
│   ├── CTestTestfile.cmake
│   ├── Makefile
│   ├── MathFunctions
│   │   ├── MakeTable
│   │   ├── Makefile
│   │   ├── Table.h
│   │   ├── cmake_install.cmake
│   │   └── libMathFunctions.a
│   ├── Tutorial
│   ├── TutorialConfig.h
│   └── cmake_install.cmake
└── tutorial.cxx
复制代码
```

可以看到，确实有这个表。 然后看一下表的内容是不是0-9的平方根且最后以0结束，执行命令：

```
 ~/Desktop/Tutorial/Step5/ cat build/MathFunctions/Table.h 
double sqrtTable[] = {
0,
1,
1.41421,
1.73205,
2,
2.23607,
2.44949,
2.64575,
2.82843,
3,
0};
复制代码
```

结果正确。这种构建方式增加了蛋疼的操作，获取用python脚本或者shell脚本更容易生成，但是为了展示cmake系统构建的强大功能，一次编写到处运行，我们忍一忍吧。


作者：saka
链接：https://juejin.cn/post/6844903558207242247
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

假如想将我们的项目分发给其他人，以便他们可以安装并使用它。我们希望在各种平台上提供可执行文件或者源文件，构建支持二进制安装和包管理特性的安装包，如cygwin、debian、rpm等。为了实现这一点，我们将使用CPack来创建与CPack在章节包装中描述的平台特定安装程序。具体来说，我们需要在根目录CMakeLists.txt的底部添加几行代码：

```
# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include (CPack)
复制代码
```

首先要加入`InstallRequiredSystemLibraries`。这个模块将包括当前平台项目所需要的任何运行时库。接下来，我们将一些CPack变量,如该项目的许可和版本信息。最后，我们将加入CPack模块，它将使用刚才定义的变量变量和系统的一些其他属性来设置安装程序。 我们执行外部构建并查看生成的文件都有什么：

```
 ~/Desktop/Tutorial/Step6/build/ tree -L 1
.
├── CMakeCache.txt
├── CMakeFiles
├── CPackConfig.cmake
├── CPackSourceConfig.cmake
├── CTestTestfile.cmake
├── Makefile
├── MathFunctions
├── TutorialConfig.h
└── cmake_install.cmake
复制代码
```

注意其中两个生成的文件`CPackConfig.cmake`,`CPackSourceConfig.cmake`,一会又大用处。

下一步是按照一般的方式构建项目，然后在其上运行CPack。要构建一个二进制版本，可以运行:

```
cpack --config CPackConfig.cmake
复制代码
```

下边是日志：

```
CPack: Create package using STGZ
CPack: Install projects
CPack: - Run preinstall target for: Tutorial
CPack: - Install project: Tutorial
CPack: Create package
CPack: - package: /Users/saka/Desktop/Tutorial/Step6/build/Tutorial-1.0.1-Darwin.sh generated.
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: Tutorial
CPack: - Install project: Tutorial
CPack: Create package
CPack: - package: /Users/saka/Desktop/Tutorial/Step6/build/Tutorial-1.0.1-Darwin.tar.gz generated.
复制代码
```

从日志可以看到生成了两个打包的文件`Tutorial-1.0.1-Darwin.sh`,`Tutorial-1.0.1-Darwin.tar.gz`。查看一下目录:

```
 ~/Desktop/Tutorial/Step6/build/ tree -L 1
.
├── CMakeCache.txt
├── CMakeFiles
├── CPackConfig.cmake
├── CPackSourceConfig.cmake
├── CTestTestfile.cmake
├── Makefile
├── MathFunctions
├── Tutorial
├── Tutorial-1.0.1-Darwin.sh
├── Tutorial-1.0.1-Darwin.tar.gz
├── TutorialConfig.h
├── _CPack_Packages
├── cmake_install.cmake
└── install_manifest.txt
复制代码
```

查看一下压缩包内的文件：

```
 ~/Desktop/Tutorial/Step6/build/ tar -tf Tutorial-1.0.1-Darwin.tar.gz 
Tutorial-1.0.1-Darwin/bin/
Tutorial-1.0.1-Darwin/bin/Tutorial
Tutorial-1.0.1-Darwin/bin/libMathFunctions.a
Tutorial-1.0.1-Darwin/include/
Tutorial-1.0.1-Darwin/include/MathFunctions.h
Tutorial-1.0.1-Darwin/include/TutorialConfig.h
复制代码
```

这个是已经编译好的问答压缩而成的，解压后可以直接运行。

要构建一个源代码版本，可以运行:

```
cpack --config CPackSourceConfig.cmake
复制代码
```

日志输出基本同上，只是包内的文件不一样：

```
 ~/Desktop/Tutorial/Step6/build/ tar -tf Tutorial-1.0.1-Source.tar.gz 
Tutorial-1.0.1-Source/CMakeLists.txt
Tutorial-1.0.1-Source/TutorialConfig.h.in
Tutorial-1.0.1-Source/tutorial.cxx
Tutorial-1.0.1-Source/MathFunctions/
Tutorial-1.0.1-Source/MathFunctions/MakeTable.cxx
Tutorial-1.0.1-Source/MathFunctions/mysqrt.cxx
Tutorial-1.0.1-Source/MathFunctions/CMakeLists.txt
...
复制代码
```

当然我们也可以自己编写这两个文件定制我们生成文件的规则，不过我估计只有闲的蛋疼的人才会自己编写者两个东西。

基本的使用教程已完毕，就是这么官方，就是这么坑爹。没有讲如何加入忽略打包的文件，没有讲解如何定制打包生成的文件名称，没有讲解如何制定打包生成的压缩包的位置，没有讲解生成压缩包的类型如何指定。就是这么官方，就是这么坑爹。后边教程我来填一些坑。


作者：saka
链接：https://juejin.cn/post/6844903558551175175
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

“蛋疼的不止语法，还有文档”

#### cmake文件格式

本节讲的命令格式遵循如下语法：

| 格式        | 注释           |
| ----------- | -------------- |
| `<command>` | 必须填写的     |
| `[command]` | 可写也可不写的 |
| `a\|b`      | a或者b均可以   |

cmake能识别CMakeLists.txt和*.cmake格式的文件。cmake能够以三种方式 来组织文件：

| 类别                  | 文件格式         |
| --------------------- | ---------------- |
| `Dierctory`（文件夹） | `CMakeLists.txt` |
| `Script`（脚本）      | `<script>.cmake` |
| `Module`（模块）      | `<module>.cmake` |

本系列主要以CMakeLists.txt的语法为主要讲解内容，至于编写这蛋疼的脚本和模块假如我还写的下去的话我就写。

##### 1. Directory

当CMake处理一个项目时，入口点是一个名为`CMakeLists.txt`的源文件，这个一定是根目录下的CMakeLists.txt。这个文件包含整个工程的构建规范，当我们有多个子文件夹需要编译时，使用`add_subdirectory(<dir_name>)`命令来为构建添加子目录。添加的每个子目录也必须包含一个`CMakeLists.txt`文件作为该子目录的入口点。每个子目录的CMakeLists.txt文件被处理时，CMake在构建树中生成相应的目录作为默认的工作和输出目录。记住这一点非常关键，这样我们就可以使用**外部构建**了，而不必每次都使用蛋疼的内部构建，然后删除一堆文件才能从新构建。

##### 2. Script

一个单独的`<script>.cmake`源文件可以使用cmake命令行工具`cmake -P <script>.cmake`选项来执行脚本。脚本模式只是在给定的文件中运行命令，并且不生成构建系统。它不允许CMake命令定义或执行构建目标。

##### 3. Module

在Directory或Script中，CMake代码可以使用include()命令来加载.cmake。cmake内置了许多模块用来帮助我们构建工程，前边文章中提到的`CheckFunctionExists`。也可以提供自己的模块，并在`CMAKE_MODULE_PATH`变量中指定它们的位置。

#### cmake基本编写格式

先看一下定义的方式

| 名称               | 表达式                                                       | 我认为的                              | 例子     |
| ------------------ | ------------------------------------------------------------ | ------------------------------------- | -------- |
| space              | `<match '[ \t]+'>`                                           | 任意个空格或者tab                     | a b      |
| newline            | `<match '\n'>`                                               | 换行符                                | a\nb     |
| line_comment       | `'#' <any text not starting in a bracket_argument and not containing a newline>` | 以'#"开头，不在'[]'块中，不包含换行符 | #bus     |
| separation         | `space\|newline`                                             | 空格或者换行                          | a b=a\nb |
| lineending         | `linecomment?newline`                                        | 从注释开头到换行符都不执行            |          |
| command_invocation | `space* identifier space* '(' arguments ')'`                 |                                       |          |
| quoted_argument    | `"quoted_element* "`                                         | 用引号包裹的参数                      | "a"      |

文档看起来很蛋疼，我直接写一个最简单的

```
add_executable(hello world.c foo.c) #这是一个注释
复制代码
```

也等于

```
add_executable(hello 
                world.c 
                foo.c)
#这是一个注释
复制代码
```

就是这么easy。注意参数这一块，可以用引号包裹起来，这代表一个参数，假如一行不能写完，则用`\\`符号来表示连接成一行,也可以不用引号，但是假如参数带有分隔符，则会被认为是多个参数。

#### 定义变量

定义变量常用的函数是`set(KEY VALUE)`，取消定义变量是`unset(KEY)`。 它们的值始终是string类型的，有些命令可能将字符串解释为其他类型的值。变量名是区分大小写的，可能包含任何文本，但是我们建议只使用字母数字字符加上_和-这样的名称。

变量的作用域和java基本一致，不多做讲解。

变量引用的形式为`${variable_name}`，并在引用的参数或未引用的参数中进行判断。变量引用被变量的值替换，或者如果变量没有设置，则由空字符串替换。变量引用可以嵌套，并从内部进行替换，例如`${outer_${inner_variable}veriable}`。 环境变量引用的形式为$ENV{VAR}，并在相同的上下文中作为正常变量引用。

#### cmake构建系统

这算是进入正文了。扯淡的介绍就不多讲了，直接干货。

可执行文件和库是使用add_executable()和add_library()命令定义的。生成的二进制文件有合适的前缀、后缀和针对平台的扩展。二进制目标之间的依赖关系使用target_link_libraries()命令表示。

```
add_library(archive archive.cpp zip.cpp lzma.cpp)
add_executable(zipapp zipapp.cpp)
target_link_libraries(zipapp archive)
复制代码
```

在构建c程序的时候，因为要生成可执行文件，`add_executable`是必须的；构建安卓动态库的时候，`add_library`是必须的，因为jni需要调用动态库。

#### 外部构建

前边提到过cmake的构建命令`cmake .`,也就是在当前目录构建工程，这样会生成一系列的缓存文件在当前目录，假如我们需要重新构建需要删除这些文件，其中CMakeCache.txt是必须删除的，否则不会构建最新的程序。 看一下我们在工程根目录下执行`cmake`后是什么情况：

```
 ~/Desktop/Tutorial/Step1/ tree -L 1
.
├── CMakeCache.txt
├── CMakeFiles
├── CMakeLists.txt
├── Makefile
├── Tutorial
├── TutorialConfig.h
├── TutorialConfig.h.in
├── cmake_install.cmake
└── tutorial.cxx
复制代码
```

生成的文件与源文件交叉在一起，相当混乱。我们可以采用外部构建来分隔源文件与生成的文件，当我们需要清空缓存重新构建项目时，就可以删除这个文件夹下的所有内容，重新运行构建命令，保持源文件的整洁，从而更容易管理项目。

首先新建一个文件夹`build`。这个文件夹就是我们用来存放生成的文件的目录，然后进入该目录，执行构建命令。

```
mkdir build # 创建build目录
cd build # 进入build目录
cmake .. # 因为程序入口构建文件在项目根目录下，采用相对路径上级目录来使用根目录下的构建文件
复制代码
```

此时可以看到生成的文件全部在build文件夹下了，构建完全没问题。

```
 ~/Desktop/Tutorial/Step1/ tree -L 2
.
├── CMakeLists.txt
├── TutorialConfig.h.in
├── build
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   ├── Makefile
│   ├── TutorialConfig.h
│   └── cmake_install.cmake
└── tutorial.cxx

复制代码
```

以后的项目讲解中将全部使用外部构建。

下面是我专门为讲解一些基本指令编写的代码

```
cmake_minimum_required (VERSION 2.6)
project (Tutorial)
message(STATUS ${PROJECT_NAME})
message(STATUS ${PROJECT_SOURCE_DIR})
message(STATUS ${PROJECT_BINARY_DIR})
message(STATUS ${Tutorial_SOURCE_DIR})
message(STATUS ${Tutorial_BINARY_DIR})
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(Tutorial tutorial.cxx)
复制代码
```

- 首先看message用法 message类似于一个向控制台输出日志的工具，但是功能又稍微强大一些，在一些模式下能够终止程序构建。

```
message([<mode>] "message to display" ...)
复制代码
```

mode有以下几个模式

| 模式           | 作用                                                         |
| -------------- | ------------------------------------------------------------ |
| (none)         | Important information                                        |
| STATUS         | Incidental information                                       |
| WARNING        | CMake Warning, continue processing                           |
| AUTHOR_WARNING | CMake Warning (dev), continue processing                     |
| SEND_ERROR     | CMake Error, continue processing,but skip generation         |
| FATAL_ERROR    | CMake Error, stop processing and generation                  |
| DEPRECATION    | CMake Deprecation Error or Warning if variable CMAKE_ERROR_DEPRECATED or CMAKE_WARN_DEPRECATED is enabled, respectively, else no message. |

STATUS我们经常用到。

- 来看第一行：

```
cmake_minimum_required (VERSION 2.6)
复制代码
```

为一个项目设置cmake的最低要求版本，并更新策略设置以匹配给定的版本(策略设置我永远也不会讲了)。无论是构建项目还是构建库，都需要这个命令。 它的语法是这样的

```
cmake_minimum_required(VERSION major.minor[.patch[.tweak]]
                       [FATAL_ERROR])
复制代码
```

版本号必须指定主次代号，后边的可选，请忽略可选项`[FATAL_ERROR]`，完全没用。

假如你指定的版本号大于你安装的cmake版本，将会停止构建并抛出一个错误:

```
CMake Error at CMakeLists.txt:1 (cmake_minimum_required):
  CMake 3.11 or higher is required.  You are running version 3.10.2


-- Configuring incomplete, errors occurred!
复制代码
```

**`cmake_minimum_required`必须在项目根目录下的最开始调用，也就是`project()`之前。在function()中调用该指令也可以，作用域将局限在函数内，但是必须以不影响全局使用为前提**

- 来看第二行

```
project (Tutorial)
复制代码
```

指定项目的名称为`Tutorial`，构建项目必须使用这个命令，构建库可以不指定。文档如下：

```
project(<PROJECT-NAME> [LANGUAGES] [<language-name>...])
project(<PROJECT-NAME>
        [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
        [DESCRIPTION <project-description-string>]
        [LANGUAGES <language-name>...])
复制代码
```

设置项目名称并将该名称存储在`PROJECT_NAME`变量中。同时也指定了四个变量：

```
PROJECT_SOURCE_DIR, <PROJECT-NAME>_SOURCE_DIR
PROJECT_BINARY_DIR, <PROJECT-NAME>_BINARY_DIR
复制代码
```

但是我们一般只使用前一个，这样更容易更改。在上边的代码中我们用message输出了这些变量的信息，执行构建命令后日志输出：

```
-- Tutorial
-- /Users/saka/Desktop/Tutorial/Step1
-- /Users/saka/Desktop/Tutorial/Step1/build
-- /Users/saka/Desktop/Tutorial/Step1
-- /Users/saka/Desktop/Tutorial/Step1/build
复制代码
```

可以看到这几个变量确实输出了正确的值。

我们也可以在指定项目名称时直接指定版本号，假如没有指定，则版本号为空。 版本号存储在下边几个变量中:

```
PROJECT_VERSION, <PROJECT-NAME>_VERSION
PROJECT_VERSION_MAJOR, <PROJECT-NAME>_VERSION_MAJOR
PROJECT_VERSION_MINOR, <PROJECT-NAME>_VERSION_MINOR
PROJECT_VERSION_PATCH, <PROJECT-NAME>_VERSION_PATCH
PROJECT_VERSION_TWEAK, <PROJECT-NAME>_VERSION_TWEAK
复制代码
```

通常我们推荐使用前一个。现在测试一下，在CMakeLists.txt文件中修改代码：

```
project (Tutorial
	VERSION 1.2.3
	DESCRIPTION "this is description"
	LANGUAGES CXX)
message(STATUS ${PROJECT_VERSION})
message(STATUS ${PROJECT_VERSION_MAJOR})
message(STATUS ${PROJECT_VERSION_MINOR})
message(STATUS ${PROJECT_VERSION_PATCH})
message(STATUS ${PROJECT_VERSION_TWEAK})
message(STATUS ${PROJECT_DESCRIPTION})
复制代码
```

输出日志如下：

```
-- 1.2.3
-- 1
-- 2
-- 3
-- 
-- this is description
复制代码
```

在这设置版本号和用set设置版本号效果一样，取最后一次设置的值。由于我们没有指定tweak版本，所以为空，同时看到description被存储到`PROJECT_DESCRIPTION`这个变量中了。

可以通过设置`LANGUAGES`来指定编程语言是C、CXX(即c++)或者Fortran等，如果没有设置此项，默认启用C和CXX。设置为NONE，或者只写`LANGUAGES`关键字而不写具体源语言，可以跳过启用任何语言。一般都是用cmake来编译c或者c++程序，所以用默认的就可以了。

- 来看重要的一行`configure_file`

该命令的作用是复制文件到另一个地方并修改文件内容。语法如下：

```
configure_file(<input> <output>
               [COPYONLY] [ESCAPE_QUOTES] [@ONLY]
               [NEWLINE_STYLE [UNIX|DOS|WIN32|LF|CRLF] ])
复制代码
```

将文件复制到文件中，并在输入文件内容中替换`@VAR@`或`${VAR}`的变量值。每个变量引用将被替换为变量的当前值，如果变量的值未被定义，则为空字符串。（VAR必须与cmakelist.txt中的变量保持一直，否则会生成注释）

input文件的定义形式为:

```
#cmakedefine VAR ..
复制代码
```

经过configure后生成的文件内容被替换为：

```
#define VAR ...  //替换成功
/* #undef VAR */ //未定义的变量
复制代码
```

生成的文件将会保留在'#'与'cmakedefine'之间的空格和制表符。

> 此处有一点需说明，现在clion默认使用cmake来构建程序，但是在clion中不支持`cmakedefine`关键字，所以可以直接使在input文件中填写`#define VAR ...`来编写宏定义，生成的结果与上边完全一样。clion有一个问题，就是直接用`cmakedefine`定义宏的时候假如#与cmakedefine之间有空格则不会替换`cmakedefine`为`define`，后边的变量会替换，但是不能编译成功，所以假如在clion中使用，要注意这几点，直接使用`#define`或者`#cmakedefine`，尽量不要加空格。

介绍其中的选项: input和output假如不指定绝对路径，则会被默认设置为`CMAKE_CURRENT_SOURCE_DIR`和`CMAKE_CURRENT_BINARY_DIR`，也就是项目根目录和构建的目录；

`COPYONLY`则只是复制文件，不替换任何东西，不能和`NEWLINE_STYLE <style>`一起使用。

`ESCAPE_QUOTES`禁止为`"`转义。这个很蛋疼，不加这个命令的话假如变量中有`a\"b`，则在生成的文件中会直接使用转义后的字符`a"b`，加上指令后则按原来的文字显示`a\"b`；

`@ONLY`只允许替换`@VAR@`包裹的变量`${VAR}`则不会被替换；

`NEWLINE_STYLE <style>`设置换行符格式

现在举个例子： foo.h.in文件如下

```
#cmakedefine FOO_ENABLE
#cmakedefine FOO_STRING "@FOO_STRING@"
复制代码
```

CMakeLists.txt中添加代码来设置一个开关，下边会执行if中的语句：

```
option(FOO_ENABLE "Enable Foo" ON)
if(FOO_ENABLE)
  set(FOO_STRING "foo")
endif()
configure_file(foo.h.in foo.h @ONLY)
复制代码
```

生成的文件foo.h

```
#define FOO_ENABLE
#define FOO_STRING "foo"
复制代码
```

假如设置为off，`option(FOO_ENABLE "Enable Foo" ON)`,则不会执行if中的语句，生成的文件如下：

```
/* #undef FOO_ENABLE */
/* #undef FOO_STRING */
复制代码
```

- `include_directories`这一行

这句话的意思将当前的二进制目录添加到编译器搜索include目录中，这样就可以直接使用上一步生成的头文件了。

```
include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])
复制代码
```

将给定的目录添加到编译器用来搜索包含文件的目录。相对路径为相对于当前根目录。

括号中的目录被添加到当前CMakeLists文件的`INCLUDE_DIRECTORIES`目录属性中。它们也被添加到当前CMakeLists文件中的每个目标的INCLUDE_DIRECTORIES目标属性中。。

默认情况下，指定的目录被追加到当前的include目录列表中。通过将`CMAKE_INCLUDE_DIRECTORIES_BEFORE`设置为ON，可以更改此默认行为。通过明确使用AFTER或BEFORE，您可以选择添加和预先设置。

如果给出SYSTEM选项，那么编译器会被告知这些目录在某些平台上是指系统包含的目录。

这翻译的真是教我头大。


作者：saka
链接：https://juejin.cn/post/6844903558861553672
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

## 流程判断

cmake中的流程判断相对简单，与c语言接近。

形式如下:

```
if(expression)
  # then section.
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  #...
elseif(expression2)
  # elseif section.
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  #...
else(expression)
  # else section.
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  #...
endif(expression)
复制代码
```

此处有一点注意：else和endif中的表达式是可以省略的，看到现在大部分网上的教程有必须加`SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)`这句才能省略表达式，一脸懵逼，关于那个变量，我也查不到api，包括废弃的，所以我都不写这句。

if表达式可以用长表达式，优先级顺序如下：

```
> EXISTS, COMMAND, DEFINED 
> EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, STREQUAL, STRLESS, STRLESS_EQUAL, STRGREATER, STRGREATER_EQUAL, VERSION_EQUAL, VERSION_LESS, VERSION_LESS_EQUAL, VERSION_GREATER, VERSION_GREATER_EQUAL, MATCHES
> NOT,AND,OR
复制代码
```

别看下面这张表很长，其实没什么内容：

| 表达式                                             | true                                                         | false                                                        | 说明                                                         |
| -------------------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| <constant>                                         | 1, ON, YES, TRUE, Y,或者是非0数字                            | 0, OFF, NO, FALSE, N, IGNORE, NOTFOUND，空字符串,或者带-NOTFOUND后缀 | 布尔判断值大小写不敏感                                       |
| <variable\|string>                                 | 已经定义且不是false的变量                                    | 未定义或者是false的变量                                      | 变量就是字符串                                               |
| <NOT expression>                                   | expression为false                                            | expression为true                                             |                                                              |
| AND                                                | 两个条件全部成立                                             | 至少有一个为假                                               |                                                              |
| COMAND command-name                                | 已经定义的command,macro或者function                          | 未定义                                                       |                                                              |
| POLICY policy-id                                   | policy存在                                                   | policy不存在                                                 | 形式为CMP                                                    |
| TARGET target-name                                 | 已经用add_executable(), add_library(), or add_custom_target()定义过的target | 未定义                                                       |                                                              |
| TEST test-name                                     | add_test()创建过的测试名称                                   | 未创建                                                       |                                                              |
| EXISTS path-to-file-or-directory                   | 文件或者路径存在                                             | 文件或者路径不存在                                           | 此处是全路径                                                 |
| file1 IS_NEWER_THAN file2                          | file1的时间戳大于file2的时间戳 其中一个文件不存在 两个文件时间戳相同 | 其他情况                                                     | 文件路径必须是全路径                                         |
| IS_DIRECTORY path-to-directory                     | 给定的变量是文件夹                                           | 不是文件夹                                                   | 全路径                                                       |
| IS_SYMLINK file-name                               | 变量是链接                                                   | 不是                                                         | 全路径                                                       |
| IS_ABSOLUTE path                                   | 是绝对路径                                                   | 不是                                                         |                                                              |
| <variable\|string> MATCHES regex                   | 正则表达式匹配成功                                           | 匹配失败                                                     |                                                              |
| <variable\|string> LESS <variable\|string>         | 给定的变量是数字并且左边小于右边                             | 左边大于右边                                                 | 用于比较数字的大小 LESS:小于 GREATER:大于 EQUAL:等于 GREATER_EQUAL:大于等于 LESS_EQUAL:小于等于 |
| <variable\|string> STRLESS <variable\|string>      | 按字典顺序左边小于右边                                       | 左边大于右边                                                 | 用于比较字符串 LESS:小于 STRGREATER:大于 STREQUAL:等于 STRLESS_EQUAL:小于等于 STRGREATER_EQUAL:大于等于 |
| <variable\|string> VERSION_LESS <variable\|string> | 左边的版本号小于右边的版本号                                 | 大于                                                         | 用于版本号的比较 LESS:小于 VERSION_GREATER:大于 VERSION_EQUAL:等于 VERSION_LESS_EQUAL:小于等于 VERSION_GREATER_EQUAL:大于等于 |
| <variable                                          | string> IN_LIST                                              | 右边的item中有左边                                           | 没有                                                         |
| DEFINED                                            | 已定义变量                                                   | 未定义变量                                                   |                                                              |
| (expr1) AND (expr2 OR (expr3))                     | 1为真且2或者3至少有一个为真                                  | 其他情况                                                     |                                                              |

在if条件表达式中，是不必用`${var}`来取变量的值的，系统会自动转换。例如设置两个变量，然后比较各种取值的情况:

```
set(var1 OFF)
set(var2 "var1")
复制代码
```

`if(var2)`实际是判断var1是否为false； `if(${var2})`相当于`if(var1)`,实际是判断OFF；

## foreach循环

### 1. 第一种形式

```
foreach(loop_var arg1 arg2 ...)
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
endforeach(loop_var)
复制代码
```

此处注意`endforeach(loop_var)`的变量最好不要省略，因为foreach循环是依靠变量来跳出循环的。

在`foreach`和匹配`endforeach`之间的所有命令都会被系统记录而不被调用。 一旦找到了了`endforeach`，则会执行原来记录的命令。在循环的每次迭代之前，`${loop_var}`将被设置为具有列表中当前值的变量。

```
foreach(i 0 1 2 3)
    message(STATUS "current is ${i}")
endforeach(i)
    message(STATUS "end")
endforeach(i)
复制代码
```

一个简单的循环，但是多了一个`endforeach`。看一下结果

```
➜  StepTest git:(master) ✗ cmake -P foreach.cmake
-- current is 0
-- current is 1
-- current is 2
-- current is 3
-- end
CMake Error at foreach.cmake:5 (endforeach):
  endforeach An ENDFOREACH command was found outside of a proper FOREACH
  ENDFOREACH structure.  Or its arguments did not match the opening FOREACH
  command.
复制代码
```

报错了。没有匹配的foreach。

### 2. 第二种形式

```
foreach(loop_var RANGE total)
复制代码
```

从0开始直到`total`结束(包含total)

```
foreach(i RANGE 3)
    message(STATUS "current is ${i}")
endforeach(i)
复制代码
```

范围将会是0-3,查看一下结果:

```
➜  StepTest git:(master) ✗ cmake -P foreach.cmake
-- current is 0
-- current is 1
-- current is 2
-- current is 3
复制代码
```

### 3. 第三种形式

```
foreach(loop_var RANGE start stop [step])
复制代码
```

从start开始直到stop结束之间的值，可以设置步进值step。

```
foreach(i RANGE 0 3 1)
message(STATUS "current is ${i}")
endforeach(i)
复制代码
```

输出结果和上面的一样.

> 注意一点：最后的结果不会大于stop值,步进值是浮点数时会被转为整形

### 4. 第四种形式

```
foreach(loop_var IN [LISTS [list1 [...]]]
                    [ITEMS [item1 [...]]])
复制代码
```

也比较简单，多了LIST关键字来循环list。不多讲。

## while循环

```
while(condition)
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
endwhile(condition)
复制代码
```

注意endwhile中的条件最好不要省略。这个条件和if中的表达式是一样的规则。 循环形式和foreach循环类似，直到碰到endwhile才开始执行每一条指令。

> 在while和foreach循环中，取变量的值请用`${var}`。break和continue的用法基本与c一样，放心使用。

在实际项目中，经常使用`option`来和`if`搭配。 option使用比较简单：

```
option(<option_variable> "help string describing option"
       [initial value])
复制代码
```

initial value只能使用ON或者OFF，假如未设定，默认为false。 `cmake_dependent_option`是cmake内置的一个module，用来生成依赖其他`option`的option,这个相当蛋疼。

看一个简单的例子:

```
include(${CMAKE_ROOT}/Modules/CMakeDependentOption.cmake)
option(USE_CURL "use libcurl" ON)
option(USE_MATH "use libm" ON)
cmake_dependent_option(DEPENT_USE_CURL "this is dependent on USE_CURL" ON "USE_CURL;NOT USE_MATH" OFF)
if(DEPENT_USE_CURL)
    message(STATUS "using lib curl")
else()
    message(STATUS "not using lib curl")
endif()
复制代码
```

第一行包含了我们需要的依赖模块。

第二行第三行定义了两个`option`，`USE_CURL`,`USE_MATH`全为`ON`。

第四行定义了一个`option`，`DEPENT_USE_CURL`,后边紧跟的是它的说明 `this is dependent on USE_CURL`,再后边相当于一个三元判断式，假如`USE_CURL;NOT USE_MATH`为真时，取前边的值，否则取后边的值。

5-9行是一个if语句，用来输出我们想要的结果。

输出结果：

```
➜  StepTest git:(master) ✗ cmake -P optionc.cmake
-- not using lib curl
```


作者：saka
链接：https://juejin.cn/post/6844903565207535630
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

cmake中有两个相似的关键字，macro和function。这两个都是创建一段有名字的代码稍后可以调用，还可以传参数。

## macro宏定义与function函数的相同点

macro形式如下：

```
macro(<name> [arg1 [arg2 [arg3 ...]]])
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
endmacro(<name>)
复制代码
```

function形式如下：

```
function(<name> [arg1 [arg2 [arg3 ...]]])
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
function(<name>)
复制代码
```

定义一个名称为name的宏（函数），arg1...是传入的参数。我们除了可以用`${arg1}`来引用变量以外，系统为我们提供了一些特殊的变量：

| 变量  | 说明                                                         |
| ----- | ------------------------------------------------------------ |
| ARGV# | #是一个下标，0指向第一个参数，累加                           |
| ARGV  | 所有的定义时要求传入的参数                                   |
| ARGN  | 定义时要求传入的参数以外的参数，比如定义宏（函数）时，要求输入1个，书记输入了3个，则剩下的两个会以数组形式存储在ARGN中 |
| ARGC  | 传入的实际参数的个数，也就是调用函数是传入的参数个数         |

## macro宏定义与function函数的不同点

宏的ARGN、ARGV等参数不是通常CMake意义上的变量。 它们是字符串替换，很像C预处理器对宏的处理。 因此，如下命令是错误的：

```
if(ARGV1) # ARGV1 is not a variable 
if(DEFINED ARGV2) # ARGV2 is not a variable
if(ARGC GREATER 2) # ARGC is not a variable
foreach(loop_var IN LISTS ARGN) # ARGN is not a variable
复制代码
```

正确写法如下：

```
if(${ARGV1})
if(DEFINED ${ARGV2})
if(${ARGC} GREATER 2)
foreach(loop_var IN LISTS ${ARGN})
or
set(list_var "${ARGN}")
foreach(loop_var IN LISTS list_var)

复制代码
```

一个简单的例子

```
macro(FOO arg1 arg2 arg3)
    message(STATUS "this is arg1:${arg1},ARGV0=${ARGV0}")
    message(STATUS "this is arg2:${arg2},ARGV1=${ARGV1}")
    message(STATUS "this is arg3:${arg3},ARGV2=${ARGV2}")
    message(STATUS "this is argc:${ARGC}")
    message(STATUS "this is args:${ARGV},ARGN=${ARGN}")
    if(arg1 STREQUAL one)
        message(STATUS "this is arg1")
    endif()
    if(ARGV2 STREQUAL "two")
        message(STATUS "this is arg2")
    endif()
    set(${arg1} nine)
    message(STATUS "after set arg1=${${arg1}}")
endmacro(FOO)

function(BAR arg1)
    message(STATUS "this is arg1:${arg1},ARGV0=${ARGV0}")
    message(STATUS "this is argn:${ARGN}")
    if(arg1 STREQUAL first)
        message(STATUS "this is first")
    endif()
    set(arg1 ten)
    message(STATUS "after set arg1=${arg1}")
endfunction(BAR arg1)

set(p1 one)
set(p2 two)
set(p3 three)
set(p4 four)
set(p5 five)
set(p6 first)
set(p7 second)

FOO(${p1} ${p2} ${p3} ${p4} ${p5})
BAR(${p6} ${p7})
message(STATUS "after bar p6=${p6}")
复制代码
```

输出结果如下:

```
-- this is arg1:one,ARGV0=one
-- this is arg2:two,ARGV1=two
-- this is arg3:three,ARGV2=three
-- this is argc:5
-- this is args:one;two;three;four;five,ARGN=four;five
-- after set arg1=nine
-- this is arg1:first,ARGV0=first
-- this is argn:second
-- this is first
-- after set arg1=ten
-- after bar p6=first
复制代码
```

接下来看一个让我们蛋都能疼碎了的例子，简直不想用cmake：

```
macro(_bar)
  foreach(arg IN LISTS ARGN)
    message(STATUS "this is in macro ${arg}")
  endforeach()
endmacro()

function(_foo)
    foreach(arg IN LISTS ARGN)
        message(STATUS "this in function is ${arg}")
    endforeach()
  _bar(x y z)
endfunction()

_foo(a b c)
复制代码
```

看一下输出:

```
-- this in function is a
-- this in function is b
-- this in function is c
-- this is in macro a
-- this is in macro b
-- this is in macro c
复制代码
```

就是这么蛋疼，我们传给了`_bar(x y z)`,结果打印出来的是`a b c`,那我们把第二行的foreach改成`foreach(arg IN LISTS ${ARGN})`， 看一下结果：

```
-- this in function is a
-- this in function is b
-- this in function is c
复制代码
```

没有输出`_bar`中的信息。为啥？因为这个ARGN的作用域是在function中的，也就是`_foo`函数中的那个ARGN。有兴趣的话可以试试在macro中调用function。


作者：saka
链接：https://juejin.cn/post/6844903565379502093
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

## 简介

该文章未介绍如何编译，可以参考我的另一篇文章

[shell脚本生成安卓全abi动态库与静态库](https://link.juejin.cn?target=https%3A%2F%2Frangaofei.github.io%2F2018%2F02%2F22%2Fshell%E8%84%9A%E6%9C%AC%E7%94%9F%E6%88%90%E5%AE%89%E5%8D%93%E5%85%A8abi%E5%8A%A8%E6%80%81%E5%BA%93%E4%B8%8E%E9%9D%99%E6%80%81%E5%BA%93%2F%23more)

用cmake交叉编译相当简单，基本可以达到一次编写，每次都能运行的目的。

CMake使用toolchain来编译，链接library和创建archives，以及其他task来驱动构建。 可用的toolchain实用程序由启用的语言确定。 在正常版本中，CMake根据系统默认值自动确定主使用的toolchain。 在交叉编译场景中，可以使用关于编译器和实用程序路径的信息来指定toolchain文件。

假如我们已经编写好了自己的toolchain文件，那么在执行构建时添加参数`-DCMAKE_TOOLCHAIN_FILE=path/to/file`即可，cmake系统会自动使用执行的toolchain.cmake文件。

安卓平台交叉编译有两种方式，一是使用ndk编译，另一种是使用自己的工具链编译，我感觉我只会ndk编译，另一中我就不介绍了，因为我不会。

这里介绍如何构建makefile和ninja两种方式的交叉编译。

## 变量说明

有几个特殊的变量需要设置：

1. `CMAKE_SYSTEM_NAME`

   编译安卓平台时请设置这个变量为`Android`。而且`CMAKE_SYSTEM_VERSION`必须指定。

2. `CMAKE_SYSTEM_VERSION`

   设置安卓的api level，假如未设置这个值，则会由以下两个方式决定：

   - `CMAKE_ANDROID_API`设置了，则使用该api level
   - `CMAKE_SYSROOT`设置了，则使用该api level
   - 都没设置，则使用ndk中最新的api levele

3. `CMAKE_ANDROID_ARCH_ABI`

   `arm64-v8a armeabi-v7a armeabi-v6 armeabi mips mips64 x86 x86_64`。 这个牛逼了，设置abi架构，假如未设置，则使用默认的`armeabi`。设置了这个变量系统会自动设置`CMAKE_ANDROID_ARCH`，不必手动设置。

4. `CMAKE_ANDROID_NDK`

   这个更牛逼了，使用它来设置ndk的路径，必须是绝对路径，到ndk的根目录即可。

5. `CMAKE_ANDROID_NDK_DEPRECATED_HEADERS`

   设置为true，则会使用每个api level中已经废弃的头文件，而不会使用统一的头文件。默认为false，使用统一的头文件。

6. `CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION`

   设置NDK编译链的版本，假如不设置，则使用最新的。

7. `CMAKE_ANDROID_STL_TYPE` 这个牛逼了。有好几个值：

   `none` `No C++ Support`

   `system` `Minimal C++ without STL`

   `gabi++_static` `GAbi++ Static`

   `gabi++_shared` `GAbi++ Shared`

   `gnustl_static` `GNU libstdc++ Static`

   `gnustl_shared` `GNU libstdc++ Shared`

   `c++_static` `LLVM libc++ Static`

   `c++_shared` `LLVM libc++ Shared`

   `stlport_static` `STLport Static`

   `stlport_shared` `STLport Shared`

   默认值是gnustl_static。

8. `CMAKE_<LANG>_ANDROID_TOOLCHAIN_PREFIX`

   自动生成的，绝对路径的前缀。

9. `CMAKE_<LANG>_ANDROID_TOOLCHAIN_SUFFIX` 自动生成的，绝对路径的后缀。

## 构建顺序

1. 假如设置了`CMAKE_ANDROID_NDK`，就会使用该变量指定的NDK路径
2. 假如设置了`CMAKE_ANDROID_STANDALONE_TOOLCHAIN`，而1中的变量未设置，就会使用该独立的工具链编译。
3. 假如设置了`CMAKE_SYSROOT`,1、2未设置，并且路径形式是`<ndk>/platforms/android-<api>/arch-<arch>`,则相当于设置了`CMAKE_ANDROID_NDK`，并且会使用该路径的ndk。
4. 假如设置了`CMAKE_SYSROOT`，1、2、3未设置，并且路径形式是`<standalone-toolchain>/sysroot`，则相当于设置了`CMAKE_ANDROID_STANDALONE_TOOLCHAIN`并且使用该工具链编译。
5. 假如设置了`ANDROID_NDK`，1、2、3、4未设置，则相当于设置了1，并且使用该ndk。
6. 假如设置了`ANDROID_STANDALONE_TOOLCHAIN`,1、2、3、4、5未设置，则相当于设置了2，并且使用该工具链。
7. 假如设置了环境变量`ANDROID_NDK_ROOT`或者`ANDROID_NDK`，1、2、3、4、5、6则相当于设置了1，并且使用NDK。
8. 假如设置了`ANDROID_STANDALONE_TOOLCHAIN`，1、2、3、4、5、6、7未设置，则相当于设置了2，并且使用该工具链。
9. 全都没设置会报错。

## 编译方式

一般需要设置的如下：

```
set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 21) # API level
set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)
set(CMAKE_ANDROID_NDK /path/to/android-ndk)
set(CMAKE_ANDROID_STL_TYPE gnustl_static)
复制代码
```

假如不想编写toolchain.cmake文件，也课可以在命令行直接输入参数来完成构建：

```
cmake ../src \
  -DCMAKE_SYSTEM_NAME=Android \
  -DCMAKE_SYSTEM_VERSION=21 \
  -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
  -DCMAKE_ANDROID_NDK=/path/to/android-ndk \
  -DCMAKE_ANDROID_STL_TYPE=gnustl_static
复制代码
```




作者：saka
链接：https://juejin.cn/post/6844903565803126798
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
