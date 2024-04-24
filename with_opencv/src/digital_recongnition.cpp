#include "../include/digital_recongnition.h"
#include <iostream>

//创建记录轮廓数量和坐标的结构体
struct num_contours
{
	double x, y;             //轮廓的x、y坐标
	int index;               //轮廓的顺序

	bool operator <(num_contours &m)
	{
		if (y > m.y+50) return false;

		else if (y > m.y-50)
		{
			if (x < m.x) return true;

			else return false;
		}

		else return true;
	}
}num_contours[100];

//创建记录结果的结构体
struct result
{
	double data;
	int num;

	bool operator<(result &m)
	{
		if (data < m.data)return true;
		else return false;
	}
}result[100];

void select_result(Mat & src, int num);
vector<Mat> imread_model();
void deal(Mat &src, Mat &model, int m);


int digital_recognition(void)
{
    Mat src, gray_src, dest;
    src = imread("../peripheral/digital_recon/test_number_is_8.png"); //加载原图

    if (src.empty())
    {
        cerr << "无法加载图像文件。" << endl;
        return 0;
    }

    cvtColor(src, gray_src, COLOR_BGR2GRAY); //对图像进行预处理
    threshold(gray_src, dest, 150, 255, THRESH_BINARY_INV);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(dest, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0)); //提取轮廓

    vector<Rect> rect;

    for (size_t i = 0; i < contours.size(); i++)
    {
        if (contourArea(contours[i]) > 50)
        {
            rect.push_back(boundingRect(contours[i]));
        }
    }

    if (rect.size() != contours.size())
    {
        cerr << "轮廓数量与矩形数量不匹配。" << endl;
        return 0;
    }

    sort(rect.begin(), rect.end(), [](const Rect &r1, const Rect &r2) {
        return r1.y < r2.y;
    });

    vector<Mat> roi_rect(contours.size());
    for (size_t j = 0; j < contours.size(); j++) //ROI处理分割对象
    {
        int k = j; //对分割对象进行预处理
        src(rect[k]).copyTo(roi_rect[j]);
        cvtColor(roi_rect[j], roi_rect[j], COLOR_BGR2GRAY);
        threshold(roi_rect[j], roi_rect[j], 150, 255, THRESH_BINARY_INV);
        select_result(roi_rect[j], j + 1);
    }

    for (size_t i = 0; i < contours.size(); i++)
    {
        rectangle(src, rect[i], Scalar(0, 0, 255), 1, 8, 0);
    }

    imshow("二值图", dest);
    imshow("原图", src);

    waitKey(0);

    cout << "Hello, Phytium OpenCV111!" << endl;

    return 0;
}

void deal(Mat &src, Mat &dst, int m)
{
	// 调整 dst 的大小，使其与 src 的大小匹配
    resize(dst, dst, src.size());

    threshold(dst, dst, 100, 255, THRESH_BINARY_INV);
    Mat dest;
    resize(src, dest, Size(src.cols * 1, src.rows * 1), 0, 0, INTER_LINEAR);

    // 创建一个数组来保存比较的结果
    Mat comparisonResult;

    // 对比两个图像，并将结果保存到 comparisonResult 中
    cv::compare(dest, dst, comparisonResult, CMP_EQ); // 这里使用了 CMP_EQ 作为比较类型，你可以根据需求选择其他比较类型

    // 将比较的结果转换为 double 类型的值，存储到 result[m].data 中
    double sum = cv::sum(comparisonResult)[0];
    result[m].data = sum;

    // 设置 result[m].num
    result[m].num = m;
}

vector<Mat> imread_model()
{
	//加载模板
	vector<Mat> myTemplate;

	for (int i = 0; i < 10; i++)
	{
		char name[64];
		snprintf(name, 64,"../peripheral/digital_recon/%d.png", i);
		Mat temp = imread(name, 0);
		myTemplate.push_back(temp);
	}

	return myTemplate;
}

void select_result(Mat & src, int num)
{
	for (int i = 0; i < 10; i++)
	{
		deal(src, imread_model()[i], i);
	}
	sort(result, result + 10);

	if (result[9].data>0.7)
	{
		cout << "第" << num << "个数字为 " << result[9].num << endl;
		cout << "识别精度为 " << result[9].data << endl << endl;
	}

}