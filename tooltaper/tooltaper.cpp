#include <vector>
#include "common.h"
#include "tooltaper.h"
#define Pi 3.1415926

using namespace std;
using namespace cv;


void getpoint(Data data, double* arrar, Mat imav,const char* filename)
{
	//toolLen�����ϳ��ȣ� toolD����ͷֱ���� cutEdgelen:�����г���
	//taperAgl:׶�ȽǶȣ� helixAgl:�����ǣ� precision:���Ȳ���
	
	double a;
	int n; 
	int i;
	int j;

	//�������������Ĳ�����toolR:ϳ���뾶�� lead:����
	/*Calculate toolmid;*/
	double toolR = data.toolD/2;
	double lead = 2*Pi*toolR/tan(data.helixAgl);

	//�����׼����ѡȡ�Ľڵ����
	int numofpoint;
	a = data.cutEdgelen/data.precision;
	if (a-floor(a)<=0.5)
		numofpoint = floor(a);
	else
		numofpoint = ceil(a);

	n = numofpoint;

	//���������߽ڵ��x,y,z���ɵ�����

		
											
	vector<double> xx(n+1);					 //n+1���ڵ��x����
	vector<double> mtoolR(n+1);				 //n+1���ڵ㴦��Ӧ��ϳ���뾶������׶��ϳ�����뾶�ı�
	vector<double> mc(n+1);				     //n+1���ڵ����ת�Ƕ�				 
	vector<double> zz(n+1);					 //n+1���ڵ��y����  
	vector<double> yy(n+1);					 //n+1���ڵ��z����  
	

	for(i=0;i<n+1;i++)
	{

		xx[i] = 0-(double)i/(double)n * data.cutEdgelen;
		mtoolR[i] = -xx[i]*tan(data.taperAgl*Pi/360);
		mc[i] = -2*Pi*xx[i] / lead;
		zz[i] = (toolR+mtoolR[i])*cos(mc[i]);
		yy[i] = (toolR+mtoolR[i])*sin(mc[i]);
	}

	/* ����׼��ͶӰ��ͼ����*/
	//pointת��Ϊvector
	vector<cv::Point3d> vec(n+1);
	for(i=0;i<n+1;i++)
		vec[i] = cv::Point3d(xx[i],yy[i],zz[i]);


	//����һ��ͶӰ���󣬽���ά��ͶӰ����άƽ����
	
	cv::Mat projectB = cv::Mat(3,3, CV_64F,arrar);

	//ͶӰ��������ά����˻�
	Mat PointMat = Mat(vec).reshape(1).t();
	cv::Mat result;
	result = projectB * PointMat;

	//����ͼ���ϵĶ�ά����
	Mat G = result.row(0).clone();
	Mat H = result.row(1).clone();
	Mat J = result.row(2).clone();

	cv::vconcat(G.mul(1/J),H.mul(1/J),imav);
	imav = imav.t();

	//ȥ��������,�õ�����ͶӰ�����Ч��
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
	// imavΪͶӰ�������
	
	ofstream fout(filename, ios_base::out | ios_base::trunc);

	fout << imav << endl;

	fout.close();
}