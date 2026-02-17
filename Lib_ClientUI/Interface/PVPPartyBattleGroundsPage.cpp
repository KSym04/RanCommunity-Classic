#include "StdAfx.h"
#include "./PVPPartyBattleGroundsPage.h"

#include "./PVPPartyBattleGroundsPageBattle.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/PVPPartyBattleGroundsClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPPartyBattlePassPage::CPVPPartyBattlePassPage ()
	: m_fUpdateTime(0.0f)
	, m_pTextMapTitle(NULL)
	, m_pTextTargetTitle(NULL)
	, m_pTextDetailTitle(NULL)
	, m_pTextRewardTitle(NULL)
	, m_pTextHistoryTitle(NULL)

	, m_pTextTargetInfo(NULL)
	, m_pTextDetailInfo(NULL)
	, m_pTextRewardInfo(NULL)
	, m_pTextHistoryInfoStatic1(NULL)
	, m_pTextHistoryInfoStatic2(NULL)
	, m_pTextHistoryInfo1(NULL)
	, m_pTextHistoryInfo2(NULL)
//	, m_pImageHistoryTop1(NULL)

	, m_pButtonReward(NULL)
	, m_pButtonRanking(NULL)

	, m_pInfoBattle(NULL)
{
}

CPVPPartyBattlePassPage::~CPVPPartyBattlePassPage ()
{
}

void CPVPPartyBattlePassPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CreateUILineBoxWhiteNoBody( "COMPETITION_PBG_PAGE_INFO_LINEBOX_X" );
	CreateUILineBoxWhiteNoBody( "COMPETITION_PBG_PAGE_INFO_LINEBOX_Y" );

	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_LINEBOX_MAP_BG" );

	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_LINEBOX_MAP_1" );
	CreateUILineBoxWhiteNoBody( "COMPETITION_PBG_PAGE_INFO_LINEBOX_MAP_1" );
	
	CreateUIControl( "COMPETITION_PBG_PAGE_INFO_MAP" );
	CreateUILineBoxWhiteNoBody( "COMPETITION_PBG_PAGE_INFO_LINEBOX_MAP_2" );

	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_TARGET_BG" );
	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_DETAIL_BG" );
	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_REWARD_BG" );
	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_HISTORY_BG" );

	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_TARGET_HEAD" );
	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_DETAIL_HEAD" );
	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_REWARD_HEAD" );
	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_HISTORY_HEAD" );

	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_LINEBOX_BATTLE_BG" );
	CreateUILineBoxQuestList( "COMPETITION_PBG_PAGE_INFO_LINEBOX_BATTLE_BG" );
	CreateUILineBoxWhiteNoBody( "COMPETITION_PBG_PAGE_INFO_LINEBOX_BATTLE_BG" );

	m_pTextMapTitle = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_MAP_TEXT", pFont9, TEXT_ALIGN_LEFT| TEXT_ALIGN_CENTER_Y );
	m_pTextMapTitle->AddText( ID2GAMEWORD( "PVP_PBG_PAGE_TEXT", 0 ), NS_UITEXTCOLOR::ORANGE );

	m_pTextTargetTitle = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_TARGET_TEXT", pFont9, TEXT_ALIGN_LEFT| TEXT_ALIGN_CENTER_Y );
	m_pTextTargetTitle->AddText( ID2GAMEWORD( "PVP_PBG_PAGE_TEXT", 1 ), NS_UITEXTCOLOR::ORANGE );

	m_pTextDetailTitle = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_DETAIL_TEXT", pFont9, TEXT_ALIGN_LEFT| TEXT_ALIGN_CENTER_Y );
	m_pTextDetailTitle->AddText( ID2GAMEWORD( "PVP_PBG_PAGE_TEXT", 2 ), NS_UITEXTCOLOR::ORANGE );

	m_pTextRewardTitle = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_REWARD_TEXT", pFont9, TEXT_ALIGN_LEFT| TEXT_ALIGN_CENTER_Y );
	m_pTextRewardTitle->AddText( ID2GAMEWORD( "PVP_PBG_PAGE_TEXT", 3 ), NS_UITEXTCOLOR::ORANGE );

	m_pTextHistoryTitle = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_HISTORY_TEXT", pFont9, TEXT_ALIGN_LEFT| TEXT_ALIGN_CENTER_Y );
	m_pTextHistoryTitle->AddText( ID2GAMEWORD( "PVP_PBG_PAGE_TEXT", 4 ), NS_UITEXTCOLOR::ORANGE );

	m_pTextTargetInfo = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_TARGET_INFO", pFont9, TEXT_ALIGN_LEFT );
	m_pTextDetailInfo = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_DETAIL_INFO", pFont9, TEXT_ALIGN_LEFT );
	m_pTextRewardInfo = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_REWARD_INFO", pFont9, TEXT_ALIGN_LEFT );

	m_pTextHistoryInfoStatic1 = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_HISTORY_INFO_STATIC_1", pFont9, TEXT_ALIGN_LEFT );
	m_pTextHistoryInfoStatic2 = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_HISTORY_INFO_STATIC_2", pFont9, TEXT_ALIGN_LEFT );
	m_pTextHistoryInfo1 = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_HISTORY_INFO_1", pFont9, TEXT_ALIGN_RIGHT );
	m_pTextHistoryInfo2 = CreateUITextBox( "COMPETITION_PBG_PAGE_INFO_HISTORY_INFO_2", pFont9, TEXT_ALIGN_RIGHT );

//	m_pImageHistoryTop1 = new CUIControl;
//	m_pImageHistoryTop1->CreateSub ( this, "COMPETITION_PBG_PAGE_INFO_HISTORY_CLUB_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
//	m_pImageHistoryTop1->SetUseRender ( TRUE );
//	RegisterControl ( m_pImageHistoryTop1 );

	m_pButtonReward = new CBasicButton;
	m_pButtonReward->CreateSub( this, "COMPETITION_PBG_PAGE_REWARD_BUTTON", UI_FLAG_DEFAULT, PVP_PBG_PAGE_BUTTON_REWARD );
	m_pButtonReward->CreateFlip( "COMPETITION_PBG_PAGE_REWARD_BUTTON_FLIP", CBasicButton::MOUSEIN_FLIP );
	RegisterControl( m_pButtonReward );

	m_pButtonRanking = new CBasicButton;
	m_pButtonRanking->CreateSub( this, "COMPETITION_PBG_PAGE_RANKING_BUTTON", UI_FLAG_DEFAULT, PVP_PBG_PAGE_BUTTON_RANKING );
	m_pButtonRanking->CreateFlip( "COMPETITION_PBG_PAGE_RANKING_BUTTON_FLIP", CBasicButton::MOUSEIN_FLIP );
	RegisterControl( m_pButtonRanking );

	m_pInfoBattle = new CPVPPartyBattleGroundsPageBattle;
	m_pInfoBattle->CreateSub( this, "COMPETITION_PBG_PAGE_INFO_LINEBOX_BATTLE_REGION", UI_FLAG_DEFAULT, PVP_PBG_PAGE_BATTLE_REGION );
	m_pInfoBattle->CreateSubControl ();
	RegisterControl ( m_pInfoBattle );
}

CBasicLineBox* CPVPPartyBattlePassPage::CreateUILineBoxQuestList( char* szBaseControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( szBaseControl );
	RegisterControl ( pLineBox );
	return pLineBox;
}

CBasicLineBox* CPVPPartyBattlePassPage::CreateUILineBoxWhiteNoBody( char* szBaseControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( szBaseControl );
	RegisterControl ( pLineBox );

	return pLineBox;
}

CUIControl* CPVPPartyBattlePassPage::CreateUIControl( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pControl->SetVisibleSingle( TRUE );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextBox* CPVPPartyBattlePassPage::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

void CPVPPartyBattlePassPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case PVP_PBG_PAGE_BUTTON_RANKING:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				CInnerInterface::GetInstance().PVPPBGShowTopRanking();
			}
		}break;

	case PVP_PBG_PAGE_BUTTON_REWARD:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{

			}
		}break;
	};
}

void CPVPPartyBattlePassPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f )
	{
		UpdateDetailInfo();
		m_fUpdateTime = 0.0f;
	}
}

void CPVPPartyBattlePassPage::UpdateDetailInfo()
{
	PVPPBG::ManagerClient &ManagerClient = PVPPBG::ManagerClient::GetInstance();

	if ( m_pTextTargetInfo )
	{
		m_pTextTargetInfo->ClearText();
		m_pTextTargetInfo->AddText( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_TARGET_INFO" ), NS_UITEXTCOLOR::WHITE );
	}

	CString strText = "";
	if ( m_pTextDetailInfo )
	{
		m_pTextDetailInfo->ClearText();

		strText.Format( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_DETAIL_INFO_1" ) );
		m_pTextDetailInfo->AddText( strText, NS_UITEXTCOLOR::WHITE );

		strText.Format( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_DETAIL_INFO_2" ), ManagerClient.m_wPlayerLimit );
		m_pTextDetailInfo->AddText( strText, NS_UITEXTCOLOR::WHITE );

		strText.Format( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_DETAIL_INFO_3" ), ManagerClient.m_wLevelReq );
		m_pTextDetailInfo->AddText( strText, NS_UITEXTCOLOR::WHITE );

		strText.Format( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_DETAIL_INFO_4" ), ManagerClient.m_dwContriReq );
		m_pTextDetailInfo->AddText( strText, NS_UITEXTCOLOR::WHITE );

		strText.Format( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_DETAIL_INFO_5" ), ManagerClient.m_wBattleDuration/60 );
		m_pTextDetailInfo->AddText( strText, NS_UITEXTCOLOR::WHITE );

		strText.Format( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_DETAIL_INFO_6" ) );
		m_pTextDetailInfo->AddText( strText, NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextRewardInfo )
	{
		m_pTextRewardInfo->ClearText();
		m_pTextRewardInfo->AddText( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_REWARD_INFO" ), NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( m_pTextHistoryInfoStatic1 )
	{
		m_pTextHistoryInfoStatic1->ClearText();
		m_pTextHistoryInfoStatic1->AddText( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_HISTORY_INFO_STATIC_1" ), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextHistoryInfoStatic2 )
	{
		m_pTextHistoryInfoStatic2->ClearText();
		m_pTextHistoryInfoStatic2->AddText( ID2GAMEINTEXT( "COMPETITION_PVP_PBG_HISTORY_INFO_STATIC_2" ), NS_UITEXTCOLOR::WHITE );
	}

	PVPPBG::EVENT_SCHEDEDULE_NEXT& sNextSched = ManagerClient.m_sScheduleNext;

	if ( m_pTextHistoryInfo1 )
	{
		m_pTextHistoryInfo1->ClearText();
		if ( sNextSched.dwIndex != UINT_MAX )
		{
			CString strCombine;
			strCombine.Format ( "%02d:%02d~%02d:%02d", sNextSched.wPrevStartHour, sNextSched.wPrevStartMinute, sNextSched.wPrevEndHour, sNextSched.wPrevEndMinute );
			m_pTextHistoryInfo1->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE  );
		}
		else
		{
			m_pTextHistoryInfo1->AddText( "00:00~00:00", NS_UITEXTCOLOR::WHITE  );
		}
	}

	if ( m_pTextHistoryInfo2 )
	{
		m_pTextHistoryInfo2->ClearText();

		PVPPBG::PLAYER_DATA& splayer_data = ManagerClient.m_sLastTop[0];

		if( splayer_data.wRank != 0 &&  splayer_data.dwInfoCharID != SCHOOLWARS_PLAYER_NULL )
		{
			CString strCombine;
			strCombine.Format ( "%s %s", ID2GAMEWORD( "COMPETITION_PVP_PBG_HISTORY", 5 ), splayer_data.szInfoCharName );
			m_pTextHistoryInfo2->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE );
		}
		else
		{
			CString strCombine;
			strCombine.Format ( "%s %s", ID2GAMEWORD( "COMPETITION_PVP_PBG_HISTORY", 5 ), ID2GAMEWORD( "COMPETITION_PVP_PBG_HISTORY", 4 ) );
			m_pTextHistoryInfo2->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE );
		}

	}
}

void CPVPPartyBattlePassPage::SetCompetitionJoin( bool bEnable )
{
	if ( m_pInfoBattle )
		m_pInfoBattle->SetCompetitionJoin( bEnable );
}
