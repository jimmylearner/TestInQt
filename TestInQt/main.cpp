#include "TestInQt.h"
#include <QtWidgets/QApplication>
#include <opencv2/opencv.hpp>
#include <QTextCodec>
#pragma execution_character_set("utf-8")

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    TestInQt w;
    w.show();
    return a.exec();
}
