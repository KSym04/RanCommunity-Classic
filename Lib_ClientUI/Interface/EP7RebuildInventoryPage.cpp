#include "StdAfx.h"
#include "./EP7RebuildInventoryPage.h"

#include "./ItemSlotEx.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------
// EP7RebuildInventoryPage : 최준혁
//--------------------------------------------------------------------
const INT CEP7RebuildInventoryPage::nSTARTLINE = 0;	// ITEMREBUILD_MARK
const INT CEP7RebuildInventoryPage::nOUTOFRANGE = -1;

CEP7RebuildInventoryPage::CEP7RebuildInventoryPage()
	: m_nONE_VIEW_SLOT( nONE_VIEW_SLOT_DEFAULT )
{
	for( INT i=0; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i] = NULL;

	for( INT i=0; i<nMAX_ONE_VIEW_SLOT; ++i )
		m_pItemSlotArrayDummy[i] = NULL;
}

CEP7RebuildInventoryPage::~CEP7RebuildInventoryPage()
{
}

VOID CEP7RebuildInventoryPage::CreateSubControl()
{
	CString strSlotKeyword[ nMAX_ONE_VIEW_SLOT ] =
	{
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT0",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT1",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT2",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT3",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT4",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT5",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT6",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT7",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT8",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT9",

		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT0",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT1",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT2",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT3",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT4",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT5",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT6",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT7",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT8",
		"EP7_REBUILD_INVENTORYSLOT_ITEM_SLOT9",
	};

	for( INT i=0; i<nMAX_ONE_VIEW_SLOT; ++i )
		m_pItemSlotArrayDummy[i] = CreateControl( strSlotKeyword[i].GetString() );

	for( INT i=0; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i] = CreateItemSlot( strSlotKeyword[0], ITEM_SLOT0 + i );

	SetItemSlotRender( nSTARTLINE, m_nONE_VIEW_SLOT );
}

CItemSlotEx* CEP7RebuildInventoryPage::CreateItemSlot( CString strKeyword, UIGUID ControlID )
{
	CItemSlotEx* pItemSlot = new CItemSlotEx;
	pItemSlot->CreateSub ( this, strKeyword.GetString(), UI_FLAG_DEFAULT, ControlID );
	pItemSlot->CreateSubTradeInventory( EM_INVENSIZE_X );
	RegisterControl( pItemSlot );

	return pItemSlot;
}

CUIControl*	CEP7RebuildInventoryPage::CreateControl( const TCHAR* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub( this, szControl );
	RegisterControl( pControl );
	return pControl;
}

VOID CEP7RebuildInventoryPage::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex( nOUTOFRANGE, nOUTOFRANGE );

	CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	UpdateBLOCK();
}

VOID CEP7RebuildInventoryPage::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
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

VOID CEP7RebuildInventoryPage::ResetItemSlotRender ( int nStart, int nEnd )
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

VOID CEP7RebuildInventoryPage::SetItemSlotRender ( int nStart, int nEnd )
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

VOID CEP7RebuildInventoryPage::SetItemIndex( INT nPosX, INT nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

VOID CEP7RebuildInventoryPage::GetItemIndex( INT* pnPosX, INT* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

VOID CEP7RebuildInventoryPage::LoadItemPage( GLInventory &ItemData )
{
	GLInventory::CELL_MAP* pItemList = ItemData.GetItemList();

	if( pItemList->size() > ( EM_INVENSIZE_X * EM_INVENSIZE_Y ) )
	{
		GASSERT( 0 && "인터페이스 사이즈보다, 데이타크기가 큽니다." );
		return;
	}

	//	삭제
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

	//	등록
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

VOID CEP7RebuildInventoryPage::LoadItem( SINVENITEM& ref_InvenItem )
{	
	INT nPosX = ref_InvenItem.wPosX;
	INT nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[ nPosY ]->SetItemImage( nPosX, ref_InvenItem );
}

VOID CEP7RebuildInventoryPage::UnLoadItemPage()
{
	for( INT y=0; y<EM_INVENSIZE_Y; ++y )
		for( INT x=0; x<EM_INVENSIZE_X; ++x )
			UnLoadItem( x, y );
}

VOID CEP7RebuildInventoryPage::UnLoadItem( INT nPosX, INT nPosY )
{
	m_pItemSlotArray[ nPosY ]->ResetItemImage( nPosX );
}

VOID CEP7RebuildInventoryPage::ResetAllFlipItem()
{
	for( WORD y=0; y<EM_INVENSIZE_Y; ++y )
		for( WORD x=0; x<EM_INVENSIZE_X; ++x )
			m_pItemSlotArray[y]->SetFlipItem( x, FALSE );
}

VOID CEP7RebuildInventoryPage::UpdateFlipItem( GLInventory& ref_Inventory )
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

VOID CEP7RebuildInventoryPage::UpdateFlipItem( SINVEN_POS& ref_RebuildItem )
{
	ResetAllFlipItem();

	if( ref_RebuildItem.wPosY >= 0 && ref_RebuildItem.wPosY < EM_INVENSIZE_Y )
	{
		if( ref_RebuildItem.wPosX >= 0 && ref_RebuildItem.wPosX < EM_INVENSIZE_X )
			m_pItemSlotArray[ ref_RebuildItem.wPosY ]->SetFlipItem( ref_RebuildItem.wPosX, TRUE );
	}
}

VOID CEP7RebuildInventoryPage::UpdateBLOCK()
{
	const WORD& wAddINVENLINE = GLGaeaClient::GetInstance().GetCharacter()->GetOnINVENLINE();
	INT nUSABLE_INVENLINE = EM_INVEN_DEF_SIZE_Y + wAddINVENLINE;

	if( EM_INVENSIZE_Y < nUSABLE_INVENLINE )
	{		
		GASSERT( 0 && "데이타 오류입니다." );

		nUSABLE_INVENLINE = EM_INVENSIZE_Y;
	}

	// 사용 가능
	for( INT i=0; i<nUSABLE_INVENLINE; ++i )
		m_pItemSlotArray[i]->ResetBLOCK();

	// 사용 불가능
	for( INT i=nUSABLE_INVENLINE; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i]->SetBLOCK();
}

SINVENITEM& CEP7RebuildInventoryPage::GetItem( INT nPosX, INT nPosY )
{
	return m_pItemSlotArray[ nPosY ]->GetItemImage( nPosX );
}

VOID CEP7RebuildInventoryPage::SetOneViewSlot( const INT& nMAX_ONE_VIEW )
{
	m_nONE_VIEW_SLOT = nMAX_ONE_VIEW;
}