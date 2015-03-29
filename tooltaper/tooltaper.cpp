#include <vector>
#include "common.h"
#include "tooltaper.h"
#define Pi 3.1415926

using namespace std;
using namespace cv;


void getpoint(Data data, 
	          const cv::Mat& ProjMat,
			  std::vector<cv::Point2f>& ImgCoordinate)
{
	//toolLen：棒料长度， toolD：钻头直径， cutEdgelen:切削刃长度
	//taperAgl:锥度角度， helixAgl:螺旋角， precision:精度步长
	double a;
	int n; 
	int i;
	int j;
	//计算过程中所需的参数，toolR:铣刀半径， lead:导程
	/*Calculate toolmid;*/
	double toolR = data.toolD/2;
	double lead = 2*Pi*toolR/tan(data.helixAgl);

	//计算基准线是选取的节点个数
	int numofpoint;
	a = data.cutEdgelen/data.precision;
	if (a-floor(a)<=0.5)
		numofpoint = floor(a);
	else
		numofpoint = ceil(a);

	n = numofpoint;

	//定义螺旋线节点的x,y,z构成的数组

	/*------------------*/
	cv::Mat CurveCoordinate = cv::Mat::zeros(3,1,CV_64F);
	cv::Mat tmp;
	double X,Y;
	double mtoolR, mc;
	for(i=0;i<n+1;i++)
	{
		/*------------------------*/
		CurveCoordinate.at<double>(0) = 0-(double)i/(double)n * data.cutEdgelen;
		mtoolR = -CurveCoordinate.at<double>(0)*tan(data.taperAgl*Pi/360);
		mc = -2*Pi*CurveCoordinate.at<double>(0) / lead;
		CurveCoordinate.at<double>(1) = (toolR+mtoolR)*sin(mc);
		CurveCoordinate.at<double>(2) = (toolR+mtoolR)*cos(mc);
		/*-----------------------*/
		tmp = ProjMat * CurveCoordinate;
		X = tmp.at<double>(0) / 
			 tmp.at<double>(2);
		Y = tmp.at<double>(1) / 
			 tmp.at<double>(2);
		if (X > 0 && X < 1&& 
			Y > 0 && Y < 1){
			ImgCoordinate.push_back(cv::Point2f(X,Y));
		}
	}
}