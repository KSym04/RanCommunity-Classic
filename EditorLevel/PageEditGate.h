#pragma once

#include "../Lib_Client/G-Logic/GLLandMan.h"

class	CsheetWithTab;
// CPageEditGate dialog

class CPageEditGate : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageEditGate)

public:
	CPageEditGate(LOGFONT logfont);
	virtual ~CPageEditGate();

// Dialog Data
	enum { IDD = IDD_PAGE_EDIT_GATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CsheetWithTab*	m_pSheetTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;

public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CsheetWithTab* pSheetTab ) { m_pSheetTab = pSheetTab; }

protected:
	GLLandMan*		m_pGLLand;
	PDXLANDGATE		m_pEdit;
	
public:
	void	SetData( GLLandMan* pLand, PDXLANDGATE pEdit );
	void	ResetData();

	void	InitUI( BOOL bENABLE );
	void	DataShow();
	BOOL	DataSave();
	void	JumpTo( D3DXVECTOR3 vMax, D3DXVECTOR3 vMin );

	void	CheckMap( int MID, int SID, int NAME );
	void	FindMap( int MID, int SID );
	void	ResetMap( int MID, int SID, int GATE, int KEY, int INDEX );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonRegnew();
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonMousepos();
	afx_msg void OnBnClickedButtonApply();
	
	afx_msg void OnBnClickedButtonToMap0();
	afx_msg void OnBnClickedButtonToMap1();
	afx_msg void OnBnClickedButtonToMap2();
	afx_msg void OnBnClickedButtonToMap3();
	afx_msg void OnBnClickedButtonToMap4();
	afx_msg void OnBnClickedButtonToMap5();
	afx_msg void OnBnClickedButtonToMap6();
	afx_msg void OnBnClickedButtonToMap7();

	afx_msg void OnEnChangeEditToMapMid0();
	afx_msg void OnEnChangeEditToMapSid0();
	afx_msg void OnEnChangeEditToMapMid1();
	afx_msg void OnEnChangeEditToMapSid1();
	afx_msg void OnEnChangeEditToMapMid2();
	afx_msg void OnEnChangeEditToMapSid2();
	afx_msg void OnEnChangeEditToMapMid3();
	afx_msg void OnEnChangeEditToMapSid3();
	afx_msg void OnEnChangeEditToMapMid4();
	afx_msg void OnEnChangeEditToMapSid4();
	afx_msg void OnEnChangeEditToMapMid5();
	afx_msg void OnEnChangeEditToMapSid5();
	afx_msg void OnEnChangeEditToMapMid6();
	afx_msg void OnEnChangeEditToMapSid6();
	afx_msg void OnEnChangeEditToMapMid7();
	afx_msg void OnEnChangeEditToMapSid7();
	
	afx_msg void OnBnClickedButtonToMapReset0();
	afx_msg void OnBnClickedButtonToMapReset1();
	afx_msg void OnBnClickedButtonToMapReset2();
	afx_msg void OnBnClickedButtonToMapReset3();
	afx_msg void OnBnClickedButtonToMapReset4();
	afx_msg void OnBnClickedButtonToMapReset5();
	afx_msg void OnBnClickedButtonToMapReset6();
	afx_msg void OnBnClickedButtonToMapReset7();

};
