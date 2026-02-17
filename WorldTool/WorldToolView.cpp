// WorldToolView.cpp : implementation of the CWorldToolView class
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./MainFrm.h"
#include "./WorldToolDoc.h"
#include "./WorldToolView.h"

#include "../Lib_Engine/DxCommon/DxViewPort.h"
#include "../Lib_Engine/G-Logic/GLPeriod.h"
#include "../Lib_Engine/DxSound/StaticSoundMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL	g_bWORLD_TOOL;
extern BOOL g_bPIECE_USE_COLLISION;
extern BOOL g_bPIECE_USE_DISTANCE;

// CWorldToolView

IMPLEMENT_DYNCREATE(CWorldToolView, CView)

BEGIN_MESSAGE_MAP(CWorldToolView, CView)
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_RENDER_DEBUGINFO, &CWorldToolView::OnRenderDebuginfo)
	ON_UPDATE_COMMAND_UI(ID_RENDER_DEBUGINFO, &CWorldToolView::OnUpdateRenderDebuginfo)
	ON_COMMAND(ID_CAMERA_SPEED500, &CWorldToolView::OnCameraSpeed500)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEED500, &CWorldToolView::OnUpdateCameraSpeed500)
	ON_COMMAND(ID_CAMERA_SPEED1000, &CWorldToolView::OnCameraSpeed1000)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEED1000, &CWorldToolView::OnUpdateCameraSpeed1000)
	ON_COMMAND(ID_CAMERA_SPEED1500, &CWorldToolView::OnCameraSpeed1500)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEED1500, &CWorldToolView::OnUpdateCameraSpeed1500)
	ON_COMMAND(ID_CAMERA_SPEEDDEFAULT, &CWorldToolView::OnCameraSpeeddefault)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEEDDEFAULT, &CWorldToolView::OnUpdateCameraSpeeddefault)
	ON_COMMAND(ID_CAMERA_RESETPOSITION, &CWorldToolView::OnCameraResetposition)
	ON_COMMAND(ID_FOGRANGE_RANGE5K, &CWorldToolView::OnFograngeRange5k)
	ON_UPDATE_COMMAND_UI(ID_FOGRANGE_RANGE5K, &CWorldToolView::OnUpdateFograngeRange5k)
	ON_COMMAND(ID_FOGRANGE_RANGE10K, &CWorldToolView::OnFograngeRange10k)
	ON_UPDATE_COMMAND_UI(ID_FOGRANGE_RANGE10K, &CWorldToolView::OnUpdateFograngeRange10k)
	ON_COMMAND(ID_FOGRANGE_RANGE20K, &CWorldToolView::OnFograngeRange20k)
	ON_UPDATE_COMMAND_UI(ID_FOGRANGE_RANGE20K, &CWorldToolView::OnUpdateFograngeRange20k)
	ON_COMMAND(ID_FOGRANGE_RANGE50K, &CWorldToolView::OnFograngeRange50k)
	ON_UPDATE_COMMAND_UI(ID_FOGRANGE_RANGE50K, &CWorldToolView::OnUpdateFograngeRange50k)
	ON_COMMAND(ID_FOGRANGE_RANGEDEFAULT, &CWorldToolView::OnFograngeRangedefault)
	ON_UPDATE_COMMAND_UI(ID_FOGRANGE_RANGEDEFAULT, &CWorldToolView::OnUpdateFograngeRangedefault)
	ON_COMMAND(ID_FOGRANGE_RANGEDEFAULTMID, &CWorldToolView::OnFograngeRangedefaultmid)
	ON_UPDATE_COMMAND_UI(ID_FOGRANGE_RANGEDEFAULTMID, &CWorldToolView::OnUpdateFograngeRangedefaultmid)
	ON_COMMAND(ID_FOGRANGE_RANGE, &CWorldToolView::OnFograngeRange)
	ON_UPDATE_COMMAND_UI(ID_FOGRANGE_RANGE, &CWorldToolView::OnUpdateFograngeRange)
	ON_COMMAND(ID_SKY_BLUETIME, &CWorldToolView::OnSkyBluetime)
	ON_COMMAND(ID_SKY_REDTIME, &CWorldToolView::OnSkyRedtime)
	ON_COMMAND(ID_SKY_NOONTIME, &CWorldToolView::OnSkyNoontime)
	ON_COMMAND(ID_SKY_NIGHTTIME, &CWorldToolView::OnSkyNighttime)
	ON_COMMAND(ID_SKY_FADEOUT, &CWorldToolView::OnSkyFadeout)
	ON_COMMAND(ID_SKY_FADEIN, &CWorldToolView::OnSkyFadein)
	ON_COMMAND(ID_WEATHER_WEATHERRAIN, &CWorldToolView::OnWeatherWeatherrain)
	ON_COMMAND(ID_WEATHER_WEATHERSNOW, &CWorldToolView::OnWeatherWeathersnow)
	ON_COMMAND(ID_WEATHER_WEATHERLEAVES, &CWorldToolView::OnWeatherWeatherleaves)
	ON_COMMAND(ID_WEATHER_WEATHERSPORE, &CWorldToolView::OnWeatherWeatherspore)
	ON_COMMAND(ID_LIGHTMAN_POINTLIGHTRENDER, &CWorldToolView::OnLightmanPointlightrender)
	ON_UPDATE_COMMAND_UI(ID_LIGHTMAN_POINTLIGHTRENDER, &CWorldToolView::OnUpdateLightmanPointlightrender)
	ON_COMMAND(ID_LIGHTMAN_LIGHTNAMERENDER, &CWorldToolView::OnLightmanLightnamerender)
	ON_UPDATE_COMMAND_UI(ID_LIGHTMAN_LIGHTNAMERENDER, &CWorldToolView::OnUpdateLightmanLightnamerender)
	
	ON_COMMAND(ID_RENDER_SOUNDMANRANGERENDER, &CWorldToolView::OnRenderSoundmanrangerender)
	ON_UPDATE_COMMAND_UI(ID_RENDER_SOUNDMANRANGERENDER, &CWorldToolView::OnUpdateRenderSoundmanrangerender)
	ON_COMMAND(ID_RENDER_SOUNDMANNAMERENDER, &CWorldToolView::OnRenderSoundmannamerender)
	ON_UPDATE_COMMAND_UI(ID_RENDER_SOUNDMANNAMERENDER, &CWorldToolView::OnUpdateRenderSoundmannamerender)
	ON_COMMAND(ID_RENDER_SHOWPOSITIONSPHERE, &CWorldToolView::OnRenderShowpositionsphere)
	ON_UPDATE_COMMAND_UI(ID_RENDER_SHOWPOSITIONSPHERE, &CWorldToolView::OnUpdateRenderShowpositionsphere)
	ON_COMMAND(ID_RENDER_RENDERSHADOWWIREFRAME, &CWorldToolView::OnRenderRendershadowwireframe)
	ON_UPDATE_COMMAND_UI(ID_RENDER_RENDERSHADOWWIREFRAME, &CWorldToolView::OnUpdateRenderRendershadowwireframe)
	ON_COMMAND(ID_RENDER_PIECEUSECOLLISION, &CWorldToolView::OnRenderPieceusecollision)
	ON_UPDATE_COMMAND_UI(ID_RENDER_PIECEUSECOLLISION, &CWorldToolView::OnUpdateRenderPieceusecollision)
	ON_COMMAND(ID_RENDER_PIECEUSEDISTANCE, &CWorldToolView::OnRenderPieceusedistance)
	ON_UPDATE_COMMAND_UI(ID_RENDER_PIECEUSEDISTANCE, &CWorldToolView::OnUpdateRenderPieceusedistance)

	ON_COMMAND(ID_RENDER_RENDEREFFECTAABB, &CWorldToolView::OnRenderRendereffectaabb)
	ON_UPDATE_COMMAND_UI(ID_RENDER_RENDEREFFECTAABB, &CWorldToolView::OnUpdateRenderRendereffectaabb)
	ON_COMMAND(ID_RENDER_RENDEREFFECTNAME, &CWorldToolView::OnRenderRendereffectname)
	ON_UPDATE_COMMAND_UI(ID_RENDER_RENDEREFFECTNAME, &CWorldToolView::OnUpdateRenderRendereffectname)
	ON_COMMAND(ID_EDIT_EDITMATRIXMOVE, &CWorldToolView::OnEditEditmatrixmove)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDITMATRIXMOVE, &CWorldToolView::OnUpdateEditEditmatrixmove)
	ON_COMMAND(ID_EDIT_EDITMATRIXROTATE, &CWorldToolView::OnEditEditmatrixrotate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDITMATRIXROTATE, &CWorldToolView::OnUpdateEditEditmatrixrotate)
	ON_COMMAND(ID_EDIT_EDITMATRIXSCALE, &CWorldToolView::OnEditEditmatrixscale)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDITMATRIXSCALE, &CWorldToolView::OnUpdateEditEditmatrixscale)
END_MESSAGE_MAP()

// CWorldToolView construction/destruction

CWorldToolView::CWorldToolView()
	: CD3DApplication()
	, m_bCreated(FALSE)
	, m_hCursorDefault(NULL)
	, m_pLand(NULL)
	, m_pPiece(NULL)
	, m_bDebugInfo(TRUE)
	, m_bShowSphere(TRUE)
	, m_bShadowWireFrame(TRUE)
	, m_vPos(0.0f,0.0f,0.0f)
	, m_strFileName("")
	, m_bRenderEffectAABB(FALSE)
	, m_bRenderEffectName(FALSE)
	, m_bEDITMRS(FALSE)
{
	m_bUseDepthBuffer = TRUE;

	D3DXMatrixIdentity( &m_matLocal );

	g_bWORLD_TOOL = TRUE;
}

CWorldToolView::~CWorldToolView()
{
}

BOOL CWorldToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWorldToolView drawing

void CWorldToolView::OnDraw(CDC* /*pDC*/)
{
	//CWorldToolDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	// TODO: add draw code for native data here
}


// CWorldToolView diagnostics

#ifdef _DEBUG
void CWorldToolView::AssertValid() const
{
	CView::AssertValid();
}

void CWorldToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//CWorldToolDoc* CWorldToolView::GetDocument() const // non-debug version is inline
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWorldToolDoc)));
//	return (CWorldToolDoc*)m_pDocument;
//}
#endif //_DEBUG


// CWorldToolView message handlers
CWorldToolView * CWorldToolView::GetView()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

	CView * pView = pFrame->GetActiveView();

	if ( !pView )
		return NULL;

	// Fail if view is of wrong kind
	// (this could occur with splitter windows, or additional
	// views on a single document
	if ( ! pView->IsKindOf( RUNTIME_CLASS(CWorldToolView) ) )
		return NULL;

	return (CWorldToolView *) pView;
}

void CWorldToolView::OnRenderDebuginfo()
{
	m_bDebugInfo = !m_bDebugInfo;
}

void CWorldToolView::OnUpdateRenderDebuginfo(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	
		pCmdUI->SetCheck( m_bDebugInfo );
}

void CWorldToolView::OnCameraSpeed500()
{
	DxViewPort::GetInstance().MoveVelocitySet( 500.0f );
}

void CWorldToolView::OnUpdateCameraSpeed500(CCmdUI *pCmdUI)
{
	float fVelocity = DxViewPort::GetInstance().MoveVelocityGet();
	if ( pCmdUI )	
		pCmdUI->SetCheck( fVelocity == 500.0f );
}

void CWorldToolView::OnCameraSpeed1000()
{
	DxViewPort::GetInstance().MoveVelocitySet( 1000.0f );
}

void CWorldToolView::OnUpdateCameraSpeed1000(CCmdUI *pCmdUI)
{
	float fVelocity = DxViewPort::GetInstance().MoveVelocityGet();
	if ( pCmdUI )	
		pCmdUI->SetCheck( fVelocity == 1000.0f );
}

void CWorldToolView::OnCameraSpeed1500()
{
	DxViewPort::GetInstance().MoveVelocitySet( 1500.0f );
}

void CWorldToolView::OnUpdateCameraSpeed1500(CCmdUI *pCmdUI)
{
	float fVelocity = DxViewPort::GetInstance().MoveVelocityGet();
	if ( pCmdUI )	
		pCmdUI->SetCheck( fVelocity == 1500.0f );
}

void CWorldToolView::OnCameraSpeeddefault()
{
	DxViewPort::GetInstance().MoveVelocityReset();
}

void CWorldToolView::OnUpdateCameraSpeeddefault(CCmdUI *pCmdUI)
{
	float fVelocity = DxViewPort::GetInstance().MoveVelocityGet();
	if ( pCmdUI )	
		pCmdUI->SetCheck( fVelocity == VIEWPORT_MOVEVELO_DEFAULT );
}

void CWorldToolView::OnCameraResetposition()
{
	DxViewPort::GetInstance().CameraJump( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
}

void CWorldToolView::OnFograngeRange5k()
{
	float fNEAR = 0.0f;
	float fFAR = 0.0f;
	DxFogMan::GetInstance().ManualRangeGet( fNEAR, fFAR );
	DxFogMan::GetInstance().ManualRangeSet( TRUE, fNEAR, 5000.0f );
}

void CWorldToolView::OnUpdateFograngeRange5k(CCmdUI *pCmdUI)
{
	float fNEAR = 0.0f;
	float fFAR = 0.0f;
	DxFogMan::GetInstance().ManualRangeGet( fNEAR, fFAR );
	if ( pCmdUI )	
		pCmdUI->SetCheck( fFAR == 5000.0f );
}

void CWorldToolView::OnFograngeRange10k()
{
	float fNEAR = 0.0f;
	float fFAR = 0.0f;
	DxFogMan::GetInstance().ManualRangeGet( fNEAR, fFAR );
	DxFogMan::GetInstance().ManualRangeSet( TRUE, fNEAR, 10000.0f );
}

void CWorldToolView::OnUpdateFograngeRange10k(CCmdUI *pCmdUI)
{
	float fNEAR = 0.0f;
	float fFAR = 0.0f;
	DxFogMan::GetInstance().ManualRangeGet( fNEAR, fFAR );
	if ( pCmdUI )	
		pCmdUI->SetCheck( fFAR == 10000.0f );
}

void CWorldToolView::OnFograngeRange20k()
{
	float fNEAR = 0.0f;
	float fFAR = 0.0f;
	DxFogMan::GetInstance().ManualRangeGet( fNEAR, fFAR );
	DxFogMan::GetInstance().ManualRangeSet( TRUE, fNEAR, 20000.0f );
}

void CWorldToolView::OnUpdateFograngeRange20k(CCmdUI *pCmdUI)
{
	float fNEAR = 0.0f;
	float fFAR = 0.0f;
	DxFogMan::GetInstance().ManualRangeGet( fNEAR, fFAR );
	if ( pCmdUI )	
		pCmdUI->SetCheck( fFAR == 20000.0f );
}

void CWorldToolView::OnFograngeRange50k()
{
	float fNEAR = 0.0f;
	float fFAR = 0.0f;
	DxFogMan::GetInstance().ManualRangeGet( fNEAR, fFAR );
	DxFogMan::GetInstance().ManualRangeSet( TRUE, fNEAR, 50000.0f );
}

void CWorldToolView::OnUpdateFograngeRange50k(CCmdUI *pCmdUI)
{
	float fNEAR = 0.0f;
	float fFAR = 0.0f;
	DxFogMan::GetInstance().ManualRangeGet( fNEAR, fFAR );
	if ( pCmdUI )	
		pCmdUI->SetCheck( fFAR == 50000.0f );
}

void CWorldToolView::OnFograngeRangedefault()
{
	DxFogMan::GetInstance().ManualRangeSet( FALSE, 0.0f, 0.0f );
	DxFogMan::GetInstance().SetFogRange( EMFR_LOW );
}

void CWorldToolView::OnUpdateFograngeRangedefault(CCmdUI *pCmdUI)
{
	BOOL bDefault = DxFogMan::GetInstance().ManualRangeIsDefault();
	FOGRANGE emRange = DxFogMan::GetInstance().GetFogRange();
	if ( pCmdUI )	
		pCmdUI->SetCheck( emRange == EMFR_LOW && bDefault );
}


void CWorldToolView::OnFograngeRangedefaultmid()
{
	DxFogMan::GetInstance().ManualRangeSet( FALSE, 0.0f, 0.0f );
	DxFogMan::GetInstance().SetFogRange( EMFR_MIDDLE);
}

void CWorldToolView::OnUpdateFograngeRangedefaultmid(CCmdUI *pCmdUI)
{
	BOOL bDefault = DxFogMan::GetInstance().ManualRangeIsDefault();
	FOGRANGE emRange = DxFogMan::GetInstance().GetFogRange();
	if ( pCmdUI )	
		pCmdUI->SetCheck( emRange == EMFR_MIDDLE && bDefault );
}

void CWorldToolView::OnFograngeRange()
{
	DxFogMan::GetInstance().ManualRangeSet( FALSE, 0.0f, 0.0f );
	DxFogMan::GetInstance().SetFogRange( EMFR_HIGH );
}

void CWorldToolView::OnUpdateFograngeRange(CCmdUI *pCmdUI)
{
	BOOL bDefault = DxFogMan::GetInstance().ManualRangeIsDefault();
	FOGRANGE emRange = DxFogMan::GetInstance().GetFogRange();
	if ( pCmdUI )	
		pCmdUI->SetCheck( emRange == EMFR_HIGH && bDefault );
}

void CWorldToolView::OnSkyBluetime()
{
	GLPeriod::GetInstance().SetHour( GLPeriod::GetInstance().GetBlueTime() );
}

void CWorldToolView::OnSkyRedtime()
{
	GLPeriod::GetInstance().SetHour( GLPeriod::GetInstance().GetRedTime() );
}

void CWorldToolView::OnSkyNoontime()
{
	GLPeriod::GetInstance().SetHour( GLPeriod::GetInstance().GetFadeOutTime()-5 );
}

void CWorldToolView::OnSkyNighttime()
{
	GLPeriod::GetInstance().SetHour( GLPeriod::GetInstance().GetFadeInTime()-5 );
}

void CWorldToolView::OnSkyFadeout()
{
	GLPeriod::GetInstance().SetHour( GLPeriod::GetInstance().GetFadeOutTime() );
}

void CWorldToolView::OnSkyFadein()
{
	GLPeriod::GetInstance().SetHour( GLPeriod::GetInstance().GetFadeInTime() );
}

void CWorldToolView::OnWeatherWeatherrain()
{
	GLPeriod::GetInstance().SetWeather( NULL );
	GLPeriod::GetInstance().SetWeather( FGW_RAIN );
}

void CWorldToolView::OnWeatherWeathersnow()
{
	GLPeriod::GetInstance().SetWeather( NULL );
	GLPeriod::GetInstance().SetWeather( FGW_SNOW );
}

void CWorldToolView::OnWeatherWeatherleaves()
{
	GLPeriod::GetInstance().SetWeather( NULL );
	GLPeriod::GetInstance().SetWeather( FGW_LEAVES );
}

void CWorldToolView::OnWeatherWeatherspore()
{
	GLPeriod::GetInstance().SetWeather( NULL );
	GLPeriod::GetInstance().SetWeather( FGW_SPORE );
}

void CWorldToolView::OnLightmanPointlightrender()
{
	BOOL bRender = DxLightMan::GetInstance()->IsEnableRangeRend();
	DxLightMan::GetInstance()->EnableRangeRend(!bRender);
}

void CWorldToolView::OnUpdateLightmanPointlightrender(CCmdUI *pCmdUI)
{
	BOOL bRender = DxLightMan::GetInstance()->IsEnableRangeRend();
	if ( pCmdUI )	
		pCmdUI->SetCheck( bRender );
}

void CWorldToolView::OnLightmanLightnamerender()
{
	BOOL bRender = DxLightMan::GetInstance()->IsDSPName();
	DxLightMan::GetInstance()->SetDSPName(!bRender);
}

void CWorldToolView::OnUpdateLightmanLightnamerender(CCmdUI *pCmdUI)
{
	BOOL bRender = DxLightMan::GetInstance()->IsDSPName();
	if ( pCmdUI )	
		pCmdUI->SetCheck( bRender );
}

void CWorldToolView::OnRenderSoundmanrangerender()
{
	if ( m_pLand )
	{
		CStaticSoundMan* psoundman = m_pLand->GetSoundMan();
		if( psoundman )
		{
			BOOL benable = psoundman->IsRenderRange();
			psoundman->EnableRenderRange( !benable );
		}
	}
}

void CWorldToolView::OnUpdateRenderSoundmanrangerender(CCmdUI *pCmdUI)
{
	if ( m_pLand && pCmdUI )
	{
		CStaticSoundMan* psoundman = m_pLand->GetSoundMan();
		if( psoundman )
		{
			BOOL benable = psoundman->IsRenderRange();
			pCmdUI->SetCheck( benable );
		}
	}
}

void CWorldToolView::OnRenderSoundmannamerender()
{
	if ( m_pLand )
	{
		CStaticSoundMan* psoundman = m_pLand->GetSoundMan();
		if( psoundman )
		{
			BOOL benable = psoundman->IsDSPName();
			psoundman->SetDSPName( !benable );
		}
	}
}

void CWorldToolView::OnUpdateRenderSoundmannamerender(CCmdUI *pCmdUI)
{
	if ( m_pLand && pCmdUI )
	{
		CStaticSoundMan* psoundman = m_pLand->GetSoundMan();
		if( psoundman )
		{
			BOOL benable = psoundman->IsDSPName();
			pCmdUI->SetCheck( benable );
		}
	}
}

void CWorldToolView::OnRenderShowpositionsphere()
{
	m_bShowSphere = !m_bShowSphere;
}

void CWorldToolView::OnUpdateRenderShowpositionsphere(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	
		pCmdUI->SetCheck( m_bShowSphere );
}

void CWorldToolView::OnRenderRendershadowwireframe()
{
	m_bShadowWireFrame = !m_bShadowWireFrame;
}

void CWorldToolView::OnUpdateRenderRendershadowwireframe(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	
		pCmdUI->SetCheck( m_bShadowWireFrame );
}

void CWorldToolView::OnRenderPieceusecollision()
{
	g_bPIECE_USE_COLLISION = !g_bPIECE_USE_COLLISION;
}

void CWorldToolView::OnUpdateRenderPieceusecollision(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	
		pCmdUI->SetCheck( g_bPIECE_USE_COLLISION );
}

void CWorldToolView::OnRenderPieceusedistance()
{
	g_bPIECE_USE_DISTANCE = !g_bPIECE_USE_DISTANCE;
}

void CWorldToolView::OnUpdateRenderPieceusedistance(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	
		pCmdUI->SetCheck( g_bPIECE_USE_DISTANCE );
}

void CWorldToolView::OnRenderRendereffectaabb()
{
	m_bRenderEffectAABB =  !m_bRenderEffectAABB;
}

void CWorldToolView::OnUpdateRenderRendereffectaabb(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	
		pCmdUI->SetCheck( m_bRenderEffectAABB );
}

void CWorldToolView::OnRenderRendereffectname()
{
	m_bRenderEffectName = !m_bRenderEffectName;
}

void CWorldToolView::OnUpdateRenderRendereffectname(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	
		pCmdUI->SetCheck( m_bRenderEffectName );
}


void CWorldToolView::OnEditEditmatrixmove()
{
	m_DxEditMRS.SetMode( OBECTMRS_M );
}

void CWorldToolView::OnUpdateEditEditmatrixmove(CCmdUI *pCmdUI)
{
	if ( pCmdUI )
	{
		pCmdUI->Enable(m_bEDITMRS);
		pCmdUI->SetCheck( m_bEDITMRS && m_DxEditMRS.GetMode() == OBECTMRS_M );
	}
}

void CWorldToolView::OnEditEditmatrixrotate()
{
	m_DxEditMRS.SetMode( OBECTMRS_R );
}

void CWorldToolView::OnUpdateEditEditmatrixrotate(CCmdUI *pCmdUI)
{
	if ( pCmdUI )
	{
		pCmdUI->Enable(m_bEDITMRS);
		pCmdUI->SetCheck( m_bEDITMRS && m_DxEditMRS.GetMode() == OBECTMRS_R );
	}
}

void CWorldToolView::OnEditEditmatrixscale()
{
	m_DxEditMRS.SetMode( OBECTMRS_S );
}

void CWorldToolView::OnUpdateEditEditmatrixscale(CCmdUI *pCmdUI)
{
	if ( pCmdUI )
	{
		pCmdUI->Enable(m_bEDITMRS);
		pCmdUI->SetCheck( m_bEDITMRS && m_DxEditMRS.GetMode() == OBECTMRS_S );
	}
}
