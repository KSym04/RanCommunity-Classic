#include "StdAfx.h"
#include "./ModernInventoryWindowInven.h"

#include "./ItemSlot.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CModernInventoryWindowInven::nSTARTLINE = 0;
const int CModernInventoryWindowInven::nOUTOFRANGE = -1;

CModernInventoryWindowInven::CModernInventoryWindowInven () :
	m_nONE_VIEW_SLOT ( nONE_VIEW_SLOT_DEFAULT )
{
	memset ( m_pItemSlotArray, 0, sizeof( m_pItemSlotArray ) );
	memset ( m_pItemSlotArrayDummy, 0, sizeof( m_pItemSlotArrayDummy ) );
}

CModernInventoryWindowInven::~CModernInventoryWindowInven ()
{
}

CUIControl*	CModernInventoryWindowInven::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CModernInventoryWindowInven::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8 );

	CString strSlotKeyword[nMAX_ONE_VIEW_SLOT] =
	{
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT0",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT1",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT2",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT3",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT4",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT5",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT6",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT7",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT8",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT9",

		"MODERN_INVENTORY_WINDOW_ITEM_SLOT0",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT1",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT2",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT3",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT4",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT5",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT6",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT7",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT8",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT9",

		"MODERN_INVENTORY_WINDOW_ITEM_SLOT0",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT1",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT2",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT3",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT4",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT5",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT6",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT7",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT8",
		"MODERN_INVENTORY_WINDOW_ITEM_SLOT9",
	};

	for ( int i = 0; i < nMAX_ONE_VIEW_SLOT; i++ )
	{
		m_pItemSlotArrayDummy[i] = CreateControl ( strSlotKeyword[i].GetString () );
	}
	
	for ( int i = 0; i < EM_INVENSIZE_Y_FORCE; i++ )
	{
		m_pItemSlotArray[i] = CreateItemSlot ( strSlotKeyword[0], ITEM_SLOT0 + i );
	}
	
	SetItemSlotRender ( nSTARTLINE, nMAX_ONE_VIEW_SLOT );
}

void CModernInventoryWindowInven::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex ( nOUTOFRANGE, nOUTOFRANGE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	UpdateBLOCK ();
}

void CModernInventoryWindowInven::ResetItemSlotRender ( int nStart, int nEnd )
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

void CModernInventoryWindowInven::SetItemSlotRender ( int nStart, int nEnd )
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

CItemSlot* CModernInventoryWindowInven::CreateItemSlot ( CString strKeyword, UIGUID ControlID )
{
	CItemSlot* pItemSlot = new CItemSlot;
	pItemSlot->CreateSub ( this, strKeyword.GetString (), UI_FLAG_DEFAULT, ControlID );
	pItemSlot->CreateSubControl ( EM_INVENSIZE_X, TRUE );	
	RegisterControl ( pItemSlot );
	return pItemSlot;
}

void CModernInventoryWindowInven::SetItemIndex ( int nPosX, int nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void CModernInventoryWindowInven::GetItemIndex ( int* pnPosX, int* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

void CModernInventoryWindowInven::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( ITEM_SLOT0 <= ControlID && ControlID < ITEM_SLOT_END )
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			const int nPosY = ControlID - ITEM_SLOT0;
			const int nPosX = m_pItemSlotArray[nPosY]->GetItemIndex ();

			if ( nPosY < 0 || nPosX < 0 ) return ;

			SetItemIndex ( nPosX, nPosY );
		}
	}
}

void CModernInventoryWindowInven::LoadItem ( SINVENITEM& ref_InvenItem )
{	
	int nPosX = ref_InvenItem.wPosX;
	int nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[nPosY]->SetItemImage ( nPosX, ref_InvenItem );
}

void CModernInventoryWindowInven::UnLoadItemPage ()
{
	for ( int y = 0; y < EM_INVENSIZE_Y_FORCE; y++ )
	{
		for ( int x = 0; x < EM_INVENSIZE_X; x++ )
		{
			UnLoadItem ( x, y );
		}
	}
}

SINVENITEM& CModernInventoryWindowInven::GetItem ( int nPosX, int nPosY )
{
	return m_pItemSlotArray[nPosY]->GetItemImage ( nPosX );
}

void CModernInventoryWindowInven::UnLoadItem ( int nPosX, int nPosY )
{
	m_pItemSlotArray[nPosY]->ResetItemImage ( nPosX );
}

void CModernInventoryWindowInven::LoadItemPage ( GLInventory &ItemData )
{
	GLInventory::CELL_MAP *pItemList = ItemData.GetItemList();
	
	if ( pItemList->size () > (EM_INVENSIZE_Y_FORCE * EM_INVENSIZE_X) )
	{
		GASSERT ( 0 && "인터페이스 사이즈보다, 데이타크기가 큽니다." );
		return ;
	}

	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for ( WORD y = 0; y < EM_INVENSIZE_Y_FORCE; y++ )
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

	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for ( ; iter!=iter_end; ++iter )
		{
			SINVENITEM* pInvenItem = (*iter).second;
			SINVENITEM& ref_InvenItemOld = GetItem ( pInvenItem->wPosX, pInvenItem->wPosY );
			if ( pInvenItem->sItemCustom != ref_InvenItemOld.sItemCustom )
			{
				LoadItem ( *pInvenItem );
			}	
		}
	}
}

void CModernInventoryWindowInven::UpdateBLOCK ()
{
	const WORD& wAddINVENLINE = GLGaeaClient::GetInstance().GetCharacter()->GetOnINVENLINE();
	int nUSABLE_INVENLINE = EM_INVEN_DEF_SIZE_Y + wAddINVENLINE;

	if ( EM_INVENSIZE_Y_FORCE < nUSABLE_INVENLINE )
	{		
		GASSERT ( 0 && "데이타 오류입니다." );

		nUSABLE_INVENLINE = EM_INVENSIZE_Y_FORCE;
	}
	
	for ( int i = 0; i < nUSABLE_INVENLINE; i++ )
		m_pItemSlotArray[i]->ResetBLOCK ();

	for ( int i = nUSABLE_INVENLINE; i < EM_INVENSIZE_Y; i++ )
		m_pItemSlotArray[i]->SetBLOCK ();
}

void CModernInventoryWindowInven::SetOneViewSlot ( const int& nMAX_ONE_VIEW )
{
	m_nONE_VIEW_SLOT = nMAX_ONE_VIEW;
}

