#pragma once

#define RANKING_SIZE 10
#include "../Lib_ClientUI/Interface/UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLPVPWoeDefine.h"

class CBasicTextBox;
class CWoeRankingGuildPageSlot;
class CWoeRankingGuildPage : public CUIWindowEx
{
public:
	void CreateSubControl();

	CWoeRankingGuildPageSlot* m_pSlot[RANKING_SIZE];
	CWoeRankingGuildPageSlot* m_pOwnSlot;

	CBasicTextBox* m_pTextBoxRank;
	CBasicTextBox* m_pTextBoxLeader;
	CBasicTextBox* m_pTextBoxGuild;
	CBasicTextBox* m_pTextBoxKill;
	CBasicTextBox* m_pTextBoxDeath;

	CBasicTextBox* m_pRankResult;
	CBasicTextBox* m_pLeaderResult;
	CBasicTextBox* m_pNameResult;
	CBasicTextBox* m_pKillResult;
	CBasicTextBox* m_pDeathResult;

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

public:
	void Refresh();
	void ClearData();

protected:
	SWOE_GUILD_RANK sMyRank;
};
