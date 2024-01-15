#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat query, image, descriptors1, descriptors2;
	Ptr<ORB> orbF = ORB::create(1000);		// ORB 객체 생성. 검출, 추출. keypoint 최대수 : 1000
	vector<KeyPoint> keypoints1, keypoints2;	// keypoint 저장
	vector<vector<DMatch>> matches;			// 이미지간 매치 결과 
	vector<DMatch> goodMatches;			// 유효한 매치 결과 
	BFMatcher matcher(NORM_HAMMING);		// 해밍 거리를 사용하여 특징 descriptor 간의 매칭을 수행하기 위해 BFMatcher 객체 생성
	Mat imgMatches;		// 결과물

	int i, k;
	float nndr;

	query = imread("query.jpg");
	image = imread("input.jpg");
	resize(query, query, Size(640, 480));
	resize(image, image, Size(640, 480));

	if (query.empty() || image.empty()) return -1;

	// Compute ORB Features
	orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);		// ORB를 사용하여 이미지에서 keypoint 검출, 각 포인트의 descriptor 추출.
	orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);

	// Find best and second-best matches
	k = 2;
	matcher.knnMatch(descriptors1, descriptors2, matches, k);		// 해밍거리를 이용하여 이미지 간의 특징 descriptor 매칭. k: 첫번째와 두번째로 좋은 매치를 찾는데 사용

	// Find out the best match is definitely better than the second-best match
	nndr = 0.4f;
	for (i = 0; i < matches.size(); i++) {		// nndr 설정. 유효한 매치 찾기. 거리 비율이 nndr보다 작을 경우 유효. goodMatches에 저장.
		if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
			goodMatches.push_back(matches[i][0]);
		}
	}

	// Draws the found matches of keypoints from two images. 매칭된 keypoint들을 시각화 하고 imgMatches에 저장
	drawMatches(query, keypoints1, image, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	
	if (goodMatches.size() < 4) {		// 유효한 매치가 4보다 작을 경우 실패
		cout << "Matching failed" << endl;
		return 0;
	}

	imshow("imgMatches", imgMatches);
	waitKey(0);
}