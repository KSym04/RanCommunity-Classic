#include "stdafx.h"
#include "PVPWoePage.h"
#include "PVPWoePageBattle.h"
#include "PVPWoePageHistory.h"

#include "../Lib_Engine/DxCommon/D3DFont.h"
#include "GameTextControl.h"
#include "UITextControl.h"

#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "BasicTextButton.h"

#include "../Lib_Client/G-Logic/GLPVPWoeClient.h"
#include "InnerInterface.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Client/DxGlobalStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPWoePage::CPVPWoePage ()
	: m_pTextMapName(NULL)
	, m_pInfoBattle(NULL)
	, m_pInfoHistory(NULL)
	, m_pTextInfoMechanics(NULL)
	, m_pTextInfoTitle(NULL)
	, m_pTextInfoTitle1(NULL)
	, m_pTextInfoMatches(NULL)
	, m_pTextInfoPets(NULL)
	, m_pTextReward(NULL)
	, m_pTextRewardItem(NULL)
	, m_pTextRewardContrib(NULL)
	, m_fUpdateTime(0.0f)
{
}

CPVPWoePage::~CPVPWoePage ()
{
}

void CPVPWoePage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CreateUILineBoxQuestList( "COMPETITION_WOE_PAGE_INFO_LINEBOX_HEAD" );
	CreateUILineBoxQuestList( "COMPETITION_WOE_PAGE_INFO_LINEBOX_HEAD" );
	CreateUILineBoxQuestList( "COMPETITION_WOE_PAGE_INFO_LINEBOX_BODY" );
	CreateUILineBoxQuestList( "COMPETITION_WOE_PAGE_INFO_LINEBOX_MAP" );
	CreateUILineBoxQuestList( "COMPETITION_WOE_PAGE_INFO_LINEBOX_BOTTOM" );

	CreateUILineBoxWhiteNoBody( "COMPETITION_WOE_PAGE_INFO_LINEBOX_HEAD" );
	CreateUILineBoxWhiteNoBody( "COMPETITION_WOE_PAGE_INFO_LINEBOX_BODY" );
	CreateUILineBoxWhiteNoBody( "COMPETITION_WOE_PAGE_INFO_LINEBOX_MAP" );
	CreateUILineBoxWhiteNoBody( "COMPETITION_WOE_PAGE_INFO_LINEBOX_BOTTOM" );

	CreateUIControl( "COMPETITION_WOE_PAGE_INFO_MAP" );
	CreateUIControl( "COMPETITION_WOE_PAGE_INFO_GUILD1" );
	CreateUIControl( "COMPETITION_WOE_PAGE_INFO_GUILD2" );

	m_pTextMapName = CreateUITextBox( "COMPETITION_WOE_PAGE_INFO_TITLE", pFont9, TEXT_ALIGN_LEFT| TEXT_ALIGN_CENTER_Y );
	m_pTextMapName->AddText("War of Emperium", NS_UITEXTCOLOR::ORANGE);

	m_pTextInfoMechanics = CreateUITextBox("COMPETITION_WOE_PAGE_INFO_DESC_MECHANICS", pFont9, TEXT_ALIGN_LEFT);

	m_pTextInfoTitle = CreateUITextBox( "COMPETITION_WOE_PAGE_INFO_DESC_TITLE", pFont9, TEXT_ALIGN_LEFT );
	m_pTextInfoTitle1 = CreateUITextBox("COMPETITION_WOE_PAGE_INFO_DESC_TITLE1", pFont9, TEXT_ALIGN_LEFT);

	m_pTextInfoMatches = CreateUITextBox("COMPETITION_WOE_PAGE_INFO_DESC_MATCHES", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoPets = CreateUITextBox("COMPETITION_WOE_PAGE_INFO_DESC_PETS", pFont9, TEXT_ALIGN_LEFT);

	m_pTextReward = CreateUITextBox("COMPETITION_WOE_PAGE_INFO_DESC_REWARD", pFont9, TEXT_ALIGN_LEFT);
	m_pTextRewardItem = CreateUITextBox("COMPETITION_WOE_PAGE_INFO_DESC_REWARD_ITEM", pFont9, TEXT_ALIGN_LEFT);
	m_pTextRewardContrib = CreateUITextBox("COMPETITION_WOE_PAGE_INFO_DESC_REWARD_CONTRIB", pFont9, TEXT_ALIGN_LEFT);
	
	m_pInfoBattle = new CPVPWoePageBattle;
	m_pInfoBattle->CreateSub( this, "COMPETITION_WOE_PAGE_BATTLE", UI_FLAG_DEFAULT, WOE_PAGE_INFO_BATTLE );
	m_pInfoBattle->CreateSubControl ();
	RegisterControl ( m_pInfoBattle );

	m_pInfoHistory = new CPVPWoePageHistory;
	m_pInfoHistory->CreateSub( this, "COMPETITION_WOE_PAGE_HISTORY", UI_FLAG_DEFAULT, WOE_PAGE_INFO_HISTORY );
	m_pInfoHistory->CreateSubControl ();
	RegisterControl ( m_pInfoHistory );
}

CBasicTextBox* CPVPWoePage::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

CBasicLineBox* CPVPWoePage::CreateUILineBoxQuestList( char* szBaseControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( szBaseControl );
	RegisterControl ( pLineBox );
	return pLineBox;
}

CBasicLineBox* CPVPWoePage::CreateUILineBoxWhiteNoBody( char* szBaseControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( szBaseControl );
	RegisterControl ( pLineBox );

	return pLineBox;
}

CUIControl* CPVPWoePage::CreateUIControl( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pControl->SetVisibleSingle( TRUE );
	RegisterControl ( pControl );
	return pControl;
}

void CPVPWoePage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CPVPWoePage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f )
	{
		UpdateInfo();
		m_fUpdateTime = 0.0f;
	}
}

void CPVPWoePage::UpdateInfo()
{
	if (m_pTextInfoMechanics)
	{
		m_pTextInfoMechanics->ClearText();
		m_pTextInfoMechanics->AddText(ID2GAMEWORD("COMPETITION_WOE_BATTLE_INFO_TEXT", 0), NS_UITEXTCOLOR::GREENYELLOW);
	}
	if ( m_pTextInfoTitle )
	{
		m_pTextInfoTitle->ClearText();
		m_pTextInfoTitle->AddText( ID2GAMEWORD( "COMPETITION_WOE_BATTLE_INFO_TEXT", 1 ), NS_UITEXTCOLOR::WHITE );
	}
	if (m_pTextInfoTitle1)
	{
		m_pTextInfoTitle1->ClearText();
		m_pTextInfoTitle1->AddText(ID2GAMEWORD("COMPETITION_WOE_BATTLE_INFO_TEXT", 2), NS_UITEXTCOLOR::WHITE);
	}
	if (m_pTextInfoMatches)
	{
		m_pTextInfoMatches->ClearText();
		m_pTextInfoMatches->AddText(ID2GAMEWORD("COMPETITION_WOE_BATTLE_INFO_TEXT", 3), NS_UITEXTCOLOR::WHITE);
	}
	if (m_pTextInfoPets)
	{
		m_pTextInfoPets->ClearText();
		m_pTextInfoPets->AddText(ID2GAMEWORD("COMPETITION_WOE_BATTLE_INFO_TEXT", 4), NS_UITEXTCOLOR::WHITE);
	}
	
	if ( m_pTextReward )
	{
		m_pTextReward->ClearText();
		m_pTextReward->AddText( ID2GAMEWORD( "COMPETITION_WOE_BATTLE_INFO_REWARD", 0), NS_UITEXTCOLOR::GREENYELLOW);
	}
	if (m_pTextRewardItem)
	{
		m_pTextRewardItem->ClearText();
		m_pTextRewardItem->AddText(ID2GAMEWORD("COMPETITION_WOE_BATTLE_INFO_REWARD", 1), NS_UITEXTCOLOR::WHITE);
	}
	if (m_pTextRewardContrib)
	{
		m_pTextRewardContrib->ClearText();
		m_pTextRewardContrib->AddText(ID2GAMEWORD("COMPETITION_WOE_BATTLE_INFO_REWARD", 2), NS_UITEXTCOLOR::WHITE);
	}
}
void CPVPWoePage::SetCompetitionJoin( bool bEnable )
{
	if ( m_pInfoBattle )
		m_pInfoBattle->SetCompetitionJoin( bEnable );
}