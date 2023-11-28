#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, canny;
	image = imread("lena.png", 0);

	Canny(image, canny, 190, 200, 3);		// input, output, edge 감지 최솟값, edge 감지 최댓값, 소벨 커널 크기 edge 검출에 사용되는 커널 크기

	imshow("Input image", image);
	imshow("canny", canny);

	waitKey(0);

}