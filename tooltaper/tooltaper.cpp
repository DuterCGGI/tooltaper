#include <vector>
#include "common.h"
#include "tooltaper.h"
#define Pi 3.1415926

using namespace std;
using namespace cv;


void getpoint(Data data, double* arrar, Mat imav,const char* filename)
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

		
											
	vector<double> xx(n+1);					 //n+1个节点的x坐标
	vector<double> mtoolR(n+1);				 //n+1个节点处对应的铣刀半径，对于锥形铣刀，半径改变
	vector<double> mc(n+1);				     //n+1个节点的旋转角度				 
	vector<double> zz(n+1);					 //n+1个节点的y坐标  
	vector<double> yy(n+1);					 //n+1个节点的z坐标  
	

	for(i=0;i<n+1;i++)
	{

		xx[i] = 0-(double)i/(double)n * data.cutEdgelen;
		mtoolR[i] = -xx[i]*tan(data.taperAgl*Pi/360);
		mc[i] = -2*Pi*xx[i] / lead;
		zz[i] = (toolR+mtoolR[i])*cos(mc[i]);
		yy[i] = (toolR+mtoolR[i])*sin(mc[i]);
	}

	/* 将基准线投影到图像上*/
	//point转化为vector
	vector<cv::Point3d> vec(n+1);
	for(i=0;i<n+1;i++)
		vec[i] = cv::Point3d(xx[i],yy[i],zz[i]);


	//生成一个投影矩阵，将三维点投影到二维平面上
	
	cv::Mat projectB = cv::Mat(3,3, CV_64F,arrar);

	//投影矩阵与三维点阵乘积
	Mat PointMat = Mat(vec).reshape(1).t();
	cv::Mat result;
	result = projectB * PointMat;

	//生成图像上的二维点阵，
	Mat G = result.row(0).clone();
	Mat H = result.row(1).clone();
	Mat J = result.row(2).clone();

	cv::vconcat(G.mul(1/J),H.mul(1/J),imav);
	imav = imav.t();

	//去掉负数项,得到最终投影后的有效点
	Mat delimav;
	int t = n+1,count =0;
	for(i=0;i<t;i++)
		if(imav.at<double>(i,0) < 0 || imav.at<double>(i,1) < 0
			||imav.at<double>(i,0) >1 || imav.at<double>(i,1) >1)
		{
			Mat front = imav(Range(0,i),Range::all());
			Mat back = imav(Range(i+1,t),Range::all());
			vconcat(front,back,delimav);

			imav.release();
			imav = delimav.clone();
			//--------------------//
			i--;
			t--;
		}
	// imav为投影后的坐标
	
	ofstream fout(filename, ios_base::out | ios_base::trunc);

	fout << imav << endl;

	fout.close();
}