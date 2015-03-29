#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
#include <opencv2\opencv.hpp>

using namespace std;


struct Data
{
	double toolLen;       //棒料长度
	double toolD;         //铣刀直径
	double cutEdgelen;    //切削刃长度
	double taperAgl;      //锥度角度
	double helixAgl;      //螺旋角
	double precision;     //精度
};

void getpoint(Data data,
	          const cv::Mat& ProjMat,
			  std::vector<cv::Point2f>& ImgCoordinate);