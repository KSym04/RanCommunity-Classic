/////////////////////////////////////////////////////////////////////////////
// CPieceTab

#pragma once

#include "../Lib_Engine/DxEffect/DxPieceContainer.h"
#include "./PieceMain.h"

class CPieceTab : public CPropertySheet
{
public:
	enum 
	{ 
		CDF_CENTER  = 0,
		CDF_TOPLEFT = 1,
		CDF_NONE	= 2,
	};

private:
	DECLARE_DYNAMIC(CPieceTab)

public:
	CWnd*		m_pWndParent;

public:
	CPieceMain		m_PagePIEMain;


public:	

// Construction
public:
	CPieceTab(CWnd* pParentWnd = NULL);
	CPieceTab(CRect& rect, LOGFONT logfont, CWnd* pParentWnd = NULL );
	virtual ~CPieceTab();

// Attributes
public:
	CRect		m_Rect;
	BOOL		m_bNoTabs;
	int			m_iDistanceFromTop;
	CFont*		m_pFont;
	DxPieceEdit*	m_pData;

// Get/Set
public:
   void NoTabs() { m_bNoTabs = TRUE; }
   void SetDistanceFromTop( int iDistance ) { m_iDistanceFromTop = iDistance; }
   void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag);

public:
	virtual BOOL OnInitDialog();

	void	InitPages();
	void	ResetData();
	void	SetData( DxPieceEdit* pData );
	void	PageActive( int nPage );

// Generated message map functions
protected:
	afx_msg HBRUSH CtlColor(CDC* pDc, UINT uCtlColor);	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
