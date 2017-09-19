#pragma once

class CFiles
{
public:
	CFiles(CString, int, int, int, int, CString, int);	// input File (Raw file)
	CFiles();	// Default
	~CFiles(void);

private:
	// input
	CString strFileName;
	int nWidth;
	int nHeight;
	int nImageSize;	
	int nResultImageSize;
	// File options
	int nBitDepth;
	int nColorType;
	CString strFileExt;
	int nBayerType;		// #define Bayer Pattern Type 1 ~ 4

	// File Data
	BYTE* szRawFileData;		// to open Raw file
	BYTE* szInterpolatedData;	// After Interpolation
	BYTE* szResultFileData;		// Result File(BMP)

public:
	CString getFileName();
	void setFileName(CString);

	int getWidth();
	void setWidth(int);

	int getHeight();
	void setHeight(int);

	int getImageSize();
	void setImageSize(int);

	int getResultImageSize();
	void setResultImageSize(int);

	int getBitDepth();
	void setBitDepth(int);

	int getColorType();
	void setColorType(int);

	CString getFileExt();
	void setFileExt(CString);

	int getBayerType();
	void setBayerType(int);

	BYTE* getRawFileData();
	void setRawFileData(BYTE*);

	BYTE* getInterpolatedData();
	void setInterpolatedData(BYTE*);
	
	BYTE* getResultFileData();
	void setResultFileData(BYTE*);
};
