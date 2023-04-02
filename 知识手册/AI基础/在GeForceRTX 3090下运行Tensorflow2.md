目录

    概述
        问题复现
        原因
    解决方案
        方案一
        方案二

概述

相信大家看这篇文章时候，肯定被3090下对tensorflow2的兼容性头疼。
下面会分析原因，且给出方案。
问题复现
Tensorflow版本	cuda版本	cudnn版本	错误
2.3.0	10.1	7.6	failed to run cuBLAS routine: CUBLAS_STATUS_NOT_SUPPORTED
原因

GeForce RTX 3090 的 computeCapability已经是 8.6。
但是CUDA10.2已经以下的版本全都不支持computeCapability 8.*的。
解决方案

自己摸索了很久，有以下解决方案。
方案一
Tensorflow版本	cuda版本	cudnn版本	驱动	操作系统
tf-nightly-gpu 2.4.0-dev20201016 或者 tensorflow 2.4.0rc0	11.0.228	8.0.3.33	455.23.05	Linux zebiao-pc 5.4.0-52-generic #57~18.04.1-Ubuntu SMP Thu Oct 15 14:04:49 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux

版本有点出入没有关系。但是cuda不要是10.1, 10.2 或者 11.1。
cudnn一定要8.* 。
Tensorflow 刚发布新版本2.4.0-rc0了，可以执行
pip install tensorflow==2.4.0rc0
方案二

考虑到可能大家的代码不一定是 Tensorflow 2.4.0能兼容。
这里提供另一个方案。
方案流程在官网有。
https://www.tensorflow.org/install/source
记得 git checkout 你代码合适的tensorlfow版本。
cuda、 cudnn、 驱动的版本看方案一。
然后编译新的tensorlfow 应该就能运行了。
————————————————
版权声明：本文为CSDN博主「心伽玛」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_45114252/article/details/109463521




官网的cuda和cudnn版本查询：
https://developer.nvidia.com/rdp/cudnn-archive