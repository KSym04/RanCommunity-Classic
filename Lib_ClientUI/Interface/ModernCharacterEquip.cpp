#include "StdAfx.h"
#include "./ModernCharacterEquip.h"
#include "./ModernCharacterEquipRender.h"

#include "./ItemImage.h"
#include "./ItemMove.h"
#include "./BasicButton.h"

#include "./InnerInterface.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CModernCharacterEquip::nOUTOFRANGE = -1;

CModernCharacterEquip::CModernCharacterEquip ()
	: m_pMouseOver ( NULL )
	, m_nIndex(nOUTOFRANGE)
	, m_pRender(NULL)
	, m_pButtonLeft(NULL)
	, m_pButtonRight(NULL)
{
	for ( int i=0; i<MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE; ++i )
	{
		m_pSlotDisplay[i] = NULL;
		m_pItemImage[i] = NULL;
		m_pSlotErrorImage[i] = NULL;
		m_ItemCustomArray[i] = SITEMCUSTOM ( SNATIVEID(false) );
	}
}

CModernCharacterEquip::~CModernCharacterEquip ()
{
}

void CModernCharacterEquip::CreateSubControl ()
{
	m_pRender = new CModernCharacterEquipRender;
	m_pRender->CreateSub ( this, "MODERN_CHARACTER_WINDOW_CHARACTER_MESHRENDER" );
	m_pRender->CreateSubControl ();
	RegisterControl ( m_pRender );

	std::string strInvenWearItem[MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE] = 
	{
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_UPPER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_WAIST",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LOWER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_HAND",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_FOOT",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY",

		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_NECK",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_WRIST",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY",

		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_VEHICLE",
	};

	std::string strInvenWearOver[MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE] = 
	{
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_UPPER_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_WAIST_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LOWER_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_HAND_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_FOOT_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY_OVER",

		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_NECK_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_WRIST_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY_OVER",

		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON_OVER",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_VEHICLE_OVER",
	};

	std::string strInvenSlotError[MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE] = 
	{
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_UPPER_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_WAIST_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LOWER_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_HAND_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_FOOT_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY_ERROR",

		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_NECK_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_WRIST_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY_ERROR",

		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON_ERROR",
		"MODERN_CHARACTER_WINDOW_WEAR_SLOT_VEHICLE_ERROR",
	};

	for ( int i = 0; i < MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE; i++ )
	{
		m_pItemImage[i] = CreateItemImage ( strInvenWearItem[i].c_str(), MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD + i );
		m_pSlotDisplay[i] = CreateControl ( strInvenWearOver[i].c_str() );
		m_pSlotDisplay[i]->SetVisibleSingle ( FALSE );

		m_pSlotErrorImage[i] = CreateControl ( strInvenSlotError[i].c_str() );
		m_pSlotErrorImage[i]->SetVisibleSingle ( FALSE );
	}

	m_pMouseOver = CreateControl ( "ITEM_MOUSE_OVER" );	


	m_pButtonLeft = new CBasicButton;
	m_pButtonLeft->CreateSub ( this, "MODERN_CHARACTER_WINDOW_WEAR_ARROW_TURN_LEFT_DEFAULT", UI_FLAG_DEFAULT, MODERN_CHARACTER_WINDOW_BUTTON_LEFT );
	m_pButtonLeft->CreateFlip ( "MODERN_CHARACTER_WINDOW_WEAR_ARROW_TURN_LEFT_CLICK", CBasicButton::CLICK_FLIP );
	m_pButtonLeft->CreateMouseOver ( "MODERN_CHARACTER_WINDOW_WEAR_ARROW_TURN_LEFT_OVER" );
	m_pButtonLeft->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonLeft );

	m_pButtonRight = new CBasicButton;
	m_pButtonRight->CreateSub ( this, "MODERN_CHARACTER_WINDOW_WEAR_ARROW_TURN_RIGHT_DEFAULT", UI_FLAG_DEFAULT, MODERN_CHARACTER_WINDOW_BUTTON_RIGHT );
	m_pButtonRight->CreateFlip ( "MODERN_CHARACTER_WINDOW_WEAR_ARROW_TURN_RIGHT_CLICK", CBasicButton::CLICK_FLIP );
	m_pButtonRight->CreateMouseOver ( "MODERN_CHARACTER_WINDOW_WEAR_ARROW_TURN_RIGHT_OVER" );
	m_pButtonRight->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonRight );
}

CUIControl*	CModernCharacterEquip::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );

	return pControl;
}

CItemImage*	CModernCharacterEquip::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	pItemImage->CreateTextBoxDownRight();
	RegisterControl ( pItemImage );

	return pItemImage;
}

void CModernCharacterEquip::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex ( nOUTOFRANGE );
	if ( m_pMouseOver ) m_pMouseOver->SetVisibleSingle ( FALSE );

	CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
	SNATIVEID sHOLD_ITEM_ID(false);
	if ( pItemMove ) sHOLD_ITEM_ID = pItemMove->GetItem();

	BOOL bChanged = FALSE;
	for ( int i = 0; i < MODERN_CHARACTER_WINDOW_ITEM_IMAGE_SIZE; i++ )
	{
		const SITEMCUSTOM& sItemCustomOld = GetItem ( i );

		EMSLOT emSlot = IMAGE2EMSLOT ( i + MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD );
		if ( emSlot >= SLOT_NSIZE_S_2 )
		{
			SITEMCUSTOM sItemCustom = sItemCustomOld;
			UnLoadItem ( i, sItemCustom );
		}
		else
		{
			SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_SLOT_ITEM ( emSlot );
			if ( sItemCustom != sItemCustomOld )
			{		
				if ( sItemCustom.sNativeID != NATIVEID_NULL () )
				{
					LoadItem ( i, sItemCustom );
				}else{
					UnLoadItem ( i, sItemCustom );
				}

				bChanged = TRUE;
			}
		}
		
		m_pSlotDisplay[i]->SetVisibleSingle ( FALSE );
	
		if ( sHOLD_ITEM_ID != NATIVEID_NULL () )
		{
			if ( GLGaeaClient::GetInstance().GetCharacter()->CHECKSLOT_ITEM ( sHOLD_ITEM_ID, emSlot ) )
			{
				const UIRECT& rcImagePos = m_pItemImage[i]->GetGlobalPos ();
				D3DXVECTOR2 vPos ( rcImagePos.left, rcImagePos.top );

				if ( GLGaeaClient::GetInstance().GetCharacter()->ACCEPT_ITEM ( sHOLD_ITEM_ID ) )
				{
					m_pSlotDisplay[i]->SetVisibleSingle ( TRUE );
				}
			}
		}

		if ( m_pSlotErrorImage[i] )
		{
			m_pSlotErrorImage[i]->SetVisibleSingle( FALSE );

			if ( RPARAM::bCheckWearUse )
			{
				const SITEMCUSTOM& sItemCustomSlot = GetItem ( i );

				if ( sItemCustomSlot.sNativeID != NATIVEID_NULL() && emSlot < SLOT_TSIZE )
				{
					EMCHECK_WEAR emCheckWear = GLGaeaClient::GetInstance().GetCharacter()->CHECK_WEAR_ITEM_SLOT( sItemCustomSlot.sNativeID, emSlot );
					if ( emCheckWear != EMCHECK_WEAR_PASS )	
						m_pSlotErrorImage[i]->SetVisibleSingle( TRUE );
				}
			}
		}

	}

	if ( bChanged )
	{
		if ( m_pRender )
			m_pRender->m_bUpdateSuit = TRUE;
	}

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernCharacterEquip::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_UPPER:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_WAIST:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LOWER:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_HAND:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_FOOT:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY:

	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_NECK:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_WRIST:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY:

	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON:
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON:

	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_VEHICLE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = ControlID - MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD;
				SetItemIndex ( nIndex );

				if ( !CInnerInterface::GetInstance().IsFirstItemSlot () ) return ;

				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
				if ( !pItemMove )
				{
					return ;
				}

				if ( pItemMove->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPos = m_pItemImage[nIndex]->GetGlobalPos ();
					pItemMove->SetGlobalPos ( rcSlotPos );						
					AddMessageEx ( UIMSG_MOUSEIN_MODERN_CHARACTER_WINDOW_WEARSLOT | UIMSG_TOTOPPARENT );
				}				

				if ( pItemMove->GetItem () == NATIVEID_NULL () && m_pItemImage[m_nIndex]->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcImagePos = m_pItemImage[nIndex]->GetGlobalPos ();
					m_pMouseOver->SetGlobalPos ( rcImagePos );
					m_pMouseOver->SetVisibleSingle ( TRUE );
				}
			}
		}break;

	case MODERN_CHARACTER_WINDOW_BUTTON_LEFT:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				if ( m_pRender )	m_pRender->m_vRot.x -=0.1f;
			}
		}break;

	case MODERN_CHARACTER_WINDOW_BUTTON_RIGHT:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				if ( m_pRender )	m_pRender->m_vRot.x +=0.1f;
			}
		}break;

	}

	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CModernCharacterEquip::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
}

EMSLOT CModernCharacterEquip::IMAGE2EMSLOT ( int nIndex )
{
	switch ( nIndex )
	{	
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD:			return SLOT_HEADGEAR;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_UPPER:			return SLOT_UPPER;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_WAIST:			return SLOT_BELT;			/*ep8 puton slots, Juver, 2018/03/29 */
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LOWER:			return SLOT_LOWER;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_HAND:			return SLOT_HAND;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_FOOT:			return SLOT_FOOT;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY:	return SLOT_ACCESSORY_L;	/*ep8 puton slots, Juver, 2018/03/29 */

	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING:	return SLOT_EARRING;		/*ep8 puton slots, Juver, 2018/03/30 */
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING:	return SLOT_DECORATION;		/*ep8 puton slots, Juver, 2018/03/30 */
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_NECK:			return SLOT_NECK;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_WRIST:			return SLOT_WRIST;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER:		return SLOT_LFINGER;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER:	return SLOT_RFINGER;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY:	return SLOT_ACCESSORY_R;	/*ep8 puton slots, Juver, 2018/03/30 */

	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON:	return SLOT_RHAND;
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON:		return SLOT_LHAND;
	
	case MODERN_CHARACTER_WINDOW_WEAR_SLOT_VEHICLE:			return SLOT_VEHICLE;	
	};

	return SLOT_TSIZE;
}

EMSLOT CModernCharacterEquip::GetItemSlot ()
{
	return IMAGE2EMSLOT ( GetItemIndex () + MODERN_CHARACTER_WINDOW_WEAR_SLOT_HEAD );
}

SITEMCUSTOM& CModernCharacterEquip::GetItem ( int nIndex )
{
	return m_ItemCustomArray[nIndex];
}

void CModernCharacterEquip::LoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom )
{
	m_ItemCustomArray[nIndex] = ref_sItemCustom;

	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( ref_sItemCustom.sNativeID );
	m_pItemImage[nIndex]->SetItem ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile(), pItemData->sBasicOp.sNativeID );
	m_pItemImage[nIndex]->SetTextDownRight( "", NS_UITEXTCOLOR::WHITE );

	if ( ref_sItemCustom.GETGRADE( EMGRINDING_NONE ) )
	{
		CString strGrind;
		strGrind.Format( "+%d", ref_sItemCustom.GETGRADE( EMGRINDING_NONE ) );
		m_pItemImage[nIndex]->SetTextDownRight( strGrind.GetString(), NS_UITEXTCOLOR::GOLD );
	}
}

void CModernCharacterEquip::UnLoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom )
{
	m_ItemCustomArray[nIndex] = ref_sItemCustom;
	m_pItemImage[nIndex]->ResetItem ();
	m_pItemImage[nIndex]->SetTextDownRight( "", NS_UITEXTCOLOR::WHITE );
}

void CModernCharacterEquip::SetTabButton( BOOL bMain )
{
	if( m_pRender )
		m_pRender->m_bUpdateSuit = TRUE;
}

void CModernCharacterEquip::ClearRender()
{
	if( m_pRender )
		m_pRender->ClearRender();
}

void CModernCharacterEquip::StartRender()
{
	if( m_pRender )
	{
		m_pRender->m_bReady = TRUE;
		m_pRender->m_fRenderDelay = 0.0f;
	}
}