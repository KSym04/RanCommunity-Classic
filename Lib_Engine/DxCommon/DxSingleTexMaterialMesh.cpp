#include "stdafx.h"
#include "./DxSingleTexMaterialMesh.h"

#include "./TextureManager.h"
#include "./DxLightMan.h"

#include "../DxEffect/DxTexEffMan.h"
#include "../DxEffect/DxTexEffDiffuse.h"
#include "../DxEffect/DxTexEffFlowUV.h"
#include "../DxEffect/DxTexEffRotate.h"
#include "../DxEffect/DxTexEffSpecular.h"
#include "../DxEffect/DxTexEffVisualMaterial.h"

#include "../G-Logic/GLPeriod.h"
#include "./DxRenderStates.h"

#include "./DxMaterialManager.h"
#include "./DxShaderManager.h"
#include "./DxEnvironment.h"
#include "./DxViewPort.h"
#include "./MaterialShaderParam.h"
#include "./DxShadowMap.h"
#include "./DxFogMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DxSingleTexMaterialMesh::DxSingleTexMaterialMesh() :
	m_pAABBOctree(NULL),
	m_pTexture(NULL),
	m_dwFlag(0L),
	m_pNext(NULL),
	unknown_var1(FALSE),
	m_fCubeMapValue(1.0f),
	m_fSpecularPower(1.0f),
	m_fSpecularIntensity(1.0f),
	unknown_var5(0),
	unknown_var6(0),
	m_bUseLightMap(TRUE),
	m_pMaterial(NULL),
	m_pEffect(NULL)
{
	m_szMaterial = "";
	m_szTexName = "";
};

DxSingleTexMaterialMesh::~DxSingleTexMaterialMesh()
{
	CleanUp();

	m_pNext = NULL;
}

void DxSingleTexMaterialMesh::CleanUp()
{
	SAFE_DELETE ( m_pAABBOctree );

	if ( m_pMaterial )
		m_pMaterial->ReleaseTexture();

	TextureManager::ReleaseTexture( m_szTexName.c_str(), m_pTexture );
	m_szTexName = "";

	m_pMaterial = NULL;
	m_pEffect = NULL;
}

void DxSingleTexMaterialMesh::Create ( const LPDIRECT3DDEVICEQ pd3dDevice, const char* szName, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, BOOL bPiece )
{
	CleanUp();

	// Note : Object의 타입을 설정한다.
	EM_LAND_TYPE emLandType(EMLR_PIECE);
	if( !bPiece )
	{
		if( DxLightMan::GetInstance()->IsNightAndDay() )	emLandType = EMLR_DAYNIGHT_ON;
		else												emLandType = EMLR_DAYNIGHT_OFF;		
	}

	SAFE_DELETE( m_pAABBOctree );
	m_pAABBOctree = new DxAABBOctreeLightMap;
	m_pAABBOctree->Create( pd3dDevice, dwFaceNUM, pVertices, dwFVF, emLandType );

	m_szTexName = szName;
}

void DxSingleTexMaterialMesh::FrameMoveAABB( const float fTime, const float fElapsedTime )
{
	if ( m_pMaterial )
	{
		m_pMaterial->FrameMove( this,  fTime, fElapsedTime );
	}

	if ( m_pEffect )
	{
		m_pEffect->SetFloat( "g_fTime", (float)fTime );
	}
}

HRESULT DxSingleTexMaterialMesh::RenderTHREAD ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading )
{
	if ( !m_pAABBOctree )	return S_OK;

	if( !m_pTexture )	TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );
	if ( m_pTexture )	pd3dDevice->SetTexture( 0, m_pTexture );

	m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV, listLoadingData, CSLockLoading );

	return S_OK;
}

HRESULT DxSingleTexMaterialMesh::RenderDYNAMIC ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, CSerialFile* const SFile )
{
	if ( !m_pAABBOctree )	return S_OK;

	if( !m_pTexture )	TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );
	if ( m_pTexture )	pd3dDevice->SetTexture( 0, m_pTexture );

	m_pAABBOctree->RenderDYNAMIC( pd3dDevice, sCV, SFile );

	return S_OK;
}


HRESULT DxSingleTexMaterialMesh::FirstLoad( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin, CSerialFile* const SFile )
{
	if ( !m_pAABBOctree )	return S_OK;

	if( !m_pTexture )	TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );
	if ( m_pTexture )	pd3dDevice->SetTexture( 0, m_pTexture );

	m_pAABBOctree->FirstLoad( pd3dDevice, vMax, vMin, SFile );

	return S_OK;
}

HRESULT DxSingleTexMaterialMesh::Render( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV )
{
	if ( !m_pAABBOctree )	return S_OK;

	return S_OK;
}

void DxSingleTexMaterialMesh::Render( const LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( !m_pAABBOctree )	return;

	if( !m_pTexture )	TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );
	if ( m_pTexture )	pd3dDevice->SetTexture( 0, m_pTexture );

	m_pAABBOctree->Render( pd3dDevice );

	return;
}


HRESULT DxSingleTexMaterialMesh::Render_THREAD( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	if ( !m_pAABBOctree )	return S_OK;

	if( !m_pTexture )	TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );

	if ( m_pMaterial && m_pMaterial->m_pEffect )
	{
		BOOL bOK = m_pMaterial->ApplyMaterialShader( pd3dDevice, this, &material );
		if ( bOK )
		{
			UINT numPasses = 0;
			HRESULT hr = m_pMaterial->m_pEffect->Begin(&numPasses, 0 );
			if ( SUCCEEDED(hr) )
			{
				for( UINT i=0; i<numPasses; ++i )
				{
					m_pMaterial->m_pEffect->BeginPass(i);
					m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV );
					m_pMaterial->m_pEffect->EndPass();
				}

				m_pMaterial->m_pEffect->End();

				pd3dDevice->SetVertexShader(NULL);
				pd3dDevice->SetPixelShader(NULL);
			}
		}
	}
	else
	{
		BOOL bOK = ApplyShader( pd3dDevice, &material );
		if ( bOK )
		{
			UINT numPasses = 0;
			HRESULT hr = m_pEffect->Begin(&numPasses, D3DXFX_DONOTSAVESTATE );
			if ( SUCCEEDED(hr) )
			{
				for( UINT i=0; i<numPasses; ++i )
				{
					m_pEffect->BeginPass(i);
					m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV );
					m_pEffect->EndPass();
				}

				m_pEffect->End();

				pd3dDevice->SetVertexShader(NULL);
				pd3dDevice->SetPixelShader(NULL);
			}
		}
		else
		{
			if ( m_pTexture )	pd3dDevice->SetTexture( 0, m_pTexture );
			m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV );
		}
	}

	return S_OK;
}


HRESULT DxSingleTexMaterialMesh::Render_PIECE( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material )
{
	if ( !m_pAABBOctree )	return S_OK;

	if( !m_pTexture )	TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );

	if ( m_pMaterial && m_pMaterial->m_pEffect )
	{
		BOOL bOK = m_pMaterial->ApplyMaterialShader( pd3dDevice, this, &material );
		if ( bOK )
		{
			UINT numPasses = 0;
			HRESULT hr = m_pMaterial->m_pEffect->Begin(&numPasses, 0 );
			if ( SUCCEEDED(hr) )
			{
				for( UINT i=0; i<numPasses; ++i )
				{
					m_pMaterial->m_pEffect->BeginPass(i);
					m_pAABBOctree->Render( pd3dDevice );
					m_pMaterial->m_pEffect->EndPass();
				}

				m_pMaterial->m_pEffect->End();

				pd3dDevice->SetVertexShader(NULL);
				pd3dDevice->SetPixelShader(NULL);
			}
		}
	}
	else
	{
		BOOL bOK = ApplyShader( pd3dDevice, &material );
		if ( bOK )
		{
			UINT numPasses = 0;
			HRESULT hr = m_pEffect->Begin(&numPasses, D3DXFX_DONOTSAVESTATE );
			if ( SUCCEEDED(hr) )
			{
				for( UINT i=0; i<numPasses; ++i )
				{
					m_pEffect->BeginPass(i);
					m_pAABBOctree->Render( pd3dDevice );
					m_pEffect->EndPass();
				}

				m_pEffect->End();

				pd3dDevice->SetVertexShader(NULL);
				pd3dDevice->SetPixelShader(NULL);
			}
		}
		else
		{
			if ( m_pTexture )	pd3dDevice->SetTexture( 0, m_pTexture );
			m_pAABBOctree->Render( pd3dDevice );
		}
	}

	return S_OK;
}

void DxSingleTexMaterialMesh::InsertColorList( LOADINGDATALIST &listColorData )
{
	if( !m_pAABBOctree )	return;
	m_pAABBOctree->InsertColorList( listColorData );
}

BOOL DxSingleTexMaterialMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, 
	LPCSTR& szName, const BOOL bFrontColl )
{
	if ( !m_pAABBOctree )	return FALSE;

	if ( m_pAABBOctree->IsCollisionLine( vStart, vEnd, vColl, vNor, bFrontColl ) )
	{
		szName = m_szTexName.c_str();
		return TRUE;
	}
	else	return FALSE;
}

BOOL DxSingleTexMaterialMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl )
{
	if ( !m_pAABBOctree )	return FALSE;
	if ( m_pAABBOctree->IsCollisionLine( vStart, vEnd, vColl, vNor, bFrontColl ) )	return TRUE;
	else																			return FALSE;
}

BOOL DxSingleTexMaterialMesh::ApplyShader( LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING* pmaterial )
{
	if( !pmaterial )	return FALSE;
	if( !m_pEffect )	return FALSE;
	
	m_pEffect->SetMatrix( "g_matWorld", &pmaterial->g_matWorld );
	m_pEffect->SetMatrix( "g_matWV", &pmaterial->g_matWV );
	m_pEffect->SetMatrix( "g_matWVP", &pmaterial->g_matWVP );
	m_pEffect->SetVector( "g_vFOG", &pmaterial->g_vFOG );
	m_pEffect->SetVector( "g_vCameraFrom", &pmaterial->g_vCameraFrom );
	m_pEffect->SetVector( "g_vWindowSize", &pmaterial->g_vWindowSize );
	m_pEffect->SetFloat( "g_fAlpha", (float)pmaterial->g_fAlpha );
	m_pEffect->SetFloat( "g_fVS_1_1_ColorMulti", (float)pmaterial->g_fVS_1_1_ColorMulti );
	m_pEffect->SetVector( "g_vLightMapUV_Offset", &pmaterial->g_vLightMapUV_Offset );

	if ( m_pTexture )
		m_pEffect->SetTexture( "g_BaseTexture", m_pTexture );

	if ( pmaterial->m_pLightMapDay )
		m_pEffect->SetTexture( "g_LightMapDayTex", pmaterial->m_pLightMapDay );

	HRESULT hr = m_pEffect->SetTechnique( "runtime_1" );
	if ( FAILED(hr) )	return FALSE;

	return TRUE;
}

void DxSingleTexMaterialMesh::UpdateLightMapUV( float fMultiX, float fMultiY )
{
	if ( m_pAABBOctree )
		m_pAABBOctree->UpdateLightMapUV( fMultiX, fMultiY );
}

void DxSingleTexMaterialMesh::CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh )
{
	if ( m_pAABBOctree )
		m_pAABBOctree->CopyMesh( pd3dDevice, vecMesh );
}
