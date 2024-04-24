#include "cv_zone.h"

/**
  * @brief  wrap images
  * @note 首先获取目标物体的变换矩阵，再带入图像获得 图像变换后的图像
  * 大约是 变换矩阵为一个 函数 ，先带入特殊点得到 函数具体表达式，最后再将图像进行变换
 */
void cv_zone_wrap()
{
    /* width and high has been measured */
    float width = 250;
    float high  = 350;
    Mat matrix, imgWarp;

    imageDisplayThead.startShowImages();
    string path = "../peripheral/cv_zone/Resources/cards.jpg";
    Mat img     = imread(path);

    Point2f src[4] = {{529, 142}, {771, 190}, {405, 395}, {674, 457}};
    Point2f dst[4] = {{0.0f, 0.0f}, {width, 0.0f}, {0.0f, high}, {width, high}};

    // This function returns the inverse of the perspective transformation matrix
    matrix = getPerspectiveTransform(src, dst);
    // Warp the image using the perspective transformation matrix
    warpPerspective(img, imgWarp, matrix, Point(width, high));

    /* label the selected Points */
    for (int i = 0; i < 4; i++) {
        circle(img, src[i], 5, Scalar(255, 0, 255), FILLED);
    }

    imageDisplayThead.addImage("img", img);
    imageDisplayThead.addImage("imgWarp", imgWarp);
    cin.get();
    imageDisplayThead.closeShowImages();
}

/**
 * @brief  color detection
 * @note  select the same color in a photo
*/
void cv_zone_color()
{
    Mat imgHSV, mask;
    int hmin = 0, smin = 110, vmin = 153;
    int hmax = 19, smax = 240, vmax = 255;
    bool windowButton = true;

    imageDisplayThead.startShowImages();
    string path = "../peripheral/cv_zone/Resources/lambo.png";
    Mat img     = imread(path);

    cvtColor(img, imgHSV, COLOR_BGR2HSV);

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
        // Set the lower bound of the range
        Scalar lower(hmin, smin, vmin);
        // Set the upper bound of the range
        Scalar upper(hmax, smax, vmax);
        /* create a mask */
        // Check if the pixel values are within the range and set them to 0
        inRange(imgHSV, lower, upper, mask);

        imshow("img", img);
        imshow("imgHSV", imgHSV);
        imshow("mask", mask);
        waitKey(1);
    }
}

/**
 * @brief  get Contours
 * @param imgDil : to find all contours
 * @param image : painting on it 
 * @note  1. findContours() can find all shapes 
 *        2. contourArea() can get the area of the shape , to filter the shape
 *        3. approxPolyDP() can reduce the number of vertices to approximate the shape
 *        4. boundingRect() can get the rectangle of the shape
 * @note  1. 轮廓检测,找到所有的形状，并保存为一个轮廓
 *        2. 轮廓面积，用来过滤掉一些小的轮廓
 *        3. 轮廓拟合，减少顶点数来逼近轮廓形状
 *        4. 轮廓属性，寻找最小外接矩形
*/
static void getContours(Mat imgDil, Mat img)
{
    /* every contour is a shape */
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    /* the function need baidu */
    /* find all shapes and save one as a contour */
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // drawContours(img, contours, -1, Scalar(255, 0, 255), 5);

    /* 保存简化后的 轮廓模型，储存顶点*/
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    string objectType;

    for (int i = 0; i < contours.size(); i++) {
        int area = contourArea(contours[i]);

        cout << area << endl;
        /* filter */
        if (area > 1000) {
            /* 计算轮廓的 周长 */
            float peri = arcLength(contours[i], true);
            /* 减少定点数来逼近轮廓形状 */
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            cout << conPoly[i].size() << endl;
            /* 寻找最小外接矩形 */
            /* return para: x , y , width , heigh 
             * (x,y) 为最左上角的点坐标 */
            boundRect[i] = boundingRect(conPoly[i]);

            float aspRatio = 1;
            int objCor     = (int)conPoly[i].size();
            switch (objCor) {
            case 3:
                objectType = "Tri";
                break;
            case 4:
                /* 判断是否为 正方形 */
                aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
                cout << aspRatio << endl;
                if (aspRatio > 0.95 && aspRatio < 1.05) {
                    objectType = "Square";
                    break;
                } else {
                    objectType = "Rect";
                    break;
                }
            default:
                objectType = "Circle";
                break;
            }
            drawContours(img, contours, i, Scalar(255, 0, 255), 5);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
            putText(img, objectType, {boundRect[i].x, boundRect[i].y - 5}, FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 69, 255), 1);
        }
    }
}

/**
 * @brief  shape detection
 * @note  select simple in photo and circle it with boundRect
*/
void cv_zone_shape()
{
    imageDisplayThead.startShowImages();
    string path = "../peripheral/cv_zone/Resources/shapes.png";
    Mat img     = imread(path);
    Mat imgGray, imgCanny, imgBlur, imgDil;

    /* photo preprocessing  */
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgDil, kernel);

    getContours(imgDil, img);

    imshow("img", img);

    waitKey(0);
}
