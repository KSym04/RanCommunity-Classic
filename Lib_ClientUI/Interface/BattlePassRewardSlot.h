#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-logic/GLDefine.h"
#include "../../Lib_Client/G-Logic/GLItem.h"

/*12-9-14, Battle Pass - CNDev*/

class CBasicLineBox;
class CBasicTextBox;
class CBasicTextButton;
class CItemImage;

class	CBattlePassRewardSlot : public CUIGroup
{
	enum
	{
		ITEM_IMAGE_NORMAL = NO_ID + 1,
		ITEM_IMAGE_PREMIUM,

		BUTTON_NORMAL_CLAIM,
		BUTTON_PREMIUM_CLAIM,
	};


public:
	CBattlePassRewardSlot ();
	virtual	~CBattlePassRewardSlot ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CItemImage*		m_pNormalRewardImage;
	CUIControl*		m_pNormalLock;
	CItemImage*		m_pPremiumRewardImage;
	CUIControl*		m_pPremiumLock;
	CBasicTextBox*	m_pTextLevel;

	SNATIVEID		m_sNormalID;
	SNATIVEID		m_sPremiumID;

	CBasicTextButton*	m_pButtonNormalClaim;
	CBasicTextButton*	m_pButtonPremiumClaim;

	DWORD			m_dwLevel;

public:
	void	ItemSet( SNATIVEID sNormalID, SNATIVEID sPremiumID, DWORD dwLevel );
	void	ItemReset();

	void	ClaimNormalReward();
	void	ClaimPremiumReward();
};