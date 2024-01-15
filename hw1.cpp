#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat frame;
	VideoCapture cap;

	if (cap.open("background.mp4") == 0) return -1;

	double fps = cap.get(CAP_PROP_FPS);	// ���� ������ �ӵ� ��������
	double time_in_msec = 0;	// ������ ���� �ð�
	int curr_frame = 0;	// ���� ������ ��ȣ
	int total_frames = cap.get(CAP_PROP_FRAME_COUNT);	// ���� ��ü ������ ��

	// video stops after 3 sec
	while (time_in_msec < 3000) {	// 3�ʰ� ���
		cap >> frame;
		if (frame.empty()) {
			break;
		}

		time_in_msec = cap.get(CAP_PROP_POS_MSEC);	// ���� ��� �ð�
		curr_frame = cap.get(CAP_PROP_POS_FRAMES);	// ���� ������ ��ȣ

		//printing current frames over total frames
		cout << "frames: " << curr_frame << " / " << total_frames << endl;
		imshow("video", frame);

		// calculating the right delay from given fps
		waitKey(1000 / fps);	// 1000�и��� = 1��. fps�� �ʴ��̱� ����
	}
	waitKey(0);
	return 0;
}
// Mine
/*#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat frame;
	int fps;
	int delay;
	VideoCapture cap;

	if (cap.open("Road.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}

	fps = cap.get(CAP_PROP_FPS);	// ������ ������ �ӵ��� ������ fps�� ����
	delay = 1000 / fps;		// �� �������� ȭ�鿡 ǥ���ϴµ� �ɸ��� �ð��� ����Ͽ� delay ������ ����. �̷��� �����ν� ������ ���� �ӵ��� ���

	while (1) {
		cap >> frame;	// ���� ĸó ��ü cap���� ���� �������� �о�� frame ������ ����
		if (frame.empty()) {		// ������ ���� �����ϸ� ����
			cout << "end of video" << endl;
			break;
		}

		imshow("video", frame);		// imshow �Լ��� �̿��Ͽ� frame ������ ����� �������� video��� â�� ǥ��
		waitKey(delay);		// �������� ȭ�鿡 ǥ���� ��, 'delay' �ð���ŭ ���.
	}
}*/