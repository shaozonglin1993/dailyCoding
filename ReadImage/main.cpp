#define _CRT_SECURE_NO_WARNINGS
//#include <windows.h>  
//#include <iostream>  
//#include <fstream>  
//#include <vector>  
//#include <gdiplus.h> 
//#include <comdef.h>
//#include <gdipluscolor.h>
//using namespace Gdiplus;
//
//#pragma comment(lib, "gdiplus.lib")
//
//
//HBITMAP GdiplusBitmapToHBitmap(Gdiplus::Bitmap* bitmap)
//{
//	// ȷ�� GDI+ �ѳ�ʼ��  
//	GdiplusStartupInput gdiplusStartupInput;
//	ULONG_PTR gdiplusToken;
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//
//	// ��ȡλͼ�ĳߴ�  
//	UINT width = bitmap->GetWidth();
//	UINT height = bitmap->GetHeight();
//
//	// ���� HBITMAP ���ݵ��豸������  
//	HDC hdc = CreateCompatibleDC(NULL);
//	HBITMAP hbitmap = CreateBitmap(width, height, 1, 24, NULL); // ����Ϊ 24 λ RGB  
//	SelectObject(hdc, hbitmap);
//
//	// ���� Gdiplus::Bitmap ��λ����  
//	Rect rect(0, 0, width, height);
//	BitmapData bmpData;
//	Gdiplus::Status status = bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat24bppRGB, &bmpData);
//
//	if (status == Ok)
//	{
//		// ��λ���ݴ� Gdiplus::Bitmap ���Ƶ� HBITMAP  
//		// ע�⣺������� Stride��ɨ���߿�ȣ���λͼ�Ŀ�ȼ��ݣ���������ݣ�����Ҫ���������߼�  
//		// ���������ʹ�� BitBlt ���и��ƣ������ Stride ��ƥ�䣬�������Ҫ�ֶ���������  
//		HDC hMemDC = CreateCompatibleDC(hdc);
//		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
//
//		// ��� Stride ��λͼ��Ȳ�ƥ�䣬����Ҫʹ�� GetDIBits �� SetDIBitsToDevice ���ֶ�����  
//		// �������Ǽ��� Stride ƥ�䣬��˿���ֱ��ʹ�� BitBlt  
//		BitBlt(hMemDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
//
//		// �ָ�֮ǰѡ�еĶ���  
//		SelectObject(hMemDC, hOldBitmap);
//		DeleteDC(hMemDC);
//
//		// ���� Gdiplus::Bitmap  
//		bitmap->UnlockBits(&bmpData);
//	}
//	else
//	{
//		// �������  
//		DeleteObject(hbitmap);
//		hbitmap = NULL;
//	}
//
//	// ɾ���豸������  
//	DeleteDC(hdc);
//
//	// ���� GDI+  
//	GdiplusShutdown(gdiplusToken);
//
//	return hbitmap;
//}
//
//// ����һ���򵥵���ɫ�ṹ��  
//typedef struct CColor {
//	unsigned char r; // ��ɫ����  
//	unsigned char g; // ��ɫ����  
//	unsigned char b; // ��ɫ����  
//	// ����ѡ���Ե����һ��alpha����  
//
//	// ���캯�������ڴ�ARGBֵ������ɫ  
//	CColor(unsigned char a = 255, unsigned char r_ = 0, unsigned char g_ = 0, unsigned char b_ = 0)
//		: r(r_), g(g_), b(b_) // ������alpha�����������Ҫ�������  
//	{}
//
//	// ��̬���������ڴӻҶ�ֵ������ɫ  
//	static CColor FromGrayscale(unsigned char grayscale) {
//		return CColor(grayscale, grayscale, grayscale);
//	}
//}CColor;
//
//
//// ��������һ��std::vector<BYTE>������ÿ��BYTE��һ���Ҷ�����ֵ  
//void VectorToGdiplusBitmap(const std::vector<BYTE>& imageData, int width, int height, Gdiplus::Bitmap** bitmap)
//{
//	// ��ʼ��GDI+  
//	GdiplusStartupInput gdiplusStartupInput;
//	ULONG_PTR gdiplusToken;
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//
//	// ����Bitmap���󣬻Ҷ�ͼ��ʹ��PixelFormat8bppIndexed  
//	*bitmap = new Gdiplus::Bitmap(width, height, PixelFormat8bppIndexed);
//
//	// ΪBitmap���õ�ɫ�壨���ڻҶ�ͼ������ֻ��Ҫһ����0��255�ĻҶȵ�ɫ�壩  
//	//ColorPalette palette;
//	//(*bitmap)->SetPalette(&palette);
//
//	// ����Bitmap����������  
//	BitmapData bmpData;
//	Rect rect(0, 0, width, height);
//	Gdiplus::Status status = (*bitmap)->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bmpData);
//
//	if (status == Ok)
//	{
//		// ��std::vector�е����ݸ��Ƶ�Bitmap��  
//		// ע�⣺�������vector�е������Ѿ��ǰ���˳�����е�  
//		// ����Bitmap��Stride��ÿ�е��ֽ�������vector�е����ݼ���  
//		// ��������ݣ��������Ҫ���������߼��Դ���Stride  
//		BYTE* pPixels = reinterpret_cast<BYTE*>(bmpData.Scan0);
//		for (int y = 0; y < height; ++y)
//		{
//			memcpy(pPixels + y * bmpData.Stride, &imageData[y * width], width * sizeof(BYTE));
//		}
//		// ����Bitmap  
//		(*bitmap)->UnlockBits(&bmpData);
//	}
//	else
//	{
//		// �������...  
//		delete* bitmap;
//		*bitmap = nullptr;
//	}
//
//	// ��������������ʹ��*bitmap�����ڳ������ǰ����������GDI+  
//}
//
//
//
//// ��ʼ��GDI+  
//void InitGDIPlus() {
//	GdiplusStartupInput gdiplusStartupInput;
//	ULONG_PTR gdiplusToken;
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//}
//
//// �ͷ�GDI+��Դ  
//void UninitGDIPlus() {
//	GdiplusShutdown(0);
//}
//
//// �Ҷ�ת������  
//Bitmap* ConvertToGrayscale(Bitmap* pBitmap) {
//	Bitmap* pGrayscaleBitmap = new Bitmap(pBitmap->GetWidth(), pBitmap->GetHeight(), PixelFormat24bppRGB);
//	Graphics graphics(pGrayscaleBitmap);
//
//	// ����һ��ColorMatrix����  
//	ColorMatrix colorMatrix;
//
//	// ���ûҶ�ת������  
//	// �Ҷ� = 0.3*R + 0.59*G + 0.11*B  
//	// ����Alphaͨ��  
//	colorMatrix.m[0][0] = 0.3f; colorMatrix.m[0][1] = 0.3f; colorMatrix.m[0][2] = 0.3f; colorMatrix.m[0][3] = 0.0f; colorMatrix.m[0][4] = 0.0f;
//	colorMatrix.m[1][0] = 0.59f; colorMatrix.m[1][1] = 0.59f; colorMatrix.m[1][2] = 0.59f; colorMatrix.m[1][3] = 0.0f; colorMatrix.m[1][4] = 0.0f;
//	colorMatrix.m[2][0] = 0.11f; colorMatrix.m[2][1] = 0.11f; colorMatrix.m[2][2] = 0.11f; colorMatrix.m[2][3] = 0.0f; colorMatrix.m[2][4] = 0.0f;
//	// Alphaͨ�����ֲ���  
//	colorMatrix.m[3][0] = 0.0f; colorMatrix.m[3][1] = 0.0f; colorMatrix.m[3][2] = 0.0f; colorMatrix.m[3][3] = 1.0f; colorMatrix.m[3][4] = 0.0f;
//
//	// ����һ��ImageAttributes����Ӧ�ûҶȾ���  
//	ImageAttributes imageAttributes;
//	imageAttributes.SetColorMatrix(&colorMatrix);
//
//	// ���ƻҶ�ͼ��  
//	graphics.DrawImage(pBitmap, Rect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight()),
//		0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), UnitPixel, &imageAttributes);
//
//	return pGrayscaleBitmap;
//}
//
//// ȷ��������GDI+��  
//// ����Ŀ���� -> �������� -> ������ -> ���� -> ��������������� "gdiplus.lib"
//
//
//// ����������һ�������������ͷ���λͼ�ľ��  
//HBITMAP CreateGrayBitmapFromBytes(const std::vector<BYTE>& rawValues, int width, int height) {
//	// ����һ�����ݵ�DC  
//	HDC hdc = GetDC(nullptr);
//	if (!hdc) {
//		std::cerr << "Failed to get device context." << std::endl;
//		return nullptr;
//	}
//
//	// ����һ������ĻDC���ݵ�λͼ  
//	HBITMAP hBitmap = CreateBitmap(width, height, 1, 8, nullptr);
//	if (!hBitmap) {
//		std::cerr << "Failed to create bitmap." << std::endl;
//		ReleaseDC(nullptr, hdc);
//		return nullptr;
//	}
//
//	// ����һ����λͼ���ݵ��ڴ�DC  
//	HDC hdcMem = CreateCompatibleDC(hdc);
//	if (!hdcMem) {
//		std::cerr << "Failed to create compatible memory DC." << std::endl;
//		DeleteObject(hBitmap);
//		ReleaseDC(nullptr, hdc);
//		return nullptr;
//	}
//
//	// ��λͼѡ���ڴ�DC  
//	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
//	if (!hOldBitmap) {
//		std::cerr << "Failed to select bitmap into memory DC." << std::endl;
//		DeleteDC(hdcMem);
//		DeleteObject(hBitmap);
//		ReleaseDC(nullptr, hdc);
//		return nullptr;
//	}
//
//	// ����λͼ���ڴ�����  
//	BITMAPINFO bitmapInfo = { 0 };
//	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	bitmapInfo.bmiHeader.biWidth = width;
//	bitmapInfo.bmiHeader.biHeight = height;
//	bitmapInfo.bmiHeader.biPlanes = 1;
//	bitmapInfo.bmiHeader.biBitCount = 8;
//	bitmapInfo.bmiHeader.biCompression = BI_RGB;
//	VOID* pBits;
//	if (SetDIBitsToDevice(hdcMem, 0, 0, width, height, 0, 0, 0, height, rawValues.data(), &bitmapInfo, DIB_RGB_COLORS) == 0) {
//		std::cerr << "Failed to set bits to device." << std::endl;
//		SelectObject(hdcMem, hOldBitmap);
//		DeleteDC(hdcMem);
//		DeleteObject(hBitmap);
//		ReleaseDC(nullptr, hdc);
//		return nullptr;
//	}
//
//	// ������Դ  
//	SelectObject(hdcMem, hOldBitmap);
//	DeleteDC(hdcMem);
//	ReleaseDC(nullptr, hdc);
//
//	return hBitmap; // ����λͼ�ľ��  
//}
//
//HBITMAP* ReadImageFromMemory() {
//	const int width = 4024;
//	const int height = 3036;
//	const char* mmapName = "RawImgMap";
//	const char* mutexName = "RawImgMapMutex";
//
//	HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, mmapName);
//	if (hMapFile == NULL) {
//		std::cerr << "Failed to open file mapping." << std::endl;
//		return nullptr;
//	}
//
//	HANDLE hMutex = CreateMutex(NULL, FALSE, mutexName);
//	if (hMutex == NULL) {
//		std::cerr << "Failed to create mutex." << std::endl;
//		CloseHandle(hMapFile);
//		return nullptr;
//	}
//
//	//WaitForSingleObject(hMutex, INFINITE);
//
//	LPVOID pView = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, width * height);
//	if (pView == NULL) {
//		std::cerr << "Failed to map view of file." << std::endl;
//		ReleaseMutex(hMutex);
//		CloseHandle(hMapFile);
//		CloseHandle(hMutex);
//		return nullptr;
//	}
//
//	std::vector<byte> vs(width * height);
//	memcpy(vs.data(), pView, width * height);
//
//	UnmapViewOfFile(pView);
//	ReleaseMutex(hMutex);
//	CloseHandle(hMapFile);
//	CloseHandle(hMutex);
//
//	// ��������������������ֽ�����ת��Ϊ�Ҷ�λͼ  
//	//HBITMAP* bitmap = ByteToGrayBitmap(vs, width, height);
//	// ת��ΪGdiplus::Bitmap  
//	Gdiplus::Bitmap* bitmap = nullptr;
//	VectorToGdiplusBitmap(vs, width, height, &bitmap);
//
//	FILE* f = fopen("photo.jpg", "w");
//	fwrite(vs.data(), vs.size(), 1, f);
//	fclose(f);
//
//	if (bitmap == nullptr) {
//		std::cerr << "Failed to convert to grayscale bitmap." << std::endl;
//		return nullptr;
//	}
//
//	// ��������һ�� Gdiplus::Bitmap* ���� named gdiplusBitmap  
//// ...  
//
//// ת��Ϊ HBITMAP  
//	HBITMAP hbitmap = GdiplusBitmapToHBitmap(bitmap);
//
//	// ʹ�� HBITMAP ...  
//
//	// ���� HBITMAP  
//	//DeleteObject(hbitmap);
//
//
//	return &hbitmap;
//}
//
//int main() {
//	HBITMAP* bitmap = ReadImageFromMemory();
//	if (bitmap != nullptr) {
//		// ʹ��λͼ...  
//		// �ǵ����ʵ���ʱ���ͷ�λͼ��Դ  
//		// delete bitmap;  
//
//		InitGDIPlus();
//
//		// ����ԭʼͼ��  
//		//Bitmap bitmap(L"path_to_your_image.jpg");
//
//		Bitmap* ptr = new Bitmap(*bitmap, NULL);
//
//		// ת��Ϊ�Ҷ�ͼ  
//		Bitmap* pGrayscaleBitmap = ConvertToGrayscale(ptr);
//
//		CLSID CLSID_JPEGFormat;
//		// ����Ҷ�ͼ����ѡ��  
//		pGrayscaleBitmap->Save(L"grayscale_image.jpg", NULL);
//
//		// �ͷ���Դ  
//		delete pGrayscaleBitmap;
//		delete ptr;
//		UninitGDIPlus();
//	}
//
//	return 0;
//}


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



Bitmap* ConvertToGrayscale(char* rgbData, int width, int height, int stride)
{
	// ����һ���µĻҶ�Bitmap����8λ������ɫ��  
	Bitmap* pGrayBitmap = new Bitmap(width, height, PixelFormat8bppIndexed);

	// ����λͼ���ڴ�����  
	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	pGrayBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bmpData);

	//��ȡͼ�����
	stride = bmpData.Stride;  // ɨ���ߵĿ��  
	int offset = stride - width;  // ��ʾ�����ɨ���߿�ȵļ�϶  
	void *iptr = bmpData.Scan0;  // ��ȡbmpData���ڴ���ʼλ��  
	int scanBytes = stride * height;// ��stride��ȣ���ʾ�����ڴ�����Ĵ�С  

	//�����ԭʼ����ʾ��С�ֽ�����ת��Ϊ�ڴ���ʵ�ʴ�ŵ��ֽ�����
	int posScan = 0, posReal = 0;// �ֱ���������λ��ָ�룬ָ��Դ�����Ŀ������  
	char* pixelValues = new char[scanBytes];  //ΪĿ����������ڴ�  

	for (int x = 0; x < height; x++)
	{
		//�����ѭ������ģ����ɨ��
		for (int y = 0; y < width; y++)
		{
			pixelValues[posScan++] = rgbData[posReal++];
		}
		posScan += offset;  //��ɨ�������Ҫ��Ŀ��λ��ָ���ƹ��ǶΡ���϶��  
	}

	memcpy(iptr, pixelValues, scanBytes);

	ColorPalette* tempPalette = (ColorPalette*)(malloc(sizeof(ColorPalette) + (256 - 1) * sizeof(ARGB)));
	Bitmap *tempBmp = new Bitmap(1, 1, PixelFormat8bppIndexed);
	tempBmp->GetPalette(tempPalette, 0);
	for (unsigned int i = 0; i < 256; i++)
	{
		printf("%ud %d\n", tempPalette->Count, i);
		tempPalette->Entries[i] = Color::MakeARGB(255, i, i, i);
	}

	pGrayBitmap->SetPalette(tempPalette);

	//// ������������  
	//int totalPixels = width * height;

	//for (int y = 0; y < height; ++y) {
	//	for (int x = 0; x < width; ++x) {
	//		// ���㵱ǰ������rgbData�е�����  
	//		int index = y * stride + x * 3; // ע������ʹ����stride  

	//		// ȷ��indexû��Խ�磨������ͨ������ͼ�����ݵĴ�С��֤�ģ�  
	//		if (index + 2 < stride * height) { // ����Ƿ�Խ�磨��������ʾ��  
	//			// ��ȡRGBֵ  
	//			BYTE red   = rgbData[index];
	//			BYTE green = rgbData[index + 1];
	//			BYTE blue  = rgbData[index + 2];

	//		// ����Ҷ�ֵ  
	//			BYTE gray = (BYTE)((red * 0.3) + (green * 0.59) + (blue * 0.11));

	//			// ����Ҷ�ͼ���е�ǰ���ص�����  
	//			int grayIndex = y * stride + x; // ʹ�ûҶ�ͼ���stride����򵥵�ʹ�ÿ�ȣ�  

	//			// ���Ҷ�ֵд��Ҷ�Bitmap���ڴ���  
	//			*((BYTE*)bmpData.Scan0 + grayIndex) = gray;
	//		}
	//	}
	//}


	// ����λͼ���ڴ�����  
	pGrayBitmap->UnlockBits(&bmpData);

	return pGrayBitmap;
}

// ...

int main()
{
	int width = 4024;
	int height = 3036;

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
		//OpenFileMapping(FILE_MAP_READ, FALSE, 0);
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
		void* pMapView = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
		if (pMapView == NULL) {
			std::cerr << "Failed to create map view of file." << std::endl;
			std::cout << GetLastError() << std::endl;
			CloseHandle(hMapFile);
			return 0;
		}
		else {
			char* ptr = (char*)pMapView;

			// ʾ��������ΪJPEG����������  
			CLSID jpegClsid;
			UINT num = 0, size = 0;
			GetImageEncodersSize(&num, &size);
			ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
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

			Bitmap* bitmap = ConvertToGrayscale(ptr, width, height, 4024);

			/*Image::Save(
				IN const WCHAR * filename,
				IN const CLSID * clsidEncoder,
				IN const EncoderParameters * encoderParams
			)*/

			bitmap->Save(L"temp.jpg", &jpegClsid, &encoderParams);
		}
		//��ɺ�ȡ��ӳ��
		//UnmapViewOfFile(pData);
	}

	// ...  
	// �ڲ�����ҪGDI+ʱ���ر�GDI+  
	GdiplusShutdown(gdiplusToken);

	// �ر��ļ�ӳ�������  
	CloseHandle(hMapFile);

	return 0;
}