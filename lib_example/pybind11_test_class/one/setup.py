#文件名：setup.py
from setuptools import setup, Extension  
  
functions_module = Extension(  
    name = 'nihao',  
    sources = ['nihao.cpp','conversion.c'],  
    include_dirs = [r'/usr/local/include', 
                    r'./']  
)  
  
setup(ext_modules = [functions_module])

import os
os.system("cp ./build/lib.linux-x86_64-3.8/nihao.cpython-38-x86_64-linux-gnu.so .")




