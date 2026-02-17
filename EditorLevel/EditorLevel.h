
// EditorLevel.h : main header file for the EditorLevel application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CEditorLevelApp:
// See EditorLevel.cpp for the implementation of this class
//

class CEditorLevelApp : public CWinApp
{
public:
	CEditorLevelApp();

public:
	BOOL		m_bIsActive;

public:
	void		SetActive(BOOL bActive) { m_bIsActive = bActive; };

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int Run();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEditorLevelApp theApp;
