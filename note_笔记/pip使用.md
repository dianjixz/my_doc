 

[pip download/install 下载/离线安装第三方包](https://www.cnblogs.com/jasonzhang-blog/p/11262738.html)

```
pip download 你的包名 -d "下载的路径(windows下双引号来表示文件夹)"
```

举个栗子：

```
pip download pymysql -d "D:\pipDownloadTest"
```

- pip离线安装

```
pip install <包名>
```

举个栗子：

```
1 # 切换终端在待安装文件目录下（D:\pipDownloadTest）
2 pip install PyMySQL-0.9.3-py2.py3-none-any.whl
```
