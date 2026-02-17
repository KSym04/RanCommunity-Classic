// EditorRandomOptionDlg.h : header file
//

#pragma once

#include "../Lib_Client/G-Logic/GLRandomOption.h"

// CEditorRandomOptionDlg dialog
class CEditorRandomOptionDlg : public CDialog
{
// Construction
public:
	CEditorRandomOptionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDITORRANDOMOPTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CListCtrl		m_List;
	CListCtrl		m_ListSet;
	CListCtrl		m_ListSetR;
	CListCtrl		m_ListSection;

	SRANDOM_OPTION_GEN		m_RandomOptionGen;
	BOOL					m_bAddGen;
	
	SRANDOM_OPTION_SET		m_RandomOptionSet;
	BOOL					m_bAddSet;
	BOOL					m_bSetR;

	SRANDOM_OPTION_SECTION	m_RandomOptionSection;
	BOOL					m_bAddSection;
	DWORD					m_dwSectionEdit;

public:
	BOOL	DataShow();
	void	RandomOptionGenShow( BOOL bEnable, BOOL bAdd );
	void	ShowListSet();
	void	ShowListSetR();
	void	RandomOptionSetShow( BOOL bEnable, BOOL bAdd, BOOL bSetR );
	void	ShowListSection();
	void	RandomOptionSectionShow( BOOL bEnable, BOOL bAdd );

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
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad2();
	afx_msg void OnBnClickedButtonSave2();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonGenSave();
	afx_msg void OnNMDblclkListRandomOption(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGenSetAdd1();
	afx_msg void OnBnClickedButtonGenSetEdit1();
	afx_msg void OnBnClickedButtonGenSetDel1();
	afx_msg void OnBnClickedButtonGenSetClear1();
	afx_msg void OnBnClickedButtonGenSetCopy1();
	afx_msg void OnBnClickedButtonGenSetAdd2();
	afx_msg void OnBnClickedButtonGenSetEdit2();
	afx_msg void OnBnClickedButtonGenSetDel2();
	afx_msg void OnBnClickedButtonGenSetClear2();
	afx_msg void OnBnClickedButtonGenSetCopy2();
	afx_msg void OnNMDblclkListGenSet(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListGenSetr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSetGenerate();
	afx_msg void OnBnClickedButtonSetSave();
	afx_msg void OnBnClickedButtonSetSectionAdd();
	afx_msg void OnBnClickedButtonSetSectionEdit();
	afx_msg void OnBnClickedButtonSetSectionDel();
	afx_msg void OnBnClickedButtonSetSectionClear();
	afx_msg void OnBnClickedButtonSectionSave();
	afx_msg void OnNMDblclkListSetSection(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGenCopySetToSetr();
	
};
