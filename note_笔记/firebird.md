# firebird
firebird是一个全功能的，强大高效的，轻量级，免维护的数据库。它很容易让您从单用户，单数据库升级到企业级的应用。  
一个firebird数据库服务器能够管理多个独立的数据库，每一个数据库同时可支持多个客户端连结。总之：它是一个开源的，强大在，可以自由使用的数据库（即使是商业上的使用）
特点：  
1、Firebird是一个真正的关系数据库，支持存储过程、视图、触发器、事务等大型关系数据库的所有特性；  
2、Firebird支持SQL92的绝大部分命令，并且支持大部分SQL99命令，新版Firebird 2.0对SQL99的支持更完整；  
3、Firebird源码基于成熟的商业数据库Interbase，有良好的稳定性，与Interbase有良好的兼容性；  
4、不用考虑授权费用（免费），不用当心将来有一天你或你的客户因为使用盗版而被数据库开发商告上法庭；  
5、发布简易，安装文件只有几M，且高度可定制，客户端的分发也很简单，只需一个DLL文件；  
6、Firebird的一嵌入式服务器版本，不用安装，直接运行，基于单机开发首选；  
7、Firebird的运行效率非常高；  
8、具备高度可移植性，可在Linux,Unix,MacOS,Windows,Solaris系统下运行，而且数据库格式完全一样，不用修改；  
9、开发环境支持良好，Delphi,C++Builder不用通过ODBC连接，直接用原生开发接口开发基于Firebird的程序。  


## 安装
firebird 的官网会提供已经编译好的 firebird 数据库二进制文件，每个平台都有，只需要到 [firebird](https://www.firebirdsql.org/) 获取对应平台的二进制文件安装包即可。
arm 上的安装：
```bash
wget https://github.com/FirebirdSQL/firebird/releases/download/v5.0.0/Firebird-5.0.0.1306-0-linux-arm32.tar.gz
tar zxvf Firebird-5.0.0.1306-0-linux-arm32.tar.gz
cd Firebird-5.0.0.1306-0-linux-arm32
sudo ./install
```
即可完成安装。

## 本地使用
在本地可以使用 /opt/firebird/bin/isql 进行管理。  
创建一个数据库：
```bash
cd /opt/firebird/bin
./isql
SQL> CREATE DATABASE '/opt/firebird/test_database.fdb' USER 'test' PASSWORD 'test';
SQL> quit;
```

连接数据库进行增删改查：
```bash
cd /opt/firebird/bin
./isql /opt/firebird/test_database.fdb -u test -p test
# 查看所有的表
SQL> SHOW TABLES;
# 如果 test_table 表不存在，则创建
SQL> SET TERM !! ; EXECUTE BLOCK AS BEGIN IF (NOT EXISTS (SELECT 1 FROM rdb$relations WHERE rdb$relation_name = 'TEST_TABLE')) THEN BEGIN EXECUTE STATEMENT 'CREATE TABLE test_table (id INTEGER NOT NULL PRIMARY KEY, name VARCHAR(100))'; END END!! SET TERM ; !!
SQL> COMMIT;
# 插入两条数据
SQL> INSERT INTO test_table (id, name) VALUES (1, 'John Doe');
SQL> INSERT INTO test_table (id, name) VALUES (2, 'Jane Doe');
# 提交本次操作
SQL> COMMIT;
# 查看数据库中的数据
SQL> SELECT * FROM test_table;
# 更改数据库中的值
SQL> UPDATE test_table SET name = 'John Smith' WHERE id = 1;
SQL> COMMIT;
SQL> SELECT * FROM test_table;
# 删除数据库中的值
SQL> DELETE FROM test_table WHERE id = 2;
SQL> COMMIT;
SQL> SELECT * FROM test_table;
# 删除表
# SQL> COMMIT;
# SQL> DROP TABLE test_table;
# SQL> COMMIT;
# 退出
SQL> QUIT;

```


## 远程使用
远程使用时需要创建帐户：
```bash
cd /opt/firebird/bin
./gsec -user sysdba -password masterkey -add newuser -pw newpassword
./gsec -user sysdba -password masterkey -modify existinguser -pw newpassword
./gsec -user sysdba -password masterkey -delete username
./gsec -user sysdba -password masterkey -display
```
- sysdba 是管理员用户名。
- masterkey 是管理员密码。
- newuser 是要添加的新用户名。
- newpassword 是新用户的密码
- existinguser 是更改的用户
- username 是需要删除的用户

安装依赖：
```bash
sudo apt install libtommath1
wget https://github.com/FirebirdSQL/firebird/releases/download/v5.0.0/Firebird-5.0.0.1306-0-linux-x64.tar.gz
tar zxvf Firebird-5.0.0.1306-0-linux-x64.tar.gz
cd Firebird-5.0.0.1306-0-linux-x64
tar zxvf buildroot.tar.gz
export LD_LIBRARY_PATH=`pwd`/buildroot/opt/firebird/lib:$LD_LIBRARY_PATH
sudo apt install python3 python3-pip
pip3 install fdb
```
### python 使用
```python
import fdb

# 连接到数据库
con = fdb.connect(
    host='127.0.0.1',
    database='/opt/firebird/test_database.fdb',  # 修改为你的数据库文件路径
    user='test',  # 修改为你的用户名
    password='test'  # 修改为你的密码
)

# 创建游标
cur = con.cursor()

# 创建表
cur.execute('''
SET TERM !! ;
EXECUTE BLOCK AS
BEGIN
  IF (NOT EXISTS (
    SELECT 1 FROM rdb$relations 
    WHERE rdb$relation_name = 'TEST_TABLE'
  )) THEN
  BEGIN
    EXECUTE STATEMENT '
      CREATE TABLE test_table (
        id INTEGER NOT NULL PRIMARY KEY,
        name VARCHAR(100)
      )
    ';
  END
END!!
SET TERM ; !!
''')
con.commit()

# # 插入数据
cur.execute("INSERT INTO test_table (id, name) VALUES (1, 'John Doe')")
cur.execute("INSERT INTO test_table (id, name) VALUES (2, 'Jane Doe')")
con.commit()

# 查询数据
cur.execute("SELECT * FROM test_table")
for row in cur.fetchall():
    print(row)

# # 更新数据
cur.execute("UPDATE test_table SET name = 'John Smith' WHERE id = 1")
con.commit()

# # 删除数据
# cur.execute("DELETE FROM test_table WHERE id = 2")
# con.commit()

# 关闭连接
cur.close()
con.close()
```



### C++ 源生使用

需要连接到 libtomath.so 库

```c
/*
* SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
*
* SPDX-License-Identifier: MIT
*/

#include <ibase.h>
#include <cstring>
#include <iostream>
#include <string>

// Firebird API error handling function
void check_isc_status(ISC_STATUS *status) {
    if (status[0] == 1 && status[1]) {
        std::cerr << "Firebird error code: " << status[1] << std::endl;
        isc_print_status(status);
        // char err_msg[512];
        // fb_interpret(err_msg, sizeof(err_msg), &status);
        // std::cerr << "Firebird error message: " << err_msg << std::endl;
        exit(1);
    }
}

void execute_immediate(isc_db_handle &db, isc_tr_handle &trans,
                       const char *sql) {
    ISC_STATUS status[20];
    if (isc_dsql_execute_immediate(status, &db, &trans, 0, sql, SQL_DIALECT_V6,
                                   NULL)) {
        check_isc_status(status);
    }
}

void query_table(isc_db_handle &db, isc_tr_handle &trans, const char *sql) {
    ISC_STATUS status[20];
    isc_stmt_handle stmt = 0;  // NULL
    XSQLDA *sqlda        = (XSQLDA *)malloc(XSQLDA_LENGTH(1));
    sqlda->version       = SQLDA_VERSION1;
    sqlda->sqln          = 1;
    sqlda->sqld          = 0;

    if (isc_dsql_allocate_statement(status, &db, &stmt)) {
        check_isc_status(status);
    }

    if (isc_dsql_prepare(status, &trans, &stmt, 0, sql, SQL_DIALECT_V6,
                         sqlda)) {
        check_isc_status(status);
    }

    if (sqlda->sqld > sqlda->sqln) {
        sqlda       = (XSQLDA *)realloc(sqlda, XSQLDA_LENGTH(sqlda->sqld));
        sqlda->sqln = sqlda->sqld;
        if (isc_dsql_describe(status, &stmt, SQL_DIALECT_V6, sqlda)) {
            check_isc_status(status);
        }
    }

    for (int i = 0; i < sqlda->sqld; i++) {
        sqlda->sqlvar[i].sqldata = (char *)malloc(sqlda->sqlvar[i].sqllen);
        sqlda->sqlvar[i].sqlind  = (short *)malloc(sizeof(short));
    }

    if (isc_dsql_execute(status, &trans, &stmt, SQL_DIALECT_V6, NULL)) {
        check_isc_status(status);
    }

    while (isc_dsql_fetch(status, &stmt, SQL_DIALECT_V6, sqlda) == 0) {
        for (int i = 0; i < sqlda->sqld; i++) {
            // XSQLVAR &var = sqlda->sqlvar[i];
            if (sqlda->sqlvar[i].sqltype == 449) {
                if (*sqlda->sqlvar[i].sqlind < 0) {
                    std::cout << "NULL ";
                } else {
                    std::cout << std::string(sqlda->sqlvar[i].sqldata,
                                             sqlda->sqlvar[i].sqllen)
                              << " ";
                }
            } else if (sqlda->sqlvar[i].sqltype == SQL_LONG) {
                    ISC_LONG data;
                    memcpy(&data, sqlda->sqlvar[i].sqldata, sizeof(ISC_LONG));
                    std::cout << data << " ";
            } else {
                std::cout << "sqltype: " << sqlda->sqlvar[i].sqltype;
            }
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < sqlda->sqld; i++) {
        free(sqlda->sqlvar[i].sqldata);
        free(sqlda->sqlvar[i].sqlind);
    }
    free(sqlda);

    if (isc_dsql_free_statement(status, &stmt, DSQL_drop)) {
        check_isc_status(status);
    }
}
int buildDpb(const char *user, const char *password, char *dpb_buffer) {
    char *dpb = dpb_buffer;

    *dpb++ = isc_dpb_version1;
    *dpb++ = isc_dpb_user_name;
    *dpb++ = strlen(user);
    strcpy(dpb, user);
    dpb += strlen(user);
    *dpb++ = isc_dpb_password;
    *dpb++ = strlen(password);
    strcpy(dpb, password);
    dpb += strlen(password);
    return dpb - dpb_buffer;
}

int main() {
    ISC_STATUS status[20];
    isc_db_handle db     = 0;
    isc_tr_handle trans  = 0;
    const char *db_name  = "127.0.0.1:/opt/firebird/test_database.fdb";
    const char *user     = "test";
    const char *password = "test";
    char dpb_buffer[256];
    int dpb_buffer_len = buildDpb(user, password, dpb_buffer);

    if (isc_attach_database(status, 0, db_name, &db, dpb_buffer_len,
                            dpb_buffer)) {
        check_isc_status(status);
    }

    if (isc_start_transaction(status, &trans, 1, &db, 0, NULL)) {
        check_isc_status(status);
    }

    // Create table
    execute_immediate(
        db, trans,
        "EXECUTE BLOCK AS BEGIN IF (NOT EXISTS (SELECT 1 FROM rdb$relations "
        "WHERE rdb$relation_name = 'TEST1_TABLE')) THEN BEGIN EXECUTE "
        "STATEMENT 'CREATE TABLE TEST1_TABLE (id INTEGER NOT NULL, name "
        "VARCHAR(50), PRIMARY KEY (id))'; END END");

    if (isc_commit_transaction(status, &trans) ||
        isc_start_transaction(status, &trans, 1, &db, 0, NULL)) {
        check_isc_status(status);
    }

    // Insert data

    execute_immediate(db, trans,
                      "INSERT INTO TEST1_TABLE (id, name) VALUES (1, 'Alice')");
    execute_immediate(db, trans,
                      "INSERT INTO TEST1_TABLE (id, name) VALUES (2, 'Bob')");
    std::cout << "Insert data:" << std::endl;
    
    // Query data
    query_table(db, trans, "SELECT * FROM TEST1_TABLE");

    // Update data
    execute_immediate(db, trans,
                      "UPDATE TEST1_TABLE SET name = 'Charlie' WHERE id = 2");
    // Query data
    std::cout << "Table content after insert and update:" << std::endl;
    query_table(db, trans, "SELECT * FROM TEST1_TABLE");

    // Delete data
    execute_immediate(db, trans, "DELETE FROM TEST1_TABLE WHERE id = 1");
    // Query data
    std::cout << "Table content after delete:" << std::endl;
    query_table(db, trans, "SELECT * FROM TEST1_TABLE");

    execute_immediate(db, trans, "DELETE FROM TEST1_TABLE WHERE id = 2");

    // Commit transaction
    if (isc_commit_transaction(status, &trans)) {
        check_isc_status(status);
    }

    // Disconnect database connection
    if (isc_detach_database(status, &db)) {
        check_isc_status(status);
    }

    return 0;
}
```


### C++ odbc 使用
ODBC（Open Database Connectivity，开放数据库互连）是一种应用程序编程接口（API）规范，用于访问数据库管理系统（DBMS）。它是由微软公司开发的，作为其Windows操作系统的一部分，并得到了其他软件供应商的支持。ODBC的目的是提供一个标准的、独立于DBMS的接口，使得应用程序可以访问不同的数据库系统，而不需要对每个数据库系统进行特定的编程。

### 主要特点：

1. **独立于数据库**：ODBC允许应用程序使用统一的接口访问不同的数据库系统，如SQL Server、Oracle、MySQL等。
2. **SQL支持**：ODBC基于SQL（Structured Query Language，结构化查询语言），这是一种用于管理和操作数据库的标准语言。
3. **驱动程序**：为了连接到特定的数据库，ODBC使用特定的驱动程序（Driver）。每个数据库系统都有自己的ODBC驱动程序，这些驱动程序负责将ODBC调用转换为特定数据库可以理解的命令。
4. **跨平台**：虽然最初是为Windows设计的，但ODBC现在也支持其他操作系统，如Linux和macOS。
5. **易于集成**：ODBC可以被集成到各种编程语言和开发环境中，如C/C++、Java、.NET等。

### 工作原理：

1. **应用程序**：开发者编写的应用程序通过调用ODBC API来执行数据库操作。
2. **ODBC驱动管理器**：这是一个中间层，负责加载和卸载ODBC驱动程序，并处理应用程序与驱动程序之间的通信。
3. **ODBC驱动程序**：这是特定于数据库的组件，它将ODBC API调用转换为数据库可以理解的命令，并将结果返回给应用程序。
4. **数据库**：最终的数据存储和处理单元，执行由ODBC驱动程序发送的SQL命令。

### 使用场景：

- **数据库迁移**：当企业需要从一种数据库系统迁移到另一种时，使用ODBC可以减少对应用程序的修改。
- **多数据库支持**：应用程序需要同时支持多种数据库系统时，ODBC提供了一个统一的接口。
- **跨平台开发**：开发跨平台的应用程序时，ODBC可以帮助实现数据库访问的兼容性。

### 示例代码（C++）：

```cpp
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

int main() {
 SQLHENV hEnv; // Environment句柄
 SQLHDBC hDbc; // Connection句柄
 SQLCHAR szConnStr[1024];
 SQLCHAR szOutStr[1024];

 // 分配环境句柄
 SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
 // 设置ODBC版本
 SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
 // 分配连接句柄
 SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
 // 设置连接字符串
 SQLDriverConnect(hDbc, NULL, (SQLCHAR*)"DRIVER={SQL Server};SERVER=myserver;DATABASE=mydb;UID=myuser;PWD=mypassword", SQL_NTS, szOutStr, 1024, NULL, SQL_DRIVER_NOPROMPT);
 // 执行SQL查询等操作...
 // 清理资源
 SQLDisconnect(hDbc);
 SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
 SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
 return 0;
}
```

这段代码展示了如何使用ODBC API在C++中连接到SQL Server数据库。首先分配环境句柄和连接句柄，然后设置连接字符串并建立连接。之后可以执行SQL查询等操作，最后清理分配的资源。

### 增删改查示例

```c
#include <iostream>
#include <sql.h>
#include <sqlext.h>

void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type, const std::string& msg) {
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        SQLCHAR state[1024];
        SQLCHAR text[1024];
        SQLSMALLINT len;
        SQLINTEGER native;
        SQLGetDiagRec(type, handle, 1, state, &native, text, sizeof(text), &len);
        std::cerr << msg << " - " << text << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main() {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;

    // Allocate environment handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    checkError(ret, env, SQL_HANDLE_ENV, "SQLAllocHandle ENV");

    // Set the ODBC version environment attribute
    ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    checkError(ret, env, SQL_HANDLE_ENV, "SQLSetEnvAttr");

    // Allocate connection handle
    ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
    checkError(ret, dbc, SQL_HANDLE_DBC, "SQLAllocHandle DBC");

    // Connect to the DSN
    ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)"FirebirdDSN;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
    checkError(ret, dbc, SQL_HANDLE_DBC, "SQLDriverConnect");

    // Disable auto-commit mode
    ret = SQLSetConnectAttr(dbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
    checkError(ret, dbc, SQL_HANDLE_DBC, "SQLSetConnectAttr AUTOCOMMIT_OFF");

    // Allocate statement handle
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    checkError(ret, stmt, SQL_HANDLE_STMT, "SQLAllocHandle STMT");

    // Example: Create a table
    ret = SQLExecDirect(stmt, (SQLCHAR*)"CREATE TABLE test (id INT PRIMARY KEY, name VARCHAR(50))", SQL_NTS);
    checkError(ret, stmt, SQL_HANDLE_STMT, "SQLExecDirect CREATE TABLE");

    // Example: Insert a record
    ret = SQLExecDirect(stmt, (SQLCHAR*)"INSERT INTO test (id, name) VALUES (1, 'John Doe')", SQL_NTS);
    checkError(ret, stmt, SQL_HANDLE_STMT, "SQLExecDirect INSERT");

    // Commit the transaction
    ret = SQLEndTran(SQL_HANDLE_DBC, dbc, SQL_COMMIT);
    checkError(ret, dbc, SQL_HANDLE_DBC, "SQLEndTran SQL_COMMIT");

    // Example: Query the table
    ret = SQLExecDirect(stmt, (SQLCHAR*)"SELECT id, name FROM test", SQL_NTS);
    checkError(ret, stmt, SQL_HANDLE_STMT, "SQLExecDirect SELECT");

    SQLINTEGER id;
    SQLCHAR name[50];
    while (SQLFetch(stmt) == SQL_SUCCESS) {
        SQLGetData(stmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
        std::cout << "ID: " << id << ", Name: " << name << std::endl;
    }

    // Example: Update a record
    ret = SQLExecDirect(stmt, (SQLCHAR*)"UPDATE test SET name = 'Jane Doe' WHERE id = 1", SQL_NTS);
    checkError(ret, stmt, SQL_HANDLE_STMT, "SQLExecDirect UPDATE");

    // Commit the transaction
    ret = SQLEndTran(SQL_HANDLE_DBC, dbc, SQL_COMMIT);
    checkError(ret, dbc, SQL_HANDLE_DBC, "SQLEndTran SQL_COMMIT");

    // Example: Delete a record
    ret = SQLExecDirect(stmt, (SQLCHAR*)"DELETE FROM test WHERE id = 1", SQL_NTS);
    checkError(ret, stmt, SQL_HANDLE_STMT, "SQLExecDirect DELETE");

    // Commit the transaction
    ret = SQLEndTran(SQL_HANDLE_DBC, dbc, SQL_COMMIT);
    checkError(ret, dbc, SQL_HANDLE_DBC, "SQLEndTran SQL_COMMIT");

    // Cleanup
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);

    return 0;
}

```
```c
#include <sql.h>  
#include <sqlext.h>  
 
int main() {  
    SQLHANDLE hEnv, hDbc;  
    SQLRETURN ret;  
 
    // 分配环境句柄  
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);  
 
    // 设置ODBC版本  
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);  
 
    // 分配连接句柄  
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);  
 
    // 连接到数据库  
    ret = SQLDriverConnect(hDbc, NULL, (SQLCHAR*)"DRIVER={YourDriver};SERVER=YourServer;DATABASE=YourDatabase;UID=YourUsername;PWD=YourPassword;",  
                           SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);  
 
    // 执行SQL语句  
    SQLCHAR outstr[1024];  
    SQLSMALLINT outstrlen;  
    ret = SQLExecDirect(hDbc, (SQLCHAR*)"SELECT * FROM YourTable", SQL_NTS);  
 
    // 处理结果集  
    while (SQLFetch(hDbc) == SQL_SUCCESS) {  
        SQLGetData(hDbc, 1, SQL_C_CHAR, outstr, sizeof(outstr), &outstrlen);  
        // 处理outstr中的数据  
    }  
 
    // 关闭连接和释放句柄  
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);  
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);  
 
    return 0;  
}

```



## 在Ubuntu上使用ODBC（Open Database Connectivity），你需要安装以下软件和配置相应的文件：

1. **安装ODBC驱动管理器**：
 - 通常使用 `unixODBC` 作为ODBC驱动管理器。可以通过以下命令安装：
 ```bash
 sudo apt-get install unixodbc unixodbc-dev
 ```

2. **安装ODBC驱动**：
 - 根据你要连接的数据库类型，安装相应的ODBC驱动。例如，如果你要连接MySQL，可以安装 `libmyodbc` 或 `myodbc-installer`。
 - 对于SQL Server，可以使用 `freetds-bin` 和 `freetds-dev`。
 - 安装命令示例（以MySQL为例）：
 ```bash
 sudo apt-get install libmyodbc
 ```

3. **配置ODBC数据源**：
 - 配置文件通常位于 `/etc/odbcinst.ini` 和 `/etc/odbc.ini`。
 - `odbcinst.ini` 用于配置ODBC驱动信息，例如：
 ```ini
 [MySQL]
 Description = ODBC for MySQL
 Driver = /usr/lib/x86_64-linux-gnu/odbc/libmyodbc5w.so
 Setup = /usr/lib/x86_64-linux-gnu/odbc/libodbcmyS.so
 FileUsage = 1
 ```
 - `odbc.ini` 用于配置数据源（DSN），例如：
 ```ini
 [MySQL_DSN]
 Description = MySQL database
 Driver = MySQL
 Database = your_database
 Server = your_server
 Port = your_port
 UserName = your_username
 Password = your_password
 ```

4. **测试ODBC连接**：
 - 使用 `isql` 工具测试连接，例如：
 ```bash
 isql -v MySQL_DSN your_username your_password
 ```

确保在配置文件中填写正确的数据库服务器信息、端口、用户名和密码。配置完成后，你可以使用ODBC连接到数据库进行数据操作。


https://help.ubuntu.com/community/ODBC