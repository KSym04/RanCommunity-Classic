#include "StdAfx.h"
#include "GachaPreviewWindow.h"
#include "GachaPreviewWindowRender.h"

#include "../InnerInterface.h"
#include "../BasicLineBox.h"
#include "../ItemImage.h"
#include "../BasicTextBox.h"
#include "../BasicButton.h"

#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"

#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Client/DxGlobalStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGachaPreviewWindow::CGachaPreviewWindow ()
	: m_pRender(NULL)
	, m_pButtonLeft(NULL)
	, m_pButtonRight(NULL)
	, m_pStanceButton(NULL)
	, m_pMaleButton(NULL)
	, m_pFemaleButton(NULL)
	, m_pStanceText(NULL)
	, sSelectedID(NATIVEID_NULL())
	, bMaleSelect(FALSE)
	, bFemaleSelect(FALSE)
{
	for (int i = 0; i < ITEM::SBOX::ITEM_SIZE; ++i)
	{
		m_pItems[i] = NULL;
	}
}

CGachaPreviewWindow::~CGachaPreviewWindow ()
{
}

void CGachaPreviewWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	std::string strItems[ITEM::SBOX::ITEM_SIZE] =
	{
		"ITEMSHOP_PREVIEW_ITEM00",
		"ITEMSHOP_PREVIEW_ITEM01",
		"ITEMSHOP_PREVIEW_ITEM02",
		"ITEMSHOP_PREVIEW_ITEM03",
		"ITEMSHOP_PREVIEW_ITEM04",
		"ITEMSHOP_PREVIEW_ITEM05",
		"ITEMSHOP_PREVIEW_ITEM06",
		"ITEMSHOP_PREVIEW_ITEM07",
		"ITEMSHOP_PREVIEW_ITEM08",
		"ITEMSHOP_PREVIEW_ITEM09",
	};

	for (int i = 0; i < ITEM::SBOX::ITEM_SIZE; ++i)
	{
		m_pItems[i] = NULL;
		m_pItems[i] = new CItemImage;
		m_pItems[i]->CreateSub(this, strItems[i].c_str(), UI_FLAG_DEFAULT, ITEMSHOP_PREVIEW_ITEM00 + i);
		m_pItems[i]->CreateSubControl();
		m_pItems[i]->SetVisibleSingle(FALSE);
		RegisterControl(m_pItems[i]);
	}

	m_pButtonLeft = new CBasicButton;
	m_pButtonLeft->CreateSub ( this, "GACHA_WINDOW_ARROW_TURN_LEFT_DEFAULT", UI_FLAG_DEFAULT, ITEM_PREVIEW_BUTTON_LEFT );
	m_pButtonLeft->CreateFlip ( "GACHA_WINDOW_ARROW_TURN_LEFT_CLICK", CBasicButton::CLICK_FLIP );
	m_pButtonLeft->CreateMouseOver ( "GACHA_WINDOW_ARROW_TURN_LEFT_OVER" );
	m_pButtonLeft->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonLeft );

	m_pButtonRight = new CBasicButton;
	m_pButtonRight->CreateSub ( this, "GACHA_WINDOW_ARROW_TURN_RIGHT_DEFAULT", UI_FLAG_DEFAULT, ITEM_PREVIEW_BUTTON_RIGHT );
	m_pButtonRight->CreateFlip ( "GACHA_WINDOW_ARROW_TURN_RIGHT_CLICK", CBasicButton::CLICK_FLIP );
	m_pButtonRight->CreateMouseOver ( "GACHA_WINDOW_ARROW_TURN_RIGHT_OVER" );
	m_pButtonRight->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonRight );

	m_pStanceButton = new CBasicButton;
	m_pStanceButton->CreateSub ( this, "ITEMSHOP_PREVIEW_STANCE_BUTTON", UI_FLAG_DEFAULT, ITEM_PREVIEW_STANCE_BUTTON );
	m_pStanceButton->CreateFlip ( "ITEMSHOP_PREVIEW_STANCE_BUTTON_ON", CBasicButton::RADIO_FLIP );
	m_pStanceButton->SetControlNameEx ( "ITEMSHOP_PREVIEW_STANCE_BUTTON" );
	m_pStanceButton->SetFlip( false );
	m_pStanceButton->SetVisibleSingle(false);
	RegisterControl ( m_pStanceButton );

	m_pMaleButton = new CBasicButton;
	m_pMaleButton->CreateSub(this, "ITEMSHOP_PREVIEW_MALE_BUTTON", UI_FLAG_DEFAULT, ITEM_PREVIEW_MALE_BUTTON);
	m_pMaleButton->CreateFlip("ITEMSHOP_PREVIEW_MALE_BUTTON_ON", CBasicButton::RADIO_FLIP);
	m_pMaleButton->SetControlNameEx("ITEMSHOP_PREVIEW_MALE_BUTTON");
	m_pMaleButton->SetVisibleSingle(false);
	RegisterControl( m_pMaleButton );

	m_pFemaleButton = new CBasicButton;
	m_pFemaleButton->CreateSub(this, "ITEMSHOP_PREVIEW_FEMALE_BUTTON", UI_FLAG_DEFAULT, ITEM_PREVIEW_FEMALE_BUTTON);
	m_pFemaleButton->CreateFlip("ITEMSHOP_PREVIEW_FEMALE_BUTTON_ON", CBasicButton::RADIO_FLIP);
	m_pFemaleButton->SetControlNameEx("ITEMSHOP_PREVIEW_FEMALE_BUTTON");
	m_pFemaleButton->SetVisibleSingle(false);
	RegisterControl( m_pFemaleButton );

	/*m_pStanceText = new CBasicTextBox;
	m_pStanceText->CreateSub ( this, "ITEMSHOP_PREVIEW_STANCE_TEXT" );
	m_pStanceText->SetFont ( pFont9 );
	m_pStanceText->SetText( ID2GAMEWORD( "ITEMPREVIEW_TEXTBOX", 0 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStanceText );

	CBasicTextBox* m_pMaleText = new CBasicTextBox;
	m_pMaleText->CreateSub(this, "ITEMSHOP_PREVIEW_MALE_TEXT");
	m_pMaleText->SetFont(pFont9);
	m_pMaleText->SetText(ID2GAMEWORD("ITEMPREVIEW_TEXTBOX", 7), NS_UITEXTCOLOR::WHITE);
	RegisterControl(m_pMaleText);

	CBasicTextBox* m_pFemaleText = new CBasicTextBox;
	m_pFemaleText->CreateSub(this, "ITEMSHOP_PREVIEW_FEMALE_TEXT");
	m_pFemaleText->SetFont(pFont9);
	m_pFemaleText->SetText(ID2GAMEWORD("ITEMPREVIEW_TEXTBOX", 8), NS_UITEXTCOLOR::WHITE);
	RegisterControl(m_pFemaleText);*/

	m_pRender = new CGachaPreviewWindowRender;
	m_pRender->CreateSub ( this, "GACHA_WINDOW_MESHRENDER" );
	m_pRender->CreateSubControl ();
	RegisterControl ( m_pRender );

	// pre-select gender button.
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( pCharacter ) 
	{
		EMCHARGENDER emCurrGender = CharClassToGender(pCharacter->m_emClass);
		m_pRender->SetCharGender( emCurrGender );

		if ( emCurrGender == GLGENDER_M ) 
		{
			bMaleSelect = TRUE;
			m_pMaleButton->SetFlip( bMaleSelect );
		}
		else
		{
			bFemaleSelect = TRUE;
			m_pFemaleButton->SetFlip( bFemaleSelect );
		}
	}
}

void CGachaPreviewWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CGachaPreviewWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case ITEM_PREVIEW_BUTTON_LEFT:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				if ( m_pRender )	m_pRender->m_vRot.x -=0.1f;
			}
		}break;
	case ITEM_PREVIEW_BUTTON_RIGHT:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				if ( m_pRender )	m_pRender->m_vRot.x +=0.1f;
				
			}
		}break;

	case ITEMSHOP_PREVIEW_ITEM00:
	case ITEMSHOP_PREVIEW_ITEM01:
	case ITEMSHOP_PREVIEW_ITEM02:
	case ITEMSHOP_PREVIEW_ITEM03:
	case ITEMSHOP_PREVIEW_ITEM04:
	case ITEMSHOP_PREVIEW_ITEM05:
	case ITEMSHOP_PREVIEW_ITEM06:
	case ITEMSHOP_PREVIEW_ITEM07:
	case ITEMSHOP_PREVIEW_ITEM08:
	case ITEMSHOP_PREVIEW_ITEM09:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{	
				int nIndex = ControlID - ITEMSHOP_PREVIEW_ITEM00;

				if ( nIndex < 0 || nIndex >= ITEMSHOP_PREVIEW_ITEM_MAX )	return;

				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_pItemCustom[nIndex].sNativeID;

				SITEM* pitem_data = GLItemMan::GetInstance().GetItem(sitemcustom.sNativeID);
				if (pitem_data && pitem_data->ISPILE())	sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM();

				if (sitemcustom.sNativeID != NATIVEID_NULL())
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO(sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL());
				}
			}
		}break;

	case ITEM_PREVIEW_STANCE_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bItemPreviewStance = !RANPARAM::bItemPreviewStance;
				m_pStanceButton->SetFlip( false );

				if ( m_pRender )
					m_pRender->ChangeStance();
			}
		}break;
	case ITEM_PREVIEW_MALE_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				bMaleSelect = !bMaleSelect;
				bFemaleSelect = !bMaleSelect;

				m_pMaleButton->SetFlip( bMaleSelect );
				m_pFemaleButton->SetFlip( bFemaleSelect );

				if ( m_pRender ) 
				{
					if( bMaleSelect ) m_pRender->SetCharGender( GLGENDER_M );
					else m_pRender->SetCharGender( GLGENDER_W );

					StartPreview( sSelectedID );
				}
					
			}
		}break;
	case ITEM_PREVIEW_FEMALE_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				bFemaleSelect = !bFemaleSelect;
				bMaleSelect = !bFemaleSelect;

				m_pFemaleButton->SetFlip( bFemaleSelect );
				m_pMaleButton->SetFlip( bMaleSelect );

				if ( m_pRender )
				{
					if ( bFemaleSelect ) m_pRender->SetCharGender( GLGENDER_W );
					else m_pRender->SetCharGender( GLGENDER_M );

					StartPreview( sSelectedID );
				}
			}
		}break;

	};

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
}

void CGachaPreviewWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}else{
		ClearRender();
	}
}

void CGachaPreviewWindow::ClearRender()
{
	if ( m_pRender )
		m_pRender->ClearRender();
}

void CGachaPreviewWindow::StartPreview( SITEMCUSTOM sItem )
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )		return;

	if ( !m_pRender )	return;

	for (int i = 0; i < ITEMSHOP_PREVIEW_ITEM_MAX; ++i)
	{
		m_pItems[i]->ResetItem();
		m_pItems[i]->SetVisibleSingle(FALSE);

		m_pItemCustom[i] = SITEMCUSTOM();
	}

	m_pRender->m_sPreviewData.wFace = pCharacter->m_wFace;
	m_pRender->m_sPreviewData.wHair = pCharacter->m_wHair;
	m_pRender->m_sPreviewData.wHairColor = pCharacter->m_wHairColor;

	m_pRender->m_sPreviewData.sPutOn[EMITEM_PREVIEW_SLOT_HEAD] = SITEMCUSTOM();
	m_pRender->m_sPreviewData.sPutOn[EMITEM_PREVIEW_SLOT_HEAD] = SITEMCUSTOM();
	m_pRender->m_sPreviewData.sPutOn[EMITEM_PREVIEW_SLOT_UPPER] = SITEMCUSTOM();
	m_pRender->m_sPreviewData.sPutOn[EMITEM_PREVIEW_SLOT_LOWER] = SITEMCUSTOM();
	m_pRender->m_sPreviewData.sPutOn[EMITEM_PREVIEW_SLOT_HAND] = SITEMCUSTOM();
	m_pRender->m_sPreviewData.sPutOn[EMITEM_PREVIEW_SLOT_FOOT] = SITEMCUSTOM();
	m_pRender->m_sPreviewData.sPutOn[EMITEM_PREVIEW_SLOT_WEAPON] = SITEMCUSTOM();

	SITEM* pItemCheck = GLItemMan::GetInstance().GetItem( sItem.sNativeID );
	if ( !pItemCheck )	
	{	
		ClearRender();
		m_pRender->m_bReady = true;

		return;
	}

	sSelectedID = sItem.sNativeID;

	if ( pItemCheck->sBasicOp.emItemType == ITEM_SUIT )
	{
		m_pItemCustom[0] = sItem;

		bool bCan = PreviewItemCheckWear( pItemCheck->sBasicOp.sNativeID );
		if ( bCan )
		{
			int nSlotNum = -1;
			switch (pItemCheck->sSuitOp.emSuit)
			{
			case SUIT_HEADGEAR:		nSlotNum = EMITEM_PREVIEW_SLOT_HEAD;	break;
			case SUIT_UPPER:		nSlotNum = EMITEM_PREVIEW_SLOT_UPPER;	break;
			case SUIT_LOWER:		nSlotNum = EMITEM_PREVIEW_SLOT_LOWER;	break;
			case SUIT_HAND:			nSlotNum = EMITEM_PREVIEW_SLOT_HAND;	break;
			case SUIT_FOOT:			nSlotNum = EMITEM_PREVIEW_SLOT_FOOT;	break;
			case SUIT_HANDHELD:		nSlotNum = EMITEM_PREVIEW_SLOT_WEAPON;	break;
			};

			if ( nSlotNum >= 0 ) m_pRender->m_sPreviewData.sPutOn[nSlotNum] = sItem;
		}
	}
	else if ( pItemCheck->sBasicOp.emItemType == ITEM_BOX || pItemCheck->sBasicOp.emItemType == ITEM_PREMIUMSET )
	{
		for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
		{
			SITEMCUSTOM sItemCustomBox;
			sItemCustomBox.sNativeID = pItemCheck->sBox.sITEMS[i].nidITEM;

			SITEM* pItemBox = GLItemMan::GetInstance().GetItem( sItemCustomBox.sNativeID );
			if( pItemBox )
			{
				m_pItemCustom[i] = sItemCustomBox;

				bool bCan = PreviewItemCheckWear( pItemBox->sBasicOp.sNativeID );
				if ( bCan )
				{
					int nSlotNum = -1;
					switch ( pItemBox->sSuitOp.emSuit )
					{
					case SUIT_HEADGEAR:		nSlotNum = EMITEM_PREVIEW_SLOT_HEAD;	break;
					case SUIT_UPPER:		nSlotNum = EMITEM_PREVIEW_SLOT_UPPER;	break;
					case SUIT_LOWER:		nSlotNum = EMITEM_PREVIEW_SLOT_LOWER;	break;
					case SUIT_HAND:			nSlotNum = EMITEM_PREVIEW_SLOT_HAND;	break;
					case SUIT_FOOT:			nSlotNum = EMITEM_PREVIEW_SLOT_FOOT;	break;
					case SUIT_HANDHELD:		nSlotNum = EMITEM_PREVIEW_SLOT_WEAPON;	break;
					}

					if ( nSlotNum >= 0 ) m_pRender->m_sPreviewData.sPutOn[nSlotNum] = sItemCustomBox;
				}
			}
		}
	}
	else 
	{
		m_pItemCustom[0] = sItem;
	}

	for (int i = 0; i < ITEMSHOP_PREVIEW_ITEM_MAX; ++i)
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem( m_pItemCustom[i].sNativeID );
		if ( pItem )
		{
			m_pItems[i]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
			m_pItems[i]->SetVisibleSingle( FALSE );
		}
		else
		{
			m_pItems[i]->ResetItem();
			m_pItems[i]->SetVisibleSingle( FALSE );
		}
	}

	ClearRender();
	m_pRender->m_bReady = true;
}

bool CGachaPreviewWindow::PreviewItemCheckWear( SNATIVEID sID )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem(sID);
	if ( !pItem )	return false;

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )		return false;

	EMCHARGENDER emCurrGender = CharClassToGender( pCharacter->m_emClass );
	EMCHARINDEX emIndex;
	EMCHARCLASS	emClass;

	if ( m_pRender->GetCharGender() == emCurrGender )
	{
		emClass = pCharacter->m_emClass;
		emIndex = CharClassToIndex( pCharacter->m_emClass );
	}
	else
	{
		emClass = CharClassToSex( pCharacter->m_emClass );
		emIndex = CharClassToIndex( emClass );
	}

	if (!(pItem->sBasicOp.dwReqCharClass & emClass))			return false;
	if (!(pItem->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool))) return false;

	if (pItem->sBasicOp.emItemType == ITEM_SUIT)
	{
		if (pItem->sSuitOp.emSuit > SUIT_HANDHELD)	return false;

		std::string strR = pItem->GetWearingFileR(emIndex);
		std::string strL = pItem->GetWearingFileL(emIndex);

		if (strR.size() > 0 || strL.size() > 0)	return true;
	}

	return false;
}