/*!
 * \file GameItemShopPageTypeBase.h
 *
 * \author Juver
 * \date 2021/11/04
 *
 * 
 */

#ifndef GAMEITEMSHOPPAGETYPEBASE_H_INCLUDED__
#define GAMEITEMSHOPPAGETYPEBASE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CGameItemShopPageTypeBase : public CUIGroup
{
private:

public:
	CGameItemShopPageTypeBase ();
	virtual	~CGameItemShopPageTypeBase ();

public:
	void	CreateSubControl ( std::string strbase, std::string strselect, std::string strover );

private:
	CUIControl*		m_pImageBase;
	CUIControl*		m_pImageSelect;
	CUIControl*		m_pImageOver;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	void	SetSelect( BOOL bSelect );
};

#endif // GAMEITEMSHOPPAGETYPEBASE_H_INCLUDED__
