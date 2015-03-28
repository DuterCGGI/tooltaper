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
 	getpoint(tool.toolLen,tool.toolD,tool.cutEdgelen,tool.taperAgl,tool.helixAgl,tool.precision);
	return 0;
}