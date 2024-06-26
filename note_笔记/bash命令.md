~~~ 
# shell中条件判断if中的-z到-d的意思
shell中条件判断if中的-z到-d的意思 
[ -a FILE ] 如果 FILE 存在则为真。 
[ -b FILE ] 如果 FILE 存在且是一个块特殊文件则为真。
[ -c FILE ] 如果 FILE 存在且是一个字特殊文件则为真。 
[ -d FILE ] 如果 FILE 存在且是一个目录则为真。 
[ -e FILE ] 如果 FILE 存在则为真。
[ -f FILE ] 如果 FILE 存在且是一个普通文件则为真。 
[ -g FILE ] 如果 FILE 存在且已经设置了SGID则为真。 
[ -h FILE ] 如果 FILE 存在且是一个符号连接则为真。 
[ -k FILE ] 如果 FILE 存在且已经设置了粘制位则为真。 
[ -p FILE ] 如果 FILE 存在且是一个名字管道(F如果O)则为真。 
[ -r FILE ] 如果 FILE 存在且是可读的则为真。 
[ -s FILE ] 如果 FILE 存在且大小不为0则为真。  
[ -t FD ] 如果文件描述符 FD 打开且指向一个终端则为真。 
[ -u FILE ] 如果 FILE 存在且设置了SUID (set user ID)则为真。 
[ -w FILE ] 如果 FILE 如果 FILE 存在且是可写的则为真。 
[ -x FILE ] 如果 FILE 存在且是可执行的则为真。 
[ -O FILE ] 如果 FILE 存在且属有效用户ID则为真。 
[ -G FILE ] 如果 FILE 存在且属有效用户组则为真。 
[ -L FILE ] 如果 FILE 存在且是一个符号连接则为真。  
[ -N FILE ] 如果 FILE 存在 and has been mod如果ied since it was last read则为真。
[ -S FILE ] 如果 FILE 存在且是一个套接字则为真。  
[ FILE1 -nt FILE2 ] 如果 FILE1 has been changed more recently than FILE2,or 如果 FILE1 exists and FILE2 does not则为真。  
[ FILE1 -ot FILE2 ] 如果 FILE1 比 FILE2 要老, 或者 FILE2 存在且 FILE1 不存在则为真。  
[ FILE1 -ef FILE2 ] 如果 FILE1 和 FILE2 指向相同的设备和节点号则为真。 
[ -o OPTIONNAME ] 如果 shell选项 “OPTIONNAME” 开启则为真。 
[ -z STRING ] “STRING” 的长度为零则为真。  
[ -n STRING ] or [ STRING ] “STRING” 的长度为非零 non-zero则为真。 

~~~

数字判断 
~~~ 
[ $count -gt "1"] 如果$count 大于1 为真
-gt  大于
-lt    小于
-ne  不等于
-eq  等于
-ge  大于等于
-le  小于等于
~~~
~~~ 
[ STRING1 == STRING2 ] 如果2个字符串相同。 “=” may be used instead of “==” for strict POSIX compliance则为真。  
[ STRING1 != STRING2 ] 如果字符串不相等则为真。  
[ STRING1 < STRING2 ] 如果 “STRING1” sorts before “STRING2” lexicographically in the current locale则为真。  
[ STRING1 > STRING2 ] 如果 “STRING1” sorts after “STRING2” lexicographically in the current locale则为真。  
~~~
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch path-to-your-remove-file' --prune-empty --tag-name-filter cat -- --all
其中, path-to-your-remove-file 就是你要删除的文件的相对路径(相对于git仓库的跟目录), 替换成你要删除的文件即可. 注意一点，这里的文件或文件夹，都不能以 '/' 开头，否则文件或文件夹会被认为是从 git 的安装目录开始。

如果你要删除的目标不是文件，而是文件夹，那么请在 `git rm --cached' 命令后面添加 -r 命令，表示递归的删除（子）文件夹和文件夹下的文件，类似于 `rm -rf` 命令。

此外，如果你要删除的文件很多, 可以写进一个.sh文件批量执行, 如果文件或路径里有中文, 由于MinGW或CygWin对中文路径设置比较麻烦, 你可以使用通配符*号, 例如: sound/music_*.mp3, 这样就把sound目录下以music_开头的mp3文件都删除了.

例如这样, 新建一个 bash 脚本文件，del-music-mp3.sh:
~~~ 
#!/bin/bash

git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch projects/Moon.mp3' --prune-empty --tag-name-filter cat -- --all
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch sound/Music_*.mp3' --prune-empty --tag-name-filter cat -- --all
~~~
 如果你看到类似下面这样的, 就说明删除成功了:

Rewrite 48dc599c80e20527ed902928085e7861e6b3cbe6 (266/266)
# Ref 'refs/heads/master' was rewritten
如果显示 xxxxx unchanged, 说明repo里没有找到该文件, 请检查路径和文件名是否正确.

步骤二: 推送我们修改后的repo
以强制覆盖的方式推送你的repo, 命令如下:
~~~ 
$ git push origin master --force --all
~~~
为了能从打了 tag 的版本中也删除你所指定的文件或文件夹，您可以使用这样的命令来强制推送您的 Git tags：
~~~ 
$ git push origin master --force --tags
~~~

步骤三: 清理和回收空间
虽然上面我们已经删除了文件, 但是我们的repo里面仍然保留了这些objects, 等待垃圾回收(GC), 所以我们要用命令彻底清除它, 并收回空间.

命令如下:

复制代码
~~~ 
$ rm -rf .git/refs/original/

$ git reflog expire --expire=now --all

$ git gc --prune=now

Counting objects: 2437, done.
# Delta compression using up to 4 threads.
# Compressing objects: 100% (1378/1378), done.
# Writing objects: 100% (2437/2437), done.
# Total 2437 (delta 1461), reused 1802 (delta 1048)

$ git gc --aggressive --prune=now

Counting objects: 2437, done.
# Delta compression using up to 4 threads.
# Compressing objects: 100% (2426/2426), done.
# Writing objects: 100% (2437/2437), done.
# Total 2437 (delta 1483), reused 0 (delta 0)
复制代码
注: 绿色字部分是命令执行后的结果.
~~~
https://www.cnblogs.com/shines77/p/3460274.html


~~~ 
pandoc --pdf-engine=xelatex README.md -o example13.pdf    转换md到pdf命令

fc-cache -fv   刷新字体缓存
/usr/share/fonts/truetype   字体目录

bash-completion		tab补全插件
#Ubuntu也可以进行命令补全 apt install -y bash-completion
~~~



 Linux Shell中捕获CTRL+C
~~~ 
#!/bin/bash

trap 'onCtrlC' INT
function onCtrlC () {
echo 'Ctrl+C is captured'
}

while true; do
echo 'I am working!'
sleep 1
done
~~~
~~~ 

Linux中shell脚本获取当前工作目录的方法：

~~~
workdir=$(cd $(dirname $0); pwd)
~~~


shell 计数命令     wc -w
~~~


linux bash shell:最方便的字符大小写转换
注意,一定是需要bash
~~~ bash
#!/bin/bash
nihao@nihao:~/work/work_space/libmaix/examples/zhongming/res$ var="Hello,Word"
nihao@nihao:~/work/work_space/libmaix/examples/zhongming/res$ echo ${var^}
Hello,Word
nihao@nihao:~/work/work_space/libmaix/examples/zhongming/res$ echo ${var^^}
HELLO,WORD
nihao@nihao:~/work/work_space/libmaix/examples/zhongming/res$ echo ${var,}
hello,Word
nihao@nihao:~/work/work_space/libmaix/examples/zhongming/res$ echo ${var,,}
hello,word
nihao@nihao:~/work/work_space/libmaix/examples/zhongming/res$ 
~~~



