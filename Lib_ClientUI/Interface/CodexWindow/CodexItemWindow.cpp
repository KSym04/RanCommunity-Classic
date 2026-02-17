#include "StdAfx.h"
#include "CodexItemWindow.h"

#include "../Lib_ClientUI/Interface/BasicLineBox.h"
#include "../Lib_ClientUI/Interface/BasicTextBox.h"
#include "../Lib_ClientUI/Interface/BasicTextButton.h"
#include "CodexItemList.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"

#include "../Lib_ClientUI/Interface/InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCodexItemWindow::CCodexItemWindow(void)
	: m_pTextDesc(NULL)
	, m_pButtonClose(NULL)
	, m_pItemList(NULL)
{
}

CCodexItemWindow::~CCodexItemWindow(void)
{
}

void CCodexItemWindow::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_CODEX_SALESITEM_WINDOW_LINEBOX_DESC" );
	RegisterControl ( pLineBox );

	m_pTextDesc = new CBasicTextBox;
	m_pTextDesc->CreateSub ( this, "RAN_CODEX_SALESITEM_WINDOW_TEXT_DESC" );
	m_pTextDesc->SetFont ( pFont9 );
	m_pTextDesc->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pTextDesc->SetText( ID2GAMEINTEXT( "ACTIVITY_SALESITEM_WINDOW_TEXT_DESC0" ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextDesc );

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, ACTIVITY_ITEM_WINDOW_BUTTON_CLOSE );
	m_pButtonClose->CreateBaseButton ( "RAN_CODEX_SALESITEM_WINDOW_BUTTON_CLOSE", CBasicTextButton::SIZE19, CBasicButton::CLICK_FLIP, ID2GAMEWORD("RAN_CODEX_SALESITEM_WINDOW_BUTTON", 0 ) );
	m_pButtonClose->SetFlip ( TRUE );
	RegisterControl ( m_pButtonClose );

	m_pItemList = new CCodexItemList;
	m_pItemList->CreateSub( this, "RAN_CODEX_SALESITEM_WINDOW_ITEMLIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE, ACTIVITY_ITEM_WINDOW_ITEM_LIST );
	m_pItemList->CreateSubControl ();
	RegisterControl ( m_pItemList );
}

void CCodexItemWindow::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;


	case ACTIVITY_ITEM_WINDOW_BUTTON_CLOSE:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) ){
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;
	};

}

void CCodexItemWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		if ( m_pItemList )
		{
			m_pItemList->ResetData();
			m_pItemList->LoadData();
		}
	}
	else
	{
	}
}