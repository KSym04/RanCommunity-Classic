/*!
 * \file ModernCharacterStatOver.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "./BasicLineBox.h"

class	CModernCharacterStatOver : public CUIGroup
{
private:
	enum
	{
		// = NO_ID + 1,
	};


public:
	CModernCharacterStatOver ();
	virtual	~CModernCharacterStatOver ();

public:
	void	CreateSubControl( std::string strMsg );

public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	std::string		m_strMsg;
	CBasicLineBox*	m_pLineBox;

};