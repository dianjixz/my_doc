#include <time.h>
#include <stdio.h>
int main()
{
    time_t tt = time(NULL);
    struct tm* t= localtime(&tt);
    printf("%d-%02d-%02d %02d:%02d:%02d\n",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec);
}



// struct tm nowtime;
// struct timeval tv;
// unsigned char time_now[128];
// gettimeofday(&tv, NULL);
// localtime_r(&tv.tv_sec,&nowtime);

// sprintf(time_now,"%d-%d-%d %d:%d:%d.%03d ",
// 	nowtime.tm_year+1900,
// 	nowtime.tm_mon+1,
// 	nowtime.tm_mday,
// 	nowtime.tm_hour,
// 	nowtime.tm_min,
// 	nowtime.tm_sec,
// 	(int)(tv.tv_usec/1000)
// );
// printf("current time is %s\n",time_now);
