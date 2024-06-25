#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <Stdio.h>
#include <Objbase.h>
#include <Gdiplus.h>
#include <GdiPlusEnums.h>
#include <comdef.h>
#include <gdiplusinit.h>
using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")



std::shared_ptr<Bitmap> ConvertToGrayscale(char* rgbData, int width, int height, int stride)
{
	// 创建一个新的灰度Bitmap对象（8位索引颜色）  
	//Bitmap* pGrayBitmap = new Bitmap(width, height, PixelFormat8bppIndexed);

	std::shared_ptr<Bitmap> pGrayBitmap = std::make_shared<Bitmap>(width, height, PixelFormat8bppIndexed);

	// 锁定位图的内存区域  
	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	pGrayBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bmpData);

	//获取图像参数
	stride = bmpData.Stride;         // 扫描线的宽度  
	int offset = stride - width;     // 显示宽度与扫描线宽度的间隙  
	void *iptr = bmpData.Scan0;      // 获取bmpData的内存起始位置  
	int scanBytes = stride * height; // 用stride宽度，表示这是内存区域的大小  

	//下面把原始的显示大小字节数组转换为内存中实际存放的字节数组
	int posScan = 0, posReal = 0;             // 分别设置两个位置指针，指向源数组和目标数组  
	char* pixelValues = new char[scanBytes];  //为目标数组分配内存  
	if (pixelValues != NULL) {
		memset(pixelValues, 0, scanBytes);
		for (int x = 0; x < height; x++)
		{
			//下面的循环节是模拟行扫描
			for (int y = 0; y < width; y++)
			{
				if (posScan < scanBytes) {
					pixelValues[posScan++] = rgbData[posReal++];
				}
				else {
					//delete pGrayBitmap;
					//pGrayBitmap = NULL;
					return pGrayBitmap;
				}
			}
			posScan += offset;  //行扫描结束，要将目标位置指针移过那段“间隙”  
		}

		memcpy(iptr, pixelValues, scanBytes);

		ColorPalette* tempPalette = (ColorPalette*)(malloc(sizeof(ColorPalette) + (256) * sizeof(ARGB)));
		if (tempPalette != NULL) {
			// 假设ColorPalette有一个成员变量Count来表示颜色条目的数量
			tempPalette->Count = 256;
			// 假设Color::MakeARGB函数接受整数参数而不是浮点数，并返回ARGB值
			for (unsigned int i = 0; i < 256; i++)
			{
				// 对于灰度调色板，将RGB值设置为相同的灰度级别 
				tempPalette->Entries[i] = Color::MakeARGB(255, i, i, i);
			}

			pGrayBitmap->SetPalette(tempPalette);
		}
		// 解锁位图的内存区域  
		pGrayBitmap->UnlockBits(&bmpData);

		//--- clear
		free(tempPalette);
		tempPalette = NULL;

		delete[] pixelValues;
		pixelValues = NULL;
	}

	return pGrayBitmap;
}

// ...

int main()
{
	int width = 4024;
	int height = 3036;
	int index = 0;

	// 初始化GDI+  
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//HANDLE hFile = CreateFile(
	//	"temp",
	//	GENERIC_READ,
	//	0,
	//	NULL,
	//	CREATE_ALWAYS,
	//	FILE_ATTRIBUTE_NORMAL,
	//	NULL);
	//if (hFile == INVALID_HANDLE_VALUE) {
	//	std::cerr << "Failed to create file." << std::endl;
	//	return 0;
	//}

	/*HANDLE CreateFileMappingA(
		[in]           HANDLE                hFile,
		[in, optional] LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
		[in]           DWORD                 flProtect,
		[in]           DWORD                 dwMaximumSizeHigh,
		[in]           DWORD                 dwMaximumSizeLow,
		[in, optional] LPCSTR                lpName
	);*/
	HANDLE hMapFile =
	//CreateFileMapping(
	//	INVALID_HANDLE_VALUE/*hFile*/, // 映射一个匿名对象 
	//	NULL,                 // 使用默认安全特性  
	//	PAGE_READONLY,       // 读写权限  
	//	0,                    // 最大大小的高32位（0表示整个文件）
	//	4096,                 // 最大大小的低32位（这里为4KB） 
	//	"RawImgMap");          // 文件映射对象的名称
		OpenFileMapping(FILE_MAP_ALL_ACCESS,
			FALSE,
			"RawImgMap"
		);
	
	if (hMapFile == NULL) {
		std::cerr << "Failed to open file mapping." << std::endl;
		std::cout << GetLastError() << std::endl;
		return 0;
	}
	else {
		// 文件映射对象创建成功  
		std::cout << "File mapping object created successfully!" << std::endl;
		// 在这里可以使用文件映射对象进行进程间的数据共享等操作  
		// ... 
	/*LPVOID MapViewOfFile(
		[in] HANDLE hFileMappingObject,
		[in] DWORD  dwDesiredAccess,
		[in] DWORD  dwFileOffsetHigh,
		[in] DWORD  dwFileOffsetLow,
		[in] SIZE_T dwNumberOfBytesToMap
	);*/

		//HANDLE mutexHandle = OpenMutex(NULL, FALSE, "RawImgMapMutex");
		//if (mutexHandle == NULL) {
		//	std::cerr << "Failed to create map view of file." << std::endl;
		//	std::cout << GetLastError() << std::endl;
		//	return 0;
		//}
		
			//WaitForSingleObject(mutexHandle, INFINITE);
			void* pMapView = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
			if (pMapView == NULL) {
				std::cerr << "Failed to create map view of file." << std::endl;
				std::cout << GetLastError() << std::endl;
				CloseHandle(hMapFile);
				return 0;
			}
			else {
				char* pChMapView = (char*)pMapView;

				// 示例：保存为JPEG并设置质量  
				CLSID jpegClsid;
				UINT num = 0, size = 0;
				GetImageEncodersSize(&num, &size);
				ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
				if (pImageCodecInfo != NULL) {
					GetImageEncoders(num, size, pImageCodecInfo);
					for (UINT j = 0; j < num; ++j) {
						if (wcscmp(pImageCodecInfo[j].MimeType, L"image/jpeg") == 0) {
							jpegClsid = pImageCodecInfo[j].Clsid;
							break;
						}
					}
					free(pImageCodecInfo);

					EncoderParameters encoderParams;
					encoderParams.Count = 1;
					encoderParams.Parameter[0].Guid = EncoderQuality;
					encoderParams.Parameter[0].Type = EncoderParameterValueTypeLong;
					encoderParams.Parameter[0].NumberOfValues = 1;
					ULONG quality = 50; // 设置JPEG质量，范围从0到100  
					encoderParams.Parameter[0].Value = &quality;
					while (true) {
						//Bitmap* bitmap = ConvertToGrayscale(pChMapView, width, height, 4024);
						std::shared_ptr<Bitmap> bitmap = ConvertToGrayscale(pChMapView, width, height, 4024);

						/*Image::Save(
							IN const WCHAR * filename,
							IN const CLSID * clsidEncoder,
							IN const EncoderParameters * encoderParams
						)*/

						bitmap->Save(L"temp.jpg", &jpegClsid, &encoderParams);
						//ReleaseMutex(mutexHandle);
						Sleep(10);
					}
					//delete bitmap;
					//bitmap = NULL;
				}
			//完成后取消映射
			UnmapViewOfFile(pMapView);
		}
	}

	// ...  
	// 在不再需要GDI+时，关闭GDI+  
	GdiplusShutdown(gdiplusToken);

	// 关闭文件映射对象句柄  
	CloseHandle(hMapFile);
	

	return 0;
}