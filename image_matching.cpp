#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat query, image, descriptors1, descriptors2;
	Ptr<ORB> orbF = ORB::create(1000);		// ORB ��ü ����. ����, ����. keypoint �ִ�� : 1000
	vector<KeyPoint> keypoints1, keypoints2;	// keypoint ����
	vector<vector<DMatch>> matches;			// �̹����� ��ġ ��� 
	vector<DMatch> goodMatches;			// ��ȿ�� ��ġ ��� 
	BFMatcher matcher(NORM_HAMMING);		// �ع� �Ÿ��� ����Ͽ� Ư¡ descriptor ���� ��Ī�� �����ϱ� ���� BFMatcher ��ü ����
	Mat imgMatches;		// �����

	int i, k;
	float nndr;

	query = imread("query.jpg");
	image = imread("input.jpg");
	resize(query, query, Size(640, 480));
	resize(image, image, Size(640, 480));

	if (query.empty() || image.empty()) return -1;

	// Compute ORB Features
	orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);		// ORB�� ����Ͽ� �̹������� keypoint ����, �� ����Ʈ�� descriptor ����.
	orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);

	// Find best and second-best matches
	k = 2;
	matcher.knnMatch(descriptors1, descriptors2, matches, k);		// �عְŸ��� �̿��Ͽ� �̹��� ���� Ư¡ descriptor ��Ī. k: ù��°�� �ι�°�� ���� ��ġ�� ã�µ� ���

	// Find out the best match is definitely better than the second-best match
	nndr = 0.4f;
	for (i = 0; i < matches.size(); i++) {		// nndr ����. ��ȿ�� ��ġ ã��. �Ÿ� ������ nndr���� ���� ��� ��ȿ. goodMatches�� ����.
		if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
			goodMatches.push_back(matches[i][0]);
		}
	}

	// Draws the found matches of keypoints from two images. ��Ī�� keypoint���� �ð�ȭ �ϰ� imgMatches�� ����
	drawMatches(query, keypoints1, image, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	
	if (goodMatches.size() < 4) {		// ��ȿ�� ��ġ�� 4���� ���� ��� ����
		cout << "Matching failed" << endl;
		return 0;
	}

	imshow("imgMatches", imgMatches);
	waitKey(0);
}