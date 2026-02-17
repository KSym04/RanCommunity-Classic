// EditorRandomOption.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CEditorRandomOptionApp:
// See EditorRandomOption.cpp for the implementation of this class
//

class CEditorRandomOptionApp : public CWinApp
{
public:
	CEditorRandomOptionApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEditorRandomOptionApp theApp;