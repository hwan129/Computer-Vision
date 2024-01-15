#include <opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int isDetectFace(CascadeClassifier face_classifier, Mat frameGray);

/* 참고
* 3개 영상 동시에 열어서 동시에 Face Detection하니까 프로그램이 너무 무거워져서 1, 2, 3으로 따로 실행되게 했어요.
* 
* ROI를 위한 사각형은 rect1, rect2, rect3, rect4로 설정해놓았습니다.
* Broi는 background subtraction을 위한 건데, 만약 전체 이미지에서만 background가 달라지는 게 있는지 확인하신다면 지우셔도 괜찮을 것 같아요!
* is1, is2, is3, is4는 각 roi에서 얼굴이 검출됐을 때는 1을 반환하고, 얼굴 검출이 안 됐을 때는 0을 리턴합니다.
* 
* 
* cout << "is1: " << is1 << endl;
			cout << "is2: " << is2 << endl;
			cout << "is3: " << is3 << endl;
			cout << "is4: " << is4 << endl;
			cout << endl;
  case에 따른 while(1)문 안에 이 코드를 쓰시면
  몇 프레임 안에 몇 번 이상 face detection이 되면 사람이 있는 걸로 할지 판단할 때 확인이 쉬울 것 같아요!
  예시로 1번 case에 넣어 놓았어요!

  잠깐 확인해본 바로는 3번 roi가 앞 사람 머리에 가려서 잘 detection이 안 되던데
  밑에 rect3 가보시면 주석처리해놓은 게 있는데 이거 쓰면 편법이지만 코드 설정하시기에는 편하실 것 같아요.
*/

int main() {
	VideoCapture cap("Full.mp4"), cap2("Empty.mp4"), cap3("Half.mp4");
	Mat fullFrame, halfFrame, background, fullGray, halfGray, fullMask, halfMask;
	Mat emptyFrame, emptyGray, emptyMask; //Add to show the case of empty

	Rect rect1, rect2, rect3, rect4; //To set roi of each seats
	Mat Broi1, Broi2, Broi3, Broi4; //ROI for background subtraction <------------- 참고에서 설명한 부분
	Mat Froi1, Froi2, Froi3, Froi4; //ROI for face detection

	int whatdoyouwant;//For check the answer of what case do you want
	int is1 = 0, is2 = 0, is3 = 0, is4 = 0;//For check the Face Dection of each seats

	int fps = cap.get(CAP_PROP_FPS);
	int wait_time = 1000 / fps;

	//Face detector
	CascadeClassifier face_classifier;
	face_classifier.load("haarcascade_frontalface_alt.xml");

	//Set roi
	rect1 = Rect(70, 180, 120, 250); //왼쪽 아래
	rect2 = Rect(280, 200, 120, 250);//오른쪽 아래
	rect3 = Rect(40, 160, 80, 160);//왼쪽 위
	//rect3 = Rect(40, 160, 100, 160); <------------- 참고에서 설명한 부분
	rect4 = Rect(250, 160, 100, 160);//오른쪽 위

	//위에는 다 변수 설정
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

			/*Broi1 = fullMask(rect1); <------------- 참고에서 설명한 부분
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


			//Face detection 확인용
			cout << "is1: " << is1 << endl;
			cout << "is2: " << is2 << endl;
			cout << "is3: " << is3 << endl;
			cout << "is4: " << is4 << endl;
			cout << endl;

			imshow("Full", fullFrame);

			//ROI 확인하고 싶으실 떄
			//imshow("Roi1", Froi1);
			//imshow("Roi2", Froi2);
			//imshow("Roi3", Froi3);
			//imshow("Roi4", Froi4);

			//Background subtraction 한 ROI 확인하고 싶으실 때
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

			//ROI 확인하고 싶으실 떄
			imshow("Roi1", Froi1);
			imshow("Roi2", Froi2);
			imshow("Roi3", Froi3);
			imshow("Roi4", Froi4);

			//Background subtraction 한 ROI 확인하고 싶으실 때
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

			//ROI 확인하고 싶으실 떄
			//imshow("Roi1", Froi1);
			//imshow("Roi2", Froi2);
			//imshow("Roi3", Froi3);
			//imshow("Roi4", Froi4);

			//Background subtraction 한 ROI 확인하고 싶으실 때
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