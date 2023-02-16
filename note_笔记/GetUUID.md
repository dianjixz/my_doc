Windows获取系统唯一标识UUID  (也叫CSP UUID)

命令行

~~~
wmic csproduct get uuid
~~~

c/c++

使用CoCreateGuid函数，可以参考官方：https://docs.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateguid

linux获取系统唯一标识UUID  (也叫CSP UUID)

Linux内核提供有UUID生成接口：

~~~
cat /proc/sys/kernel/random/uuid
~~~
