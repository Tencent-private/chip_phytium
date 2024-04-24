/* @fileName: common.h
*  @function: 工程中的通用函数
*/
#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <math.h>
#include <sys/time.h>

using namespace std;

/* C++ for OPENCV */
#include <mutex>
#include <thread>
#include <vector>

#ifdef CLOSE_OPENCV
#define CLOSE_PHYTIUM_OPENCV
#endif

extern void get_current_time(timeval *tv);

#endif // COMMON_H