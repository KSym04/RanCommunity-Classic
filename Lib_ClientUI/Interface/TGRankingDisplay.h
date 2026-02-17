#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLPVPTyrannyData.h"

class CBasicTextBox;
class CTGRankingDisplaySlot;

class CTGRankingDisplay : public CUIGroup
{
	enum
	{
		CDM_RANKING_SLOT_0 = NO_ID + 1,
		CDM_RANKING_SLOT_1,
		CDM_RANKING_SLOT_2,
		CDM_RANKING_SLOT_3,
		CDM_RANKING_SLOT_4,
		CDM_RANKING_SLOT_5,
		CDM_RANKING_SLOT_6,
		CDM_RANKING_SLOT_7,
		CDM_RANKING_SLOT_8,
		CDM_RANKING_SLOT_9,
		CDM_RANKING_SLOT_OWN,
	};

private:
	CBasicTextBox*		m_pRankText;
	CBasicTextBox*		m_pClubText;
	CBasicTextBox*		m_pScoreText;

	CTGRankingDisplaySlot*		m_pSlot[TYRANNY_GUILD_RANKING_NUM];
	CTGRankingDisplaySlot*		m_pSlotOwn;

public:
	CTGRankingDisplay();
	~CTGRankingDisplay();

	void	CreateSubControl ();
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:	
	void	RefreshTGRanking();
};