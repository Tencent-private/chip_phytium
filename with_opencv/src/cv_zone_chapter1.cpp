#include "cv_zone.h"

/**
 * @brief show a picture
*/
void cv_zone_test()
{
    string path = "../peripheral/cv_zone/Resources/test.png";
    Mat img     = imread(path);
    imshow("Image", img);
    waitKey(0);
}

/** 
 * @brief play a video
*/
void cv_zone_vedio()
{

    string path = "../peripheral/cv_zone/Resources/test_video.mp4";
    VideoCapture cap(path);
    Mat img;

    while (true) {
        cap.read(img);
        imshow("Image", img);
        waitKey(20);
    }
}

/**
 * @brief get vedio from webcam
*/
void cv_zone_webcam()
{
    cout << "this is cv_zone_webcam function" << endl;
    /* the number "0" is the label of cam , 
    if you have many , you may change it */
    VideoCapture capture(0);
    struct timeval start_time, end_time;
    uint32_t delta_time;
    get_current_time(&start_time);
    while (1) {
        Mat frame;
        capture >> frame;
        imshow("读取视频帧", frame);
        waitKey(1);
        get_current_time(&end_time);
        delta_time = end_time.tv_sec - start_time.tv_sec;
        cout << "Time: " << delta_time << endl;
        if (delta_time >= WEB_CAM_TIME_SEC) {
            break;
        }
    }
    capture.release();
    destroyAllWindows();
    system("pause");
}
