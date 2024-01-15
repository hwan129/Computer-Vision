#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, edge, result;
	vector<Vec4i> lines;		// ���� ��ȯ ����� �� ����

	image = imread("chess_pattern.png");
	result = image.clone();
	cvtColor(image, image, CV_BGR2GRAY);		// �̹����� �׷��� �����Ϸ� ����
	Canny(image, edge, 50, 200, 3);				// Canny edge ������ �̿��Ͽ� edge ����

	HoughLinesP(edge, lines, 1, CV_PI / 180, 50, 10, 300);		// edge �̹������� ���� ����
	//		input, output, �Ÿ� �ػ�(�ȼ�), ���� �ػ�(����), �������� ������ �ּ� ��ǥ ��, ���� ���� �ּҰ���, ���� �ּ� ����

	for (int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];		// ������ i��° ������ ������ Vec4i ������ ���� l�� ����. �� ������ ������ �������� ������ ��Ÿ��
		line(result, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, 8);		// ����� ������ result�� ����. Point�� ���۰� ����. ���������� 3�ȼ��� ����.
	}

	imshow("Input image", image);
	imshow("edge", edge);
	imshow("Hough Transform", result);

	waitKey(0);

}