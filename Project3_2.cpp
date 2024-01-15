#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main() {
	VideoCapture cap2;
	int fps, delay;
	Mat frame2;
	Mat homo_mat;

	if (cap2.open("Empty.mp4") == 0) {
		cout << "There is no such video file" << endl;
		waitKey(0);
	}

	fps = cap2.get(CAP_PROP_FPS);
	delay = 1000 / fps;

	vector<Point2f> src;
	src.push_back(Point2f(0, 240));
	src.push_back(Point2f(320, 240));
	src.push_back(Point2f(320, 480));
	src.push_back(Point2f(0, 480));

	Mat result(600, 600, CV_8UC3);
	vector<Point2f> dst;
	dst.push_back(Point2f(0, 0));
	dst.push_back(Point2f(result.cols, 0));
	dst.push_back(Point2f(result.cols, result.rows));
	dst.push_back(Point2f(0, result.rows));

	while (1) {
		waitKey(delay);
		cap2 >> frame2; //contest
		resize(frame2, frame2, Size(640, 480));
		if (frame2.empty()) break;

		homo_mat = getPerspectiveTransform(src, dst);
		warpPerspective(frame2, result, homo_mat, result.size());
		//rectangle(frame2, Rect(200, 300, 600, 300), Scalar(0, 0, 255), 1, 8, 0);

		imshow("input", frame2);
		imshow("result", result);
	}
}