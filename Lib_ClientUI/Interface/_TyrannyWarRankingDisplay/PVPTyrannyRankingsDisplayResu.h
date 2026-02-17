#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

#include "../Lib_Client/G-Logic/GLPVPTyrannyDefine.h"

/* 7-13-23 Tyranny War Ranking System - Montage */

class CBasicTextBox;
class CPVPTyrannyRankingsDisplayResuSlot;
class CPVPTyrannyRankingsDisplayResuSlotSelf;

class CPVPTyrannyRankingsDisplayResu : public CUIGroup
{
	enum
	{
		TYRANNY_RANKING_DISPLAY_SLOT_0 = NO_ID + 1,
		TYRANNY_RANKING_DISPLAY_SLOT_1,
		TYRANNY_RANKING_DISPLAY_SLOT_2,
		TYRANNY_RANKING_DISPLAY_SLOT_3,
		TYRANNY_RANKING_DISPLAY_SLOT_4,
		TYRANNY_RANKING_DISPLAY_SLOT_5,
		TYRANNY_RANKING_DISPLAY_SLOT_6,
		TYRANNY_RANKING_DISPLAY_SLOT_7,
		TYRANNY_RANKING_DISPLAY_SLOT_8,
		TYRANNY_RANKING_DISPLAY_SLOT_9,
		TYRANNY_RANKING_DISPLAY_SLOT_OWN,
	};

private:
	CBasicTextBox*		m_pRankText;
	CBasicTextBox*		m_pNameText;
	CBasicTextBox*		m_pScoreText;

	CPVPTyrannyRankingsDisplayResuSlot*		m_pSlot[TYRANNY_RANKING_NUM];
	CPVPTyrannyRankingsDisplayResuSlotSelf*	m_pSlotOwn;

	int		m_nStart;
	int		m_nTotal;

public:
	CPVPTyrannyRankingsDisplayResu();
	~CPVPTyrannyRankingsDisplayResu();

	void	CreateSubControl ();
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:	
	void	RefreshTyrannyRankingResu();
	void	ShowData();
};