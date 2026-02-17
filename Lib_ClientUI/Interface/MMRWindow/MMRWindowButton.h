/**MMR Rank Stats, Mhundz 01/02/2025**/

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class	CMMRWindowButton : public CUIGroup
{
private:
	enum
	{
		MMR_WINDOW_BUTTON_IMAGE = NO_ID + 1,
	};

public:
	CMMRWindowButton ();
	virtual	~CMMRWindowButton ();

public:
	void	CreateSubControl ();

public:
	CUIControl*			m_pButtonImage;

public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
};