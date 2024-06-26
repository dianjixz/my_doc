* void cv::cvtColor(
        cv::InputArray src, // 输入序列
        cv::OutputArray dst, // 输出序列
        int code, // 颜色映射码
        int dstCn = 0 // 输出的通道数 (0='automatic')
       );

  参数解释： 
  . InputArray src: 输入图像即要进行颜色空间变换的原图像，可以是Mat类 
  . OutputArray dst: 输出图像即进行颜色空间变换后存储图像，也可以Mat类 
  . int code: 转换的代码或标识，即在此确定将什么制式的图片转换成什么制式的图片，
  . int dstCn = 0: 目标图像通道数，如果取值为0，则由src和code决定![20180717150512898](../image/20180717150512898.png)

~~~ c++
#include <opencv2/opencv.hpp>
#include <iostream>
 
using namespace cv;
using namespace std;
 
int main(int argc, char** argv) 
	{
	 Mat src,dst,dst1;
	 src = imread("image5.jpg");
 
//判断图片是否载入成功
	if (src.empty()) 
	{
		printf("图片加载失败\n");
		system("pause");
	}
	imshow("原图",src);
 
	cvtColor(src,dst,CV_BGR2GRAY);//转换方式1，这种是转换为灰度图，经常使用，需要记住
	cvtColor(src,dst1,COLOR_BGR2Lab);//转换方式2
 
	imshow("CV_BGR2GRAY转换后",dst);
	imshow("COLOR_BGR2Lab转换后",dst1);
 
	waitKey(0);
	return 0;
}
~~~

~~~ c++
Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
erode(src_gray, src_gau, element); //腐蚀
dilate(src_gau, src_di, element); //膨胀
~~~

* retval, dst = cv.threshold( src, thresh, maxval, type[, dst] ) 
threshold函数作用：

去掉噪，例如过滤很小或很大像素值的图像点。
参数说明：

src：原图像。
dst：结果图像。
thresh：当前阈值。
maxVal：最大阈值，一般为255.

thresholdType:阈值类型，主要有下面几种：  
~~~ c++
enum ThresholdTypes {
    THRESH_BINARY     = 0,
    THRESH_BINARY_INV = 1,
    THRESH_TRUNC      = 2,
    THRESH_TOZERO     = 3,
    THRESH_TOZERO_INV = 4,
    THRESH_MASK       = 7,
    THRESH_OTSU       = 8,
    THRESH_TRIANGLE   = 16
};
~~~
 返回值:
 retval： 与参数thresh一致
dst： 结果图像 

|编号|	阈值类型枚举	|注意|
| --- | --- | --- |
|1|	THRESH_BINARY| |
|2	|THRESH_BINARY_INV| |
|3|	THRESH_TRUNC| |
|4|	THRESH_TOZERO| |
|5|	THRESH_TOZERO_IN|
|6|	THRESH_MASK|	不支持|
|7|	THRESH_OTSU	|不支持32位|
|8|	THRESH_TRIANGLE|	不支持32位|

* 绘制点和圆
~~~ c++
void cvCircle( CvArr* img, CvPoint center, int radius, CvScalar color,
               int thickness=1, int line_type=8, int shift=0 );
~~~


-    img：图像。
-    center：圆心坐标。
-    radius：圆形的半径。
-    color：线条的颜色。
-    thickness：如果是正数，表示组成圆的线条的粗细程度。否则，表示圆是否被填充。
-    line_type：线条的类型。见 cvLine 的描述
-    shift：圆心坐标点和半径值的小数点位数。
~~~ c++
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//画圆画点都是使用circle()函数来画，点就是圆，我们平常所说的圆只不过是半径大一点而已。
int main()
{
	Mat img = imread("lol16.jpg");	

	//画空心点
	Point p(20, 20);//初始化点坐标为(20,20)
	circle(img, p, 2, Scalar(0, 255, 0)); //第三个参数表示点的半径，第四个参数选择颜色。这样子我们就画出了绿色的空心点

	//这种初始化点的方式也可以
	Point p2;
	p2.x = 100;
	p2.y = 100;
	//画实心点
	circle(img, p2, 3,Scalar(255,0,0),-1); //第五个参数我设为-1，表明这是个实点。

	//画空心圆
	Point p3(300, 300);
	circle(img,p3,100,Scalar(0,0,255),3);//第五个参数我们调高点，让线更粗

	//画实心圆
	Point p4;
	p4.x = 600;
	p4.y = 600;
	circle(img, p4, 100, Scalar(120, 120, 120), - 1);

	imshow("画点画圆", img);

	waitKey();
	return 0;
}
~~~

* 绘制椭圆


 -   img：图像。
 -   center：椭圆圆心坐标。
 -   axes：轴的长度。
 -   angle：偏转的角度。
 -   start_angle：圆弧起始角的角度。
 -   end_angle：圆弧终结角的角度。
 -   color：线条的颜色。
 -   thickness：线条的粗细程度。
 -   line_type：线条的类型,见CVLINE的描述。
 -   shift：圆心坐标点和数轴的精度。


~~~ c++
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("lol16.jpg");	
	int thickness = 3;
	int lineType = 8;
	double angle = 30;  //椭圆旋转角度
	//第三个参数Size中的两个参数分别是横轴长、纵轴长。
	//同理，thickness若是小于0，表示实心
	ellipse(img,Point(100, 100),Size(90, 60),angle,0,360,Scalar(255, 255, 0),thickness,lineType);

	imshow("画椭圆", img);

	waitKey();
	return 0;
}

~~~

* 绘制矩形


 -   img：图像。
 -   rec：表征矩形的位置和长宽。
 -   color：线条颜色 (RGB) 或亮度（灰度图像 ）(grayscale image）。
 -   thickness：组成矩形的线条的粗细程度。取负值时（如CV_FILLED）函数绘制填充了色彩的矩形。
 -   line_type：线条的类型。见cvLine的描述
 -   shift：坐标点的小数点位数。

~~~ c++
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("lol16.jpg");	
	Rect r(250, 250, 120, 200);
	rectangle(img, r, Scalar(0, 255, 255), 3);

	imshow("画矩形", img);

	waitKey();
	return 0;
}
~~~

~~~ c++
void rectangle(InputOutputArray img, Point pt1, Point pt2,
                          const Scalar& color, int thickness = 1,
                          int lineType = LINE_8, int shift = 0);
void rectangle(CV_IN_OUT Mat& img, Rect rec,
                          const Scalar& color, int thickness = 1,
                          int lineType = LINE_8, int shift = 0);

rectangle(matImage,Point(20,200),Point(200,300),Scalar(255,0,0),1,1,0);  
//Rect(int a,int b,int c,int d)a,b为矩形的左上角坐标,c,d为矩形的长和宽  
rectangle(matImage,Rect(100,300,20,200),Scalar(0,0,255),1,1,0);  
~~~

 * 绘制直线


 -   img：图像.
 -   pt1：线条起点.
 -   pt2：线条终点.
 -   color：线条颜色.
 -   thickness：线条宽度.
 -   lineType：线型

~~~ c++
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("lol16.jpg");	
	Point p1(100, 100);
	Point p2(758, 50);
	line(img, p1, p2, Scalar(33, 33, 133), 2);

	//画第二条线
	line(img, Point(300, 300), Point(758, 300), Scalar(89, 90, 90), 3);

	imshow("画矩形", img);

	waitKey();
	return 0;
}

~~~

~~~ c++
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main()
{
	Mat img = Mat::zeros(Size(800,600), CV_8UC3);
	img.setTo(255);
	Point p1(100, 100);
	Point p2(758, 50);
	line(img, p1, p2, Scalar(0, 0, 255), 2);
	line(img, Point(300, 300), Point(758, 400), Scalar(0, 255, 255), 3);

	Point p(20, 20);//初始化点坐标为(20,20)
	circle(img, p, 2, Scalar(0, 255, 0),-1);

	Point p4;
	p4.x = 600;
	p4.y = 600;
	circle(img, p4, 100, Scalar(120, 120, 120), -1);

	int thickness = 3;
	int lineType = 8;
	double angle = 30;  //椭圆旋转角度
	ellipse(img, Point(100, 100), Size(90, 60), angle, 0, 360, Scalar(255, 255, 0), thickness, lineType);


	Rect r(250, 250, 120, 200);
	rectangle(img, r, Scalar(0, 255, 255), 3);
	imshow("大杂烩", img);

	waitKey();
	return 0;
}

~~~





* 最小二乘法拟合

~~~ c++
#include<iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


int main()
{
    vector<Point>points;
    //(27 39) (8 5) (8 9) (16 22) (44 71) (35 44) (43 57) (19 24) (27 39) (37 52)

    points.push_back(Point(27, 39));
    points.push_back(Point(8, 5));
    points.push_back(Point(8, 9));
    points.push_back(Point(16, 22));
    points.push_back(Point(44, 71));
    points.push_back(Point(35, 44));
    points.push_back(Point(43, 57));
    points.push_back(Point(19, 24));
    points.push_back(Point(27, 39));
    points.push_back(Point(37, 52));

    Mat src = Mat::zeros(400, 400, CV_8UC3);

    for (int i = 0;i < points.size();i++)
    {
        //在原图上画出点
        circle(src, points[i], 3, Scalar(0, 0, 255), 1, 8);
    }

    //构建A矩阵
    int N = 2;
    Mat A = Mat::zeros(N, N, CV_64FC1);

    for (int row = 0;row < A.rows;row++)
    {
        for (int col = 0; col < A.cols;col++)
        {
            for (int k = 0;k < points.size();k++)
            {
                A.at<double>(row, col) = A.at<double>(row, col) + pow(points[k].x, row + col);
            }
        }
    }
    //构建B矩阵
    Mat B = Mat::zeros(N, 1, CV_64FC1);
    for (int row = 0;row < B.rows;row++)
    {

        for (int k = 0;k < points.size();k++)
        {
            B.at<double>(row, 0) = B.at<double>(row, 0) + pow(points[k].x, row)*points[k].y;
        }
    }
    //A*X=B
    Mat X;
    //cout << A << endl << B << endl;
    solve(A, B, X,DECOMP_LU);

    cout << X << endl;
    vector<Point>lines;
    for (int x = 0;x < src.size().width;x++)
    {                // y = b + ax;
        double y = X.at<double>(0, 0) + X.at<double>(1, 0)*x;
        printf("(%d,%lf)\n", x, y);
        lines.push_back(Point(x, y));
    }
    polylines(src, lines, false, Scalar(255, 0, 0), 1, 8);
    imshow("src", src);

    //imshow("src", A);
    waitKey(0);
    return 0;
~~~

* 当得到对象轮廓后，可用boundingRect()得到包覆此轮廓的最小正矩形，minAreaRect()得到包覆轮廓的最小斜矩形。

- Rect boundingRect(InputArray points)

points：输入信息，可以为包含点的容器(vector)或是Mat。
 返回包覆输入信息的最小正矩形。
~~~ c++
#include<opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
int main(){
    Mat src = imread("C:/Users/齐明洋/Desktop/7.jpg");
    imshow("src", src);

    Mat gray, bin_img;
    cvtColor(src, gray, COLOR_BGR2GRAY);   //将原图转换为灰度图
    imshow("gray", gray);

    //二值化
    threshold(gray, bin_img, 150, 255, THRESH_BINARY_INV);
    imshow("bin_img", bin_img);
    
    //寻找最外围轮廓
    vector<vector<Point> >contours;
    findContours(bin_img, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    //绘制边界矩阵
    RNG rngs = { 12345 };
    Mat dst = Mat::zeros(src.size(), src.type());
    for (int i = 0; i < contours.size(); i++) {
        Scalar colors = Scalar(rngs.uniform(0, 255), rngs.uniform(0, 255), rngs.uniform(0, 255));
        drawContours(dst, contours, i, colors, 1);
        Rect rects = boundingRect(contours[i]);
        rectangle(dst, rects, colors, 2);
    }
    imshow("dst", dst);

    waitKey(0);
}

~~~



- RotatedRect minAreaRect(InputArray points)

  points：输入信息，可以为包含点的容器(vector)或是Mat。

  返回包覆输入信息的最小斜矩形。

  
  ~~~ c++
  #include<opencv.hpp>
  #include<iostream>
  #include<vector>
  using namespace cv;
  using namespace std;
  int main() {
    Mat src = imread("C:/Users/齐明洋/Desktop/7.jpg");
    imshow("src", src);
  
    Mat gray, bin_img;
    cvtColor(src, gray, COLOR_BGR2GRAY);   //将原图转换为灰度图
    imshow("gray", gray);
  
    //二值化
    threshold(gray, bin_img, 150, 255, THRESH_BINARY_INV);
    imshow("bin_img", bin_img);
  
    //寻找最外围轮廓
    vector<vector<Point> >contours;
    findContours(bin_img, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
  
    //绘制最小边界矩阵
    RNG rngs = { 12345 };
    Mat dst = Mat::zeros(src.size(), src.type());
    Point2f pts[4];
    for (int i = 0; i < contours.size(); i++) {
        Scalar colors = Scalar(rngs.uniform(0, 255), rngs.uniform(0, 255), rngs.uniform(0, 255));
        drawContours(dst, contours, i, colors, 1);
  
        RotatedRect rects = minAreaRect(contours[i]);
        rects.points(pts);//确定旋转矩阵的四个顶点
        for (int i = 0; i < 4; i++) {
            line(dst, pts[i], pts[(i + 1) % 4], colors, 2);
        }
    }
    imshow("dst", dst);
  
    waitKey(0);
  }
  ~~~
  
  C++: void calcHist(const Mat* images, int nimages, const int* channels, InputArray mask, OutputArray hist, int dims, const int* histSize, const float** ranges, bool uniform=true, bool accumulate=false )

参数详解：

onst Mat* images：输入图像

 int nimages：输入图像的个数

const int* channels：需要统计直方图的第几通道

InputArray mask：掩膜，，计算掩膜内的直方图  ...Mat()

OutputArray hist:输出的直方图数组

int dims：需要统计直方图通道的个数

const int* histSize：指的是直方图分成多少个区间，就是 bin的个数

const float** ranges： 统计像素值得区间

bool uniform=true::是否对得到的直方图数组进行归一化处理

bool accumulate=false：在多个图像时，是否累计计算像素值得个数
————————————————
版权声明：本文为CSDN博主「水亦心」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/shuiyixin/article/details/80032167


    我们需要把下面这个图像的直方图计算并画在画布上。一共有如下几个步骤：
    
        1.创建一些矩阵；
    
        2.加载原图像；
    
        3.使用OpenCV函数 split() 将图像分割成3个单通道图像；
    
        4.设定像素取值范围，我们知道像素值的范围是 [0,255]；
    
        5.使用OpenCV函数calcHist() 分别计算三个通道的直方图；
    
        6.创建显示直方图的画布并使用 normalize() 函数归一化直方图；
    
        7.最后显示直方图并等待用户退出程序；
~~~ c++
#define INPUT_TITLE "input image"
#define OUTPUT_TITLE "直方图计算"
 
#include<iostream>
#include<opencv2\opencv.hpp>
 
using namespace std;
using namespace cv;
 
int main() {
	Mat src, dst;
        //加载图像
        src = imread("D:/equalizeHist.jpg");
	if (!src.data)
	{
		cout << "ERROR : could not load image.";
		return -1;
	}
	namedWindow(INPUT_TITLE, CV_WINDOW_AUTOSIZE);
	namedWindow(OUTPUT_TITLE, CV_WINDOW_AUTOSIZE);
 
	imshow(INPUT_TITLE, src);
 
	//分通道显示
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
 
	//设定像素取值范围
	int histSize = 256;
	float range[] = { 0,256 };
	const float *histRanges = { range };
 
        //三个通道分别计算直方图
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRanges, true, false);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRanges, true, false);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRanges, true, false);
 
	//创建直方图画布并归一化处理
        int hist_h = 400;
	int hist_w = 512;
	int bin_w = hist_w / histSize;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
 
	//render histogram chart  在直方图画布上画出直方图
	for (int i = 0; i < histSize; i++)
	{
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))), 
			Point((i)*bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);
	}
 
        
        imshow(OUTPUT_TITLE, histImage);
 
	waitKey(0);
	return 0;
}

~~~
- opencv对二值图像进行颜色反色操作
> bitwise_not(singleROI,singleROI);//颜色反转

- opencv形态学运算
开运算（Opening Operation），其实就是先腐蚀后膨胀的操作。
作用：
去除噪声，消除小物体
在纤细点处分离物体
平滑较大物体的边界的同时并不明显改变其面积
闭运算：操作为先膨胀后腐蚀
![](https://img-blog.csdnimg.cn/2018110717234546.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2RpZWp1ODMzMA==,size_16,color_FFFFFF,t_70)


作用：

排除小型空洞（指黑色区域）
平滑物体轮廓
弥合（连接）窄的间断点，沟壑
填补轮廓线断裂
![](https://img-blog.csdnimg.cn/20181107173450544.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2RpZWp1ODMzMA==,size_16,color_FFFFFF,t_70)


~~~ C++
void morphologyEx( InputArray src, OutputArray dst,
                                int op, InputArray kernel,
                                Point anchor=Point(-1,-1), int iterations=1,
                                int borderType=BORDER_CONSTANT,
                                const Scalar& borderValue=morphologyDefaultBorderValue() );
~~~
一般来说，看前四个参数就行了，后面的就用默认值

第一个参数 输入
第二个参数 输出
第三个参数 操作类型
MORTH_OPEN                函数做开运算
MORTH_CLOSE              函数做闭运算
MORTH_GRADIENT       函数做形态学梯度运算
MORTH_TOPHAT            函数做顶帽运算
MORTH_BLACKHAT       函数做黑帽运算
MORTH_DILATE              函数做膨胀运算
MORTH_ERODE             函数做腐蚀运算

第四个参数  内核类型    用getStructuringElement函数得到
~~~ c++
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
void main()
{
	Mat srcImg = imread("F:\\opencv_re_learn\\test.jpg");
	if (!srcImg.data){
		cout << "failed to read" << endl;
		system("pause");
		return;
	}
	imshow("src", srcImg);
	Mat srcGray;
	cvtColor(srcImg, srcGray, CV_BGR2GRAY);
	Mat thresh;
	threshold(srcGray, thresh, 230, 255, CV_THRESH_BINARY_INV);
	imshow("thresh", thresh);
	//自定义核
	Mat element = getStructuringElement(MORPH_RECT,
		Size(5, 5));
	//开运算
	Mat open_result;
	morphologyEx(thresh, open_result, MORPH_OPEN,element);
	imshow("开运算", open_result);
	waitKey(0);
}
~~~
~~~ c++
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
void main()
{
	Mat srcImg = imread("F:\\opencv_re_learn\\hand.jpg");
	if (!srcImg.data){
		cout << "failed to read" << endl;
		system("pause");
		return;
	}
	imshow("src", srcImg);
	Mat srcGray;
	cvtColor(srcImg, srcGray, CV_BGR2GRAY);
	Mat thresh;
	threshold(srcGray, thresh, 200, 255, CV_THRESH_BINARY_INV|
		CV_THRESH_OTSU);
	imshow("thresh", thresh);
	//自定义核
	Mat element = getStructuringElement(MORPH_RECT,
		Size(5, 5));
	//闭运算
	Mat close_result;
	morphologyEx(thresh, close_result, MORPH_CLOSE,element);
	imshow("闭运算", close_result);
	waitKey(0);
}
~~~



OpenCV + CPP 系列（七）通道的拆分|合并|混合

https://blog.csdn.net/wsp_1138886114/article/details/118605939











- python3 opencv 图像二值化笔记（cv2.adaptiveThreshold）
https://blog.csdn.net/laoyezha/article/details/106445437

opencv如何在图片上写汉字

netdisk.tongxinmao.com/%E9%80%9A%E4%BF%A1%E7%8C%ABMQTT%E8%B0%83%E8%AF%95%E5%AE%A2%E6%88%B7%E7%AB%AF.zip?e=1651729741&token=e2H295TKnR7Y_pBeAh3HqrpDDBFO7-7le0sbxF1a:gZzO7D7WOK75VT1T3Il13jyt0k0=



利用OpenCV DNN模块进行深度学习：一个最好使用指导书
https://blog.csdn.net/zhuoqingjoking97298/article/details/122763074









