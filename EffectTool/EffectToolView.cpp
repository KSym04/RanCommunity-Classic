
// EffectToolView.cpp : implementation of the CEffectToolView class
//

#include "stdafx.h"
#include "./EffectTool.h"

#include "./EffectToolDoc.h"
#include "./EffectToolView.h"
#include "./SheetWithTab.h"
#include "./MainFrm.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/DxCommon/DxViewPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern int g_nITEMLEVEL;
extern BOOL g_bCHAR_EDIT_RUN;

extern BOOL g_bEffectAABBBox;
extern BOOL g_bEffectTool;

// CEffectToolView

IMPLEMENT_DYNCREATE(CEffectToolView, CView)

BEGIN_MESSAGE_MAP(CEffectToolView, CView)
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_RENDER_DEBUG, OnRenderDebug)
	ON_UPDATE_COMMAND_UI(ID_RENDER_DEBUG, OnUpdateRenderDebug)
	ON_COMMAND(ID_RENDER_PLANE, OnRenderPlane)
	ON_UPDATE_COMMAND_UI(ID_RENDER_PLANE, OnUpdateRenderPlane)

	ON_COMMAND(ID_RENDER_RESETCAMERA, OnRenderResetcamera)
	

	ON_COMMAND(ID_BUTTON_FILE_NEW, &CEffectToolView::OnButtonFileNew)
	ON_COMMAND(ID_BUTTON_FILE_OPEN, &CEffectToolView::OnButtonFileOpen)
	ON_COMMAND(ID_BUTTON_FILE_OPEN2, &CEffectToolView::OnButtonFileOpen2)
	ON_COMMAND(ID_BUTTON_FILE_SAVE, &CEffectToolView::OnButtonFileSave)
	ON_COMMAND(ID_RENDER_RENDERAABB, &CEffectToolView::OnRenderRenderaabb)
	ON_UPDATE_COMMAND_UI(ID_RENDER_RENDERAABB, &CEffectToolView::OnUpdateRenderRenderaabb)
	ON_COMMAND(ID_BUTTON_EFFECT_PLAY, &CEffectToolView::OnButtonEffectPlay)
	ON_COMMAND(ID_BUTTON_EFFECT_STOP, &CEffectToolView::OnButtonEffectStop)
	ON_COMMAND(ID_BUTTON_EFFECT_PAUSE, &CEffectToolView::OnButtonEffectPause)
END_MESSAGE_MAP()

// CEffectToolView construction/destruction

CEffectToolView::CEffectToolView()
	: CD3DApplication()
	, m_bCreated(FALSE)
	, m_hCursorDefault(NULL)
	
	, m_pPropGroup( NULL )
	, m_bRenderDebug( TRUE )
	, m_bRenderPlane( TRUE )
{
	m_bUseDepthBuffer = TRUE;
	D3DXMatrixIdentity ( &m_matLocal );

	g_nITEMLEVEL = 0;
	g_bCHAR_EDIT_RUN = TRUE;

	g_bEffectAABBBox = TRUE;
	g_bEffectTool = TRUE;

	m_pEffSingle = NULL;
	m_pPropGroup = NULL;
	m_strFileName = "";

	m_bEffectFrameMove = TRUE;
	m_bEffectRender = TRUE;
}

CEffectToolView::~CEffectToolView()
{
	SAFE_DELETE ( m_pEffSingle );
	SAFE_DELETE ( m_pPropGroup );

	m_pEffSingle = NULL;
	m_pPropGroup = NULL;
}

BOOL CEffectToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEffectToolView drawing

void CEffectToolView::OnDraw(CDC* /*pDC*/)
{
	//CEffectToolDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	// TODO: add draw code for native data here
}


// CEffectToolView diagnostics

#ifdef _DEBUG
void CEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

// CEffectToolDoc* CEffectToolView::GetDocument() const // non-debug version is inline
// {
// 	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolDoc)));
// 	return (CEffectToolDoc*)m_pDocument;
// }
#endif //_DEBUG


// CEffectToolView message handlers
CEffectToolView * CEffectToolView::GetView()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

	CView * pView = pFrame->GetActiveView();

	if ( !pView )
		return NULL;

	// Fail if view is of wrong kind
	// (this could occur with splitter windows, or additional
	// views on a single document
	if ( ! pView->IsKindOf( RUNTIME_CLASS(CEffectToolView) ) )
		return NULL;

	return (CEffectToolView *) pView;
}

void CEffectToolView::OnRenderDebug()
{
	m_bRenderDebug = !m_bRenderDebug;
}

void CEffectToolView::OnUpdateRenderDebug(CCmdUI *pCmdUI)
{
	if ( pCmdUI )
	{
		pCmdUI->SetCheck( m_bRenderDebug );
	}
}

void CEffectToolView::OnRenderPlane()
{
	m_bRenderPlane = !m_bRenderPlane;
}

void CEffectToolView::OnUpdateRenderPlane(CCmdUI *pCmdUI)
{
	if ( pCmdUI )
	{
		pCmdUI->SetCheck( m_bRenderPlane );
	}
}

void CEffectToolView::OnRenderResetcamera()
{
	D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 0.0f, 70.0f, -70.0f );
	D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
	DxViewPort::GetInstance().MoveVelocityReset();
	DxViewPort::GetInstance().CameraJump ( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
}


void CEffectToolView::OnRenderRenderaabb()
{
	g_bEffectAABBBox = !g_bEffectAABBBox;
}

void CEffectToolView::OnUpdateRenderRenderaabb(CCmdUI *pCmdUI)
{
	if ( pCmdUI )
	{
		pCmdUI->SetCheck( g_bEffectAABBBox );
	}
}


void CEffectToolView::DataReset()
{
	if ( m_pEffSingle )
	{
		m_pEffSingle->CleanUp();
		SAFE_DELETE ( m_pEffSingle );
	}

	if ( m_pPropGroup )	
	{
		m_pPropGroup->InvalidateDeviceObjects();
		m_pPropGroup->DeleteDeviceObjects();
		SAFE_DELETE ( m_pPropGroup );
	}

	m_pEffSingle = NULL;
	m_pPropGroup = NULL;

	CMainFrame	*pMainFrame = (CMainFrame*) AfxGetMainWnd();
	if ( pMainFrame )
	{
		CsheetWithTab*  pSheetTab = pMainFrame->m_wndEditor.m_pSheetTab;
		if ( pSheetTab )
			pSheetTab->ResetData();
	}

	m_strFileName = "";

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	if ( pFrame )
		pFrame->SetWindowText( m_strFileName.c_str() );


	m_bEffectFrameMove = TRUE;
	m_bEffectRender = TRUE;
}

void CEffectToolView::OnButtonFileNew()
{
	DataReset();
}

void CEffectToolView::OnButtonFileOpen()
{
	CString szFilter = "Effect (*.egp)|*.egp;|";
	CFileDialog dlg ( TRUE, ".EGP", DxEffSinglePropGMan::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = DxEffSinglePropGMan::GetInstance().GetPath();
	if ( dlg.DoModal() == IDOK )
	{
		DataReset();

		m_strFileName = dlg.GetFileName().GetString();

		EFF_PROPGROUP*  pPropGroup = new EFF_PROPGROUP;
		HRESULT hr = pPropGroup->LoadFile ( m_strFileName.c_str(), m_pd3dDevice );
		if ( FAILED(hr) )
		{
			CDebugSet::MsgBoxAfx( "Failed to load %s", m_strFileName.c_str() );
			SAFE_DELETE(pPropGroup);
			return;
		}

		m_pEffSingle = pPropGroup->NEWEFFGROUP ();
		if ( !m_pEffSingle )	return;

		m_pPropGroup = NULL;
		m_pPropGroup = pPropGroup;

		STARGETID sTargetID( CROW_MOB, 0, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
		m_pEffSingle->SetTargetID ( &sTargetID );
		m_pEffSingle->m_matWorld = m_matLocal;
		m_pEffSingle->Create ( m_pd3dDevice );


		CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
		if ( pFrame )
			pFrame->SetWindowText( m_strFileName.c_str() );

		CMainFrame	*pMainFrame = (CMainFrame*) AfxGetMainWnd();
		if ( pMainFrame )
		{
			CsheetWithTab*  pSheetTab = pMainFrame->m_wndEditor.m_pSheetTab;
			if ( pSheetTab )
				pSheetTab->ShowData();
		}
	}
}

void CEffectToolView::OnButtonFileOpen2()
{
	CString szFilter = "Effect (*.egp)|*.egp;|";
	CFileDialog dlg ( TRUE, ".EGP", DxEffSinglePropGMan::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = DxEffSinglePropGMan::GetInstance().GetPath();
	if ( dlg.DoModal() == IDOK )
	{
		DataReset();

		m_strFileName = dlg.GetFileName().GetString();

		EFF_PROPGROUP*  pPropGroup = new EFF_PROPGROUP;
		HRESULT hr = pPropGroup->R2LoadFile ( m_strFileName.c_str(), m_pd3dDevice );
		if ( FAILED(hr) )
		{
			CDebugSet::MsgBoxAfx( "Failed to load %s", m_strFileName.c_str() );
			SAFE_DELETE(pPropGroup);
			return;
		}

		m_pEffSingle = pPropGroup->NEWEFFGROUP ();
		if ( !m_pEffSingle )	return;

		m_pPropGroup = NULL;
		m_pPropGroup = pPropGroup;

		STARGETID sTargetID( CROW_MOB, 0, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
		m_pEffSingle->SetTargetID ( &sTargetID );
		m_pEffSingle->m_matWorld = m_matLocal;
		m_pEffSingle->Create ( m_pd3dDevice );


		CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
		if ( pFrame )
			pFrame->SetWindowText( m_strFileName.c_str() );

		CMainFrame	*pMainFrame = (CMainFrame*) AfxGetMainWnd();
		if ( pMainFrame )
		{
			CsheetWithTab*  pSheetTab = pMainFrame->m_wndEditor.m_pSheetTab;
			if ( pSheetTab )
				pSheetTab->ShowData();
		}
	}
}

void CEffectToolView::OnButtonFileSave()
{
	CString szFilter = "Effect (*.egp)|*.egp;|";
	std::string strsavefile = DxEffSinglePropGMan::GetInstance().GetPath();
	strsavefile += m_strFileName;
	CFileDialog dlgInput( false, "*.*", strsavefile.c_str(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter,this);
	dlgInput.m_ofn.lpstrInitialDir =  DxEffSinglePropGMan::GetInstance().GetPath();
	if ( dlgInput.DoModal() != IDOK ) return;

	if ( !m_pPropGroup )	return;

	HRESULT hr = m_pPropGroup->SaveFile( dlgInput.GetPathName().GetString() );
	if ( FAILED( hr ))
	{
		CDebugSet::MsgBoxAfx( "Failed to save %s", dlgInput.GetPathName().GetString() );
		return;
	}

	m_strFileName = dlgInput.GetFileName().GetString();

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	if ( pFrame )
		pFrame->SetWindowText( m_strFileName.c_str() );

	CMainFrame	*pMainFrame = (CMainFrame*) AfxGetMainWnd();
	if ( pMainFrame )
	{
		CsheetWithTab*  pSheetTab = pMainFrame->m_wndEditor.m_pSheetTab;
		if ( pSheetTab )
			pSheetTab->ShowData();
	}
}

void CEffectToolView::OnButtonEffectPlay()
{
	if ( m_pEffSingle )
		m_pEffSingle->ReStartEff();

	m_bEffectFrameMove = TRUE;
	m_bEffectRender = TRUE;
}

void CEffectToolView::OnButtonEffectStop()
{
	m_bEffectFrameMove = FALSE;
	m_bEffectRender = FALSE;
}

void CEffectToolView::OnButtonEffectPause()
{
	m_bEffectFrameMove = !m_bEffectFrameMove;
	m_bEffectRender = TRUE;
}
