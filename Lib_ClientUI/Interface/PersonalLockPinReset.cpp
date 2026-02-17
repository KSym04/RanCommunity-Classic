#include "StdAfx.h"
#include "./PersonalLockPinReset.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicComboBox.h"
#include "./BasicComboBoxRollOver.h"
#include "./BasicButton.h"
#include "./BasicTextButton.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLPersonalLock.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPersonalLockPinReset::CPersonalLockPinReset(void)
	: m_pFont(NULL)
	, m_pLineBoxMain(NULL)
	, m_pStaticSelect(NULL)
	, m_pComboBoxSelectOpen(NULL)
	, m_pComboBoxSelectRollOver(NULL)
	, m_pButtonClose(NULL)
	, m_pButtonReset(NULL)
	, m_RollOverID(NO_ID)
	, m_bFirstLBUP(FALSE)
	, m_nSelected(0)
{
}

CPersonalLockPinReset::~CPersonalLockPinReset(void)
{
}

void CPersonalLockPinReset::CreateSubControl()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBoxMain = new CBasicLineBox;
	m_pLineBoxMain->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxMain->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_RESET_LINEBOX" );
	RegisterControl ( m_pLineBoxMain );

	m_pStaticSelect = new CBasicTextBox;
	m_pStaticSelect->CreateSub ( this, "PERSONAL_LOCK_PIN_RESET_SELECT_STATIC" );
	m_pStaticSelect->SetFont ( m_pFont );
	m_pStaticSelect->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticSelect->SetOneLineText ( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_RESET", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticSelect );

	m_pComboBoxSelectOpen = new CBasicComboBox;
	m_pComboBoxSelectOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_RESET_SELECTION_COMBO_OPEN );
	m_pComboBoxSelectOpen->CreateBaseComboBox ( "PERSONAL_LOCK_PIN_RESET_SELECT_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxSelectOpen );

	m_pComboBoxSelectRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxSelectRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PERSONAL_LOCK_PIN_RESET_SELECTION_COMBO_ROLLOVER );
	m_pComboBoxSelectRollOver->CreateBaseComboBoxRollOver ( "PERSONAL_LOCK_PIN_RESET_SELECT_ROLLOVER" );			
	m_pComboBoxSelectRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxSelectRollOver );

	m_pButtonReset = new CBasicTextButton;
	m_pButtonReset->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_RESET_BUTTON_RESET );
	m_pButtonReset->CreateBaseButton ( "PERSONAL_LOCK_PIN_RESET_BUTTON_RESET", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_RESET", 2 ) );
	RegisterControl ( m_pButtonReset );

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_RESET_BUTTON_CANCEL );
	m_pButtonClose->CreateBaseButton ( "PERSONAL_LOCK_PIN_RESET_BUTTON_CANCEL", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_RESET", 3 ) );
	RegisterControl ( m_pButtonClose );
}

void CPersonalLockPinReset::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
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

	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	case PERSONAL_LOCK_PIN_RESET_SELECTION_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxSelectRollOver->SetVisibleSingle(TRUE);

					m_RollOverID = PERSONAL_LOCK_PIN_RESET_SELECTION_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case PERSONAL_LOCK_PIN_RESET_SELECTION_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxSelectRollOver )
				{
					int nIndex = m_pComboBoxSelectRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_nSelected = nIndex;
					SetTextComboPart( m_nSelected, m_pComboBoxSelectOpen );
				}	
			}
		}break;

	case PERSONAL_LOCK_PIN_RESET_BUTTON_CANCEL:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	case PERSONAL_LOCK_PIN_RESET_BUTTON_RESET:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				DoReset();
			}
		}break;
	};
}

void CPersonalLockPinReset::Update ( int x ,int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( m_RollOverID == NO_ID )
	{
		CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	}
	else
	{
		ResetMessageEx ();

		CUIControl::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		CUIControl* pControl = m_ControlContainer.FindControl ( m_RollOverID );
		if ( !pControl )
		{
			GASSERT ( 0 && "Invalid RollOver ID." );
			return;
		}

		pControl->Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		DWORD dwControlMsg = pControl->GetMessageEx ();
		NS_UIDEBUGSET::BlockBegin ();
		if ( dwControlMsg ) TranslateUIMessage ( m_RollOverID, dwControlMsg );
		NS_UIDEBUGSET::BlockEnd ();

		if ( !(dwControlMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL) )
		{
			DWORD dwMsg = GetMessageEx ();
			if ( dwMsg & UIMSG_LB_UP )
			{
				if ( !m_bFirstLBUP )
				{
					m_RollOverID = NO_ID;
					pControl->SetVisibleSingle ( FALSE );					
				}
				m_bFirstLBUP = FALSE;
			}
		}
	}
	
}

void CPersonalLockPinReset::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		LoadData();
	}
}

void CPersonalLockPinReset::LoadData()
{
	if ( m_pComboBoxSelectRollOver )
	{
		m_pComboBoxSelectRollOver->ClearText ();

		for ( int i = 0; i < PERSONAL_LOCK_PIN_RESET_MAX_SELECTION; i++ )
		{
			m_pComboBoxSelectRollOver->AddText ( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_RESET_SELECT", i ) );
		}
	}

	m_nSelected = 0;
	SetTextComboPart( m_nSelected, m_pComboBoxSelectOpen );
}

void CPersonalLockPinReset::SetTextComboPart( DWORD dwIndex, CBasicComboBox* pComboBox )
{
	if ( 0 <= dwIndex && dwIndex < PERSONAL_LOCK_PIN_RESET_MAX_SELECTION )
	{
		CString strTemp( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_RESET_SELECT", dwIndex ) );

		if ( pComboBox )
			pComboBox->SetText ( strTemp );
	}
}

void CPersonalLockPinReset::DoReset()
{
	switch ( m_nSelected )
	{
	case 1://Reset Equipment PIN
		{
			GLGaeaClient::GetInstance().GetCharacter()->PersonalLockPinReset(EMPERSONAL_LOCK_EQUIP);
			CInnerInterface::GetInstance().HideGroup( GetWndID() );
		}break;

	case 2://Reset Inventory PIN
		{
			GLGaeaClient::GetInstance().GetCharacter()->PersonalLockPinReset(EMPERSONAL_LOCK_INVEN);
			CInnerInterface::GetInstance().HideGroup( GetWndID() );
		}break;

	case 3://Reset Locker PIN
		{
			GLGaeaClient::GetInstance().GetCharacter()->PersonalLockPinReset(EMPERSONAL_LOCK_LOCKER);
			CInnerInterface::GetInstance().HideGroup( GetWndID() );
		}break;

	case 4://Reset ALL PIN
		{
			GLGaeaClient::GetInstance().GetCharacter()->PersonalLockPinReset(EMPERSONAL_LOCK_SIZE);
			CInnerInterface::GetInstance().HideGroup( GetWndID() );
		}break;

	default:
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RESET_INVALID_LOCK_TYPE") );
		break;
	}
}