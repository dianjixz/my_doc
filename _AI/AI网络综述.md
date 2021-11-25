# 深度学习中的一些网络

- BP

​    BP神经网络是一种多层的前馈神经网络，其主要的特点是：信号是前向传播的，而误差是反向传播的。

- CNN 

​    卷积神经网络（Convolutional Neural Networks, CNN）是一类包含[卷积](https://baike.baidu.com/item/卷积/9411006)计算且具有深度结构的[前馈神经网络](https://baike.baidu.com/item/前馈神经网络/7580523)（Feedforward Neural Networks），是[深度学习](https://baike.baidu.com/item/深度学习/3729729)（deep learning）的代表算法之一 [1-2] 。卷积神经网络具有[表征学习](https://baike.baidu.com/item/表征学习/2140515)（representation learning）能力，能够按其阶层结构对输入信息进行平移不变分类（shift-invariant  classification），因此也被称为“平移不变人工神经网络（Shift-Invariant Artificial Neural  Networks, SIANN）”

- RNN

​    RNN是一种特殊的神经网络结构, 它是根据"人的认知是基于过往的经验和记忆"这一观点提出的. 它与DNN,CNN不同的是: 它不仅考虑前一时刻的输入,而且赋予了网络对前面的内容的一种'记忆'功能.  
​    RNN之所以称为循环神经网路，即一个序列当前的输出与前面的输出也有关。具体的表现形式为网络会对前面的信息进行记忆并应用于当前输出的计算中，即隐藏层之间的节点不再无连接而是有连接的，并且隐藏层的输入不仅包括输入层的输出还包括上一时刻隐藏层的输出。

- DNN

​    DNN是指深度神经网络。与RNN循环神经网络、CNN卷积神经网络的区别就是DNN特指全连接的神经元结构，并不包含卷积单元或是时间上的关联。