#include "StdAfx.h"
#include "./MapWindowMark.h"
#include "./InnerInterface.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMapWindowMark::CMapWindowMark () /*:*/
{
	strTEXT = "";
}

CMapWindowMark::~CMapWindowMark ()
{
}

void CMapWindowMark::CreateSubControl ()
{
}

void CMapWindowMark::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( CHECK_MOUSE_IN ( GetMessageEx () ) )
	{
		CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine ( strTEXT.c_str() , NS_UITEXTCOLOR::DARKLBUE );
	}	
}

