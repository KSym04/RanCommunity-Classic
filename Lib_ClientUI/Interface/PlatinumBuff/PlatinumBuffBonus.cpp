#include "StdAfx.h"
#include "PlatinumBuffBonus.h"
#include "../InnerInterface.h"
#include "../GameTextControl.h"
#include "../UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPlatinumBuffBonus::CPlatinumBuffBonus ()
	: m_pButtonImage(NULL)
{
}

CPlatinumBuffBonus::~CPlatinumBuffBonus ()
{
}

void CPlatinumBuffBonus::CreateSubControl ()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub ( this, "PLATINUM_MARK_NONE", UI_FLAG_DEFAULT, PLATINUM_MARK_NONE );
	m_pButtonImage->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonImage );
}

void CPlatinumBuffBonus::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CPlatinumBuffBonus::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
		case PLATINUM_MARK_NONE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( "Get Your platinum buff here." , NS_UITEXTCOLOR::PALEGREEN  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) ){ 
				CInnerInterface::GetInstance().ShowGroupFocus( PLATINUM_WINDOW );
			}
		}break;
	}
}
