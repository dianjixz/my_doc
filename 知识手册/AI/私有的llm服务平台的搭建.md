# 私有LLM服务平台搭建指南

## 概述

搭建私有LLM服务平台可以为企业或个人提供安全、可控的大语言模型服务。本文档介绍两个主流的开源解决方案：NEW API 和 Open WebUI，并提供详细的搭建和操作步骤。

## 平台介绍

### 1. NEW API
- **项目地址**: https://github.com/Calcium-Ion/new-api
- **功能**: 提供统一的API接口管理平台，支持多种LLM模型的API转发和管理
- **特点**: 
  - 支持多种模型提供商（OpenAI、Claude、Gemini等）
  - 用户管理和配额控制
  - API密钥管理
  - 使用统计和监控
  - 支持负载均衡

### 2. Open WebUI
- **项目地址**: https://github.com/open-webui/open-webui
- **功能**: 提供类似ChatGPT的Web界面，支持本地和远程LLM模型
- **特点**:
  - 现代化的聊天界面
  - 支持多种模型切换
  - 文档上传和处理
  - 插件系统
  - 用户管理
  - 支持Ollama、OpenAI API等

## 系统要求

### 硬件要求
- **CPU**: 4核心以上
- **内存**: 8GB以上（推荐16GB）
- **存储**: 50GB以上可用空间
- **网络**: 稳定的互联网连接

### 软件要求
- **操作系统**: Linux (Ubuntu 20.04+/CentOS 7+) 或 macOS
- **Docker**: 20.10+
- **Docker Compose**: 1.29+
- **Git**: 2.0+

## NEW API 平台搭建

### 1. 环境准备

```bash
# 更新系统包
sudo apt update && sudo apt upgrade -y

# 安装必要工具
sudo apt install -y git curl wget

# 安装Docker
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh

# 安装Docker Compose
sudo curl -L "https://github.com/docker/compose/releases/latest/download/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose

# 将当前用户添加到docker组
sudo usermod -aG docker $USER
```

### 2. 下载和配置

```bash
# 克隆项目
git clone https://github.com/Calcium-Ion/new-api.git
cd new-api

# 复制配置文件
cp docker-compose.yml.example docker-compose.yml
cp .env.example .env
```

### 3. 配置文件修改

编辑 `.env` 文件：

```bash
# 数据库配置
SQL_DSN=new-api:123456@tcp(mysql:3306)/new-api

# Redis配置（可选）
REDIS_CONN_STRING=redis://redis:6379

# 系统配置
SESSION_SECRET=your-session-secret-here
JWT_SECRET=your-jwt-secret-here

# 管理员配置
INITIAL_ROOT_TOKEN=your-root-token-here

# 服务端口
PORT=3000

# 日志级别
LOG_LEVEL=info
```

编辑 `docker-compose.yml` 文件：

```yaml
version: '3.8'

services:
  new-api:
    image: calciumion/new-api:latest
    container_name: new-api
    restart: always
    ports:
      - "3000:3000"
    environment:
      - SQL_DSN=new-api:123456@tcp(mysql:3306)/new-api
      - REDIS_CONN_STRING=redis://redis:6379
      - SESSION_SECRET=${SESSION_SECRET}
      - JWT_SECRET=${JWT_SECRET}
      - INITIAL_ROOT_TOKEN=${INITIAL_ROOT_TOKEN}
    depends_on:
      - mysql
      - redis
    volumes:
      - ./data:/data

  mysql:
    image: mysql:8.0
    container_name: new-api-mysql
    restart: always
    environment:
      - MYSQL_ROOT_PASSWORD=123456
      - MYSQL_DATABASE=new-api
      - MYSQL_USER=new-api
      - MYSQL_PASSWORD=123456
    volumes:
      - ./mysql_data:/var/lib/mysql
    ports:
      - "3306:3306"

  redis:
    image: redis:7-alpine
    container_name: new-api-redis
    restart: always
    volumes:
      - ./redis_data:/data
    ports:
      - "6379:6379"
```

### 4. 启动服务

```bash
# 启动所有服务
docker-compose up -d

# 查看服务状态
docker-compose ps

# 查看日志
docker-compose logs -f new-api
```

### 5. NEW API 操作步骤

#### 5.1 初始登录
1. 打开浏览器访问 `http://your-server-ip:3000`
2. 使用管理员账号登录（用户名：root，密码：在环境变量中设置的INITIAL_ROOT_TOKEN）

#### 5.2 配置模型渠道
1. 进入"渠道管理"页面
2. 点击"新增渠道"
3. 填写渠道信息：
   - 渠道名称：自定义名称
   - 渠道类型：选择对应的API提供商（如OpenAI、Claude等）
   - Base URL：API基础地址
   - 密钥：对应的API密钥
   - 模型：支持的模型列表

#### 5.3 用户管理
1. 进入"用户管理"页面
2. 点击"新增用户"创建新用户
3. 设置用户配额和权限
4. 为用户分配令牌

#### 5.4 API使用
用户可以使用分配的令牌调用API：

```bash
curl -X POST http://your-server-ip:3000/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer your-token-here" \
  -d '{
    "model": "gpt-3.5-turbo",
    "messages": [
      {"role": "user", "content": "Hello, world!"}
    ]
  }'
```

## Open WebUI 平台搭建

### 1. 使用Docker快速部署

```bash
# 创建工作目录
mkdir -p ~/open-webui
cd ~/open-webui

# 直接运行Open WebUI
docker run -d \
  --name open-webui \
  -p 3000:8080 \
  -v open-webui:/app/backend/data \
  --restart always \
  ghcr.io/open-webui/open-webui:main
```

### 2. 使用Docker Compose部署（推荐）

创建 `docker-compose.yml` 文件：

```yaml
version: '3.8'

services:
  open-webui:
    image: ghcr.io/open-webui/open-webui:main
    container_name: open-webui
    restart: always
    ports:
      - "3000:8080"
    environment:
      - OLLAMA_BASE_URL=http://ollama:11434
      - WEBUI_SECRET_KEY=your-secret-key-here
      - WEBUI_JWT_SECRET_KEY=your-jwt-secret-here
    volumes:
      - ./data:/app/backend/data
    depends_on:
      - ollama

  ollama:
    image: ollama/ollama:latest
    container_name: ollama
    restart: always
    ports:
      - "11434:11434"
    volumes:
      - ./ollama_data:/root/.ollama
    environment:
      - OLLAMA_HOST=0.0.0.0
```

### 3. 启动服务

```bash
# 启动服务
docker-compose up -d

# 查看服务状态
docker-compose ps

# 查看日志
docker-compose logs -f open-webui
```

### 4. Open WebUI 操作步骤

#### 4.1 初始设置
1. 打开浏览器访问 `http://your-server-ip:3000`
2. 首次访问会要求创建管理员账户
3. 填写用户名、邮箱和密码完成注册

#### 4.2 配置模型
1. 点击右上角的设置图标
2. 进入"模型"设置页面
3. 配置模型连接：
   - **Ollama模型**: 如果使用本地Ollama，确保Ollama服务正在运行
   - **OpenAI API**: 在"外部连接"中配置OpenAI API密钥
   - **自定义API**: 配置其他兼容OpenAI格式的API

#### 4.3 下载和使用Ollama模型

```bash
# 进入ollama容器
docker exec -it ollama bash

# 下载模型（示例）
ollama pull llama2
ollama pull codellama
ollama pull mistral

# 查看已下载的模型
ollama list
```

#### 4.4 用户管理
1. 管理员可以在设置中管理用户
2. 创建新用户或邀请用户注册
3. 设置用户权限和访问控制

#### 4.5 使用聊天功能
1. 在主界面选择要使用的模型
2. 开始对话
3. 支持文档上传和分析
4. 可以创建和管理对话历史

## 高级配置

### 1. 反向代理配置（Nginx）

创建 `/etc/nginx/sites-available/llm-platform` 文件：

```nginx
server {
    listen 80;
    server_name your-domain.com;

    # NEW API
    location /api/ {
        proxy_pass http://localhost:3000/;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }

    # Open WebUI
    location / {
        proxy_pass http://localhost:3001/;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
    }
}
```

### 2. SSL证书配置

```bash
# 安装Certbot
sudo apt install certbot python3-certbot-nginx

# 获取SSL证书
sudo certbot --nginx -d your-domain.com

# 自动续期
sudo crontab -e
# 添加以下行
0 12 * * * /usr/bin/certbot renew --quiet
```

### 3. 数据备份

```bash
#!/bin/bash
# backup.sh

BACKUP_DIR="/backup/llm-platform"
DATE=$(date +%Y%m%d_%H%M%S)

# 创建备份目录
mkdir -p $BACKUP_DIR

# 备份NEW API数据
docker exec new-api-mysql mysqldump -u root -p123456 new-api > $BACKUP_DIR/new-api-$DATE.sql

# 备份Open WebUI数据
docker cp open-webui:/app/backend/data $BACKUP_DIR/open-webui-data-$DATE

# 压缩备份文件
tar -czf $BACKUP_DIR/llm-platform-backup-$DATE.tar.gz $BACKUP_DIR/*-$DATE*

# 清理旧备份（保留7天）
find $BACKUP_DIR -name "*.tar.gz" -mtime +7 -delete
```

## 监控和维护

### 1. 系统监控

```bash
# 查看容器状态
docker ps -a

# 查看资源使用情况
docker stats

# 查看日志
docker logs -f container-name

# 清理未使用的镜像和容器
docker system prune -a
```

### 2. 性能优化

```yaml
# docker-compose.yml 中添加资源限制
services:
  open-webui:
    deploy:
      resources:
        limits:
          cpus: '2.0'
          memory: 4G
        reservations:
          cpus: '1.0'
          memory: 2G
```

### 3. 安全建议

1. **网络安全**:
   - 使用防火墙限制访问端口
   - 配置SSL/TLS加密
   - 定期更新系统和容器镜像

2. **访问控制**:
   - 设置强密码策略
   - 启用双因素认证
   - 定期轮换API密钥

3. **数据安全**:
   - 定期备份数据
   - 加密敏感数据
   - 监控异常访问

## 故障排除

### 常见问题

1. **容器启动失败**:
   ```bash
   # 查看详细错误信息
   docker-compose logs container-name
   
   # 检查端口占用
   netstat -tlnp | grep :3000
   ```

2. **数据库连接失败**:
   ```bash
   # 检查数据库容器状态
   docker exec -it mysql-container mysql -u root -p
   
   # 重置数据库密码
   docker exec -it mysql-container mysql -u root -p -e "ALTER USER 'root'@'%' IDENTIFIED BY 'new-password';"
   ```

3. **模型加载失败**:
   ```bash
   # 检查Ollama服务
   docker exec -it ollama ollama list
   
   # 重新下载模型
   docker exec -it ollama ollama pull model-name
   ```

## 总结

通过本指南，您可以成功搭建包含NEW API和Open WebUI的私有LLM服务平台。NEW API提供了强大的API管理功能，而Open WebUI则提供了友好的用户界面。两个平台可以独立使用，也可以结合使用以提供完整的LLM服务解决方案。

建议在生产环境中：
- 使用专用服务器或云服务
- 配置适当的监控和告警
- 定期备份数据
- 保持系统和软件更新
- 实施适当的安全措施

这样可以确保平台的稳定性、安全性和可用性。
