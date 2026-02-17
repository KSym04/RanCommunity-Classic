/**MMR Rank Stats, Mhundz 01/02/2025**/
#pragma once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBoxEx;
class CBasicScrollBarEx;
class CBasicTextBox;
class CBasicButton;
class CMMRPageMain;
class CBasicProgressBar;
class	CBasicLineBox;
struct	GLCHARLOGIC_CLIENT;

class	CMMRPage : public CUIGroup
{
private:
	enum
	{
		MMR_PAGE_LIST_TEXTBOX = NO_ID + 1,
		RAN_MDEV_ICON_RANK_IMAGE0,
		RAN_MDEV_ICON_RANK_IMAGE1,
		RAN_MDEV_ICON_RANK_IMAGE2,
		RAN_MDEV_ICON_RANK_IMAGE3,
		RAN_MDEV_ICON_RANK_IMAGE4,
		RAN_MDEV_ICON_RANK_IMAGE5,
		RAN_MDEV_ICON_RANK_IMAGE6,
		RAN_MDEV_ICON_RANK_IMAGE7,
		RAN_MDEV_ICON_RANK_IMAGE8,
		RAN_MDEV_ICON_RANK_IMAGE9,
		RAN_MDEV_MMR_PROGRESSBAR_TEXT,
	};
	enum
	{
		nRANK_MARK		= 10,
	};

public:
	CMMRPage ();
	virtual	~CMMRPage ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:
	CBasicLineBox*		CreateUILineBoxQuestList( char* szBaseControl );
	CBasicTextBox*		CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, CString strText, D3DCOLOR D3DCOLOR, int nAlign );
	CBasicTextBox*		CreateText ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign );
	CBasicProgressBar*	m_pMMRProgressBar;
	CBasicTextBox*		m_pMMRTextBar;
	CBasicTextBox*		m_pMMRText;
	CBasicTextBox*		m_pMMRRankText;
	void	SetMMR ( const GLCHARLOGIC_CLIENT& sCharData );
	void	SetMMRProgressBar ( const GLCHARLOGIC_CLIENT& sCharData );
	CUIControl*		m_pRankMark[nRANK_MARK];
	int CalculateVal( DWORD dwValue );
	int CalculateProgressVal( DWORD dwValue );


	CBasicTextBoxEx*	m_pListTextBox;
	CBasicScrollBarEx*	m_pListScrollBar;
	CBasicTextBox*		m_pTextTitle;
	CBasicTextBox*		m_pTextReportTitle;
	CBasicTextBox*		m_pTextCompleted;
	CBasicTextBox*		m_pTextPoint;
	CBasicButton*		m_pButtonItem;


	CMMRPageMain*	m_pMainPage;

	int					m_nSelectIndex;

	float				m_fActPointUpdate;
	
	//CBasicComboBox*				m_pComboBoxTypeOpen;
	//CBasicComboBoxRollOver*		m_pComboBoxTypeRollOver;

	DWORD	m_dwHPIncr;
	DWORD	m_dwMPIncr;
	DWORD	m_dwSPIncr;
	DWORD	m_dwAttIncr;
	DWORD	m_dwDefIncr;
	DWORD	m_dwShootIncr;
	DWORD	m_dwMeleeIncr;
	DWORD	m_dwEnerIncr;
	DWORD	m_dwResistIncr;
	DWORD	m_dwHRIncr;
	DWORD	m_dwARIncr;
	
	CBasicTextBox*		m_pTextList[11];


private:
	//void	UpdateActPoint();
	void	LoadData();
	//void	SelectType( DWORD dwSelect );

};