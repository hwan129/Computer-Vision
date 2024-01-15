#include <opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int isDetectFace(CascadeClassifier face_classifier, Mat frameGray);

/* ����
* 3�� ���� ���ÿ� ��� ���ÿ� Face Detection�ϴϱ� ���α׷��� �ʹ� ���ſ����� 1, 2, 3���� ���� ����ǰ� �߾��.
* 
* ROI�� ���� �簢���� rect1, rect2, rect3, rect4�� �����س��ҽ��ϴ�.
* Broi�� background subtraction�� ���� �ǵ�, ���� ��ü �̹��������� background�� �޶����� �� �ִ��� Ȯ���ϽŴٸ� ����ŵ� ������ �� ���ƿ�!
* is1, is2, is3, is4�� �� roi���� ���� ������� ���� 1�� ��ȯ�ϰ�, �� ������ �� ���� ���� 0�� �����մϴ�.
* 
* 
* cout << "is1: " << is1 << endl;
			cout << "is2: " << is2 << endl;
			cout << "is3: " << is3 << endl;
			cout << "is4: " << is4 << endl;
			cout << endl;
  case�� ���� while(1)�� �ȿ� �� �ڵ带 ���ø�
  �� ������ �ȿ� �� �� �̻� face detection�� �Ǹ� ����� �ִ� �ɷ� ���� �Ǵ��� �� Ȯ���� ���� �� ���ƿ�!
  ���÷� 1�� case�� �־� ���Ҿ��!

  ��� Ȯ���غ� �ٷδ� 3�� roi�� �� ��� �Ӹ��� ������ �� detection�� �� �Ǵ���
  �ؿ� rect3 �����ø� �ּ�ó���س��� �� �ִµ� �̰� ���� ��������� �ڵ� �����Ͻñ⿡�� ���Ͻ� �� ���ƿ�.
*/

int main() {
	VideoCapture cap("Full.mp4"), cap2("Empty.mp4"), cap3("Half.mp4");
	Mat fullFrame, halfFrame, background, fullGray, halfGray, fullMask, halfMask;
	Mat emptyFrame, emptyGray, emptyMask; //Add to show the case of empty

	Rect rect1, rect2, rect3, rect4; //To set roi of each seats
	Mat Broi1, Broi2, Broi3, Broi4; //ROI for background subtraction <------------- ������ ������ �κ�
	Mat Froi1, Froi2, Froi3, Froi4; //ROI for face detection

	int whatdoyouwant;//For check the answer of what case do you want
	int is1 = 0, is2 = 0, is3 = 0, is4 = 0;//For check the Face Dection of each seats

	int fps = cap.get(CAP_PROP_FPS);
	int wait_time = 1000 / fps;

	//Face detector
	CascadeClassifier face_classifier;
	face_classifier.load("haarcascade_frontalface_alt.xml");

	//Set roi
	rect1 = Rect(70, 180, 120, 250); //���� �Ʒ�
	rect2 = Rect(280, 200, 120, 250);//������ �Ʒ�
	rect3 = Rect(40, 160, 80, 160);//���� ��
	//rect3 = Rect(40, 160, 100, 160); <------------- ������ ������ �κ�
	rect4 = Rect(250, 160, 100, 160);//������ ��

	//������ �� ���� ����
	/////////////////////////////////////////////////////////////////////////////////////////////////

	cap2 >> background;
	resize(background, background, Size(640, 480));
	cvtColor(background, background, CV_BGR2GRAY);

	//Ask what case do you want
	cout << "What case do you want?(1: Full, 2: Half, 3: Empty) ";
	cin >> whatdoyouwant;

	//If Full
	if (whatdoyouwant == 1) {
		while (1) {
			if (cap.grab() == 0) break;

			//Backgorund subtraction
			cap.retrieve(fullFrame);

			resize(fullFrame, fullFrame, Size(640, 480));
			cvtColor(fullFrame, fullGray, CV_BGR2GRAY);

			absdiff(background, fullGray, fullMask);
			threshold(fullMask, fullMask, 50, 255, CV_THRESH_BINARY);

			/*Broi1 = fullMask(rect1); <------------- ������ ������ �κ�
			Broi2 = fullMask(rect2);
			Broi3 = fullMask(rect3);
			Broi4 = fullMask(rect4);*/

			//Face detection
			Froi1 = fullGray(rect1);
			Froi2 = fullGray(rect2);
			Froi3 = fullGray(rect3);
			Froi4 = fullGray(rect4);
			is1 = isDetectFace(face_classifier, Froi1);
			is2 = isDetectFace(face_classifier, Froi2);
			is3 = isDetectFace(face_classifier, Froi3);
			is4 = isDetectFace(face_classifier, Froi4);


			//Face detection Ȯ�ο�
			cout << "is1: " << is1 << endl;
			cout << "is2: " << is2 << endl;
			cout << "is3: " << is3 << endl;
			cout << "is4: " << is4 << endl;
			cout << endl;

			imshow("Full", fullFrame);

			//ROI Ȯ���ϰ� ������ ��
			//imshow("Roi1", Froi1);
			//imshow("Roi2", Froi2);
			//imshow("Roi3", Froi3);
			//imshow("Roi4", Froi4);

			//Background subtraction �� ROI Ȯ���ϰ� ������ ��
			//imshow("Roi1", Broi1);
			//imshow("Roi2", Broi2);
			//imshow("Roi3", Broi3);
			//imshow("Roi4", Broi4);
			waitKey(wait_time);
		}
	}
	//If half
	else if (whatdoyouwant == 2) {
		while (1) {
			if (cap3.grab() == 0) break;

			cap3.retrieve(halfFrame);

			resize(halfFrame, halfFrame, Size(640, 480));
			cvtColor(halfFrame, halfGray, CV_BGR2GRAY);

			absdiff(background, halfGray, halfMask);
			threshold(halfMask, halfMask, 50, 255, CV_THRESH_BINARY);

			//Broi1 = halfMask(rect1);
			//Broi2 = halfMask(rect2);
			//Broi3 = halfMask(rect3);
			//Broi4 = halfMask(rect4);

			Froi1 = halfGray(rect1);
			Froi2 = halfGray(rect2);
			Froi3 = halfGray(rect3);
			Froi4 = halfGray(rect4);
			is1 = isDetectFace(face_classifier, Froi1);
			is2 = isDetectFace(face_classifier, Froi2);
			is3 = isDetectFace(face_classifier, Froi3);
			is4 = isDetectFace(face_classifier, Froi4);

			imshow("Half", halfFrame);

			//ROI Ȯ���ϰ� ������ ��
			imshow("Roi1", Froi1);
			imshow("Roi2", Froi2);
			imshow("Roi3", Froi3);
			imshow("Roi4", Froi4);

			//Background subtraction �� ROI Ȯ���ϰ� ������ ��
			//imshow("Roi1", Broi1);
			//imshow("Roi2", Broi2);
			//imshow("Roi3", Broi3);
			//imshow("Roi4", Broi4);

			waitKey(wait_time);
		}
	}
	//If empty
	else if (whatdoyouwant == 3) {
		while (1) {
			if (cap2.grab() == 0) break;

			cap2.retrieve(emptyFrame);

			resize(emptyFrame, emptyFrame, Size(640, 480));
			cvtColor(emptyFrame, emptyGray, CV_BGR2GRAY);

			absdiff(background, emptyGray, emptyMask);
			threshold(emptyMask, emptyMask, 50, 255, CV_THRESH_BINARY);

			/*Broi1 = emptyMask(rect1);
			Broi2 = emptyMask(rect2);
			Broi3 = emptyMask(rect3);
			Broi4 = emptyMask(rect4);*/

			Froi1 = emptyGray(rect1);
			Froi2 = emptyGray(rect2);
			Froi3 = emptyGray(rect3);
			Froi4 = emptyGray(rect4);
			is1 = isDetectFace(face_classifier, Froi1);
			is2 = isDetectFace(face_classifier, Froi2);
			is3 = isDetectFace(face_classifier, Froi3);
			is4 = isDetectFace(face_classifier, Froi4);

			imshow("Empty", emptyFrame);

			//ROI Ȯ���ϰ� ������ ��
			//imshow("Roi1", Froi1);
			//imshow("Roi2", Froi2);
			//imshow("Roi3", Froi3);
			//imshow("Roi4", Froi4);

			//Background subtraction �� ROI Ȯ���ϰ� ������ ��
			//imshow("Roi1", Broi1);
			//imshow("Roi2", Broi2);
			//imshow("Roi3", Broi3);
			//imshow("Roi4", Broi4);

			waitKey(wait_time);
		}
	}
	else {
		cout << "There is no such case" << endl;
		return 0;
	}
}

//If find a face, return 1. But if not, return 0
int isDetectFace(CascadeClassifier face_classifier, Mat frameGray) {
	vector<Rect> faces;
	face_classifier.detectMultiScale(frameGray, faces, 1.1, 2, 0, Size(20, 20), Size(100, 100));
	if (faces.size() == 0) return 0;
	else return 1;
}