#pragma once

#include "../../Lib_Engine/G-Logic/GLDefine.h"

#include "./UIWindowEx.h"

class CMultiModeButton;
class CMaxRvPreviewPage;

class CMaxRvPreviewWindow : public CUIWindowEx
{
	enum
	{
		MAXRV_PREVIEW_BUTTON_0 = ET_CONTROL_NEXT,
		MAXRV_PREVIEW_BUTTON_1,

		MAXRV_PREVIEW_PAGE_MOBGEN,
		MAXRV_PREVIEW_PAGE_REBUILD,
	};

	enum
	{
		MAXRV_PREVIEW_MAX_PAGE = 2,
	};

public:
	CMaxRvPreviewWindow(void);
	virtual ~CMaxRvPreviewWindow(void);

public:
	void CreateSubControl();
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

public:
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	
public:
	CMultiModeButton* m_pButtonPage[MAXRV_PREVIEW_MAX_PAGE];
	CUIControl*	m_pPage[MAXRV_PREVIEW_MAX_PAGE];

	CMaxRvPreviewPage* m_pPageMobGen;
	CMaxRvPreviewPage* m_pPageRebuild;

public:
	int	m_nPage;

public:
	void	OpenPage( int nPage );
	void	ShowItemData();
};