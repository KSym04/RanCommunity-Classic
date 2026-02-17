// EditorSetOptionGenDlg.h : header file
//

#pragma once

#include "../Lib_Client/G-Logic/GLSetOption.h"

// CEditorSetOptionGenDlg dialog
class CEditorSetOptionGenDlg : public CDialog
{
// Construction
public:
	CEditorSetOptionGenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDITOR_SET_OPTION_GEN_DIALOG };

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
	CListCtrl		m_ListGen;
public:
	BOOL				m_bAdd;
	SSET_OPTION_GEN		m_sDataEdit;

	BOOL	DataShow();
	void	SetOptionShow( BOOL bEnable, BOOL bAdd );

	void	GenShow();

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
	afx_msg void OnNMDblclkListSetOption(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGenSave();
	afx_msg void OnBnClickedButtonGenDel();
	afx_msg void OnBnClickedButtonGenAdd();
	afx_msg void OnBnClickedButtonGenAddSelect();
	afx_msg void OnBnClickedButtonGenAddSave();
	afx_msg void OnEnChangeEditGenAddId();
	afx_msg void OnBnClickedButtonGenTest();
};
