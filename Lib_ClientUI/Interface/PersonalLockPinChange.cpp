#include "StdAfx.h"
#include "./PersonalLockPinChange.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicComboBox.h"
#include "./BasicComboBoxRollOver.h"
#include "./BasicButton.h"
#include "./BasicTextButton.h"
#include "./UIEditBoxMan.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLPersonalLock.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/GUInterface/UIKeyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT	CPersonalLockPinChange::nLIMIT_PIN		= 4;

CPersonalLockPinChange::CPersonalLockPinChange(void)
	: m_pFont(NULL)
	, m_pStaticSelect(NULL)
	, m_pStaticPinOld(NULL)
	, m_pStaticPinNew(NULL)
	, m_pStaticPinNew2(NULL)
	, m_pComboBoxSelectOpen(NULL)
	, m_pComboBoxSelectRollOver(NULL)
	, m_pButtonClose(NULL)
	, m_pButtonChange(NULL)
	, m_RollOverID(NO_ID)
	, m_bFirstLBUP(FALSE)
	, m_nSelected(0)
{
}

CPersonalLockPinChange::~CPersonalLockPinChange(void)
{
}

void CPersonalLockPinChange::CreateSubControl()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CHANGE_LINEBOX" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CHANGE_LINEBOX_PIN_OLD" );
	RegisterControl ( pBasicLineBox );
	
	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CHANGE_LINEBOX_PIN_NEW" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CHANGE_LINEBOX_PIN_NEW_2" );
	RegisterControl ( pBasicLineBox );


	m_pStaticSelect = new CBasicTextBox;
	m_pStaticSelect->CreateSub ( this, "PERSONAL_LOCK_PIN_CHANGE_SELECT_STATIC" );
	m_pStaticSelect->SetFont ( m_pFont );
	m_pStaticSelect->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticSelect->SetOneLineText ( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_CHANGE", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticSelect );

	m_pStaticPinOld = new CBasicTextBox;
	m_pStaticPinOld->CreateSub ( this, "PERSONAL_LOCK_PIN_CHANGE_PIN_OLD_STATIC" );
	m_pStaticPinOld->SetFont ( m_pFont );
	m_pStaticPinOld->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPinOld->SetOneLineText ( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_CHANGE", 4 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPinOld );

	m_pStaticPinNew = new CBasicTextBox;
	m_pStaticPinNew->CreateSub ( this, "PERSONAL_LOCK_PIN_CHANGE_PIN_NEW_STATIC" );
	m_pStaticPinNew->SetFont ( m_pFont );
	m_pStaticPinNew->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPinNew->SetOneLineText ( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_CHANGE", 5 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPinNew );

	m_pStaticPinNew2 = new CBasicTextBox;
	m_pStaticPinNew2->CreateSub ( this, "PERSONAL_LOCK_PIN_CHANGE_PIN_NEW_2_STATIC" );
	m_pStaticPinNew2->SetFont ( m_pFont );
	m_pStaticPinNew2->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPinNew2->SetOneLineText ( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_CHANGE", 6 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPinNew2 );

	m_pEditBoxMan = new CUIEditBoxMan;
	m_pEditBoxMan->CreateSub ( this, "PERSONAL_LOCK_PIN_CHANGE_EDITMAN", UI_FLAG_DEFAULT, PERSONAL_LOCK_PIN_CHANGE_EDIT_MAN );
	m_pEditBoxMan->CreateEditBox ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_OLD, "PERSONAL_LOCK_PIN_CHANGE_TEXT_PIN_OLD", "PERSONAL_LOCK_PIN_CHANGE_CARRAT", TRUE, UINT_MAX, m_pFont, nLIMIT_PIN );
	m_pEditBoxMan->CreateEditBox ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW, "PERSONAL_LOCK_PIN_CHANGE_TEXT_PIN_NEW", "PERSONAL_LOCK_PIN_CHANGE_CARRAT", TRUE, UINT_MAX, m_pFont, nLIMIT_PIN );
	m_pEditBoxMan->CreateEditBox ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW2, "PERSONAL_LOCK_PIN_CHANGE_TEXT_PIN_NEW_2", "PERSONAL_LOCK_PIN_CHANGE_CARRAT", TRUE, UINT_MAX, m_pFont, nLIMIT_PIN );
	m_pEditBoxMan->SetHide ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_OLD, TRUE );
	m_pEditBoxMan->SetHide ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW, TRUE );
	m_pEditBoxMan->SetHide ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW2, TRUE );
	RegisterControl ( m_pEditBoxMan );

	m_pComboBoxSelectOpen = new CBasicComboBox;
	m_pComboBoxSelectOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_CHANGE_SELECTION_COMBO_OPEN );
	m_pComboBoxSelectOpen->CreateBaseComboBox ( "PERSONAL_LOCK_PIN_CHANGE_SELECT_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxSelectOpen );

	m_pComboBoxSelectRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxSelectRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PERSONAL_LOCK_PIN_CHANGE_SELECTION_COMBO_ROLLOVER );
	m_pComboBoxSelectRollOver->CreateBaseComboBoxRollOver ( "PERSONAL_LOCK_PIN_CHANGE_SELECT_ROLLOVER" );			
	m_pComboBoxSelectRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxSelectRollOver );

	m_pButtonChange = new CBasicTextButton;
	m_pButtonChange->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_CHANGE_BUTTON_RESET );
	m_pButtonChange->CreateBaseButton ( "PERSONAL_LOCK_PIN_CHANGE_BUTTON_CHANGE", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_CHANGE", 2 ) );
	RegisterControl ( m_pButtonChange );

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_CHANGE_BUTTON_CANCEL );
	m_pButtonClose->CreateBaseButton ( "PERSONAL_LOCK_PIN_CHANGE_BUTTON_CANCEL", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_CHANGE", 3 ) );
	RegisterControl ( m_pButtonClose );
}

void CPersonalLockPinChange::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
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

	case PERSONAL_LOCK_PIN_CHANGE_SELECTION_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxSelectRollOver->SetVisibleSingle(TRUE);

					m_RollOverID = PERSONAL_LOCK_PIN_CHANGE_SELECTION_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case PERSONAL_LOCK_PIN_CHANGE_SELECTION_COMBO_ROLLOVER:
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

	case PERSONAL_LOCK_PIN_CHANGE_BUTTON_CANCEL:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	case PERSONAL_LOCK_PIN_CHANGE_BUTTON_RESET:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				DoChange();
			}
		}break;
	};
}

void CPersonalLockPinChange::Update ( int x ,int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( m_RollOverID == NO_ID )
	{
		CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		if ( m_pEditBoxMan )
		{
			CUIControl* pParent = m_pEditBoxMan->GetTopParent ();
			if ( !pParent )	pParent = this;
			BOOL bFocus = ( pParent->IsFocusControl() );

			if ( bFocus )
			{
				if ( UIKeyCheck::GetInstance()->Check ( DIK_TAB, DXKEY_DOWN ) )
				{
					m_pEditBoxMan->GoNextTab ();
				}
			}
		}
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

void CPersonalLockPinChange::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		LoadData();

		if ( m_pEditBoxMan )
		{
			m_pEditBoxMan->EndEdit ();
			m_pEditBoxMan->ClearEdit ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_OLD );
			m_pEditBoxMan->ClearEdit ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW );
			m_pEditBoxMan->ClearEdit ( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW2 );

			m_pEditBoxMan->Init();
			m_pEditBoxMan->BeginEdit();

			if( m_pEditBoxMan->IsMODE_NATIVE() )
			{
				m_pEditBoxMan->DoMODE_TOGGLE();
			}
		}
	}
	else
	{
		if ( m_pEditBoxMan )	m_pEditBoxMan->EndEdit ();
	}
}

void CPersonalLockPinChange::LoadData()
{
	if ( m_pComboBoxSelectRollOver )
	{
		m_pComboBoxSelectRollOver->ClearText ();

		for ( int i = 0; i < PERSONAL_LOCK_PIN_CHANGE_MAX_SELECTION; i++ )
		{
			m_pComboBoxSelectRollOver->AddText ( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_CHANGE_SELECT", i ) );
		}
	}

	m_nSelected = 0;
	SetTextComboPart( m_nSelected, m_pComboBoxSelectOpen );
}

void CPersonalLockPinChange::SetTextComboPart( DWORD dwIndex, CBasicComboBox* pComboBox )
{
	if ( 0 <= dwIndex && dwIndex < PERSONAL_LOCK_PIN_CHANGE_MAX_SELECTION )
	{
		CString strTemp( ID2GAMEWORD ( "PERSONAL_LOCK_PIN_CHANGE_SELECT", dwIndex ) );

		if ( pComboBox )
			pComboBox->SetText ( strTemp );
	}
}

void CPersonalLockPinChange::DoChange()
{
	if ( m_nSelected <= 0 || m_nSelected >= PERSONAL_LOCK_PIN_CHANGE_MAX_SELECTION )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_INVALID_LOCK_TYPE") );
		return;
	}

	std::string strPinOld = m_pEditBoxMan->GetEditString( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_OLD );
	std::string strPinNew = m_pEditBoxMan->GetEditString( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW );
	std::string strPinNew2 = m_pEditBoxMan->GetEditString( PERSONAL_LOCK_PIN_CHANGE_EDIT_PIN_NEW2 );

	if ( strPinOld.empty() || strPinNew.empty() || strPinNew2.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_EMPTY") );
		return;
	}

	if ( strPinOld.size() < nLIMIT_PIN || strPinOld.size() > nLIMIT_PIN ||
		strPinNew.size() < nLIMIT_PIN || strPinNew.size() > nLIMIT_PIN ||
		strPinNew2.size() < nLIMIT_PIN || strPinNew2.size() > nLIMIT_PIN )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_SIZE") );
		return;
	}

	if ( strPinOld.find_first_not_of( "0123456789" ) != std::string::npos ||
		strPinNew.find_first_not_of( "0123456789" ) != std::string::npos ||
		strPinNew2.find_first_not_of( "0123456789" ) != std::string::npos )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_NUMBER_ONLY") );
		return;
	}

	if ( strcmp( strPinNew.c_str(), strPinNew2.c_str() ) != 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_MISMATCH") );
		return;
	}

	switch ( m_nSelected )
	{
	case 1://Change Equipment PIN
		{
			GLGaeaClient::GetInstance().GetCharacter()->PersonalLockPinChange(EMPERSONAL_LOCK_EQUIP, strPinOld, strPinNew, strPinNew2 );
			CInnerInterface::GetInstance().HideGroup( GetWndID() );
		}break;

	case 2://Change Inventory PIN
		{
			GLGaeaClient::GetInstance().GetCharacter()->PersonalLockPinChange(EMPERSONAL_LOCK_INVEN, strPinOld, strPinNew, strPinNew2 );
			CInnerInterface::GetInstance().HideGroup( GetWndID() );
		}break;

	case 3://Change Locker PIN
		{
			GLGaeaClient::GetInstance().GetCharacter()->PersonalLockPinChange(EMPERSONAL_LOCK_LOCKER, strPinOld, strPinNew, strPinNew2 );
			CInnerInterface::GetInstance().HideGroup( GetWndID() );
		}break;

	default:
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_INVALID_LOCK_TYPE") );
		break;
	}


}