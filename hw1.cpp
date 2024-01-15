#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat frame;
	VideoCapture cap;

	if (cap.open("background.mp4") == 0) return -1;

	double fps = cap.get(CAP_PROP_FPS);	// 비디오 프레임 속도 가져오기
	double time_in_msec = 0;	// 비디오의 현재 시간
	int curr_frame = 0;	// 현재 프레임 번호
	int total_frames = cap.get(CAP_PROP_FRAME_COUNT);	// 비디오 전체 프레임 수

	// video stops after 3 sec
	while (time_in_msec < 3000) {	// 3초간 재생
		cap >> frame;
		if (frame.empty()) {
			break;
		}

		time_in_msec = cap.get(CAP_PROP_POS_MSEC);	// 현재 재생 시간
		curr_frame = cap.get(CAP_PROP_POS_FRAMES);	// 현재 프레임 번호

		//printing current frames over total frames
		cout << "frames: " << curr_frame << " / " << total_frames << endl;
		imshow("video", frame);

		// calculating the right delay from given fps
		waitKey(1000 / fps);	// 1000밀리초 = 1초. fps가 초당이기 때문
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

	fps = cap.get(CAP_PROP_FPS);	// 비디오의 프레임 속도를 가져와 fps에 저장
	delay = 1000 / fps;		// 각 프레임을 화면에 표시하는데 걸리는 시간을 계산하여 delay 변수에 저장. 이렇게 함으로써 비디오가 원래 속도로 재생

	while (1) {
		cap >> frame;	// 비디오 캡처 객체 cap에서 다음 프레임을 읽어와 frame 변수에 저장
		if (frame.empty()) {		// 비디오의 끝에 도달하면 종료
			cout << "end of video" << endl;
			break;
		}

		imshow("video", frame);		// imshow 함수를 이용하여 frame 변수에 저장된 프레임을 video라는 창에 표시
		waitKey(delay);		// 프레임을 화면에 표시한 후, 'delay' 시간만큼 대기.
	}
}*/