
// ServerSettingDlg.h : header file
//

#include "../Lib_Engine/Common/CFileFind.h"


#pragma once

class CServerSettingTab;

// CServerSettingDlg dialog
class CServerSettingDlg : public CDialog
{
// Construction
public:
	CServerSettingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERVER_SETTING_DIALOG };

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

private:
	CServerSettingTab*		m_pTab;

protected:
	virtual void PostNcDestroy();

public:
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnImport();
};
