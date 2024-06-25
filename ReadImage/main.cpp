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
	// ����һ���µĻҶ�Bitmap����8λ������ɫ��  
	//Bitmap* pGrayBitmap = new Bitmap(width, height, PixelFormat8bppIndexed);

	std::shared_ptr<Bitmap> pGrayBitmap = std::make_shared<Bitmap>(width, height, PixelFormat8bppIndexed);

	// ����λͼ���ڴ�����  
	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	pGrayBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bmpData);

	//��ȡͼ�����
	stride = bmpData.Stride;         // ɨ���ߵĿ��  
	int offset = stride - width;     // ��ʾ�����ɨ���߿�ȵļ�϶  
	void *iptr = bmpData.Scan0;      // ��ȡbmpData���ڴ���ʼλ��  
	int scanBytes = stride * height; // ��stride��ȣ���ʾ�����ڴ�����Ĵ�С  

	//�����ԭʼ����ʾ��С�ֽ�����ת��Ϊ�ڴ���ʵ�ʴ�ŵ��ֽ�����
	int posScan = 0, posReal = 0;             // �ֱ���������λ��ָ�룬ָ��Դ�����Ŀ������  
	char* pixelValues = new char[scanBytes];  //ΪĿ����������ڴ�  
	if (pixelValues != NULL) {
		memset(pixelValues, 0, scanBytes);
		for (int x = 0; x < height; x++)
		{
			//�����ѭ������ģ����ɨ��
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
			posScan += offset;  //��ɨ�������Ҫ��Ŀ��λ��ָ���ƹ��ǶΡ���϶��  
		}

		memcpy(iptr, pixelValues, scanBytes);

		ColorPalette* tempPalette = (ColorPalette*)(malloc(sizeof(ColorPalette) + (256) * sizeof(ARGB)));
		if (tempPalette != NULL) {
			// ����ColorPalette��һ����Ա����Count����ʾ��ɫ��Ŀ������
			tempPalette->Count = 256;
			// ����Color::MakeARGB���������������������Ǹ�������������ARGBֵ
			for (unsigned int i = 0; i < 256; i++)
			{
				// ���ڻҶȵ�ɫ�壬��RGBֵ����Ϊ��ͬ�ĻҶȼ��� 
				tempPalette->Entries[i] = Color::MakeARGB(255, i, i, i);
			}

			pGrayBitmap->SetPalette(tempPalette);
		}
		// ����λͼ���ڴ�����  
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

	// ��ʼ��GDI+  
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
	//	INVALID_HANDLE_VALUE/*hFile*/, // ӳ��һ���������� 
	//	NULL,                 // ʹ��Ĭ�ϰ�ȫ����  
	//	PAGE_READONLY,       // ��дȨ��  
	//	0,                    // ����С�ĸ�32λ��0��ʾ�����ļ���
	//	4096,                 // ����С�ĵ�32λ������Ϊ4KB�� 
	//	"RawImgMap");          // �ļ�ӳ����������
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
		// �ļ�ӳ����󴴽��ɹ�  
		std::cout << "File mapping object created successfully!" << std::endl;
		// ���������ʹ���ļ�ӳ�������н��̼�����ݹ���Ȳ���  
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

				// ʾ��������ΪJPEG����������  
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
					ULONG quality = 50; // ����JPEG��������Χ��0��100  
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
			//��ɺ�ȡ��ӳ��
			UnmapViewOfFile(pMapView);
		}
	}

	// ...  
	// �ڲ�����ҪGDI+ʱ���ر�GDI+  
	GdiplusShutdown(gdiplusToken);

	// �ر��ļ�ӳ�������  
	CloseHandle(hMapFile);
	

	return 0;
}