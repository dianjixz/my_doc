# ax620 模型训练转换

经过很久的基础工作，现在开始进行 ax620 的模型训练工作。距离官方的 620 模型训练教程的编写时间已经过很长时间，一些东西也已经改变。重新部署的同时也记录好这次的踩坑的经过。




# 坑点
yolov8的仓库的代码已经改变，想过的代码也已经不在原来的位置了。  
训练和导出的时候需要的网络结构的代码是不一样的，这个还是比较烦人的。  
爱芯提供的模型转换工具真的一言难尽，第一天转化模型就花费了4个小时。在这部署训练的过程基本很难接受。




git clone https://github.com/ultralytics/ultralytics.git  # clone
cd ultralytics
git checkout ec10002a4ade5a43abb9d5765f77eefddf98904b







相关链接：
[爱芯元智AX620A部署yolov5 6.0模型实录](https://zhuanlan.zhihu.com/p/569083585)  
[AX620A转换yolo系列模型踩坑](https://blog.csdn.net/flamebox/article/details/130970851?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-130970851-blog-127249243.235%5Ev38%5Epc_relevant_anti_t3_base&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-130970851-blog-127249243.235%5Ev38%5Epc_relevant_anti_t3_base&utm_relevant_index=3)  



[yolov8仓库 ultralytics](https://github.com/ultralytics/ultralytics.git)


[模型部署教程 m3axpi_model](https://github.com/Abandon-ht/m3axpi_model.git)


[2023年最新 使用 YOLOv8 训练自己的数据集，并在 爱芯派硬件 上实现 目标检测 和 钢筋检测 ！！](https://www.yuque.com/prophetmu/chenmumu/m3axpi#kdjIY)

释放出的转换docker仓库:sipeed/pulsar
