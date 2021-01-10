
#include "TestInQt.h"
#include "ChangeFormat.h"
#include "calculate.h"
#include <iostream>

using namespace cv;
using namespace std;

//对数变换
void Log(Mat src, Mat intermediate, int c) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int b = c * log(1 + src.at<uchar>(i, j));
			int d = b % 256;
			intermediate.at<uchar>(i, j) = d;
		}
	}
}

//幂变换
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

//图像反转
void Reverse(Mat src, Mat intermediate) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			intermediate.at<uchar>(i, j) = 255 - src.at<uchar>(i, j);
		}
	}
}

//均值滤波
Mat AverageImg(Mat Img, int size) {
	Mat OutImg = Img.clone();
	for (int i = 0; i < Img.rows; i++) {
		for (int j = 0; j < Img.cols; j++) {
			if ((i - (size - 1) / 2) >= 0 && (j - (size - 1) / 2) >= 0 &&
				(i + (size - 1) / 2) < Img.rows && (j + (size - 1) / 2) < Img.cols) {
				int all = 0;  //初始化平均值
				for (int col = (0 - (size - 1) / 2); col <= (size - 1) / 2; col++) {
					for (int row = (0 - (size - 1) / 2); row <= (size - 1) / 2; row++) {
						all = all + Img.at<uchar>(i + col, j + row);  //size内总和
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

//求中值或最大值或最小值，type=1为最大值,type=2为中值，type=0为最小值
uchar GetValue(Mat img, int size, int i, int j,int type) {
	int N = size * size;
	uchar *arr = new uchar[N];
	int n = 0;
	for (int col = (0 - (size - 1) / 2); col <= (size - 1) / 2; col++) {
		for (int row = (0 - (size - 1) / 2); row <= (size - 1) / 2; row++) {
			arr[n] = img.at<uchar>(i + col, j + row);  //将size内的数放进数组
			n++;
		}
	}
	for (int gap = N / 2; gap > 0; gap /= 2)//希尔排序
		for (int i = gap; i < N; ++i)
			for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)
				swap(arr[j], arr[j + gap]);
	if (type == 1)
		return arr[N-1];//返回最大值
	else if (type == 0)
		return arr[0];//返回最小值
	else if(type == 2)
		return arr[(N - 1) / 2];//返回中值
}

//中值滤波函数
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

//二值化，
//type决定类型：
//type=1大于阈值value则变白，小于则变黑；type=0或者大于变黑，小于变白
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

//腐蚀，让黑色更多,size表示模板大小,x表示锚点水平偏移，y表示锚点垂直偏移
Mat Corrosion(Mat img, int size, int x, int y) {
	Mat OutImg = img.clone();
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if ((i - (size - 1) / 2) >= 0 && (j - (size - 1) / 2) >= 0 &&
				(i + (size - 1) / 2) < img.rows && (j + (size - 1) / 2) < img.cols) {
				//int all = 0;  //初始化是否存在黑色像素
				uchar MinValue = 0;  //最小的值默认为0
				MinValue = GetValue(img, size, i, j, 0); //获取模板内的最小值
				//如果结构元素内存在黑色像素且不是全白，则锚点变为黑色，腐蚀
				//二值图像时，有黑色最小也为0，就将锚点设为黑色0
				//灰度图像时，获得最小值，直接将值给锚点
				if (MinValue != 255) {  //如果不是全白(全255，此时最小为255)
					OutImg.at<uchar>(i + x, j + y) = MinValue; //直接给最小值
				}
			}
			else {
				OutImg.at<uchar>(i, j) = img.at<uchar>(i, j);
			}
		}
	}
	return OutImg;
}

//膨胀，让白色更多,size表示模板大小,x表示锚点水平偏移，y表示锚点垂直偏移
Mat Inflation(Mat img, int size, int x, int y) {
	Mat OutImg = img.clone();
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if ((i - (size - 1) / 2) >= 0 && (j - (size - 1) / 2) >= 0 &&
				(i + (size - 1) / 2) < img.rows && (j + (size - 1) / 2) < img.cols) {
				//int all = 0;  //初始化是否存在黑色像素
				 uchar MaxValue = 0; //初始化最大值为0
				 MaxValue = (int)GetValue(img, size, i, j, 1); //获取模板内的最大值
				 //MaxValue = (int)MaxValue;
				
				//如果结构元素内存在白色像素且不是全黑，则锚点变为白色，膨胀
				//二值图像时，有白色最大也为255，就将锚点设为白色255
				//灰度图像时，获得最大值，直接将值给锚点
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

//顶帽变换
Mat TopHat(Mat Img,int size,int x,int y) {
	Mat OutImg = Img.clone();
	Mat TempImg = Img.clone();
	TempImg = Corrosion(Img, size, x, y); //得到腐蚀
	TempImg = Inflation(TempImg, size, x, y);//腐蚀后膨胀，得到开运算
	OutImg = Img - TempImg; //顶帽=原图-开运算
	return OutImg;
}

//底帽变换
Mat BottonHat(Mat Img,int size,int x, int y) {
	Mat OutImg = Img.clone();
	Mat TempImg = Img.clone();
	TempImg = Inflation(Img, size, x, y); //得到膨胀
	TempImg = Corrosion(TempImg, size, x, y);//膨胀后腐蚀，得到闭运算
	OutImg = TempImg - Img;  //底帽=闭运算-原图
	return OutImg;
}
//截图函数
cv::Mat org, dst, img, tmp;

void on_mouse(int event, int x, int y, int flags, void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{
	static Point pre_pt = (-1, -1);//初始坐标  
	static Point cur_pt = (-1, -1);//实时坐标  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		org.copyTo(img);//将原始图片复制到img中  
		sprintf(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//在窗口上显示坐标  
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
		imshow("img", img);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{
		img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标  
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{
		img.copyTo(tmp);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
		
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
	{
		org.copyTo(img);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上  
		imshow("img", img);
		img.copyTo(tmp);
		//截取矩形包围的图像，并保存到dst中  
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

//对每条连通域上的各点根据y从小到大进行排序
void  SortContourPoint(vector<vector<Point>> inputContours, vector<vector<Point>> &outputContours)
{
	vector<Point> tempContoursPoint;
	for(int i = 0; i < inputContours.size(); i++)
	{
		tempContoursPoint.clear();    //每次循环注意清空
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

