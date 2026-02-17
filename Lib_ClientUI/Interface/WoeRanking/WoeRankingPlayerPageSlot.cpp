#include "stdafx.h"

#include "WoeRankingPlayerPageSlot.h"
#include "../BasicTextBox.h"
#include "../UITextControl.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CWoeRankingPlayerPageSlot::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);

	m_pRankResult = new CBasicTextBox;
	m_pRankResult->CreateSub(this, "WOE_RANKING_PLAYER_RANK_RESULT");
	m_pRankResult->SetFont(pFont8);
	m_pRankResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pRankResult);

	m_pNameResult = new CBasicTextBox;
	m_pNameResult->CreateSub(this, "WOE_RANKING_PLAYER_NAME_RESULT");
	m_pNameResult->SetFont(pFont8);
	m_pNameResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pNameResult);

	m_pKillResult = new CBasicTextBox;
	m_pKillResult->CreateSub(this, "WOE_RANKING_PLAYER_KILL_RESULT");
	m_pKillResult->SetFont(pFont8);
	m_pKillResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pKillResult);

	m_pDeathResult = new CBasicTextBox;
	m_pDeathResult->CreateSub(this, "WOE_RANKING_PLAYER_DEATH_RESULT");
	m_pDeathResult->SetFont(pFont8);
	m_pDeathResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pDeathResult);

	m_pCountHit = new CBasicTextBox;
	m_pCountHit->CreateSub(this, "WOE_RANKING_PLAYER_COUNTHIT_RESULT");
	m_pCountHit->SetFont(pFont8);
	m_pCountHit->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pCountHit);

	std::string strSchool[GLSCHOOL_NUM] =
	{
		"WOE_RANKING_PLAYER_SCHOOL0_RESULT",
		"WOE_RANKING_PLAYER_SCHOOL1_RESULT",
		"WOE_RANKING_PLAYER_SCHOOL2_RESULT"
	};
	for (int i = 0; i < GLSCHOOL_NUM; i++) {
		m_pSchoolResult[i] = new CUIControl;
		m_pSchoolResult[i]->CreateSub(this, strSchool[i].c_str(), UI_FLAG_CENTER_X);
		RegisterControl(m_pSchoolResult[i]);
	}

	std::string strClass[GLCI_NUM_6CLASS] =
	{
		"WOE_RANKING_PLAYER_BRAWLER_M",
		"WOE_RANKING_PLAYER_SWORDIE_M",
		"WOE_RANKING_PLAYER_ARCHER_W",
		"WOE_RANKING_PLAYER_SHAMAN_W",
		"WOE_RANKING_PLAYER_EXTREME_M",
		"WOE_RANKING_PLAYER_EXTREME_W",
		"WOE_RANKING_PLAYER_BRAWLER_W",
		"WOE_RANKING_PLAYER_SWORDIE_W",
		"WOE_RANKING_PLAYER_ARCHER_M",
		"WOE_RANKING_PLAYER_SHAMAN_M",
		"WOE_RANKING_PLAYER_GUNNER_M",
		"WOE_RANKING_PLAYER_GUNNER_W"
	};
	for (int i = 0; i < GLCI_NUM_6CLASS; ++i) {
		m_pClassResult[i] = new CUIControl;
		m_pClassResult[i]->CreateSub(this, strClass[i].c_str(), UI_FLAG_CENTER_X);
		m_pClassResult[i]->SetVisibleSingle(false);
		RegisterControl(m_pClassResult[i]);
	}
}
void CWoeRankingPlayerPageSlot::UpdateData(int nRank, WORD wSchool, WORD wClass, std::string strName, WORD wKill, WORD wDeath, DWORD dwCountHit)
{
	ClearData();

	CString strText;

	if (m_pRankResult) {
		strText.Format("%d", nRank);
		m_pRankResult->AddText(strText.GetString(), NS_UITEXTCOLOR::ITEMMONEY);
	}
	if (m_pSchoolResult) {
		if (wSchool < GLSCHOOL_NUM) {
			m_pSchoolResult[wSchool]->SetVisibleSingle(true);
		}
	}
	if (m_pClassResult) {
		if (wClass < GLCI_NUM_6CLASS)
		m_pClassResult[wClass]->SetVisibleSingle(true);
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
	if (m_pCountHit) {
		strText.Format("%d", dwCountHit);
		m_pCountHit->AddText(strText.GetString(), NS_UITEXTCOLOR::WHITE);
	}
}
void CWoeRankingPlayerPageSlot::ClearData()
{
	if (m_pRankResult)		m_pRankResult->ClearText();
	if (m_pNameResult)		m_pNameResult->ClearText();
	if (m_pKillResult)		m_pKillResult->ClearText();
	if (m_pDeathResult)		m_pDeathResult->ClearText();
	if (m_pCountHit)		m_pCountHit->ClearText();

	for (int i = 0; i < GLSCHOOL_NUM; i++) {
		m_pSchoolResult[i]->SetVisibleSingle(false);
	}

	for (int i = 0; i < GLCI_NUM_6CLASS; ++i) {
		if (m_pClassResult[i])
		m_pClassResult[i]->SetVisibleSingle(false);
	}
}
