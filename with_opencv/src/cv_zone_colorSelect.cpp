#include "cv_zone.h"

/**
 * @brief 颜色选择器
*/
void colorSelector()
{
    Mat img;
    Mat imgHSV, mask;
    int hmin = 0, smin = 110, vmin = 153;
    int hmax = 19, smax = 240, vmax = 255;
    bool windowButton = true;

    VideoCapture capture(0);
    /* create a trackbar */
    /* 可以滑动 改变从而找到 lower 与 upper 的值*/
    namedWindow("Trackbars", (640, 200));
    createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);

    while (windowButton) {
        capture >> img;
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        // Set the lower bound of the range
        Scalar lower(hmin, smin, vmin);
        // Set the upper bound of the range
        Scalar upper(hmax, smax, vmax);
        /* create a mask */
        // Check if the pixel values are within the range and set them to 0
        inRange(imgHSV, lower, upper, mask);
        cout << "hmin,smin,vim,hmax,smax,vmax" << endl
             << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl; // hmin,smin,vim,hmax,smax,vmax
        imshow("img", img);
        imshow("mask", mask);
        waitKey(1);
    }
}