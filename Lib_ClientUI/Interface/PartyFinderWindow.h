
#pragma	once

#include "./UIWindowEx.h"

#include "../../Lib_Client/G-Logic/GLPartyFinder.h"

class CBasicTextBoxEx;
class CBasicScrollBarEx;
class CBasicTextButton;
class CPartyFinderSlot;
class CBasicButton;

class CPartyFinderWindow : public CUIWindowEx
{
public:
	enum
	{
		PARTY_FINDER_SELECTION_TEXTBOX = ET_CONTROL_NEXT,
		PARTY_FINDER_SELECTION_SCROLLBAR,
		PARTY_FINDER_BUTTON_SEARCH,
		PARTY_FINDER_BUTTON_PREV,
		PARTY_FINDER_BUTTON_NEXT,

		PARTY_FINDER_SLOT_00,
		PARTY_FINDER_SLOT_01,
		PARTY_FINDER_SLOT_02,
		PARTY_FINDER_SLOT_03,
		PARTY_FINDER_SLOT_04,
		PARTY_FINDER_SLOT_05,
		PARTY_FINDER_SLOT_06,
		PARTY_FINDER_SLOT_07,

	};

	enum
	{
		PARTY_FINDER_MAX_SLOT = 8,
	};

public:
	CPartyFinderWindow ();
	virtual	~CPartyFinderWindow ();

public:
	void	CreateSubControl ();

public:
	CBasicTextBoxEx*	m_pListTextBox;
	CBasicScrollBarEx*	m_pListScrollBar;
	CBasicTextButton*	m_pButtonSearch;
	CPartyFinderSlot*	m_pSlot[PARTY_FINDER_MAX_SLOT];

	CBasicButton*		m_pButtonPrev;
	CBasicButton*		m_pButtonNext;
	CBasicTextBox*		m_pTextPage;

private:
	int	m_nSelectIndexType;
	int	m_nCurrentPage;
	int m_nMaxPage;
	DWORD m_dwSelectedIndex;

	SPARTY_FINDER_RESULT_VEC m_vecResult;
	DWORD m_dwSearchType;

public:
	void ResetData();
	void InitData();

	void ShowSlot( int nPage );
	void SelectSlot( int nSelect );
	void SelectType( DWORD dwIndex );

	void SearchParty();
	void UpdateSearchResult();

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
};