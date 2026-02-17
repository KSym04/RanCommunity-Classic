#include "stdafx.h"
#include "CTestMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTestMemDC::CTestMemDC(CDC* pDC, const CRect* pRect /*= NULL*/) 
	: CDC()
{
	// make sure the call provided a valid device context
	ASSERT(pDC != NULL);

	// save the original device context for later BitBlt()
	m_pDC = pDC;

	// check whether we're printing
	m_bMemDC = !pDC->IsPrinting();

	// get the output rectangle
	if (pRect == NULL)
		pDC->GetClipBox(&m_rect);
	else
		m_rect = *pRect;

	if (m_bMemDC)
	{
		// do the memory DC stuff if not printing

		// create a memory DC
		CreateCompatibleDC(pDC);

		// create new bitmap and save old bitmap
		pDC->LPtoDP(&m_rect);
		m_MemBitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
		m_pOldBitmap = SelectObject(&m_MemBitmap);
		SetMapMode(pDC->GetMapMode());
		SetWindowExt(pDC->GetWindowExt());
		SetViewportExt(pDC->GetViewportExt());
		pDC->DPtoLP(&m_rect);

		// set window origin
		SetWindowOrg(m_rect.left, m_rect.top);
	}
	else
	{
		// make a copy of the relevant parts of the original DC for printing
		m_bPrinting = pDC->m_bPrinting;
		m_hDC		= pDC->m_hDC;
		m_hAttribDC = pDC->m_hAttribDC;
	}

	// fill background
	CWnd* pWnd = pDC->GetWindow();
	HWND hWnd = NULL;
	if (pWnd != NULL)
	{
		hWnd = pWnd->m_hWnd;

		if (hWnd != NULL)
		{
			if (GetClassLongPtr(hWnd, GCW_ATOM) == 32770)
			{
				// if window is a dialog box, use dialog background color
				FillSolidRect(m_rect, GetSysColor(COLOR_BTNFACE));
			}
			else
			{
				// try to get background brush for window class
				ULONG_PTR ulpBrush = GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND);
				if (ulpBrush)
				{
					// fill with class background brush
					FillRect(m_rect, CBrush::FromHandle((HBRUSH)ulpBrush));
				}
				else
					hWnd = NULL;
			}
		}
	}
	if (hWnd == NULL)
		FillSolidRect(m_rect, pDC->GetBkColor());
}

// destructor copies the contents of the memory DC to the original DC
CTestMemDC::~CTestMemDC()
{
	if (m_bMemDC)
	{
		// copy the offscreen bitmap onto the screen
		m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			this, m_rect.left, m_rect.top, SRCCOPY);

		// restore the original bitmap
		m_pDC->SelectObject(m_pOldBitmap);
	}
	else
	{
		// all we need to do is replace the DC with an illegal value,
		// this keeps us from accidently deleting the handles associated
		// with the CDC that was passed to the constructor
		m_hDC = m_hAttribDC = NULL;
	}
}