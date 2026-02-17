#include "stdafx.h"
#include "resource.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "./ToolPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CLandmanTab

IMPLEMENT_DYNAMIC(CLandmanTab, CPropertySheet)

CLandmanTab::CLandmanTab(CRect& rect, LOGFONT logfont, CWnd* pParentWnd)
	: CPropertySheet("SheetLandman", pParentWnd),
	m_bNoTabs(FALSE),
	m_pWndParent(NULL),
	m_iDistanceFromTop(0),
	m_pData(NULL),
	
	m_PageWLDMain(logfont),
	m_PageWLDBasic(logfont),
	m_PageWLDFog(logfont),
	m_PageWLDSky(logfont),
	m_PageWLDBGM(logfont),
	m_PageWLDSound(logfont),
	m_PageWLDEffect(logfont),
	
	m_pFont ( NULL )
{
	m_pWndParent = pParentWnd;
	m_Rect = rect;
	m_bNoTabs = TRUE;
	m_bStacked = FALSE;

	m_PageWLDMain.SetSheetTab(this);
	m_PageWLDBasic.SetSheetTab(this);
	m_PageWLDFog.SetSheetTab(this);
	m_PageWLDSky.SetSheetTab(this);
	m_PageWLDBGM.SetSheetTab(this);
	m_PageWLDSound.SetSheetTab(this);
	m_PageWLDEffect.SetSheetTab(this);

	AddPage(&m_PageWLDMain);
	AddPage(&m_PageWLDBasic);
	AddPage(&m_PageWLDFog);
	AddPage(&m_PageWLDSky);
	AddPage(&m_PageWLDBGM);
	AddPage(&m_PageWLDSound);
	AddPage(&m_PageWLDEffect);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CLandmanTab::~CLandmanTab()
{
	SAFE_DELETE( m_pFont );
}

static int module_piButtons[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };

BOOL CLandmanTab::OnInitDialog() 
{
	HWND hTabWnd;
	CRect rectTabCtrl;

	ChangeDialogFont( this, m_pFont, CDF_NONE );

	if( m_Rect.right )
	{
		hTabWnd = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL);
		ASSERT(hTabWnd != NULL);
		::GetWindowRect(hTabWnd, &rectTabCtrl);  // Tab-control rectangle (screen coordinates)
		ScreenToClient(rectTabCtrl);

		if( !m_bNoTabs && rectTabCtrl.right > (m_Rect.Width() - 3) )
		{
			rectTabCtrl.right = m_Rect.Width() - 3; // Set new right position
		}

		rectTabCtrl.bottom = 32; // Height for tabs

		::MoveWindow( hTabWnd, 0, m_iDistanceFromTop, rectTabCtrl.Width(), rectTabCtrl.bottom, TRUE );
		BOOL bResult = (BOOL)Default();          // Process messages

		if( m_bNoTabs )
		{
			::ShowWindow(hTabWnd, SW_HIDE);
			::EnableWindow(hTabWnd, FALSE);
		}

		if (!m_bStacked)
		{
			hTabWnd = (HWND)SendMessage(PSM_GETTABCONTROL);
			if (hTabWnd != NULL)
			{
				CWnd::ModifyStyle(hTabWnd, TCS_MULTILINE, TCS_SINGLELINE, 0);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			HWND hWnd = ::GetDlgItem(m_hWnd, module_piButtons[i]);
			if (hWnd != NULL)
			{
				::ShowWindow(hWnd, SW_HIDE);
				::EnableWindow(hWnd, FALSE);
			}
		}

		MoveWindow( m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom /*- cyDiff*/ );
		return bResult;
	}
	return CPropertySheet::OnInitDialog();
}

BEGIN_MESSAGE_MAP(CLandmanTab, CPropertySheet)
	//{{AFX_MSG_MAP(CLandmanTab)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HBRUSH CLandmanTab::CtlColor(CDC* pDc, UINT uCtlColor) 
{
	CWnd*    pwndParent;   // Parent window to property-sheet
	CDC*     pdcParent;    // Parent dc
	COLORREF color;        // color on parent dc           
	pwndParent = GetParent();
	ASSERT( pwndParent->IsKindOf( RUNTIME_CLASS( CWnd ) ) ); 
	pdcParent = pwndParent->GetDC();
	color = pdcParent->GetBkColor();    // Get parent color
	pwndParent->ReleaseDC( pdcParent );
	CBrush brush( color );              // Make a brush
	return (HBRUSH)brush;
}

BOOL CLandmanTab::PreTranslateMessage(MSG* pMsg)
{
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		{
			if ( pMsg->wParam == VK_ESCAPE )
			{
			}
		}break;
	}
	return CPropertySheet::PreTranslateMessage(pMsg);
}

void CLandmanTab::ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag)
{
	CRect windowRect;
	TEXTMETRIC tmOld, tmNew;
	CDC * pDC = pWnd->GetDC();
	CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pSavedFont);
	pWnd->ReleaseDC(pDC);

	CRect clientRect, newClientRect, newWindowRect;
	pWnd->GetWindowRect(windowRect);
	pWnd->GetClientRect(clientRect);
	double fHeigth = (double)(m_Rect.bottom-m_Rect.top) / clientRect.bottom;
	double fWidth = (double) (m_Rect.right-m_Rect.left) / clientRect.right;

	if (nFlag != CDF_NONE)
	{
		long xDiff = windowRect.Width() - clientRect.Width();
		long yDiff = windowRect.Height() - clientRect.Height();
	
		newClientRect.left = newClientRect.top = 0;
		newClientRect.right = LONG(clientRect.right * fWidth);
		newClientRect.bottom = LONG(clientRect.bottom * fHeigth);

		if (nFlag == CDF_TOPLEFT)
		{
			newWindowRect.left = windowRect.left;
			newWindowRect.top = windowRect.top;
			newWindowRect.right = windowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = windowRect.top + newClientRect.bottom + yDiff;
		}else if (nFlag == CDF_CENTER){
			newWindowRect.left = windowRect.left - 
							(newClientRect.right - clientRect.right)/2;
			newWindowRect.top = windowRect.top -
							(newClientRect.bottom - clientRect.bottom)/2;
			newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
		}
		pWnd->MoveWindow(newWindowRect);
	}

	pWnd->SetFont(pFont);

	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);
	while (pChildWnd)
	{
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(windowRect);

		CString strClass;
		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
		strClass.MakeUpper();
		if(strClass==_T("COMBOBOX"))
		{
			CRect rect;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
			windowRect.right = rect.right;
			windowRect.bottom = rect.bottom;
		}

		pWnd->ScreenToClient(windowRect);
		windowRect.left = LONG(windowRect.left * fWidth);
		windowRect.right = LONG(windowRect.right * fWidth);
		windowRect.top = LONG(windowRect.top * fHeigth);
		windowRect.bottom = LONG(windowRect.bottom *fHeigth);
		pChildWnd->MoveWindow(windowRect);
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CLandmanTab::InitPages()
{
	int nPAGE = GetPageCount();
	for( int i = 0; i < nPAGE; ++ i )
	{
		SetActivePage( i );
	}

	if ( nPAGE > 0 )
		SetActivePage( 0 );
}

void CLandmanTab::ResetData()
{
	m_PageWLDMain.ResetData();
	m_PageWLDBasic.ResetData();
	m_PageWLDFog.ResetData();
	m_PageWLDSky.ResetData();
	m_PageWLDBGM.ResetData();
	m_PageWLDSound.ResetData();
	m_PageWLDEffect.ResetData();

	m_pData = NULL;

	PageActive(EMWLD_PAGE_MAIN);
}

void CLandmanTab::SetData( DxLandMan* pData )
{
	m_pData = pData;

	m_PageWLDMain.SetData( m_pData );
	m_PageWLDBasic.SetData( m_pData );
	m_PageWLDFog.SetData( m_pData );
	m_PageWLDSky.SetData( m_pData );
	m_PageWLDBGM.SetData( m_pData );
	m_PageWLDSound.SetData( m_pData );
	m_PageWLDEffect.SetData( m_pData );

	PageActive(EMWLD_PAGE_MAIN);
}

void CLandmanTab::PageActive( int nPAGE )
{
	if( nPAGE < 0 )	return;

	if ( !m_pData )	return;

	switch( nPAGE )
	{
	case EMWLD_PAGE_MAIN:
		{
			SetActivePage ( EMWLD_PAGE_MAIN );
			m_PageWLDMain.SetData( m_pData );
		}break;

	case EMWLD_PAGE_BASIC:
		{
			SetActivePage ( EMWLD_PAGE_BASIC );
			m_PageWLDBasic.SetData( m_pData );
		}break;

	case EMWLD_PAGE_FOG:
		{
			SetActivePage ( EMWLD_PAGE_FOG );
			m_PageWLDFog.SetData( m_pData );
		}break;

	case EMWLD_PAGE_SKY:
		{
			SetActivePage ( EMWLD_PAGE_SKY );
			m_PageWLDSky.SetData( m_pData );
		}break;

	case EMWLD_PAGE_BGM:
		{
			SetActivePage ( EMWLD_PAGE_BGM );
			m_PageWLDBGM.SetData( m_pData );
		}break;

	case EMWLD_PAGE_SOUND:
		{
			SetActivePage ( EMWLD_PAGE_SOUND );
			m_PageWLDSound.SetData( m_pData );
		}break;

	case EMWLD_PAGE_EFFECT:
		{
			SetActivePage ( EMWLD_PAGE_EFFECT );
			m_PageWLDEffect.SetData( m_pData );
		}break;

	};

	//moving to pages reset mouse control edits
	CWorldToolView::GetView()->ResetMouseEdit();
}