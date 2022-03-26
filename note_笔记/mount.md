是因为有进程占用/home，可以用下面的命令来停止占用的进程，再umount。 fuser -km /home/

~~~
fuser -km /home/
~~~
