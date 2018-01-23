#include "synopsis.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;
void getFgImagedd(const IplImage * frame, IplImage * fgImg, const IplImage * bgImg, const int threshold) {
    if (frame == NULL || fgImg == NULL || bgImg == NULL) {
        printf("Fail: There exists NULL input. Fail to get Foreground Image!\n");
        return;
    }

    if (frame->nChannels != 3 || fgImg->nChannels != 3 || bgImg->nChannels != 3) {
        printf("Fail: All input image should be color!\nframe channel:%d\nfgImg channel:%d\nbgImg channel:%d\n", \
			frame->nChannels, fgImg->nChannels, bgImg->nChannels);
        return;
    }

    CvMat * frameMat = cvCreateMat(frame->height, frame->width, CV_32FC3);
    CvMat * fgMat = cvCreateMat(fgImg->height, fgImg->width, CV_32FC3);
    CvMat * bgMat = cvCreateMat(bgImg->height, bgImg->width, CV_32FC3);

    cvConvert(frame, frameMat);
    cvConvert(fgImg, fgMat);
    cvConvert(bgImg, bgMat);

    cvSmooth(frameMat, frameMat, CV_GAUSSIAN, 5, 5, 4, 4); //高斯滤波先，以平滑图像

    cvAbsDiff(frameMat, bgMat, fgMat); //当前帧跟背景图相减(求背景差并取绝对值)

    cvThreshold(fgMat, fgImg, threshold, 255, CV_THRESH_BINARY); //二值化前景图(这里采用特定阈值进行二值化)

    //进行形态学滤波，去掉噪音
    cvErode(fgImg, fgImg, 0, 1);
    cvDilate(fgImg, fgImg, 0, 1);

    //释放矩阵内存
    cvReleaseMat(&frameMat);
    cvReleaseMat(&fgMat);
    cvReleaseMat(&bgMat);
}

int main(int argc, char* argv[])
{

    const char file_path[] = "/home/xin/Desktop/mydata/segment_1min.avi";
    const char file_out_path[] = "/home/xin/Desktop/output/test_dav_out.avi";

    int fps, frame_number;
    CvSize size;
    IplImage* bgImage = NULL;
    const int frame_num_used = 500;
    list< list<tube *> > database;
    int code = (int)CV_FOURCC('d', 'i', 'v', '3');//MPEG-4.3 codec
    bgModeling(file_path, file_out_path, frame_num_used, &bgImage, fps, frame_number, size, code);

    code = (int)CV_FOURCC('x', 'v', 'i', 'd');

    buildTrackDB(file_path, bgImage, database);
    mergeDB(database, file_out_path, fps, size, bgImage);

    VideoCapture capture("/home/xin/Desktop/output/test_dav_out.avi");
    Mat frame;

    while (capture.isOpened())
    {
        capture >> frame;

        imshow("video", frame);
        if (waitKey(20) == 27)//27是键盘摁下esc时，计算机接收到的ascii码值
        {
            break;
        }
    }
    return 0;
}