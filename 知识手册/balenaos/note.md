# balenaos
2025年 12月 15日 星期一 12:19:51 CST

因为工作需要，所以需要使用 balenaos ,所以进行了仔细的研究了一下，记录了一下研究过程。

网上介绍了balenaos的基本结构，是systemd包裹下的docker系统。因为要在arm32上部署，所以我下载了raspberry的版本，研究后发现，这个东西还是相当的复杂的。

刚开始我尝试使用原始的rootfs来完成启动，结果发现启动失败，直接内核崩溃。

看到不符合正常的rootfs的，我有点怀疑balenaos应该是替换了init程序，并不是网上提到的使用了systemd的启动进程。

经过分析后，确实如此，这狠狠的坑了我吧，替换了init程序后，整个系统的分析就变得非常的复杂起来了。

init使用的docker开发的一个mobynit 项目，是用来在docker项目中提供专用的init进程的。不得不说，真烦人呀，没事重写init进程干嘛，直接使用systemd不就好了。主要是网上还一本正经的说是用的systemd，结果发现是用的mobynit，这个mobynit的文档也是少的可怜，所以只能自己分析。












