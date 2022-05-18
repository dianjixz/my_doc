2015年11月10日更新 在实践中，我又发现了一些`graphviz`的有趣的特性，比如`时序图`，`rank`以及`图片节点`等。在这里一并更新。

## 前言

日常的开发工作中，为代码添加注释是代码可维护性的一个重要方面，但是仅仅提供注释是不够的，特别是当系统功能越来越复杂，涉及到的模块越来越多的时候，仅仅靠代码就很难从宏观的层次去理解。因此我们需要图例的支持，图例不仅仅包含功能之间的交互，也可以包含复杂的数据结构的示意图，数据流向等。

但是，常用的UML建模工具，如`Visio`等都略显复杂，且体积庞大。对于开发人员，特别是后台开发人员来说，命令行，脚本才是最友好的，而图形界面会很大程度的限制开发效率。相对于鼠标，键盘才是开发人员最好的朋友。

### graphviz简介

本文介绍一个高效而简洁的绘图工具`graphviz`。`graphviz`是贝尔实验室开发的一个开源的工具包，它使用一个特定的`DSL`(领域特定语言): `dot`作为脚本语言，然后使用布局引擎来解析此脚本，并完成自动布局。`graphviz`提供丰富的导出格式，如常用的图片格式，SVG，PDF格式等。

`graphviz`中包含了众多的布局器：

- `dot` 默认布局方式，主要用于有向图
- `neato` 基于spring-model(又称force-based)算法
- `twopi` 径向布局
- `circo` 圆环布局
- `fdp` 用于无向图

`graphviz`的设计初衷是对`有向图/无向图`等进行自动布局，开发人员使用dot脚本定义图形元素，然后选择算法进行布局，最终导出结果。

首先，在dot脚本中定义图的顶点和边，顶点和边都具有各自的属性，比如形状，颜色，填充模式，字体，样式等。然后使用合适的布局算法进行布局。布局算法除了绘制各个顶点和边之外，需要尽可能的将顶点均匀的分布在画布上，并且尽可能的减少边的交叉(如果交叉过多，就很难看清楚顶点之间的关系了)。所以使用`graphviz`的一般流程为：

- 定义一个图，并向图中添加需要的顶点和边
- 为顶点和边添加样式
- 使用布局引擎进行绘制

一旦熟悉这种开发模式，就可以快速的将你的想法绘制出来。配合一个良好的编辑器(vim/emacs)等，可以极大的提高开发效率，与常见的GUI应用的所见即所得模式对应，此模式称为所思即所得。比如在我的机器上，使用Sublime Text 编辑`dot`脚本，然后将`F7/Cmd-B`映射为调用`dot引擎`去绘制当前脚本，并打开一个新的窗口来显示运行结果：

![workspace](https://icodeit.org/images/2015/11/workspace-resized.png)

对于开发人员而言，经常会用到的图形绘制可能包括：函数调用关系，一个复杂的数据结构，系统的模块组成，抽象语法树等。

### 基础知识

graphviz包含3中元素，`图`，`顶点`和`边`。每个元素都可以具有各自的属性，用来定义字体，样式，颜色，形状等。下面是一些简单的示例，可以帮助我们快速的了解graphviz的基本用法。

#### 第一个graphviz图

比如，要绘制一个有向图，包含4个节点`a,b,c,d`。其中`a`指向`b`，`b`和`c`指向`d`。可以定义下列脚本：

```
digraph abc{
	a;
	b;
	c;
	d;
 
	a -> b;
	b -> d;
	c -> d;
}
```

使用`dot`布局方式，绘制出来的效果如下：

![dot-simple](https://icodeit.org/images/2012/01/clip_image004.gif)

默认的顶点中的文字为定义顶点变量的名称，形状为椭圆。边的默认样式为黑色实线箭头，我们可以在脚本中做一下修改，将顶点改为`方形`，边改为`虚线`。

\####定义顶点和边的样式

在`digraph`的花括号内，添加顶点和边的新定义：

```
node [shape="record"];
edge [style="dashed"];
```

则绘制的效果如下：

![dot-simple2](https://icodeit.org/images/2012/01/clip_image006.gif)

#### 进一步修改顶点和边样式

进一步，我们将顶点`a`的颜色改为`淡绿色`，并将`c`到`d`的边改为`红色`，脚本如下：

```
digraph abc{
	node [shape="record"];
	edge [style="dashed"];
	 
	a [style="filled", color="black", fillcolor="chartreuse"];
	b;
	c;
	d;
	 
	a -> b;
	b -> d;
	c -> d [color="red"];
}
```

绘制的结果如下：

![dot-simple3](https://icodeit.org/images/2012/01/clip_image008.gif)

应当注意到，顶点和边都接受属性的定义，形式为在顶点和边的定义之后加上一个由方括号括起来的`key-value`列表，每个`key-value`对由逗号隔开。如果图中顶点和边采用统一的风格，则可以在图定义的首部定义`node`, `edge`的属性。比如上图中，定义所有的顶点为方框，所有的边为虚线，在具体的顶点和边之后定义的属性将覆盖此全局属性。如特定与`a`的绿色，`c`到`d`的边的红色。

#### 以图片为节点

除了颜色，节点还可以使用图片。不过需要注意的是，在使用图片作为节点的时候，需要将本来的形状设置为`none`，并且将`label`置为空字符串，避免出现文字对图片的干扰。

```
digraph abc{
	node [shape="record"];
	edge [style="dashed"];
	 
	a [style="filled", color="black", fillcolor="chartreuse"];
	b;
	c [shape="none", image="logos/browser-icon-chrome-resized.png", label=""];
	d;
	 
	a -> b;
	b -> d;
	c -> d [color="red"];
}
```

![image-node](https://icodeit.org/images/2015/11/image-node.png)

### 子图的绘制

graphviz支持子图，即图中的部分节点和边相对对立(软件的模块划分经常如此)。比如，我们可以将顶点c和d归为一个子图：

```
digraph abc{

	node [shape="record"];
	edge [style="dashed"];
	 
	a [style="filled", color="black", fillcolor="chartreuse"];
	b;
 
    subgraph cluster_cd{
	    label="c and d";
	    bgcolor="mintcream";
	    c;
	    d;
    }
 
	a -> b;
	b -> d;
	c -> d [color="red"];
}
```

将`c`和`d`划分到`cluster_cd`这个子图中，标签为`c and d`,并添加背景色，以方便与主图区分开，绘制结果如下：

![cluster](https://icodeit.org/images/2012/01/clip_image010.gif)

应该注意的是，子图的名称必须以`cluster`开头，否则`graphviz`无法设别。

#### 数据结构的可视化

实际开发中，经常要用到的是对复杂数据结构的描述，`graphviz`提供完善的机制来绘制此类图形。

##### 一个hash表的数据结构

比如一个hash表的内容，可能具有下列结构：

```c
struct st_hash_type {
    int (*compare) ();
    int (*hash) ();
};
 
struct st_table_entry {
    unsigned int hash;
    char *key;
    char *record;
    st_table_entry *next;
};
 
struct st_table {
    struct st_hash_type *type;
    int num_bins; /* slot count */
    int num_entries; /* total number of entries */
    struct st_table_entry **bins; /* slot */
};
```

##### 绘制hash表的数据结构

从代码上看，由于结构体存在引用关系，不够清晰，如果层次较多，则很难以记住各个结构之间的关系，我们可以通过下图来更清楚的展示：

![hash-datastruct](https://icodeit.org/images/2012/01/clip_image012.gif)

脚本如下：

```
digraph st2{
	fontname = "Verdana";
	fontsize = 10;
	rankdir=TB;
	 
	node [fontname = "Verdana", fontsize = 10, color="skyblue", shape="record"];
	 
	edge [fontname = "Verdana", fontsize = 10, color="crimson", style="solid"];
	 
	st_hash_type [label="{<head>st_hash_type|(*compare)|(*hash)}"];
	st_table_entry [label="{<head>st_table_entry|hash|key|record|<next>next}"];
	st_table [label="{st_table|<type>type|num_bins|num_entries|<bins>bins}"];
	 
	st_table:bins -> st_table_entry:head;
	st_table:type -> st_hash_type:head;
	st_table_entry:next -> st_table_entry:head [style="dashed", color="forestgreen"];
}
```

应该注意到，在顶点的形状为`record`的时候，`label`属性的语法比较奇怪，但是使用起来非常灵活。比如，用竖线”|”隔开的串会在绘制出来的节点中展现为一条分隔符。用`<>`括起来的串称为锚点，当一个节点具有多个锚点的时候，这个特性会非常有用，比如节点`st_table`的`type`属性指向`st_hash_type`，第4个属性指向`st_table_entry`等，都是通过锚点来实现的。

我们发现，使用默认的`dot`布局后，绿色的这条边覆盖了数据结构`st_table_entry`，并不美观，因此可以使用别的布局方式来重新布局，如使用`circo`算法：

![circo](https://icodeit.org/images/2012/01/clip_image014.gif)

则可以得到更加合理的布局结果。

#### hash表的实例

另外，这个hash表的一个实例如下：

![hash-instance](https://icodeit.org/images/2012/01/clip_image016.gif)

脚本如下：

```
digraph st{
	fontname = "Verdana";
	fontsize = 10;
	rankdir = LR;
	rotate = 90;
	 
	node [ shape="record", width=.1, height=.1];
	node [fontname = "Verdana", fontsize = 10, color="skyblue", shape="record"];
	 
	edge [fontname = "Verdana", fontsize = 10, color="crimson", style="solid"];
	node [shape="plaintext"];
	 
	st_table [label=<
	    <table border="0" cellborder="1" cellspacing="0" align="left">
	    <tr>
	    <td>st_table</td>
	    </tr>
	    <tr>
	    <td>num_bins=5</td>
	    </tr>
	    <tr>
	    <td>num_entries=3</td>
	    </tr>
	    <tr>
	    <td port="bins">bins</td>
	    </tr>
	    </table>
	>];
	 
	node [shape="record"];
	num_bins [label=" <b1> | <b2> | <b3> | <b4> | <b5> ", height=2];
	node[ width=2 ];
	 
	entry_1 [label="{<e>st_table_entry|<next>next}"];
	entry_2 [label="{<e>st_table_entry|<next>null}"];
	entry_3 [label="{<e>st_table_entry|<next>null}"];
	 
	st_table:bins -> num_bins:b1;
	num_bins:b1 -> entry_1:e;
	entry_1:next -> entry_2:e;
	num_bins:b3 -> entry_3:e;
}
```

上例中可以看到，节点的`label`属性支持类似于`HTML`语言中的TABLE形式的定义，通过行列的数目来定义节点的形状，从而使得节点的组成更加灵活。

#### 软件模块组成图

Apache httpd 模块关系

![httpd](https://icodeit.org/images/2012/01/clip_image018.gif)

在实际的开发中，随着系统功能的完善，软件整体的结构会越来越复杂，通常开发人员会将软件划分为可理解的多个子模块，各个子模块通过协作，完成各种各样的需求。

下面有个例子，是某软件设计时的一个草稿：

![idp](https://icodeit.org/images/2012/01/clip_image020.gif)

IDP支持层为一个相对独立的子系统，其中包括如数据库管理器，配置信息管理器等模块，另外为了提供更大的灵活性，将很多其他的模块抽取出来作为外部模块，而支持层提供一个模块管理器，来负责加载/卸载这些外部的模块集合。

这些模块间的关系较为复杂，并且有部分模块关系密切，应归类为一个子系统中，上图对应的`dot`脚本为：

```
digraph idp_modules{
 
	rankdir = TB;
	fontname = "Microsoft YaHei";
	fontsize = 12;
	 
	node [ fontname = "Microsoft YaHei", fontsize = 12, shape = "record" ]; 
	edge [ fontname = "Microsoft YaHei", fontsize = 12 ];
	 
	    subgraph cluster_sl{
	        label="IDP支持层";
	        bgcolor="mintcream";
	        node [shape="Mrecord", color="skyblue", style="filled"];
	        network_mgr [label="网络管理器"];
	        log_mgr [label="日志管理器"];
	        module_mgr [label="模块管理器"];
	        conf_mgr [label="配置管理器"];
	        db_mgr [label="数据库管理器"];
	    };
	 
	    subgraph cluster_md{
	        label="可插拔模块集";
	        bgcolor="lightcyan";
	        node [color="chartreuse2", style="filled"];
	        mod_dev [label="开发支持模块"];
	        mod_dm [label="数据建模模块"];
	        mod_dp [label="部署发布模块"];
	    };
	 
	mod_dp -> mod_dev [label="依赖..."];
	mod_dp -> mod_dm [label="依赖..."];
	mod_dp -> module_mgr [label="安装...", color="yellowgreen", arrowhead="none"];
	mod_dev -> mod_dm [label="依赖..."];
	mod_dev -> module_mgr [label="安装...", color="yellowgreen", arrowhead="none"];
	mod_dm -> module_mgr [label="安装...", color="yellowgreen", arrowhead="none"];
}
```

#### 状态图

有限自动机示意图

![fsm](https://icodeit.org/images/2012/01/clip_image022.gif)

上图是一个简易有限自动机，接受`a`及`a`结尾的任意长度的串。其脚本定义如下：

```
digraph automata_0 {
	size = "8.5, 11";
	fontname = "Microsoft YaHei";
	fontsize = 10;
	 
	node [shape = circle, fontname = "Microsoft YaHei", fontsize = 10];
	edge [fontname = "Microsoft YaHei", fontsize = 10];
	 
	0 [ style = filled, color=lightgrey ];
	2 [ shape = doublecircle ];
	 
	0 -> 2 [ label = "a " ];
	0 -> 1 [ label = "other " ];
	1 -> 2 [ label = "a " ];
	1 -> 1 [ label = "other " ];
	2 -> 2 [ label = "a " ];
	2 -> 1 [ label = "other " ];
	 
	"Machine: a" [ shape = plaintext ];
}
```

形状值为plaintext的表示不用绘制边框，仅展示纯文本内容，这个在绘图中，绘制指示性的文本时很有用，如上图中的`Machine: a`。

#### OSGi中模块的生命周期图

OSGi中，模块具有生命周期，从安装到卸载，可能的状态具有已安装，已就绪，正在启动，已启动，正在停止，已卸载等。如下图所示：

![osgi](https://icodeit.org/images/2012/01/clip_image024.gif)

对应的脚本如下：

```
digraph module_lc{
	rankdir=TB;
	fontname = "Microsoft YaHei";
	fontsize = 12;
	 
	node [fontname = "Microsoft YaHei", fontsize = 12, shape = "Mrecord", color="skyblue", style="filled"]; 
	edge [fontname = "Microsoft YaHei", fontsize = 12, color="darkgreen" ];
	 
	installed [label="已安装状态"];
	resolved [label="已就绪状态"];
	uninstalled [label="已卸载状态"];
	starting [label="正在启动"];
	active [label="已激活(运行)状态"];
	stopping [label="正在停止"];
	start [label="", shape="circle", width=0.5, fixedsize=true, style="filled", color="black"];
	 
	start -> installed [label="安装"];
	installed -> uninstalled [label="卸载"];
	installed -> resolved [label="准备"];
	installed -> installed [label="更新"];
	resolved -> installed [label="更新"];
	resolved -> uninstalled [label="卸载"];
	resolved -> starting [label="启动"];
	starting -> active [label=""];
	active -> stopping [label="停止"];
	stopping -> resolved [label=""]; 
}
```

### 其他实例

一棵简单的抽象语法树(AST)

表达式 `(3+4)*5` 在编译时期，会形成一棵语法树，一边在计算时，先计算`3+4`的值，最后与5相乘。

![ast-calc](https://icodeit.org/images/2012/01/clip_image026.gif)

对应的脚本如下：

```
digraph ast{
	fontname = "Microsoft YaHei";
	fontsize = 10;
	 
	node [shape = circle, fontname = "Microsoft YaHei", fontsize = 10];
	edge [fontname = "Microsoft YaHei", fontsize = 10];
	node [shape="plaintext"];
	 
	mul [label="mul(*)"];
	add [label="add(+)"];
	 
	add -> 3
	add -> 4;
	mul -> add;
	mul -> 5;
}
```

#### 简单的UML类图

下面是一简单的UML类图，`Dog`和`Cat`都是`Animal`的子类，`Dog`和`Cat`同属一个包，且有可能有联系`(0..n)`。

![uml](https://icodeit.org/images/2012/01/clip_image028.gif)

脚本：

```
digraph G{
	 
	fontname = "Courier New"
	fontsize = 10
	 
	node [ fontname = "Courier New", fontsize = 10, shape = "record" ];
	edge [ fontname = "Courier New", fontsize = 10 ];
	 
	Animal [ label = "{Animal |+ name : String\l+ age : int\l|+ die() : void\l}" ];
	 
	    subgraph clusterAnimalImpl{
	        bgcolor="yellow"
	        Dog [ label = "{Dog||+ bark() : void\l}" ];
	        Cat [ label = "{Cat||+ meow() : void\l}" ];
	    };
	 
	edge [ arrowhead = "empty" ];
	 
	Dog->Animal;
	Cat->Animal;
	Dog->Cat [arrowhead="none", label="0..*"];
}
```

#### 状态图

![status-chart](https://icodeit.org/images/2012/01/clip_image030.gif)

脚本：

```
digraph finite_state_machine {
	rankdir = LR;
	size = "8,5"
	 
	node [shape = doublecircle]; 
	 
	LR_0 LR_3 LR_4 LR_8;
	 
	node [shape = circle];
	 
	LR_0 -> LR_2 [ label = "SS(B)" ];
	LR_0 -> LR_1 [ label = "SS(S)" ];
	LR_1 -> LR_3 [ label = "S($end)" ];
	LR_2 -> LR_6 [ label = "SS(b)" ];
	LR_2 -> LR_5 [ label = "SS(a)" ];
	LR_2 -> LR_4 [ label = "S(A)" ];
	LR_5 -> LR_7 [ label = "S(b)" ];
	LR_5 -> LR_5 [ label = "S(a)" ];
	LR_6 -> LR_6 [ label = "S(b)" ];
	LR_6 -> LR_5 [ label = "S(a)" ];
	LR_7 -> LR_8 [ label = "S(b)" ];
	LR_7 -> LR_5 [ label = "S(a)" ];
	LR_8 -> LR_6 [ label = "S(b)" ];
	LR_8 -> LR_5 [ label = "S(a)" ];
}
```

#### 时序图

```
digraph G { 
    rankdir="LR"; 
    node[shape="point", width=0, height=0]; 
    edge[arrowhead="none", style="dashed"] 

    { 
        rank="same"; 
        edge[style="solided"];
        LC[shape="plaintext"]; 
        LC -> step00 -> step01 -> step02 -> step03 -> step04 -> step05; 
    } 

    { 
        rank="same"; 
        edge[style="solided"];
        Agency[shape="plaintext"];
        Agency -> step10 -> step11 -> step12 -> step13 -> step14 -> step15; 
    } 

    { 
        rank="same"; 
        edge[style="solided"];
        Agent[shape="plaintext"];
        Agent -> step20 -> step21 -> step22 -> step23 -> step24 -> step25; 
    } 

    step00 -> step10 [label="sends email new custumer", arrowhead="normal"]; 
    step11 -> step01 [label="declines", arrowhead="normal"]; 
    step12 -> step02 [label="accepts", arrowhead="normal"]; 
    step13 -> step23 [label="forward to", arrowhead="normal"]; 
    step24 -> step14; 
    step14 -> step04 [arrowhead="normal"]; 
} 
```

`rankdir="LR"`表示，布局从左`L`到右`R`。可以看到，在代码中有`{}`括起来的部分。

```
{ 
    rank="same"; 
    edge[style="solided"];
    Agency[shape="plaintext"];
    Agency -> step10 -> step11 -> step12 -> step13 -> step14 -> step15; 
}
```

每一个`rank="same"`的block中的所有节点都会在同一条线上。我们设置了所有的线为虚线，但是在该block中，将线改为`solided`。

![seq](https://icodeit.org/images/2015/11/seq.png)

### 附录

事实上，从`dot`的语法及上述的示例中，很容易看出，dot脚本很容易被其他语言生成。比如，使用一些简单的数据库查询就可以生成数据库中的ER图的dot脚本。

如果你追求高效的开发速度，并希望快速的将自己的想法画出来，那么`graphviz`是一个很不错的选择。

当然，`graphviz`也有一定的局限，比如绘制时序图(序列图)就很难实现。`graphviz`的节点出现在画布上的位置事实上是不确定的，依赖于所使用的布局算法，而不是在脚本中出现的位置，这可能使刚开始接触`graphviz`的开发人员有点不适应。`graphviz`的强项在于自动布局，当图中的顶点和边的数目变得很多的时候，才能很好的体会这一特性的好处：

![complex](https://icodeit.org/images/2012/01/clip_image034.gif)

比如上图，或者较上图更复杂的图，如果采用手工绘制显然是不可能的，只能通过`graphviz`提供的自动布局引擎来完成。如果仅用于展示模块间的关系，子模块与子模块间通信的方式，模块的逻辑位置等，`graphviz`完全可以胜任，但是如果图中对象的物理位置必须是准确的，如节点A必须位于左上角，节点B必须与A相邻等特性，使用`graphviz`则很难做到。毕竟，它的强项是自动布局，事实上，所有的节点对与布局引擎而言，权重在初始时都是相同的，只是在渲染之后，节点的大小，形状等特性才会影响权重。

本文只是初步介绍了`graphviz`的简单应用，如图的定义，顶点/边的属性定义，如果运行等，事实上还有很多的属性，如画布的大小，字体的选择，颜色列表等，大家可以通过`graphviz`的官网来找到更详细的资料。

文中的代码都已经在[Github](https://github.com/abruzzi/graphviz-scripts)上。


dot语法
无向图: - -
有向图：- >
属性：节点和边显示样式（颜色、形状、线形）； 用[key=value,]表示
注释：//单行注释； ＃注释此行 ； /***/多行注释

属性
label＝“123” 节点标签，替换节点默认名称
shape＝box 节点形状（）
style＝filled 样式 (filled 填充 dashed）
fontcolor＝red 字体颜色
color＝yellow 节点配色
fontname＝Courier 字体
penwidth＝1.0 线条宽度

／／
nodesep=1.0 节点间距
node [color=Red,] # 所有节点属性配置
edge [color=Blue,] # 所有边属性配置

图像属性
label="My Graph"; # 给图像设置标签
rankdir=LR; # 从左到右布局
a->{b c d} # a 分别指向b c d 节点
{rank=same; a, b, c } # 将一组元素放到同一个level 将abc节点放置同一水平
splines="line"; # 让边框变为直线，没有曲线和锐角
K=0.6; # 在布局中影响spring属性，spring属性用于将节点往外推，在twopi和sfdp布局中很有用。
bgcolor 背景颜色
concentrate = false 让多条边有公共部分
nodesep = .25 节点之间的间隔（英寸）
peripheries = 1 边界数
rank (same,min,source, max,sink)设置多个节点顺序
rankdir = TB 排序方向
ranksep = .75 间隔
size 图的大小（英寸）

译注：暂时还没明白这个spring属性应该怎么翻，初步猜测是弹性。

交点属性
[label="Some Label"] # 给交点打标签
[color="red"] # 给交点上色
[fillcolor="blue"] # 设置交点的填充色
shape ellipse 形状
sides 4 当shape=polygon时的边数
fillcolor lightgrey/black 填充颜色
fixedsize false 标签是否影响节点的大小

边的属性
[label="Some Label"] # 给边设置标签 (设置路径权重的时候很有用)
[color="red"] # 给交点上色 (标示路径的时候很有用)
[penwidth=2.0] # 给边适配厚度，标示路径的时候很有用。
arrowhead = normal # 箭头头部形状
arrowsize = 1.0 # 箭头大小
arrowtail = normal # 箭头尾部形状
constraint=true #是否根据边来影响节点的排序
decorate #设置之后会用一条线来连接edge和label
dir=forward # 设置方向：forward,back,both,none
headclip=true # 是否到边界为止
tailclip=true #与headclip类似

尺寸, 背景颜色
fixedsize=true;
size="1,1";
resolution=72;
bgcolor="#C6CFD532";

eg
digraph structs {
node[shape=record]
struct1 [label="<f0> left|<f1> mid\ dle|<f2> right"];
struct2 [label="{<f0> one|<f1> two\n\n\n}" shape=Mrecord];
struct3 [label="hello\nworld |{ b |{c|<here> d|e}| f}| g | h"];
struct1:f1 -> struct2:f0;
struct1:f0 -> struct3:f1;
}

dot 语言 三种对象：图表、结点、边缘
digraph 可控制的
graph 不可控制
subgraph 子图表

绘制属性：

形状：多边形和纪录
box盒子 ,
polygon多边形 ,
ellipse椭圆,
oval椭圆,
circle圆,
point ,
egg卵形,
triangle三角 ,
plaintext ,
diamond菱形 ,
trapezium梯形 ,
parallelogram平行四边形 ,
house ,
pentagon五角形 ,
hexagon六角形 ,
septagon ,
octagon八边形 ,
doublecircle ,
doubleoctagon ,
tripleoctagon

作者：神刀
链接：https://www.jianshu.com/p/acad26331d2f
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



## 链接
http://www.graphviz.org/gallery/