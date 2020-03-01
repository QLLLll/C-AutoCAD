#include <windows.h>
#include <iostream>
#include "stdio.h"
#include "../qrencode/qrencode.h"
using namespace std;
#pragma warning(disable:4099)
#ifdef _DEBUG
#pragma comment(lib,"qrencode.lib")
#else
#pragma comment(lib,"qrencode.lib")
#endif
int main()
{
	char*           szSourceSring = "https://www.cnblogs.com/HelloQLQ/";
	unsigned int    unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char*  pRGBData, *pSourceData, *pDestData;
	QRcode*         pQRC;
	FILE*           f;
	if (pQRC = QRcode_encodeString(szSourceSring, 1, QR_ECLEVEL_L, QR_MODE_8, 1))
	{
		unWidth = pQRC->width;
		unWidthAdjusted = unWidth * 8 * 3;
		if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
		unDataBytes = unWidthAdjusted * unWidth * 8;
		// Allocate pixels buffer
		if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
		{
			exit(-1);
		}
		// Preset to white
		memset(pRGBData, 0xff, unDataBytes);
		// Prepare bmp headers
		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType = 0x4d42;  // "BM"
		kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) +
			sizeof(BITMAPINFOHEADER) +
			unDataBytes;
		kFileHeader.bfReserved1 = 0;
		kFileHeader.bfReserved2 = 0;
		kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +
			sizeof(BITMAPINFOHEADER);
		
		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSizeImage = unDataBytes;
		kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth = unWidth * 8;
		kInfoHeader.biHeight = -((int)unWidth * 8);
		kInfoHeader.biPlanes = 1;
		kInfoHeader.biBitCount = 24;
		kInfoHeader.biCompression = BI_RGB;
		kInfoHeader.biSizeImage = 0;
		kInfoHeader.biXPelsPerMeter = 0;
		kInfoHeader.biYPelsPerMeter = 0;
		kInfoHeader.biClrUsed = 0;
		kInfoHeader.biClrImportant = 0;
		// Convert QrCode bits to bmp pixels
		pSourceData = pQRC->data;
		for (y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * 8;
			for (x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					for (l = 0; l < 8; l++)
					{
						for (n = 0; n < 8; n++)
						{
							//this is qrcode color default black
							*(pDestData + n * 3 + unWidthAdjusted * l) = 0x00;
							*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = 0;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = 0;
						}
					}
				}
				pDestData += 3 * 8;
				pSourceData++;
			}
		}
		//把图片字节数据copy到字节数组中
		int dwSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + unDataBytes * sizeof(unsigned char);
		unsigned char* imgbytes = new unsigned char[dwSize] {0};
		memcpy(imgbytes, &kFileHeader, sizeof(BITMAPFILEHEADER));
		memcpy(imgbytes + sizeof(BITMAPFILEHEADER), &kInfoHeader, sizeof(BITMAPINFOHEADER));
		memcpy(imgbytes + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), pRGBData, sizeof(unsigned char)*unDataBytes);
		//记得释放
		delete[] imgbytes;
		// Output the bmp file
		if (!(fopen_s(&f, "temp.bmp", "wb")))
		{
			fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
			fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
			fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);
			fclose(f);
		}
		else
		{
			printf("Unable to open file");
			exit(-1);
		}
		// Free data
		free(pRGBData);
		QRcode_free(pQRC);
	}
	else
	{
		printf("NULL returned");
		exit(-1);
	}
	getchar();
	return 0;
}