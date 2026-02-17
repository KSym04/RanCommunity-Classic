
// EditorClassConstDlg.h : header file
//

#pragma once

#include "../Lib_Client/G-Logic/GLogicData.h"

// CEditorClassConstDlg dialog
class CEditorClassConstDlg : public CDialog
{
// Construction
public:
	CEditorClassConstDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDITOR_CLASS_CONST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	GLCONST_CHARCLASS	m_CharClass;
	std::string			m_strFileName;

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
	void	DataShow();
	void	DataSave();

public:

	afx_msg void OnBnClickedButtonFileLoad();
	afx_msg void OnBnClickedButtonFileSave();
	afx_msg void OnBnClickedButtonFileImport();
	afx_msg void OnBnClickedButtonFileExport();
};
