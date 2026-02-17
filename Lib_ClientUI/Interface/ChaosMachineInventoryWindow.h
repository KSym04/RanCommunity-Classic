/*!
 * \file ChaosMachineInventoryWindow.h
 *
 * \author Juver
 * \date 2021/07/08
 *
 * 
 */

#ifndef CHAOSMACHINEINVENTORYWINDOW_H_INCLUDED__
#define CHAOSMACHINEINVENTORYWINDOW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./UIWindowEx.h"

class CChaosMachineInvenPage;

class CChaosMachineInvenWindow : public CUIWindowEx	
{
protected:
	enum
	{
		CHAOS_MACHINE_INVEN_MONEY_BUTTON = ET_CONTROL_NEXT,
		CHAOS_MACHINE_INVEN_GAME_POINTS_BUTTON,
		CHAOS_MACHINE_INVEN_WAR_CHIPS_BUTTON,
		CHAOS_MACHINE_INVEN_PAGE
	};

private:
	CChaosMachineInvenPage*	m_pPage;
	CBasicTextBox*		m_pMoneyTextBox;
	CBasicTextBox*		m_pGamePointsTextBox;
	CBasicTextBox*		m_pWarChipsTextBox;
	INT					m_nONE_VIEW_SLOT;

public:
	CChaosMachineInvenWindow();
	virtual	~CChaosMachineInvenWindow();

public:
	void CreateSubControl();

public:
	virtual	void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void SetOneViewSlot( const INT& nONE_VIEW_SLOT );
};

#endif // CHAOSMACHINEINVENTORYWINDOW_H_INCLUDED__