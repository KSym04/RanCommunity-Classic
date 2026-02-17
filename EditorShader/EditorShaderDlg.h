
// EditorShaderDlg.h : header file
//

#include "../Lib_Engine/Common/CFileFind.h"


#pragma once


// CEditorShaderDlg dialog
class CEditorShaderDlg : public CDialog
{
// Construction
public:
	CEditorShaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDITOR_SHADER_DIALOG };

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
	void ShaderShow( std::string strTitle, std::string strShader, BOOL bENABLE, BOOL bADD );

public:
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonDataSave();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonClear();
};
