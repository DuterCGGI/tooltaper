#include <iostream>
#include "tooltaper.h"

int main()
{
	Data tool;
	tool.toolLen = 75;
	tool.toolD = 12;
	tool.cutEdgelen = 24;
	tool.taperAgl = 0;
	tool.helixAgl = 45; 
	tool.precision = 0.1;
	
    double aa[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
	cv::Mat image_vector;
 	getpoint(tool, *aa, image_vector, "image_vector.txt");
	return 0;
}