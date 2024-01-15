#include <opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture cap("Full.mp4"), cap2("Empty.mp4"), cap3("Half.mp4");
	Mat fullFrame, halfFrame, background, fullGray, halfGray, fullMask, halfMask, frame;

	int fps = cap.get(CAP_PROP_FPS);
	int wait_time = 1000 / fps;

	cap2 >> background;
	resize(background, background, Size(640, 480));
	cvtColor(background, background, CV_BGR2GRAY);

	while (1) {
		if (cap.grab() == 0 || cap2.grab() == 0 || cap3.grab() == 0) break;
		cap.retrieve(fullFrame);	
		
		resize(fullFrame, fullFrame, Size(640, 480));
		cvtColor(fullFrame, fullGray, CV_BGR2GRAY);

		absdiff(background, fullGray, fullMask);
		threshold(fullMask, fullMask, 30, 255, CV_THRESH_BINARY);

		cap3.retrieve(halfFrame);

		resize(halfFrame, halfFrame, Size(640, 480));
		cvtColor(halfFrame, halfGray, CV_BGR2GRAY);

		absdiff(background, halfGray, halfMask);
		threshold(halfMask, halfMask, 30, 255, CV_THRESH_BINARY);

		cap3 >> frame;
		resize(frame, frame, Size(640, 480));

		imshow("Half", halfMask);
		imshow("Full", frame);
		imshow("background", background);
		
		waitKey(wait_time);
	}

}