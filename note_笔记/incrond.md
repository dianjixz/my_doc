# incron 使用笔记

incron 是一个类似于 cron 的守护进程，用于在文件系统发生变化时执行命令。可用于监控到文件的变化时执行对应的动作。
## 安装
```bash
sudo apt install incron -y
```

## 配置
配置文件路径：`/etc/incron.d/`，文件名以 `.` 开头，如：`.test`。

配置文件格式：
```
/path/to/file : user : command
```

## 示例
监控 `/tmp/test` 文件夹下所有文件的变化，当文件发生变化时，执行 `echo "file changed"` 命令。
```bash
echo "/tmp/test/* : root : echo 'file changed'" > /etc/incron.d/test
```

## 启动
```bash
systemctl start incrond
```

## 常用命令
```bash
# 查看配置文件
cat /etc/incron.d/test

# 查看日志
tail -f /var/log/incrond.log

# 停止
systemctl stop incrond
```
## 监控类型
- `IN_MODIFY`：文件被修改
- `IN_ATTRIB`：文件属性被修改
- `IN_CREATE`：文件被创建
- `IN_DELETE`：文件被删除
- `IN_MOVE`：文件被移动
- `IN_MOVED_FROM`：文件被移动（源文件）
- `IN_MOVED_TO`：文件被移动（目标文件）
- `IN_UNMOUNT`：文件系统被卸载
- `IN_CLOSE_WRITE`：文件被写入并关闭
- `IN_CLOSE_NOWRITE`：文件被关闭（未写入）
- `IN_OPEN`：文件被打开
- `IN_ACCESS`：文件被访问
- `IN_DELETE_SELF`：文件被删除
- `IN_MOVE_SELF`：文件被移动
- `IN_NOOP`：无操作
- `IN_IGNORED`：事件被忽略
- `IN_CLOSE`：文件被关闭
- `IN_Q_OVERFLOW`：事件队列溢出


## options 
可以是以下命令之一（即，您可以将arg作为参数传递给命令）：
-    $$ –美元符号
-    $@ –监视的文件系统路径（见上文）
-    $# –与事件相关的文件名
-    $% –事件标志（以文本形式）
-    $& –事件标志（按数字）



## 使用注意
incrond 在监控文件时，非常容易丢失文件变化事件，导致监控不到文件的变化。这是因为 incrond 主要监控的是文件的 node，而不是文件的内容。当文件被修改时，文件的 inode 会发生变化，但是文件的 node 不会发生变化。因此，incrond 非常容易失去对文件的监控。

推荐操作是监控文件夹，重要文件单独列出一个文件夹，这样文件夹内的文件发生变化时 incrond 很容易就能捕捉到事件，不会丢失对文件的监控。