#pragma once

#include "../Lib_Engine/DxEffect/DxPieceContainer.h"
class CPieceTab;
// CPieceMain dialog

class CPieceMain : public CPropertyPage
{
	DECLARE_DYNAMIC(CPieceMain)

public:
	CPieceMain(LOGFONT logfont);
	virtual ~CPieceMain();

// Dialog Data
	enum { IDD = IDD_PAGE_PIE_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CPieceTab*		m_pTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;
	DxPieceEdit*	m_pData;

public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CPieceTab* pSheetTab ) { m_pTab = pSheetTab; }
	void	SetData( DxPieceEdit* pData );
	void	ResetData();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnPieLoad();
	afx_msg void OnBnClickedBtnPieSave();
};
