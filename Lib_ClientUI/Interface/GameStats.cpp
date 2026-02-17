#include "StdAfx.h"
#include "./GameStats.h"
#include "./BasicTextBox.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGameStats::CGameStats()
	: m_pPingTextAgent(NULL)
	, m_pPingTextField(NULL)
	, m_pFpsText(NULL)
	, m_fPingAgent(0.0f)
	, m_fPingField(0.0f)
	, m_fFPS(0.0f)
	, m_fPingAgentBack(0.0f)
	, m_fPingFieldBack(0.0f)
	, m_fFPSBack(0.0f)
{
}

CGameStats::~CGameStats(void)
{
}

void CGameStats::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pPingTextAgent = new CBasicTextBox;
	m_pPingTextAgent->CreateSub ( this, "GAMESTATS_PING1" );
	m_pPingTextAgent->SetFont ( pFont );
	m_pPingTextAgent->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pPingTextAgent );

	m_pPingTextField = new CBasicTextBox;
	m_pPingTextField->CreateSub ( this, "GAMESTATS_PING2" );
	m_pPingTextField->SetFont ( pFont );
	m_pPingTextField->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pPingTextField );

	m_pFpsText = new CBasicTextBox;
	m_pFpsText->CreateSub ( this, "GAMESTATS_FPS" );
	m_pFpsText->SetFont ( pFont );
	m_pFpsText->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pFpsText );
}

void CGameStats::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	
	if ( m_fPingAgent != m_fPingAgentBack || m_fPingField != m_fPingFieldBack || m_fFPS != m_fFPSBack )
	{
		UpdateInfo();

		m_fPingAgentBack = m_fPingAgent;
		m_fPingFieldBack = m_fPingField;
		m_fFPSBack = m_fFPS;
	}
}

void CGameStats::UpdateInfo()
{
	if ( m_pPingTextAgent )		m_pPingTextAgent->ClearText();
	if ( m_pPingTextField )		m_pPingTextField->ClearText();
	if ( m_pFpsText )			m_pFpsText->ClearText();

	if ( RPARAM::bShowGameStats == FALSE ) return;

	if ( m_pPingTextAgent )
	{
		DWORD dwTEXTCOLOR = NS_UITEXTCOLOR::AQUAMARINE;

		if ( m_fPingAgent >= 100.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::YELLOW;
		if ( m_fPingAgent >= 200.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::ORANGE;
		if ( m_fPingAgent >= 300.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::RED;

		CString strPing("");
		strPing.Format( "%2.0f ms", m_fPingAgent );

		if ( RPARAM::bShowPingFPS == 1 )
		{
			m_pPingTextAgent->AddText( strPing.GetString(), dwTEXTCOLOR );
		}
	}

	if ( m_pPingTextField )
	{
		DWORD dwTEXTCOLOR = NS_UITEXTCOLOR::AQUAMARINE;

		if ( m_fPingField >= 100.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::YELLOW;
		if ( m_fPingField >= 200.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::ORANGE;
		if ( m_fPingField >= 300.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::RED;

		CString strPing("");
		strPing.Format( "%2.0f ms", m_fPingField );

		if ( RPARAM::bShowPingFPS == 1 )
		{
			m_pPingTextField->AddText( strPing.GetString(), dwTEXTCOLOR );
		}
	}

	if ( m_pFpsText )
	{
		float fFPS = m_fFPS;

		DWORD dwTEXTCOLOR = NS_UITEXTCOLOR::RED;

		if ( fFPS >= 30.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::YELLOW;
		if ( fFPS >= 40.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::ORANGE;
		if ( fFPS >= 50.0f )	dwTEXTCOLOR = NS_UITEXTCOLOR::AQUAMARINE;

		CString strFPS("");
		strFPS.Format( "%2.0f fps", fFPS );

		if ( RPARAM::bShowPingFPS == 1 )
		{
			m_pFpsText->AddText( strFPS.GetString(), dwTEXTCOLOR );
		}
	}
}