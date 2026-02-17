/*!
 * \file ModernCharacterStatMark.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class	CModernCharacterStatMark : public CUIGroup
{
private:
	enum
	{
		MARK_IMAGE_UP = NO_ID + 1,
		MARK_IMAGE_DOWN,
	};


public:
	CModernCharacterStatMark ();
	virtual	~CModernCharacterStatMark ();

public:
	void	CreateSubControl();

public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID cID, DWORD dwMsg );
	virtual	void	SetVisibleSingle ( BOOL bVisible );

public:
	void	Reset();
	void	ShowImage( bool bUp );
	bool	CheckValue( int nOrig, int nToCheck );
	bool	CheckValue( float fOrig, float fToCheck );
	bool	CheckValue( int nCheck );

private:
	CUIControl*	m_pUpImage;
	CUIControl* m_pDownImage;

};