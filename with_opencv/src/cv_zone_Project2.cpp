#include "cv_zone.h"

/* Global Variables */
Mat imgOriginal, imgThre, imgGray, imgBlur, imgCanny, imgDil, imgErode, imgWrap, imgCrop;
vector<Point> initialPoints, docPoints;

float width = 420, height = 596;

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
static vector<Point> getContours(Mat imgDil, Mat img)
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

    vector<Point> biggest;
    int maxArea = 0;

    for (int i = 0; i < contours.size(); i++) {
        int area = contourArea(contours[i]);

        cout << area << endl;
        /* filter */
        if (area > 500) {
            /* 计算轮廓的 周长 */
            float peri = arcLength(contours[i], true);
            /* 减少定点数来逼近轮廓形状 */
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            if (area > maxArea && conPoly[i].size() == 4) {
                // drawContours(img, conPoly, i, Scalar(255, 0, 255), 10);
                biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
                maxArea = area;
            }
            // drawContours(img, conPoly, i, Scalar(255, 0, 255), 5);
            // rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
        }
    }
    return biggest;
}

/**
 * @brief  Preprocessing
 * @param img : the original image
 * @note  To get dilate img 
*/
static Mat preProcessing(Mat img)
{

    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgDil, kernel);
    // erode(imgDil, imgErode, kernel);
    return imgDil;
}

static void drawPoints(vector<Point> points, Scalar color)
{

    for (int i = 0; i < points.size(); i++) {

        circle(imgOriginal, points[i], 10, color, FILLED);
        putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 6, color, 4);
    }
}

/**
 * @brief  reorder the points
 * @param points : the points need to be reordered
*/
vector<Point> reorder(vector<Point> points)
{
    vector<Point> newPoints;
    vector<int> sumPoints, subPoints;

    for (int i = 0; i < points.size(); i++) {
        sumPoints.push_back(points[i].x + points[i].y);
        subPoints.push_back(points[i].x - points[i].y);
    }

    newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 0
    newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 1
    newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 2
    newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 3

    return newPoints;
}

/**
 * @brief  warp the image
 * @param img : the original image
 * @param points : the points need to be warped (It's like finding the special points used to solve the equation)
 * @param w : the width of the warped image
 * @param h : the height of the warped image
*/
static Mat getWrap(Mat img, vector<Point> points, float w, float h)
{
    Point2f src[4] = {points[0], points[1], points[2], points[3]};
    Point2f dst[4] = {{0, 0}, {w, 0}, {0, h}, {w, h}};

    Mat matrix = getPerspectiveTransform(src, dst);
    warpPerspective(img, imgWrap, matrix, Size(w, h));

    return imgWrap;
}

/**
 * @brief file scan
*/
void cv_zone_FileScan()
{
    imageDisplayThead.startShowImages();
    string file_path = "../peripheral/cv_zone/Resources/paper.jpg";
    imgOriginal      = imread(file_path);

    // resize(imgOriginal, imgOriginal, Size(), 2, 2);

    /* Preprocessing */
    imgThre = preProcessing(imgOriginal);
    /* Get Contours -- Biggest  */
    initialPoints = getContours(imgThre, imgOriginal);
    // drawPoints(initialPoints, Scalar(0, 0, 255));
    docPoints = reorder(initialPoints);
    // drawPoints(docPoints, Scalar(0, 255, 0));
    /* Warp */
    imgWrap = getWrap(imgOriginal, docPoints, width, height);

    // Crop
    int cropSize = 10;
    Rect roi(cropSize, cropSize, width - cropSize, height - cropSize);
    imgCrop = imgWrap(roi);

    imageDisplayThead.addImage("img", imgOriginal);
    imageDisplayThead.addImage("img Dilation ", imgThre);
    imageDisplayThead.addImage("imgWrap ", imgWrap);
    cin.get();
    imageDisplayThead.closeShowImages();
}