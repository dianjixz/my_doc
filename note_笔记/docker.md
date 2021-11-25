 

本篇文章为大家展示了--privileged=true参数在docker中的作用是什么，内容简明扼要并且容易理解，绝对能使你眼前一亮，通过这篇文章的详细介绍希望你能有所收获。

大约在0.6版，privileged被引入docker。

使用该参数，container内的root拥有真正的root权限。

否则，container内的root只是外部的一个普通用户权限。

privileged启动的容器，可以看到很多host上的设备，并且可以执行mount。

甚至允许你在docker容器中启动docker容器。
