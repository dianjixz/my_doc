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



