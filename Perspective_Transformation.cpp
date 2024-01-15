#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

struct MouseParams {	// 마우스 이벤트
	Mat img;	// 입력 이미지
	vector<Point2f> in, out;	// 선택한 점 저장, 출력 이미지 상의 점
};

static void onMouse(int event, int x, int y, int, void* param) {	// 마우스 이벤트
	MouseParams* mp = (MouseParams*)param;
	Mat img = mp->img;
	if (event == EVENT_LBUTTONDOWN) { // 왼쪽 버튼
		Mat result;

		mp->in.push_back(Point2f(x, y));	// 클릭한 좌표를 mp->n에 저장


		if (mp->in.size() == 4) {	// 클릭한 좌표가 4개가 된다면,
			Mat homo_mat = getPerspectiveTransform(mp->in, mp->out);	// 4개의 입력 점과 출력 점의 원근 변환 행렬 
			warpPerspective(img, result, homo_mat, Size(300, 300));		// 행렬을 이용하여 이미지 원근 변환
			imshow("output", result);	// 결과
		}
		else {
			result = img.clone();
			for (size_t i = 0; i < mp->in.size(); i++) {
				circle(result, mp->in[i], 3, Scalar(0, 0, 255), 5);
			}
			imshow("input", result);
		}
	}

	if (event == EVENT_RBUTTONDOWN) {	// 오른쪽 버튼
		mp->in.clear();		// 입력한 점들을 초기화
		imshow("input", img);
	}
}

int main() {
	Mat imput = imread("book.jpg");
	imshow("input", imput);

	MouseParams mp;		// 구조체
	mp.out.push_back(Point2f(0, 0));	// 각 점들의 위치 지정. 2D포인트
	mp.out.push_back(Point2f(300, 0));
	mp.out.push_back(Point2f(300, 300));
	mp.out.push_back(Point2f(0, 300));
	mp.img = imput;

	setMouseCallback("input", onMouse, (void*)&mp);		// 마우스 이벤트가 발생하면 onMouse로 이동
	waitKey();
	return 0;
}