#include "stdafx.h"

#ifndef STRICT
	#define STRICT
#endif

#include "../Lib_Engine/Core/NSROption.h"
#include "../Lib_Engine/Core/NSRParam.h"

#include "../Lib_Client/DxGlobalStage.h"
#include "../Lib_Engine/Common/SUBPATH.h"

#include "../Lib_Client/dxparamset.h"
#include "../Lib_Engine/GUInterface/Cursor.h"
#include "../Lib_Engine/DxCommon/DxCursor.h"

#include "./RanClient.h"
#include "./RanClientWnd.h"
#include "../Lib_Engine/Utils/BlockProg.h"
#include "../Lib_Client/G-Logic/GLogicData.h"
#include "../Lib_Engine/G-Logic/GLogic.h"
#include "../Lib_Network/s_NetClient.h"
#include "../Lib_Helper/rol_clipboard.h"

#include "../Lib_Client/MShieldGlobal.h"

#include "./AppString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd(NULL);

/////////////////////////////////////////////////////////////////////////////
// CRanClientWnd

CRanClientWnd::CRanClientWnd() :
	CD3DApplication(),
	m_bCreated(FALSE),
	m_wndSizeX(1024),
	m_wndSizeY(768),
	m_hCursorDefault(FALSE),
	m_nGGTimer(0),
	m_nGGATimer(0),
	m_nGGA12Timer(0),

	m_hMutex( NULL )
{
	m_bUseDepthBuffer = TRUE;
	m_pApp = (CRanClientApp*)AfxGetApp();
}

CRanClientWnd::~CRanClientWnd()
{
}

BEGIN_MESSAGE_MAP(CRanClientWnd, CWnd)
	ON_WM_ACTIVATEAPP()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_MESSAGE(NET_NOTIFY,  OnNetNotify)
	ON_WM_SYSCOMMAND()
	ON_WM_MOUSEMOVE()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCACTIVATE()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRanClientWnd message handlers

BOOL CRanClientWnd::Create() 
{
	CRanClientApp *pRanClientApp = (CRanClientApp *) AfxGetApp();

	CRect rectWnd;
	LONG nWindowStyle = WS_POPUP | WS_VISIBLE;
	
	if ( ROPTION::bScrWindowed )
	{
		m_nScreenWinX = GetSystemMetrics(SM_CXSCREEN);
		m_nScreenWinY = GetSystemMetrics(SM_CYSCREEN);

		m_wndSizeX = ROPTION::dwScrWidth;
		m_wndSizeY = ROPTION::dwScrHeight;

		if ( m_wndSizeX>m_nScreenWinX || m_wndSizeY>m_nScreenWinY )
		{
			m_wndSizeX = m_nScreenWinX;
			m_wndSizeY = m_nScreenWinY;
		}

		if ( ROPTION::bScrWndFullSize )
		{
			m_wndSizeX = m_nScreenWinX;
			m_wndSizeY = m_nScreenWinY;

			/*float fScreen = m_nScreenWinX / float(m_nScreenWinY);
			if ( fScreen > (16.0f/9.0f) )
			{
			m_wndSizeX = m_nScreenWinX/2;
			}*/

			rectWnd = CRect( 0, 0, m_wndSizeX, m_wndSizeY );
		}else{
			if ( m_wndSizeX>=(m_nScreenWinX) || m_wndSizeY>=(m_nScreenWinY) )
			{
				m_wndSizeX = 1024;
				m_wndSizeY = 768;

				if ( m_wndSizeX>=m_nScreenWinX || m_wndSizeY>=m_nScreenWinY )
				{
					m_wndSizeX = m_nScreenWinX-60;
					m_wndSizeY = m_nScreenWinY-80;
				}
			}

			nWindowStyle = WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION |  WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_CLIPCHILDREN;

			RECT rt = {0, 0, m_wndSizeX, m_wndSizeY};
			AdjustWindowRectEx(&rt, nWindowStyle, FALSE, WS_EX_APPWINDOW);

			m_wndSizeX = rt.right - rt.left;
			m_wndSizeY = rt.bottom - rt.top;
			INT nX = (m_nScreenWinX - m_wndSizeX) / 2;
			INT nY = (m_nScreenWinY - m_wndSizeY) / 2;

			rectWnd = CRect( nX, nY, m_wndSizeX, m_wndSizeY );
		}
	}
	else
	{
		m_nScreenWinX = GetSystemMetrics(SM_CXSCREEN);
		m_nScreenWinY = GetSystemMetrics(SM_CYSCREEN);

		m_wndSizeX = m_nScreenWinX;
		m_wndSizeY = m_nScreenWinY;

		rectWnd = CRect( 0, 0, m_wndSizeX, m_wndSizeY );
	}

	if ( GetLastError() == ERROR_ALREADY_EXISTS ) 
	{ 
		return FALSE;
	}

	LPCSTR strClass = AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), AfxGetApp()->LoadIcon(IDR_MAINFRAME) );
	if( !strClass )
	{
		CDebugSet::MsgBoxAfx( NSAPP_STRING::getc( EMAPP_STRING_CLASS_REG_FAILED ) );
		return FALSE;
	}

	if ( CreateEx ( WS_EX_APPWINDOW, strClass, AfxGetAppName(), nWindowStyle, rectWnd.left, rectWnd.top, rectWnd.right, rectWnd.bottom, NULL, NULL, 0 ) == FALSE )
	{
		return FALSE;
	}

	SetWindowText (_T(RPARAM::szClientWindowTitle));

	//attempt to change icon
	const HANDLE hIconS = ::LoadImage( ::GetModuleHandle(0), "game.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
	if (hIconS)
		::SendMessage(GetSafeHwnd(), WM_SETICON, ICON_SMALL, (LPARAM)hIconS);

	const HANDLE hIconB = ::LoadImage( ::GetModuleHandle(0), "game.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE );
	if (hIconB)
		::SendMessage(GetSafeHwnd(), WM_SETICON, ICON_BIG, (LPARAM)hIconB);

	UpdateWindow();

	CD3DApplication::SetScreen( ROPTION::dwScrWidth, ROPTION::dwScrHeight, ROPTION::emScrFormat, ROPTION::uScrRefreshHz, ROPTION::bScrWindowed );

	if ( FAILED( CD3DApplication::Create ( m_hWnd, m_hWnd, AfxGetInstanceHandle() ) ) )
		return FALSE;
	
	m_bCreated = TRUE;
	g_hWnd = m_hWnd;

	return TRUE;
}

void CRanClientWnd::PostNcDestroy() 
{
	::UnregisterClass( AfxGetAppName(), AfxGetInstanceHandle() ); 

	Cleanup3DEnvironment();

	CWnd::PostNcDestroy();

	m_pApp->ShutDown();

	if ( ROPTION::bGameCursor )
	{
		::SetCursor ( NULL );
		if ( m_hCursorDefault )		::DestroyCursor ( m_hCursorDefault );
	}

	delete this;

	HWND hTrayWnd = ::FindWindow("Shell_TrayWnd", NULL);
	::SendMessage( hTrayWnd, WM_COMMAND, 416, 0 );
}

void CRanClientWnd::OnActivateApp(BOOL bActive, DWORD hTask)
{
	CWnd::OnActivateApp(bActive, hTask);
}

BOOL CRanClientWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ( ROPTION::bGameCursor )
	{
		HCURSOR hCursl = CCursor::GetInstance().GetCurCursor();
		if ( hCursl )
		{
			CCursor::GetInstance().SetCursorNow ();
			return TRUE;
		}
	}

	return CWnd::OnSetCursor ( pWnd, nHitTest, message );
}

void CRanClientWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( ROPTION::bGameCursor )
	{
		HCURSOR hCursl = CCursor::GetInstance().GetCurCursor();
		if ( hCursl )
		{
			CCursor::GetInstance().SetCursorNow ();
			return;
		}
	}

	__super::OnMouseMove(nFlags, point);
}

void CRanClientWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_ESCAPE:
		break;
	};

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CRanClientWnd::OnNetNotify(WPARAM wParam, LPARAM lParam)
{
	DxGlobalStage::GetInstance().OnNetNotify ( wParam, lParam );
	return NULL;
}
