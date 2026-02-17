#include "StdAfx.h"
#include "./PartyFinderIcon.h"
#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPartyFinderIcon::CPartyFinderIcon ()
	: m_pButtonImage(NULL)
{
}

CPartyFinderIcon::~CPartyFinderIcon ()
{
}

void CPartyFinderIcon::CreateSubControl ()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub ( this, "PARTY_FINDER_ICON_IMAGE", UI_FLAG_DEFAULT, BUTTON_IMAGE );
	m_pButtonImage->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonImage );
}

void CPartyFinderIcon::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CPartyFinderIcon::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case BUTTON_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEINTEXT( "PARTY_FINDER_BUTTON_MESSAGE" ), NS_UITEXTCOLOR::WHITE );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{ 
				CInnerInterface::GetInstance().OpenPartyFinderWindow();
			}
		}break;
	}
}
