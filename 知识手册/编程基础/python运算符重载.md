基础知识

实际上，“运算符重载”只是意味着在类方法中拦截内置的操作……当类的实例出现在内置操作中，Python自动调用你的方法，并且你的方法的返回值变成了相应操作的结果。以下是对重载的关键概念的复习：

运算符重载让类拦截常规的Python运算。

类可重载所有Python表达式运算符

类可以重载打印、函数调用、属性点号运算等内置运算

重载使类实例的行为像内置类型。

重载是通过特殊名称的类方法来实现的。



换句话说，当类中提供了某个特殊名称的方法，在该类的实例出现在它们相关的表达式时，Python自动调用它们。正如我们已经学习过的，运算符重载方法并非必须的，并且通常也不是默认的；如果你没有编写或继承一个运算符重载方法，只是意味着你的类不会支持相应的操作。然而，当使用的时候，这些方法允许类模拟内置对象的接口，因此表现得更一致。



构造函数和表达式：__init__ 和 __sub__

让我们来看一下一个简单的重载例子吧。例如，下列文件number.py类的Number类提供一个方法来拦截实例的构造函数（__init__），此外还有一个方法捕捉减法表达式(__sub__)。这种特殊的方法是钩子，可与内置运算相绑定。

class Number:
    def __init__(self, start):                # On Number(start)
        self.data = start
    def __sub__(self, other):                  # On instance - other
        return Number(self.data - other)        # Result is a new instance
        
>>> from number import Number
>>> X = Number(5)                            # Number.__init__(X, 5)
>>> Y = X - 2                                # Number.__sub__(X, 2)
>>> Y.data                                   # Y is new Number instance
>>> 3
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 就像前边讨论过的一样，该代码中所见到的__init__构造函数是Python中最常见的运算符重载方法，它存在于绝大多数类中。在本节中，我们会举例说明这个领域中其他一些可用的工具，病看一看这些工具常用的例程。



常见的运算符重载方法

在类中，对内置对象（例如，整数和列表）所能做的事，几乎都有相应的特殊名称的重载方法。下表列出其中一些常用的重载方法。事实上，很多重载方法有好几个版本（例如，加法就有__add__、__radd__和__iadd__）。

方法	重载	调用
__init__	构造函数	对象建立：X = Class（args）
__del__	析构函数	X对象收回
__add__	运算符 + 	如果没有__iadd__， X + Y, X += Y
__or__	运算符|（位OR）	如果没有__ior__, X | Y, X |= Y
__repr__, __str__	打印、转换	print(X), repr(X), str(X)
__call__	函数调用	X(*args, **kargs)
__getattr__	点号运算	X.undefined
__setattr__	属性赋值语句	X.any = value
__delattr__	属性删除	
del X.any

__getattribute__	属性获取	
X.any

__getitem__	索引运算	
X[key],X[i:j]，没__iter__时的for循环和其他迭代器

__setitem__	索引赋值语句	X[key] = value, X[i:j] = sequence
__delitem__
索引和分片删除	del X[key],  del X[i:j]
__len__	长度	len(X), 如果没有__bool__， 真值测试
__bool__	布尔测试	bool(X), 真测试（在Python 2.6中叫做__nonzero__）
__lt__, __gt__

__lt__, __ge__

__eq__, __ne__

特定比较	X<Y, X>Y, X<=Y, X>=Y, X == Y, X != Y(或者在Python 2.6中只有__cmp__)
__radd__	右侧加法	Other + X
__iadd__	实地（增强的）加法	X += Y(or else __add__)
__iter, __next__	迭代环境	I = iter(X), next(I); for loops, in  if no __contains__, all comprehensions, map(F, X), 其他(__next__在Python2.6中成为next)
__contains__	成员关系测试	item in X(任何可迭代的)
__index__	整数值	hex(X), bin(X), oct(X), O[X], O[X:]（替代Python 2中的__oct__、__hex__）
__enter__, __exit__	环境管理器	with obj as var:
__get__, __set__

__delete

描述符属性	X.attr, X.attr = value, del X.attr
__new__	创建	在__init__之前创建对象
所有重载方法的名称前后都有两个下划线，以便把同类中定义的变量名区别开来。特殊方法名称和表达式或运算的映射关系，是由Python语言预先定义好的（在标准语言手册中有说明）。例如名称，__add__按照Python语言的定义，无论__add__方法的代码实际在做些什么，总是对应到了表达式 + 。



如果没有定义运算符重载方法的话，它可能继承自超类，就像任何其他的方法一样。运算符重载方法也都是可选的……如果没有编写或继承一个方法，你的类直接不支持这些运算，并且试图使用它们会引发一个异常。一些内置操作，比如打印，有默认的重载方法（继承自Python 3.x中隐含的object类），但是，如果没有给出相应的运算符重载方法的话，大多数内置函数会对类实例失败。



多数重载方法只用在需要对象行为表现得就像内置类型一样的高级程序中。然而__init__构造函数常出现在绝大多数类中。我们已见到过__init__初始定义构造函数，以及上表中的一些其他的方法。让我们通过例子来说明表中的其他方法吧。





索引和分片：__getitem__ 和 __setitem__

如果在类中定义了（或继承了）的话，则对于实例的索引运算，会自动调用__getitem__。当实例X出现在X[i]这样的索引运算中时，Python会调用这个实例继承的__getitem__方法（如果有的话），把X作为第一个参数传递，并且方括号类的索引值传给第二个参数。例如，下面的类将返回索引值的平方。

>>> class Indexer:
>>> def __getitem__(self, index):
>>> return index ** 2

>>> X = Indexer()
>>> X[2]                                # X[i] calls X.__getitem__(i)
>>> 4

>>> for i in range(5):
>>> print(X[i], end=' ')            # Runs __getitem__(X, i) each time

0 1 4 9 16
1.
2.
3.
4.
5.
6.
7.
8.
9.
10.
11.
12.


拦截分片

有趣的是，除了索引，对于分片表达式也调用__getitem__。 正式地讲，内置类型以同样的方式处理分片。例如，下面是在一个内置列表上工作的分片，使用了上边界和下边界以及一个stride（可以回顾关于分片的知识）：

>>> L = [5, 6, 7, 8, 9]
>>> L[2:4]                        # Slice with slice syntax
>>> [7, 8]
>>> L[1:]
>>> [6, 7, 8, 9]
>>> L[:-1]
>>> [5, 6, 7, 8]
>>> L[::2]
>>> [5, 7, 9]
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 实际上，分片边界绑定到了一个分片对象中，并且传递给索引的列表实现。实际上，我们总是可以手动地传递一个分片对象……分片语法主要是用一个分片对象进行索引的语法糖：

>>> L[slice(2, 4)]                # Slice with slice objects
>>> [7, 8]
>>> L[slice(1, None)]
>>> [6, 7, 8, 9]
>>> L[slice(None, -1)]
>>> [5, 6, 7, 8]
>>> L[slice(None, None, 2)]
>>> [5, 7, 9]
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 对于带有一个__getitem__的类，这是很重要的……该方法将既针对基本索引（带有一个索引）调用，又针对分片（带有一个分片对象）调用。我们前面的类没有处理分片，因为它的数学假设传递了整数索引，但是，如下类将会处理分片。当针对索引调用的时候，参数像前面一样是一个整数：

>>> class Indexer:
>>> data = [5, 6, 7, 8, 9]
>>> def __getitem__(self, index):
>>> print("getitem:", index)
>>> return self.data[index]

>>> X = Indexer()
>>> X[0]
>>> getitem: 0
>>> 5
>>> X[1]
>>> getitem: 1
>>> 6
>>> X[-1]
>>> getitem: -1
>>> 9
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 16.
>>> 然而，当针对分片调用的时候，方法接收一个分片对象，它在一个新的索引表达式中直接传递给嵌套的列表索引：

>>> X[2:4]
>>> getitem: slice(2, 4, None)
>>> [7, 8]
>>> X[1:]
>>> getitem: slice(1, None, None)
>>> [6, 7, 8, 9]
>>> X[:-1]
>>> getitem: slice(None, -1, None)
>>> [5, 6, 7, 8]
>>> X[::2]
>>> getitem: slice(None, None, 2)
>>> [5, 7, 9]
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 如果使用的话，__setitem__索引赋值方法类似地拦截索引和分片赋值……它为后者接收了一个分片对象，它可能以同样的方式传递到另一个索引赋值中：

def __setitem__(self, index, value):
    ...
    self.data[index] = value
1.
2.
3.
实际上，__getitem__可能在甚至比索引和分片更多的环境中自动调用，正如下面的小节所介绍的。

                            Python 2.6中的分片和索引

在Python 3.0之前，类也可以定义__getslice__和__setslice__方法来专门拦截分片获取和赋值；它们将传递一系列的分片表达式，并且优先于__getitem__和__setitem__用于分片。



这些特定于分片的方法已经从Python 3.0中移除了，因此，你应该使用__getitem__和__setitem__来替代，以考虑到索引和分片对象都可能作为参数。



索引迭代：__getitem__

初学者可能不见得马上就能领会这里的技巧，但这些技巧都是非常有用的，for语句的作用是从0到更大的索引值，重复对序列进行索引运算，知道检测到超出边界的异常。因此，__getitem__也可以是Python中一种重载迭代的方式。如果定义了这个方法，for循环每次循环时都会调用类的__getitem__，并持续搭配有更高的偏移值。这是一种“买一送一”的情况：任何会响应索引运算的内置或用户定义的对象，同样会响应迭代。

>>> class stepper:
>>> def __getitem__(self, i):
>>> return self.data[i]

>>> X = stepper()
>>> X.data = "Spam"
>
>>> X[1]
>>> 'p'
>>> for item in X:
>>> print(item, end=' ')

S p a m
1.
2.
3.
4.
5.
6.
7.
8.
9.
10.
11.
12.
13.
事实上，这其实是“买一送一”的情况。任何支持for循环的类都会自动支持Python所有迭代环境，而其中多种环境我们已经在前面看过了。例如，成员关系测试in、列表解析、内置函数map、列表和元组赋值运算以及类型构造方法也会自动调用__getitem__（如果定义了的话）。

>>> 'p' in X
>>> True

>>> [c for c in X]
>>> ['S', 'p', 'a', 'm']

>>> list(map(str.upper, X))
>>> ['S', 'P', 'A', 'M']

>>> (a, b, c, d) = X
>>> a, b, c
>>> ('S', 'p', 'a')

>>> list(X), tuple(X), ''.join(X)
>>> (['S', 'p', 'a', 'm'], ('S', 'p', 'a', 'm'), 'Spam')

>>> X
>>> <__main__.stepper object at 0x000001E19957DF28>
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 16.
>>> 17.
>>> 18.
>>> 在实际应用中，这个技巧可用于建立提供序列接口的对下你给，并新增逻辑到内置的序列类型运算。







迭代器对象：__iter__ 和 __next__

尽管上文中的__getitem__技术有效，但它真的只是迭代的一种退而求其次的的方法。如今，Python中所有的迭代环境都会先尝试__iter__方法，再尝试__getitem__。也就是说，它们宁愿使用迭代协议，然后才是重复对对象进行索引运算。只有在对象不支持迭代协议的时候，才会尝试索引运算。一般来讲，你也应该优先使用__iter__，它能够比__getiter__更好地支持一般的迭代环境。



从技术角度来讲，迭代环境是通过调用内置函数iter去尝试寻找__iter__方法来实现的，而这种方法应该返回一个迭代器对象。如果已经提供了，Python就会重复调用这个迭代器对象的next方法，知道发生StopIteration异常。如果没找到这类__iter__方法，Python会改用__getitem__机制，就像之前那样通过偏移量重复索引，知道发生IndexError异常（对于手动迭代来说，一个next内置函数也可以很方便地使用：next(I)与I.__next__()是相同的）。



用户定义的迭代器

在__iter__机制中，类就是通过实现迭代器协议来实现用户定义的迭代器的。例如，下面的iters.py，定义了用户定义的迭代器来生成平方值。

>>> class Squares:
>>> def __init__(self, start, stop):
>>> self.value = start - 1
>>> self.stop = stop
>>> def __iter__(self):
>>> return self
>>> def __next__(self):
>>> if self.value == self.stop:
>>> raise StopIteration
>>> self.value += 1
>>> return self.value ** 2

>>> for i in Squares(1, 5):
>>> print(i, end=' ')

1 4 9 16 25
1.
2.
3.
4.
5.
6.
7.
8.
9.
10.
11.
12.
13.
14.
15.
16.
在这里，迭代器对象就是实例self，应为next方法是这个类的一部分。在较为浮渣的场景中，迭代器对象可定义为个别的类或自己的状态信息的对象，对相同数据支持多种迭代（下面会看到这种例子）。以Python raise语句发出的信号表示迭代结束。手动迭代对内置类型也有效：

>>> X = Squares(1, 5)
>>> I = iter(X)
>>> next(I)
>>> 1
>>> next(I)
>>> 4
>>> ......
>>> next(I)
>>> 25
>>> next(I)
>>> Traceback (most recent call last):
>>> File "<pyshell#91>", line 1, in <module>
>>> next(I)
>>> File "<pyshell#77>", line 9, in __next__
>>> raise StopIteration
>>> StopIteration
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 16.
>>> __getitem__所写的等效代码可能不是很自然，应为for会对所有的0和比较高值的偏移值进行迭代。传入的偏移值和所产生的值的范围只有间接的关系（0..N需要因设为start..stop）。因为__iter__对象会在调用过程中明确地保留状态信息，所以比__getitem__具有更好的通用性。



另外，有时__iter__迭代器会比__getitem__更复杂和难用。迭代器是用来迭代，不是随机的索引运算。事实上，迭代器根本没有重载索引表达式：

>>> X = Squares(1, 5)
>>> X[1]
>>> Traceback (most recent call last):
>>> File "<pyshell#95>", line 1, in <module>
>>> X[1]
>>> TypeError: 'Squares' object does not support indexing
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> __iter__机制也是我们在__getitem__中所见到的其他所有迭代环境的实现方式（成员关系测试、类型构造函数、序列赋值运算等）。然而，和__getitem__不同的是，__iter__只循环一次，而不是循环多次。例如Squares类只循环一次，循环之后就变为空。每次新的循环，都得创建一个新的迭代器对象。

>>> X = Squares(1, 5)
>>> [n for n in X]
>>> [1, 4, 9, 16, 25]
>>> [n for n in X]
>>> []
>>> [n for n in Squares(1, 5)]
>>> [1, 4, 9, 16, 25]
>>> list(Squares(1, 3))
>>> [1, 4, 9]
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 注意，如果用生成器函数编写，这个例子可能更简单一些。

>>> def gsquares(start, stop):
>>> for i in range(start, stop + 1):
>>> yield i ** 2

>>> for i in gsquares(1, 5):
>>> print(i, end=' ')

1 4 9 16 25
1.
2.
3.
4.
5.
6.
7.
8.
和类不同的是，这个函数会自动在迭代中存储状态。当然，这是假设的例子。实际上，可以跳过这两种技术，只用for循环、map或是列表解析，一次创建这个列表。在Python中，完成任务最佳而且是最快的方式通常也是最简单的方式：

>>> [x ** 2 for x in range(1, 6)]
>>> [1, 4, 9, 16, 25]
>>> 1.
>>> 2.
>>> 然而，在模拟更复杂的迭代时，类会比较好用，特别是能够获益于状态信息和继承层次。 下一节要探索这种情况下的使用例子。





有多个迭代器的对象

之前，提到过迭代器对象可以定义成一个独立的类。有其自己的状态信息，从而能够支持相同数据的多个迭代。考虑一下，当步进到字符串这类内置类型时，会发生什么事情。

>>> S = 'ace'
>>> for x in S:
>>> for y in S:
>>> print(x + y, end=' ')

aa ac ae ca cc ce ea ec ee
1.
2.
3.
4.
5.
6.
在这里，外层循环调用iter从字符串中取得迭代器，而每个嵌套的循环也做相同的事来获得独立的迭代器。因为每个激活状态下的迭代器都有自己的状态信息，而不管其他激活状态下的循环时什么状态。



我们前面看到过相关例子。例如，生成器函数和表达式，以及map和zip这样的内置函数，都证明是单迭代对象；相反，range内置函数和其他的内置类型（如列表），支持独立位置的多个活跃迭代器。



当我们用这类编写用户定义的迭代器的时候，由我们来决定是支持一个单个的或是多个活跃的迭代。要达到多个迭代器的效果，__iter__只需替迭代器定义新的状态对象，而不是返回self。



例如，下面定义了一个迭代器类，迭代时，跳过下一个元素。因为迭代器对象是在每次迭代时都重新创建，所以能够支持多个处于激活状态下的循环。

    class SkipIterator:
    def __init__(self, wrapped):
    	self.wrapped = wrapped
    	self.offset = 0
    def __next__(self):
    	if self.offset >= len(self.wrapped):
    		raise StopIteration
    	else:
    		item = self.wrapped[self.offset]
    		self.offset += 2
    		return item


​		
    class SkipObject:
    def __init__(self, wrapped):
    	self.wrapped = wrapped
    def __iter__(self):
    	return SkipIterator(self.wrapped)


​	
    if __name__ == '__main__':
    alpha = 'abcdef'
    skipper = SkipObject(alpha)
    I = iter(skipper)
    print(next(I), next(I), next(I))
    
    for x in skipper:
    	for y in skipper:
    		print(x + y, end=' ')
1.
2.
3.
4.
5.
6.
7.
8.
9.
10.
11.
12.
13.
14.
15.
16.
17.
18.
19.
20.
21.
22.
23.
24.
25.
26.
27.
28.
29.
运行时，这个例子工作起来就像是对内置字符串进行嵌套循环一样，因为每个循环都会获得独立的迭代器对象来记录自己的状态信息，所以每个激活状态下的循环都有自己在字符串中的位置。

a c e
aa ac ae ca cc ce ea ec ee
1.
2.
作为对比，除非我们在嵌套循环中再次调用Squares来获得新的迭代对象，否者之前的Squares例子只支持一个激活状态下的迭代。在这里，只有SkipOjbect，但从该对象中创建了许多的迭代器对象。



就像往常一样，我们可以用内置工具达到类似的效果。例如，用第三参数边界值进行分片运算来跳过元素。

>>> S = 'abcdef'
>>> for x in S[::2]:
>>> for y in S[::2]:
>>> print(x + y, end=' ')

aa ac ae ca cc ce ea ec ee
1.
2.
3.
4.
5.
6.
然而，并非不完全相同，主要有两个原因。首先，这里的每个分片表达式，实质上都是一次把结果存储在内存中；另一方面，迭代器则是一次产生一个值，这样使大型结果列表节省了实际的空间。其次，分片产生的新对象，其实我们没有对同一个对象进行多处的循环。为了更接近于类，我们需要事先创建一个独立的对象通过分片运算进行步进。

>>> S = 'abcdef'
>>> S = S[::2]
>>> S
>>> 'ace'
>>> for x in S:
>>> for y in S:
>>> print(x + y, end=' ')

aa ac ae ca cc ce ea ec ee
1.
2.
3.
4.
5.
6.
7.
8.
9.
这样与基于类的解决办法更相似一些，但是，它仍是一次性把分片结果存储在内存中（目前内置分片运算并没有生成器），并且只等效于这里跳过一个元素的特殊情况。



因为迭代器能够做类能做的任何事，所以它比这个例子所展示出来的更通用。无论我们的应用程序是否需要这种通用性，用户定义的迭代器都是强大的工具，可让我们把任意对象的外观和用法变得很像书本所遇到过的其他序列和可迭代对象。例如，我们可将这项技术在数据库对象中运用，通过迭代进行数据库的读取，让多个游标进入同一个查询结果。







成员关系：__contains__、__iter__和__getitem__

迭代器的内容比我们目前所见到的还要丰富。运算符重载往往是多个层级的：类可以提供特定的方法，或者用作退而求其次选项的更通用的替代方案。例如：

Python 2.6中的比较使用__lt__这样的特殊方法来表示少于比较（如果有的话），或者使用通用的__cmp__。Python 3只使用特殊的方法，而不是__cmp__，如本文后面所介绍的。

布尔测试类似于先尝试一个特定的__bool__（以给出一个明确的True/False结果），并且，如果没有它，将会退而求其次到更通用的__len__（一个非零的长度意味着True）。正如我们将在本文随后见到的，Python 2.6也一样起作用，但是，使用名称__nonzero__而不是__bool__。



在迭代领域，类通常把in成员关系运算符实现为一个迭代，使用__iter__方法或__getitem__方法。要支持更加特定的成员关系，类可能编写一个__contains__方法……当出现的时候，该方法优先于__iter__方法，__iter__方法优先于__getitem__方法。__contains__方法应该把成员关系定义为对一个映射应用键（并且可以使用快速查找），以及用于序列的搜索。



考虑如下的类，它编写了所有3个方法和测试成员关系以及应用于一个实例的各种迭代环境。调用的时候，其方法会打印出跟踪消息：

class Iters:
    def __init__(self, value):
        self.data = value

    def __getitem__(self, i):
        print('get[%s]:' % i, end=' ')
        return self.data[i]
    
    def __iter__(self):
        print("iter=> ", end=' ')
        self.ix = 0
        return self
    
    def __next__(self):
        print('next:', end=' ')
        if self.ix == len(self.data):
            raise StopIteration
        item = self.data[self.ix]
        self.ix += 1
        return item
    
    def __contains__(self, x):
        print('contains: ', end=' ')
        return x in self.data


X = Iters([1, 2, 3, 4, 5])
print(3 in X)
for i in X:
    print(i, end=" | ")

print()
print([i ** 2 for i in X])
print(list(map(bin, X)))
I = iter(X)
while True:
    try:
        print(next(I), end=' @ ')
    except StopIteration:
        break
1.
2.
3.
4.
5.
6.
7.
8.
9.
10.
11.
12.
13.
14.
15.
16.
17.
18.
19.
20.
21.
22.
23.
24.
25.
26.
27.
28.
29.
30.
31.
32.
33.
34.
35.
36.
37.
38.
39.
40.
这段脚本运行的时候，其输出如下所示……特定的__contains__拦截成员关系，通用__iter__捕获其他的迭代环境以至__next__重复地被调用，而__getitem__不会被调用：

contains:  True
iter=>  next: 1 | next: 2 | next: 3 | next: 4 | next: 5 | next: 
iter=>  next: next: next: next: next: next: [1, 4, 9, 16, 25]
iter=>  next: next: next: next: next: next: ['0b1', '0b10', '0b11', '0b100', '0b101']
iter=>  next: 1 @ next: 2 @ next: 3 @ next: 4 @ next: 5 @ next:
1.
2.
3.
4.
5.
但是，要观察如果注释掉__contains__方法后的代码的输出发生了什么变化……成员关系现在路由到了通用的__iter__：

iter=>  next: next: next: True
iter=>  next: 1 | next: 2 | next: 3 | next: 4 | next: 5 | next: 
iter=>  next: next: next: next: next: next: [1, 4, 9, 16, 25]
iter=>  next: next: next: next: next: next: ['0b1', '0b10', '0b11', '0b100', '0b101']
iter=>  next: 1 @ next: 2 @ next: 3 @ next: 4 @ next: 5 @ next:
1.
2.
3.
4.
5.
最后，如果__contains__和__iter__都注释掉的话，其输出如下……索引__getitem__替代方法会被调用，针对成员关系和其他迭代环境使用的连续较高的索引：

get[0]: get[1]: get[2]: True
get[0]: 1 | get[1]: 2 | get[2]: 3 | get[3]: 4 | get[4]: 5 | get[5]: 
get[0]: get[1]: get[2]: get[3]: get[4]: get[5]: [1, 4, 9, 16, 25]
get[0]: get[1]: get[2]: get[3]: get[4]: get[5]: ['0b1', '0b10', '0b11', '0b100', '0b101']
get[0]: 1 @ get[1]: 2 @ get[2]: 3 @ get[3]: 4 @ get[4]: 5 @ get[5]:
1.
2.
3.
4.
5.
正如我们所看到的，__getitem__方法甚至更加通用：除了迭代，它还拦截显示索引和分片。分片表达式用包含边界的一个分片对象来触发__getitem__，既针对内置类型，也针对用户定义的类，因此，我们的类中分片是自动化的：

>>> X = Iters('spam')
>>> X[0]
>>> get[0]: 's'

>>> 'spam'[1:]
>>> 'pam'
>>> 'spam'[slice(1, None)]
>>> 'pam'

>>> X[1:]
>>> get[slice(1, None, None)]: 'pam'
>>> X[:-1]
>>> get[slice(None, -1, None)]: 'spa'
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 然而，在并非面向序列、更加现实的迭代用例中，__iter__方法可能很容易编写，因为它不必管理一个整数序列，并且__contains__考虑到作为一种特殊情况化成员关系。





属性引用：__getattr__和__setattr__

__getattr__方法是拦截属性点号运算。更确切的说，当通过未定义（不存在）属性名称和实例警醒点号运算时，就会用属性名称作为字符串调用这个方法。如果Python可通过其继承树搜索找到这个属性，该方法就不会调用。应为有这种情况，所以__getattr__可以作为钩子来通过通用的方式响应属性请求，例子如下：

>>> class empty:
>>> def __getattr__(self, attrname):
>>> if attrname == 'age':
>>> return 40
>>> else:
>>> raise AttributeError(attrname)

>>> X = empty()
>>> X.age
>>> 40
>>> X.name
>>> Traceback (most recent call last):
>>> File "<pyshell#251>", line 1, in <module>
>>> X.name
>>> File "<pyshell#248>", line 6, in __getattr__
>>> raise AttributeError(attrname)
>>> AttributeError: name
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 16.
>>> 17.
>>> 在这里，empty类和其实例X本身并没有属性，所以对X.age的存取会转至__getattr__方法，self则赋值为实例X，而attrname则赋值为未定义的属性名称字符串“age”。这个类传回一个实际值作为X.age点号表达式的结果40，让age看起来像实际的属性。实际上，age编程了动态计算的属性。



对于类不知道该如何处理的属性，这个__getattr__会引发内置的 AttributeError异常，高数Python，那真的是未定义的属性。请求X.name时，会引发错误。当我们在后面文章看到实际的委托和内容属性时，你会再看到__getattr__。



有个相关的重载方法__setattr__会拦截所有属性的赋值语句。如果定义了这个方法，self.attr = value就会变成self.__setattr__('attr', value)。这一点技巧性很高，因为在__setattr__中对任何self属性做赋值，都会再调用__setattr__，导致了无穷递归循环（最后就是堆栈溢出异常）。如果想使用这个方法，要确定是通过对属性字典做索引运算来赋值任何实例属性。也就是说，是使用self.__dict__['name'] = x，而不是self.name = x。

>>> class accesscontrol:
>>> def __setattr__(self, attr, value):
>>> if attr == 'age':
>>> self.__dict__[attr] = value
>>> else:
>>> raise AttributeError(attr + 'not allowed')


>>> X = accesscontrol()
>>> X.age = 40
>>> X.age
>>> 40
>>> X.name = 'mel'
>>> Traceback (most recent call last):
>>> File "<pyshell#270>", line 1, in <module>
>>> X.name = 'mel'
>>> File "<pyshell#265>", line 6, in __setattr__
>>> raise AttributeError(attr + 'not allowed')
>>> AttributeError: namenot allowed
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 16.
>>> 17.
>>> 18.
>>> 19.
>>> 有两个属性访问重载方法，允许我们控制或特化对象中的属性的访问。它们倾向于扮演高度专用的角色，其中的一些我们将在其他文章中介绍。





其他属性管理工具

为了方便将来参考，还要注意，有其他的方式来管理Python中的属性访问：

__getattribute__方法拦截所有的属性获取，而不仅是那些未定义的，但是，当使用它的时候，必须必使用__getattr__更小心地避免循环。

Property内置函数允许我们将方法和特定类属性上的获取和设置操作关联起来。

描述符提供了一个协议，把一个类的__get__和__set__方法与对特定类属性的访问关联起来。



由于这些颇有些高级的工具并不是每个Python程序员都有用，所以我们将推迟到其他文章在介绍这些特性。





模拟实例属性的私有性：第一部分

下列程序代码上把上一个例子通用化了，让每个子类都有自己的私有变量名列表，这些变量名无法通过其实例进行赋值。

class PrivateExc(Exception): pass

class Privacy:
    def __setattr__(self, attrname, value):
        if attrname in self.privates:
            raise PrivateExc(attrname, self)
        else:
            self.__dict__[attrname] = value

class Test1(Privacy):
    privates = ['age']

class Test2(Privacy):
    privates = ['name', 'pay']
    def __init__(self):
        self.__dict__['name'] = 'Tom'

x = Test1
y = Test2

x.name = 'Bob'
y.name = 'Sue'

y.age = 30
x.age = 40
1.
2.
3.
4.
5.
6.
7.
8.
9.
10.
11.
12.
13.
14.
15.
16.
17.
18.
19.
20.
21.
22.
23.
24.
25.
实际上，这是Python中实现属性私有性（也就是无法在类外部对属性名进行修改）的首选方法。虽然Python不支持private声明，但类似这种技术可以模拟其主要的目的。不过，这只是一部分的解决方案。为使其更有效，必须增强它的功能，让子类也能够设置私有属性，并且使用__getattr__和包装（有时称为代理）来检车对私有属性的读取。



我们将推迟到装饰器一文中再给出属性私有性的一个更完整的解决方案，在那里，我们将使用类装饰器来更加通用地拦截和验证属性。即使私有性可以以此方式模拟，但实际应用中几乎不会这么做。不用private声明，Python程序员就可以编写大型的OOP软件框架和应用程序：这是关于访问控制的一般意义上的、有趣的发现，超出了我们这里的介绍范围。



捕捉属性引用值和赋值，往往是很有用的技术。这可支持委托，也是一种设计技术，可以让控制器对象包裹内嵌的对象，增加新行为，并且把其他运算传回包装的对象。





__repr__和__str__会返回字符串表达形式

下个例子是已经见过的__init__构造函数和__add__重载方法，本例也会定义返回实例的字符串表达形式的__repr__方法。字符串格式把self.data对象转换为字符串。如果定义了的话，当类的实例打印或转换成字符串时__repr__（或其近亲__str__）就会自动调用。这些方法可替对象定义更好的显示格式，而不是使用默认的实例显示。



实例对象的默认显示既无用也不好看：

>>> class adder:
>>> def __init__(self, value=0):
>>> self.data = value
>>> def __add__(self, other):
>>> self.data += other

>>> x = adder()
>>> print(x)
>>> <__main__.adder object at 0x000001E19959A828>
>>> x
>>> <__main__.adder object at 0x000001E19959A828>
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 但是，编写或继承字符串表示方法允许我们定制显示：

>>> class addrepr(adder):
>>> def __repr__(self):
>>> return 'addrepr(%s)' % self.data

>>> x = addrepr(2)
>>> x + 1
>>> x
>>> addrepr(3)
>>> print(x)
>>> addrepr(3)
>>> str(x), repr(x)
>>> ('addrepr(3)', 'addrepr(3)')
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 那么，为什么要两个显示方法呢？ 概括地讲，是为了进行用户友好的显示。具体来说：

打印操作会首先尝试__str__和str内置函数（print运行的内部等价形式）。它通常应该返回一个用户友好的显示。

__repr__用于所有其他的环境中：用于交互模式下提示回应以及repr函数，如果没有使用__str__，会使用print和str。它通常应该返回一个编码字符串，可以用来重新创建对象，或者给开发者一个详细的显示。

总之而言，__repr__用于任何地方，除了当定义了一个__str__的时候，使用print和str。既然要注意，如果没有定义__str__，打印还是使用__repr__，但反过来并不成立……其他环境，例如，交互式响应模式，只是使用__repr__，并且根本不要尝试__str__：

>>> class addstr(adder):
>>> def __str__(self):
>>> return '[Value: %s]' % self.data

>>> x = addstr(3)
>>> x + 1
>>> x
>>> <__main__.addstr object at 0x000001E19959A828>
>>> print(x)
>>> [Value: 4]
>>> str(x), repr(x)
>>> ('[Value: 4]', '<__main__.addstr object at 0x000001E19959A828>')
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 正是由于这一点，如果想让所有环境都有统一的显示，__repr__是最佳选择。不过，通过分别定义这两个方法，就可在不同环境内支撑不同显示。例如，终端用户显示使用__str__，而程序员在开发期间则使用底层的__repr__来显示。实际上，__str__只是覆盖了__repr__以得到用户友好的显示环境：

>>> class addboth(adder):
>>> def __str__(self):
>>> return '[Value: %s]' % self.data
>>> def __repr__(self):
>>> return 'addboth(%s)' % self.data

>>> x = addboth(4)
>>> x + 1
>>> x
>>> addboth(5)
>>> print(x)
>>> [Value: 5]
>>> str(x), repr(x)
>>> ('[Value: 5]', 'addboth(5)')
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 我在这里应该提到两种用法。首先，记住__str__和__repr__都必须返回字符串；其他的结果类型不会转换并会引发错误，因此，如果必要的话，确保用一个转换器处理它们。其次，根据一个容器的字符串转换逻辑，__str__的用户友好的显示可能只有当对象出现在一个打印操作顶层的时候才应用，嵌套到交大对象中的对象可能用其__repr__或默认方法打印。如下代码说明了这两点：

>>> class Printer:
>>> def __init__(self, val):
>>> self.val = val
>>> def __str__(self):
>>> return str(self.val)

>>> objs = [Printer(2), Printer(3)]
>>> for x in objs: print(x)

2
3
>>> print(objs)
>>> [<__main__.Printer object at 0x000001E19959A828>, <__main__.Printer object at 0x000001E1995A50B8>]
>>> objs
>>> [<__main__.Printer object at 0x000001E19959A828>, <__main__.Printer object at 0x000001E1995A50B8>]
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 为了确保一个定制显示在所有的环境中都显示而不管容器是什么，请编写__repr__，而不是__str__；前者在所有的情况下都运行，即便是后者不适用的情况也是如此：

>>> class Printer:
>>> def __init__(self, val):
>>> self.val = val
>>> def __repr__(self):
>>> return str(self.val)

>>> objs = [Printer(2), Printer(3)]
>>> for x in objs: print(x)

2
3
>>> print(objs)
>>> [2, 3]
>>> objs
>>> [2, 3]
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 在实际应用中，除了__init__以外，__str__（或其底层的近亲__repr__）似乎是Python脚本中第二个最常用的运算符重载方法。在可以打印对象并且看见定制显示的任何时候，可能就是使用了这两个工具中的一个。





右侧加法和原处加法：__radd__和__iadd__

从技术方面来讲，前边例子中出现的__add__方法并不支持+运算符右侧使用实例对象。要实现这类表达式，而支持可互换的运算符，可以一并编写__radd__方法。只有当+右侧的对象是类实例，而左边对象不是类实例时，Python才会调用__radd__。在其他所有情况下，则由左侧对象调用__add__方法。

>>> class Commuter:
>>> def __init__(self, val):
>>> self.val = val
>>> def __add__(self, other):
>>> print('add', self.val, other)
>>> return self.val + other
>>> def __radd__(self, other):
>>> print('radd', self.val, other)
>>> return other + self.val

>>> x = Commuter(88)
>>> y = Commuter(99)
>>> x + 1
>>> add 88 1
>>> 89
>>> 1 + y
>>> radd 99 1
>>> 100
>>> x + y
>>> add 88 <__main__.Commuter object at 0x000001E19959A828>
>>> radd 99 88
>>> 187
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 16.
>>> 17.
>>> 18.
>>> 19.
>>> 20.
>>> 21.
>>> 22.
>>> 注意，__radd__中的顺序与之相反：self是在+的右侧，而other是在左侧。此外，注意x和y是同一个类的实例。当不同类的实例混合出现在表达式时，Python优先选择左侧的那个类。当我们把两个实例相加的时候，Python运行__add__，它反过来通过简化左边的运算数来触发__radd__。



在更为实际的类中，其中类类型可能需要在结果中传播，事情可能变得更需要技巧：类型测试可能需要辨别它是否能够安全地转换并由此避免嵌套。例如，下面的代码中如果没有isinstance测试，当两个实例相加并且__add__触发__radd__的时候，我们最终得到一个Commuter，其val是另一个Commuter：

>>> class Commuter:
>>> def __init__(self, val):
>>> self.val = val
>>> def __add__(self, other):
>>> if isinstance(other, Commuter): other = other.val
>>> return Commuter(self.val + other)
>>> def __radd__(self, other):
>>> return Commuter(other + self.val)
>>> def __str__(self):
>>> return '<Commuter: %s>' % self.val

>>> x = Commuter(88)
>>> y = Commuter(99)
>>> print(x + 10)
>>> <Commuter: 98>
>>> print(10 + y)
>>> <Commuter: 109>

>>> z = x + y
>>> print(z)
>>> <Commuter: 187>
>>> print(z + 10)
>>> <Commuter: 197>
>>> print(z + z)
>>> <Commuter: 374>
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 16.
>>> 17.
>>> 18.
>>> 19.
>>> 20.
>>> 21.
>>> 22.
>>> 23.
>>> 24.
>>> 25.




原处加法

为了实现+=原处扩展加法，编写一个__iadd__或__add__。如果前者空缺的话，使用后者。实际上，前面小节的Commuter类为此已经支持+=了，但是，__iadd__考虑到了更加高效的原处修改。

>>> class Number:
>>> def __init__(self, val):
>>> self.val = val
>>> def __iadd__(self, other):
>>> self.val += other
>>> return self

>>> x = Number(5)
>>> x += 1
>>> x += 1
>>> x.val
>>> 7

>>> class Number:
>>> def __init__(self, val):
>>> self.val = val
>>> def __add__(self, other):
>>> return Number(self.val + other)

>>> x = Number(5)
>>> x += 1
>>> x += 1
>>> x.val
>>> 7
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 14.
>>> 15.
>>> 16.
>>> 17.
>>> 18.
>>> 19.
>>> 20.
>>> 21.
>>> 22.
>>> 23.
>>> 24.
>>> 每个二院运算都有类似的右侧和原处重载方法，它们以相同的方式工作（例如，__mul__，__rmul__和__imul__）。右侧方法是一个高级话题，并且在实际中很少用到；只有在需要运算符具有交换性的时候，才会编写它们，并且只有在真正需要支持这样的运算符的时候，才会使用。例如，一个Vector类可能使用这些工具，但一个Employee或Button类可能就不会。







Call表达式：__call__

当调用实例时，使用__call__方法。不，这不是循环定义：如果定义了，Python就会为实例应用函数调用表达式运行__call__方法。这样可以让类实例的外观和用法类似于函数。

>>> class Callee:
>>> def __call__(self, *pargs, **kargs):
>>> print('Called:', pargs, kargs)

>>> C = Callee()
>>> C(1, 2, 3)
>>> Called: (1, 2, 3) {}
>>> C(1, 2, 3, x=4, y=5)
>>> Called: (1, 2, 3) {'y': 5, 'x': 4}
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 更正式地说，我们将在之前文章介绍的所有参数传递方式，__call__方法都支持……传递给实例的任何内容都会传递给该方法，包括通常隐式的实例参数。例如，方法定义：

class C:
    def __call__(self, a, b, c=5, d=6): ...
    
class C:
    def __call__(self, *pargs, **kargs): ...

class C:
    def __call__(self, *pargs, d=6, **kargs): ...
1.
2.
3.
4.
5.
6.
7.
8.
都匹配如下所有的实例调用：

X = C()
X(1, 2)
X(1, 2, 3, 4)
X(a=1, b=2, d=3)
X(*[1, 2], **dict(c=3, d=4))
X(1, *(2,), c=3, **dict(d=4))
1.
2.
3.
4.
5.
6.
直接的效果是，带有一个__call__的类和实例，支持与常规函数和方法完全相同的参数语法和语义。



像这样的拦截调用表达式允许类实例模拟类似函数的外观，但是，也在调用中保持了状态信息以供使用：

>>> class Prod:
>>> def __init__(self, value):
>>> self.value = value
>>> def __call__(self, other):
>>> return self.value * other

>>> x = Prod(2)
>>> x(3)
>>> 6
>>> x(4)
>>> 8
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 在这个实例中，__call__乍一看可能有点奇怪。一个简单的方法可以提供类似的功能：

>>> class Prod:
>>> def __init__(self, value):
>>> self.value = value
>>> def comp(self, other):
>>> return self.value * other

>>> x = Prod(3)
>>> x.comp(3)
>>> 9
>>> x.comp(4)
>>> 12
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 然而，当需要为函数的API编写接口时，__call__就变得很有用：这可以编写遵循所需要的函数来调用接口对象，同时又能保留状态信息。事实上，这可能是除了__init__构造函数以及__str__和__repr__显示格外方法外，第三个最常用的运算符重载方法了。





函数接口和回调代码

作为例子：tkinter GUI工具箱（在Python 2.6中时Tkinter）可以把函数注册成事件处理器（也就是回调函数callback）。当事件发生时，tkinter会调用已注册的对象。如果想让事件处理器保存事件之间的状态，可以注册类的绑定方法（bound method）或者遵循所需接口的实例（使用__call__）。在这一节的代码中，第二个例子中的x.comp和第一个例子中的x都可以用这种方式作为类似于函数的对象进行传递。



这里仅举一个假设的__call__例子，应用于GUI领域。下列类定义了一个对象，支持函数调用接口，但是也有状态信息，可记住稍后按下按钮后应该变成什么颜色。

>>> class Callback:
>>> def __init__(self, color):
>>> self.color = color
>>> def __call__(self):
>>> print('turn', self.color)
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 现在，在GUI环境中，即使这个GUI期待的事件处理器是无参数的简单函数，我们还是可以为按钮吧这个类的实例注册成事件处理器。

cb1 = Callback('blue')
cb2 = Callback('green')

B1 = Button(command=cb1)
B2 = Button(command=cb2)
1.
2.
3.
4.
5.
当这个按钮按下时，会把实例对象当成简单的函数来调用，就像下面的调用一样。不过，因为它把状态保留成实例的属性，所以知道应该做什么。

cb1()
cb2()
1.
2.
实际上，这可能是Python语言中保留状态信息的最好方式，比之前针对函数所讨论的技术更好（全局变量、嵌套函数作用域引用 以及默认可变参数等）。利用OOP，状态的记忆是明确地使用属性赋值运算而实现的。



在继续之前，Python程序员偶尔还会用两种其他方式，把信息和回调函数联系起来。其中一个选项是使用lambda函数的默认参数：

cb3 = (lambda color="red": "turn" + color)
print(cb3())
1.
2.
另一种是使用类的绑定方法：这种对象记住了self实例以及所引用的函数，使其可以在稍后通过简单的函数调用而不需要实例来实现。

from tkinter import Button

class Callback:
    def __init__(self, color):
        self.color = color
    def changeColor(self):
        print('turn', self.color)

cb1 = Callback("blue")
cb2 = Callback("yellow")

B1 = Button(command=cb1.changeColor)
B2 = Button(command=cb2.changeColor)
1.
2.
3.
4.
5.
6.
7.
8.
9.
10.
11.
12.
13.
当按下按钮时，就好像是GUI这么做的，启用changeColor方法来处理对象的状态信息：

object = Callback("blue")
cb = object.changeColor
cb()
1.
2.
3.
这种技巧较为简单，但是比起__call__重载调用而言就不通用了；同样，有关绑定方法可参考另外的内容。



在类的高级主题你会看到另一个__call__例子，我们会通过它来实现所谓的函数装饰器的概念：它是可调用对象，在嵌入的函数上多加一层逻辑。因为__call__可让我们把状态信息附加在可调用对象上，所以自然而然地成为了被一个函数记住并调用了另一个函数的实现技术。





比较：__lt__、__gt__和其他方法

正如文章开头表格所示，类可以定义方法来捕获所有的6中比较运算符：<,>,<=,>=, == 和 !=。这些方法通常很容易使用，但是，记住如下的一些限制：

与前面讨论的__add__/__radd__对不同，比较方法没有右端形式。相反，当只有一个运算数支持比较的时候，使用其对应方法（例如，__lt__和__gt__互为对应）。

比较运算符没有隐式关系。例如，==并不意味着!=是假的，因此，__eq__和__ne__应该定义为确保两个运算符都正确地作用。

在Python 2.6中，如果没有定义更为具体的比较方法的话，对所有比较使用一个__cmp__方法。它返回一个小于、等于或大于0的数，以表示比较其两个参数（self和另一个操作数）的结果。这个方法往往使用cmp(x, y)内置函数来计算其结果。__cmp__方法和cmp内置函数都从Python 3.0中删除了：使用更特定的方法来替代。



我们没有篇幅来深入介绍比较方法，但是，作为一个快速介绍，考虑如下的类和测试代码：

class C:
    data = 'spam'
    def __gt__(self, other):
        return self.data > other
    def __lt__(self, other):
        return self.data < other

X = C()
print(X > 'ham')            # True
print(X < 'ham')            # False
1.
2.
3.
4.
5.
6.
7.
8.
9.
10.
在Python 3.x和Python 2.6下运行的时候，末尾的打印语句显示他们的注释中得到的结果，因为该类的方法拦截并实现了比较表达式。





Python 2.6的__cmp__方法（已经从Python 3.x中移除了）

在Python 2.6中，如果没有定义更加具体的方法的话，__cmp__方法作为一种退而求其次的方法：它的整数结果用来计算正在运行的运算符。例如，如下的代码在Python 2.6下产生同样的结果，但是在Python 3.x中失败，因为__cmp__不再可用：

class C:                                # 2.6 only
    data = 'spam'
    def __cmp__(self, other):            # __cmp__ not used in 3.x
        return cmp(self.data, other)     # cmp not defined in 3.x

X = C()
print(X > 'ham')                        # True
print(X < 'ham')                        # False
1.
2.
3.
4.
5.
6.
7.
8.
注意，这在Python 3.x中失效是应为__cmp__不在特殊，而不是因为cmp内置函数不再使用。如果我们把前面的类修改为如下的形式，以试图模拟cmp调用，那么代码将在Python 2.6中工作，但在Python 3.x下无效：

class C:
    data = 'spam'
    def __cmp__(self, other):
        return (self.data > other) - (self.data < other)
1.
2.
3.
4.






布尔测试：__bool__和__len__

正如前面所提到的，类可能也定义了赋予其实例布尔特性的方法……在布尔环境中，Python首先尝试__bool__来获取一个直接的布尔值，然后，如果没有该方法，就尝试__len__类根据对象的长度确定一个真值。通常，首先使用对象状态或其他信息来生成一个布尔结果：

>>> class Truth:
>>> def __bool__(self): return True

>>> X = Truth()
>>> if X: print('yes!')
>>> yes!

>>> class Truth:
>>> def __bool__(self): return False

>>> X = Truth()
>>> bool(X)
>>> False
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 13.
>>> 如果没有这个方法，Python退而求其次地求长度，因为一个空对象看做是真（如，一个非零长度意味着对象是真，并且一个零长度意味着它为假）：

>>> class Truth:
>>> def __len__(self): return 0

>>> X = Truth()
>>> if not X: print("no!")

no!
1.
2.
3.
4.
5.
6.
7.
如果两个方法都有，Python喜欢__bool__胜过__len__，因为它更具体：

>>> class Truth:
>>> def __bool__(self): return True
>>> def __len__(self): return 0

>>> X = Truth()
>>> if X: print("yes!")

yes!
1.
2.
3.
4.
5.
6.
7.
8.
如果没有定义真的方法，对象毫无意义地看作为真：

>>> class Truth:
>>> pass

>>> X = Truth()
>>> bool(X)
>>> True
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 既然我们已经尝试突入哲学的领域，让我们进一步看看最后一种重载环境：对象转让。







对象析构函数：__del__

每当实例产生时，就会调用__init__构造函数。每当实例空间被收回时(在垃圾收集时)，它的对立面__del__，也就是析构函数，就会自动执行。

>>> class Life:
>>> def __init__(self, name='unknown'):
>>> print('Hello', name)
>>> self.name = name
>>> def __del__(self):
>>> print("Goodbye", self.name)


>>> brian = Life('Brain')
>>> Hello Brain
>>> brian = 'loretta'
>>> Goodbye Brain
>>> 1.
>>> 2.
>>> 3.
>>> 4.
>>> 5.
>>> 6.
>>> 7.
>>> 8.
>>> 9.
>>> 10.
>>> 11.
>>> 12.
>>> 在这里，当Brian赋值为字符串时，我们会失去Life实例的最后一个引用。因此会触发其析构函数。这样行得通，可用于完成一些清理行为（例如，中断服务器的连接）。然而，基于某些原因，在Python中，析构函数不像其他OOP语言那么常用。



原因之一就是，因为Python在实例回收时，会自动收回该实例所拥有的所有空间，对于空间管理来说，是不需要析构函数的。原因之二是无法轻易地预测实例何时收回，通常最好是在有意调用的方法中（或者try/finally语句）编写代码去终止活动。在某种情况下，系统表中可能还在引用该对象，使析构函数无法执行。
-----------------------------------
©著作权归作者所有：来自51CTO博客作者Professor哥的原创作品，如需转载，请与作者联系，否则将追究法律责任
Python 3 之 运算符重载详解
https://blog.51cto.com/professor/1750391