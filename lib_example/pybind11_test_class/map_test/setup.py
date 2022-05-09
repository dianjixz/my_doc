#文件名：setup.py
from setuptools import setup, Extension  
  
functions_module = Extension(  
    name = 'nihao',  
    sources = ['my_module.cpp','conversion.c'],  
    include_dirs = [r'/usr/local/include', 
                    r'./'
                    # r'/usr/include/c++/9',
                    # r'/usr/include/x86_64-linux-gnu/c++/9'
                    ]  
)  
  
setup(ext_modules = [functions_module])

import os
os.system("cp ./build/lib.linux-x86_64-3.8/nihao.cpython-38-x86_64-linux-gnu.so .")




