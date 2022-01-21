https://www.cnblogs.com/laozhanghahaha/p/12580822.html

# [图像插值算法总结](https://www.cnblogs.com/laozhanghahaha/p/12580822.html) 

插值指的是利用已知数据去预测未知数据，图像插值则是给定一个像素点，根据它周围像素点的信息来对该像素点的值进行预测。当我们调整图片尺寸或者对图片变形的时候常会用到图片插值。比如说我们想把一个4x4的图片， 就会产生一些**新的像素点**（ 如下图红点所示），

如何给这些值赋值， 就是图像插值所要解决的问题, 图片来源[此网址](http://www.1zlab.com/wiki/python-opencv-tutorial/opencv-interpolation-algrithm/)

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325124525249-2039725060.png)

 

常见的插值算法可以分为两类：自适应和非自适应。 自适应的方法可以根据插值的内容来改变（尖锐的边缘或者是平滑的纹理），非自适应的方法对所有的像素点都进行同样的处理。 非自适应算法包括：最近邻，双线性，双三次，样条，sinc，lanczos等。 由于其复杂度, 这些插值的时候使用从0 to 256 (or more) 邻近像素。 包含越多的邻近像素，他们越精确，但是花费的时间也越长。这些算法可以用来扭曲和缩放照片。

自适应算法包括许可软件中的许多专有算法，例如：Qimage，PhotoZoom Pro和正版Fractals。这篇博客通过opencv中cv.resize()函数介绍一些非自适应性插值算法

```
cv2.resize(src, dsize[, dst[, fx[, fy[, interpolation]]]]) → dst
```

其中interpolation的选项包括，图片来源[此网址](https://blog.csdn.net/JNingWei/article/details/78822026)

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200327130832096-907395494.png)

 

------

 

我们主要介绍最近邻，线性插值，双三次插值三种插值方式，下图是对双三次插值与一些一维和二维插值的比较。 黑色和红色/黄色/绿色/蓝色点分别对应于插值点和相邻样本。 点的高度与其值相对应。图片来源于[维基百科](https://en.wikipedia.org/wiki/Bicubic_interpolation)

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325125319873-461037239.png) 

# 最近邻

顾名思义最近邻插值就是选取离目标点最近的点的值(黑点，原来就存在的点)作为新的插入点的值，

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325125141948-1453328317.png) 

 用opencv进行图像处理时，根据

```
srcX = dstX* (srcWidth/dstWidth)
srcY = dstY * (srcHeight/dstHeight)
```

得到来计算目标像素在源图像中的位置，dstY代表输出图Y的坐标，srcY代表原图Y的坐标，srcX、srcY同理。这里计算的srcX和srcY一般都是浮点数，比如原图A尺寸为3x3，扩大后的图片B的尺寸为7x7，如下图

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325125208055-282995952.png)

那么B中(1,4)这个点在A图中的位置就是(1*(3/7), 4*(3/7))->(0.43, 1.71) ，四舍五入也就是(0, 2)，所以把源图像中(0,2)点的像素值0填入到目标图像中

插值后的边缘效果：由于是以最近的点作为新的插入点，因此边缘不会出现缓慢的渐慢过度区域，这也导致放大的图像容易出现锯齿的现象，缩小后的图像有很严重的失真；效果不好的根源就是其简单的最临近插值方法引入了严重的图像失真，比如，当由目标图的坐标反推得到的源图的的坐标是一个浮点数的时候，采用了四舍五入的方法，直接采用了和这个浮点数最接近的象素的值，这种方法是很不科学的，当推得坐标值为 0.75的时候，不应该就简单的取为1，既然是0.75，比1要小0.25 ，比0要大0.75 ,那么目标象素值其实应该根据这个源图中虚拟的点四周的四个真实的点来按照一定的规律计算出来的，这样才能达到更好的缩放效果。双线型内插值算法就是一种比较好的图像缩放算法，它充分的利用了源图中虚拟点四周的四个真实存在的像素值来共同决定目标图中的一个像素值，因此缩放效果比简单的最邻近插值要好很多。

# 线性插值

线性插值是以距离为权重的一种插值方式。已知数据 (x0, y0) 与 (x1, y1)，要计算 [x0, x1] 区间内某一位置 x 在直线上的y值：

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325132134131-1667070702.png)

 把上边的式子整理得：

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325132310225-844732946.png)

这个式子的意思就是用x和x0，x1的距离作为一个权重，用于y0和y1的加权。双线性插值本质上就是在两个方向上做线性插值。

# 双线性插值

在数学上，双线性插值是有两个变量的插值函数的线性插值扩展，其核心思想是在两个方向分别进行一次线性插值。见下图：

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325132455721-1181643405.png)

我们想得到未知函数 f 在点 P = (x, y) 的值，假设我们已知函数 f 在 Q11 = (x1, y1)、Q12 = (x1, y2), Q21 = (x2, y1) 以及 Q22 = (x2, y2) 四个点的值。最常见的情况，f就是一个像素点的像素值。思路是我们可以将求解过程分解为两次插值过程，首先在x轴方向上进行插值，根据点Q11,Q21得到

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325133518541-703290109.png)

根据点Q12,Q22得到 

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325133605450-198295277.png)

然后在y轴方向上进行插值， 根据点R1 和R2得到

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325133736327-1921924791.png)

整理得

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200325134759278-781722354.png)

仍以刚刚的AB图像为例，B中(1,4)点对应A图中(0.43, 1.71)这个点，将该点像素值f(0.43,1.71)写成 f(i+u,j+v)的形式，则 `u=0.43,v=0.71, i=0, j=1，该点在A中是`虚拟存在的，先找到A中与它临近的四个实际存在的像素点

```
（0，1） （0，2）
（1，1） （1，2）
```

根据上边整理的公式得

```
f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1) 
```

 由于图像双线性插值只会用相邻的4个点，因此上述公式的分母都是1, 带入相应数值

```
f(0+0.43, 1+0.71) = 0.57*0.29*f(0,1)+0.57*0.71*f(0, 2)+0.43*0.29*f(1,1)+0.43*0.71*f(1,2)
```

# 双三次插值

假设源图像A大小为m*n，缩放后的目标图像B的大小为M*N。那么根据上边提到的方法我们可以得到B(X,Y)在A上的对应坐标为

```
A(x,y)=A(X*(m/M),Y*(n/N))
```

在双线性插值法中，我们选取A(x,y)的最近四个点。而在双三次插值法中，我们选取的是最近的16个像素点作为计算目标图像B(X,Y)处像素值的参数。如图所示：

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200326125127555-891377641.png)

如图所示P点就是目标图像B在(X,Y)处对应于源图像A中的位置， 假设P的坐标为(i+u,j+v)，其中i,j分别表示整数部分，u,v分别表示小数部分。找到如图所示距离p最近的16个像素的位置，在这里用a(m,n)(m,n=0,1,2,3)来表示。双立方插值的目的就是通过找到一种关系，或者说系数，可以把这16个像素对于P处像素值得影响因子找出来，从而根据这个影响因子来获得目标图像对应点的像素值，达到图像缩放的目的。我在这次的学习中学习的是基于BiCubic基函数的双三次插值法，BiCubic基函数形式如下：
![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200326125618988-703180248.png)

我们要做的就是找到BiCubic函数中的参数x,从而获得上面所说的16个像素所对应的系数。在学习双线性插值法的时候，我们是把图像的行和列分开来理解的，那么在这里，我们也用这种方法描述如何求出a(m,n)对应的系数k_m_n。假设行系数为k_m,列系数为k_n。我们以a00位置为例：首先，我们要求出当前像素与P点的位置，比如a00距离P(i+u,j+v)的距离为(1+u,1+v)。那么a00的横坐标权重k_m_0=W(1+u)，纵坐标权重k_n_0=W(1+v).
同理我们可以得到所有行和列对应的系数：

k_m_0=W(1+u), k_m_1=W(u), k_m_2=W(1-u), k_m_3=W(2-u);
k_n_0=W(1+v), k_n_1=W(v), k_n_2=W(1-v), k_n_3=W(2-v);

这样我们就分别得到了行和列方向上的系数。

由横纵坐标权重的乘积可以得到每个像素a(m,n)对应的权值了。比如a00对B(X,Y)的贡献值为：（a00像素值）* k_m_0* k_n_0

最后通过求和公式可以得到目标图片B(X,Y)对应的像素值：
![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200328112654146-1797886902.png)

 其中W(m)表示amn横坐标上的权重，W(n)表示amn纵坐标上的权重。三次曲线插值方法相较于前面的最近邻和线性插值而言计算量较大，但插值后的图像效果最好。

------

 

以下是利用cv2.resize()函数对图像进行缩放的例子：

将图像缩小为原来的1/2

![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200327125255757-1858149976.png)

 

将图像放大为原来的2倍
![img](https://img2020.cnblogs.com/blog/721540/202003/721540-20200327125150002-96171760.png)

 reference [图像插值算法](http://www.1zlab.com/wiki/python-opencv-tutorial/opencv-interpolation-algrithm/)[
](http://www.1zlab.com/wiki/python-opencv-tutorial/opencv-interpolation-algrithm/#_1)

​         [OpenCV图像缩放resize各种插值方式的比较](https://blog.csdn.net/guyuealian/article/details/85097633?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)

​         [三十分钟理解：线性插值，双线性插值Bilinear Interpolation算法](https://blog.csdn.net/xbinworld/article/details/65660665)

​         [ 图像缩放之双三次插值法](https://blog.csdn.net/qq_29058565/article/details/52769497)

​         [图像处理: 五种 插值法](https://blog.csdn.net/JNingWei/article/details/78822026)

​         [最近邻插值、双线性插值、双三次插值](https://blog.csdn.net/caomin1hao/article/details/81092134?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)





# 图像缩放之双三次插值法

转自http://blog.csdn.net/qq_29058565/article/details/52769497

https://blog.csdn.net/qingkongyeyue/article/details/73554921











~~~ C
/** 
** method to remove sharp the raw image with unsharp mask 
* @param src input grayscale binary array  
* @param dst output grayscale result, the memory need to be allocated outside of the function 
* @param srcWidth width of the input grayscale image 
* @param srcHeight height of the input grayscale image 
* @param scalePercent, scale percentage (0-xxx) 
*/  
void stretchImage (const unsigned char* src, unsigned char* dst, int srcWidth, int srcHeight, int scalePercent)  
{     
    if (scalePercent < 0)  
        return;  
    int x, y;  
    int ox, oy;  
    int tmpx,tmpy;  
    int ratio = (100 << 8)/scalePercent;  
    int dstWidth = srcWidth * scalePercent / 100;  
    int dstHeight = srcHeight * scalePercent / 100;  
    unsigned char color[2][2];  
    for (int j = 0; j < dstHeight; j ++)  
    {  
        for (int i = 0; i < dstWidth; i ++)  
        {  
            tmpx = i * ratio;  
            tmpy = j * ratio;  
            ox = tmpx >> 8;  
            oy = tmpy >> 8;  
            x = tmpx & 0xFF;  
            y = tmpy & 0xFF;  
            color[0][0] = src[ oy*srcWidth + ox ];   
            color[1][0] = src[ oy*srcWidth + ox +1 ];   
            color[0][1] = src[ (oy+1)*srcWidth + ox ];   
            color[1][1] = src[ (oy+1)*srcWidth + ox+1 ];  
            int final = (0x100 - x)*(0x100 - y)*color[0][0] + x*(0x100 - y)*color[1][0] + (0x100-x)*y*color[0][1] + x*y*color[1][1];  
            final = final >> 16;  
            if (final>255)  
                final = 255;  
            if (final<0)  
                final = 0;  
            dst [ j*dstWidth + i] = (unsigned char)final;  
        }  
    }  
}  
~~~



~~~ c


static void _ieInterpImageBilinear8UC1_Ver3_RowFilter(unsigned char src, long dst, int len, int leftIdx, int rightIdx, long weight, int shift)
{
    int i;
    for(i = 0; i < len - 4; i+=4) {
        dst++ = ((1<<shift) - weight[i])src[leftIdx[i]] + weight[i]src[rightIdx[i]];
        dst++ = ((1<<shift) - weight[i+1])src[leftIdx[i+1]] + weight[i+1]src[rightIdx[i+1]];
        dst++ = ((1<<shift) - weight[i+2])src[leftIdx[i+2]] + weight[i+2]src[rightIdx[i+2]];
        dst++ = ((1<<shift) - weight[i+3])src[leftIdx[i+3]] + weight[i+3]*src[rightIdx[i+3]];


}
for( ; i < len; ++i) {
    *dst++ = ((1<<shift) - weight[i])*src[leftIdx[i]] + weight[i]*src[rightIdx[i]];
}
 

}


define IET_MAX(x,y) (x)>(y)?(x):(y)

define IET_MIN(x,y) (x)>(y)?(y):(x)

define IET_SWAP(x,y,tmp) (tmp)=(x);(x)=(y);(y)=(tmp);

static void ResizeImage(unsigned char pSrc,int src_w,int src_h,
                        unsigned char pDst,int dst_w, int dst_h)
{
    int i, j;
    int sw, sh, sstep;
    int dw, dh, dstep;
    unsigned char sdata, ddata;
    float horScaleRatio, verScaleRatio;
    long rowBuf1, rowBuf2;
    long upLinePtr, downLinePtr, tempPtr;
    long horWeight;
    int horLeftIdx, horRightIdx;
    int preVerUpIdx, preVerDownIdx;
    int shift = 8;



sw=src_w;
sh=src_h;
sstep=24;
sdata=pSrc;
dw=dst_w;
dh=dst_h;
dstep=24;
ddata=pDst;

horScaleRatio = sw / (float)(dw);
verScaleRatio = sh / (float)(dh);

rowBuf1 = new long[dw];
rowBuf2 = new long[dw];
horWeight = new long[dw];
horLeftIdx = new int[dw];
horRightIdx = new int[dw];


//col interpolation

//计算目标图像像素横向的左右邻居序号，和权重。
for(i = 0; i < dw; i++) {
    float pos = (i + 0.5f) * horScaleRatio;
    horLeftIdx[i] = (int)(IET_MAX(pos - 0.5f, 0));
    horRightIdx[i] = (int)(IET_MIN(pos + 0.5f, sw-1));
    horWeight[i] = (long) (fabs(pos - 0.5f - horLeftIdx[i]) * (1<<shift));
}

preVerUpIdx = -1;
preVerDownIdx = -1;
upLinePtr = rowBuf1;
downLinePtr = rowBuf2;
for(j = 0; j < dh; j++) {
    float pos = (j + 0.5f) * verScaleRatio;
    int verUpIdx = (int)(IET_MAX(pos - 0.5f, 0));
    int verDownIdx = (int)(IET_MIN(pos + 0.5f, sh-1));
    long verWeight = (long) (fabs(pos - 0.5f - verUpIdx) * (1<<shift));

    if(verUpIdx == preVerUpIdx && verDownIdx == preVerDownIdx) {
        ;
        //do nothing
    } else if(verUpIdx == preVerDownIdx) {
        IET_SWAP(upLinePtr, downLinePtr, tempPtr);
        _ieInterpImageBilinear8UC1_Ver3_RowFilter(sdata + sstep*verDownIdx, downLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
    } else {
        _ieInterpImageBilinear8UC1_Ver3_RowFilter(sdata + sstep*verUpIdx,   upLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
        _ieInterpImageBilinear8UC1_Ver3_RowFilter(sdata + sstep*verDownIdx, downLinePtr, dw, horLeftIdx, horRightIdx, horWeight, shift);
    }

    unsigned char* _ptr = ddata + dstep*j;
    for(i = 0; i < dw-4; i+=4) {
        *_ptr++ = (unsigned char) ( (((1<<shift) - verWeight)*upLinePtr[i] + verWeight*downLinePtr[i]) >> (2*shift) );
        *_ptr++ = (unsigned char) ( (((1<<shift) - verWeight)*upLinePtr[i+1] + verWeight*downLinePtr[i+1]) >> (2*shift) );
        *_ptr++ = (unsigned char) ( (((1<<shift) - verWeight)*upLinePtr[i+2] + verWeight*downLinePtr[i+2]) >> (2*shift) );
        *_ptr++ = (unsigned char) ( (((1<<shift) - verWeight)*upLinePtr[i+3] + verWeight*downLinePtr[i+3]) >> (2*shift) );
    }
    for(; i < dw; i++) {
        *_ptr++ = (unsigned char) ( (((1<<shift) - verWeight)*upLinePtr[i] + verWeight*downLinePtr[i]) >> (2*shift) );
    }
    preVerUpIdx = verUpIdx;
    preVerDownIdx = verDownIdx;
}
delete []rowBuf1;
delete []rowBuf2;
delete []horWeight;
delete []horLeftIdx;
delete []horRightIdx;
~~~

