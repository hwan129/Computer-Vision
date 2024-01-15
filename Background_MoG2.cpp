#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();	// 배경 제거 모델 초기화
	Mat image, foregroundMask, backgroundImg, foregroundImg;
	VideoCapture cap("background.mp4");

	while (true) {
		cap >> image;
		resize(image, image, Size(640, 480));

		if (foregroundMask.empty()) {
			foregroundMask.create(image.size(), image.type());
		}

		bg_model->apply(image, foregroundMask);		// 배경 제거 모델을 이용해 전경 마스크 계산
		GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);	// 전경 마스크 노이즈 감소
		threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY);		// 이진화
		foregroundImg = Scalar::all(0);		// 전경 이미지를 모두 검은색으로
		image.copyTo(foregroundImg, foregroundMask);		// 전경 마스크를 이용하여 현재 프레임을 foregroundImg에 저장
		bg_model->getBackgroundImage(backgroundImg);		// 배경 제거 모델을 이용해 배경 이미지 가져오기

		imshow("foreground mask", foregroundMask);
		imshow("foreground image", foregroundImg);

		if (!backgroundImg.empty()) {
			imshow("mean background image", backgroundImg);
		}
		waitKey(33);

	}
}