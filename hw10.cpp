#include <opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void quantize(Mat& matrix, Mat quantize_mat) {
	for (int i = 0; i < matrix.rows; i += 8) {
		for (int j = 0; j < matrix.cols; j += 8) {
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {
					matrix.at<float>(i + x, j + y) = round(matrix.at<float>(i + x, j + y) / quantize_mat.at<double>(x, y));
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {
	Mat image;
	Mat image_Ycbcr;
	Mat Ycbcr_channels[3];
	Mat y(512, 512, CV_8UC1);	// 채널의 밝기 정보
	Mat quantization_mat1 = (Mat_<double>(8, 8) <<
		16, 11, 10, 16, 24, 40, 51, 61,
		12, 12, 14, 19, 26, 58, 60, 55,
		14, 13, 16, 24, 40, 57, 69, 56,
		14, 17, 22, 29, 51, 87, 80, 62,
		18, 22, 37, 56, 68, 109, 103, 77,
		24, 35, 55, 64, 81, 104, 113, 92,
		49, 64, 78, 87, 103, 121, 120, 101,
		72, 92, 95, 98, 112, 100, 103, 99
		);
	Mat quantization_mat2 = (Mat_<double>(8, 8) <<
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1
		);
	Mat quantization_mat3 = (Mat_<double>(8, 8) << 
		100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100
		);

	cout << "1" << endl;
	
	image = imread("lena.png", IMREAD_COLOR);
	cvtColor(image, image_Ycbcr, CV_BGR2YCrCb);		// 이미지 변환
	split(image_Ycbcr, Ycbcr_channels);	// 채널 분리

	for (int j = 0; j < 512; j++) {
		for (int i = 0; i < 512; i++) {
			y.at<uchar>(j, i) = 0;
			y.at<uchar>(j, i) = Ycbcr_channels[0].at<uchar>(j, i);
		 }
	}

	quantize(y, quantization_mat1);


	imshow("Original Y", y);
	imshow("QM1", quantization_mat1);



	waitKey(0);
	return 0;
}
