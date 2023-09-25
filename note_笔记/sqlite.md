# SQLite 数据库使用

遇到的一个项目，期望能够记录一些数据，原本是想使用简单的日志去储存。但是日志没有索引功能，后面如果需要查寻的话，还要手动分辨数据，比较麻烦。所以考虑使用数据库的形式来保存数据。





此次使用 python3 对 SQLite3 数据库进行读写。
```python
# 数据库创建写
try:
    conn = sqlite3.connect(self.args.sqlite_path)
    cursor = conn.cursor()
    cursor.execute('''CREATE TABLE IF NOT EXISTS server_task
                    (time TEXT, id TEXT, uuid TEXT, datetime INTEGER, email TEXT, url TEXT, startDatetime INTEGER, status TEXT)''')
    beijing_timezone = pytz.timezone('Asia/Shanghai')
    current_datetime = datetime.now(beijing_timezone)
    formatted_date = current_datetime.strftime('%Y-%m-%d %H:%M:%S.%f')
    cursor.execute('''INSERT INTO server_task (time, id, uuid, datetime, email, url, startDatetime, status) VALUES (?, ?, ?, ?, ?, ?, ?, ?)''',
                (formatted_date, userid, jsonRequest['uuid'], jsonRequest['datetime'], jsonRequest['email'], jsonRequest['url'], jsonRequest['startDatetime'], 'get task info'))
    conn.commit()
    conn.close()
except:
    self.logger.error(f'sqlite3 INSERT {self.args.sqlite_path} fail!')
```

```python
# 数据库创建修改
try:
    if os.path.exists(self.server_args.sqlite_path):
        conn = sqlite3.connect(self.server_args.sqlite_path)
        cursor = conn.cursor()
        cursor.execute('''UPDATE server_task
                        SET status = ?
                        WHERE uuid = ?''',
                    (task_info['error_msg'], task_info['uuid']))
        conn.commit()
        conn.close()
except:
    pass

```

Sqlite大批量导入数据提高效率的几种方式
https://blog.csdn.net/wangjunhe/article/details/117767941

```c
#include <iostream>  
#include <string>  
#include <sstream>  
#include <time.h>  
#include "sqlite3.h"  
  
const int nCount = 500000;  
      
int main (int argc,char** argv)  
{  
    sqlite3* db;  
    sqlite3_open("testdb.db" ,&db);  
    sqlite3_exec(db,"PRAGMA synchronous = OFF; ",0,0,0);  
    sqlite3_exec(db,"drop table if exists t1",0,0,0);  
    sqlite3_exec(db,"create table t1(id integer,x integer,y integer ,weight real)",0,0,0);  
    clock_t t1 = clock();  
      
    sqlite3_exec(db,"begin;",0,0,0);  
    sqlite3_stmt *stmt;  
    const char* sql = "insert into t1 values(?,?,?,?)";  
    sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,0);  
      
    for(int i=0;i<nCount;++i)  
    {  
        // std::stringstream ssm;  
        // ssm<<"insert into t1 values("<<i<<","<<i*2<<","<<i/2<<","<<i*i<<")";  
        // sqlite3_exec(db,ssm.str().c_str(),0,0,0);  
        sqlite3_reset(stmt);  
        sqlite3_bind_int(stmt,1,i);  
        sqlite3_bind_int(stmt,2,i*2);  
        sqlite3_bind_int(stmt,3,i/2);  
        sqlite3_bind_double(stmt,4,i*i);  
        sqlite3_step(stmt);  
    }  
    sqlite3_finalize(stmt);  
    sqlite3_exec(db,"commit;",0,0,0);  
    clock_t t2 = clock();  
      
    sqlite3_close(db);  
      
    std::cout<<"cost tima: "<<(t2-t1)/1000.<<"s"<<std::endl;  
      
    return 0;  
}  
```









sqlite3数据库小白入门（Linux）
https://zhuanlan.zhihu.com/p/130981843