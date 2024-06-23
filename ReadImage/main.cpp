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
//	// 确保 GDI+ 已初始化  
//	GdiplusStartupInput gdiplusStartupInput;
//	ULONG_PTR gdiplusToken;
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//
//	// 获取位图的尺寸  
//	UINT width = bitmap->GetWidth();
//	UINT height = bitmap->GetHeight();
//
//	// 创建 HBITMAP 兼容的设备上下文  
//	HDC hdc = CreateCompatibleDC(NULL);
//	HBITMAP hbitmap = CreateBitmap(width, height, 1, 24, NULL); // 假设为 24 位 RGB  
//	SelectObject(hdc, hbitmap);
//
//	// 锁定 Gdiplus::Bitmap 的位数据  
//	Rect rect(0, 0, width, height);
//	BitmapData bmpData;
//	Gdiplus::Status status = bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat24bppRGB, &bmpData);
//
//	if (status == Ok)
//	{
//		// 将位数据从 Gdiplus::Bitmap 复制到 HBITMAP  
//		// 注意：这里假设 Stride（扫描线宽度）与位图的宽度兼容，如果不兼容，则需要调整复制逻辑  
//		// 在这里，我们使用 BitBlt 进行复制，但如果 Stride 不匹配，你可能需要手动复制数据  
//		HDC hMemDC = CreateCompatibleDC(hdc);
//		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
//
//		// 如果 Stride 与位图宽度不匹配，则需要使用 GetDIBits 和 SetDIBitsToDevice 来手动处理  
//		// 这里我们假设 Stride 匹配，因此可以直接使用 BitBlt  
//		BitBlt(hMemDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
//
//		// 恢复之前选中的对象  
//		SelectObject(hMemDC, hOldBitmap);
//		DeleteDC(hMemDC);
//
//		// 解锁 Gdiplus::Bitmap  
//		bitmap->UnlockBits(&bmpData);
//	}
//	else
//	{
//		// 处理错误  
//		DeleteObject(hbitmap);
//		hbitmap = NULL;
//	}
//
//	// 删除设备上下文  
//	DeleteDC(hdc);
//
//	// 清理 GDI+  
//	GdiplusShutdown(gdiplusToken);
//
//	return hbitmap;
//}
//
//// 定义一个简单的颜色结构体  
//typedef struct CColor {
//	unsigned char r; // 红色分量  
//	unsigned char g; // 绿色分量  
//	unsigned char b; // 蓝色分量  
//	// 可以选择性地添加一个alpha分量  
//
//	// 构造函数，用于从ARGB值创建颜色  
//	CColor(unsigned char a = 255, unsigned char r_ = 0, unsigned char g_ = 0, unsigned char b_ = 0)
//		: r(r_), g(g_), b(b_) // 不包含alpha分量，如果需要可以添加  
//	{}
//
//	// 静态函数，用于从灰度值创建颜色  
//	static CColor FromGrayscale(unsigned char grayscale) {
//		return CColor(grayscale, grayscale, grayscale);
//	}
//}CColor;
//
//
//// 假设你有一个std::vector<BYTE>，其中每个BYTE是一个灰度像素值  
//void VectorToGdiplusBitmap(const std::vector<BYTE>& imageData, int width, int height, Gdiplus::Bitmap** bitmap)
//{
//	// 初始化GDI+  
//	GdiplusStartupInput gdiplusStartupInput;
//	ULONG_PTR gdiplusToken;
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//
//	// 创建Bitmap对象，灰度图像使用PixelFormat8bppIndexed  
//	*bitmap = new Gdiplus::Bitmap(width, height, PixelFormat8bppIndexed);
//
//	// 为Bitmap设置调色板（对于灰度图像，我们只需要一个从0到255的灰度调色板）  
//	//ColorPalette palette;
//	//(*bitmap)->SetPalette(&palette);
//
//	// 锁定Bitmap的像素数据  
//	BitmapData bmpData;
//	Rect rect(0, 0, width, height);
//	Gdiplus::Status status = (*bitmap)->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bmpData);
//
//	if (status == Ok)
//	{
//		// 将std::vector中的数据复制到Bitmap中  
//		// 注意：这里假设vector中的数据已经是按行顺序排列的  
//		// 并且Bitmap的Stride（每行的字节数）与vector中的数据兼容  
//		// 如果不兼容，你可能需要调整复制逻辑以处理Stride  
//		BYTE* pPixels = reinterpret_cast<BYTE*>(bmpData.Scan0);
//		for (int y = 0; y < height; ++y)
//		{
//			memcpy(pPixels + y * bmpData.Stride, &imageData[y * width], width * sizeof(BYTE));
//		}
//		// 解锁Bitmap  
//		(*bitmap)->UnlockBits(&bmpData);
//	}
//	else
//	{
//		// 处理错误...  
//		delete* bitmap;
//		*bitmap = nullptr;
//	}
//
//	// 你可以在这里继续使用*bitmap，但在程序结束前别忘了清理GDI+  
//}
//
//
//
//// 初始化GDI+  
//void InitGDIPlus() {
//	GdiplusStartupInput gdiplusStartupInput;
//	ULONG_PTR gdiplusToken;
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//}
//
//// 释放GDI+资源  
//void UninitGDIPlus() {
//	GdiplusShutdown(0);
//}
//
//// 灰度转换函数  
//Bitmap* ConvertToGrayscale(Bitmap* pBitmap) {
//	Bitmap* pGrayscaleBitmap = new Bitmap(pBitmap->GetWidth(), pBitmap->GetHeight(), PixelFormat24bppRGB);
//	Graphics graphics(pGrayscaleBitmap);
//
//	// 创建一个ColorMatrix对象  
//	ColorMatrix colorMatrix;
//
//	// 设置灰度转换矩阵  
//	// 灰度 = 0.3*R + 0.59*G + 0.11*B  
//	// 忽略Alpha通道  
//	colorMatrix.m[0][0] = 0.3f; colorMatrix.m[0][1] = 0.3f; colorMatrix.m[0][2] = 0.3f; colorMatrix.m[0][3] = 0.0f; colorMatrix.m[0][4] = 0.0f;
//	colorMatrix.m[1][0] = 0.59f; colorMatrix.m[1][1] = 0.59f; colorMatrix.m[1][2] = 0.59f; colorMatrix.m[1][3] = 0.0f; colorMatrix.m[1][4] = 0.0f;
//	colorMatrix.m[2][0] = 0.11f; colorMatrix.m[2][1] = 0.11f; colorMatrix.m[2][2] = 0.11f; colorMatrix.m[2][3] = 0.0f; colorMatrix.m[2][4] = 0.0f;
//	// Alpha通道保持不变  
//	colorMatrix.m[3][0] = 0.0f; colorMatrix.m[3][1] = 0.0f; colorMatrix.m[3][2] = 0.0f; colorMatrix.m[3][3] = 1.0f; colorMatrix.m[3][4] = 0.0f;
//
//	// 创建一个ImageAttributes对象并应用灰度矩阵  
//	ImageAttributes imageAttributes;
//	imageAttributes.SetColorMatrix(&colorMatrix);
//
//	// 绘制灰度图像  
//	graphics.DrawImage(pBitmap, Rect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight()),
//		0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), UnitPixel, &imageAttributes);
//
//	return pGrayscaleBitmap;
//}
//
//// 确保链接了GDI+库  
//// 在项目属性 -> 配置属性 -> 链接器 -> 输入 -> 附加依赖项中添加 "gdiplus.lib"
//
//
//// 假设我们有一个函数来创建和返回位图的句柄  
//HBITMAP CreateGrayBitmapFromBytes(const std::vector<BYTE>& rawValues, int width, int height) {
//	// 创建一个兼容的DC  
//	HDC hdc = GetDC(nullptr);
//	if (!hdc) {
//		std::cerr << "Failed to get device context." << std::endl;
//		return nullptr;
//	}
//
//	// 创建一个与屏幕DC兼容的位图  
//	HBITMAP hBitmap = CreateBitmap(width, height, 1, 8, nullptr);
//	if (!hBitmap) {
//		std::cerr << "Failed to create bitmap." << std::endl;
//		ReleaseDC(nullptr, hdc);
//		return nullptr;
//	}
//
//	// 创建一个与位图兼容的内存DC  
//	HDC hdcMem = CreateCompatibleDC(hdc);
//	if (!hdcMem) {
//		std::cerr << "Failed to create compatible memory DC." << std::endl;
//		DeleteObject(hBitmap);
//		ReleaseDC(nullptr, hdc);
//		return nullptr;
//	}
//
//	// 将位图选入内存DC  
//	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
//	if (!hOldBitmap) {
//		std::cerr << "Failed to select bitmap into memory DC." << std::endl;
//		DeleteDC(hdcMem);
//		DeleteObject(hBitmap);
//		ReleaseDC(nullptr, hdc);
//		return nullptr;
//	}
//
//	// 锁定位图的内存区域  
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
//	// 清理资源  
//	SelectObject(hdcMem, hOldBitmap);
//	DeleteDC(hdcMem);
//	ReleaseDC(nullptr, hdc);
//
//	return hBitmap; // 返回位图的句柄  
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
//	// 假设你有这个函数来将字节数组转换为灰度位图  
//	//HBITMAP* bitmap = ByteToGrayBitmap(vs, width, height);
//	// 转换为Gdiplus::Bitmap  
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
//	// 假设你有一个 Gdiplus::Bitmap* 变量 named gdiplusBitmap  
//// ...  
//
//// 转换为 HBITMAP  
//	HBITMAP hbitmap = GdiplusBitmapToHBitmap(bitmap);
//
//	// 使用 HBITMAP ...  
//
//	// 清理 HBITMAP  
//	//DeleteObject(hbitmap);
//
//
//	return &hbitmap;
//}
//
//int main() {
//	HBITMAP* bitmap = ReadImageFromMemory();
//	if (bitmap != nullptr) {
//		// 使用位图...  
//		// 记得在适当的时候释放位图资源  
//		// delete bitmap;  
//
//		InitGDIPlus();
//
//		// 加载原始图像  
//		//Bitmap bitmap(L"path_to_your_image.jpg");
//
//		Bitmap* ptr = new Bitmap(*bitmap, NULL);
//
//		// 转换为灰度图  
//		Bitmap* pGrayscaleBitmap = ConvertToGrayscale(ptr);
//
//		CLSID CLSID_JPEGFormat;
//		// 保存灰度图（可选）  
//		pGrayscaleBitmap->Save(L"grayscale_image.jpg", NULL);
//
//		// 释放资源  
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
	// 创建一个新的灰度Bitmap对象（8位索引颜色）  
	Bitmap* pGrayBitmap = new Bitmap(width, height, PixelFormat8bppIndexed);

	// 锁定位图的内存区域  
	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	pGrayBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bmpData);

	//获取图像参数
	stride = bmpData.Stride;  // 扫描线的宽度  
	int offset = stride - width;  // 显示宽度与扫描线宽度的间隙  
	void *iptr = bmpData.Scan0;  // 获取bmpData的内存起始位置  
	int scanBytes = stride * height;// 用stride宽度，表示这是内存区域的大小  

	//下面把原始的显示大小字节数组转换为内存中实际存放的字节数组
	int posScan = 0, posReal = 0;// 分别设置两个位置指针，指向源数组和目标数组  
	char* pixelValues = new char[scanBytes];  //为目标数组分配内存  

	for (int x = 0; x < height; x++)
	{
		//下面的循环节是模拟行扫描
		for (int y = 0; y < width; y++)
		{
			pixelValues[posScan++] = rgbData[posReal++];
		}
		posScan += offset;  //行扫描结束，要将目标位置指针移过那段“间隙”  
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

	//// 计算总像素数  
	//int totalPixels = width * height;

	//for (int y = 0; y < height; ++y) {
	//	for (int x = 0; x < width; ++x) {
	//		// 计算当前像素在rgbData中的索引  
	//		int index = y * stride + x * 3; // 注意这里使用了stride  

	//		// 确保index没有越界（尽管这通常是由图像数据的大小保证的）  
	//		if (index + 2 < stride * height) { // 检查是否越界（仅用于演示）  
	//			// 获取RGB值  
	//			BYTE red   = rgbData[index];
	//			BYTE green = rgbData[index + 1];
	//			BYTE blue  = rgbData[index + 2];

	//		// 计算灰度值  
	//			BYTE gray = (BYTE)((red * 0.3) + (green * 0.59) + (blue * 0.11));

	//			// 计算灰度图像中当前像素的索引  
	//			int grayIndex = y * stride + x; // 使用灰度图像的stride（或简单地使用宽度）  

	//			// 将灰度值写入灰度Bitmap的内存中  
	//			*((BYTE*)bmpData.Scan0 + grayIndex) = gray;
	//		}
	//	}
	//}


	// 解锁位图的内存区域  
	pGrayBitmap->UnlockBits(&bmpData);

	return pGrayBitmap;
}

// ...

int main()
{
	int width = 4024;
	int height = 3036;

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
		//OpenFileMapping(FILE_MAP_READ, FALSE, 0);
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
		void* pMapView = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
		if (pMapView == NULL) {
			std::cerr << "Failed to create map view of file." << std::endl;
			std::cout << GetLastError() << std::endl;
			CloseHandle(hMapFile);
			return 0;
		}
		else {
			char* ptr = (char*)pMapView;

			// 示例：保存为JPEG并设置质量  
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
			ULONG quality = 50; // 设置JPEG质量，范围从0到100  
			encoderParams.Parameter[0].Value = &quality;

			Bitmap* bitmap = ConvertToGrayscale(ptr, width, height, 4024);

			/*Image::Save(
				IN const WCHAR * filename,
				IN const CLSID * clsidEncoder,
				IN const EncoderParameters * encoderParams
			)*/

			bitmap->Save(L"temp.jpg", &jpegClsid, &encoderParams);
		}
		//完成后取消映射
		//UnmapViewOfFile(pData);
	}

	// ...  
	// 在不再需要GDI+时，关闭GDI+  
	GdiplusShutdown(gdiplusToken);

	// 关闭文件映射对象句柄  
	CloseHandle(hMapFile);

	return 0;
}