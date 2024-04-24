#include "cv_common.h"

cn::TheadOfImage imageDisplayThead;
namespace cn {
TheadOfImage::TheadOfImage()
{
    this->displayFlag = true;
    this->threadFlag  = true;
}

/**
     * @brief TheadOfImage::setFlag 设置flag的值
     * @param value 要设置的值，1表示显示图像，0表示不显示
     */
void TheadOfImage::setDisplayFlag(bool value)
{
    this->displayFlag = value;
}

/**
     * @brief TheadOfImage::removeImage 移除显示的图片
     * @param idx 要移除显示图像的索引
     * @return 返回是否移除成功，0表示成功，否则表示失败
     */
int TheadOfImage::removeImage(int idx)
{
    if (idx < 0 || idx >= (int)images.size()) {
        cout << "索引位置超出vector边界！" << endl;
        return 1;
    }
    this->vectorMutex.lock();
    try {
        // 有时候窗口还没来的及建立就被销毁，此时会抛出异常。捕获异常并进行提示
        destroyWindow(this->windowNames[idx]);
    } catch (cv::Exception) {
        std::cout << this->windowNames[idx] + " 窗口销毁过快！但是销毁成功了！" << endl;
    }
    this->images.erase(this->images.begin() + idx);
    this->windowNames.erase(this->windowNames.begin() + idx);
    this->vectorMutex.unlock();
    return 0;
}

/**
     * @brief TheadOfImage::removeImage 根据串口名字销毁窗口
     * @param windowName 要销毁窗口的名字
     * @return 返回是否移除成功，0表示成功，否则表示失败
     */
int TheadOfImage::removeImage(String windowName)
{
    for (uint idx = 0; idx < this->windowNames.size(); idx++) //对于名字相同的窗口，进行销毁
    {
        if (windowName == this->windowNames[idx]) {
            this->vectorMutex.lock();
            try { // 有时候窗口还没来的及建立就被销毁，此时会抛出异常。捕获异常并进行提示
                destroyWindow(this->windowNames[idx]);
            } catch (cv::Exception) {
                std::cout << this->windowNames[idx] + " 窗口销毁过快！但是销毁成功了！" << endl;
            }
            this->images.erase(this->images.begin() + idx);
            this->windowNames.erase(this->windowNames.begin() + idx);
            this->vectorMutex.unlock();
            return 0;
        }
    }
    cout << "窗口 " + windowName + " 移除失败，不存在该窗口" << endl;
    return 1;
}

/**
     * @brief TheadOfImage::addImage 添加要显示的图像
     * @param windowName 显示的窗口名字
     * @param image Mat图像
     * @param idx 写入的位置，可以用于替换之前的图像。默认添加一个新的图像
     * @return 返回当前的图像的索引
     */
int TheadOfImage::addImage(String windowName, Mat image, int idx)
{
    for (uint i = 0; i < this->windowNames.size(); i++) //对于名字相同的窗口，直接替换原来的图片
    {
        if (windowName == this->windowNames[i]) {
            this->vectorMutex.lock();
            this->images[i] = image;
            this->vectorMutex.unlock();
            return i;
        }
    }

    if (idx == -1) //默认在最后添加一个新的
    {
        this->vectorMutex.lock();
        this->images.push_back(image);
        this->windowNames.push_back(windowName);
        this->vectorMutex.unlock();
        return images.size();
    } else if (idx < 0 || idx > (int)(images.size() - 1)) // 索引越界
    {
        cout << "图像索引位置输入有误" << endl;
        return idx;
    } else {
        this->vectorMutex.lock();
        this->images[idx]      = image;
        this->windowNames[idx] = windowName;
        this->vectorMutex.unlock();
        return idx;
    }
}

/**
     * @brief TheadOfImage::showImages 显示图像的主要函数
     */
void TheadOfImage::showImages(void)
{
    while (this->threadFlag) {
        if (this->displayFlag == true) {
            this->vectorMutex.lock();
            for (uint i = 0; i < this->images.size(); i++) //循环显示images里的所有图像
            {
                namedWindow(this->windowNames[i], WINDOW_NORMAL);
                imshow(this->windowNames[i], this->images[i]);
            }
            this->vectorMutex.unlock();
            waitKey(1);
        } else {
            destroyAllWindows();
        }
    }
    destroyAllWindows();
}

/**
     * @brief TheadOfImage::startShowImages 开启显示图像的线程
     */
void TheadOfImage::startShowImages(void)
{
    this->displayFlag = true;
    this->threadFlag  = true;
    thread show(&TheadOfImage::showImages, this);
    show.detach();
}

/** 关闭显示图像的线程，建议只在程序最后调用。因为关闭后就无法再打开。
     * @brief TheadOfImage::closeShowImages 只想临时关闭图像显示，请调用setDisplayFlag
     */
void TheadOfImage::closeShowImages(void)
{
    this->threadFlag = false;
}
} //namespace cn
