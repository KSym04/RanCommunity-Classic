#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLDailyLogin.h"

/*12-16-24, daily login - CNDev*/

class CBasicScrollBarEx;
class CBasicTextBox;
class CDailyLoginListSlot;

class	CDailyLoginList : public CUIGroup
{
private:
	enum
	{
		DAILYLOGIN_LIST_SLOT_0 = NO_ID + 1,
		DAILYLOGIN_LIST_SLOT_1,
		DAILYLOGIN_LIST_SLOT_2,
		DAILYLOGIN_LIST_SLOT_3,
		DAILYLOGIN_LIST_SLOT_4,
		DAILYLOGIN_LIST_SLOT_5,
		DAILYLOGIN_LIST_SLOT_6,
		DAILYLOGIN_LIST_SLOT_7,
		DAILYLOGIN_LIST_SLOT_8,
		DAILYLOGIN_LIST_SLOT_9,
		DAILYLOGIN_LIST_SLOT_10,
		DAILYLOGIN_LIST_SLOT_11,
		DAILYLOGIN_LIST_SLOT_12,
		DAILYLOGIN_LIST_SLOT_13,
		DAILYLOGIN_LIST_SLOT_14,
		DAILYLOGIN_LIST_SLOT_15,
		DAILYLOGIN_LIST_SLOT_16,
		DAILYLOGIN_LIST_SLOT_17,
		DAILYLOGIN_LIST_SLOT_18,
		DAILYLOGIN_LIST_SLOT_19,
		DAILYLOGIN_LIST_SLOT_20,
		DAILYLOGIN_LIST_SLOT_21,
		DAILYLOGIN_LIST_SLOT_22,
		DAILYLOGIN_LIST_SLOT_23,
		DAILYLOGIN_LIST_SLOT_24,
		DAILYLOGIN_LIST_SLOT_25,
		DAILYLOGIN_LIST_SLOT_26,
		DAILYLOGIN_LIST_SLOT_27,
	};

	enum
	{
		DAILYLOGIN_LIST_SLOT_MAX = 28,
	};

public:
	CDailyLoginList ();
	virtual	~CDailyLoginList ();

public:
	void	CreateSubControl ();

public:
	CDailyLoginListSlot*		m_pSlot[DAILYLOGIN_LIST_SLOT_MAX];

	DAILYLOGIN_REWARD_VEC		m_vecData;

private:
	float						m_fUpdateTime;


public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	void	ResetData();
	void	LoadData();
	void	ShowData();
};