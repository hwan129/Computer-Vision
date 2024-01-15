#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void white_balacing(Mat img) {
	Mat bgr_channels[3];			// bgr�� �и��Ͽ� ����
	split(img, bgr_channels);		// bgr�� �и�

	double avg;
	int sum, temp, i, j, c;

	for (c = 0; c < img.channels(); c++){
		sum = 0;
		avg = 0.0f;
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				sum += bgr_channels[c].at<uchar>(i, j);		// �� ä���� �ȼ����� ���� ����
			}
		}
		avg = sum / (img.rows * img.cols);		// �� ä���� �ȼ� ��հ�

		for (i = 0; i < img.rows; i++) {		// �̹����� ��� �ȼ��� ���� �뷱�� ����.
			for (j = 0; j < img.cols; j++) {
				temp = (128 / avg) * bgr_channels[c].at<uchar>(i, j);		// �ȼ����� (128 / avg)�� ������
				if (temp > 255) {		// �ȼ����� 255�� ���� ��� 255�� ����
					bgr_channels[c].at<uchar>(i, j) = 255;
				}
				else {					// �ƴҰ�� �뷱�� ������ ������ ����
					bgr_channels[c].at<uchar>(i, j) = temp;
				}
			}
		}
	}
	merge(bgr_channels, 3, img);		// �ٽ� bgr�� ���ļ� img�� ����
}

int main() {
	Mat balancing, balancing_result;

	balancing = imread("balancing.jpg");
	balancing_result = balancing.clone();
	white_balacing(balancing_result);
	
	imshow("image", balancing);
	imshow("balancing", balancing_result);

	waitKey(0);
}