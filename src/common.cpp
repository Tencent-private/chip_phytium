/* @fileName: common.cpp
*  @function: 工程中的通用函数
*/
#include "common.h"

// 获取当前时间的函数
void get_current_time(timeval *tv) {
    gettimeofday(tv, NULL);
}