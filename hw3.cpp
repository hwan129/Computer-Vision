#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat moon = imread("moon.jpeg", 0);
	Mat total, copied, copied2;
	Mat mask = Mat::zeros(moon.size(), moon.type());	// 빈 이미지 생성
	Mat mask2 = Mat::zeros(moon.size(), moon.type());	// 빈 이미지 생성

	Mat blur_filter, unsharp_mask, sharpening_filter;

	blur(moon, blur_filter, Size(3, 3));
	unsharp_mask = moon - blur_filter;	// 원본 이미지에서 블러 이미지를 뺌
	sharpening_filter = moon + 2.5 * unsharp_mask;	// 원본과 unsharp이미지를 더해서 sharpen게 만듬

	Rect rect_left = Rect(0, 0, moon.size().width / 2, moon.size().height);	// 이미지의 왼
	rectangle(mask, rect_left, Scalar(255, 255, 255), -1, 8, 0);	// 마스크 생성
	moon.copyTo(copied, mask);	// 마스크 결합
	Rect rect_right = Rect(moon.size().width / 2, 0, moon.size().width / 2, moon.size().height); // 이미지의 오른
	rectangle(mask2, rect_right, Scalar(255, 255, 255), -1, 8, 0);
	sharpening_filter.copyTo(copied2, mask2);

	total = copied + copied2;	// 이미지 결합
	
	imshow("moon", moon);
	imshow("moon_filtered", total);

	Mat saltnpepper = imread("saltnpepper.png", 0);
	Mat median_filter;
	mask = Mat::zeros(saltnpepper.size(), saltnpepper.type());
	mask2 = Mat::zeros(saltnpepper.size(), saltnpepper.type());

	medianBlur(saltnpepper, median_filter, 9);	//중간값 필터

	rect_left = Rect(0, 0, saltnpepper.size().width / 2, saltnpepper.size().height);
	rectangle(mask, rect_left, Scalar(255, 255, 255), -1, 8, 0);
	median_filter.copyTo(copied, mask);
	rect_right = Rect(saltnpepper.size().width / 2, 0, saltnpepper.size().width / 2, saltnpepper.size().height);
	rectangle(mask2, rect_right, Scalar(255, 255, 255), -1, 8, 0);
	saltnpepper.copyTo(copied2, mask2);

	total = copied + copied2;

	imshow("saltnpepper", saltnpepper);
	imshow("saltnpepper_filtered", total);

	waitKey(0);
	return 0;
}

/*#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat moon = imread("moon.jpeg", 0);
	Mat saltnpepper = imread("saltnpepper.png", 0);
	Mat gray_moon, abs_moon, filtered_moon, sharp_moon;

	if (moon.empty() || saltnpepper.empty()) {
		cout << "Could not open or find the image!" << endl;
		return -1;
	}

	// moon. 오른쪽만 필터
	GaussianBlur(moon, moon, Size(3, 3), 0, 0, BORDER_DEFAULT);			// unsharp masking

	Laplacian(moon, gray_moon, CV_16S, 1, 1, 0);		// filtered

	convertScaleAbs(gray_moon, abs_moon);		// 결과를 절대값으로
	
	add(abs_moon, moon, filtered_moon);			// 전체가 필터
	sharp_moon = moon.clone();					// 흐린 이미지
	
	for (int y = 0; y < moon.rows; y++) {		// filtered_moon의 오른쪽만 sharp_moon에 씌우기
		for (int x = moon.cols / 2; x < moon.cols; x++) {
			sharp_moon.at<uchar>(y, x) = filtered_moon.at<uchar>(y, x);
		}
	}
	//Rect roiRect(moon.cols / 2, 0, moon.cols / 2, moon.rows);
	//Mat roi_sharp_moon = sharp_moon(roiRect);
	//Mat roi_filtered_moon = filtered_moon(roiRect);

	//roi_filtered_moon.copyTo(roi_sharp_moon);
	

	// salt and pepper. 왼쪽만 필터
	Mat saltnpepper_filtered, salt_result;

	medianBlur(saltnpepper, saltnpepper_filtered, 9);

	salt_result = saltnpepper.clone();					// 원본 복사
	for (int y = 0; y < saltnpepper.rows; y++) {		// saltnpepper의 왼쪽만 필터 씌우기
		for (int x = 0; x < saltnpepper.cols / 2; x++) {
			salt_result.at<uchar>(y, x) = saltnpepper_filtered.at<uchar>(y, x);
		}
	}
	
	imshow("moon", moon);
	imshow("moon_filtered", sharp_moon);
	imshow("saltnpepper", saltnpepper);
	imshow("saltnpepper_filtered", salt_result);

	waitKey(0);
	return 0;

}*/