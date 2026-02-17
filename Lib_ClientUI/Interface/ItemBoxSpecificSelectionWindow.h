/*!
 * \file ItemBoxSpecificSelectionWindow.h
 *
 * \author Juver
 * \date September 2018
 *
 * 
 */

#pragma	once

#include "./UIWindowEx.h"

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Client/G-Logic/GLItem.h"
#include "../../Lib_Client/G-Logic/GLItemBox.h"

class CBasicTextBox;
class CItemImage;
class CBasicTextButton;

class CItemBoxSpecificSelectionWindow: public CUIWindowEx
{
public:
	enum
	{
		ITEM_BOX_SPECIFIC_SELECTION_BUTTON_CLOSE = ET_CONTROL_NEXT + 1,
		ITEM_BOX_SPECIFIC_SELECTION_BUTTON_OPEN,

		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_0,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_1,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_2,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_3,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_4,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_5,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_6,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_7,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_8,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_9,

		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_10,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_11,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_12,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_13,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_14,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_15,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_16,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_17,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_18,
		ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_19,

		ITEM_BOX_SPECIFIC_SELECTION_ITEM_SELECTED,
	};

public:
	CItemBoxSpecificSelectionWindow ();
	virtual	~CItemBoxSpecificSelectionWindow ();


public:
	void	CreateSubControl ();

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:
	CBasicTextBox*	m_pui_item_main_name;
	CItemImage*		m_pui_item_image_box[ITEM::SBOX::ITEM_SIZE];
	CUIControl*		m_pui_mouse_over;

	CItemImage*		m_pui_item_image_selected;
	CBasicTextBox*	m_pui_item_name_selected;

	CBasicTextButton*	m_pui_button_open;
	CBasicTextButton*	m_pui_button_close;

private:
	SNATIVEID		m_item_id;
	SNATIVEID		m_item_id_box[ITEM::SBOX::ITEM_SIZE];
	DWORD			m_item_box_num[ITEM::SBOX::ITEM_SIZE];

	SNATIVEID		m_item_id_selected;
	DWORD			m_item_selected_num;

	SNATIVEID		m_item_id_selected_old;
	DWORD			m_item_selected_num_old;
	int				m_item_selected_index;

public:
	void reset_info();
	void selection_show( SITEM* pitem_data );
	void selection_open();
};