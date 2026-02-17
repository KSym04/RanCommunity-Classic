#include "StdAfx.h"
#include "./PVPClubDeathMatchScore.h"

#include "./BasicLineBoxEx.h"
#include "./BasicTextBox.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"
#include "../../Lib_Client/G-Logic/PVPClubDeathMatchClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchScore::CPVPClubDeathMatchScore ()
	: m_bCHECK_MOUSE_STATE(FALSE)
	, m_bFirstGap(FALSE)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_fUpdateTime(0.0f)
	, m_dwLastTopClub(PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL)

	, m_pLineBoxBG(NULL)
	, m_pTextFirstRank(NULL)
	, m_pTextFirstName(NULL)
	, m_pTextFirstScore(NULL)
	, m_pFirstClubImage(NULL)

	, m_pTextMyRank(NULL)
	, m_pTextMyName(NULL)
	, m_pTextMyScore(NULL)
	, m_pMyClubImage(NULL)

	, m_pTextState(NULL)
	, m_pTextTime(NULL)

{
}

CPVPClubDeathMatchScore::~CPVPClubDeathMatchScore ()
{
}

void CPVPClubDeathMatchScore::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBoxBG = new CBasicLineBoxEx;
	m_pLineBoxBG->CreateSub ( this, "BASIC_LINE_BOX_RNCDMPROGRESS_BG", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PVP_CLUB_DEATH_MATCH_SCORE_BG );
	m_pLineBoxBG->CreateBaseBoxCDMProgressBG ( "RNCDMPROGRESS_BG" );
	RegisterControl ( m_pLineBoxBG );	


	m_pTextFirstRank = new CBasicTextBox;
	m_pTextFirstRank->CreateSub ( this, "RNCDMPROGRESS_FIRST_RANK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextFirstRank->SetFont ( pFont9 );
	m_pTextFirstRank->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextFirstRank->SetText("--");
	RegisterControl ( m_pTextFirstRank );

	m_pTextFirstName = new CBasicTextBox;
	m_pTextFirstName->CreateSub ( this, "RNCDMPROGRESS_FIRST_RANK_CLUB_NAME", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextFirstName->SetFont ( pFont9 );
	m_pTextFirstName->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextFirstName->SetText("--");
	RegisterControl ( m_pTextFirstName );

	m_pTextFirstScore = new CBasicTextBox;
	m_pTextFirstScore->CreateSub ( this, "RNCDMPROGRESS_FIRST_RANK_CLUB_SCORE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextFirstScore->SetFont ( pFont9 );
	m_pTextFirstScore->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextFirstScore->SetText("--");
	RegisterControl ( m_pTextFirstScore );

	m_pFirstClubImage = new CUIControl;
	m_pFirstClubImage->CreateSub ( this, "RNCDMPROGRESS_FIRST_RANK_CLUB_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pFirstClubImage->SetUseRender ( TRUE );
	RegisterControl ( m_pFirstClubImage );


	m_pTextMyRank = new CBasicTextBox;
	m_pTextMyRank->CreateSub ( this, "RNCDMPROGRESS_MY_RANK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextMyRank->SetFont ( pFont9 );
	m_pTextMyRank->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextMyRank->SetText("--");
	RegisterControl ( m_pTextMyRank );

	m_pTextMyName = new CBasicTextBox;
	m_pTextMyName->CreateSub ( this, "RNCDMPROGRESS_MY_CLUB_NAME", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextMyName->SetFont ( pFont9 );
	m_pTextMyName->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextMyName->SetText("--");
	RegisterControl ( m_pTextMyName );

	m_pTextMyScore = new CBasicTextBox;
	m_pTextMyScore->CreateSub ( this, "RNCDMPROGRESS_MY_SCORE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextMyScore->SetFont ( pFont9 );
	m_pTextMyScore->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextMyScore->SetText("--");
	RegisterControl ( m_pTextMyScore );

	m_pMyClubImage = new CUIControl;
	m_pMyClubImage->CreateSub ( this, "RNCDMPROGRESS_MY_CLUB_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pMyClubImage->SetUseRender ( TRUE );
	RegisterControl ( m_pMyClubImage );

	m_pTextState = new CBasicTextBox;
	m_pTextState->CreateSub ( this, "RNCDMPROGRESS_STATE_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextState->SetFont ( pFont9 );
	m_pTextState->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextState->SetText("--");
	RegisterControl ( m_pTextState );

	m_pTextTime = new CBasicTextBox;
	m_pTextTime->CreateSub ( this, "RNCDMPROGRESS_STATE_TIME", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextTime->SetFont ( pFont9 );
	m_pTextTime->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextTime->SetText("--");
	RegisterControl ( m_pTextTime );

}

void CPVPClubDeathMatchScore::CheckMouseState ()
{
	const DWORD dwMsg = GetMessageEx ();
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if( UIMSG_LB_DUP & dwMsg )
		{
			AddMessageEx( UIMSG_PVP_CLUB_DEATH_MATCH_SCORE_WINDOW_LBDUP );
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

void CPVPClubDeathMatchScore::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
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

void CPVPClubDeathMatchScore::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( !m_bCHECK_MOUSE_STATE )
	{
		CheckMouseState ();
		m_bCHECK_MOUSE_STATE = true;
	}
}

void CPVPClubDeathMatchScore::UpdateInfo()
{
	if ( m_pTextFirstRank )		m_pTextFirstRank->ClearText();
	if ( m_pTextFirstName )		m_pTextFirstName->ClearText();
	if ( m_pTextFirstScore )	m_pTextFirstScore->ClearText();

	if ( m_pTextMyRank )		m_pTextMyRank->ClearText();
	if ( m_pTextMyName )		m_pTextMyName->ClearText();
	if ( m_pTextMyScore )		m_pTextMyScore->ClearText();

	const PVPClubDeathMatchClient* pClient = &PVPClubDeathMatchClient::GetInstance();
	if ( !pClient )	return;

	if ( pClient->m_sFirstClubRank.wRank != PVP_CLUB_DEATH_MATCH_RANK_NONE && pClient->m_sFirstClubRank.dwClubID != PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )
	{
		if ( m_pTextFirstRank )	
		{
			m_pTextFirstRank->SetText( ID2GAMEWORD( "RN_CMD_RANK_RANKING_1", 0 ), NS_UITEXTCOLOR::WHITE );
		}

		if ( m_pTextFirstName )	
		{
			m_pTextFirstName->SetText( pClient->m_sFirstClubRank.szClubName, NS_UITEXTCOLOR::WHITE );
		}

		if ( m_pTextFirstScore )
		{
			CString strTemp;
			strTemp.Format( "%u %s", pClient->m_sFirstClubRank.wPoints, ID2GAMEWORD( "RN_CDM_POINT_TEXT", 0) );
			m_pTextFirstScore->SetText( strTemp.GetString(), NS_UITEXTCOLOR::UICDM_SCORE_GREEN );
		}
	}

	if ( pClient->m_sMyClubRank.wRank != PVP_CLUB_DEATH_MATCH_RANK_NONE && pClient->m_sMyClubRank.dwClubID != PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )
	{
		if ( m_pTextMyRank )	
		{
			CString strTemp;
			strTemp.Format( "%u %s", pClient->m_sMyClubRank.wRank, ID2GAMEWORD( "RN_CDM_RANKING_TEXT", 0) );

			m_pTextMyRank->SetText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
		}

		if ( m_pTextMyName )	
		{
			m_pTextMyName->SetText( pClient->m_sMyClubRank.szClubName, NS_UITEXTCOLOR::WHITE );
		}

		if ( m_pTextMyScore )
		{
			CString strTemp;
			strTemp.Format( "%u %s", pClient->m_sMyClubRank.wPoints, ID2GAMEWORD( "RN_CDM_POINT_TEXT", 0) );
			m_pTextMyScore->SetText( strTemp.GetString(), NS_UITEXTCOLOR::UICDM_SCORE_GREEN );
		}
	}
	
	
	if ( m_pTextState )
	{
		m_pTextState->SetText( ID2GAMEWORD( "RN_CDM_PROEGSS_TEXT", 0), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextTime )
	{
		if ( pClient->m_fRemain > 0.0f && pClient->m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE )
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


	if( m_dwLastTopClub != pClient->m_sFirstClubRank.dwClubID && pClient->m_sFirstClubRank.dwClubID != PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )
	{
		m_dwLastTopClub = pClient->m_sFirstClubRank.dwClubID;

		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::GOLD, ID2GAMEINTEXT( "PVP_CLUB_DEATH_MATCH_MESSAGE_FIRST_POSITION_CLAIMED"), pClient->m_sFirstClubRank.szClubName  );	
	}


}

HRESULT CPVPClubDeathMatchScore::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if ( !IsVisible () ) return S_OK;

	HRESULT hr = S_OK;

	m_pFirstClubImage->SetVisibleSingle ( FALSE );
	m_pMyClubImage->SetVisibleSingle ( FALSE );
	{
		hr = CUIGroup::Render ( pd3dDevice );
		if ( FAILED ( hr ) ) return hr;
	}
	m_pFirstClubImage->SetVisibleSingle ( TRUE );
	m_pMyClubImage->SetVisibleSingle ( TRUE );

	const PVPClubDeathMatchClient* pClient = &PVPClubDeathMatchClient::GetInstance();

	if ( pClient && pClient->m_sFirstClubRank.wRank != PVP_CLUB_DEATH_MATCH_RANK_NONE && pClient->m_sFirstClubRank.dwClubID != PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )
	{
		const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nSERVER, pClient->m_sFirstClubRank.dwClubID, pClient->m_sFirstClubRank.dwCLUB_MARK_VER );

		m_pFirstClubImage->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		m_pFirstClubImage->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		m_pFirstClubImage->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		m_pFirstClubImage->SetTexturePos ( 2, sMarkData.vTex_4_RD );

		const UIRECT& rcGlobalPos = m_pFirstClubImage->GetGlobalPos ();
		m_pFirstClubImage->SetGlobalPos( UIRECT( ceil(rcGlobalPos.left), ceil(rcGlobalPos.top), ceil(rcGlobalPos.sizeX), ceil(rcGlobalPos.sizeY) ) );

		m_pFirstClubImage->SetTexture ( sMarkData.pddsTexture );
		hr = m_pFirstClubImage->Render ( pd3dDevice );
	}
	else
	{
		m_pFirstClubImage->SetVisibleSingle ( FALSE );
	}

	if ( pClient &&  pClient->m_sMyClubRank.wRank != PVP_CLUB_DEATH_MATCH_RANK_NONE && pClient->m_sMyClubRank.dwClubID != PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )
	{
		const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nSERVER, pClient->m_sMyClubRank.dwClubID, pClient->m_sMyClubRank.dwCLUB_MARK_VER );

		m_pMyClubImage->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		m_pMyClubImage->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		m_pMyClubImage->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		m_pMyClubImage->SetTexturePos ( 2, sMarkData.vTex_4_RD );

		const UIRECT& rcGlobalPos = m_pMyClubImage->GetGlobalPos ();
		m_pMyClubImage->SetGlobalPos( UIRECT( ceil(rcGlobalPos.left), ceil(rcGlobalPos.top), ceil(rcGlobalPos.sizeX), ceil(rcGlobalPos.sizeY) ) );

		m_pMyClubImage->SetTexture ( sMarkData.pddsTexture );
		hr = m_pMyClubImage->Render ( pd3dDevice );
	}
	else
	{
		m_pMyClubImage->SetVisibleSingle ( FALSE );
	}

	

	return hr;
}