 

# **一、环境安装**

本机或者服务器[安装 pytorch](https://pytorch.org/get-started/locally/)

分类训练数据量小只需要 CPU就能够胜任

 ``` pip install torch ```

# **二、 分类**

使用了resnet18：

https://github.com/sipeed/maix_train/tree/master/pytorch/classifier

~~~ python
classes = ('chair', 'people')
dataset_path = "data/datasets"
val_split_from_data = 0.1 # 10%
batch_size = 4
learn_rate = 0.001
total_epoch = 100
eval_every_epoch = 5
save_every_epoch = 20
dataload_num_workers = 2
input_shape = (3, 224, 224)
cards_id = [1, 2]
param_save_path = './out/classifier_{}.pth'
~~~

cards_id 用 nvidia-smi 查看， 哪个显卡显存没有被占用就用哪个， 服务器4张显卡编号从0 到 4

~~~ bash

| NVIDIA-SMI 460.91.03    Driver Version: 460.91.03    CUDA Version: 11.2     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|                               |                      |               MIG M. |
|===============================+======================+======================|
|   0  GeForce GTX 108...  Off  | 00000000:04:00.0 Off |                  N/A |
|  0%   31C    P8     9W / 250W |  10647MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   1  GeForce GTX 108...  Off  | 00000000:08:00.0 Off |                  N/A |
|  0%   34C    P8    12W / 250W |    460MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   2  GeForce GTX 108...  Off  | 00000000:86:00.0 Off |                  N/A |
|  0%   26C    P8     8W / 280W |    460MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   3  GeForce GTX 108...  Off  | 00000000:8A:00.0 Off |                  N/A |
|  0%   33C    P8    12W / 250W |    189MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+

~~~

训练： 

数据集：一个分类一个文件夹， 文件夹名字就是分类的名字

![jietu](/home/nihao/work/my_doc/image/jietu.png)

python classifier_resnet_train.py

看 train loss 和 val loss， 当val loss不再下降， 就可以认为达到了最优， 每隔 save_every_epoch 个 epoch 就会保存一个模型参数到 out 文件夹

训练结束后， 可以测试图片， 执行 python classifier_resnet_test.py images_folder_path model_param_path

这会导出 onnx 模型文件， 以及ncnn模型， 这里需要编译[ncnn](https://github.com/Tencent/ncnn) ， 在build/tools/onnx 目录下得到 onnx2ncnn 工具

然后将ncnn导出为awnn， 参考：[ https://neucrack.com/p/358](https://neucrack.com/p/358) ， 这里使用了maixhub的工具， 也可以使用离线工具(需要opencv3， 参考 https://neucrack.com/p/349）

~~~ bash
awnntools optimize resnet.param resnet.bin opt.param opt.bin

awnntools calibrate -p="opt.param" -b="opt.bin" -i="images/224x224"  -m="127.5, 127.5, 127.5" -n="0.0078125, 0.0078125, 0.0078125" -o="opt.table" -s="224, 224" -c="swapRB" -t=8
awnntools quantize opt.param opt.bin  resnet_int8.param resnet_int8.bin opt.table
~~~

# **三、检测**

源工程: https://github.com/yjh0410/yolov2-yolov3_PyTorch

基于这个工程适当修改了下

训练:

\* 准备数据集, 使用 VOC 数据集同样的格式, 文件夹取名为 custom 放到 data 目录下, 比如

~~~ bash
├── custom
│   ├── Annotations
│   ├── ImageSets
│   │    └── Main
│   │         ├── train.txt
│   │         └── val.txt
│   ├── JPEGImages
~~~

train.txt 和 val.txt 中, 没一行是一个数据(图像)名, 路径相对于`JPEGImages`

\* 修改配置

修改`data/custom.py`中的`CUSTOM_CLASSES`变量为正确的 `labels`

\* 训练

~~~ bash
python train.py -d custom --cuda -v slim_yolo_v2 -hr -ms
~~~

或者安装好horovod, 然后多卡训练

~~~ ba
horovodrun -np 4 python train.py -d custom --cuda -v slim_yolo_v2 -hr -ms
~~~

\* 导出模型

~~~ ba
python test.py -d custom -v slim_yolo_v2 --trained_model weights/custom/slim_yolo_v2/slim_yolo_v2_1000.pth --visual_threshold 0.3 -size 224 --export
~~~

