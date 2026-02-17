#include "StdAfx.h"
#include "./BasicRebornDisplay.h"
#include "./BasicTextBox.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./UITextControl.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicRebornDisplay::CBasicRebornDisplay ()
	: m_pRebornText ( NULL )
	, m_dwRebornBACK(0)
{
}

CBasicRebornDisplay::~CBasicRebornDisplay ()
{
	m_dwRebornBACK = 0;
}

void CBasicRebornDisplay::CreateSubControl ()
{	
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pRebornText = new CBasicTextBox;
	pRebornText->CreateSub ( this, "BASIC_REBORN_DISPLAY_TEXT" );
    pRebornText->SetFont ( pFont8 );
	pRebornText->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	pRebornText->SetOneLineText( "0", NS_UITEXTCOLOR::GOLD /*PLUSOPTION*/ );
	RegisterControl ( pRebornText );
	m_pRebornText = pRebornText;	
}

void CBasicRebornDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	const DWORD& dwReborn = GLGaeaClient::GetInstance().GetCharacter()->m_dwReborn;

	if( m_dwRebornBACK != dwReborn )
	{
		m_dwRebornBACK = dwReborn;

		CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		CString strReborn;
		strReborn.Format( "%d", m_dwRebornBACK );	
		m_pRebornText->SetOneLineText( strReborn, NS_UITEXTCOLOR::GOLD /*PLUSOPTION*/ );
	}
}