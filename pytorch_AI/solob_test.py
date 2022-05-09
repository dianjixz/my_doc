#!/usr/bin/env python3
'''
    simple sobel edge demo
    visit: https://neucrack.com/p/377
    @author neucrack
    @license MIT
'''
# import torch
# import torch.nn as nn
# import numpy as np
# import cv2
# from PIL import Image
# import matplotlib.pyplot as plt


# class Net(nn.Module):
#     def __init__(self):
#         super(Net, self).__init__()
#         self.conv1 = nn.Conv2d(3, 3, 3, padding=(0, 0), bias=False)
#     def forward(self, x):
#         x = self.conv1(x)
#         return x


# net = Net()


# conv_rgb_core_sobel = [
#     [[-1, -1, -1], [-1, 8, -1], [-1,    -1,    -1],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0]
#      ],
#     [[0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [-1, -1, -1], [-1, 8, -1], [-1,    -1,    -1],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0]
#      ],
#     [[0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [-1, -1, -1], [-1, 8, -1], [-1,    -1,    -1],
#      ]]
# conv_rgb_core_sobel_vertical = [
#     [[-1, 0, 1], [-2, 0, 2], [-1,    0,    1],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0]
#      ],
#     [[0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [-1, 0, 1], [-2, 0, 2], [-1,    0,    1],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0]
#      ],
#     [[0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [-1, 0, 1], [-2, 0, 2], [-1,    0,    1],
#      ]]
# conv_rgb_core_sobel_horizontal = [
#     [[1, 2, 1], [0, 0, 0], [-1, -2, -1],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0]
#      ],
#     [[0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [1, 2, 1], [0, 0, 0], [-1, -2, -1],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0]
#      ],
#     [[0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [0, 0, 0], [0, 0, 0], [0, 0, 0],
#      [1, 2, 1], [0, 0, 0], [-1, -2, -1],
#      ]]


# def sobel(net, kernel):
#     sobel_kernel = np.array(kernel,    dtype='float32')
#     sobel_kernel = sobel_kernel.reshape((3,    3,    3,    3))
#     net.conv1.weight.data = torch.from_numpy(sobel_kernel)


# params = list(net.parameters())

# img = cv2.imread("/home/nihao/work/lin/pictures/class1_5.jpg")
# input_img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
# input_tensor = (input_img.astype(np.float32) - 127.5) / 128  # to [-1, 1]
# input_tensor = torch.Tensor(input_tensor).permute((2, 0, 1))

# print(input_tensor.shape)

# input_tensor = input_tensor.unsqueeze(0)

# print("input shape:", input_tensor.shape)


# sobel(net, conv_rgb_core_sobel)
# out = net(input_tensor).detach().numpy()[0].transpose([1, 2, 0])

# sobel(net, conv_rgb_core_sobel_vertical)
# out_v = net(input_tensor).detach().numpy()[0].transpose([1, 2, 0])

# sobel(net, conv_rgb_core_sobel_horizontal)
# out_h = net(input_tensor).detach().numpy()[0].transpose([1, 2, 0])

# print("out shape: {}, tensor:{}".format(out.shape, out))
# print(out.shape, out.max(), out.min())
# plt.figure()
# plt.figure()
# plt.subplot(1, 5, 1)
# input = input_tensor.numpy()[0].transpose((1, 2, 0))
# print(input.max(), input.min())
# plt.imshow(input_img)
# plt.subplot(1, 5, 2)
# print(out.max(), out.min())
# # out = np.sqrt(np.square(out))
# # out = out * 255.0 / out.max()
# # out = out.astype(np.uint8)
# # print(out.max(), out.min())
# plt.imshow(out)
# plt.subplot(1, 5, 3)
# out = np.abs(out_v)
# # out = out * 255.0 / out.max()
# # plt.imshow(out.astype(np.uint8))
# plt.imshow(out)
# plt.subplot(1, 5, 4)
# out = np.abs(out_h)
# # out = out * 255.0 / out.max()
# # plt.imshow(out.astype(np.uint8))
# plt.imshow(out)
# plt.subplot(1, 5, 5)
# out = np.sqrt(np.square(out_v) + np.square(out_h))
# # out = out * 255.0 / out.max()
# # plt.imshow(out.astype(np.uint8))
# plt.imshow(out)
# plt.show()



'''
    simple sobel edge demo
    visit: https://neucrack.com/p/377
    @author neucrack
    @license MIT
'''
import torch
import torch.nn as nn
import numpy as np
import cv2
from PIL import Image
import matplotlib.pyplot as plt



class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(3, 3, 3, padding=(0, 0), bias=False)
    def forward(self, x):
        x = self.conv1(x)
        return x


net = Net()




# sobel全边缘检测算子
conv_rgb_core_sobel = [
                        [[-1,-1,-1],[-1,8,-1], [-1,    -1,    -1],
                         [0,0,0],[0,0,0], [0,0,0],
                         [0,0,0],[0,0,0], [0,0,0]
                        ],
                        [[0,0,0],[0,0,0], [0,0,0],
                         [-1,-1,-1],[-1,8,-1], [-1,    -1,    -1],
                         [0,0,0],[0,0,0], [0,0,0]
                        ],
                        [[0,0,0],[0,0,0], [0,0,0],
                         [0,0,0],[0,0,0], [0,0,0],
                         [-1,-1,-1],[-1,8,-1], [-1,    -1,    -1],
                        ]]

# sobel垂直边缘检测算子
conv_rgb_core_sobel_vertical = [
                        [[-1,0,1],[-2,0,2], [-1,    0,    1],
                         [0,0,0],[0,0,0], [0,0,0],
                         [0,0,0],[0,0,0], [0,0,0]
                        ],
                        [[0,0,0],[0,0,0], [0,0,0],
                         [-1,0,1],[-2,0,2], [-1,    0,    1],
                         [0,0,0],[0,0,0], [0,0,0]
                        ],
                        [[0,0,0],[0,0,0], [0,0,0],
                         [0,0,0],[0,0,0], [0,0,0],
                         [-1,0,1],[-2,0,2], [-1,    0,    1],
                        ]]

# sobel水平边缘检测算子
conv_rgb_core_sobel_horizontal = [
                        [[1,2,1],[0,0,0], [-1, -2, -1],
                         [0,0,0],[0,0,0], [0,0,0],
                         [0,0,0],[0,0,0], [0,0,0]
                        ],
                        [[0,0,0],[0,0,0], [0,0,0],
                         [1,2,1],[0,0,0], [-1, -2, -1],
                         [0,0,0],[0,0,0], [0,0,0]
                        ],
                        [[0,0,0],[0,0,0], [0,0,0],
                         [0,0,0],[0,0,0], [0,0,0],
                         [1,2,1],[0,0,0], [-1, -2, -1],
                        ]]

# 网络载入权重函数
def sobel(net, kernel):
    sobel_kernel = np.array(kernel,    dtype='float32')
    sobel_kernel = sobel_kernel.reshape((3,    3,    3,    3))
    net.conv1.weight.data = torch.from_numpy(sobel_kernel)


params = list(net.parameters())
img = cv2.imread("/home/nihao/work/lin/pictures/class1_5.jpg")

# cv2.imshow('input_image', img)
# cv2.waitKey(0)


input_img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

input_tensor = (input_img.astype(np.float32) - 127.5) / 128 # to [-1, 1]
input_tensor = torch.Tensor(input_tensor).permute((2, 0, 1))
print(input_tensor.shape)
input_tensor = input_tensor.unsqueeze(0)
print("input shape:", input_tensor.shape)
sobel(net, conv_rgb_core_sobel)
out = net(input_tensor).detach().numpy()[0].transpose([1,2,0])






plt.figure()
plt.subplot(1, 3, 1)
plt.imshow(out)

plt.subplot(1, 3, 2)

md = np.abs(out)

plt.imshow(md)

plt.subplot(1, 3, 3)

mc = np.around(md)

plt.imshow(mc)

plt.show()