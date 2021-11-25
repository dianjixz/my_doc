正文

　　Python使用引用计数和垃圾回收来做内存管理，前面也写过一遍文章《Python内存优化》，介绍了在python中，如何profile内存使用情况，并做出相应的优化。本文介绍两个更致命的问题：内存泄露与循环引用。内存泄露是让所有程序员都闻风丧胆的问题，轻则导致程序运行速度减慢，重则导致程序崩溃；而循环引用是使用了引用计数的数据结构、编程语言都需要解决的问题。本文揭晓这两个问题在python语言中是如何存在的，然后试图利用gc模块和objgraph来解决这两个问题。

　　注意：本文的目标是Cpython，测试代码都是运行在Python2.7。另外，本文不考虑C扩展造成的内存泄露，这是另一个复杂且头疼的问题。

 　  本文地址：http://www.cnblogs.com/xybaby/p/7491656.html
一分钟版本
回到顶部

　　（1）python使用引用计数和垃圾回收来释放（free）Python对象

　　（2）引用计数的优点是原理简单、将消耗均摊到运行时；缺点是无法处理循环引用

　　（3）Python垃圾回收用于处理循环引用，但是无法处理循环引用中的对象定义了__del__的情况，而且每次回收会造成一定的卡顿
　　（4）gc module是python垃圾回收机制的接口模块，可以通过该module启停垃圾回收、调整回收触发的阈值、设置调试选项
 
　　（5）如果没有禁用垃圾回收，那么Python中的内存泄露有两种情况：要么是对象被生命周期更长的对象所引用，比如global作用域对象；要么是循环引用中存在__del__
　　（6）使用gc module、objgraph可以定位内存泄露，定位之后，解决很简单
 
　　（7）垃圾回收比较耗时，因此在对性能和内存比较敏感的场景也是无法接受的，如果能解除循环引用，就可以禁用垃圾回收。
　　（8）使用gc module的DEBUG选项可以很方便的定位循环引用，解除循环引用的办法要么是手动解除，要么是使用weakref
python内存管理
回到顶部

　　Python中，一切都是对象，又分为mutable和immutable对象。二者区分的标准在于是否可以原地修改，“原地“”可以理解为相同的地址。可以通过id()查看一个对象的“地址”，如果通过变量修改对象的值，但id没发生变化，那么就是mutable，否则就是immutable。比如：

    >>> a = 5;id(a)
    35170056
    >>> a = 6;id(a)
    35170044

    >>> lst = [1,2,3]; id(lst)
    39117168
    >>> lst.append(4); id(lst)
    39117168

　　a指向的对象（int类型）就是immutable， 赋值语句只是让变量a指向了一个新的对象，因为id发生了变化。而lst指向的对象（list类型）为可变对象，通过方法（append）可以修改对象的值，同时保证id一致。

　　判断两个变量是否相等（值相同）使用==， 而判断两个变量是否指向同一个对象使用 is。比如下面a1 a2这两个变量指向的都是空的列表，值相同，但是不是同一个对象。

    >>> a1, a2 = [], []
    >>> a1 == a2
    True
    >>> a1 is a2
    False

　　

　　为了避免频繁的申请、释放内存，避免大量使用的小对象的构造析构，python有一套自己的内存管理机制。在巨著《Python源码剖析》中有详细介绍，在python源码obmalloc.h中也有详细的描述。如下所示：

![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170919090908056-1998847597.png)

　　可以看到，python会有自己的内存缓冲池（layer2）以及对象缓冲池（layer3）。在Linux上运行过Python服务器的程序都知道，python不会立即将释放的内存归还给操作系统，这就是内存缓冲池的原因。而对于可能被经常使用、而且是immutable的对象，比如较小的整数、长度较短的字符串，python会缓存在layer3，避免频繁创建和销毁。例如：

    >>> a, b = 1, 1
    >>> a is b
    True
    >>> a, b = (), ()
    >>> a is b
    True
    >>> a, b = {}, {}
    >>> a is b
    False

　　本文并不关心python是如何管理内存块、如何管理小对象，感兴趣的读者可以参考伯乐在线和csdn上的这两篇文章。

　　本文关心的是，一个普通的对象的生命周期，更明确的说，对象是什么时候被释放的。当一个对象理论上（或者逻辑上）不再被使用了，但事实上没有被释放，那么就存在内存泄露；当一个对象事实上已经不可达（unreachable），即不能通过任何变量找到这个对象，但这个对象没有立即被释放，那么则可能存在循环引用。
引用计数

　　引用计数（References count），指的是每个Python对象都有一个计数器，记录着当前有多少个变量指向这个对象。

　　将一个对象直接或者间接赋值给一个变量时，对象的计数器会加1；当变量被del删除，或者离开变量所在作用域时，对象的引用计数器会减1。当计数器归零的时候，代表这个对象再也没有地方可能使用了，因此可以将对象安全的销毁。Python源码中，通过Py_INCREF和Py_DECREF两个宏来管理对象的引用计数，代码在object.h
复制代码
~~~
 1 #define Py_INCREF(op) (                         \
 2     _Py_INC_REFTOTAL  _Py_REF_DEBUG_COMMA       \
 3     ((PyObject*)(op))->ob_refcnt++)
 4 
 5 #define Py_DECREF(op)                                   \
 6     do {                                                \
 7         if (_Py_DEC_REFTOTAL  _Py_REF_DEBUG_COMMA       \
 8         --((PyObject*)(op))->ob_refcnt != 0)            \
 9             _Py_CHECK_REFCNT(op)                        \
10         else                                            \
11         _Py_Dealloc((PyObject *)(op));                  \
12     } while (0)
~~~
复制代码

 　　通过sys.getrefcount(obj)对象可以获得一个对象的引用数目，返回值是真实引用数目加1（加1的原因是obj被当做参数传入了getrefcount函数），例如：

    >>> import sys
    >>> s = 'asdf'
    >>> sys.getrefcount(s)
    2
    >>> a = 1
    >>> sys.getrefcount(a)
    605

　　从对象1的引用计数信息也可以看到，python的对象缓冲池会缓存十分常用的immutable对象，比如这里的整数1。

　　引用计数的优点在于原理通俗易懂；且将对象的回收分布在代码运行时：一旦对象不再被引用，就会被释放掉（be freed），不会造成卡顿。但也有缺点：额外的字段（ob_refcnt）；频繁的加减ob_refcnt，而且可能造成连锁反应。但这些缺点跟循环引用比起来都不算事儿。

 

　　什么是循环引用，就是一个对象直接或者间接引用自己本身，引用链形成一个环。且看下面的例子：
循环引用示例
~~~
1 # -*- coding: utf-8 -*-
 2 import objgraph, sys
 3 class OBJ(object):
 4     pass
 5 
 6 def show_direct_cycle_reference():
 7     a = OBJ()
 8     a.attr = a
 9     objgraph.show_backrefs(a, max_depth=5, filename = "direct.dot")
10 
11 def show_indirect_cycle_reference():
12     a, b = OBJ(), OBJ()
13     a.attr_b = b
14     b.attr_a = a
15     objgraph.show_backrefs(a, max_depth=5, filename = "indirect.dot")
16 
17 if __name__ == '__main__':
18     if len(sys.argv) > 1:
19         show_direct_cycle_reference()
20     else:
21         show_indirect_cycle_reference()


~~~
　　运行上面的代码，使用graphviz工具集（本文使用的是dotty）打开生成的两个文件，direct.dot 和 indirect.dot，得到下面两个图
![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170919095651650-812154604.png)
 　　 　　
![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170919095733931-1174920281.png)
　　通过属性名(attr, attr_a, attr_b）可以很清晰的看出循环引用是怎么产生的

　　前面已经提到，对于一个对象，当没有任何变量指向自己时，引用计数降到0，就会被释放掉。我们以上面左边那个图为例，可以看到，红框里面的OBJ对象想在有两个引用（两个入度），分别来自帧对象frame（代码中，函数局部空间持有对OBJ实例的引用）、attr变量。我们再改一下代码，在函数运行技术之后看看是否还有OBJ类的实例存在，引用关系是怎么样的：
循环引用示例2
~~~
1 # -*- coding: utf-8 -*-
 2 import objgraph, sys
 3 class OBJ(object):
 4     pass
 5 
 6 def direct_cycle_reference():
 7     a = OBJ()
 8     a.attr = a
 9     
10 if __name__ == '__main__':
11     direct_cycle_reference()
12     objgraph.show_backrefs(objgraph.by_type('OBJ')[0], max_depth=5, filename = "direct.dot"


~~~
　　

　　修改后的代码，OBJ实例(a)存在于函数的local作用域。因此，当函数调用结束之后，来自帧对象frame的引用被解除。从图中可以看到，当前对象的计数器（入度）为1，按照引用计数的原理，是不应该被释放的，但这个对象在函数调用结束之后就是事实上的垃圾，这个时候就需要另外的机制来处理这种情况了。
![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170919103210603-75598682.png)
　　python的世界，很容易就会出现循环引用，比如标准库Collections中OrderedDict的实现（已去掉无关注释）：
复制代码
~~~
 1 class OrderedDict(dict):
 2     def __init__(self, *args, **kwds):
 3         if len(args) > 1:
 4             raise TypeError('expected at most 1 arguments, got %d' % len(args))
 5         try:
 6             self.__root
 7         except AttributeError:
 8             self.__root = root = []                     # sentinel node
 9             root[:] = [root, root, None]
10             self.__map = {}
11         self.__update(*args, **kwds)
~~~
复制代码

　　注意第8、9行，root是一个列表，列表里面的元素之自己本身！
垃圾回收

　　这里强调一下，本文中的的垃圾回收是狭义的垃圾回收，是指当出现循环引用，引用计数无计可施的时候采取的垃圾清理算法。

　　在python中，使用标记-清除算法（mark-sweep）和分代（generational）算法来垃圾回收。在《Garbage Collection for Python》一文中有对标记回收算法，然后在《Python内存管理机制及优化简析》一文中，有对前文的翻译，并且有分代回收的介绍。在这里，引用后面一篇文章：

    　　在Python中, 所有能够引用其他对象的对象都被称为容器(container). 因此只有容器之间才可能形成循环引用. Python的垃圾回收机制利用了这个特点来寻找需要被释放的对象. 为了记录下所有的容器对象, Python将每一个 容器都链到了一个双向链表中, 之所以使用双向链表是为了方便快速的在容器集合中插入和删除对象. 有了这个 维护了所有容器对象的双向链表以后, Python在垃圾回收时使用如下步骤来寻找需要释放的对象:

        对于每一个容器对象, 设置一个gc_refs值, 并将其初始化为该对象的引用计数值.
        对于每一个容器对象, 找到所有其引用的对象, 将被引用对象的gc_refs值减1.
        执行完步骤2以后所有gc_refs值还大于0的对象都被非容器对象引用着, 至少存在一个非循环引用. 因此 不能释放这些对象, 将他们放入另一个集合.
        在步骤3中不能被释放的对象, 如果他们引用着某个对象, 被引用的对象也是不能被释放的, 因此将这些 对象也放入另一个集合中.
        此时还剩下的对象都是无法到达的对象. 现在可以释放这些对象了.

 　　关于分代回收：

    　　除此之外, Python还将所有对象根据’生存时间’分为3代, 从0到2. 所有新创建的对象都分配为第0代. 当这些对象 经过一次垃圾回收仍然存在则会被放入第1代中. 如果第1代中的对象在一次垃圾回收之后仍然存货则被放入第2代. 对于不同代的对象Python的回收的频率也不一样. 可以通过gc.set_threshold(threshold0[, threshold1[, threshold2]]) 来定义. 当Python的垃圾回收器中新增的对象数量减去删除的对象数量大于threshold0时, Python会对第0代对象 执行一次垃圾回收. 每当第0代被检查的次数超过了threshold1时, 第1代对象就会被执行一次垃圾回收. 同理每当 第1代被检查的次数超过了threshold2时, 第2代对象也会被执行一次垃圾回收.

　　注意，threshold0，threshold1，threshold2的意义并不相同！

　　为什么要分代呢，这个算法的根源来自于weak generational hypothesis。这个假说由两个观点构成：首先是年亲的对象通常死得也快，比如大量的对象都存在于local作用域；而老对象则很有可能存活更长的时间，比如全局对象，module， class。

　　垃圾回收的原理就如上面提示，详细的可以看Python源码，只不过事实上垃圾回收器还要考虑__del__，弱引用等情况，会略微复杂一些。

　　什么时候会触发垃圾回收呢，有三种情况：

　　（1）达到了垃圾回收的阈值，Python虚拟机自动执行

　　（2）手动调用gc.collect()

　　（3）Python虚拟机退出的时候

　

　　对于垃圾回收，有两个非常重要的术语，那就是reachable与collectable（当然还有与之对应的unreachable与uncollectable），后文也会大量提及。

　　reachable是针对python对象而言，如果从根集（root）能到找到对象，那么这个对象就是reachable，与之相反就是unreachable，事实上就是只存在于循环引用中的对象，Python的垃圾回收就是针对unreachable对象。

　　而collectable是针对unreachable对象而言，如果这种对象能被回收，那么是collectable；如果不能被回收，即循环引用中的对象定义了__del__， 那么就是uncollectable。Python垃圾回收对uncollectable对象无能为力，会造成事实上的内存泄露。

　　
gc module

　　这里的gc（garbage collector）是Python 标准库，该module提供了与上一节“垃圾回收”内容相对应的接口。通过这个module，可以开关gc、调整垃圾回收的频率、输出调试信息。gc模块是很多其他模块（比如objgraph）封装的基础，在这里先介绍gc的核心API。

　　gc.enable(); gc.disable(); gc.isenabled()

　　开启gc（默认情况下是开启的）；关闭gc；判断gc是否开启

　　gc.collection()

　　执行一次垃圾回收，不管gc是否处于开启状态都能使用

　　gc.set_threshold(t0, t1, t2); gc.get_threshold()

　　设置垃圾回收阈值； 获得当前的垃圾回收阈值

　　注意：gc.set_threshold(0)也有禁用gc的效果

　　gc.get_objects()

　　返回所有被垃圾回收器（collector）管理的对象。这个函数非常基础！只要python解释器运行起来，就有大量的对象被collector管理，因此，该函数的调用比较耗时！

　　比如，命令行启动python

    >>> import gc
    >>> len(gc.get_objects())
    3749

　　gc.get_referents(*obj)

　　返回obj对象直接指向的对象

　　gc.get_referrers(*obj)

　　返回所有直接指向obj的对象

　　下面的实例展示了get_referents与get_referrers两个函数

    >>> class OBJ(object):

    ... pass
    ...
    >>> a, b = OBJ(), OBJ()
    >>> hex(id(a)), hex(id(b))
    ('0x250e730', '0x250e7f0')


    >>> gc.get_referents(a)
    [<class '__main__.OBJ'>]
    >>> a.attr = b
    >>> gc.get_referents(a)
    [{'attr': <__main__.OBJ object at 0x0250E7F0>}, <class '__main__.OBJ'>]
    >>> gc.get_referrers(b)
    [{'attr': <__main__.OBJ object at 0x0250E7F0>}, {'a': <__main__.OBJ object at 0x0250E730>, 'b': <__main__.OBJ object at 0x0250E7F0>, 'OBJ': <class '__main__.OBJ'>, '__builtins__': <modu
    le '__builtin__' (built-in)>, '__package__': None, 'gc': <module 'gc' (built-in)>, '__name__': '__main__', '__doc__': None}]
    >>>

　　a, b都是类OBJ的实例，执行"a.attr = b"之后，a就通过‘’attr“这个属性指向了b。

　　gc.set_debug(flags)

　　设置调试选项，非常有用，常用的flag组合包含以下

    　　gc.DEBUG_COLLETABLE： 打印可以被垃圾回收器回收的对象

    　　gc.DEBUG_UNCOLLETABLE： 打印无法被垃圾回收器回收的对象，即定义了__del__的对象

    　　gc.DEBUG_SAVEALL：当设置了这个选项，可以被拉起回收的对象不会被真正销毁（free），而是放到gc.garbage这个列表里面，利于在线上查找问题

内存泄露
回到顶部

　　既然Python中通过引用计数和垃圾回收来管理内存，那么什么情况下还会产生内存泄露呢？有两种情况：

　　第一是对象被另一个生命周期特别长的对象所引用，比如网络服务器，可能存在一个全局的单例ConnectionManager，管理所有的连接Connection，如果当Connection理论上不再被使用的时候，没有从ConnectionManager中删除，那么就造成了内存泄露。

　　第二是循环引用中的对象定义了__del__函数，这个在《程序员必知的Python陷阱与缺陷列表》一文中有详细介绍，简而言之，如果定义了__del__函数，那么在循环引用中Python解释器无法判断析构对象的顺序，因此就不错处理。

 

　　在任何环境，不管是服务器，客户端，内存泄露都是非常严重的事情。

　　如果是线上服务器，那么一定得有监控，如果发现内存使用率超过设置的阈值则立即报警，尽早发现些许还有救。当然，谁也不希望在线上修复内存泄露，这无疑是给行驶的汽车换轮子，因此尽量在开发环境或者压力测试环境发现并解决潜在的内存泄露。在这里，发现问题最为关键，只要发现了问题，解决问题就非常容易了，因为按照前面的说法，出现内存泄露只有两种情况，在第一种情况下，只要在适当的时机解除引用就可以了；在第二种情况下，要么不再使用__del__函数，换一种实现方式，要么解决循环引用。

　　那么怎么查找哪里存在内存泄露呢？武器就是两个库：gc、objgraph

　　在上面已经介绍了gc这个模块，理论上，通过gc模块能够拿到所有的被garbage collector管理的对象，也能知道对象之间的引用和被引用关系，就可以画出对象之间完整的引用关系图。但事实上还是比较复杂的，因为在这个过程中一不小心又会引入新的引用关系，所以，有好的轮子就直接用吧，那就是objgraph。
objgraph

　　objgraph的实现调用了gc的这几个函数：gc.get_objects(), gc.get_referents(), gc.get_referers()，然后构造出对象之间的引用关系。objgraph的代码和文档都写得比较好，建议一读。

　　下面先介绍几个十分实用的API

　　def count(typename)

　　返回该类型对象的数目，其实就是通过gc.get_objects()拿到所用的对象，然后统计指定类型的数目。

　　def by_type(typename)

　　返回该类型的对象列表。线上项目，可以用这个函数很方便找到一个单例对象

　　def show_most_common_types(limits = 10)

　　打印实例最多的前N（limits）个对象，这个函数非常有用。在《Python内存优化》一文中也提到，该函数能发现可以用slots进行内存优化的对象

　　def show_growth()

　　统计自上次调用以来增加得最多的对象，这个函数非常有利于发现潜在的内存泄露。函数内部调用了gc.collect()，因此即使有循环引用也不会对判断造成影响。

　　值得一提，该函数的实现非常有意思，简化后的代码如下：
复制代码
~~~
 1 def show_growth(limit=10, peak_stats={}, shortnames=True, file=None):
 2     gc.collect()
 3     stats = typestats(shortnames=shortnames)
 4     deltas = {}
 5     for name, count in iteritems(stats):
 6         old_count = peak_stats.get(name, 0)
 7         if count > old_count:
 8             deltas[name] = count - old_count
 9             peak_stats[name] = count
10     deltas = sorted(deltas.items(), key=operator.itemgetter(1),
11                     reverse=True)
~~~
复制代码

 

　　注意形参peak_stats使用了可变参数作为默认形参，这样很方便记录上一次的运行结果。在《程序员必知的Python陷阱与缺陷列表》中提到，使用可变对象做默认形参是最为常见的python陷阱，但在这里，却成为了方便的利器！

　　def show_backrefs()

　　生产一张有关objs的引用图，看出看出对象为什么不释放，后面会利用这个API来查内存泄露。

　　该API有很多有用的参数，比如层数限制(max_depth)、宽度限制(too_many)、输出格式控制(filename output)、节点过滤(filter, extra_ignore)，建议使用之间看一些document。

　　def find_backref_chain(obj, predicate, max_depth=20, extra_ignore=()):

　　找到一条指向obj对象的最短路径，且路径的头部节点需要满足predicate函数 （返回值为True）

　　可以快捷、清晰指出 对象的被引用的情况，后面会展示这个函数的威力

　　def show_chain():

　　将find_backref_chain 找到的路径画出来, 该函数事实上调用show_backrefs，只是排除了所有不在路径中的节点。
查找内存泄露

　　在这一节，介绍如何利用objgraph来查找内存是怎么泄露的

　　如果我们怀疑一段代码、一个模块可能会导致内存泄露，那么首先调用一次obj.show_growth()，然后调用相应的函数，最后再次调用obj.show_growth()，看看是否有增加的对象。比如下面这个简单的例子：
复制代码
~~~
 1 # -*- coding: utf-8 -*-
 2 import objgraph
 3 
 4 _cache = []
 5 
 6 class OBJ(object):
 7     pass
 8 
 9 def func_to_leak():
10     o  = OBJ()
11     _cache.append(o)
12     # do something with o, then remove it from _cache 
13 
14     if True: # this seem ugly, but it always exists
15         return 
16     _cache.remove(o)
17 
18 if __name__ == '__main__':
19     objgraph.show_growth()
20     try:
21         func_to_leak()
22     except:
23         pass
24     print 'after call func_to_leak'
25     objgraph.show_growth()
~~~
复制代码

　　运行结果（我们只关心后一次show_growth的结果）如下

    wrapper_descriptor 1073 +13
    member_descriptor 204 +5
    getset_descriptor 168 +5
    weakref 338 +3
    dict 458 +3
    OBJ 1 +1

　　代码很简单，函数开始的时候讲对象加入了global作用域的_cache列表，然后期望是在函数退出之前从_cache删除，但是由于提前返回或者异常，并没有执行到最后的remove语句。从运行结果可以发现，调用函数之后，增加了一个类OBJ的实例，然而理论上函数调用结束之后，所有在函数作用域（local）中声明的对象都改被销毁，因此这里就存在内存泄露。

　　当然，在实际的项目中，我们也不清楚泄露是在哪段代码、哪个模块中发生的，而且往往是发生了内存泄露之后再去排查，这个时候使用obj.show_most_common_types就比较合适了，如果一个自定义的类的实例数目特别多，那么就可能存在内存泄露。如果在压力测试环境，停止压测，调用gc.collet，然后再用obj.show_most_common_types查看，如果对象的数目没有相应的减少，那么肯定就是存在泄露。

　　当我们定位了哪个对象发生了内存泄露，那么接下来就是分析怎么泄露的，引用链是怎么样的，这个时候就该show_backrefs出马了，还是以之前的代码为例，稍加修改：
show_backrefs查看内存泄露
~~~
1 import objgraph
 2 
 3 _cache = []
 4 
 5 class OBJ(object):
 6     pass
 7 
 8 def func_to_leak():
 9     o  = OBJ()
10     _cache.append(o)
11     # do something with o, then remove it from _cache 
12 
13     if True: # this seem ugly, but it always exists
14         return 
15     _cache.remove(o)
16 
17 if __name__ == '__main__':
18     try:
19         func_to_leak()
20     except:
21         pass
22     objgraph.show_backrefs(objgraph.by_type('OBJ')[0], max_depth = 10, filename = 'obj.dot')

show_backrefs查看内存泄露
~~~
　　注意，上面的代码中，max_depth参数非常关键，如果这个参数太小，那么看不到完整的引用链，如果这个参数太大，运行的时候又非常耗时间。

　　然后打开dot文件，结果如下

　　![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170920191806696-341981095.png)

　　可以看到泄露的对象（红框表示），是被一个叫_cache的list所引用，而_cache又是被__main__这个module所引用。

　　对于示例代码，dot文件的结果已经非常清晰，但是对于真实项目，引用链中的节点可能成百上千，看起来非常头大，下面用tornado起一个最最简单的web服务器（代码不知道来自哪里，且没有内存泄露，这里只是为了显示引用关系），然后绘制socket的引用关关系图，代码和引用关系图如下：
tornado_server实例
~~~
1 import objgraph
 2 import errno
 3 import functools
 4 import tornado.ioloop
 5 import socket
 6 
 7 def connection_ready(sock, fd, events):
 8     while True:
 9         try:
10             connection, address = sock.accept()
11             print 'connection_ready', address
12         except socket.error as e:
13             if e.args[0] not in (errno.EWOULDBLOCK, errno.EAGAIN):
14                 raise
15             return
16         connection.setblocking(0)
17         # do sth with connection
18 
19 
20 if __name__ == '__main__':
21     sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
22     sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
23     sock.setblocking(0)
24     sock.bind(("", 8888))
25     sock.listen(128)
26 
27     io_loop = tornado.ioloop.IOLoop.current()
28     callback = functools.partial(connection_ready, sock)
29     io_loop.add_handler(sock.fileno(), callback, io_loop.READ)
30     #objgraph.show_backrefs(sock, max_depth = 10, filename = 'tornado.dot')
31     # objgraph.show_chain(
32     #     objgraph.find_backref_chain(
33     #         sock,
34     #         objgraph.is_proper_module
35     #     ),
36     #     filename='obj_chain.dot'
37     # )
38     io_loop.start()

tornado_server实例

~~~
　　
![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170920192140384-1129990925.png)
　　可见，代码越复杂，相互之间的引用关系越多，show_backrefs越难以看懂。这个时候就使用show_chain和find_backref_chain吧，这种方法，在官方文档也是推荐的，我们稍微改改代码，结果如下：
show_chain查看内存泄露
~~~
1 import objgraph
 2 
 3 _cache = []
 4 
 5 class OBJ(object):
 6     pass
 7 
 8 def func_to_leak():
 9     o  = OBJ()
10     _cache.append(o)
11     # do something with o, then remove it from _cache 
12 
13     if True: # this seem ugly, but it always exists
14         return 
15     _cache.remove(o)
16 
17 if __name__ == '__main__':
18     try:
19         func_to_leak()
20     except:
21         pass
22     # objgraph.show_backrefs(objgraph.by_type('OBJ')[0], max_depth = 10, filename = 'obj.dot')
23     objgraph.show_chain(
24         objgraph.find_backref_chain(
25             objgraph.by_type('OBJ')[0],
26             objgraph.is_proper_module
27         ),
28         filename='obj_chain.dot'
29     )

show_chain查看内存泄露
~~~
　　
![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170920192258868-420261279.png)
　　

　　上面介绍了内存泄露的第一种情况，对象被“非期望”地引用着。下面看看第二种情况，循环引用中的__del__， 看下面的代码：
复制代码
~~~
 1 # -*- coding: utf-8 -*-
 2 import objgraph, gc
 3 class OBJ(object):
 4     def __del__(self):
 5         print('Dangerous!')
 6 
 7 def show_leak_by_del():
 8     a, b = OBJ(), OBJ()
 9     a.attr_b = b
10     b.attr_a = a
11 
12     del a, b
13     print gc.collect()
14 
15     objgraph.show_backrefs(objgraph.by_type('OBJ')[0], max_depth = 10, filename = 'del_obj.dot')
~~~
复制代码

　　上面的代码存在循环引用，而且OBJ类定义了__del__函数。如果没有定义__del__函数，那么上述的代码会报错， 因为gc.collect会将循环引用删除，objgraph.by_type('OBJ')返回空列表。而因为定义了__del__函数，gc.collect也无能为力，结果如下：

　　![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170920192523650-1055819216.png)

　　从图中可以看到，对于这种情况，还是比较好辨识的，因为objgraph将__del__函数用特殊颜色标志出来，一眼就看见了。另外，可以看见gc.garbage（类型是list）也引用了这两个对象，原因在document中有描述，当执行垃圾回收的时候，会将定义了__del__函数的类实例（被称为uncollectable object）放到gc.garbage列表，因此，也可以直接通过查看gc.garbage来找出定义了__del__的循环引用。在这里，通过增加extra_ignore来排除gc.garbage的影响：

 　　将上述代码的最后一行改成：

    　　objgraph.show_backrefs(objgraph.by_type('OBJ')[0], extra_ignore=(id(gc.garbage),),  max_depth = 10, filename = 'del_obj.dot')

　　　

　　![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170920193643243-1346396200.png)

 　　另外，也可以设置DEBUG_UNCOLLECTABLE 选项，直接将uncollectable对象输出到标准输出，而不是放到gc.garbage
循环引用
回到顶部

　　除非定义了__del__方法，那么循环引用也不是什么万恶不赦的东西，因为垃圾回收器可以处理循环引用，而且不准是python标准库还是大量使用的第三方库，都可能存在循环引用。如果存在循环引用，那么Python的gc就必须开启（gc.isenabled()返回True），否则就会内存泄露。但是在某些情况下，我们还是不希望有gc，比如对内存和性能比较敏感的应用场景，在这篇文章中，提到instagram通过禁用gc，性能提升了10%；另外，在一些应用场景，垃圾回收带来的卡顿也是不能接受的，比如RPG游戏。从前面对垃圾回收的描述可以看到，执行一次垃圾回收是很耗费时间的，因为需要遍历所有被collector管理的对象（即使很多对象不属于垃圾）。因此，要想禁用GC，就得先彻底干掉循环引用。

　　同内存泄露一样，解除循环引用的前提是定位哪里出现了循环引用。而且，如果需要在线上应用关闭gc，那么需要自动、持久化的进行检测。下面介绍如何定位循环引用，以及如何解决循环引用。
定位循环引用

　　这里还是是用GC模块和objgraph来定位循环引用。需要注意的事，一定要先禁用gc（调用gc.disable()）， 防止误差。

　　这里利用之前介绍循环引用时使用过的例子： a， b两个OBJ对象形成循环引用
复制代码

 1 # -*- coding: utf-8 -*-
 2 import objgraph, gc
 3 class OBJ(object):
 4     pass
 5 
 6 def show_cycle_reference():
 7     a, b = OBJ(), OBJ()
 8     a.attr_b = b
 9     b.attr_a = a
10 
11 if __name__ == '__main__':
12     gc.disable()
13     for _ in xrange(50):
14         show_cycle_reference()
15     objgraph.show_most_common_types(20)

复制代码

　　运行结果（部分）：

    wrapper_descriptor 1060
    dict 555
    OBJ 100

　　上面的代码中使用的是show_most_common_types，而没有使用show_growth（因为growth会手动调用gc.collect()），通过结果可以看到，内存中现在有100个OBJ对象，符合预期。当然这些OBJ对象没有在函数调用后被销毁，不一定是循环引用的问题，也可能是内存泄露，比如前面OBJ对象被global作用域中的_cache引用的情况。怎么排除是否是被global作用域的变量引用的情况呢，方法还是objgraph.find_backref_chain(obj)，在__doc__中指出，如果找不到符合条件的应用链（chain），那么返回[obj]，稍微修改上面的代码：
纯循环引用判断
~~~
1 # -*- coding: utf-8 -*-
 2 import objgraph, gc
 3 class OBJ(object):
 4     pass
 5 
 6 def show_cycle_reference():
 7     a, b = OBJ(), OBJ()
 8     a.attr_b = b
 9     b.attr_a = a
10 
11 if __name__ == '__main__':
12     gc.disable()
13     for _ in xrange(50):
14         show_cycle_reference()
15     ret = objgraph.find_backref_chain(objgraph.by_type('OBJ')[0], objgraph.is_proper_module)
16     print ret

纯循环引用判断
~~~
　　上面的代码输出：

    [<__main__.OBJ object at 0x0244F810>]　　

　　验证了我们的想法，OBJ对象不是被global作用域的变量所引用。

　　

　　在实际项目中，不大可能到处用objgraph.show_most_common_types或者objgraph.by_type来排查循环引用，效率太低。有没有更好的办法呢，有的，那就是使用gc模块的debug 选项。在前面介绍gc模块的时候，就介绍了gc.DEBUG_COLLECTABLE 选项，我们来试试：
复制代码
~~~
 1 # -*- coding: utf-8 -*-
 2 import gc, time
 3 class OBJ(object):
 4     pass
 5 
 6 def show_cycle_reference():
 7     a, b = OBJ(), OBJ()
 8     a.attr_b = b
 9     b.attr_a = a
10 
11 if __name__ == '__main__':
12     gc.disable() # 这里是否disable事实上无所谓
13     gc.set_debug(gc.DEBUG_COLLECTABLE | gc.DEBUG_OBJECTS)
14     for _ in xrange(1):
15         show_cycle_reference()
16     gc.collect()
17     time.sleep(5)
~~~
复制代码

　　上面代码第13行设置了debug flag，可以打印出collectable对象。另外，只用调用一次show_cycle_reference函数就足够了（这也比objgraph.show_most_common_types方便一点）。在第16行手动调用gc.collect()，输出如下：

    gc: collectable <OBJ 023B46F0>
    gc: collectable <OBJ 023B4710>
    gc: collectable <dict 023B7AE0>
    gc: collectable <dict 023B7930>

　　注意：只有当对象是unreachable且collectable的时候，在collect的时候才会被输出，也就是说，如果是reachable，比如被global作用域的变量引用，那么也是不会输出的。

　　通过上面的输出，我们已经知道OBJ类的实例存在循环引用，但是这个时候，obj实例已经被回收了。那么如果我想通过show_backrefs找出这个引用关系，需要重新调用show_cycle_reference函数，然后不调用gc.collect，通过show_backrefs 和 by_type绘制。有没有更好的办法呢，可以让我在一次运行中发现循环引用，并找出引用链？答案就是使用DEBUG_SAVEALL，下面为了展示方便，直接在命令行中操作（当然，使用ipython更好）

    >>> import gc, objgraph
    >>> class OBJ(object):
    ... pass
    ...
    >>> def show_cycle_reference():
    ... a, b = OBJ(), OBJ()
    ... a.attr_b = b
    ... b.attr_a = a
    ...
    >>> gc.set_debug(gc.DEBUG_SAVEALL| gc.DEBUG_OBJECTS)
    >>> show_cycle_reference()
    >>> print 'before collect', gc.garbage
    before collect []
    >>> print gc.collect()
    4
    >>>
    >>> for o in gc.garbage:
    ... print o
    ...
    <__main__.OBJ object at 0x024BB7D0>
    <__main__.OBJ object at 0x02586850>
    {'attr_b': <__main__.OBJ object at 0x02586850>}
    {'attr_a': <__main__.OBJ object at 0x024BB7D0>}
    >>>
    >>> objgraph.show_backrefs(objgraph.at(0x024BB7D0), 5, filename = 'obj.dot')
    Graph written to obj.dot (13 nodes)
    >>>

　　上面在调用gc.collect之前，gc.garbage里面是空的，由于设置了DEBUG_SAVEALL，那么调用gc.collect时，会将collectable对象放到gc.garbage。此时，对象没有被释放，我们就可以直接绘制出引用关系，这里使用了objgraph.at，当然也可以使用objgraph.by_type， 或者直接从gc.garbage取对象，结果如下：

　　![](https://images2017.cnblogs.com/blog/1089769/201709/1089769-20170922152536431-1632527133.png)

　　出了循环引用，可以看见还有两个引用，gc.garbage与局部变量o，相信大家也能理解。

　　

 

 
消灭循环引用

　　找到循环引用关系之后，解除循环引用就不是太难的事情，总的来说，有两种办法：手动解除与使用weakref。

　　手动解除很好理解，就是在合适的时机，解除引用关系。比如，前面提到的collections.OrderedDict：

    >>> root = []
    >>> root[:] = [root, root, None]
    >>>
    >>> root
    [[...], [...], None]
    >>>
    >>> del root[:]
    >>> root
    []

　　更常见的情况，是我们自定义的对象之间存在循环引用：要么是单个对象内的循环引用，要么是多个对象间的循环引用，我们看一个单个对象内循环引用的例子：
复制代码

 1 class Connection(object):
 2     MSG_TYPE_CHAT = 0X01
 3     MSG_TYPE_CONTROL = 0X02
 4     def __init__(self):
 5         self.msg_handlers = {
 6             self.MSG_TYPE_CHAT : self.handle_chat_msg,
 7             self.MSG_TYPE_CONTROL : self.handle_control_msg
 8         } 
 9 
10     def on_msg(self, msg_type, *args):
11         self.msg_handlers[msg_type](*args)
12 
13     def handle_chat_msg(self, msg):
14         pass
15 
16     def handle_control_msg(self, msg):
17         pass

复制代码

 

　　上面的代码非常常见，代码也很简单，初始化函数中为每种消息类型定义响应的处理函数，当消息到达(on_msg)时根据消息类型取出处理函数。但这样的代码是存在循环引用的，感兴趣的读者可以用objgraph看看引用图。如何手动解决呢，为Connection增加一个destroy（或者叫clear）函数，该函数将 self.msg_handlers 清空（self.msg_handlers.clear()）。当Connection理论上不在被使用的时候调用destroy函数即可。

　　对于多个对象间的循环引用，处理方法也是一样的，就是在“适当的时机”调用destroy函数，难点在于什么是适当的时机。

　　

　　另外一种更方便的方法，就是使用弱引用weakref， weakref是Python提供的标准库，旨在解决循环引用。

　　weakref模块提供了以下一些有用的API：

　　（1）weakref.ref(object, callback = None)

　　创建一个对object的弱引用，返回值为weakref对象，callback: 当object被删除的时候，会调用callback函数，在标准库logging （__init__.py）中有使用范例。使用的时候要用()解引用，如果referant已经被删除，那么返回None。比如下面的例子
复制代码

 1 # -*- coding: utf-8 -*-
 2 import weakref
 3 class OBJ(object):
 4     def f(self):
 5         print 'HELLO'
 6 
 7 if __name__ == '__main__':
 8     o = OBJ()
 9     w = weakref.ref(o)
10     w().f()
11     del o
12     w().f()

复制代码

 

　　运行上面的代码，第12行会抛出异常：AttributeError: 'NoneType' object has no attribute 'f'。因为这个时候被引用的对象已经被删除了

　　（2）weakref.proxy(object, callback = None)

　　创建一个代理，返回值是一个weakproxy对象，callback的作用同上。使用的时候直接用 和object一样，如果object已经被删除 那么跑出异常   ReferenceError: weakly-referenced object no longer exists。
复制代码

 1 # -*- coding: utf-8 -*-
 2 import weakref
 3 class OBJ(object):
 4     def f(self):
 5         print 'HELLO'
 6 
 7 if __name__ == '__main__':
 8     o = OBJ()
 9     w = weakref.proxy(o)
10     w.f()
11     del o
12     w.f()

复制代码

　　注意第10行 12行与weakref.ref示例代码的区别

　　（3）weakref.WeakSet

　　这个是一个弱引用集合，当WeakSet中的元素被回收的时候，会自动从WeakSet中删除。WeakSet的实现使用了weakref.ref，当对象加入WeakSet的时候，使用weakref.ref封装，指定的callback函数就是从WeakSet中删除。感兴趣的话可以直接看源码（_weakrefset.py），下面给出一个参考例子：
复制代码

 1 # -*- coding: utf-8 -*-
 2 import weakref
 3 class OBJ(object):
 4     def f(self):
 5         print 'HELLO'
 6 
 7 if __name__ == '__main__':
 8     o = OBJ()
 9     ws = weakref.WeakSet()
10     ws.add(o)
11     print len(ws) #  1
12     del o
13     print len(ws) # 0

复制代码

　　（4）weakref.WeakValueDictionary， weakref.WeakKeyDictionary

　　实现原理和使用方法基本同WeakSet

 
总结
回到顶部

 　　本文的篇幅略长，首选是简单介绍了python的内存管理，重点介绍了引用计数与垃圾回收，然后阐述Python中内存泄露与循环引用产生的原因与危害，最后是利用gc、objgraph、weakref等工具来分析并解决内存泄露、循环引用问题。