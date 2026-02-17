#include "StdAfx.h"
#include "PVPNextBattleAndWinner.h"

#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "GameTextControl.h"
#include "UITextControl.h"

#include "../../Lib_Client/G-Logic/GLPVPTyrannyClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPNextBattleAndWinner::CPVPNextBattleAndWinner ()
	: m_bCHECK_MOUSE_STATE(FALSE)
	, m_bFirstGap(FALSE)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_fUpdateTime(0.0f)

	, m_pTextStatus(NULL)
	, m_pTextTimer(NULL)
	, m_pTextTimerNextBattle(NULL)
	, m_pTextStatusNextBattle(NULL)
	, m_pTextRewardProgress(NULL)
	, m_pTextRewardProgressTimer(NULL)
	, m_pTextRegistrationProgress(NULL)
	, m_pTextRegistrationProgressTimer(NULL)
{
	for( int i=0; i<TOWER_STATUS_TYPES; ++i )
	{
		m_pStatusWinner[i] = NULL;
	}
}

CPVPNextBattleAndWinner::~CPVPNextBattleAndWinner ()
{
}

void CPVPNextBattleAndWinner::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "TYRANNY_TOWER_CAPTURE_BG1_MARKET", UI_FLAG_DEFAULT, TOWER_STATUS_BOX1 );
	RegisterControl ( pControl );

	m_pTextStatus = new CBasicTextBox;
	m_pTextStatus->CreateSub ( this, "TYRANNY_TOWER_NEXT_BATTLE_STATUS_TEXT" );
	m_pTextStatus->SetFont ( pFont );
	m_pTextStatus->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextStatus->AddText( ID2GAMEINTEXT("TYRANNY_TOWER_STATUS_COMPLETE_MARKET"), NS_UITEXTCOLOR::BRIGHTGREEN );
	RegisterControl ( m_pTextStatus );

	m_pTextTimer = new CBasicTextBox;
	m_pTextTimer->CreateSub ( this, "TYRANNY_TOWER_CAPTURE_TIME_TEXT_MARKET" );
	m_pTextTimer->SetFont ( pFont );
	m_pTextTimer->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextTimer->AddText( "FF:FF", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextTimer );

	m_pTextStatusNextBattle = new CBasicTextBox;
	m_pTextStatusNextBattle->CreateSub ( this, "TYRANNY_TOWER_NEXT_BATTLE_STATUS_TEXT_MARKET" );
	m_pTextStatusNextBattle->SetFont ( pFont );
	m_pTextStatusNextBattle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextStatusNextBattle->AddText( ID2GAMEINTEXT("TYRANNY_TOWER_STATUS_BATTLE_MARKET"), NS_UITEXTCOLOR::GREENYELLOW );
	RegisterControl ( m_pTextStatusNextBattle );

	m_pTextTimerNextBattle = new CBasicTextBox;
	m_pTextTimerNextBattle->CreateSub ( this, "TYRANNY_TOWER_NEXT_BATTLE_CAPTURE_TIME_TEXT" );
	m_pTextTimerNextBattle->SetFont ( pFont );
	m_pTextTimerNextBattle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextTimerNextBattle->AddText( "FF:FF", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextTimerNextBattle );
	
	// Reward Timer
	m_pTextRewardProgressTimer = new CBasicTextBox;
	m_pTextRewardProgressTimer->CreateSub ( this, "TYRANNY_TOWER_REWARD_PROGRESS_TIME_TEXT_MARKET" );
	m_pTextRewardProgressTimer->SetFont ( pFont );
	m_pTextRewardProgressTimer->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRewardProgressTimer->AddText( "FF:FF", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextRewardProgressTimer );
	
	// Reward String
	m_pTextRewardProgress = new CBasicTextBox;
	m_pTextRewardProgress->CreateSub ( this, "TYRANNY_TOWER_STATUS_REWARD_PROGRESS_TEXT_MARKET" );
	m_pTextRewardProgress->SetFont ( pFont );
	m_pTextRewardProgress->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRewardProgress->AddText( ID2GAMEINTEXT("TYRANNY_TOWER_STATUS_REWARD_PROGRESS_MARKET"), NS_UITEXTCOLOR::GREENYELLOW );
	RegisterControl ( m_pTextRewardProgress );

	// Registration Timer
	m_pTextRegistrationProgressTimer = new CBasicTextBox;
	m_pTextRegistrationProgressTimer->CreateSub ( this, "TYRANNY_TOWER_REGISTRATION_PROGRESS_TIME_TEXT_MARKET" );
	m_pTextRegistrationProgressTimer->SetFont ( pFont );
	m_pTextRegistrationProgressTimer->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRegistrationProgressTimer->AddText( "FF:FF", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextRegistrationProgressTimer );
	
	// Registration String
	m_pTextRegistrationProgress = new CBasicTextBox;
	m_pTextRegistrationProgress->CreateSub ( this, "TYRANNY_TOWER_STATUS_REGISTRATION_PROGRESS_TEXT_MARKET" );
	m_pTextRegistrationProgress->SetFont ( pFont );
	m_pTextRegistrationProgress->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRegistrationProgress->AddText( ID2GAMEINTEXT("TYRANNY_REGISTRATION_TOWER_STATUS_BATTLE_MARKET"), NS_UITEXTCOLOR::GREENYELLOW );
	RegisterControl ( m_pTextRegistrationProgress );

	std::string strstatuswinner[TOWER_STATUS_TYPES] = 
	{
		"TYRANNY_TOWER_WINNER_IMAGE_SG_MARKET",
		"TYRANNY_TOWER_WINNER_IMAGE_MP_MARKET",
		"TYRANNY_TOWER_WINNER_IMAGE_PHX_MARKET",
		"TYRANNY_TOWER_WINNER_IMAGE_NONE_MARKET",
	};

	for( int i=0; i<TOWER_STATUS_TYPES; ++i )
	{
		m_pStatusWinner[i] = new CUIControl;
		m_pStatusWinner[i]->CreateSub ( this, strstatuswinner[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pStatusWinner[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pStatusWinner[i] );
	}
}

void CPVPNextBattleAndWinner::CheckMouseState ()
{
	const DWORD dwMsg = GetMessageEx ();
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if( UIMSG_LB_DUP & dwMsg )
		{
			AddMessageEx( UIMSG_PVPTYRANNY_NEXT_BATTLE_WINNER_LBDUP );
			SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
			return ;
		}

		if ( dwMsg & UIMSG_LB_DOWN )
		{
			SetExclusiveControl();	

			if ( !m_bFirstGap )
			{
				UIRECT rcPos = GetGlobalPos ();
				m_vGap.x = m_nPosX - rcPos.left;
				m_vGap.y = m_nPosY - rcPos.top;
				m_bFirstGap = TRUE;

				SetDiffuse ( D3DCOLOR_ARGB ( 0xB4, 0xFF, 0xFF, 0xFF ) );
			}
		}
		else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
		{
			ResetExclusiveControl();
			m_bFirstGap = FALSE;				

			SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
		}
	}
	else if ( CHECK_LB_UP_LIKE ( dwMsg ) )		
	{								
		ResetExclusiveControl();
		m_bFirstGap = FALSE;					

		SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
	}
}

void CPVPNextBattleAndWinner::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	m_bCHECK_MOUSE_STATE = false;
	m_nPosX = x;
	m_nPosY = y;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( IsExclusiveSelfControl() ){
		SetGlobalPos ( D3DXVECTOR2 ( x - m_vGap.x, y - m_vGap.y ) );
	}

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f ){
		m_fUpdateTime = 0.0f;

		UpdateInfo();
	}
}

void CPVPNextBattleAndWinner::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( !m_bCHECK_MOUSE_STATE )
	{
		CheckMouseState ();
		m_bCHECK_MOUSE_STATE = true;
	}
}

void CPVPNextBattleAndWinner::UpdateInfo()
{
	TYRANNY_STATE emstate = GLPVPTyrannyClient::GetInstance().m_emState;
	TYRANNY_SCHED_NEXT& sNextSched = GLPVPTyrannyClient::GetInstance().m_sScheduleNext;
	float fbattle_remain_time = GLPVPTyrannyClient::GetInstance().m_fRemain;
	WORD wwinner = GLPVPTyrannyClient::GetInstance().m_wLastWinner;
	float fRamainTimeToNextBattle = GLPVPTyrannyClient::GetInstance().m_fRemainTimeToNextBattle;
	WORD wowner_tower0 = GLPVPTyrannyClient::GetInstance().m_wTowerOwner[TYRANNY_TOWER_0];
	WORD wowner_tower1 = GLPVPTyrannyClient::GetInstance().m_wTowerOwner[TYRANNY_TOWER_1];
	WORD wowner_tower2 = GLPVPTyrannyClient::GetInstance().m_wTowerOwner[TYRANNY_TOWER_2];

	/////////////////////////////////////////////////////
	if ( m_pTextStatus )
		m_pTextStatus->ClearText();

	if ( m_pTextTimer )
		m_pTextTimer->ClearText();
	/////////////////////////////////////////////////////
	// Reward Time
	//if ( m_pTextRewardProgress )
	//	m_pTextRewardProgress->ClearText();

	if ( m_pTextRewardProgressTimer )
		m_pTextRewardProgressTimer->ClearText();
	/////////////////////////////////////////////////////
	// Registration Time
	//if ( m_pTextRegistrationProgress )
	//	m_pTextRegistrationProgress->ClearText();

	if ( m_pTextRegistrationProgressTimer )
		m_pTextRegistrationProgressTimer->ClearText();
	/////////////////////////////////////////////////////
	// Next Battle
	//if ( m_pTextStatusNextBattle )
	//	m_pTextStatusNextBattle->ClearText();

	if ( m_pTextTimerNextBattle )
		m_pTextTimerNextBattle->ClearText();
	/////////////////////////////////////////////////////

	for( int i=0; i<TOWER_STATUS_TYPES; ++i )
	{

		if ( m_pStatusWinner[i] )
			m_pStatusWinner[i]->SetVisibleSingle( FALSE );
	}
	
	if ( m_pTextStatus )
	{
		if ( emstate == TYRANNY_STATE_BATTLE )
			m_pTextStatus->AddText( ID2GAMEINTEXT("UNDERWAY_TYRANNY_TOWER_STATUS_BATTLE_MARKET" ), NS_UITEXTCOLOR::GREENYELLOW );
		else
			m_pTextStatus->AddText( ID2GAMEINTEXT("TYRANNY_TOWER_STATUS_COMPLETE_MARKET" ), NS_UITEXTCOLOR::WHITE );
	}
	
	// Reward Time
	if ( m_pTextRewardProgress )
	{
		if ( emstate == TYRANNY_STATE_REWARD )
			m_pTextRewardProgress->AddText( ID2GAMEWORD( "COMPETITION_CTF_BATTLE_PROGRESS_TEXT", 2 ), NS_UITEXTCOLOR::GREENYELLOW );
	}
	
	// Registration Progress
	if ( m_pTextRegistrationProgress )
	{
		if ( emstate == TYRANNY_STATE_REGISTER )
			m_pTextRegistrationProgress->AddText( ID2GAMEWORD( "COMPETITION_CTF_BATTLE_PROGRESS_TEXT", 0 ), NS_UITEXTCOLOR::GREENYELLOW );
	}

	// Next Battle
	if ( m_pTextStatusNextBattle )
	{
		m_pTextStatusNextBattle->AddText( ID2GAMEINTEXT("TYRANNY_TOWER_STATUS_BATTLE_MARKET" ), NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( emstate == TYRANNY_STATE_REGISTER )
	{
		if ( m_pTextRegistrationProgressTimer )
		{
			m_pTextTimerNextBattle->ClearText(); // To hide Next Tyranny Time
			m_pTextStatusNextBattle->ClearText(); // To hide Next Tyranny String
			m_pTextRewardProgressTimer->ClearText(); // To hide Progress Reward Time
			m_pTextRewardProgress->ClearText();	// To hide Progress Reward String

			if ( fbattle_remain_time > 0.0f && ( emstate == TYRANNY_STATE_REGISTER ) )
			{
				DWORD dwTextColor = NS_UITEXTCOLOR::GREENYELLOW;
				if ( fbattle_remain_time < 300.0f ){
					dwTextColor = NS_UITEXTCOLOR::RED;
				}
				CString strCombine;
				strCombine.Format ( "%02d:%02d", (DWORD)fbattle_remain_time/60, (DWORD)fbattle_remain_time%60 );
				m_pTextRegistrationProgressTimer->AddText( strCombine.GetString(), dwTextColor );
			}
			else
			{
				m_pTextRegistrationProgressTimer->AddText( "00:00", NS_UITEXTCOLOR::WHITE  );
			}
		}
	}
	else if ( emstate == TYRANNY_STATE_REWARD )
	{
		if ( m_pTextRewardProgressTimer )
		{
			m_pTextTimerNextBattle->ClearText(); // To hide Next Tyranny Time
			m_pTextStatusNextBattle->ClearText(); // To hide Next Tyranny String
			m_pTextRegistrationProgress->ClearText(); // To hide Registration String
			m_pTextRegistrationProgressTimer->ClearText(); // To hide Registration Time

			if ( fbattle_remain_time > 0.0f && ( emstate == TYRANNY_STATE_REWARD ) )
			{
				DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
				if ( fbattle_remain_time < 300.0f ){
					dwTextColor = NS_UITEXTCOLOR::RED;
				}
				CString strCombine;
				strCombine.Format ( "%02d:%02d", (DWORD)fbattle_remain_time/60, (DWORD)fbattle_remain_time%60 );
				m_pTextRewardProgressTimer->AddText( strCombine.GetString(), dwTextColor );
			}
			else
			{
				m_pTextRewardProgressTimer->AddText( "00:00", NS_UITEXTCOLOR::WHITE  );
			}
		}
	}
	else
	{
		if ( m_pTextTimerNextBattle )
		{
			m_pTextRewardProgressTimer->ClearText(); // To hide Progress Reward Time
			m_pTextRewardProgress->ClearText();	// To hide Progress Reward String
			m_pTextRegistrationProgress->ClearText(); // To hide Registration String
			m_pTextRegistrationProgressTimer->ClearText(); // To hide Registration Time

			if ( sNextSched.dwIndex != UINT_MAX && fRamainTimeToNextBattle > 0.0f )
			{
				CString strCombine;
				strCombine.Format (  "%02d:%02d:%02d", (DWORD)fRamainTimeToNextBattle/3600, ((DWORD)fRamainTimeToNextBattle/60) % 60, (DWORD)fRamainTimeToNextBattle%60);
				m_pTextTimerNextBattle->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE  );
			}
			else
			{
				CString strCombine;
				strCombine.Format (  "%02d:%02d:%02d", (DWORD)fRamainTimeToNextBattle/3600, ((DWORD)fRamainTimeToNextBattle/60) % 60, (DWORD)fRamainTimeToNextBattle%60);
				//strCombine.Format (  "%02d:%02d", sNextSched.wBattleStartHour, sNextSched.wBattleStartMinute);
				m_pTextTimerNextBattle->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE  );
			}
		}
	}


	if ( emstate == TYRANNY_STATE_BATTLE )
	{
		m_pTextTimerNextBattle->ClearText(); // To hide Next Tyranny Time
		m_pTextStatusNextBattle->ClearText(); // To hide Next Tyranny String
		if ( m_pTextTimer )
		{
			if ( fbattle_remain_time > 0.0f )
			{
				DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
				if ( fbattle_remain_time < 300.0f ){
					dwTextColor = NS_UITEXTCOLOR::RED;
				}
				CString strCombine;
				strCombine.Format ( "%02d:%02d", (DWORD)fbattle_remain_time/60, (DWORD)fbattle_remain_time%60 );
				m_pTextTimer->AddText( strCombine.GetString(), dwTextColor );
			}
		}
	}
	else
	{
		if ( wwinner < TOWER_STATUS_TYPES && m_pStatusWinner[wwinner] )
			m_pStatusWinner[wwinner]->SetVisibleSingle( TRUE );
	}
}