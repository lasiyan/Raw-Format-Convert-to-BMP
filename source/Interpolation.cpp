// Interpolation.cpp : implementation file
//

#include "stdafx.h"
#include "Interpolation.h"


// CInterpolation
CInterpolation::CInterpolation(){}
CInterpolation::~CInterpolation(){}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//			
//		[ Bayer Pattern ]
//						Type. 1		2			3			4
//		p1	p2	=>		G	B		G	R		B	G		R	G
//		p3	p4			R	G		B	G		G	R		G	B
//
//		Type 1 and Type 2 have same G pattern	--> P14_G
//												--> P2_RB
//												--> P3_RB
//
//		and Type 3, Type 4 have same G pattern	--> P23_G
//												--> P1_RB
//												--> P4_RB
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInterpolation::DoInterpolation(CFiles* pFiles)
{
	// Get File Info from CFiles
	nWidth = pFiles->getWidth();
	nHeight = pFiles->getHeight();
	nImageSize = pFiles->getImageSize();
	nBayerType = pFiles->getBayerType();
	szRawFile = pFiles->getRawFileData();

	szInterpolated = pFiles->getInterpolatedData();


	// Make Pattern Arrays
	szRedPattern = new BYTE[nImageSize];
	ZeroMemory(szRedPattern, nImageSize);
	szGreenPattern = new BYTE[nImageSize];
	ZeroMemory(szGreenPattern, nImageSize);	
	szBluePattern = new BYTE[nImageSize];
	ZeroMemory(szBluePattern, nImageSize);


	// Do Interpolation using Bilinear
	switch( nBayerType )
	{
	case GBRG:
		P14_G(szGreenPattern, _T("GREEN"));
		P2_RB(szBluePattern, _T("P2"));
		P3_RB(szRedPattern, _T("P3"));
		break;
	case GRBG:
		P14_G(szGreenPattern, _T("GREEN"));
		P2_RB(szRedPattern, _T("P2"));
		P3_RB(szBluePattern, _T("P3"));
		break;
	case BGGR:
		P1_RB(szBluePattern, _T("P1"));
		P23_G(szGreenPattern, _T("GREEN"));
		P4_RB(szRedPattern, _T("P4"));
		break;
	case RGGB:
		P1_RB(szRedPattern, _T("P1"));
		P23_G(szGreenPattern, _T("GREEN"));
		P4_RB(szBluePattern, _T("P4"));
		break;
	}


	// if Result Color Type is Gray Sacle,
	// szInterpolated has 8bit (Not 24bit)
	// And RGB type -> YCrCb ( Luma Coding | https://en.wikipedia.org/wiki/Grayscale )
	if( pFiles->getColorType() == GRAY_SCALE )
	{
		for(int i = 0; i < nImageSize; i++)
		{
			szInterpolated[i] = ( szBluePattern[i] * 0.0722 + szRedPattern[i] * 0.2126 + szGreenPattern[i] * 0.7152 );
		}
	}
	else	// Make True Color Image
	{
		for(int i = 0; i < nImageSize * 3; i++)
		{
			if( i % 3 == 0 )	// 0, 3, 6, 9, ...
			{
				szInterpolated[i] = szBluePattern[ i / 3 ];	// 0, 1, 2, ...
			}
			else if( i % 3 == 1 )	// 1, 4, 7, 10, ...
			{
				szInterpolated[i] = szGreenPattern[ i / 3 ];	// 0, 1, 2, ...
			}
			else	// 2, 5, 8, 11, ...
			{
				szInterpolated[i] = szRedPattern[ i / 3 ];	// 0, 1, 2, ...
			}
		}
	}

	// Return Pattern Arrays
	delete[] szRedPattern;
	delete[] szBluePattern;
	delete[] szGreenPattern;

	return TRUE;
}

BYTE* CInterpolation::MakePattern(CString color, BYTE* szResult)
{
	int nRowStartIdx, nHeightStartIdx;

	/****** GRENN ******/
	if( color == "GREEN" )
	{
		// Type 1,2(GB, GR)'s Green Start EVEN(0) idx
		// Type 3,4(BG, RG)'s Green  Start ODD(1) idx
		bool nRowStartIdx = nBayerType / 3;	

		// Pattern GBRG, GRBG
		// so even row takes green(Gr) in even index(0, 2, 4, ..., 4638),
		// and odd row takes green(Gb) in odd index(1, 3, 5, ..., 4639)	

		// Pattern RGGB, BGGR
		// so even row takes green(Gr) in odd index(1, 3, 5, ..., 4639),
		// and odd row takes green(Gb) in even index(0, 2, 4, ..., 4638)	
		for(int j = 0; j < nHeight; j++)
		{
			for(int i = nRowStartIdx; i < nWidth; i += 2)
			{
				szResult[nWidth * j + i] = szRawFile[nWidth * j + i];
			}

			nRowStartIdx = !nRowStartIdx;	// ODD to EVEN  or  EVEN to ODD
		}
	}

	/****** RED or BLEU *****/
	else
	{
		if( color == "P1" )
		{
			nHeightStartIdx = nRowStartIdx = 0;
		}
		else if( color == "P2")
		{
			nHeightStartIdx = 0;
			nRowStartIdx = 1;
		}
		else if( color == "P3")
		{
			nHeightStartIdx = 1;
			nRowStartIdx = 0;
		}
		else
		{
			nHeightStartIdx = nRowStartIdx = 1;
		}


		for(int j = nHeightStartIdx; j < nHeight; j += 2)
		{
			for(int i = nRowStartIdx; i < nWidth; i += 2)
			{
				szResult[nWidth * j + i] = szRawFile[nWidth * j + i];
			}

		}
	}

	return szResult;
}


// Algorithms
/// Type 1, 2
void CInterpolation::P14_G(BYTE* szArray, CString color)
{
	int pos = 0;
	bool nStartIndex = (nBayerType == 1 || nBayerType == 2) ? 1 : 0 ;	// !!! RED or BLUE's start index
	int nAverage = 0;

	szArray = MakePattern(color, szArray);
	
	for(int j = 0; j < nHeight; j++)
	{
		for( int i = nStartIndex; i < nWidth; i += 2 )
		{
			pos = j * nWidth + i;

			if( j == 0 && i == nWidth - 1 )
			{
				nAverage = (szArray[ pos - 1 ] + szArray[ pos + nWidth ]) / 2;
			}
			if( j == nHeight - 1 && i == 0 )
			{
				nAverage = (szArray[ pos + 1 ] + szArray[ pos - nWidth ]) / 2;
			}

			if( j == 0 )
			{
				nAverage = (szArray[ pos - 1 ]  + szArray[ pos + nWidth ] + szArray[ pos + 1 ]) / 3;
			}
			else if( i == 0 )
			{
				nAverage = (szArray[ pos - nWidth ] + szArray[ pos + 1 ] + szArray[ pos + nWidth ]) / 3;
			}
			else if( j == nHeight - 1 )
			{
				nAverage = (szArray[ pos - 1 ] + szArray[ pos + 1 ] + szArray[ pos - nWidth ]) / 3;
			}
			else if( i == nWidth - 1 )
			{
				nAverage = (szArray[ pos - nWidth ] + szArray[ pos - 1 ] + szArray[ pos + nWidth ]) / 3;
			}

			else
			{
				nAverage = (szArray[ pos - 1 ] + szArray[ pos + 1 ] + szArray[ pos - nWidth ] + szArray[ pos + nWidth ]) / 4;
			}

			szArray[ pos ] = nAverage;
		}
		nStartIndex = !nStartIndex;
	}


}
void CInterpolation::P2_RB(BYTE* szArray, CString color) 
{
	int pos = 0;
	int nAverage = 0;

	szArray = MakePattern(color, szArray);


	for(int j = 0; j < nHeight; j++)
	{
		if( j % 2 == 0)	
		{
			for(int i = 0; i < nWidth; i++)
			{
				pos = j * nWidth + i;	

				if( i % 2 == 1)	
				{
					continue;
				}
				else
				{
					if ( i == 0 )	
					{
						nAverage = szArray[ pos + 1 ];	
					}
					else
					{
						nAverage = (szArray[ pos - 1 ] + szArray[ pos + 1 ]) / 2;
					}
				}
				szArray[ pos ] = nAverage;
			}
		}

		else	
		{
			for(int i = 0; i < nWidth; i++)
			{
				pos = j * nWidth + i;

				if( i % 2 == 1)
				{
					if( j == nHeight - 1)	
					{
						nAverage = szArray[ pos - nWidth ];
					}
					else
					{
						nAverage = (szArray[ pos - nWidth ] + szArray[ pos + nWidth ]) / 2;
					}
				}
				else
				{
					if ( i == 0 && j == nHeight - 1 )
					{
						nAverage = szArray[ pos - nWidth + 1 ];
					}
					else if( i == 0 )
					{
						nAverage = (szArray[ pos - nWidth + 1 ] + szArray[ pos + nWidth + 1]) / 2;
					}
					else if ( j == nHeight - 1 )
					{
						nAverage = (szArray[ pos - nWidth - 1 ] + szArray[ pos - nWidth + 1]) / 2;
					}
					else
					{
						nAverage = (szArray[pos - nWidth - 1] + szArray[pos - nWidth + 1] + szArray[pos + nWidth - 1] + szArray[pos + nWidth + 1]) / 4;
					}
				}
				szArray[ pos ] = nAverage;
			}
		}
	}
}
void CInterpolation::P3_RB(BYTE* szArray, CString color) 
{
	int pos = 0;
	int nAverage = 0;

	szArray = MakePattern(color, szArray);

	// interpolation P3 Color
	for(int j = 0; j < nHeight; j++)
	{
		// Even Row ( 0, 2, 4, ... ,3486 )
		if( j % 2 == 0)	
		{
			for(int i = 0; i < nWidth; i++)
			{
				pos = j * nWidth + i;

				if( i % 2 == 1)
				{
					if( i == nWidth - 1 && j == 0 )	
					{
						nAverage = szArray[ pos + nWidth -1 ];	
					}
					else if( j == 0 )	
					{
						nAverage = (szArray[ pos + nWidth - 1 ] + szArray[ pos + nWidth + 1 ]) / 2;
					}
					else if( i == nWidth - 1 )	
					{
						nAverage = (szArray[ pos - nWidth - 1] + szArray[ pos + nWidth - 1]) / 2;
					}
					else
					{
						nAverage = (szArray[pos - nWidth - 1] + szArray[pos - nWidth + 1] + szArray[pos + nWidth - 1] + szArray[pos + nWidth + 1]) / 4;
					}
				}
				else	
				{
					if( j == 0 )
					{
						nAverage = szArray[ pos + nWidth ];
					}
					else
					{
						nAverage = (szArray[ pos - nWidth ] + szArray[ pos + nWidth ]) / 2;
					}
				}
				szArray[ pos ] = nAverage;
			}
		}

		else	
		{
			for(int i = 0; i < nWidth; i++)
			{
				pos = j * nWidth + i;

				if( i % 2 == 1)
				{
					if ( i == 0 )
					{
						nAverage = szArray[ pos - 1 ];
					}
					else
					{
						nAverage = (szArray[ pos - 1 ] + szArray[ pos + 1 ]) / 2;
					}
				}
				else
				{
					continue;					
				}
				szArray[ pos ] = nAverage;
			}
		}
	}
}


/// Type 3, 4
void CInterpolation::P1_RB(BYTE* szArray, CString color)
{
	int pos = 0;
	int nAverage = 0;

	szArray = MakePattern(color, szArray);


	// interpolation P1 Color
	for(int j = 0; j < nHeight; j++)	// i : Row Index, j : Column Index
	{
		// Even Row ( 0, 2, 4, ... ,3486 )
		if( j % 2 == 0)	
		{
			for(int i = 0; i < nWidth; i++)
			{
				pos = j * nWidth + i;	// pos is current index

				if( i % 2 == 0)	// Even Index
				{
					continue;
				}
				else	// Even Row's Odd Index ( Bayer Pattern Color is G, So prev-index and next-index's avg )
				{
					if ( i == nWidth - 1 )	// But pos is Row's last index, next-index isn't exist
					{
						nAverage = szArray[ pos - 1 ];	// So average takes only prev-index
					}
					else
					{
						nAverage = (szArray[ pos - 1 ] + szArray[ pos + 1 ]) / 2;
					}
				}
				szArray[ pos ] = nAverage;
			}
		}

		// Odd Row ( 1, 3, 5, ... 3487 )
		else	
		{
			for(int i = 0; i < nWidth; i++)
			{
				pos = j * nWidth + i;	// pos is current index

				if( i % 2 == 0)	// Even Index ( Bayer Pattern Color is G, It takes top-index and bottom-index's avg )
				{
					if( j == nHeight - 1)	// But pos is last row, bottom-index isn't exist
					{
						nAverage = szArray[ pos - nWidth ];	// so Take only top-index
					}
					else
					{
						nAverage = (szArray[ pos - nWidth ] + szArray[ pos + nWidth ]) / 2;
					}
				}
				else	// Odd Index ( it takes 4 Edge-index's avg )
				{
					if ( i == nWidth - 1 && j == nHeight - 1 )	// Pattern Array's edge point ( Array[4639][3487] )
					{
						nAverage = szArray[ pos - nWidth - 1 ];
					}
					else if( i == nWidth - 1 )	// But row's last index, It doesn't take right-edges
					{
						nAverage = (szArray[ pos - nWidth - 1 ] + szArray[ pos + nWidth - 1]) / 2;	// so It takes left-edges data
					}
					else if ( j == nHeight - 1 )	// But last row, It doesn't take bottom-edges
					{
						nAverage = (szArray[ pos - nWidth - 1 ] + szArray[ pos - nWidth + 1]) / 2;	// so It takes left-edges data
					}
					else
					{
						nAverage = (szArray[pos - nWidth - 1] + szArray[pos - nWidth + 1] + szArray[pos + nWidth - 1] + szArray[pos + nWidth + 1]) / 4;
					}
				}
				szArray[ pos ] = nAverage;
			}
		}
	}
}

void CInterpolation::P4_RB(BYTE* szArray, CString color)
{
	int pos = 0;
	int nAverage = 0;

	szArray = MakePattern(color, szArray);

	// interpolation P4 Color
	for(int j = 0; j < nHeight; j++)	// i : Row Index, j : Column Index
	{
		// Even Row ( 0, 2, 4, ... ,3486 )
		if( j % 2 == 0)	
		{
			for(int i = 0; i < nWidth; i++)
			{
				pos = j * nWidth + i;	// pos is current index

				if( i % 2 == 0)	// Even Index ( Bayer Pattern Color is G, It takes top-index and bottom-index's avg )
				{
					if( i == 0 && j == 0 )	// Pattern Array's Left-Top edge point ( Array[0][0] )
					{
						nAverage = szArray[ pos + nWidth + 1 ];	// Get P4 Color from Array[1][1]
					}
					else if( j == 0 )	// If pos is in first Row, Cannot attach [pos - width] index
					{
						nAverage = (szArray[ pos + nWidth - 1 ] + szArray[ pos + nWidth + 1 ]) / 2;
					}
					else if( i == 0 )		// If pos is in first column, Cannot attach [pos - 1] index
					{
						nAverage = (szArray[ pos - nWidth + 1] + szArray[ pos + nWidth + 1]) / 2;
					}
					else
					{
						nAverage = (szArray[pos - nWidth - 1] + szArray[pos - nWidth + 1] + szArray[pos + nWidth - 1] + szArray[pos + nWidth + 1]) / 4;
					}
				}
				else	// Odd Index ( it takes 4 Edge-index(Red)'s avg )
				{
					if( j == 0 )	// But pos is first row, top-index isn't exist
					{
						nAverage = szArray[ pos + nWidth ];	// so Take only top-index
					}
					else
					{
						nAverage = (szArray[ pos - nWidth ] + szArray[ pos + nWidth ]) / 2;
					}
				}
				szArray[ pos ] = nAverage;
			}
		}

		// Odd Row ( 1, 3, 5, ... 3487 )
		else	
		{
			for(int i = 0; i < nWidth; i++)
			{
				pos = j * nWidth + i;	// pos is current index

				if( i % 2 == 0)	// Even Row's Odd Index ( Bayer Pattern Color is G, So prev-index and next-index's avg )
				{
					if ( i == 0 )	// But pos is Row's first index, prev-index isn't exist
					{
						nAverage = szArray[ pos - 1 ];	// So average takes only prev-index
					}
					else
					{
						nAverage = (szArray[ pos - 1 ] + szArray[ pos + 1 ]) / 2;
					}
				}
				else	// Even Index
				{
					continue;					
				}
				szArray[ pos ] = nAverage;
			}
		}
	}
}

void CInterpolation::P23_G(BYTE* szArray, CString color)
{
	int pos = 0;
	bool nStartIndex = 0;	// !!! RED or BLUE's start index
	int nAverage = 0;

	szArray = MakePattern(color, szArray);

	// Row's start index is toggle 0(R), 1(B), 0(R), 1(B) according to GRRB
	// Gr, Gb is get default Green value, so Pass ( i = i + 2 )
	for(int j = 0; j < nHeight; j++)
	{
		for( int i = nStartIndex; i < nWidth; i += 2 )
		{
			pos = j * nWidth + i;

			// (1) Two Edge points( Left-Top, Right-Bottom ) take two near pixel's avg
			if( j == 0 && i == 0 )
			{
				nAverage = (szArray[ pos + 1 ] + szArray[ pos + nWidth ]) / 2;
			}
			if( j == nHeight - 1 && i == nWidth - 1 )
			{
				nAverage = (szArray[ pos - 1 ] + szArray[ pos - nWidth ]) / 2;
			}

			// (2) Four Edge Border excluding (1) take three near pixel's avg 
			if( j == 0 )
			{
				nAverage = (szArray[ pos - 1 ]  + szArray[ pos + nWidth ] + szArray[ pos + 1 ]) / 3;
			}
			else if( i == 0 )
			{
				nAverage = (szArray[ pos - nWidth ] + szArray[ pos + 1 ] + szArray[ pos + nWidth ]) / 3;
			}
			else if( j == nHeight - 1 )
			{
				nAverage = (szArray[ pos - 1 ] + szArray[ pos + 1 ] + szArray[ pos - nWidth ]) / 3;
			}
			else if( i == nWidth - 1 )
			{
				nAverage = (szArray[ pos - nWidth ] + szArray[ pos - 1 ] + szArray[ pos + nWidth ]) / 3;
			}

			// (3) Else All RED or BLUE pixels
			else
			{
				nAverage = (szArray[ pos - 1 ] + szArray[ pos + 1 ] + szArray[ pos - nWidth ] + szArray[ pos + nWidth ]) / 4;
			}

			szArray[ pos ] = nAverage;
		}
		nStartIndex = !nStartIndex;
	}
}

