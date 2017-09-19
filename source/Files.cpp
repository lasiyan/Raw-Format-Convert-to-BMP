#include "StdAfx.h"
#include "Files.h"

CFiles::~CFiles(void){}
CFiles::CFiles(){}

// Input Data 
CFiles::CFiles(CString _fileName, int _width, int _height, int _bitDepth, int _colorType, CString _fileExt, int _bayerType)
{
	strFileName = _fileName;
	nWidth = _width;
	nHeight = _height;

	int ctype = (_colorType == 8) ? 1 : 3 ;

	nImageSize = nWidth * nHeight;
	nResultImageSize = nWidth * nHeight * ctype;
	
	nBitDepth = _bitDepth;
	nColorType = _colorType;
	strFileExt = _fileExt;
	nBayerType = _bayerType;
	
	szRawFileData = new BYTE[ nImageSize ];	// Need to File Open
	ZeroMemory(szRawFileData, nImageSize);

	szInterpolatedData = new BYTE[ nResultImageSize ];
	ZeroMemory(szInterpolatedData, nResultImageSize);

	szResultFileData = new BYTE[ nResultImageSize ];
	ZeroMemory(szResultFileData, nResultImageSize);
}

CString CFiles::getFileName()
{
	return strFileName;
}
void CFiles::setFileName(CString _fileName)
{
	strFileName = _fileName;
}

int CFiles::getWidth()
{
	return nWidth;
}
void CFiles::setWidth(int _width)
{
	nWidth = _width;
}

int CFiles::getHeight()
{
	return nHeight;
}
void CFiles::setHeight(int _height)
{
	nHeight = _height;
}

int CFiles::getImageSize()
{
	return nImageSize;
}
void CFiles::setImageSize(int _imageSize)
{
	nImageSize = _imageSize;
}

int CFiles::getResultImageSize()
{
	return nResultImageSize;
}
void CFiles::setResultImageSize(int _resultImageSize)
{
	nResultImageSize = _resultImageSize;
}

int CFiles::getBitDepth()
{
	return nBitDepth;
}
void CFiles::setBitDepth(int _bitDepth)
{
	nBitDepth = _bitDepth;
}

int CFiles::getColorType()
{
	return nColorType;
}
void CFiles::setColorType(int _colorType)
{
	nColorType = _colorType;
}

CString CFiles::getFileExt()
{
	return strFileExt;
}
void CFiles::setFileExt(CString _fileExt)
{
	strFileExt = _fileExt;
}

int CFiles::getBayerType()
{
	return nBayerType;
}
void CFiles::setBayerType(int _bayerType)
{
	nBayerType = _bayerType;
}

BYTE* CFiles::getRawFileData()
{
	return szRawFileData;
}
void CFiles::setRawFileData(BYTE* _rawFileData)
{
	szRawFileData = _rawFileData;
}

BYTE* CFiles::getInterpolatedData()
{
	return szInterpolatedData;
}
void CFiles::setInterpolatedData(BYTE* _fileData)
{
	szInterpolatedData = _fileData;
}

BYTE* CFiles::getResultFileData()
{
	return szResultFileData;
}
void CFiles::setResultFileData(BYTE* _fileData)
{
	szResultFileData = _fileData;
}