js框架jQuery
什么是 AJAX？

AJAX = 异步 JavaScript 和 XML（Asynchronous JavaScript and XML）。

简短地说，在不重载整个网页的情况下，AJAX 通过后台加载数据，并在网页上进行显示。

使用 AJAX 的应用程序案例：谷歌地图、腾讯微博、优酷视频、人人网等等。

jQuery 库是一个 JavaScript 文件，您可以使用 HTML 的 <script> 标签引用它：


<head>
<script src="jquery-1.10.2.min.js"></script>
</head>


<head>
<script src="https://apps.bdimg.com/libs/jquery/2.1.4/jquery.min.js">
</script>
</head>


jQuery 语法

jQuery 语法是通过选取 HTML 元素，并对选取的元素执行某些操作。

基础语法： $(selector).action()

    美元符号定义 jQuery
    选择符（selector）"查询"和"查找" HTML 元素
    jQuery 的 action() 执行对元素的操作

实例:

    $(this).hide() - 隐藏当前元素

    $("p").hide() - 隐藏所有 <p> 元素

    $("p.test").hide() - 隐藏所有 class="test" 的 <p> 元素

    $("#test").hide() - 隐藏 id="test" 的元素


文档就绪事件

$(document).ready(function(){
 
   // 开始写 jQuery 代码...
 
});

$(document).ready(function(){
    // 执行代码
});
或者
$(function(){
    // 执行代码
});

window.onload = function () {
    // 执行代码
}

元素选择器
在页面中选取所有 <p> 元素:
$("p") 

#id 选择器
通过 id 选取元素语法如下：
$("#test") 

.class 选择器
$(".test") 


语法 	描述 	实例
$("*") 	选取所有元素 	在线实例
$(this) 	选取当前 HTML 元素 	在线实例
$("p.intro") 	选取 class 为 intro 的 <p> 元素 	在线实例
$("p:first") 	选取第一个 <p> 元素 	在线实例
$("ul li:first") 	选取第一个 <ul> 元素的第一个 <li> 元素 	在线实例
$("ul li:first-child") 	选取每个 <ul> 元素的第一个 <li> 元素 	在线实例
$("[href]") 	选取带有 href 属性的元素 	在线实例
$("a[target='_blank']") 	选取所有 target 属性值等于 "_blank" 的 <a> 元素 	在线实例
$("a[target!='_blank']") 	选取所有 target 属性值不等于 "_blank" 的 <a> 元素 	在线实例
$(":button") 	选取所有 type="button" 的 <input> 元素 和 <button> 元素 	在线实例
$("tr:even") 	选取偶数位置的 <tr> 元素 	在线实例
$("tr:odd") 	选取奇数位置的 <tr> 元素 	在线实例


jQuery 事件


鼠标事件 	键盘事件 	表单事件 	文档/窗口事件
click 	keypress 	submit 	load
dblclick 	keydown 	change 	resize
mouseenter 	keyup 	focus 	scroll
mouseleave 	  	blur 	unload
hover 	  	  	 


jQuery 效果- 隐藏和显示

隐藏、显示、切换，滑动，淡入淡出，以及动画，哇哦！


获得内容 - text()、html() 以及 val()

三个简单实用的用于 DOM 操作的 jQuery 方法：

    text() - 设置或返回所选元素的文本内容
    html() - 设置或返回所选元素的内容（包括 HTML 标签）
    val() - 设置或返回表单字段的值


获取属性 - attr()

jQuery attr() 方法用于获取属性值。

下面的例子演示如何获得链接中 href 属性的值：


设置内容 - text()、html() 以及 val()

我们将使用前一章中的三个相同的方法来设置内容：

    text() - 设置或返回所选元素的文本内容
    html() - 设置或返回所选元素的内容（包括 HTML 标记）
    val() - 设置或返回表单字段的值


设置属性 - attr()

jQuery attr() 方法也用于设置/改变属性值。

下面的例子演示如何改变（设置）链接中 href 属性的值：


添加新的 HTML 内容

我们将学习用于添加新内容的四个 jQuery 方法：

    append() - 在被选元素的结尾插入内容
    prepend() - 在被选元素的开头插入内容
    after() - 在被选元素之后插入内容
    before() - 在被选元素之前插入内容


删除元素/内容

如需删除元素和内容，一般可使用以下两个 jQuery 方法：

    remove() - 删除被选元素（及其子元素）
    empty() - 从被选元素中删除子元素


jQuery 操作 CSS

jQuery 拥有若干进行 CSS 操作的方法。我们将学习下面这些：

    addClass() - 向被选元素添加一个或多个类
    removeClass() - 从被选元素删除一个或多个类
    toggleClass() - 对被选元素进行添加/删除类的切换操作
    css() - 设置或返回样式属性

jQuery 尺寸方法

jQuery 提供多个处理尺寸的重要方法：

    width()
    height()
    innerWidth()
    innerHeight()
    outerWidth()
    outerHeight()

!(https://www.runoob.com/images/img_jquerydim.gif)[https://www.runoob.com/images/img_jquerydim.gif]


Query load() 方法

jQuery load() 方法是简单但强大的 AJAX 方法。

load() 方法从服务器加载数据，并把返回的数据放入被选元素中。


jQuery get() 和 post() 方法用于通过 HTTP GET 或 POST 请求从服务器请求数据。
HTTP 请求：GET vs POST

两种在客户端和服务器端进行请求-响应的常用方法是：GET 和 POST。

    GET - 从指定的资源请求数据
    POST - 向指定的资源提交要处理的数据

GET 基本上用于从服务器获得（取回）数据。注释：GET 方法可能返回缓存数据。

POST 也可用于从服务器获取数据。不过，POST 方法不会缓存数据，并且常用于连同请求一起发送数据。

如需学习更多有关 GET 和 POST 以及两方法差异的知识，请阅读我们的 HTTP 方法 - GET 对比 POST。



$("button").click(function(){
  $.get("demo_test.php",function(data,status){
    alert("数据: " + data + "\n状态: " + status);
  });
});


$("button").click(function(){
    $.post("/try/ajax/demo_test_post.php",
    {
        name:"菜鸟教程",
        url:"http://www.runoob.com"
    },
    function(data,status){
        alert("数据: \n" + data + "\n状态: " + status);
    });
});




























