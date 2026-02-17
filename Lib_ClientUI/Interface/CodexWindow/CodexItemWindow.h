/*!
 * \file CodexItemWindow.h
 *
 * \author Juver
 * \date November 2017
 *
 * 
 */

#pragma once

#include "../Lib_ClientUI/Interface/UIWindowEx.h"

class CBasicTextBox;
class CBasicTextButton;
class CCodexItemList;

class CCodexItemWindow : public CUIWindowEx
{
	enum
	{
		ACTIVITY_ITEM_WINDOW_BUTTON_CLOSE = ET_CONTROL_NEXT,
		ACTIVITY_ITEM_WINDOW_ITEM_LIST,
	};

public:
	CCodexItemWindow(void);
	virtual ~CCodexItemWindow(void);

public:
	void CreateSubControl();

public:
	CBasicTextBox*		m_pTextDesc;
	CBasicTextButton*	m_pButtonClose;
	CCodexItemList*	m_pItemList;

public:
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

};