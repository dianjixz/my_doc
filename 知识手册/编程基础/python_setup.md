 

在安装python依赖库时，我们使用pip install 或者python setup.py install。
pip 会自己搜索适合的版本，python setup.py 需要下载源码本地安装。但是python setup.py 适合一键打包安装。

setup函数包含的参数解释
--name 包名称------------生成的egg名称
--version (-V) 包版本----生成egg包的版本号
--author 程序的作者------包的制作者名字
--author_email 程序的作者的邮箱地址
--maintainer 维护者
--maintainer_email 维护者的邮箱地址
--url 程序的官网地址
--license 程序的授权信息
--description 程序的简单描述-------程序的概要介绍
--long_description 程序的详细描述---程序的详细描述
--platforms 程序适用的软件平台列表
--classifiers 程序的所属分类列表
--keywords 程序的关键字列表
--packages 需要处理的包目录（包含__init__.py的文件夹）-------和setup.py同一目录下搜索各个含有 **init**.py的包
--py_modules 需要打包的python文件列表
--download_url 程序的下载地址
--cmdclass
--data_files 打包时需要打包的数据文件，如图片，配置文件等
--scripts 安装时需要执行的脚步列表
--package_dir 告诉setuptools哪些目录下的文件被映射到哪个源码包。一个例子：package_dir = {'': 'lib'}，表示“root package”中的模块都在lib 目录中。
--requires 定义依赖哪些模块
--provides定义可以为哪些模块提供依赖
--find_packages() 对于简单工程来说，手动增加packages参数很容易，刚刚我们用到了这个函数，它默认在和setup.py同一目录下搜索各个含有 **init**.py的包。
其实我们可以将包统一放在一个src目录中，另外，这个包内可能还有aaa.txt文件和data数据文件夹。另外，也可以排除一些特定的包
find_packages(exclude=["*.tests", "*.tests.*", "tests.*", "tests"])
--install_requires = ["requests"] 需要安装的依赖包
--entry_points 动态发现服务和插件

样例1如下：
from setuptools import setup, find_packages
setup(
name="prml",
version="0.0.1",
description="Collection of PRML algorithms",
author="ctgk",
python_requires=">=3.6",
install_requires=["numpy", "scipy"],#需要安装的依赖
packages=find_packages(exclude=["test", "test.*"]), # 需要打包的package,使用find_packages 来动态获取package，exclude参数的存在，使打包的时候，排除掉这些文件
test_suite="test"
)

样例2如下：
个人对使用packages相关参数的看法，
首先告诉程序去哪个目录中找包，因此有了packages参数，
其次，告诉程序我包的起始路径是怎么样的，因此有了package_dir参数
最后，找到包以后，我应该把哪些文件打到包里面，因此有了package_data参数
setup(
name = "demo",
version = "0.1",
\# 包含所有src目录下的包 ---------项目中的所有源码和测试用例文件目录一般都存放在统一的src目录下方便管理，默认也是创建src目录
packages = find_packages('src'),
package_dir = {'':'src'},
package_data = {
\# 包含所有.txt文件
'':['*.txt'],
\# 包含data目录下所有的.dat文件
'test':['data/*.dat'],
}
)



https://docs.python.org/2/distutils/apiref.html



The basic do-everything function that does most everything you could ever ask for from a Distutils method.

The setup function takes a large number of arguments. These are laid out in the following table.

|   argument name    |                            value                             |                             type                             |
| :----------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|       *name*       |                   The name of the package                    |                           a string                           |
|     *version*      | The version number of the package; see [`distutils.version`](https://docs.python.org/2/distutils/apiref.html#module-distutils.version) |                           a string                           |
|   *description*    |             A single line describing the package             |                           a string                           |
| *long_description* |              Longer description of the package               |                           a string                           |
|      *author*      |                The name of the package author                |                           a string                           |
|   *author_email*   |           The email address of the package author            |                           a string                           |
|    *maintainer*    | The name of the current maintainer, if different from the author. Note that if the maintainer is provided, distutils will use it as the author in `PKG-INFO` |                           a string                           |
| *maintainer_email* | The email address of the current maintainer, if different from the author |                           a string                           |
|       *url*        |               A URL for the package (homepage)               |                           a string                           |
|   *download_url*   |                A URL to download the package                 |                           a string                           |
|     *packages*     |   A list of Python packages that distutils will manipulate   |                      a list of strings                       |
|    *py_modules*    |   A list of Python modules that distutils will manipulate    |                      a list of strings                       |
|     *scripts*      | A list of standalone script files to be built and installed  |                      a list of strings                       |
|   *ext_modules*    |           A list of Python extensions to be built            | a list of instances of [`distutils.core.Extension`](https://docs.python.org/2/distutils/apiref.html#distutils.core.Extension) |
|   *classifiers*    |             A list of categories for the package             | a list of strings; valid classifiers are listed on [PyPI](https://pypi.org/classifiers). |
|    *distclass*     | the [`Distribution`](https://docs.python.org/2/distutils/apiref.html#distutils.core.Distribution) class to use | a subclass of [`distutils.core.Distribution`](https://docs.python.org/2/distutils/apiref.html#distutils.core.Distribution) |
|   *script_name*    | The name of the setup.py script - defaults to `sys.argv[0]`  |                           a string                           |
|   *script_args*    |           Arguments to supply to the setup script            |                      a list of strings                       |
|     *options*      |             default options for the setup script             |                         a dictionary                         |
|     *license*      |                 The license for the package                  |                           a string                           |
|     *keywords*     | Descriptive meta-data, see [**PEP 314**](https://www.python.org/dev/peps/pep-0314) |        a list of strings or a comma-separated string         |
|    *platforms*     |                                                              |        a list of strings or a comma-separated string         |
|     *cmdclass*     | A mapping of command names to [`Command`](https://docs.python.org/2/distutils/apiref.html#distutils.core.Command) subclasses |                         a dictionary                         |
|    *data_files*    |               A list of data files to install                |                            a list                            |
|   *package_dir*    |           A mapping of package to directory names            |                         a dictionary                         |





*class* `distutils.core.``Extension`

The Extension class describes a single C or C++ extension module in a setup script. It accepts the following keyword arguments in its constructor

|     argument name      |                            value                             |       type        |
| :--------------------: | :----------------------------------------------------------: | :---------------: |
|         *name*         | the full name of the extension, including any packages — ie. *not* a filename or pathname, but Python dotted name |     a string      |
|       *sources*        | list of source filenames, relative to the distribution root (where the setup script lives), in Unix form (slash- separated) for portability. Source files may be C, C++, SWIG (.i), platform-specific resource files, or whatever else is recognized by the **build_ext** command as source for a Python extension. | a list of strings |
|     *include_dirs*     | list of directories to search for C/C++ header files (in Unix form for portability) | a list of strings |
|    *define_macros*     | list of macros to define; each macro is defined using a 2-tuple `(name, value)`, where *value* is either the string to define it to or `None` to define it without a particular value (equivalent of `#define FOO` in source or `-DFOO` on Unix C compiler command line) | a list of tuples  |
|     *undef_macros*     |            list of macros to undefine explicitly             | a list of strings |
|     *library_dirs*     | list of directories to search for C/C++ libraries at link time | a list of strings |
|      *libraries*       | list of library names (not filenames or paths) to link against | a list of strings |
| *runtime_library_dirs* | list of directories to search for C/C++ libraries at run time (for shared extensions, this is when the extension is loaded) | a list of strings |
|    *extra_objects*     | list of extra files to link with (eg. object files not implied by ‘sources’, static library that must be explicitly specified, binary resource files, etc.) | a list of strings |
|  *extra_compile_args*  | any extra platform- and compiler-specific information to use when compiling the source files in ‘sources’. For platforms and compilers where a command line makes sense, this is typically a list of command-line arguments, but for other platforms it could be anything. | a list of strings |
|   *extra_link_args*    | any extra platform- and compiler-specific information to use when linking obje*class* `distutils.core.``Extension`ct files together to create the extension (or to create a new static Python interpreter). Similar interpretation as for ‘extra_compile_args’. | a list of strings |
|    *export_symbols*    | list of symbols to be exported from a shared extension. Not used on all platforms, and not generally necessary for Python extensions, which typically export exactly one symbol: `init` + extension_name. | a list of strings |
|       *depends*        |         list of files that the extension depends on          | a list of strings |
|       *language*       | extension language (i.e. `'c'`, `'c++'`, `'objc'`). Will be detected from the source extensions if not provided. |     a string      |
