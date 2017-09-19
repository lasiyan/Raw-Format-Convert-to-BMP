#include "StdAfx.h"
#include "BmpClass.h"

CBmpClass::CBmpClass(CFiles* pFiles)
{
	// Fill BITMAP struct
	memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&bih, 0, sizeof(BITMAPINFOHEADER));
	memset(&rgb, 0, sizeof(RGBQUAD) * 256);


	bfh.bfType = 19778;		// Ascii 'B' and 'M'
	bih.biBitCount = pFiles->getColorType();	// Need bit to draw per Pixel(1, 4, 8, 16, 24, 32)

	if(bih.biBitCount == 8)	// Gray Scale 
	{
		bfh.bfSize = pFiles->getImageSize() + sizeof(RGBQUAD) * 256 + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
		bfh.bfOffBits = 14 + 40 + 4 * 256;	// size of bf, bi, rgbquad*4 // 24bit not use color table, so 54 val;
	}
	else	// True Color(RGB)
	{
		bfh.bfSize = pFiles->getImageSize() + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
		bfh.bfOffBits = 14 + 40;	// size of bf, bi, rgbquad*4 // 24bit not use color table, so 54 val;
	}
	bih.biSize = 40;	// Size of BITMAPINFOHEADER
	bih.biWidth = pFiles->getWidth();	// Bitmap Width size(px)
	bih.biHeight = pFiles->getHeight();	// Bitmap Height size(px)
	bih.biPlanes = 1;	// Always 1
	bih.biSizeImage = pFiles->getImageSize();
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;

	for(int i = 0; i < 256; i++)
	{
		rgb[i].rgbBlue = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbRed = i;
		rgb[i].rgbReserved = 0;
	}
	rwsize = WIDTHBYTES(bih.biBitCount * bih.biWidth);
}

CBmpClass::~CBmpClass(void) { }

void CBmpClass::CreateBmpImage(BYTE* szBitMapFile, BYTE* szInputData)
{
	for(int i = 0; i < bih.biHeight; i++)
	{
		for(int j = 0; j < rwsize; j++)
		{
			if(bih.biBitCount == TRUE_COLOR)
			{
				szBitMapFile[i * rwsize + j] = szInputData[(bih.biHeight - i - 1) * rwsize + j];
			}
			else if(bih.biBitCount == GRAY_SCALE) 
			{
				szBitMapFile[i * rwsize + j] = szInputData[(bih.biHeight - i - 1) * rwsize + j];
			}
			else
			{
				AfxMessageBox(_T("This ColorType is not supported"));
				exit(0);
			}
		}
	}
}