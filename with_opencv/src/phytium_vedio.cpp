#include "../include/phytium_vedio.h"

int veido_function(void)
{
	cout<< "this is vedio_function"<<endl;
    VideoCapture capture(0);
    struct timeval start_time, end_time;
    uint32_t delta_time;
    get_current_time(&start_time);
	while (1)
	{
		Mat frame;
		capture >> frame;
		imshow("读取视频帧", frame);
		waitKey(30);
        get_current_time(&end_time);
        delta_time = end_time.tv_sec - start_time.tv_sec;
		cout << "Time: " << delta_time << endl;
		if(delta_time >= VEDIO_CAPTURE_TIME_SEC)
		{
			break;
		}
	}
    capture.release();
    destroyAllWindows();
	system("pause");
	return 0;
}