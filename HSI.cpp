#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("colorful.jpg");
	Mat HSV, intensity_change, mask_out, change_color;
	vector<Mat> ic(3);		// 3개의 채널을 가진 이미지를 저장
	vector<Mat> mo(3);
	vector<Mat> cc(3);

	int rows = image.rows;
	int cols = image.cols;
	uchar* h;
	uchar* s;
	uchar* v;

	cvtColor(image, HSV, COLOR_BGR2HSV);		// BGR 공간 -> HSV 공간으로 변환
	split(HSV, ic);			// HSV 이미지를 ic, mo, cc 벡터에 채널별로 저장
	split(HSV, mo);
	split(HSV, cc);

	equalizeHist(ic[2], ic[2]);		// 명암채널(Y채널)에 대해 히스토그램 평활화 수행

	for (int j = 0; j < rows; j++)		// mo 벡터에서 H(색상) 채널 조절. 특정범위 마스크 아웃. 나머지는 그대로
	{
		h = mo[0].ptr<uchar>(j);
		s = mo[1].ptr<uchar>(j);
		for (int i = 0; i < cols; i++)
		{
			if (h[i] > 9 && h[i] < 23) {
				s[i] = s[i];
			}
			else
			{
				s[i] = 0;
			}
		}
	}
	for (int j = 0; j < rows; j++)		// cc 벡터에서 H(색상) 채널 조절. 색상을 50만큼 회전
	{
		h = cc[0].ptr<uchar>(j);
		s = cc[1].ptr<uchar>(j);
		for (int i = 0; i < cols; i++) {
			if (h[i] + 50 > 179) {
				h[i] = h[i] + 50 - 179;
			}
			else
			{
				h[i] += 50;
			}
		}
	}

	merge(ic, intensity_change);		// 채널별로 분리된 이미지 합침. 
	merge(mo, mask_out);
	merge(cc, change_color);

	cvtColor(intensity_change, intensity_change, COLOR_HSV2BGR);	// HSV -> BGR로 다시 변환
	cvtColor(mask_out, mask_out, COLOR_HSV2BGR);
	cvtColor(change_color, change_color, COLOR_HSV2BGR);

	imshow("image", image);
	imshow("intensity change", intensity_change);
	imshow("mask out", mask_out);
	imshow("change color", change_color);

	waitKey(0);

	return 0;
}