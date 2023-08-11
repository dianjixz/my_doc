选择器是什么？
html的元素索引

选择器的种类

类型、类和 ID 选择器


这个选择器组，第一个是指向了所有 HTML 元素 <h1>。

h1 {
}


它也包含了一个 class 的选择器：

.box {
}


亦或，一个 id 选择器：
#unique {
}


标签属性选择器
这组选择器根据一个元素上的某个标签的属性的存在以选择元素的不同方式：
a[title] {
}

或者根据一个有特定值的标签属性是否存在来选择：
a[href="https://example.com"]
{
}


伪类与伪元素
这组选择器包含了伪类，用来样式化一个元素的特定状态。例如:hover伪类会在鼠标指针悬浮到一个元素上的时候选择这个元素：
a:hover {
}

它还可以包含了伪元素，选择一个元素的某个部分而不是元素自己。例如，::first-line是会选择一个元素（下面的情况中是<p>）中的第一行，类似<span>包在了第一个被格式化的行外面，然后选择这个<span>。
p::first-line {
}


运算符
最后一组选择器可以将其他选择器组合起来，更复杂的选择元素。下面的示例用运算符（>）选择了<article>元素的初代子元素。
article > p {
}


类型选择器


全局选择器

全局选择器，是由一个星号（*）代指的，它选中了文档中的所有内容（或者是父元素中的所有内容，比如，它紧随在其他元素以及邻代运算符之后的时候）。下面的示例中，我们已经用全局选择器，移去了所有元素上的外边距。这就是说，和浏览器以外边距隔开标题和段的方式默认加上的样式不同的是，每个物件都紧紧地挨在一起，我们不能那么容易就看清楚不同的段。


参考节

有很多伪类和伪元素，所以有一个用于参考的列表会有用。下面是列出它们的表格，链接到了 MDN 上它们的参考页。把这作为参考，看看你能选中什么。
伪类
选择器 	描述
:active 	在用户激活（例如点击）元素的时候匹配。
:any-link 	匹配一个链接的:link和:visited状态。
:blank 	匹配空输入值的<input>元素。
:checked 	匹配处于选中状态的单选或者复选框。
:current (en-US) 	匹配正在展示的元素，或者其上级元素。
:default 	匹配一组相似的元素中默认的一个或者更多的 UI 元素。
:dir 	基于其方向性（HTMLdir属性或者 CSSdirection属性的值）匹配一个元素。
:disabled 	匹配处于关闭状态的用户界面元素
:empty 	匹配除了可能存在的空格外，没有子元素的元素。
:enabled 	匹配处于开启状态的用户界面元素。
:first 	匹配分页媒体的第一页。
:first-child 	匹配兄弟元素中的第一个元素。
:first-of-type 	匹配兄弟元素中第一个某种类型的元素。
:focus 	当一个元素有焦点的时候匹配。
:focus-visible 	当元素有焦点，且焦点对用户可见的时候匹配。
:focus-within 	匹配有焦点的元素，以及子代元素有焦点的元素。
:future (en-US) 	匹配当前元素之后的元素。
:hover 	当用户悬浮到一个元素之上的时候匹配。
:indeterminate 	匹配未定态值的 UI 元素，通常为复选框。
:in-range 	用一个区间匹配元素，当值处于区间之内时匹配。
:invalid 	匹配诸如<input>的位于不可用状态的元素。
:lang 	基于语言（HTMLlang属性的值）匹配元素。
:last-child 	匹配兄弟元素中最末的那个元素。
:last-of-type 	匹配兄弟元素中最后一个某种类型的元素。
:left 	在分页媒体中，匹配左手边的页。
:link 	匹配未曾访问的链接。
:local-link (en-US) 	匹配指向和当前文档同一网站页面的链接。
:is() 	匹配传入的选择器列表中的任何选择器。
:not 	匹配作为值传入自身的选择器未匹配的物件。
:nth-child 	匹配一列兄弟元素中的元素——兄弟元素按照an+b形式的式子进行匹配（比如 2n+1 匹配元素 1、3、5、7 等。即所有的奇数个）。
:nth-of-type 	匹配某种类型的一列兄弟元素（比如，<p>元素）——兄弟元素按照an+b形式的式子进行匹配（比如 2n+1 匹配元素 1、3、5、7 等。即所有的奇数个）。
:nth-last-child 	匹配一列兄弟元素，从后往前倒数。兄弟元素按照an+b形式的式子进行匹配（比如 2n+1 匹配按照顺序来的最后一个元素，然后往前两个，再往前两个，诸如此类。从后往前数的所有奇数个）。
:nth-last-of-type 	匹配某种类型的一列兄弟元素（比如，<p>元素），从后往前倒数。兄弟元素按照an+b形式的式子进行匹配（比如 2n+1 匹配按照顺序来的最后一个元素，然后往前两个，再往前两个，诸如此类。从后往前数的所有奇数个）。
:only-child 	匹配没有兄弟元素的元素。
:only-of-type 	匹配兄弟元素中某类型仅有的元素。
:optional 	匹配不是必填的 form 元素。
:out-of-range 	按区间匹配元素，当值不在区间内的的时候匹配。
:past (en-US) 	匹配当前元素之前的元素。
:placeholder-shown 	匹配显示占位文字的 input 元素。
:playing 	匹配代表音频、视频或者相似的能“播放”或者“暂停”的资源的，且正在“播放”的元素。
:paused 	匹配代表音频、视频或者相似的能“播放”或者“暂停”的资源的，且正在“暂停”的元素。
:read-only 	匹配用户不可更改的元素。
:read-write 	匹配用户可更改的元素。
:required 	匹配必填的 form 元素。
:right 	在分页媒体中，匹配右手边的页。
:root 	匹配文档的根元素。
:scope 	匹配任何为参考点元素的的元素。
:valid 	匹配诸如<input>元素的处于可用状态的元素。
:target 	匹配当前 URL 目标的元素（例如如果它有一个匹配当前URL 分段的元素）。
:visited 	匹配已访问链接。


伪元素
选择器 	描述
::after 	匹配出现在原有元素的实际内容之后的一个可样式化元素。
::before 	匹配出现在原有元素的实际内容之前的一个可样式化元素。
::first-letter 	匹配元素的第一个字母。
::first-line 	匹配包含此伪元素的元素的第一行。
::grammar-error 	匹配文档中包含了浏览器标记的语法错误的那部分。
::selection 	匹配文档中被选择的那部分。
::spelling-error 	匹配文档中包含了浏览器标记的拼写错误的那部分。


盒模型的各个部分

CSS 中组成一个块级盒子需要：

    Content box: 这个区域是用来显示内容，大小可以通过设置 width 和 height.
    Padding box: 包围在内容区域外部的空白区域；大小通过 padding 相关属性设置。
    Border box: 边框盒包裹内容和内边距。大小通过 border 相关属性设置。
    Margin box: 这是最外面的区域，是盒子和其他元素之间的空白区域。大小通过 margin 相关属性设置。

如下图：

.box {
  width: 350px;
  height: 150px;
  margin: 25px;
  padding: 25px;
  border: 5px solid black;
}


默认浏览器会使用标准模型。如果需要使用替代模型，您可以通过为其设置 box-sizing: border-box 来实现。这样就可以告诉浏览器使用 border-box 来定义区域，从而设定您想要的大小。
.box {
  box-sizing: border-box;
}

如果你希望所有元素都使用替代模式，而且确实很常用，设置 box-sizing 在 <html> 元素上，然后设置所有元素继承该属性，正如下面的例子。如果想要深入理解，请看 the CSS Tricks article on box-sizing。
html {
  box-sizing: border-box;
}
*,
*::before,
*::after {
  box-sizing: inherit;
}



可以使用border属性一次设置所有四个边框的宽度、颜色和样式。

分别设置每边的宽度、颜色和样式，可以使用：

    border-top
    border-right
    border-bottom
    border-left

设置所有边的颜色、样式或宽度，请使用以下属性：

    border-width
    border-style
    border-color

设置单边的颜色、样式或宽度，可以使用最细粒度的普通属性之一：

    border-top-width
    border-top-style
    border-top-color
    border-right-width
    border-right-style
    border-right-color
    border-bottom-width
    border-bottom-style
    border-bottom-color
    border-left-width
    border-left-style
    border-left-color

设置边框的颜色、样式或宽度，可以使用最细粒度的普通属性或者简写属性。在下面的示例中，我们使用了各种普通属性或者简写属性来创建边框。尝试一下不同的属性，以检查您是否理解它们是如何工作的。MDN 中的边框属性页面为您提供可用的不同边框样式的信息。

内边距

内边距位于边框和内容区域之间。与外边距不同，您不能有负数量的内边距，所以值必须是 0 或正的值。应用于元素的任何背景都将显示在内边距后面，内边距通常用于将内容推离边框。

我们可以使用padding简写属性控制元素所有边，或者每边单独使用等价的普通属性：

    padding-top
    padding-right
    padding-bottom
    padding-left

如果在下面的示例中更改类.box的内边距值，您可以看到，这将更改文本开始的位置。

您还可以更改类.container的内边距，这将在容器和方框之间留出空间。任何元素上的内边距都可以更改，并在其边界和元素内部的任何内容之间留出空间。

盒子模型和内联盒子

以上所有的方法都完全适用于块级盒子。有些属性也可以应用于内联盒子，例如由<span>元素创建的那些内联盒子。

在下面的示例中，我们在一个段落中使用了<span>，并对其应用了宽度、高度、边距、边框和内边距。可以看到，宽度和高度被忽略了。外边距、内边距和边框是生效的，但它们不会改变其他内容与内联盒子的关系，因此内边距和边框会与段落中的其他单词重叠。


使用 display: inline-block

display 有一个特殊的值，它在内联和块之间提供了一个中间状态。这对于以下情况非常有用：您不希望一个项切换到新行，但希望它可以设定宽度和高度，并避免上面看到的重叠。

一个元素使用 display: inline-block，实现我们需要的块级的部分效果：

    设置width 和height 属性会生效。
    padding, margin, 以及border 会推开其他元素。

但是，它不会跳转到新行，如果显式添加 width 和 height 属性，它只会变得比其内容更大。

在下一个示例中，我们将 display: inline-block 添加到 <span> 元素中。尝试将此更改为 display: block 或完全删除行，以查看显示模型中的差异。


CSS 的背景样式

CSS background 属性是本课程中我们将遇到的一些普通背景属性的简写表示。如果你在样式表中发现了一个复杂的背景属性，可能会觉得有点难以理解，因为可以同时传入这么多的值。


背景颜色

background-color 属性定义了 CSS 中任何元素的背景颜色。属性接受任何有效的 <color> 值。background-color 可以延伸至元素的内容和内边距盒子的下面。

在下面的例子中，我们用各种颜色值为盒子、标题和 <span> 元素添加背景色。

尝试修改为任何可用的 <color> 值。


背景图像

background-image 属性可以在一个元素的背景中显示一个图像。在下面的例子中，我们有两个盒子，其中一个盒子具有比盒子大的背景图像（balloons.jpg），另一个盒子具有较小的单个星星的图像（star.png）。

这个示例演示了关于背景图像的两种情形。默认情况下，大图不会缩小以适应盒子，因此我们只能看到它的一个小角，而小图则是平铺以填充方框。在


如果除了背景图像外，还指定了背景颜色，则图像将显示在颜色的顶部。尝试向上面的示例添加 background-color 属性，看看效果如何。
控制背景平铺行为

background-repeat 属性用于控制图像的平铺行为。可用的值是：

    no-repeat——阻止背景重复平铺。
    repeat-x——仅水平方向上重复平铺。
    repeat-y——仅垂直方向上重复平铺。
    repeat——默认值，在水平和垂直两个方向重复平铺。

在下面的示例中尝试这些值。我们已经将值设置为 no-repeat，因此你将只能看到一个星星。尝试不同的值（repeat-x 和 repeat-y），看看它们的效果如何。




调整背景图像的大小

在上面的例子中，我们有一个很大的图像（ballons.jpg），由于它比作为背景的元素大，所以最后被裁剪掉了。在这种情况下，我们可以使用 background-size 属性，它可以设置长度或百分比值，来调整图像的大小以适应背景。

你也可以使用关键字：

    cover：浏览器将使图像足够大，使它完全覆盖了盒子区域，同时仍然保持其宽高比。在这种情况下，图像的部分区域可能会跳出盒子外。
    contain：浏览器会将图像调整到适合框内的尺寸。在这种情况下，如果图像的长宽比与盒子的长宽比不同，你可能会在图像的两边或顶部和底部出现空隙。

在下面的示例中，我使用了上面示例中的 balloons.jpg 图片，并使用长度单位来调整方框内的大小。你可以看到这扭曲了图像。

试试这些：

    改变用于修改背景大小的长度单位。
    去掉长度单位，看看使用 background-size: cover 或 background-size: contain 时会发生什么。
    如果你的图像小于盒子，可以更改 background-repeat 的值来重复平铺图像。




背景图像定位

background-position 属性允许你选择背景图片出现在它所应用的盒子上的位置。这使用了一个坐标系统，其中方框的左上角是 (0,0)，方框沿水平（x）和垂直（y）轴定位。

备注： 默认的 background-position 值是 (0,0)。

最常见的 background-position 值有两个单独的值——一个水平值后面跟着一个垂直值。

你可以使用像 top 和 right 这样的关键字（在 background-image 页面上查找其他的关键字）：


我们之所以要在这里探讨书写模式和方向，是因为目前为止我们已经了解了很多与屏幕的物理显示密切相关的很多属性，而书写模式和方向在水平书写模式下会很有意义。

让我们再来看看那两个盒子——一个用horizontal-tb设定了书写模式，一个用vertical-rl设定了书写模式。我为这两个盒子分别设定了宽度（ width）。可以看到，当盒子处于纵向书写模式下时，宽度也发生了变化，从而导致文本超出了盒子的范围。

通过这一些列调整，我们想要的实际上是使宽和高随着书写模式一起变化。当处于纵向书写模式之下时，我们希望盒子可以向横向模式下一样得到拓宽。

为了更容易实现这样的转变，CSS 最近开发了一系列映射属性。这些属性用逻辑（logical）和相对变化（flow relative）代替了像宽width和高height一样的物理属性。

横向书写模式下，映射到width的属性被称作内联尺寸（inline-size）——内联维度的尺寸。而映射height的属性被称为块级尺寸（block-size），这是块级维度的尺寸。下面的例子展示了替换掉width的inline-size是如何生效的。
逻辑外边距、边框和内边距属性

我们在前面两节中学习了 CSS 的盒模型和 CSS 边框。在外边距、边框和内边距属性中，你会发现许多物理属性，例如 margin-top、 padding-left和 border-bottom。就像 width 和 height 有映射，这些属性也有相应的映射。

margin-top属性的映射是margin-block-start——总是指向块级维度开始处的边距。

padding-left属性映射到 padding-inline-start，这是应用到内联开始方向（这是该书写模式文本开始的地方）上的内边距。border-bottom属性映射到的是border-block-end，也就是块级维度结尾处的边框。

下面是物理和逻辑属性之间的对比。




overflow属性是你控制一个元素溢出的方式，它告诉浏览器你想怎样处理溢出。overflow的默认值为visible，这就是我们的内容溢出的时候，我们在默认情况下看到它们的原因。


数字、长度和百分比

你可能会发现自己在 CSS 中使用了各种数值数据类型。以下全部归类为数值：
数值类型 	描述
<integer> 	<integer> 是一个整数，比如 1024 或 -55。
<number> 	<number> 表示一个小数——它可能有小数点后面的部分，也可能没有，例如 0.255、128 或 -1.2。
<dimension> 	<dimension> 是一个 <number> 它有一个附加的单位，例如 45deg、5s 或 10px。<dimension> 是一个伞形类别，包括 <length>、<angle>、<time> 和 <resolution> 类型。
<percentage> 	<percentage> 表示一些其他值的一部分，例如 50%。百分比值总是相对于另一个量。例如，一个元素的长度相对于其父元素的长度。



长度

最常见的数字类型是 <length>，例如 10px（像素）或 30em。CSS 中有两种类型的长度——相对长度和绝对长度。重要的是要知道它们之间的区别，以便理解他们控制的元素将变得有多大。


这些值中的大多数在用于打印时比用于屏幕输出时更有用。例如，我们通常不会在屏幕上使用 cm（厘米）。惟一一个你经常使用的值，估计就是 px（像素）。



相对长度单位

相对长度单位相对于其他一些东西，比如父元素的字体大小，或者视图端口的大小。使用相对单位的好处是，经过一些仔细的规划，你可以使文本或其他元素的大小与页面上的其他内容相对应。下表列出了 web 开发中一些最有用的单位。
单位 	相对于
em 	在 font-size 中使用是相对于父元素的字体大小，在其他属性中使用是相对于自身的字体大小，如 width。
ex 	字符“x”的高度。
ch 	数字“0”的宽度。
rem 	根元素的字体大小。
lh 	元素的行高。
rlh 	根元素的行高。当用于根元素的 font-size 或 line-height 属性时，它指的是这些属性的初始值。
vw 	视口宽度的 1%。
vh 	视口高度的 1%。
vmin 	视口较小尺寸的 1%。
vmax 	视口大尺寸的 1%。
vb 	在根元素的块向上，初始包含块的尺寸的 1%。
vi 	在根元素的行向上，初始包含块的尺寸的 1%。
svw、svh 	分别为视口较小尺寸的宽度和高度的 1%。
lvw、lvh 	分别为视口大尺寸的宽度和高度的 1%。
dvw、dvh 	分别为动态视口的宽度和高度的 1%。













字体属性：(font)

大小 font-size:x-large;(特大) xx-small;(极小) 一般中文用不到，只要用数值就可以，单位：PX、PD
样式 font-style:oblique;(偏斜体) italic;(斜体) normal;(正常)
行高 line-height:normal;(正常) 单位：PX、PD、EM
粗细 font-weight:bold;(粗体) lighter;(细体) normal;(正常)
变体font-variant: small-caps;(小型大写字母) normal;(正常)
大小写text-transform: capitalize;(首字母大写) uppercase;(大写) lowercase;(小写) none;(无)
修饰text-decoration: underline;(下划线) overline;(上划线) line-through;(删除线) blink;(闪烁)
常用字体：(font-family)
"Courier New", Courier, monospace, "Times New Roman",Times, serif, Arial, Helvetica, sans-serif, Verdana

    1
    2
    3
    4
    5
    6
    7
    8
    9

背景属性： (background)

色彩background-color:#FFFFFF;
图片background-image:url();
重复background-repeat:no-repeat;
滚动background-attachment:fixed;(固定) scroll;(滚动)
位置background-position:left(水平) top(垂直);
简写方法 background:#000url(..) repeat fixed left top;

    1
    2
    3
    4
    5
    6

区块属性： (Block)

字间距letter-spacing:normal; 数值
对刘text-align:justify;(两端对齐) left;(左对齐) right;(右对齐) center;(居中)
缩进text-indent: 数值px;
垂直对齐vertical-align:baseline;(基线) sub;(下标) super;(下标) top; text-top; middle;bottom; text-bottom;
词间距word-spacing:normal; 数值
空格white-space:pre;(保留) nowrap;(不换行)
显示display:block;(块) inline;(内嵌) list-item;(列表项) run-in;(追加部分) compact;(紧凑) marker;(标记) table; inline-table;table-raw-group; table-header-group; table-footer-group; table-raw; table-column-group;table-column; table-cell; table-caption;(表格标题)

    1
    2
    3
    4
    5
    6
    7

方框属性： (Box)

width:; height:; float:; clear:both; margin:; padding:;   顺序：上右下左

    1

边框属性： (Border)

border-style: dotted;(点线) dashed;(虚线) solid; double;(双线) groove;(槽线) ridge;(脊状) inset;(凹陷) outset;
border-width:; 边框宽度
border-color:#;
简写方法border：width style color;

    1
    2
    3
    4

列表属性： (List-style)

类型list-style-type:  disc;(圆点) circle;(圆圈) square;(方块) decimal;(数字) lower-roman;(小罗码数字) upper-roman;lower-alpha; upper-alpha;
位置list-style-position:outside;(外) inside;
图像list-style-image:url(..);

    1
    2
    3

定位属性： (Position)

Position: absolute; relative; static;
visibility: inherit; visible; hidden;
overflow: visible; hidden; scroll; auto;
clip: rect(12px,auto,12px,auto) (裁切) 

    1
    2
    3
    4

css属性代码大全
一 CSS文字属性：

color : #999999; /\*文字颜色\*/
font-family : 宋体,sans-serif;/\*文字字体\*/ 
font-size : 9pt; /\*文字大小\*/ 
font-style:itelic; /\*文字斜体\*/ 
font-variant:small-caps; /\*小字体\*/ 
letter-spacing : 1pt; /\*字间距离\*/ 
line-height : 200%; /\*设置行高\*/ 
font-weight:bold; /\*文字粗体\*/ 
vertical-align:sub; /\*下标字\*/ 
vertical-align:super; /\*上标字\*/ 
text-decoration:line-through; /\*加删除线\*/ 
text-decoration: overline; /\*加顶线\*/ 
text-decoration:underline; /\*加下划线\*/ 
text-decoration:none; /\*删除链接下划线\*/ 
text-transform : capitalize; /\*首字大写\*/ 
text-transform : uppercase; /\*英文大写\*/ 
text-transform : lowercase; /\*英文小写\*/ 
text-align:right; /\*文字右对齐\*/ 
text-align:left; /\*文字左对齐\*/ 
text-align:center; /\*文字居中对齐\*/ 
text-align:justify; /\*文字分散对齐\*/ 
vertical-align属性
vertical-align:top; /\*垂直向上对齐\*/ 
vertical-align:bottom; /\*垂直向下对齐\*/ 
vertical-align:middle; /\*垂直居中对齐\*/ 
vertical-align:text-top; /\*文字垂直向上对齐\*/ 
vertical-align:text-bottom; /\*文字垂直向下对齐\*/ 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27

二、CSS边框空白

padding-top:10px; /\*上边框留空白\*/ 
padding-right:10px; /\*右边框留空白\*/ 
padding-bottom:10px; /\*下边框留空白\*/ 
padding-left:10px; /\*左边框留空白

    1
    2
    3
    4

三、CSS符号属性：

list-style-type:none; /\*不编号\*/ 
list-style-type:decimal; /\*阿拉伯数字\*/ 
list-style-type:lower-roman; /\*小写罗马数字\*/ 
list-style-type:upper-roman; /\*大写罗马数字\*/ 
list-style-type:lower-alpha; /\*小写英文字母\*/ 
list-style-type:upper-alpha; /\*大写英文字母\*/ 
list-style-type:disc; /\*实心圆形符号\*/ 
list-style-type:circle; /\*空心圆形符号\*/ 
list-style-type:square; /\*实心方形符号\*/ 
list-style-image:url(/dot.gif); /\*图片式符号\*/ 
list-style-position: outside; /\*凸排\*/ 
list-style-position:inside; /\*缩进\*/ 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12

四、CSS背景样式：

background-color:#F5E2EC; /\*背景颜色\*/ 
background:transparent; /\*透视背景\*/ 
background-image : url(/image/bg.gif); /\*背景图片\*/ 
background-attachment : fixed; /\*浮水印固定背景\*/ 
background-repeat : repeat; /\*重复排列-网页默认\*/ 
background-repeat : no-repeat; /\*不重复排列\*/ 
background-repeat : repeat-x; /\*在x轴重复排列\*/ 
background-repeat : repeat-y; /\*在y轴重复排列\*/ 
指定背景位置
background-position : 90% 90%; /\*背景图片x与y轴的位置\*/ 
background-position : top; /\*向上对齐\*/ 
background-position : buttom; /\*向下对齐\*/ 
background-position : left; /\*向左对齐\*/ 
background-position : right; /\*向右对齐\*/ 
background-position : center; /\*居中对齐\*/ 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15

五、CSS连接属性：

a /\*所有超链接\*/ 
a:link /\*超链接文字格式\*/ 
a:visited /\*浏览过的链接文字格式\*/ 
a:active /\*按下链接的格式\*/ 
a:hover /\*鼠标转到链接\*/ 
鼠标光标样式：
链接手指 CURSOR: hand 
十字体cursor:crosshair 
箭头朝下cursor:s-resize 
十字箭头 cursor:move 
箭头朝右 cursor:move 
加一问号 cursor:help 
箭头朝左cursor:w-resize 
箭头朝上cursor:n-resize 
箭头朝右上cursor:ne-resize 
箭头朝左上cursor:nw-resize 
文字I型 cursor:text 
箭头斜右下cursor:se-resize 
箭头斜左下cursor:sw-resize 
漏斗 cursor:wait 
光标图案(IE6) p {cursor:url("光标文件名.cur"),text;} 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21

六、CSS框线一览表：

border-top : 1px solid #6699cc; /\*上框线\*/ 
border-bottom : 1px solid #6699cc; /\*下框线\*/ 
border-left : 1px solid #6699cc; /\*左框线\*/ 
border-right : 1px solid #6699cc; /\*右框线\*/ 
以上是建议书写方式,但也可以使用常规的方式如下:
border-top-color : #369 /\*设置上框线top颜色\*/ 
border-top-width :1px /\*设置上框线top宽度\*/ 
border-top-style : solid/\*设置上框线top样式\*/ 
其他框线样式
solid /\*实线框\*/ 
dotted /\*虚线框\*/ 
double /\*双线框\*/ 
groove /\*立体内凸框\*/ 
ridge /\*立体浮雕框\*/ 
inset /\*凹框\*/ 
outset /\*凸框\*/ 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16

七、CSS表单运用：

文字方块 
按钮 
复选框 
选择钮 
多行文字方块 
下拉式菜单选项1选项2 

    1
    2
    3
    4
    5
    6

八、CSS边界样式：

margin-top:10px; /\*上边界\*/ 
margin-right:10px; /\*右边界值\*/ 
margin-bottom:10px; /\*下边界值\*/ 
margin-left:10px; /\*左边界值\*/ 

    1
    2
    3
    4

CSS 属性：字体样式(Font Style)

序号中文说明标记语法 
1 字体样式{font:font-style font-variant font-weight font-size font-family} 
2 字体类型{font-family:"字体1","字体2","字体3",...} 
3 字体大小 {font-size:数值|inherit| medium| large|larger| x-large| xx-large| small| smaller| x-small| xx-small} 
4 字体风格{font-style:inherit|italic|normal|oblique} 
5 字体粗细 {font-weight:100-900|bold|bolder|lighter|normal;} 
6 字体颜色 {color:数值;} 
7 阴影颜色{text-shadow:16位色值} 
8 字体行高 {line-height:数值|inherit|normal;} 
9 字间距 {letter-spacing:数值|inherit|normal} 
10 单词间距{word-spacing:数值|inherit|normal} 
11 字体变形{font-variant:inherit|normal|small-cps } 
12 英文转换{text-transform:inherit|none|capitalize|uppercase|lowercase} 
13 字体变形 {font-size-adjust:inherit|none} 
14 字体{font-stretch:condensed|expanded|extra-condensed|extra-expanded|inherit|narrower|normal|semi-condensed|semi-expanded|ultra-condensed|ultra-expanded|wider} 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15

文本样式(Text Style)

序号中文说明标记语法 
1 行间距 {line-height:数值|inherit|normal;} 
2 文本修饰{text-decoration:inherit|none|underline|overline|line-through|blink} 
3 段首空格 {text-indent:数值|inherit} 
4 水平对齐{text-align:left|right|center|justify} 
5 垂直对齐{vertical-align:inherit|top|bottom|text-top|text-bottom|baseline|middle|sub|super} 
6 书写方式{writing-mode:lr-tb|tb-rl} 

    1
    2
    3
    4
    5
    6
    7

背景样式

序号中文说明标记语法 
1 背景颜色{background-color:数值} 
2 背景图片{background-image: url(URL)|none} 
3 背景重复{background-repeat:inherit|no-repeat|repeat|repeat-x|repeat-y} 
4 背景固定{background-attachment:fixed|scroll} 
5 背景定位 {background-position:数值|top|bottom|left|right|center} 
6 背影样式 {background:背景颜色|背景图象|背景重复|背景附件|背景位置} 

    1
    2
    3
    4
    5
    6
    7

框架样式(BoxStyle)

序号中文说明标记语法 
1 边界留白{margin:margin-top margin-right margin-bottom margin-left} 
2 补　　白{padding:padding-top padding-right padding-bottom padding-left} 
3 边框宽度{border-width:border-top-width border-right-width border-bottom-widthborder-left-width}
宽度值：thin|medium|thick|数值 
4 边框颜色{border-color:数值数值数值数值}　　数值：分别代表top、right、bottom、left颜色值 
5 边框风格{border-style:none|hidden|inherit|dashed|solid|double|inset|outset|ridge|groove} 
6 边　　框{border:border-width border-style color} 
  上边框{border-top:border-top-width border-style color} 
  右边框{border-right:border-right-width border-style color} 
  下边框{border-bottom:border-bottom-width border-style color} 
  左边框{border-left:border-left-width border-style color} 
7 宽　　度 {width:长度|百分比| auto} 
8 高　　度 {height:数值|auto} 
9 漂　　浮{float:left|right|none} 
10 清　　除{clear:none|left|right|both} 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16

分类列表

序号中文说明标记语法 
1 控制显示{display:none|block|inline|list-item} 
2 控制空白{white-space:normal|pre|nowarp} 
3 符号列表{list-style-type:disc|circle|square|decimal|lower-roman|upper-roman|lower-alpha|upper-alpha|none} 
4 图形列表{list-style-image:URL} 
5 位置列表{list-style-position:inside|outside} 
6 目录列表 {list-style:目录样式类型|目录样式位置|url} 
7 鼠标形状{cursor:hand|crosshair|text|wait|move|help|e-resize|nw-resize|w-resize|s-resize|se-resize|sw-resize}

    1
    2
    3
    4
    5
    6
    7
    8

下面是我自己整理的
css属性大全

背景属性

background	简写属性，作用是将背景属性设置在一个声明中。
background-attachment	背景图像是否固定或者随着页面的其余部分滚动。
background-color	设置元素的背景颜色。
background-image	把图像设置为背景。
background-position	设置背景图像的起始位置。
background-repeat	设置背景图像是否及如何重复。
background-size 设置背景图片大小

    1
    2
    3
    4
    5
    6
    7

文本属性。

color	设置文本颜色
direction	设置文本方向。
letter-spacing	设置字符间距
line-height	设置行高
text-align	对齐元素中的文本
text-decoration	向文本添加修饰
text-indent	缩进元素中文本的首行
text-shadow	设置文本阴影
text-transform	控制元素中的字母
unicode-bidi	设置或返回文本是否被重写 
vertical-align	设置元素的垂直对齐
white-space	设置元素中空白的处理方式
word-spacing	设置字间距

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13

字体属性

font	在一个声明中设置所有的字体属性
font-family	指定文本的字体系列
font-size	指定文本的字体大小
font-style	指定文本的字体样式
font-variant	以小型大写字体或者正常字体显示文本。
font-weight	指定字体的粗细。

    1
    2
    3
    4
    5
    6

链接属性

a:link - 正常，未访问过的链接
a:visited - 用户已访问过的链接
a:hover - 当用户鼠标放在链接上时
a:active - 链接被点击的那一刻

当设置为若干链路状态的样式，也有一些顺序规则：
a:hover 必须跟在 a:link 和 a:visited后面
a:active 必须跟在 a:hover后面

    1
    2
    3
    4
    5
    6
    7
    8

列表属性

list-style	简写属性。用于把所有用于列表的属性设置于一个声明中
list-style-image	将图像设置为列表项标志。
list-style-position	设置列表中列表项标志的位置。
list-style-type	设置列表项标志的类型。

    1
    2
    3
    4

table表格

border	指定CSS表格边框
border-collapse 属性设置表格的边框是否被折叠成一个单一的边框或隔开
Width和height属性定义表格的宽度和高度。
text-align	属性设置水平对齐方式，向左，右，或中心
vertical-align	垂直对齐属性设置垂直对齐，比如顶部，底部或中间

    1
    2
    3
    4
    5

盒子模型

Margin(外边距) - 清除边框外的区域，外边距是透明的。
Border(边框) - 围绕在内边距和内容外的边框。
Padding(内边距) - 清除内容周围的区域，内边距是透明的。
Content(内容) - 盒子的内容，显示文本和图像。

最终元素的总宽度计算公式是这样的：
总元素的宽度=宽度+左填充+右填充+左边框+右边框+左边距+右边距
元素的总高度最终计算公式是这样的：
总元素的高度=高度+顶部填充+底部填充+上边框+下边框+上边距+下边距

    1
    2
    3
    4
    5
    6
    7
    8
    9

边框

border-style 值:
dotted: 定义一个点线边框
dashed: 定义一个虚线边框
solid: 定义实线边框
double: 定义两个边框。 两个边框的宽度和 border-width 的值相同
groove: 定义3D沟槽边框。效果取决于边框的颜色值
ridge: 定义3D脊边框。效果取决于边框的颜色值
inset:定义一个3D的嵌入边框。效果取决于边框的颜色值
outset: 定义一个3D突出边框。 效果取决于边框的颜色值

border-width	属性为边框指定宽度。
border-color	属性用于设置边框的颜色
注意： border-color单独使用是不起作用的，必须得先使用border-style来设置边框样式

border	简写属性，用于把针对四个边的属性设置在一个声明。
border-style	用于设置元素所有边框的样式，或者单独地为各边设置边框样式。
border-width	简写属性，用于为元素的所有边框设置宽度，或者单独地为各边边框设置宽度。
border-color	简写属性，设置元素的所有边框中可见部分的颜色，或为 4 个边分别设置颜色。
border-bottom	简写属性，用于把下边框的所有属性设置到一个声明中。
border-bottom-color	设置元素的下边框的颜色。
border-bottom-style	设置元素的下边框的样式。
border-bottom-width	设置元素的下边框的宽度。
border-left	简写属性，用于把左边框的所有属性设置到一个声明中。
border-left-color	设置元素的左边框的颜色。
border-left-style	设置元素的左边框的样式。
border-left-width	设置元素的左边框的宽度。
border-right	简写属性，用于把右边框的所有属性设置到一个声明中。
border-right-color	设置元素的右边框的颜色。
border-right-style	设置元素的右边框的样式。
border-right-width	设置元素的右边框的宽度。
border-top	简写属性，用于把上边框的所有属性设置到一个声明中。
border-top-color	设置元素的上边框的颜色。
border-top-style	设置元素的上边框的样式。
border-top-width	设置元素的上边框的宽度。
border-radius	设置圆角的边框。

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27
    28
    29
    30
    31
    32
    33
    34
    35

轮廓

outline	在一个声明中设置所有的轮廓属性
outline-color	设置轮廓的颜色
outline-style	设置轮廓的样式
outline-width	设置轮廓的宽度

    1
    2
    3
    4

margin外边距

margin	简写属性。在一个声明中设置所有外边距属性。
margin-bottom	设置元素的下外边距。
margin-left	设置元素的左外边距。
margin-right	设置元素的右外边距。
margin-top	设置元素的上外边距。

    1
    2
    3
    4
    5

padding内边距

padding	使用简写属性设置在一个声明中的所有填充属性
padding-bottom	设置元素的底部填充
padding-left	设置元素的左部填充
padding-right	设置元素的右部填充
padding-top	设置元素的顶部填充

    1
    2
    3
    4
    5

尺寸属性

height	设置元素的高度。
line-height	设置行高。
max-height	设置元素的最大高度。
max-width	设置元素的最大宽度。
min-height	设置元素的最小高度。
min-width	设置元素的最小宽度。
width	设置元素的宽度。

    1
    2
    3
    4
    5
    6
    7

Display(显示) 与 Visibility（可见性）

display:none;
visibility:hidden;

    1
    2

Position：(定位)

static	HTML 元素的默认值，即没有定位，遵循正常的文档流对象。
relative	相对定位元素的定位是相对其正常位置。
fixed	元素的位置相对于浏览器窗口是固定位置。即使窗口是滚动的它也不会移动
absolute	绝对定位的元素的位置相对于最近的已定位父元素，如果元素没有已定位的父元素，那么它的位置相对于<html>
sticky	基于用户的滚动位置来定位。粘性定位的元素是依赖于用户的滚动，在 position:relative 与 position:fixed 定位之间切换。
它的行为就像 position:relative; 而当页面滚动超出目标区域时，它的表现就像 position:fixed;，它会固定在目标位置。
元素定位表现为在跨越特定阈值前为相对定位，之后为固定定位。这个特定阈值指的是 top, right, bottom 或 left 之一，换言之，指定 top, right, bottom 或 left 四个阈值其中之一，才可使粘性定位生效。否则其行为与相对定位相同。

bottom	定义了定位元素下外边距边界与其包含块下边界之间的偏移。
clip	剪辑一个绝对定位的元素
cursor	显示光标移动到指定的类型
left	定义了定位元素左外边距边界与其包含块左边界之间的偏移。
overflow	设置当元素的内容溢出其区域时发生的事情。
overflow-y	指定如何处理顶部/底部边缘的内容溢出元素的内容区域
overflow-x	指定如何处理右边/左边边缘的内容溢出元素的内容区域
position	指定元素的定位类型
right	定义了定位元素右外边距边界与其包含块右边界之间的偏移。
top		定义了一个定位元素的上外边距边界与其包含块上边界之间的偏移。
z-index	设置元素的堆叠顺序

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19

Overflow： 属性可以控制内容溢出元素框时在对应的元素区间内添加滚动条

visible	默认值。内容不会被修剪，会呈现在元素框之外。
hidden	内容会被修剪，并且其余内容是不可见的。
scroll	内容会被修剪，但是浏览器会显示滚动条以便查看其余的内容。
auto	如果内容被修剪，则浏览器会显示滚动条以便查看其余的内容。
inherit	规定应该从父元素继承 overflow 属性的值。

    1
    2
    3
    4
    5

浮动

clear	指定不允许元素周围有浮动元素。
float	指定一个盒子（元素）是否可以浮动。







https://blog.csdn.net/qq_45677671/article/details/111319697






























































