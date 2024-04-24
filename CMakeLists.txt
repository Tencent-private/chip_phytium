#该命令指定了编译该工程对cmake的最高、最低版本要求，如果 CMake 的运行版本低于所需版本，它将停止处理项目并报告错误。
cmake_minimum_required(VERSION 3.16.3)

Project(opencv_SDK_phytium)

# 设置编译类型
option(RUNNING_OPENCV "Build with OpenCV" ON)
if(DEFINED RUNNING_OPENCV)
    if(RUNNING_OPENCV)
        message("Build with OpenCV")
        find_package(OpenCV REQUIRED)
        # opencv 官方库文件
        # set(OpenCV_INCLUDE_DIRS /usr/include)
        include_directories(${OpenCV_INCLUDE_DIRS}) 
        # 自定义文件
        include_directories(with_opencv/include)  
        aux_source_directory (with_opencv/src SRC_LIST)  
    else()
        message("Build without OpenCV")
        ADD_DEFINITIONS(-DCLOSE_OPENCV)
    endif()
endif()
# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/opencv_output)

# 可以在运行中正确使用管道
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

##下面是输出信息
message(STATUS "Opnecv ;ibrary status: ")
message(STATUS "> version: ${OpenCV_VERSION} ")
message(STATUS "libraries: ${OpenCV_LIBS} ")
message(STATUS "> include: ${OpenCV_INCLUDE_DIRS}  ")

###----------库文件----------------### start
include_directories(include) 
###----------库文件------------------### end

###----------源文件------------------### start
aux_source_directory (src SRC_LIST)
###----------源文件------------------### end


# 使用指定的源文件来生成目标可执行文件。
add_executable(vedio_opencv main.cpp ${SRC_LIST})
if(RUNNING_OPENCV)
    # 将目标文件与库文件进行链接，一些动态连接库需要在程序运行阶段才被调用
    target_link_libraries(vedio_opencv ${OpenCV_LIBS})
endif()