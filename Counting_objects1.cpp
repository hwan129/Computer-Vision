#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat gray = imread("contours.png", 0);
	Mat result;
	threshold(gray, result, 230, 255, THRESH_BINARY_INV);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(result, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	putText(result, format("contour count: %d", contours.size()), Point(50, 80), FONT_HERSHEY_SIMPLEX, 1, Scalar(177, 8), 4);

	imshow("contours", result);
	waitKey(0);
}