#pragma once
#include "Files.h"

class CInterpolation
{
private:
	int nWidth;
	int nHeight;
	int nImageSize;
	int nBayerType;
	BYTE* szRawFile;

	// Make Pattern
	BYTE* szRedPattern;
	BYTE* szBluePattern;
	BYTE* szGreenPattern;


	// for GBRG, GRBG
	void P14_G(BYTE*, CString);
	void P2_RB(BYTE*, CString);
	void P3_RB(BYTE*, CString);
	// for BGGR, RGGB
	void P1_RB(BYTE*, CString);
	void P4_RB(BYTE*, CString);
	void P23_G(BYTE*, CString);

	BYTE* MakePattern(CString, BYTE*);

public:
	BYTE* szInterpolated;

	CInterpolation(void);
	~CInterpolation(void);

	BOOL DoInterpolation(CFiles*);
};
