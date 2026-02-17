#include "stdafx.h"
#include "./DxSoftShadowMap.h"
#include "../G-Logic/glperiod.h"

#include "../Meshs/DxSkinChar.h"

#include "./TextureManager.h"
#include "./DxViewPort.h"
#include "./DxLightMan.h"
#include "./DxBackUpRendTarget.h"
#include "./DxSurfaceTex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxSoftShadowMap& DxSoftShadowMap::GetInstance()
{
	static DxSoftShadowMap Instance;
	return Instance;
}

const float					DxSoftShadowMap::EYE_TO_LOOKAT			= 220.f;
LPDIRECT3DSTATEBLOCK9		DxSoftShadowMap::m_pSavedStateBlock		= NULL;
LPDIRECT3DSTATEBLOCK9		DxSoftShadowMap::m_pEffectStateBlock	= NULL;


DxSoftShadowMap::DxSoftShadowMap() :
	m_bUse(FALSE),
	m_dwUseSwShader(D3DUSAGE_SOFTWAREPROCESSING),
	m_bUseMIXED(TRUE),
	m_emShadowDetail(SOFT_SHADOW_ENABLE),
	m_fTexWidth(2048.f),
	m_fTexHeight(2048.f),
	m_fDistanceRate(2.0f),
	m_vBaseViewMin(0.f,0.f,0.f),
	m_vBaseViewMax(0.f,0.f,0.f),
	m_vViewMin(0.f,0.f,0.f),
	m_vViewMax(0.f,0.f,0.f),
	m_fDistance(0.f), 
	m_bCharUse(TRUE),
	m_bImageBlurUse(TRUE)
{
	D3DXMatrixIdentity ( &m_matView );
	D3DXMatrixIdentity ( &m_matProj );
	D3DXMatrixIdentity ( &m_matLastWVP_D );
}

DxSoftShadowMap::~DxSoftShadowMap()
{
}

HRESULT DxSoftShadowMap::InitDeviceObjects ( LPDIRECT3DQ pD3D, LPDIRECT3DDEVICEQ pd3dDevice, D3DSURFACE_DESC &d3dsdBackBuffer )
{
	D3DCAPSQ d3dCaps;
	pd3dDevice->GetDeviceCaps ( &d3dCaps );

	D3DDEVICE_CREATION_PARAMETERS d3dCreateParam;
	pd3dDevice->GetCreationParameters ( &d3dCreateParam );


	if ( d3dCreateParam.BehaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
	{
		m_emShadowDetail = SOFT_SHADOW_DISABLE;
	}
	else
	{
		m_emShadowDetail = SOFT_SHADOW_ENABLE;
	}
	

	return S_OK;
}

HRESULT DxSoftShadowMap::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	for( UINT which=0; which<2; which++ )
	{
		pd3dDevice->BeginStateBlock();

		pd3dDevice->SetRenderState ( D3DRS_LIGHTING,			FALSE );
		pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE,		FALSE );
		pd3dDevice->SetRenderState ( D3DRS_FOGENABLE,			FALSE );
		pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,	0xffffffff );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TFACTOR  );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );

		if( which==0 )	pd3dDevice->EndStateBlock( &m_pSavedStateBlock );
		else			pd3dDevice->EndStateBlock( &m_pEffectStateBlock );
	}

	DxSurfaceTex::GetInstance().InitViewProjection ( m_vBaseViewMax, m_vBaseViewMin, m_matProj, EYE_TO_LOOKAT, m_fTexWidth, m_fTexHeight );

	return S_OK;
}

HRESULT DxSoftShadowMap::InvalidateDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	SAFE_RELEASE( m_pSavedStateBlock );
	SAFE_RELEASE( m_pEffectStateBlock );

	return S_OK;
}

HRESULT DxSoftShadowMap::DeleteDeviceObjects()
{
	return S_OK;
}

HRESULT DxSoftShadowMap::FrameMove ( float fTime, float fElapsedTime )
{
	m_bCharUse = TRUE;
	m_bImageBlurUse = TRUE;

	return S_OK;
}

void DxSoftShadowMap::LastImageBlur ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_bUse )								return;
	if( !m_bImageBlurUse )						return;
	if( m_emShadowDetail==SOFT_SHADOW_DISABLE )	return;
	
	DxBackUpRendTarget sBackupTarget ( pd3dDevice );

	pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pShadowSuf2048 );
	pd3dDevice->SetDepthStencilSurface( NULL );
	D3DXImageBlur( pd3dDevice, DxSurfaceTex::GetInstance().m_pTempTex2048, m_fTexWidth, m_fTexHeight );

	sBackupTarget.RestoreTarget ( pd3dDevice );

	m_bImageBlurUse = FALSE;
}

HRESULT DxSoftShadowMap::RenderShadowCharMob ( DxSkinChar *pSkinObj, D3DXMATRIX &matRot, LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_bUse )	return S_OK;

    if ( m_bCharUse )	RenderShadow ( pSkinObj, matRot, pd3dDevice );
	else				RenderShadowMob ( pSkinObj, matRot, pd3dDevice );

	m_bCharUse = FALSE;

	return S_OK;
}

HRESULT DxSoftShadowMap::RenderShadow ( DxSkinChar *pSkinObj, D3DXMATRIX &matRot, LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( m_emShadowDetail == SOFT_SHADOW_DISABLE )	return S_OK;

	D3DXMATRIX matWorld, matView, matProj;
	pd3dDevice->GetTransform ( D3DTS_WORLD, &matWorld );
	pd3dDevice->GetTransform ( D3DTS_VIEW, &matView );
	pd3dDevice->GetTransform ( D3DTS_PROJECTION, &matProj );

	D3DXVECTOR3 &FromPt		= DxViewPort::GetInstance().GetFromPt ();
	D3DXVECTOR3	vCharPos	= D3DXVECTOR3 ( matRot._41, matRot._42, matRot._43 );

	m_fDistance = sqrtf(powf(FromPt.x - vCharPos.x,2) + powf(FromPt.y - vCharPos.y,2) + powf(FromPt.z - vCharPos.z,2) ) * m_fDistanceRate;

	DxBackUpRendTarget sBackupTarget ( pd3dDevice );

	pd3dDevice->SetRenderTarget ( 0, DxSurfaceTex::GetInstance().m_pTempSuf2048 );
	pd3dDevice->SetDepthStencilSurface( DxSurfaceTex::GetInstance().m_pShadowZBuf2048 );
	Shadow_Directional_Light ( pd3dDevice, pSkinObj, matRot );

	sBackupTarget.RestoreTarget ( pd3dDevice );

	pd3dDevice->SetTransform ( D3DTS_WORLD, &matWorld );
	pd3dDevice->SetTransform ( D3DTS_VIEW, &matView );
	pd3dDevice->SetTransform ( D3DTS_PROJECTION, &matProj );

	return S_OK;
}

HRESULT DxSoftShadowMap::RenderShadowMob ( DxSkinChar *pSkinObj, D3DXMATRIX &matRot, LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;

	if( m_emShadowDetail == SOFT_SHADOW_DISABLE )	return S_OK;

	D3DXVECTOR3		vMax, vMin;
	vMax.x = m_vBaseViewMax.x + 50.f;
	vMax.z = m_vBaseViewMax.z + 50.f;
	vMin.x = m_vBaseViewMin.x - 50.f;
	vMin.z = m_vBaseViewMin.z - 50.f;

	D3DXVECTOR3 &LookatPt	= DxViewPort::GetInstance().GetLookatPt ();
	D3DXVECTOR3	vCharPos	= D3DXVECTOR3 ( matRot._41, matRot._42, matRot._43 );
	D3DXVECTOR3	vDis;
	vDis.x	= vCharPos.x - LookatPt.x;
	if ( vDis.x > vMax.x || vDis.x < vMin.x )	return S_OK;
	vDis.z	= vCharPos.z - LookatPt.z;
	if ( vDis.z > vMax.z || vDis.z < vMin.z )	return S_OK;

	D3DXMATRIX matWorld, matView, matProj;
	pd3dDevice->GetTransform ( D3DTS_WORLD, &matWorld );
	pd3dDevice->GetTransform ( D3DTS_VIEW, &matView );
	pd3dDevice->GetTransform ( D3DTS_PROJECTION, &matProj );

	D3DXMATRIX	matIdentity;
	D3DXMatrixIdentity ( &matIdentity );
	pd3dDevice->SetTransform ( D3DTS_VIEW, &m_matLastWVP_D );
	pd3dDevice->SetTransform ( D3DTS_PROJECTION, &matIdentity );

	DxBackUpRendTarget sBackupTarget ( pd3dDevice );

	pd3dDevice->SetRenderTarget ( 0, DxSurfaceTex::GetInstance().m_pTempSuf2048 );
	pd3dDevice->SetDepthStencilSurface( DxSurfaceTex::GetInstance().m_pShadowZBuf2048 );
	Shadow_Directional_Light ( pd3dDevice, pSkinObj, matRot );

	sBackupTarget.RestoreTarget ( pd3dDevice );

	pd3dDevice->SetTransform ( D3DTS_WORLD, &matWorld );
	pd3dDevice->SetTransform ( D3DTS_VIEW, &matView );
	pd3dDevice->SetTransform ( D3DTS_PROJECTION, &matProj );

	return S_OK;
}

HRESULT	DxSoftShadowMap::Shadow_Directional_Light ( LPDIRECT3DDEVICEQ pd3dDevice, DxSkinChar *pSkinObj, D3DXMATRIX &matRot )
{
	HRESULT hr = S_OK;

	D3DXVECTOR3	vLightPos;
	D3DXVECTOR3	vLightDirect;

	DXLIGHT &Light = *DxLightMan::GetInstance()->GetDirectLight ();
	vLightDirect = Light.m_Light.Direction;
	
	D3DXVECTOR3 &LookatPt	= DxViewPort::GetInstance().GetLookatPt ();

	D3DXVECTOR3	 vCharPos	= D3DXVECTOR3 ( matRot._41, matRot._42, matRot._43 );

	D3DXVECTOR3 up;
	up.x = 0.0f; up.y = 1.0f; up.z = 0.0f;

	D3DXMATRIX lightView;

	D3DXMATRIX matWorld, matView, matProj;
	pd3dDevice->GetTransform ( D3DTS_WORLD, &matWorld );
	pd3dDevice->GetTransform ( D3DTS_VIEW, &matView );
	pd3dDevice->GetTransform ( D3DTS_PROJECTION, &matProj );

	float kk	= 0.40f;
	
	float		fDirectFact = GLPeriod::GetInstance().GetDirectionFact();
	if ( fDirectFact < 0.f ) fDirectFact = -fDirectFact;
	fDirectFact *= 0.2f;
	fDirectFact += 0.3f;

	if ( !GLPeriod::GetInstance().IsOffLight() )
		fDirectFact -=0.3f;

	vLightDirect.y = 0.f;
	D3DXVec3Normalize ( &vLightDirect, &vLightDirect );

	vLightDirect.x = vLightDirect.x*fDirectFact;
	vLightDirect.y = fabsf(fDirectFact) - 1.f;
	vLightDirect.z = vLightDirect.z*fDirectFact;

	D3DXVec3Normalize ( &vLightDirect, &vLightDirect );

	kk *= ( (0.7f-fabsf(fDirectFact)) + 0.5f );	


	D3DXVECTOR3 vTempDir;
	vTempDir.x  = LookatPt.x - (vLightDirect.x*40.f);
	vTempDir.y  = LookatPt.y;
	vTempDir.z  = LookatPt.z - (vLightDirect.z*40.f);

	vLightPos.x = vTempDir.x - (vLightDirect.x*(50.f+m_fDistance));
	vLightPos.y = vTempDir.y - (vLightDirect.y*(50.f+m_fDistance));
	vLightPos.z = vTempDir.z - (vLightDirect.z*(50.f+m_fDistance));

	D3DXMatrixLookAtLH(&lightView, &vLightPos, &vTempDir, &up);

	pd3dDevice->SetTransform ( D3DTS_VIEW, &lightView );
	pd3dDevice->SetTransform ( D3DTS_PROJECTION, &m_matProj );

	D3DXMatrixMultiply ( &m_matLastWVP_D, &lightView,	&m_matProj );

	m_pSavedStateBlock->Capture();
	m_pEffectStateBlock->Apply();

	DWORD TexFactor = D3DCOLOR_COLORVALUE(kk,kk,kk,kk);;
	pd3dDevice->SetRenderState ( D3DRS_TEXTUREFACTOR, TexFactor );

	pSkinObj->RenderShadow( pd3dDevice, matRot, TRUE );

	m_pSavedStateBlock->Apply();

	pd3dDevice->SetTransform ( D3DTS_WORLD,			&matWorld );
	pd3dDevice->SetTransform ( D3DTS_VIEW,			&matView );
	pd3dDevice->SetTransform ( D3DTS_PROJECTION,	&matProj );

	return hr;
}

HRESULT DxSoftShadowMap::ClearShadow ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;
	if ( !DxSurfaceTex::GetInstance().m_pShadowSuf2048 )	return S_FALSE;

	if ( m_emShadowDetail == SOFT_SHADOW_DISABLE )
		return S_OK;

	DxBackUpRendTarget sBackupTarget ( pd3dDevice );

	pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pTempSuf2048 );
	pd3dDevice->SetDepthStencilSurface( DxSurfaceTex::GetInstance().m_pShadowZBuf2048 );
	hr = pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L);
	if(FAILED(hr))	return E_FAIL;

	pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pShadowSuf );
	hr = pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L);
	if(FAILED(hr))	return E_FAIL;

	sBackupTarget.RestoreTarget ( pd3dDevice );

	return hr;
}