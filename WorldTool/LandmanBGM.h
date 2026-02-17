#pragma once

#include "../Lib_Engine/DxOctree/DxLandMan.h"
class CLandmanTab;
// CLandmanBGM dialog

class CLandmanBGM : public CPropertyPage
{
	DECLARE_DYNAMIC(CLandmanBGM)

public:
	CLandmanBGM(LOGFONT logfont);
	virtual ~CLandmanBGM();

// Dialog Data
	enum { IDD = IDD_PAGE_WLD_BGM };

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
	afx_msg void OnBnClickedBtnBgm();
};
