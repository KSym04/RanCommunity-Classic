
// HWDetectDlg.h : header file
//

#pragma once

// CHWDetectDlg dialog
class CHWDetectDlg : public CDialog
{
// Construction
public:
	CHWDetectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HWDETECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CEdit			m_cEdit;
	CComboBox		m_cSelect;

	void InfoAdd( const char *szFormat, ... );
	void InfoReset();

	void InfoPCIDBasic();
	void InfoSMBIOS();
	void InfoPCIDSMBIOS();
	void InfoPCIDMAC();

public:
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonClear();
};
