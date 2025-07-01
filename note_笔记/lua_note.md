# lua 使用

lua 是一个非常轻量且快速的脚本语言，一般用于嵌入到应用程序中，提供灵活的扩展功能。最重要的是，lua足够的小，能够为一些内存紧凑的嵌入式设备提供一个完整的脚本语言。
因为嵌入式中的 sh 环境是非常差劲的，几乎没有什么功能，所以需要lua 来提供一些功能。

需要注意的是，lua 的使用 python 有一些区别，特别是网上提供的一些示例，有些无法直接在交互环境中运行。



## 基本环境
https://www.lua.org/ftp/lua-5.4.7.tar.gz
https://github.com/zhaojh329/lua-ffi.git  443336a542e2585ed99d6d999bf42e6661fe62ad
https://github.com/libffi/libffi.git    c6f1610509d3d146017d6cc30020ce334bde8425

lua、libffi、lua-ffi
可用于执行一些脚本，替代sh

### 交叉编译
```shell
# lua
make HOST_CC="gcc -m32" CROSS=arm-nia7ux2111v2e-linux-uclibcgnueabihf- TARGET_SYS=Linux
make HOST_CC="gcc -m32" CROSS=arm-nia7ux2111v2e-linux-uclibcgnueabihf- TARGET_SYS=Linux PREFIX=`pwd`/install install
```

```shell
# lua-ffi 编译经过了修改,手动生成的 config.h lex.c lex.h 
arm-nia7ux2111v2e-linux-uclibcgnueabihf-gcc -o ffi.os -c -D_GNU_SOURCE -DLUA_USE_LINUX -Os -Wall -Werror --std=gnu99 -fno-strict-aliasing -fPIC -I. -Ilua/libffi/build/install/include -Ilua/lua-5.4.7/src ffi.c
arm-nia7ux2111v2e-linux-uclibcgnueabihf-gcc -o lex.os -c -D_GNU_SOURCE -DLUA_USE_LINUX -Os -Wall -Werror --std=gnu99 -fno-strict-aliasing -fPIC -I. -Ilua/libffi/build/install/include -Ilua/lua-5.4.7/src lex.c
arm-nia7ux2111v2e-linux-uclibcgnueabihf-gcc -o libffi.so -shared ffi.os lex.os -Llua/libffi/build/install/lib -lffi
```

```shell
# libffi 
./configure --host=arm-nia7ux2111v2e-linux-uclibcgnueabihf --prefix=`pwd`/install
make 
make install
```






luajit: 一个即时编译的lua，性能比标准lua高很多，但是需要重新编译，并且不能直接使用标准lua的库，需要重新编译。
luajit自带libffi，无需重新支持。luajit 可用于高性能计算。
https://luajit.org/ext_buffer.html





Question: 为什么lua在终端执行时和执行脚本表现的不一样
在终端交互式执行Lua和运行脚本时表现不一致，通常由以下原因导致：

---

### 1. **作用域差异**
   - **交互式环境**：每行输入都是一个独立代码块，局部变量作用域仅限当前行（除非显式声明为`local`且跨行续写）。
   ```lua
   > x = 42      -- 全局变量（整个会话可见）
   > local y = 10 -- 仅当前行有效，下一行无法访问
   ```
   - **脚本执行**：整个文件是一个代码块，`local`变量作用域贯穿整个文件。

---

### 2. **输出行为差异**
   - **交互式环境**：自动打印表达式结果（类似`print()`），但**仅限最后一行**的直接表达式。
     ```lua
     > 1 + 1      -- 输出 2
     > local a = 5 -- 无输出（赋值语句不返回值）
     ```
   - **脚本执行**：除非显式调用`print()`，否则无输出。

---

### 3. **语法限制**
   - **交互式环境**：某些语法（如跨行的`if-then-end`）需要特殊处理（如使用`do-end`包裹）。
     ```lua
     > do
     >> if true then print("OK") end
     >> end
     ```
   - **脚本执行**：直接支持多行代码块。

---

### 4. **环境变量影响**
   - **交互式环境**：可能继承或修改全局变量（如`_G`或`_PROMPT`）。
   - **脚本执行**：通常以纯净环境启动，除非显式引入外部模块。

---

### 5. **错误处理差异**
   - **交互式环境**：错误会中断当前行，但会话继续。
   - **脚本执行**：错误默认导致脚本终止（除非用`pcall`捕获）。

---

### 6. **标准库行为**
   - **`io.read()`**：交互式下可能阻塞等待输入，而脚本中若未提供输入会直接失败。

---

### 解决方案
- **显式输出**：在脚本中主动使用`print()`。
- **作用域控制**：避免依赖交互式环境的全局特性，合理使用`local`。
- **语法适配**：交互式中用`do-end`包裹多行代码块。

通过理解这些差异，可以避免因环境不同导致的意外行为。




   - Lua 5.1 默认无位运算库，需第三方库（如 `bitop`）。
   - Lua 5.2/5.3 用 `bit32`。
   - LuaJIT 用 `bit`。
   - Lua 5.4+ 直接支持运算符。


   Question: lua以16进制打印数字
在 Lua 中，可以使用 `string.format` 函数将数字以十六进制格式打印。以下是几种常见的方法：

### 1. 基本用法
```lua
local num = 255
print(string.format("%x", num)) -- 输出: ff (小写)
print(string.format("%X", num)) -- 输出: FF (大写)
```

### 2. 控制位数（补前导零）
```lua
local num = 15
print(string.format("%02X", num)) -- 输出: 0F (固定2位，不足补零)
print(string.format("%04X", num)) -- 输出: 000F (固定4位)
```

### 3. 带前缀的十六进制
```lua
local num = 48879
print(string.format("0x%X", num)) -- 输出: 0xBEEF
```

### 4. 从十六进制字符串转数字
```lua
local hexStr = "FF"
local num = tonumber(hexStr, 16)
print(num) -- 输出: 255
```

### 示例代码
```lua
local number = 48879

-- 小写十六进制
print(string.format("Hex (lower): %x", number))  -- 输出: beef

-- 大写十六进制带前缀
print(string.format("Hex (upper): 0x%X", number)) -- 输出: 0xBEEF

-- 8位固定宽度，补前导零
print(string.format("Padded: %08X", number))     -- 输出: 0000BEEF
```

### 注意事项
- Lua 5.1+ 都支持这种格式化方式
- 负数会以二进制补码形式显示（如 `-1` 会显示为 `FFFFFFFF`）
- 大数建议用 `%X`，避免字母大小写混用导致的歧义


Question: lua将十六进制字符串转换成数字
在Lua中，将十六进制字符串转换为数字可以通过以下几种方法实现：

### 方法1：使用 `tonumber` 函数（最常用）
```lua
local hexStr = "1A3F"
local num = tonumber(hexStr, 16)
print(num)  -- 输出 6719
```


Question: lua gsub函数
# Lua 中的 gsub 函数详解

`gsub` 是 Lua 字符串处理中的一个重要函数，用于全局替换字符串中的内容。它的全称是 "global substitution"（全局替换）。

## 基本语法

```lua
new_string, count = string.gsub(original_string, pattern, replacement [, n])
```

或者使用字符串方法的形式：

```lua
new_string, count = original_string:gsub(pattern, replacement [, n])
```

## 参数说明

- `original_string`: 原始字符串
- `pattern`: 匹配模式（Lua 模式，不是正则表达式）
- `replacement`: 替换内容（可以是字符串、函数或表）
- `n` (可选): 最大替换次数

## 返回值

- `new_string`: 替换后的新字符串
- `count`: 实际发生的替换次数

## 使用示例

### 1. 简单字符串替换

```lua
local str = "hello world"
local new_str, count = str:gsub("hello", "hi")
print(new_str)  -- 输出: hi world
print(count)    -- 输出: 1
```

### 2. 使用模式匹配

```lua
local str = "The rain in Spain"
local new_str = str:gsub("%a+", "word")
print(new_str)  -- 输出: word word word word
```

### 3. 限制替换次数

```lua
local str = "banana"
local new_str = str:gsub("a", "o", 2)
print(new_str)  -- 输出: bonona (只替换前两个a)
```

### 4. 使用函数作为替换

```lua
local str = "1 2 3 4"
local new_str = str:gsub("%d", function(match)
    return tonumber(match) * 2
end)
print(new_str)  -- 输出: 2 4 6 8
```

### 5. 使用表作为替换

```lua
local replacements = {cat = "dog", red = "blue"}
local str = "The cat is red"
local new_str = str:gsub("%a+", replacements)
print(new_str)  -- 输出: The dog is blue
```

## 注意事项

1. Lua 的模式匹配与正则表达式类似但不完全相同，更简单轻量
2. 常用模式项：
   - `.`: 任意字符
   - `%a`: 字母
   - `%d`: 数字
   - `%s`: 空白字符
   - `%w`: 字母或数字
   - `+`: 1次或多次
   - `*`: 0次或多次
   - `-`: 最小匹配（懒惰匹配）

3. 如果要替换的字符串中包含魔法字符（如 `%`, `.`, `$` 等），需要用 `%` 进行转义

`gsub` 是 Lua 字符串处理中非常强大和常用的函数，掌握它能极大地提高字符串处理效率。

