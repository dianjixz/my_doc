multipart/x-mixed-replace 技术可以实现简易的流媒体传输，在此做一个技术细节实现。



服务器请求数据：
```
GET / HTTP/1.1
Host: 127.0.0.1:18813
User-Agent: Wget/1.21.2
Accept: */*
Accept-Encoding: identity
Connection: close

```

服务器回复数据：
```
HTTP/1.0 200 OK
Server: BaseHTTP/0.6 Python/3.8.5
Date: Thu, 01 Jan 1970 00:09:51 GMT
Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0
Connection: close
Content-Type: multipart/x-mixed-replace;boundary=--boundarydonotcross
Expires: Mon, 1 Jan 2030 00:00:00 GMT
Pragma: no-cache
Access-Control-Allow-Origin: *

--boundarydonotcross
X-Timestamp: 591.595504114
Content-Length: 3990
Content-Type: image/jpeg

......
--boundarydonotcross
X-Timestamp: 591.622948406
Content-Length: 4028
Content-Type: image/jpeg

....
--boundarydonotcross
X-Timestamp: 591.622948406
Content-Length: 4028
Content-Type: image/jpeg

....



```