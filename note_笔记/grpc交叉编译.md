# grpc交叉编译

gRPC（全称为 gRPC Remote Procedure Call）是一个高性能、开源和通用的远程过程调用（RPC）框架，最初由Google开发。它允许客户端应用程序直接调用不同服务器上的方法，就像它们是本地对象一样，简化了分布式系统的开发。以下是一些关于gRPC的关键点：

### 核心特性

1. **高性能和可扩展性**：
   - gRPC 基于 HTTP/2 协议，提供了多路复用、流控、头部压缩和双向流等特性，使其在性能上有优势。
   - 适用于低延迟、高吞吐量的分布式系统。

2. **多语言支持**：
   - gRPC 支持多种编程语言，包括但不限于C、C++、Java、Python、Go、Ruby、Node.js、C#等，使其在多语言环境下非常灵活。

3. **Protocol Buffers**：
   - gRPC 默认使用 Protocol Buffers（protobuf）作为接口定义语言（IDL）和数据序列化格式。protobuf 高效、紧凑的二进制格式有助于提高传输速度和节省带宽。
   - 使用 protobuf 文件定义服务和消息类型，然后生成相应的客户端和服务器端代码。

4. **双向流**：
   - gRPC 支持四种服务方法类型：单向 RPC、服务器流 RPC、客户端流 RPC 和双向流 RPC。
   - 这使得 gRPC 能够处理各种复杂的通信模式，适应不同的应用场景。

5. **负载均衡和命名解析**：
   - gRPC 内置支持各种负载均衡策略和命名解析机制，使其在大规模分布式系统中表现良好。

6. **安全性**：
   - gRPC 支持通过 TLS 进行通信加密，确保数据在传输过程中的安全性。

### 基本架构

1. **服务定义**：
   - 使用 `.proto` 文件定义服务和消息类型。例如：
     ```proto
     syntax = "proto3";

     service Greeter {
       rpc SayHello (HelloRequest) returns (HelloReply) {}
     }

     message HelloRequest {
       string name = 1;
     }

     message HelloReply {
       string message = 1;
     }
     ```

2. **代码生成**：
   - 使用 `protoc` 编译器生成客户端和服务器的代码。

3. **实现服务逻辑**：
   - 在生成的代码基础上，实现具体的服务逻辑。

4. **客户端调用**：
   - 客户端使用生成的代码进行远程调用。

### 使用场景

- 微服务架构：gRPC 能够高效地连接大量的微服务。
- 分布式系统：适用于需要高性能和低延迟的分布式应用。
- 移动和物联网：由于其高效的带宽利用率和多语言支持，适用于移动设备和物联网设备。

### 总结

gRPC 是一个功能强大、性能优越的远程过程调用框架，适用于构建高效、可扩展的分布式系统。它的多语言支持、基于 HTTP/2 的特性、灵活的流处理能力和高效的 Protocol Buffers 数据格式，使其在现代软件开发中得到广泛应用。

## 编译过程
交叉编译 grpc 是一件比较困难的事情，本教程是从 buildroot 中提取出的编译过程。使用的版本也是根据 buildroot 中获取的，请酌情使用。  
grpc 依赖 c-ares host-grpc libabseil-cpp openssl protobuf re2 zlib。  
在编译 grpc 前先要编译相应的依赖。  

### 编译 protobuf
protobuf 的交叉编译依赖比较小，但是，protobuf 的编译需要编译两次，第一次是编译出主机用的 protobuf 工具，第二次才能编译目标平台的软件库。
