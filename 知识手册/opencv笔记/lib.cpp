 
 /*
 
 
 
 
 方式一
从仿射变换矩阵[ cos ⁡ α − s i n α T x s i n α cos ⁡ α T y ]
[cosαsinα−sinαcosαTxTy]
[cos⁡α−sinαTxsinαcos⁡αTy]
[ 
cosα
sinα
​
  
−sinα
cosα
​
  
Tx
Ty
​
 ]看，参数有3个：α , T x , T y \alpha,Tx,Tyα,Tx,Ty，如果有把它们都看成未知数，那么只要有三个独立的方程就可以解出仿射变换矩阵,对应实际的情况就是不共线的三个点。而opencv中提供了一个函数cv::getAffineTransform，只需要输入三个对应点对就可以得到仿射变换矩阵，然后用cv::warpAffine进行变换。这种方式主要是因为矩形的角点很容易计算，只需要计算三个点就可以得到变换矩阵。
方式二
直接构造仿射变换矩阵，角度已知，只需要求平移量，而平移量就是“最负”的点的坐标值，因为负数坐标的地方不显示，加上一点偏移使得所有坐标为负的点都变为正的。
————————————————
版权声明：本文为CSDN博主「1037号森林里一段干木头」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_40622955/article/details/119180886
 
 
 
 
 
 
 
 
 
 
 
 
 
 */
 
#include <iostream>
#include<algorithm>
#include "opencv.hpp"

int rotateImage(const cv::Mat &src, cv::Mat &dst, const double angle, const int mode)
{
	//mode = 0 ,Keep the original image size unchanged
	//mode = 1, Change the original image size to fit the rotated scale, padding with zero

	if (src.empty())
	{
		std::cout << "Damn, the input image is empty!\n";
		return -1;
	}

	if (mode == 0)
	{
		cv::Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
		cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
		cv::warpAffine(src, dst, rot, src.size());//the original size
	}
	else {

		double alpha = -angle * CV_PI / 180.0;//convert angle to radian format 

		cv::Point2f srcP[3];
		cv::Point2f dstP[3];
		srcP[0] = cv::Point2f(0, src.rows);
		srcP[1] = cv::Point2f(src.cols, 0);
		srcP[2] = cv::Point2f(src.cols, src.rows);
		
		//rotate the pixels
		for (int i=0;i<3;i++)
					dstP[i] = cv::Point2f(srcP[i].x*cos(alpha) - srcP[i].y*sin(alpha), srcP[i].y*cos(alpha) + srcP[i].x*sin(alpha));
		double minx, miny, maxx, maxy;
		minx = std::min(std::min(std::min(dstP[0].x, dstP[1].x), dstP[2].x),float(0.0));
		miny  = std::min(std::min(std::min(dstP[0].y, dstP[1].y), dstP[2].y),float(0.0));
		maxx = std::max(std::max(std::max(dstP[0].x, dstP[1].x), dstP[2].x),float(0.0));
		maxy = std::max(std::max(std::max(dstP[0].y, dstP[1].y), dstP[2].y),float(0.0));

		int w = maxx - minx;
		int h = maxy - miny;

		//translation
		for (int i = 0; i < 3; i++)
		{
			if (minx < 0)
				dstP[i].x -= minx;
			if (miny < 0)
				dstP[i].y -= miny;
		}

		cv::Mat warpMat = cv::getAffineTransform(srcP, dstP);
		cv::warpAffine(src, dst, warpMat, cv::Size(w, h));//extend size

	}//end else

	return 0;
}

int rotateImage2(const cv::Mat &src, cv::Mat &dst, const double angle, const int mode)
{
	//mode = 0 ,Keep the original image size unchanged
	//mode = 1, Change the original image size to fit the rotated scale, padding with zero

	if (src.empty())
	{
		std::cout << "Damn, the input image is empty!\n";
		return -1;
	}

	if (mode == 0)
	{
		cv::Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
		cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
		cv::warpAffine(src, dst, rot, src.size());//the original size
	}
	else {

		double alpha = -angle * CV_PI / 180.0;//convert angle to radian format 

		cv::Point2f srcP[3];
		cv::Point2f dstP[3];
		srcP[0] = cv::Point2f(0, src.rows);
		srcP[1] = cv::Point2f(src.cols, 0);
		srcP[2] = cv::Point2f(src.cols, src.rows);

		//rotate the pixels
		for (int i = 0; i < 3; i++)
			dstP[i] = cv::Point2f(srcP[i].x*cos(alpha) - srcP[i].y*sin(alpha), srcP[i].y*cos(alpha) + srcP[i].x*sin(alpha));
		double minx, miny, maxx, maxy;
		minx = std::min(std::min(std::min(dstP[0].x, dstP[1].x), dstP[2].x), float(0.0));
		miny = std::min(std::min(std::min(dstP[0].y, dstP[1].y), dstP[2].y), float(0.0));
		maxx = std::max(std::max(std::max(dstP[0].x, dstP[1].x), dstP[2].x), float(0.0));
		maxy = std::max(std::max(std::max(dstP[0].y, dstP[1].y), dstP[2].y), float(0.0));

		int w = maxx - minx;
		int h = maxy - miny;

		cv::Mat warpMat =cv::Mat::zeros(cv::Size(3,2),CV_64F);//rows=2,cols=3
		
		std::cout << warpMat.type() << std::endl;
		std::cout << warpMat.size()<<std::endl;
		warpMat.at<double>(0, 0) = cos(alpha);
		warpMat.at<double>(0, 1) = 0- sin(alpha);
		warpMat.at<double>(1, 0) = sin(alpha);
		warpMat.at<double>(1, 1) = cos(alpha);
		warpMat.at<double>(0, 2) =0- minx;
		warpMat.at<double>(1, 2) =0-miny;
		//std::cout << warpMat;
		cv::warpAffine(src, dst, warpMat, cv::Size(w, h));//extend size

	}//end else

	return 0;
}

int main()
{
    std::cout << "Hello World!\n";
	std::string filePath = "K:\\imageData\\lena\\images.png";
	cv::Mat src = cv::imread(filePath);
	cv::Mat dst1,dst2;
	for (double i = -360; i <= 360; i++)
	{
		int flg1 = rotateImage(src, dst1, i, 0);
		int flg2 = rotateImage(src, dst2, i, 1);
		if (flg1 == -1 || flg2 == -1) continue;
		cv::imshow("src", src);
		cv::imshow("dst1", dst1);
		cv::imshow("dst2", dst2);
		cv::waitKey(5);
	}
	cv::waitKey(0);
	cv::destroyAllWindows();

}



