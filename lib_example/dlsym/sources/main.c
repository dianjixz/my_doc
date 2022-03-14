#include <stdio.h>
#include <dlfcn.h>
int main()
{
    int m,n;
    //打开库文件
    void* handler = dlopen("./libmymath.so",RTLD_LAZY);
    if(dlerror() != NULL){
        printf("%s",dlerror());
    }
   
    //获取库文件中的 add() 函数
    int(*add)(int,int)=dlsym(handler,"add");
    if(dlerror()!=NULL){
        printf("%s",dlerror());
    }
  
    //获取库文件中的 sub() 函数
    int(*sub)(int,int)=dlsym(handler,"sub");
    if(dlerror()!=NULL){
        printf("%s",dlerror());
    }

    //获取库文件中的 div() 函数
    int(*div)(int,int)=dlsym(handler,"div");
    if(dlerror()!=NULL){
        printf("%s",dlerror());
    }
    //使用库文件中的函数实现相关功能
    printf("Input two numbers: ");
    scanf("%d %d", &m, &n);
    printf("%d+%d=%d\n", m, n, add(m, n));
    printf("%d-%d=%d\n", m, n, sub(m, n));
    printf("%d÷%d=%d\n", m, n, div(m, n));
    //关闭库文件
    dlclose(handler);
    return 0;
}