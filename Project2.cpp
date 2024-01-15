#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

//720*480
int main() {
	Mat frame, lane;
	VideoCapture cap;
	vector<Vec2f> lines1;
	float rho, theta, a, b, x0, y0, total_rho, total_theta;
	Point p1, p2;

	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();		// lane
	Ptr<BackgroundSubtractor> bg_model2 = createBackgroundSubtractorMOG2();		// move
	Mat move, foregroundMask;		// 앞 차량
	Mat move2, foregroundMask2;		// 배경. 내가 움직이는가

	if (cap.open("Project2_video.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}
	
	Rect lane_depart(270, 240, 180, 180);
	Rect moving(270, 240, 180, 120);		// 앞 차량이 움직이는가
	Rect moving2(270, 0, 180, 120);			// 본인이 정지해 있는가

	int fps = cap.get(CAP_PROP_FPS);
	int time = 0, lane_start = 0, move_start = 0;
	bool lane_text = false, move_text = false;

	while (1) {
		cap >> frame;
		time = cap.get(CAP_PROP_POS_MSEC);

		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}

		lane = frame(lane_depart);	
		move = frame(moving);
		move2 = frame(moving2);

		cvtColor(lane, lane, CV_BGR2GRAY);
		GaussianBlur(lane, lane, Size(5, 5), 0);
		Canny(lane, lane, 50, 200, 3);

		HoughLines(lane, lines1, 1, CV_PI / 180, 73, 0, 0, 5 * CV_PI / 6, 7 * CV_PI / 6);		// lane

		if (lines1.size()) {
			lane_start = time;
			lane_text = true;

			total_rho = 0;
			total_theta = 0;
			for (int i = 0; i < lines1.size(); i++) {
				rho = lines1[i][0];
				theta = lines1[i][1];

				total_rho += rho;
				total_theta += theta;
			}
			total_rho /= lines1.size();
			total_theta /= lines1.size();

			a = cos(total_theta);
			b = sin(total_theta);
			x0 = a * total_rho;
			y0 = b * total_rho;
			p1 = Point(cvRound(x0 + 500 * (-b)) + 270, cvRound(y0 + 500 * a) + 240);
			p2 = Point(cvRound(x0 - 500 * (-b)) + 270, cvRound(y0 - 500 * a) + 240);
		}

		if (lane_text == true) {
			putText(frame, "Lane departure", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
		}
		
		if (time - lane_start > 2000) {
			lane_text = false;
		}

		bg_model->apply(move, foregroundMask);		// 배경 제거 모델을 이용해 전경 마스크 계산
		GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 5, 5);	// 전경 마스크 노이즈 감소
		threshold(foregroundMask, foregroundMask, 20, 255, THRESH_BINARY);		// 이진화

		bg_model2->apply(move2, foregroundMask2);
		GaussianBlur(foregroundMask2, foregroundMask2, Size(11, 11), 5, 5);
		threshold(foregroundMask2, foregroundMask2, 20, 255, THRESH_BINARY);

		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(foregroundMask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		int white = 0;
		for (int j = 0; j < foregroundMask.rows; j++) {			// 이진화 한 영상에 흰색이 얼마나 있는가
			for (int i = 0; i < foregroundMask.cols; i++) {
				if (foregroundMask.at<uchar>(j, i) == 255 ) {
					white++;
				}
			}
		}

		vector<vector<Point>> contours2;
		vector<Vec4i> hierarchy2;
		findContours(foregroundMask2, contours2, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		int white2 = 0;
		for (int j = 0; j < foregroundMask2.rows; j++) {
			for (int i = 0; i < foregroundMask2.cols; i++) {
				if (foregroundMask2.at<uchar>(j, i) == 255) {
					white2++;
				}
			}
		}
		
		if (white > 20000 && time > 1000/fps && white2 < 7000) {		// 앞 차량이 움직일 경우 and 첫 프레임을 제외 and 본인이 정지해 있는가
			move_start = time;
			move_text = true;
		}
		if (move_text == true) {
				putText(frame, "Start Moving!", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
		}
		if (time - move_start > 1000) {
			move_text = false;
		}

		imshow("Project2", frame);
		waitKey(1000 / fps);
	}
}