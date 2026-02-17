#include "StdAfx.h"
#include "./PersonalLockPinRecoverResult.h"

#include "./BasicLineBox.h"
#include "./BasicButton.h"
#include "./BasicTextButton.h"
#include "./BasicTextBox.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPersonalLockPinRecoverResult::CPersonalLockPinRecoverResult(void)
	: m_pButtonClose(NULL)
	, m_pTextResult(NULL)
{
}

CPersonalLockPinRecoverResult::~CPersonalLockPinRecoverResult(void)
{
}

void CPersonalLockPinRecoverResult::CreateSubControl()
{
	CD3DFontPar* pFont09 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_RECOVER_RESULT_LINEBOX" );
	RegisterControl ( pLineBox );

	m_pTextResult = new CBasicTextBox;
	m_pTextResult->CreateSub ( this, "PERSONAL_LOCK_PIN_RECOVER_RESULT_INFO_TEXT", UI_FLAG_DEFAULT );
	m_pTextResult->SetFont ( pFont09 );
	m_pTextResult->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextResult );

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_RECOVER_RESULT_BUTTON_CLOSE );
	m_pButtonClose->CreateBaseButton ( "PERSONAL_LOCK_PIN_RECOVER_RESULT_BUTTON_CANCEL", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_RECOVER_RESULT", 1 ) );
	RegisterControl ( m_pButtonClose );
}

void CPersonalLockPinRecoverResult::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;

	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	case PERSONAL_LOCK_PIN_RECOVER_RESULT_BUTTON_CLOSE:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;
	
	};
}

void CPersonalLockPinRecoverResult::Update ( int x ,int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CPersonalLockPinRecoverResult::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
}

void CPersonalLockPinRecoverResult::InfoClear()
{
	if ( m_pTextResult )
		m_pTextResult->ClearText();
}

void CPersonalLockPinRecoverResult::InfoAdd( SPERSONAL_LOCK_PIN* pPin, EMPERSONAL_LOCK emLock )
{
	if ( !pPin )							return;
	if ( emLock >= EMPERSONAL_LOCK_SIZE )	return;

	if ( m_pTextResult )
	{
		CString strTemp;
		strTemp.Format( ID2GAMEINTEXT( "PERSONAL_LOCK_PIN_RECOVER_RESULT_INFO" ), ID2GAMEWORD( "PERSONAL_LOCK_INFO", emLock ), pPin->szPin );
		m_pTextResult->AddText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
	}
}