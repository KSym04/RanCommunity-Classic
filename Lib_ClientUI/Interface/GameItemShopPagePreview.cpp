#include "stdafx.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "./BasicLineBoxEx.h"
#include "./BasicButton.h"
#include "./BasicTextBox.h"
#include "./ItemImage.h"

#include "./GameItemShopPageRender.h"
#include "./GameItemShopPagePreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGameItemShopPagePreview::CGameItemShopPagePreview () 
	: m_pMouseOver(NULL)
	, m_pRender(NULL)
	, m_pButtonLeft(NULL)
	, m_pButtonRight(NULL)

	, m_pTextMale(NULL)
	, m_pButtonMale(NULL)
	, m_pTextFemale(NULL)
	, m_pButtonFemale(NULL)
{
	for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		m_pItemImageBox[i] = NULL;
		m_sItemIDBox[i] = NATIVEID_NULL();
	}
}

CGameItemShopPagePreview::~CGameItemShopPagePreview ()
{
}

void CGameItemShopPagePreview::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pRender = new CGameItemShopPageRender;
	m_pRender->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_RENDER_LINE", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PAGE_RENDER_PREVIEW );
	m_pRender->CreateSubControl ();
	m_pRender->SetVisibleSingle( TRUE );
	RegisterControl ( m_pRender );

	std::string strItemImage[ITEM::SBOX::ITEM_SIZE] = 
	{
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_00",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_01",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_02",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_03",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_04",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_05",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_06",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_07",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_08",
		"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_09",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_10",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_11",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_12",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_13",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_14",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_15",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_16",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_17",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_18",
		//"GAME_ITEM_SHOP_PAGE_RENDER_ITEM_19",
	};

	for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		m_pItemImageBox[i] = new CItemImage;
		m_pItemImageBox[i]->CreateSub ( this, strItemImage[i].c_str(), UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_0 + i );
		m_pItemImageBox[i]->CreateSubControl ();
		m_pItemImageBox[i]->CreateTextBoxDownRight();
		RegisterControl ( m_pItemImageBox[i] );
	}
	
	m_pMouseOver = new CUIControl;
	m_pMouseOver->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_RENDER_ITEM_MOUSE_OVER" );
	RegisterControl ( m_pMouseOver );


	m_pTextMale = new CBasicTextBox;
	m_pTextMale->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_RENDER_TEXT_MALE" );
	m_pTextMale->SetFont ( pFont9 );
	m_pTextMale->SetText( ID2GAMEWORD( "GAME_ITEM_SHOP_WINDOW_PAGE_GENDER", 0 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextMale );

	m_pTextFemale = new CBasicTextBox;
	m_pTextFemale->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_RENDER_TEXT_FEMALE" );
	m_pTextFemale->SetFont ( pFont9 );
	m_pTextFemale->SetText( ID2GAMEWORD( "GAME_ITEM_SHOP_WINDOW_PAGE_GENDER", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextFemale );

	m_pButtonMale = new CBasicButton;
	m_pButtonMale->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_MALE", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_MALE );
	m_pButtonMale->CreateFlip ( "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_ON_MALE", CBasicButton::RADIO_FLIP );
	m_pButtonMale->SetControlNameEx ( "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_MALE" );
	m_pButtonMale->SetFlip( TRUE );
	RegisterControl ( m_pButtonMale );

	m_pButtonFemale = new CBasicButton;
	m_pButtonFemale->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_FEMALE", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_FEMALE );
	m_pButtonFemale->CreateFlip ( "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_ON_FEMALE", CBasicButton::RADIO_FLIP );
	m_pButtonFemale->SetControlNameEx ( "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_FEMALE" );
	m_pButtonFemale->SetFlip( FALSE );
	RegisterControl ( m_pButtonFemale );


	m_pButtonLeft = new CBasicButton;
	m_pButtonLeft->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_LEFT_DEFAULT", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PAGE_RENDER_ROTATE_LEFT );
	m_pButtonLeft->CreateFlip ( "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_LEFT_CLICK", CBasicButton::CLICK_FLIP );
	m_pButtonLeft->CreateMouseOver ( "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_LEFT_OVER" );
	m_pButtonLeft->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonLeft );

	m_pButtonRight = new CBasicButton;
	m_pButtonRight->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_RIGHT_DEFAULT", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PAGE_RENDER_ROTATE_RIGHT );
	m_pButtonRight->CreateFlip ( "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_RIGHT_CLICK", CBasicButton::CLICK_FLIP );
	m_pButtonRight->CreateMouseOver ( "GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_RIGHT_OVER" );
	m_pButtonRight->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonRight );

}

void CGameItemShopPagePreview::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	if ( m_pMouseOver ) 
		m_pMouseOver->SetVisibleSingle ( FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CGameItemShopPagePreview::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_0:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_1:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_2:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_3:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_4:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_5:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_6:

	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_7:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_8:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_9:
	/*case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_10:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_11:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_12:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_13:

	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_14:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_15:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_16:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_17:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_18:
	case GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_19:*/
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = ControlID - GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_0;

				if ( nIndex < ITEM::SBOX::ITEM_SIZE )
				{
					if( m_sItemIDBox[nIndex] != NATIVEID_NULL() )
					{
						SITEMCUSTOM sitemcustom;
						sitemcustom.sNativeID = m_sItemIDBox[nIndex];

						CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE );

						/*item preview, Juver, 2017/07/27 */
						if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
						{
							if ( dwMsg & UIMSG_LB_UP )
							{
								CInnerInterface::GetInstance().PreviewItem( sitemcustom );
								return;
							}
						}

						/*box contents, Juver, 2017/08/30 */
						if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
						{
							if ( dwMsg & UIMSG_RB_UP )
							{
								CInnerInterface::GetInstance().ShowBoxContents( sitemcustom.sNativeID );
								return;
							}
						}

						if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
						{
							if ( dwMsg & UIMSG_LB_UP )
							{
								CInnerInterface::GetInstance().ReqMaxRVShow( sitemcustom );
								return;
							}
						}

						if ( m_pMouseOver && m_pItemImageBox[nIndex] )
						{
							const UIRECT& rcImagePos = m_pItemImageBox[nIndex]->GetGlobalPos();
							m_pMouseOver->SetGlobalPos ( rcImagePos );
							m_pMouseOver->SetVisibleSingle ( TRUE );
						}
					}
				}
			}
		}break;

	case GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_MALE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_pButtonMale )
					m_pButtonMale->SetFlip( TRUE );

				if ( m_pButtonFemale )
					m_pButtonFemale->SetFlip( FALSE );

				if ( m_pRender )
				{
					m_pRender->m_bMale = TRUE;
					m_pRender->ClearRender();
					m_pRender->m_bReady = TRUE;
				}
			}
		}break;

	case GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_FEMALE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_pButtonMale )
					m_pButtonMale->SetFlip( FALSE );

				if ( m_pButtonFemale )
					m_pButtonFemale->SetFlip( TRUE );

				if ( m_pRender )
				{
					m_pRender->m_bMale = FALSE;
					m_pRender->ClearRender();
					m_pRender->m_bReady = TRUE;
				}
			}
		}break;
		
	case GAME_ITEM_SHOP_PAGE_RENDER_ROTATE_LEFT:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				if ( m_pRender )	
					m_pRender->m_vRot.x -=0.1f;
			}
		}break;

	case GAME_ITEM_SHOP_PAGE_RENDER_ROTATE_RIGHT:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				if ( m_pRender )	
					m_pRender->m_vRot.x +=0.1f;

			}
		}break;
	};
}

void CGameItemShopPagePreview::DefaultPreview()
{
	if ( m_pRender )
		m_pRender->DefaultPreview();
}

void CGameItemShopPagePreview::DataReset()
{
	for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		m_sItemIDBox[i] = NATIVEID_NULL();

		if ( m_pItemImageBox[i] )	
		{
			m_pItemImageBox[i]->ResetItem();
			m_pItemImageBox[i]->SetTextDownRight( "", NS_UITEXTCOLOR::ORANGE );
		}
	}

	if ( m_pRender )
		m_pRender->ClearRender();


	if ( m_pButtonLeft )
		m_pButtonLeft->SetVisibleSingle( FALSE );

	if ( m_pButtonRight )
		m_pButtonRight->SetVisibleSingle( FALSE );

	if ( m_pTextMale )
		m_pTextMale->SetVisibleSingle( FALSE );

	if ( m_pButtonMale )
		m_pButtonMale->SetVisibleSingle( FALSE );

	if ( m_pTextFemale )
		m_pTextFemale->SetVisibleSingle( FALSE );

	if ( m_pButtonFemale )
		m_pButtonFemale->SetVisibleSingle( FALSE );
}

void CGameItemShopPagePreview::DataShow( SNATIVEID sItemID )
{
	DataReset();

	SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemID );
	if ( !pItem )	
		return;

	if ( pItem->sBasicOp.emItemType == ITEM_BOX || 
		pItem->sBasicOp.emItemType == ITEM_PREMIUMSET ||
		pItem->sBasicOp.emItemType == ITEM_BOX_SPECIFIC )
	{
		for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
		{
			m_sItemIDBox[i] = pItem->sBox.sITEMS[i].nidITEM;
			DWORD dwNum = pItem->sBox.sITEMS[i].dwAMOUNT;

			SITEM* pitembox = GLItemMan::GetInstance().GetItem( m_sItemIDBox[i] );
			if ( !pitembox )	continue;;

			if ( m_pItemImageBox[i] )
			{
				CString strText;
				strText.Format( "%u", dwNum );
				m_pItemImageBox[i]->SetItem( pitembox->sBasicOp.sICONID, pitembox->GetInventoryFile() );

				if ( dwNum > 1 )
					m_pItemImageBox[i]->SetTextDownRight( strText.GetString(), NS_UITEXTCOLOR::ORANGE );
			}
		}
	}
	else
	{
		m_sItemIDBox[0] = pItem->sBasicOp.sNativeID;

		SITEM* pitembox = GLItemMan::GetInstance().GetItem( m_sItemIDBox[0] );
		if ( !pitembox )	return;

		if ( m_pItemImageBox[0] )
		{
			CString strText;
			strText.Format( "%u", pitembox->GETAPPLYNUM() );
			m_pItemImageBox[0]->SetItem( pitembox->sBasicOp.sICONID, pitembox->GetInventoryFile() );
			m_pItemImageBox[0]->SetTextDownRight( strText.GetString(), NS_UITEXTCOLOR::ORANGE );
		}
	}

	if ( pItem->sBasicOp.emItemType == ITEM_SUIT ||
		pItem->sBasicOp.emItemType == ITEM_BOX || 
		pItem->sBasicOp.emItemType == ITEM_PREMIUMSET ||
		pItem->sBasicOp.emItemType == ITEM_BOX_SPECIFIC )
	{
		if ( m_pRender )
		{
			for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
			{
				m_pRender->m_sItem[i] = m_sItemIDBox[i];
			}

			m_pRender->m_bReady = true;
		}

		if ( m_pButtonLeft )
			m_pButtonLeft->SetVisibleSingle( TRUE );

		if ( m_pButtonRight )
			m_pButtonRight->SetVisibleSingle( TRUE );

		if ( m_pTextMale )
			m_pTextMale->SetVisibleSingle( TRUE );

		if ( m_pButtonMale )
		{
			m_pButtonMale->SetVisibleSingle( TRUE );

			if ( m_pRender )
				m_pButtonMale->SetFlip( m_pRender->m_bMale == TRUE );
		}

		if ( m_pTextFemale )
			m_pTextFemale->SetVisibleSingle( TRUE );

		if ( m_pButtonFemale )
		{
			m_pButtonFemale->SetVisibleSingle( TRUE );

			if ( m_pRender )
				m_pButtonFemale->SetFlip( m_pRender->m_bMale == FALSE );
		}
	}
	

	

}
