 

## C语言函数调用关系图、结构体关系图 自动绘制

标签： [代码阅读](https://www.freesion.com/tag/代码阅读/)





### 文章目录

- [1. cview](https://www.freesion.com/article/5133151170/#1_cview_2)

- - [1.1. 介绍](https://www.freesion.com/article/5133151170/#11__6)
  - [1.2. 软件架构](https://www.freesion.com/article/5133151170/#12__11)
  - [1.3. 安装教程](https://www.freesion.com/article/5133151170/#13__15)
  - [1.4. 使用说明](https://www.freesion.com/article/5133151170/#14__24)
  - [1.5. pygraphviz文档](https://www.freesion.com/article/5133151170/#15_pygraphviz_133)



# 1. cview

https://gitee.com/wuu1010/cview

## 1.1. 介绍

Python3.x + Graphviz自动化绘制C语言 函数调用关系图、结构体关系图
 已使用Cython将Python代码转化为c，可以自行编译安装

## 1.2. 软件架构

软件架构说明

## 1.3. 安装教程

1. sudo -H pip3 install filetype pygraphviz
2. sudo -H pip3 install Cython setuptools
3. sudo apt install graphviz
4. cd cview
5. make all
6. sudo -H make install

## 1.4. 使用说明

1. 生成源文件

- 函数调用关系图
   **只支持ARM**
   对编译生成的文件进行反汇编

```shell
aarch64-linux-gnu-objdump -d vmlinux > vmlinux.dis
1
```

- 结构体关系图
   在编译Makefile中，CFLAGS选项中添加-save-temps=obj，再进行编译

```makefile
KBUILD_CFLAGS   := -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs \
                   -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE \
                   -Werror=implicit-function-declaration -Werror=implicit-int \
                   -Wno-format-security \
                   -std=gnu89 -save-temps=obj
12345
```

1. 解析文件
    解析文件实例

```python
#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import os
import sys
import cview
import time

def main(argv):

    db_path = '../../test.db'

    t_start = time.time()
    # step1: init
    test = cview.CView()
    os.remove(db_path)
    test.load_db(db_path)

    # step2: parser file
    test.parse_file('Func', 'function', '../../vmlinux.dis')
    test.parse_file('Data', 'struct', '../../exit.i')

    test.db.table_info()

    t_end = time.time()
    print('use {t_use:.2f}s'.format(t_use=(t_end - t_start)))


if __name__ == '__main__':
    main(sys.argv[0:])
123456789101112131415161718192021222324252627282930
```

1. 生成关系图
    关系图实例

```python
#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import os
import sys
import cview
import time


def main(argv):

    db_path = '../../test.db'

    t_start = time.time()
    # step1: init and load database
    test = cview.CView()
    test.load_db(db_path)

    test.db.table_info()

    # step2: the first table
    test.db.set_table('function')
    node_list = {
        'start_kernel',
        'kernel_init',
    }
    test.add_nodes_from(node_list)
    test.tree('kernel_init', 1, 2)
    test.build(random=False)

    # step3: the second table
    test.db.set_table('struct')
    node_list = {
        'struct zone',
    }
    test.add_nodes_from(node_list)
    test.tree('struct page', 1, 3)
    test.build(random=False)

    # step4: the end
    test.write('./test.gv')

    t_end = time.time()
    print('use {t_use:.2f}s'.format(t_use=(t_end - t_start)))

if __name__ == '__main__':
    main(sys.argv[0:])

123456789101112131415161718192021222324252627282930313233343536373839404142434445464748
```

![在这里插入图片描述](https://www.freesion.com/images/938/dabcf3480bae7a1bc37df8dfef4ae73a.JPEG)
 ![在这里插入图片描述](https://www.freesion.com/images/522/cd3da54dab2ac15fbbd1e452835c43a2.JPEG)

## 1.5. pygraphviz文档

[http://pygraphviz.github.io/documentation/pygraphviz-1.5/](https://pygraphviz.github.io/documentation/pygraphviz-1.5/)

 [ ](https://creativecommons.org/licenses/by-sa/4.0/)  版权声明：本文为wuu1010原创文章，遵循[ CC 4.0 BY-SA ](https://creativecommons.org/licenses/by-sa/4.0/)版权协议，转载请附上原文出处链接和本声明。  本文链接：https://blog.csdn.net/wuu1010/article/details/102489078

https://www.freesion.com/article/5133151170/
