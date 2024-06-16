目标检测网络的框架

    Input：Image、Patches、Image Pyramid等。
    BackBone：目标检测网络的主干，主要作用是对进行特征提取，常需要对大型数据集进行与训练。常见有VGG系列、ResNet系列、SpineNet、EfficientNet系列等。
    Head：目标检测网络的头部，主要作用是预测对象所属的类别和可边界框的位置大小。分为one-stage和two-stage两种。one-stage以YOLO、SSD、RetinaNet为代表，two-stage以R-CNN系列为代表。
        two-stage方法：主要思路是先通过启发式算法（selective search）或者CNN网络（RPN）产生一系列稀疏的候选框，然后对这些候选框进行分类和回归。例如R-CNN、Fast R-CNN、Faster R-CNN等，其优势是准确度较高。
        one-stage方法：主要思路是均匀地在图片的不同位置进行密集抽样，抽样时可以采用不同尺度和长宽比，然后利用CNN提取特征后直接进行分类与回归，整个过程只需要一步。例如YOLO、SSD等，其优势是速度快，但是均匀的密集采样的一个重要缺点是训练比较困难，这主要是因为正样本与负样本（背景）极其不均衡，导致模型准确度稍低。
    Neck:随着后来的发展，人们常在BackBone和Head之间添加一些网络层，称为Neck。Neck常由几个自下而上的path或自上而下的path组成。主要分为Additional Blocks和Path-aggregation blocks。
        Additional Blocks：SPP、ASPP、RFB、SAM。
        Path-aggregation blocks：FPN、PAN、BiFPN、NAS-FPN。
————————————————
版权声明：本文为CSDN博主「Rosen.」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/Rosen_er/article/details/122774959








检测类的神经网络的总体结构：
一.head、neck与backbone

在刚刚接触目标检测时，会接触到几个术语head、neck与backbone，在逐渐熟悉算法之后，对这三个部分开始有了理解：在进行目标识别任务时，无论是one-stage或者two-stage，都会有以下的任务依次完成：使用卷积神经网络进行特征提取，使用得到的特征进行目标识别。完成特征提取的网络就叫做backbone，其输入为原始图像，输出为特征。完成识别任务的网络例如fasterrcnn中的RPN等，叫做head（检测头），其作用就是完成任务的主体：检测框定位与目标分类等。

但是由于目标检测任务的特殊性，其具有多尺度的信息（大小维度不同的目标），所以使用单一维度的特征（RCNN等）有欠缺，这时，neck就被引出使用连接检测头head与身体backbone，完成特征增强的任务（多尺度特征融合等），常见的有FPN等。

通过以下示意图就可以直观了解到三个模块之间的联系与信息的传递
![](https://pic2.zhimg.com/80/v2-e606e3c55a504fbe3ac802d6eecf3311_720w.webp)
图中使用的neck就是FPN，也就是接下来要讲的。


速通目标检测：head、neck与backbone和特征图像金字塔（FPN）含代码解读
https://zhuanlan.zhihu.com/p/596689721


backbone

"Backbone" 是深度学习中的一个术语，通常用于描述卷积神经网络 (Convolutional Neural Network, CNN) 或其他神经网络架构的一部分。这个术语指的是网络的主要结构或骨干部分，它负责提取输入数据的特征，并将这些特征传递给后续的层来执行任务，如分类、检测或分割。

    特征提取：骨干网络包括卷积层、池化层和正则化层，它们协同工作以从原始输入数据中提取有用的特征。这些特征可以是图像、文本、声音等不同类型的数据，具体取决于网络的应用领域。

    迁移学习：骨干网络通常在大规模数据集上进行预训练，这使得它们能够捕获大量的通用特征。这些预训练的骨干网络可以用作迁移学习的基础，以加速新任务的训练过程和提高性能。

backbone 的主要用途是为深度学习任务提供强大的特征提取和表示学习能力，从而提高模型的性能和泛化能力。不同的任务可能需要不同的 backbone 架构和权重。
head

head是获取网络输出内容的网络，利用之前提取的特征，head利用这些特征，做出预测。

"head" 是神经网络模型的一个关键组成部分，主要负责执行特定任务的操作和生成模型的最终输出。Head 的功能和作用因任务类型而异，以下是一些常见的任务以及 Head 的主要功能和作用：

    图像分类任务：
        Head 用于将骨干网络（如卷积神经网络）提取的图像特征映射到类别标签。
        通常包括一个或多个全连接层，最后一层通常是 softmax 层，用于产生类别概率分布。

    目标检测任务：
        在目标检测中，Head 用于预测目标的位置（边界框）和类别。
        通常包括两个分支：一个用于回归边界框的坐标，另一个用于预测目标类别。
        Head 的输出会生成多个边界框和相应的类别概率。

    图像分割任务：
        在图像分割中，Head 负责生成像素级的类别标签。
        Head 可能包括卷积和上采样层，以将骨干网络提取的特征映射到与输入图像相同大小的分割结果。

    自然语言处理任务：
        在自然语言处理中，Head 可用于执行文本分类、情感分析、命名实体识别等任务。
        Head 可能包括全连接层、递归神经网络（RNN）、长短时记忆网络（LSTM）等结构，用于处理文本数据。

       总之，Head 的主要功能是将从骨干网络提取的特征映射到任务相关的输出。这些特征映射可能包括类别标签、位置信息、文本分类等，具体取决于任务的性质。 Head 的设计通常是根据任务的需求和网络架构来定制的，以确保模型能够有效地解决特定任务。
neck

"neck" 是指神经网络模型中的一个组成部分，通常位于“骨干网络”（backbone）和“头部”（head）之间。Neck 的作用是进一步处理骨干网络提取的特征，并将其准备好用于特定任务的头部。

    特征融合和增强：Neck 通常包括卷积层、池化层和其他操作，用于融合、增强和调整骨干网络提取的特征。这有助于提高特征的表示能力，使其更适合任务的要求。

    尺度调整：在一些计算机视觉任务中，不同尺度的特征对于正确执行任务非常重要。Neck 可以用于将多个尺度的特征融合在一起，以增强模型的多尺度感知能力。

    特征金字塔：一种常见的 Neck 结构是特征金字塔网络（Feature Pyramid Network, FPN），它可以用于处理多尺度输入并生成多尺度的特征图，适用于目标检测和分割等任务。

    上采样和下采样：Neck 可能包括上采样层和下采样层，用于将特征图的分辨率调整到与任务相匹配的分辨率。

    连接不同任务的头部：在多任务学习中，Neck 可以用于连接不同的头部，以共享特征提取过程并提高模型的效率和性能。

"neck" 在神经网络模型中扮演了一个关键的角色，它有助于进一步处理和改进从骨干网络提取的特征，以便适应特定任务的需求。
————————————————
版权声明：本文为CSDN博主「何处不逢君」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/danger2/article/details/134028394


backbone：主干网络，用来提取特征，常用Resnet、VGG等

head：获取网络输出，利用提取特征做出预测

neck：放在backbone和head之间，进一步提升特征的多样性及鲁棒性

bottleneck：瓶颈，通常指网络输入输出数据维度不同，通常输出维度比输入维度小很多

GAP：Global Average Pool全局平均池化，将某个通道的特征取平均值

Warm up：小的学习率先训练几个epoch，这是因为网络的参数是随机初始化的，一开始就采用较大的学习率容易数值不稳定


二、常用backbone

    VGG
    ResNet（ResNet18，50，100）
    ResNeXt
    DenseNet
    SqueezeNet
    Darknet（Darknet19,53）
    MobileNet
    ShuffleNet
    DetNet
    DetNAS
    SpineNet
    EfficientNet（EfficientNet-B0/B7）
    CSPResNeXt50
    CSPDarknet53
![](https://img-blog.csdnimg.cn/img_convert/4ae21097c01eb270cb55d46800516d0c.png)



三、常用Head

Dense Prediction (one-stage):

    RPN
    SSD
    YOLO
    RetinaNet
    (anchor based)
    CornerNet
    CenterNet
    MatrixNet
    FCOS(anchor free)

Sparse Prediction (two-stage):

    Faster R-CNN
    R-FCN
    Mask RCNN (anchor based)
    RepPoints(anchor free)


四、常用neck

Additional blocks:

    SPP
    ASPP
    RFB
    SAM

Path-aggregation blocks:

    FPN
    PAN
    NAS-FPN
    Fully-connected FPN
    BiFPN
    ASFF
    SFAM
    NAS-FPN
![](https://img-blog.csdnimg.cn/img_convert/f07fdcaf4808272bc58ed2bb09459b12.png)

五、Skip-connections

    Residual connections
    Weighted residual connections
    Multi-input weighted residual connections
    Cross stage partial connections (CSP)

六、常用激活函数和loss

激活函数：

    ReLU
    LReLU
    PReLU
    ReLU6
    Scaled Exponential Linear Unit (SELU)
    Swish
    hard-Swish
    Mish

loss：

    MSE
    Smooth L1
    Balanced L1
    KL Loss
    GHM loss
    IoU Loss
    Bounded IoU Loss
    GIoU Loss
    CIoU Loss
    DIoU Loss

七、正则化和BN方式

正则化：

    DropOut
    DropPath
    Spatial DropOut
    DropBlock

BN：

    Batch Normalization (BN)
    Cross-GPU Batch Normalization (CGBN or SyncBN)
    Filter Response Normalization (FRN)
    Cross-Iteration Batch Normalization (CBN)


八、训练技巧

    Label Smoothing
    Warm Up















