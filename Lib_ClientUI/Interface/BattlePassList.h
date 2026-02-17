#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLBattlePassData.h"

/*12-9-14, Battle Pass - CNDev*/

class CBasicScrollBarEx;
class CBasicTextBox;
class CBattlePassListSlot;

class	CBattlePassList : public CUIGroup
{
private:
	enum
	{
		BATTLEPASS_LIST_SLOT_0 = NO_ID + 1,
		BATTLEPASS_LIST_SLOT_1,
		BATTLEPASS_LIST_SLOT_2,
		BATTLEPASS_LIST_SLOT_3,
		BATTLEPASS_LIST_SLOT_4,
		BATTLEPASS_LIST_SLOT_5,
		BATTLEPASS_LIST_SLOT_6,
		BATTLEPASS_LIST_SLOT_7,
	};

	enum
	{
		BATTLEPASS_LIST_SLOT_MAX = 8,
	};

public:
	CBattlePassList ();
	virtual	~CBattlePassList ();

public:
	void	CreateSubControl ();

public:
	CBasicTextBox*		m_pTextMission;

	CBattlePassListSlot*		m_pSlot[BATTLEPASS_LIST_SLOT_MAX];

	SBATTLEPASS_FILE_DATA_VEC	m_vecData;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	void	ResetData();
	void	LoadData();
	void	ShowData();
};