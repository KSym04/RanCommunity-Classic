/**MMR Rank Stats, Mhundz 01/02/2025**/
#include "StdAfx.h"
#include "MMRWindowButton.h"
#include "../InnerInterface.h"
#include "../GameTextControl.h"
#include "../UITextControl.h"
#include "../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMMRWindowButton::CMMRWindowButton ()
	: m_pButtonImage(NULL)
{
}

CMMRWindowButton::~CMMRWindowButton ()
{
}

void CMMRWindowButton::CreateSubControl ()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub ( this, "MMR_WINDOW_BUTTON_IMAGE", UI_FLAG_DEFAULT, MMR_WINDOW_BUTTON_IMAGE );
	m_pButtonImage->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonImage );
}

void CMMRWindowButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CMMRWindowButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case MMR_WINDOW_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "RAN_MMR_WINDOW_TITLE", 0 ), NS_UITEXTCOLOR::WHITE  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{ 
				CInnerInterface::GetInstance().OpenMMRWindow();
			}
		}break;
	}
}
