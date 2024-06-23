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

// �����ʹ��OpenCV����ͼ��������Ҫ����OpenCV��ͷ�ļ�  
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

	// ���������ڴ���Է�ֹ��������  
	//QMutex mutex(QString("%1Mutex").arg(sharedMemoryName));
	//mutex.lock();

	// ��ȡ�����ڴ�����  
	//const char* data = static_cast<const char*>(sharedMemory.data());
	//if (!data) {
	//	// �������  
	//	qWarning() << "Failed to get data from shared memory segment";
	//	//mutex.unlock();
	//	sharedMemory.detach();
	//	return QImage();
	//}

	// ����һ��QImage���󣬼���������RGB��ʽ���������Ҫ����ʵ�����������  
	//QImage image((uchar*)data, width, height, QImage::Format_RGB888);

	// �����Ҫ����QImageת��Ϊ�Ҷ�ͼ��  
	// ����ʹ����һ���򵥵ĻҶ�ת���㷨���������ʹ�ø����ӵ��㷨  
	//QImage grayImage(width, height, QImage::Format_Grayscale8);
	//for (int y = 0; y < height; ++y) {
	//	QRgb* scanLine = reinterpret_cast<QRgb*>(image.scanLine(y));
	//	uchar* grayScanLine = grayImage.scanLine(y);
	//	for (int x = 0; x < width; ++x) {
	//		QRgb rgb = scanLine[x];
	//		int gray = qGray(rgb); // Qt��qGray��������Ҷ�ֵ  
	//		grayScanLine[x] = gray;
	//	}
	//}

	// ���������ڴ�β�����  
	//mutex.unlock();
	//sharedMemory.detach();

	//grayImage.save("t.jpg");

	// ���ػҶ�ͼ��  
	//return grayImage;
}