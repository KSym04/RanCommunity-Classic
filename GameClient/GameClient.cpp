
// GameClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#ifndef STRICT
#define STRICT
#endif

#include "./GameClient.h"
#include "./GameClientWnd.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"

#include "../Lib_Engine/Common/SUBPATH.h"
#include "../Lib_Engine/Utils/MinBugTrap.h"
#include "../Lib_Client/dxparamset.h"
#include "../Lib_Engine/Common/StringUtils.h"
#include "../Lib_Engine/DxCommon9/getdxver.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"
#include "../Lib_Engine/G-Logic/GLogic.h"

#include "../Lib_Client/HackShieldGlobal.h"

#include "./HShield.h"

#include "../Lib_Client/MShieldGlobal.h"

#include <boost/filesystem.hpp>

#include "./AppString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DO_RTC_CATCH _rtc_catch;

// CGameClientApp

BEGIN_MESSAGE_MAP(CGameClientApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()


// CGameClientApp construction

CGameClientApp::CGameClientApp()
	: m_bIsActive(TRUE)
	, m_bRedrawScreen(FALSE)
	, m_bShutDown(FALSE)
{
	BUG_TRAP::BugTrapInstall( std::string(_T("GameClient")));
	//RPARAM::emSERVICE_TYPE = EMSERVICE_KOREA;
}


// The one and only CGameClientApp object

CGameClientApp theApp;


// CGameClientApp initialization

BOOL CGameClientApp::InitInstance()
{
	AfxEnableControlContainer();

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DXPARAMSET::INIT ();

#ifdef USE_MSHIELD
	srand ( time(NULL) );
	MShieldGlobal::nKey = rand() % 9999 + 1;
	MShieldGlobal::nResult = MShieldGlobal::nKey * MSHIELD_KEY;
#endif

#ifdef USE_HACK_SHIELD
	/*hackshield implementation, Juver, 2018/06/19 */
	HackShieldGlobal::hs_sdk_version = RPARAM::hs_dll_version;
	HackShieldGlobal::hs_start_ret = HS_ERR_OK;
	HackShieldGlobal::hs_service_ret = HS_ERR_OK;
	HackShieldGlobal::hs_monitor_ret = HS_ERR_OK;
	HackShieldGlobal::hs_detect_ret = HS_ERR_OK;
#endif

	CString StrCmdLine = m_lpCmdLine;

	STRUTIL::ClearSeparator ();
	STRUTIL::RegisterSeparator ( "/" );
	STRUTIL::RegisterSeparator ( " " );

	CStringArray strCmdArray;
	STRUTIL::StringSeparate ( StrCmdLine, strCmdArray );

	BOOL bAPM = FALSE;
	BOOL bRCC = FALSE;

	for ( int i=0; i<strCmdArray.GetCount(); i++ )
	{
		CString strTemp = strCmdArray.GetAt(i);

		/*game launch parameter, Juver, 2018/09/19 */
		if ( strcmp( strTemp.GetString(), RPARAM::game_launch_param.c_str() ) == 0 )
		{
			bAPM = TRUE;
		}

		if ( strcmp( strTemp.GetString(), NSAPP_STRING::getc( EMAPP_STRING_RCC_PARAMETER_1 ) ) == 0 )
		{
			bRCC = TRUE;
		}

		if ( strcmp( strTemp.GetString(), NSAPP_STRING::getc( EMAPP_STRING_RCC_PARAMETER_2 ) ) == 0 )
		{
			bRCC = TRUE;
		}
	}

	if ( !bAPM )
	{
		CDebugSet::MsgBox( NSAPP_STRING::getc( EMAPP_STRING_LAUNCH_FAILED ) );
		return FALSE;
	}

	//rcc initialize
	GLOGIC::bGLOGIC_PACKFILE = FALSE;
	GLOGIC::bGLOGIC_ZIPFILE = FALSE;
	GLOGIC::bENGLIB_ZIPFILE = FALSE;

	RPARAM::bShowGameStats = TRUE;

	if ( bRCC )
	{
		GLOGIC::bGLOGIC_PACKFILE = FALSE;
		GLOGIC::bGLOGIC_ZIPFILE = TRUE;
		GLOGIC::bENGLIB_ZIPFILE = TRUE;
	}

	DWORD dwDirectXVersion = 0;
	TCHAR strDirectXVersion[10];
	HRESULT hr = getdxversion ( &dwDirectXVersion, strDirectXVersion, 10 );

	if ( hr==S_OK && dwDirectXVersion<0x00090003 )
	{
		CDebugSet::MsgBox( NSAPP_STRING::getc( EMAPP_STRING_DIRECTX_OLD ) );
		return FALSE;
	}

	TCHAR szFullPath[MAX_PATH] = {0};
	StringCchCopy( szFullPath, MAX_PATH, RPATH::getAppPath() );
	StringCchCat( szFullPath, MAX_PATH, SUBPATH::GUI_FILE_ROOT);
	CGameTextMan::GetInstance().SetPath(szFullPath);
	CGameTextMan::GetInstance().LoadText(RPARAM::strGameWord.GetString(), CGameTextMan::EM_GAME_WORD, RPARAM::bXML_USE );
	CGameTextMan::GetInstance().LoadText(RPARAM::strGameInText.GetString(), CGameTextMan::EM_GAME_IN_TEXT, RPARAM::bXML_USE );
	CGameTextMan::GetInstance().LoadText(RPARAM::strGameExText.GetString(), CGameTextMan::EM_GAME_EX_TEXT, RPARAM::bXML_USE );

	CGameClientWnd* pWnd = new CGameClientWnd();
	if ( pWnd->Create() == FALSE )	return FALSE;

	m_pWnd = pWnd;
	
	return TRUE;
}

int CGameClientApp::ExitInstance() 
{
	m_pWnd = NULL;

	return CWinApp::ExitInstance();
}

int CGameClientApp::Run() 
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE );

	while( WM_QUIT != msg.message )
	{
		if ( m_bShutDown == TRUE || m_pWnd == NULL )
		{
			ExitInstance ();
			return 0;
		}

		if( m_bIsActive )
			bGotMsg = PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0, 0 );

		if( bGotMsg )
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);

			if ( !m_bIsActive && !m_bShutDown )
			{
				if ( FAILED ( m_pWnd->FrameMove3DEnvironment() ) )
					m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
			}
		}else{
			if ( m_bIsActive )
			{
				if ( FAILED ( m_pWnd->Render3DEnvironment() ) )
					m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
			}
		}		
	}

	ExitInstance ();
	return static_cast<int>(msg.wParam);
}

void CGameClientApp::SetActive ( BOOL bActive )
{
	m_bIsActive = bActive;
	m_bRedrawScreen = bActive;
}