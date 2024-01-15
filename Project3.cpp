#include <opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
	VideoCapture cap;
	Mat frame, foregroundMask, backgroundImg, foregroundImg;

	if (cap.open("Full.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}

	int fps = cap.get(CAP_PROP_FPS);
	int wait_time = 1000 / fps;


	while (1) {
		cap >> frame;
		resize(frame, frame, Size(640, 480));

		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}

		bg_model->apply(frame, foregroundMask);		// ��� ���� ���� �̿��� ���� ����ũ ���
		GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);	// ���� ����ũ ������ ����
		threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY);		// ����ȭ
		foregroundImg = Scalar::all(0);		// ���� �̹����� ��� ����������
		frame.copyTo(foregroundImg, foregroundMask);		// ���� ����ũ�� �̿��Ͽ� ���� �������� foregroundImg�� ����
		bg_model->getBackgroundImage(backgroundImg);		// ��� ���� ���� �̿��� ��� �̹��� ��������

		imshow("foreground mask", foregroundMask);
		imshow("foreground image", foregroundImg);
		waitKey(wait_time);
	}

}

/*

*/