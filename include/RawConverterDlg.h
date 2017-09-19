
// RawConverterDlg.h : header file
//

#pragma once


// CRawConverterDlg dialog
class CRawConverterDlg : public CDialog
{
// Construction
public:
	CRawConverterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RAWCONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	// Created Values
	CString m_edFilePath;
	int m_edWidth;
	int m_edHeight;
	int m_radioBitDepth;
	int m_radioColorType;
	int m_radioFileType;
	int m_radioBayerType;
	// Created Methods
	afx_msg void OnBnClickedConvert();
	afx_msg void OnBnClickedFileopen();
	afx_msg void OnBnClickedRadioBitDepth(UINT);
	afx_msg void OnBnClickedRadioColorType(UINT);
	afx_msg void OnBnClickedRadioFileType(UINT);
	afx_msg void OnBnClickedRadioBayerType(UINT);

	// Custom Values
	int m_bitDepth;
	int m_colorType;
	CString m_fileExt;
	int m_bayerType;
};
