#pragma once
#include "BmpClass.h"
#include "Interpolation.h"

class CFileControl
{
private:
	CInterpolation Interpolation;

	BOOL OpenRawFile(CFiles*);
	BOOL MakeBmpImage(CFiles*, BYTE*);
	BOOL MakeJpgImage();	// JPG and PNG yet..
	BOOL MakePngImage();
	CString MakeFileName(CString, CString);
	
public:
	CFileControl(void);
	~CFileControl(void);

	BOOL DoConvertRawFile(CFiles*);	
};
