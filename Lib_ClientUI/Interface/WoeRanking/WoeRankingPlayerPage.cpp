#include "stdafx.h"

#include "WoeRankingPlayerPage.h"
#include "WoeRankingPlayerPageSlot.h"
#include "../BasicLineBox.h"
#include "../BasicTextBox.h"
#include "../Lib_Engine/GUInterface/UIControl.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"

void CWoeRankingPlayerPage::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);

	m_pTextBoxRank = new CBasicTextBox;
	m_pTextBoxRank->CreateSub(this, "WOE_RANKING_PLAYER_RANK_STATIC");
	m_pTextBoxRank->SetFont(pFont8);
	m_pTextBoxRank->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxRank->AddText("Rank");
	RegisterControl(m_pTextBoxRank);

	m_pTextBoxSchool = new CBasicTextBox;
	m_pTextBoxSchool->CreateSub(this, "WOE_RANKING_PLAYER_SCHOOL_STATIC");
	m_pTextBoxSchool->SetFont(pFont8);
	m_pTextBoxSchool->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxSchool->AddText("School");
	RegisterControl(m_pTextBoxSchool);

	m_pTextBoxClass = new CBasicTextBox;
	m_pTextBoxClass->CreateSub(this, "WOE_RANKING_PLAYER_CLASS_STATIC");
	m_pTextBoxClass->SetFont(pFont8);
	m_pTextBoxClass->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxClass->AddText("Class");
	RegisterControl(m_pTextBoxClass);

	m_pTextBoxName = new CBasicTextBox;
	m_pTextBoxName->CreateSub(this, "WOE_RANKING_PLAYER_NAME_STATIC");
	m_pTextBoxName->SetFont(pFont8);
	m_pTextBoxName->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxName->AddText("Name");
	RegisterControl(m_pTextBoxName);

	m_pTextBoxKill = new CBasicTextBox;
	m_pTextBoxKill->CreateSub(this, "WOE_RANKING_PLAYER_KILL_STATIC");
	m_pTextBoxKill->SetFont(pFont8);
	m_pTextBoxKill->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxKill->AddText("Kill");
	RegisterControl(m_pTextBoxKill);

	m_pTextBoxDeath = new CBasicTextBox;
	m_pTextBoxDeath->CreateSub(this, "WOE_RANKING_PLAYER_DEATH_STATIC");
	m_pTextBoxDeath->SetFont(pFont8);
	m_pTextBoxDeath->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxDeath->AddText("Death");
	RegisterControl(m_pTextBoxDeath);

	m_pTextBoxCountHit = new CBasicTextBox;
	m_pTextBoxCountHit->CreateSub(this, "WOE_RANKING_PLAYER_COUNTHIT_STATIC");
	m_pTextBoxCountHit->SetFont(pFont8);
	m_pTextBoxCountHit->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxCountHit->AddText("Damage");
	RegisterControl(m_pTextBoxCountHit);

	std::string strslot[RANKING_SIZE] =
	{
		"WOE_RANKING_PLAYER_SLOT0",
		"WOE_RANKING_PLAYER_SLOT1",
		"WOE_RANKING_PLAYER_SLOT2",
		"WOE_RANKING_PLAYER_SLOT3",
		"WOE_RANKING_PLAYER_SLOT4",
		"WOE_RANKING_PLAYER_SLOT5",
		"WOE_RANKING_PLAYER_SLOT6",
		"WOE_RANKING_PLAYER_SLOT7",
		"WOE_RANKING_PLAYER_SLOT8",
		"WOE_RANKING_PLAYER_SLOT9"
	};

	for (int i = 0; i < RANKING_SIZE; i++) {
		m_pSlot[i] = new CWoeRankingPlayerPageSlot;
		m_pSlot[i]->CreateSub(this, strslot[i].c_str());
		m_pSlot[i]->CreateSubControl();
		RegisterControl(m_pSlot[i]);
	}

	m_pOwnSlot = new CWoeRankingPlayerPageSlot;
	m_pOwnSlot->CreateSub(this, "WOE_RANKING_PLAYER_OWN_SLOT", TEXT_ALIGN_CENTER_X);
	m_pOwnSlot->CreateSubControl();
	RegisterControl(m_pOwnSlot);
}
void CWoeRankingPlayerPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;

	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}
void CWoeRankingPlayerPage::Refresh()
{
	for (int i = 0; i < RANKING_NUM; ++i) {
		if (m_pSlot[i])	m_pSlot[i]->ClearData();
	}

	GLCharacter* pCHAR = GLGaeaClient::GetInstance().GetCharacter();
	if (!pCHAR)		return;

	EMCHARINDEX emIndex = CharClassToIndex(pCHAR->m_emClass);

	SWOE_PLAYER_RANK_EX& sMyRank = GLGaeaClient::GetInstance().m_sSelfWoePlayerRank;

	if (m_pOwnSlot)
		m_pOwnSlot->UpdateData(sMyRank.wRanking, pCHAR->m_wSchool, emIndex, pCHAR->m_szName, sMyRank.wKillNum, sMyRank.wDeathNum, sMyRank.dwHitCount);

	int nRank = GLGaeaClient::GetInstance().m_vecWoePlayerRank.size();

	WOE_PLAYER_RANK_VEC& sRank = GLGaeaClient::GetInstance().m_vecWoePlayerRank;

	for (int i = 0; i < nRank; i++) {
		if (i >= RANKING_NUM) {
			continue;
		}
		if (m_pSlot[i]) {
			m_pSlot[i]->UpdateData(sRank[i].wRanking, sRank[i].wSchool, sRank[i].wClass, sRank[i].szName, sRank[i].wKillNum, sRank[i].wDeathNum, sRank[i].dwCountHit);
		}
	}
}
void CWoeRankingPlayerPage::ClearData()
{
	if (m_pTextBoxRank)		m_pTextBoxRank->ClearText();
	if (m_pTextBoxSchool)	m_pTextBoxSchool->ClearText();
	if (m_pTextBoxClass)	m_pTextBoxClass->ClearText();
	if (m_pTextBoxName)		m_pTextBoxName->ClearText();
	if (m_pTextBoxKill)		m_pTextBoxKill->ClearText();
	if (m_pTextBoxDeath)	m_pTextBoxDeath->ClearText();
	if (m_pTextBoxCountHit)	m_pTextBoxCountHit->ClearText();
}