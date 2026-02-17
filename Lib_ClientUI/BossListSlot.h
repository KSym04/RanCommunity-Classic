

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../Lib_Engine/G-logic/GLdefine.h"
#include "../Lib_Client/G-Logic/GLCrowData.h"

class CBasicLineBox;
class CBasicTextBox;
class CItemImage;
class CBasicTextButton;

const DWORD UIMSG_NPC_SHOP_SLOT_ITEM_IMAGE = UIMSG_USER1;

class	CBossListSlot : public CUIGroup
{
	enum
	{
		NPC_SHOP_SLOT_ITEM_IMAGE = NO_ID + 1
	};


public:
	CBossListSlot ();
	virtual	~CBossListSlot ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextBossStatus;

	float			m_fTimer;
	BOOL			m_bIsALive;

	float			m_fUpdateTime;
	SNATIVEID		m_sMobID;

public:
	void	ItemSet( CString szBossName, float fTimer, BOOL isAlive );
	void	ItemReset();
	void	UpdateInfo ();
	void	SetMobID ( SNATIVEID sMobID ) { m_sMobID = sMobID; }
	SNATIVEID	GetMobID () { return m_sMobID; }

};