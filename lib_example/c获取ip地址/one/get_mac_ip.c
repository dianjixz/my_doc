#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/if.h>
#include <string.h>
#include "get_mac.h"

void ip_list_init(ip_list_t *ptr, int data_len)
{
    ptr->head_ptr = NULL;
    ptr->tail_ptr = NULL;
    ptr->size = 0;
    ptr->data_len = data_len;
}

int ip_list_size(ip_list_t *ptr)
{
    return ptr->size;
}
void ip_list_push_back(ip_list_t *ptr, void *data)
{
    ip_list_lnk_t *tmp = (ip_list_lnk_t *) malloc(sizeof(ip_list_lnk_t) + ptr->data_len);
    memcpy(tmp->data, data, ptr->data_len);

    if (ptr->size++) {
        tmp->next_ptr = NULL;
        tmp->prev_ptr = ptr->tail_ptr;
        ptr->tail_ptr->next_ptr = tmp;
        ptr->tail_ptr = tmp;
    } else {
        tmp->next_ptr = NULL;
        tmp->prev_ptr = NULL;
        ptr->head_ptr = tmp;
        ptr->tail_ptr = tmp;
    }
}

void ip_list_pop_back(ip_list_t *ptr, void *data)
{
    ip_list_lnk_t *tmp = ptr->tail_ptr;
    if (data) {
        memcpy(data, tmp->data, ptr->data_len);
    }
    if(ptr->size != 1)
    {
        tmp->prev_ptr->next_ptr = NULL;
    }
    ptr->tail_ptr = tmp->prev_ptr;
    ptr->size -= 1;
    free(tmp);
}



// 获取地址
// 返回IP地址字符串
int getlocalip(ip_list_t *out)
{
    int i=0;
    int sockfd;
    struct ifconf ifconf;
    unsigned char buf[512];
    struct ifreq *ifreq;
    //初始化ifconf
    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
    ip_list_init(out, 100);
    char tmps[100];
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        perror("socket" );
        return -1;
    }
    ioctl(sockfd, SIOCGIFCONF, &ifconf); //获取所有接口信息
    //接下来一个一个的获取IP地址
    ifreq = (struct ifreq*)buf;
    for (i=(ifconf.ifc_len/sizeof (struct ifreq)); i>0; i--)
    {
        sprintf(tmps, "%s %s", ifreq->ifr_name, inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
        ip_list_push_back(out, tmps);
        // printf("name = [%s]\n" , ifreq->ifr_name);
        // printf("local addr = [%s]\n" ,inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
        ifreq++;
    }
    return 0;
}




/* use by test */
// #define DEBUG
#ifdef DEBUG
// ——————————-函数的调用方式————————————-
int main(int argc, char **argv)
{
    ip_list_t out;
    char tmps[100];
    getlocalip(&out);
    while (ip_list_size(&out))
    {
        printf("size:%d\r\n", ip_list_size(&out));
        ip_list_pop_back(&out, tmps);
        printf("%s\r\n", tmps);
    }
    return 0;
}
#endif


