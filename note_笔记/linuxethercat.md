linux 平台下的 ethercat 的使用

ethercat的主站软件很多中，开源的大概有两个，一个是已经停止更新的 ethercat ，一个是还在更新的 soe.soe包含soem，soes。

本次使用的是soem
https://github.com/OpenEtherCATsociety/SOEM



https://blog.csdn.net/weixin_43956732/category_10071133.html


SOEM简单示例
https://blog.csdn.net/weixin_43455581/article/details/106171165

Soem配置汇川SV660N
https://blog.csdn.net/lg28870983/article/details/124393074






SOEM主站开发笔记-- 点亮第一个LED----SOEM 的simpletest.c代码的解析以及改动

本周实现了SOEM 点亮第一个LED的操作，记录一下实现的过程。

开发平台使用的是野火的imx6ull。
1. 下载代码

SOEM的代码在github上就可以找到，它的官网下载链接是：

https://github.com/OpenEtherCATsociety/SOEM

    1

主站代码如果懒得下载可以直接下载我提供的工程。我的项目资料托管在gitee，里面有zip 形式的代码压缩包。

https://gitee.com/jeasonb/soem_-imx6ull

    1

2.编译代码

SOEM的代码支持很多的平台，我的实验是基于linux 平台进行的。另外一方面就是 SOEM 是使用cmake 生成和管理Makefile 文件的，如果读者的开发环境没有cmake的话是需要去安装一个的。

先分享一个错误的编译过程：
```bash
root@jeason:~/work/ethercat/SOEM-master# ls
appveyor.yml  bin  ChangeLog  cmake  CMakeLists.txt  doc  Doxyfile  drvcomment.txt  LICENSE  osal  oshw  README.md  soem  test
root@jeason:~/work/ethercat/SOEM-master# mkdir build
root@jeason:~/work/ethercat/SOEM-master# cd build/
root@jeason:~/work/ethercat/SOEM-master/build# cmake ..
-- The C compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
OS is linux
LIB_DIR: lib
-- Configuring done
-- Generating done
-- Build files have been written to: /root/work/ethercat/SOEM-master/build
root@jeason:~/work/ethercat/SOEM-master/build# make
Scanning dependencies of target soem
[  4%] Building C object CMakeFiles/soem.dir/soem/ethercatbase.c.o
[  9%] Building C object CMakeFiles/soem.dir/soem/ethercatcoe.c.o
[ 14%] Building C object CMakeFiles/soem.dir/soem/ethercatconfig.c.o
[ 19%] Building C object CMakeFiles/soem.dir/soem/ethercatdc.c.o
[ 23%] Building C object CMakeFiles/soem.dir/soem/ethercateoe.c.o
[ 28%] Building C object CMakeFiles/soem.dir/soem/ethercatfoe.c.o
[ 33%] Building C object CMakeFiles/soem.dir/soem/ethercatmain.c.o
[ 38%] Building C object CMakeFiles/soem.dir/soem/ethercatprint.c.o
[ 42%] Building C object CMakeFiles/soem.dir/soem/ethercatsoe.c.o
[ 47%] Building C object CMakeFiles/soem.dir/osal/linux/osal.c.o
[ 52%] Building C object CMakeFiles/soem.dir/oshw/linux/nicdrv.c.o
[ 57%] Building C object CMakeFiles/soem.dir/oshw/linux/oshw.c.o
[ 61%] Linking C static library libsoem.a
[ 61%] Built target soem
Scanning dependencies of target slaveinfo
[ 66%] Building C object test/linux/slaveinfo/CMakeFiles/slaveinfo.dir/slaveinfo.c.o
[ 71%] Linking C executable ../../../../bin/slaveinfo
[ 71%] Built target slaveinfo
Scanning dependencies of target eepromtool
[ 76%] Building C object test/linux/eepromtool/CMakeFiles/eepromtool.dir/eepromtool.c.o
[ 80%] Linking C executable ../../../../bin/eepromtool
[ 80%] Built target eepromtool
Scanning dependencies of target simple_test
[ 85%] Building C object test/linux/simple_test/CMakeFiles/simple_test.dir/simple_test.c.o
[ 90%] Linking C executable ../../../../bin/simple_test
[ 90%] Built target simple_test
Scanning dependencies of target my_test
[ 95%] Building C object test/linux/my_test/CMakeFiles/my_test.dir/my_test.c.o
[100%] Linking C executable ../../../../bin/my_test
[100%] Built target my_test
root@jeason:~/work/ethercat/SOEM-master/build#



```
上面的虽然最后都成功的编译出了可执行的文件，但是 并没有使用交叉编译工具链，编译出来的版本是基于当前的开发服务器版本的。所以就是说如果是需要在树莓派或者ubuntu 上直接搭建SOEM的主站的小伙伴可以使用这种编译方法，但是为了在imx6ull 上跑主站，我们显然是不能这么编译。
我们需要设置工具链！！！
在进行开发之前我已经将交叉编译工具链安装到我的系统路径下，所以我不需要export PATH

root@jeason:~/work/ethercat/SOEM-master/build# which arm-linux-gnueabihf-gcc
/usr/bin/arm-linux-gnueabihf-gcc

    1
    2

如果没有将工具链的路径加入到系统路径 请执行

export PATH=$PATH:/your/tool/path   #  /your/tool/path 是你的工具链的位置

    1

接下来修改系统内的默认编译工具 执行

export CC=arm-linux-gnueabihf-gcc

    1

经过上述的操作之后现在我们的编译工具链就已经设置好了

此时就可以开始新的编译了
``` bash
root@jeason:~/work/ethercat/SOEM-master_2/SOEM-master# mkdir build
root@jeason:~/work/ethercat/SOEM-master_2/SOEM-master# cd build/
root@jeason:~/work/ethercat/SOEM-master_2/SOEM-master/build# cmake ..
-- The C compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/arm-linux-gnueabihf-gcc
-- Check for working C compiler: /usr/bin/arm-linux-gnueabihf-gcc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
OS is linux
LIB_DIR: lib
-- Configuring done
-- Generating done
-- Build files have been written to: /root/work/ethercat/SOEM-master_2/SOEM-master/build
root@jeason:~/work/ethercat/SOEM-master_2/SOEM-master/build# make
Scanning dependencies of target soem
[  5%] Building C object CMakeFiles/soem.dir/soem/ethercatbase.c.o
[ 10%] Building C object CMakeFiles/soem.dir/soem/ethercatcoe.c.o
[ 15%] Building C object CMakeFiles/soem.dir/soem/ethercatconfig.c.o
[ 21%] Building C object CMakeFiles/soem.dir/soem/ethercatdc.c.o
[ 26%] Building C object CMakeFiles/soem.dir/soem/ethercateoe.c.o
[ 31%] Building C object CMakeFiles/soem.dir/soem/ethercatfoe.c.o
[ 36%] Building C object CMakeFiles/soem.dir/soem/ethercatmain.c.o
[ 42%] Building C object CMakeFiles/soem.dir/soem/ethercatprint.c.o
[ 47%] Building C object CMakeFiles/soem.dir/soem/ethercatsoe.c.o
[ 52%] Building C object CMakeFiles/soem.dir/osal/linux/osal.c.o
[ 57%] Building C object CMakeFiles/soem.dir/oshw/linux/nicdrv.c.o
[ 63%] Building C object CMakeFiles/soem.dir/oshw/linux/oshw.c.o
[ 68%] Linking C static library libsoem.a
[ 68%] Built target soem
Scanning dependencies of target slaveinfo
[ 73%] Building C object test/linux/slaveinfo/CMakeFiles/slaveinfo.dir/slaveinfo.c.o
[ 78%] Linking C executable slaveinfo
[ 78%] Built target slaveinfo
Scanning dependencies of target eepromtool
[ 84%] Building C object test/linux/eepromtool/CMakeFiles/eepromtool.dir/eepromtool.c.o
[ 89%] Linking C executable eepromtool
[ 89%] Built target eepromtool
Scanning dependencies of target simple_test
[ 94%] Building C object test/linux/simple_test/CMakeFiles/simple_test.dir/simple_test.c.o
[100%] Linking C executable simple_test
[100%] Built target simple_test
root@jeason:~/work/ethercat/SOEM-master_2/SOEM-master/build#


```

可以很明显看到现在工具链已经切换过去了，并且已经编译出来了我们想要的可执行文件。 这一阶段的开发环境搭建就完成了，接下来就是撸代码了。
3.运行demo查看效果

在正式的撸代码之前，我们需要对官方提供的CMakeLists.txt 进行一定程序的修改。

1.设置输出bin 的路径，因为我不希望去SOEM/test/linux/xxx/xx下面去找我的可执行文件，所以要限定可执行文件的生成路径。

在cmake 中添加这条语句：

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

    1

这句话的意思就是让输出的文件 到你工程路径下的bin 文件夹

SOEM 为我们提供了三个 DEMO ，分别是①读取从站的信息②从站的 eeprom 操作（读取，写入，这个功能可以实现烧录XML文件）③简单的测试demo

程序编译完成之后我们可以先运行一下测试程序（slaveinfo），检查一下当前环境的硬件信息。程序的运行效果如下：
```bash
root@imx6ull14x14evk:/mnt/ethercat/SOEM-master/bin# ls
1.bin  111.bin  eepromtool  my_test  no_cpu.bin  simple_test  slaveinfo  spi.bin
root@imx6ull14x14evk:/mnt/ethercat/SOEM-master/bin# ./slaveinfo eth1
SOEM (Simple Open EtherCAT Master)
Slaveinfo
Starting slaveinfo
ec_init on eth1 succeeded.
1 slaves found and configured.
Calculated workcounter 3
Not all slaves reached safe operational state.
Slave 1 State= 8 StatusCode=   0 : No error

Slave:1
 Name:ECAT-EVB
 Output size: 48bits
 Input size: 48bits
 State: 8
 Delay: 0[ns]
 Has DC: 1
 DCParentport:0
 Activeports:1.0.0.0
 Configured address: 1001
 Man: 00000009 ID: 00009252 Rev: 00000001
 SM0 A:1000 L: 128 F:00010026 Type:1
 SM1 A:1080 L: 128 F:00010022 Type:2
 SM2 A:1100 L:   6 F:00010064 Type:3
 SM3 A:1400 L:   6 F:00010020 Type:4
 FMMU0 Ls:00000000 Ll:   6 Lsb:0 Leb:7 Ps:1100 Psb:0 Ty:02 Act:01
 FMMU1 Ls:00000006 Ll:   6 Lsb:0 Leb:7 Ps:1400 Psb:0 Ty:01 Act:01
 FMMUfunc 0:1 1:2 2:3 3:0
 MBX length wr: 128 rd: 128 MBX protocols : 04
 CoE details: 23 FoE details: 00 EoE details: 00 SoE details: 00
 Ebus current: 0[mA]
 only LRD/LWR:0
End slaveinfo, close socket
End program
root@imx6ull14x14evk:/mnt/ethercat/SOEM-master/bin#


```

通过上述的片段可以看出，我只是接了一个ethcat的从站模块，而且这个模块只是一个简单的8个LED 和8个按键的板子。

至于eeprom的demo 可以用来烧录xml 的bin文件，这里就不去继续演示了。
4.修改代码

这里要修改的代码是依据simpletest 这个demo的。其路径是在 test\linux\simple_test\simple_test.c

这里面是一个简单的主站的测试demo。

主函数的介绍
``` c
//  ./slaveinfo eth1
int main(int argc, char *argv[])
{
   printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");

   if (argc > 1) // 输入参数检验，只支持一个参数的输入
   {
      /* create thread to handle slave error handling in OP */
//      pthread_create( &thread1, NULL, (void *) &ecatcheck, (void*) &ctime);
      osal_thread_create(&thread1, 128000, &ecatcheck, (void*) &ctime); // 创建一个监控线程，打印换行符和检测错误信息。
      /* start cyclic part */
      simpletest(argv[1]); // 开启测试脚本
   }
   else
   {
      printf("Usage: simple_test ifname1\nifname = eth0 for example\n");
   }

   printf("End program\n");
   return (0);
}

```

上面的主函数的主要执行部分其实是在 simpletest 中去实现的这一部分的代码如下：（注释在代码中）
``` c
/* 注意 : 这部分代码会看到很多变量第一次出现，我刚开始看得时候也是不知道那些是什么  
 * 现在大概就是明白 很多的变量是全局变量，可以作用在整个程序的运行空间之内，所以这些参数会在我们执行init  或者是 config 函数的时候被更新
*/
void simpletest(char *ifname)  // 这里还是需要网卡  我的是  eth1  主要是用于 初始化函数的
{
    int i, j, oloop, iloop, chk;
    //needlf = FALSE;
    inOP = FALSE;

   printf("Starting simple test\n");

   /* initialise SOEM, bind socket to ifname */
   if (ec_init(ifname))  //  初始化网卡，绑定端口等很多的操作 未深究
   {
      printf("ec_init on %s succeeded.\n",ifname);
      /* find and auto-config slaves */

       if ( ec_config_init(FALSE) > 0 )     // 配置从站
      {
         printf("%d slaves found and configured.\n",ec_slavecount);

         ec_config_map(&IOmap);
         // 配置 PDO 映射   这个 IOmap我的理解就是一个 内存池，SOEM 会将PDO 的指针 根据从站的 eeprom 内部的 PDO 参数
         // 将映射的地址 指向这一片的空间，实验测试中发现 ec_slave[0].outputs[0]的地址 就是指向了IOmap的起始地址
         // 所以将其称之为映射是非常合理的！
         ec_configdc();// 配置时钟, 还不是很清楚机理
         printf("Slaves mapped, state to SAFE_OP.\n");
         /* wait for all slaves to reach SAFE_OP state */
         ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4); // 切换第1个从站到 OP （operation）模式
         // 这段代码应该是初始化的一些操作
         oloop = ec_slave[0].Obytes;  // 从站1的输出的 长度
         if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;  // 不足一字节按照一字节来算 ？？
         if (oloop > 8) oloop = 8;
         
         iloop = ec_slave[0].Ibytes;
         if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
         if (iloop > 8) iloop = 8;

         printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

         printf("Request operational state for all slaves\n");
         expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
         printf("Calculated workcounter %d\n", expectedWKC);
         ec_slave[0].state = EC_STATE_OPERATIONAL;
         /* send one valid process data to make outputs in slaves happy*/
         ec_send_processdata(); // 发送PDO 数据
         ec_receive_processdata(EC_TIMEOUTRET); // 接收PDO数据
         /* request OP state for all slaves */
         ec_writestate(0); // 切换状态
         chk = 200;
         /* wait for all slaves to reach OP state */
         do
         {
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
         }
         while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
         // 等待第一个从站的状态切换到OPERATIONAL  或者最多尝试200 次超时
 
   // 这之下就是开始操作的代码了
         if (ec_slave[0].state == EC_STATE_OPERATIONAL )
         {
            printf("Operational state reached for all slaves.\n");
            inOP = TRUE;
                /* cyclic loop */
            for(i = 1; i <= 10000; i++)  // 循环执行 
            {
               ec_send_processdata(); // 发送PDO 数据
               wkc = ec_receive_processdata(EC_TIMEOUTRET); // 接收PDO数据
					// wkc 
					// LRD（读）	1
					// LWR（写）	1
					// LRW（读写）—读成功	1
					// LRW（读写）—写成功	2
					// LRW（读写）—全部完成	3
                    if(wkc >= expectedWKC) // 这句话的意思应该是 从站的操作完成了
                    {
                        printf("Processdata cycle %4d, WKC %d , O:", i, wkc);

                        for(j = 0 ; j < oloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].outputs + j));
                        }

                        printf(" I:");
                        for(j = 0 ; j < iloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].inputs + j));
                        }
                        printf(" T:%"PRId64"\r",ec_DCtime);
                        //needlf = TRUE;
                    }
                    osal_usleep(5000);
                }
                inOP = FALSE;
            }
            else
            {
                printf("Not all slaves reached operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL) // 看看哪个从站没达到状态
                    {
                        printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                            i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
            }
            printf("\nRequest init state for all slaves\n");
            ec_slave[0].state = EC_STATE_INIT;
            /* request INIT state for all slaves */
            ec_writestate(0); 
        }
        else
        {
            printf("No slaves found!\n");
        }
        printf("End simple test, close socket\n");
        /* stop SOEM, close socket */
        ec_close();
    }
    else
    {
        printf("No socket connection on %s\nExecute as root\n",ifname);
    }
}
```

关于上述的代码的 wkc 部分，我参考了 孔晨星 同志的博客

https://blog.csdn.net/kcx295810/article/details/113128954

    1

在这里插入图片描述

不是很懂这一部分。 以上就是官方的代码了

我自己的代码改动在这里， 主要的内容都在注释里面了
``` c
/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : simple_test [ifname1]
 * ifname is NIC interface, f.e. eth0
 *
 * This is a minimal test.
 *
 * (c)Arthur Ketels 2010 - 2011   
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "ethercat.h"
#define EC_TIMEOUTMON 500

char IOmap[4096];  //  个人理解 这里就是给提供了一块内存空间
OSAL_THREAD_HANDLE thread1;      // demo 里面的一个 检测线程
OSAL_THREAD_HANDLE thread_freash;// 笔者自己加的  每5ms 就去刷新一下pdo 数据
int expectedWKC;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;
// 状态监测线程，主要是监测ethcat 的异常状态？ 
// 这个线程笔者并没去深究，大概的用途就是去 定时的检测当前从站的在线状态，及时发现从站掉线
OSAL_THREAD_FUNC ecatcheck( void *ptr )
{
    int slave;
    (void)ptr;                  /* Not used */
    while(1)
    {
        if( inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {           
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();// 读取从站状态
            for (slave = 1; slave <= ec_slavecount; slave++) // 遍历
            {
               if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL)) 
               {
                  ec_group[currentgroup].docheckstate = TRUE;
                  if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                  {
                     printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                     ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                     ec_writestate(slave);  // 将指定的状态写到指定的从站之中
                  }
                  else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                  {
                     printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                     ec_slave[slave].state = EC_STATE_OPERATIONAL;
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state > EC_STATE_NONE)
                  {
                     if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d reconfigured\n",slave);
                     }
                  }
                  else if(!ec_slave[slave].islost)
                  {
                     /* re-check state */
                     ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                     if (ec_slave[slave].state == EC_STATE_NONE)
                     {
                        ec_slave[slave].islost = TRUE;
                        printf("ERROR : slave %d lost\n",slave);
                     }
                  }
               }
               if (ec_slave[slave].islost) // 检查从站是否 离线？
               {
                  if(ec_slave[slave].state == EC_STATE_NONE)
                  {
                     if (ec_recover_slave(slave, EC_TIMEOUTMON)) // 恢复和从站之间的连接
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d recovered\n",slave);
                     }
                  }
                  else
                  {
                     ec_slave[slave].islost = FALSE;
                     printf("MESSAGE : slave %d found\n",slave);
                  }
               }
            }
            if(!ec_group[currentgroup].docheckstate)
               printf("OK : all slaves resumed OPERATIONAL.\n");
        }
        osal_usleep(10000);
    }
}

// PDO 刷新线程，定时的去 收  发 PDO 数据。
OSAL_THREAD_FUNC ecatfreash( void *ptr )
{
   (void)ptr;         
   int i, oloop, iloop, chk;              /* Not used */
   inOP = FALSE;
   char *ifname = "eth1";
   if (ec_init(ifname))  // 初始化网卡1  基于野火的 i.mx6ull  这个是右面的那个网卡
   {
      printf("ec_init on %s succeeded.\n",ifname);
      /* find and auto-config slaves */

      if ( ec_config_init(FALSE) > 0 )  //  检查是否初始化完成了
      {
         printf("%d slaves found and configured.\n",ec_slavecount);

         ec_config_map(&IOmap); //根据 FMMU 的地址 完成数据的映射。

         ec_configdc();  //  配置时钟信息

         printf("Slaves mapped, state to SAFE_OP.\n");
         /* wait for all slaves to reach SAFE_OP state */
         ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);  // 检查当前的状态

         oloop = ec_slave[0].Obytes;
         if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
         if (oloop > 8) oloop = 8;
         iloop = ec_slave[0].Ibytes;
         if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
         if (iloop > 8) iloop = 8;

         printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

         printf("Request operational state for all slaves\n");
         expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
         printf("Calculated workcounter %d\n", expectedWKC);
         ec_slave[0].state = EC_STATE_OPERATIONAL;
         /* send one valid process data to make outputs in slaves happy*/
         ec_send_processdata();              
         ec_receive_processdata(EC_TIMEOUTRET);
         /* request OP state for all slaves */
         ec_writestate(0);
         chk = 200;
         /* wait for all slaves to reach OP state */
         do
         {
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
         }
         while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL)); //等待第一个从站进入到可以操作的状态 或者200个周期没进入 判定为超时
         if (ec_slave[0].state == EC_STATE_OPERATIONAL )
         {
            printf("Operational state reached for all slaves.\n");
            inOP = TRUE;
                /* cyclic loop */
            //for(i = 1; i <= 10000; i++)
            while(1)  // 笔者的改动之处 线程在这里进行无限的循环，每隔5ms 刷新一下pdo
            {
               ec_send_processdata();
               wkc = ec_receive_processdata(EC_TIMEOUTRET);  //
               osal_usleep(5000);                 
            }
               inOP = FALSE;
         }
         else
         {
               printf("Not all slaves reached operational state.\n");
               ec_readstate();
               for(i = 1; i<=ec_slavecount ; i++)
               {
                  if(ec_slave[i].state != EC_STATE_OPERATIONAL)
                  {
                     printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                           i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                  }
               }
         }
         printf("\nRequest init state for all slaves\n");
         ec_slave[0].state = EC_STATE_INIT;
         /* request INIT state for all slaves */
         ec_writestate(0);
      }
      else
      {
         printf("No slaves found!\n");
      }
   }
}

int main(int argc, char *argv[])
{  
   printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");
   if (argc > 1)
   {      
      /* create thread to handle slave error handling in OP */
      // pthread_create( &thread1, NULL, (void *) &ecatcheck, (void*) &ctime);
      osal_thread_create(&thread1, 128000, &ecatcheck, NULL);
      /* start cyclic part */
      // simpletest(argv[1]);      
      
      osal_thread_create(&thread_freash, 128000, &ecatfreash, NULL);
      // 这里需要等待一段时间，让刷新现成初始化好
      while( ec_slave[0].outputs == 0);  //  这个等待是必须的， 因为创建的子线程  ecatfreash  需要一段时间才能完成初始化工作
      while(1)
      {
         ec_slave[0].outputs[0] = ec_slave[0].inputs[0]; // 将第一个从站的 输入 等于输出
         // 显示的效果上面就是  我按下按键之后 从站上的灯就会亮起
         // 测试使用的从站模块是只有8个LED 和 8个 按键的模块。
         osal_usleep(5000);
      }              
   }else
   {  
      printf("Usage: simple_test ifname1\nifname = eth0 for example\n");
   }      
   ec_close();       
   printf("End program %s\n",argv[0]);
   return (0);
}

```

5.测试效果

运行的效果就是按下板载的按键 S5 对应的D5 亮起

其余的按键也是一样的 S1 按下 D1 亮起
————————————————
版权声明：本文为CSDN博主「Coder_BCM」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_41534481/article/details/113486446






















