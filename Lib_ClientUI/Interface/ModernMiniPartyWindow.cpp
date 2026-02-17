#include "StdAfx.h"
#include "./ModernMiniPartyWindow.h"
#include "./ModernMiniPartyExpeditionPage.h"

#include "./BasicLineBox.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernMiniPartyWindow::CModernMiniPartyWindow () :
	m_pBackGround( NULL )
{
	
}

CModernMiniPartyWindow::~CModernMiniPartyWindow ()
{
}

void CModernMiniPartyWindow::CreateSubControl ()
{	
	m_pBackGround = new CUIControl;
	m_pBackGround->CreateSub ( this, "MODERN_MINI_PARTY_WINDOW_REGION", UI_FLAG_DEFAULT );	
	m_pBackGround->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pBackGround );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_MINI_PARTY_INFO_GROUP_REGION" );
	RegisterControl ( pBasicLineBox );

	CModernMiniPartyExpeditionPage* pPartyPage = new CModernMiniPartyExpeditionPage;
	pPartyPage->CreateSub ( this, "MODERN_MINI_PARTY_INFO_GROUP_REGION" );
	pPartyPage->CreateSubControl ();
	RegisterControl ( pPartyPage );
	m_pPartyPage = pPartyPage;
}

void CModernMiniPartyWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernMiniPartyWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( MINIPARTY_WINDOW );
			}
		}
		break;
	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
				CInnerInterface::GetInstance().ShowGroupBottom ( MINIPARTY_OPEN );
			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
}