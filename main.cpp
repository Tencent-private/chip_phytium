#include "main.h"

int main()
{
    /* If you use RUNNING_OPENCV=OFF , please add your code here . */

    cout << "Hello, Phytium OpenCV2!" << endl;
    // testDemo();
    // chh_reco();

#ifndef CLOSE_PHYTIUM_OPENCV
    /* If you use RUNNING_OPENCV=ON , please add your code here . */

    // veido_function();
    // digital_recognition();
    // cv_zone_basic();
    // cv_zone_intermediate();
    // cv_zone_advanced();
    // cv_zone_shape();
    // colorSelector();
    // cv_zone_PenPainter();
    // cv_zone_FileScan();
    cv_zone_licenseRecognition();
    // cv_zone_faceDetection();

#endif

    return 0;
}
