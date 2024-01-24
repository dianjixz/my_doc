# args kwargs用法
在Python中，*args和**kwargs是用于函数定义中的特殊语法，允许函数接收可变数量的参数。*args用于非关键字参数，而**kwargs用于关键字参数。
*args 的用法

    *args 允许你将一个不确定数量的参数传递给一个函数。
    这些参数被封装成一个元组。
    在函数定义中，*args 是放在其他参数后面的。

示例

```python

def my_function(*args):
    for arg in args:
        print(arg)

my_function(1, 2, 3, 4)  # 输出 1, 2, 3, 4
```
**kwargs 的用法

    **kwargs 允许你将不确定数量的关键字参数传递给一个函数。
    这些参数被封装成一个字典。
    关键字参数是以键值对的形式传递的。

示例

```python

def my_function(**kwargs):
    for key, value in kwargs.items():
        print(f"{key}: {value}")

my_function(a=1, b=2, c=3)  # 输出 a: 1, b: 2, c: 3
```
结合使用 *args 和 **kwargs

可以在同一个函数中同时使用 *args 和 **kwargs 来允许任意数量的参数和关键字参数。
示例

```python

def my_function(*args, **kwargs):
    for arg in args:
        print(arg)
    for key, value in kwargs.items():
        print(f"{key}: {value}")

my_function(1, 2, 3, a=4, b=5)  # 输出 1, 2, 3, a: 4, b: 5
```
注意事项

    当使用 *args 和 **kwargs 时，所有的位置参数必须在 *args 之前，所有的关键字参数必须在 **kwargs 之后。
    不是必须将这些参数命名为 args 和 kwargs。星号 * 和双星号 ** 才是重要的部分，它们分别表示非关键字参数和关键字参数的集合。然而，使用 args 和 kwargs 是一种广泛接受的惯例。