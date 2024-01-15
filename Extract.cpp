#include <opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	Mat image;
	Mat image_Ycbcr;
	Mat Ycbcr_channels[3];
	Mat y(512, 512, CV_8UC1);	// ä���� ��� ����

	image = imread("lena.png", IMREAD_COLOR);
	cvtColor(image, image_Ycbcr, CV_BGR2YCrCb);		// �̹��� ��ȯ
	split(image_Ycbcr, Ycbcr_channels);	// ä�� �и�

	for (int j = 0; j < 512; j++) {
		for (int i = 0; i < 512; i++) {
			y.at<uchar>(j, i) = 0;
			y.at<uchar>(j, i) = Ycbcr_channels[0].at<uchar>(j, i);
		}
	}

	imshow("Ychannel", y);
	imshow("image", image);
	waitKey(0);
	return 0;
}
