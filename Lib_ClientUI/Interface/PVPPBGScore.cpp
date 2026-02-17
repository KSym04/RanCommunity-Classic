#include "StdAfx.h"
#include "./PVPPBGScore.h"

#include "./BasicLineBoxEx.h"
#include "./BasicTextBox.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"
#include "../../Lib_Client/G-Logic/PVPPartyBattleGroundsClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPPBGScore::CPVPPBGScore ()
	: m_bCHECK_MOUSE_STATE(FALSE)
	, m_bFirstGap(FALSE)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_fUpdateTime(0.0f)

	, m_pLineBoxBG(NULL)
	, m_pTextName(NULL)
	, m_pTextKills(NULL)
	, m_pTextDeaths(NULL)

	, m_pTextTime(NULL)

{
}

CPVPPBGScore::~CPVPPBGScore ()
{
}

void CPVPPBGScore::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBoxBG = new CBasicLineBoxEx;
	m_pLineBoxBG->CreateSub ( this, "BASIC_LINE_BOX_RNCDMPROGRESS_BG", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PVP_PBG_SCORE_BG );
	m_pLineBoxBG->CreateBaseBoxCDMProgressBG ( "COMPETITION_PBG_SCORE_BG" );
	RegisterControl ( m_pLineBoxBG );	

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "COMPETITION_PBG_SCORE_NAME", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextName->SetFont ( pFont9 );
	m_pTextName->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextName->SetText("--");
	RegisterControl ( m_pTextName );

	m_pTextKills = new CBasicTextBox;
	m_pTextKills->CreateSub ( this, "COMPETITION_PBG_SCORE_KILLS", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextKills->SetFont ( pFont9 );
	m_pTextKills->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextKills->SetText("--");
	RegisterControl ( m_pTextKills );

	m_pTextDeaths = new CBasicTextBox;
	m_pTextDeaths->CreateSub ( this, "COMPETITION_PBG_SCORE_DEATHS", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextDeaths->SetFont ( pFont9 );
	m_pTextDeaths->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextDeaths->SetText("--");
	RegisterControl ( m_pTextDeaths );

	m_pTextTime = new CBasicTextBox;
	m_pTextTime->CreateSub ( this, "COMPETITION_PBG_SCORE_STATE_TIME", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextTime->SetFont ( pFont9 );
	m_pTextTime->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextTime->SetText("--");
	RegisterControl ( m_pTextTime );

}

void CPVPPBGScore::CheckMouseState ()
{
	const DWORD dwMsg = GetMessageEx ();
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if( UIMSG_LB_DUP & dwMsg )
		{
			AddMessageEx( UIMSG_PVP_PBG_SCORE_WINDOW_LBDUP );
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

void CPVPPBGScore::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	m_bCHECK_MOUSE_STATE = false;
	m_nPosX = x;
	m_nPosY = y;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( IsExclusiveSelfControl() )
	{
		SetGlobalPos ( D3DXVECTOR2 ( x - m_vGap.x, y - m_vGap.y ) );
	}

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f )
	{
		m_fUpdateTime = 0.0f;

		UpdateInfo();
	}
}

void CPVPPBGScore::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( !m_bCHECK_MOUSE_STATE )
	{
		CheckMouseState ();
		m_bCHECK_MOUSE_STATE = true;
	}
}

void CPVPPBGScore::UpdateInfo()
{
	if ( m_pTextName )		
		m_pTextName->ClearText();

	if ( m_pTextKills )	
		m_pTextKills->ClearText();

	if ( m_pTextDeaths )	
		m_pTextDeaths->ClearText();


	const PVPPBG::ManagerClient* pClient = &PVPPBG::ManagerClient::GetInstance();
	if ( !pClient )
		return;

	const PVPPBG::PLAYER_DATA& PlayerData = pClient->m_PlayerData;
	if ( PlayerData.dwInfoCharID == PVP_PBG_CHAR_ID_NULL )
		return;

	if ( m_pTextName )	
	{
		m_pTextName->SetText( PlayerData.szInfoCharName, NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextKills )
	{
		CString strTemp;
		strTemp.Format( "K:%u", PlayerData.wScoreKill );
		m_pTextKills->SetText( strTemp.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );
	}

	if ( m_pTextDeaths )
	{
		CString strTemp;
		strTemp.Format( "K:%u", PlayerData.wScoreDeath );
		m_pTextDeaths->SetText( strTemp.GetString(), NS_UITEXTCOLOR::RED );
	}

	if ( m_pTextTime )
	{
		if ( pClient->m_fRemain > 0.0f && pClient->m_emState == PVPPBG::EVENT_STATE_BATTLE )
		{
			DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
			if ( pClient->m_fRemain < 300.0f )
			{
				dwTextColor = NS_UITEXTCOLOR::RED;
			}
			CString strCombine;
			strCombine.Format ( "%02d:%02d", (DWORD)pClient->m_fRemain/60, (DWORD)pClient->m_fRemain%60 );
			m_pTextTime->SetText( strCombine.GetString(), dwTextColor );
		}
		else
		{
			m_pTextTime->SetText( "--:--", NS_UITEXTCOLOR::WHITE  );
		}
	}


}
