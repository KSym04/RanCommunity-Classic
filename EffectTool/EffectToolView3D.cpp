
// EffectToolView.cpp : implementation of the CEffectToolView class
//

#include "stdafx.h"
#include "./EffectTool.h"

#include "./EffectToolDoc.h"
#include "./EffectToolView.h"

#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"
#include "../Lib_Engine/Core/NSRPath.h"

#include "../Lib_Engine/DxResponseMan.h"
#include "../Lib_Client/DxGlobalStage.h"
#include "../Lib_Client/G-Logic/GLLandMan.h"
#include "../Lib_Engine/DxOctree/DxLandMan.h"
#include "../Lib_Engine/G-Logic/GLogic.h"
#include "../Lib_Client/G-Logic/GLogicData.h"
#include "../Lib_Client/G-Logic/GLCommentFile.h"

#include "../Lib_Engine/DxCommon/DxInputDevice.h"
#include "../Lib_Engine/Common/SUBPATH.h"
#include "../Lib_Client/DxParamSet.h"
#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Client/G-Logic/GLMapAxisInfo.h"
#include "../Lib_Engine/GUInterface/Cursor.h"
#include "../Lib_Engine/DxCommon/DxViewPort.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Engine/DxCommon/DxGlowMan.h"
#include "../Lib_Engine/DxCommon/DxLightMan.h"
#include "../Lib_Engine/Meshs/DxBoneCollector.h"
#include "../Lib_Engine/Meshs/DxSkinMeshMan.h"
#include "../Lib_Engine/DxCommon/DxPostProcess.h"
#include "../Lib_Engine/DxEffect/EffProj/DxEffProj.h"
#include "../Lib_Engine/DxEffect/Single/DxEffGroupPlayer.h"
#include "../Lib_Engine/DxEffect/DxEffectMan.h"
#include "../Lib_Engine/DxCommon/DxCubeMap.h"
#include "../Lib_Engine/DxCommon/DxDynamicVB.h"
#include "../Lib_Engine/DxCommon9/DXUTmisc.h"
#include "../Lib_Engine/TextTexture/TextUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern std::string	g_strPICK_BONE;

void CEffectToolView::PostNcDestroy()
{
	CD3DApplication::Cleanup3DEnvironment();

	::SetCursor ( NULL );
	::DestroyCursor ( m_hCursorDefault );

	__super::PostNcDestroy();
}

void CEffectToolView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CEffectToolApp *pApp = (CEffectToolApp*) AfxGetApp();
	pApp->SetActive ( bActivate );
	CEffectToolView::SetActive ( bActivate );

	__super::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CEffectToolView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ( m_bCreated )
	{
		ReSizeWindow(cx,cy);

		if ( m_pd3dDevice )
		{
			InvalidateDeviceObjects();
			RestoreDeviceObjects();
		}
	}
}

void CEffectToolView::SetActive ( BOOL bActive )
{
	DxInputDevice::GetInstance().OnActivate ( bActive );
}

BOOL CEffectToolView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR hCursl = CCursor::GetInstance().GetCurCursor();
	if ( !hCursl )
	{
		if ( !m_hCursorDefault )		m_hCursorDefault = LoadCursor ( NULL, IDC_ARROW );
		hCursl = m_hCursorDefault;
	}

	if ( hCursl )	::SetCursor ( hCursl );

	return TRUE;
}


void CEffectToolView::OnInitialUpdate()
{
	char szFullPath[MAX_PATH] = {0};
	strcpy ( szFullPath, RPATH::getAppPath() );
	strcat ( szFullPath, SUBPATH::GLOGIC_FILE );
	GLOGIC::SetPath ( szFullPath );

	CGLCommentFile glcomment;
	glcomment.LOADFILE( "comment.ini" );

	BOOL bOK = RPARAM::Init ();
	DXPARAMSET::INIT ();

	if (!bOK )
	{
		MessageBox("Unable to Load PARAM.ini!","Error", MB_OK );
	}
	else
	{
		CD3DApplication::SetScreen (ROPTION::dwScrWidth, ROPTION::dwScrHeight, ROPTION::emScrFormat, ROPTION::uScrRefreshHz, TRUE );
		if ( FAILED( CD3DApplication::Create ( m_hWnd, m_hWnd, AfxGetInstanceHandle() ) ) )		return;
		m_bCreated = TRUE;
	}

	__super::OnInitialUpdate();
}

HRESULT CEffectToolView::FrameMove3DEnvironment ()
{
	HRESULT hr=S_OK;

	PROFILE_BLOCKSTART();

	if ( m_pd3dDevice )
	{
		hr = CD3DApplication::FrameMove3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CEffectToolView::Render3DEnvironment ()
{
	HRESULT hr=S_OK;

	PROFILE_BLOCKSTART();

	if ( m_pd3dDevice )
	{
		hr = CD3DApplication::Render3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CEffectToolView::OneTimeSceneInit()
{
	HRESULT hr = S_OK;

	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create( RPATH::getAppPath() );
	CCursor::GetInstance().SetGameCursor ( ROPTION::bGameCursor );

	return S_OK;
}

HRESULT CEffectToolView::ConfirmDevice ( D3DCAPSQ* pCaps, DWORD dwBehavior, D3DFORMAT Format )
{
	if ( dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectToolView::CreateObjects()
{
	DxFontMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );
	CD3DFontPar* pD3dFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	CDebugSet::InitDeviceObjects( pD3dFont9 );

	HRESULT hr = S_OK;

	hr = InitDeviceObjects();
	if( FAILED(hr) )	
	{
		CDebugSet::MsgBox( "CRanEditSkinPieceView::CreateObjects()" );
		return hr;
	}

	hr = RestoreDeviceObjects();	
	if( FAILED(hr) )	
	{
		CDebugSet::MsgBox( "CRanEditSkinPieceView::CreateObjects()" );
		return hr;
	}

	return hr;
}

HRESULT CEffectToolView::InitDeviceObjects()
{
	HRESULT hr = S_OK;

	DxViewPort::GetInstance().InitDeviceObjects ( m_pd3dDevice, m_hWnd );
	DxResponseMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );
	GLogicData::GetInstance().InitDeviceObjects ( m_pd3dDevice );

	DXLIGHT sDirectional;
	sDirectional.SetDefault();
	sDirectional.m_Light.Diffuse = D3DXCOLOR(1.1f,1.1f,1.1f,1);
	sDirectional.m_Light.Ambient = D3DXCOLOR(1.1f,1.1f,1.1f,1);
	DxLightMan::SetDefDirect ( sDirectional );

	hr = m_pd3dDevice->GetDeviceCaps ( &m_d3dCaps );
	DxBoneCollector::GetInstance().PreLoad ( _PRELOAD_BONE, m_pd3dDevice );
	DxSkinAniMan::GetInstance().PreLoad ( _PRELOAD_ANI, m_pd3dDevice );
	DxSkinMeshMan::GetInstance().PreLoad( _PRELOAD_SKIN, m_pd3dDevice, FALSE );

	WORD wWidth = 1024; 
	WORD wHeight = 768;

	LPDIRECT3DSURFACEQ pBackBuffer=NULL;
	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

	if ( pBackBuffer )
	{
		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();

		wWidth = static_cast<WORD> ( m_d3dsdBackBuffer.Width );
		wHeight = static_cast<WORD> ( m_d3dsdBackBuffer.Height );
	}

	DxViewPort::GetInstance().SetCameraType ((CAMERA_TYPE)2);

	D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 0.0f, 70.0f, -70.0f );
	D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
	DxViewPort::GetInstance().MoveVelocityReset();
	DxViewPort::GetInstance().CameraJump ( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	

	if ( m_pEffSingle )	
		m_pEffSingle->InitDeviceObjects( m_pd3dDevice );



	CCursor::GetInstance().InitDeviceObjects ();
	DxResponseMan::GetInstance().SetRenderState ();

	return S_OK;
}

HRESULT CEffectToolView::RestoreDeviceObjects()
{
	HRESULT hr=S_OK;

	DxResponseMan::GetInstance().RestoreDeviceObjects ();

	if ( m_pd3dDevice )
	{
		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )
		{
			m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE,		TRUE );
		}

		float fFogStart=720.0f, fFogEnd=790.0f, fFongDensity=0.0f;

		m_pd3dDevice->SetRenderState ( D3DRS_FOGSTART,		*((DWORD *)(&fFogStart)) );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGEND,		*((DWORD *)(&fFogEnd)) );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGDENSITY,	*((DWORD *)(&fFongDensity)) );

		m_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,	D3DFOG_LINEAR );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGTABLEMODE,	D3DFOG_NONE );

		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGRANGE )
		{
			m_pd3dDevice->SetRenderState ( D3DRS_RANGEFOGENABLE,	TRUE );
		}

		HRESULT hr;
		LPDIRECT3DSURFACEQ pBackBuffer;
		m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();

		DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
			(float)m_d3dsdBackBuffer.Height, 1.0f, 1500000.0f );

		hr = DxViewPort::GetInstance().SetViewPort ();
		if ( FAILED(hr) )	return hr;

		

		if ( m_pEffSingle )	
			m_pEffSingle->RestoreDeviceObjects( m_pd3dDevice );

		
	}

	DXPARAMSET::INIT ();
	DxGlowMan::GetInstance().SetProjectActiveON();
	DxPostProcess::GetInstance().SetProjectActiveON();

	PROFILE_INIT();

	return S_OK;
}

HRESULT CEffectToolView::FrameMove()
{
	PROFILE_BEGIN("FrameMove");

	CheckControlfp();
	GLPeriod::GetInstance().UpdateWeather ( m_fElapsedTime );
	DxResponseMan::GetInstance().FrameMove( m_fTime, m_fElapsedTime, m_bDefWin );

	DxEffProjMan::GetInstance().FrameMove( m_fElapsedTime );
	DxViewPort::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );

	if ( m_pEffSingle && m_bEffectFrameMove )	
		m_pEffSingle->FrameMove( m_fTime, m_fElapsedTime );

	DxInputDevice &dxInputDev = DxInputDevice::GetInstance();
	DWORD dwML = dxInputDev.GetMouseState ( DXMOUSE_LEFT );
	bool bCLICK_LEFT = NULL != (dwML&(DXKEY_DOWNED|DXKEY_UP|DXKEY_DUP));

	D3DXVECTOR3 vTargetPt, vFromPt;
	vFromPt = DxViewPort::GetInstance().GetFromPt ();
	BOOL bOk = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vTargetPt );

	//if ( bOk && bCLICK_LEFT && ( dwML&DXKEY_UP ) && !m_bEditMRS )

	if ( bOk && bCLICK_LEFT && ( dwML&DXKEY_UP ) )
	{
		
	}

	PROFILE_END("FrameMove");

	return S_OK;
}

HRESULT CEffectToolView::Render()
{
	//render from emulator
	if ( !m_pd3dDevice )	return S_FALSE;

	PROFILE_BEGIN("Render");

	D3DCOLOR colorClear = D3DCOLOR_XRGB(100,100,100);
	HRESULT hr = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,colorClear, 1.0f, 0L );

	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		DxResponseMan::GetInstance().Render( m_pd3dDevice );
		DxGlowMan::GetInstance().RenderTex ( m_pd3dDevice );
		DxLightMan::GetInstance()->Render ( m_pd3dDevice );
		DXLIGHT &Light = *DxLightMan::GetInstance()->GetDirectLight ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT, (float*)&Light.m_Light.Direction, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIFFUSE, (float*)&Light.m_Light.Diffuse, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTAMBIENT, (float*)&Light.m_Light.Ambient, 1 );
		D3DXVECTOR3 &vFromPt = DxViewPort::GetInstance().GetFromPt ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_CAMERAPOSITION, (float*)&vFromPt, 1 );

		D3DLIGHTQ	pLight;
		D3DXVECTOR4	vPointPos;
		D3DXVECTOR3	vPointDiff;
		for ( int i=0; i<7; i++ )
		{
			if ( DxLightMan::GetInstance()->GetClosedLight(i+1) )
			{
				pLight = DxLightMan::GetInstance()->GetClosedLight(i+1)->m_Light;
				vPointDiff = D3DXVECTOR3 ( pLight.Diffuse.r, pLight.Diffuse.g, pLight.Diffuse.b );
				vPointPos.x = pLight.Position.x;
				vPointPos.y = pLight.Position.y;
				vPointPos.z = pLight.Position.z;
				vPointPos.w = pLight.Range;
			}else{
				vPointPos = D3DXVECTOR4 ( 0.f, 0.f, 0.f, 0.1f );
				vPointDiff = D3DXVECTOR3 ( 0.f, 0.f, 0.f );
			}
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTPOS01, (float*)&vPointPos, 1);
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTDIFF01, (float*)&vPointDiff, 1);
		}

		D3DXMATRIX matView = DxViewPort::GetInstance().GetMatView();
		D3DXMATRIX matProj = DxViewPort::GetInstance().GetMatProj();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_SKIN_DEFAULT, (float*)&D3DXVECTOR4 (1.f, 0.5f, 0.f, 765.01f), 1 );
		D3DXVECTOR3	vLightVector = DxLightMan::GetInstance()->GetDirectLight()->m_Light.Direction;
		D3DXVec3TransformNormal ( &vLightVector, &vLightVector, &matView );
		D3DXVec3Normalize ( &vLightVector, &vLightVector);
		vLightVector = -vLightVector;
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT_VIEW, (float*)&vLightVector, 1 );
		D3DXMatrixTranspose( &matView, &matView );
		D3DXMatrixTranspose( &matProj, &matProj );
		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity( &matIdentity );
		m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE );	

		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )		
			m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, FALSE );

		CLIPVOLUME &CV = DxViewPort::GetInstance().GetClipVolume ();

		if ( m_bRenderPlane )
		{
			D3DXVECTOR3 vCenter ( 0.0f, 0.f, 0.0f );
			D3DXVECTOR3 vSize ( 20.0f, 0.f, 20.0f );
			D3DXVECTOR3 vSnap ( 5.0f, 0, 5.0f );

			DWORD dwZFunc;
			m_pd3dDevice->GetRenderState ( D3DRS_ZFUNC,		&dwZFunc );
			m_pd3dDevice->SetRenderState ( D3DRS_ZFUNC,		D3DCMP_ALWAYS );
			EDITMESHS::RENDERPLANE ( m_pd3dDevice, vCenter, vSize, vSnap );
			m_pd3dDevice->SetRenderState ( D3DRS_ZFUNC,		dwZFunc );
		}

		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )		
			m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, TRUE );

		

		D3DLIGHT9 light;
		D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 0.0f, 10.0f, -5.0f );
		light.Direction = DxViewPort::GetInstance().GetLookDir();
		m_pd3dDevice->SetLight(0, &light);

		
		if ( m_pEffSingle && m_bEffectRender )
			m_pEffSingle->Render( m_pd3dDevice );




		DxEffGroupPlayer::GetInstance().Render ( m_pd3dDevice );
		OPTMManager::GetInstance().Render( m_pd3dDevice );

		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )		
			m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, FALSE );

		DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
		DxCubeMap::GetInstance().Render ( m_pd3dDevice );
		DxGlowMan::GetInstance().RenderTex ( m_pd3dDevice );
		DxGlowMan::GetInstance().Render ( m_pd3dDevice );
		DxGlowMan::GetInstance().RenderBURN( m_pd3dDevice );
		DxPostProcess::GetInstance().Render( m_pd3dDevice );
		DxEffProjMan::GetInstance().Render( m_pd3dDevice );

		CCursor::GetInstance().Render ( m_pd3dDevice, DxInputDevice::GetInstance().GetMouseLocateX(), DxInputDevice::GetInstance().GetMouseLocateY() );
		m_pd3dDevice->EndScene();
	}

	if ( !m_bRenderDebug )
	{
		PROFILE_END("Render");
		return S_OK;
	}

	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,0,0);
	D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255,255,0,0);
	TCHAR szMsg[MAX_PATH] = TEXT("");

	CD3DFontPar* pD3dFont = DxFontMan::GetInstance().FindFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	if ( !pD3dFont )	return S_OK;

	CTextUtil::Get()->Render( TRUE );

	FLOAT fNextLine = 40.0f; 

	lstrcpy( szMsg, m_strDeviceStats );
	fNextLine -= 20.0f;
	pD3dFont->DrawText( 2, fNextLine, fontColor, szMsg );

	lstrcpy( szMsg, m_strFrameStats );
	fNextLine -= 20.0f;
	pD3dFont->DrawText( 2, fNextLine, fontColor, szMsg );

	CDebugSet::Render ();

	EMANI_MAINTYPE emMAIN = AN_NONE;
	EMANI_SUBTYPE emSUB = AN_SUB_NONE;


	CTextUtil::Get()->Render( FALSE );

	PROFILE_END("Render");
	return S_OK;
}

HRESULT CEffectToolView::InvalidateDeviceObjects()
{
	DxResponseMan::GetInstance().InvalidateDeviceObjects ();

	if ( m_pEffSingle )
		m_pEffSingle->InvalidateDeviceObjects();

	if ( m_pPropGroup )
		m_pPropGroup->InvalidateDeviceObjects();

	return S_OK;
}

HRESULT CEffectToolView::DeleteDeviceObjects()
{
#ifdef _DEBUG
	_CrtCheckMemory();
#endif //_DEBUG

	
	if ( m_pEffSingle )	
		m_pEffSingle->DeleteDeviceObjects();

	if ( m_pPropGroup )
		m_pPropGroup->DeleteDeviceObjects();

	DxResponseMan::GetInstance().DeleteDeviceObjects ();
	GLogicData::GetInstance().DeleteDeviceObjects ();
	CCursor::GetInstance().DeleteDeviceObjects ();

	return S_OK;
}

HRESULT CEffectToolView::FinalCleanup()
{
	CDebugSet::FinalCleanup ();
	GLogicData::GetInstance().ClearData ();
	DxResponseMan::GetInstance().FinalCleanup ();

	if ( m_pEffSingle )	
		m_pEffSingle->FinalCleanup();

	return S_OK;
}