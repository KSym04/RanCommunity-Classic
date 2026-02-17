#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class	CGameItemShopIcon : public CUIGroup
{
private:
	enum
	{
		BUTTON_IMAGE = NO_ID + 1,
	};

public:
	CGameItemShopIcon ();
	virtual	~CGameItemShopIcon ();

public:
	void	CreateSubControl ();

public:
	CUIControl*			m_pButtonImage;

public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
};