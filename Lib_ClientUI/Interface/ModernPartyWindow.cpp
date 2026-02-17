#include "StdAfx.h"
#include "./ModernPartyWindow.h"

#include "./PartySlot.h"
#include "./MiniMap.h"
#include "./BasicTextButton.h"
#include "./BasicTextBox.h"
#include "./MultiModeButton.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLPartyClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartyWindow::CModernPartyWindow () :
	m_pPARTYTAB ( NULL ),
	m_pBackGround ( NULL ),
	m_pHelp( NULL )
{	
}

CModernPartyWindow::~CModernPartyWindow ()
{
}

CMultiModeButton* CModernPartyWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
{
	CMultiModeButton* pButton = new CMultiModeButton;
	pButton->CreateSub ( this, "TAB_BUTTON_WHITE", UI_FLAG_XSIZE, ControlID );
	pButton->CreateImageBaseMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT", "TAB_BUTTON_WHITE_IMAGE_CENTER", "TAB_BUTTON_WHITE_IMAGE_RIGHT" );
	pButton->CreateImageFlipMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT_F", "TAB_BUTTON_WHITE_IMAGE_CENTER_F", "TAB_BUTTON_WHITE_IMAGE_RIGHT_F" );
	pButton->CreateTextBox( "TAB_BUTTON_WHITE_IMAGE_TEXTBOX", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	pButton->AlignToControl( szButtonBox );
	pButton->SetOneLineText( szButtonText, NS_UITEXTCOLOR::WHITE );
	pButton->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pButton );
	return pButton;
}

void CModernPartyWindow::CreateSubControl ()
{	
	m_pBackGround = new CUIControl;
	m_pBackGround->CreateSub ( this, "MODERN_PARTY_WINDOW_RENEWAL_REGION", UI_FLAG_DEFAULT );	
	m_pBackGround->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pBackGround );

	m_pPARTYTAB = CreateTabButton ( "MODERN_PARTY_RENEWAL_INFO_TAP", (char*)ID2GAMEWORD("MODERN_PARTYWINDOW_TEXTS", 0 ), PARTY_TAB );
	m_pPARTYTAB->DoImageFlip(TRUE);

	m_pPageParty = new CModernPartyWindowPageParty;
	m_pPageParty->CreateSub ( this, "MODERN_PARTY_RENEWAL_INFO_TAP_WINDOW", UI_FLAG_DEFAULT, PAGE_PARTY );	
	m_pPageParty->CreateSubControl();
	m_pPageParty->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pPageParty );

	m_pHelp = new CBasicButton;
	m_pHelp->CreateSub ( this, "MODERN_PARTY_WINDOW_HELP_BUTTON", UI_FLAG_DEFAULT, HELP_BUTTON );
	m_pHelp->CreateFlip ( "MODERN_PARTY_WINDOW_HELP_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pHelp->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pHelp );
}

void CModernPartyWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernPartyWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( PARTY_WINDOW );
			}
		}
		break;

	case HELP_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoWindowOpen( GetWndID() );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("PARTY_RENEWAL_HELP_1" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("PARTY_RENEWAL_HELP_2" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("PARTY_RENEWAL_HELP_3" ), NS_UITEXTCOLOR::DARKLBUE );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("PARTY_RENEWAL_HELP_4" ), NS_UITEXTCOLOR::DARKLBUE );
			}
		}break;
	}
}


