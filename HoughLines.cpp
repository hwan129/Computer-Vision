#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, edge, result;
	float rho, theta, a, b, x0, y0;		// 허프 변환 결과인 선의 정보를 저장
	Point p1, p2;		// 선의 시작점과 끝점
	vector<Vec2f>lines;		// 직선의 정보 저장
	image = imread("chess_pattern.png");
	result = image.clone();

	cvtColor(image, image, CV_BGR2GRAY);		// image를 그레이 스케일 이미지로 변환
	Canny(image, edge, 50, 200, 3);		// Canny edge 검출. edge 이미지 생성

	HoughLines(edge, lines, 1, CV_PI / 180, 100, 0, CV_PI);		// 허프 변환. edge 이미지에서 직선 검출
	//		 input, output,  거리 해상도(pixel단위), 각도 해상도(라디안 단위), 허프 변환 공간에서 직선 인정 수(높을 수록 edge가 직선으로 판단되기 어려움), 각도의 최소값, 각도의 최대값(pi).

	for (int i = 0; i < lines.size(); i++) {
		rho = lines[i][0];		// 허프 변환 결과 직선
		theta = lines[i][1];	// 허프 변환 결과 직선
		a = cos(theta);
		b = sin(theta);		// sin과 cos으로 직선 방향 정의

		x0 = a * rho;
		y0 = b * rho;		// 시작점 정의

		p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
		p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));		// 끝점 정의

		line(result, p1, p2, Scalar(0, 0, 255), 3, 8);		// 검출된 직선을 저장. 빨간, 3 굵기.
	}

	imshow("input image", image);
	imshow("edge", edge);
	imshow("Hough Transform", result);
	
	waitKey(0);
}