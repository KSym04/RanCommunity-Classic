#ifndef _MEMDC_H_
#define _MEMDC_H_


////////////////////////////////////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-2002, Keith Rule
//
// You may freely use or modify this code provided this
// copyright is included in all derived versions.
//
// History -	10/03/97	Fixed scrolling bug.
//							Added print support. - KR
//
//				11/03/99	Fixed most common complaint. Added
//							background color fill. - KR
//
//				11/03/99	Added support for mapping modes other than
//							MM_TEXT as suggested by Lee Sang Hun. - KR
//
//				02/11/02	Added support for CScrollView as supplied
//							by Gary Kirkham. - KR
//
//				04/19/06	Modified background fill to try and use
//							correct background brush/color - AGB
//
// This class implements a memory device context which allows
// flicker free drawing.
////////////////////////////////////////////////////////////////////////////////


class CTestMemDC : public CDC
{
private:
	BOOL		m_bMemDC;					// TRUE if not printing
	CBitmap		m_MemBitmap;				// offscreen bitmap
	CBitmap*	m_pOldBitmap;				// original bitmap
	CDC*		m_pDC;						// saves CDC passed in constructor
	CRect		m_rect;						// rectangle of drawing area

public:
	// constructor sets up the memory DC
	CTestMemDC(CDC* pDC, const CRect* pRect = NULL);

	// destructor copies the contents of the memory DC to the original DC
	~CTestMemDC();

	// allow usage as a pointer
	CTestMemDC* operator->() { return this; }

	// allow usage as a pointer
	operator CTestMemDC*() { return this; }
};

#endif											//#ifndef _MEMDC_H_
