 

![你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (中)](https://pic3.zhimg.com/v2-dde47004792ffb4259ccdb1783fa303e_1440w.jpg?source=172ae18b)

# 你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (中)

[![科技猛兽](https://pic2.zhimg.com/v2-7519f1e718ed6230763867ab473b3e96_xs.jpg?source=172ae18b)](https://www.zhihu.com/people/wang-jia-hao-53-3)

[科技猛兽](https://www.zhihu.com/people/wang-jia-hao-53-3)[](https://www.zhihu.com/question/48510028)



清华大学 自动化系硕士在读

**本文原创，转载请注明出处。**

上篇文章我们介绍了YOLO v1的设计和演变过程**(下面的链接)**，从本文开始我们继续介绍YOLO series接下来的工作，但是因为YOLO下面的工作内容太多，所以本文只介绍YOLO v2 v3 v4 v5对于**检测头head**和**损失函数loss**的优化，剩下的**backbone**方面的优化留到下一篇文章吧。

[科技猛兽：你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (上)2777 赞同 · 59 评论文章![img](https://pic3.zhimg.com/v2-dde47004792ffb4259ccdb1783fa303e_ipico.jpg)](https://zhuanlan.zhihu.com/p/183261974)

为了使本文**尽量生动有趣**，我仍然用葫芦娃作为例子展示YOLO的过程(真的是尽力了。。。)。

![img](https://pic2.zhimg.com/80/v2-6f33f7b365e952d0cb1436c540135a0d_720w.jpg)葫芦娃



下面进入正题，首先回顾下YOLO v1的模型结构，忘记了的同学请看上面的文章**并点赞**，如下面2图所示：

![img](https://pic1.zhimg.com/80/v2-adc371c1de58af018e9b097f73105b4c_720w.jpg)YOLO

![img](https://pic2.zhimg.com/80/v2-3c018cd25d7e0f696bdf78e64da76655_720w.jpg)YOLO

我们认为，**检测模型=特征提取器+检测头**

在YOLO v1的模型中检测头就是最后的2个全连接层(Linear in PyTorch)，它们是参数量最大的2个层，也是最值得改进的2个层。后面的YOLO模型都对这里进行改进：

YOLO v1一共预测49个目标，一共98个框。

## 5 YOLO v2

- **检测头的改进：**

YOLO v1虽然快，但是预测的框不准确，很多目标找不到：

- **预测的框不准确：准确度不足。**
- **很多目标找不到：recall不足。**

我们一个问题一个问题解决，首先第1个：

- **问题1：预测的框不准确：**

**当时别人是怎么做的？**

同时代的检测器有R-CNN，人家预测的是偏移量。

什么是偏移量？

![img](https://pic4.zhimg.com/80/v2-4883b178ed0e2bb95f1d504dc6bed6a7_720w.jpg)YOLO v2

之前YOLO v1直接预测x,y,w,h，范围比较大，现在我们想预测一个稍微小一点的值，来增加准确度。

不得不先介绍2个新概念：**基于grid的偏移量和基于anchor的偏移量**。什么意思呢？

**基于anchor的偏移量**的意思是，anchor的位置是固定的，**偏移量=目标位置-anchor的位置**。

**基于grid的偏移量**的意思是，grid的位置是固定的，**偏移量=目标位置-grid的位置**。



**Anchor是什么玩意？**

Anchor是R-CNN系列的一个概念，你可以把它理解为一个预先定义好的框，它的位置，宽高都是已知的，是一个参照物，供我们预测时参考。



上面的图就是YOLO v2给出的改进，你可能现在看得一脸懵逼，我先解释下各个字母的含义：

![[公式]](https://www.zhihu.com/equation?tex=b_%7Bx%7D%2Cb_%7By%7D%2Cb_%7Bw%7D%2Cb_%7Bh%7D) ：模型最终得到的的检测结果。

![[公式]](https://www.zhihu.com/equation?tex=t_%7Bx%7D%2Ct_%7By%7D%2Ct_%7Bw%7D%2Ct_%7Bh%7D) ：模型要预测的值。

![[公式]](https://www.zhihu.com/equation?tex=c_%7Bx%7D%2Cc_%7By%7D) ：grid的左上角坐标，如下图所示。

![[公式]](https://www.zhihu.com/equation?tex=p_%7Bw%7D%2Cp_%7Bh%7D) ：Anchor的宽和高，这里的anchor是人为定好的一个框，宽和高是固定的。

![img](https://pic1.zhimg.com/80/v2-cc3885ebc9f24892cf30c58c564044c0_720w.jpg)

通过这样的定义我们从**直接预测位置**改为**预测一个偏移量**，基于**Anchor框的宽和高**和**grid的先验位置**的**偏移量**，得到最终目标的位置，这种方法也叫作**location prediction**。

**这里还涉及到一个尺寸问题：**

刚才说到 ![[公式]](https://www.zhihu.com/equation?tex=t_%7Bx%7D%2Ct_%7By%7D%2Ct_%7Bw%7D%2Ct_%7Bh%7D) 是模型要预测的值，这里 ![[公式]](https://www.zhihu.com/equation?tex=c_%7Bx%7D%2Cc_%7By%7D) 为grid的坐标，画个图就明白了：

![img](https://pic3.zhimg.com/80/v2-e31634384bb34bbbb1de0e8ddfc5ddc2_720w.jpg)图1：原始值

如图1所示，假设此图分为9个grid，GT如红色的框所示，Anchor如紫色的框所示。图中的数字为image的真实信息。

我们首先会对这些值**归一化**，结果如下图2所示：

![img](https://pic3.zhimg.com/80/v2-63ca4ed417c5db83b18c95a42a7f60f2_720w.jpg)图2：要预测的值

归一化之后你会发现，要预测的值就变为了：

![[公式]](https://www.zhihu.com/equation?tex=t_%7Bx%7D%2Ct_%7By%7D%2Ct_%7Bw%7D%2Ct_%7Bh%7D%3D0.172%2C-0.148%2C-0.340%2C-0.326) 

这是一个偏移量，且值很小，有利于神经网络的学习。



**你可能会有疑问：**为什么YOLO v2改预测偏移量而不是直接去预测 ![[公式]](https://www.zhihu.com/equation?tex=%28x%2Cy%2Cw%2Ch%29) ？

上面我说了作者看到了同时代的R-CNN，人家预测的是偏移量。另一个重要的原因是：直接预测位置会导致神经网络在一开始训练时不稳定，使用偏移量会使得训练过程更加稳定，性能指标提升了5%左右。

位置上不使用Anchor框，宽高上使用Anchor框。以上就是YOLO v2的一个改进。用了YOLO v2的改进之后确实是更准确了，但别激动，上面还有一个问题呢~



- **问题2：很多目标找不到：**

你还记得上一篇讲得**YOLO v1一次能检测多少个目标吗**？答案是**49个目标**，98个框，并且2个框对应一个类别。可以是大目标也可以是小目标。因为输出的尺寸是：[N, 7, 7, 30]。式中N为图片数量，7,7为49个区域(grid)。

![[公式]](https://www.zhihu.com/equation?tex=30%3D2%5Ctimes5%28c%2Cx%2Cy%2Cw%2Ch%29%2B1%5Ctimes20+classes) 

YOLO v2首先把 ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7) 个区域改为 ![[公式]](https://www.zhihu.com/equation?tex=13%5Ctimes13) 个区域，每个区域有5个anchor，且每个anchor对应着1个类别，那么，输出的尺寸就应该为：[N,13,13,125]。

![[公式]](https://www.zhihu.com/equation?tex=125%3D5%5Ctimes5%28c%2Cx%2Cy%2Cw%2Ch%29%2B5%5Ctimes20+classes) 



这里面有个bug，就是YOLO v2先对每个区域得到了5个anchor作为参考，那你就会问2个问题：

**1.为什么要用Anchor呢？**

**答：一开始YOLO v1的初始训练过程很不稳定**，在YOLO v2中，作者观察了很多图片的所有Ground Truth，发现：比如车，GT都是矮胖的长方形，再比如行人，GT都是瘦高的长方形，且宽高比具有相似性。那**能不能根据这一点，从数据集中预先准备几个几率比较大的bounding box，再以它们为基准进行预测呢？**这就是Anchor的初衷。

**2.每个区域的5个anchor是如何得到的？**

下图可以回答你的问题：

![img](https://pic1.zhimg.com/80/v2-7e157d61f41ca02634f06b0b78c71684_720w.jpg)methods to get the 5 anchor

方法：对于任意一个数据集，就比如说COCO吧(紫色的anchor)，先对训练集的GT bounding box进行聚类，聚成几类呢？作者进行了实验之后发现**5**类的**recall vs. complexity**比较好，现在聚成了**5**类，当然9类的mAP最好，预测的最全面，但是在复杂度上升很多的同时对模型的准确度提升不大，所以采用了一个比较折中的办法选取了5个聚类簇，即使用5个先验框。

所以到现在为止，有了anchor再结合刚才的 ![[公式]](https://www.zhihu.com/equation?tex=t_%7Bx%7D%2Ct_%7By%7D%2Ct_%7Bw%7D%2Ct_%7Bh%7D) ，就可以求出目标位置。

**anchor是从数据集中统计得到的(Faster-RCNN中的Anchor的宽高和大小是手动挑选的)。**



- **损失函数为：**

![img](https://pic1.zhimg.com/80/v2-faed5df5818795d5fc047815f0338768_720w.jpg)YOLO v2损失函数

> 这里的W=13,H=13,A=5。
> 每个 ![[公式]](https://www.zhihu.com/equation?tex=%5Clambda) 都是一个权重值。c表示类别，r表示rectangle，即(x,y,w,h)。
> 第1,4行是confidence_loss，注意这里的真值变成了0和IoU(GT, anchor)的值，你看看这些细节。。。
> 第5行是class_loss。
> 第2,3行：t是迭代次数，即前12800步我们计算这个损失，后面不计算了。这部分意义何在？
> 意思是：前12800步我们会优化**预测的(x,y,w,h)与anchor的(x,y,w,h)的距离+预测的(x,y,w,h)与GT的(x,y,w,h)的距离**，12800步之后就只优化**预测的(x,y,w,h)与GT的(x,y,w,h)的距离，为啥？因为这时的预测结果已经较为准确了，anchor已经满足我了我们了，而在一开始预测不准的时候，用上anchor可以加速训练。**
> **你看看这操作多么的细节。。。**
>  ![[公式]](https://www.zhihu.com/equation?tex=1_%7Bk%7D%5E%7Btruth%7D) 是什么？第k个anchor与所有GT的IoU的maximum，如果大于一个阈值，就 ![[公式]](https://www.zhihu.com/equation?tex=1_%7Bk%7D%5E%7Btruth%7D%3D1) ，否则的话： ![[公式]](https://www.zhihu.com/equation?tex=1_%7Bk%7D%5E%7Btruth%7D%3D0) 。 



好，到现在为止，YOLO v2做了这么多改进，整体性能大幅度提高，但是小目标检测仍然是YOLO v2的痛。直到kaiming大神的ResNet出现，backbone可以更深了，所以darknet53诞生。

最后我们做个比较：

![img](https://pic1.zhimg.com/80/v2-e4dce0794b6d4aa5a67133633baed6b4_720w.jpg)YOLO v1和v2的比较

------

## 6 YOLO v3

- **检测头的改进：**

之前在说小目标检测仍然是YOLO v2的痛，YOLO v3是如何改进的呢？如下图所示。

![img](https://pic1.zhimg.com/80/v2-4cf1b6f6afec393122305ca2bb2725a4_720w.jpg)YOLO v3

我们知道，YOLO v2的检测头已经由YOLO v1的 ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7) 变为 ![[公式]](https://www.zhihu.com/equation?tex=13%5Ctimes13)了，我们看YOLO v3检测头分叉了，分成了3部分：

- 13*13*3*(4+1+80)
- 26*26*3*(4+1+80)
- 52*52*3*(4+1+80)

预测的框更多更全面了，并且分级了。

我们发现3个分支分别为**32倍下采样，16倍下采样，8倍下采样**，分别取预测**大，中，小目标**。为什么这样子安排呢？

因为**32倍下采样**每个点感受野更大，所以去预测**大目标，8倍下采样**每个点感受野最小，所以去预测**小目标。专人专事。**

**发现预测地更准确了，性能又提升了。**

又有人会问，你现在是3个分支，我改成5个，6个分支会不会更好？

理论上会，但还是那句话，作者遵循recall vs. complexity的trade off。



图中的123456789是什么意思？

**答：框**。每个grid设置9个先验框，3个大的，3个中的，3个小的。

每个分支预测3个框，每个框预测5元组+80个one-hot vector类别，所以一共size是：

**3\*(4+1+80)**

**每个分支的输出size为：**

- **[13,13,3\*(4+1+80)]**
- **[26,26,3\*(4+1+80)]**
- **[52,52,3\*(4+1+80)]**

**当然你也可以用5个先验框，这时每个分支的输出size为：**

- **[13,13,5\*(4+1+80)]**
- **[26,26,5\*(4+1+80)]**
- **[52,52,5\*(4+1+80)]**

**读到这里，请你数一下YOLO v3可以预测多少个bounding box？**

![[公式]](https://www.zhihu.com/equation?tex=%2813%5Ctimes13%2B26%5Ctimes26%2B52%5Ctimes52%29%5Ctimes3%3D10467%28YOLO+v3%29%3E%3E845%28YOLO+v2%29%3D13%5Ctimes13%5Ctimes5) 

多了这么多可以预测的bounding box，模型的能力增强了。

**为确定priors**，YOLO v3 应用**k均值聚类**。然后它**预先选择9个聚类簇**。

对于**COCO数据集**，锚定框的宽度和高为**(10 ×13)，(16 ×30)，(33 ×23)，(30 ×61)，(62 ×45)，(59 ×119)，(116 × 90)，( 156 ×198)，(373373×326326)**。这应该是按照输入图像的尺寸416×416 计算得到的。这 9个priors根据它们的尺度分为3个不同的组。在检测目标时，给一特定的特征图分配一个组。



就对应了下面这个图：

![img](https://pic4.zhimg.com/80/v2-24ff3cd5062021493490e28e7d73d6bf_720w.jpg)YOLO v3

检测头是**DBL**，定义在图上，没有了FC。

还有一种画法，更加直观一点：

![img](https://pic3.zhimg.com/80/v2-085b6d95dc53894e5de4fe95d2249b06_720w.jpg)

![img](https://pic2.zhimg.com/80/v2-151886b99cf8a93f116cd845a4524c89_720w.jpg)YOLO v3 head

**anchor和YOLO v2一样，依然是从数据集中统计得到的。**



- **损失函数为：**

![img](https://pic4.zhimg.com/80/v2-1714579e2a7f9ca88335bdaeae9e1c4f_720w.jpg)YOLO v3损失函数

> 第4行说明：loss分3部分组成：
> 第1行代表geo_loss，S代表13,26,52，就是grid是几乘几的。B=5。
> 第2行代表confidence_loss，和YOLO v2一模一样。
> 第3行代表class_loss，和YOLO v2的区别是改成了交叉熵。



- **边界框预测和代价函数的计算：**

YOLO v3使用多标签分类，用多个独立的logistic分类器代替softmax函数，以计算输入属于特定标签的可能性。在计算分类损失进行训练时，YOLO v3对每个标签使用二元交叉熵损失。

**正负样本的确定：**

> 如果**边界框先验（锚定框）与 GT 目标比其他目标重叠多**，则相应的**目标性得分应为 1**。
> 对于**重叠大于等于0.5的其他先验框(anchor)**，忽略，**不算损失**。
> 每个 GT 目标**仅与一个先验边界框相关联**。 如果没有分配先验边界框，则不会导致**分类和定位损失，只会有目标性的置信度损失。**
> 使用**tx和ty(**而不是 bx 和by**)**来计算损失。

![img](https://pic2.zhimg.com/80/v2-1e31f33f86d3f2bb9c7d516efd974765_720w.jpg)使用tx和ty(而不是 bx 和by)来计算损失

![img](https://pic1.zhimg.com/80/v2-5b4352b9a3138190717febf40e5f0384_720w.png)交叉熵损失

**总结起来就是下面4句话：**

- 正样本：与GT的IOU最大的框。
- 负样本：与GT的IOU<0.5 的框。
- 忽略的样本：与GT的IOU>0.5 但不是最大的框。
- 使用 tx 和ty （而不是 bx 和by ）来计算损失。

最后我们做个比较：

![img](https://pic4.zhimg.com/80/v2-e35154c0fe812a210295ca8c3cd92df3_720w.jpg)YOLO v1 v2和v3的比较

------

## 7 疫情都挡不住的YOLO v4

第一次看到YOLO v4公众号发文是在疫情期间，那时候还来不了学校。不得不说疫情也挡不住作者科研的动力。。。

- **检测头的改进：**

YOLO v4的作者换成了Alexey Bochkovskiy大神，检测头总的来说还是多尺度的，3个尺度，分别负责大中小目标。只不过多了一些细节的改进：

**1.Using multi-anchors for single ground truth**

之前的YOLO v3是1个anchor负责一个GT，YOLO v4中用多个anchor去负责一个GT。方法是：对于 ![[公式]](https://www.zhihu.com/equation?tex=GT_%7Bj%7D) 来说，只要 ![[公式]](https://www.zhihu.com/equation?tex=IoU%28anchor_%7Bi%7D%2CGT_%7Bj%7D%29%3Ethreshold) ，就让 ![[公式]](https://www.zhihu.com/equation?tex=anchor_%7Bi%7D) 去负责 ![[公式]](https://www.zhihu.com/equation?tex=GT_%7Bj%7D) 。

这就相当于你anchor框的数量没变，但是选择的**正样本**的比例增加了，就**缓解了正负样本不均衡的问题**。

**2.Eliminate_grid sensitivity**

还记得之前的YOLO v2的这幅图吗？YOLO v2，YOLO v3都是预测4个这样的偏移量

![img](https://pic3.zhimg.com/80/v2-63ca4ed417c5db83b18c95a42a7f60f2_720w.jpg)图3：YOLO v2，YOLO v3要预测的值

这里其实还隐藏着一个问题：

模型预测的结果是： ![[公式]](https://www.zhihu.com/equation?tex=t_%7Bx%7D%2Ct_%7By%7D%2Ct_%7Bw%7D%2Ct_%7Bh%7D) ，那么最终的结果是： ![[公式]](https://www.zhihu.com/equation?tex=b_%7Bx%7D%2Cb_%7By%7D%2Cb_%7Bw%7D%2Cb_%7Bh%7D) 。这个 ![[公式]](https://www.zhihu.com/equation?tex=b) 按理说应该能取到一个grid里面的任意位置。但是实际上边界的位置是取不到的，因为sigmoid函数的值域是： ![[公式]](https://www.zhihu.com/equation?tex=%280%2C1%29) ，它不是 ![[公式]](https://www.zhihu.com/equation?tex=%5B0%2C1%5D) 。所以作者提出的Eliminate_grid sensitivity的意思是：将 ![[公式]](https://www.zhihu.com/equation?tex=b_%7Bx%7D%2Cb_%7By%7D) 的计算公式改为：

![[公式]](https://www.zhihu.com/equation?tex=b_%7Bx%7D%3D1.1%5Ccdot%5Csigma%28t_%7Bx%7D%29%2Bc_%7Bx%7D) 

![[公式]](https://www.zhihu.com/equation?tex=b_%7By%7D%3D1.1%5Ccdot%5Csigma%28t_%7By%7D%29%2Bc_%7By%7D) 

这里的1.1就是一个示例，你也可以是1.05,1.2等等，反正要乘上一个略大于1的数，作者发现经过这样的改动以后效果会再次提升。

**3.CIoU-loss**

之前的YOLO v2，YOLO v3在计算geo_loss时都是用的MSE Loss，之后人们开始使用IoU Loss。

![[公式]](https://www.zhihu.com/equation?tex=L_%7BIoU%7D%3D1-%5Cfrac%7B%7CB%5Ccap+B_%7Bgt%7D%7C%7D%7B%7CB%5Ccup+B_%7Bgt%7D%7C%7D) 它可以反映预测检测框与真实检测框的检测效果。

但是问题也很多：不能反映两者的距离大小（重合度）。同时因为loss=0，**当GT和bounding box不挨着时，没有梯度回传，无法进行学习训练。**如下图4所示，三种情况IoU都相等，但看得出来他们的重合度是不一样的，左边的图回归的效果最好，右边的最差：

![img](https://pic2.zhimg.com/80/v2-a52e8fc7166b29c08b80de1ead22ec79_720w.jpg)图4：IoU Loss不能反映两者的距离大小

- **所以接下来的改进是：**

![[公式]](https://www.zhihu.com/equation?tex=L_%7BGIoU%7D%3D1-IoU%2B%5Cfrac%7B%7CC-B%5Ccup+B_%7Bgt%7D%7C%7D%7B%7CC%7C%7D) , ![[公式]](https://www.zhihu.com/equation?tex=C) 为同时包含了预测框和真实框的最小框的面积。

![img](https://pic1.zhimg.com/80/v2-4ccbf64fa4eefb0e321809a803f90c74_720w.jpg)图4：GIoU Loss

GIoU Loss可以解决上面IoU Loss对距离不敏感的问题。但是GIoU Loss存在训练过程中发散等问题。

- **接下来的改进是：**

![[公式]](https://www.zhihu.com/equation?tex=L_%7BDIoU%7D+%3D+1-IoU+%2B+%5Cfrac%7B%5Crho%5E%7B2%7D%28b%2Cb%5E%7Bgt%7D%29%7D%7Bc%5E%7B2%7D%7D)

其中， ![[公式]](https://www.zhihu.com/equation?tex=b) ， ![[公式]](https://www.zhihu.com/equation?tex=b%5E%7Bgt%7D) 分别代表了预测框和真实框的中心点，且![[公式]](https://www.zhihu.com/equation?tex=%5Crho)代表的是计算两个中心点间的欧式距离。![[公式]](https://www.zhihu.com/equation?tex=c)代表的是能够同时包含预测框和真实框的**最小闭包区域**的对角线距离。

![img](https://pic1.zhimg.com/80/v2-2345aacc478cc5523d439ffcd84958ac_720w.jpg)图5：DIoU Loss

**DIoU loss**可以直接最小化两个目标框的距离，因此比GIoU loss收敛快得多。

**DIoU loss除了这一点之外，还有一个好处是：**

![img](https://pic3.zhimg.com/80/v2-3db202166e0c206001ca03e191489532_720w.jpg)IoU Loss和GIoU loss都一样时

如上图所示，此3种情况IoU Loss和GIoU loss都一样，但是DIoU Loss右图最小，中间图次之，左图最大。

这里就是一道面试题：**请总结DIoU loss的好处是？**

**答：**

- 收敛快(需要的epochs少)。
- 缓解了Bounding box全包含GT问题。

**但是**DIoU loss只是**缓解了Bounding box全包含GT问题**，**依然没有彻底解决包含的问题**，即：

![img](https://pic4.zhimg.com/80/v2-96838980b7fd4443661cf0019802ea7b_720w.jpg)

这2种情况![[公式]](https://www.zhihu.com/equation?tex=b) 和 ![[公式]](https://www.zhihu.com/equation?tex=b%5E%7Bgt%7D)是重合的，DIoU loss的第3项没有区别，所以在这个意义上DIoU loss依然存在问题。

- **接下来的改进是：**

惩罚项如下面公式：

![[公式]](https://www.zhihu.com/equation?tex=%5Cmathcal%7BR%7D_%7BC+I+o+U%7D%3D%5Cfrac%7B%5Crho%5E%7B2%7D%5Cleft%28%5Cmathbf%7Bb%7D%2C+%5Cmathbf%7Bb%7D%5E%7Bg+t%7D%5Cright%29%7D%7Bc%5E%7B2%7D%7D%2B%5Calpha+v) 其中 ![[公式]](https://www.zhihu.com/equation?tex=%5Calpha) 是权重函数，

而 ![[公式]](https://www.zhihu.com/equation?tex=%5Cnu) 用来度量长宽比的相似性，定义为![[公式]](https://www.zhihu.com/equation?tex=v%3D%5Cfrac%7B4%7D%7B%5Cpi%5E%7B2%7D%7D%5Cleft%28%5Carctan+%5Cfrac%7Bw%5E%7Bg+t%7D%7D%7Bh%5E%7Bg+t%7D%7D-%5Carctan+%5Cfrac%7Bw%7D%7Bh%7D%5Cright%29%5E%7B2%7D)

完整的 CIoU 损失函数定义：

![[公式]](https://www.zhihu.com/equation?tex=%5Cmathcal%7BL%7D_%7BC+I+o+U%7D%3D1-I+o+U%2B%5Cfrac%7B%5Crho%5E%7B2%7D%5Cleft%28%5Cmathbf%7Bb%7D%2C+%5Cmathbf%7Bb%7D%5E%7Bg+t%7D%5Cright%29%7D%7Bc%5E%7B2%7D%7D%2B%5Calpha+v)

最后，CIoU loss的梯度类似于DIoU loss，但还要考虑 ![[公式]](https://www.zhihu.com/equation?tex=%5Cnu) 的梯度。在长宽在 ![[公式]](https://www.zhihu.com/equation?tex=%5B0%2C1%5D) 的情况下， ![[公式]](https://www.zhihu.com/equation?tex=w%5E%7B2%7D%2Bh%5E%7B2%7D) 的值通常很小，会导致梯度爆炸，因此在 ![[公式]](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7Bw%5E%7B2%7D%2Bh%5E%7B2%7D%7D) 实现时将替换成1。

CIoU loss实现代码：

```python3
def bbox_overlaps_ciou(bboxes1, bboxes2):
    rows = bboxes1.shape[0]
    cols = bboxes2.shape[0]
    cious = torch.zeros((rows, cols))
    if rows * cols == 0:
        return cious
    exchange = False
    if bboxes1.shape[0] > bboxes2.shape[0]:
        bboxes1, bboxes2 = bboxes2, bboxes1
        cious = torch.zeros((cols, rows))
        exchange = True

    w1 = bboxes1[:, 2] - bboxes1[:, 0]
    h1 = bboxes1[:, 3] - bboxes1[:, 1]
    w2 = bboxes2[:, 2] - bboxes2[:, 0]
    h2 = bboxes2[:, 3] - bboxes2[:, 1]

    area1 = w1 * h1
    area2 = w2 * h2

    center_x1 = (bboxes1[:, 2] + bboxes1[:, 0]) / 2
    center_y1 = (bboxes1[:, 3] + bboxes1[:, 1]) / 2
    center_x2 = (bboxes2[:, 2] + bboxes2[:, 0]) / 2
    center_y2 = (bboxes2[:, 3] + bboxes2[:, 1]) / 2

    inter_max_xy = torch.min(bboxes1[:, 2:],bboxes2[:, 2:])
    inter_min_xy = torch.max(bboxes1[:, :2],bboxes2[:, :2])
    out_max_xy = torch.max(bboxes1[:, 2:],bboxes2[:, 2:])
    out_min_xy = torch.min(bboxes1[:, :2],bboxes2[:, :2])

    inter = torch.clamp((inter_max_xy - inter_min_xy), min=0)
    inter_area = inter[:, 0] * inter[:, 1]
    inter_diag = (center_x2 - center_x1)**2 + (center_y2 - center_y1)**2
    outer = torch.clamp((out_max_xy - out_min_xy), min=0)
    outer_diag = (outer[:, 0] ** 2) + (outer[:, 1] ** 2)
    union = area1+area2-inter_area
    u = (inter_diag) / outer_diag
    iou = inter_area / union
    with torch.no_grad():
        arctan = torch.atan(w2 / h2) - torch.atan(w1 / h1)
        v = (4 / (math.pi ** 2)) * torch.pow((torch.atan(w2 / h2) - torch.atan(w1 / h1)), 2)
        S = 1 - iou
        alpha = v / (S + v)
        w_temp = 2 * w1
    ar = (8 / (math.pi ** 2)) * arctan * ((w1 - w_temp) * h1)
    cious = iou - (u + alpha * ar)
    cious = torch.clamp(cious,min=-1.0,max = 1.0)
    if exchange:
        cious = cious.T
    return cious
```

所以最终的演化过程是：

**MSE Loss ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) IoU Loss![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) GIoU Loss![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) DIoU Loss![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) CIoU Loss**

- **YOLO v4损失函数：**

所以基于上面的改进，YOLO v4的损失函数为：

![img](https://pic3.zhimg.com/80/v2-310600feb7204188d9738fbd062567ca_720w.jpg)YOLO v4的损失函数



------

## 8 代码比论文都早的YOLO v5

- **检测头的改进：**

**head部分没有任何改动**，和yolov3和yolov4完全相同，也是三个输出头，stride分别是8,16,32，大输出特征图检测小物体，小输出特征图检测大物体。

但采用了**自适应anchor，而且这个功能还可以手动打开/关掉，具体是什么意思呢？**

加上了自适应anchor的功能，个人感觉YOLO v5其实变成了2阶段方法。

**先回顾下之前的检测器得到anchor的方法：**

**Yolo v2 v3 v4：聚类得到anchor**，不是完全基于anchor的，w,h是基于anchor的，而x,y是基于grid的坐标，所以人家叫**location prediction**。

**R-CNN系列：手动指定**anchor的位置。

**基于anchor的方法是怎么用的：**

![img](https://pic4.zhimg.com/80/v2-5dc0cdfb531add5071abf2abc7399467_720w.jpg)anchor是怎么用的

有了anchor的 ![[公式]](https://www.zhihu.com/equation?tex=%28x_%7BA%7D%2Cy_%7BA%7D%2Cw_%7BA%7D%2Ch_%7BA%7D%29) ,和我们预测的偏移量 ![[公式]](https://www.zhihu.com/equation?tex=t_%7Bx%7D%2Ct_%7By%7D%2Ct_%7Bw%7D%2Ct_%7Bh%7D) ，就可以计算出最终的output： ![[公式]](https://www.zhihu.com/equation?tex=b_%7Bx%7D%2Cb_%7By%7D%2Cb_%7Bw%7D%2Cb_%7Bh%7D) 。

之前anchor是固定的，自适应anchor利用网络的学习功能，让 ![[公式]](https://www.zhihu.com/equation?tex=%28x_%7BA%7D%2Cy_%7BA%7D%2Cw_%7BA%7D%2Ch_%7BA%7D%29) 也是可以学习的。我个人觉得自适应anchor策略，影响应该不是很大，除非是**刚开始设置的anchor是随意设置的**，一般我们都会基于实际项目数据重新运用**kmean算法聚类得到anchor**，这一步本身就不能少。

最后总结一下：

![img](https://pic2.zhimg.com/80/v2-34554f1fe4165e6b85d4905b925faa79_720w.jpg)YOLO series的比较



本文只介绍了**YOLO v2 v3 v4 v5**对于**检测头head**和**损失函数loss**的优化，剩下的**backbone**方面和**输入端的优化**实在是写不动了，放到下一篇吧。

[科技猛兽：你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (下)765 赞同 · 15 评论文章![img](https://pic3.zhimg.com/v2-dde47004792ffb4259ccdb1783fa303e_ipico.jpg)](https://zhuanlan.zhihu.com/p/186014243)



编辑于 2020-08-21 18:17
