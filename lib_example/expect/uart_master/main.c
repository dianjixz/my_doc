#include <stdio.h> /*标准输入输出定义*/
#include <stdlib.h> /*标准函数库定义*/
#include <unistd.h> /*Unix 标准函数定义*/
#include <sys/types.h> /*数据类型，比如一些XXX_t的那种*/
#include <sys/stat.h> /*定义了一些返回值的结构，没看明白*/
#include <fcntl.h> /*文件控制定义*/
#include <termios.h> /*PPSIX 终端控制定义*/
#include <errno.h> /*错误号定义*/
#include "linux_uart.h"
char uart_one[20];

uart_t uart_parm = {
    .baud = 115200,
    .data_bits = 8,
    .stop_bits = 1,
    .parity = 'n'
};
static void help_p()
{
    printf("\n--------Welcome to debugging help--------------\n");
    printf("-h:\t\tDisplay the Help page\n");
    printf("-d:\t\tEnable debugging log display\n");
    printf("\n-----------------------------------------------\n");
}
int debugging ;
int main(int argc,char *argv[])
{
    int uart1;
    struct timeval timeout;
    char data;
    fd_set readfd;

    debugging = 0;

    FD_ZERO(&readfd); /* 清空文件描述符集合 */
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    int opt;
    if (1 != argc)
    {
        while ((opt = getopt(argc, argv, "hdf:")) != -1)
        {
            switch (opt)
            {
            case 'h':
                help_p();
                return 0;
                break;
            case 'd':
                debugging = 1;
                break;
            case 'f':
                sprintf(uart_one,"%s", optarg);
                break;
            case '?':
                printf("Unknown option: %c\n",(char)optarg);  
                break;
            default:
                break;
            }
        }
    }
    else
    {
        help_p();
        return 1;
    }

    if(optind < argc)
    {
        sprintf(uart_one,"%s", argv[optind]);
    }

    uart1 = linux_uart_init(uart_one, &uart_parm);

    if(uart1 < 0)
    {
        printf("%s open fail!\n", uart_one);
        return 2;
    }


    char test_data[10] = "nihaotest";
    write(uart1, test_data, 9);
    sleep(1);
    char reace_data[10];

    read(uart1,reace_data, 10);

    reace_data[9] = '\0';
    if(strcmp(test_data, reace_data) == 0)
    {
        linux_uart_deinit(uart1);
        printf ("%s test success!\n",uart_one);
        return 0;
    }
    else
    {
        printf ("%s test fail!\n",uart_one);
        linux_uart_deinit(uart1);
        return 4;
    }

    // for(;;){
        
    //     FD_SET(uart1, &readfd);
    //     select(uart1 + 1, &readfd, NULL, NULL, &timeout);
    //     if (FD_ISSET(uart1, &readfd)) /* 测试fd_key是否在描述符集合中 */
    //     {
    //         if(linux_uart_read(uart1, 1, &data)){
    //             if(debugging){
    //                 printf("ttyS2 reace:%c  \n", data);
    //             }
    //             linux_uart_write(uart1, 1, &data);
    //         }
    //     }
    // }

    return 0;
}