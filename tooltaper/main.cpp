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
	cv::Mat ProJ = cv::Mat(3,3,CV_64F,*aa);
	cv::Mat image_vector;
	std::vector<cv::Point2f> ImgCoor;
 	getpoint(tool, ProJ, ImgCoor);
	for (size_t i = 0; i < ImgCoor.size(); ++i)
	{
		std::cout<< ImgCoor[i]<< std::endl;
	}
	cv::waitKey();
	getchar();
	return 0;
}