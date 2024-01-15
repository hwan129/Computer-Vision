#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, blur, grad_x, grad_y, abs_grad_x, abs_grad_y, result;
	image = imread("lena.png", 0);
	GaussianBlur(image, blur, Size(5, 5), 5, 5, BORDER_DEFAULT);		// ����þ� ����. ����� ���̰� �ε巴��

	Sobel(blur, grad_x, CV_16S, 1, 0, 3);		// ���� ���� gradient ���.  CV_16S: 16��Ʈ ����. grad_x��
	convertScaleAbs(grad_x, abs_grad_x);		// grad_x �̹����� ���밪 ���. 

	Sobel(blur, grad_y, CV_16S, 0, 1, 3);		// input, output, ����� ������ Ÿ���� ����(���⼱ 16��Ʈ ����), x�� y ������ �̺�����(1 0�� x��, 0 1�� y��), Ŀ���� ũ��(3�� 3*3)
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, result);		// �� gradient �̹��� ����. edge ���� ���. abs_grad_x�� abs_grad_y �̹����� ���� 0.5�� ����ġ�� ��� result�� ����. ���� �� ���� ������ �׷����Ʈ ���� ����, ���� ������ ����.

	imshow("X", abs_grad_x);
	imshow("Y", abs_grad_y);
	imshow("Input image", image);
	imshow("Sobel Edge Detector", result);

	waitKey(0);
}