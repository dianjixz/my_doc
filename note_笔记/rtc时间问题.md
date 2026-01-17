# RTC 时间错误

当RTC时间复位后，时间会变成1970年1月1日，这是由于RTC的初始值是1970年1月1日，所以当系统启动时，RTC会读取这个初始值，并将其设置为当前时间。这个错误非常的致命，这会让原本的一些服务出现错误。所以写一个脚本，用于当时间错误时，自动将时间设置为出厂时间，从而避免错误。

```
#!/bin/bash

# 目标时间设定
TARGET_TIME="2024-06-15 10:00:00"

# 当前时间转成时间戳
CURRENT_TS=$(date +%s)
# 目标时间转成时间戳
TARGET_TS=$(date -d "$TARGET_TIME" +%s)

# 判断当前时间是否小于目标时间
if [ "$CURRENT_TS" -lt "$TARGET_TS" ]; then
    echo "当前时间早于 $TARGET_TIME，强制设置系统时间。"
    sudo date -s "$TARGET_TIME"
else
    echo "当前时间已在 $TARGET_TIME 之后，无需设置。"
fi
```