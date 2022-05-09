#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>                   /*标准输入输出定义*/
#include <stdlib.h>                  /*标准函数库定义*/
#include <unistd.h>                  /*Unix 标准函数定义*/
#include <sys/types.h>               /*数据类型，比如一些XXX_t的那种*/
#include <sys/stat.h>                /*定义了一些返回值的结构，没看明白*/
#include <fcntl.h>                   /*文件控制定义*/
#include <termios.h>                 /*PPSIX 终端控制定义*/
#include <errno.h>                   /*错误号定义*/
#define DEV_PATH "/dev/input/event0" //difference is possible
void signal_handle(int signal_num)
{
    printf("fun exit!\r\n");
    exit(0);
}
int keys_fd;
fd_set readfd;
struct timeval timeout;
char ret[2];
struct input_event t;

int key_p()
{
    
}

int main()
{

    signal(SIGINT, signal_handle);
    keys_fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);    
    if (keys_fd <= 0)
    {
        printf("open /dev/input/event2 device error!\n");
        return -1;
    }
    // FD_ZERO(&readfd);         /* 清空文件描述符集合 */
    // FD_SET(keys_fd, &readfd); /* 添加文件描述符到集合 */
    // timeout.tv_sec = 1;
    // timeout.tv_usec = 10;
    int mk;
    while (1)
    {
        // printf("rese:%d\n",FD_ISSET(keys_fd, &readfd));
        // printf("select: %d\n", md);
        // if (FD_ISSET(keys_fd, &readfd)) /* 测试fd_key是否在描述符集合中 */
        // {
            // printf("select!\n");
            if (read(keys_fd, &t, sizeof(t)) == sizeof(t))
            {
                // printf("keys_fd:%d\n",t.value);
                // return t.value;
                if ((t.type == EV_KEY) && (t.value == 0 || t.value == 1))
                    {
                        printf("key %d %s\n", t.code, (t.value) ? "Pressed" : "Released");
                        // if (t.code == KEY_ESC)
                            // return1;
                    }
            }
            // else
            // {
                // printf("read faile!\n");
            // }
        // }
        // usleep(1000 * 100);
    }
    close(keys_fd);
    return 0;
}