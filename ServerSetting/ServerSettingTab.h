#ifndef _SERVER_SETTING_TAB_H_INCLUDED_
#define _SERVER_SETTING_TAB_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CServerSettingTab

#include "PageBasic.h"
#include "PageDatabase.h"
#include "PageServer1.h"
#include "PageServer2.h"

class CServerSettingTab : public CPropertySheet
{
public:
	enum 
	{ 
		CDF_CENTER  = 0,
		CDF_TOPLEFT = 1,
		CDF_NONE	= 2,
	};

private:
	DECLARE_DYNAMIC(CServerSettingTab)

public:
	CWnd*		m_pWndParent;

public:
	CPageBasic			m_PageBasic;
	CPageDatabase		m_PageDatabase;
	CPageServer1		m_PageServer1;
	CPageServer2		m_PageServer2;

public:	

// Construction
public:
	CServerSettingTab(CWnd* pParentWnd = NULL);
	CServerSettingTab(CRect& rect, LOGFONT logfont, CWnd* pParentWnd = NULL );
	virtual ~CServerSettingTab();

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
	void	ShowData();
	

// Generated message map functions
protected:
	afx_msg HBRUSH CtlColor(CDC* pDc, UINT uCtlColor);	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif	// _SERVER_SETTING_TAB_H_INCLUDED_