#include "StdAfx.h"
#include "./ModernMarketPage.h"
#include "./ModernMarketPageItemSlot.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CModernMarketPage::nSTARTLINE = 0;
const int CModernMarketPage::nOUTOFRANGE = -1;

CModernMarketPage::CModernMarketPage ()
{
	memset ( m_pItemSlotArray, 0, sizeof ( CModernMarketPageItemSlot* ) * SALE_INVEN_Y );
	memset ( m_pItemSlotArrayDummy, 0, sizeof ( CUIControl* ) * nMAX_ONE_VIEW_SLOT );
}

CModernMarketPage::~CModernMarketPage ()
{
}

CUIControl*	CModernMarketPage::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CModernMarketPage::CreateSubControl ()
{
	CString strSlotKeyword[nMAX_ONE_VIEW_SLOT] =
	{
		"MARKET_ITEM_SLOT0",
		"MARKET_ITEM_SLOT1",
		"MARKET_ITEM_SLOT2",
		"MARKET_ITEM_SLOT3",
		"MARKET_ITEM_SLOT4",
		"MARKET_ITEM_SLOT5",
		"MARKET_ITEM_SLOT6",
		"MARKET_ITEM_SLOT7",
	};

	{
		for ( int i = 0; i < nMAX_ONE_VIEW_SLOT; i++ )
		{
			m_pItemSlotArrayDummy[i] = CreateControl ( strSlotKeyword[i].GetString () );
		}
	}

	const int nExampleCount = 8;
	{
		for ( int i = 0; i < nExampleCount; i++ )
		{
			m_pItemSlotArray[i] = CreateItemSlot ( strSlotKeyword[0], ITEM_SLOT0 + i );
		}
	}

	SetItemSlotRender ( nSTARTLINE, nMAX_ONE_VIEW_SLOT );
}

void CModernMarketPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex ( nOUTOFRANGE, nOUTOFRANGE );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernMarketPage::ResetAllItemSlotRender ( int nTotal )
{
	if ( nTotal < 0 ) return ;

	for ( int i = 0; i < nTotal; i++ )
	{
		CModernMarketPageItemSlot* pItemSlot = m_pItemSlotArray[i];
		if ( pItemSlot )
		{
			pItemSlot->SetVisibleSingle ( FALSE );
		}
	}
}

void CModernMarketPage::SetItemSlotRender ( int nStartIndex, int nTotal )
{
	if ( nTotal < 0 ) return ;

	const UIRECT& rcParentPos = GetGlobalPos ();

	for ( int i = nStartIndex; i < nTotal; i++ )
	{
		CModernMarketPageItemSlot* pItemSlot = m_pItemSlotArray[i];
		if ( pItemSlot )
		{
			int nAbsoluteIndex = i - nStartIndex;

			CUIControl* pDummyControl = m_pItemSlotArrayDummy[nAbsoluteIndex];			
			const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos ();
			const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos ();

			pItemSlot->SetLocalPos ( rcSlotLocalPos );
			pItemSlot->SetGlobalPos ( rcSlotPos );

			pItemSlot->SetVisibleSingle ( TRUE );
		}
	}
}

CModernMarketPageItemSlot* CModernMarketPage::CreateItemSlot ( CString strKeyword, UIGUID ControlID )
{
	CModernMarketPageItemSlot* pItemSlot = new CModernMarketPageItemSlot;
	pItemSlot->CreateSub ( this, strKeyword.GetString (), UI_FLAG_DEFAULT, ControlID );
	pItemSlot->CreateSubControl ( SALE_INVEN_X );
	RegisterControl ( pItemSlot );
	return pItemSlot;
}

void CModernMarketPage::SetItemIndex ( int nPosX, int nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void CModernMarketPage::GetItemIndex ( int* pnPosX, int* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

void CModernMarketPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( ITEM_SLOT0 <= ControlID && ControlID < ITEM_SLOT_END )
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			const int nPosY = ControlID - ITEM_SLOT0;
			CModernMarketPageItemSlot* pSlot = m_pItemSlotArray[nPosY];
			const int nPosX = pSlot->GetItemIndex ();

			SetItemIndex ( nPosX, nPosY );
		}
	}
}

void CModernMarketPage::LoadItem ( SINVENITEM& ref_InvenItem )
{	
	int nPosX = ref_InvenItem.wPosX;
	int nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[nPosY]->SetItemImage ( nPosX, ref_InvenItem );
}

void CModernMarketPage::LoadItemPage ( GLInventory &ItemData, SNATIVEID sNPCCrowID, DWORD dwNPCChannel )
{
	GLInventory::CELL_MAP *ItemList = ItemData.GetItemList();

	if ( ItemList->size () > (SALE_INVEN_Y * SALE_INVEN_X) )
	{
		GASSERT ( 0 && "CModernMarketPage::LoadItemPage. ItemList->size () > (SALE_INVEN_Y * SALE_INVEN_X) " );
		return ;
	}

	GLInventory::CELL_MAP_ITER iter = ItemList->begin();
	GLInventory::CELL_MAP_ITER iter_end = ItemList->end();

	for ( ; iter!=iter_end; ++iter )
	{
		SINVENITEM* pInvenItem = (*iter).second;
		SITEMCUSTOM &ref_ItemCustom = pInvenItem->sItemCustom;
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( ref_ItemCustom.sNativeID );
		if ( !pItemData )	continue;
		
		LoadItem ( *pInvenItem );
	}

	for ( int i = 0; i < SALE_INVEN_Y; i++ )
	{
		if ( m_pItemSlotArray[i] )
		{
			m_pItemSlotArray[i]->NPCCrowIDSet( sNPCCrowID );
			m_pItemSlotArray[i]->NPCChannelSet( dwNPCChannel );
		}
	}
}


void CModernMarketPage::UnLoadItemPage ( DWORD dwNPCChannel )
{
	for ( int y = 0; y < SALE_INVEN_Y; y++ )
	{
		for ( int x = 0; x < SALE_INVEN_X; x++ )
		{
			UnLoadItem ( x, y );
		}
	}

	for ( int i = 0; i < SALE_INVEN_Y; i++ )
	{
		if ( m_pItemSlotArray[i] )
		{
			m_pItemSlotArray[i]->NPCCrowIDReset();
			m_pItemSlotArray[i]->NPCChannelSet( dwNPCChannel );
		}
	}
}

SINVENITEM& CModernMarketPage::GetItem ( int nPosX, int nPosY )
{
	return m_pItemSlotArray[nPosY]->GetItemImage ( nPosX );
}

void CModernMarketPage::UnLoadItem ( int nPosX, int nPosY )
{
	m_pItemSlotArray[nPosY]->ResetItemImage ( nPosX );
}