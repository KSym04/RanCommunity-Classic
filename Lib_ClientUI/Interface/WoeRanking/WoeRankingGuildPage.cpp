#include "stdafx.h"

#include "WoeRankingGuildPage.h"
#include "WoeRankingGuildPageSlot.h"
#include "../BasicLineBox.h"
#include "../BasicTextBox.h"
#include "../UITextControl.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"

void CWoeRankingGuildPage::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);

	m_pTextBoxRank = new CBasicTextBox;
	m_pTextBoxRank->CreateSub(this, "WOE_RANKING_GUILD_RANK_STATIC");
	m_pTextBoxRank->SetFont(pFont8);
	m_pTextBoxRank->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxRank->AddText("Rank");
	RegisterControl(m_pTextBoxRank);

	m_pTextBoxLeader = new CBasicTextBox;
	m_pTextBoxLeader->CreateSub(this, "WOE_RANKING_GUILD_LEADER_STATIC");
	m_pTextBoxLeader->SetFont(pFont8);
	m_pTextBoxLeader->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxLeader->AddText("Leader");
	RegisterControl(m_pTextBoxLeader);

	m_pTextBoxGuild = new CBasicTextBox;
	m_pTextBoxGuild->CreateSub(this, "WOE_RANKING_GUILD_NAME_STATIC");
	m_pTextBoxGuild->SetFont(pFont8);
	m_pTextBoxGuild->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxGuild->AddText("Guild Name");
	RegisterControl(m_pTextBoxGuild);

	m_pTextBoxKill = new CBasicTextBox;
	m_pTextBoxKill->CreateSub(this, "WOE_RANKING_GUILD_KILL_STATIC");
	m_pTextBoxKill->SetFont(pFont8);
	m_pTextBoxKill->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxKill->AddText("Kill");
	RegisterControl(m_pTextBoxKill);

	m_pTextBoxDeath = new CBasicTextBox;
	m_pTextBoxDeath->CreateSub(this, "WOE_RANKING_GUILD_DEATH_STATIC");
	m_pTextBoxDeath->SetFont(pFont8);
	m_pTextBoxDeath->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxDeath->AddText("Death");
	RegisterControl(m_pTextBoxDeath);

	std::string strslot[RANKING_SIZE] =
	{
		"WOE_RANKING_GUILD_SLOT0",
		"WOE_RANKING_GUILD_SLOT1",
		"WOE_RANKING_GUILD_SLOT2",
		"WOE_RANKING_GUILD_SLOT3",
		"WOE_RANKING_GUILD_SLOT4",
		"WOE_RANKING_GUILD_SLOT5",
		"WOE_RANKING_GUILD_SLOT6",
		"WOE_RANKING_GUILD_SLOT7",
		"WOE_RANKING_GUILD_SLOT8",
		"WOE_RANKING_GUILD_SLOT9"
	};

	for (int i = 0; i < RANKING_SIZE; i++) {
		m_pSlot[i] = new CWoeRankingGuildPageSlot;
		m_pSlot[i]->CreateSub(this, strslot[i].c_str());
		m_pSlot[i]->CreateSubControl();
		RegisterControl(m_pSlot[i]);
	}

	m_pOwnSlot = new CWoeRankingGuildPageSlot;
	m_pOwnSlot->CreateSub(this, "WOE_RANKING_GUILD_OWN_SLOT", TEXT_ALIGN_CENTER_X);
	m_pOwnSlot->CreateSubControl();
	RegisterControl(m_pOwnSlot);
}
void CWoeRankingGuildPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;

	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}
void CWoeRankingGuildPage::Refresh()
{
	for (int i = 0; i < RANKING_NUM; ++i) {
		if (m_pSlot[i])	m_pSlot[i]->ClearData();
	}

	GLCLUB& sCLUB = GLGaeaClient::GetInstance().GetCharacter()->m_sCLUB;
	CString strClubName;
	strClubName.Format("%s", sCLUB.m_szName);

	CString strLeader;
	strLeader.Format("%s", sCLUB.m_szMasterName);

	SWOE_GUILD_RANK_EX& sMyRank = GLGaeaClient::GetInstance().m_sMyWoeRank;

	if (m_pOwnSlot)
		m_pOwnSlot->UpdateData(sMyRank.wClubRanking, strLeader.GetString(), strClubName.GetString(), sMyRank.wKillNum, sMyRank.wDeathNum);

	int nRank = GLGaeaClient::GetInstance().m_vecWoeGuild.size();

	WOE_GUILD_RANK_VEC& sRank = GLGaeaClient::GetInstance().m_vecWoeGuild;

	for (int i = 0; i < nRank; i++) {
		if (i >= RANKING_NUM) {
			continue;
		}
		if (m_pSlot[i]) {
			m_pSlot[i]->UpdateData(sRank[i].wClubRanking, sRank[i].szLeaderName, sRank[i].szClubName, sRank[i].wKillNum, sRank[i].wDeathNum);
		}
	}
}
void CWoeRankingGuildPage::ClearData()
{
	if (m_pTextBoxRank)		m_pTextBoxRank->ClearText();
	if (m_pTextBoxLeader)	m_pTextBoxLeader->ClearText();
	if (m_pTextBoxGuild)	m_pTextBoxGuild->ClearText();
	if (m_pTextBoxKill)		m_pTextBoxKill->ClearText();
	if (m_pTextBoxDeath)	m_pTextBoxDeath->ClearText();
}