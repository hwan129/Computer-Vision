#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("colorful.jpg");
	Mat HSV, intensity_change, mask_out, change_color;
	vector<Mat> ic(3);		// 3���� ä���� ���� �̹����� ����
	vector<Mat> mo(3);
	vector<Mat> cc(3);

	int rows = image.rows;
	int cols = image.cols;
	uchar* h;
	uchar* s;
	uchar* v;

	cvtColor(image, HSV, COLOR_BGR2HSV);		// BGR ���� -> HSV �������� ��ȯ
	split(HSV, ic);			// HSV �̹����� ic, mo, cc ���Ϳ� ä�κ��� ����
	split(HSV, mo);
	split(HSV, cc);

	equalizeHist(ic[2], ic[2]);		// ����ä��(Yä��)�� ���� ������׷� ��Ȱȭ ����

	for (int j = 0; j < rows; j++)		// mo ���Ϳ��� H(����) ä�� ����. Ư������ ����ũ �ƿ�. �������� �״��
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
	for (int j = 0; j < rows; j++)		// cc ���Ϳ��� H(����) ä�� ����. ������ 50��ŭ ȸ��
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

	merge(ic, intensity_change);		// ä�κ��� �и��� �̹��� ��ħ. 
	merge(mo, mask_out);
	merge(cc, change_color);

	cvtColor(intensity_change, intensity_change, COLOR_HSV2BGR);	// HSV -> BGR�� �ٽ� ��ȯ
	cvtColor(mask_out, mask_out, COLOR_HSV2BGR);
	cvtColor(change_color, change_color, COLOR_HSV2BGR);

	imshow("image", image);
	imshow("intensity change", intensity_change);
	imshow("mask out", mask_out);
	imshow("change color", change_color);

	waitKey(0);

	return 0;
}