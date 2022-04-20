---
layout: hexo_note
title: hexo_note
date: 2021-10-05 15:00:40
tags:
---
## 介绍常用的Hexo 命令

~~~ bash
npm install hexo -g #安装Hexo
npm update hexo -g #升级
hexo init #初始化博客

#命令简写
hexo n "我的博客" == hexo new "我的博客" #新建文章
hexo g == hexo generate #生成
hexo s == hexo server #启动服务预览
hexo d == hexo deploy #部署

hexo server #Hexo会监视文件变动并自动更新，无须重启服务器
hexo server -s #静态模式
hexo server -p 5000 #更改端口
hexo server -i 192.168.1.1 #自定义 IP
hexo clean #清除缓存，若是网页正常情况下可以忽略这条命令
~~~

刚刚的三个命令依次是新建一篇博客文章、生成网页、在本地预览的操作。
部署博客分别输入三条命令：

~~~ bash
hexo clean 
hexo g 
hexo d
~~~

其实第三条的 hexo d 就是部署网站命令，d是deploy的缩写。完成后，打开浏览器，在地址栏输入你的放置个人网站的仓库路径，即 http://xxxx.github.io (知乎排版可能会出现”http://“字样，参考下图) 比如我的xxxx就是我的GitHub用户名：

你就会发现你的博客已经上线了，可以在网络上被访问了。