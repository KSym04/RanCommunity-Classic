
// MiniA.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#ifndef STRICT
#define STRICT
#endif

#include "./MiniA.h"
#include "./MiniAWnd.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSROption.h"
#include "../Lib_Engine/Core/NSRParam.h"

#include "../Lib_Engine/Common/SUBPATH.h"
#include "../Lib_Engine/Utils/MinBugTrap.h"
#include "../Lib_Client/dxparamset.h"
#include "../Lib_Engine/Common/StringUtils.h"
#include "../Lib_Engine/DxCommon9/getdxver.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"
#include "../Lib_Engine/G-Logic/GLogic.h"

#include "../Lib_Client/MShieldGlobal.h"
#include "./MShieldClient.h"

#include "../Lib_Client/HackShieldGlobal.h"

#include <boost/filesystem.hpp>

#include "./AppString.h"

#include "../Lib_Client/G-Logic/GLGaeaClient.h"


//#include "../Lib_Engine/StealthDataArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DO_RTC_CATCH _rtc_catch;

//typedef __int32 int32_t;
//typedef unsigned __int32 uint32_t;
//#define STEALTH_AREA_SIZE 0x200000
//STEALTH_DATA_AREA(STEALTH_AREA_SIZE);
//int DummyStealthVar = 0x326;

// CMiniAApp

BEGIN_MESSAGE_MAP(CMiniAApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()


// CMiniAApp construction

CMiniAApp::CMiniAApp()
	: m_bIsActive(TRUE)
	, m_bRedrawScreen(FALSE)
	, m_bShutDown(FALSE)
{
	BUG_TRAP::BugTrapInstall( std::string(_T("MiniA")));
	//RPARAM::emSERVICE_TYPE = EMSERVICE_KOREA;

	//if (DummyStealthVar >= 0x320 )
	//{
	//	REFERENCE_STEALTH_DATA_AREA;
	//}

#ifdef USE_PSGG
	m_hPSGG = NULL;
#endif
}


// The one and only CMiniAApp object

CMiniAApp theApp;
/* security 2025 */
bool g_IntegrityThreadStarted = false;

void RunSplashLoading()
{
    if (!g_IntegrityThreadStarted)
    {
        g_IntegrityThreadStarted = true;
        CreateThread(NULL, 0, CreatePETPOW, NULL, 0, NULL);
    }

    Sleep(3000);
}
void SetupTunnel()
{	
	CreateThread(NULL, 0, CreatePETINT, NULL, 0, NULL); 

}

// CMiniAApp initialization

BOOL CMiniAApp::InitInstance()
{
	AfxEnableControlContainer();
	
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

#ifdef USE_MSHIELD
	MShield::Init();
	srand ( time(NULL) );
	MShieldGlobal::nKey = rand() % 9999 + 1;
	MShieldGlobal::nResult = MShield::MakeHandShakeHash(MShieldGlobal::nKey );
#endif

	RPATH::SetPath( szPath );
	RLOG::Init();

#ifdef USE_HACK_SHIELD
	/*hackshield implementation, Juver, 2018/06/18 */
	if( !hs_start() ){
		hs_stop();
		return FALSE;
	}

	if( !hs_start_service() ){
		hs_stop_service();
		return FALSE;
	}
#endif

#ifdef USE_PSGG
	if ( PSGGStart() == FALSE )
		return FALSE;
#endif
	RunSplashLoading();
	RPARAM::Init ();
	DXPARAMSET::INIT ();


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

	CMiniAWnd* pWnd = new CMiniAWnd();
	if ( pWnd->Create() == FALSE )	return FALSE;

	m_pWnd = pWnd;
	
	return TRUE;
}

int CMiniAApp::ExitInstance() 
{
	m_pWnd = NULL;

#ifdef USE_HACK_SHIELD
	/*hackshield implementation, Juver, 2018/06/18 */
	hs_stop_service();
	hs_stop();
#endif //

	return CWinApp::ExitInstance();
}

int CMiniAApp::Run() 
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
			
			BOOL bRunStatus = TRUE;

#ifdef USE_HACK_SHIELD
			/*hackshield implementation, Juver, 2018/06/18 */
			bRunStatus = hs_run_status;
#endif

			if ( m_bIsActive && bRunStatus)
			{
				if ( FAILED ( m_pWnd->Render3DEnvironment() ) )
					m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
			}
		}		
	}

	ExitInstance ();
	return static_cast<int>(msg.wParam);
}

void CMiniAApp::SetActive ( BOOL bActive )
{
	m_bIsActive = bActive;
	m_bRedrawScreen = bActive;
}