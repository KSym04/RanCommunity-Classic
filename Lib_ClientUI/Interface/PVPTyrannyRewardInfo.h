/*!
 * \file PVPTyrannyRewardInfo.h
 *
 * \author Juver
 * \date July 2018
 *
 * 
 */
#pragma	once

#include "./UIWindowEx.h"

class CBasicTextBox;
class CItemImage;

class	CPVPTyrannyRewardInfo : public CUIWindowEx
{
	enum
	{
		TYRANNY_REWARD_INFO_ITEM_WIN = ET_CONTROL_NEXT,
		TYRANNY_REWARD_INFO_ITEM_LOSS
	};

public:
	CPVPTyrannyRewardInfo ();
	virtual ~CPVPTyrannyRewardInfo ();

public:
	void	CreateSubControl ();

public:
	CBasicTextBox*	m_pTitleWin;
	CBasicTextBox*	m_pTitleLoss;
	CBasicTextBox*	m_pInfoWin;
	CBasicTextBox*	m_pInfoLoss;
	CItemImage*		m_pItemWin;
	CItemImage*		m_pItemLoss;
	CBasicTextBox*	m_pItemInfoWin;
	CBasicTextBox*	m_pItemInfoLoss;

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

	void ShowInfo();
};