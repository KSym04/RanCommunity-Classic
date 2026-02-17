#include "StdAfx.h"
#include "./GameItemShopIcon.h"
#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGameItemShopIcon::CGameItemShopIcon ()
	: m_pButtonImage(NULL)
{
}

CGameItemShopIcon::~CGameItemShopIcon ()
{
}

void CGameItemShopIcon::CreateSubControl ()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub ( this, "GAME_ITEM_SHOP_ICON_IMAGE", UI_FLAG_DEFAULT, BUTTON_IMAGE );
	m_pButtonImage->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonImage );
}

void CGameItemShopIcon::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CGameItemShopIcon::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case BUTTON_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEINTEXT( "GAME_ITEM_SHOP_BUTTON_MESSAGE" ), NS_UITEXTCOLOR::WHITE );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{ 
				CInnerInterface::GetInstance().OpenGameItemShopWindow();
			}
		}break;
	}
}
