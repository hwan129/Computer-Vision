#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat gray_image = imread("lena.png", 0);
	int gamma = 10;
	unsigned char pix[256];

	if (gray_image.empty())
	{
		cout << "Could not open or find the image!" << endl;
		return -1;
	}

	Mat result_image = gray_image.clone();

	int h = gray_image.rows;
	int w = gray_image.cols;

	int value;

	for (int k = 0; k < 256; k++) {
		pix[k] = saturate_cast<uchar>(pow((float)(k / 255.0), gamma) * 255.0f);
	}


	for (int j = 0; j < gray_image.rows; j++) {
		for (int i = 0; i < gray_image.cols; i++) {
			value = gray_image.at<uchar>(j, i);		// �� pixel value ���ϱ�

			if (value < 127) {		// �ȼ� ���� 127���� ���� ���. nagative
				result_image.at<uchar>(h - 1 - i, j) = 255 - gray_image.at<uchar>(j, i);	// ���������� ����
			}
			else {			// �ȼ� ���� 127���� ū ���. gamma
				result_image.at<uchar>(h - 1 - i, j) = pix[result_image.at<uchar>(j, i)];	// ���������� ����
			}
		}
	}

	imshow("gray image", gray_image);
	imshow("result", result_image);

	waitKey(0);
}