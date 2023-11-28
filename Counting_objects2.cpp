#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat gray = imread("contours.png", 0);
	Mat result;
	threshold(gray, result, 180, 255, THRESH_BINARY_INV);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(result, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		boundRect[i] = boundingRect(Mat(contours[i]));
	}
	
	for (int i = 0; i < contours.size(); i++) {
		rectangle(result, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2, 8, 0);
	}

	imshow("contours", result);
	waitKey(0);
}