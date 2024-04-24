#include "cv_zone.h"

/* Global Value */

Mat img;
vector<vector<int>> newPoints;

/* hmin,smin,vim,hmax,smax,vmax */
vector<vector<int>> myColors{
    {36, 86, 87, 125, 255, 255},    /* blue */
    {116, 109, 116, 179, 255, 255}, /* red */
};
vector<Scalar> myColorValues{
    {255, 0, 0}, /* blue*/
    {0, 0, 255}, /* red */
};

/**
 * @brief  get Contours
 * @param imgDil : to find all contours
 *        @param img : draw rectangle on it 
 * @note  1. findContours() can find all shapes 
 *        2. contourArea() can get the area of the shape , to filter the shape
 *        3. approxPolyDP() can reduce the number of vertices to approximate the shape
 *        4. boundingRect() can get the rectangle of the shape
 * @note  1. 轮廓检测,找到所有的形状，并保存为一个轮廓
 *        2. 轮廓面积，用来过滤掉一些小的轮廓
 *        3. 轮廓拟合，减少顶点数来逼近轮廓形状
 *        4. 轮廓属性，寻找最小外接矩形
*/
static Point getContours(Mat imgDil, Mat img)
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

    Point myPoint(0, 0);

    for (int i = 0; i < contours.size(); i++) {
        int area = contourArea(contours[i]);

        cout << area << endl;
        /* filter */
        if (area > 500) {
            /* 计算轮廓的 周长 */
            float peri = arcLength(contours[i], true);
            /* 减少定点数来逼近轮廓形状 */
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            cout << conPoly[i].size() << endl;
            /* 寻找最小外接矩形 */
            /* return para: x , y , width , heigh 
             * (x,y) 为最左上角的点坐标 */
            boundRect[i] = boundingRect(conPoly[i]);
            myPoint.x    = boundRect[i].x + boundRect[i].width / 2;
            myPoint.y    = boundRect[i].y;

            drawContours(img, conPoly, i, Scalar(255, 0, 255), 5);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
        }
    }
    return myPoint;
}

/**
 * @brief  find color
 * @param img : to get the imgHSV
 * @param myColors : it contains the pre-measured color ranges in the camera, which can be used as a basis to select the appropriate mask.
 * @param myColorValues : it contains the color you want to paint , it has the same number as myColors.
 * @note  1. cvtColor() can convert the color of the image to HSV
 *        2. there is a loop to select color mask between myColors`s lower and upper data.
 *        3. inRange() can implemente function 2.
 *        4. getContours() can get the Points in boundingRect top midpoint.
 *        5. return the new points
*/
static vector<vector<int>> findColor(Mat img, vector<vector<int>> myColors, vector<Scalar> myColorValues)
{
    /* convert to HSV */
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);
    for (int i = 0; i < myColors.size(); i++) {
        Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
        Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
        /* create a mask */
        // Check if the pixel values are within the range and set them to 0
        Mat mask;
        inRange(imgHSV, lower, upper, mask);
        // imshow(to_string(i), mask);
        Point myPoint = getContours(mask, img);
        if (myPoint.x != 0 && myPoint.y != 0) {
            newPoints.push_back({myPoint.x, myPoint.y, i});
        }
    }
    return newPoints;
}

/** 
 * @brief  draw on canvas
 * @param newPoints : it contains the Points in boundingRect top midpoint.
 * @param myColorValues : it contains the color you want to paint , it has the same number as myColors.
*/
static void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
    for (int i = 0; i < newPoints.size(); i++) {
        circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
    }
}

/**
 * @brief cv_zone painter
 * @note  1. use a color picker to determine the mask range for each color and save it in myColors. 
 *        2. use this range to determine the mask for the selected color. 
 *        3. save the midpoint of the top edge of the bounding rectangle of the mask as newPoints. 
 *        4. draw these newPoints.
*/
void cv_zone_PenPainter()
{

    VideoCapture capture(0);

    struct timeval start_time, end_time;
    uint32_t delta_time;
    get_current_time(&start_time);
    while (1) {

        capture >> img;

        newPoints = findColor(img, myColors, myColorValues);

        drawOnCanvas(newPoints, myColorValues);

        imshow("读取视频帧", img);
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