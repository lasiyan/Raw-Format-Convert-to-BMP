#pragma once
#include "Files.h"

#define WIDTHBYTES(bits) (((bits)+31)/32*4)

class CBmpClass : public CFiles
{
private:
	BYTE* szInputData;
	int rwsize;

public:
	BITMAPFILEHEADER bfh;	// File Header	// sz : 14
	BITMAPINFOHEADER bih;	// Info Header	// sz : 40
	RGBQUAD rgb[256];		// Color Table
	
	CBmpClass(CFiles*);
	~CBmpClass(void);

	void BMPHeaderSetting();
	void CreateBmpImage(BYTE*, BYTE*);
};
