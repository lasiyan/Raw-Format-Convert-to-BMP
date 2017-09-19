#include "StdAfx.h"
#include "FileControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFileControl::CFileControl(void){}
CFileControl::~CFileControl(void){}

BOOL CFileControl::DoConvertRawFile(CFiles* pFile)
{
	// Create File Data
	if( !OpenRawFile(pFile))
	{
		AfxMessageBox(_T("Cannot Create File"));
		return FALSE;
	}

	// Do interpolation
	if(!Interpolation.DoInterpolation(pFile))
	{
		return FALSE;
	}
	else
	{
		delete[] pFile->getRawFileData();
	}

	// Create Full Image ( Headers + Data )
	if(pFile->getFileExt() == "BMP")
	{
		if( !MakeBmpImage(pFile, pFile->getInterpolatedData()) )
		{
			return FALSE;
		}
		delete[] pFile->getInterpolatedData();
	}
	else // JPG, PNG, etc
	{
		AfxMessageBox(_T("Cannot Support yet"));
		exit(0);
	}

	return TRUE;
}

BOOL CFileControl::OpenRawFile(CFiles* pFile)
{
	FILE* fp;

	CT2A ascii(pFile->getFileName(), CP_UTF8);	// CString to const char*
	if((fopen_s(&fp, ascii.m_psz, "rb")) != 0)
	{
		AfxMessageBox(_T("fopen_s() Error"));
		return FALSE ;
	}

	switch(pFile->getBitDepth())	// File Bit Depth Check
	{
	case 8:
		fread(pFile->getRawFileData(), 1, pFile->getImageSize(), fp);
		break;
	case 10:
		BYTE* szTemp = new BYTE[(pFile->getImageSize()) * 2];
		fread(szTemp, 2, pFile->getImageSize(), fp);

		// Convert 10bit to 8bit
		for(int i = 0; i < pFile->getImageSize(); i++)
		{
			// and Normalization 0~1023 to 0~255
			pFile->getRawFileData()[i] = (szTemp[2 * i] + (szTemp[(2 * i) + 1] * 256)) >> 2;	// 16^0 and 16^1 -> 16^2 and 16^3 Because Little Endian
		}
		delete[] szTemp;
		break;
	}

	fclose(fp);
	return TRUE;
}

BOOL CFileControl::MakeBmpImage(CFiles* pFile, BYTE* szInputData)
{
	FILE* fp2;
	CBmpClass BmpFile(pFile);
	
	BmpFile.CreateBmpImage(pFile->getResultFileData(), szInputData);

	CT2A ascii(MakeFileName(pFile->getFileName(), pFile->getFileExt()), CP_UTF8);	// CString to const char*

	if( (fopen_s(&fp2, ascii.m_psz, "wb")) != NULL)
	{
		AfxMessageBox(_T("Cannot Save File"));
		return FALSE;
	}

	// Write Bitmap Headers
	fwrite(&BmpFile.bfh, sizeof(BITMAPFILEHEADER), 1, fp2);	
	fwrite(&BmpFile.bih, sizeof(BITMAPINFOHEADER), 1, fp2);

	if(pFile->getColorType() == GRAY_SCALE)
	{
		fwrite(&BmpFile.rgb, sizeof(RGBQUAD), 256, fp2);
	}
	fwrite(pFile->getResultFileData(), 1, pFile->getResultImageSize(), fp2);

	fclose(fp2);
	return TRUE;
}

CString CFileControl::MakeFileName(CString fName, CString ext)
{
	int nPos;
	CString originFileName = fName;
	CString strNewPath;
	nPos = originFileName.ReverseFind('.');
	if(nPos == -1)
	{
		strNewPath = originFileName + _T(".") + ext;
	}
	else 
	{
		strNewPath = originFileName.Left(nPos) + _T(".") + ext;
	}

	return strNewPath;
}