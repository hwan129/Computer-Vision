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
		capture.retrieve(image);	// �������� ����
		cvtColor(image, gray, CV_BGR2GRAY);

		absdiff(background, gray, foregroundMask);	// ���� �����Ӱ� ��� ������ ���� ����ũ ���
		threshold(foregroundMask, foregroundMask, 50, 255, CV_THRESH_BINARY);	// ����ȭ. �ּ� 0, �ִ� 255 
		foregroundMask.copyTo(foregroundImg);	// ���� ����ũ �����Ͽ� foregroundImg��
		gray.copyTo(foregroundImg, foregroundMask);		// ���� ����ũ�� �̿��Ͽ� ���� �������� ������ foregroundImg�� ����

		imshow("foregroundImg", foregroundImg);
		imshow("foregroundMask", foregroundMask);
		imshow("background", background);

		waitKey(33);

	}
}