#pragma once

#include "../Lib_Engine/DxOctree/DxLandMan.h"
class CLandmanTab;
// CLandmanMain dialog

class CLandmanMain : public CPropertyPage
{
	DECLARE_DYNAMIC(CLandmanMain)

public:
	CLandmanMain(LOGFONT logfont);
	virtual ~CLandmanMain();

// Dialog Data
	enum { IDD = IDD_PAGE_WLD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CLandmanTab*	m_pTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;
	DxLandMan*		m_pData;

public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CLandmanTab* pSheetTab ) { m_pTab = pSheetTab; }
	void	SetData( DxLandMan* pData );
	void	ResetData();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnWldLoad();
	afx_msg void OnBnClickedBtnWldSave();
	afx_msg void OnBnClickedBtnWldSelect();
	afx_msg void OnBnClickedBtnMakeShadowMesh();
	afx_msg void OnBnClickedBtnMakeShadowNavi();
	afx_msg void OnBnClickedBtnMakeShadowNavi2();
	afx_msg void OnBnClickedBtnNavigationMeshReplace();
};
