#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

struct MouseParams {	// 마우스 이벤트
	Mat frame1, frame1_display, frame2;	// Timesquare. contest 
	vector<Point2f> in, out;	//contest의 전체. Timesquare에서 contest를 삽입할 부분.
};

static void onMouse(int event, int x, int y, int, void* param) {	// 마우스 이벤트
	MouseParams* mp = (MouseParams*)param;

	if (event == EVENT_LBUTTONDOWN && mp->out.size() < 4) { // 왼쪽 버튼 
		Mat result;

		mp->out.push_back(Point2f(x, y));	// 클릭한 좌표를 mp->out에 저장. Timesquare에 클리하는 부분

		if (mp->out.size() == 4) {	// 클릭한 좌표가 4개가 된다면,
			mp->in.push_back(Point2f(0, 0));			// contest 영상의 전체
			mp->in.push_back(Point2f(mp->frame2.cols, 0));
			mp->in.push_back(Point2f(mp->frame2.cols, mp->frame2.rows));
			mp->in.push_back(Point2f(0, mp->frame2.rows));
		}
	}
}

int main() {
	Mat frame1, frame2;			// 1 : Timesquare, 2 : contest
	VideoCapture cap1, cap2;
	MouseParams mp;		// 구조체
	

	if (cap1.open("Timesquare.mp4") == 0 || cap2.open("contest.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}
	double fps = cap1.get(CAP_PROP_FPS);
	double wait_time = 1000 / fps;
	cap1 >> frame1;
	cap2 >> frame2;


	while (true)
	{
		cap1 >> frame1;
		cap2 >> frame2;
		mp.frame1 = frame1;
		mp.frame2 = frame2;
		if (frame1.empty() || frame2.empty()) {
			cout << "end of video" << endl;
			break;
		}

		if (mp.out.size() < 4) {

			setMouseCallback("background", onMouse, (void*)&mp);		// 마우스 이벤트가 발생하면 onMouse로 이동
			for (size_t i = 0; i < mp.out.size(); i++) {
				circle(frame1, mp.out[i], 3, Scalar(0, 0, 255), 5);
			}
			
			imshow("background", frame1);
		}
		else {
			Mat result;
			Mat black;
			Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);
			warpPerspective(frame2, result, homo_mat, frame1.size());

			vector<vector<Point>> ppt = { vector<Point>{mp.out[0], mp.out[1], mp.out[2], mp.out[3]} };
			fillPoly(frame1, ppt, Scalar(0, 0, 0));

			result += frame1;

			imshow("background", result);
		}
		imshow("input", frame2);
		waitKey(wait_time);
	}
	return 0;
}