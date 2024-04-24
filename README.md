# chip_phytium

#### 介绍

第八届集创赛项目\_phytium 杯

拉取代码使用 ssh / https

按照说明添加一下 user.name 与 user.mail

生成公钥，然后添加

#### 软件架构

.
├── bak_code
├── build
│ ├── CMakeCache.txt
│ ├── CMakeFiles
│ ├── cmake_install.cmake
│ └── Makefile
├── CMakeLists.txt
├── include
│ ├── chh_number_reco.h
│ ├── common.h
│ ├── opencv_sdk_phytium.h
│ └── testFile.h
├── LICENSE
├── main.cpp
├── main.h
├── opencv_output
├── peripheral
│ ├── chh_number_recon
│ ├── cv_zone
│ ├── data
│ ├── digital_recon
│ └── test_photo.jpg
├── README.en.md
├── README.md
├── src
│ ├── chh_number_reco.cpp
│ ├── common.cpp
│ └── testFile.cpp
└── with_opencv
├── include
└── src

14 directories, 17 files

#### 安装教程

需要安装(git,g++环境等，否则编译不过)
待补充...

#### 使用说明

1. 首先将 build 文件夹中所有的文件删除
   rm -rf build/\*

2. 进入编译文件夹
   cd build/

3. 生成 Makefile 文件
   cmake .. -DRUNNING_OPENCV=ON
   (注：如果电脑上没有 opencv 的 c++环境，则执行
   cmake .. -DRUNNING_OPENCV=OFF
   )

4. 清除上次编译的不相关文件
   make clean

5. 编译文件
   make all

6. 进入 可执行文件/目标文件所在目录
   cd ../opencv_output
7. 执行文件
   ./your_file_name
   例 ./vedio_opencv
   这里执行了名为 vedio_opencv 的可执行文件

#### 参与贡献

1.  切换到 develop 分支
    git checkout develop
2.  拉取最新代码
    git pull
3.  创建一个自己的分支()
    git checkout -b your_branch_name
    例：git checkout -b first_branch
    这里创建了一个名字叫做"first_branch"的分支
4.  进行代码修改
5.  将所有修改打包;
    git add .
    (注：这步要位于工程目录下，不能位于其子文件夹下)
6.  添加修改信息
    git commit -s -m "the message of modify"
    例：git commit -s -m "first submit"
    这里给提交内容附加的注释是：first submit
7.  上传代码到自己的分支
    git push your_branch_name
8.  切回到 develop 分支
    git checkout develop
9.  将 改动 merge 到 develop 分支
    git merge your_branch_name develop
10. 上传改动
    git push
