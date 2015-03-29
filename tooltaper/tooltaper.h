#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
#include <opencv2\opencv.hpp>

using namespace std;


struct Data
{
	double toolLen;       //���ϳ���
	double toolD;         //ϳ��ֱ��
	double cutEdgelen;    //�����г���
	double taperAgl;      //׶�ȽǶ�
	double helixAgl;      //������
	double precision;     //����
};

void getpoint(Data data,
	          const cv::Mat& ProjMat,
			  std::vector<cv::Point2f>& ImgCoordinate);