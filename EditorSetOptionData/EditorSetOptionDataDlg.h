// EditorSetOptionDataDlg.h : header file
//

#pragma once

#include "../Lib_Client/G-Logic/GLSetOption.h"

// CEditorSetOptionDataDlg dialog
class CEditorSetOptionDataDlg : public CDialog
{
// Construction
public:
	CEditorSetOptionDataDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDITOR_SET_OPTION_DATA_DIALOG };

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

protected:
	virtual void PostNcDestroy();

public:
	CListCtrl		m_List;

public:
	BOOL				m_bAdd;
	SSET_OPTION_DATA	m_sDataEdit;

	BOOL	DataShow();
	void	SetOptionShow( BOOL bEnable, BOOL bAdd );

public:
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoadCsv();
	afx_msg void OnBnClickedButtonSaveCsv();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonSetOptionSave();
	afx_msg void OnNMDblclkListSetOption(NMHDR *pNMHDR, LRESULT *pResult);
	
};
