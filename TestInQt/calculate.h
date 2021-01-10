#pragma once
#include "ui_TestInQt.h"
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <QtCore/QCoreApplication>
#include <iostream>
#include<QFileDialog>
#include<QPushButton>
#include <QtWidgets/QApplication>
#include <Qstring>
#include <QComboBox>

using namespace cv;

extern cv::Mat org, dst, img, tmp;
void  SortContourPoint(vector<vector<Point>> inputContours, vector<vector<Point>> &outputContours);
void Log(Mat src, Mat intermediate, int c); //对数变换
void Power(Mat src, Mat intermediate, float gamma);//幂变换
void Reverse(Mat src, Mat intermediate);//均值滤波
Mat AverageImg(Mat Img, int size);  //均值滤波
uchar GetValue(Mat Img, int size, int i, int j,int type);  //求中值或最大值或最小值
Mat MedianFlitering(Mat Img, int size); //中值滤波
Mat BinaryImg(Mat Img, int value,int type);  //二值化
Mat Corrosion(Mat Img, int size, int x, int y);//腐蚀
Mat Inflation(Mat Img, int size, int x, int y);//膨胀
Mat TopHat(Mat Img,int size,int x,int y);  //顶帽变换
Mat BottonHat(Mat Img,int size,int x ,int y);  //底帽变换
void on_mouse(int event, int x, int y, int flags, void *ustc);//截图函数






