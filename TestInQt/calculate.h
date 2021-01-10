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
void Log(Mat src, Mat intermediate, int c); //�����任
void Power(Mat src, Mat intermediate, float gamma);//�ݱ任
void Reverse(Mat src, Mat intermediate);//��ֵ�˲�
Mat AverageImg(Mat Img, int size);  //��ֵ�˲�
uchar GetValue(Mat Img, int size, int i, int j,int type);  //����ֵ�����ֵ����Сֵ
Mat MedianFlitering(Mat Img, int size); //��ֵ�˲�
Mat BinaryImg(Mat Img, int value,int type);  //��ֵ��
Mat Corrosion(Mat Img, int size, int x, int y);//��ʴ
Mat Inflation(Mat Img, int size, int x, int y);//����
Mat TopHat(Mat Img,int size,int x,int y);  //��ñ�任
Mat BottonHat(Mat Img,int size,int x ,int y);  //��ñ�任
void on_mouse(int event, int x, int y, int flags, void *ustc);//��ͼ����






