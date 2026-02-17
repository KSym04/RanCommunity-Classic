#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CPVPClubDeathMatchRankingPageSlot;
class CBasicScrollBarEx;
class CBasicTextBox;

class CPVPClubDeathMatchRankingPage : public CUIGroup
{
private:
	enum
	{
		PVP_CLUB_DEATH_MATCH_RANKING_PAGE_SCROLLBAR = NO_ID + 1,
	};

	enum
	{
		PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT = 10,
	};

public:
	CPVPClubDeathMatchRankingPage ();
	virtual	~CPVPClubDeathMatchRankingPage ();

public:
	void	CreateSubControl ( int nType );
	CBasicTextBox*	CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign );

public:
	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextClubName;
	CBasicTextBox*	m_pTextClubMaster;
	CBasicTextBox*	m_pTextPoint;
	CBasicTextBox*	m_pTextKill;
	CBasicTextBox*	m_pTextDeath;
	CBasicTextBox*	m_pTextResu;
	CBasicTextBox*	m_pTextReward;


	CPVPClubDeathMatchRankingPageSlot*		m_pSlotRank[PVP_CLUB_DEATH_MATCH_RANKING_PAGE_MAX_SLOT];
	CPVPClubDeathMatchRankingPageSlot*		m_pSlotRankSelf;
	CBasicScrollBarEx*				m_pScrollBar;

public:
	int		m_nType;
	int		m_nStart;
	int		m_nTotal;

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void	RefreshRanking();
	void	RenderView();

};