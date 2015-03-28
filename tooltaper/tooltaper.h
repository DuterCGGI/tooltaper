#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>

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

struct Result               // ���������
{
	double x;               // x
	double y;               // R �뾶
	double z;               // ��ת�Ƕ�
	struct Result* next;
};


void getpoint(double toolLen,double toolD,double cutEdgelen,
	          double taperAgl,double helixAgl,double precision);