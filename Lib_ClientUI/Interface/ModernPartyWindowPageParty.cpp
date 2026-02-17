#include "StdAfx.h"
#include "./ModernPartyWindowPageParty.h"

#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./MultiModeButton.h"
#include "./BasicLineBox.h"
#include "./BasicComboBoxRollOver.h"
#include "./BasicComboBox.h"
#include "./ModernPartySlot.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLPartyClient.h"
#include "../../Lib_Client/DxGlobalStage.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSROption.h"
#include "../../Lib_Engine/Core/NSRProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartyWindowPageParty::CModernPartyWindowPageParty () :
	m_pButtonRelease ( NULL ),
	m_pButtonBan ( NULL ),
	m_pButtonDelegate ( NULL ),
	m_pButtonInfo ( NULL ),
	m_pButtonWithdraw ( NULL ),
	m_pCheckBoxBuff( NULL ),
	m_pCheckBoxInfo( NULL ),
	m_pComboBoxItemOpen( NULL ),
	//m_pComboBoxItemFilterOpen( NULL ),
	m_pComboBoxMoneyOpen( NULL ),	
	m_pComboBoxItemRollOver( NULL ),
	//m_pComboBoxItemFilterRollOver( NULL ),
	m_pComboBoxMoneyRollOver( NULL ),
	m_bMASTER( FALSE ),
	m_bMEMBER( FALSE ),
	m_RollOverID ( NO_ID ),
	m_pTextNoParty( NULL ),
	m_nSelect( -1 ),
	m_pCheckBoxPrivate(NULL)
{
	for ( int i = 0; i < MAXPARTY; i++ )
	{
		m_pPartySlot[i] = NULL;
	}
}

CModernPartyWindowPageParty::~CModernPartyWindowPageParty ()
{
}

CBasicTextBox* CModernPartyWindowPageParty::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

void CModernPartyWindowPageParty::CreateSubControl ()
{
	CBasicTextBox* pTextBox = NULL;
	int nTextAlign = TEXT_ALIGN_LEFT;
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_PARTY_INFO_OPTION_REGION" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_PARTY_INFO_MEMBER_REGION" );
	RegisterControl ( pBasicLineBox );

	pTextBox = CreateStaticControl ( "MODERN_PARTY_INFO_ITEM_STATIC", pFont, nTextAlign );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "MODERN_PARTYWINDOW_TEXTS", 1 ), NS_UITEXTCOLOR::WHITE );

	pTextBox = CreateStaticControl ( "MODERN_PARTY_INFO_MONEY_STATIC", pFont,nTextAlign );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "MODERN_PARTYWINDOW_TEXTS", 2 ), NS_UITEXTCOLOR::WHITE );

	pTextBox = CreateStaticControl ( "MODERN_PARTY_INFO_BUFF_CHECK_TEXT", pFont, nTextAlign );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "MODERN_PARTYWINDOW_TEXTS", 3 ), NS_UITEXTCOLOR::WHITE );

	pTextBox = CreateStaticControl ( "MODERN_PARTY_INFO_CHECK_TEXT", pFont, nTextAlign );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "MODERN_PARTYWINDOW_TEXTS", 4 ), NS_UITEXTCOLOR::WHITE );

	//pTextBox = CreateStaticControl ( "MODERN_PARTY_INFO_ITEM_FILTER_STATIC", pFont, nTextAlign );
	//pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "MODERN_PARTYWINDOW_TEXTS", 5 ), NS_UITEXTCOLOR::WHITE );

	pTextBox = CreateStaticControl ( "MODERN_PARTY_PRIVATE_CHECK_TEXT", pFont, nTextAlign );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "MODERN_PARTYWINDOW_TEXTS_EX", 0 ), NS_UITEXTCOLOR::WHITE );


	CString strMemberKeyword[MAXPARTY] = 
	{
		"MODERN_PARTY_INFO_SLOT0",
		"MODERN_PARTY_INFO_SLOT1",
		"MODERN_PARTY_INFO_SLOT2",
		"MODERN_PARTY_INFO_SLOT3",
		"MODERN_PARTY_INFO_SLOT4",
		"MODERN_PARTY_INFO_SLOT5",
		"MODERN_PARTY_INFO_SLOT6",
		"MODERN_PARTY_INFO_SLOT7",
	};

	for ( int i = 0; i < MAXPARTY; i++ )
	{
		CModernPartySlot* pPartySlot = new CModernPartySlot;
		pPartySlot->CreateSub ( this, strMemberKeyword[i].GetString (), UI_FLAG_DEFAULT, PARTY_SLOT_MEMBER0 + i );
		pPartySlot->CreateSubControl ();
		pPartySlot->SetVisibleSingle( FALSE );
		RegisterControl ( pPartySlot );
		m_pPartySlot[i] = pPartySlot;
	}

	m_pCheckBoxBuff = new CBasicButton;
	m_pCheckBoxBuff->CreateSub ( this, "MODERN_PARTY_INFO_BUFF_CHECK_BOX", UI_FLAG_DEFAULT, PARTY_CHECKBOX_BUFF );
	m_pCheckBoxBuff->CreateFlip ( "MODERN_PARTY_INFO_CHECK_BOX_ON", CBasicButton::RADIO_FLIP );
	m_pCheckBoxBuff->SetControlNameEx ( "MODERN_PARTY_INFO_BUFF_CHECK_BOX" );
	RegisterControl ( m_pCheckBoxBuff );
	m_pCheckBoxBuff->SetFlip( RPROFILE::bPartyWindowShowBuff );

	m_pCheckBoxInfo = new CBasicButton;
	m_pCheckBoxInfo->CreateSub ( this, "MODERN_PARTY_INFO_CHECK_BOX", UI_FLAG_DEFAULT, PARTY_CHECKBOX_INFO );
	m_pCheckBoxInfo->CreateFlip ( "MODERN_PARTY_INFO_CHECK_BOX_ON", CBasicButton::RADIO_FLIP );
	m_pCheckBoxInfo->SetControlNameEx ( "MODERN_PARTY_INFO_CHECK_BOX" );
	RegisterControl ( m_pCheckBoxInfo );
	m_pCheckBoxInfo->SetFlip( RPROFILE::bPartyWindowShowInfo );

	m_pCheckBoxPrivate = new CBasicButton;
	m_pCheckBoxPrivate->CreateSub ( this, "MODERN_PARTY_PRIVATE_CHECK_BOX", UI_FLAG_DEFAULT, PARTY_CHECKBOX_PRIVATE );
	m_pCheckBoxPrivate->CreateFlip ( "MODERN_PARTY_PRIVATE_CHECK_BOX_ON", CBasicButton::RADIO_FLIP );
	m_pCheckBoxPrivate->SetControlNameEx ( "MODERN_PARTY_PRIVATE_CHECK_BOX" );
	RegisterControl ( m_pCheckBoxPrivate );
	m_pCheckBoxPrivate->SetFlip( RPROFILE::bPARTY_PRIVATE );


	m_pComboBoxItemOpen = new CBasicComboBox;
	m_pComboBoxItemOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, PARTY_ITEM_COMBO_OPEN );
	m_pComboBoxItemOpen->CreateBaseComboBox ( "MODERN_PARTY_INFO_ITEM_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxItemOpen );

	//m_pComboBoxItemFilterOpen = new CBasicComboBox;
	//m_pComboBoxItemFilterOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, PARTY_ITEMFILTER_COMBO_OPEN );
	//m_pComboBoxItemFilterOpen->CreateBaseComboBox ( "MODERN_PARTY_INFO_ITEM_FILTER_COMBO_OPEN" );			
	//RegisterControl ( m_pComboBoxItemFilterOpen );

	m_pComboBoxMoneyOpen = new CBasicComboBox;
	m_pComboBoxMoneyOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, PARTY_MONEY_COMBO_OPEN );
	m_pComboBoxMoneyOpen->CreateBaseComboBox ( "MODERN_PARTY_INFO_MONEY_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxMoneyOpen );

	m_pComboBoxItemRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxItemRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PARTY_ITEM_COMBO_ROLLOVER );
	m_pComboBoxItemRollOver->CreateBaseComboBoxRollOver ( "MODERN_PARTY_INFO_ITEM_COMBO_ROLLOVER" );			
	m_pComboBoxItemRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxItemRollOver );

	//m_pComboBoxItemFilterRollOver = new CBasicComboBoxRollOver;
	//m_pComboBoxItemFilterRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PARTY_ITEMFILTER_COMBO_ROLLOVER );
	//m_pComboBoxItemFilterRollOver->CreateBaseComboBoxRollOver ( "MODERN_PARTY_INFO_ITEM_FILTER_COMBO_ROLLOVER" );			
	//m_pComboBoxItemFilterRollOver->SetVisibleSingle ( FALSE );
	//RegisterControl ( m_pComboBoxItemFilterRollOver );

	m_pComboBoxMoneyRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxMoneyRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PARTY_MONEY_COMBO_ROLLOVER );
	m_pComboBoxMoneyRollOver->CreateBaseComboBoxRollOver ( "MODERN_PARTY_INFO_MONEY_COMBO_ROLLOVER" );
	m_pComboBoxMoneyRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxMoneyRollOver );

	m_pButtonRelease	= CreateButton( "MODERN_PARTY_INFO_RELEASE_PARTY_BUTTON", ID2GAMEWORD( "MODERN_PARTYWINDOW_BUTTONS", 0 ), PARTY_BUTTON_RELEASE );
	m_pButtonBan		= CreateButton( "MODERN_PARTY_INFO_BAN_BUTTON", ID2GAMEWORD( "MODERN_PARTYWINDOW_BUTTONS", 1 ), PARTY_BUTTON_BAN );
	m_pButtonDelegate	= CreateButton( "MODERN_PARTY_INFO_DELEGATE_BUTTON", ID2GAMEWORD( "MODERN_PARTYWINDOW_BUTTONS", 2 ), PARTY_BUTTON_DELEGATE );
	m_pButtonInfo		= CreateButton( "MODERN_PARTY_INFO_INFO_BUTTON", ID2GAMEWORD( "MODERN_PARTYWINDOW_BUTTONS", 3 ), PARTY_BUTTON_INFO );
	m_pButtonWithdraw	= CreateButton( "MODERN_PARTY_INFO_WITHDRAW_BUTTON", ID2GAMEWORD( "MODERN_PARTYWINDOW_BUTTONS", 4 ), PARTY_BUTTON_WITHDRAW );

	m_pComboBoxItemRollOver->ClearText ();
	for ( int i = 1; i < EMPTYITEMOPT_SIZE; i++ )
	{
		m_pComboBoxItemRollOver->AddText( (char*)ID2GAMEWORD("PARTY_MODAL_ITEM_OPTION", i) );
	}

	m_pComboBoxMoneyRollOver->ClearText ();
	for ( int i = 1; i < EMPTYMONEYOPT_SIZE; i++ )
	{
		m_pComboBoxMoneyRollOver->AddText( (char*)ID2GAMEWORD("PARTY_MODAL_MONEY_OPTION", i) );
	}

	//m_pComboBoxItemFilterRollOver->ClearText ();
	//for ( int i = 0; i < LEVEL_NSIZE; i++ )
	//{
	//	m_pComboBoxItemFilterRollOver->AddText( COMMENT::ITEMLEVEL[i].c_str() );
	//}

	m_pTextNoParty = new CBasicTextBox;
	m_pTextNoParty->CreateSub ( this, "MODERN_PARTY_NONPARTY_TEXT" );
	m_pTextNoParty->SetFont ( pFont );
	m_pTextNoParty->SetTextAlign ( TEXT_ALIGN_CENTER_X );	
	m_pTextNoParty->AddText( ID2GAMEWORD("MODERN_PARTYWINDOW_TEXTS",6) );
	RegisterControl ( m_pTextNoParty );
}

CMultiModeButton* CModernPartyWindowPageParty::CreateButton( char* szControl, const char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, "MODERN_BUTTON_BLUE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, cID );
	pControl->CreateImageBaseMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_BASE_IMAGE_LEFT", "MODERN_BUTTON_BLUE_BASE_IMAGE_CENTER", "MODERN_BUTTON_BLUE_BASE_IMAGE_RIGHT" );
	pControl->CreateImageOverMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_OVER_IMAGE_LEFT", "MODERN_BUTTON_BLUE_OVER_IMAGE_CENTER", "MODERN_BUTTON_BLUE_OVER_IMAGE_RIGHT" );
	pControl->CreateImageHoldMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_CLICK_IMAGE_LEFT", "MODERN_BUTTON_BLUE_CLICK_IMAGE_CENTER", "MODERN_BUTTON_BLUE_CLICK_IMAGE_RIGHT" );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->CreateTextBox( "MODERN_BUTTON_BLUE_TEXT_POS", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->AlignToControl( szControl );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pControl );

	return pControl;
}

void CModernPartyWindowPageParty::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( m_RollOverID != NO_ID )
	{
		ResetMessageEx ();

		CUIControl::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		CUIControl* pControl = m_ControlContainer.FindControl ( m_RollOverID );
		if ( !pControl )
		{
			GASSERT ( 0 && "심각한 오류, 노드가 널입니다." );
			return ;
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
	else
	{
		CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		for ( int i = 0; i < MAXPARTY; i++ ) m_pPartySlot[i]->SetVisibleSingle ( FALSE );

		InitButtons();

		GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();	
		if ( pMaster )
		{
			m_pTextNoParty->SetVisibleSingle( FALSE );
			GLPARTY_CLIENT *pSelf = FindSelfClient ();

			if ( !pSelf ) return ;

			SetSlotMember ( m_pPartySlot[0], pMaster, IsSameMap ( pSelf, pMaster ) );
			m_pPartySlot[0]->SetMaster ( TRUE, 0 );

			DWORD nMEMBER_NUM = GLPartyClient::GetInstance().GetMemberNum();
			if( nMEMBER_NUM > 1 )
			{
				nMEMBER_NUM -= 1; 
				for ( DWORD i = 0; i < nMEMBER_NUM; i++ )
				{
					GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember ( i );			
					if ( pMember )
					{				
						m_pPartySlot[i+1]->SetMaster ( FALSE, i+1 );
						SetSlotMember ( m_pPartySlot[i+1], pMember, IsSameMap ( pSelf, pMember ) );
					}
				}
			}

			{
				const SPARTY_OPT& sPartyOption = GLPartyClient::GetInstance().GetOption ();

				if ( m_sPartyOption != sPartyOption )
				{
					m_sPartyOption = sPartyOption;
					m_pComboBoxItemOpen->SetText ( (char*)ID2GAMEWORD("PARTY_MODAL_ITEM_OPTION", m_sPartyOption.emGET_ITEM) );
					m_pComboBoxMoneyOpen->SetText ( (char*)ID2GAMEWORD("PARTY_MODAL_MONEY_OPTION", m_sPartyOption.emGET_MONEY) );	

					if ( m_pCheckBoxPrivate )
						m_pCheckBoxPrivate->SetFlip( m_sPartyOption.bPrivateParty );
				}
			}
		}	
		else
		{
			m_pTextNoParty->SetVisibleSingle( TRUE );
		}


	}
}

void CModernPartyWindowPageParty::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case PARTY_SLOT_MEMBER0:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if( UIMSG_LB_UP & dwMsg )
				{
					SetSelect( 0 );
				}
			}			
		}
		break;

	case PARTY_SLOT_MEMBER1:
	case PARTY_SLOT_MEMBER2:
	case PARTY_SLOT_MEMBER3:
	case PARTY_SLOT_MEMBER4:
	case PARTY_SLOT_MEMBER5:
	case PARTY_SLOT_MEMBER6:
	case PARTY_SLOT_MEMBER7:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = ControlID - PARTY_SLOT_MEMBER1;

				if( UIMSG_LB_UP & dwMsg )
				{
					SetSelect( nIndex +1 );
				}
			}		
		}
		break;

	case PARTY_ITEM_COMBO_OPEN:
		{
			if ( m_bMASTER )
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{
						m_pComboBoxItemRollOver->SetVisibleSingle ( TRUE );
						m_pComboBoxItemRollOver->SetScrollPercent ( 0.0f );
						m_pComboBoxMoneyRollOver->SetVisibleSingle ( FALSE );	
						//m_pComboBoxItemFilterRollOver->SetVisibleSingle ( FALSE );	

						m_RollOverID = PARTY_ITEM_COMBO_ROLLOVER;
						m_bFirstLBUP = TRUE;
					}
				}
			}
		}
		break;

	case PARTY_ITEM_COMBO_ROLLOVER:
		{
			if ( m_bMASTER )
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
					int nIndex = m_pComboBoxItemRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return ;
					RPROFILE::dwPARTY_GET_ITEM = nIndex+1;
					CheckChanges();
				}
			}
		}
		break;

	/*case PARTY_ITEMFILTER_COMBO_OPEN:
		{
			if ( m_bMASTER )
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{
						m_pComboBoxItemFilterRollOver->SetVisibleSingle ( TRUE );
						m_pComboBoxItemFilterRollOver->SetScrollPercent ( 0.0f );
						m_pComboBoxMoneyRollOver->SetVisibleSingle ( FALSE );	
						m_pComboBoxItemRollOver->SetVisibleSingle ( FALSE );	

						m_RollOverID = PARTY_ITEMFILTER_COMBO_ROLLOVER;
						m_bFirstLBUP = TRUE;
					}
				}
			}
		}
		break;

	case PARTY_ITEMFILTER_COMBO_ROLLOVER:
		{
			if ( m_bMASTER )
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
					int nIndex = m_pComboBoxItemFilterRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return ;
				}
			}
		}
		break;*/

	case PARTY_MONEY_COMBO_OPEN:
		{
			if ( m_bMASTER )
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{
						m_pComboBoxItemRollOver->SetVisibleSingle ( FALSE );
						//m_pComboBoxItemFilterRollOver->SetVisibleSingle ( FALSE );
						m_pComboBoxMoneyRollOver->SetVisibleSingle ( TRUE );
						m_pComboBoxMoneyRollOver->SetScrollPercent ( 0.0f );

						m_RollOverID = PARTY_MONEY_COMBO_ROLLOVER;
						m_bFirstLBUP = TRUE;
					}
				}
			}
		}
		break;

	case PARTY_MONEY_COMBO_ROLLOVER:
		{
			if ( m_bMASTER )
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
					int nIndex = m_pComboBoxMoneyRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return ;
					RPROFILE::dwPARTY_GET_MONEY = nIndex+1;
					CheckChanges();;				
				}
			}
		}
		break;

	case PARTY_CHECKBOX_BUFF:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				RPROFILE::bPartyWindowShowBuff = !RPROFILE::bPartyWindowShowBuff;
				m_pCheckBoxBuff->SetFlip( RPROFILE::bPartyWindowShowBuff );
			}
		}break;

	case PARTY_CHECKBOX_INFO:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				RPROFILE::bPartyWindowShowInfo = !RPROFILE::bPartyWindowShowInfo;
				m_pCheckBoxInfo->SetFlip( RPROFILE::bPartyWindowShowInfo );
			}
		}break;

	case PARTY_CHECKBOX_PRIVATE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				RPROFILE::bPARTY_PRIVATE = !RPROFILE::bPARTY_PRIVATE;

				if ( m_pCheckBoxPrivate )
					m_pCheckBoxPrivate->SetFlip( RPROFILE::bPARTY_PRIVATE );

				CheckChanges();
			}
		}break;

	case PARTY_BUTTON_WITHDRAW:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{	
				if ( m_bMASTER )
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PARTY_QUIT_MASTER") );
					return;
				}
				else
				{
					GLPartyClient::GetInstance().SecedeSelf();
				}	
			}
		}break;

	case PARTY_BUTTON_DELEGATE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_nSelect > 0 && m_nSelect < MAXPARTY )
				{
					if ( m_pPartySlot[m_nSelect] )
					{
						DWORD dwID = m_pPartySlot[m_nSelect]->GetID();
						if ( dwID != GAEAID_NULL )
						{
							GLPartyClient::GetInstance().AuthorityEx( dwID );
						}
					}
				}
				else if ( m_nSelect == 0 )
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PARTY_ALREADY_MASTER") );
					return;
				}
			}
		}break;

	case PARTY_BUTTON_BAN:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_nSelect > 0 && m_nSelect < MAXPARTY )
				{
					if ( m_pPartySlot[m_nSelect] )
					{
						DWORD dwID = m_pPartySlot[m_nSelect]->GetID();
						if ( dwID != GAEAID_NULL )
						{
							GLPartyClient::GetInstance().SecedeEx( dwID );
						}
					}
				}
				else if ( m_nSelect == 0 )
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PARTY_KICK_SELF") );
					return;
				}
			}
		}break;

	case PARTY_BUTTON_RELEASE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLPartyClient::GetInstance().Dissolve();
			}
		}break;

	case PARTY_BUTTON_INFO:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_nSelect >= 0 && m_nSelect < MAXPARTY )
				{
					if ( m_pPartySlot[m_nSelect] )
					{
						DWORD dwID = m_pPartySlot[m_nSelect]->GetID();
						if ( dwID != GAEAID_NULL )
						{
							DWORD dwGaeaID = GLGaeaClient::GetInstance().GetCharacter()->m_dwGaeaID;
							if ( dwGaeaID == dwID )
							{
								CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PARTY_INFO_SELF") );
								return;
							}

							GLGaeaClient::GetInstance().GetCharacter()->RequestCharacterInfoGaea( dwID );
						}
					}
				}
			}
		}break;

	}
}

void CModernPartyWindowPageParty::SetSlotMember ( CModernPartySlot* pPartySlot, GLPARTY_CLIENT *pMember, BOOL bSameMap )
{
	if ( !pPartySlot )
	{
		GASSERT ( 0 && "파티 슬롯이 없습니다. - 메인" );
		return ;
	}
	if ( !pMember )
	{
		GASSERT ( 0 && "멤버가 없습니다. - 메인" );
		return ;
	}

	if ( bSameMap )	
	{
		int nPosX(0), nPosY(0);
		PLANDMANCLIENT pLandClient = GLGaeaClient::GetInstance().GetActiveMap();
		GLMapAxisInfo &sMapAxisInfo = pLandClient->GetMapAxisInfo();
		sMapAxisInfo.Convert2MapPos ( pMember->m_vPos.x, pMember->m_vPos.z, nPosX, nPosY );

		CString strPos;
		strPos.Format ( "%d/%d", nPosX, nPosY );
		pPartySlot->SetPlayerPos ( strPos );

		const CString strMapName = GLGaeaClient::GetInstance().GetMapName ( pMember->m_sMapID );
		pPartySlot->SetMapName ( strMapName );
	}
	else
	{
		const CString strMapName = GLGaeaClient::GetInstance().GetMapName ( pMember->m_sMapID );
		pPartySlot->SetMapName ( strMapName );
		pPartySlot->SetPlayerPos ( "" );
	}


	const int nClassType = CharClassToIndex ( pMember->m_emClass );	
	const CString strName = pMember->m_szName;

	pPartySlot->SetID( pMember->m_dwGaeaID );
	pPartySlot->SetSchool( pMember->m_wSchool );
	pPartySlot->SetClass ( nClassType, bSameMap );
	pPartySlot->SetPlayerName ( strName );	

	bool bDIE = (bSameMap && pMember->m_sHP.wNow == 0);
	pPartySlot->SetDie( bDIE );

	if ( bSameMap )	
	{
		CString strTemp;
		strTemp.Format( "Lv%d", pMember->m_wLevel );
		pPartySlot->SetLevel( strTemp.GetString() );
	}
	else
	{
		pPartySlot->SetLevel("");
	}
	

	pPartySlot->SetVisibleSingle ( TRUE );
}

void CModernPartyWindowPageParty::SetSelect( int nSelect )
{
	if ( nSelect < 0 || nSelect >= MAXPARTY )	return;

	for ( int i = 0; i < MAXPARTY; i++ )
	{
		if ( m_pPartySlot[i] )
			m_pPartySlot[i]->SetSelect( false );
	}

	if ( m_pPartySlot[nSelect] )
		m_pPartySlot[nSelect]->SetSelect( true );

	m_nSelect = nSelect;
}

BOOL CModernPartyWindowPageParty::IsSameMap ( GLPARTY_CLIENT *pSelf, GLPARTY_CLIENT *pMember )
{
	if ( !pSelf || !pMember )
	{
		GASSERT ( 0 && "잘못된 데이타입니다." );
		return FALSE;
	}

	return pSelf->m_sMapID.dwID == pMember->m_sMapID.dwID;
}

GLPARTY_CLIENT*	CModernPartyWindowPageParty::FindSelfClient ()
{
	GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
	if ( pMaster && pMaster->ISONESELF() )
	{
		return pMaster;
	}
	else
	{
		for ( int i = 0; i < 7; i++ )
		{
			GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember( i );
			if ( pMember && pMember->ISONESELF () )
			{
				return pMember;				
			}
		}
	}

	return NULL;
}

void	CModernPartyWindowPageParty::InitButtons()
{
	m_bMASTER = FALSE;
	m_bMEMBER = FALSE;

	GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
	if ( pMaster && pMaster->ISONESELF() )
	{
		m_bMASTER = TRUE;
	}else{
		for ( int i = 0; i < 7; i++ )
		{
			GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember( i );
			if ( pMember && pMember->ISONESELF () )
			{
				m_bMEMBER = TRUE;		
			}
		}
	}

	if ( m_pButtonRelease )		m_pButtonRelease->SetVisibleSingle( m_bMASTER );
	if ( m_pButtonBan )			m_pButtonBan->SetVisibleSingle( m_bMASTER );
	if ( m_pButtonDelegate )	m_pButtonDelegate->SetVisibleSingle( m_bMASTER );
	if ( m_pButtonInfo )		m_pButtonInfo->SetVisibleSingle( m_bMASTER || m_bMEMBER );
	if ( m_pButtonWithdraw )	m_pButtonWithdraw->SetVisibleSingle( m_bMASTER || m_bMEMBER );
}

void CModernPartyWindowPageParty::SetPartyInfo ( DWORD dwPartyID, DWORD dwMasterID )
{
	m_dwPartyID = dwPartyID;
	m_dwMasterID = dwMasterID;
}

DWORD CModernPartyWindowPageParty::GetPartyID ()
{
	return m_dwPartyID;
}

DWORD CModernPartyWindowPageParty::GetMasterID()
{
	return m_dwMasterID;
}

void CModernPartyWindowPageParty::CheckChanges()
{
	SPARTY_OPT sOPT  = GLPartyClient::GetInstance().GetOption();
	SPARTY_OPT sOption;
	sOption.emGET_ITEM = (EMPARTY_ITEM_OPT)RPROFILE::dwPARTY_GET_ITEM;
	sOption.emGET_MONEY = (EMPARTY_MONEY_OPT)RPROFILE::dwPARTY_GET_MONEY;
	sOption.bPrivateParty = RPROFILE::bPARTY_PRIVATE;

	if ( sOPT != sOption )
	{
		GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
		if ( pMaster && pMaster->ISONESELF() )
		{
			GLMSG::SNET_PARTY_CHANGEOPTION NetMsg;
			NetMsg.dwGaeaID = pMaster->m_dwGaeaID;
			NetMsg.sOption = sOption;
			NetMsg.dwPartyID = GLPartyClient::GetInstance().GetPartyID();
			NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
		}
	}
}