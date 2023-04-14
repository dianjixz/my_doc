
#ifndef _get_mac_H_
#define _get_mac_H_
#if __cplusplus
extern "C"
{
#endif

typedef struct ip_list_lnk
{
    struct ip_list_lnk *next_ptr, *prev_ptr;
    char data[];
}
ip_list_lnk_t;

typedef struct ip_list
{
    ip_list_lnk_t *head_ptr, *tail_ptr;
    int size, data_len;
}
ip_list_t;


// void ip_list_init(ip_list_t *ptr, int data_len);
int  ip_list_size(ip_list_t *ptr);
void ip_list_push_back(ip_list_t *ptr, void *data);
void ip_list_pop_back(ip_list_t *ptr, void *data);


int getlocalip(ip_list_t *out);


#if __cplusplus
}
#endif
#endif
