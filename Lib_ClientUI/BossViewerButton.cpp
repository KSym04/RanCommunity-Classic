#include "StdAfx.h"
#include "BossViewerButton.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BLINK_TIME_LOOP (float)0.2f

CBossViewerButton::CBossViewerButton ()
	: m_pButtonImage(NULL)
{
}

CBossViewerButton::~CBossViewerButton ()
{
}

void CBossViewerButton::CreateSubControl ()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub ( this, "BOSS_VIEWER_BUTTON_IMAGE", UI_FLAG_DEFAULT, BOSS_VIEWER_BUTTON_IMAGE );
	m_pButtonImage->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonImage );
}

void CBossViewerButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CBossViewerButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case BOSS_VIEWER_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) ){
				//CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Boss Spawn", NS_UITEXTCOLOR::WHITE  );			
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( "Boss Tracker", NS_UITEXTCOLOR::WHITE );
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) ){ 
				CInnerInterface::GetInstance().ShowGroupBottom ( BOSS_DETAILS_WINDOW );
			}
		}break;
	}
}
