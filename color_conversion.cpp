#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image, image_YUV, dst;
	Mat yuv_channels[3];		// YUV ä�� ����

	image = imread("lena.png");

	cvtColor(image, image_YUV, CV_BGR2YUV);		// ����ȯ. BGR -> YUV

	split(image_YUV, yuv_channels);		// �̹����� YUV ä�η� �и�

	merge(yuv_channels, 3, dst);		// �и��� �̹����� �ٽ� ��ħ

	imshow("input image", image);
	imshow("Y", yuv_channels[0]);
	imshow("U", yuv_channels[1]);
	imshow("V", yuv_channels[2]);
	imshow("YUV image", dst);

	waitKey(0);

	return 0;
}