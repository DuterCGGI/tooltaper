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
	double toolR = data.toolD/2;
	double lead = 2*Pi*toolR/tan(data.helixAgl);      //toolR:铣刀半径， lead:导程
	int NumofPoint;									  //NumofPoint:节点个数
	NumofPoint = floor(data.cutEdgelen/data.precision);
	/*------------------*/
	cv::Mat CurveCoordinate = cv::Mat::zeros(3,1,CV_64F);
	cv::Mat tmp;
	double X,Y;
	double mtoolR, mc;
	for(int i=0;i<NumofPoint+1;i++)
	{
		/*------------------------*/
		CurveCoordinate.at<double>(0) = -(double)i * data.precision;
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