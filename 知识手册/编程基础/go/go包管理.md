Go 语言从1.11和1.12开始实现了对包的管理，Golang从1.11版本官方开始推出版本管理工具go module，并且从1.13版本开始，go module将是go语言默认的包依赖管理工具。

**1.前提条件**

使用go module前需要开启GO111MODULE，需要设置环境变量GO111MODULE=on

**2.网络优化**

golang1.13以后GOPROXY默认值为[https://proxy.golang.org](https://link.zhihu.com/?target=https%3A//proxy.golang.org)，在国内访问是会限制的，如果没有条件翻墙的话，可以将GOPROXY设置为下面的配置

```go
go env -w GOPROXY=https://goproxy.cn,direct
```

**3.实战**

下面通过github上star比较高的go的elastic包为例,github地址为[https://github.com/olivere/elastic](https://link.zhihu.com/?target=https%3A//github.com/olivere/elastic)

先创建一个目录go-elastic，然后进入该目录，执行下面的命令初始化模块

```text
go mod init go-elastic
```

生成一个go.mod文件，内容如下：

```text
module go-elastic

go 1.17
```

然后创建主程序文件main.go

```text
package main

import (
    "context"
    "fmt"

    "github.com/olivere/elastic/v7"
)

func main() {
    // 创建ES client用于后续操作ES
    _, err := elastic.NewClient(
    // 设置ES服务地址，支持多个地址
    elastic.SetURL("http://127.0.0.1:9200"), # 此处填写自己的es地址
    // 设置基于http base auth验证的账号和密码
    elastic.SetBasicAuth("elastic", "elastic123456")) # 此处填写自己的es用户名密码
    if err != nil {
        // Handle error
        fmt.Printf("连接失败: %v\n", err)
        } else {
        fmt.Println("连接成功")
     }
}
```

执行下面的命令，增加缺少的module

```text
go mod tidy
```

会生成一个名为go.sum的文件，其中包含特定模块版本内容的预期加密哈希，go命令使用go.sum文件确保这些模块的未来下载检索与第一次下载相同的位，以确保项目所依赖的模块不会出现意外更改，无论是出于恶意、意外还是其他原因。 go.mod和go.sum都应检入版本控制。go.sum 不需要手工维护，所以可以不用太关注。

**4.go mod常用命令介绍**

```text
go mod init        初始化当前文件夹, 创建go.mod文件
go mod tidy        增加缺少的module，删除无用的module
go mod vendor      将依赖复制到vendor下
go mod verify      校验依赖
go mod edit        编辑go.mod文件 
go mod graph       打印模块依赖图
go mod download    下载依赖的module到本地cache（默认为$GOPATH/pkg/mod目录）
```



编辑于 2022-02-27 09:19

[Go 语言](https://www.zhihu.com/topic/19625982)

[管理方式](https://www.zhihu.com/topic/19572862)



