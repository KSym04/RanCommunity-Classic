/*!
 * \file ItemRandomOptionRebuildWindow.h
 *
 * \author Juver
 * \date July 2018
 *
 * 
 */

#pragma	once

#include "./UIWindowEx.h"

class CBasicTextBox;
class CItemImage;
class CBasicTextButton;

class CItemImage;
class CBasicTextBox;
class CBasicTextButton;
class CBasicButton;

class CItemRandomOptionRebuildWindow : public CUIWindowEx	
{
protected:
	enum
	{
		ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_IMAGE_TARGET = ET_CONTROL_NEXT,
		ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_IMAGE_STAMP,
		
		ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_0,
		ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_1,
		ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_2,
		ITEM_RANDOM_OPTION_REBUILD_WINDOW_ITEM_OPTION_CHECK_3,

		ITEM_RANDOM_OPTION_REBUILD_WINDOW_BUTTON_START,
		ITEM_RANDOM_OPTION_REBUILD_WINDOW_BUTTON_CANCEL,
	};

protected:
	
public:

public:
	CItemRandomOptionRebuildWindow();
	virtual	~CItemRandomOptionRebuildWindow();

public:
	void CreateSubControl();

public:
	CItemImage*		m_pItemImageTarget;
	CItemImage*		m_pItemImageStamp;
	CBasicTextBox*	m_pHelpTextBox;
	CBasicTextButton* m_pButtonStart;
	CBasicTextButton* m_pButtonCancel;

	CBasicButton*	m_pOptionCheck0;
	CBasicButton*	m_pOptionCheck1;
	CBasicButton*	m_pOptionCheck2;
	CBasicButton*	m_pOptionCheck3;

	CBasicTextBox*	m_pOptionText0;
	CBasicTextBox*	m_pOptionText1;
	CBasicTextBox*	m_pOptionText2;
	CBasicTextBox*	m_pOptionText3;

	bool			m_last_valid_check;

public:
	virtual	void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual void SetVisibleSingle( BOOL bVisible );

};