#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat frame, grayframe;
	VideoCapture cap;
	CascadeClassifier face_classifier;
	vector<Rect> faces;
	String text;

	Point lb, tr;

	Mat m_backproj, hsv;
	Mat m_model3d;
	Rect m_rc;
	float hrange[] = { 0, 180 };
	float srange[] = { 0, 255 };
	float vrange[] = { 0, 255 };
	const float* ranges[] = { hrange, srange, vrange };
	int channels[] = { 0, 1, 2 };
	int hist_sizes[] = { 16, 16, 16 };

	if (cap.open("Faces.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}

	face_classifier.load("haarcascade_frontalface_alt.xml");

	double fps = cap.get(CAP_PROP_FPS);
	double wait_time = 1000 / fps;
	int keyboard = -1;
	int status = 82;
	int prev_status = status;

	while (1) {
		cap >> frame;
		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}

		cvtColor(frame, grayframe, COLOR_BGR2GRAY);

		keyboard = waitKey(wait_time);
		if (keyboard != -1) {
			status = keyboard;
		}
		if (status != 84 && status != 116) {
			destroyWindow("Tracking");
		}

		if (status == 70 || status == 102) {		// 'F' or 'f'
			face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(30, 30), Size(40, 40));
			text = "f";
		}
		else if (status == 77 || status == 109) {		// 'M' or 'm'
			face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(55, 55), Size(56, 56));
			text = "m";
		}
		else if (status == 78 || status == 110) {	// 'N' or 'n'
			face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(70, 70), Size(90, 90));
			text = "n";
		}
		else if (status == 84 || status == 116) {		// 'T' or 't'
			cap >> frame;

			if (text == "") {
				putText(frame, "Detect before tracking!", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
			}
			else {
				if (text == "f") {
					face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(30, 30), Size(40, 40));
				}
				else if (text == "m") {
					face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(55, 55), Size(56, 56));
				}
				else if (text == "n") {
					face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(70, 70), Size(90, 90));
				}

				for (int i = 0; i < faces.size(); i++) {
					lb = Point(faces[i].x + faces[i].width + 20, faces[i].y + faces[i].height + 20);	// 우측 하단
					tr = Point(faces[i].x - 20, faces[i].y - 20);	// 좌측 상단
					rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
				}

				for (int i = 0; i < frame.rows; i++) {
					for (int j = 0; j < frame.cols; j++) {
						if ((tr.y > i || tr.x > j) || (lb.y < i || lb.x < j)) {
							frame.at<Vec3b>(i, j) = Vec3b(255, 0, 0);
						}
					}
				}

				prev_status = status;
				imshow("Tracking", frame);
				continue;
			}
		}
		else if (status == 82 || status == 114) {		// 'R' or 'r'
			prev_status = status;
			imshow("faces", frame);
			continue;
		}
		else {
			status = prev_status;
			continue;
		}


		for (int i = 0; i < faces.size(); i++) {
			lb = Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height);	// 우측 하단
			tr = Point(faces[i].x, faces[i].y);	// 좌측 상단
			rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
			putText(frame, text, Point(faces[i].x, faces[i].y + faces[i].height), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
		}

		cout << "prev_status : " << prev_status << "	status : " << status << endl;
		prev_status = status;
		imshow("faces", frame);
	}
}