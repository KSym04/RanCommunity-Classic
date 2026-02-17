#include "StdAfx.h"
#include "./ItemGarbage.h"

#include "./ItemImage.h"
#include "./BasicTextButton.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./GameTextControl.h"
#include "../../Lib_Engine/DxCommon/D3DFont.h"
#include "./ItemMove.h"
#include "./InnerInterface.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_ClientUI/Interface/ModalWindow.h"
#include "../../Lib_ClientUI/Interface/ModalCallerID.h"
#include "./UITextControl.h"
#include "./BasicTextBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------
// CItemGarbage
//--------------------------------------------------------------------
CItemGarbage::CItemGarbage()	// 휴지통
	: m_pOkButton(NULL)
	, m_pCancelButton(NULL)
{
	for ( int i=0; i<ITEM_GARBAGE_MAX_SLOT; ++i )
		m_pItemImage[i] = NULL;
}

CItemGarbage::~CItemGarbage()
{
}

VOID CItemGarbage::CreateSubControl()
{
	std::string strSlot[ITEM_GARBAGE_MAX_SLOT] = 
	{
		"ITEM_GARBAGE_ITEM_SLOT_00",
		"ITEM_GARBAGE_ITEM_SLOT_01",
		"ITEM_GARBAGE_ITEM_SLOT_02",
		"ITEM_GARBAGE_ITEM_SLOT_03",
		"ITEM_GARBAGE_ITEM_SLOT_04",
	};

	for ( int i=0; i<ITEM_GARBAGE_MAX_SLOT; ++i )
	{
		m_pItemImage[i] = new CItemImage;
		m_pItemImage[i]->CreateSub( this, strSlot[i].c_str(), UI_FLAG_DEFAULT, ITEM_GARBAGE_ITEM_SLOT_00 + i );
		m_pItemImage[i]->CreateSubControl();
		RegisterControl( m_pItemImage[i] );
	}
	

	{
		CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
		CBasicTextBox* pTextBox = NULL;
		pTextBox = CreateStaticControl( "ITEM_GARBAGE_INFO_TEXT_STATIC", pFont, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
		pTextBox->AddText( ID2GAMEINTEXT( "ITEM_GARBAGE_INFO" ), NS_UITEXTCOLOR::WHITE );
	}

	// 승인, 거절
	{
		m_pOkButton = new CBasicTextButton;
		m_pOkButton->CreateSub( this, "BASIC_TEXT_BUTTON22", UI_FLAG_DEFAULT, ITEM_GARBAGE_OK_BUTTON );
		m_pOkButton->CreateBaseButton( "ITEM_GARBAGE_OK_BUTTON", CBasicTextButton::SIZE22,
									CBasicButton::CLICK_FLIP, ID2GAMEWORD( "ITEM_GARBAGE_BUTTON", 0 ) );
		m_pOkButton->SetShortcutKey( DIK_RETURN, DIK_NUMPADENTER );
		RegisterControl( m_pOkButton );

		m_pCancelButton = new CBasicTextButton;
		m_pCancelButton->CreateSub( this, "BASIC_TEXT_BUTTON22", UI_FLAG_DEFAULT, ITEM_GARBAGE_CANCEL_BUTTON );
		m_pCancelButton->CreateBaseButton( "ITEM_GARBAGE_CANCEL_BUTTON", CBasicTextButton::SIZE22,
									CBasicButton::CLICK_FLIP, ID2GAMEWORD( "ITEM_GARBAGE_BUTTON", 1 ) );
		m_pCancelButton->SetShortcutKey( DIK_ESCAPE );
		RegisterControl( m_pCancelButton );
	}
}

VOID CItemGarbage::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	for ( int i=0; i<ITEM_GARBAGE_MAX_SLOT; ++i )
	{
		SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_GARBAGE_ITEM(i);

		if( m_pItemImage[i] )
		{
			if( sItemCustom.sNativeID == NATIVEID_NULL() )
			{
				m_pItemImage[i]->ResetItem();
			}
			else
			{
				SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
				if( pItem )
					m_pItemImage[i]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
				else
					m_pItemImage[i]->ResetItem();
			}
		}
	}
	
}

VOID CItemGarbage::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case ITEM_GARBAGE_ITEM_SLOT_00:
	case ITEM_GARBAGE_ITEM_SLOT_01:
	case ITEM_GARBAGE_ITEM_SLOT_02:
	case ITEM_GARBAGE_ITEM_SLOT_03:
	case ITEM_GARBAGE_ITEM_SLOT_04:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = ControlID - ITEM_GARBAGE_ITEM_SLOT_00;

				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
				if( !pItemMove )
				{
					GASSERT( 0 && "CItemGarbage::TranslateUIMessage, pItemMove == NULL" );
					break;
				}

				SNATIVEID sItem = pItemMove->GetItem();
				if( sItem != NATIVEID_NULL() )
				{
					const UIRECT& rcSlotPos = m_pItemImage[nIndex]->GetGlobalPos();
					pItemMove->SetGlobalPos( rcSlotPos );
					CInnerInterface::GetInstance().SetSnapItem();
				}

				if( dwMsg & UIMSG_LB_UP )
					GLGaeaClient::GetInstance().GetCharacter()->ReqGarbageMoveItem(nIndex);

				if( dwMsg & UIMSG_RB_UP )
					GLGaeaClient::GetInstance().GetCharacter()->ReSetGarbageItem(nIndex);

				SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_GARBAGE_ITEM(nIndex);
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

	case ITEM_GARBAGE_OK_BUTTON:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				BOOL bValid = FALSE;
				for ( int i=0; i<ITEM_GARBAGE_MAX_SLOT; ++i )
				{
					SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_GARBAGE_ITEM(i);
					if ( sItemCustom.sNativeID != NATIVEID_NULL() )	
					{
						bValid = TRUE;
						break;
					}
				}
				
				if ( !bValid )
					break;

				DoModal( ID2GAMEINTEXT("ITEM_GARBAGE_OKQUESTION"), MODAL_QUESTION, YESNO, MODAL_ITEMGARBAGE_OKQUESTION );
				SetMessageEx( dwMsg &= ~UIMSG_KEY_FOCUSED ); // Focus 문제로 메세지 삭제
			}
		}break;

	case ITEM_GARBAGE_CANCEL_BUTTON:
		if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			CInnerInterface::GetInstance().CloseItemGarbageWindow();
		break;

	case ET_CONTROL_BUTTON:
		if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			CInnerInterface::GetInstance().CloseItemGarbageWindow();
		break;
	}

	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );
}