#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	// Averaging filter
	Mat image, AvgImg, GaussianImg;
	image = imread("lena.png");

	blur(image, AvgImg, Size(10, 10));

	GaussianBlur(image, GaussianImg, Size(5, 5), 1.5);

	

	// Sharpening using second derivative
	Mat moonImg, laplacian, laplacian2, abs_lapacian, sharpening;
	moonImg = imread("Moon.jpeg", 0);

	GaussianBlur(moonImg, moonImg, Size(3, 3), 0, 0, BORDER_DEFAULT);

	Laplacian(moonImg, laplacian, CV_16S, 1, 1, 0);
	Laplacian(moonImg, laplacian2, CV_16S, 5, 5, 5);

	convertScaleAbs(laplacian, abs_lapacian);

	sharpening = abs_lapacian + moonImg;


	//Median filter
	Mat saltImg = imread("saltnpepper.png", 0);
	imshow("SaltAndPepper", saltImg);
	Mat mf1, mf2;

	medianBlur(saltImg, mf1, 3);
	imshow("MedianFiltered 1", mf1);

	medianBlur(saltImg, mf2, 9);
	imshow("MedianFiltered 2", mf2);

	imshow("image", image);
	imshow("blur", AvgImg);
	imshow("Gaussian", GaussianImg);

	imshow("input", moonImg);
	imshow("Laplacian", laplacian);
	imshow("Laplacian 2", laplacian2);
	imshow("abs_Laplacian", abs_lapacian);
	imshow("Sharpening", sharpening);

	waitKey(0);
	return 0;
}