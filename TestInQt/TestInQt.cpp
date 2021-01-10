#include "TestInQt.h"
#include "ChangeFormat.h"
#include "calculate.h"
#include "qlabel.h"
#include <opencv2/ml.hpp>
#include <io.h>
using namespace std;
using namespace cv;
using namespace CVS;
using namespace ml;

TestInQt::TestInQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	ui.GrayTypeLine->setPlaceholderText("value");
	ui.DeNoTypeLine->setPlaceholderText("3");
	ui.BinaryValue->setPlaceholderText("125");
	ui.FormTypeLine->setPlaceholderText("3"); 
	ui.IdentifyText->setPlaceholderText(QStringLiteral("识别文字区"));
}

//在label处(PicShowPlace)显示图片，输入Mat类型
void TestInQt::PicShow(Mat pic){
	QPixmap tmp = CVS::cvMatToQPixmap(pic);
	ui.PicShowPlace->clear();  //清空label
	ui.PicShowPlace->setScaledContents(true); //主动比例压缩图片
	ui.PicShowPlace->setPixmap(tmp);//加载到Label标签
	ui.PicShowPlace->show();//显示
}

//在label处(TxtShowPlace)显示文本
void TestInQt::TxtShow(QString s) {
	ui.TxtShowPlace->clear();  //清空label
	ui.TxtShowPlace->setText(s);  //显示文本
}

//“载入”按钮
void TestInQt::ReadPic_Clicked() {
	QString OpenFile = QFileDialog::getOpenFileName(this, "Please choose an image file", "./");
	//第三个参数为过滤器例如："Image(*.jpg *.tif *.png)"；
	QString OpenFilePath;
	QFileInfo OpenFileInfo;
	OpenFileInfo = QFileInfo(OpenFile);
	OpenFilePath = OpenFileInfo.filePath();
	s1 = OpenFilePath.toStdString();
	QString s = QString::fromStdString(s1);
	QPixmap tmp;
	tmp.load(s);  //载入图像
	Img = CVS::QPixmapToCvMat(tmp);
	ReturnImg = Img;  //最开始的也是上一步的
	BeginImg = Img;  //保存最开始的图片
	PicShow(Img);
	TxtShow(QStringLiteral("图片已加载！"));
}

//“保存”按钮
void TestInQt::SavePic_Clicked() {

}

//“返回”按钮
void TestInQt::ReturnPic_Clicked() {
	if (!(CVS::cvMatToQPixmap(ReturnImg))) {
		TxtShow(QStringLiteral("请先加载一张图片！"));
		return;
	}
	Img = ReturnImg;  //将处理前的值给回Img
	PicShow(ReturnImg);
	TxtShow(QStringLiteral("已返回到上一步！"));
}

//“复原”按钮
void TestInQt::RestorePic_Clicked() {
	if (!(CVS::cvMatToQPixmap(BeginImg))) {
		TxtShow(QStringLiteral("请先加载一张图片！"));
		return;
	}
	PicShow(BeginImg);
	Img = BeginImg;
	TxtShow(QStringLiteral("图片已复原！"));
}

//“清空”按钮
void TestInQt::PicClear_Clicked() {
	ui.PicShowPlace->clear();  //清空label
	TxtShow(QStringLiteral("图片已清空！"));
	QPixmap clearPix = QPixmap();
	Img = CVS::QPixmapToCvMat(clearPix);
	ReturnImg = Img;
	BeginImg = Img;
	update();
}

//“灰度化”按钮
void TestInQt::GrayPic() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("请先加载一张图片！"));
		return;
	}
	ReturnImg = Img;//保存处理前的图像
	TxtShow(QStringLiteral("请先加载一张图片！"));
	Mat grayImg;
	cvtColor(Img, grayImg, COLOR_BGR2GRAY);  //将TempImg转化为灰度图;
	Img = grayImg;  //将得到的灰度图grayImg给Img，进行后续处理
	PicShow(Img);
	TxtShow(QStringLiteral("灰度化成功！"));
}

//“二值化”按钮
void TestInQt::BinImg() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("请先加载一张图片！"));
		return;
	}
	ReturnImg = Img;//保存处理前的图像
	TxtShow(QStringLiteral("正在处理，请稍等......"));
	Mat binImg = Img.clone();
	//获取输入的value
	double value = ui.BinaryValue->text().toDouble();
	value = (int)value; //强制转换为整型
	if (!value) {  //若没有输入value，则默认为125
		value = 125;
	}
	binImg = BinaryImg(Img, value, 1);
	Img = binImg;
	PicShow(binImg);
	TxtShow(QStringLiteral("二值化成功！"));
}

//“灰度变换”按钮
void TestInQt::GrayChange() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("请先加载一张图片！"));
		return;
	}
	ReturnImg = Img;//保存处理前的图像
	TxtShow(QStringLiteral("正在处理，请稍等......"));
	//获取ComboBox列表索引Index，0：对数变换，1：幂变换，2：反转
	int Index = ui.GrayChangeType->currentIndex(); 
	Mat output = Img.clone();
	//ui.GrayTypeLine->setPlaceholderText("value input");
	//判断所选灰度变换方法
	//-----对数变换-----
	if (Index == 0) {
		double c = ui.GrayTypeLine->text().toDouble();
		c = (int)c; //强制转换为整型
		//TxtShow(QString::number(c));
		if (!c) {  //若没有输入c，则输出提示信息
			TxtShow(QStringLiteral("请先输入参数！"));
			return;
		}
		Log(Img, output, c);
	}
	//-----幂变换-----
	else if (Index == 1) { 
		double gamma = ui.GrayTypeLine->text().toDouble();
		//gamma = (int)gamma; //强制转换为整型
		if (!gamma) {  //若没有输入c，则输出提示信息
			TxtShow(QStringLiteral("请先输入参数！"));
			return;
		}
		Power(Img, output, gamma);
	}
	//-----图像反转-----
	else if (Index == 2) { 
		Reverse(Img, Img);
	}
	Img = output;
	PicShow(Img);
	TxtShow(QStringLiteral("灰度变换成功！"));
}

//“去除噪声”
void TestInQt::DeNoice() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("请先加载一张图片！"));
		return;
	}
	ReturnImg = Img;//保存处理前的图像
	TxtShow(QStringLiteral("正在处理，请稍等......"));
	//获取ComboBox列表索引Index，0：均值，1：中值，2：高斯
	int Index = ui.DeNoiceType->currentIndex();
	Mat output = Img.clone();
	//获取输入的size
	double size = ui.GrayTypeLine->text().toDouble();
	size = (int)size; //强制转换为整型
	if (!size) {  //若没有输入size，则默认为size=3
		size = 3;
	}
	//-----均值滤波-----
	if (Index == 0) {  
		output = AverageImg(Img, size);
	}
	//-----中值滤波-----
	else if (Index == 1) {
		output = MedianFlitering(Img, size);
	}
	Img = output;
	PicShow(Img);
	TxtShow(QStringLiteral("去除噪声成功！"));
}

//“形态运算”
void TestInQt::FormCount() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("请先加载一张图片！"));
		return;
	}
	ReturnImg = Img;//保存处理前的图像
	TxtShow(QStringLiteral("正在处理，请稍等......"));
	//获取ComboBox列表索引Index，0：开运算，1：闭运算，2：腐蚀，3：膨胀
	int Index = ui.FormType->currentIndex();
	Mat output = Img.clone();
	Mat temp = Img.clone();
	double size = ui.FormTypeLine->text().toDouble();
	size = (int)size; //强制转换为整型
	if (!size) {  //若没有输入size，则默认为size=3
		size = 3;
	}
	if (Index == 0) {  //开运算
		temp = Corrosion(Img, size, 0, 0);  //得到腐蚀的图像
		output = Inflation(temp, size, 0, 0); //开运算：将腐蚀后的图像进行膨胀
	}
	else if (Index == 1) {  //闭运算
		output = Inflation(Img, size, 0, 0);  //得到膨胀的图像
		output = Corrosion(output, size, 0, 0); //闭运算：将膨胀后的图像进行腐蚀
	}
	else if (Index == 2) {  //腐蚀，(未灰度化时求最小值)
		output = Corrosion(Img, size, 0, 0);
	}
	else if (Index == 3) {  //膨胀，(未灰度化时求最大值)
		output = Inflation(Img, size, 0, 0);
	}
	else if (Index == 4) {  //顶帽
		output = TopHat(Img, size, 0, 0);
	}
	else if (Index == 5) {  //底帽
		output = BottonHat(Img, size, 0, 0);
	}
	else if (Index == 6) { //原图-底帽
		output = Img - BottonHat(Img, size, 0, 0);
	}
	else if (Index == 7) { //原图+顶帽
		output = Img + TopHat(Img, size, 0, 0);
	}

	Img = output;
	PicShow(Img);
	TxtShow(QStringLiteral("形态运算完成！"));
}


//“一键增强”
void TestInQt::OneStep() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("请先加载一张图片！"));
		return;
	}
	ReturnImg = Img;//保存处理前的图像
	TxtShow(QStringLiteral("正在处理，请稍等......"));
	Mat output;
	Mat GrayImg;  //灰度图
	cvtColor(Img, GrayImg, COLOR_BGR2GRAY);  //①灰度化

	Mat PowImg = GrayImg.clone();  //灰度变换图
	Power(GrayImg, PowImg, 1.5);  //②灰度变换：幂变换1.5
	output = GrayImg - BottonHat(PowImg, 3, 0, 0);  //③原图-底帽变换一次
	output = GrayImg - BottonHat(output, 3, 0, 0);  //两次
	/*
	Img = output;
	PicShow(Img);
	TxtShow(QStringLiteral("一键增强完成！"));
	*/
	/////////////////////////////////////////////////////////
	Mat BinImg = BinaryImg(output, 80, 1);  //二值化图像
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(BinImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	Mat imageContours = Mat::zeros(BinImg.size(), CV_8UC1);
	Mat Contours = Mat::zeros(BinImg.size(), CV_8UC1);  //绘制 

	for (int i = 0; i < contours.size(); i++){
		//contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数  
		for (int j = 0; j < contours[i].size(); j++){
			//绘制出contours向量内所有的像素点  
			Point P = Point(contours[i][j].x, contours[i][j].y);
			Contours.at<uchar>(P) = 255;
		}

		//输出hierarchy向量内容  
		char ch[256];
		sprintf(ch, "%d", i);
		string str = ch;
		cout << "向量hierarchy的第" << str << " 个元素内容为：" << endl << hierarchy[i] << endl << endl;

		//绘制轮廓  
		drawContours(imageContours, contours, i, Scalar(255), 1, 8, hierarchy);
	}
	imshow("Contours Image", imageContours); //轮廓  
	imshow("Point of Contours", Contours);   //向量contours内保存的所有轮廓点集
	/*testImage = imageContours;
	Img = imageContours;
	PicShow(Img);
	TxtShow(QStringLiteral("一键增强运算完成！"));
	*/
}

void TestInQt::screenshot()
{
	////////////////////截图保存
	org = imread(s1);
	org.copyTo(img);
	org.copyTo(tmp);
	namedWindow("img");//定义一个img窗口 
	imshow("img", img);
	setMouseCallback("img", on_mouse, this);//调用回调函数
	cv::waitKey(0);
}

void TestInQt::recognize()
{
	destroyWindow("img");
	//启动控制台
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	Mat recognize=imread("recognize.png");
	/////////////////找轮廓画矩阵
	IplImage *image = (IplImage*)&IplImage(recognize);
	if (image == NULL) {
		printf("错误：无法打开该图像文件！");
	}
	//转换到灰度图
	IplImage *img_gray = cvCreateImage(cvGetSize(image), image->depth, 1);
	cvCvtColor(image, img_gray, CV_BGR2GRAY);
	//二值化
	IplImage *img_value = cvCreateImage(cvGetSize(img_gray), image->depth, 1);
	cvThreshold(img_gray, img_value, 210, 255, CV_THRESH_BINARY_INV);
	//寻找轮廓
	CvMemStorage *pStorage = cvCreateMemStorage(0);
	CvSeq *pConInner = NULL;
	int num = cvFindContours(img_value, pStorage, &pConInner, sizeof(CvContour),
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	cout << num << endl;
	//此时轮廓输出逆序，要顺序排放
	



	IplImage* imgNo[100] = { NULL };

	//根据轮廓坐标使用方框标出来
	for (int i = 0; i < num; i++)
	{
		CvRect rc = cvBoundingRect(pConInner);
		//cvDrawRect(image, cvPoint(rc.x, rc.y), cvPoint(rc.x + rc.width, rc.y + rc.height), CV_RGB(255, 0, 0));	//为了防止裁剪时出现矩阵颜色，所以将这段代码屏蔽掉！
		imgNo[i] = cvCreateImage(cvSize(rc.width, rc.height), IPL_DEPTH_8U, 3);
		cvSetImageROI(image, cvRect(rc.x, rc.y, rc.width, rc.height));//设置源图像ROI
		cvCopy(image, imgNo[i]);	//裁剪
		pConInner = pConInner->h_next;
	}
	
	
	

	//此时已得到所切割出来的各个图
	//显示图像
	char a[200];
	char b[200];
	for (int i = 0; i < num; ++i) {
		//sprintf(a, "%d", i);
		//cvNamedWindow(a,CV_WINDOW_AUTOSIZE);
		//cvShowImage(a, imgNo[i]);//显示所有切割出来的图片
		sprintf(b, "D:\\Class_Learning\\DImageProcessing\\TestInQt\\TestInQt\\recognizepic\\%d.jpg", i);
		Mat mat = cvarrToMat(imgNo[i]);
		imwrite(b, mat);//保存模板图片
	}
	//将所有图片大小统一转化为12*16
	const int imageRows = 12;
	const int imageCols = 16;
	//图片共有34类
	const int classSum = 34;
	//每类共50张图片
	const int imagesSum = 50;
	double alpha; /**< 控制对比度 */
	int beta;  /**< 控制亮度 */
	////==========================预测部分==============================
	//读取模型
	Ptr<ANN_MLP> model = StatModel::load<ANN_MLP>("D:/Class_Learning/DImageProcessing/TestInQt/TestInQt/model.xml");
	//cout << "读取模型完毕!!!!!" << endl;
	
	char str[10];
	char n[10];
	for (int w = 0; w < num; ++w)
	{
			//读取测试图像
			Mat test;
			sprintf(b, "D:\\Class_Learning\\DImageProcessing\\TestInQt\\TestInQt\\recognizepic\\%d.jpg", w);
			test = imread(b, 0);
			cv::resize(test, test, cv::Size(imageRows, imageCols), (0, 0), (0, 0), INTER_AREA);
			//imshow("0", test);
			//反转
			for (int i = 0; i < test.rows; i++) {
				for (int j = 0; j < test.cols; j++) {
					test.at<uchar>(i, j) = 255 - test.at<uchar>(i, j);
				}
			}
			//imshow("1", test);
			//增加对比度
			int fa = 43; int fb = 98; float k = 2.5;
			for (int x = 0; x < test.rows; x++)
			{
				for (int y = 0; y < test.cols; y++)
				{
					if (test.at<uchar>(x, y) < fa)
						test.at<uchar>(x, y) = 0;
					else if (test.at<uchar>(x, y) > fb)
						test.at<uchar>(x, y) = 255;
					else
						test.at<uchar>(x, y) = k * (test.at<uchar>(x, y) - fa);
				}
			}
			//imshow("2", test);
			//二值化
			threshold(test, test, 80, 255, 0); 
			
			//sprintf(n,"%d",w);
			//cvNamedWindow(n,0);
			//imshow(n, test);//显示所有切割出来的图片
			//将测试图像转化为1*192的向量
			Mat_<float>testMat(1, imageRows*imageCols);
			for (int i = 0; i < imageRows*imageCols; i++)
			{
				testMat.at<float>(0, i) = (float)test.at<uchar>(i / imageRows, i % imageRows);
			}
			Mat arr;
			//使用训练好的model预测测试图像
			model->predict(testMat, arr);
			//std::cout << "testMat: \n" << testMat << "\n" << std::endl;
			//std::cout << "dst: \n" << arr << "\n" << std::endl;
			double maxVal = 0;
			Point maxLoc;
			minMaxLoc(arr, NULL, &maxVal, NULL, &maxLoc);//在一个数组中找到全局最小值和全局最大值
			//std::cout << "测试结果：" << maxLoc.x << "置信度:" << maxVal * 100 << "%" << std::endl;
			//imshow("test", test);
			char tmp[15];
			for (int t = 0; t < 10; t++) {
				if (maxLoc.x == t)
				{	
					sprintf(tmp,"%d",t);
					str[w] = *tmp;
				}
			}
			if (maxLoc.x == 10)str[w] = 'A';
			if (maxLoc.x == 11)str[w] = 'B';
			if (maxLoc.x == 12)str[w] = 'C';
			if (maxLoc.x == 13)str[w] = 'D';
			if (maxLoc.x == 14)str[w] = 'E';
			if (maxLoc.x == 15)str[w] = 'F';
			if (maxLoc.x == 16)str[w] = 'G';
			if (maxLoc.x == 17)str[w] = 'H';
			if (maxLoc.x == 18)str[w] = 'J';
			if (maxLoc.x == 19)str[w] = 'K';
			if (maxLoc.x == 20)str[w] = 'L';
			if (maxLoc.x == 21)str[w] = 'M';
			if (maxLoc.x == 22)str[w] = 'N';
			if (maxLoc.x == 23)str[w] = 'P';
			if (maxLoc.x == 24)str[w] = 'Q';
			if (maxLoc.x == 25)str[w] = 'R';
			if (maxLoc.x == 26)str[w] = 'S';
			if (maxLoc.x == 27)str[w] = 'T';
			if (maxLoc.x == 28)str[w] = 'U';
			if (maxLoc.x == 29)str[w] = 'V';
			if (maxLoc.x == 30)str[w] = 'W';
			if (maxLoc.x == 31)str[w] = 'X';
			if (maxLoc.x == 32)str[w] = 'Y';
			if (maxLoc.x == 33)str[w] = 'Z';	
	}
	
	for (int i = 0; i < num;i++) cout << "测试结果：" << str[num-1-i] << endl;
}


/////ANN神经网络训练部分代码
////每一行一个训练图片
	//float trainingData[classSum*imagesSum][imageRows*imageCols] = { {0} };
	////训练样本标签
	//float labels[classSum*imagesSum][classSum] = { {0} };
	//Mat src, resizeImg, trainImg;
	//for (int i = 0; i < classSum; i++)
	//{
	//	//目标文件夹路径
	//	std::string inPath = "D:\\MyDownloads\\vs2017_workspace\\qt_work\\work_1\\Qtwork1\\Qtwork1\\samples\\";
	//	char temp[256];
	//	int k = 0;
	//	sprintf_s(temp, "%d", i);
	//	inPath = inPath + temp + "\\*.png";
	//	//用于查找的句柄
	//	long long handle;
	//	struct _finddata_t fileinfo;
	//	//第一次查找
	//	handle = _findfirst(inPath.c_str(), &fileinfo);
	//	if (handle == -1)
	//		return -1;
	//	do
	//	{
	//		//找到的文件的文件名
	//		std::string imgname = "D:/MyDownloads/vs2017_workspace/qt_work/work_1/Qtwork1/Qtwork1/samples/";
	//		imgname = imgname + temp + "/" + fileinfo.name;
	//		src = imread(imgname, 0);
	//		if (src.empty())
	//		{
	//			std::cout << "can not load image \n" << std::endl;
	//			return -1;
	//		}
	//		//将所有图片大小统一转化为12*16
	//		resize(src, resizeImg, Size(imageRows, imageCols), (0, 0), (0, 0), INTER_AREA);
	//		threshold(resizeImg, trainImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//		for (int j = 0; j < imageRows*imageCols; j++)
	//		{
	//			trainingData[i*imagesSum + k][j] = (float)resizeImg.data[j];
	//		}
	//		// 设置标签数据
	//		for (int j = 0; j < classSum; j++)
	//		{
	//			if (j == i)
	//				labels[i*imagesSum + k][j] = 1;
	//			else
	//				labels[i*imagesSum + k][j] = 0;
	//		}
	//		k++;

	//	} while (!_findnext(handle, &fileinfo));
	//	Mat labelsMat(classSum*imagesSum, classSum, CV_32FC1, labels);

	//	_findclose(handle);
	//}
	////训练样本数据及对应标签
	//Mat trainingDataMat(classSum*imagesSum, imageRows*imageCols, CV_32FC1, trainingData);
	//Mat labelsMat(classSum*imagesSum, classSum, CV_32FC1, labels);
	////std::cout<<"trainingDataMat: \n"<<trainingDataMat<<"\n"<<std::endl;
	////std::cout<<"labelsMat: \n"<<labelsMat<<"\n"<<std::endl;
	////==========================训练部分==============================

	//Ptr<ANN_MLP> model = ANN_MLP::create();
	//Mat layerSizes = (Mat_<int>(1, 5) << imageRows * imageCols, 128,128, 128, classSum);
	//model->setLayerSizes(layerSizes);
	//model->setTrainMethod(ANN_MLP::BACKPROP, 0.001, 0.1);
	//model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1.0, 1.0);
	//model->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001));

	//Ptr<TrainData> trainData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	//model->train(trainData);
	////保存训练结果
	//model->save("D:\\MyDownloads\\vs2017_workspace\\qt_work\\work_1\\Qtwork1\\Qtwork1\\model\\model.xml");