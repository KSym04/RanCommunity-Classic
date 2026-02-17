/*!
 * \file ChaosMachineInventoryPage.h
 *
 * \author Juver
 * \date 2021/07/08
 *
 * 
 */

#ifndef CHAOSMACHINEINVENTORYPAGE_H_INCLUDED__
#define CHAOSMACHINEINVENTORYPAGE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLInventory.h"

class CItemSlotEx;
class CBasicScrollBarEx;

class CChaosMachineInvenPage : public CUIGroup	
{
private:
	static const INT	nSTARTLINE;
	static const INT	nOUTOFRANGE;

protected:
	enum
	{
		nONE_VIEW_SLOT_DEFAULT = 4,
		nMAX_ONE_VIEW_SLOT = 10
	};

	enum
	{
		ITEM_SLOT0 = NO_ID + 1,
		ITEM_SLOT1,
		ITEM_SLOT_END = ITEM_SLOT0 + EM_INVENSIZE_Y
	};

private:
	CItemSlotEx*		m_pItemSlotArray[ EM_INVENSIZE_Y ];
	CUIControl*			m_pItemSlotArrayDummy[ nMAX_ONE_VIEW_SLOT ];
	CBasicScrollBarEx*	m_pScrollBar;
	int					m_nPosX;
	int					m_nPosY;
	int					m_nCurPos;
	int					m_nONE_VIEW_SLOT;

public:
	CChaosMachineInvenPage();
	virtual	~CChaosMachineInvenPage();

public:
	void CreateSubControl();

protected:
	CItemSlotEx* CreateItemSlot( CString strKeyword, UIGUID ControlID );
	CUIControl* CreateControl( const TCHAR* szControl );

public:
	virtual void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );

protected:
	void ResetAllItemSlotRender( INT nTotal );
	void SetItemSlotRender( INT nStartIndex, INT nTotal );

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


#endif // CHAOSMACHINEINVENTORYPAGE_H_INCLUDED__