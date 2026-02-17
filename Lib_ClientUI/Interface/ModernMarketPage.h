#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class	CModernMarketPageItemSlot;
class	GLInventory;
struct	SINVENITEM;

class	CModernMarketPage : public CUIGroup
{
private:
static	const	int	nSTARTLINE;
static	const	int	nOUTOFRANGE;

protected:
	enum
	{		
		nMAX_ONE_VIEW_SLOT = 8
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
		ITEM_SLOT_END = ITEM_SLOT0 + SALE_INVEN_Y,
	};

public:
	CModernMarketPage ();
	virtual	~CModernMarketPage ();

public:
	void CreateSubControl ();

public:
	void	SetItemIndex ( int nPosX, int nPosY );
	void	GetItemIndex ( int* pnPosX, int* pnPosY );

public:
	void	LoadItemPage ( GLInventory &ItemData, SNATIVEID sNPCCrowID, DWORD dwNPCChannel );
	void	LoadItem ( SINVENITEM& ref_InvenItem );

	SINVENITEM&	GetItem ( int nPosX, int nPosY );

public:
	void	UnLoadItemPage ( DWORD dwNPCChannel );
	void	UnLoadItem ( int nPosX, int nPosY );

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

protected:
	void	ResetAllItemSlotRender ( int nTotal );
	void	SetItemSlotRender ( int nStartIndex, int nTotal );

protected:
	CModernMarketPageItemSlot*	CreateItemSlot ( CString strKeyword, UIGUID ControlID );
	CUIControl*	CreateControl ( const char* szControl );

private:
	int		m_nPosX;
	int		m_nPosY;

private:
	CModernMarketPageItemSlot*	m_pItemSlotArray[SALE_INVEN_Y];
	CUIControl*					m_pItemSlotArrayDummy[nMAX_ONE_VIEW_SLOT];
};