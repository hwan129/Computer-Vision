#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, canny;
	image = imread("lena.png", 0);

	Canny(image, canny, 190, 200, 3);		// input, output, edge ���� �ּڰ�, edge ���� �ִ�, �Һ� Ŀ�� ũ�� edge ���⿡ ���Ǵ� Ŀ�� ũ��

	imshow("Input image", image);
	imshow("canny", canny);

	waitKey(0);

}