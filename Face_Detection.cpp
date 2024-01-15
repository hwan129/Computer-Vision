#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	CascadeClassifier face_classifier;
	Mat frame, grayframe;
	vector<Rect> faces;
	int i;

	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cout << "Could not open camera" << endl;
		return -1;
	}

	face_classifier.load("haarcascade_frontalface_alt.xml");

	while (true) {
		cap >> frame;

		cvtColor(frame, grayframe, COLOR_BGR2GRAY);

		face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(30, 30));
			
		for (i = 0; i < faces.size(); i++) {
				Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
				Point tr(faces[i].x, faces[i].y);
				rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
		}

		imshow("Face Detection", frame);
		if (waitKey(33) == 27) break;
	}
	

}