#pragma once

#include "../Lib_Engine/DxEffect/DxPieceContainer.h"

class CMainTab;
class CPieceTab;
// CMainPiece dialog

class CMainPiece : public CPropertyPage
{
	DECLARE_DYNAMIC(CMainPiece)

public:
	CMainPiece(LOGFONT logfont);
	virtual ~CMainPiece();

// Dialog Data
	enum { IDD = IDD_PAGE_MAIN_PIE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CMainTab*		m_pTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;
	CPieceTab*		m_pToolTab;
	DxPieceEdit*	m_pData;

public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CMainTab* pSheetTab ) { m_pTab = pSheetTab; }
	void	ResetData();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnPieLoad();
	afx_msg void OnBnClickedBtnPieSave();
};
