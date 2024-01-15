#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, edge, result;
	vector<Vec4i> lines;		// 허프 변환 검출된 선 저장

	image = imread("chess_pattern.png");
	result = image.clone();
	cvtColor(image, image, CV_BGR2GRAY);		// 이미지를 그레이 스케일로 저장
	Canny(image, edge, 50, 200, 3);				// Canny edge 검출을 이용하여 edge 검출

	HoughLinesP(edge, lines, 1, CV_PI / 180, 50, 10, 300);		// edge 이미지에서 직선 검출
	//		input, output, 거리 해상도(픽셀), 각도 해상도(라디안), 직선으로 인정될 최소 투표 수, 직선 사이 최소간격, 직선 최소 길이

	for (int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];		// 벡터의 i번째 직선의 정보를 Vec4i 형식의 변수 l에 저장. 이 형식은 직선의 시작점과 끝점을 나타냄
		line(result, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, 8);		// 검출된 직선을 result에 저장. Point로 시작과 끝점. 빨간색으로 3픽셀의 굵기.
	}

	imshow("Input image", image);
	imshow("edge", edge);
	imshow("Hough Transform", result);

	waitKey(0);

}