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

struct Calculate
{
	
	double toolR;          // ϳ���뾶
	double lead;           // ����
	int numofpoint;        // ��ĸ���
};


void getpoint(Data data,double* arrar,cv::Mat imav, const char* filename);