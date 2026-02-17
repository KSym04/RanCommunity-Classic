#include "StdAfx.h"
#include "GachaButton.h"
#include "../InnerInterface.h"
#include "../GameTextControl.h"
#include "../UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGachaButton::CGachaButton ()
	: m_pButtonImage(NULL)
{
}

CGachaButton::~CGachaButton ()
{
}

void CGachaButton::CreateSubControl ()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub ( this, "PERSONAL_GACHA_IMAGE", UI_FLAG_DEFAULT, STUDENT_RECORD_BUTTON_IMAGE );
	m_pButtonImage->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonImage );
}

void CGachaButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CGachaButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case STUDENT_RECORD_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "GACHA_WINDOW_TITLE", 0 ), NS_UITEXTCOLOR::WHITE  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) ){ 
				CInnerInterface::GetInstance().OpenGachaWindow();
			}
		}break;
	}
}
