#include "StdAfx.h"
#include "./InfoWindow.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "./InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CInfoWindow::CInfoWindow()
	: m_pTextInfo(NULL)
	, m_pListScrollBar(NULL)
	, m_CallerID(NO_ID)
{
}

CInfoWindow::~CInfoWindow()
{
}

void CInfoWindow::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_INFO_WINDOW_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_INFO_WINDOW_LINE" );
	RegisterControl ( pLineBox );

	m_pTextInfo = new CBasicTextBox;
	m_pTextInfo->CreateSub ( this, "RAN_INFO_WINDOW_TEXT" );
	m_pTextInfo->SetFont ( pFont9 );
	m_pTextInfo->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pTextInfo->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextInfo );

	int nTotalLine = m_pTextInfo->GetVisibleLine ();

	m_pListScrollBar = new CBasicScrollBarEx;
	m_pListScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, INFO_WINDOW_SCROLLBAR );
	m_pListScrollBar->CreateBaseScrollBar ( "RAN_INFO_WINDOW_SCROLL" );
	m_pListScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( m_pListScrollBar );
}

void CInfoWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();
	const int nTotalLine = m_pTextInfo->GetTotalLine ();
	const int nLinePerOneView = m_pTextInfo->GetVisibleLine ();
	pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
	if ( nLinePerOneView < nTotalLine )
	{
		const int nMovableLine = nTotalLine - nLinePerOneView;
		float fPercent = pThumbFrame->GetPercent ();
		int nPos = (int)floor(fPercent * nMovableLine);
		m_pTextInfo->SetCurLine ( nPos );			
	}

	if ( m_CallerID != NO_ID )
	{
		if ( !CInnerInterface::GetInstance().IsVisibleGroup( m_CallerID ) )
		{
			CInnerInterface::GetInstance().HideGroup( GetWndID() );
		}
	}
}

void CInfoWindow::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;
	};
}

void CInfoWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		if ( m_pTextInfo )
		{
			m_pTextInfo->ClearText();
		}
	}
}
