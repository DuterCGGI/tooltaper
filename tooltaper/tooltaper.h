#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>

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

struct Calculate
{
	
	double toolR;          // 铣刀半径
	double lead;           // 导程
	int numofpoint;        // 点的个数
};

struct Result               // 点的柱坐标
{
	double x;               // x
	double y;               // R 半径
	double z;               // 旋转角度
	struct Result* next;
};


void getpoint(double toolLen,double toolD,double cutEdgelen,
	          double taperAgl,double helixAgl,double precision);