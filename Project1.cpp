#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat negative_transformation(Mat img) {
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            img.at<uchar>(y, x) = 255 - img.at<uchar>(y, x);
        }
    }
    return img;
}
Mat gamma_transformation(Mat img) {
    img.convertTo(img, CV_32F);
    float gamma = 2.5;
    normalize(img, img, 0, 1.0, NORM_MINMAX);

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            img.at<float>(y, x) = pow(img.at<float>(y, x), gamma);
        }
    }
    normalize(img, img, 0, 255, NORM_MINMAX);
    convertScaleAbs(img, img, 1.0, 0);
    return img;
}
Mat white_space(Mat img) {
    Mat bgr_channels[3];
    split(img, bgr_channels);

    double avg;
    int sum, temp, i, j, c;

    for (c = 0; c < img.channels(); c++) {
        sum = 0;
        avg = 0.0f;
        for (i = 0; i < img.rows; i++) {
            for (j = 0; j < img.cols; j++) {
                temp = (128 / avg) * bgr_channels[c].at<uchar>(i, j);
                if (temp > 255) bgr_channels[c].at<uchar>(i, j) = 255;
                else bgr_channels[c].at<uchar>(i, j) = temp;
            }
        }
    }
    merge(bgr_channels, 3, img);
    return img;
}

int main() {
    Mat frame;
    VideoCapture cap;

    if (cap.open("Road.mp4") == 0) {
        cout << "no such file!" << endl;
        waitKey(0);
    }

    double fps = cap.get(CAP_PROP_FPS);
    double wait_time = 1000 / fps;
    int keyboard = -1;
    int status = 82;
    int prev_status = status;
    Mat copy_frame;
    Mat frame_HSV;
    Mat HSV_channels[3];

    while (1) {
        cap >> frame;
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }

        keyboard = waitKey(wait_time);
        if (keyboard != -1) {
            status = keyboard;
        }

        if (status == 78 || status == 110) {    // n
            cvtColor(frame, frame_HSV, CV_BGR2HSV);
            split(frame_HSV, HSV_channels);
            HSV_channels[2] = negative_transformation(HSV_channels[2]);
            merge(HSV_channels, 3, frame);
            cvtColor(frame, frame, CV_HSV2BGR);
        }
        else if (status == 71 || status == 103) {   // g
            cvtColor(frame, frame_HSV, CV_BGR2HSV);
            split(frame_HSV, HSV_channels);
            HSV_channels[2] = gamma_transformation(HSV_channels[2]);
            merge(HSV_channels, 3, frame);
            cvtColor(frame, frame, CV_HSV2BGR);
        }
        else if (status == 72 || status == 104) {   // h
            cvtColor(frame, frame_HSV, CV_BGR2HSV);
            split(frame_HSV, HSV_channels);
            equalizeHist(HSV_channels[2], HSV_channels[2]);
            merge(HSV_channels, 3, frame);
            cvtColor(frame, frame, CV_HSV2BGR);
        }
        else if (status == 83 || status == 115) {   // s
            cvtColor(frame, frame_HSV, CV_BGR2HSV);
            split(frame_HSV, HSV_channels);
            for (int y = 0; y < HSV_channels[0].rows; y++) {
                for (int x = 0; x < HSV_channels[0].cols; x++) {
                    if (HSV_channels[0].at<uchar>(y, x) <= 9 || HSV_channels[0].at<uchar>(y, x) >= 23) {
                        HSV_channels[1].at<uchar>(y, x) = 0;
                    }
                }
            }
            merge(HSV_channels, 3, frame);
            cvtColor(frame, frame, CV_HSV2BGR);
        }
        else if (status == 67 || status == 99) {    // c
            cvtColor(frame, frame_HSV, CV_BGR2HSV);
            split(frame_HSV, HSV_channels);
            for (int y = 0; y < HSV_channels[0].rows; y++) {
                for (int x = 0; x < HSV_channels[0].cols; x++) {
                    if (HSV_channels[0].at<uchar>(y, x) >= 129) {
                        HSV_channels[0].at<uchar>(y, x) = HSV_channels[0].at<uchar>(y, x) - 129;
                    }
                    else {
                        HSV_channels[0].at<char>(y, x) = HSV_channels[0].at<uchar>(y, x) + 50;
                    }
                }
            }
            merge(HSV_channels, 3, frame);
            cvtColor(frame, frame, CV_HSV2BGR);
        }
        else if (status == 65 || status == 97) {    // a
            blur(frame, frame, Size(9, 9));
        }
        else if (status == 87 || status == 119) {   // w
            frame = white_space(frame);
        }
        else if (status == 82 || status == 114) {   // r
            prev_status = status;
            imshow("video", frame);
            continue;
        }
        else if (status == 27) {    // esc
            break;
        }
        else {
            status = prev_status;
            continue;
        }
        prev_status = status;
        imshow("video", frame);
    }

}


/*
#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void white_balacing(Mat img) {
    Mat bgr_channels[3];			// bgr을 분리하여 저장
    split(img, bgr_channels);		// bgr로 분리

    double avg;
    int sum, temp, i, j, c;

    for (c = 0; c < img.channels(); c++) {
        sum = 0;
        avg = 0.0f;
        for (i = 0; i < img.rows; i++) {
            for (j = 0; j < img.cols; j++) {
                sum += bgr_channels[c].at<uchar>(i, j);		// 각 채널의 픽셀값을 합을 구함
            }
        }
        avg = sum / (img.rows * img.cols);		// 각 채널의 픽셀 평균값

        for (i = 0; i < img.rows; i++) {		// 이미지의 모든 픽셀에 대해 밸런스 조절.
            for (j = 0; j < img.cols; j++) {
                temp = (128 / avg) * bgr_channels[c].at<uchar>(i, j);		// 픽셀값에 (128 / avg)를 곱해줌
                if (temp > 255) {		// 픽셀값이 255가 넘을 경우 255로 고정
                    bgr_channels[c].at<uchar>(i, j) = 255;
                }
                else {					// 아닐경우 밸런스 조절된 값으로 지정
                    bgr_channels[c].at<uchar>(i, j) = temp;
                }
            }
        }
    }
    merge(bgr_channels, 3, img);		// 다시 bgr을 합쳐서 img에 저장
}

int main() {
    VideoCapture cap("video.mp4");
    if (!cap.isOpened()) {
        cerr << "Error: Couldn't open the video file." << endl;
        return -1;
    }

    Mat frame, original_frame;
    int key = 0;
    int num = 0;

    while (1) {
        cap >> frame;
        cap >> original_frame;
        if (frame.empty()) {
            break;
        }
        
        if (num == 0) {
            key = waitKey(cap.get(CAP_PROP_FPS));
            imshow("video", frame);
        }

        if (key == 110 || num == 110) { // 'n' Negative
            cvtColor(frame, frame, COLOR_BGR2HSV);
            for (int i = 0; i < frame.rows; i++) {
                for (int j = 0; j < frame.cols; j++) {
                    frame.at<Vec3b>(i, j)[2] = 255 - frame.at<Vec3b>(i, j)[2];
                }
            }
            cvtColor(frame, frame, COLOR_HSV2BGR);
            imshow("video", frame);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 110;
        }
        if (key == 103 || num == 103) { // 'g' Gamma
            float g = 2.5;
            Mat hsv_frame;
            cvtColor(frame, hsv_frame, COLOR_BGR2HSV);

            for (int i = 0; i < hsv_frame.rows; i++) {
                for (int j = 0; j < hsv_frame.cols; j++) {
                    Vec3b& pixel = hsv_frame.at<Vec3b>(i, j);
                    pixel[2] = saturate_cast<uchar>(pow((float)(pixel[2] / 255.0), g) * 255.0f);
                }
            }

            cvtColor(hsv_frame, frame, COLOR_HSV2BGR);

            imshow("video", frame);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 103;
        }
        if (key == 104 || num == 104) { // 'h' Histogram equalization

            Mat hsv_frame;
            cvtColor(frame, hsv_frame, COLOR_BGR2HSV);
            vector<Mat> channels;
            split(hsv_frame, channels);
            equalizeHist(channels[2], channels[2]);     // 명도 조절
            merge(channels, hsv_frame);
            cvtColor(hsv_frame, frame, COLOR_HSV2BGR);


            imshow("video", frame);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 104;
        }
        if (key == 115 || num == 115) { // 's' Color slicing
            Mat hsv_frame;
            cvtColor(frame, hsv_frame, COLOR_BGR2HSV);

            for (int i = 0; i < hsv_frame.rows; i++) {
                for (int j = 0; j < hsv_frame.cols; j++) {
                    int hue = hsv_frame.at<Vec3b>(i, j)[0];
                    if (hue < 9 || hue > 23) {
                        hsv_frame.at<Vec3b>(i, j)[1] = 0;   // 채도 0
                    }
                }
            }

            cvtColor(hsv_frame, frame, COLOR_HSV2BGR);

            imshow("video", frame);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 115;
        }
        if (key == 99 || num == 99) { // 'c' Color conversion
            cvtColor(frame, frame, COLOR_BGR2HSV);
            for (int i = 0; i < frame.rows; i++) {
                for (int j = 0; j < frame.cols; j++) {
                    if (frame.at<Vec3b>(i, j)[0] > 129) {
                        frame.at<Vec3b>(i, j)[0] -= 129;
                    }
                    else {
                        frame.at<Vec3b>(i, j)[0] += 50;
                    }
                }
            }
            cvtColor(frame, frame, COLOR_HSV2BGR);

            imshow("video", frame);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 99;
        }
        if (key == 97 || num == 97) { // 'a' Average filtering
            blur(frame, frame, Size(9, 9));

            imshow("video", frame);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 97;
        }
        if (key == 117 || num == 117) { // 'u' Sharpening
            
            Mat blurred, laplacian, abs_laplacian, sharpened;
            
            GaussianBlur(frame, blurred, Size(9, 9), 1.5);
            Laplacian(blurred, laplacian, CV_16S, 1.9, 1.9, 1.9);
            convertScaleAbs(laplacian, abs_laplacian);
            sharpened = abs_laplacian + frame;
           
            imshow("video", sharpened);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 117;
        }
        if (key == 119 || num == 119) { // 'w' White balancing
            
            white_balacing(frame);

            imshow("video", frame);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 119;
        }
        if (key == 114 || num == 0) { // 'r' Reset
            imshow("video", original_frame);
            key = waitKey(cap.get(CAP_PROP_FPS));
            num = 0;
        }
        
    }

    return 0;
}*/