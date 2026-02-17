#pragma once

#include "../Lib_Client/G-Logic/GLCrowData.h"
#include "../Lib_Engine/Meshs/DxSkinDataDummy.h"

class	CsheetWithTab;
// CPageEdit3 dialog

class CPageEdit3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageEdit3)

public:
	CPageEdit3(LOGFONT logfont);
	virtual ~CPageEdit3();

// Dialog Data
	enum { IDD = IDD_EDIT3 };

private:
	CsheetWithTab*	m_pSheetTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;

public:
	PCROWDATA	m_pDummyCrow;
	PCROWDATA	m_pCrow;

public:
	BOOL	CrowSet ( PCROWDATA pCROW );
	void	CrowClear();

	BOOL	DataDefault();
	BOOL	DataShow();
	BOOL	DataSave();


public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CsheetWithTab* pSheetTab ) { m_pSheetTab = pSheetTab; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPageBack2();
	afx_msg void OnBnClickedButtonPageCancel();
	afx_msg void OnBnClickedButtonPageSave();
	afx_msg void OnBnClickedButtonWardenItemGen1();
	afx_msg void OnBnClickedButtonWardenItemGen2();
	afx_msg void OnBnClickedButtonWardenItemGen3();
	afx_msg void OnBnClickedButtonWardenItemGen4();
};
