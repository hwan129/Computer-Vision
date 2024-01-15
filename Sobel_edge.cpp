#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, blur, grad_x, grad_y, abs_grad_x, abs_grad_y, result;
	image = imread("lena.png", 0);
	GaussianBlur(image, blur, Size(5, 5), 5, 5, BORDER_DEFAULT);		// 가우시안 필터. 노이즈를 줄이고 부드럽게

	Sobel(blur, grad_x, CV_16S, 1, 0, 3);		// 가로 방향 gradient 계산.  CV_16S: 16비트 정수. grad_x에
	convertScaleAbs(grad_x, abs_grad_x);		// grad_x 이미지의 절대값 계산. 

	Sobel(blur, grad_y, CV_16S, 0, 1, 3);		// input, output, 결과의 데이터 타입의 깊이(여기선 16비트 정수), x와 y 방향의 미분차수(1 0은 x축, 0 1은 y축), 커널의 크기(3은 3*3)
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, result);		// 두 gradient 이미지 결합. edge 강도 계산. abs_grad_x와 abs_grad_y 이미지에 각각 0.5의 가중치로 섞어서 result에 저장. 수평 및 수직 방향의 그래디언트 값을 결합, 엣지 강도를 얻음.

	imshow("X", abs_grad_x);
	imshow("Y", abs_grad_y);
	imshow("Input image", image);
	imshow("Sobel Edge Detector", result);

	waitKey(0);
}