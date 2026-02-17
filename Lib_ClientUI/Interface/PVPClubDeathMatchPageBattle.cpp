#include "stdafx.h"
#include "./PVPClubDeathMatchPageBattle.h"


#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./BasicTextBox.h"
#include "./BasicLineBox.h"
#include "./BasicTextButton.h"

#include "../../Lib_Client/G-Logic/PVPClubDeathMatchClient.h"
#include "../../Lib_Client/DxGlobalStage.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchPageBattle::CPVPClubDeathMatchPageBattle ()
	: m_fUpdateTime(0.0f)
	, m_pButtonRegister(NULL)
	, m_pTextBattleTitleStatic(NULL)
	, m_pTextBattleTimeStartStatic(NULL)
	, m_pTextBattleTimeEndStatic(NULL)
	, m_pTextBattleProgressStatic(NULL)
	, m_pTextBattleStatusStatic(NULL)
	, m_pTextClubMembersStatic(NULL)
	, m_pTextBattleTimeStartTimer(NULL)
	, m_pTextBattleTimeEndTimer(NULL)
	, m_pTextBattleProgress(NULL)
	, m_pTextBattleStatus(NULL)
	, m_pTextClubMembers(NULL)
{
}

CPVPClubDeathMatchPageBattle::~CPVPClubDeathMatchPageBattle ()
{
}

void CPVPClubDeathMatchPageBattle::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pTextBattleTimeStartStatic = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_STARTTIME_STATICTEXT", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextBattleTimeEndStatic = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_ENDTIME_STATICTEXT", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextBattleProgressStatic = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_PROGRESSTIME_STATICTEXT", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextBattleStatusStatic = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_STATUS_STATICTEXT", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextClubMembersStatic = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_CLUB_MEMBERS_STATICTEXT", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );

	m_pTextBattleTimeStartTimer = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_STARTTIME_TEXT", pFont9, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextBattleTimeEndTimer = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_ENDTIME_TEXT", pFont9, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextBattleProgress = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_PROGRESSTIME_TEXT", pFont9, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextBattleStatus = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_STATUS_TEXT", pFont9, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextClubMembers = CreateUITextBox( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_CLUB_MEMBERS_TEXT", pFont9, TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );

	m_pTextBattleTimeStartStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_STATIC", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextBattleTimeEndStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_STATIC", 1 ), NS_UITEXTCOLOR::WHITE );
	m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_STATIC", 2 ), NS_UITEXTCOLOR::WHITE );
	m_pTextBattleStatusStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_STATIC", 3 ), NS_UITEXTCOLOR::WHITE );
	m_pTextClubMembersStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_STATIC", 4 ), NS_UITEXTCOLOR::WHITE );

	m_pTextBattleTimeStartTimer->AddText( "1", NS_UITEXTCOLOR::WHITE );
	m_pTextBattleTimeEndTimer->AddText( "2", NS_UITEXTCOLOR::WHITE );
	m_pTextBattleProgress->AddText( "3", NS_UITEXTCOLOR::WHITE );
	m_pTextBattleStatus->AddText( "4", NS_UITEXTCOLOR::WHITE );
	m_pTextClubMembers->AddText( "5", NS_UITEXTCOLOR::WHITE );

	m_pButtonRegister = new CBasicTextButton;
	m_pButtonRegister->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, BUTTON_REGISTER );
	m_pButtonRegister->CreateBaseButton ( "COMPETITION_CLUB_DEATH_MATCH_PAGE_BATTLE_BUTTON", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 0 ) );
	RegisterControl ( m_pButtonRegister );
}

CBasicTextBox* CPVPClubDeathMatchPageBattle::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

void CPVPClubDeathMatchPageBattle::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case BUTTON_REGISTER:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				PVPClubDeathMatchClient::GetInstance().DoRegister();
			}
		}break;
	};
}

void CPVPClubDeathMatchPageBattle::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f )
	{
		ShowInfo();
		m_fUpdateTime = 0.0f;
	}
}

void CPVPClubDeathMatchPageBattle::ShowInfo()
{
	const PVPClubDeathMatchClient* pClient = &PVPClubDeathMatchClient::GetInstance();
	if ( !pClient )	return;

	if ( m_pTextBattleTimeStartTimer )
	{
		m_pTextBattleTimeStartTimer->ClearText();
		if ( pClient->m_sScheduleNext.dwIndex != PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE )
		{
			CString strCombine;
			strCombine.Format ( "%02d:%02d", pClient->m_sScheduleNext.wBattleStartHour, pClient->m_sScheduleNext.wBattleStartMinute );
			m_pTextBattleTimeStartTimer->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE  );
		}
		else
		{
			m_pTextBattleTimeStartTimer->AddText( "00:00", NS_UITEXTCOLOR::WHITE  );
		}
	}

	if ( m_pTextBattleTimeEndTimer )
	{
		m_pTextBattleTimeEndTimer->ClearText();
		if ( pClient->m_sScheduleNext.dwIndex != PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE )
		{
			CString strCombine;
			strCombine.Format ( "%02d:%02d", pClient->m_sScheduleNext.wBattleEndHour, pClient->m_sScheduleNext.wBattleEndMinute );
			m_pTextBattleTimeEndTimer->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE  );
		}
		else
		{
			m_pTextBattleTimeEndTimer->AddText( "00:00", NS_UITEXTCOLOR::WHITE  );
		}
	}

	if ( m_pTextBattleProgressStatic )
	{
		m_pTextBattleProgressStatic->ClearText();
		if ( pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER )
			m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_PROGRESS_TEXT", 0 ), NS_UITEXTCOLOR::ORANGE );
		else if ( pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE )
			m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_PROGRESS_TEXT", 1 ), NS_UITEXTCOLOR::ORANGE );
		else if ( pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD)
			m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_PROGRESS_TEXT", 2 ), NS_UITEXTCOLOR::ORANGE );
		else
			m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_STATIC", 2 ), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextBattleProgress )
	{
		m_pTextBattleProgress->ClearText();
		if ( pClient->m_fRemain > 0.0f && ( pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER || pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE || pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD ) )
		{
			DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
			if ( pClient->m_fRemain < 300.0f ){
				dwTextColor = NS_UITEXTCOLOR::RED;
			}
			CString strCombine;
			strCombine.Format ( "%02d:%02d", (DWORD)pClient->m_fRemain/60, (DWORD)pClient->m_fRemain%60 );
			m_pTextBattleProgress->AddText( strCombine.GetString(), dwTextColor );
		}
		else
		{
			m_pTextBattleProgress->AddText( "00:00", NS_UITEXTCOLOR::WHITE  );
		}
	}


	if ( m_pTextBattleStatus )
	{
		m_pTextBattleStatus->ClearText();

		if ( DxGlobalStage::GetInstance().GetChannel() != 0 )
		{
			m_pTextBattleStatus->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 2 ), NS_UITEXTCOLOR::RED );
		}
		else
		{
			if ( pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER )
			{
				if ( pClient->m_bRegistered )
					m_pTextBattleStatus->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 3 ), NS_UITEXTCOLOR::ORANGE );
				else
					m_pTextBattleStatus->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 4 ), NS_UITEXTCOLOR::GREENYELLOW );
			}
			else
			{
				if ( pClient->m_bRegistered )
					m_pTextBattleStatus->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 5 ), NS_UITEXTCOLOR::ORANGE );
				else
					m_pTextBattleStatus->AddText( ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 1), NS_UITEXTCOLOR::RED );
			}
		}
	}

	if ( m_pTextClubMembers )
	{
		m_pTextClubMembers->ClearText();
		CString strCombine;
		strCombine.Format ( "%u", pClient->m_wClubParticipants );
		m_pTextClubMembers->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pButtonRegister )
	{
		if ( pClient->m_bRegistered && pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER )
			m_pButtonRegister->SetOneLineText(ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 6 ));
		else if ( pClient->m_bRegistered && pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE )
			m_pButtonRegister->SetOneLineText(ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 5 ));
		else
			m_pButtonRegister->SetOneLineText(ID2GAMEWORD( "COMPETITION_PVP_CLUB_DEATH_MATCH_BATTLE_TEXT", 0 ));
	}
}

void CPVPClubDeathMatchPageBattle::SetCompetitionJoin( bool bEnable )
{
	if ( m_pButtonRegister )
		m_pButtonRegister->SetVisibleSingle( bEnable );
}