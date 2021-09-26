 

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
