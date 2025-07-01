# ax630 模型训练转换

ax630 是公司的新款AI芯片，基于双核 ARM Cortex-A55，拥有 NPU 加速硬件，能够很好的加速深度神经网络的运算。

ax630 升级了模型包的格式，新的模型后缀为 **.axmodel** ,官方说这个模型和 onnx 格式兼容，可以直接改为onnx，使用Netron查看模型结构。但axmodel只是将神经网络封装成了onnx一个定制算子，并不能直接使用onnx的推理引擎。也很难使用Netron查看原来的神经网络结构。

## 模型训练
在这里使用yolov5模型的训练举例说明，使用 https://github.com/m5stack/ultralytics.git 进行训练。

