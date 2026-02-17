#ifndef _MAINTAB_H_INCLUDED_
#define _MAINTAB_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CMainTab

#include "./MainLandman.h"
#include "./MainPiece.h"

class CMainTab : public CPropertySheet
{
public:
	enum 
	{ 
		CDF_CENTER  = 0,
		CDF_TOPLEFT = 1,
		CDF_NONE	= 2,
	};

private:
	DECLARE_DYNAMIC(CMainTab)

public:
	CWnd*		m_pWndParent;

public:
	CMainLandman	m_PageWLD;
	CMainPiece		m_PagePIE;
	

public:	

// Construction
public:
	CMainTab(CWnd* pParentWnd = NULL);
	CMainTab(CRect& rect, LOGFONT logfont, CWnd* pParentWnd = NULL );
	virtual ~CMainTab();

// Attributes
public:
	CRect		m_Rect;
	BOOL		m_bNoTabs;
	int			m_iDistanceFromTop;
	CFont*		m_pFont;

// Get/Set
public:
   void NoTabs() { m_bNoTabs = TRUE; }
   void SetDistanceFromTop( int iDistance ) { m_iDistanceFromTop = iDistance; }
   void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag);

public:
	virtual BOOL OnInitDialog();

	void	InitPages();
	void	ResetData();
	

// Generated message map functions
protected:
	afx_msg HBRUSH CtlColor(CDC* pDc, UINT uCtlColor);	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif	// _MAINTAB_H_INCLUDED_