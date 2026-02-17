#include "StdAfx.h"
#include "ItemShopButton.h"
#include "../Lib_ClientUI/Interface/BasicButton.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BLINK_TIME_LOOP (float)0.2f

CItemShopButton::CItemShopButton ()
: m_pButtonImage(NULL)
, m_pButtonBlink(NULL)
, m_pButtonLock(NULL)
, m_pButtonAlarm(NULL)
, m_fBLINK_TIME(0.0f)
, m_bAlarm(FALSE)
, m_bLocked(FALSE)
{
}

CItemShopButton::~CItemShopButton ()
{
}

void CItemShopButton::CreateSubControl ()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub ( this, "ITEM_SHOP_BUTTON_BUTTON_IMAGE", UI_FLAG_DEFAULT, ITEM_SHOP_BUTTON_BUTTON_IMAGE );
	m_pButtonImage->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonImage );

	m_pButtonBlink = new CUIControl;
	m_pButtonBlink->CreateSub ( this, "ITEM_SHOP_BUTTON_BUTTON_BLINK" );
	m_pButtonBlink->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pButtonBlink );

	m_pButtonLock = new CUIControl;
	m_pButtonLock->CreateSub ( this, "ITEM_SHOP_BUTTON_LOCK" );
	m_pButtonLock->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pButtonLock );
}

void CItemShopButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_bAlarm ){		
		m_fBLINK_TIME += fElapsedTime;
		if ( BLINK_TIME_LOOP <= m_fBLINK_TIME ){
			if ( m_pButtonBlink ){
				BOOL bVisible = m_pButtonBlink->IsVisible ();
				m_pButtonBlink->SetVisibleSingle ( !bVisible );
			}

			m_fBLINK_TIME = 0.0f;
		}
	}
}

void CItemShopButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ITEM_SHOP_BUTTON_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) ){
				//CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( ID2GAMEWORD("ITEMSHOP_NAME_STATIC"), NS_UITEXTCOLOR::WHITE  );			
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "ITEMSHOP_NAME_STATIC" ), NS_UITEXTCOLOR::WHITE );
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{ 
				CInnerInterface::GetInstance().ItemShopAuth();
			}
		}break;
	}
}

void CItemShopButton::SetButtonAlarm( BOOL bSet )
{
	m_bAlarm = bSet;

	if ( m_pButtonBlink ){
		m_pButtonBlink->SetVisibleSingle ( m_bAlarm );
	}
}

void CItemShopButton::SetButtonLock( BOOL bSet )
{
	m_bLocked = bSet;
	if ( m_pButtonLock ){
		m_pButtonLock->SetVisibleSingle( m_bLocked );
	}
}