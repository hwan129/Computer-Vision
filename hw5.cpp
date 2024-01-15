#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat finger_print = imread("finger_print.png"), adaptive_1 = imread("adaptive_1.jpg"), adaptive = imread("adaptive.png");

	cvtColor(finger_print, finger_print, CV_BGR2GRAY);
	cvtColor(adaptive_1, adaptive_1, CV_BGR2GRAY);
	cvtColor(adaptive, adaptive, CV_BGR2GRAY);

	threshold(finger_print, finger_print, 160, 255, THRESH_BINARY);		// 이진화 160보다 크면 255, 나머지 0
	adaptiveThreshold(adaptive, adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5);	// size 3, C 5
	adaptiveThreshold(adaptive_1, adaptive_1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 95, 20); // size 95, C 20

	imshow("finger_print", finger_print);
	imshow("adaptive_1", adaptive_1);
	imshow("adaptive", adaptive);
	waitKey(0);

}