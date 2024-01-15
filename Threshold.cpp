#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("lena.png");
	Mat thresh, adaptive, inrange, otsu;
	cvtColor(image, image, CV_BGR2GRAY);
	
	threshold(image, thresh, 100, 255, THRESH_BINARY);

	adaptiveThreshold(image, adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);

	inRange(image, Scalar(0, 133, 77), Scalar(255, 173, 127), inrange);

	threshold(image, otsu, 0, 255, THRESH_BINARY | THRESH_OTSU);

	imshow("image", image);
	imshow("thresh", thresh);
	imshow("adaptive", adaptive);
	imshow("inRange", inrange);
	imshow("Otsu", otsu);

	waitKey(0);
	return 0;
}