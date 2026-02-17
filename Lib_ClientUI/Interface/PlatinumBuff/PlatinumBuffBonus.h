/*!
 * \file SandoraBoxButton.h
 *
 * \author Juver
 * \date October 2017
 *
 * 
 */

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class	CPlatinumBuffBonus : public CUIGroup
{
private:
	enum
	{
		PLATINUM_MARK_NONE = NO_ID + 1,
	};

public:
	CPlatinumBuffBonus ();
	virtual	~CPlatinumBuffBonus ();

public:
	void	CreateSubControl();

public:
	CUIControl*			m_pButtonImage;

public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
};