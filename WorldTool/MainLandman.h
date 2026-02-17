#pragma once

#include "../Lib_Engine/DxOctree/DxLandMan.h"

class CMainTab;
class CLandmanTab;
// CMainLandman dialog

class CMainLandman : public CPropertyPage
{
	DECLARE_DYNAMIC(CMainLandman)

public:
	CMainLandman(LOGFONT logfont);
	virtual ~CMainLandman();

// Dialog Data
	enum { IDD = IDD_PAGE_MAIN_WLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CMainTab*		m_pTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;
	CLandmanTab*	m_pToolTab;
	DxLandMan*		m_pData;

public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CMainTab* pSheetTab ) { m_pTab = pSheetTab; }
	void	ResetData();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnWldLoad();
	afx_msg void OnBnClickedBtnWldSave();
};
