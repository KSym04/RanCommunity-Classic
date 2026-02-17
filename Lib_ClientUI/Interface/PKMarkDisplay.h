/*!
 * \file PKMarkDisplay.h
 *
 * \author Juver
 * \date 2022/02/14
 *
 * 
 */

#ifndef PKMARKDISPLAY_H_INCLUDED__
#define PKMARKDISPLAY_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLPKMark.h"

class CPKMarkSlot;

class CPKMarkDisplay : public CUIGroup
{
	enum
	{
		//INBOX_LIST_TEXTBOX = NO_ID + 1,
	};

public:
	CPKMarkDisplay();
	virtual ~CPKMarkDisplay();

public:
	void CreateSubControl();

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:

public:
	CPKMarkSlot* m_pSlot[PKMARK_UI_ARRAY_SIZE];

};

#endif // PKMARKDISPLAY_H_INCLUDED__
