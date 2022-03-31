 

![你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (下)](https://pic2.zhimg.com/v2-dde47004792ffb4259ccdb1783fa303e_1440w.jpg?source=172ae18b)

# 你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (下)

[![科技猛兽](https://pic2.zhimg.com/v2-7519f1e718ed6230763867ab473b3e96_xs.jpg?source=172ae18b)](https://www.zhihu.com/people/wang-jia-hao-53-3)

[科技猛兽](https://www.zhihu.com/people/wang-jia-hao-53-3)[](https://www.zhihu.com/question/48510028)



清华大学 自动化系硕士在读

**本文原创，转载请注明出处。**

上篇文章我们介绍了YOLO v2 v3 v4 v5对于**检测头head**和**损失函数loss**的优化，如果你忘记了，请看下面的链接**并点赞**。本文是YOLO系列的最后一篇文章了，会介绍YOLO这个系列对于**backbone**方面的优化和对**输入端的改进**：

[科技猛兽：你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (上)2777 赞同 · 59 评论文章![img](https://pic3.zhimg.com/v2-dde47004792ffb4259ccdb1783fa303e_ipico.jpg)](https://zhuanlan.zhihu.com/p/183261974)

[科技猛兽：你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (中)1073 赞同 · 27 评论文章![img](https://pic3.zhimg.com/v2-dde47004792ffb4259ccdb1783fa303e_ipico.jpg)](https://zhuanlan.zhihu.com/p/183781646)

为了使本文**尽量生动有趣**，我仍然和以前一样使用葫芦娃作为例子展示YOLO的过程(真的是尽力了。。。)。

![img](https://pic2.zhimg.com/80/v2-6f33f7b365e952d0cb1436c540135a0d_720w.jpg)葫芦娃



下面进入正题，目标检测器模型的结构如下图1所示，之前看过了YOLO v2 v3 v4 v5对于检测头和loss函数的改进，如下图2所示，下面着重介绍backbone的改进：

![img](https://pic1.zhimg.com/80/v2-8c5bac2266cf9b9b857529805511221c_720w.jpg)图1：检测器的结构

![img](https://pic2.zhimg.com/80/v2-b641aebaf5d009504d30a1d17a5f7019_720w.jpg)图2：YOLO系列比较

我们发现YOLO v1只是把最后的特征分成了 ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7) 个grid，到了YOLO v2就变成了 ![[公式]](https://www.zhihu.com/equation?tex=13%5Ctimes13) 个grid，再到YOLO v3 v4 v5就变成了多尺度的**(strides=8,16,32)**，更加复杂了。那**为什么一代比一代检测头更加复杂呢？答案是：因为它们的提特征网络更加强大了，能够支撑起检测头做更加复杂的操作。**换句话说，如果没有backbone方面的优化，你即使用这么复杂的检测头，可能性能还会更弱。所以引出了今天的话题：

- **backbone的改进：**

## **YOLO v1**

我们先看看YOLO v1的backbone长什么样子：

![img](https://pic2.zhimg.com/80/v2-70005c21ddd8470ae5587b231987cb7d_720w.jpg)YOLO v1 backbone

最后2层是全连接层，其他使用了大量的卷积层，网络逐渐变宽，是非常标准化的操作。注意这里面试官可能会问你一个问题：为什么都是卷积，图上要分开画出来，不写在一起？答案是：按照feature map的分辨率画出来。分辨率A变化到分辨率B的所有卷积画在了一起。因为写代码时经常会这么做，所以问这个问题的意图是看看你是否经常写代码。



然后我们看下检测类网络的结构，如下图3所示，这个图是YOLO v4中总结的：

![img](https://pic1.zhimg.com/80/v2-d2fb92212cbfb1d29bc395000602231c_720w.jpg)图3：检测类网络的结构

YOLO v1没有Neck，Backbone是GoogLeNet，属于Dense Prediction。**1阶段的检测器属于Dense Prediction，而2阶段的检测器既有Dense Prediction，又有Sparse Prediction**。

------

## **YOLO v2**

为了进一步提升性能，YOLO v2重新训练了一个darknet-19，如下图4所示：

![img](https://pic1.zhimg.com/80/v2-3ea70cd81cb6103ad41e9bb01b3114e4_720w.jpg)图4：darknet-19

仔细观察上面的backbone的结构(双横线上方)，提出3个问题：

- 为什么没有 ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7) 卷积了？只剩下了 ![[公式]](https://www.zhihu.com/equation?tex=3%5Ctimes3) 卷积和 ![[公式]](https://www.zhihu.com/equation?tex=1%5Ctimes1) 卷积了？

**答：**vgg net论文得到一个结论，![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7) 卷积可以用更小的卷积代替，且![[公式]](https://www.zhihu.com/equation?tex=3%5Ctimes3) 卷积更加节约参数，使模型更小。

网络可以做得更深，更好地提取到特征。为什么？因为每做一次卷积，后面都会接一个非线性的激活函数，更深意味着非线性的能力更强了。所以，你可能以后再也见不到 ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7) 卷积了。

另外还用了bottleneck结构(红色框)：

![[公式]](https://www.zhihu.com/equation?tex=3%5Ctimes3) 卷积负责扩大感受野， ![[公式]](https://www.zhihu.com/equation?tex=1%5Ctimes1) 卷积负责减少参数量。

- 为什么没有FC层了？

**答：**使用了GAP(Global Average Pooling)层，把 ![[公式]](https://www.zhihu.com/equation?tex=1000%5Ctimes7%5Ctimes7) 映射为 ![[公式]](https://www.zhihu.com/equation?tex=1000%5Ctimes1) ，满足了输入不同尺度的image的需求。你不管输入图片是 ![[公式]](https://www.zhihu.com/equation?tex=224%5Ctimes224) 还是 ![[公式]](https://www.zhihu.com/equation?tex=256%5Ctimes256) ，最后都给你映射为 ![[公式]](https://www.zhihu.com/equation?tex=1000%5Ctimes1)。

这对提高检测器的性能有什么作用呢？

对于小目标的检测，之前输入图片是固定的大小的，小目标很难被检测准确；现在允许多尺度输入图片了，只要把图片放大，小目标就变成了大目标，提高检测的精度。

- 为什么最后一层是softmax？

**答：**因为backbone网络darknet-19是单独train的，是按照分类网络去train的，用的数据集是imagenet，是1000个classes，所以最后加了一个softmax层，使用cross entropy loss。

**接下来总结下YOLO v2的网络结构：**

- 图4中的双横线的上半部分(第0-22层)为backbone，train的方法如上文。
- 后面的结构如下图5所示：

![img](https://pic3.zhimg.com/80/v2-1498b3c3c5ebebbb31b871329bc97b2e_720w.jpg)图5：YOLO v2网络结构

- 从第23层开始为检测头，其实是3个 3 * 3 * 1024 的卷积层。
- 同时增加了一个 passthrough 层(27层)，最后使用 1 * 1 卷积层输出预测结果，输出结果的size为 ![[公式]](https://www.zhihu.com/equation?tex=13%5Ctimes13%5Ctimes125) 。
- route层的作用是进行层的合并(concat)，后面的数字指的是合并谁和谁。
- passthrough 层可以把 ![[公式]](https://www.zhihu.com/equation?tex=26%5Ctimes26%5Ctimes64%5Crightarrow13%5Ctimes13%5Ctimes256) 。

**YOLO2 的训练主要包括三个阶段：**

1. 先在 **ImageNet 分类数据集**上**预训练 Darknet-19**，此时模型输入为 **224 \* 224** ，共训练 160 个 epochs。(为什么可以这样训练？因为有GAP)
2. 将网络的输入调整为 **448 \* 448**（注意在测试的时候使用 **416 \* 416** 大小） ，继续在 ImageNet 数据集上 **finetune 分类模型**，训练 10 个 epochs。注意为什么测试的时候使用大小？**答案是：**将输入图像大小从**448 ×448** 更改为 **416 ×416** 。这将创建**奇数空间维度(**7×7 v.s 8 ×8 **grid cell)**。 图片的中心通常被大目标占据。 对于奇数网格单元，可以更容易确定目标所属的位置。对于一些大目标，它们中心点往落入图片中心位置，此时使用特征图中心的**1个cell**去预测这些目标的bounding box相对容易些，否则就要用中间**4个Cells**来进行预测。
3. 修改 Darknet-19 分类模型为检测模型为图5形态，即：移除最后一个卷积层、global avgpooling 层以及 softmax  层，并且新增了3个 3 * 3 * 1024 卷积层，同时增加了一个 passthrough 层，最后使用 1 * 1 卷积层输出预测结果，并在**检测数据集**上继续**finetune** 网络。
4. YOLO v1的训练：先使用**224 \* 224**的分辨率训练分类网络，再切换到**448 \* 448**的分辨率训练检测网络。而YOLO v2在使用**224 \* 224**的分辨率训练分类网络**160 epochs**之后，先使用**448 \* 448**的分辨率finetune分类网络**10 epochs**，再使用**448 \* 448**的分辨率训练检测网络。可提升4%mAP。**为什么可以改变输入维度了？**因为YOLO v2中使用了GAP(Global Average Pooling)不论输入图片多大最后都给你从 ![[公式]](https://www.zhihu.com/equation?tex=1000%5Ctimes7%5Ctimes7%5Crightarrow1000%5Ctimes1) ，进行分类训练。

> 注意这里图5有个地方得解释一下：第25层把第16层进行reorg，即passthrough操作，得到的结果为27层，再与第24层进行route，即concat操作，得到第28层。

**可视化的图为：**

![img](https://pic3.zhimg.com/80/v2-33a0c926dc46085ac913b5262edb3a0a_720w.jpg)图5：YOLO v2可视化结构

## YOLO v3

先看下YOLO v3的backbone，如下图6所示：

![img](https://pic2.zhimg.com/80/v2-2bc3e653ec7744e36e45960d0cc060c9_720w.jpg)图6：YOLO v3 backbone

先声明下darknet 53指的是convolution层有52层+1个conv层把1024个channel调整为1000个，你会发现YOLO v2中使用的GAP层在YOLO v3中还在用，他还是在ImageNet上先train的backbone，

观察发现依然是有bottleneck的结构和**残差网络**。

**为什么**YOLO v3敢用3个检测头**？因为**他的backbone更强大了。

**为什么**更强大了**？因为**当时已经出现了ResNet结构。

所以YOLO v3的提高，有一部分功劳应该给ResNet。



再观察发现YOLO v3没有Pooling layer了，用的是conv(stride = 2)进行下采样，**为什么？**

**因为**Pooling layer，不管是MaxPooling还是Average Pooling，本质上都是下采样减少计算量，本质上就是不更新参数的conv，但是他们会损失信息，所以用的是conv(stride = 2)进行下采样。

下图7是YOLO v3的网络结构：

![img](https://pic2.zhimg.com/80/v2-997f8c0bc3bde01329a52ba6afa49cfd_720w.jpg)图7：YOLO v3 Structure

![img](https://pic4.zhimg.com/80/v2-3946e19705fcabb730daf30f9cd1408f_720w.jpg)图8：YOLO v3 Structure

特征融合的方式更加直接，没有YOLO v2的passthrough操作，直接上采样之后concat在一起。

------

## YOLO v4

图9,10展示了YOLO v4的结构：

![img](https://pic2.zhimg.com/80/v2-0ea4884cd89aaf5b87ba1464d02f358d_720w.jpg)图9：YOLO v4 Structure

![img](https://pic3.zhimg.com/80/v2-05a713ffc8cee40dbaa3b808aa63dcea_720w.jpg)图10：YOLO v4 Structure

![img](https://pic1.zhimg.com/80/v2-c70a13fe3c658d20e9d11a1880619e70_720w.jpg)图11：YOLO v4 Structure

Yolov4的结构图和Yolov3相比，因为多了**CSP结构，PAN结构**，如果单纯看可视化流程图，会觉得很绕，不过在绘制出上面的图形后，会觉得豁然开朗，其实整体架构和Yolov3是相同的，不过使用各种新的算法思想对各个子结构都进行了改进。



**YOLOv4使用了CSPDarknet53作为backbone，加上SPP模块，PANET作为neck，以及YOLO v3的head。**



**Yolov4的五个基本组件**：

1. **CBM：**Yolov4网络结构中的最小组件，由Conv+Bn+Mish激活函数三者组成。
2. **CBL：**由Conv+Bn+Leaky_relu激活函数三者组成。
3. **Res unit：**借鉴Resnet网络中的残差结构，让网络可以构建的更深。
4. **CSPX：**借鉴CSPNet网络结构，由三个卷积层和X个Res unint模块Concate组成。
5. **SPP：**采用1×1，5×5，9×9，13×13的最大池化的方式，进行多尺度融合。

**其他基础操作：**

1. **Concat：**张量拼接，维度会扩充，和Yolov3中的解释一样，对应于cfg文件中的route操作。
2. **add：**张量相加，不会扩充维度，对应于cfg文件中的shortcut操作。

**Backbone中卷积层的数量：**

和Yolov3一样，再来数一下Backbone里面的卷积层数量。

每个CSPX中包含3+2*X个卷积层，因此整个主干网络Backbone中一共包含2+（3+2*1）+2+（3+2*2）+2+（3+2*8）+2+（3+2*8）+2+（3+2*4）+1=72。



- **输入端的改进：**

YOLO v4对输入端进行了改进，主要包括**数据增强Mosaic、cmBN、SAT自对抗训练**，使得在卡不是很多时也能取得不错的结果。

这里介绍下数据增强Mosaic：

![img](https://pic1.zhimg.com/80/v2-9ddb5f309a28e99f6c1901ec908923e4_720w.jpg)Mosaic数据增强

**CutMix**只使用了两张图片进行拼接，而**Mosaic数据增强**则采用了4张图片，**随机缩放、随机裁剪、随机排布**的方式进行拼接。

Yolov4的作者采用了**Mosaic数据增强**的方式。

主要有几个优点：

1. **丰富数据集：**随机使用**4张图片**，随机缩放，再随机分布进行拼接，大大丰富了检测数据集，特别是随机缩放增加了很多小目标，让网络的鲁棒性更好。
2. **减少GPU：**可能会有人说，随机缩放，普通的数据增强也可以做，但作者考虑到很多人可能只有一个GPU，因此Mosaic增强训练时，可以直接计算4张图片的数据，使得Mini-batch大小并不需要很大，一个GPU就可以达到比较好的效果。

cmBN的方法如下图：

![img](https://pic2.zhimg.com/80/v2-bc8cfe6198d8c2a0d8a3a646a27a6e0d_720w.jpg)cmBN的方法

------

## YOLO v5

图12,13展示了YOLO v5的结构：

![img](https://pic2.zhimg.com/80/v2-2ca7dfc26a64e7ca0e5e8b21c87ed13d_720w.jpg)图12：YOLO v5 Structure

![img](https://pic2.zhimg.com/80/v2-31ca3528d3bb53273f74a23b5b59ab51_720w.jpg)图13：YOLO v5 Structure

检测头的结构基本上是一样的，融合方法也是一样。

**Yolov5的基本组件**：

1. **Focus：**基本上就是YOLO v2的passthrough。
2. **CBL：**由Conv+Bn+Leaky_relu激活函数三者组成。
3. **CSP1_X：**借鉴CSPNet网络结构，由三个卷积层和X个Res unint模块Concate组成。
4. **CSP2_X：**不再用Res unint模块，而是改为CBL。
5. **SPP：**采用1×1，5×5，9×9，13×13的最大池化的方式，进行多尺度融合，如图13所示。

提特征的网络变短了，速度更快。YOLO v5的结构没有定下来，作者的代码还在持续更新。

## **YOLO v5的四种结构的深度：**

下图展示了YOLO v5的四种结构：

![img](https://pic3.zhimg.com/80/v2-6c383a7d5fdafda4a18799b1a1e7585e_720w.jpg)图14：YOLO四种结构深度

Yolov5代码中，每个网络结构的两个参数：

**（1）Yolov5s.yaml**

```python3
depth_multiple: 0.33  # model depth multiple
width_multiple: 0.50  # layer channel multiple
```

**（2）Yolov5m.yaml**

```python3
depth_multiple: 0.67  # model depth multiple
width_multiple: 0.75  # layer channel multiple
```

**（3）Yolov5l.yaml**

```python3
depth_multiple: 1.0  # model depth multiple
width_multiple: 1.0  # layer channel multiple
```

**（4）Yolov5x.yaml**

```python3
depth_multiple: 1.33  # model depth multiple
width_multiple: 1.25  # layer channel multiple
```

四种结构就是通过上面的两个参数，来进行控制网络的**深度**和**宽度**。其中**depth_multiple**控制网络的**深度**，**width_multiple**控制网络的**宽度**。

### Yolov5网络结构

四种结构的yaml文件中，下方的网络架构代码都是一样的。

将Backbone部分提取出来，讲解如何控制网络的宽度和深度，yaml文件中的Head部分也是同样的原理。

```python3
# YOLOv5 backbone
backbone:
  # [from, number, module, args]
  [[-1, 1, Focus, [64, 3]],  # 0-P1/2
   [-1, 1, Conv, [128, 3, 2]],  # 1-P2/4
   [-1, 3, BottleneckCSP, [128]],
   [-1, 1, Conv, [256, 3, 2]],  # 3-P3/8
   [-1, 9, BottleneckCSP, [256]],
   [-1, 1, Conv, [512, 3, 2]],  # 5-P4/16
   [-1, 9, BottleneckCSP, [512]],
   [-1, 1, Conv, [1024, 3, 2]],  # 7-P5/32
   [-1, 1, SPP, [1024, [5, 9, 13]]],
   [-1, 3, BottleneckCSP, [1024, False]],  # 9
  ]
```

在对网络结构进行解析时，yolo.py中下方的这一行代码将四种结构的**depth_multiple**，**width_multiple**提取出，赋值给**gd，gw**。后面主要对这**gd，gw**这两个参数进行讲解。

```python3
anchors, nc, gd, gw = d['anchors'], d['nc'], d['depth_multiple'], d['width_multiple']
```

下面再细致的剖析下，看是如何控制每种结构，深度和宽度的。

**1) 不同网络的深度**

在上图中有2种结构：CSP1和CSP2，其中CSP1结构主要应用于**Backbone**中，CSP2结构主要应用于**Neck**中。

**需要注意的是，四种网络结构中每个CSP结构的深度都是不同的。**

a.以yolov5s为例，第一个CSP1中，使用了**1个残差组件**，因此是**CSP1_1**。而在Yolov5m中，则增加了网络的深度，在第一个CSP1中，使用了**2个残差组件**，因此是**CSP1_2**。

而Yolov5l中，同样的位置，则使用了**3个残差组件**，Yolov5x中，使用了**4个残差组件**。

其余的第二个CSP1和第三个CSP1也是同样的原理。

b.在第二种CSP2结构中也是同样的方式，以第一个CSP2结构为例，Yolov5s组件中使用了**1组**卷积，因此是**CSP2_1**。

而Yolov5m中使用了**2组**，Yolov5l中使用了**3组**，Yolov5x中使用了**4组。**

其他的四个CSP2结构，也是同理。

Yolov5中，网络的不断加深，也在不断**增加网络特征提取**和**特征融合**的能力。

**2) 控制深度的代码**

控制四种网络结构的核心代码是**yolo.py**中下面的代码，存在两个变量，**n和gd**。

我们再将**n和gd**带入计算，看每种网络的变化结果。

```python3
n = max(round(n * gd), 1) if n > 1 else n  # depth gain
```

**3) 验证控制深度的有效性**

我们选择**最小的yolov5s.yaml**和中间的**yolov5l.yaml**两个网络结构，将**gd(height_multiple)**系数带入，看是否正确。

![img](https://pic1.zhimg.com/80/v2-2bc10e474354757a4d7947e219f56cc0_720w.jpg)

**a. yolov5x.yaml**

其中**height_multiple=0.33**，即**gd=0.33**，而n则由上面红色框中的信息获得。

以上面网络框图中的第一个CSP1为例，即上面的第一个红色框。n等于第二个数值3。

而**gd=0.33**，带入（2）中的计算代码，结果n=1。因此第一个CSP1结构内只有1个残差组件，即CSP1_1。

第二个CSP1结构中，n等于第二个数值9，而**gd=0.33**，带入（2）中计算，结果**n=3**，因此第二个CSP1结构中有3个残差组件，即CSP1_3。

第三个CSP1结构也是同理，这里不多说。

**b. yolov5l.xml**

其中**height_multiple=1**，即**gd=1**

和上面的计算方式相同，第一个CSP1结构中，n=1，带入代码中，结果n=3，因此为CSP1_3。

下面第二个CSP1和第三个CSP1结构都是同样的原理。

## YOLO v5的四种结构的宽度：

![img](https://pic3.zhimg.com/80/v2-76ec41e464f5ce3ce103136837937112_720w.jpg)图15：YOLO四种结构宽度

**1) 不同网络的宽度:**

如上图表格中所示，四种yolov5结构在不同阶段的卷积核的数量都是不一样的，因此也直接影响卷积后特征图的第三维度，即**宽度**。

a.以Yolo v5s结构为例，第一个Focus结构中，最后卷积操作时，卷积核的数量是32个，因此经过**Focus结构**，特征图的大小变成**304\*304\*32**。

而Yolo v5m的**Focus结构**中的卷积操作使用了48个卷积核，因此**Focus结构**后的特征图变成3**04\*304\*48**。yolov5l，yolov5x也是同样的原理。

b. 第二个卷积操作时，Yolo v5s使用了64个卷积核，因此得到的特征图是**152\*152\*64**。而yolov5m使用96个特征图，因此得到的特征图是**152\*152\*96**。Yolo v5l，Yolo v5x也是同理。

c. 后面三个卷积下采样操作也是同样的原理。

四种不同结构的卷积核的数量不同，这也直接影响网络中，比如**CSP1，CSP2等结构**，以及各个普通卷积，卷积操作时的卷积核数量也同步在调整，影响整体网络的计算量。

当然卷积核的数量越多，特征图的厚度，即**宽度越宽**，网络提取特征的**学习能力也越强**。

2) **控制宽度的代码**

在Yolo v5的代码中，控制宽度的核心代码是**yolo.py**文件里面的这一行：

```adl
c2 = make_divisible(c2 * gw, 8) if c2 != no else c2
```

它所调用的子函数**make_divisible**是一个很出名的函数，在各大任务中均会使用，作用是：

**make_divisible(A,B)：**找到比A大的，能整除B的最小整数。

> make_divisible(54,8) = 56

**3) 验证控制宽度的有效性**

我们还是选择**最小的Yolo v5s**和**中间的Yolo v5l**两个网络结构，将**width_multiple**系数带入，看是否正确。

![img](https://pic2.zhimg.com/80/v2-7544dc64d6cfce2fb0dbe69bec73e3e1_720w.jpg)图16：backbone

**a. Yolo v5x.yaml**

其中**width_multiple=0.5**，即**gw=0.5**。

以第一个卷积下采样为例，即Focus结构中下面的卷积操作。

按照上面Backbone的信息，我们知道Focus中，标准的c2=64，而**gw=0.5**，代入（2）中的计算公式，最后的结果=32。即Yolo v5s的Focus结构中，卷积下采样操作的卷积核数量为**32个。**

再计算后面的第二个卷积下采样操作，标准c2的值=128，**gw=0.5**，代入（2）中公式，最后的结果=64，也是正确的。

**b. Yolo v5l.yaml**

其中**width_multiple=1**，即**gw=1**，而标准的**c2=64**，代入上面（2）的计算公式中，可以得到Yolo v5l的Focus结构中，卷积下采样操作的卷积核的数量为64个，而第二个卷积下采样的卷积核数量是128个。

另外的三个卷积下采样操作，以及**Yolo v5m，Yolo v5x结构**也是同样的计算方式。

比如：YOLO V5s默认depth_multiple=0.33， width_multiple=0.50。即BottleneckCSP中Bottleneck的数量为**默认的1/3**，而所有卷积操作的卷积核个数均为**默认的1/2。**

![img](https://pic4.zhimg.com/80/v2-c1422e55da6ba5030371f0111d8d4e8f_720w.jpg)

![img](https://pic4.zhimg.com/80/v2-4c47b276ff2f51fb59a056e961666f67_720w.jpg)图17：SPP结构

**Focus操作如下图所示：**

![img](https://pic1.zhimg.com/80/v2-560db4a00dd21975f64e7addff498ecc_720w.jpg)图18：Focus操作

**Focus**的**slice**操作如下图所示：

![img](https://pic3.zhimg.com/80/v2-eeec8b5b35b9ab4f096dfbfb229ea1fa_720w.jpg)图19：slice操作

这个其实就是**Yolo v2**里面的**ReOrg+Conv**操作，也是亚像素卷积的反向操作版本，简单来说就是把数据切分为4份，每份数据都是相当于2倍下采样得到的，然后在channel维度进行拼接，最后进行卷积操作。其最大好处是可以**最大程度的减少信息损失而进行下采样操作**。

**YOLO v5s**默认**3x640x640**的输入，复制四份，然后通过切片操作将这个四个图片切成了四个**3x320x320**的切片，接下来使用concat从深度上连接这四个切片，输出为**12x320x320**，之后再通过卷积核数为32的卷积层，生成**32x320x320**的输出，最后经过batch_borm 和leaky_relu将结果输入到下一个卷积层。

**Focus的代码如下：**

```python3
class Focus(nn.Module):
    # Focus wh information into c-space
    def __init__(self, c1, c2, k=1, s=1, p=None, g=1, act=True):  # ch_in, ch_out, kernel, stride, padding, groups
        super(Focus, self).__init__()
        self.conv = Conv(c1 * 4, c2, k, s, p, g, act)

    def forward(self, x):  # x(b,c,w,h) -> y(b,4c,w/2,h/2)
        return self.conv(torch.cat([x[..., ::2, ::2], x[..., 1::2, ::2], x[..., ::2, 1::2], x[..., 1::2, 1::2]], 1))
```





这里解释以下**PANET**结构是什么意思，PAN结构来自论文Path Aggregation Network，可视化结果如图19所示：

![img](https://pic3.zhimg.com/80/v2-93d93db2c7f60ebf3fa5d894d1b446f2_720w.jpg)图20：PAN结构

可以看到包含了自底向上和自顶向下的连接，值得注意的是这里的红色虚线和绿色虚线：

FPN的结构把浅层特征传递给顶层要经历**几十甚至上百层**，显然经过这么多层的传递，浅层信息(小目标)丢失比较厉害。这里的**红色虚线**就象征着ResNet的几十甚至上百层。

**自下而上的路径**由**不到10层**组成，**浅层特征**经过FPN的**laterial connection**连接到 ![[公式]](https://www.zhihu.com/equation?tex=P_%7B2%7D) ，再经过**bottom-up path augmentation**连接到顶层，经过的层数不到10层，能较好地保留浅层的信息。这里的**绿色虚线**就象征着自下而上的路径的不到10层。

YOLO V5借鉴了YOLO V4的**修改版PANET**结构。

**PANET**通常使用自适应特征池将相邻层**加**在一起，以进行掩模预测。但是，当在YOLO v4中使用PANET时，此方法略麻烦，因此，YOLO v4的作者没有使用自适应特征池添加相邻层，而是对其进行**Concat**操作，从而提高了预测的准确性。

![img](https://pic4.zhimg.com/80/v2-98fce9f04482ad10b4fb0c04281362df_720w.jpg)图21：Modified PAN



- **输入端的改进：**

**1.Mosaic数据增强，和YOLO v4一样。**

**2.自适应锚框计算：**

在Yolo算法中，针对不同的数据集，都会有**初始设定长宽的锚框**。

在网络训练中，网络在初始锚框的基础上输出预测框，进而和**真实框groundtruth**进行比对，计算两者差距，再反向更新，**迭代网络参数**。

因此初始锚框也是比较重要的一部分，比如Yolov5在Coco数据集上初始设定的锚框：

![img](https://pic1.zhimg.com/80/v2-f8a55d48eb2003b47629f4e92d7ab5a8_720w.jpg)Yolov5在Coco数据集上初始设定的锚框

在Yolov3、Yolov4中，训练不同的数据集时，计算初始锚框的值是通过单独的程序运行的。

但Yolov5中将此功能嵌入到代码中，每次训练时，自适应的计算不同训练集中的最佳锚框值。

当然，如果觉得计算的锚框效果不是很好，也可以在代码中将自动计算锚框功能**关闭**。

```python3
parser.add_argument('--noautoanchor', action='store_true', help='disable autoanchor check')
```

设置成**False**，每次训练时，不会自动计算。

**3.自适应图片缩放**

在常用的目标检测算法中，不同的图片长宽都不相同，因此常用的方式是将原始图片统一缩放到一个标准尺寸，再送入检测网络中。

比如Yolo算法中常用**416\*416，608\*608**等尺寸，比如对下面**800\*600**的图像进行缩放:，如图15所示：

![img](https://pic3.zhimg.com/80/v2-21213f06cfd4b9025ac0aec29e2e8a82_720w.jpg)图22：Yolo算法的缩放填充

但**Yolov5代码**中对此进行了改进，也是**Yolov5推理速度**能够很快的一个不错的trick。

作者认为，在项目实际使用时，很多图片的长宽比不同，因此缩放填充后，两端的黑边大小都不同，而如果**填充的比较多**，则存在**信息冗余**，影响**推理速度**。

因此在Yolov5的代码中datasets.py的letterbox函数中进行了修改，对原始图像**自适应的添加最少的黑边**，如图16所示：

![img](https://pic4.zhimg.com/80/v2-d9d3685aae6ba2f99e1c7c27a4f1ae53_720w.jpg)图22：YOLO v5的自适应填充

图像高度上两端的黑边变少了，在推理时，计算量也会减少，即目标检测速度会得到提升。

通过这种简单的改进，推理速度得到了37%的提升，可以说效果很明显。

Yolov5中填充的是灰色，即（114,114,114），都是一样的效果，且训练时没有采用缩减黑边的方式，还是采用传统填充的方式，即缩放到416*416大小。只是在测试，使用模型推理时，才采用缩减黑边的方式，提高目标检测，推理的速度。



**关于样本：**

yolov5的很大区别就是在于正样本区域的定义。**在yolov3中，每一个** ![[公式]](https://www.zhihu.com/equation?tex=GT) **只有1个对应的anchor，**设定的规则是取 ![[公式]](https://www.zhihu.com/equation?tex=IoU%28anchor%2CGT%29) 最大的anchor。而且某个 ![[公式]](https://www.zhihu.com/equation?tex=GT) 一定不可能在三个预测层的某几层上同时进行匹配。这样就会导致一个问题：正样本的anchor太少了，导致整体收敛比较慢。后面的FCOS等工作研究表明：**增加高质量正样本anchor可以显著加速收敛**。



YOLO v5和前YOLO系列相比，特点应该是：

- **(1) 增加了正样本：方法是邻域的正样本anchor匹配策略。**
- **(2) 通过灵活的配置参数，可以得到不同复杂度的模型**
- **(3) 通过一些内置的超参优化策略，提升整体性能**
- **(4) 和yolov4一样，都用了mosaic增强，提升小物体检测性能**

------

最后我们对YOLO series总的做个比较，结束这个系列的解读，喜欢的同学点赞关注评论3连：

![img](https://pic1.zhimg.com/80/v2-ba712736fadbf364cc2fec97dd374f98_720w.jpg)YOLO v5 series的比较



**参考和部分图源：**

[江大白：深入浅出Yolo系列之Yolov5核心基础知识完整讲解2141 赞同 · 383 评论文章![img](https://pic2.zhimg.com/v2-c28dc6c85854680a7fdfa84035ba6fe5_180x120.jpg)](https://zhuanlan.zhihu.com/p/172121380)

[江大白：深入浅出Yolo系列之Yolov3&Yolov4&Yolov5核心基础知识完整讲解2735 赞同 · 337 评论文章![img](https://pic1.zhimg.com/v2-c9293cf2976e5bba7ec721c01a365af4_180x120.jpg)](https://zhuanlan.zhihu.com/p/143747206)



编辑于 2020-08-21 18:00
