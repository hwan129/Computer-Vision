#include "opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;
int main() {
    string query_address = "query_image/", query_name;
    vector<String> path;
    Mat query, dbs_img, descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    Mat imgMatches;		// �����
    float nndr;
    int j, k;

    cout << "Enter query image name : ";
    cin >> query_name;
    query_address = query_address + query_name;
    query = imread(query_address);  // �Է� ���� �̹���
    resize(query, query, Size(640, 480));

    if (query.empty()) {
        cout << "No file!" << endl;
        return -1;
    }

    glob("DBs/*.jpg", path, false);
    if (path.size() == 0) {
        cout << "No file!" << endl;
    }
    cout << endl << "Sample Image Load Size : " << path.size() << endl;

    orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);		// ORB�� ����Ͽ� �̹������� keypoint ����, �� ����Ʈ�� descriptor ����.

    for (int i = 0; i < path.size(); i++) {
        BFMatcher matcher(NORM_HAMMING);
        vector<vector<DMatch>> matches;
        vector<DMatch> goodMatches;

        dbs_img = imread(path[i]);
        resize(dbs_img, dbs_img, Size(640, 480));

        orbF->detectAndCompute(dbs_img, noArray(), keypoints2, descriptors2);

        k = 2;
        matcher.knnMatch(descriptors1, descriptors2, matches, k);

        nndr = 0.6f;
        for (j = 0; j < matches.size(); j++) {		// nndr ����. ��ȿ�� ��ġ ã��. �Ÿ� ������ nndr���� ���� ��� ��ȿ. goodMatches�� ����.
            if (matches.at(j).size() == 2 && matches.at(j).at(0).distance <= nndr * matches.at(j).at(1).distance) {
                goodMatches.push_back(matches[j][0]);
            }
        }

        cout << "Image Number : " << i+1;
        cout << " Matching : " << goodMatches.size() << endl;
        if (goodMatches.size() < 4) {		// ��ȿ�� ��ġ�� 4���� ���� ��� ����
        }
        else {
            drawMatches(query, keypoints1, dbs_img, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        }
    }

    if (imgMatches.empty()) {
        cout << "No match found!" << endl;
        return -1;
    }
    else {
        cout << "Match found!" << endl;
    }

    imshow("Query", query);
    imshow("Best Matching", imgMatches);
    waitKey(0);

    return 0;
}
