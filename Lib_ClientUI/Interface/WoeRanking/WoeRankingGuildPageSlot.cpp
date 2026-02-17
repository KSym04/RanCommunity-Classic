#include "stdafx.h"

#include "WoeRankingGuildPageSlot.h"
#include "../BasicTextBox.h"
#include "../UITextControl.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"

void CWoeRankingGuildPageSlot::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);

	m_pRankResult = new CBasicTextBox;
	m_pRankResult->CreateSub(this, "WOE_RANKING_GUILD_RANK_RESULT");
	m_pRankResult->SetFont(pFont8);
	m_pRankResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pRankResult);

	m_pLeaderResult = new CBasicTextBox;
	m_pLeaderResult->CreateSub(this, "WOE_RANKING_GUILD_LEADER_RESULT");
	m_pLeaderResult->SetFont(pFont8);
	m_pLeaderResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pLeaderResult);

	m_pNameResult = new CBasicTextBox;
	m_pNameResult->CreateSub(this, "WOE_RANKING_GUILD_NAME_RESULT");
	m_pNameResult->SetFont(pFont8);
	m_pNameResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pNameResult);

	m_pKillResult = new CBasicTextBox;
	m_pKillResult->CreateSub(this, "WOE_RANKING_GUILD_KILL_RESULT");
	m_pKillResult->SetFont(pFont8);
	m_pKillResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pKillResult);

	m_pDeathResult = new CBasicTextBox;
	m_pDeathResult->CreateSub(this, "WOE_RANKING_GUILD_DEATH_RESULT");
	m_pDeathResult->SetFont(pFont8);
	m_pDeathResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pDeathResult);
}
void CWoeRankingGuildPageSlot::ClearData()
{
	if (m_pRankResult)		m_pRankResult->ClearText();
	if (m_pLeaderResult)	m_pLeaderResult->ClearText();
	if (m_pNameResult)		m_pNameResult->ClearText();
	if (m_pKillResult)		m_pKillResult->ClearText();
	if (m_pDeathResult)		m_pDeathResult->ClearText();
}
void CWoeRankingGuildPageSlot::UpdateData(int nRank, std::string strLeader, std::string strName, WORD wKill, WORD wDeath)
{
	ClearData();

	CString strText;

	if (m_pRankResult) {
		strText.Format("%d", nRank);
		m_pRankResult->AddText(strText.GetString(), NS_UITEXTCOLOR::ITEMMONEY);
	}
	if (m_pLeaderResult) {
		m_pLeaderResult->AddText(strLeader.c_str(), NS_UITEXTCOLOR::WHITE);
	}
	if (m_pNameResult) {
		m_pNameResult->AddText(strName.c_str(), NS_UITEXTCOLOR::WHITE);
	}
	if (m_pKillResult) {
		strText.Format("%d", wKill);
		m_pKillResult->AddText(strText.GetString(), NS_UITEXTCOLOR::GREEN_WOW);
	}
	if (m_pDeathResult) {
		strText.Format("%d", wDeath);
		m_pDeathResult->AddText(strText.GetString(), NS_UITEXTCOLOR::RED);
	}
}