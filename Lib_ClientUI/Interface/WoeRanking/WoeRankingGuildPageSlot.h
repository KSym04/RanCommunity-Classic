#pragma once

#include "../Lib_ClientUI/Interface/UIWindowEx.h"

class CBasicTextBox;
class CWoeRankingGuildPage;
class CWoeRankingGuildPageSlot : public CUIWindowEx
{
public:
	void CreateSubControl();

	CBasicTextBox* m_pRankResult;
	CBasicTextBox* m_pLeaderResult;
	CBasicTextBox* m_pNameResult;
	CBasicTextBox* m_pKillResult;
	CBasicTextBox* m_pDeathResult;

	void ClearData();
	void UpdateData(int nRank, std::string strLeader, std::string strName, WORD wKill, WORD wDeath);
};