
// RawConverterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RawConverter.h"
#include "RawConverterDlg.h"
#include "FileControl.h"


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRawConverterDlg dialog
CRawConverterDlg* pDlg = (CRawConverterDlg*)AfxGetApp();


CRawConverterDlg::CRawConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRawConverterDlg::IDD, pParent)
	, m_edFilePath(_T(""))
	, m_edWidth(0)
	, m_edHeight(0)
	, m_radioBitDepth(0)	// Init BitDepth 10bit
	, m_radioColorType(0)	// Init ColorType Gray Scale
	, m_radioFileType(0)	// Init FileType .Bmp
	, m_radioBayerType(0)	// Init BayerType GBRG
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRawConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, m_edFilePath);
	DDX_Text(pDX, IDC_WIDTH, m_edWidth);
	DDX_Text(pDX, IDC_WIDTH2, m_edHeight);
	DDX_Radio(pDX, IDC_BITDEPTH8, m_radioBitDepth);
	DDX_Radio(pDX, IDC_COLORTYPE8, m_radioColorType);
	DDX_Radio(pDX, IDC_BMPTYPE, m_radioFileType);
	DDX_Radio(pDX, IDC_BAYER_GBRG, m_radioBayerType);
}

BEGIN_MESSAGE_MAP(CRawConverterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FILEOPEN, &CRawConverterDlg::OnBnClickedFileopen)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BITDEPTH8, IDC_BITDEPTH10, OnBnClickedRadioBitDepth)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_COLORTYPE8, IDC_COLORTYPE24, OnBnClickedRadioColorType)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BMPTYPE, IDC_PNGTYPE, OnBnClickedRadioFileType)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BAYER_GBRG, IDC_BAYER_RGGB, OnBnClickedRadioBayerType)
	ON_BN_CLICKED(IDC_CONVERT, &CRawConverterDlg::OnBnClickedConvert)
END_MESSAGE_MAP()


// CRawConverterDlg message handlers

BOOL CRawConverterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_bitDepth = 8;	// init bit depth and color type
	m_colorType = GRAY_SCALE;
	GetDlgItemText(IDC_BMPTYPE, m_fileExt);	// Default File Type is ".BMP"
	m_bayerType = GBRG;


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRawConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRawConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRawConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRawConverterDlg::OnBnClickedFileopen()
{
	// Open File Dialog
	TCHAR szFilter[] = _T("Raw Files(*.raw)|*.raw|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if(dlg.DoModal() == IDOK)
	{
		m_edFilePath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

// Check Radio Control Changed
void CRawConverterDlg::OnBnClickedRadioBitDepth(UINT rid)
{
	UpdateData(TRUE);
	switch(m_radioBitDepth)
	{
	case 0:
		m_bitDepth = 8;		// 8 bit File
		break;
	case 1:
		m_bitDepth = 10;		// 10 bit File
		break;
	}
	UpdateData(FALSE);
}

void CRawConverterDlg::OnBnClickedRadioColorType(UINT rid)
{
	UpdateData(TRUE);
	switch(m_radioColorType)
	{
	case 0:	// Color Type means number of channels
		m_colorType = GRAY_SCALE;
		break;
	case 1:
		m_colorType = TRUE_COLOR;
		break;
	}
	UpdateData(FALSE);
}

void CRawConverterDlg::OnBnClickedRadioFileType(UINT rid)
{
	UpdateData(TRUE);
	switch(m_radioFileType)
	{
	case 0:
		GetDlgItemText(IDC_BMPTYPE, m_fileExt);	// BMP
		break;
	case 1:	
		GetDlgItemText(IDC_JPGETYPE, m_fileExt);	// JPG
		break;
	case 2:
		GetDlgItemText(IDC_PNGTYPE, m_fileExt);	// PNG
		break;
	}
	UpdateData(FALSE);
}

void CRawConverterDlg::OnBnClickedRadioBayerType(UINT rid)
{
	UpdateData(TRUE);
	switch(m_radioBayerType)
	{
	case 0:
		m_bayerType = GBRG;
		break;
	case 1:	
		m_bayerType = GRBG;
		break;
	case 2:
		m_bayerType = BGGR;
		break;
	case 3:
		m_bayerType = RGGB;
		break;
	}
	UpdateData(FALSE);
}



void CRawConverterDlg::OnBnClickedConvert()
{
	// Create Raw File Object
	CFiles* RawFile = new CFiles(m_edFilePath, m_edWidth, m_edHeight, m_bitDepth, m_colorType, m_fileExt, m_bayerType);
	CFileControl FileControl;

	if( !FileControl.DoConvertRawFile(RawFile) )
	{
		AfxMessageBox(_T("Cannot Convert File"));
		exit(0);
	}	
	else
	{
		MessageBox(_T("File Created !!"));
		delete[] RawFile->getResultFileData();
	}

	delete(RawFile);
}
