#pragma once

#include "../Lib_Engine/DxOctree/DxLandMan.h"
class CLandmanTab;
// CLandmanSky dialog

class CLandmanSky : public CPropertyPage
{
	DECLARE_DYNAMIC(CLandmanSky)

public:
	CLandmanSky(LOGFONT logfont);
	virtual ~CLandmanSky();

// Dialog Data
	enum { IDD = IDD_PAGE_WLD_SKY };

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

	BOOL	ShowData();
	BOOL	SaveData();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnWldClose();
	afx_msg void OnBnClickedBtnWldApply();
};
