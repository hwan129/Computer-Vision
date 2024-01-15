#include "opencv.hpp"
#include <iostream>


using namespace std;
using namespace cv;

int main() {
	Mat frame, canny_left, canny_right;
	VideoCapture cap;
	float rho, theta, a, b, x0, y0, total_rho, total_theta;
	float rho1, theta1, a1, b1, x1, y1, total_rho1, total_theta1;
	Point p1, p2;
	Point p3, p4;
	vector<Vec2f> lines1, lines2;

	Rect rect_left(200, 400, 400, 200);
	Rect rect_right(600, 400, 400, 200);

	if (cap.open("Road.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}

	int fps = cap.get(CAP_PROP_FPS);
	int time = 0;

	while (1)
	{
		cap >> frame;
		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}
		else if (time > 20000) {
			break;
		}

		time = cap.get(CAP_PROP_POS_MSEC);	// 현재 시간
		canny_left = frame(rect_left);		// 왼쪽 영역 프레임 가져옴
		canny_right = frame(rect_right);	// 오른쪽 영역 프레임 가져옴

		cvtColor(canny_left, canny_left, CV_BGR2GRAY);
		blur(canny_left, canny_left, Size(5, 5));
		Canny(canny_left, canny_left, 40, 200, 3);

		cvtColor(canny_right, canny_right, CV_BGR2GRAY);
		blur(canny_right, canny_right, Size(5, 5));
		Canny(canny_right, canny_right, 50, 200, 3);

		HoughLines(canny_left, lines1, 1, CV_PI / 180, 50, 0, 0, 0, CV_PI / 2);	// 직선의 정보 검출
		HoughLines(canny_right, lines2, 1, CV_PI / 180, 50, 0, 0, CV_PI / 2, CV_PI);
		//		 input, output,  거리 해상도(pixel단위), 각도 해상도(라디안 단위), 허프 변환 공간에서 직선 인정 수(높을 수록 edge가 직선으로 판단되기 어려움), 각도의 최소값, 각도의 최대값(pi).
		/*
		HoughLines(canny_left, lines1, 1, CV_PI / 180, 50, 0, 0, CV_PI / 6, CV_PI / 3);	// 직선의 정보 검출
		HoughLines(canny_right, lines2, 1, CV_PI / 180, 50, 0, 0, CV_PI / 3 * 2, CV_PI / 6 * 5);
		*/
		if (lines1.size()) {
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
			p1 = Point(cvRound(x0 + 1000 * (-b)) + 200, cvRound(y0 + 1000 * a) + 400);
			p2 = Point(cvRound(x0 - 1000 * (-b)) + 200, cvRound(y0 - 1000 * a) + 400);
			line(frame, p1, p2, Scalar(0, 0, 255), 3, 8);
		}
		
		if (lines2.size()) {
			total_rho1 = 0;
			total_theta1 = 0;

			for (int i = 0; i < lines2.size(); i++) {
				rho1 = lines2[i][0];
				theta1 = lines2[i][1];

				total_rho1 += rho1;
				total_theta1 += theta1;
			}
			total_rho1 /= lines2.size();
			total_theta1 /= lines2.size();

			a1 = cos(total_theta1);
			b1 = sin(total_theta1);
			x1 = a1 * total_rho1;
			y1 = b1 * total_rho1;
			p3 = Point(cvRound(x1 + 1000 * (-b1)) + 600, cvRound(y1 + 1000 * a1) + 400);
			p4 = Point(cvRound(x1 - 1000 * (-b1)) + 600, cvRound(y1 - 1000 * a1) + 400);
			line(frame, p3, p4, Scalar(0, 0, 255), 3, 8);
		}


		moveWindow("Left canny", 200, 0);
		imshow("Left canny", canny_left);

		moveWindow("Right canny", 600, 0);
		imshow("Right canny", canny_right);

		imshow("Frame", frame);
		waitKey(1000 / fps);
	}
}

// Mine
/*#include "opencv.hpp"
#include <iostream>


using namespace std;
using namespace cv;

int main() {
	Mat frame, right_roi, left_roi, canny_left, canny_right;
    vector<Vec2f> lines_left, lines_right;
    Point p1_left, p1_right, p2_left, p2_right;
	VideoCapture cap("Road.mp4");

	if (!cap.isOpened()) {
		cout << "no such file!" << endl;
		waitKey(0);
	}

    float a_left, a_right, b_left, b_right, x0_left, x0_right, y0_left, y0_right;

	while (1) {
		cap >> frame;	// 비디오 캡처 객체 cap에서 다음 프레임을 읽어와 frame 변수에 저장

		if (frame.empty()) {		// 비디오의 끝에 도달하면 종료
			cout << "end of video" << endl;
			break;
		}

		left_roi = frame(Rect(200, 400, 400, 200));
		right_roi = frame(Rect(600, 400, 400, 200));

		cvtColor(left_roi, canny_left, CV_BGR2GRAY);
		cvtColor(right_roi, canny_right, CV_BGR2GRAY);

		GaussianBlur(canny_left, canny_left, Size(5, 5), 0);
		GaussianBlur(canny_right, canny_right, Size(5, 5), 0);

		Canny(canny_left, canny_left, 10, 60, 3);
		Canny(canny_right, canny_right, 10, 60, 3);

		HoughLines(canny_left, lines_left, 1, CV_PI / 180, 50, 0, 0, CV_PI / 6, CV_PI / 3);
		HoughLines(canny_right, lines_right, 1, CV_PI / 180, 50, 0, 0, 2 * CV_PI / 3, 5 * CV_PI / 6);

        float avg_rho_left = 0, avg_theta_left = 0, avg_rho_right = 0, avg_theta_right = 0, count_left=0, count_right=0;
		
        for (int i = 0; i < lines_left.size(); i++) {
            avg_rho_left += lines_left[i][0];
            avg_theta_left += lines_left[i][1];
			count_left++;
        }
        for (int i = 0; i < lines_right.size(); i++) {
            avg_rho_right += lines_right[i][0];
            avg_theta_right += lines_right[i][1];
			count_right++;
        }

        avg_rho_left /= lines_left.size();
        avg_theta_left /= lines_left.size();
        avg_rho_right /= lines_right.size();
        avg_theta_right /= lines_right.size();

        a_left = cos(avg_theta_left);
        b_left = sin(avg_theta_left);
        x0_left = a_left * avg_rho_left+200;
        y0_left = b_left * avg_rho_left+400;
        p1_left = Point(cvRound(x0_left + 1000 * (-b_left)), cvRound(y0_left + 1000 * (a_left)));
        p2_left = Point(cvRound(x0_left - 1000 * (-b_left)), cvRound(y0_left - 1000 * (a_left)));

        a_right = cos(avg_theta_right);
        b_right = sin(avg_theta_right);
        x0_right = a_right * avg_rho_right+600;
        y0_right = b_right * avg_rho_right+400;
        p1_right = Point(cvRound(x0_right + 1000 * (-b_right)), cvRound(y0_right + 1000 * (a_right)));
        p2_right = Point(cvRound(x0_right - 1000 * (-b_right)), cvRound(y0_right - 1000 * (a_right)));

        line(frame, p1_left, p2_left, Scalar(0, 0, 255), 3, 8);
        line(frame, p1_right, p2_right, Scalar(0, 0, 255), 3, 8);

		imshow("Frame", frame);

		imshow("Left canny", canny_left);

		imshow("Right canny", canny_right);
		
		waitKey(5);
	}

	return 0;
}*/