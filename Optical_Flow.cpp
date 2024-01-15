#include <iostream>
#include "opencv.hpp"

using namespace std;
using namespace cv;

struct feature {
	Point2f pt;
	int val;
};
bool initialization = false;
void DrawTrackingPoints(vector<Point2f>& points, Mat& image) {
	for (int i = 0; i < points.size(); i++) {
		int x = cvRound(points[i].x);
		int y = cvRound(points[i].y);
		circle(image, Point(x, y), 3, Scalar(255, 0, 0, 0), 2);
	}
}

int main(int argc, char* argv[]) {
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Cannot open cap" << endl;
		return 0;
	}
	double fps = cap.get(CV_CAP_PROP_FPS);
	Mat currImage, prevImage;
	Mat frame, dstImage;

	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	int maxCorners = 500;

	TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 10, 0.01);
	Size winSize(11, 11);

	vector<Point2f> prevPoints;
	vector<Point2f> currPoints;
	vector<Point2f> boundPoints;

	int delay = 1000 / fps;
	int nframe = 0;

	while (1) {
		cap >> frame;
		if (frame.empty()) break;
		frame.copyTo(dstImage);
		cvtColor(dstImage, currImage, CV_BGR2GRAY);
		GaussianBlur(currImage, currImage, Size(5, 5), 0.5);

		if (initialization) {
			goodFeaturesToTrack(prevImage, prevPoints, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
		}

		cornerSubPix(prevImage, prevPoints, winSize, Size(-1, -1), criteria);

		DrawTrackingPoints(prevPoints, dstImage);
		initialization = false;

		if (prevPoints.size() > 0) {
			vector<Mat> prevPyr, currPyr;
			Mat status, err;

			buildOpticalFlowPyramid(prevImage, prevPyr, winSize, 3, true);
			buildOpticalFlowPyramid(currImage, currPyr, winSize, 3, true);
			calcOpticalFlowPyrLK(prevPyr, currPyr, prevPoints, currPoints, status, err, winSize);

			for (int i = 0; i < prevPoints.size(); i++) {
				if (!status.at<uchar>(i)) {
					prevPoints.erase(prevPoints.begin() + i);
					currPoints.erase(currPoints.begin() + i);
				}
			}
			DrawTrackingPoints(currPoints, dstImage);
			prevPoints = currPoints;
		}
		imshow("dstImage", dstImage);
		currImage.copyTo(prevImage);

		int ch = waitKey(33);
		if (ch == 27) break;
		if (ch == 32) initialization = true;
	}
	return 0;
}