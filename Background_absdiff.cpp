#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture capture("background.mp4");
	Mat background, image, gray, result, foregroundMask, foregroundImg;

	capture >> background;
	cvtColor(background, background, CV_BGR2GRAY);

	while (true) {
		if (capture.grab() == 0) break;
		capture.retrieve(image);	// 프레임을 저장
		cvtColor(image, gray, CV_BGR2GRAY);

		absdiff(background, gray, foregroundMask);	// 현재 프레임과 배경 사이의 전경 마스크 얻기
		threshold(foregroundMask, foregroundMask, 50, 255, CV_THRESH_BINARY);	// 이진화. 최소 0, 최대 255 
		foregroundMask.copyTo(foregroundImg);	// 전경 마스크 복사하여 foregroundImg에
		gray.copyTo(foregroundImg, foregroundMask);		// 전경 마스크를 이용하여 현재 프레임의 전경을 foregroundImg에 저장

		imshow("foregroundImg", foregroundImg);
		imshow("foregroundMask", foregroundMask);
		imshow("background", background);

		waitKey(33);

	}
}