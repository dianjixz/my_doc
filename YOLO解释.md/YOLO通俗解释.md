 

![你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (上)](https://pic3.zhimg.com/v2-dde47004792ffb4259ccdb1783fa303e_1440w.jpg?source=172ae18b)

# 你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (上)

[![科技猛兽](https://pic3.zhimg.com/v2-7519f1e718ed6230763867ab473b3e96_xs.jpg?source=172ae18b)](https://www.zhihu.com/people/wang-jia-hao-53-3)

[科技猛兽](https://www.zhihu.com/people/wang-jia-hao-53-3)[](https://www.zhihu.com/question/48510028)



清华大学 自动化系硕士在读

**转载请务必注明出处。**

思路和 PPT 等来自**开课吧**：

[知乎用户](https://www.zhihu.com/people/zhaomingming0617/columns)

[www.zhihu.com/people/zhaomingming0617/columns![img](https://pic2.zhimg.com/zhihu-card-default_ipico.jpg)](https://www.zhihu.com/people/zhaomingming0617/columns)

[赵明明-搜索-开课吧](https://link.zhihu.com/?target=https%3A//www.kaikeba.com/search%3Fq%3D%E8%B5%B5%E6%98%8E%E6%98%8E)

[www.kaikeba.com/search?q=%E8%B5%B5%E6%98%8E%E6%98%8E](https://link.zhihu.com/?target=https%3A//www.kaikeba.com/search%3Fq%3D%E8%B5%B5%E6%98%8E%E6%98%8E)

[科技猛兽：你一定从未看过如此通俗易懂的YOLO系列(从v1到v5)模型解读 (上)2777 赞同 · 59 评论文章![img](https://pic3.zhimg.com/v2-dde47004792ffb4259ccdb1783fa303e_ipico.jpg)](https://zhuanlan.zhihu.com/p/183261974)

## 0 前言

本文目的是用尽量浅显易懂的语言让零基础小白能够理解什么是YOLO系列模型，以及他们的设计思想和改进思路分别是什么。我不会把YOLO的论文给你用软件翻译一遍，这样做毫无意义；也不会使用太专业晦涩的名词和表达，对于每一个新的概念都会解释得尽量通俗一些，目的是使得你能像看故事一样学习YOLO模型，我觉得这样的学习方式才是知乎博客的意义所在。

为了使本文**尽量生动有趣**，我用葫芦娃作为例子展示YOLO的过程(真的是尽力了。。。)。

![img](https://pic2.zhimg.com/80/v2-6f33f7b365e952d0cb1436c540135a0d_720w.jpg)葫芦娃

同时，会对**YOLO v1和YOLOv5**的代码进行解读，其他的版本就只介绍改进了。

------

## 1 先从一款强大的app说起

![img](https://pic1.zhimg.com/80/v2-6821e6ed09cedb367cde596fbfcb4328_720w.jpg)i detection APP

**YOLO v5**其实一开始是以一款**app**进入人们的视野的，就是上图的这个，叫：**i detection**(图上标的是YOLO v4，但其实算法是YOLO v5)，使用**iOS**系列的小伙伴呢，就可以立刻**点赞后**关掉我这篇文章，去下载这个app玩一玩。在任何场景下(工业场景，生活场景等等)都可以试试这个app和这个算法，这个app中间还有一个**button**，来调节app使用的模型的大小，更大的模型实时性差但精度高，更小的模型实时性好但精度差。

值得一提的是，这款app就是**YOLO v5的作者**亲自完成的。而且，我写这篇文章的时候YOLO v5的论文还没有出来，还在实验中，等论文出来应该是2020年底或者2021年初了。



**读到这里，你觉得YOLO v5的最大特点是什么？**

**答案就是：一个字：快**，应用于移动端，模型小，速度快。



首先我个人觉得任何一个模型都有下面3部分组成：

- **前向传播部分：90%**
- **损失函数部分**
- **反向传播部分**

其中前向传播部分占用的时间应该在90%左右，即搞清楚前向传播部分也就搞清楚了这模型的实现流程和细节。本着这一原则，我们开始YOLO系列模型的解读：

------

## 2 不得不谈的分类模型

在进入目标检测任务之前首先得学会图像分类任务，这个任务的特点是输入一张图片，输出是它的类别。

对于**输入图片**，我们一般用一个矩阵表示。

对于**输出结果**，我们一般用一个one-hot vector表示： ![[公式]](https://www.zhihu.com/equation?tex=%5B0%2C0%2C1%2C0%2C0%2C0%5D) ，哪一维是1，就代表图片属于哪一类。

所以，在设计神经网络时，结构大致应该长这样：

img ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp16![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp32![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp64![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp128![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) ...![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) fc256-fc[10]

这里的cbrp指的是conv，bn，relu，pooling的串联。

由于输入要是one-hot形式，所以最后我们设计了2个fc层(fully connencted layer)，我们称之为**“分类头”**或者**“决策层”**。

------

## 3 YOLO系列思想的雏形：YOLO v0

有了上面的分类器，我们能不能用它来做检测呢？

要回答这个问题，首先得看看检测器和分类器的输入输出有什么不一样。首先他们的输入都是image，但是分类器的输出是一个one-hot vector，而检测器的输出是一个框(Bounding Box)。

**框**，该怎么表示？

在一个图片里面表示一个框，有很多种方法，比如：

![img](https://pic3.zhimg.com/80/v2-5d8895c050a05b9ce813b5289ec2a4da_720w.jpg)x,y,w,h表示一个框

- **x,y,w,h**(如上图)
- **p1,p2,p3,p4**(4个点坐标)
- **cx,cy,w,h**(cx,cy为中心点坐标)
- **x,y,w,h**,**angle**(还有的目标是有角度的，这时叫做Rotated Bounding Box)
- ......

所以表示的方法不是一成不变的，但你会发现：不管你用什么形式去表达这个Bounding Box，你模型输出的结果一定是一个vector，那这个vector和分类模型输出的vector本质上有什么区别吗？

**答案是：没有**，都是向量而已，只是分类模型输出是one-hot向量，检测模型输出是我们标注的结果。

所以你应该会发现，检测的方法呼之欲出了。**那分类模型可以用来做检测吗？**

**当然可以，**这时，你可以把检测的任务当做是**遍历性的分类任务。**

**如何遍历？**

**我们的目标是一个个框，那就用这个框去遍历所有的位置，所有的大小。**

比如下面这张图片，我需要你检测葫芦娃的脸，如图1所示：

![img](https://pic4.zhimg.com/80/v2-a1843eda0e8f2c6ea8cddb5180da1da3_720w.jpg)图1：检测葫芦娃的脸

我们可以对边框的区域进行二分类：属于头或者不属于头。

你先预设一个框的大小，然后在图片上遍历这个框，比如第一行全都不是头。第4个框只有一部分目标在，也不算。第5号框算是一个头，我们记住它的位置。这样不断地滑动，就是遍历性地分类。

接下来要遍历框的大小：因为你刚才是预设一个框的大小，但葫芦娃的头有大有小，你还得遍历框的大小，如下图2所示：

![img](https://pic4.zhimg.com/80/v2-9c3d5b1c85adf80791780697b9ad8ecb_720w.jpg)图2：遍历框的大小

还没有结束，刚才滑窗时是挨个滑，但其实没有遍历所有的位置，更精确的遍历方法应该如下图3所示：

![img](https://pic2.zhimg.com/80/v2-eda9abf77cb3597a41cc460c6f4f6d0d_720w.jpg)图3：更精确地遍历框的位置

这种方法其实就是RCNN全家桶的初衷，专业术语叫做：**滑动窗口分类方法**。

现在需要你思考一个问题：**这种方法的精确和什么因素有关？**

**答案是：遍历得彻不彻底**。遍历得越精确，检测器的精度就越高。所以这也就带来一个问题就是：**检测的耗时非常大**。

举个例子：比如输入图片大小是(800,1000)也就意味着有800000个位置。窗口大小最小 ![[公式]](https://www.zhihu.com/equation?tex=%281%5Ctimes1%29) ,最大 ![[公式]](https://www.zhihu.com/equation?tex=%28800%5Ctimes1000%29) ，所以这个遍历的次数是**无限次**。我们看下伪代码：

![img](https://pic4.zhimg.com/80/v2-72224eb25b6bcfce14d31a5a571422f3_720w.jpg)滑动窗口分类方法伪代码

那这种方法如何训练呢？

本质上还是训练一个二分类器。这个二分类器的**输入是一个框的内容，输出是(前景/背景)**。

**第1个问题：**

框有不同的大小，对于不同大小的框，输入到相同的二分类器中吗？

是的。要先把不同大小的input归一化到统一的大小。

**第2个问题：**

背景图片很多，前景图片很少：二分类样本不均衡。



确实是这样，你看看一张图片有多少框对应的是背景，有多少框才是葫芦娃的头。

以上就是传统检测方法的主要思路：

- 耗时。
- 操作复杂，需要手动生成大量的样本。



**到现在为止，我们用分类的算法设计了一个检测器，它存在着各种各样的问题，现在是优化的时候了(接下来正式进入YOLO系列方法了)：**

YOLO的作者当时是这么想的：你分类器输出一个one-hot vector，那我把它换成**(x,y,w,h,c)**，c表示confidence置信度，把问题转化成一个回归问题，直接回归出Bounding Box的位置不就好了吗？

刚才的分类器是：img ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp16![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp32![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp64![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp128![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) ...![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) fc256-fc[10]

现在我变成：img ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp16![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp32![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp64![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp128![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) ...![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) fc256-fc[5]，这个输出是**(x,y,w,h,c)**，不就变成了一个检测器吗？

**本质上都是矩阵映射到矩阵，只是代表的意义不一样而已。**

> 传统的方法为什么没有这么做呢？我想肯定是效果不好，终其原因是算力不行，conv操作还没有推广。

好，现在模型是：

img ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp16![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp32![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp64![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp128![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) ...![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) fc256-fc[5] ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) c,x,y,w,h

![img](https://pic2.zhimg.com/80/v2-833df61bdd1b13d0647acf5fbe139339_720w.jpg)

那如何组织训练呢？找1000张图片，把label设置为 ![[公式]](https://www.zhihu.com/equation?tex=%281%2Cx%5E%7B%2A%7D%2Cy%5E%7B%2A%7D%2Cw%5E%7B%2A%7D%2Ch%5E%7B%2A%7D%29) 。这里 ![[公式]](https://www.zhihu.com/equation?tex=x%5E%7B%2A%7D) 代表真值。有了数据和标签，就完成了设计。

我们会发现，这种方法比刚才的**滑动窗口分类方法**简单太多了。这一版的思路我把它叫做**YOLO v0**，因为它是**You Only Look Once**最简单的版本。

------

## 4 YOLO v1终于诞生

- **需求1：YOLO v0只能输出一个目标，那比如下图4的多个目标怎么办呢？**

![img](https://pic1.zhimg.com/80/v2-bdf872e1bc2ac749425d3e45b123b3ec_720w.jpg)图4：多个目标情况

你可能会回答：我输出N个向量不就行了吗？但具体输出多少个合适呢？图4有7个目标，那有的图片有几百个目标，你这个N又该如何调整呢？

**答：为了保证所有目标都被检测到，我们应该输出尽量多的目标。**

![img](https://pic1.zhimg.com/80/v2-de3aec28683d4e58fca1218ddfaff670_720w.jpg)输出尽量多的目标

但这种方法也不是最优的，最优的应该是下图这样：

![img](https://pic3.zhimg.com/80/v2-0c9a23e1c1740d62ccf50cdc97b0bcb2_720w.jpg)图5：用一个(c,x,y,w,h)去负责image某个区域的目标

如图5所示：用一个(c,x,y,w,h)去负责image某个区域的目标。

比如说图片设置为16个区域，每个区域用1个(c,x,y,w,h)去负责：

![img](https://pic2.zhimg.com/80/v2-a6230457e694322c04998b7d0536a889_720w.jpg)图6：图片设置为16个区域

就可以一次输出16个框，每个框是1个(c,x,y,w,h)，如图6所示。

为什么这样子更优？因为conv操作是位置强相关的，就是原来的目标在哪里，你conv之后的feature map上还在哪里，所以图片划分为16个区域，结果也应该分布在16个区域上，所以我们的**结果(Tensor)的维度size是：(5,4,4)**。

那现在你可能会问：**c的真值该怎么设置呢？**

**答：**看葫芦娃的大娃，他的脸跨了4个区域(grid)，但只能某一个grid的c=1，其他的c=0。那么该让哪一个grid的c=1呢？就看他的脸的中心落在了哪个grid里面。根据这一原则，c的真值为下图7所示：

![img](https://pic4.zhimg.com/80/v2-02a45bc027de8ff65693bcf1ae21c2e3_720w.jpg)图7：c的label值

但是你发现7个葫芦娃只有6个1，原因是某一个grid里面有2个目标，确实如此，第三行第三列的grid既有**水娃**又有**隐身娃**。这种一个区域有多个目标的情况我们**目前没法解决，因为我们的模型现在能力就这么大，只能在一个区域中检测出一个目标，如何改进我们马上就讨论，你可以现在先自己想一想。**

总之现在我们设计出了模型的输出结果，那距离完成模型的设计还差一个损失函数，那Loss咋设计呢？看下面的伪代码：

```python3
loss = 0
for img in img_all:
   for i in range(4):
      for j in range(4):
         loss_ij = lamda_1*(c_pred-c_label)**2 + c_label*(x_pred-x_label)**2 +\
                     c_label*(y_pred-y_label)**2 + c_label*(w_pred-w_label)**2 + \
                     c_label*(h_pred-h_label)**2
         loss += loss_ij
loss.backward()
```

> 遍历所有图片，遍历所有位置，计算loss。

- 好现在模型设计完了，回到刚才的问题：模型现在能力就这么大，只能在一个区域中检测出一个目标，如何改进？

**答：**刚才区域是 ![[公式]](https://www.zhihu.com/equation?tex=4%5Ctimes4) ，现在变成 ![[公式]](https://www.zhihu.com/equation?tex=40%5Ctimes40) ，或者更大，使区域更密集，就可以缓解多个目标的问题，但无法从根本上去解决。



- 另一个问题，按上面的设计你检测得到了16个框，可是图片上只有7个葫芦娃的脸，怎么从16个结果中筛选出7个我们要的呢？

**答：**

**法1：聚类。**聚成7类，在这7个类中，选择confidence最大的框。听起来挺好。

**法1的bug：**2个目标本身比较近聚成了1个类怎么办？如果不知道到底有几个目标呢？为何聚成7类？不是3类？

**法2：NMS(非极大值抑制)。**2个框重合度很高，大概率是一个目标，那就只取一个框。

重合度的计算方法：交并比IoU=两个框的交集面积/两个框的并集面积。

具体算法：

![img](https://pic2.zhimg.com/80/v2-f2cf821fc64f14d2b60ae2f61409e3f5_720w.jpg)面试必考的NMS

**法1的bug：**2个目标本身比较近怎么办？依然没有解决。

如果不知道到底有几个目标呢？NMS自动解决了这个问题。

**面试的时候会问这样一个问题：**NMS的适用情况是什么？

**答：**1图多目标检测时用NMS。



到现在为止我们终于解决了第4节开始提出的多个目标的问题，现在又有了新的需求：

- **需求2：多类的目标怎么办呢？**

比如说我现在既要你检测**葫芦娃的脸**，又要你检测**葫芦娃的葫芦**，怎么设计？

img ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp16![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp32![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp64![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp128![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) ...![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) fc256-fc[5+2]*N ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) [c,x,y,w,h,one-hot]*N

2个类，one-hot就是[0,1],[1,0]这样子，如下图8所示：

![img](https://pic2.zhimg.com/80/v2-38ce7b4f57bc0833a66a6baf5559da99_720w.jpg)图8：多类的目标的label

伪代码依然是：

```text
loss = 0
for img in img_all:
   for i in range(3):
      for j in range(4):
         c_loss = lamda_1*(c_pred-c_label)**2
         geo_loss = c_label*(x_pred-x_label)**2 +\
                     c_label*(y_pred-y_label)**2 + c_label*(w_pred-w_label)**2 + \
                     c_label*(h_pred-h_label)**2
         class_loss = 1/m * mse_loss(p_pred, p_label)
         loss_ij =c_loss  + geo_loss + class_loss
         loss += loss_ij
loss.backward()
```

至此，多个类的问题也解决了，现在又有了新的需求：

- **需求3：小目标检测怎么办呢？**

小目标总是检测不佳，所以我们专门设计神经元去拟合小目标。
 

![img](https://pic2.zhimg.com/80/v2-efb58a2c9d3e881df099789f640471ad_720w.jpg)图9：多类的小目标的label，分别预测大目标和小目标

对于每个区域，我们用2个五元组(c,x,y,w,h)，一个负责回归大目标，一个负责回归小目标，同样添加one-hot vector，one-hot就是[0,1],[1,0]这样子，来表示属于哪一类(葫芦娃的头or葫芦娃的葫芦)。

伪代码变为了：

```text
loss = 0
for img in img_all:
   for i in range(3):
      for j in range(4):
         c_loss = lamda_1*(c_pred-c_label)**2
         geo_loss = c_label_big*(x_big_pred-x_big_label)**2 +\
                     c_label_big*(y_big_pred-y_big_label)**2 + c_label_big*(w_big_pred-w_big_label)**2 + \
                     c_label_big*(h_big_pred-h_big_label)**2 +\
                     c_label_small*(x_small_pred-x_small_label)**2 +\
                     c_label_small*(y_small_pred-y_small_label)**2 + c_label_small*(w_small_pred-w_small_label)**2 + \
                     c_label_small*(h_small_pred-h_small_label)**2
         class_loss = 1/m * mse_loss(p_pred, p_label)
         loss_ij =c_loss  + geo_loss + class_loss
         loss += loss_ij
loss.backward()
```



至此，小目标的问题也有了解决方案。

到这里，我们设计的检测器其实就是YOLO v1，只是有的参数跟它不一样，我们看论文里的图：

![img](https://pic3.zhimg.com/80/v2-ce26d13cfd3b7145f4594524435a9b92_720w.jpg)YOLO v1

![img](https://pic4.zhimg.com/80/v2-3af308f7096bda4c621c077302b90533_720w.jpg)YOLO v1

YOLO v1其实就是把我们划分的16个区域变成了 ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7) 个区域，我们预测16个目标，YOLO v1预测49个目标。我们是2类(葫芦娃的头or葫芦娃的葫芦)，YOLO v1是20类。

![[公式]](https://www.zhihu.com/equation?tex=30%3D5%2A2%28c_%7Bbig%7D%2Cx_%7Bbig%7D%2Cy_%7Bbig%7D%2Cw_%7Bbig%7D%2Ch_%7Bbig%7D%2C+c_%7Bsmall%7D%2Cx_%7Bsmall%7D%2Cy_%7Bsmall%7D%2Cw_%7Bsmall%7D%2Ch_%7Bsmall%7D%29%2B20+classes) 

backbone也是一堆卷积+检测头(FC层)，所以说设计到现在，我们其实是把YOLO v1给设计出来了。

再看看作者的解释：

![img](https://pic2.zhimg.com/80/v2-6b90a56a5c57d2ed05ec620f4230ca45_720w.jpg)

发现train的时候用的小图片，检测的时候用的是大图片(肯定是经过了无数次试验证明了效果好)。

结构学完了，再看loss函数，并比较下和我们设计的loss函数有什么区别。

![img](https://pic4.zhimg.com/80/v2-f81c565d41b681263689626331325ac3_720w.jpg)YOLO v1 loss函数

**解读一下这个损失函数：**

我们之前说的损失函数是设计了3个for循环，而作者为了方便写成了求和的形式：

- 前2行计算前景的geo_loss。
- 第3行计算前景的confidence_loss。
- 第4行计算背景的confidence_loss。
- 第5行计算分类损失class_loss。

伪代码上面已经有了，现在我们总体看一下这个模型：

img ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp192![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp256![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp512![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) cbrp1024![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) ...![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) fc4096-fc[5+2]*N ![[公式]](https://www.zhihu.com/equation?tex=%5Crightarrow) ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7%5Ctimes30) 

检测层的设计：回归坐标值+one-hot分类

![img](https://pic2.zhimg.com/80/v2-bf46a1073c877181e06cba19b8c20995_720w.jpg)检测层的设计

- **样本不均衡的问题解决了吗？**

没有计算背景的geo_loss，只计算了前景的geo_loss，这个问题YOLO v1回避了，依然存在。



**最后我们解读下YOLO v1的代码：**

**1.模型定义：**

定义特征提取层：

```python3
class VGG(nn.Module):
    def __init__(self):
       super(VGG,self).__init__()
       # the vgg's layers
       #self.features = features
       cfg = [64,64,'M',128,128,'M',256,256,256,'M',512,512,512,'M',512,512,512,'M']
       layers= []
       batch_norm = False
       in_channels = 3
       for v in cfg:
           if v == 'M':
               layers += [nn.MaxPool2d(kernel_size=2,stride = 2)]
           else:
               conv2d = nn.Conv2d(in_channels,v,kernel_size=3,padding = 1)
               if batch_norm:
                   layers += [conv2d,nn.Batchnorm2d(v),nn.ReLU(inplace=True)]
               else:
                   layers += [conv2d,nn.ReLU(inplace=True)]
               in_channels = v
       # use the vgg layers to get the feature
       self.features = nn.Sequential(*layers)
       # 全局池化
       self.avgpool = nn.AdaptiveAvgPool2d((7,7))
       # 决策层：分类层
       self.classifier = nn.Sequential(
           nn.Linear(512*7*7,4096),
           nn.ReLU(True),
           nn.Dropout(),
           nn.Linear(4096,4096),
           nn.ReLU(True),
           nn.Dropout(),
           nn.Linear(4096,1000),
       )

       for m in self.modules():
           if isinstance(m,nn.Conv2d):
               nn.init.kaiming_normal_(m.weight,mode='fan_out',nonlinearity='relu')
               if m.bias is not None: 
                   nn.init.constant_(m.bias,0)
           elif isinstance(m,nn.BatchNorm2d):
               nn.init.constant_(m.weight,1)
               nn.init.constant_(m.bias,1)
           elif isinstance(m,nn.Linear):
               nn.init.normal_(m.weight,0,0.01)
               nn.init.constant_(m.bias,0)

    def forward(self,x):
         x = self.features(x)
         x_fea = x
         x = self.avgpool(x)
         x_avg = x
         x = x.view(x.size(0),-1)
         x = self.classifier(x)
         return x,x_fea,x_avg
    def extractor(self,x):
         x = self.features(x)
         return x
```

定义检测头：

```python3
       self.detector = nn.Sequential(
          nn.Linear(512*7*7,4096),
          nn.ReLU(True),
          nn.Dropout(),
          nn.Linear(4096,1470),
       )
```

整体模型：

```python3
class YOLOV1(nn.Module):
    def __init__(self):
       super(YOLOV1,self).__init__()
       vgg = VGG()
       self.extractor = vgg.extractor
       self.avgpool = nn.AdaptiveAvgPool2d((7,7))
       # 决策层：检测层
       self.detector = nn.Sequential(
          nn.Linear(512*7*7,4096),
          nn.ReLU(True),
          nn.Dropout(),
          #nn.Linear(4096,1470),
          nn.Linear(4096,245),
          #nn.Linear(4096,5),
       )
       for m in self.modules():
           if isinstance(m,nn.Conv2d):
               nn.init.kaiming_normal_(m.weight,mode='fan_out',nonlinearity='relu')
               if m.bias is not None: 
                   nn.init.constant_(m.bias,0)
           elif isinstance(m,nn.BatchNorm2d):
               nn.init.constant_(m.weight,1)
               nn.init.constant_(m.bias,1)
           elif isinstance(m,nn.Linear):
               nn.init.normal_(m.weight,0,0.01)
               nn.init.constant_(m.bias,0)
    def forward(self,x):
        x = self.extractor(x)
        #import pdb
        #pdb.set_trace()
        x = self.avgpool(x)
        x = x.view(x.size(0),-1)
        x = self.detector(x)
        b,_ = x.shape
        #x = x.view(b,7,7,30)
        x = x.view(b,7,7,5)
        
        #x = x.view(b,1,1,5)
        return x
```

主函数：

```python3
if __name__ == '__main__':
    vgg = VGG()
    x  = torch.randn(1,3,512,512)
    feature,x_fea,x_avg = vgg(x)
    print(feature.shape)
    print(x_fea.shape)
    print(x_avg.shape)
 
    yolov1 = YOLOV1()
    feature = yolov1(x)
    # feature_size b*7*7*30
    print(feature.shape)
```

**2.模型训练：**

主函数：

```python3
if __name__ == "__main__":
    train()
```

下面看train()函数：

```python3
def train():
    for epoch in range(epochs):
        ts = time.time()
        for iter, batch in enumerate(train_loader):
            optimizer.zero_grad()
            # 取图片
            inputs = input_process(batch)
            # 取标注
            labels = target_process(batch)
            
            # 获取得到输出
            outputs = yolov1_model(inputs)
            #import pdb
            #pdb.set_trace()
            #loss = criterion(outputs, labels)
            loss,lm,glm,clm = lossfunc_details(outputs,labels)
            loss.backward()
            optimizer.step()
            #print(torch.cat([outputs.detach().view(1,5),labels.view(1,5)],0).view(2,5))
            if iter % 10 == 0:
            #    print(torch.cat([outputs.detach().view(1,5),labels.view(1,5)],0).view(2,5))
                print("epoch{}, iter{}, loss: {}, lr: {}".format(epoch, iter, loss.data.item(),optimizer.state_dict()['param_groups'][0]['lr']))
        
        #print("Finish epoch {}, time elapsed {}".format(epoch, time.time() - ts))
        #print("*"*30)
        #val(epoch)
        scheduler.step()
```

训练过程比较常规，先取1个batch的训练数据，分别得到inputs和labels，依次计算loss，反传，step等。

下面说下2个训练集的数据处理函数：

input_process：

```python3
def input_process(batch):
    #import pdb
    #pdb.set_trace()
    batch_size=len(batch[0])
    input_batch= torch.zeros(batch_size,3,448,448)
    for i in range(batch_size):
        inputs_tmp = Variable(batch[0][i])
        inputs_tmp1=cv2.resize(inputs_tmp.permute([1,2,0]).numpy(),(448,448))
        inputs_tmp2=torch.tensor(inputs_tmp1).permute([2,0,1])
        input_batch[i:i+1,:,:,:]= torch.unsqueeze(inputs_tmp2,0)
    return input_batch 
```

> batch[0]为image，batch[1]为label，batch_size为1个batch的图片数量。
> batch[0][i]为这个batch的第i张图片，inputs_tmp2为尺寸变成了3,448,448之后的图片，再经过unsqueeze操作拓展1维，size=[1,3,448,448]，存储在input_batch中。

最后，返回的是size=[batch_size,3,448,448]的输入数据。



target_process：

```python3
def target_process(batch,grid_number=7):
    # batch[1]表示label
    # batch[0]表示image
    batch_size=len(batch[0])
    target_batch= torch.zeros(batch_size,grid_number,grid_number,30)
    #import pdb
    #pdb.set_trace()
    for i in range(batch_size):
        labels = batch[1]
        batch_labels = labels[i]
        #import pdb
        #pdb.set_trace()
        number_box = len(batch_labels['boxes'])
        for wi in range(grid_number):
            for hi in range(grid_number):
                # 遍历每个标注的框
                for bi in range(number_box):
                    bbox=batch_labels['boxes'][bi]
                    _,himg,wimg = batch[0][i].numpy().shape
                    bbox = bbox/ torch.tensor([wimg,himg,wimg,himg])
                    #import pdb
                    #pdb.set_trace()
                    center_x= (bbox[0]+bbox[2])*0.5
                    center_y= (bbox[1]+bbox[3])*0.5
                    #print("[%s,%s,%s],[%s,%s,%s]"%(wi/grid_number,center_x,(wi+1)/grid_number,hi/grid_number,center_y,(hi+1)/grid_number))
                    if center_x<=(wi+1)/grid_number and center_x>=wi/grid_number and center_y<=(hi+1)/grid_number and center_y>= hi/grid_number:
                        #pdb.set_trace()
                        cbbox =  torch.cat([torch.ones(1),bbox])
                        # 中心点落在grid内，
                        target_batch[i:i+1,wi:wi+1,hi:hi+1,:] = torch.unsqueeze(cbbox,0)
                    #else:
                        #cbbox =  torch.cat([torch.zeros(1),bbox])
                #import pdb
                #pdb.set_trace()
                #rint(target_batch[i:i+1,wi:wi+1,hi:hi+1,:])
                #target_batch[i:i+1,wi:wi+1,hi:hi+1,:] = torch.unsqueeze(cbbox,0)
    return target_batch
```

> 要从batch里面获得label，首先要想清楚label(就是bounding box)应该是什么size，输出的结果应该是 ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7%5Ctimes30) 的，所以label的size应该是：[batch_size,7,7,30]。在这个程序里我们实现的是输出 ![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7%5Ctimes5) 。这个 ![[公式]](https://www.zhihu.com/equation?tex=5) 就是x,y,w,h，所以label的size应该是：[batch_size,7,7,5]
> batch_labels表示这个batch的第i个图片的label，number_box表示这个图有几个真值框。
> 接下来3重循环遍历每个grid的每个框，bbox表示正在遍历的这个框。
> bbox = bbox/ torch.tensor([wimg,himg,wimg,himg])表示对x,y,w,h进行归一化。
> 接下来if语句得到confidence的真值，存储在target_batch中返回。

最后是loss函数：

```python3
def lossfunc_details(outputs,labels):
    # 判断维度
    assert ( outputs.shape == labels.shape),"outputs shape[%s] not equal labels shape[%s]"%(outputs.shape,labels.shape)
    #import pdb
    #pdb.set_trace()
    b,w,h,c = outputs.shape
    loss = 0
    #import pdb
    #pdb.set_trace()
    conf_loss_matrix = torch.zeros(b,w,h)
    geo_loss_matrix = torch.zeros(b,w,h)
    loss_matrix = torch.zeros(b,w,h)
    
    for bi in range(b):
        for wi in range(w):
            for hi in range(h):
                #import pdb
                #pdb.set_trace()
                # detect_vector=[confidence,x,y,w,h]
                detect_vector = outputs[bi,wi,hi]
                gt_dv = labels[bi,wi,hi]
                conf_pred = detect_vector[0]
                conf_gt = gt_dv[0]
                x_pred = detect_vector[1]
                x_gt = gt_dv[1]
                y_pred = detect_vector[2]
                y_gt = gt_dv[2]
                w_pred = detect_vector[3]
                w_gt = gt_dv[3]
                h_pred = detect_vector[4]
                h_gt = gt_dv[4]
                loss_confidence = (conf_pred-conf_gt)**2 
                #loss_geo = (x_pred-x_gt)**2 + (y_pred-y_gt)**2 + (w_pred**0.5-w_gt**0.5)**2 + (h_pred**0.5-h_gt**0.5)**2
            
                loss_geo = (x_pred-x_gt)**2 + (y_pred-y_gt)**2 + (w_pred-w_gt)**2 + (h_pred-h_gt)**2
                loss_geo = conf_gt*loss_geo
                loss_tmp = loss_confidence + 0.3*loss_geo
                #print("loss[%s,%s] = %s,%s"%(wi,hi,loss_confidence.item(),loss_geo.item()))
                loss += loss_tmp
                conf_loss_matrix[bi,wi,hi]=loss_confidence
                geo_loss_matrix[bi,wi,hi]=loss_geo
                loss_matrix[bi,wi,hi]=loss_tmp
    #打印出batch中每张片的位置loss,和置信度输出
    print(geo_loss_matrix)
    print(outputs[0,:,:,0]>0.5)
    return loss,loss_matrix,geo_loss_matrix,conf_loss_matrix
```

> 首先需要注意：label和output的size应该是：[batch_size,7,7,5]。
> outputs[bi,wi,hi]就是一个5位向量： ![[公式]](https://www.zhihu.com/equation?tex=%28c%5E%7Bpred%7D%2Cx%5E%7Bpred%7D%2Cy%5E%7Bpred%7D%2Cw%5E%7Bpred%7D%2Ch%5E%7Bpred%7D%29) 。
> 我们分别计算了loss_confidence和loss_geo，因为我们实现的这个模型只检测1个类，所以没有class_loss。



终于把YOLO v1写完了，实在写不动了，接下来的几种模型等我的下一篇文章吧(附一个总结)：

**YOLO v1：直接回归出位置。**

**YOLO v2：全流程多尺度方法。**

**YOLO v3：多尺度检测头，resblock darknet53**

**YOLO v4：cspdarknet53，spp，panet，tricks**



编辑于 2022-03-28 01:58
