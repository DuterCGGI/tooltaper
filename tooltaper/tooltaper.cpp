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
	//toolLen�����ϳ��ȣ� toolD����ͷֱ���� cutEdgelen:�����г���
	//taperAgl:׶�ȽǶȣ� helixAgl:�����ǣ� precision:���Ȳ���
	double a;
	int n; 
	int i;
	int j;

	//�������������Ĳ�����toolR:ϳ���뾶�� lead:����
	Calculate toolmid;
	toolmid.toolR = toolD/2;
	toolmid.lead = 2*Pi*toolmid.toolR/tan(helixAgl);

	//�����׼����ѡȡ�Ľڵ����
	a = cutEdgelen/precision;
	if (a-floor(a)<=0.5)
		toolmid.numofpoint = floor(a);
	else
		toolmid.numofpoint = ceil(a);

	n = toolmid.numofpoint;

	//���������߽ڵ��x,y,z���ɵ�����
	
	double *mtoolR = new double [n+1];	   //n+1���ڵ㴦��Ӧ��ϳ���뾶������׶��ϳ�����뾶�ı�
	double *mc = new double [n+1];         //n+1���ڵ����ת�Ƕ�
	double *xx = new double [n+1];         //n+1���ڵ��x����
	double *yy = new double [n+1];         //n+1���ڵ��y����
	double *zz = new double [n+1];		   //n+1���ڵ��z����
	for(i=0;i<n+1;i++)
	{

		xx[i] = 0-(double)i/(double)n * cutEdgelen;
		mtoolR[i] = -xx[i]*tan(taperAgl*Pi/360);
		mc[i] = -2*Pi*xx[i] / toolmid.lead;
		zz[i] = (toolmid.toolR+mtoolR[i])*cos(mc[i]);
		yy[i] = (toolmid.toolR+mtoolR[i])*sin(mc[i]);
	}

	// ����ϳ�����ߵ�n+1���ڵ�
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

	/* ����׼��ͶӰ��ͼ����*/
	//pointת��Ϊvector
	vector<cv::Point3d> vec(n+1);
	for(i=0;i<n+1;i++)
		vec[i] = cv::Point3d(xx[i],yy[i],zz[i]);

	//����һ��ͶӰ���󣬽���ά��ͶӰ����άƽ����
	double aa[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
	cv::Mat projectB = Mat(3,3, CV_64F,aa);

	//ͶӰ��������ά����˻�
	Mat PointMat = Mat(vec).reshape(1).t();
	cv::Mat result;
	result = projectB * PointMat;
	
	//����ͼ���ϵĶ�ά����
	Mat G = result.row(0).clone();
	Mat H = result.row(1).clone();
	Mat J = result.row(2).clone();
	Mat imav;
	cv::vconcat(G.mul(1/J),H.mul(1/J),imav);
	imav = imav.t();

	//ȥ��������
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