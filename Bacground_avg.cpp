#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture capture("background.mp4");
	Mat image, sum, avg;	// sum : 이미지 누적 합, avg : 평균 이미지 저장
	int cnt = 2;	// 이미지 누적할 때 사용, 프레임 초기값 2

	capture >> avg;		// 첫번째 프레임 avg에 저장

	while (true) {
		if (!capture.read(image)) break;
		add(image / cnt, avg * (cnt - 1) / cnt, avg);	// 새로운 누적 평균 이미지 저장

		imshow("avg", avg);	
		cnt++;		// 카운터 변수 증가. 다음 프레임에 사용	
		waitKey(33);	// 33 밀리초만큼 기다림
	}

}