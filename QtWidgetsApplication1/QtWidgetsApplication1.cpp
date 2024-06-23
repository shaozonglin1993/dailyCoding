#include "QtWidgetsApplication1.h"
#include <QFile>  
#include <QIODevice>  
#include <QMutex>  
#include <QSharedMemory>  
#include <QImage>  
#include <QColor>  
#include <QString>
#include <QDebug>
#include <windows.h>
#include <iostream>
using namespace std;

// 如果你使用OpenCV进行图像处理，还需要包含OpenCV的头文件  
// #include <opencv2/opencv.hpp>

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	readImageFromSharedMemory("RawImgMap", 4024, 3036);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

QImage QtWidgetsApplication1::readImageFromSharedMemory(const QString& sharedMemoryName, int width, int height) {
	const char* mmapName = "RawImgMap";
	const char* mutexName = "RawImgMapMutex";

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, (LPCWSTR)mmapName);
	if (hMapFile == NULL) {
		std::cerr << "Failed to open file mapping." << std::endl;
		return QImage();
	}

	HANDLE hMutex = CreateMutex(NULL, FALSE, (LPCWSTR)mutexName);
	if (hMutex == NULL) {
		std::cerr << "Failed to create mutex." << std::endl;
		CloseHandle(hMapFile);
		return QImage();
	}


	LPVOID pView = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, width * height);
	if (pView == NULL) {
		std::cerr << "Failed to map view of file." << std::endl;
		ReleaseMutex(hMutex);
		CloseHandle(hMapFile);
		CloseHandle(hMutex);
		return  QImage();
	}

	// 锁定共享内存段以防止并发访问  
	//QMutex mutex(QString("%1Mutex").arg(sharedMemoryName));
	//mutex.lock();

	// 读取共享内存数据  
	//const char* data = static_cast<const char*>(sharedMemory.data());
	//if (!data) {
	//	// 处理错误  
	//	qWarning() << "Failed to get data from shared memory segment";
	//	//mutex.unlock();
	//	sharedMemory.detach();
	//	return QImage();
	//}

	// 创建一个QImage对象，假设数据是RGB格式（你可能需要根据实际情况调整）  
	//QImage image((uchar*)data, width, height, QImage::Format_RGB888);

	// 如果需要，将QImage转换为灰度图像  
	// 这里使用了一个简单的灰度转换算法，但你可以使用更复杂的算法  
	//QImage grayImage(width, height, QImage::Format_Grayscale8);
	//for (int y = 0; y < height; ++y) {
	//	QRgb* scanLine = reinterpret_cast<QRgb*>(image.scanLine(y));
	//	uchar* grayScanLine = grayImage.scanLine(y);
	//	for (int x = 0; x < width; ++x) {
	//		QRgb rgb = scanLine[x];
	//		int gray = qGray(rgb); // Qt的qGray函数计算灰度值  
	//		grayScanLine[x] = gray;
	//	}
	//}

	// 解锁共享内存段并分离  
	//mutex.unlock();
	//sharedMemory.detach();

	//grayImage.save("t.jpg");

	// 返回灰度图像  
	//return grayImage;
}