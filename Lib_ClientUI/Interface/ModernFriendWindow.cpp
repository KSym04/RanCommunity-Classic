#include "StdAfx.h"
#include "./ModernFriendWindow.h"

#include "./ModernFriendWindowBlockPage.h"
#include "./ModernFriendWindowNormalPage.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./MultiModeButton.h"

#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernFriendWindow::CModernFriendWindow ()
	: m_pNORMAL_TAB_BUTTON( NULL )
    , m_pBLOCK_TAB_BUTTON( NULL )
	, m_pBlockPage( NULL )
	, m_pNormalPage( NULL )
{
}

CModernFriendWindow::~CModernFriendWindow ()
{
}

void CModernFriendWindow::CreateSubControl ()
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "MODERN_FRIEND_WINDOW_WHITE" );
	RegisterControl ( pControl );

	m_pNORMAL_TAB_BUTTON = CreateTabButton( "MODERN_FRIEND_WINDOW_FRIEND_BUTTON", ID2GAMEWORD ( "MODERN_FRIEND_WINDOW_TEXT", 0 ), FRIEND_NORMAL_TAB_BUTTON );
	m_pBLOCK_TAB_BUTTON	 = CreateTabButton( "MODERN_FRIEND_WINDOW_BLOCK_BUTTON", ID2GAMEWORD ( "MODERN_FRIEND_WINDOW_TEXT", 1 ), FRIEND_BLOCK_TAB_BUTTON );

	m_pNormalPage = new CModernFriendWindowNormalPage;
	m_pNormalPage->CreateSub ( this, "MODERN_FRIEND_WINDOW_FRIEND_TAB", UI_FLAG_DEFAULT, FRIEND_NORMAL_PAGE );
	m_pNormalPage->CreateSubControl ();
	RegisterControl ( m_pNormalPage );

	m_pBlockPage = new CModernFriendWindowBlockPage;
	m_pBlockPage->CreateSub ( this, "MODERN_FRIEND_WINDOW_BLOCK_TAB", UI_FLAG_DEFAULT, FRIEND_BLOCK_PAGE );
	m_pBlockPage->CreateSubControl ();
	RegisterControl ( m_pBlockPage );

	ChangePage ( FRIEND_NORMAL_TAB_BUTTON );
}

CMultiModeButton* CModernFriendWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
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

void CModernFriendWindow::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( FRIEND_WINDOW );
			}
		}break;

	case FRIEND_NORMAL_TAB_BUTTON:
	case FRIEND_BLOCK_TAB_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_DOWN & dwMsg ) ChangePage ( cID );
			}
		}break;
	}
}

void CModernFriendWindow::ChangePage ( const UIGUID& cClickButton )
{
	m_pNORMAL_TAB_BUTTON->DoImageFlip( FALSE );
	m_pBLOCK_TAB_BUTTON->DoImageFlip( FALSE );

	m_pNormalPage->SetVisibleSingle( FALSE );
	m_pBlockPage->SetVisibleSingle( FALSE );

	switch ( cClickButton )
	{
	case FRIEND_NORMAL_TAB_BUTTON:
		{
			m_pNORMAL_TAB_BUTTON->DoImageFlip( TRUE );
			m_pNormalPage->SetVisibleSingle( TRUE );
		}break;

	case FRIEND_BLOCK_TAB_BUTTON:
		{
			m_pBLOCK_TAB_BUTTON->DoImageFlip( TRUE );
			m_pBlockPage->SetVisibleSingle( TRUE );
		}break;
	}
}

void  CModernFriendWindow::LoadFriendList ()
{
	if ( m_pNormalPage->IsVisible () )	m_pNormalPage->LoadFriendList ();
	else								m_pBlockPage->LoadBlockList ();
}

void CModernFriendWindow::ADD_FRIEND_NAME_TO_EDITBOX ( const CString& strName )
{
	m_pNormalPage->ADD_FRIEND_NAME_TO_EDITBOX ( strName );
	m_pBlockPage->ADD_NAME_TO_EDITBOX ( strName );
}

void CModernFriendWindow::ADD_FRIEND ( const CString& strName )
{
	m_pNormalPage->ADD_FRIEND ( strName );
}

const CString& CModernFriendWindow::GET_FRIEND_NAME () const
{
	return m_pNormalPage->GET_FRIEND_NAME ();
}

const CString& CModernFriendWindow::GET_BLOCK_NAME () const
{
	return m_pBlockPage->GET_BLOCK_NAME ();
}

void CModernFriendWindow::EDIT_END ()
{
	m_pNormalPage->EDIT_END ();
	m_pBlockPage->EDIT_END ();
}

void CModernFriendWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		GLGaeaClient::GetInstance().GetCharacter()->ReqFriendWindowOpen( true );

		if( m_pNormalPage->IsVisible() )		m_pNormalPage->SetVisibleSingle( TRUE );
		else if( m_pBlockPage->IsVisible() )	m_pBlockPage->SetVisibleSingle( TRUE );
	}
	else
	{
		GLGaeaClient::GetInstance().GetCharacter()->ReqFriendWindowOpen( false );
	}
}

SFRIEND & CModernFriendWindow::GetFriendSMSInfo()
{ 
	return m_pNormalPage->GetFriendInfo(); 
}