#include "StdAfx.h"
#include "./ModernTradeInventoryPage.h"
#include "./ItemSlotEx.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLInventory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CModernTradeInventoryPage::nSTARTLINE = 0;
const int CModernTradeInventoryPage::nOUTOFRANGE = -1;

CModernTradeInventoryPage::CModernTradeInventoryPage () :
	m_nONE_VIEW_SLOT ( nONE_VIEW_SLOT_DEFAULT )
{
	SecureZeroMemory ( m_pItemSlotArray, sizeof( m_pItemSlotArray ) );
	SecureZeroMemory ( m_pItemSlotArrayDummy, sizeof( m_pItemSlotArrayDummy ) );
}

CModernTradeInventoryPage::~CModernTradeInventoryPage ()
{
}

CUIControl*	CModernTradeInventoryPage::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );

	return pControl;
}

void CModernTradeInventoryPage::CreateSubControl ()
{
	CString strSlotKeyword[nMAX_ONE_VIEW_SLOT] =
	{
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT0",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT1",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT2",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT3",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT4",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT5",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT6",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT7",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT8",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT9",

		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT0",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT1",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT2",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT3",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT4",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT5",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT6",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT7",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT8",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT9",

		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT0",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT1",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT2",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT3",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT4",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT5",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT6",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT7",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT8",
		"MODERN_TRADE_INVENTORY_WINDOW_ITEM_SLOT9",
	};

	//	더미 슬롯
	for ( int i = 0; i < nMAX_ONE_VIEW_SLOT; i++ )
	{
		m_pItemSlotArrayDummy[i] = CreateControl ( strSlotKeyword[i].GetString () );
	}
	
	//	실제 데이타
	for ( int i = 0; i < EM_INVENSIZE_Y; i++ )
	{
		m_pItemSlotArray[i] = CreateItemSlot ( strSlotKeyword[0], ITEM_SLOT0 + i );
	}
	
	SetItemSlotRender ( nSTARTLINE, m_nONE_VIEW_SLOT );
}

void CModernTradeInventoryPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex ( nOUTOFRANGE, nOUTOFRANGE );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	//	NOTE
	//		사용/가부 설정
	UpdateBLOCK ();
}

void CModernTradeInventoryPage::ResetItemSlotRender( int nStart, int nEnd )
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

void CModernTradeInventoryPage::SetItemSlotRender ( int nStart, int nEnd  )
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

CItemSlotEx* CModernTradeInventoryPage::CreateItemSlot ( CString strKeyword, UIGUID ControlID )
{
	CItemSlotEx* pItemSlot = new CItemSlotEx;
	pItemSlot->CreateSub ( this, strKeyword.GetString (), UI_FLAG_DEFAULT, ControlID );
	pItemSlot->CreateSubTradeInventory ( EM_INVENSIZE_X );
	RegisterControl ( pItemSlot );

	return pItemSlot;
}

void CModernTradeInventoryPage::SetItemIndex ( int nPosX, int nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void CModernTradeInventoryPage::GetItemIndex ( int* pnPosX, int* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

void CModernTradeInventoryPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( ITEM_SLOT0 <= ControlID && ControlID < ITEM_SLOT_END )
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			const int nPosY = ControlID - ITEM_SLOT0;
			CItemSlot* pSlot = m_pItemSlotArray[nPosY];
			const int nPosX = pSlot->GetItemIndex ();

			if ( nPosY < 0 || nPosX < 0 ) return ;

			SetItemIndex ( nPosX, nPosY );
		}
	}
}

void CModernTradeInventoryPage::LoadItem ( SINVENITEM& ref_InvenItem )
{	
	int nPosX = ref_InvenItem.wPosX;
	int nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[nPosY]->SetItemImage ( nPosX, ref_InvenItem );
}

void CModernTradeInventoryPage::UnLoadItemPage ()
{
	for ( int y = 0; y < EM_INVENSIZE_Y; y++ )
	{		
		for ( int x = 0; x < EM_INVENSIZE_X; x++ )
		{
			UnLoadItem ( x, y );
		}
	}
}

SINVENITEM& CModernTradeInventoryPage::GetItem ( int nPosX, int nPosY )
{
	return m_pItemSlotArray[nPosY]->GetItemImage ( nPosX );
}

void CModernTradeInventoryPage::UnLoadItem ( int nPosX, int nPosY )
{
	m_pItemSlotArray[nPosY]->ResetItemImage ( nPosX );
}

void CModernTradeInventoryPage::LoadItemPage ( GLInventory &ItemData )
{
	GLInventory::CELL_MAP *pItemList = ItemData.GetItemList();

	if ( pItemList->size () > (EM_INVENSIZE_X * EM_INVENSIZE_Y) )
	{
		GASSERT ( 0 && "인터페이스 사이즈보다, 데이타크기가 큽니다." );
		return ;
	}


	{	//	삭제

		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for ( WORD y = 0; y < EM_INVENSIZE_Y; y++ )
		{
			for ( WORD x = 0; x < EM_INVENSIZE_X; x++ )
			{
				SINVENITEM& ref_InvenItem = GetItem ( x, y );
				if ( ref_InvenItem.sItemCustom.sNativeID != NATIVEID_NULL() )
				{
					GLInventory::CELL_KEY foundkey ( x, y );
					GLInventory::CELL_MAP_ITER found = pItemList->find ( foundkey );
					if ( found == iter_end )
					{
						UnLoadItem ( x, y );
					}
				}
			}
		}
	}

	{	//	등록
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for ( ; iter!=iter_end; ++iter )
		{
			SINVENITEM* pInvenItem = (*iter).second;
			SINVENITEM& ref_InvenItemOld = GetItem ( pInvenItem->wPosX, pInvenItem->wPosY );
			if ( *pInvenItem != ref_InvenItemOld )
			{
				LoadItem ( *pInvenItem );
			}	
		}
	}
}

void  CModernTradeInventoryPage::UpdateFlipItem ( GLInventory& ref_TradeInventory )
{
	GLInventory::CELL_MAP_ITER iter = ref_TradeInventory.GetItemList()->begin();
	GLInventory::CELL_MAP_ITER iter_end = ref_TradeInventory.GetItemList()->end();

	ResetAllFlipItem ();
	for ( ; iter != iter_end; iter++ )
	{
		SINVENITEM* pInvenItem = (*iter).second;
		if ( pInvenItem )
		{
			int nPosX = pInvenItem->wBackX;
			int nPosY = pInvenItem->wBackY;
			m_pItemSlotArray[nPosY]->SetFlipItem ( nPosX, TRUE );
		}
	}
}

void CModernTradeInventoryPage::ResetAllFlipItem ()
{
	for ( WORD y = 0; y < EM_INVENSIZE_Y; y++ )
	{
		for ( WORD x = 0; x < EM_INVENSIZE_X; x++ )
		{
			m_pItemSlotArray[y]->SetFlipItem ( x, FALSE );
		}
	}
}

void CModernTradeInventoryPage::UpdateBLOCK ()
{
	const WORD& wAddINVENLINE = GLGaeaClient::GetInstance().GetCharacter()->GetOnINVENLINE();
	int nUSABLE_INVENLINE = EM_INVEN_DEF_SIZE_Y + wAddINVENLINE;

	if ( EM_INVENSIZE_Y < nUSABLE_INVENLINE )
	{		
		GASSERT ( 0 && "데이타 오류입니다." );

		nUSABLE_INVENLINE = EM_INVENSIZE_Y;
	}
	
	//	사용 가능
	for ( int i = 0; i < nUSABLE_INVENLINE; i++ )
		m_pItemSlotArray[i]->ResetBLOCK ();

	//	사용 불가능
	for ( int i = nUSABLE_INVENLINE; i < EM_INVENSIZE_Y; i++ )
		m_pItemSlotArray[i]->SetBLOCK ();
}

void CModernTradeInventoryPage::SetOneViewSlot ( const int& nMAX_ONE_VIEW )
{
	m_nONE_VIEW_SLOT = nMAX_ONE_VIEW;
}