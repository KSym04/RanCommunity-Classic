#include "StdAfx.h"
#include "./InventoryPageWear.h"
#include "./ItemImage.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "./InnerInterface.h"
#include "./ItemMove.h"
#include "./BasicButton.h"
#include "./GameTextControl.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CInventoryPageWear::nOUTOFRANGE = -1;

CInventoryPageWear::CInventoryPageWear () 
	: m_pMouseOver ( NULL )
	, m_pButtonLock(NULL)
	, m_pButtonUnlock(NULL)
{
	for ( int i=0; i<ITEM_IMAGE_SIZE; ++i )
	{
		m_pSlotDisplay[i] = NULL;
		m_pItemImage[i] = NULL;
		m_pSlotErrorImage[i] = NULL;
	}
}

CInventoryPageWear::~CInventoryPageWear ()
{
}

void CInventoryPageWear::CreateSubControl ( int nClassType )
{
	CString	strInvenWearBack[GLCI_NUM_8CLASS] = 
	{
		"INVENTORY_PAGE_WEAR_FIGHTER_M",
		"INVENTORY_PAGE_WEAR_ARMS_M",
		"INVENTORY_PAGE_WEAR_ARCHER_W",
		"INVENTORY_PAGE_WEAR_SPIRIT_W",
		"INVENTORY_PAGE_WEAR_EXTREME_M",
		"INVENTORY_PAGE_WEAR_EXTREME_W",
		"INVENTORY_PAGE_WEAR_FIGHTER_W",
		"INVENTORY_PAGE_WEAR_ARMS_W",
		"INVENTORY_PAGE_WEAR_ARCHER_M",
		"INVENTORY_PAGE_WEAR_SPIRIT_M",
		"INVENTORY_PAGE_WEAR_SCIENTIST_M",
		"INVENTORY_PAGE_WEAR_SCIENTIST_W",
		"INVENTORY_PAGE_WEAR_ASSASSIN_M",
		"INVENTORY_PAGE_WEAR_ASSASSIN_W",
		"INVENTORY_PAGE_WEAR_TRICKER_M",
		"INVENTORY_PAGE_WEAR_TRICKER_W",
	};

	CreateControl ( strInvenWearBack[nClassType].GetString () );

	{	//	아이템 이미지 ( 0 - 11 )
		CString strInvenWearItem[ITEM_IMAGE_SIZE] = 
		{
			"INVENTORY_WEAR_ITEM_IMAGE0",
			"INVENTORY_WEAR_ITEM_IMAGE1",
			"INVENTORY_WEAR_ITEM_IMAGE2",
			"INVENTORY_WEAR_ITEM_IMAGE3",
			"INVENTORY_WEAR_ITEM_IMAGE4",
			"INVENTORY_WEAR_ITEM_IMAGE5",
			"INVENTORY_WEAR_ITEM_IMAGE6",
			"INVENTORY_WEAR_ITEM_IMAGE7",
			"INVENTORY_WEAR_ITEM_IMAGE8",
			"INVENTORY_WEAR_ITEM_IMAGE9",
			"INVENTORY_WEAR_ITEM_IMAGE10",
			"INVENTORY_WEAR_ITEM_IMAGE11",
			"INVENTORY_WEAR_ITEM_IMAGE12",
			"INVENTORY_WEAR_ITEM_IMAGE13",
			"INVENTORY_WEAR_ITEM_IMAGE14",
		};

		CString strInvenWearOver[ITEM_IMAGE_SIZE] = 
		{
			"INVENTORY_WEAR_OVER_IMAGE0",
			"INVENTORY_WEAR_OVER_IMAGE1",
			"INVENTORY_WEAR_OVER_IMAGE2",
			"INVENTORY_WEAR_OVER_IMAGE3",
			"INVENTORY_WEAR_OVER_IMAGE4",
			"INVENTORY_WEAR_OVER_IMAGE5",
			"INVENTORY_WEAR_OVER_IMAGE6",
			"INVENTORY_WEAR_OVER_IMAGE7",
			"INVENTORY_WEAR_OVER_IMAGE8",
			"INVENTORY_WEAR_OVER_IMAGE9",
			"INVENTORY_WEAR_OVER_IMAGE10",
			"INVENTORY_WEAR_OVER_IMAGE11",
			"INVENTORY_WEAR_OVER_IMAGE12",
			"INVENTORY_WEAR_OVER_IMAGE13",
			"INVENTORY_WEAR_OVER_IMAGE14",
		};

		std::string strInvenSlotError[ITEM_IMAGE_SIZE] = 
		{
			"INVENTORY_WEAR_OVER_IMAGE0_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE1_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE2_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE3_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE4_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE5_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE6_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE7_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE8_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE9_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE10_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE11_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE12_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE13_ERROR",
			"INVENTORY_WEAR_OVER_IMAGE14_ERROR",
		};


		for ( int i = 0; i < ITEM_IMAGE_SIZE; i++ )
		{
			m_pItemImage[i] = CreateItemImage ( strInvenWearItem[i].GetString (), ITEM_IMAGE0 + i );
			m_pSlotDisplay[i] = CreateControl ( strInvenWearOver[i].GetString () );
			m_pSlotDisplay[i]->SetVisibleSingle ( FALSE );

			m_pSlotErrorImage[i] = CreateControl ( strInvenSlotError[i].c_str() );
			m_pSlotErrorImage[i]->SetVisibleSingle ( FALSE );
		}
	}

	m_pMouseOver = CreateControl ( "ITEM_MOUSE_OVER" );
//	m_pSlotDisplay = CreateControl ( "TRADE_INVENTORY_OVER_0" );
//	m_pSlotBlockDisplay = CreateControl ( "TRADE_OVER_0" );

	m_pButtonLock = new CBasicButton;
	m_pButtonLock->CreateSub ( this, "INVEN_BUTTON_LOCK", UI_FLAG_DEFAULT, BUTTON_KEY_LOCK );
	m_pButtonLock->CreateFlip ( "INVEN_BUTTON_LOCK_F", CBasicButton::CLICK_FLIP );
	m_pButtonLock->SetFlip(false);
	m_pButtonLock->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pButtonLock );

	m_pButtonUnlock = new CBasicButton;
	m_pButtonUnlock->CreateSub ( this, "INVEN_BUTTON_UNLOCK", UI_FLAG_DEFAULT, BUTTON_KEY_UNLOCK );
	m_pButtonUnlock->CreateFlip ( "INVEN_BUTTON_UNLOCK_F", CBasicButton::CLICK_FLIP );
	m_pButtonUnlock->SetFlip(false);
	m_pButtonUnlock->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pButtonUnlock );
}

CUIControl*	CInventoryPageWear::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );

	return pControl;
}

CItemImage*	CInventoryPageWear::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );

	return pItemImage;
}

EMSLOT CInventoryPageWear::IMAGE2EMSLOT ( int nIndex )
{
	switch ( nIndex )
	{	
	case ITEM_IMAGE0:	return SLOT_HEADGEAR;	//	머리
	case ITEM_IMAGE1:	return SLOT_NECK;		//	목걸이
	case ITEM_IMAGE2:	return SLOT_UPPER;		//	상의
	case ITEM_IMAGE3:	return SLOT_LHAND;		//	왼손도구
	case ITEM_IMAGE4:	return SLOT_WRIST;		//	손목
	case ITEM_IMAGE5:	return SLOT_HAND;		//	장갑
	case ITEM_IMAGE6:	return SLOT_LOWER;		//	하의
	case ITEM_IMAGE7:	return SLOT_LFINGER;	//	왼손 손가락
	case ITEM_IMAGE8:	return SLOT_RHAND;		//	오른손도구
	case ITEM_IMAGE9:	return SLOT_FOOT;		//	신발
	case ITEM_IMAGE10:	return SLOT_RFINGER;	//	오른손 손가락	
	case ITEM_IMAGE11:	return SLOT_VEHICLE;	
	case ITEM_IMAGE12:	return SLOT_BELT;	
	case ITEM_IMAGE13:	return SLOT_DECORATION;	
	case ITEM_IMAGE14:	return SLOT_WING;	
	}

	GASSERT ( 0 && "범위를 벗어납니다." );
	return SLOT_TSIZE;
}

void CInventoryPageWear::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex ( nOUTOFRANGE );
	if ( m_pMouseOver ) m_pMouseOver->SetVisibleSingle ( FALSE );
//	if ( m_pSlotDisplay ) m_pSlotDisplay->SetVisibleSingle ( FALSE );
//	if ( m_pSlotBlockDisplay ) m_pSlotBlockDisplay->SetVisibleSingle ( FALSE );

	//	스냅, 스킬 이미지 붙이기
	CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
	SNATIVEID sHOLD_ITEM_ID(false);
//	bool bFOUND_TOWEAR = false;
	if ( pItemMove ) sHOLD_ITEM_ID = pItemMove->GetItem();

	/*ep8 puton slots, Juver, 2018/03/29 */
	for ( int i = 0; i < ITEM_IMAGE_SIZE; i++ )
	{
		const SITEMCUSTOM& sItemCustomOld = GetItem ( i );

		EMSLOT emSlot = IMAGE2EMSLOT ( i + ITEM_IMAGE0 );
		SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_SLOT_ITEM ( emSlot );

		//	NOTE
		//		이전 프레임과 비교하여,
		//		데이타가 달라진 경우에만,
		//		로드/언로드 작업을 진행한다.
		if ( sItemCustom != sItemCustomOld )
		{		
			if ( sItemCustom.sNativeID != NATIVEID_NULL () )
			{
				LoadItem ( i, sItemCustom );
			}
			else
			{
				UnLoadItem ( i );
			}
		}

		m_pSlotDisplay[i]->SetVisibleSingle ( FALSE );

		//	NOTE
		//		장착될 위치 표시
		if ( sHOLD_ITEM_ID != NATIVEID_NULL () /*&& !bFOUND_TOWEAR*/ )
		{
			if ( GLGaeaClient::GetInstance().GetCharacter()->CHECKSLOT_ITEM ( sHOLD_ITEM_ID, emSlot ) )
			{
				const UIRECT& rcImagePos = m_pItemImage[i]->GetGlobalPos ();
				D3DXVECTOR2 vPos ( rcImagePos.left, rcImagePos.top );

				if ( GLGaeaClient::GetInstance().GetCharacter()->ACCEPT_ITEM ( sHOLD_ITEM_ID ) )
				{
					m_pSlotDisplay[i]->SetVisibleSingle ( TRUE );
				}

//				bFOUND_TOWEAR = true;
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
	
	EMPERSONAL_LOCK emLock = EMPERSONAL_LOCK_EQUIP;
	BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->isPersonalLock( emLock );
	//BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->PersonalLockRequest( emLock );
	//if ( m_pButtonLock )	m_pButtonLock->SetVisibleSingle( bEnabled && bEnabled );
	//if ( m_pButtonUnlock )	m_pButtonUnlock->SetVisibleSingle( bEnabled && !bEnabled );
	if (m_pButtonLock) m_pButtonLock->SetVisibleSingle( bEnabled );  
    if (m_pButtonUnlock) m_pButtonUnlock->SetVisibleSingle( !bEnabled ); 
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CInventoryPageWear::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
    switch (ControlID)
    {
    case ITEM_IMAGE0:
    case ITEM_IMAGE1:
    case ITEM_IMAGE2:
    case ITEM_IMAGE3:
    case ITEM_IMAGE4:
    case ITEM_IMAGE5:
    case ITEM_IMAGE6:
    case ITEM_IMAGE7:
    case ITEM_IMAGE8:
    case ITEM_IMAGE9:
    case ITEM_IMAGE10:
    case ITEM_IMAGE11:
	case ITEM_IMAGE12:
	case ITEM_IMAGE13:
	case ITEM_IMAGE14:
        {
            if (CHECK_MOUSE_IN(dwMsg))
            {
                int nIndex = ControlID - ITEM_IMAGE0;
                SetItemIndex(nIndex);

                if (!CInnerInterface::GetInstance().IsFirstItemSlot()) return;

                CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
                if (!pItemMove)
                {
                    GASSERT(0 && "CItemMove is NULL.");
                    return;
                }

                if (pItemMove->GetItem() != NATIVEID_NULL())
                {
                    const UIRECT& rcSlotPos = m_pItemImage[nIndex]->GetGlobalPos();
                    pItemMove->SetGlobalPos(rcSlotPos);

                    AddMessageEx(UIMSG_MOUSEIN_WEARSLOT | UIMSG_TOTOPPARENT);
                }

                if (pItemMove->GetItem() == NATIVEID_NULL() && m_pItemImage[m_nIndex]->GetItem() != NATIVEID_NULL())
                {
                    const UIRECT& rcImagePos = m_pItemImage[nIndex]->GetGlobalPos();
                    m_pMouseOver->SetGlobalPos(rcImagePos);
                    m_pMouseOver->SetVisibleSingle(TRUE);
                }
            }
        }
        break;

        /* Puton Lock Logic */
    case BUTTON_KEY_LOCK:
		{
			if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			{
				//BOOL bHasPin = GLGaeaClient::GetInstance().GetCharacter()->HasPersonalLockPin();
    //    
				//if (!bHasPin)
				//{   
				//	EMPERSONAL_LOCK emLock = EMPERSONAL_LOCK_EQUIP;
				//	CInnerInterface::GetInstance().PersonalLockPinCreate(emLock);
				//	return;  // Stop further logic if PIN creation is triggered
				//}

				EMPERSONAL_LOCK emLock = EMPERSONAL_LOCK_EQUIP;
				CInnerInterface::GetInstance().PersonalLockPinInput(emLock);

				if (m_pButtonLock) m_pButtonLock->SetVisibleSingle(FALSE);
				if (m_pButtonUnlock) m_pButtonUnlock->SetVisibleSingle(TRUE);
			}
		} 
		break;


    case BUTTON_KEY_UNLOCK:
        {
            if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
            {
                EMPERSONAL_LOCK emLock = EMPERSONAL_LOCK_EQUIP;
                CInnerInterface::GetInstance().PersonalLockPinInput(emLock);

                if (m_pButtonLock) m_pButtonLock->SetVisibleSingle(TRUE);
                if (m_pButtonUnlock) m_pButtonUnlock->SetVisibleSingle(FALSE);
            }
        }
        break;
    }
}


EMSLOT CInventoryPageWear::GetItemSlot ()
{
	return IMAGE2EMSLOT ( GetItemIndex () + ITEM_IMAGE0 );
}

void CInventoryPageWear::LoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom )
{
	m_ItemCustomArray[nIndex] = ref_sItemCustom;

	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( ref_sItemCustom.sNativeID );
	m_pItemImage[nIndex]->SetItem ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile(), pItemData->sBasicOp.sNativeID );
}

SITEMCUSTOM& CInventoryPageWear::GetItem ( int nIndex )
{
	return m_ItemCustomArray[nIndex];
}

void CInventoryPageWear::UnLoadItem ( int nIndex )
{
	m_ItemCustomArray[nIndex].sNativeID = NATIVEID_NULL ();
	m_pItemImage[nIndex]->ResetItem ();
}