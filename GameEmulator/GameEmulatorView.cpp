
// GameEmulatorView.cpp : implementation of the CGameEmulatorView class
//

#include "stdafx.h"
#include "./GameEmulator.h"

#include "./GameEmulatorDoc.h"
#include "./GameEmulatorView.h"

#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"
#include "../Lib_Engine/Core/NSRPath.h"

#include "./DlgCharset.h"
#include "../Lib_Engine/G-Logic/GLogic.h"
#include "../Lib_Engine/Common/SUBPATH.h"
#include "../Lib_Client/G-Logic/GLogicData.h"

#include "../Lib_Client/DxParamSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameEmulatorView

IMPLEMENT_DYNCREATE(CGameEmulatorView, CView)

BEGIN_MESSAGE_MAP(CGameEmulatorView, CView)
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_EMULATOR_RUN, OnEmulatorRun)
END_MESSAGE_MAP()

// CGameEmulatorView construction/destruction

CGameEmulatorView::CGameEmulatorView()
	: CD3DApplication()
	, m_bCreated(FALSE)
	, m_hCursorDefault(NULL)
{
	m_bUseDepthBuffer	= TRUE;
}

CGameEmulatorView::~CGameEmulatorView()
{
}

BOOL CGameEmulatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGameEmulatorView drawing

void CGameEmulatorView::OnDraw(CDC* /*pDC*/)
{
	//CGameEmulatorDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	// TODO: add draw code for native data here
}


// CGameEmulatorView diagnostics

#ifdef _DEBUG
void CGameEmulatorView::AssertValid() const
{
	CView::AssertValid();
}

void CGameEmulatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//CGameEmulatorDoc* CGameEmulatorView::GetDocument() const // non-debug version is inline
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameEmulatorDoc)));
//	return (CGameEmulatorDoc*)m_pDocument;
//}
#endif //_DEBUG


// CGameEmulatorView message handlers


void CGameEmulatorView::OnEmulatorRun()
{
	CGameEmulatorApp *pApp = (CGameEmulatorApp *) AfxGetApp();

	char szFullPath[MAX_PATH] = {0};
	strcpy ( szFullPath, RPATH::getAppPath() );
	strcat ( szFullPath, SUBPATH::GLOGIC_FILE );
	GLOGIC::SetPath ( szFullPath );

	m_sCharacterSetting.Reset();
	CDlgCharset dlg( this, &m_sCharacterSetting );
	if ( dlg.DoModal() != IDOK ) return;

	DXPARAMSET::INIT ();
	CD3DApplication::SetScreen (ROPTION::dwScrWidth, ROPTION::dwScrHeight, ROPTION::emScrFormat, ROPTION::uScrRefreshHz, TRUE );
	if ( FAILED( CD3DApplication::Create ( m_hWnd, m_hWnd, AfxGetInstanceHandle() ) ) )		return;
	m_bCreated = TRUE;

}
