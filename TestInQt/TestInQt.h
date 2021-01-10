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
	Mat Img;   //ȫ��������Img����
	Mat ReturnImg;  //���洦��ǰ��ͼ��
	Mat BeginImg;   //����տ�ʼ��ͼ��
	Mat testImage;
	string s1;
	

private slots:
	void ReadPic_Clicked();     //�����롱��ť
	void PicClear_Clicked();    //����ա���ť
	void SavePic_Clicked();     //�����桱��ť
	void ReturnPic_Clicked();   //�����ء���ť
	void RestorePic_Clicked();  //����ԭ����ť
	void PicShow(Mat pic);  //label����ʾͼƬ
	void TxtShow(QString s);    //label����ʾ�ı�
	
	void GrayPic();             //���ҶȻ���
	void BinImg();				//����ֵ����
	void GrayChange();          //���Ҷȱ任��
	void DeNoice();             //��ȥ�롱
	void FormCount();           //����̬���㡱

	void OneStep();       //��һ����ǿ��
	void screenshot();
	void recognize();	//����ʶ��
};
