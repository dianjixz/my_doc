 

​		 [     tr 命令 （字符替换，大小写替换） linux命令五分钟系列之二十五         ](https://www.cnblogs.com/leaven/archive/2011/01/05/1926194.html) 	

http://www.linuxidc.com/Linux/2010-12/30564.htm  

写Bash Shell脚本，大小写转换通常这样做：  

str="This is a Bash Shell script."  

newstr=`tr '[A-Z]' '[a-z]' <<<"$str"`  

今天看bash的man page，发现有更简单的方法  

转小写，只需要将变量名字declare -l 后，再给变量赋值，变量的内容即为小写  

转大写，只需要将变量名字declare -u后，再给变量赋值，变量的内容即为大写  

例如:
m="abc"
echo $m # 输出为abc
declare -u m
echo $m # 输出为abc,
m="cde"
echo $m # 输出为CDE
declare -l m="HELL"
echo $m # 输出为hell  

注意： 
1，declare 不影响变量当前值，而是影响declare之后的赋值  

2，通过declare -l/-u进行设置过的变量，可以通过declare +l/+u来取消设置。  

3，Bash版本比较低的不行……  

 

http://roclinux.cn/?p=1347  

tr是translate的简写，亦即翻译，但是遗憾的是，它不能翻译句子，只能翻译单个字符。  

**1 tr的工作原理是什么?**  

先记住一点，tr命令不接受指定的文件参数，而只是对标准输入进行翻译。好了，记住这点后，我们继续。  

tr的命令格式是tr SET1 SET2，凡是在SET1中的字符，都会被替换为SET2中相应位置上的字符，简单吧!  

**2 举个例子吧!**  

tr的例子其实都大同小异，看一个最简单的例子：  

我们有一个包含了四个人身高的数据文件，名字叫height.txt，它的内容是这样的：  

[rocrocket@rocrocket programming]$ cat height.txt
1.79 1.83
1.65 1.59  

我们想搞一个恶作剧，将所有人的身高从1米级别都提高到2米级别，呵呵，一个tr就可以搞定。
[rocrocket@rocrocket programming]$ tr 1 2 < height.txt
2.79 2.83
2.65 2.59  

希望你没有忘记，tr只处理标准输入，所以我们需要将height.txt通过重定向指到tr的标准输入才可以。  

**3 我们可以用tr来修改文件中的间隔符么？**  

Good idea!  

当使用cut的时候，通常会被间隔符问题所困扰，我们这个时候可以考虑到tr这个命令。  

还是以height.txt文件为例，其中的间隔符是空格，我们把它修改为制表符吧！  

[rocrocket@rocrocket programming]$ tr ‘ ‘ ‘\11′ < height.txt
1.79  1.83
1.65  1.59  

这里有一个小知识点，像制表符、换行符这些字符不好表示，我们可以考虑使用ASCII的八进制形式来表示，制表符的八进制形式是11，回车是15，换行是12。而在tr命令中，可以使用\nnn形式表示八进制形式的字符。如果你实在记不住这些编号，那么用\t表示制表符你总该可以记住吧！（\n代表新行，\r代表回车）  

这下，你应该可以理解上面那条命令的作用了吧。如果你仍然怀疑，那么，看看下面这条命令，你就该心服口服了：  

[rocrocket@rocrocket programming]$ tr ‘ ‘ ‘\11′ < height.txt |sed -n l
1.79\t1.83$
1.65\t1.59$  

看！空格的的确确被替换成了制表符喽！  

**4 使用tr能把文章中的小写都转换成大写么？**  

这个正是tr所擅长的地方。  

加入我们拥有一个文件叫做word.txt，里面包含的内容是AbcdE。  

那么最简单的替换大小写的方法是：  

[rocrocket@rocrocket programming]$ cat word.txt
AbcdE
[rocrocket@rocrocket programming]$ tr ‘a-z’ ‘A-Z’ < word.txt
ABCDE  

**5 [CHAR\*]怎么用？**  

这是 SET2 专用的设定，功能是重复指定的字符到与 SET1 相同长度为止  

例子：  

[rocrocket@rocrocket programming]$ cat number.txt
1234567890
[rocrocket@rocrocket programming]$ tr ‘1-5′ ‘[A*]‘ < number.txt
AAAAA67890  

**6 [CHAR\*REPEAT]怎么用？**  

这也是SET2专用的设定，功能是将CHAR重复REPEAT次数。其中REPEAT次数可以用八进制数表示，但记得要以0开头表示八进制数。  

例子：  

[rocrocket@rocrocket programming]$ tr ‘1-9′ ‘[A*5]BCDE’ < number.txt
AAAAABCDE0  

**7 在tr中还有哪些表示集合的符号呢?**  

[:alnum:] ：所有字母字符与数字
[:alpha:] ：所有字母字符
[:blank:] ：所有水平空格
[:cntrl:] ：所有控制字符
[:digit:] ：所有数字
[:graph:] ：所有可打印的字符(不包含空格符)
[:lower:] ：所有小写字母
[:print:] ：所有可打印的字符(包含空格符)
[:punct:] ：所有标点字符
[:space:] ：所有水平与垂直空格符
[:upper:] ：所有大写字母
[:xdigit:] ：所有 16 进位制的数字  

例子：  

将所有的数字都转换为字符x。  

[rocrocket@rocrocket programming]$ tr [:alnum:] ‘[x*]‘ < number.txt
xxxxxxxxxx  

**8 tr里面包含SET1和SET2，那如果出现两个集合的大小不同的情况，tr如何处理呢？**  

这个问题，最好的解决办法就是做实验啊。  

第一种情况是SET1>SET2：  

[rocrocket@rocrocket programming]$ tr 1-5 AB < number.txt
ABBBB67890  

结论一下子就出来了，SET1中多出来的字符都会和SET2中最后一个字符相对应。  

第二种情况SET1<SET2：  

[rocrocket@rocrocket programming]$ tr 1-3 ABCDE < number.txt
ABC4567890  

很明了，SET2中多余的部分将被抛弃。  

**9 讲一讲tr命令的-s选项吧！**  

这个-s选项，是专门针对SET1起作用的，意思是如果发现有连续的SET1里的字符，就把它们缩减为1个。  

一个很经典的应用就是把不规律的空格缩减为一个空格：  

[rocrocket@rocrocket programming]$ cat spaces.txt
How  are        you?
Fine! Thank  you!
[rocrocket@rocrocket programming]$ tr -s ‘ ‘ ‘ ‘ < spaces.txt
How are you?
Fine! Thank you!  

效果很明显，用户很满意。恩！  

**10 -d选项咋用？**  

-d选项是用来删除字符用的。格式是这样的：tr -d charset  

[rocrocket@rocrocket programming]$ tr -d ‘ ‘ < spaces.txt
Howareyou?
Fine!Thankyou!  

看，该有的空格都没了…这就是-d的作用，把空格都删除了!  

如果你想把文章中的数字都删除，就tr -d [0-9] < filename就可以了。  

*ps: 看似tr只有两个字母，不过这个命令的功能却不少，可不能小觑，不过，读过本文，搞定tr也就不在话下了:D*  

over~  

https://www.cnblogs.com/leaven/archive/2011/01/05/1926194.html
