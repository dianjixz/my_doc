glibc 下载网站: https://ftp.gnu.org/pub/gnu/glibc


../configure --host=arm-hisiv200-linux-gnueabi CC=arm-hisiv200-linux-gcc CFLAGS="-g -O2 -U_FORTIFY_SOURCE" --enable-add-ons  --prefix=/root/temp/lijiancheng/henan/glibc-2.11.1/build_hi3716m/installdir/ libc_cv_forced_unwind=yes libc_cv_c_cleanup=yes --enable-kernel=2.6.0
————————————————
版权声明：本文为CSDN博主「jianchenglee」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/jianchenglee/article/details/11724363