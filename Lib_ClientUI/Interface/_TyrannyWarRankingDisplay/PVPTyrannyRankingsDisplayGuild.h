#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

#include "../Lib_Client/G-Logic/GLPVPTyrannyDefine.h"

/* 7-13-23 Tyranny War Ranking System - Montage */

class CBasicTextBox;
class CPVPTyrannyRankingsDisplayGuildSlot;
class CPVPTyrannyRankingsDisplayGuildSlotSelf;

class CPVPTyrannyRankingsDisplayGuild : public CUIGroup
{
	enum
	{
		TYRANNY_CLUB_RANKING_SLOT_0 = NO_ID + 1,
		TYRANNY_CLUB_RANKING_SLOT_1,
		TYRANNY_CLUB_RANKING_SLOT_2,
		TYRANNY_CLUB_RANKING_SLOT_3,
		TYRANNY_CLUB_RANKING_SLOT_4,
		TYRANNY_CLUB_RANKING_SLOT_5,
		TYRANNY_CLUB_RANKING_SLOT_6,
		TYRANNY_CLUB_RANKING_SLOT_7,
		TYRANNY_CLUB_RANKING_SLOT_8,
		TYRANNY_CLUB_RANKING_SLOT_9,
		TYRANNY_CLUB_RANKING_SLOT_OWN,
	};

private:
	CBasicTextBox*		m_pRankText;
	CBasicTextBox*		m_pClubText;
	CBasicTextBox*		m_pScoreText;

	CPVPTyrannyRankingsDisplayGuildSlot*		m_pSlot[TYRANNY_CLUB_RANKING_NUM];
	CPVPTyrannyRankingsDisplayGuildSlotSelf*	m_pSlotOwn;

	int		m_nStart;
	int		m_nTotal;

public:
	CPVPTyrannyRankingsDisplayGuild();
	~CPVPTyrannyRankingsDisplayGuild();

	void	CreateSubControl ();
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:	
	void	RefreshTyrannyClubRanking();
	void	ShowData();
};