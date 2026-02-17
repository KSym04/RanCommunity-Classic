#include "stdafx.h"

#include "WoeRanking.h"
#include "../BasicLineBox.h"
#include "../Lib_Engine/GUInterface/UIControl.h"
#include "../BasicTextBox.h"
#include "../UITextControl.h"
#include "../InnerInterface.h"

#include "WoeRankingGuildPage.h"
#include "WoeRankingPlayerPage.h"
#include "WoeRankingResuPage.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"

void CWoeRanking::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);

	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub(this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLineBox->CreateBaseBoxQuestList("WOE_LINEBOX_LEFT");
	RegisterControl(m_pLineBox);

	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub(this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLineBox->CreateBaseBoxQuestList("WOE_LINEBOX_UPPER");
	RegisterControl(m_pLineBox);

	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub(this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLineBox->CreateBaseBoxWhiteBlankBody("WOE_LINEBOX_WHITE_UPPER");
	RegisterControl(m_pLineBox);

	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub(this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLineBox->CreateBaseBoxWhiteBlankBody("WOE_LINEBOX_WHITE_LOWER");
	RegisterControl(m_pLineBox);

	m_pTextBoxPlayer = new CBasicTextBox;
	m_pTextBoxPlayer->CreateSub(this, "WOE_PLAYER_STATIC", TEXT_ALIGN_LEFT, WOE_SELECTION_PLAYER);
	m_pTextBoxPlayer->SetFont(pFont8);
	m_pTextBoxPlayer->SetTextAlign(TEXT_ALIGN_LEFT);
	m_pTextBoxPlayer->AddText("Top 10 Player");
	RegisterControl(m_pTextBoxPlayer);

	m_pTextBoxGuild = new CBasicTextBox;
	m_pTextBoxGuild->CreateSub(this, "WOE_GUILD_STATIC", TEXT_ALIGN_LEFT, WOE_SELECTION_GUILD);
	m_pTextBoxGuild->SetFont(pFont8);
	m_pTextBoxGuild->SetTextAlign(TEXT_ALIGN_LEFT);
	m_pTextBoxGuild->AddText("Top 10 Guild");
	RegisterControl(m_pTextBoxGuild);

	m_pTextBoxResu = new CBasicTextBox;
	m_pTextBoxResu->CreateSub(this, "WOE_RESU_STATIC", TEXT_ALIGN_LEFT, WOE_SELECTION_RESU);
	m_pTextBoxResu->SetFont(pFont8);
	m_pTextBoxResu->SetTextAlign(TEXT_ALIGN_LEFT);
	m_pTextBoxResu->AddText("Top 10 Resu");
	RegisterControl(m_pTextBoxResu);

	std::string button[BUTTON_SIZE] =
	{
		"WOE_BUTTON0",
		"WOE_BUTTON1",
		"WOE_BUTTON2"
	};
	std::string buttonf[BUTTON_SIZE] =
	{
		"WOE_BUTTON0_F",
		"WOE_BUTTON1_F",
		"WOE_BUTTON2_F"
	};
	
	for (int i = 0; i < BUTTON_SIZE; i++) {
		m_pButton[i] = new CUIControl;
		m_pButton[i]->CreateSub(this, button[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE, WOE_BUTTON0 + i);
		RegisterControl(m_pButton[i]);

		m_pButtonF[i] = new CUIControl;
		m_pButtonF[i]->CreateSub(this, buttonf[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pButtonF[i]->SetVisibleSingle(false);
		RegisterControl(m_pButtonF[i]);
	}

	m_pButtonF[0]->SetVisibleSingle(true);

	m_pWoePlayerPage = new CWoeRankingPlayerPage;
	m_pWoePlayerPage->CreateSub(this, "WOE_PLAYER_PAGE", TEXT_ALIGN_CENTER_X);
	m_pWoePlayerPage->CreateSubControl();
	RegisterControl(m_pWoePlayerPage);

	m_pWoeGuildPage = new CWoeRankingGuildPage;
	m_pWoeGuildPage->CreateSub(this, "WOE_GUILD_PAGE", TEXT_ALIGN_CENTER_X);
	m_pWoeGuildPage->CreateSubControl();
	m_pWoeGuildPage->SetVisibleSingle(false);
	RegisterControl(m_pWoeGuildPage);

	m_pWoeResuPage = new CWoeRankingResuPage;
	m_pWoeResuPage->CreateSub(this, "WOE_RESU_PAGE", TEXT_ALIGN_CENTER_X);
	m_pWoeResuPage->CreateSubControl();
	m_pWoeResuPage->SetVisibleSingle(false);
	RegisterControl(m_pWoeResuPage);

}
void CWoeRanking::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{
	case WOE_BUTTON0:
	case WOE_SELECTION_PLAYER:
		{
			if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			{
				PlayerRadio();
			}
		}
		break;
	case WOE_BUTTON1:
	case WOE_SELECTION_GUILD:
		{
			if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			{
				GuildRadio();
			}
		}
		break;
	case WOE_BUTTON2:
	case WOE_SELECTION_RESU:
		{
			if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			{
				ResuRadio();
			}
		}
		break;
	}
}
void CWoeRanking::PlayerRadio()
{
	for (int i = 0; i < BUTTON_SIZE; i++) {
		m_pButton[i]->SetVisibleSingle(true);
		m_pButtonF[i]->SetVisibleSingle(false);
	}
	m_pButton[0]->SetVisibleSingle(false);
	m_pButtonF[0]->SetVisibleSingle(true);

	m_pTextBoxPlayer->SetUseTextColor(0, TRUE);
	m_pTextBoxPlayer->SetTextColor(0, NS_UITEXTCOLOR::ORANGE);

	m_pTextBoxGuild->SetTextColor(0, NS_UITEXTUTIL::ENABLE);
	m_pTextBoxResu->SetTextColor(0, NS_UITEXTUTIL::ENABLE);

	m_pWoePlayerPage->SetVisibleSingle(true);
	m_pWoeGuildPage->SetVisibleSingle(false);
	m_pWoeResuPage->SetVisibleSingle(false);
}
void CWoeRanking::GuildRadio()
{
	for (int i = 0; i < BUTTON_SIZE; i++) {
		m_pButton[i]->SetVisibleSingle(true);
		m_pButtonF[i]->SetVisibleSingle(false);
	}
	m_pButton[1]->SetVisibleSingle(false);
	m_pButtonF[1]->SetVisibleSingle(true);

	m_pTextBoxGuild->SetUseTextColor(0, TRUE);
	m_pTextBoxGuild->SetTextColor(0, NS_UITEXTCOLOR::ORANGE);

	m_pTextBoxPlayer->SetTextColor(0, NS_UITEXTUTIL::ENABLE);
	m_pTextBoxResu->SetTextColor(0, NS_UITEXTUTIL::ENABLE);

	m_pWoePlayerPage->SetVisibleSingle(false);
	m_pWoeGuildPage->SetVisibleSingle(true);
	m_pWoeResuPage->SetVisibleSingle(false);
}
void CWoeRanking::ResuRadio()
{
	for (int i = 0; i < BUTTON_SIZE; i++) {
		m_pButton[i]->SetVisibleSingle(true);
		m_pButtonF[i]->SetVisibleSingle(false);
	}
	m_pButton[2]->SetVisibleSingle(false);
	m_pButtonF[2]->SetVisibleSingle(true);

	m_pTextBoxResu->SetUseTextColor(0, TRUE);
	m_pTextBoxResu->SetTextColor(0, NS_UITEXTCOLOR::ORANGE);

	m_pTextBoxGuild->SetTextColor(0, NS_UITEXTUTIL::ENABLE);
	m_pTextBoxPlayer->SetTextColor(0, NS_UITEXTUTIL::ENABLE);

	m_pWoePlayerPage->SetVisibleSingle(false);
	m_pWoeGuildPage->SetVisibleSingle(false);
	m_pWoeResuPage->SetVisibleSingle(true);
}
void CWoeRanking::RefreshRanking()
{
	m_pWoePlayerPage->Refresh();
	m_pWoeGuildPage->Refresh();
	m_pWoeResuPage->Refresh();
}
void CWoeRanking::SetVisibleSingle(BOOL bVisible)
{
	CUIWindowEx::SetVisibleSingle(bVisible);
}