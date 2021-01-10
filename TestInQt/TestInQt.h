#include "ui_TestInQt.h"
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <QtCore/QCoreApplication>
#include <iostream>
#include<QFileDialog>
#include <opencv2/ml.hpp>
#include <io.h>
#include<QPushButton>
#include <QtWidgets/QApplication>
#include <Qstring>
#include <QComboBox>


using namespace cv;
using namespace std;


class TestInQt : public QMainWindow
{
    Q_OBJECT

public:
    TestInQt(QWidget *parent = Q_NULLPTR);

private:
    Ui::TestInQtClass ui;
	Mat Img;   //全部操作对Img进行
	Mat ReturnImg;  //保存处理前的图像
	Mat BeginImg;   //保存刚开始的图像
	Mat testImage;
	string s1;
	

private slots:
	void ReadPic_Clicked();     //“载入”按钮
	void PicClear_Clicked();    //“清空”按钮
	void SavePic_Clicked();     //“保存”按钮
	void ReturnPic_Clicked();   //“返回”按钮
	void RestorePic_Clicked();  //“复原”按钮
	void PicShow(Mat pic);  //label处显示图片
	void TxtShow(QString s);    //label处显示文本
	
	void GrayPic();             //“灰度化”
	void BinImg();				//“二值化”
	void GrayChange();          //“灰度变换”
	void DeNoice();             //“去噪”
	void FormCount();           //“形态运算”

	void OneStep();       //“一键增强”
	void screenshot();
	void recognize();	//文字识别
};
