 

我们在编写bash脚本的时候，经常需要替换掉字符窗中特殊的字符，我们看看有几种方法可以实现。

1. 最常用的方法是使用sed命令。例如    

   ```
   [nhuang@localhost test]$ a="This is a / and you will know /" ; echo "$a" | sed "s/\//\\\\\//g"
   This is a \/ and you will know \/
   ```

   但是相当复杂，应为在替换部分要使用双反斜杠，"\\\\\/"，而不是"\\\/",为什么呢？应为在escape"\“的时候，必须要使用"\\"，而不是对待"/"的"\/"。例如

   ```
   [nhuang@localhost test]$ a="This is a / and you will know /" ; echo "$a" | sed -s "s/\//\//g"
   This is a / and you will know /
   [nhuang@localhost test]$ a="This is a / and you will know /" ; echo "$a" | sed -s "s/\//\\\\/g"
   This is a \ and you will know \
   [nhuang@localhost test]$ a="This is a / and you will know /" ; echo "$a" | sed -s "s/\//\\/g"
   sed: -e expression #1, char 8: unterminated `s' command
   ```

    

2. 那么，第二种方法是什么呢？使用${}这个符号，例如：    

   ```
   [nhuang@localhost test]$ a="This is a / and you will know /" ; echo ${a//\//\\}
   This is a \ and you will know \
   [nhuang@localhost test]$ a="This is a / and you will know /" ; echo ${a//\//\\\/}
   This is a \/ and you will know \/
   ```

   这里，我们不再需要使用"\\"了，而是正常的使用正则表达式。所以，第二种方法，最为方便。如果使用expr substr，则会把问题复杂化。dan

3. 但是，如果要同时替换多个不同位置的字符串，例如"/", "\", "?",等，要在他们前面都加上反斜杠，使用sed，用-e，或者"{;}"来操作。    

   ```
   [nhuang@localhost test]$ str="asdasdf / asdfasdf / asdf,adf" ; echo $str | sed "{s/\//\\\\\//g;s/,/:/g}" 
   asdasdf \/ asdfasdf \/ asdf:adf
   ```
