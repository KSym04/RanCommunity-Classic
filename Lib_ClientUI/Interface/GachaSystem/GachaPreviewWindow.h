#pragma	once

#include "../UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLItem.h"
#include "../Lib_Client/G-Logic/GLItemDef.h"

class	CGachaPreviewWindowRender;
class	CItemImage;
class	CBasicTextBox;

const DWORD UIMSG_ITEM_SHOP_PREVIEW_SLOT_ITEM_IMAGE = UIMSG_USER1;

class	CGachaPreviewWindow : public CUIWindowEx
{
private:
	enum
	{
		ITEM_PREVIEW_BUTTON_LEFT = ET_CONTROL_NEXT,
		ITEM_PREVIEW_BUTTON_RIGHT,
		ITEM_PREVIEW_STANCE_BUTTON,
		ITEM_PREVIEW_MALE_BUTTON,
		ITEM_PREVIEW_FEMALE_BUTTON
	};

	enum
	{
		ITEMSHOP_PREVIEW_ITEM00 = ITEM_PREVIEW_FEMALE_BUTTON + 1,
		ITEMSHOP_PREVIEW_ITEM01,
		ITEMSHOP_PREVIEW_ITEM02,
		ITEMSHOP_PREVIEW_ITEM03,
		ITEMSHOP_PREVIEW_ITEM04,
		ITEMSHOP_PREVIEW_ITEM05,
		ITEMSHOP_PREVIEW_ITEM06,
		ITEMSHOP_PREVIEW_ITEM07,
		ITEMSHOP_PREVIEW_ITEM08,
		ITEMSHOP_PREVIEW_ITEM09,
		ITEMSHOP_PREVIEW_ITEM_MAX = ITEMSHOP_PREVIEW_ITEM09 - ITEM_PREVIEW_FEMALE_BUTTON
	};

public:
	CGachaPreviewWindow ();
	virtual	~CGachaPreviewWindow ();

public:
	void	CreateSubControl ();

private:
	CGachaPreviewWindowRender*	m_pRender;
	CBasicButton*		m_pButtonLeft;
	CBasicButton*		m_pButtonRight;

	CBasicButton*		m_pStanceButton;
	CBasicButton*		m_pMaleButton;
	CBasicButton*		m_pFemaleButton;

	CBasicTextBox*		m_pStanceText;

	CItemImage*			m_pItems[ITEM::SBOX::ITEM_SIZE];
	SITEMCUSTOM			m_pItemCustom[ITEM::SBOX::ITEM_SIZE];

	SNATIVEID			sSelectedID;
	BOOL				bMaleSelect;
	BOOL				bFemaleSelect;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:

public:
	void	ClearRender();
	void	StartPreview( SITEMCUSTOM sItem );
	bool	PreviewItemCheckWear(SNATIVEID sID);

	SNATIVEID	GetSelectedID() { return sSelectedID;  };
};