#include "StdAfx.h"
#include "./EP7ChaosMachineInventoryPage.h"
#include "./ItemSlotEx.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const INT CEP7ChaosMachineInvenPage::nSTARTLINE = 0;	
const INT CEP7ChaosMachineInvenPage::nOUTOFRANGE = -1;

CEP7ChaosMachineInvenPage::CEP7ChaosMachineInvenPage()
	: m_nONE_VIEW_SLOT( nONE_VIEW_SLOT_DEFAULT )
{
	for( INT i=0; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i] = NULL;

	for( INT i=0; i<nMAX_ONE_VIEW_SLOT; ++i )
		m_pItemSlotArrayDummy[i] = NULL;
}

CEP7ChaosMachineInvenPage::~CEP7ChaosMachineInvenPage()
{
}

void CEP7ChaosMachineInvenPage::CreateSubControl()
{
	CString strSlotKeyword[ nMAX_ONE_VIEW_SLOT ] =
	{
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT0",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT1",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT2",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT3",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT4",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT5",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT6",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT7",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT8",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT9",

		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT0",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT1",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT2",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT3",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT4",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT5",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT6",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT7",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT8",
		"EP7_CHAOS_MACHINE_INVENTORYSLOT_ITEM_SLOT9",
	};

	for( INT i=0; i<nMAX_ONE_VIEW_SLOT; ++i )
		m_pItemSlotArrayDummy[i] = CreateControl( strSlotKeyword[i].GetString() );

	for( INT i=0; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i] = CreateItemSlot( strSlotKeyword[0], ITEM_SLOT0 + i );

	SetItemSlotRender( nSTARTLINE, m_nONE_VIEW_SLOT );
}

CItemSlotEx* CEP7ChaosMachineInvenPage::CreateItemSlot( CString strKeyword, UIGUID ControlID )
{
	CItemSlotEx* pItemSlot = new CItemSlotEx;
	pItemSlot->CreateSub ( this, strKeyword.GetString(), UI_FLAG_DEFAULT, ControlID );
	pItemSlot->CreateSubTradeInventory( EM_INVENSIZE_X );
	RegisterControl( pItemSlot );

	return pItemSlot;
}

CUIControl*	CEP7ChaosMachineInvenPage::CreateControl( const TCHAR* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub( this, szControl );
	RegisterControl( pControl );
	return pControl;
}

void CEP7ChaosMachineInvenPage::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex( nOUTOFRANGE, nOUTOFRANGE );

	CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	UpdateBLOCK();
}

void CEP7ChaosMachineInvenPage::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	if( ITEM_SLOT0 <= ControlID && ControlID < ITEM_SLOT_END )
	{
		if( CHECK_MOUSE_IN( dwMsg ) )
		{
			const INT nPosY = ControlID - ITEM_SLOT0;
			CItemSlot* pSlot = m_pItemSlotArray[ nPosY ];
			const INT nPosX = pSlot->GetItemIndex();

			if( nPosY < 0 || nPosX < 0 )
				return;

			SetItemIndex( nPosX, nPosY );
		}
	}
}

void CEP7ChaosMachineInvenPage::ResetItemSlotRender ( int nStart, int nEnd )
{
	if ( nEnd < 0 )			return;
	if ( nStart > nEnd )	return;

	for ( int i = nStart; i < nEnd; i++ )
	{
		if ( m_pItemSlotArray[i] )
		{
			m_pItemSlotArray[i]->SetVisibleSingle ( FALSE );
		}
	}
}

void CEP7ChaosMachineInvenPage::SetItemSlotRender ( int nStart, int nEnd )
{
	if ( nEnd < 0 ) return ;
	if ( nStart > nEnd ) return ;

	const UIRECT& rcParentPos = GetGlobalPos ();

	for ( int i = nStart; i < nEnd; i++ )
	{
		if ( m_pItemSlotArray[i] )
		{
			int nAbsoluteIndex = i - nStart;

			CUIControl* pDummyControl = m_pItemSlotArrayDummy[nAbsoluteIndex];			
			const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos ();
			const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos ();

			m_pItemSlotArray[i]->SetLocalPos ( rcSlotLocalPos );
			m_pItemSlotArray[i]->SetGlobalPos ( rcSlotPos );
			m_pItemSlotArray[i]->SetVisibleSingle ( TRUE );
		}
	}
}


void CEP7ChaosMachineInvenPage::SetItemIndex( INT nPosX, INT nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void CEP7ChaosMachineInvenPage::GetItemIndex( INT* pnPosX, INT* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

void CEP7ChaosMachineInvenPage::LoadItemPage( GLInventory &ItemData )
{
	GLInventory::CELL_MAP* pItemList = ItemData.GetItemList();

	if( pItemList->size() > ( EM_INVENSIZE_X * EM_INVENSIZE_Y ) )
	{
		GASSERT( 0 && "인터페이스 사이즈보다, 데이타크기가 큽니다." );
		return;
	}

	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for( WORD y=0; y<EM_INVENSIZE_Y; ++y )
		{
			for( WORD x=0; x<EM_INVENSIZE_X; ++x )
			{
				SINVENITEM& ref_InvenItem = GetItem( x, y );
				if( ref_InvenItem.sItemCustom.sNativeID != NATIVEID_NULL() )
				{
					GLInventory::CELL_KEY foundkey( x, y );
					GLInventory::CELL_MAP_ITER found = pItemList->find( foundkey );
					if( found == iter_end )
						UnLoadItem( x, y );
				}
			}
		}
	}

	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for( ; iter != iter_end; ++iter )
		{
			SINVENITEM* pInvenItem = (*iter).second;
			SINVENITEM& ref_InvenItemOld = GetItem( pInvenItem->wPosX, pInvenItem->wPosY );
			if( *pInvenItem != ref_InvenItemOld )
				LoadItem( *pInvenItem );
		}
	}
}

void CEP7ChaosMachineInvenPage::LoadItem( SINVENITEM& ref_InvenItem )
{	
	INT nPosX = ref_InvenItem.wPosX;
	INT nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[ nPosY ]->SetItemImage( nPosX, ref_InvenItem );
}

void CEP7ChaosMachineInvenPage::UnLoadItemPage()
{
	for( INT y=0; y<EM_INVENSIZE_Y; ++y )
		for( INT x=0; x<EM_INVENSIZE_X; ++x )
			UnLoadItem( x, y );
}

void CEP7ChaosMachineInvenPage::UnLoadItem( INT nPosX, INT nPosY )
{
	m_pItemSlotArray[ nPosY ]->ResetItemImage( nPosX );
}

void CEP7ChaosMachineInvenPage::ResetAllFlipItem()
{
	for( WORD y=0; y<EM_INVENSIZE_Y; ++y )
		for( WORD x=0; x<EM_INVENSIZE_X; ++x )
			m_pItemSlotArray[y]->SetFlipItem( x, FALSE );
}

void CEP7ChaosMachineInvenPage::UpdateFlipItem( GLInventory& ref_Inventory )
{
	ResetAllFlipItem();

	GLInventory::CELL_MAP_ITER iter = ref_Inventory.GetItemList()->begin();
	GLInventory::CELL_MAP_ITER iter_end = ref_Inventory.GetItemList()->end();

	for( ; iter != iter_end; ++iter )
	{
		SINVENITEM* pInvenItem = (*iter).second;
		if( pInvenItem )
		{
			INT nPosX = pInvenItem->wBackX;
			INT nPosY = pInvenItem->wBackY;
			m_pItemSlotArray[ nPosY ]->SetFlipItem( nPosX, TRUE );
		}
	}
}

void CEP7ChaosMachineInvenPage::UpdateFlipItem( SINVEN_POS& ref_GarbageItem )
{
	if( ref_GarbageItem.wPosY >= 0 && ref_GarbageItem.wPosY < EM_INVENSIZE_Y )
	{
		if( ref_GarbageItem.wPosX >= 0 && ref_GarbageItem.wPosX < EM_INVENSIZE_X )
			m_pItemSlotArray[ ref_GarbageItem.wPosY ]->SetFlipItem( ref_GarbageItem.wPosX, TRUE );
	}
}

void CEP7ChaosMachineInvenPage::UpdateBLOCK()
{
	const WORD& wAddINVENLINE = GLGaeaClient::GetInstance().GetCharacter()->GetOnINVENLINE();
	INT nUSABLE_INVENLINE = EM_INVEN_DEF_SIZE_Y + wAddINVENLINE;

	if( EM_INVENSIZE_Y < nUSABLE_INVENLINE )
	{		
		GASSERT( 0 && "데이타 오류입니다." );
		nUSABLE_INVENLINE = EM_INVENSIZE_Y;
	}

	for( INT i=0; i<nUSABLE_INVENLINE; ++i )
		m_pItemSlotArray[i]->ResetBLOCK();

	for( INT i=nUSABLE_INVENLINE; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i]->SetBLOCK();
}

SINVENITEM& CEP7ChaosMachineInvenPage::GetItem( INT nPosX, INT nPosY )
{
	return m_pItemSlotArray[ nPosY ]->GetItemImage( nPosX );
}

void CEP7ChaosMachineInvenPage::SetOneViewSlot( const INT& nMAX_ONE_VIEW )
{
	m_nONE_VIEW_SLOT = nMAX_ONE_VIEW;
}