/*!
 * \file GachaPagee.h
 *
 * \author Juver
 * \date October 2017
 *
 * 
 */
#pragma once

#include "../Lib_Engine/GUInterface/UIGroup.h"

#include "../Lib_Client/G-Logic/GLItemMan.h"
#define GACHA_REWARD_SLOT_MAX 18
class CBasicTextBoxEx;
class CBasicScrollBarEx;
class CBasicTextBox;
class CBasicButton;
class CItemImage;
class CBasicLineBox;
class CBasicProgressBar;
class CBasicTextButton;

class CGachaPageRewardSlot;

class CGachaPage : public CUIGroup
{
private:
	enum
	{
		OPEN_GACHA = NO_ID + 1
		, GACHA_REWARD_PAGE_SLOT_0
		, GACHA_REWARD_PAGE_SLOT_1
		, GACHA_REWARD_PAGE_SLOT_2
		, GACHA_REWARD_PAGE_SLOT_3
		, GACHA_REWARD_PAGE_SLOT_4
		, GACHA_REWARD_PAGE_SLOT_5
		, GACHA_REWARD_PAGE_SLOT_6
		, GACHA_REWARD_PAGE_SLOT_7
		, GACHA_REWARD_PAGE_SLOT_8
		, GACHA_REWARD_PAGE_SLOT_9
		, GACHA_REWARD_PAGE_SLOT_10
		, GACHA_REWARD_PAGE_SLOT_11
		, GACHA_REWARD_PAGE_SLOT_12
		, GACHA_REWARD_PAGE_SLOT_13
		, GACHA_REWARD_PAGE_SLOT_14
		, GACHA_REWARD_PAGE_SLOT_15
		, GACHA_REWARD_PAGE_SLOT_16
		, GACHA_REWARD_PAGE_SLOT_17
		, GACHA_REWARD_PAGE_SLOT_18
		, GACHA_REWARD_PAGE_SLOT_19
		, CONFIRM_DRAW_CHECK_BUTTON
		, AUTO_DRAW_CHECK_BUTTON
		, GACHA_ITEM_SLOT_IMAGE
		, GACHA_REWARD_ITEM_SLOT_IMAGE
		, GACHA_GUARANTEED_REWARD_ITEM_SLOT_IMAGE
		, EXP_TEXT
	};


public:
	CGachaPage ();
	virtual	~CGachaPage ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );
	CBasicTextBox*	CreateStaticControl ( char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	CUIControl*		CreateUIControl( const char* szControl );
private:
	CBasicTextBox*		m_pTextGachaTitle;
	CBasicTextBox*		m_pTextIntroductionTitle;
	CBasicTextBox*		m_pTextCostTitle;
	CBasicTextBox*		m_pTextIntroductionText;
	CBasicTextButton*	m_pButtonOpenGacha;

	CItemImage*			m_pGachaItemImage;
	CBasicTextBox*		m_pTextGachaItemName;

	CItemImage*			m_pGachaRewardItemImage;
	CBasicTextBox*		m_pTextGachaRewardItemName;

	CItemImage*			m_pGachaGuaranteedRewardItemImage;
	CBasicTextBox*		m_pTextGachaGuaranteedRewardItemName;

	CBasicTextBox*		m_pTextConfirm;
	CBasicTextBox*		m_pTextAuto;

	CBasicButton*		m_pConfirmButton;
	CBasicButton*		m_pAutoButton;

	CGachaPageRewardSlot* m_pGachaRewardPageSlot[GACHA_REWARD_SLOT_MAX];
	
	float			m_fRefreshTimer;
	float			m_fLimitTimer;
	int				m_nSelected;
	SNATIVEID		m_sidItem;
	SNATIVEID		m_sidItemMainReward;
	SNATIVEID		m_sidItemGuaranteedReward;

	CBasicTextBox*		m_pEXPText;
	CBasicProgressBar*	m_pEXP;
public:
	void			LoadData();
	void			SetItem ( WORD nSelected );
	BOOL			m_bDraw;
private:
	void	ResetData();
	void	ResetRewardSlot();

	void	CreateLineBox();
	void	SetEXP( WORD wExp );
};