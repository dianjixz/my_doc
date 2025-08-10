perf安装和使用

    #centos
    yum install perf
     
    #ubuntu
    https://www.jianshu.com/p/4659063b3c0d

perf官网例子

Linux perf Examples

性能调优时，我们通常需要分析查找到程序百分比高的热点代码片段，这便需要使用 perf record 记录单个函数级别的统计信息，并使用 perf report 来显示统计结果。

perf record表示记录，-F 99表示每秒99次采样，-p 1336 是针对哪个进程进行抓取，-g表示记录调用栈，sleep 10则是持续10秒

perf record -F 99 -p 1336 -g -- sleep 10

抓取所有的记录则使用如下命令

perf record -F 99 -a -g -- sleep 10

抓取完后会产生一个perf.data文件，用下面命令将其转化为可被直接读懂的callstack信息文件 out.perf

perf script > out.perf

使用如下命令也能够看到各个stack之间的调用关系和采样时间

perf report -n --stdio

火焰图生成

    git clone https://github.com/brendangregg/FlameGraph.git
    cd FlameGraph
    ./stackcollapse-perf.pl out.perf > out.folded
    ./flamegraph.pl out.folded > test.svg

参考

Perf和火焰图 | Half Coder
————————————————
版权声明：本文为CSDN博主「小伟db」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_35985044/article/details/127670918



sudo perf record -F 99 -p 339361 -g -- sleep 30

sudo chown nihao:nihao perf.data
sleep 1
perf script > out.perf
sleep 1
./FlameGraph/stackcollapse-perf.pl out.perf > out.folded
sleep 1
./FlameGraph/flamegraph.pl out.folded > test.svg




sudo chown nihao:nihao perf.data
perf script > out.perf
/home/nihao/w2T/github/FlameGraph/stackcollapse-perf.pl out.perf > out.folded
/home/nihao/w2T/github/FlameGraph/flamegraph.pl out.folded > test.svg