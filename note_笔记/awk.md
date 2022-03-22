 

# Linux之awk详解

[![img](https://cdn2.jianshu.io/assets/default_avatar/13-394c31a9cb492fcb39c27422ca7d2815.jpg)](https://www.jianshu.com/u/242f108d3236)

[魏镇坪](https://www.jianshu.com/u/242f108d3236)关注

0.8982016.03.07 14:24:15字数 3,558阅读 33,254

'#Gun awk的相关用法

#### awk的工作原理

一次读取一行文本，按输入分隔符进行切片，切成多个组成部分，将每片直接保存在内建的变量中，$1,$2,$3....，引用指定的变量，可以显示指定断，或者多个断。如果需要显示全部的，需要使用$0来引用。可以对单个片断进行判断，也可以对所有断进行循环判断。其默认分隔符为空格

#### awk的基本用法格式

- ###### awk [options] 'program' FILE......

  - 语句之间用分号分隔

- ###### [options]

  - `-F` : 指明输入时用到的字段分隔符

  - ```
    -v var=VALUE
    ```

     

    : 自定义变量

    - 在awk中变量的引用不需要加`$`，而是直接引用

#### awk用法的简要介绍

- ###### 第一种模式

  - awk [options] 'scripts' file1,file2.....

    在这种模式中，scripts主要是命令的堆砌，对输入的文本行进行处理，通过命令print,printf或是输出重定向的方式显示出来，这里经常用到的知识点是：awk的内置变量，以及命令print和printf的使用

- ###### 第二种模式

  - awk [options] 'PATTERN{action}' file,file2.....

    在这种模式中，最重要的是5种模式和5种action的使用，以及awk的数组的使用和内置函数

### 第一种模式

###### 1、print

- 1、各项目之间使用逗号隔开，而输出时则以空白字符分隔
- 2、输出的Item可以为字符串或数值，当前记录的字段（如$1）、变量或awk的表达式，数值会先转换为字符串，而后再输出
- 3、print命令后面的Item可以省略，此时其功能相当于print $0,因此，如果想输出空白行，则需要使用print""
- 4、如果引用变量$1或其他的，是不能使用引号引起来

###### 2、内置变量

- `FS` : input field seperator,输入的分隔符，默认为空白字符
- `OFS`: output field seperator,输出的分隔符，默认为空白字符
- `RS` : input record seperator,输入的换行符
- `ORS`: output record seperator,输出时的换行符
- `NF` : number of field ,字段个数
  - awk '{print NF}' /etc/fstab :打印每行的最后一个字段为第几个字段，这里是数量引用，不是对应的值引用
  - awk '{print $NF}' /etc/fstab : 打印每行中的最后一个字段
- `NR` : number of record,文件中的行数
  - awk '{print NR}' /etc/fstab: 打印行号，其会个行号都显示
  - awk 'END{print NR}' /etc/fstab: 显示文本的总行数，其只是在文本处理完成后，只显示一次行号
  - awk '{print NR}' file1 file2 : 会每把所有文档进行总的编号，而不是单独对文件进行编号
- `FNR` : 对每个文件进行行数单独编号
  - awk '{print FNR}' file1 file2 : 会对每个文件的行数进行单独的编号显示
- `FILENAME` : awk命令所处理的文件的名称
  - awk '{print FILENAME}' file1 : 显示当前文件名，但会每行显示一次
  - awk 'END{print FILENAME}' file1 : 显示当前文件名，但只会显示一次
- `ARGC` : 命令行中参数的个数，其awk命令也算一个参数
  - awk 'END{print ARGC}' /etc/fstab : 显示共有几个参数
- `ARGV` : 其是一个数组，保存的是命令行所给定的各参数
  - awk 'END{print ARGV[0]}' /etc/fstab : 显示第一个参数，默认第一个参数个awk命令本身

###### 3、自定义变量

- `-v var=VALUE` : 在选项位置定义

- `awk 'BEGIN{test="hello";print test}'` : 在program中定义

  

  ```undefined
    变量在program中定义时，需要使用引号引起来
  ```

###### 4、printf命令

- 其格式化输出：printf FORMAT,item1,item2....

###### 要点：

- 1、其与print命令最大不同是，printf需要指定format
- 2、printf后面的字串定义内容需要使用双引号引起来
- 3、字串定义后的内容需要使用","分隔，后面直接跟Item1,item2....
- 4、format用于指定后面的每个item的输出格式
- 5、printf语句不会自动打印换行符，\n

###### 格式符

- `%c`: 显示字符的ASCII码
- `%d`,`%i` : 显示十进制整数
- `%e`,`%E`: 科学计数法数值显示
- `%f` : 显示为浮点数
- `%g`,`%G`: 以科学数法或浮点形式显示数值
- `%s`: 显示字符串
- `%u`: 无符号整数
- `%%`: 显示%号自身，相当于转义

###### 修饰符

- `N` : 显示宽度
- `-` : 左对齐（默认为右对齐）
- `+` : 显示数值符号

- 示例：
  - awk -F: '{printf "%s\n",$1}' /etc/fstab
  - awk -F: '{printf "username: %s,UID:%d\n",$1,$3}' /etc/passwd
  - awk -F: '{printf "username: %-20s shell: %s\n",$1,$NF}' /etc/passwd

###### 输出重定向

- print items > "output-file"

- print items >> "output-file"

- print items | command

  

  ```cpp
    特殊文件描述符：
    /dev/stdin :标准输入
    /dev/stdout:标准输出
    /dev/stderr:错误输出
    /dev/fd/N : 某特定文件描述符，如/dev/stdin就相当于/dev/fd/0
  ```

  - 示例
  - awk -F: '{printf "%-15s %i\n",$1,$3 > "/dev/stderr"}' /etc/passwd

### 第二种模式

- awk [option] 'PATTERN{action}' file1,file2....

##### PATTERN的使用

- `REGEXP`：正则表达式，格式为`/regular expression/`,仅处理能够被此处模式匹配到的行

  

  ```dart
    awk '/^UUID/{print $1}' /etc/fstab
    awk '!/^UUID/{print $1}' /etc/fstab
  ```

- `relational expression`:表达式，其值非0或为非空字符时满足条件,用运算符`~`(匹配)和`!~`（不匹配）

  

  ```ruby
    $1 ~ /foo/ 或者 $1 == "magedu"
  ```

- `Ranges` : 指定匹配范围，格式为`/pat1/,/pat2/`

  

  ```bash
    awk -F: '{NR>=2&&<=10){print $1}' /etc/passwd
    awk -F: /^root/,/^myuser/{print $1}' /etc/passwd
    注意：不支持直接给出数字的格式
  ```

- `BEGIN/END模式` : 特殊模式，仅在awk命令执行前运行一次或结束前运行一次

  

  ```swift
    awk -F: 'BEGIN{print "Username    ID    Shell"}{printf "%-10s%-10s%-20s\n",$1,$3,$7}' /etc/passwd ：先打印一个表头
    awk -F: 'BEGIN{print "username   ID     Shell"}{printf "%-10s%-10s%-20s\n",$1,$3,$7}END{print "end of report."} /etc/passwd ：打印一个表尾
  ```

- Empty(空模式)：匹配任意输入行

  

  ```ruby
    /正则表达式/：使用通配符的扩展集。
    关系表达式：可以用下面运算符表中的关系运算符进行操作，可以是字符串或数字的比较，如$2>$1选择第二个字段比第一个字段长的行。
    模式匹配表达式：
    模式，模式：指定一个行的范围。该语法不能包括BEGIN和END模式。
    BEGIN：让用户指定在第一条输入记录被处理之前所发生的动作，通常可在这里设置全局变量。
    END：让用户在最后一条输入记录被读取之后发生的动作。
  ```

##### 常见的Action

- 1）Expressions
- 1. Control statements :if while等
- 1. Compound statements:组合语句
- 1. Input statements
- 1. Output statements

##### 控制语句

###### 1、if-else

- 语法：if (condition){then-body} else{[else-body]}
- 示例：
  - awk -F: '{if($3>=1000)print $1,$3}' /etc/passwd
  - awk -F: '{if($3>=1000){printf "Common user: %s\n",$1} else {printf "root or sysuser: %s\n",$1}}' /etc/passwd
  - awk -F: '{if($NF=="/bin/bash")print $1}' /etc/passwd
  - awk -F: '{if(NF>5) print $0}' /etc/fstab
  - df -h | awk -F[%] '/^/dev/{print $1}' | awk {if($NF>=20) print $1}'
  - awk -F: '{if($1=="root") print $1,"Admin";else print $1, "Common User"}' /etc/passwd
  - awk -F: '{if($1=="root") printf "%-15s: %s\n",$1,"Admin";else printf "%-15s: %s\n",$1, "Common user"}' /etc/passwd
  - awk -F: -v sum=0 '{if($3>=500) sum++}END{print sum}' /etc/passwd : 统计用户ID大于500的有多少行
  - awk -F: -v OFS="\t" '{if($3<=999)printf "Sys user:\t%-15s ID is :%d\n", $1,$3;else{printf "Common user:\t%-15s ID is :%d\n",$1,$3}}' /etc/passwd :可以使用\t制表符控制 输出格式

###### 2、while：用于循环字段的

- 语法：while (condition){statement1;statment2;....}
- 示例：
  - awk '/^[[:space:]]*linux16/{print}' /boot/grub2/grub.cfg
  - awk '/^[[:space:]]*linux16/{i=1;while(i<=NF){print $i,length($i);i++}}' /etc/grub2.cfg :对每个字段进行字符个数统计
  - awk '/^[[:space:]]]*linux16/{i=1;while(i<=NF){if(length($i)<=7)print $i,length($i);i++}}' /etc/grub2.cfg
  - awk -F: '{i=1;while(i<=3){print $i;i++}}' /etc/passwd:打印用户名、密码占位符、ID
  - awk -F: '{i=1;while(i<=NF){if(length($i)>=4){print $i};i++}}' /etc/passwd : 字段大小于等于4的都显示

###### 3、do-while

- 语法：do {statement1,statement2,....} while (dondition)
- 示例：
  - awk -F: '{i=1;do{print $i;i++}while(i<=3)}' /etc/passwd :打印用户名、密码占位符、UID

###### 4、for

- 语法：for(variable assignment;condition;iteration process){ statement1,statement2,...}
- 示例：
  - awk '/^[[:space:]]*linux16/{for(i=1;i<=NF;i++) {print $i,length($i)}}' /etc/grub2.cfg
  - awk -F: '{for(i=1;i<=3;i++)print $i}' /etc/passwd
  - awk -F: '{for(i=1;i<=NF;i++) { if (length($i)>=4) {print $i}}}' /etc/passwd

###### 5、for循环还可以用来遍历数组元素

- 语法：for (i in array) {statement1,statement2,....}
- 示例
  - awk -F: '$NF!~/^$/{BASH[$NF]++}END{for(A in BASH){printf "%15s:%i\n",A,BASH[A]}}' /etc/passwd
  - awk ‘{for(i=1;i<=NF;i++){count[$i]++}}END{for(i in count) {print i,count[i]}}’ /etc/fstab
  - awk '/^UUID/{filesystem[$3]++}END{for (i in filesystem) {print i,filesystem[i]}}' /etc/fstab ：统计/etc/fstab中各文件系统的次数
  - netstat -tan | awk '/^tcp>/{state[$NF]++}END{for(i in state) {print i,state[i]}}' ： 统计各连接状态的次数
  - awk '{ip[$1]++} END {for (i in ip) {print i,ip[i]}}' /var/log/httpd/access_log : 统计访问日志中各IP的访问次数

###### 6、case

- 语法：switch (expression) { case VALUE or /REGEXP/: statement1, statement2,... default: statement1, ...}

###### 7、break和continue

- break [n]
- continue : 进入下一个字段

###### 8、next

- 功能：提前结束本行文本的处理，并接着处理下一行
- 示例：
  - awk -F: '{if($3%2==0) next;print $1,$3}' /etc/passwd
  - awk -F: ‘{if ($3%2!=0) next;print $1,$3}’ /etc/passwd

##### awk的操作符

- ###### 1、算术操作符

  - `-x` : 负值
  - `+x` : 转换为数值
  - `x^y` :
  - `x**y` : 次方
  - `x*y` : 乘法
  - `x/y` : 除法
  - `x+y` :
  - `x-y` :
  - `x%y` :

- ###### 2、字符串操作符

  - 只有一个，而且不用写出来，用于实现字符串连接

- ###### 3、赋值操作符

  - `=`
  - `+=`
  - `-=`
  - `*=`
  - `/=`
  - `%=`
  - `^=`
  - `**=`
  - `++`
  - `--`

  需要注意的是，如果某模式为`=`号，此时使用/=/可能会有语法错误，就以/[=]/替代

- ###### 4、布尔值

  awk中，任何非0值或非空字符串都为真，反之为假

- ###### 5、比较操作符

  - `x<y`
  - `x<=y`
  - `x>y`
  - `x>=y`
  - `x==y`
  - `x!=y`
  - `x~y`
  - `x!~y`

- ###### 6、逻辑操作符

  - `&&`
  - `||`
  - `!`

- ###### 7、函数调用

  - `func_name(argu1,argu2,....)

- ###### 8、条件表达式

  - selector?if-true-expression:if-false-expression
    - awk -F: ‘{$3>=1000?usertype=“Common user”:usertype=“Sysadmin or sysUser”;printf “%15s:%-s\n",$1,usertype}’ /etc/passwd

##### awk的数组

###### 1、定义数组的注意事项

- 1、可使用任意字符串，字符串要使用双引号

- 2、如果某数组元素事先不存在，在引用时awk会自动创建此元素，并将其初始化为空串

- 3、要遍历数组中的每个元素，要使用for循环

  - for(var in array){statement1,.....}

    

    ```csharp
      注意：var用于引用数组时，是引用的下标，而不是元素值
    ```

###### 定义数组的格式

- awk 'BEGIN{weekdays["mon"]="Monday";weekdays["tue"]="Tuesday";print week["mon"]}'

###### 示例：

- 统计netstat -tan中各状态的次数
  - netstat -tan | awk '/^tcp>/{state[$NF]++}END{for(i in state) {print i,state[i]}}’
    - 每出现一被/^tcp/模式匹配到的行，数组S[$NF]就加1，NF为当前匹配到的行的最后一个字段，此处用其值做为数组S的元素索引
- 统计www服务的访问日志中IP数量
  - awk ‘{ip[$1]++} END {for (i in ip) {print i,ip[i]}}’ /var/log/httpd/access_log
- 统计/etc/fstab文件中每个文件系统类型出现的次数
  - awk '/^UUID/{filesystem[$3]++}END{for (i in filesystem) {print i,filesystem[i]}}' /etc/fstab
- 统计指定文件中单词的出现次数
  - awk ‘{for(i=1;i<=NF;i++){count[$i]++}}END{for(i in count) {print i,count[i]}}’ /etc/fstab
- 统计出/etc/passwd文件中shell的种类和个数
  - awk -F: '{shell[$NF]++}END{for(A in shell){print A,shell[A]}}' /etc/passwd



```bash
    重点解析一下这条命令：
      首先，shell[$NF]++这个语句。其中的$NF所代表的是一个字符串，即shell的类型。也就是说在这个数组中的元素名称是shell的名称。而后面的++是对这个数组进行赋值。因为在/etc/passwd 这个文件中的shell类型只有两种，即/bin/bash和/sbin/nologin。也就是说这个数组就只有两个元素，并且它的值是不断被更新的。其次，语句for(A in shell)是设定了A是数组shell中的坐标变量，即是A是元素的名称，shell[A]是数组的值。
```

###### 删除数组变量

- delete array[index]

##### awk的内置函数

- split(string, array [, fieldsep [, seps ] ])

  - 功能：将string表示的字符串以fieldsep为分隔符进行分隔，并将分隔后的结果保存至array为名的数组中；数组下标为从0开始的序列；

  - 示例：

    

    ```swift
      netstat -tn | awk '/^tcp\>/{split($5,ip,":");count[ip[1]]++}END{for(i in count)print i,count[i]}'
      netstat -ant | awk '/:80\>/{split($5,clients,":");IP[clients[1]]++}END{for(i in IP){print IP[i],i}}' | sort -rn | head -50
    ```

- length([string])

  - 功能：返回string字串中字符的个数

- substr(string, start [, length])

  - 功能：取string字符串中的子串，从start开始，取length个；start从1开始计数

- system(command)

  - 功能：执行系统command并将结果返回至awk命令

- systime()

  - 功能：取系统当前时间

- tolower(s)

  - 功能：将s中的所有字母转为小写

- toupper(s)

  - 功能：将s中的所有字母转为大写

###### 番外篇！！！！



```ruby
    Linux Web服务器网站故障分析常用的命令

    系统连接状态篇：
    1.查看TCP连接状态
    netstat -nat |awk ‘{print $6}’|sort|uniq -c|sort -rn

    netstat -n | awk ‘/^tcp/ {++S[$NF]};END {for(a in S) print a, S[a]}’ 或
    netstat -n | awk ‘/^tcp/ {++state[$NF]}; END {for(key in state) print key,"\t",state[key]}’
    netstat -n | awk ‘/^tcp/ {++arr[$NF]};END {for(k in arr) print k,"t",arr[k]}’

    netstat -n |awk ‘/^tcp/ {print $NF}’|sort|uniq -c|sort -rn

    netstat -ant | awk ‘{print $NF}’ | grep -v ‘[a-z]‘ | sort | uniq -c

     

    2.查找请求数请20个IP（常用于查找攻来源）：

    netstat -anlp|grep 80|grep tcp|awk ‘{print $5}’|awk -F: ‘{print $1}’|sort|uniq -c|sort -nr|head -n20

    netstat -ant |awk ‘/:80/{split($5,ip,":");++A[ip[1]]}END{for(i in A) print A[i],i}’ |sort -rn|head -n20

    3.用tcpdump嗅探80端口的访问看看谁最高

    tcpdump -i eth0 -tnn dst port 80 -c 1000 | awk -F"." ‘{print $1"."$2"."$3"."$4}’ | sort | uniq -c | sort -nr |head -20

    4.查找较多time_wait连接

    netstat -n|grep TIME_WAIT|awk ‘{print $5}’|sort|uniq -c|sort -rn|head -n20

    5.找查较多的SYN连接

    netstat -an | grep SYN | awk ‘{print $5}’ | awk -F: ‘{print $1}’ | sort | uniq -c | sort -nr | more

    6.根据端口列进程

    netstat -ntlp | grep 80 | awk ‘{print $7}’ | cut -d/ -f1
    netstat -tnlp | awk '/22/{split($NF,port,"/");d[port[1]]++}END{for(i in d)print i}'

     

    网站日志分析篇1（Apache）：

    1.获得访问前10位的ip地址

    cat access.log|awk ‘{print $1}’|sort|uniq -c|sort -nr|head -10
    cat access.log|awk ‘{counts[$(11)]+=1}; END {for(url in counts) print counts[url], url}’

    2.访问次数最多的文件或页面,取前20

    cat access.log|awk ‘{print $11}’|sort|uniq -c|sort -nr|head -20

    3.列出传输最大的几个exe文件（分析下载站的时候常用）

    cat access.log |awk ‘($7~/.exe/){print $10 " " $1 " " $4 " " $7}’|sort -nr|head -20

    4.列出输出大于200000byte(约200kb)的exe文件以及对应文件发生次数

    cat access.log |awk ‘($10 > 200000 && $7~/.exe/){print $7}’|sort -n|uniq -c|sort -nr|head -100

    5.如果日志最后一列记录的是页面文件传输时间，则有列出到客户端最耗时的页面

    cat access.log |awk ‘($7~/.php/){print $NF " " $1 " " $4 " " $7}’|sort -nr|head -100

    6.列出最最耗时的页面(超过60秒的)的以及对应页面发生次数

    cat access.log |awk ‘($NF > 60 && $7~/.php/){print $7}’|sort -n|uniq -c|sort -nr|head -100

    7.列出传输时间超过 30 秒的文件

    cat access.log |awk ‘($NF > 30){print $7}’|sort -n|uniq -c|sort -nr|head -20

    8.统计网站流量（G)

    cat access.log |awk ‘{sum+=$10} END {print sum/1024/1024/1024}’

    9.统计404的连接

    awk ‘($9 ~/404/)’ access.log | awk ‘{print $9,$7}’ | sort

    10. 统计http status

    cat access.log |awk ‘{counts[$(9)]+=1}; END {for(code in counts) print code, counts[code]}'
    cat access.log |awk '{print $9}'|sort|uniq -c|sort -rn

    10.蜘蛛分析，查看是哪些蜘蛛在抓取内容。

    /usr/sbin/tcpdump -i eth0 -l -s 0 -w - dst port 80 | strings | grep -i user-agent | grep -i -E 'bot|crawler|slurp|spider'

    网站日分析2(Squid篇）按域统计流量

    zcat squid_access.log.tar.gz| awk '{print $10,$7}' |awk 'BEGIN{FS="[ /]"}{trfc[$4]+=$1}END{for(domain in trfc){printf "%st%dn",domain,trfc[domain]}}'

    数据库篇
    1.查看数据库执行的sql

    /usr/sbin/tcpdump -i eth0 -s 0 -l -w - dst port 3306 | strings | egrep -i 'SELECT|UPDATE|DELETE|INSERT|SET|COMMIT|ROLLBACK|CREATE|DROP|ALTER|CALL'

    系统Debug分析篇
    1.调试命令
    strace -p pid
    2.跟踪指定进程的PID
    gdb -p pid
```

9.gawk应用实现

（1）内建变量；
1）字段和数据行分隔符变量



```ruby
    数据字段变量：
        允许你使用美元符号($)和数据字段在数据行中位置对应的数值来引用该数据行中的字段

    字段分隔符：
        默认情况下，字段分隔符是一个空白字符，也就是空格符或制表符(tab)。在命令行下使用命令行参数-F或者在gawk程序中使用特殊的内置变量FS来更改字段分隔符

    变量汇总：
        FIELDWIDTHS         由空格分隔开的定义了每个数据字段确切宽度的一列数字
        FS                  输入字段分隔符
        RS                  输入数据行分隔符
        OFS                 输出字段分隔符
        ORS                 输出数据行分隔符

    变量FS和OFS定义了gawk如何处理数据流中的数据字段

        [root@ahui ~]# cat data1
        data11,data12,data13,data14,data15
        data21,data22,data23,data24,data25
        data31,data32,data33,data34,data35
        
        [root@ahui ~]# awk 'BEGIN{FS=","; OFS="-"} {print $1,$2,$3}' data1
        data11-data12-data13
        data21-data22-data23
        data31-data32-data33

    FIELDWIDTHS变量允许你读取数据行，而不用字段分隔符来划分字段。一旦设置了FIELDWIDTHS变量，gawk就会忽略FS变量，而根据提供的字段宽度大小来计算字段

        [root@ahui ~]# cat data1b
        1005.3847887.37
        133-2.374893.23
        23987.8237488.9
        
        [root@ahui ~]# awk 'BEGIN{FIELDWIDTHS="3 5 2 5"} {print $1,$2,$3,$4}' data1b
        100 5.384 78 87.37
        133 -2.37 48 93.23
        239 87.82 37 488.9

    默认情况下，gawk将RS和ORS设为换行符，但是有时会碰到数据流中字段占了多行的情况。
    经典的例子就是包含地址和电话号码的数据，其中地址和电话号码各占一行

        [root@ahui ~]# cat data2
        Riley Mullen
        123 main street
        Chicago, IL 60601
        (312)555-2378

        Frank Williams
        123 main street
        Chicago, IL 60601
        (312)234-3253

        Haley Snell
        123 main street
        Chicago, IL 60601
        (312)123-2374
        
        [root@ahui ~]# gawk 'BEGIN{FS="\n"; RS=""} {print $1,$4}' data2
        Riley Mullen  (312)555-2378
        Frank Williams (312)234-3253
        Haley Snell (312)123-2374

        只需把FS变量设置成换行符。这就表明数据流中的每行都是一个单独的字段，每行上的所有数据都属于同一个字段。
        接着只需把RS变量设置成空字符串，然后在数据行间留一个空白行。gawk会把每个空白行当做一个数据行分隔符

2）数据变量：
    ARGC        当前命令行参数个数
    ARGIND      当前文件在ARGV中的位置
    ARGV        包含命令行参数的数组
    CONVFMT     数字的转换格式(参见printf语句)；默认值为%.6 g
    ENVIRON     当前shell环境变量及其值组成的关联数组
    ERRNO       当读取或关闭输入文件发生错误时的系统错误号
    FILENAME    用作gawk输入数据的数据文件的文件名
    FNR         当前数据文件中的数据行数
    IGNORECASE  设成非零值时，忽略gawk命令中出现的字符串的字符大小写
    NF          数据文件中的字段总数
    NR          已处理的输入数据行数目
    OFMT        数字的输出格式；默认值为%.6 g
    RLENGTH     由match函数所匹配的子字符串的长度
    RSTART      由match函数所匹配的子字符串的起始位置

    相关实例：

        [root@ahui ~]# gawk 'BEGIN{print ARGC,ARGV[1]}' data1
        2 data1

        注意：ARGC变量表明命令行上有两个参数，这包括gawk命令和data1参数(记住，程序脚本并不算参数)。
             ARGV数组从代表该命令的索引0开始，第一个数组值是gawk命令后的第一个命令行参数。


        [root@ahui ~]# gawk '
        > BEGIN{
        > print ENVIRON["HOME"]
        > print ENVIRON["PATH"]
        > }'
        /root
        /usr/lib64/qt-3.3/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin

        注意：ENVIRON变量使用关联数组来提取shell环境变量。关联数组用文本作为数组的索引值，而不用数值。可用这种方式来从shell中提取任何环境变量的值来在gawk程序中使用。

        [root@ahui ~]# gawk 'BEGIN{FS=":"; OFS=":"} {print $1,$NF}' /etc/passwd
        root:/bin/bash
        bin:/sbin/nologin
        daemon:/sbin/nologin
        adm:/sbin/nologin
        lp:/sbin/nologin
        sync:/bin/sync
        shutdown:/sbin/shutdown
        halt:/sbin/halt
        mail:/sbin/nologin
        uucp:/sbin/nologin
        operator:/sbin/nologin
        games:/sbin/nologin
        gopher:/sbin/nologin
        ftp:/sbin/nologin
        nobody:/sbin/nologin
        dbus:/sbin/nologin
        usbmuxd:/sbin/nologin
        vcsa:/sbin/nologin
        rpc:/sbin/nologin
        rtkit:/sbin/nologin
        avahi-autoipd:/sbin/nologin
        abrt:/sbin/nologin
        rpcuser:/sbin/nologin
        nfsnobody:/sbin/nologin
        haldaemon:/sbin/nologin
        gdm:/sbin/nologin
        ntp:/sbin/nologin
        apache:/sbin/nologin
        saslauth:/sbin/nologin
        postfix:/sbin/nologin
        pulse:/sbin/nologin
        sshd:/sbin/nologin
        tcpdump:/sbin/nologin
        ahui:/bin/bash

        注意：NF变量允许你指定数据行中的最后一个数据字段，即NF变量含有数据文件中最后一个数据字段的数字值。可以在它前面加个美元符将它用作字段变量。

        [root@ahui ~]# gawk '
        > BEGIN{FS=","}
        > {print $1, "FNR="FNR, "NR="NR}
        > END{print "There were",NR,"records processed"}' data1 data1
        data11 FNR=1 NR=1
        data21 FNR=2 NR=2
        data31 FNR=3 NR=3
        data11 FNR=1 NR=4
        data21 FNR=2 NR=5
        data31 FNR=3 NR=6
        There were 6 records processed

        注意：FNR变量的值在gawk处理第二个文件时被重置了,而NR变量则在进入第二个数据文件后继续计数
             注意变量赋值的语法结构："FNR="FNR。引号位置错了会出现语法错误
```

（2）自定义变量
1）在脚本中给变量赋值



```swift
    [root@ahui ~]# gawk '
    > BEGIN{
    > testing="This is a test"
    > print testing
    > }'
    This is a test

    注意：gawk编程语言包含了用来处理数字值的标准数学操作符，其中包括求余符号(%)和幂运算符号(^或**)

2）在命令行上给变量赋值

    [root@ahui ~]# cat script1
    BEGIN{FS=","}
    {print $n}
    
    [root@ahui ~]# gawk -f script1 n=2 data1
    data12
    data22
    data32

    这个特性允许你改变脚本的行为而不需要修改实际的脚本代码。
    使用命令行参数来定义变量值会有个问题。在你设置了变量后，这个值在代码的BEGIN部分不可用

    [root@ahui ~]# cat script2
    BEGIN{print "The starting value is",n; FS=","}
    {print $n}
    
    [root@ahui ~]# gawk -f script2 n=3 data1
    The starting value is
    data13
    data23
    data33

    可以用-v命令行参数来解决这个问题。它允许你指定在BEGIN代码部分之前设定的变量。在命令行上，-v命令行参数必须放在脚本代码之前

    [root@ahui ~]# gawk -v n=3 -f script2 data1
    The starting value is 3
    data13
    data23
    data33
```

（3）处理数组



```php
gawk编程语言使用关联数组来提供数组功能，关联数组跟数字数组不同之处在于它的索引值可以是任意文本字符串。每个索引字符串都必须是唯一的，并唯一地标识赋给它的数据元素。

1）定义数组变量
    数组变量赋值的格式如下；
        var[index] = element
    其中var是变量名，index是关联数组的索引值，element是数据元素值。

    [root@ahui ~]# gawk 'BEGIN{
    > capital["Illinois"] = "Springfiled"
    > print capital["Illinois"]
    > }'
    Springfiled

2）遍历数组变量
    可以用for语句的一种特殊形式：
        for (var in array)
        {
            statements
        }
    这个for语句会在每次将关联数组array的下一个索引值赋给变量var时，执行一遍statements。重要的是记住这个变量是索引值而不是数据元素值。

        [root@ahui ~]# gawk 'BEGIN{
        > var["a"] = 1
        > var["b"] = 2
        > var["c"] = 3
        > var["d"] = 4
        > for (test in var)
        > {
        > print "Index:",test," - value:",var[test]
        > }
        > }'
        Index: a  - value: 1
        Index: b  - value: 2
        Index: c  - value: 3
        Index: d  - value: 4

3）删除数组变量
    从关联数组中删除数组索引要用一个特别的命令：
        delete array[index]

    删除命令会从数组中删除关联索引值和相关的数据元素值
        [root@ahui ~]# gawk 'BEGIN{
        > var["a"] = 1
        > var["g"] = 2
        > for (test in var)
        > {
        > print "Index:",test," - value:",var[test]
        > }
        > delete var["g"]
        > print "---"
        > for (test in var)
        > print "Index:",test," - Value:",var[test]
        > }'
        Index: a  - value: 1
        Index: g  - value: 2
        ---
        Index: a  - Value: 1
```

（4）使用模式



```ruby
1）正则表达式
    在使用正则表达式时，正则表达式必须出现在它要控制的程序脚本的左花括号前：

        [root@ahui ~]# gawk 'BEGIN{FS=","} /11/{print $1}' data1
        data11

2）匹配操作符
    匹配操作符(matching operate)允许将正则表达式限定在数据行中的特定数据字段。匹配操作符是波浪线(~)。
        $1 ~ /^data/
    $1变量代表数据行中的第一个数据字段，这个表达式会过滤出第一个字段以文本data开头的所有数据行。

        [root@ahui ~]# gawk 'BEGIN{FS=","} $2 ~ /^data2/{print $0}' data1
        data21,data22,data23,data24,data25  

    也可以使用!符号来排除正则表达式的匹配：
        $1 !~ /expression/

        [root@ahui ~]# gawk  '$1 !~ /root/{print $1,$NF}' /etc/passwd
        bin:x:1:1:bin:/bin:/sbin/nologin bin:x:1:1:bin:/bin:/sbin/nologin
        daemon:x:2:2:daemon:/sbin:/sbin/nologin daemon:x:2:2:daemon:/sbin:/sbin/nologin
        adm:x:3:4:adm:/var/adm:/sbin/nologin adm:x:3:4:adm:/var/adm:/sbin/nologin
        lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
        sync:x:5:0:sync:/sbin:/bin/sync sync:x:5:0:sync:/sbin:/bin/sync
        shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown
        halt:x:7:0:halt:/sbin:/sbin/halt halt:x:7:0:halt:/sbin:/sbin/halt
        mail:x:8:12:mail:/var/spool/mail:/sbin/nologin mail:x:8:12:mail:/var/spool/mail:/sbin/nologin
        uucp:x:10:14:uucp:/var/spool/uucp:/sbin/nologin uucp:x:10:14:uucp:/var/spool/uucp:/sbin/nologin
        games:x:12:100:games:/usr/games:/sbin/nologin games:x:12:100:games:/usr/games:/sbin/nologin
        gopher:x:13:30:gopher:/var/gopher:/sbin/nologin gopher:x:13:30:gopher:/var/gopher:/sbin/nologin
        ftp:x:14:50:FTP User:/var/ftp:/sbin/nologin
        nobody:x:99:99:Nobody:/:/sbin/nologin nobody:x:99:99:Nobody:/:/sbin/nologin
        dbus:x:81:81:System bus:/:/sbin/nologin
        usbmuxd:x:113:113:usbmuxd user:/:/sbin/nologin
        vcsa:x:69:69:virtual owner:/dev:/sbin/nologin
        rpc:x:32:32:Rpcbind Daemon:/var/cache/rpcbind:/sbin/nologin
        rtkit:x:499:497:RealtimeKit:/proc:/sbin/nologin rtkit:x:499:497:RealtimeKit:/proc:/sbin/nologin
        avahi-autoipd:x:170:170:Avahi Stack:/var/lib/avahi-autoipd:/sbin/nologin
        abrt:x:173:173::/etc/abrt:/sbin/nologin abrt:x:173:173::/etc/abrt:/sbin/nologin
        rpcuser:x:29:29:RPC User:/var/lib/nfs:/sbin/nologin
        nfsnobody:x:65534:65534:Anonymous User:/var/lib/nfs:/sbin/nologin
        haldaemon:x:68:68:HAL daemon:/:/sbin/nologin
        gdm:x:42:42::/var/lib/gdm:/sbin/nologin gdm:x:42:42::/var/lib/gdm:/sbin/nologin
        ntp:x:38:38::/etc/ntp:/sbin/nologin ntp:x:38:38::/etc/ntp:/sbin/nologin
        apache:x:48:48:Apache:/var/www:/sbin/nologin apache:x:48:48:Apache:/var/www:/sbin/nologin
        saslauth:x:498:76:Saslauthd user:/var/empty/saslauth:/sbin/nologin
        postfix:x:89:89::/var/spool/postfix:/sbin/nologin postfix:x:89:89::/var/spool/postfix:/sbin/nologin
        pulse:x:497:496:PulseAudio Daemon:/var/run/pulse:/sbin/nologin
        sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin
        tcpdump:x:72:72::/:/sbin/nologin tcpdump:x:72:72::/:/sbin/nologin
        ahui:x:500:500:ahuihaerbin:/home/ahui:/bin/bash ahui:x:500:500:ahuihaerbin:/home/ahui:/bin/bash

3）数学表达式

    可以使用任意的普通股数学比较表达式
        x == y：值x等于y
        x <= y：
        x < y
        x >= y
        x > y

    可以对文本数据使用表达式，但是它跟正则表达式不同，表达式必须完全匹配。

        [root@ahui ~]# gawk -F, '$1 == "data"{print $1}' data1
        
        [root@ahui ~]# gawk -F, '$1 == "data11"{print $1}' data1
        data11
```

（5）结构化命令
1）if语句
格式：
if (condition)
statement1
或
if (condition) statement1



```swift
    [root@ahui ~]# cat data4
    10
    5
    13
    50
    34
    
    [root@ahui ~]# gawk '{
    > if ($1 > 20)
    > {
    >   x = $1 * 2
    >   print x
    > }
    > }' data4
    100
    68

    gawk的if语句也支持else子句，允许在if语句条件不成立的情况下执行一条或多条语句

    [root@ahui ~]# gawk '{
    > if ($1 > 20)
    > {
    >   x = $1 * 2
    >   print x
    > } else
    > {
    >   x = $1 / 2
    >   print x
    > }}' data4
    5
    2.5
    6.5
    100
    68

    也可以在单行上使用else子句，但必须在if语句部分使用分号：

        if (condition) statement1; else statement2

    [root@ahui ~]# gawk '{if ($1 > 20) print $1 * 2; else print $1 / 2}' data4
    5
    2.5
    6.5
    100
    68


2）while语句

    while语句的格式：

        while (condition)
        {
        statements
        }

    while循环允许遍历一组数据，并检查结束迭代的条件

    [root@ahui ~]# gawk '{
    total = 0
    i = 1
    while (i < 4)
    {
       total += $i
       i++
    }
    avg = total / 3
    print "Average:" ,avg
    }' data5
    Average: 128.333
    Average: 137.667
    Average: 176.667

    [root@ahui ~]# gawk '{
    > total = 0
    > i = 1
    > while (i < 4)
    > {
    >   total += $i
    >   if (i == 2)
    >      break
    >   i++
    > }
    > avg = total / 2
    > print "The average of the first two data elements is:",avg
    > }' data5
    The average of the first two data elements is: 125
    The average of the first two data elements is: 136.5
    The average of the first two data elements is: 157.5

3）do-while语句
    do-while语句类似于while语句，但会在检查条件语句之前执行命令。格式为：
        do
        {
            statement
        } while (condition)

    这种格式保证了语句会在条件被评估之前至少执行一次

4）for语句
    gawk支持C风格的for循环：
        for( variable assignment; condition; iteration process)
```

（6）格式化打印
注意到print语句在gawk如何显示数据上并未提供多少控制。你能做的大概只是控制输出字段分隔符(OFS)。



```objectivec
因此要用到格式化打印，我们需要printf命令。格式：
    printf "format string", var1, var2...

format string是格式化输出地关键。它会用到文本元素和格式化指定符来具体指定如何呈现格式化输出。

格式化指定符采用如下格式：
    %[modifier]control-letter
其中control-letter是指明显示什么类型数据值的单字符码，而modifier定义了另一个可选的格式化特性。

    c           将一个数作为ASCII字符显示
    d           显示一个整数值
    i           显示一个整数值(跟d一样)
    e           用科学计数法显示一个数
    f           显示一个浮点数
    g           用科学计数法或浮点数中较短的显示
    o           显示一个八进制值
    s           显示一个文本字符串
    x           显示一个十六进制值
    X           显示一个十六进制值，但用大写字母A-F

除了控制字母外，还有3种修饰符可以用来进一步控制输出

    width：
        指定了输出字段最小宽度的数字值。如果输出短于这个值，printf会向右对齐，并用空格来填充这段空间。如果输出比指定的宽度还要长，它就会覆盖width值。
    prec：
        指定了浮点数中小数点后面位数的数字值，或者文本字符串中显示的最大字符数。
    -(减号)：
        减号指明在向格式化空间中放入数据时采用左对齐而不是右对齐

        [root@ahui ~]# gawk 'BEGIN{FS="\n"; RS=""} {printf "%s %s\n", $1, $4}' data2
        Riley Mullen  (312)555-2378
        Frank Williams (312)234-3253
        Haley Snell (312)123-2374

    注意：我们需要自己手动在printf命令的末尾添加换行符来生成新行。没加的话，printf命令会继续用同一行来打印后续输出。

    如果你需要用几个单独的printf命令来在同一行上打印多个输出，它会非常有用：

        [root@ahui ~]# gawk 'BEGIN{FS=","} {printf "%s ", $1} END{printf "\n"}' data1
        data11 data21 data31

    接着我们用修饰符来格式化第一个字符串值：

        [root@ahui ~]# gawk 'BEGIN{FS="\n"; RS=""} {printf "%16s  %s\n",$1, $4}' data2
            Riley Mullen  (312)555-2378
          Frank Williams  (312)234-3253
             Haley Snell  (312)123-2374

    通过添加一个值为16的修饰符，我们强制第一个字符串的输出采用16位字符。默认情况下，printf命令使用右对齐来将数据放到格式化空间中。要改成左对齐，只要给修饰符加一个减号就可以了。

        [root@ahui ~]# gawk 'BEGIN{FS="\n"; RS=""} {printf "%-16s  %s\n", $1, $4}' data2
        Riley Mullen      (312)555-2378
        Frank Williams    (312)234-3253
        Haley Snell       (312)123-2374
```

（7）自定义函数
1）定义函数
要定义自己的函数，你必须使用function关键字：
function name([variables])
{
statements
}



```ruby
    函数名必须能够唯一标识函数

2）使用自定义函数
    在定义函数时，它必须出现在所有代码块之前(包括BEGIN代码块)。

        [root@ahui ~]# gawk '
        > function myprint()
        > {
        >     printf "%-16s - %s\n", $1, $4
        > }
        > BEGIN{FS="\n"; RS=""}
        > {
        >     myprint()
        > }' data2
        Riley Mullen     - (312)555-2378
        Frank Williams   - (312)234-3253
        Haley Snell      - (312)123-2374

3）创建函数库
    首先创建一个存储所有gawk函数的文件

        [root@ahui ~]# cat funclib
        function myprint()
        {
            printf "%-16s - %s\n", $1, $4
        }
        function myrand()
        {
            return int(limit * rand())
        }
        function printthird()
        {
            printf $3
        }

    注意：不能将-f命令行参数和内联gawk脚本放到一起使用，不过可以在同一个命令行中使用多个-f参数

        [root@ahui ~]# cat script4
        BEGIN{ FS="\n"; RS="" }
        {
            myprint()
        }

        [root@ahui ~]# gawk -f funclib -f script4 data2
        Riley Mullen     - (312)555-2378
        Frank Williams   - (312)234-3253
        Haley Snell      - (312)123-2374
```
