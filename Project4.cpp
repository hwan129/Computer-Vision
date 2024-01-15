#include <opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat panorama(Mat img1, Mat img2) {
    Mat gray1, gray2;
    cvtColor(img1, gray1, COLOR_BGR2GRAY);
    cvtColor(img2, gray2, COLOR_BGR2GRAY);

    // �̹������� Ű����Ʈ�� ��ũ���� ����
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    orbF->detectAndCompute(gray1, noArray(), keypoints1, descriptors1);
    orbF->detectAndCompute(gray2, noArray(), keypoints2, descriptors2);

    // ��ũ���� ��Ī
    BFMatcher matcher(NORM_HAMMING);
    vector<vector<DMatch>> matches;
    vector<Point2f> goodMatches1, goodMatches2;
    int k = 2;
    matcher.knnMatch(descriptors1, descriptors2, matches, k);

    // ���� ��ġ ����. 
    // �������� ��� nndr = 0.7 ����
    // ���� ��� nndr = 0.8 ����
    float nndr = 0.7f;
    for (int j = 0; j < matches.size(); j++) {		// nndr ����. ��ȿ�� ��ġ ã��. �Ÿ� ������ nndr���� ���� ��� ��ȿ. goodMatches�� ����.
        if (matches.at(j).size() == 2 && matches.at(j).at(0).distance <= nndr * matches.at(j).at(1).distance) {
            goodMatches1.push_back(keypoints1[matches[j][0].queryIdx].pt);
            goodMatches2.push_back(keypoints2[matches[j][0].trainIdx].pt);
        }
    }

    // ȣ��׷��� ���
    Mat H = findHomography(goodMatches2, goodMatches1, RANSAC);

    // img2�� img1�� ��ǥ �������� ��ȯ
    Mat result;
    warpPerspective(img2, result, H, Size(img1.cols*3, img1.rows*1.5));

    // �̹��� ��ġ��
    Mat roi_img2(result, Rect(0, 0, img1.cols, img1.rows));
    img1.copyTo(roi_img2);

    return result;
}

int main() {
    Mat img1 = imread("pano1.jpg");
    Mat img2 = imread("pano2.jpg");
    Mat img3 = imread("pano3.jpg");
    Mat img4 = imread("pano4.jpg");

    if (img1.empty() || img2.empty() || img3.empty() || img4.empty()) {
        cout << "�̹����� �ҷ��� �� �����ϴ�." << endl;
        return -1;
    }

    // �������� ���. �̹����� �ư� �ٸ��� ����. 
    // ���� ��� 4���� resize �ּ� ó�� ��, nndr = 0.8�� ���
    // �������� ��� ���� �ڵ� ���. ȭ���� ����. nndr = 0.7 ���
    resize(img1, img1, Size(640, 480));
    resize(img2, img2, Size(640, 480));
    resize(img3, img3, Size(640, 480));
    resize(img4, img4, Size(640, 480));

    Mat result = panorama(img3, img4);
    result = panorama(img2, result);
    result = panorama(img1, result);
    imshow("Result", result);
    waitKey(0);

    return 0;
}