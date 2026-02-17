#pragma	once

#include "./UIWindowEx.h"
#include "../../Lib_Client/G-Logic/GLItemDef.h"

class CItemImage;
class CBasicTextButton;

//--------------------------------------------------------------------
// CItemGarbage : 
//--------------------------------------------------------------------
class CItemGarbage : public CUIWindowEx	// »ﬁ¡ˆ≈Î
{
protected:
	enum
	{
		ITEM_GARBAGE_ITEM_SLOT_00 = ET_CONTROL_NEXT,
		ITEM_GARBAGE_ITEM_SLOT_01,
		ITEM_GARBAGE_ITEM_SLOT_02,
		ITEM_GARBAGE_ITEM_SLOT_03,
		ITEM_GARBAGE_ITEM_SLOT_04,
		ITEM_GARBAGE_OK_BUTTON,
		ITEM_GARBAGE_CANCEL_BUTTON
	};

protected:
	CItemImage*			m_pItemImage[ITEM_GARBAGE_MAX_SLOT];
	CBasicTextButton*	m_pOkButton;
	CBasicTextButton*	m_pCancelButton;

public:
	CItemGarbage();
	virtual	~CItemGarbage();

public:
	VOID CreateSubControl();

public:
	virtual	VOID Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	VOID TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
};