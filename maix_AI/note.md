 

- 简单的cnn网络层

~~~ python
作者：机器之心
链接：https://www.zhihu.com/question/39022858/answer/203073911
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

model = Sequential()

model.add(keras.layers.InputLayer(input_shape=input_shape))

model.add(keras.layers.convolutional.Conv2D(filters, filtersize, strides=(1, 1), padding='valid', data_format="channels_last", activation='relu'))
model.add(keras.layers.MaxPooling2D(pool_size=(2, 2)))
model.add(keras.layers.Flatten())

model.add(keras.layers.Dense(units=2, input_dim=50,activation='softmax'))

model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model.fit(images, label, epochs=epochs, batch_size=batchsize,validation_split=0.3)

model.summary()

~~~

CNN的部件其实大致分为三个，卷积层、池化层、全连接层，这也是LeNet-5的经典结构



暴力加深流派：以AlexNet和VGGNet为首的模型，这一派观点很直接，就是不断交替使用卷积层池化层，暴力增加网络层数，最后接一下全连接层分类。这类模型在CNN早期是主流，特点是参数量大，尤其是后面的全连接层，几乎占了一般参数量。而且相比于后续的模型，效果也较差。因此这类模型后续慢慢销声匿迹了。

Inception流派：谷歌流派，这一派最早起源于NIN，称之为网络中的网络，后被谷歌发展成Inception模型（这个单词真的不好翻译。。。）。这个模型的特点是增加模型的宽度，使得模型不仅仅越长越高，还越长越胖。也就是说每一层不再用单一的卷积核卷积，而是用多个尺度的卷积核试试。显然，如果你熟悉CNN，就不难发现，这样做会使每一层的feature map数量猛增，因为一种尺寸的卷积核就能卷出一系列的feature map，何况多个！这里google使用了1*1的卷积核专门用来降channel。谷歌的特点是一个模型不玩到烂绝不算完，所以又发展出了Inception v2、Inception v3、Inception v4等等

残差流派：2015年ResNet横空出世，开创了残差网络。使用残差直连边跨层连接，居然得到了意想不到的好效果。最重要的是，这一改进几乎彻底突破了层数的瓶颈，1000层的resnet不是梦！之后，最新的DenseNet丧心病狂地在各个层中间都引入了残差连接。目前大部分模型都在尝试引入残差连接。

注意，到此为止，大部分模型已经丢弃了全连接层，改为全局平均池化。大大降低了参数量。

混合流派：这一派不说了，就是看到哪几类模型效果好，就把这类技术混杂起来。典型的就是Xception和ResIception，将Inception和残差网络结合起来了。

BatchNromalization：不得不提这个批量标准化技术，在此技术出现之前，CNN收敛很慢，此技术出现后，大大加快了模型收敛速度，还兼具一定的防过拟合效果。当然，这个技术不仅仅限于CNN。

作者：张旭
链接：https://www.zhihu.com/question/39022858/answer/120211609
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



- LeNet：第一个成功的卷积神经网络应用
- AlexNet：类似LeNet，但更深更大。使用了层叠的卷积层来抓取特征（通常是一个卷积层马上一个max pooling层）
- ZF Net：增加了中间卷积层的尺寸，让第一层的stride和filter size更小。
- GoogLeNet：减少parameters数量，最后一层用max pooling层代替了全连接层，更重要的是[Inception-v4](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1602.07261)模块的使用。
- VGGNet：只使用3x3 卷积层和2x2 pooling层从头到尾堆叠。
- ResNet：引入了跨层连接和batch normalization。
- DenseNet：将跨层连接从头进行到尾。

手写神经网络图展示:https://www.cs.ryerson.ca/~aharley/vis/conv/

https://m.huxiu.com/article/138857/1.html
