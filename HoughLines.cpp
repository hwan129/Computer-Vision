#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, edge, result;
	float rho, theta, a, b, x0, y0;		// ���� ��ȯ ����� ���� ������ ����
	Point p1, p2;		// ���� �������� ����
	vector<Vec2f>lines;		// ������ ���� ����
	image = imread("chess_pattern.png");
	result = image.clone();

	cvtColor(image, image, CV_BGR2GRAY);		// image�� �׷��� ������ �̹����� ��ȯ
	Canny(image, edge, 50, 200, 3);		// Canny edge ����. edge �̹��� ����

	HoughLines(edge, lines, 1, CV_PI / 180, 100, 0, CV_PI);		// ���� ��ȯ. edge �̹������� ���� ����
	//		 input, output,  �Ÿ� �ػ�(pixel����), ���� �ػ�(���� ����), ���� ��ȯ �������� ���� ���� ��(���� ���� edge�� �������� �ǴܵǱ� �����), ������ �ּҰ�, ������ �ִ밪(pi).

	for (int i = 0; i < lines.size(); i++) {
		rho = lines[i][0];		// ���� ��ȯ ��� ����
		theta = lines[i][1];	// ���� ��ȯ ��� ����
		a = cos(theta);
		b = sin(theta);		// sin�� cos���� ���� ���� ����

		x0 = a * rho;
		y0 = b * rho;		// ������ ����

		p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
		p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));		// ���� ����

		line(result, p1, p2, Scalar(0, 0, 255), 3, 8);		// ����� ������ ����. ����, 3 ����.
	}

	imshow("input image", image);
	imshow("edge", edge);
	imshow("Hough Transform", result);
	
	waitKey(0);
}