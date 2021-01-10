
#include "TestInQt.h"
#include "ChangeFormat.h"
#include "calculate.h"
#include <iostream>

using namespace cv;
using namespace std;

//�����任
void Log(Mat src, Mat intermediate, int c) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int b = c * log(1 + src.at<uchar>(i, j));
			int d = b % 256;
			intermediate.at<uchar>(i, j) = d;
		}
	}
}

//�ݱ任
void Power(Mat src, Mat intermediate, float gamma) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			float c = pow(255, 1 - gamma);
			int b = c * pow(src.at<uchar>(i, j), gamma);
			int d = b % 256;
			intermediate.at<uchar>(i, j) = d;
		}
	}
}

//ͼ��ת
void Reverse(Mat src, Mat intermediate) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			intermediate.at<uchar>(i, j) = 255 - src.at<uchar>(i, j);
		}
	}
}

//��ֵ�˲�
Mat AverageImg(Mat Img, int size) {
	Mat OutImg = Img.clone();
	for (int i = 0; i < Img.rows; i++) {
		for (int j = 0; j < Img.cols; j++) {
			if ((i - (size - 1) / 2) >= 0 && (j - (size - 1) / 2) >= 0 &&
				(i + (size - 1) / 2) < Img.rows && (j + (size - 1) / 2) < Img.cols) {
				int all = 0;  //��ʼ��ƽ��ֵ
				for (int col = (0 - (size - 1) / 2); col <= (size - 1) / 2; col++) {
					for (int row = (0 - (size - 1) / 2); row <= (size - 1) / 2; row++) {
						all = all + Img.at<uchar>(i + col, j + row);  //size���ܺ�
					}
				}
				OutImg.at<uchar>(i, j) = all / (size*size);
			}
			else {
				OutImg.at<uchar>(i, j) = Img.at<uchar>(i, j);
			}
		}
	}
	return OutImg;
}

//����ֵ�����ֵ����Сֵ��type=1Ϊ���ֵ,type=2Ϊ��ֵ��type=0Ϊ��Сֵ
uchar GetValue(Mat img, int size, int i, int j,int type) {
	int N = size * size;
	uchar *arr = new uchar[N];
	int n = 0;
	for (int col = (0 - (size - 1) / 2); col <= (size - 1) / 2; col++) {
		for (int row = (0 - (size - 1) / 2); row <= (size - 1) / 2; row++) {
			arr[n] = img.at<uchar>(i + col, j + row);  //��size�ڵ����Ž�����
			n++;
		}
	}
	for (int gap = N / 2; gap > 0; gap /= 2)//ϣ������
		for (int i = gap; i < N; ++i)
			for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)
				swap(arr[j], arr[j + gap]);
	if (type == 1)
		return arr[N-1];//�������ֵ
	else if (type == 0)
		return arr[0];//������Сֵ
	else if(type == 2)
		return arr[(N - 1) / 2];//������ֵ
}

//��ֵ�˲�����
Mat MedianFlitering(Mat Img, int size) {
	Mat OutImg = Img.clone();
	for (int i = 0; i < Img.rows; ++i)
		for (int j = 0; j < Img.cols; ++j) {
			if ((i - (size - 1) / 2) >= 0 && (j - (size - 1) / 2) >= 0 &&
				(i + (size - 1) / 2) < Img.rows && (j + (size - 1) / 2) < Img.cols) {
				OutImg.at<uchar>(i, j) = GetValue(Img, size, i, j, 2);
			}
			else
				OutImg.at<uchar>(i, j) = Img.at<uchar>(i, j);
		}
	return OutImg;
}

//��ֵ����
//type�������ͣ�
//type=1������ֵvalue���ף�С�����ڣ�type=0���ߴ��ڱ�ڣ�С�ڱ��
Mat BinaryImg(Mat Img, int value,int type) {
	Mat OutImg = Img.clone();
	for (int i = 0; i < Img.rows; ++i) {
		for (int j = 0; j < Img.cols; ++j) {
			if (type == 1) {
				if (Img.at<uchar>(i, j) > value) {
					OutImg.at<uchar>(i, j) = 255;
				}
				else
					OutImg.at<uchar>(i, j) = 0;
			}
			else {
				if (Img.at<uchar>(i, j) < value) {
					OutImg.at<uchar>(i, j) = 255;
				}
				else
					OutImg.at<uchar>(i, j) = 0;
			}

		}
	}
	return OutImg;
}

//��ʴ���ú�ɫ����,size��ʾģ���С,x��ʾê��ˮƽƫ�ƣ�y��ʾê�㴹ֱƫ��
Mat Corrosion(Mat img, int size, int x, int y) {
	Mat OutImg = img.clone();
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if ((i - (size - 1) / 2) >= 0 && (j - (size - 1) / 2) >= 0 &&
				(i + (size - 1) / 2) < img.rows && (j + (size - 1) / 2) < img.cols) {
				//int all = 0;  //��ʼ���Ƿ���ں�ɫ����
				uchar MinValue = 0;  //��С��ֵĬ��Ϊ0
				MinValue = GetValue(img, size, i, j, 0); //��ȡģ���ڵ���Сֵ
				//����ṹԪ���ڴ��ں�ɫ�����Ҳ���ȫ�ף���ê���Ϊ��ɫ����ʴ
				//��ֵͼ��ʱ���к�ɫ��СҲΪ0���ͽ�ê����Ϊ��ɫ0
				//�Ҷ�ͼ��ʱ�������Сֵ��ֱ�ӽ�ֵ��ê��
				if (MinValue != 255) {  //�������ȫ��(ȫ255����ʱ��СΪ255)
					OutImg.at<uchar>(i + x, j + y) = MinValue; //ֱ�Ӹ���Сֵ
				}
			}
			else {
				OutImg.at<uchar>(i, j) = img.at<uchar>(i, j);
			}
		}
	}
	return OutImg;
}

//���ͣ��ð�ɫ����,size��ʾģ���С,x��ʾê��ˮƽƫ�ƣ�y��ʾê�㴹ֱƫ��
Mat Inflation(Mat img, int size, int x, int y) {
	Mat OutImg = img.clone();
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if ((i - (size - 1) / 2) >= 0 && (j - (size - 1) / 2) >= 0 &&
				(i + (size - 1) / 2) < img.rows && (j + (size - 1) / 2) < img.cols) {
				//int all = 0;  //��ʼ���Ƿ���ں�ɫ����
				 uchar MaxValue = 0; //��ʼ�����ֵΪ0
				 MaxValue = (int)GetValue(img, size, i, j, 1); //��ȡģ���ڵ����ֵ
				 //MaxValue = (int)MaxValue;
				
				//����ṹԪ���ڴ��ڰ�ɫ�����Ҳ���ȫ�ڣ���ê���Ϊ��ɫ������
				//��ֵͼ��ʱ���а�ɫ���ҲΪ255���ͽ�ê����Ϊ��ɫ255
				//�Ҷ�ͼ��ʱ��������ֵ��ֱ�ӽ�ֵ��ê��
				if (MaxValue != 0) {
					OutImg.at<uchar>(i + x, j + y) = MaxValue;
				}
			}
			else {
				OutImg.at<uchar>(i, j) = img.at<uchar>(i, j);
			}
		}
	}
	return OutImg;
}

//��ñ�任
Mat TopHat(Mat Img,int size,int x,int y) {
	Mat OutImg = Img.clone();
	Mat TempImg = Img.clone();
	TempImg = Corrosion(Img, size, x, y); //�õ���ʴ
	TempImg = Inflation(TempImg, size, x, y);//��ʴ�����ͣ��õ�������
	OutImg = Img - TempImg; //��ñ=ԭͼ-������
	return OutImg;
}

//��ñ�任
Mat BottonHat(Mat Img,int size,int x, int y) {
	Mat OutImg = Img.clone();
	Mat TempImg = Img.clone();
	TempImg = Inflation(Img, size, x, y); //�õ�����
	TempImg = Corrosion(TempImg, size, x, y);//���ͺ�ʴ���õ�������
	OutImg = TempImg - Img;  //��ñ=������-ԭͼ
	return OutImg;
}
//��ͼ����
cv::Mat org, dst, img, tmp;

void on_mouse(int event, int x, int y, int flags, void *ustc)//event����¼����ţ�x,y������꣬flags��ק�ͼ��̲����Ĵ���  
{
	static Point pre_pt = (-1, -1);//��ʼ����  
	static Point cur_pt = (-1, -1);//ʵʱ����  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ  
	{
		org.copyTo(img);//��ԭʼͼƬ���Ƶ�img��  
		sprintf(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//�ڴ�������ʾ����  
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//��Բ  
		imshow("img", img);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�����  
	{
		img.copyTo(tmp);//��img���Ƶ���ʱͼ��tmp�ϣ�������ʾʵʱ����  
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//ֻ��ʵʱ��ʾ����ƶ�������  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ�����  
	{
		img.copyTo(tmp);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
		
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP)//����ɿ�������ͼ���ϻ�����  
	{
		org.copyTo(img);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//���ݳ�ʼ��ͽ����㣬�����λ���img��  
		imshow("img", img);
		img.copyTo(tmp);
		//��ȡ���ΰ�Χ��ͼ�񣬲����浽dst��  
		int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);
		if (width == 0 || height == 0)
		{
			printf("width == 0 || height == 0");
			return;
		}
		dst = org(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
		/*namedWindow("dst");
		imshow("dst", dst);*/
		imwrite("recognize.png",dst);
		waitKey(0);
	}
	
}

//��ÿ����ͨ���ϵĸ������y��С�����������
void  SortContourPoint(vector<vector<Point>> inputContours, vector<vector<Point>> &outputContours)
{
	vector<Point> tempContoursPoint;
	for(int i = 0; i < inputContours.size(); i++)
	{
		tempContoursPoint.clear();    //ÿ��ѭ��ע�����
		for(int j = 0; j < inputContours[i].size(); j++)
		{
			for(int k = j; k < inputContours[i].size(); k++)
			{
				if( inputContours[i][j].y > inputContours[i][k].y)
				{
					swap(inputContours[i][j], inputContours[i][k]);
				}
			}
			tempContoursPoint.push_back(inputContours[i][j]);
		}
		outputContours.push_back(tempContoursPoint);
	}
}

