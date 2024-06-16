# 简单的 ai 终端助手



## deepseek 平台的简单函数
到 https://platform.deepseek.com 申请一个 api-key 替换下面的 API_KEY就能直接在终端中使用 helpme 命令获取 ai 助手的帮助。  

简单 ai 助手，帮助你解决一些问题。
```bash
API_KEY="sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
API_URL="https://api.deepseek.com/chat/completions"
helpme() {
  local question=$*
  echo "Question: $question"
  local response=$(curl -s -X POST $API_URL \
    -H "Content-Type: application/json" \
    -H "Authorization: Bearer $API_KEY" \
    -d '{
     "model": "deepseek-chat",
        "messages": [
          {"role": "system", "content": "You are a helpful assistant.用中文回答"},
          {"role": "user", "content":"'"$question"'"}
        ],
        "stream": false
    }')

  local answer=$(echo $response | jq -r '.choices[0].message.content' )

  
  echo "Answer: $answer"
}
```

简单 ai 翻译助手，帮助你翻译一些话语。

```bash
API_KEY="sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
API_URL="https://api.deepseek.com/chat/completions"
translate () {
    local question=$*
    echo "Question: $question"
    local response=$(curl -s -X POST "https://api.deepseek.com/chat/completions" 
    -H "Content-Type: application/json"     
    -H "Authorization: Bearer $API_KEY"     
    -d '{
     "model": "deepseek-chat",
        "messages": [
          {"role": "system", "content": "你是一个好用的翻译助手。请将我的中文翻译成英文，将所有非中文的翻译成中文。我发给你所有的话都是需要翻译的内容，你只需要回答翻译结果。翻译结果请符合中文的语言习惯。"},
          {"role": "user", "content":"'"$question"'"}
        ],
        "stream": false
    }');
    local answer=$(echo $response | jq -r '.choices[0].message.content' );
    echo "Answer: $answer"
}
```


使用效果：
```bash
root@root-z690:~$ helpme "用c写一个heloworl demo"
Question: 用c写一个heloworl demo
Answer: 在C语言中，一个简单的"Hello, World"示例程序如下：

```c
#include <stdio.h>

int main() {
 printf("Hello, World\n");
 return 0;
}
```

这个程序包含以下几个部分：
1. `#include <stdio.h>`：这是一个预处理指令，用于包含标准输入输出库，使得我们可以使用`printf`函数。
2. `int main()`：这是程序的主函数，每个C程序都从这里开始执行。
3. `printf("Hello, World\n");`：这是一个函数调用，用于在控制台上输出字符串"Hello, World"，`\n`表示换行。
4. `return 0;`：这是主函数的返回语句，通常返回0表示程序正常结束。

你可以将这段代码保存为一个`.c`文件，例如`hello.c`，然后使用C语言编译器（如GCC）编译并运行它：

```bash
gcc hello.c -o hello
./hello
```

这将会在控制台上输出"Hello, World"。

root@root-z690:~$ translate "你是一个好用的翻译助手"
Question: 你是一个好用的翻译助手
Answer: You are a useful translation assistant.
```
