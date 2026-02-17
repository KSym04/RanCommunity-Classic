#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"

const DWORD UIMSG_MOUSEIN_MODERN_MARKET_WINDOW_PAGE_ITEMSLOT = UIMSG_USER1;

class	CItemImage;
class	CBasicTextBox;

class	CModernMarketPageItemSlot : public CUIGroup
{
protected:
static	const	int	nOUTOFRANGE;

public:
	enum
	{
		nLIMIT_COLUMN = 10,
		nBLOCK_MAX = 6
	};

protected:
	int			m_nIndex;
	int			m_nMaxColumn;
	SNATIVEID	m_sNPCCrowID;
	DWORD		m_dwNPCChannel;

public:
	CUIControl*		m_pMouseOver;

protected:
	CBasicTextBox*	m_pNumberBoxArray[nLIMIT_COLUMN];

protected:
	CItemImage*		m_pItemImageArray[nLIMIT_COLUMN];	
	SINVENITEM		m_InvenItem[nLIMIT_COLUMN];

private:
	enum
	{
		ITEM_IMAGE0 = NO_ID + 1,
		ITEM_IMAGE1,
		ITEM_IMAGE2,
		ITEM_IMAGE3,
		ITEM_IMAGE4,
		ITEM_IMAGE5,
		ITEM_IMAGE6,
		ITEM_IMAGE7,
		ITEM_IMAGE8,
		ITEM_IMAGE9
	};

public:
	CModernMarketPageItemSlot ();
	virtual	~CModernMarketPageItemSlot ();

public:
	void	CreateSubControl ( int nMaxColumn, BOOL bNumberUse = FALSE );

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CItemImage*		CreateItemImage ( const char* szControl, UIGUID ControlID );
	void	CreateMouseOver ( char* szControl );	
	void	CreateNumberBox ();

private:
	CBasicTextBox*	CreateNumberBox ( const char* szControl );

public:
	void	SetItemImage ( int nIndex, SINVENITEM& ref_InvenItem );
	SINVENITEM&	GetItemImage ( int nIndex )		{ return m_InvenItem[nIndex]; }
	void	ResetItemImage ( int nIndex );

public:
	void	SetItemIndex ( int nIndex )			{ m_nIndex = nIndex; }
	int		GetItemIndex ( )					{ return m_nIndex; }

public:
	void	SetNumber ( int nIndex, int nNumber, int nMaxNumber );
	void	ResetNumber ( int nIndex );

	BOOL	NPCItemCheck( SNATIVEID sID, WORD wNum );

	void	NPCCrowIDSet( SNATIVEID sID );
	void	NPCCrowIDReset()	{	m_sNPCCrowID = NATIVEID_NULL();	}

	void	NPCChannelSet( DWORD dwChannel ){ m_dwNPCChannel = dwChannel; }
};