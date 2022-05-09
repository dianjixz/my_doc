# python编程笔记

 

### Python3 编码hexstring到ascii详解
~~~ python
Hex-->Ascii

#coding =utf-8
from binascii import a2b_hex

h ='6a6a6a6a6a6a6a6a6a'
s = a2b_hex(h)
print(s)
~~~

### Python3 字符串与hex之间的相互转换
在python3.5之前，这个转换的其中一种方式是这样的，利用bytes.fromhex()加单个字符转换
~~~ python
>>> a = 'aabbccddeeff'
>>> a_bytes = bytes.fromhex(a)
>>> print(a_bytes)
b'\xaa\xbb\xcc\xdd\xee\xff'
>>> aa = ''.join(['%02x' % b for b in a_bytes])
>>> print(aa)
aabbccddeeff
>>>
~~~
到了python 3.5之后，直接用bytes.fromhex即可完成转换

~~~ python
>>> a = 'aabbccddeeff'
>>> a_bytes = bytes.fromhex(a)
>>> print(a_bytes)
b'\xaa\xbb\xcc\xdd\xee\xff'
>>> aa = a_bytes.hex()
>>> print(aa)
aabbccddeeff
>>>
~~~



~~~ python

# Filename : test.py
# author by : www.runoob.com
 
# 用户输入字符
c = input("请输入一个字符: ")
 
# 用户输入ASCII码，并将输入的数字转为整型
a = int(input("请输入一个ASCII码: "))
 
 
print( c + " 的ASCII 码为", ord(c))
print( a , " 对应的字符为", chr(a))


~~~

~~~ bash

python3 test.py 
请输入一个字符: a
请输入一个ASCII码: 101
a 的ASCII 码为 97
101  对应的字符为 e

~~~


Python里 int 和 bytes互转的方法


~~~ python

Python里 int 和 bytes互转的方法

在Python3之前，一般是使用下面的方法：

>>> import struct

>>> struct.pack(“B”, 2)

‘\x02’

>>> struct.pack(">H", 2)

‘\x00\x02’

>>> struct.pack("<H", 2)

‘\x02\x00’

也就是使用struct.pack方法，它实现了从int到bytes的转换。

在Python3里，也可以使用bytes转换0到255的整数，如下：

>>> bytes([2])

b’\x02`

这时候不要写成：bytes(3)，这样导致下面的结果：

>>> bytes(3)

b’\x00\x00\x00’

从Python 3.1起，就可以使用int.to_bytes()来转换整数到字节数组：

>>> (258).to_bytes(2, byteorder=“little”)

b’\x02\x01’

>>> (258).to_bytes(2, byteorder=“big”)

b’\x01\x02’

>>> (258).to_bytes(4, byteorder=“little”, signed=True)

b’\x02\x01\x00\x00’

>>>

~~~




资源:

Python 自动识别图片文字—OCR实战教程

https://zhuanlan.zhihu.com/p/384620684