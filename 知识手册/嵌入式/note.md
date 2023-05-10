stm32 使用gcc把函数放入RAM

又想 data段自动被启动文件搬运，何不函数放到data段，写下
attribute ((section(".data"))) void test(void){
char x[]=“12345678”;
GPIOA->BRR=1;
}