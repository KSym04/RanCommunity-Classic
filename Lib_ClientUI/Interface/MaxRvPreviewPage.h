/*!
 * \file MaxRvPreviewPage.h
 *
 * \author Juver
 * \date 2022/02/25
 *
 * 
 */

#ifndef MAXRVPREVIEWPAGE_H_INCLUDED__
#define MAXRVPREVIEWPAGE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLMaxRvPreview.h"

class CBasicScrollBarEx;
class CItemImage;
class CBasicTextBoxEx;
class CMaxRvPreviewPageSlot;

class CMaxRvPreviewPage : public CUIGroup
{
private:
	enum
	{
		MAXRV_PREVIEW_PAGE_ITEM_IMAGE = NO_ID + 1,
		MAXRV_PREVIEW_PAGE_SCROLLBAR,
		MAXRV_PREVIEW_PAGE_SLOT_00,
		MAXRV_PREVIEW_PAGE_SLOT_01,
		MAXRV_PREVIEW_PAGE_SLOT_02,
		MAXRV_PREVIEW_PAGE_SLOT_03,
		MAXRV_PREVIEW_PAGE_SLOT_04,
		MAXRV_PREVIEW_PAGE_SLOT_05,
		MAXRV_PREVIEW_PAGE_SLOT_06,
		MAXRV_PREVIEW_PAGE_SLOT_07,
		MAXRV_PREVIEW_PAGE_SLOT_08,
		MAXRV_PREVIEW_PAGE_SLOT_09,
	};

	enum
	{
		MAXRV_PREVIEW_PAGE_MAX_SLOT = 10,
	};

public:
	CMaxRvPreviewPage ();
	virtual	~CMaxRvPreviewPage ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	EMMAXRV_PREVIEW_TYPE m_emType;
	CBasicScrollBarEx* m_pScrollBar;
	CItemImage*	m_pItemImage;
	CBasicTextBoxEx* m_pItemName;

	CMaxRvPreviewPageSlot* m_pSlot[MAXRV_PREVIEW_PAGE_MAX_SLOT];

public:
	int m_nStart;
	int	m_nTotal;

public:
	void SetPreviewType( EMMAXRV_PREVIEW_TYPE emType ) { m_emType = emType; }
	void ShowItemData();
	void ResetItemData();
	void RenderView();
};

#endif // MAXRVPREVIEWPAGE_H_INCLUDED__