#include "stdafx.h"

#include "../G-Logic/GLDefine.h"
#include "../Common/SerialFile.h"
#include "./DxTexEffVisualMaterial.h"
#include "../DxCommon/DxCubeMap.h"
#include "../DxCommon/DxDynamicVB.h"
#include "../G-Logic/glperiod.h"
#include "../DxCommon/VMSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD DxTexEffVisualMaterial::TYPEID = DEF_TEXEFF_VISUALMATERIAL;
const DWORD	DxTexEffVisualMaterial::VERSION = 0x00000102;
const char DxTexEffVisualMaterial::NAME[] = "DxTexEffVisualMaterial";

TEXEFF_VISUALMATERIAL_PROPERTY::TEXEFF_VISUALMATERIAL_PROPERTY()
	: dw0(0)
	, dw1(0)

	, dwVMType(EMVISUALMATERIAL_TYPE_NONE)
	, bWhenDayOffGlow( FALSE )
	, bSpeedGlow(TRUE)
	, fScaleFlow(1.f)
	, fAlphaFlow(1.f)
	, fSpeedGlow(1.f)
	, fAlphaGlow(1.f)
	, fSpeedFlow(1.f)
	, fSpeedRotate(1.f)
	, vSpeedFlow(0.f,1.f)
	, vDirectFlow(0.f,1.f)
	, pAlphaTexFlow(NULL)
	, pFlowTexFlow(NULL)
	, pTexSpec(NULL)
	, pTexGlow(NULL)
	, pTexRotate(NULL)
{
	strVisualMaterial ="";

	for ( int i=0; i<VISUAL_MATERIAL_MAX_TEXTURES; ++i )
	{
		strTextures[i] = "";
	}

	strTexSpec ="";
	strAlphaTexFlow ="";
	strFlowTexFlow ="";
	strTexGlow ="";
	strTexRotate ="";
}

TEXEFF_VISUALMATERIAL_PROPERTY::~TEXEFF_VISUALMATERIAL_PROPERTY()
{
	TextureManager::ReleaseTexture( strTexSpec.c_str(), pTexSpec );
	TextureManager::ReleaseTexture( strAlphaTexFlow.c_str(), pAlphaTexFlow );
	TextureManager::ReleaseTexture( strFlowTexFlow.c_str(), pFlowTexFlow );
	TextureManager::ReleaseTexture( strTexGlow.c_str(), pTexGlow );
	TextureManager::ReleaseTexture( strTexRotate.c_str(), pTexRotate );
}

void TEXEFF_VISUALMATERIAL_PROPERTY::Save( CSerialFile& SFile )
{
	SFile << strVisualMaterial;
	SFile << dw0;
	SFile << dw1;

	for ( DWORD i=0; i<dw0; ++i )
	{
		SFile << strTextures[i];
	}

	SFile << DWORD(4);
	SFile << DWORD(0);
}

void TEXEFF_VISUALMATERIAL_PROPERTY::Load_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	SFile >> strVisualMaterial;
	SFile >> dw0;
	SFile >> dw1;

	TextureManager::ReleaseTexture( strTexSpec.c_str(), pTexSpec );
	TextureManager::ReleaseTexture( strAlphaTexFlow.c_str(), pAlphaTexFlow );
	TextureManager::ReleaseTexture( strFlowTexFlow.c_str(), pFlowTexFlow );
	TextureManager::ReleaseTexture( strTexGlow.c_str(), pTexGlow );
	TextureManager::ReleaseTexture( strTexRotate.c_str(), pTexRotate );

	for ( DWORD i=0; i<dw0; ++i )
	{
		std::string strtexture = "";
		SFile >> strtexture;

		if ( i<VISUAL_MATERIAL_MAX_TEXTURES )
			strTextures[i] = strtexture.c_str();
	}

	if ( dw0 >= VISUAL_MATERIAL_MAX_TEXTURES )
		dw0 = VISUAL_MATERIAL_MAX_TEXTURES - 1;

	GenerateType();

	TextureManager::LoadTexture( strTexSpec.c_str(), pd3dDevice, pTexSpec, 0 , 0 );
	TextureManager::LoadTexture( strAlphaTexFlow.c_str(), pd3dDevice, pAlphaTexFlow, 0 , 0 );
	TextureManager::LoadTexture( strFlowTexFlow.c_str(), pd3dDevice, pFlowTexFlow, 0 , 0 );
	TextureManager::LoadTexture( strTexGlow.c_str(), pd3dDevice, pTexGlow, 0 , 0 );
	TextureManager::LoadTexture( strTexRotate.c_str(), pd3dDevice, pTexRotate, 0 , 0 );
}

void TEXEFF_VISUALMATERIAL_PROPERTY::Load_102( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	SFile >> strVisualMaterial;
	SFile >> dw0;
	SFile >> dw1;

	TextureManager::ReleaseTexture( strTexSpec.c_str(), pTexSpec );
	TextureManager::ReleaseTexture( strAlphaTexFlow.c_str(), pAlphaTexFlow );
	TextureManager::ReleaseTexture( strFlowTexFlow.c_str(), pFlowTexFlow );
	TextureManager::ReleaseTexture( strTexGlow.c_str(), pTexGlow );
	TextureManager::ReleaseTexture( strTexRotate.c_str(), pTexRotate );

	for ( DWORD i=0; i<dw0; ++i )
	{
		std::string strtexture = "";
		SFile >> strtexture;

		if ( i<VISUAL_MATERIAL_MAX_TEXTURES )
			strTextures[i] = strtexture.c_str();
	}

	if ( dw0 >= VISUAL_MATERIAL_MAX_TEXTURES )
		dw0 = VISUAL_MATERIAL_MAX_TEXTURES - 1;

	GenerateType();

	TextureManager::LoadTexture( strTexSpec.c_str(), pd3dDevice, pTexSpec, 0 , 0 );
	TextureManager::LoadTexture( strAlphaTexFlow.c_str(), pd3dDevice, pAlphaTexFlow, 0 , 0 );
	TextureManager::LoadTexture( strFlowTexFlow.c_str(), pd3dDevice, pFlowTexFlow, 0 , 0 );
	TextureManager::LoadTexture( strTexGlow.c_str(), pd3dDevice, pTexGlow, 0 , 0 );
	TextureManager::LoadTexture( strTexRotate.c_str(), pd3dDevice, pTexRotate, 0 , 0 );

	DWORD dwBlockSize = SFile.ReadBlockSize();
	SFile.SetOffSet ( SFile.GetfTell()+dwBlockSize );
}

void TEXEFF_VISUALMATERIAL_PROPERTY::GenerateType()
{
	if ( strVisualMaterial.empty() )	return;

	SVM_PROP_SETTING* prop = GLVMSetting::GetInstance().GetProp( strVisualMaterial.c_str() );
	if ( !prop )
	{
		dwVMType = EMVISUALMATERIAL_TYPE_NONE;
		CDebugSet::ErrorCode( "", "Get VM Prop failed :%s", strVisualMaterial.c_str() );
		return;
	}

	dwVMType = prop->dwVMType;
	bWhenDayOffGlow = prop->bWhenDayOffGlow;
	bSpeedGlow = prop->bSpeedGlow;
	fScaleFlow = prop->fScaleFlow;
	fAlphaFlow = prop->fAlphaFlow;
	fSpeedGlow = prop->fSpeedGlow;
	fAlphaGlow = prop->fAlphaGlow;
	fSpeedFlow = prop->	fSpeedFlow;
	fSpeedRotate = prop->fSpeedRotate;
	vSpeedFlow.x = prop->fSpeedFlowx;
	vSpeedFlow.y = prop->fSpeedFlowy;
	vDirectFlow.x = prop->fDirectFlowx;
	vDirectFlow.y = prop->fDirectFlowy;

	strTexSpec.clear();
	strAlphaTexFlow.clear();
	strFlowTexFlow.clear();
	strTexGlow.clear();
	strTexRotate.clear();

	if ( prop->wTexSpec0 < VISUAL_MATERIAL_MAX_TEXTURES )
		strTexSpec = strTextures[prop->wTexSpec0].c_str();

	if ( strTexSpec.empty() && prop->wTexSpec1 < VISUAL_MATERIAL_MAX_TEXTURES )
		strTexSpec = strTextures[prop->wTexSpec1].c_str();

	if ( prop->wAlphaTexFlow < VISUAL_MATERIAL_MAX_TEXTURES )
		strAlphaTexFlow = strTextures[prop->wAlphaTexFlow].c_str();

	if ( prop->wFlowTexFlow < VISUAL_MATERIAL_MAX_TEXTURES )
		strFlowTexFlow = strTextures[prop->wFlowTexFlow].c_str();

	if ( prop->wTexGlow < VISUAL_MATERIAL_MAX_TEXTURES )
		strTexGlow = strTextures[prop->wTexGlow].c_str();

	if ( prop->wTexRotate < VISUAL_MATERIAL_MAX_TEXTURES )
		strTexRotate = strTextures[prop->wTexRotate].c_str();
}

// Note : DxTexEffVisualMaterial
LPDIRECT3DSTATEBLOCK9	DxTexEffVisualMaterial::m_pSavedSBSpec = NULL;
LPDIRECT3DSTATEBLOCK9	DxTexEffVisualMaterial::m_pDrawSBSpec = NULL;
LPDIRECT3DSTATEBLOCK9	DxTexEffVisualMaterial::m_pSavedSBFlow = NULL;
LPDIRECT3DSTATEBLOCK9	DxTexEffVisualMaterial::m_pDrawSBFlow = NULL;
LPDIRECT3DSTATEBLOCK9	DxTexEffVisualMaterial::m_pSavedSBGlow = NULL;
LPDIRECT3DSTATEBLOCK9	DxTexEffVisualMaterial::m_pDrawSBGlow = NULL;
LPDIRECT3DSTATEBLOCK9	DxTexEffVisualMaterial::m_pSavedSBRotate = NULL;
LPDIRECT3DSTATEBLOCK9	DxTexEffVisualMaterial::m_pDrawSBRotate = NULL;

DxTexEffVisualMaterial::DxTexEffVisualMaterial()
	: m_bUpFlow(TRUE)
	, m_bUpGlow(TRUE)
	, m_fTimeFlow(-D3DX_PI)
	, m_fTimeGlow(-D3DX_PI)
	, m_fTimeRotate(0.f)
	, m_fRandSpeedFlow(1.f)
	, m_fRandSpeedGlow(1.f)
	, m_vLastSpeedFlow(0.f,0.f)
	, m_dwColorFlow(0L)
	, m_dwColorGlow(0L)
	, m_fRotate_ValueENDRotate(0.f)
	, m_fSin_ValueRotate(0.f)
	, m_fCos_ValueRotate(0.f)
{
}

DxTexEffVisualMaterial::~DxTexEffVisualMaterial()
{
}

void DxTexEffVisualMaterial::SetProperty( LPDIRECT3DDEVICEQ pd3dDevice, PBYTE &pProp )
{
	TextureManager::ReleaseTexture( m_sProp.strTexSpec.c_str(), m_sProp.pTexSpec );
	TextureManager::ReleaseTexture( m_sProp.strAlphaTexFlow.c_str(), m_sProp.pAlphaTexFlow );
	TextureManager::ReleaseTexture( m_sProp.strFlowTexFlow.c_str(), m_sProp.pFlowTexFlow );
	TextureManager::ReleaseTexture( m_sProp.strTexGlow.c_str(), m_sProp.pTexGlow );
	TextureManager::ReleaseTexture( m_sProp.strTexRotate.c_str(), m_sProp.pTexRotate );

	TEXEFF_VISUALMATERIAL_PROPERTY* pTemp = (TEXEFF_VISUALMATERIAL_PROPERTY*)pProp;

	m_sProp.strVisualMaterial = pTemp->strVisualMaterial;
	m_sProp.dw0	= pTemp->dw0;
	m_sProp.dw1	= pTemp->dw1;

	for ( int i=0; i<VISUAL_MATERIAL_MAX_TEXTURES; ++i )
	{
		m_sProp.strTextures[i] = pTemp->strTextures[i];
	}

	m_sProp.dwVMType		= pTemp->dwVMType;
	m_sProp.bWhenDayOffGlow	= pTemp->bWhenDayOffGlow;
	m_sProp.bSpeedGlow		= pTemp->bSpeedGlow;
	m_sProp.fScaleFlow		= pTemp->fScaleFlow;
	m_sProp.fAlphaFlow		= pTemp->fAlphaFlow;
	m_sProp.fSpeedGlow		= pTemp->fSpeedGlow;
	m_sProp.fAlphaGlow		= pTemp->fAlphaGlow;
	m_sProp.vSpeedFlow		= pTemp->vSpeedFlow;
	m_sProp.vDirectFlow		= pTemp->vDirectFlow;
	m_sProp.strAlphaTexFlow	= pTemp->strAlphaTexFlow.c_str();
	m_sProp.strFlowTexFlow	= pTemp->strFlowTexFlow.c_str();
	m_sProp.strTexSpec		= pTemp->strTexSpec.c_str();
	m_sProp.strTexGlow		= pTemp->strTexGlow.c_str();
	m_sProp.fSpeedRotate	= pTemp->fSpeedRotate;
	m_sProp.strTexRotate	= pTemp->strTexRotate;

	TextureManager::LoadTexture( m_sProp.strTexSpec.c_str(), pd3dDevice, m_sProp.pTexSpec, 0 , 0 );
	TextureManager::LoadTexture( m_sProp.strAlphaTexFlow.c_str(), pd3dDevice, m_sProp.pAlphaTexFlow, 0 , 0 );
	TextureManager::LoadTexture( m_sProp.strFlowTexFlow.c_str(), pd3dDevice, m_sProp.pFlowTexFlow, 0 , 0 );
	TextureManager::LoadTexture( m_sProp.strTexGlow.c_str(), pd3dDevice, m_sProp.pTexGlow, 0, 0 );
	TextureManager::LoadTexture( m_sProp.strTexRotate.c_str(), pd3dDevice, m_sProp.pTexRotate, 0, 0 );

	m_sProp.fSpeedFlow		= sqrtf( (m_sProp.vSpeedFlow.x*m_sProp.vSpeedFlow.x) + (m_sProp.vSpeedFlow.y*m_sProp.vSpeedFlow.y) );
	if( m_sProp.fSpeedFlow > 0.f )
	{
		m_sProp.vDirectFlow.x	= m_sProp.vSpeedFlow.x / m_sProp.fSpeedFlow;
		m_sProp.vDirectFlow.y	= m_sProp.vSpeedFlow.y / m_sProp.fSpeedFlow;
	}
}

void DxTexEffVisualMaterial::OnCreateDevice( LPDIRECT3DDEVICEQ pd3dDevice )
{
	D3DCAPSQ d3dCaps;
	pd3dDevice->GetDeviceCaps ( &d3dCaps );

	DWORD dwMinFilter;
	if( d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC )	dwMinFilter = D3DTEXF_ANISOTROPIC;
	else if( d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR )	dwMinFilter = D3DTEXF_LINEAR;
	else	dwMinFilter = D3DTEXF_POINT;

	for( UINT which=0; which<2; which++ )
	{
		pd3dDevice->BeginStateBlock();

		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );
		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
		pd3dDevice->SetRenderState( D3DRS_FOGCOLOR,			0x00000000 );

		pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
		pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_ONE );
		pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_ONE );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_MODULATE );

		pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,			D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS,	D3DTTFF_COUNT3 );

		if( which==0 )	pd3dDevice->EndStateBlock ( &m_pSavedSBSpec );
		else			pd3dDevice->EndStateBlock ( &m_pDrawSBSpec );
	}

	for( UINT which=0; which<2; which++ )
	{
		pd3dDevice->BeginStateBlock();

		if( dwMinFilter == D3DTEXF_ANISOTROPIC )
		{
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, dwMinFilter );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

			pd3dDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, dwMinFilter );
			pd3dDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		}

		pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_ONE );
		pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_ONE );

		float fBias = -0.0001f;
		pd3dDevice->SetRenderState( D3DRS_DEPTHBIAS,	*((DWORD*)&fBias) );
		pd3dDevice->SetRenderState( D3DRS_LIGHTING,			FALSE );
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );
		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
		pd3dDevice->SetRenderState( D3DRS_FOGCOLOR,			0x00000000 );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_MODULATE );

		if( which==0 )	pd3dDevice->EndStateBlock ( &m_pSavedSBFlow );
		else			pd3dDevice->EndStateBlock ( &m_pDrawSBFlow );
	}

	for( UINT which=0; which<2; which++ )
	{
		pd3dDevice->BeginStateBlock();

		if( dwMinFilter == D3DTEXF_ANISOTROPIC )
		{
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, dwMinFilter );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		}

		pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_ONE );
		pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_ONE );
		pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,	0xffffffff );

		float fBias = -0.0001f;
		pd3dDevice->SetRenderState( D3DRS_DEPTHBIAS,		*((DWORD*)&fBias) );
		pd3dDevice->SetRenderState( D3DRS_LIGHTING,			FALSE );
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );
		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
		pd3dDevice->SetRenderState( D3DRS_FOGCOLOR,			0x00000000 );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_TFACTOR );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE );

		if ( which==0 )	pd3dDevice->EndStateBlock ( &m_pSavedSBGlow );
		else			pd3dDevice->EndStateBlock ( &m_pDrawSBGlow );
	}

	for( UINT which=0; which<2; which++ )
	{
		pd3dDevice->BeginStateBlock();

		float fBias = -0.0001f;
		pd3dDevice->SetRenderState( D3DRS_DEPTHBIAS,	*((DWORD*)&fBias) );
		pd3dDevice->SetRenderState( D3DRS_LIGHTING,			FALSE );
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );
		pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	TRUE );

		pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

		if( which==0 )	pd3dDevice->EndStateBlock ( &m_pSavedSBRotate );
		else			pd3dDevice->EndStateBlock ( &m_pDrawSBRotate );
	}
}

void DxTexEffVisualMaterial::OnReleaseDevice( LPDIRECT3DDEVICEQ pd3dDevice )
{
	SAFE_RELEASE( m_pSavedSBSpec );
	SAFE_RELEASE( m_pDrawSBSpec );
	SAFE_RELEASE( m_pSavedSBFlow );
	SAFE_RELEASE( m_pDrawSBFlow );
	SAFE_RELEASE( m_pSavedSBGlow );
	SAFE_RELEASE( m_pDrawSBGlow );
	SAFE_RELEASE( m_pSavedSBRotate );
	SAFE_RELEASE( m_pDrawSBRotate );
}

void DxTexEffVisualMaterial::FrameMove( const float fElapsedTime )	
{

	if ( m_sProp.dwVMType == EMVISUALMATERIAL_TYPE_FLOWUV )
	{
		m_fTimeFlow += m_fRandSpeedFlow * fElapsedTime;

		if( m_fTimeFlow > 0.f && m_bUpFlow )		
		{
			m_bUpFlow = FALSE;
			m_fRandSpeedFlow = (RANDOM_POS*5.f+1.f);
		}
		else if( m_fTimeFlow>D3DX_PI && !m_bUpFlow )
		{
			m_fTimeFlow -= D3DX_PI*2.f;
			m_bUpFlow = TRUE;
			m_fRandSpeedFlow = (RANDOM_POS*5.f+1.f);
		}

		float fSpeed = cosf( m_fTimeFlow );
		fSpeed += 2.f;
		fSpeed *= 0.33f;

		m_vLastSpeedFlow += m_sProp.vDirectFlow * (fSpeed*m_sProp.fSpeedFlow) * fElapsedTime;
	}

	if ( m_sProp.dwVMType == EMVISUALMATERIAL_TYPE_DIFFUSE )
	{
		if ( m_sProp.bWhenDayOffGlow && GLPeriod::GetInstance().IsOffLight() )
		{
			m_dwColorGlow = 0xff000000;
			return;
		}

		if( m_sProp.bSpeedGlow )
		{
			m_fTimeGlow += m_sProp.fSpeedGlow * m_fRandSpeedGlow * fElapsedTime;

			if( m_fTimeGlow > 0.f && m_bUpGlow )		
			{
				m_bUpGlow = FALSE;
				m_fRandSpeedGlow = (RANDOM_POS*5.f+1.f);
			}
			else if( m_fTimeGlow>D3DX_PI && !m_bUpGlow )
			{
				m_fTimeGlow -= D3DX_PI*2.f;
				m_bUpGlow = TRUE;
				m_fRandSpeedGlow = (RANDOM_POS*5.f+1.f);
			}

			float fSpeed = cosf( m_fTimeGlow );
			fSpeed += 1.f;
			fSpeed *= 0.5f;

			m_dwColorGlow = (DWORD)(fSpeed*m_sProp.fAlphaGlow*255.f);
			m_dwColorGlow = 0xff000000 + (m_dwColorGlow<<16) + (m_dwColorGlow<<8) + m_dwColorGlow;
		}
		else
		{
			m_dwColorGlow = (DWORD)(m_sProp.fAlphaGlow*255.f);
			m_dwColorGlow = 0xff000000 + (m_dwColorGlow<<16) + (m_dwColorGlow<<8) + m_dwColorGlow;
		}	
	}

	if ( m_sProp.dwVMType == EMVISUALMATERIAL_TYPE_ROTATE )
	{
		m_fRotate_ValueENDRotate += m_sProp.fSpeedRotate * fElapsedTime;
		m_fSin_ValueRotate = sinf( m_fRotate_ValueENDRotate );
		m_fCos_ValueRotate = cosf( m_fRotate_ValueENDRotate );
	}

}

void DxTexEffVisualMaterial::FrameMoveMultiTexFlow( LPDIRECT3DVERTEXBUFFERQ pVB, DWORD dwVert, VERTEX* pVertSrc )
{
	struct VERTEXTEX2 { D3DXVECTOR3 vPos; D3DXVECTOR2 vTex1, vTex2; float fTex3; };
	VERTEXTEX2 sBackUp;
	VERTEXTEX2* pVert;
	HRESULT hr = pVB->Lock( 0, 0, (VOID**)&pVert, D3DLOCK_DISCARD );
	if( FAILED(hr) )	return;
	for( DWORD i=0; i<dwVert; ++i )
	{
		sBackUp.vPos = pVertSrc[i].vPos;
		sBackUp.vTex1 = pVertSrc[i].vTex;
		sBackUp.vTex2 = pVertSrc[i].vTex + m_vLastSpeedFlow;
		sBackUp.fTex3 = 1.f;

		pVert[i] = sBackUp;
	}
	pVB->Unlock();
}

void DxTexEffVisualMaterial::FrameMoveMultiTexRotate( LPDIRECT3DVERTEXBUFFERQ pVB, DWORD dwVert, VERTEX* pVertSrc )
{
	float fUV_X, fUV_Y;
	float fUV_X2, fUV_Y2;
	VERTEX sBackUp;
	VERTEX *pVert;
	pVB->Lock( 0, 0, (VOID**)&pVert, D3DLOCK_DISCARD );
	for( DWORD i=0; i<dwVert; ++i )
	{
		fUV_X = (pVertSrc[i].vTex.x*2.f)-1.f;
		fUV_Y = (pVertSrc[i].vTex.y*2.f)-1.f;

		fUV_X2 = (fUV_X*m_fCos_ValueRotate) + (fUV_Y*m_fSin_ValueRotate);
		fUV_Y2 = (fUV_Y*m_fCos_ValueRotate) - (fUV_X*m_fSin_ValueRotate);

		fUV_X2 = (fUV_X2+1.f)*0.5f;
		fUV_Y2 = (fUV_Y2+1.f)*0.5f;

		sBackUp.vPos = pVertSrc[i].vPos;
		sBackUp.vNor = pVertSrc[i].vNor;
		sBackUp.vTex.x = fUV_X2;
		sBackUp.vTex.y = fUV_Y2;

		pVert[i] = sBackUp;
	}
	pVB->Unlock();
}

void DxTexEffVisualMaterial::RenderFlow( LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DVERTEXBUFFERQ pVB, LPDIRECT3DINDEXBUFFERQ pIB, 
	LPD3DXATTRIBUTERANGE pAttrib, const DWORD dwAttrib, VERTEX* pVertSrc, DWORD dwVert )
{
	FrameMoveMultiTexFlow( pVB, dwVert, pVertSrc );

	SetRenderBeginFlow( pd3dDevice );

	pd3dDevice->SetStreamSource( 0, pVB, 0, sizeof(VERTEX) );
	pd3dDevice->SetIndices( pIB );

	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
		0,
		pAttrib[dwAttrib].VertexStart,
		pAttrib[dwAttrib].VertexCount,
		pAttrib[dwAttrib].FaceStart*3,
		pAttrib[dwAttrib].FaceCount );

	SetRenderEndFlow( pd3dDevice );
}

void DxTexEffVisualMaterial::RenderFlow( LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwVert, const DWORD dwFaces, VERTEXNORCOLORTEX_850* pVertSrc )
{
	DWORD dwFlag = D3DLOCK_NOOVERWRITE; 
	DWORD dwVertexSizeFULL = dwVert*sizeof(VERTEX);
	if( dwVertexSizeFULL > DxDynamicVB::m_sVB_PNT.nFullByte )
	{
		return;
	}

	if( DxDynamicVB::m_sVB_PNT.nOffsetToLock + dwVertexSizeFULL > DxDynamicVB::m_sVB_PNT.nFullByte )
	{
		dwFlag = D3DLOCK_DISCARD; 
		DxDynamicVB::m_sVB_PNT.nVertexCount = 0;
		DxDynamicVB::m_sVB_PNT.nOffsetToLock = 0; 
	}

	struct VERTEXTEX2 { D3DXVECTOR3 vPos; D3DXVECTOR2 vTex1, vTex2; float fTex3; };
	VERTEXTEX2 sBackUp;
	VERTEXTEX2 *pVertices;
	DxDynamicVB::m_sVB_PNT.pVB->Lock( DxDynamicVB::m_sVB_PNT.nOffsetToLock, dwVertexSizeFULL, (VOID**)&pVertices, dwFlag );
	for( DWORD i=0; i<dwVert; ++i )
	{
		sBackUp.vPos = pVertSrc[i].vPos;
		sBackUp.vTex1 = pVertSrc[i].vTex1;
		sBackUp.vTex2 = pVertSrc[i].vTex1 + m_vLastSpeedFlow;
		sBackUp.fTex3 = 1.f;

		pVertices[i] = sBackUp;
	}
	DxDynamicVB::m_sVB_PNT.pVB->Unlock ();

	pd3dDevice->SetStreamSource ( 0, DxDynamicVB::m_sVB_PNT.pVB, 0, sizeof(VERTEX) );
	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, DxDynamicVB::m_sVB_PNT.nVertexCount, 0, dwVert, 0, dwFaces );

	DxDynamicVB::m_sVB_PNT.nVertexCount += dwVert;
	DxDynamicVB::m_sVB_PNT.nOffsetToLock += dwVertexSizeFULL;
}

void DxTexEffVisualMaterial::SetRenderBeginFlow( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_sProp.pAlphaTexFlow )
	{
		TextureManager::GetTexture( m_sProp.strAlphaTexFlow.c_str(), m_sProp.pAlphaTexFlow );
	}
	if( !m_sProp.pFlowTexFlow )
	{
		TextureManager::GetTexture( m_sProp.strFlowTexFlow.c_str(), m_sProp.pFlowTexFlow );
	}
	pd3dDevice->SetTexture( 0, m_sProp.pAlphaTexFlow );
	pd3dDevice->SetTexture( 1, m_sProp.pFlowTexFlow );

	m_pSavedSBFlow->Capture();
	m_pDrawSBFlow->Apply();

	pd3dDevice->SetFVF( D3DFVF_XYZ|D3DFVF_TEX3|D3DFVF_TEXCOORDSIZE1(2) );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_MODULATE2X );
}

void DxTexEffVisualMaterial::SetRenderEndFlow( LPDIRECT3DDEVICEQ pd3dDevice )
{
	m_pSavedSBFlow->Apply();
	pd3dDevice->SetTexture( 1, NULL );
}

void DxTexEffVisualMaterial::RenderSpec( LPDIRECT3DDEVICEQ pd3dDevice, LPD3DXMESH pMesh, const DWORD dwAttrib )
{
	SetRenderBeginSpec( pd3dDevice );
	pMesh->DrawSubset( dwAttrib );
	SetRenderEndSpec( pd3dDevice );
}

void DxTexEffVisualMaterial::SetRenderBeginSpec( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_sProp.pTexSpec )
	{
		TextureManager::GetTexture( m_sProp.strTexSpec.c_str(), m_sProp.pTexSpec );
	}

	pd3dDevice->SetTexture( 0, m_sProp.pTexSpec );
	pd3dDevice->SetTexture( 1, DxCubeMap::GetInstance().GetCubeTexTEST() );

	m_pSavedSBSpec->Capture();
	m_pDrawSBSpec->Apply();
}

void DxTexEffVisualMaterial::SetRenderEndSpec( LPDIRECT3DDEVICEQ pd3dDevice )
{
	m_pSavedSBSpec->Apply();
	pd3dDevice->SetTexture( 1, NULL );
}

void DxTexEffVisualMaterial::RenderGlow( LPDIRECT3DDEVICEQ pd3dDevice, LPD3DXMESH pMesh, const DWORD dwAttrib )
{
	SetRenderBeginGlow( pd3dDevice );
	pMesh->DrawSubset( dwAttrib );
	SetRenderEndGlow( pd3dDevice );
}

void DxTexEffVisualMaterial::SetRenderBeginGlow( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_sProp.pTexGlow )
	{
		TextureManager::GetTexture( m_sProp.strTexGlow.c_str(), m_sProp.pTexGlow );
	}
	pd3dDevice->SetTexture( 0, m_sProp.pTexGlow );

	m_pSavedSBGlow->Capture();
	m_pDrawSBGlow->Apply();

	pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_dwColorGlow );
}

void DxTexEffVisualMaterial::SetRenderEndGlow( LPDIRECT3DDEVICEQ pd3dDevice )
{
	m_pSavedSBGlow->Apply();
}

void DxTexEffVisualMaterial::RenderRotate( LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DVERTEXBUFFERQ pVB, LPDIRECT3DINDEXBUFFERQ pIB, 
	LPD3DXATTRIBUTERANGE pAttrib, const DWORD dwAttrib, VERTEX* pVertSrc, DWORD dwVert )
{
	FrameMoveMultiTexRotate( pVB, dwVert, pVertSrc );

	SetRenderBeginRotate( pd3dDevice );

	pd3dDevice->SetStreamSource( 0, pVB, 0, sizeof(VERTEX) );
	pd3dDevice->SetIndices ( pIB );

	pd3dDevice->DrawIndexedPrimitive ( D3DPT_TRIANGLELIST, 
		0,
		pAttrib[dwAttrib].VertexStart,
		pAttrib[dwAttrib].VertexCount,
		pAttrib[dwAttrib].FaceStart*3,
		pAttrib[dwAttrib].FaceCount );

	SetRenderEndRotate( pd3dDevice );
}

void DxTexEffVisualMaterial::RenderRotate( LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwVert, const DWORD dwFaces, VERTEXNORCOLORTEX_850* pVertSrc )
{
	DWORD dwFlag = D3DLOCK_NOOVERWRITE; 
	DWORD dwVertexSizeFULL = dwVert*sizeof(VERTEX);
	if( dwVertexSizeFULL > DxDynamicVB::m_sVB_PNT.nFullByte )
	{
		return;	
	}
	if( DxDynamicVB::m_sVB_PNT.nOffsetToLock + dwVertexSizeFULL > DxDynamicVB::m_sVB_PNT.nFullByte )
	{
		dwFlag = D3DLOCK_DISCARD; 
		DxDynamicVB::m_sVB_PNT.nVertexCount = 0;
		DxDynamicVB::m_sVB_PNT.nOffsetToLock = 0; 
	}

	float fUV_X, fUV_Y;
	float fUV_X2, fUV_Y2;
	VERTEX sBackUp;
	VERTEX *pVertices;
	DxDynamicVB::m_sVB_PNT.pVB->Lock( DxDynamicVB::m_sVB_PNT.nOffsetToLock, dwVertexSizeFULL, (VOID**)&pVertices, dwFlag );
	for( DWORD i=0; i<dwVert; ++i )
	{
		fUV_X = (pVertSrc[i].vTex1.x*2.f)-1.f;
		fUV_Y = (pVertSrc[i].vTex1.y*2.f)-1.f;

		fUV_X2 = (fUV_X*m_fCos_ValueRotate) + (fUV_Y*m_fSin_ValueRotate);
		fUV_Y2 = (fUV_Y*m_fCos_ValueRotate) - (fUV_X*m_fSin_ValueRotate);

		fUV_X2 = (fUV_X2+1.f)*0.5f;
		fUV_Y2 = (fUV_Y2+1.f)*0.5f;

		sBackUp.vPos = pVertSrc[i].vPos;
		sBackUp.vNor = pVertSrc[i].vNor;
		sBackUp.vTex.x = fUV_X2;
		sBackUp.vTex.y = fUV_Y2;

		pVertices[i] = sBackUp;
	}
	DxDynamicVB::m_sVB_PNT.pVB->Unlock ();

	pd3dDevice->SetStreamSource ( 0, DxDynamicVB::m_sVB_PNT.pVB, 0, sizeof(VERTEX) );

	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, DxDynamicVB::m_sVB_PNT.nVertexCount, 0, dwVert, 0, dwFaces );

	DxDynamicVB::m_sVB_PNT.nVertexCount += dwVert;
	DxDynamicVB::m_sVB_PNT.nOffsetToLock += dwVertexSizeFULL;
}

void DxTexEffVisualMaterial::SetRenderBeginRotate( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_sProp.pTexRotate )
	{
		TextureManager::GetTexture( m_sProp.strTexRotate.c_str(), m_sProp.pTexRotate );
	}
	pd3dDevice->SetTexture( 0, m_sProp.pTexRotate );

	m_pSavedSBRotate->Capture();
	m_pDrawSBRotate->Apply();

	pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_dwColorRotate );

	pd3dDevice->SetFVF( D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1 );
}

void DxTexEffVisualMaterial::SetRenderEndRotate( LPDIRECT3DDEVICEQ pd3dDevice )
{
	m_pSavedSBRotate->Apply();
	pd3dDevice->SetTexture( 1, NULL );
}

void DxTexEffVisualMaterial::SavePSF( CSerialFile& SFile )
{
	SFile << VERSION;
	SFile.BeginBlock( EMBLOCK_02 );
	m_sProp.Save( SFile );
	SFile.EndBlock( EMBLOCK_02 );
}

void DxTexEffVisualMaterial::LoadPSF( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwSize;
	DWORD dwVer;
	SFile >> dwVer;
	SFile >> dwSize;

	if( dwVer==VERSION )
	{
		m_sProp.Load_102( pd3dDevice, SFile );
	}
	else if( dwVer==0x00000100 )
	{
		m_sProp.Load_100( pd3dDevice, SFile );
	}
	else
	{
		DWORD dwCurBuffer = SFile.GetfTell();
		SFile.SetOffSet( dwCurBuffer+dwSize );
	}
}
