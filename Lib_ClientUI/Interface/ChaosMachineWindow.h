/*!
 * \file ChaosMachineWindow.h
 *
 * \author Juver
 * \date 2021/07/07
 *
 * 
 */

#ifndef CHAOSMACHINEWINDOW_H_INCLUDED__
#define CHAOSMACHINEWINDOW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./UIWindowEx.h"

#include "../Lib_Client/G-Logic/GLChaosMachineDefine.h"

class CItemImage;
class CBasicTextBox;
class CBasicTextButton;

class CChaosMachineWindow : public CUIWindowEx
{
protected:
	enum
	{
		CHAOS_MACHINE_ITEM_IMAGE_MAIN = ET_CONTROL_NEXT,
		CHAOS_MACHINE_ITEM_IMAGE_REQ_0,
		CHAOS_MACHINE_ITEM_IMAGE_REQ_1,
		CHAOS_MACHINE_ITEM_IMAGE_REQ_2,
		CHAOS_MACHINE_BUTTON_START,
		CHAOS_MACHINE_BUTTON_CANCEL,
	};

public:
	CChaosMachineWindow ();
	virtual	~CChaosMachineWindow ();

public:
	void	CreateSubControl ();

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	CItemImage*			m_pItemImageMain;
	CItemImage*			m_pItemImageRequire[CHAOS_MACHINE_MATERIAL_NUM];
	CBasicTextBox*		m_pItemReqCountText[CHAOS_MACHINE_MATERIAL_NUM];

	CBasicTextBox*		m_pMainItemInfo;
	CBasicTextBox*		m_pMainItemCost;
	CBasicTextBox*		m_pMainItemRate;

	CBasicTextBox*		m_pReqItemInfo;
	
	CBasicTextButton*	m_pStartButton;
	CBasicTextButton*	m_pCancelButton;

public:
	void UpdateInfo();
	void ChaosMachineStart();
};




#endif // CHAOSMACHINEWINDOW_H_INCLUDED__
