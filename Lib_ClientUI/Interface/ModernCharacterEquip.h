/*!
 * \file ModernCharacterEquip.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLItem.h"

class	CItemImage;
class	CBasicButton;
class	CModernCharacterEquipRender;

const DWORD UIMSG_MOUSEIN_MODERN_CHARACTER_WINDOW_WEARSLOT = UIMSG_USER1;

class	CModernCharacterEquip : public CUIGroup
{
private:
	static	const	int	nOUTOFRANGE;

private:
	enum
	{
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD = NO_ID + 1,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_UPPER,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_WAIST,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_LOWER,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_HAND,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_FOOT,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY,

		MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_NECK,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_WRIST,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY,

		MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON,
		MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON,

		MODERN_CHARACTER_WINDOW_WEAR_SLOT_VEHICLE,

		MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE = 17,

		MODERN_CHARACTER_WINDOW_BUTTON_LEFT,
		MODERN_CHARACTER_WINDOW_BUTTON_RIGHT,
	};


public:
	CModernCharacterEquip ();
	virtual	~CModernCharacterEquip ();

public:
	void	CreateSubControl ();
	CUIControl*	CreateControl ( const char* szControl );
	CItemImage*	CreateItemImage ( const char* szControl, UIGUID ControlID );

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	SetItemIndex ( int nIndex )				{ m_nIndex = nIndex; }
	int		GetItemIndex ()							{ return m_nIndex; }
	EMSLOT	IMAGE2EMSLOT ( int nIndex );
	EMSLOT	GetItemSlot ();
	SITEMCUSTOM&	GetItem ( int nIndex );
	void	LoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom );
	void	UnLoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom );
	void	SetTabButton( BOOL bMain ); 
	void	ClearRender();
	void	StartRender();

private:
	CUIControl*		m_pMouseOver;
	CUIControl*		m_pSlotDisplay[MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE];
	CItemImage*		m_pItemImage[MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE];
	CUIControl*		m_pSlotErrorImage[MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE];

	SITEMCUSTOM		m_ItemCustomArray[MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE];
	int				m_nIndex;	

	CModernCharacterEquipRender*	m_pRender;
	CBasicButton*	m_pButtonLeft;
	CBasicButton*	m_pButtonRight;
};