#include "stdafx.h"
#include "./DxSingleTexMesh.h"

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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DxSingleTexMesh::DxSingleTexMesh() :
	m_pAABBOctree(NULL),
	m_pTexture(NULL),
	m_pTextureLightMapD(NULL),
	m_pTextureLightMapN(NULL),
	m_bCheckTex(FALSE),
	m_bCheckTexLightMapD(FALSE),
	m_bCheckTexLightMapN(FALSE),
	m_pTexEffList(NULL),
	m_dwFlag(0L),
	m_pNext(NULL),		// m_pNext 를 파괴 할 수는 없다.... Octree 만 파괴하라.
	unknown_var1(FALSE),
	unknown_var2(FALSE),
	unknown_var3(0),
	unknown_var4(""),
	unknown_var5(FALSE)
{
	m_szTexName = "";
	m_szTexNameLightMapD = "";
	m_szTexNameLightMapN = "";
};

DxSingleTexMesh::~DxSingleTexMesh()
{
	CleanUp();

	m_pNext = NULL;
}

void DxSingleTexMesh::CleanUp()
{
	m_bCheckTex = FALSE;
	SAFE_DELETE ( m_pAABBOctree );

	SAFE_DELETE( m_pTexEffList );

	TextureManager::ReleaseTexture( m_szTexName.c_str(), m_pTexture );
	m_szTexName = "";

	if ( m_szTexNameLightMapD.size() )
		TextureManager::ReleaseTexture( m_szTexNameLightMapD.c_str(), m_pTextureLightMapD );

	if ( m_szTexNameLightMapN.size() )
		TextureManager::ReleaseTexture( m_szTexNameLightMapN.c_str(), m_pTextureLightMapN );

	m_bCheckTexLightMapD = FALSE;
	m_bCheckTexLightMapN = FALSE;
}

void DxSingleTexMesh::Create ( const LPDIRECT3DDEVICEQ pd3dDevice, const char* szName, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, BOOL bPiece )
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
	m_pAABBOctree = new DxAABBOctreeOrig;
	m_pAABBOctree->Create( pd3dDevice, dwFaceNUM, pVertices, dwFVF, emLandType );

	m_szTexName = szName;
}

void DxSingleTexMesh::ConvertTexEffMesh( const LPDIRECT3DDEVICEQ pd3dDevice, DxTextureEffMan* pTexEff )
{
	// Note : 삭제
	SAFE_DELETE( m_pTexEffList );
	m_dwFlag = 0L;

	// Note : 
	DxTextureEffMan::MAPTEXEFF* pmapTexEff = pTexEff->GetMapTexEff();
	DxTextureEffMan::MAPTEXEFF_ITER iter = pmapTexEff->begin();
	for( ; iter!=pmapTexEff->end(); ++iter )
	{
		if( (*iter).first.c_str()!=m_szTexName )	continue;

		PBYTE pProp;
		DxTexEffBase* pNew;
		DxTexEffBase* pTexEff = (*iter).second;
		while( pTexEff )
		{
			DWORD dwTypeID = pTexEff->GetTypeID();
			switch( dwTypeID )
			{
			case DEF_TEXEFF_DIFFUSE:
				pProp = (PBYTE)((DxTexEffDiffuse*)pTexEff)->GetProperty();
				pNew = new DxTexEffDiffuse;
				pNew->SetProperty( pd3dDevice, pProp );

				pNew->m_pNext = m_pTexEffList;
				m_pTexEffList = pNew;

				m_dwFlag |= dwTypeID;
				break;
			case DEF_TEXEFF_FLOWUV:
				pProp = (PBYTE)((DxTexEffFlowUV*)pTexEff)->GetProperty();
				pNew = new DxTexEffFlowUV;
				pNew->SetProperty( pd3dDevice, pProp );

				pNew->m_pNext = m_pTexEffList;
				m_pTexEffList = pNew;

				m_dwFlag |= dwTypeID;
				break;
			case DEF_TEXEFF_ROTATE:
				pProp = (PBYTE)((DxTexEffRotate*)pTexEff)->GetProperty();
				pNew = new DxTexEffRotate;
				pNew->SetProperty( pd3dDevice, pProp );

				pNew->m_pNext = m_pTexEffList;
				m_pTexEffList = pNew;

				m_dwFlag |= dwTypeID;
				break;
			case DEF_TEXEFF_SPECULAR:
				pProp = (PBYTE)((DxTexEffSpecular*)pTexEff)->GetProperty();
				pNew = new DxTexEffSpecular;
				pNew->SetProperty( pd3dDevice, pProp );

				pNew->m_pNext = m_pTexEffList;
				m_pTexEffList = pNew;

				m_dwFlag |= dwTypeID;
				break;

			case DEF_TEXEFF_VISUALMATERIAL:
				pProp = (PBYTE)((DxTexEffVisualMaterial*)pTexEff)->GetProperty();
				pNew = new DxTexEffVisualMaterial;
				pNew->SetProperty( pd3dDevice, pProp );

				pNew->m_pNext = m_pTexEffList;
				m_pTexEffList = pNew;

				m_dwFlag |= dwTypeID;
				break;
			};

			pTexEff = pTexEff->m_pNext;
		}
	}
}

void DxSingleTexMesh::FrameMoveAABB( const float fTime, const float fElapsedTime )
{
	DxTexEffBase* pCur = m_pTexEffList;
	while( pCur )
	{
		pCur->FrameMove( fElapsedTime );	// 예전엔 DEF_TEXEFF_DIFFUSE	< switch 써서 씀 >
		pCur = pCur->m_pNext;
	}
}

HRESULT DxSingleTexMesh::RenderTHREAD ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading )
{
	if ( !m_pAABBOctree )	return S_OK;

	// Note : Texture Loading
	if( !m_pTexture )
	{
		if( !m_bCheckTex )
		{
			if ( m_szTexName.size() )
			{
				TextureManager::LoadTexture( m_szTexName.c_str(), pd3dDevice, m_pTexture, 0L, 0L, TRUE );
			}
			m_bCheckTex = TRUE;
		}

		TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );
	}

	// Note : Render
	pd3dDevice->SetTexture( 0, m_pTexture );

	//lightmap test
	LoadLightMaps( pd3dDevice );

	LPDIRECT3DTEXTUREQ plightmap = NULL;
	if ( GLPeriod::GetInstance().IsOffLight() )
		plightmap = m_pTextureLightMapD;
	else 
		plightmap = m_pTextureLightMapN;

	if ( plightmap )
	{
		pd3dDevice->SetTexture( 1, plightmap );

		DxRenderStates::GetInstance().SetLightMapSB( pd3dDevice );
		m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV, listLoadingData, CSLockLoading );
		DxRenderStates::GetInstance().ReSetLightMapSB( pd3dDevice );
	}
	else
	{
		m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV, listLoadingData, CSLockLoading );
	}

	// Note : Texture Eff
	DxTexEffBase* pCur = m_pTexEffList;
	while( pCur )
	{
		switch( pCur->GetTypeID() )
		{
		case DEF_TEXEFF_DIFFUSE:
			((DxTexEffDiffuse*)pCur)->SetRenderBegin( pd3dDevice );
			m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV );
			((DxTexEffDiffuse*)pCur)->SetRenderEnd( pd3dDevice );
			break;
		case DEF_TEXEFF_FLOWUV:
			((DxTexEffFlowUV*)pCur)->SetRenderBegin( pd3dDevice );
			m_pAABBOctree->RenderTHREAD_FlowUV( pd3dDevice, sCV, (DxTexEffFlowUV*)pCur );
			((DxTexEffFlowUV*)pCur)->SetRenderEnd( pd3dDevice );
			break;
		case DEF_TEXEFF_ROTATE:
			((DxTexEffRotate*)pCur)->SetRenderBegin( pd3dDevice );
			m_pAABBOctree->RenderTHREAD_Rotate( pd3dDevice, sCV, (DxTexEffRotate*)pCur );
			((DxTexEffRotate*)pCur)->SetRenderEnd( pd3dDevice );
			break;
		case DEF_TEXEFF_SPECULAR:
			((DxTexEffSpecular*)pCur)->SetRenderBegin( pd3dDevice );
			m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV );
			((DxTexEffSpecular*)pCur)->SetRenderEnd( pd3dDevice );
			break;

		case DEF_TEXEFF_VISUALMATERIAL:
			{
				DxTexEffVisualMaterial* pEffVM = (DxTexEffVisualMaterial*)pCur;
				if ( pEffVM )
				{
					if( pEffVM->IsUseSpec() )
					{
						pEffVM->SetRenderBeginSpec( pd3dDevice );
						m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV );
						pEffVM->SetRenderEndSpec( pd3dDevice );
					}

					if( pEffVM->IsUseGlow() )
					{
						pEffVM->SetRenderBeginGlow( pd3dDevice );
						m_pAABBOctree->RenderTHREAD( pd3dDevice, sCV );
						pEffVM->SetRenderEndGlow( pd3dDevice );
					}

					if(pEffVM->IsUseFlow() )
					{
						pEffVM->SetRenderBeginFlow( pd3dDevice );
						m_pAABBOctree->RenderTHREAD_FlowUV( pd3dDevice, sCV, pEffVM );
						pEffVM->SetRenderEndFlow( pd3dDevice );
					}

					if(pEffVM->IsUseRotate())
					{
						pEffVM->SetRenderBeginRotate( pd3dDevice );
						m_pAABBOctree->RenderTHREAD_Rotate( pd3dDevice, sCV, pEffVM);
						pEffVM->SetRenderEndRotate( pd3dDevice );
					}
				}
			}break;

		};

		pCur = pCur->m_pNext;
	}

	return S_OK;
}

HRESULT DxSingleTexMesh::RenderDYNAMIC ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, CSerialFile* const SFile )
{
	if ( !m_pAABBOctree )	return S_OK;

	// Note : Texture Loading
	if( !m_pTexture )
	{
		if( !m_bCheckTex )
		{
			if ( m_szTexName.size() )
			{
				TextureManager::LoadTexture( m_szTexName.c_str(), pd3dDevice, m_pTexture, 0L, 0L, TRUE );
			}
			m_bCheckTex = TRUE;
		}

		TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );
	}

	pd3dDevice->SetTexture( 0, m_pTexture );
	
	//lightmap test
	LoadLightMaps( pd3dDevice );

	LPDIRECT3DTEXTUREQ plightmap = NULL;
	if ( GLPeriod::GetInstance().IsOffLight() )
		plightmap = m_pTextureLightMapD;
	else 
		plightmap = m_pTextureLightMapN;

	if ( plightmap )
	{
		pd3dDevice->SetTexture( 1, plightmap );

		DxRenderStates::GetInstance().SetLightMapSB( pd3dDevice );
		m_pAABBOctree->RenderDYNAMIC( pd3dDevice, sCV, SFile );
		DxRenderStates::GetInstance().ReSetLightMapSB( pd3dDevice );
	}
	else
	{
		m_pAABBOctree->RenderDYNAMIC( pd3dDevice, sCV, SFile );
	}

	return S_OK;
}

HRESULT DxSingleTexMesh::FirstLoad( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin, CSerialFile* const SFile )
{
	if ( !m_pAABBOctree )	return S_OK;

	// Note : Texture Loading
	if( !m_pTexture )
	{
		if( !m_bCheckTex )
		{
			if ( m_szTexName.size() )
			{
				TextureManager::LoadTexture( m_szTexName.c_str(), pd3dDevice, m_pTexture, 0L, 0L );
			}
			m_bCheckTex = TRUE;
		}
	}

	pd3dDevice->SetTexture( 0, m_pTexture );
	m_pAABBOctree->FirstLoad( pd3dDevice, vMax, vMin, SFile );

	return S_OK;
}

HRESULT DxSingleTexMesh::Render( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV )
{
	if ( !m_pAABBOctree )	return S_OK;

	//PROFILE_BEGIN("DxSingleTexMesh::Render_2");
	//{
	//	// Note : Texture Loading
	//	if( !m_pTexture )
	//	{
	//		if( !m_bCheckTex )
	//		{
	//			if ( m_szTexName.size() )
	//			{
	//				TextureManager::LoadTexture( m_szTexName.c_str(), pd3dDevice, m_pTexture, 0L, 0L, TRUE );
	//			}
	//			m_bCheckTex = TRUE;
	//		}

	//		TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );
	//	}

	//	m_pAABBOctree->Render( pd3dDevice, sCV );
	//}
	//PROFILE_END("DxSingleTexMesh::Render_2");

	return S_OK;
}

void DxSingleTexMesh::Render( const LPDIRECT3DDEVICEQ pd3dDevice, DxTextureEffMan::MAPTEXEFF* pmapTexEff )
{
	if ( !m_pAABBOctree )	return;

	// Note : Texture Loading
	if( !m_pTexture )
	{
		if( !m_bCheckTex )
		{
			if ( m_szTexName.size() )
			{
				TextureManager::LoadTexture( m_szTexName.c_str(), pd3dDevice, m_pTexture, 0L, 0L, TRUE );
			}
			m_bCheckTex = TRUE;
		}

		TextureManager::GetTexture( m_szTexName.c_str(), m_pTexture );
	}


	pd3dDevice->SetTexture( 0, m_pTexture );
	
	//lightmap test
	LoadLightMaps( pd3dDevice );

	LPDIRECT3DTEXTUREQ plightmap = NULL;
	if ( GLPeriod::GetInstance().IsOffLight() )
		plightmap = m_pTextureLightMapD;
	else 
		plightmap = m_pTextureLightMapN;

	if ( plightmap )
	{
		pd3dDevice->SetTexture( 1, plightmap );

		DxRenderStates::GetInstance().SetLightMapSB( pd3dDevice );
		m_pAABBOctree->Render( pd3dDevice );
		DxRenderStates::GetInstance().ReSetLightMapSB( pd3dDevice );
	}
	else
	{
		m_pAABBOctree->Render( pd3dDevice );
	}

	if( !pmapTexEff )					return;	// TexEff 자체가 없다면 안함.
	if( !(m_dwFlag&DEF_TEXEFF_ALL) )	return;	// 셋팅된 효과가 없다면 안함.

	// Note : Texture Eff
	DxTextureEffMan::MAPTEXEFF_ITER iter = pmapTexEff->find( m_szTexName.c_str() );
	if( iter!=pmapTexEff->end() )
	{
		DxTexEffBase* pCur = (*iter).second;
		while( pCur )
		{
			switch( pCur->GetTypeID() )
			{
			case DEF_TEXEFF_DIFFUSE:
				((DxTexEffDiffuse*)pCur)->SetRenderBegin( pd3dDevice );
				m_pAABBOctree->Render( pd3dDevice );
				((DxTexEffDiffuse*)pCur)->SetRenderEnd( pd3dDevice );
				break;
			case DEF_TEXEFF_FLOWUV:
				((DxTexEffFlowUV*)pCur)->SetRenderBegin( pd3dDevice );
				m_pAABBOctree->Render_FlowUV( pd3dDevice, (DxTexEffFlowUV*)pCur );
				((DxTexEffFlowUV*)pCur)->SetRenderEnd( pd3dDevice );
				break;
			case DEF_TEXEFF_ROTATE:
				((DxTexEffRotate*)pCur)->SetRenderBegin( pd3dDevice );
				m_pAABBOctree->Render_Rotate( pd3dDevice, (DxTexEffRotate*)pCur );
				((DxTexEffRotate*)pCur)->SetRenderEnd( pd3dDevice );
				break;
			case DEF_TEXEFF_SPECULAR:
				((DxTexEffSpecular*)pCur)->SetRenderBegin( pd3dDevice );
				m_pAABBOctree->Render( pd3dDevice );
				((DxTexEffSpecular*)pCur)->SetRenderEnd( pd3dDevice );
				break;

			case DEF_TEXEFF_VISUALMATERIAL:
				{
					DxTexEffVisualMaterial* pEffVM = (DxTexEffVisualMaterial*)pCur;
					if ( pEffVM )
					{
						if( pEffVM->IsUseSpec() )
						{
							pEffVM->SetRenderBeginSpec( pd3dDevice );
							m_pAABBOctree->Render( pd3dDevice );
							pEffVM->SetRenderEndSpec( pd3dDevice );
						}

						if( pEffVM->IsUseGlow() )
						{
							pEffVM->SetRenderBeginGlow( pd3dDevice );
							m_pAABBOctree->Render( pd3dDevice );
							pEffVM->SetRenderEndGlow( pd3dDevice );
						}

						if(pEffVM->IsUseFlow() )
						{
							pEffVM->SetRenderBeginFlow( pd3dDevice );
							m_pAABBOctree->Render_FlowUV( pd3dDevice, pEffVM );
							pEffVM->SetRenderEndFlow( pd3dDevice );
						}

						if(pEffVM->IsUseRotate())
						{
							pEffVM->SetRenderBeginRotate( pd3dDevice );
							m_pAABBOctree->Render_Rotate( pd3dDevice, pEffVM );
							pEffVM->SetRenderEndRotate( pd3dDevice );
						}					
					}
				}break;
			};

			pCur = pCur->m_pNext;
		} 
	}

	return;
}

void DxSingleTexMesh::InsertColorList( LOADINGDATALIST &listColorData )
{
	if( !m_pAABBOctree )	return;
	m_pAABBOctree->InsertColorList( listColorData );
}

BOOL DxSingleTexMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, 
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

BOOL DxSingleTexMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl )
{
	if ( !m_pAABBOctree )	return FALSE;
	if ( m_pAABBOctree->IsCollisionLine( vStart, vEnd, vColl, vNor, bFrontColl ) )	return TRUE;
	else																			return FALSE;
}


void DxSingleTexMesh::LoadLightMaps( const LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_pTextureLightMapD )
	{
		if( !m_bCheckTexLightMapD )
		{
			if ( m_szTexNameLightMapD.size() )
				TextureManager::LoadTexture( m_szTexNameLightMapD.c_str(), pd3dDevice, m_pTextureLightMapD, 0L, 0L, TRUE );

			m_bCheckTexLightMapD = TRUE;
		}

		TextureManager::GetTexture( m_szTexNameLightMapD.c_str(), m_pTextureLightMapD );
	}

	if( !m_pTextureLightMapN )
	{
		if( !m_bCheckTexLightMapN )
		{
			if ( m_szTexNameLightMapN.size() )
				TextureManager::LoadTexture( m_szTexNameLightMapN.c_str(), pd3dDevice, m_pTextureLightMapN, 0L, 0L, TRUE );

			m_bCheckTexLightMapN = TRUE;
		}

		TextureManager::GetTexture( m_szTexNameLightMapN.c_str(), m_pTextureLightMapN );
	}
}