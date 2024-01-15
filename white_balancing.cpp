#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void white_balacing(Mat img) {
	Mat bgr_channels[3];			// bgr을 분리하여 저장
	split(img, bgr_channels);		// bgr로 분리

	double avg;
	int sum, temp, i, j, c;

	for (c = 0; c < img.channels(); c++){
		sum = 0;
		avg = 0.0f;
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				sum += bgr_channels[c].at<uchar>(i, j);		// 각 채널의 픽셀값을 합을 구함
			}
		}
		avg = sum / (img.rows * img.cols);		// 각 채널의 픽셀 평균값

		for (i = 0; i < img.rows; i++) {		// 이미지의 모든 픽셀에 대해 밸런스 조절.
			for (j = 0; j < img.cols; j++) {
				temp = (128 / avg) * bgr_channels[c].at<uchar>(i, j);		// 픽셀값에 (128 / avg)를 곱해줌
				if (temp > 255) {		// 픽셀값이 255가 넘을 경우 255로 고정
					bgr_channels[c].at<uchar>(i, j) = 255;
				}
				else {					// 아닐경우 밸런스 조절된 값으로 지정
					bgr_channels[c].at<uchar>(i, j) = temp;
				}
			}
		}
	}
	merge(bgr_channels, 3, img);		// 다시 bgr을 합쳐서 img에 저장
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