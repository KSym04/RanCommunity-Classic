/////////////////////////////////////////////////////////////////////////////
// CLandmanTab

#pragma once

#include "../Lib_Engine/DxOctree/DxLandMan.h"
#include "./LandmanMain.h"
#include "./LandmanBasic.h"
#include "./LandmanFog.h"
#include "./LandmanSky.h"
#include "./LandmanBGM.h"
#include "./LandmanSound.h"
#include "./LandmanEffect.h"

class CLandmanTab : public CPropertySheet
{
public:
	enum 
	{ 
		CDF_CENTER  = 0,
		CDF_TOPLEFT = 1,
		CDF_NONE	= 2,
	};

private:
	DECLARE_DYNAMIC(CLandmanTab)

public:
	CWnd*		m_pWndParent;

public:
	CLandmanMain		m_PageWLDMain;
	CLandmanBasic		m_PageWLDBasic;
	CLandmanFog			m_PageWLDFog;
	CLandmanSky			m_PageWLDSky;
	CLandmanBGM			m_PageWLDBGM;
	CLandmanSound		m_PageWLDSound;
	CLandmanEffect		m_PageWLDEffect;

public:	

// Construction
public:
	CLandmanTab(CWnd* pParentWnd = NULL);
	CLandmanTab(CRect& rect, LOGFONT logfont, CWnd* pParentWnd = NULL );
	virtual ~CLandmanTab();

// Attributes
public:
	CRect		m_Rect;
	BOOL		m_bNoTabs;
	int			m_iDistanceFromTop;
	CFont*		m_pFont;
	DxLandMan*	m_pData;

// Get/Set
public:
   void NoTabs() { m_bNoTabs = TRUE; }
   void SetDistanceFromTop( int iDistance ) { m_iDistanceFromTop = iDistance; }
   void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag);

public:
	virtual BOOL OnInitDialog();

	void	InitPages();
	void	ResetData();
	void	SetData( DxLandMan* pData );
	void	PageActive( int nPage );

// Generated message map functions
protected:
	afx_msg HBRUSH CtlColor(CDC* pDc, UINT uCtlColor);	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
