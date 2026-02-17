
// EditorMaterialDlg.h : header file
//

#include "../Lib_Engine/Common/CFileFind.h"

#include "../Lib_Engine/DxCommon/DxMaterialManager.h"

#pragma once


// CEditorMaterialDlg dialog
class CEditorMaterialDlg : public CDialog
{
// Construction
public:
	CEditorMaterialDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDITOR_MATERIAL_DIALOG };

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
	BOOL			m_bAdd;

public:
	void ShowData();
	void MaterialShow( const SMATERIAL_STATIC& material, BOOL bENABLE, BOOL bADD );

public:
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonImportList();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonDataSave();
};
