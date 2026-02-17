#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLCharDefine.h"
#include "../../Lib_Client/G-Logic/GLPartyFinder.h"

class CBasicTextBox;
class CBasicTextButton;
class CBasicLineBox;

class	CPartyFinderSlot : public CUIGroup
{
	enum
	{
		PARTY_FINDER_SLOT_JOIN_BUTTON = NO_ID + 1,
	};


public:
	CPartyFinderSlot ();
	virtual	~CPartyFinderSlot ();

public:
	void	CreateSubControl ();

public:
	CBasicTextBox*	m_pTextLeaderInfo1;
	CBasicTextBox*	m_pTextLeaderInfo2;
	CBasicTextBox*	m_pTextLeaderInfo3;
	CBasicTextBox*	m_pTextLeaderInfo4;
	CBasicTextBox*	m_pTextLeaderInfo5;

	CUIControl*		m_pSchoolImage[GLSCHOOL_NUM];
	CUIControl*		m_pClassImage[GLCI_NUM_8CLASS];

	CBasicLineBox*	m_pSelectImage;

	CBasicTextButton*	m_pButtonJoin;

	DWORD			m_dwPartyID;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void	DataSet( const SPARTY_FINDER_RESULT* pParty );
	void	DataReset();
	void	SlotSelect( BOOL bSelect );
};