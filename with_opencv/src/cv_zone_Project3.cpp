#include "cv_zone.h"

void cv_zone_faceDetection()
{
    // string path = "../peripheral/cv_zone/Resources/test.png";
    // Mat img     = imread(path);

    struct timeval start_time, end_time;
    uint32_t delta_time;
    get_current_time(&start_time);

    VideoCapture capture(0);

    CascadeClassifier faceCascade;
    faceCascade.load("../peripheral/cv_zone/Resources/haarcascade_frontalface_default.xml");
    if (faceCascade.empty()) {
        cout << "XML file not loaded" << endl;
    }
    Mat frame;
    vector<Rect> faces;
    while (1) {
        capture >> frame;
        faceCascade.detectMultiScale(frame, faces, 1.1, 10);
        for (int i = 0; i < faces.size(); i++) {
            rectangle(frame, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
        }
        imshow("Image", frame);
        waitKey(1);

        /* Time controller */
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

void cv_zone_licenseRecognition()
{
    cout << "cv_zone_licenseRecognition" << endl;
    // 读取图像
    VideoCapture capture(0);

    struct timeval start_time, end_time;
    uint32_t delta_time;
    get_current_time(&start_time);

    CascadeClassifier plateCascade;
    plateCascade.load("../peripheral/cv_zone/Resources/haarcascade_russian_plate_number.xml");
    if (plateCascade.empty()) {
        cout << "XML file not loaded" << endl;
    }
    vector<Rect> plate;

    while (1) {
        Mat frame;
        capture >> frame;
        plateCascade.detectMultiScale(frame, plate, 1.1, 4);

        for (int i = 0; i < plate.size(); i++) {
            Mat imgCrop = frame(plate[i]);
            imshow(to_string(i), imgCrop);
            imwrite("../peripheral/cv_zone/Resources/Plates/" + to_string(i) + ".png", imgCrop);
            rectangle(frame, plate[i].tl(), plate[i].br(), Scalar(255, 0, 255), 3);
        }

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