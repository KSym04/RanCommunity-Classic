#include "StdAfx.h"
#include "./ItemRandomOptionRebuildWindow.h"

#include "./ItemImage.h"
#include "./BasicTextBox.h"
#include "./BasicLineBox.h"
#include "./BasicTextButton.h"
#include "./BasicButton.h"

#include "../../Lib_Engine/DxCommon/D3DFont.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "./InnerInterface.h"
#include "./ItemMove.h"

#include "./ModalWindow.h"
#include "./ModalCallerID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CItemRandomOptionRebuildWindow::CItemRandomOptionRebuildWindow()
	: m_pItemImageTarget(NULL)
	, m_pItemImageStamp(NULL)
	, m_pHelpTextBox(NULL)

	, m_pButtonStart(NULL)
	, m_pButtonCancel(NULL)

	, m_pOptionCheck0(NULL)
	, m_pOptionCheck1(NULL)
	, m_pOptionCheck2(NULL)
	, m_pOptionCheck3(NULL)

	, m_pOptionText0(NULL)
	, m_pOptionText1(NULL)
	, m_pOptionText2(NULL)
	, m_pOptionText3(NULL)
	, m_last_valid_check(true)
{
}

CItemRandomOptionRebuildWindow::~CItemRandomOptionRebuildWindow()
{
}


void CItemRandomOptionRebuildWindow::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG  );
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "ITEM_RANDOM_OPTION_REBUILD_HELP_REGION" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "ITEM_RANDOM_OPTION_REBUILD_OPTION_REGION_BOX" );
	RegisterControl ( pBasicLineBox );

	m_pItemImageTarget = new CItemImage;
	m_pItemImageTarget->CreateSub( this, "ITEM_RANDOM_OPTION_REBUILD_ITEM_SLOT_TARGET", UI_FLAG_DEFAULT, ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_IMAGE_TARGET );
	m_pItemImageTarget->CreateSubControl();
	RegisterControl( m_pItemImageTarget );

	m_pItemImageStamp = new CItemImage;
	m_pItemImageStamp->CreateSub( this, "ITEM_RANDOM_OPTION_REBUILD_ITEM_SLOT_STAMP", UI_FLAG_DEFAULT, ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_IMAGE_STAMP );
	m_pItemImageStamp->CreateSubControl();
	RegisterControl( m_pItemImageStamp );

	m_pHelpTextBox = new CBasicTextBox;
	m_pHelpTextBox->CreateSub ( this, "ITEM_RANDOM_OPTION_REBUILD_TEXT_BOX" );
	m_pHelpTextBox->SetFont ( pFont9 );
	m_pHelpTextBox->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pHelpTextBox );
	

	m_pOptionCheck0 = new CBasicButton;
	m_pOptionCheck0->CreateSub ( this, "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_BOX_0", UI_FLAG_DEFAULT, ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_0 );
	m_pOptionCheck0->CreateFlip ( "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_F", CBasicButton::RADIO_FLIP );
	m_pOptionCheck0->SetControlNameEx ( "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_BOX_0" );
	RegisterControl ( m_pOptionCheck0 );

	m_pOptionCheck1 = new CBasicButton;
	m_pOptionCheck1->CreateSub ( this, "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_BOX_1", UI_FLAG_DEFAULT, ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_1 );
	m_pOptionCheck1->CreateFlip ( "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_F", CBasicButton::RADIO_FLIP );
	m_pOptionCheck1->SetControlNameEx ( "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_BOX_1" );
	RegisterControl ( m_pOptionCheck1 );

	m_pOptionCheck2 = new CBasicButton;
	m_pOptionCheck2->CreateSub ( this, "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_BOX_2", UI_FLAG_DEFAULT, ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_2 );
	m_pOptionCheck2->CreateFlip ( "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_F", CBasicButton::RADIO_FLIP );
	m_pOptionCheck2->SetControlNameEx ( "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_BOX_2" );
	RegisterControl ( m_pOptionCheck2 );

	m_pOptionCheck3 = new CBasicButton;
	m_pOptionCheck3->CreateSub ( this, "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_BOX_3", UI_FLAG_DEFAULT, ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_3 );
	m_pOptionCheck3->CreateFlip ( "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_F", CBasicButton::RADIO_FLIP );
	m_pOptionCheck3->SetControlNameEx ( "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_CHECK_BOX_3" );
	RegisterControl ( m_pOptionCheck3 );


	m_pOptionText0 = new CBasicTextBox;
	m_pOptionText0->CreateSub ( this, "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_TEXT_BOX_0" );
	m_pOptionText0->SetFont ( pFont9 );
	m_pOptionText0->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pOptionText0 );

	m_pOptionText1 = new CBasicTextBox;
	m_pOptionText1->CreateSub ( this, "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_TEXT_BOX_1" );
	m_pOptionText1->SetFont ( pFont9 );
	m_pOptionText1->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pOptionText1 );

	m_pOptionText2 = new CBasicTextBox;
	m_pOptionText2->CreateSub ( this, "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_TEXT_BOX_2" );
	m_pOptionText2->SetFont ( pFont9 );
	m_pOptionText2->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pOptionText2 );

	m_pOptionText3 = new CBasicTextBox;
	m_pOptionText3->CreateSub ( this, "ITEM_REBUILD_RENEWAL_RANDOM_OPTION_TEXT_BOX_3" );
	m_pOptionText3->SetFont ( pFont9 );
	m_pOptionText3->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pOptionText3 );


	m_pButtonStart = new CBasicTextButton;
	m_pButtonStart->CreateSub( this, "BASIC_TEXT_BUTTON16", UI_FLAG_DEFAULT, ITEM_RANDOM_OPTION_REBUILD_WINDOW_BUTTON_START );
	m_pButtonStart->CreateBaseButton( "ITEM_RANDOM_OPTION_REBUILD_OK_BUTTON", CBasicTextButton::SIZE16, CBasicButton::CLICK_FLIP, ID2GAMEWORD( "ITEM_RANDOM_OPTION_REBUILD_WINDOW", 2 ) );
	RegisterControl( m_pButtonStart );

	m_pButtonCancel = new CBasicTextButton;
	m_pButtonCancel->CreateSub( this, "BASIC_TEXT_BUTTON16", UI_FLAG_DEFAULT, ITEM_RANDOM_OPTION_REBUILD_WINDOW_BUTTON_CANCEL );
	m_pButtonCancel->CreateBaseButton( "ITEM_RANDOM_OPTION_REBUILD_CANCEL_BUTTON", CBasicTextButton::SIZE16, CBasicButton::CLICK_FLIP, ID2GAMEWORD( "ITEM_RANDOM_OPTION_REBUILD_WINDOW", 3 ) );
	RegisterControl( m_pButtonCancel );
}

void CItemRandomOptionRebuildWindow::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )	return;

	if ( m_pItemImageTarget )
	{
		const SITEMCUSTOM sitemcustom = pCharacter->GetItemRandomOptionRebuildItem();

		if( sitemcustom.sNativeID == NATIVEID_NULL() ){
			m_pItemImageTarget->ResetItem();
		}else{
			SITEM* pItem = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
			if( pItem )
				m_pItemImageTarget->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
			else
				m_pItemImageTarget->ResetItem();
		}
	}

	if ( m_pItemImageStamp )
	{
		const SITEMCUSTOM sitemcustom = pCharacter->GetItemRandomOptionRebuildStamp();

		if( sitemcustom.sNativeID == NATIVEID_NULL() ){
			m_pItemImageStamp->ResetItem();
		}else{
			SITEM* pItem = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
			if( pItem )
				m_pItemImageStamp->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
			else
				m_pItemImageStamp->ResetItem(); 
		}
	}

	const SITEMCUSTOM sitemcustom_target = pCharacter->GetItemRandomOptionRebuildItem();
	const SITEMCUSTOM sitemcustom_stamp = pCharacter->GetItemRandomOptionRebuildStamp();

	SITEM* pitem_target = GLItemMan::GetInstance().GetItem( sitemcustom_target.sNativeID );
	SITEM* pitem_stamp = GLItemMan::GetInstance().GetItem( sitemcustom_stamp.sNativeID );

	if ( m_pHelpTextBox )
	{
		m_pHelpTextBox->ClearText();

		CString text;

		if ( !pitem_target && !pitem_stamp )
		{
			m_pHelpTextBox->AddText( ID2GAMEINTEXT( "ITEM_RANDOM_OPTION_REBUILD_USE_REBUILD" ), NS_UITEXTCOLOR::WHITE );
		}
		else if ( pitem_target && !pitem_stamp )
		{
			m_pHelpTextBox->AddText( ID2GAMEINTEXT( "ITEM_RANDOM_OPTION_REBUILD_USE_ITEM" ), NS_UITEXTCOLOR::WHITE );
		}
		else if ( !pitem_target && pitem_stamp )
		{
			m_pHelpTextBox->AddText( ID2GAMEINTEXT( "ITEM_RANDOM_OPTION_REBUILD_NO_ITEM" ), NS_UITEXTCOLOR::WHITE );
		}
		else if ( pitem_target && pitem_stamp )
		{
			text.Format( ID2GAMEINTEXT( "ITEM_RANDOM_OPTION_REBUILD_USE_ONLY_STAMP_CARD_NO_LIMIT" ), pitem_stamp->sSuitOp.wReModelNum );
			m_pHelpTextBox->AddText( text.GetString(), NS_UITEXTCOLOR::WHITE );
		}
	}

	if ( m_pOptionCheck0 )	m_pOptionCheck0->SetVisibleSingle( FALSE );
	if ( m_pOptionCheck1 )	m_pOptionCheck1->SetVisibleSingle( FALSE );
	if ( m_pOptionCheck2 )	m_pOptionCheck2->SetVisibleSingle( FALSE );
	if ( m_pOptionCheck3 )	m_pOptionCheck3->SetVisibleSingle( FALSE );

	if ( m_pOptionText0 )	m_pOptionText0->ClearText();
	if ( m_pOptionText1 )	m_pOptionText1->ClearText();
	if ( m_pOptionText2 )	m_pOptionText2->ClearText();
	if ( m_pOptionText3 )	m_pOptionText3->ClearText();

	if ( m_pOptionText0 )	m_pOptionText0->SetVisibleSingle( FALSE );
	if ( m_pOptionText1 )	m_pOptionText1->SetVisibleSingle( FALSE );
	if ( m_pOptionText2 )	m_pOptionText2->SetVisibleSingle( FALSE );
	if ( m_pOptionText3 )	m_pOptionText3->SetVisibleSingle( FALSE );

	if ( pitem_target && sitemcustom_target.IsSetRandOpt() )
	{
		CString strDescText;

		int nRandOptType = sitemcustom_target.GETOptTYPE1();
		float fRandomOptVal = sitemcustom_target.GETOptVALUE1();
		if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) && ( fRandomOptVal != 0.0f ) )
		{
			strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );
			if( fRandomOptVal > 0.0f) strDescText += _T('+');
			strDescText.AppendFormat( "%.2f", fRandomOptVal );
			if( sitemcustom_target.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

			if ( m_pOptionText0 )
			{
				m_pOptionText0->SetVisibleSingle( TRUE );
				m_pOptionText0->AddText( strDescText, NS_UITEXTCOLOR::TURQUOISE );
			}

			if ( m_pOptionCheck0 && pitem_stamp )
			{
				if ( !pCharacter->m_bItemRandomOptionRebuildOptionLock0 && pCharacter->GetItemRandomOptionRebuildLockNum() >= pitem_stamp->sSuitOp.wReModelNum )
				{
					m_pOptionCheck0->SetVisibleSingle( FALSE );
					m_pOptionCheck0->SetFlip( FALSE );
				}
				else
				{
					m_pOptionCheck0->SetVisibleSingle( TRUE );
					m_pOptionCheck0->SetFlip( pCharacter->m_bItemRandomOptionRebuildOptionLock0 );
				}
			}
		}
		
		nRandOptType = sitemcustom_target.GETOptTYPE2();
		fRandomOptVal = sitemcustom_target.GETOptVALUE2();
		if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) && ( fRandomOptVal != 0.0f ) )
		{
			strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );
			if( fRandomOptVal > 0.0f) strDescText += _T('+');
			strDescText.AppendFormat( "%.2f", fRandomOptVal );
			if( sitemcustom_target.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

			if ( m_pOptionText1 )
			{
				m_pOptionText1->SetVisibleSingle( TRUE );
				m_pOptionText1->AddText( strDescText, NS_UITEXTCOLOR::TURQUOISE );
			}

			if ( m_pOptionCheck1 && pitem_stamp )
			{
				if ( !pCharacter->m_bItemRandomOptionRebuildOptionLock1 && pCharacter->GetItemRandomOptionRebuildLockNum() >= pitem_stamp->sSuitOp.wReModelNum )
				{
					m_pOptionCheck1->SetVisibleSingle( FALSE );
					m_pOptionCheck1->SetFlip( FALSE );
				}
				else
				{
					m_pOptionCheck1->SetVisibleSingle( TRUE );
					m_pOptionCheck1->SetFlip( pCharacter->m_bItemRandomOptionRebuildOptionLock1 );
				}
			}
		}
		
		nRandOptType = sitemcustom_target.GETOptTYPE3();
		fRandomOptVal = sitemcustom_target.GETOptVALUE3();
		if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) && ( fRandomOptVal != 0.0f ) )
		{
			strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );
			if( fRandomOptVal > 0.0f) strDescText += _T('+');
			strDescText.AppendFormat( "%.2f", fRandomOptVal );
			if( sitemcustom_target.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

			if ( m_pOptionText2 )
			{
				m_pOptionText2->SetVisibleSingle( TRUE );
				m_pOptionText2->AddText( strDescText, NS_UITEXTCOLOR::TURQUOISE );
			}

			if ( m_pOptionCheck2 && pitem_stamp )
			{
				if ( !pCharacter->m_bItemRandomOptionRebuildOptionLock2 && pCharacter->GetItemRandomOptionRebuildLockNum() >= pitem_stamp->sSuitOp.wReModelNum )
				{
					m_pOptionCheck2->SetVisibleSingle( FALSE );
					m_pOptionCheck2->SetFlip( FALSE );
				}
				else
				{
					m_pOptionCheck2->SetVisibleSingle( TRUE );
					m_pOptionCheck2->SetFlip( pCharacter->m_bItemRandomOptionRebuildOptionLock2 );
				}
			}
		}
		
		nRandOptType = sitemcustom_target.GETOptTYPE4();
		fRandomOptVal = sitemcustom_target.GETOptVALUE4();
		if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) && ( fRandomOptVal != 0.0f ) )
		{
			strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );
			if( fRandomOptVal > 0.0f) strDescText += _T('+');
			strDescText.AppendFormat( "%.2f", fRandomOptVal );
			if( sitemcustom_target.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

			if ( m_pOptionText3 )
			{
				m_pOptionText3->SetVisibleSingle( TRUE );
				m_pOptionText3->AddText( strDescText, NS_UITEXTCOLOR::TURQUOISE );
			}

			if ( m_pOptionCheck3 && pitem_stamp )
			{
				if ( !pCharacter->m_bItemRandomOptionRebuildOptionLock3 && pCharacter->GetItemRandomOptionRebuildLockNum() >= pitem_stamp->sSuitOp.wReModelNum )
				{
					m_pOptionCheck3->SetVisibleSingle( FALSE );
					m_pOptionCheck3->SetFlip( FALSE );
				}
				else
				{
					m_pOptionCheck3->SetVisibleSingle( TRUE );
					m_pOptionCheck3->SetFlip( pCharacter->m_bItemRandomOptionRebuildOptionLock3 );
				}
			}
		}
	
		

		
	}

	bool valid_check = pCharacter->item_random_option_rebuild_task_check();
	if ( m_last_valid_check != valid_check )
	{
		m_last_valid_check = valid_check;

		if ( m_pButtonStart )
			m_pButtonStart->SetVisibleSingle( m_last_valid_check );

		if ( m_pButtonCancel )
			m_pButtonCancel->SetVisibleSingle( m_last_valid_check );
	}

}

void CItemRandomOptionRebuildWindow::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_IMAGE_TARGET:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
				if( !pItemMove )	break;

				SNATIVEID sItem = pItemMove->GetItem();
				if( sItem != NATIVEID_NULL() )
				{
					const UIRECT& rcSlotPos = m_pItemImageTarget->GetGlobalPos();
					pItemMove->SetGlobalPos( rcSlotPos );
					CInnerInterface::GetInstance().SetSnapItem();
				}

				if( dwMsg & UIMSG_LB_UP )
				{
					GLGaeaClient::GetInstance().GetCharacter()->SetItemRandomOptionRebuildMoveItem();
				}

				if( dwMsg & UIMSG_RB_UP )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReSetItemRandomOptionRebuildItem();
				}

				SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GetItemRandomOptionRebuildItem();
				if( sItemCustom.sNativeID != NATIVEID_NULL() )
					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sItemCustom, FALSE, FALSE, FALSE, 0, 0 );

				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sItemCustom );
						return;
					}
				}

				/*box contents, Juver, 2017/08/30 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().ShowBoxContents( sItemCustom.sNativeID );
						return;
					}
				}

				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().ReqMaxRVShow( sItemCustom );
						return;
					}
				}
			}
		}break;

	case ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_IMAGE_STAMP:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
				if( !pItemMove )	break;

				SNATIVEID sItem = pItemMove->GetItem();
				if( sItem != NATIVEID_NULL() )
				{
					const UIRECT& rcSlotPos = m_pItemImageStamp->GetGlobalPos();
					pItemMove->SetGlobalPos( rcSlotPos );
					CInnerInterface::GetInstance().SetSnapItem();
				}

				if( dwMsg & UIMSG_LB_UP )
				{
					GLGaeaClient::GetInstance().GetCharacter()->SetItemRandomOptionRebuildMoveStamp();
				}

				if( dwMsg & UIMSG_RB_UP )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReSetItemRandomOptionRebuildStamp();
				}
			}
		}break;

	case ITEM_RANDOM_OPTION_REBUILD_WINDOW_BUTTON_CANCEL:
		if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			CInnerInterface::GetInstance().CloseItemRandomOptionRebuildWindow();
		break;

	case ET_CONTROL_BUTTON:
		if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			CInnerInterface::GetInstance().CloseItemRandomOptionRebuildWindow();
		break;

	case ITEM_RANDOM_OPTION_REBUILD_WINDOW_BUTTON_START:
		if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			DoModal( ID2GAMEINTEXT("ITEM_RANDOM_OPTION_REBUILD_START_QUESTION"), MODAL_INFOMATION, YESNO, ITEM_RANDOM_OPTION_REBUILD_START_QUESTION );
		break;

	case ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_0:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				BOOL b = GLGaeaClient::GetInstance().GetCharacter()->m_bItemRandomOptionRebuildOptionLock0;
				GLGaeaClient::GetInstance().GetCharacter()->SetRandomOptionRebuildLock(0, !b );
			}
		}break;

	case ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_1:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				BOOL b = GLGaeaClient::GetInstance().GetCharacter()->m_bItemRandomOptionRebuildOptionLock1;
				GLGaeaClient::GetInstance().GetCharacter()->SetRandomOptionRebuildLock(1, !b );
			}
		}break;

	case ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_2:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				BOOL b = GLGaeaClient::GetInstance().GetCharacter()->m_bItemRandomOptionRebuildOptionLock2;
				GLGaeaClient::GetInstance().GetCharacter()->SetRandomOptionRebuildLock(2, !b );
			}
		}break;

	case ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_3:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				BOOL b = GLGaeaClient::GetInstance().GetCharacter()->m_bItemRandomOptionRebuildOptionLock3;
				GLGaeaClient::GetInstance().GetCharacter()->SetRandomOptionRebuildLock(3, !b );
			}
		}break;
		
	}
	
	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );
}

void CItemRandomOptionRebuildWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );
}

