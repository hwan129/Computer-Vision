#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image, image_YUV, dst;
	Mat yuv_channels[3];		// YUV 채널 저장

	image = imread("lena.png");

	cvtColor(image, image_YUV, CV_BGR2YUV);		// 색전환. BGR -> YUV

	split(image_YUV, yuv_channels);		// 이미지를 YUV 채널로 분리

	merge(yuv_channels, 3, dst);		// 분리된 이미지를 다시 합침

	imshow("input image", image);
	imshow("Y", yuv_channels[0]);
	imshow("U", yuv_channels[1]);
	imshow("V", yuv_channels[2]);
	imshow("YUV image", dst);

	waitKey(0);

	return 0;
}