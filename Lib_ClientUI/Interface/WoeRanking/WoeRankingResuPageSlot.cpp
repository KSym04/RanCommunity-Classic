#include "stdafx.h"

#include "WoeRankingResuPageSlot.h"
#include "../BasicTextBox.h"
#include "../UITextControl.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CWoeRankingResuPageSlot::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);

	m_pRankResult = new CBasicTextBox;
	m_pRankResult->CreateSub(this, "WOE_RANKING_RESU_RANK_RESULT");
	m_pRankResult->SetFont(pFont8);
	m_pRankResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pRankResult);

	m_pNameResult = new CBasicTextBox;
	m_pNameResult->CreateSub(this, "WOE_RANKING_RESU_NAME_RESULT");
	m_pNameResult->SetFont(pFont8);
	m_pNameResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pNameResult);

	m_pResuResult = new CBasicTextBox;
	m_pResuResult->CreateSub(this, "WOE_RANKING_RESU_RESU_RESULT");
	m_pResuResult->SetFont(pFont8);
	m_pResuResult->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(m_pResuResult);

	std::string strSchool[GLSCHOOL_NUM] =
	{
		"WOE_RANKING_RESU_SCHOOL0_RESULT",
		"WOE_RANKING_RESU_SCHOOL1_RESULT",
		"WOE_RANKING_RESU_SCHOOL2_RESULT"
	};
	for (int i = 0; i < GLSCHOOL_NUM; i++) {
		m_pSchoolResult[i] = new CUIControl;
		m_pSchoolResult[i]->CreateSub(this, strSchool[i].c_str(), UI_FLAG_CENTER_X);
		RegisterControl(m_pSchoolResult[i]);
	}

	std::string strClass[GLCI_NUM_6CLASS] =
	{
		"WOE_RANKING_RESU_BRAWLER_M",
		"WOE_RANKING_RESU_SWORDIE_M",
		"WOE_RANKING_RESU_ARCHER_W",
		"WOE_RANKING_RESU_SHAMAN_W",
		"WOE_RANKING_RESU_EXTREME_M",
		"WOE_RANKING_RESU_EXTREME_W",
		"WOE_RANKING_RESU_BRAWLER_W",
		"WOE_RANKING_RESU_SWORDIE_W",
		"WOE_RANKING_RESU_ARCHER_M",
		"WOE_RANKING_RESU_SHAMAN_M",
		"WOE_RANKING_RESU_GUNNER_M",
		"WOE_RANKING_RESU_GUNNER_W"
	};
	for (int i = 0; i < GLCI_NUM_6CLASS; ++i) {
		m_pClassResult[i] = new CUIControl;
		m_pClassResult[i]->CreateSub(this, strClass[i].c_str(), UI_FLAG_CENTER_X);
		m_pClassResult[i]->SetVisibleSingle(false);
		RegisterControl(m_pClassResult[i]);
	}
}
void CWoeRankingResuPageSlot::UpdateData(int nRank, WORD wSchool, WORD wClass, std::string strName, WORD wResu)
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
	if (m_pResuResult) {
		strText.Format("%d", wResu);
		m_pResuResult->AddText(strText.GetString(), NS_UITEXTCOLOR::GREEN_WOW);
	}
}
void CWoeRankingResuPageSlot::ClearData()
{
	if (m_pRankResult)		m_pRankResult->ClearText();
	if (m_pNameResult)		m_pNameResult->ClearText();
	if (m_pResuResult)		m_pResuResult->ClearText();

	for (int i = 0; i < GLSCHOOL_NUM; i++) {
		m_pSchoolResult[i]->SetVisibleSingle(false);
	}

	for (int i = 0; i < GLCI_NUM_6CLASS; ++i) {
		if (m_pClassResult[i])
			m_pClassResult[i]->SetVisibleSingle(false);
	}
}
