#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class CItemShopButtonAlarm;


class	CItemShopButton : public CUIGroup
{
private:
	enum
	{
		ITEM_SHOP_BUTTON_BUTTON_IMAGE = NO_ID + 1,
		ITEM_SHOP_BUTTON_BUTTON_ALARM,
	};

public:
	CItemShopButton ();
	virtual	~CItemShopButton ();

public:
	void	CreateSubControl ();

public:
	CUIControl*							m_pButtonImage;
	CUIControl*							m_pButtonBlink;
	CUIControl*							m_pButtonLock;
	CItemShopButtonAlarm*		m_pButtonAlarm;
	

public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	float	m_fBLINK_TIME;
	BOOL	m_bAlarm;
	BOOL	m_bLocked;

public:
	void	SetButtonAlarm( BOOL bSet );
	void	SetButtonLock( BOOL bSet );
};