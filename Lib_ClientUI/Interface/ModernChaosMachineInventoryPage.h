/*!
 * \file ModernChaosMachineInventoryPage.h
 *
 * \author Juver
 * \date 2021/07/08
 *
 * 
 */

#ifndef MODERNCHAOSMACHINEINVENTORYPAGE_H_INCLUDED__
#define MODERNCHAOSMACHINEINVENTORYPAGE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLInventory.h"

class CItemSlotEx;

class CModernChaosMachineInvenPage : public CUIGroup	
{
private:
	static const INT	nSTARTLINE;
	static const INT	nOUTOFRANGE;

protected:
	enum
	{
		nONE_VIEW_SLOT_DEFAULT = 10,
		nMAX_ONE_VIEW_SLOT = 30,
	};

	enum
	{
		ITEM_SLOT0 = NO_ID + 1,
		ITEM_SLOT1,
		ITEM_SLOT2,
		ITEM_SLOT3,
		ITEM_SLOT4,
		ITEM_SLOT5,
		ITEM_SLOT6,
		ITEM_SLOT7,
		ITEM_SLOT8,
		ITEM_SLOT9,
		ITEM_SLOT10,
		ITEM_SLOT11,
		ITEM_SLOT12,
		ITEM_SLOT13,
		ITEM_SLOT14,
		ITEM_SLOT15,
		ITEM_SLOT16,
		ITEM_SLOT17,
		ITEM_SLOT18,
		ITEM_SLOT19,
		ITEM_SLOT20,
		ITEM_SLOT21,
		ITEM_SLOT22,
		ITEM_SLOT23,
		ITEM_SLOT24,
		ITEM_SLOT25,
		ITEM_SLOT26,
		ITEM_SLOT27,
		ITEM_SLOT28,
		ITEM_SLOT29,
		ITEM_SLOT_END = ITEM_SLOT0 + EM_INVENSIZE_Y
	};

private:
	CItemSlotEx*		m_pItemSlotArray[ EM_INVENSIZE_Y ];
	CUIControl*			m_pItemSlotArrayDummy[ nMAX_ONE_VIEW_SLOT ];
	int					m_nPosX;
	int					m_nPosY;
	int					m_nONE_VIEW_SLOT;

public:
	CModernChaosMachineInvenPage();
	virtual	~CModernChaosMachineInvenPage();

public:
	void CreateSubControl();

protected:
	CItemSlotEx* CreateItemSlot( CString strKeyword, UIGUID ControlID );
	CUIControl* CreateControl( const TCHAR* szControl );

public:
	virtual void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );

public:
	void	ResetItemSlotRender ( int nStart, int nEnd );
	void	SetItemSlotRender ( int nStart, int nEnd );

public:
	void SetItemIndex( INT nPosX, INT nPosY );
	void GetItemIndex( INT* pnPosX, INT* pnPosY );

public:
	void LoadItemPage( GLInventory &ItemData );
	void LoadItem( SINVENITEM& ref_InvenItem );

public:
	void UnLoadItemPage();
	void UnLoadItem( INT nPosX, INT nPosY );

public:
	void ResetAllFlipItem();
	void UpdateFlipItem( GLInventory& ref_Inventory );
	void UpdateFlipItem( SINVEN_POS& ref_RebuildItem );

public:
	void UpdateBLOCK();
	SINVENITEM&	GetItem( INT nPosX, INT nPosY );

public:
	void SetOneViewSlot( const INT& nMAX_ONE_VIEW );
};

#endif // MODERNCHAOSMACHINEINVENTORYPAGE_H_INCLUDED__