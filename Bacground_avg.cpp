#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture capture("background.mp4");
	Mat image, sum, avg;	// sum : �̹��� ���� ��, avg : ��� �̹��� ����
	int cnt = 2;	// �̹��� ������ �� ���, ������ �ʱⰪ 2

	capture >> avg;		// ù��° ������ avg�� ����

	while (true) {
		if (!capture.read(image)) break;
		add(image / cnt, avg * (cnt - 1) / cnt, avg);	// ���ο� ���� ��� �̹��� ����

		imshow("avg", avg);	
		cnt++;		// ī���� ���� ����. ���� �����ӿ� ���	
		waitKey(33);	// 33 �и��ʸ�ŭ ��ٸ�
	}

}