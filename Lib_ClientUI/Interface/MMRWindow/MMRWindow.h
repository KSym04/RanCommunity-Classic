/**MMR Rank Stats, Mhundz 01/02/2025**/
#pragma	once

#include "../UIWindowEx.h"

class CMultiModeButton;
class CMMRPage; 
class CMMRWindow: public CUIWindowEx
{
public:
	enum
	{
		MMR_WINDOW_BUTTON_PAGE1 = ET_CONTROL_NEXT,
		MMR_WINDOW_BUTTON_PAGE2,

		MMR_WINDOW_PAGE1,
		MMR_WINDOW_PAGE2,
	};


	enum
	{
		MMR_WINDOW_PAGE_TEMP_1 = 0,
		MMR_WINDOW_PAGE_TEMP_2 = 1,
	};

public:
	CMMRWindow ();
	virtual	~CMMRWindow ();

public:
	CMultiModeButton*		m_pButtonPage1;
	CMultiModeButton*		m_pButtonPage2;
	CMMRPage*				m_pPageCodex;

public:
	void	CreateSubControl ();
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	int		m_nPage;

public:
	void	OpenPage( int nPage );
};