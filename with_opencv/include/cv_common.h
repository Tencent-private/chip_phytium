#ifndef CV_COMMON_H
#define CV_COMMON_H

#include "common.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

namespace cn {

using namespace std;
using namespace cv;

class TheadOfImage {
private:
    vector<Mat> images;         // 存放需要显示的图像，每一个Mat单独显示一个窗口
    vector<String> windowNames; // 与images对应的显示名字，每一个名字不能一样
    std::mutex vectorMutex;     // 线程锁，读写图片时加锁
    bool displayFlag;           // 控制是否显示图像
    bool threadFlag;            // 控制线程开关

public:
    TheadOfImage();

    int addImage(String windowName, Mat image, int idx = -1);
    void showImages(void);
    void startShowImages(void);
    void closeShowImages(void);
    int removeImage(int idx);
    int removeImage(String windowName);
    void setDisplayFlag(bool value);
};

} // namespace cn

extern cn::TheadOfImage imageDisplayThead; // 用于单独开一个线程用于显示图像的全家变量类

#endif // CV_COMMON_H
