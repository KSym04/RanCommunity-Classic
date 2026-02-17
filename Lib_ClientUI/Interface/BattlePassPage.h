#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLBattlePassDefine.h"

/*12-9-14, Battle Pass - CNDev*/

class CBasicTextBoxEx;
class CBasicScrollBarEx;
class CBasicTextBox;
class CBasicButton;
class CBattlePassList;
class CBattlePassReward;


class	CBattlePassPage : public CUIGroup
{
private:
	enum
	{
		BATTLEPASS_LIST = NO_ID + 1,
		BATTLEPASS_REWARD,
	};

public:
	CBattlePassPage ();
	virtual	~CBattlePassPage ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:
	CBasicTextBox*		m_pTextMissionTitle;
	CBasicTextBox*		m_pTextRewardTitle;

	CBattlePassList*	m_pBattlePassList;
	CBattlePassReward*	m_pBattlePassReward;

private:
	void	LoadData();

};