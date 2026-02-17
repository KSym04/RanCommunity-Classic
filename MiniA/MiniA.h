
// MiniA.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define NO_GAMEGUARD

class CMiniAWnd;

#include "../Lib_Client/HackShieldGlobal.h"
#include "../Lib_Client/PSDefine.h"

// CMiniAApp:
// See MiniA.cpp for the implementation of this class
//

class CMiniAApp : public CWinApp
{
public:
	CMiniAApp();

protected:
	CMiniAWnd*	m_pWnd;
	BOOL		m_bIsActive;    
	BOOL		m_bRedrawScreen;
	BOOL		m_bShutDown;

public:
	void SetActive(BOOL bActive);
	void ShutDown() { m_bShutDown = TRUE; };

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();

// Implementation

	DECLARE_MESSAGE_MAP()



public:
#ifdef USE_HACK_SHIELD
	/*hackshield implementation, Juver, 2018/06/18 */
	static const char* hs_dll_name;
	static const char* hs_folder;
	static int hs_gamecode;
	static const char* hs_key;
	static DWORD hs_option;
	static DWORD hs_sh_option;
	static BOOL	hs_run_status;

	static void hs_output( long code, long param_size, void* pparam );
	BOOL hs_start();
	BOOL hs_stop();
	BOOL hs_start_service();
	BOOL hs_stop_service();
#endif

public:
#ifdef USE_PSGG
	HMODULE m_hPSGG;

	BOOL PSGGStart();
#endif // USE_PSGG

};

extern CMiniAApp theApp;