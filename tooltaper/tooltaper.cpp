#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\core\mat.hpp>
#include "tooltaper.h"
#define Pi 3.1415926

using namespace std;
using namespace cv;


void getpoint(double toolLen,double toolD,double cutEdgelen,
	double taperAgl,double helixAgl,double precision)
{
	//toolLen：棒料长度， toolD：钻头直径， cutEdgelen:切削刃长度
	//taperAgl:锥度角度， helixAgl:螺旋角， precision:精度步长
	double a;
	int n; 
	int i;
	int j;

	//计算过程中所需的参数，toolR:铣刀半径， lead:导程
	Calculate toolmid;
	toolmid.toolR = toolD/2;
	toolmid.lead = 2*Pi*toolmid.toolR/tan(helixAgl);

	//计算基准线是选取的节点个数
	a = cutEdgelen/precision;
	if (a-floor(a)<=0.5)
		toolmid.numofpoint = floor(a);
	else
		toolmid.numofpoint = ceil(a);

	n = toolmid.numofpoint;

	//定义螺旋线节点的x,y,z构成的数组
	
	double *mtoolR = new double [n+1];	   //n+1个节点处对应的铣刀半径，对于锥形铣刀，半径改变
	double *mc = new double [n+1];         //n+1个节点的旋转角度
	double *xx = new double [n+1];         //n+1个节点的x坐标
	double *yy = new double [n+1];         //n+1个节点的y坐标
	double *zz = new double [n+1];		   //n+1个节点的z坐标
	for(i=0;i<n+1;i++)
	{

		xx[i] = 0-(double)i/(double)n * cutEdgelen;
		mtoolR[i] = -xx[i]*tan(taperAgl*Pi/360);
		mc[i] = -2*Pi*xx[i] / toolmid.lead;
		zz[i] = (toolmid.toolR+mtoolR[i])*cos(mc[i]);
		yy[i] = (toolmid.toolR+mtoolR[i])*sin(mc[i]);
	}

	// 生成铣刀刃线的n+1个节点
	Result point, *Ppoint;
	for (i = 0;i<n+1;i++)
	{
		point.x = xx[i];
		point.y = yy[i];
		point.z = zz[i];
		Ppoint = new Result;
		Ppoint->x = xx[i+1];
		Ppoint->y = yy[i+1];
		Ppoint->z = zz[i+1];
		point.next =Ppoint;

	}

	/* 将基准线投影到图像上*/
	//point转化为vector
	vector<cv::Point3d> vec(n+1);
	for(i=0;i<n+1;i++)
		vec[i] = cv::Point3d(xx[i],yy[i],zz[i]);

	//生成一个投影矩阵，将三维点投影到二维平面上
	double aa[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
	cv::Mat projectB = Mat(3,3, CV_64F,aa);

	//投影矩阵与三维点阵乘积
	Mat PointMat = Mat(vec).reshape(1).t();
	cv::Mat result;
	result = projectB * PointMat;
	
	//生成图像上的二维点阵，
	Mat G = result.row(0).clone();
	Mat H = result.row(1).clone();
	Mat J = result.row(2).clone();
	Mat imav;
	cv::vconcat(G.mul(1/J),H.mul(1/J),imav);
	imav = imav.t();

	//去掉负数项
	Mat delimav;
	int t = n+1,count =0;
	for(i=0;i<t;i++)
		if(imav.at<double>(i,0) < 0 | imav.at<double>(i,1) < 0
			|imav.at<double>(i,0) >1 | imav.at<double>(i,1) >1)
		{
			Mat front = imav(Range(0,i),Range::all());
			Mat back = imav(Range(i+1,t),Range::all());
			vconcat(front,back,delimav);
			
			imav.release();
			imav = delimav.clone();
			
			i--;
			t--;
		}
	cout << imav << endl;

}