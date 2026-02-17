#include "stdafx.h"

#include "WoeRankingResuPage.h"
#include "WoeRankingResuPageSlot.h"
#include "../BasicLineBox.h"
#include "../BasicTextBox.h"
#include "../Lib_Engine/GUInterface/UIControl.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"

void CWoeRankingResuPage::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);

	m_pTextBoxRank = new CBasicTextBox;
	m_pTextBoxRank->CreateSub(this, "WOE_RANKING_RESU_RANK_STATIC");
	m_pTextBoxRank->SetFont(pFont8);
	m_pTextBoxRank->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxRank->AddText("Rank");
	RegisterControl(m_pTextBoxRank);

	m_pTextBoxSchool = new CBasicTextBox;
	m_pTextBoxSchool->CreateSub(this, "WOE_RANKING_RESU_SCHOOL_STATIC");
	m_pTextBoxSchool->SetFont(pFont8);
	m_pTextBoxSchool->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxSchool->AddText("School");
	RegisterControl(m_pTextBoxSchool);

	m_pTextBoxClass = new CBasicTextBox;
	m_pTextBoxClass->CreateSub(this, "WOE_RANKING_RESU_CLASS_STATIC");
	m_pTextBoxClass->SetFont(pFont8);
	m_pTextBoxClass->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxClass->AddText("Class");
	RegisterControl(m_pTextBoxClass);

	m_pTextBoxName = new CBasicTextBox;
	m_pTextBoxName->CreateSub(this, "WOE_RANKING_RESU_NAME_STATIC");
	m_pTextBoxName->SetFont(pFont8);
	m_pTextBoxName->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxName->AddText("Name");
	RegisterControl(m_pTextBoxName);

	m_pTextBoxResu = new CBasicTextBox;
	m_pTextBoxResu->CreateSub(this, "WOE_RANKING_RESU_KILL_STATIC");
	m_pTextBoxResu->SetFont(pFont8);
	m_pTextBoxResu->SetTextAlign(TEXT_ALIGN_CENTER_X);
	m_pTextBoxResu->AddText("Resurrection");
	RegisterControl(m_pTextBoxResu);

	std::string strslot[RANKING_SIZE] =
	{
		"WOE_RANKING_RESU_SLOT0",
		"WOE_RANKING_RESU_SLOT1",
		"WOE_RANKING_RESU_SLOT2",
		"WOE_RANKING_RESU_SLOT3",
		"WOE_RANKING_RESU_SLOT4",
		"WOE_RANKING_RESU_SLOT5",
		"WOE_RANKING_RESU_SLOT6",
		"WOE_RANKING_RESU_SLOT7",
		"WOE_RANKING_RESU_SLOT8",
		"WOE_RANKING_RESU_SLOT9"
	};

	for (int i = 0; i < RANKING_SIZE; i++) {
		m_pSlot[i] = new CWoeRankingResuPageSlot;
		m_pSlot[i]->CreateSub(this, strslot[i].c_str());
		m_pSlot[i]->CreateSubControl();
		RegisterControl(m_pSlot[i]);
	}

	m_pOwnSlot = new CWoeRankingResuPageSlot;
	m_pOwnSlot->CreateSub(this, "WOE_RANKING_RESU_OWN_SLOT", TEXT_ALIGN_CENTER_X);
	m_pOwnSlot->CreateSubControl();
	RegisterControl(m_pOwnSlot);
}
void CWoeRankingResuPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;

	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}
void CWoeRankingResuPage::Refresh()
{
	for (int i = 0; i < RANKING_NUM; ++i) {
		if (m_pSlot[i])	m_pSlot[i]->ClearData();
	}

	GLCharacter* pCHAR = GLGaeaClient::GetInstance().GetCharacter();
	if (!pCHAR)		return;

	EMCHARINDEX emIndex = CharClassToIndex(pCHAR->m_emClass);

	SWOE_RESU_RANK_EX& sMyRank = GLGaeaClient::GetInstance().m_sSelfWoeResuRank;

	if (m_pOwnSlot)
		m_pOwnSlot->UpdateData(sMyRank.wRanking, pCHAR->m_wSchool, emIndex, pCHAR->m_szName, sMyRank.wResuNum);

	int nRank = GLGaeaClient::GetInstance().m_vecWoeResuRank.size();

	WOE_RANK_RESU_VEC& sRank = GLGaeaClient::GetInstance().m_vecWoeResuRank;

	for (int i = 0; i < nRank; i++) {
		if (i >= RANKING_NUM) {
			continue;
		}
		if (m_pSlot[i]) {
			m_pSlot[i]->UpdateData(sRank[i].wRanking, sRank[i].wSchool, sRank[i].wClass, sRank[i].szName, sRank[i].wResuNum);
		}
	}
}
void CWoeRankingResuPage::ClearData()
{
	if (m_pTextBoxRank)		m_pTextBoxRank->ClearText();
	if (m_pTextBoxSchool)	m_pTextBoxSchool->ClearText();
	if (m_pTextBoxClass)	m_pTextBoxClass->ClearText();
	if (m_pTextBoxName)		m_pTextBoxName->ClearText();
	if (m_pTextBoxResu)		m_pTextBoxResu->ClearText();
}