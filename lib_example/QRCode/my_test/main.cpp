
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
using namespace std;
using namespace cv;
int main()
{	//读取当前工程内图片
	Mat img = imread("/home/nihao/nihao.png");
	QRCodeDetector QRdetecter;
	std::vector<cv::Point> list;
	Mat res;
	std::string str=QRdetecter.detectAndDecode(img, list, res);
	printf("\r\n %s \r\n",str.data());
 
	for (int i = 0; i < list.size(); i++)
	{
	    if(i==3)
		line(img, list[i], list[0], Scalar(255, 255, 0), 2);
		else
		line(img, list[i], list[i + 1], Scalar(255, 255, 0), 2);
	}
	putText(img,str, Point(10, 400),FONT_HERSHEY_COMPLEX,2,Scalar(0, 255, 255), 2, 8);
	imshow("QR二维码定位", img);
	waitKey(0);
	return 0;

}

