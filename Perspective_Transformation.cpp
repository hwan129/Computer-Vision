#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

struct MouseParams {	// ���콺 �̺�Ʈ
	Mat img;	// �Է� �̹���
	vector<Point2f> in, out;	// ������ �� ����, ��� �̹��� ���� ��
};

static void onMouse(int event, int x, int y, int, void* param) {	// ���콺 �̺�Ʈ
	MouseParams* mp = (MouseParams*)param;
	Mat img = mp->img;
	if (event == EVENT_LBUTTONDOWN) { // ���� ��ư
		Mat result;

		mp->in.push_back(Point2f(x, y));	// Ŭ���� ��ǥ�� mp->n�� ����


		if (mp->in.size() == 4) {	// Ŭ���� ��ǥ�� 4���� �ȴٸ�,
			Mat homo_mat = getPerspectiveTransform(mp->in, mp->out);	// 4���� �Է� ���� ��� ���� ���� ��ȯ ��� 
			warpPerspective(img, result, homo_mat, Size(300, 300));		// ����� �̿��Ͽ� �̹��� ���� ��ȯ
			imshow("output", result);	// ���
		}
		else {
			result = img.clone();
			for (size_t i = 0; i < mp->in.size(); i++) {
				circle(result, mp->in[i], 3, Scalar(0, 0, 255), 5);
			}
			imshow("input", result);
		}
	}

	if (event == EVENT_RBUTTONDOWN) {	// ������ ��ư
		mp->in.clear();		// �Է��� ������ �ʱ�ȭ
		imshow("input", img);
	}
}

int main() {
	Mat imput = imread("book.jpg");
	imshow("input", imput);

	MouseParams mp;		// ����ü
	mp.out.push_back(Point2f(0, 0));	// �� ������ ��ġ ����. 2D����Ʈ
	mp.out.push_back(Point2f(300, 0));
	mp.out.push_back(Point2f(300, 300));
	mp.out.push_back(Point2f(0, 300));
	mp.img = imput;

	setMouseCallback("input", onMouse, (void*)&mp);		// ���콺 �̺�Ʈ�� �߻��ϸ� onMouse�� �̵�
	waitKey();
	return 0;
}