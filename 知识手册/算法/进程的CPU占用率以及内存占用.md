最近做监控相关东西的时候，需要获取某一进程CPU以及内存使用情况，就简单的写了一下，代码具体如下：


``` c++
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#define VMRSS_LINE 17
#define VMSIZE_LINE 13
#define PROCESS_ITEM 14

typedef struct {
	unsigned long user;
	unsigned long nice;
	unsigned long system;
	unsigned long idle;
}Total_Cpu_Occupy_t;


typedef struct {
	unsigned int pid;
	unsigned long utime;  //user time
	unsigned long stime;  //kernel time
	unsigned long cutime; //all user time
        unsigned long cstime; //all dead time
}Proc_Cpu_Occupy_t;


//获取第N项开始的指针
const char* get_items(const char*buffer ,unsigned int item){
	
	const char *p =buffer;

	int len = strlen(buffer);
	int count = 0;
	
	for (int i=0; i<len;i++){
		if (' ' == *p){
			count ++;
			if(count == item -1){
				p++;
				break;
			}
		}
		p++;
	}

	return p;
}


//获取总的CPU时间
unsigned long get_cpu_total_occupy(){
	
	FILE *fd;
	char buff[1024]={0};
	Total_Cpu_Occupy_t t;

	fd =fopen("/proc/stat","r");
	if (nullptr == fd){
		return 0;
	}
		
	fgets(buff,sizeof(buff),fd);
	char name[64]={0};
	sscanf(buff,"%s %ld %ld %ld %ld",name,&t.user,&t.nice,&t.system,&t.idle);
	fclose(fd);
	
	return (t.user + t.nice + t.system + t.idle);
}


//获取进程的CPU时间
unsigned long get_cpu_proc_occupy(unsigned int pid){
	
	char file_name[64]={0};
	Proc_Cpu_Occupy_t t;
	FILE *fd;
	char line_buff[1024]={0};
	sprintf(file_name,"/proc/%d/stat",pid);
	
	fd = fopen(file_name,"r");
	if(nullptr == fd){
		return 0;
	}
	
	fgets(line_buff,sizeof(line_buff),fd);
	
	sscanf(line_buff,"%u",&t.pid);
	const char *q =get_items(line_buff,PROCESS_ITEM);
	sscanf(q,"%ld %ld %ld %ld",&t.utime,&t.stime,&t.cutime,&t.cstime);
	fclose(fd);
	
	return (t.utime + t.stime + t.cutime + t.cstime);
}


//获取CPU占用率
float get_proc_cpu(unsigned int pid){
 	
	unsigned long totalcputime1,totalcputime2;
	unsigned long procputime1,procputime2;
	
	totalcputime1=get_cpu_total_occupy();
	procputime1=get_cpu_proc_occupy(pid);

	usleep(200000);

	totalcputime2=get_cpu_total_occupy();
	procputime2=get_cpu_proc_occupy(pid);
	
	float pcpu = 0.0;
	if(0 != totalcputime2-totalcputime1){ 
		pcpu=100.0 * (procputime2-procputime1)/(totalcputime2-totalcputime1);
	}
	
	return pcpu;
}


//获取进程占用内存
unsigned int get_proc_mem(unsigned int pid){
	
	char file_name[64]={0};
	FILE *fd;
	char line_buff[512]={0};
	sprintf(file_name,"/proc/%d/status",pid);
	
	fd =fopen(file_name,"r");
	if(nullptr == fd){
		return 0;
	}
	
	char name[64];
	int vmrss;
	for (int i=0; i<VMRSS_LINE-1;i++){
		fgets(line_buff,sizeof(line_buff),fd);
	}
	
	fgets(line_buff,sizeof(line_buff),fd);
	sscanf(line_buff,"%s %d",name,&vmrss);
	fclose(fd);

	return vmrss;
}


//获取进程占用虚拟内存
unsigned int get_proc_virtualmem(unsigned int pid){
	
	char file_name[64]={0};
	FILE *fd;
	char line_buff[512]={0};
	sprintf(file_name,"/proc/%d/status",pid);
	
	fd =fopen(file_name,"r");
	if(nullptr == fd){
		return 0;
	}
	
	char name[64];
	int vmsize;
	for (int i=0; i<VMSIZE_LINE-1;i++){
		fgets(line_buff,sizeof(line_buff),fd);
	}
	
	fgets(line_buff,sizeof(line_buff),fd);
	sscanf(line_buff,"%s %d",name,&vmsize);
	fclose(fd);

	return vmsize;
}


//进程本身
int get_pid(const char* process_name, const char* user = nullptr)
{
	if(user == nullptr){
		user = getlogin();	
	}
	
	char cmd[512];
	if (user){
		sprintf(cmd, "pgrep %s -u %s", process_name, user);	
	}

	FILE *pstr = popen(cmd,"r");	
	
	if(pstr == nullptr){
		return 0;	
	}

	char buff[512];
	::memset(buff, 0, sizeof(buff));
	if(NULL == fgets(buff, 512, pstr)){
		return 0;
	}

	return atoi(buff);
}


int main(int argc, char *argv[])
{
	
	if(argc < 2){
		printf("Usage:test <process_name> [user]\n");
		return 1;
	}
	
	unsigned int pid=0;
	
	if(argc > 2){
		pid = get_pid(argv[1],argv[2]);
	}
	else{
		pid = get_pid(argv[1]);
	}
	
	printf("pid=%d\n",pid);
	printf("pcpu=%f\n",get_proc_cpu(pid));
	printf("procmem=%d\n",get_proc_mem(pid));
	printf("virtualmem=%d\n",get_proc_virtualmem(pid));	
	return 0;
}

```