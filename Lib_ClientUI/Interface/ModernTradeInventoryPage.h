//	마켓 윈도우
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.12.6]
//			@ 작성
//

#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLInventory.h"

class	CItemSlotEx;
struct	SINVENITEM;

class	CModernTradeInventoryPage : public CUIGroup
{
private:
static	const	int	nSTARTLINE;
static	const	int	nOUTOFRANGE;

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

public:
	CModernTradeInventoryPage ();
	virtual	~CModernTradeInventoryPage ();

public:
	void	CreateSubControl ();

	void	UpdateBLOCK ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	ResetItemSlotRender ( int nStart, int nEnd );
	void	SetItemSlotRender ( int nStart, int nEnd );

protected:
	CItemSlotEx*	CreateItemSlot ( CString strKeyword, UIGUID ControlID );
	CUIControl*	CreateControl ( const char* szControl );

public:
	void	SetOneViewSlot ( const int& nMAX_ONE_VIEW );
	const int& GetOneViewSlot () const						{ return m_nONE_VIEW_SLOT; }

public:
	void	SetItemIndex ( int nPosX, int nPosY );
	void	GetItemIndex ( int* pnPosX, int* pnPosY );

public:
	void	LoadItemPage ( GLInventory &ItemData );
	void	LoadItem ( SINVENITEM& ref_InvenItem );

	SINVENITEM&	GetItem ( int nPosX, int nPosY );

public:
	void	UnLoadItemPage ();
	void	UnLoadItem ( int nPosX, int nPosY );

public:
	void	ResetAllFlipItem ();
	void	UpdateFlipItem ( GLInventory& ref_TradeInventory );

private:
	CItemSlotEx*	m_pItemSlotArray[EM_INVENSIZE_Y];
	CUIControl*		m_pItemSlotArrayDummy[nMAX_ONE_VIEW_SLOT];

private:
	int		m_nPosX;
	int		m_nPosY;

private:
	int		m_nONE_VIEW_SLOT;
};