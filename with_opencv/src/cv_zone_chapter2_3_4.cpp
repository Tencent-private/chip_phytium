#include "cv_zone.h"

/**
 * @brief dilate and erode
*/
void cv_zone_basic()
{
    /* 创建一个图像显示的线程 */
    imageDisplayThead.startShowImages();

    string path = "../peripheral/cv_zone/Resources/test.png";
    Mat img     = imread(path);
    Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

    /* 改变灰度值，BGR -> GRAY ,彩色 到 灰色 */
    cvtColor(img, imgGray, COLOR_BGR2GRAY);

    // 定义高斯模糊函数，参数分别为输入图像、输出图像、高斯核大小、sigma值、截断值
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);

    // 利用Canny算法提取图像边缘
    Canny(imgBlur, imgCanny, 25, 75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    // 膨胀操作
    // Dilate the image using the given kernel
    dilate(imgCanny, imgDil, kernel);
    // 腐蚀操作
    // Erodes image using the given kernel
    erode(imgDil, imgErode, kernel);

    /* 创建新的通道来 显示图片 */
    imageDisplayThead.addImage("img", img);
    // imageDisplayThead.addImage("img_gray", imgGray);
    // imageDisplayThead.addImage("img_blur", imgBlur);
    imageDisplayThead.addImage("img_canny", imgCanny);
    imageDisplayThead.addImage("img_dil", imgDil);
    imageDisplayThead.addImage("img_erode", imgErode);
    cin.get();
    imageDisplayThead.closeShowImages();
}

/**
 * @brief resize, crop
*/
void cv_zone_intermediate()
{
    imageDisplayThead.startShowImages();
    string path = "../peripheral/cv_zone/Resources/test.png";
    Mat img     = imread(path);
    Mat imgResize, imgRect;

    cout << "img.size() = " << img.size() << endl; //img.size() = [768 x 559]
    /* 缩放到指定的大小 */
    // resize(img, imgResize, Size(640, 480));
    /* 缩放指定的比例 */
    resize(img, imgResize, Size(), 0.5, 0.5);
    cout << "imgResize.size() = " << imgResize.size() << endl;

    /* 定义裁剪核， 对图像进行裁剪 */
    Rect roi(200, 100, 300, 300);
    imgRect = img(roi);

    imageDisplayThead.addImage("img", img);
    // imageDisplayThead.addImage("imgResize", imgResize);
    imageDisplayThead.addImage("imgRect", imgRect);

    cin.get();
    imageDisplayThead.closeShowImages();
}

/**
 * @brief draw shapes and text
*/
void cv_zone_advanced()
{
    imageDisplayThead.startShowImages();
    /* black image */
    Mat img(512, 512, CV_8UC3, Scalar(255, 253, 252));

    /* draw circle */
    /* 在哪张图形上画，圆心位置，半径，边界颜色，边框粗细 */
    circle(img, Point(256, 256), 300, Scalar(0, 0, 255), 2); //FILLED 实心
    // circle(img, Point(256, 256), 100, Scalar(0, 255, 0), 2, LINE_8); //LINE_8 八边形

    /* draw rectangle */
    rectangle(img, Point(100, 100), Point(400, 400), Scalar(0, 255, 0), FILLED);

    /* draw line */
    line(img, Point(100, 100), Point(400, 100), Scalar(255, 0, 0), 2);

    /* draw text */
    putText(img, "Hello OpenCV", Point(100, 100), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 2);

    imageDisplayThead.addImage("img", img);

    cin.get();
    imageDisplayThead.closeShowImages();
}
