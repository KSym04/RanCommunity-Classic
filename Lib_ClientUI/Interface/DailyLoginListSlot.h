#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

/*12-16-24, daily login - CNDev*/

class CBasicTextBox;
class CBasicTextButton;
class CItemImage;

class	CDailyLoginListSlot : public CUIGroup
{
private:
	enum
	{
		ITEM_IMAGE = NO_ID + 1,
		BUTTON_CLAIM,
	};

public:
	CDailyLoginListSlot ();
	virtual	~CDailyLoginListSlot ();

public:
	void	CreateSubControl ();

public:
	CItemImage*		m_pRewardImage;
	SNATIVEID		m_sRewardID;

	CBasicTextBox*	m_pTextDay;
	CBasicTextBox*	m_pTextClaimed;

	CUIControl*		m_pLockImage;

	CBasicTextButton*	m_pButtonClaim;

	DWORD			m_dwDay;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	void	ResetData();
	void	SetData( DWORD dwDay, SNATIVEID sRewardID );
	void	ClaimReward();
};