/*!
 * \file BattlePassPageSlot.h
 *
 * \author Juver
 * \date November 2017
 *
 * 
 */
#pragma once

#include "../Lib_Engine/GUInterface/UIGroup.h"

#include "../Lib_Engine/G-Logic/GLDefine.h"

class CBasicTextBox;
class CItemImage;
class CBasicLineBox;
class CBasicTextButton;
class	CGachaPageRewardSlot : public CUIGroup
{
private:
	enum
	{
		ITEM_SLOT_IMAGE = NO_ID + 1,
	};

public:
	CGachaPageRewardSlot ();
	virtual	~CGachaPageRewardSlot ();

public:
	void	CreateSubControl ();

public:
	CBasicLineBox*		m_pBlackLineBox;
	CBasicLineBox*		m_pBlueLineBox;
	CBasicTextBox*		m_pTextRate;

	CItemImage*			m_pItemImage;
	SNATIVEID			m_sidItem;
	float				m_fRATE;

	//select highlight
	CUIControl*		m_pSelectImage;
protected:
	CItemImage*		CreateItemImage0 ();
public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	DataReset();
	void	DataSet(SNATIVEID sItemID, float fRATE);

	void	SetOverVisible();
	void	SetOverVisibleOFF();
};