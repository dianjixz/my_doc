# resnet18自学笔记

## 前言

`ResNet`是恺明大神提出来的一种结构，近些年的一些结构变种，很多也是基于`ResNet`做的一些改进，可以说`ResNet`开创了更深的网络的先河，并且在很多计算机视觉学习上都取得了不错的效果。  

特点：cnn卷积网络 残差网络 缓解梯度问题

## Resnet和传统网络的对比

`ResNet`本质上是为了缓解梯度问题的，随着传统的卷积网络结构越来越深，大家发现效果可能会降低，所以限制了网络层数的加深。下面的图片展示了传统的CNN：  

![](D:\work\git\my_doc\image\chuantong_cnn.jpg)

可以看出就是卷积层的叠加，一层卷积一层卷积的直接连起来。  

下面图片展示了`ResNet`的核心设计：    

![](D:\work\git\my_doc\image\resnet_can.jpg)

其中的`weight layer`你可以把它看作是卷积层。如果用数学公式来规范下，设`F(x)`为卷积操作，则普通的网络规范为`Net = F(x)`,而`ResNet`则规范为`Net = F(x) +x`。  

那为什么这样的结构就好呢，具体推理可以看原论文，我这里给出一种直观上的解释，`ResNet`里面的核心就是右边的那个`shortcut`，也就是公式中的`x`，这样网络就能做出选择，如果过深的时候，网络没有作用了，那么网络学习过程中就把`F(x)`学习为0，这样网络就变为了`Net = x`，其实就相当于过于深的层不起作用了，但也不会造成负影响，所以`ResNet`最终的结果只会大于等于传统的网络，而不会小于。所以这一设计虽然简单，但是作用却重大，在`ResNet`出现之前网络最多十几层，比如`Vgg16`，但是`ResNet`出现后，网络能达到`100`层，比如`ResNet 101`。  

## ResNet结构详解--结合pytorch官方代码

上面的只是给出了一些核心思想，但是设计编码还是有些复杂，所以结合`pytorch`官方给的代码，对结构进行分析一下，这里以`ResNet 18`以及`ResNet 50`为例。大家想过`ResNet 18`的这个18层都哪18层吗，其实`18 = 1+(2*2 + 2*2 +2*2 +2*2) +1`。其中第一个1和最后一个1代表开头的一个普通卷积层，和一个全连接层（这里不计算Relu、BN等不带weight的层）。而中间的四个是四层ResNet的layer，每个layer包含了两个Block，每个Block有2个卷积层。而每个Block的样子就类似于上面介绍ResNet时的形状。如果你想看下结构详情，可以输入如下代码：  

~~~ python
from torchvision.models.resnet import resnet18,resnet50,resnet34, Bottleneck
net = resnet18()
print(net)
~~~

最后得到如下输出：  

~~~ python
ResNet(
  (conv1): Conv2d(3, 64, kernel_size=(7, 7), stride=(2, 2), padding=(3, 3), bias=False)
  (bn1): BatchNorm2d(64, eps=1e-05, momentum=0.1, affine=True, track_running_stats=True)
  (relu): ReLU(inplace)
  (maxpool): MaxPool2d(kernel_size=3, stride=2, padding=1, dilation=1, ceil_mode=False)
  (layer1): Sequential(
    (0): BasicBlock(
      (conv1): Conv2d(64, 64, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1), bias=False)
      (bn1): BatchNorm2d(64, eps=1e-05, momentum=0.1, affine=True, track_running_stats=True)
      (relu): ReLU(inplace)
      (conv2): Conv2d(64, 64, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1), bias=False)
      (bn2): BatchNorm2d(64, eps=1e-05, momentum=0.1, affine=True, track_running_stats=True)
    )
    (1): BasicBlock(
      (conv1): Conv2d(64, 64, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1), bias=False)
      (bn1): BatchNorm2d(64, eps=1e-05, momentum=0.1, affine=True, track_running_stats=True)
      (relu): ReLU(inplace)
      (conv2): Conv2d(64, 64, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1), bias=False)
      (bn2): BatchNorm2d(64, eps=1e-05, momentum=0.1, affine=True, track_running_stats=True)
    )
  )
....后面有省略
~~~

从输出也可以看出，开始有一个conv1，也就是第一层，之后有layer1、layer2..,其中layer1中包含了2个BasicBlock，而每个BasicBlock里面有两个卷积，下面我画了画了一层的简图：

![](D:\work\git\my_doc\image\resnet_Block.jpg)

可以看出每个Block就是之前ResNet的一个小结构，也叫做一个ResNet块。  

下面我们再结合pytroch代码解读一下，是如何编写的。
 代码参考这个地址： [https://github.com/pytorch/vision/blob/master/torchvision/models/resnet.py#L227](https://link.zhihu.com/?target=https%3A//github.com/pytorch/vision/blob/master/torchvision/models/resnet.py%23L227) 

在上面的`_resnet`函数中`model = ResNet(block, layers, **kwargs)` 代表了调用ResNet，其中block代表定义基本块的方法（ResNet18和ResNet50是不一样的）、layers是一个list，长度代表层数，而每个值代表每层的基本块个数。从resnet18函数可以看出基本块为`BasicBlock`，而layers为`[2,2,2,2]`,这个2，也是我们`18 = 1 + 2*2 +2*2 +2*2 +2*2 +1`中的2的来源，每层两个块，而第二个2，我们要去BacicBlock里面看了。找到`class BasicBlock(nn.Module):`，在init里面发现里面有两个conv操作，在forward里面可以看出具体的运算，先把x复制给identity，然后进行两波卷积操作`conv1(x)、conv2(out)`，而最终输出为`out += identity`，也就是我们之前说的ResNet的基本操作`Net = F(x) +x`，也就在这里体现了，从这个BaciBloc里面可以看出，每个块有两个卷积层，这就是另外一个2的来源了。  

看完了基本结构，下面我们对具体的定义再做一下解析，在`_resnet`里面，定义网络是通过`model = ResNet(block, layers, **kwargs)`定义的，所以我们转到`class ResNet(nn.Module):`，可以看出在init里面，先进行了第一个conv1，也就是18中的第一个1，之后layer1、layer2，layer3，layer4，最后一个fc层。这里主要分析一下如何构造layer的，找到函数`def _make_layer`，可以看出，主要传入的是构造块的方法，这里是BacicBlock，以及块的个数（2），利用for循环构造多个块即可。到此为止ResNet18分析完毕，知道了18层都包含什么，以及是如何构造的了。  

下面再简单分析一下ResNet50，因为其和18的块是不一样，我们转到`def resnet50`，可以看出基本块是`Bottleneck`,并且层数是`[3, 4, 6, 3]`，转到`class Bottleneck(nn.Module)`，可以看出里面的conv是有3个（这里其实还有一个downsample，这是为了工程上的，本次不予考虑），所以`50 = 1 + (3+4+6+3)*3 +1`。  

## 总结

当对ResNet进行了透彻的分析后，其实很利于之后自己做网络改装，比如把你之前的一些小网络也加入一个`F(x)+x`的结构，甚至是`F(x)*x`（可以参考注意力机制）的结构，亦或者把ResNet里面的几层拿出来做你的预训练，都是非常有利的。  

#本文章是转载`小新蜡笔`的ResNet结构解析及pytorch代码。

原文连接：https://zhuanlan.zhihu.com/p/94439904



