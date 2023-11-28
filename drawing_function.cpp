#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat drawHistogram(Mat src) {
	Mat hist, histImage;
	int i, hist_w, hist_h, bin_w, histSize;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	hist_w = 512;
	hist_h = 400;
	histSize = 256;
	bin_w = cvRound((double)hist_w / histSize);

	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));	// draw the histograms. rect
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);		// compute the histograms
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (i = 0; i < histSize; i++) {
		rectangle(histImage, Point(bin_w * i, hist_h), Point(bin_w * i + hist_w / histSize, hist_h - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), -1);
	}

	return histImage;
}

int main()
{
	Mat image = imread("lena.png", 0);
	Mat rect_img = image.clone();
	Mat line_img = image.clone();
	Mat trapezoid_img = Mat::zeros(400, 400, CV_8UC3);
	Mat write_img = Mat::zeros(400, 600, CV_8UC3);

	
	// rect
	Rect rect = Rect(10, 10, 100, 100); //LT position, width, height
	rectangle(rect_img, rect, Scalar(0, 255, 0), 4, 8, 0);		// img, rect, color, thickness, lineType, shift

	// line
	Point p1(25, 25), p2(100, 50);
	line(line_img, p1, p2, Scalar(255, 0, 0), 3, 8, 0);			// img, pt1, pt2, color, thickness, lineType, shift

	//trapezoid
	int w = 400;
	Point trapezoid[1][4];
	trapezoid[0][0] = Point(w * 2 / 6, w / 4);
	trapezoid[0][1] = Point(w * 4 / 6, w / 4);
	trapezoid[0][2] = Point(w * 5 / 6, w * 3 / 4);
	trapezoid[0][3] = Point(w / 6, w * 3 / 4);

	const Point* ppt[1] = { trapezoid[0] };
	int npt[] = { 4 };

	fillPoly(trapezoid_img, ppt, npt, 1, Scalar(255, 255, 255), 8);		// img, pts(꼭지점의 위치 2차원 배열), npts(꼭지점의 개수 1차원 배열), ncontours(둘러싸고 있는 둘레 개수), color, lineType, shift, offset

	// write
	int wi = write_img.cols;
	int he = write_img.rows;
	putText(write_img, format("width: %d, height: %d", wi, he),		// printf 식의 String 리턴
		Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,
		Scalar(0, 200, 200), 4);				// img, text, org(위치), fontFace, fontScale, bottomLeftOrigin

	// histogram
	Mat hist_graph = drawHistogram(image);

	imshow("histogram", hist_graph);
	imshow("write", write_img);
	imshow("trapezoid", trapezoid_img);
	imshow("line", line_img);
	imshow("rect", rect_img);

	waitKey(0);
	return 0;
}
