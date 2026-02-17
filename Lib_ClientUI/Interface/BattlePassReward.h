#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLBattlePassReward.h"

/*12-9-14, Battle Pass - CNDev*/

class CBasicScrollBarEx;
class CBasicTextBox;
class CBasicProgressBar;
class CBattlePassRewardSlot;

class	CBattlePassReward : public CUIGroup
{
private:
	enum
	{
		BATTLEPASS_REWARD_SCROLLBAR = NO_ID + 1,
		BATTLEPASS_REWARD_SLOT_0,
		BATTLEPASS_REWARD_SLOT_1,
		BATTLEPASS_REWARD_SLOT_2,
		BATTLEPASS_REWARD_SLOT_3,
		BATTLEPASS_REWARD_SLOT_4,
		BATTLEPASS_REWARD_SLOT_5,
		BATTLEPASS_REWARD_SLOT_6,
		BATTLEPASS_REWARD_SLOT_7,
	};

	enum
	{
		BATTLEPASS_REWARD_MAX_SLOT = 8,
	};

public:
	CBattlePassReward ();
	virtual	~CBattlePassReward ();

public:
	void	CreateSubControl ();

public:
	CBasicScrollBarEx*	m_pScrollBar;
	CBasicTextBox*		m_pTextLevel;
	CBasicTextBox*		m_pTextPremium;


	CBasicProgressBar*	m_pBarEXP;
	CBasicTextBox*		m_pTextEXP;
	DWORD				m_dwBackEXP;

	CBattlePassRewardSlot*	m_pSlot[BATTLEPASS_REWARD_MAX_SLOT];

	BATTLEPASS_LEVEL_REWARD_VEC m_vecData;
	int		m_nStart;
	int		m_nTotal;

	float	m_fUpdateLevel;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	void	ResetData();
	void	LoadData();
	void	ShowData();

	void	UpdateBattlePassLevel();
	void	SetBattlePassEXP( DWORD dwEXPCurrent, DWORD dwEXPRequire );
};