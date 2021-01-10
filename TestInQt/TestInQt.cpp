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
	ui.IdentifyText->setPlaceholderText(QStringLiteral("ʶ��������"));
}

//��label��(PicShowPlace)��ʾͼƬ������Mat����
void TestInQt::PicShow(Mat pic){
	QPixmap tmp = CVS::cvMatToQPixmap(pic);
	ui.PicShowPlace->clear();  //���label
	ui.PicShowPlace->setScaledContents(true); //��������ѹ��ͼƬ
	ui.PicShowPlace->setPixmap(tmp);//���ص�Label��ǩ
	ui.PicShowPlace->show();//��ʾ
}

//��label��(TxtShowPlace)��ʾ�ı�
void TestInQt::TxtShow(QString s) {
	ui.TxtShowPlace->clear();  //���label
	ui.TxtShowPlace->setText(s);  //��ʾ�ı�
}

//�����롱��ť
void TestInQt::ReadPic_Clicked() {
	QString OpenFile = QFileDialog::getOpenFileName(this, "Please choose an image file", "./");
	//����������Ϊ���������磺"Image(*.jpg *.tif *.png)"��
	QString OpenFilePath;
	QFileInfo OpenFileInfo;
	OpenFileInfo = QFileInfo(OpenFile);
	OpenFilePath = OpenFileInfo.filePath();
	s1 = OpenFilePath.toStdString();
	QString s = QString::fromStdString(s1);
	QPixmap tmp;
	tmp.load(s);  //����ͼ��
	Img = CVS::QPixmapToCvMat(tmp);
	ReturnImg = Img;  //�ʼ��Ҳ����һ����
	BeginImg = Img;  //�����ʼ��ͼƬ
	PicShow(Img);
	TxtShow(QStringLiteral("ͼƬ�Ѽ��أ�"));
}

//�����桱��ť
void TestInQt::SavePic_Clicked() {

}

//�����ء���ť
void TestInQt::ReturnPic_Clicked() {
	if (!(CVS::cvMatToQPixmap(ReturnImg))) {
		TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
		return;
	}
	Img = ReturnImg;  //������ǰ��ֵ����Img
	PicShow(ReturnImg);
	TxtShow(QStringLiteral("�ѷ��ص���һ����"));
}

//����ԭ����ť
void TestInQt::RestorePic_Clicked() {
	if (!(CVS::cvMatToQPixmap(BeginImg))) {
		TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
		return;
	}
	PicShow(BeginImg);
	Img = BeginImg;
	TxtShow(QStringLiteral("ͼƬ�Ѹ�ԭ��"));
}

//����ա���ť
void TestInQt::PicClear_Clicked() {
	ui.PicShowPlace->clear();  //���label
	TxtShow(QStringLiteral("ͼƬ����գ�"));
	QPixmap clearPix = QPixmap();
	Img = CVS::QPixmapToCvMat(clearPix);
	ReturnImg = Img;
	BeginImg = Img;
	update();
}

//���ҶȻ�����ť
void TestInQt::GrayPic() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
		return;
	}
	ReturnImg = Img;//���洦��ǰ��ͼ��
	TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
	Mat grayImg;
	cvtColor(Img, grayImg, COLOR_BGR2GRAY);  //��TempImgת��Ϊ�Ҷ�ͼ;
	Img = grayImg;  //���õ��ĻҶ�ͼgrayImg��Img�����к�������
	PicShow(Img);
	TxtShow(QStringLiteral("�ҶȻ��ɹ���"));
}

//����ֵ������ť
void TestInQt::BinImg() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
		return;
	}
	ReturnImg = Img;//���洦��ǰ��ͼ��
	TxtShow(QStringLiteral("���ڴ������Ե�......"));
	Mat binImg = Img.clone();
	//��ȡ�����value
	double value = ui.BinaryValue->text().toDouble();
	value = (int)value; //ǿ��ת��Ϊ����
	if (!value) {  //��û������value����Ĭ��Ϊ125
		value = 125;
	}
	binImg = BinaryImg(Img, value, 1);
	Img = binImg;
	PicShow(binImg);
	TxtShow(QStringLiteral("��ֵ���ɹ���"));
}

//���Ҷȱ任����ť
void TestInQt::GrayChange() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
		return;
	}
	ReturnImg = Img;//���洦��ǰ��ͼ��
	TxtShow(QStringLiteral("���ڴ������Ե�......"));
	//��ȡComboBox�б�����Index��0�������任��1���ݱ任��2����ת
	int Index = ui.GrayChangeType->currentIndex(); 
	Mat output = Img.clone();
	//ui.GrayTypeLine->setPlaceholderText("value input");
	//�ж���ѡ�Ҷȱ任����
	//-----�����任-----
	if (Index == 0) {
		double c = ui.GrayTypeLine->text().toDouble();
		c = (int)c; //ǿ��ת��Ϊ����
		//TxtShow(QString::number(c));
		if (!c) {  //��û������c���������ʾ��Ϣ
			TxtShow(QStringLiteral("�������������"));
			return;
		}
		Log(Img, output, c);
	}
	//-----�ݱ任-----
	else if (Index == 1) { 
		double gamma = ui.GrayTypeLine->text().toDouble();
		//gamma = (int)gamma; //ǿ��ת��Ϊ����
		if (!gamma) {  //��û������c���������ʾ��Ϣ
			TxtShow(QStringLiteral("�������������"));
			return;
		}
		Power(Img, output, gamma);
	}
	//-----ͼ��ת-----
	else if (Index == 2) { 
		Reverse(Img, Img);
	}
	Img = output;
	PicShow(Img);
	TxtShow(QStringLiteral("�Ҷȱ任�ɹ���"));
}

//��ȥ��������
void TestInQt::DeNoice() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
		return;
	}
	ReturnImg = Img;//���洦��ǰ��ͼ��
	TxtShow(QStringLiteral("���ڴ������Ե�......"));
	//��ȡComboBox�б�����Index��0����ֵ��1����ֵ��2����˹
	int Index = ui.DeNoiceType->currentIndex();
	Mat output = Img.clone();
	//��ȡ�����size
	double size = ui.GrayTypeLine->text().toDouble();
	size = (int)size; //ǿ��ת��Ϊ����
	if (!size) {  //��û������size����Ĭ��Ϊsize=3
		size = 3;
	}
	//-----��ֵ�˲�-----
	if (Index == 0) {  
		output = AverageImg(Img, size);
	}
	//-----��ֵ�˲�-----
	else if (Index == 1) {
		output = MedianFlitering(Img, size);
	}
	Img = output;
	PicShow(Img);
	TxtShow(QStringLiteral("ȥ�������ɹ���"));
}

//����̬���㡱
void TestInQt::FormCount() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
		return;
	}
	ReturnImg = Img;//���洦��ǰ��ͼ��
	TxtShow(QStringLiteral("���ڴ������Ե�......"));
	//��ȡComboBox�б�����Index��0�������㣬1�������㣬2����ʴ��3������
	int Index = ui.FormType->currentIndex();
	Mat output = Img.clone();
	Mat temp = Img.clone();
	double size = ui.FormTypeLine->text().toDouble();
	size = (int)size; //ǿ��ת��Ϊ����
	if (!size) {  //��û������size����Ĭ��Ϊsize=3
		size = 3;
	}
	if (Index == 0) {  //������
		temp = Corrosion(Img, size, 0, 0);  //�õ���ʴ��ͼ��
		output = Inflation(temp, size, 0, 0); //�����㣺����ʴ���ͼ���������
	}
	else if (Index == 1) {  //������
		output = Inflation(Img, size, 0, 0);  //�õ����͵�ͼ��
		output = Corrosion(output, size, 0, 0); //�����㣺�����ͺ��ͼ����и�ʴ
	}
	else if (Index == 2) {  //��ʴ��(δ�ҶȻ�ʱ����Сֵ)
		output = Corrosion(Img, size, 0, 0);
	}
	else if (Index == 3) {  //���ͣ�(δ�ҶȻ�ʱ�����ֵ)
		output = Inflation(Img, size, 0, 0);
	}
	else if (Index == 4) {  //��ñ
		output = TopHat(Img, size, 0, 0);
	}
	else if (Index == 5) {  //��ñ
		output = BottonHat(Img, size, 0, 0);
	}
	else if (Index == 6) { //ԭͼ-��ñ
		output = Img - BottonHat(Img, size, 0, 0);
	}
	else if (Index == 7) { //ԭͼ+��ñ
		output = Img + TopHat(Img, size, 0, 0);
	}

	Img = output;
	PicShow(Img);
	TxtShow(QStringLiteral("��̬������ɣ�"));
}


//��һ����ǿ��
void TestInQt::OneStep() {
	if (!(CVS::cvMatToQPixmap(Img))) {
		TxtShow(QStringLiteral("���ȼ���һ��ͼƬ��"));
		return;
	}
	ReturnImg = Img;//���洦��ǰ��ͼ��
	TxtShow(QStringLiteral("���ڴ������Ե�......"));
	Mat output;
	Mat GrayImg;  //�Ҷ�ͼ
	cvtColor(Img, GrayImg, COLOR_BGR2GRAY);  //�ٻҶȻ�

	Mat PowImg = GrayImg.clone();  //�Ҷȱ任ͼ
	Power(GrayImg, PowImg, 1.5);  //�ڻҶȱ任���ݱ任1.5
	output = GrayImg - BottonHat(PowImg, 3, 0, 0);  //��ԭͼ-��ñ�任һ��
	output = GrayImg - BottonHat(output, 3, 0, 0);  //����
	/*
	Img = output;
	PicShow(Img);
	TxtShow(QStringLiteral("һ����ǿ��ɣ�"));
	*/
	/////////////////////////////////////////////////////////
	Mat BinImg = BinaryImg(output, 80, 1);  //��ֵ��ͼ��
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(BinImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	Mat imageContours = Mat::zeros(BinImg.size(), CV_8UC1);
	Mat Contours = Mat::zeros(BinImg.size(), CV_8UC1);  //���� 

	for (int i = 0; i < contours.size(); i++){
		//contours[i]������ǵ�i��������contours[i].size()������ǵ�i�����������е����ص���  
		for (int j = 0; j < contours[i].size(); j++){
			//���Ƴ�contours���������е����ص�  
			Point P = Point(contours[i][j].x, contours[i][j].y);
			Contours.at<uchar>(P) = 255;
		}

		//���hierarchy��������  
		char ch[256];
		sprintf(ch, "%d", i);
		string str = ch;
		cout << "����hierarchy�ĵ�" << str << " ��Ԫ������Ϊ��" << endl << hierarchy[i] << endl << endl;

		//��������  
		drawContours(imageContours, contours, i, Scalar(255), 1, 8, hierarchy);
	}
	imshow("Contours Image", imageContours); //����  
	imshow("Point of Contours", Contours);   //����contours�ڱ�������������㼯
	/*testImage = imageContours;
	Img = imageContours;
	PicShow(Img);
	TxtShow(QStringLiteral("һ����ǿ������ɣ�"));
	*/
}

void TestInQt::screenshot()
{
	////////////////////��ͼ����
	org = imread(s1);
	org.copyTo(img);
	org.copyTo(tmp);
	namedWindow("img");//����һ��img���� 
	imshow("img", img);
	setMouseCallback("img", on_mouse, this);//���ûص�����
	cv::waitKey(0);
}

void TestInQt::recognize()
{
	destroyWindow("img");
	//��������̨
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	Mat recognize=imread("recognize.png");
	/////////////////������������
	IplImage *image = (IplImage*)&IplImage(recognize);
	if (image == NULL) {
		printf("�����޷��򿪸�ͼ���ļ���");
	}
	//ת�����Ҷ�ͼ
	IplImage *img_gray = cvCreateImage(cvGetSize(image), image->depth, 1);
	cvCvtColor(image, img_gray, CV_BGR2GRAY);
	//��ֵ��
	IplImage *img_value = cvCreateImage(cvGetSize(img_gray), image->depth, 1);
	cvThreshold(img_gray, img_value, 210, 255, CV_THRESH_BINARY_INV);
	//Ѱ������
	CvMemStorage *pStorage = cvCreateMemStorage(0);
	CvSeq *pConInner = NULL;
	int num = cvFindContours(img_value, pStorage, &pConInner, sizeof(CvContour),
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	cout << num << endl;
	//��ʱ�����������Ҫ˳���ŷ�
	



	IplImage* imgNo[100] = { NULL };

	//������������ʹ�÷�������
	for (int i = 0; i < num; i++)
	{
		CvRect rc = cvBoundingRect(pConInner);
		//cvDrawRect(image, cvPoint(rc.x, rc.y), cvPoint(rc.x + rc.width, rc.y + rc.height), CV_RGB(255, 0, 0));	//Ϊ�˷�ֹ�ü�ʱ���־�����ɫ�����Խ���δ������ε���
		imgNo[i] = cvCreateImage(cvSize(rc.width, rc.height), IPL_DEPTH_8U, 3);
		cvSetImageROI(image, cvRect(rc.x, rc.y, rc.width, rc.height));//����Դͼ��ROI
		cvCopy(image, imgNo[i]);	//�ü�
		pConInner = pConInner->h_next;
	}
	
	
	

	//��ʱ�ѵõ����и�����ĸ���ͼ
	//��ʾͼ��
	char a[200];
	char b[200];
	for (int i = 0; i < num; ++i) {
		//sprintf(a, "%d", i);
		//cvNamedWindow(a,CV_WINDOW_AUTOSIZE);
		//cvShowImage(a, imgNo[i]);//��ʾ�����и������ͼƬ
		sprintf(b, "D:\\Class_Learning\\DImageProcessing\\TestInQt\\TestInQt\\recognizepic\\%d.jpg", i);
		Mat mat = cvarrToMat(imgNo[i]);
		imwrite(b, mat);//����ģ��ͼƬ
	}
	//������ͼƬ��Сͳһת��Ϊ12*16
	const int imageRows = 12;
	const int imageCols = 16;
	//ͼƬ����34��
	const int classSum = 34;
	//ÿ�๲50��ͼƬ
	const int imagesSum = 50;
	double alpha; /**< ���ƶԱȶ� */
	int beta;  /**< �������� */
	////==========================Ԥ�ⲿ��==============================
	//��ȡģ��
	Ptr<ANN_MLP> model = StatModel::load<ANN_MLP>("D:/Class_Learning/DImageProcessing/TestInQt/TestInQt/model.xml");
	//cout << "��ȡģ�����!!!!!" << endl;
	
	char str[10];
	char n[10];
	for (int w = 0; w < num; ++w)
	{
			//��ȡ����ͼ��
			Mat test;
			sprintf(b, "D:\\Class_Learning\\DImageProcessing\\TestInQt\\TestInQt\\recognizepic\\%d.jpg", w);
			test = imread(b, 0);
			cv::resize(test, test, cv::Size(imageRows, imageCols), (0, 0), (0, 0), INTER_AREA);
			//imshow("0", test);
			//��ת
			for (int i = 0; i < test.rows; i++) {
				for (int j = 0; j < test.cols; j++) {
					test.at<uchar>(i, j) = 255 - test.at<uchar>(i, j);
				}
			}
			//imshow("1", test);
			//���ӶԱȶ�
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
			//��ֵ��
			threshold(test, test, 80, 255, 0); 
			
			//sprintf(n,"%d",w);
			//cvNamedWindow(n,0);
			//imshow(n, test);//��ʾ�����и������ͼƬ
			//������ͼ��ת��Ϊ1*192������
			Mat_<float>testMat(1, imageRows*imageCols);
			for (int i = 0; i < imageRows*imageCols; i++)
			{
				testMat.at<float>(0, i) = (float)test.at<uchar>(i / imageRows, i % imageRows);
			}
			Mat arr;
			//ʹ��ѵ���õ�modelԤ�����ͼ��
			model->predict(testMat, arr);
			//std::cout << "testMat: \n" << testMat << "\n" << std::endl;
			//std::cout << "dst: \n" << arr << "\n" << std::endl;
			double maxVal = 0;
			Point maxLoc;
			minMaxLoc(arr, NULL, &maxVal, NULL, &maxLoc);//��һ���������ҵ�ȫ����Сֵ��ȫ�����ֵ
			//std::cout << "���Խ����" << maxLoc.x << "���Ŷ�:" << maxVal * 100 << "%" << std::endl;
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
	
	for (int i = 0; i < num;i++) cout << "���Խ����" << str[num-1-i] << endl;
}


/////ANN������ѵ�����ִ���
////ÿһ��һ��ѵ��ͼƬ
	//float trainingData[classSum*imagesSum][imageRows*imageCols] = { {0} };
	////ѵ��������ǩ
	//float labels[classSum*imagesSum][classSum] = { {0} };
	//Mat src, resizeImg, trainImg;
	//for (int i = 0; i < classSum; i++)
	//{
	//	//Ŀ���ļ���·��
	//	std::string inPath = "D:\\MyDownloads\\vs2017_workspace\\qt_work\\work_1\\Qtwork1\\Qtwork1\\samples\\";
	//	char temp[256];
	//	int k = 0;
	//	sprintf_s(temp, "%d", i);
	//	inPath = inPath + temp + "\\*.png";
	//	//���ڲ��ҵľ��
	//	long long handle;
	//	struct _finddata_t fileinfo;
	//	//��һ�β���
	//	handle = _findfirst(inPath.c_str(), &fileinfo);
	//	if (handle == -1)
	//		return -1;
	//	do
	//	{
	//		//�ҵ����ļ����ļ���
	//		std::string imgname = "D:/MyDownloads/vs2017_workspace/qt_work/work_1/Qtwork1/Qtwork1/samples/";
	//		imgname = imgname + temp + "/" + fileinfo.name;
	//		src = imread(imgname, 0);
	//		if (src.empty())
	//		{
	//			std::cout << "can not load image \n" << std::endl;
	//			return -1;
	//		}
	//		//������ͼƬ��Сͳһת��Ϊ12*16
	//		resize(src, resizeImg, Size(imageRows, imageCols), (0, 0), (0, 0), INTER_AREA);
	//		threshold(resizeImg, trainImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//		for (int j = 0; j < imageRows*imageCols; j++)
	//		{
	//			trainingData[i*imagesSum + k][j] = (float)resizeImg.data[j];
	//		}
	//		// ���ñ�ǩ����
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
	////ѵ���������ݼ���Ӧ��ǩ
	//Mat trainingDataMat(classSum*imagesSum, imageRows*imageCols, CV_32FC1, trainingData);
	//Mat labelsMat(classSum*imagesSum, classSum, CV_32FC1, labels);
	////std::cout<<"trainingDataMat: \n"<<trainingDataMat<<"\n"<<std::endl;
	////std::cout<<"labelsMat: \n"<<labelsMat<<"\n"<<std::endl;
	////==========================ѵ������==============================

	//Ptr<ANN_MLP> model = ANN_MLP::create();
	//Mat layerSizes = (Mat_<int>(1, 5) << imageRows * imageCols, 128,128, 128, classSum);
	//model->setLayerSizes(layerSizes);
	//model->setTrainMethod(ANN_MLP::BACKPROP, 0.001, 0.1);
	//model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1.0, 1.0);
	//model->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001));

	//Ptr<TrainData> trainData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	//model->train(trainData);
	////����ѵ�����
	//model->save("D:\\MyDownloads\\vs2017_workspace\\qt_work\\work_1\\Qtwork1\\Qtwork1\\model\\model.xml");