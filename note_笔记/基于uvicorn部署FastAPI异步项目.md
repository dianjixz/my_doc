https://fastapi.tiangolo.com/zh/
简介

定义：FastAPI 是一个现代、快速（高性能）的 Web 框架，用于基于标准 Python 类型提示使用 Python 构建 API。---（源于官网）
前期准备
安装FastAPI 和 Uvicorn

pip install fastapi uvicorn

示例

main.py
```python
# main.py
from fastapi import FastAPI

# 创建一个 FastAPI 类的实例
app = FastAPI()

# 使用@app.get()装饰器来创建一个路由
@app.get("/")
async def read_root():
    return {"Hello": "World"}

# 你可以添加更多的路由，例如：
@app.get("/items/{item_id}")
async def read_item(item_id: int):
    return {"item_id": item_id}

# POST 请求的路由，创建新项目
@app.post("/items/")
async def create_item(item: Item):
    # 这里可以添加代码来保存项目到数据库
    return item
```
    get/post请求的基本路由：
    1、/：根路由，当用户访问这个地址时，会返回一个 JSON 对象{"Hello": "World"}
    2、/items/{item_id}：一个带路径参数的路由，{item_id} 是一个动态参数，用户可以访问类似 /items/3 的 URL，其中 3 会被传递给 read_item 函数。

启动服务器

开启终端，cd到main.py目录下，执行以下命令：
``` bash
uvicorn main:app --reload
```
    main 是你的 Python 文件（不带 .py 扩展名）。
    app 是 FastAPI 实例的名称。
    --reload 选项使得服务器在代码改变时自动重启，这在开发过程中非常有用。

运行命令后，Uvicorn 服务器将启动，并且你的 FastAPI 应用将在本地运行。默认情况下，它会在 http://127.0.0.1:8000 上提供服务。

你可以通过访问 http://127.0.0.1:8000 来查看默认的交互式 API 文档（由 Swagger UI 自动生成），也可以通过 http://127.0.0.1:8000/docs 访问 ReDoc 文档。
参考链接
FastAPI (tiangolo.com)​
fastapi.tiangolo.com/

参考：chatgpt、kimi