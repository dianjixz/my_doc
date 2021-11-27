 

最近在网上买了个树莓派4B玩玩，很早就听说树莓派操作GPIO非常方便，于是就想来研究下

升级wringPi
树莓派本身有一个gpio的命令，貌似发现这个命令使用起来有点问题，原因是我使用的是树莓派4B， 而我的wiringPi版本太久了需要升级，官方有一个教程指导如何升级：http://wiringpi.com/wiringpi-updated-to-2-52-for-the-raspberry-pi-4b/

$ cd /tmp
$ wget https://project-downloads.drogon.net/wiringpi-latest.deb
$ sudo dpkg -i wiringpi-latest.deb
升级完成之后，输入命令读取树莓派所有GPIO的状态

$ gpio readall
+-----+-----+---------+------+---+---Pi 4B--+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |  OUT | 0 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | IN   | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | OUT  | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 1 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 0 | OUT  | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 4B--+---+------+---------+-----+-----+
使用echo命令操作GPIO
现在我们对某个GPIO进行操作，网上有很多博客都介绍用echo命令来配置GPIO，下面以GPIO.1为例子，由上图可以看到GPIO1对应的wPi编码为1，BCM编码为18，而使用echo命令的时候默认使用的是BCM编码，下面开始配置

$ sudo echo 18 > /sys/class/gpio/export  # 把GPIO18暴露到用户空间
$ cd /sys/class/gpio/gpio18 # 上面那条命令会创建/sys/class/gpio/gpio18这个目录
$ sudo echo out > direction  # 把GPIO配置成输出模式
$ sudo echo 0 > value # 把GPIO拉低
$ sudo echo 1 > value # 把GPIO拉高
注意：上面的GPIO18采用的是BCM编码，它对应的是名字为GPIO.1的PIN脚

注销接口的命令：

$ cd ~
$ sudo echo 18 > /sys/class/gpio/unexport
这种方式个人觉得有点麻烦，需要敲好多个命令才行，其实还有一种更加简单的方式，那就是使用gpio命令

使用gpio命令操作PIN脚
我们还是对GPIO.1这个PIN脚进行操作，需要注意的是，gpio命令默认使用的是wPi的编码方式，也即是说GPIO.1这个PIN脚的编号变成了1

下面我把这个PIN脚设置为输出模式，并对其拉高拉低

$ gpio mode 1 out # 把GPIO.1 配置为输出模式
$ gpio write 1 1 # 把GPIO.1 拉高
$ gpio write 1 0 # 把GPIO.1 拉低
$ gpio mode 1 in # 把GPIO.1 配置为输入模式
是不是很简单多了
————————————————
版权声明：本文为CSDN博主「依然微笑100」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u010764600/article/details/119708630
