#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat gray = imread("xray.jpg", 0);
	Mat color;
	// Applies a colormap on a given image
	applyColorMap(gray, color, COLORMAP_JET);		// input, output, COLORMAT_JET: gray�� ������ ���� ���� �Է� 
	
	imshow("gray", gray);
	imshow("image", color);

	waitKey(0);

}