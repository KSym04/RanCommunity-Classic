#include "stdafx.h"

#include <algorithm>
#include "../Common/StlFunctions.h"

#include "../Common/SerialFile.h"
#include "../DxCommon/DxRenderStates.h"
#include "../DxCommon/DxViewPort.h"
#include "./TextureSetDXT.h"
#include "../DxOctree/DxLandMan.h"

#include "./DxStaticMaterialMesh.h"
#include "DxStaticMaterialMeshThread.h"

#include "../DxCommon/TextureManager.h"
#include "../DxCommon/DxMaterialManager.h"
#include "../DxCommon/MaterialShaderParam.h"
#include "../DxCommon/DxShaderManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// -----------------------------------------------------------------------------------------------------------------------------------------
//										D	x		S	t	a	t	i	c		M	e	s	h
// -----------------------------------------------------------------------------------------------------------------------------------------
DxStaticMaterialMesh::DxStaticMaterialMesh() :
	m_emRenderType(EMRT_OCTREE),
	m_pTextureLightMapDay(NULL),
	m_pList(NULL),
	m_pMeshTree(NULL),
	m_vMin(FLT_MAX,FLT_MAX,FLT_MAX),
	m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX),
	unknown_var1(0),
	m_pmatPieceFrame(NULL)
{
}

DxStaticMaterialMesh::~DxStaticMaterialMesh()
{
	CleanUp();
}

void DxStaticMaterialMesh::CleanUp()
{
	m_vMin = D3DXVECTOR3 (FLT_MAX,FLT_MAX,FLT_MAX);
	m_vMax = D3DXVECTOR3 (-FLT_MAX,-FLT_MAX,-FLT_MAX);
	unknown_var1 = 0;

	m_vecMeshNORMAL.clear();
	m_vecMeshALPHA.clear();

	m_vecMeshSOFTALPHA00.clear();
	m_vecMeshSOFTALPHA01.clear();
	m_vecMeshSOFTALPHA02.clear();

	m_vecMeshBLEND00.clear();
	m_vecMeshBLEND01.clear();
	m_vecMeshBLEND02.clear();

	m_vecMeshBLEND10.clear();
	m_vecMeshBLEND11.clear();
	m_vecMeshBLEND12.clear();

	m_vecMeshBLEND20.clear();
	m_vecMeshBLEND21.clear();
	m_vecMeshBLEND22.clear();

	m_vecMeshSPECULAR_BLEND.clear();

	m_vecMeshMATERIAL_EFFECT.clear();

	m_pList = NULL;
	SAFE_DELETE ( m_pMeshTree );

	std::for_each ( m_mapMesh.begin(), m_mapMesh.end(), std_afunc::DeleteMapObject() );
	m_mapMesh.clear();

	if ( m_strLightMapDay.size() )
		TextureManager::ReleaseTexture( m_strLightMapDay.c_str(), m_pTextureLightMapDay );

	
	m_strLightMapDay = "";
	m_strLightMapDS1 = "";
	m_strLightMapDS2 = "";
	m_strLightMapShadow = "";

	m_pmatPieceFrame = NULL;
}

void DxStaticMaterialMesh::FrameMove( const float fTime, const float fElapsedTime )
{
	DxSingleTexMaterialMesh* pCur = m_pList;
	while( pCur )
	{
		pCur->FrameMoveAABB( fTime, fElapsedTime );
		pCur = pCur->m_pNext;
	}

	// Note : 조각 파일 일 경우는 계산하면 안된다.
	if( m_emRenderType==EMRT_PIECE )	return;

	// Note : 컬러 계산을 다하여서 리스트가 비어있다면 다시 삽입한다.
	if( DxStaticMaterialMeshColor_THREAD::GetInstance().IsInsertListData() )
	{
		DxStaticMaterialMeshColor_THREAD::GetInstance().EnterCS();
		{
			LOADINGDATALIST& sListColorData = DxStaticMaterialMeshColor_THREAD::GetInstance().m_listColorData;

			DxSingleTexMaterialMesh* pSTMesh = m_pList;
			while( pSTMesh )
			{
				pSTMesh->InsertColorList( sListColorData );
				pSTMesh = pSTMesh->m_pNext;
			}

		}
		DxStaticMaterialMeshColor_THREAD::GetInstance().LeaveCS();
	}

}


void DxStaticMaterialMesh::Render_THREAD_Normal( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	material.m_pLightMapDay = m_pTextureLightMapDay;
	material.m_emRender = EMMATERIAL_RENDER_TYPE_NORMAL;

	for ( unsigned int i=0; i<m_vecMeshNORMAL.size(); ++i )
	{
		m_vecMeshNORMAL[i]->Render_THREAD( pd3dDevice, sCV, material );
	}
}

void DxStaticMaterialMesh::Render_THREAD_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	material.m_pLightMapDay = m_pTextureLightMapDay;
	material.m_emRender = EMMATERIAL_RENDER_TYPE_ALPHA;

	DxRenderStates::GetInstance().SetOnAlphaMap ( pd3dDevice );

	for ( unsigned int i=0; i<m_vecMeshALPHA.size(); ++i )
	{
		m_vecMeshALPHA[i]->Render_THREAD( pd3dDevice, sCV, material );
	}

	DxRenderStates::GetInstance().ReSetOnAlphaMap ( pd3dDevice );
}

void DxStaticMaterialMesh::Render_THREAD_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	material.m_pLightMapDay = m_pTextureLightMapDay;
	material.m_emRender = EMMATERIAL_RENDER_TYPE_SOFTALPHA;

	DxRenderStates::GetInstance().SetOnSoftAlphaMap ( pd3dDevice );

	pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	for ( unsigned int i=0; i<m_vecMeshSOFTALPHA00.size(); ++i )
	{
		m_vecMeshSOFTALPHA00[i]->Render_THREAD( pd3dDevice, sCV, material );
	}

	for ( unsigned int i=0; i<m_vecMeshSOFTALPHA01.size(); ++i )
	{
		m_vecMeshSOFTALPHA01[i]->Render_THREAD( pd3dDevice, sCV, material );
	}

	for ( unsigned int i=0; i<m_vecMeshSOFTALPHA02.size(); ++i )
	{
		m_vecMeshSOFTALPHA02[i]->Render_THREAD( pd3dDevice, sCV, material );
	}

	float fDBias = 0.0f;
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );

	DxRenderStates::GetInstance().ReSetOnSoftAlphaMap ( pd3dDevice );
}

void DxStaticMaterialMesh::Render_THREAD_SpecularBlending( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	material.m_pLightMapDay = m_pTextureLightMapDay;
	material.m_emRender = EMMATERIAL_RENDER_TYPE_SPECULAR_BLEND;

	DxRenderStates::GetInstance().SetOnSoftAlphaMap ( pd3dDevice );

	float fDBias = -0.000001f;
	float fSBias = -0.099999994f;
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
	pd3dDevice->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*((DWORD*)&fSBias) );
	pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, FALSE );

	DWORD dwState0;
	pd3dDevice->GetRenderState ( D3DRS_FOGCOLOR, &dwState0 );
	pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_ONE );
	pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ONE );
	pd3dDevice->SetRenderState ( D3DRS_FOGCOLOR, 0x00000000 );

	for ( unsigned int i=0; i<m_vecMeshSPECULAR_BLEND.size(); ++i )
	{
		m_vecMeshSPECULAR_BLEND[i]->Render_THREAD( pd3dDevice, sCV, material );
	}

	fDBias = 0.0f;
	fSBias = 0.0f;
	pd3dDevice->SetRenderState ( D3DRS_FOGCOLOR, dwState0 );
	pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
	pd3dDevice->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*((DWORD*)&fSBias) );

	DxRenderStates::GetInstance().ReSetOnSoftAlphaMap ( pd3dDevice );
}

void DxStaticMaterialMesh::Render_THREAD_MaterialEffect( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	material.m_pLightMapDay = m_pTextureLightMapDay;
	material.m_emRender = EMMATERIAL_RENDER_TYPE_MATERIAL_EFFECT;

	for ( unsigned int i=0; i<m_vecMeshMATERIAL_EFFECT.size(); ++i )
	{
		m_vecMeshMATERIAL_EFFECT[i]->Render_THREAD( pd3dDevice, sCV, material );
	}

	float fDBias = 0.0f;
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
}

void DxStaticMaterialMesh::Render_THREAD_Blending( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	material.m_pLightMapDay = m_pTextureLightMapDay;
	material.m_emRender = EMMATERIAL_RENDER_TYPE_BLEND;

	DxRenderStates::GetInstance().SetOnSoftAlphaMap ( pd3dDevice );

	float fDBias = -0.000001f;
	float fSBias = -0.099999994f;
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
	pd3dDevice->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*((DWORD*)&fSBias) );
	pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, FALSE );

	DWORD dwState0;
	pd3dDevice->GetRenderState ( D3DRS_ALPHATESTENABLE, &dwState0 );

	if ( m_vecMeshBLEND00.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );

		for ( unsigned int i=0; i<m_vecMeshBLEND00.size(); ++i )
		{
			m_vecMeshBLEND00[i]->Render_THREAD( pd3dDevice, sCV, material );
		}

		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, dwState0 );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	}

	if ( m_vecMeshBLEND10.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		for ( unsigned int i=0; i<m_vecMeshBLEND10.size(); ++i )
		{
			m_vecMeshBLEND10[i]->Render_THREAD( pd3dDevice, sCV, material );
		}
	}

	if ( m_vecMeshBLEND20.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ZERO );

		for ( unsigned int i=0; i<m_vecMeshBLEND20.size(); ++i )
		{
			m_vecMeshBLEND20[i]->Render_THREAD( pd3dDevice, sCV, material );
		}
	}


	if ( m_vecMeshBLEND01.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );

		for ( unsigned int i=0; i<m_vecMeshBLEND01.size(); ++i )
		{
			m_vecMeshBLEND01[i]->Render_THREAD( pd3dDevice, sCV, material );
		}

		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, dwState0 );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	}

	if ( m_vecMeshBLEND11.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		for ( unsigned int i=0; i<m_vecMeshBLEND11.size(); ++i )
		{
			m_vecMeshBLEND11[i]->Render_THREAD( pd3dDevice, sCV, material );
		}
	}

	if ( m_vecMeshBLEND21.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ZERO );

		for ( unsigned int i=0; i<m_vecMeshBLEND21.size(); ++i )
		{
			m_vecMeshBLEND21[i]->Render_THREAD( pd3dDevice, sCV, material );
		}
	}


	if ( m_vecMeshBLEND02.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );

		for ( unsigned int i=0; i<m_vecMeshBLEND02.size(); ++i )
		{
			m_vecMeshBLEND02[i]->Render_THREAD( pd3dDevice, sCV, material );
		}

		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, dwState0 );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	}

	if ( m_vecMeshBLEND12.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		for ( unsigned int i=0; i<m_vecMeshBLEND12.size(); ++i )
		{
			m_vecMeshBLEND12[i]->Render_THREAD( pd3dDevice, sCV, material );
		}
	}

	if ( m_vecMeshBLEND22.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ZERO );

		for ( unsigned int i=0; i<m_vecMeshBLEND22.size(); ++i )
		{
			m_vecMeshBLEND22[i]->Render_THREAD( pd3dDevice, sCV, material );
		}
	}
	
	fDBias = 0.0f;
	fSBias = 0.0f;
	pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
	pd3dDevice->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*((DWORD*)&fSBias) );

	DxRenderStates::GetInstance().ReSetOnSoftAlphaMap ( pd3dDevice );
}

// Piece File Render
void DxStaticMaterialMesh::Render_PIECE_Normal( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
 	if ( m_pmatPieceFrame )
 	{
		D3DXMATRIX matLocal = *m_pmatPieceFrame;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matLocal );
		material.UpdateMatrix( pd3dDevice, matLocal );
 	}
 
 	material.m_emRender = EMMATERIAL_RENDER_TYPE_NORMAL;
 
	for ( unsigned int i=0; i<m_vecMeshNORMAL.size(); ++i )
	{
		m_vecMeshNORMAL[i]->Render_PIECE( pd3dDevice, material );
	}
}

void DxStaticMaterialMesh::Render_PIECE_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	if ( m_pmatPieceFrame )
	{
		D3DXMATRIX matLocal = *m_pmatPieceFrame;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matLocal );
		material.UpdateMatrix( pd3dDevice, matLocal );
	}
 
 	material.m_emRender = EMMATERIAL_RENDER_TYPE_ALPHA;
 
	DxRenderStates::GetInstance().SetOnAlphaMap ( pd3dDevice );

	for ( unsigned int i=0; i<m_vecMeshALPHA.size(); ++i )
	{
		m_vecMeshALPHA[i]->Render_PIECE( pd3dDevice, material );
	}

	DxRenderStates::GetInstance().ReSetOnAlphaMap ( pd3dDevice );
}

void DxStaticMaterialMesh::Render_PIECE_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	if ( m_pmatPieceFrame )
	{
		D3DXMATRIX matLocal = *m_pmatPieceFrame;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matLocal );
		material.UpdateMatrix( pd3dDevice, matLocal );
	}
 
 	material.m_emRender = EMMATERIAL_RENDER_TYPE_SOFTALPHA;
 
	DxRenderStates::GetInstance().SetOnSoftAlphaMap ( pd3dDevice );

	pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	for ( unsigned int i=0; i<m_vecMeshSOFTALPHA00.size(); ++i )
	{
		m_vecMeshSOFTALPHA00[i]->Render_PIECE( pd3dDevice, material );
	}
 
	for ( unsigned int i=0; i<m_vecMeshSOFTALPHA01.size(); ++i )
	{
		m_vecMeshSOFTALPHA01[i]->Render_PIECE( pd3dDevice, material );
	}

	for ( unsigned int i=0; i<m_vecMeshSOFTALPHA02.size(); ++i )
	{
		m_vecMeshSOFTALPHA02[i]->Render_PIECE( pd3dDevice, material );
	}

	float fDBias = 0.0f;
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );

	DxRenderStates::GetInstance().ReSetOnSoftAlphaMap ( pd3dDevice );
}

void DxStaticMaterialMesh::Render_PIECE_Blending( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	if ( m_pmatPieceFrame )
	{
		D3DXMATRIX matLocal = *m_pmatPieceFrame;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matLocal );
		material.UpdateMatrix( pd3dDevice, matLocal );
	}

 	material.m_emRender = EMMATERIAL_RENDER_TYPE_BLEND;

	DxRenderStates::GetInstance().SetOnSoftAlphaMap ( pd3dDevice );

	float fDBias = -0.000001f;
	float fSBias = -0.099999994f;
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
	pd3dDevice->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*((DWORD*)&fSBias) );
	pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, FALSE );

	DWORD dwState0;
	pd3dDevice->GetRenderState ( D3DRS_ALPHATESTENABLE, &dwState0 );

	if ( m_vecMeshBLEND00.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );

		for ( unsigned int i=0; i<m_vecMeshBLEND00.size(); ++i )
		{
			m_vecMeshBLEND00[i]->Render_PIECE( pd3dDevice, material );
		}

		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, dwState0 );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	}

	if ( m_vecMeshBLEND10.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		for ( unsigned int i=0; i<m_vecMeshBLEND10.size(); ++i )
		{
			m_vecMeshBLEND10[i]->Render_PIECE( pd3dDevice, material );
		}
	}

	if ( m_vecMeshBLEND20.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ZERO );

		for ( unsigned int i=0; i<m_vecMeshBLEND20.size(); ++i )
		{
			m_vecMeshBLEND20[i]->Render_PIECE( pd3dDevice, material );
		}
	}


	if ( m_vecMeshBLEND01.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );

		for ( unsigned int i=0; i<m_vecMeshBLEND01.size(); ++i )
		{
			m_vecMeshBLEND01[i]->Render_PIECE( pd3dDevice, material );
		}

		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, dwState0 );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	}

	if ( m_vecMeshBLEND11.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		for ( unsigned int i=0; i<m_vecMeshBLEND11.size(); ++i )
		{
			m_vecMeshBLEND11[i]->Render_PIECE( pd3dDevice, material );
		}
	}

	if ( m_vecMeshBLEND21.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ZERO );

		for ( unsigned int i=0; i<m_vecMeshBLEND21.size(); ++i )
		{
			m_vecMeshBLEND21[i]->Render_PIECE( pd3dDevice, material );
		}
	}


	if ( m_vecMeshBLEND02.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );

		for ( unsigned int i=0; i<m_vecMeshBLEND02.size(); ++i )
		{
			m_vecMeshBLEND02[i]->Render_PIECE( pd3dDevice, material );
		}

		pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, dwState0 );
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	}

	if ( m_vecMeshBLEND12.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		for ( unsigned int i=0; i<m_vecMeshBLEND12.size(); ++i )
		{
			m_vecMeshBLEND12[i]->Render_PIECE( pd3dDevice, material );
		}
	}

	if ( m_vecMeshBLEND22.size() )
	{
		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ZERO );

		for ( unsigned int i=0; i<m_vecMeshBLEND22.size(); ++i )
		{
			m_vecMeshBLEND22[i]->Render_PIECE( pd3dDevice, material );
		}
	}

	fDBias = 0.0f;
	fSBias = 0.0f;
	pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
	pd3dDevice->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*((DWORD*)&fSBias) );

	DxRenderStates::GetInstance().ReSetOnSoftAlphaMap ( pd3dDevice );
}

void DxStaticMaterialMesh::Render_PIECE_SpecularBlending( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	if ( m_pmatPieceFrame )
	{
		D3DXMATRIX matLocal = *m_pmatPieceFrame;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matLocal );
		material.UpdateMatrix( pd3dDevice, matLocal );
	}
 
 	material.m_emRender = EMMATERIAL_RENDER_TYPE_SPECULAR_BLEND;
 
	DxRenderStates::GetInstance().SetOnSoftAlphaMap ( pd3dDevice );

	float fDBias = -0.000001f;
	float fSBias = -0.099999994f;
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
	pd3dDevice->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*((DWORD*)&fSBias) );
	pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, FALSE );

	DWORD dwState0;
	pd3dDevice->GetRenderState ( D3DRS_FOGCOLOR, &dwState0 );
	pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_ONE );
	pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ONE );
	pd3dDevice->SetRenderState ( D3DRS_FOGCOLOR, 0x00000000 );

	for ( unsigned int i=0; i<m_vecMeshSPECULAR_BLEND.size(); ++i )
	{
		m_vecMeshSPECULAR_BLEND[i]->Render_PIECE( pd3dDevice, material );
	}

	fDBias = 0.0f;
	fSBias = 0.0f;
	pd3dDevice->SetRenderState ( D3DRS_FOGCOLOR, dwState0 );
	pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
	pd3dDevice->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*((DWORD*)&fSBias) );

	DxRenderStates::GetInstance().ReSetOnSoftAlphaMap ( pd3dDevice );
}

void DxStaticMaterialMesh::Render_PIECE_MaterialEffect( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	if ( m_pmatPieceFrame )
	{
		D3DXMATRIX matLocal = *m_pmatPieceFrame;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matLocal );
		material.UpdateMatrix( pd3dDevice, matLocal );
	}

	material.m_emRender = EMMATERIAL_RENDER_TYPE_MATERIAL_EFFECT;
 
	for ( unsigned int i=0; i<m_vecMeshMATERIAL_EFFECT.size(); ++i )
	{
		m_vecMeshMATERIAL_EFFECT[i]->Render_PIECE( pd3dDevice, material );
	}

	float fDBias = 0.0f;
	pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fDBias) );
}

BOOL DxStaticMaterialMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, LPCSTR& szName, const BOOL bFrontColl )
{
	BOOL		bUse		= FALSE;

	MAPSINGLETEXMATERIALMESH_ITER iter = m_mapMesh.begin();
	for( ; iter!=m_mapMesh.end(); ++iter )
	{
		if( (*iter).second->IsCollisionLine( vStart, vEnd, vColl, vNor, bFrontColl ) )	bUse = TRUE;
	}

	return bUse;
}

BOOL DxStaticMaterialMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl )
{
	BOOL		bUse		= FALSE;

	MAPSINGLETEXMATERIALMESH_ITER iter = m_mapMesh.begin();
	for( ; iter!=m_mapMesh.end(); ++iter )
	{
		if( (*iter).second->IsCollisionLine( vStart, vEnd, vColl, vNor, bFrontColl ) )	bUse = TRUE;
	}

	return bUse;
}

void DxStaticMaterialMesh::SetAABBMaxMin ()
{
	m_vMax = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	m_vMin = D3DXVECTOR3( FLT_MAX, FLT_MAX, FLT_MAX );
	D3DXVECTOR3	vMax( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	D3DXVECTOR3	vMin( FLT_MAX, FLT_MAX, FLT_MAX );

	MAPSINGLETEXMATERIALMESH_ITER iter;
	for( iter=m_mapMesh.begin(); iter!=m_mapMesh.end(); ++iter )
	{
		(*iter).second->GetAABBSize( vMax, vMin );

		if ( m_vMax.x < vMax.x )	m_vMax.x = vMax.x;
		if ( m_vMax.y < vMax.y )	m_vMax.y = vMax.y;
		if ( m_vMax.z < vMax.z )	m_vMax.z = vMax.z;

		if ( m_vMin.x > vMin.x )	m_vMin.x = vMin.x;
		if ( m_vMin.y > vMin.y )	m_vMin.y = vMin.y;
		if ( m_vMin.z > vMin.z )	m_vMin.z = vMin.z;
	}
}

DxSingleTexMaterialMesh* DxStaticMaterialMesh::FindMesh( std::string _strMeshName )
{
	MAPSINGLETEXMATERIALMESH_ITER iter = m_mapMesh.find( _strMeshName.c_str() );
	if ( iter != m_mapMesh.end() )
	{
		return (*iter).second;
	}

	return NULL;
}

void DxStaticMaterialMesh::LoadLightMaps( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( m_strLightMapDay.size() )
		TextureManager::LoadTexture( m_strLightMapDay.c_str(), pd3dDevice, m_pTextureLightMapDay, 0L, 0L, FALSE );
}

void DxStaticMaterialMesh::SortMaterialContainer( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bPiece )
{
	//cleanup all mesh
	m_vecMeshNORMAL.clear();
	m_vecMeshALPHA.clear();

	m_vecMeshSOFTALPHA00.clear();
	m_vecMeshSOFTALPHA01.clear();
	m_vecMeshSOFTALPHA02.clear();

	m_vecMeshBLEND00.clear();
	m_vecMeshBLEND01.clear();
	m_vecMeshBLEND02.clear();

	m_vecMeshBLEND10.clear();
	m_vecMeshBLEND11.clear();
	m_vecMeshBLEND12.clear();

	m_vecMeshBLEND20.clear();
	m_vecMeshBLEND21.clear();
	m_vecMeshBLEND22.clear();

	m_vecMeshSPECULAR_BLEND.clear();

	m_vecMeshMATERIAL_EFFECT.clear();


	MAPSINGLETEXMATERIALMESH_ITER iter;
	for( iter=m_mapMesh.begin(); iter!=m_mapMesh.end(); ++iter )
	{
		std::string strname = (*iter).first;
		DxSingleTexMaterialMesh* pmesh = (*iter).second;

		if ( !pmesh )	continue;

		DWORD dwFlag = pmesh->GetFlag();
		if ( dwFlag & 0x38 )
		{
			if ( bPiece )
				pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceBlend.fx", pd3dDevice );
			else
				pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandBlend.fx", pd3dDevice );

			int nVal1 = dwFlag & 0xF00;
			int nVal2, nVal3;
			if ( nVal1 == 512 )
				nVal2 = 1;
			else if ( nVal1 == 768 )
				nVal2 = 2;
			else
				nVal2 = 0;

			if ( dwFlag & 8 )
				nVal3 = nVal2 + 8;
			else if ( dwFlag & 0x10 )
				nVal3 = nVal2 + 5;
			else if ( dwFlag & 0x20 )
				nVal3 = nVal2 + 11;
			else 
				continue;

			int nIndex = 32 * nVal3;

			switch( nIndex )
			{
			case 160:	m_vecMeshBLEND00.push_back( pmesh );		break;
			case 192:	m_vecMeshBLEND01.push_back( pmesh );		break;
			case 224:	m_vecMeshBLEND02.push_back( pmesh );		break;
			case 256:	m_vecMeshBLEND10.push_back( pmesh );		break;
			case 288:	m_vecMeshBLEND11.push_back( pmesh );		break;
			case 320:	m_vecMeshBLEND12.push_back( pmesh );		break;
			case 352:	m_vecMeshBLEND20.push_back( pmesh );		break;
			case 384:	m_vecMeshBLEND21.push_back( pmesh );		break;
			case 416:	m_vecMeshBLEND22.push_back( pmesh );		break;
			};

			continue;
		}

		if ( dwFlag & 0x40 )
		{
			if ( bPiece )
				pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSpecularBlend.fx", pd3dDevice );
			else
				pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSpecularBlend.fx", pd3dDevice );

			m_vecMeshSPECULAR_BLEND.push_back( pmesh );
			continue;
		}

		if ( pmesh->m_pMaterial )
		{
			m_vecMeshMATERIAL_EFFECT.push_back( pmesh );
			continue;
		}

		TextureManager::EM_TEXTYPE emtype = TextureManager::GetTexType( pmesh->GetTextureName().c_str() );

		if ( bPiece )
		{
			switch( emtype )
			{
			default:
			case TextureManager::EMTT_NORMAL:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceNormalLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceNormalNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceNormalNoLightmap.fx", pd3dDevice );

					m_vecMeshNORMAL.push_back( pmesh );
				}break;

			case TextureManager::EMTT_ALPHA_HARD:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceAlphaLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceAlphaNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceAlphaNoLightmap.fx", pd3dDevice );

					m_vecMeshALPHA.push_back( pmesh );
				}break;

			case TextureManager::EMTT_ALPHA_SOFT:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaNoLightmap.fx", pd3dDevice );

					m_vecMeshSOFTALPHA00.push_back( pmesh );
				}break;

			case TextureManager::EMTT_ALPHA_SOFT01:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaNoLightmap.fx", pd3dDevice );

					m_vecMeshSOFTALPHA01.push_back( pmesh );
				}break;

			case TextureManager::EMTT_ALPHA_SOFT02:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "PieceSoftAlphaNoLightmap.fx", pd3dDevice );

					m_vecMeshSOFTALPHA02.push_back( pmesh );
				}break;

			}
		}
		else
		{
			switch( emtype )
			{
			default:
			case TextureManager::EMTT_NORMAL:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandNormalLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandNormalNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandNormalNoLightmap.fx", pd3dDevice );

					m_vecMeshNORMAL.push_back( pmesh );
				}break;

			case TextureManager::EMTT_ALPHA_HARD:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandAlphaLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandAlphaNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandAlphaNoLightmap.fx", pd3dDevice );

					m_vecMeshALPHA.push_back( pmesh );
				}break;

			case TextureManager::EMTT_ALPHA_SOFT:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaNoLightmap.fx", pd3dDevice );

					m_vecMeshSOFTALPHA00.push_back( pmesh );
				}break;

			case TextureManager::EMTT_ALPHA_SOFT01:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaNoLightmap.fx", pd3dDevice );

					m_vecMeshSOFTALPHA01.push_back( pmesh );
				}break;

			case TextureManager::EMTT_ALPHA_SOFT02:
				{
					pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x1000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaNoLightmap.fx", pd3dDevice );

					if ( dwFlag & 0x2000 )
						pmesh->m_pEffect = DxShaderManager::GetInstance().LoadFile( "LandSoftAlphaNoLightmap.fx", pd3dDevice );

					m_vecMeshSOFTALPHA02.push_back( pmesh );
				}break;

			}
		}
		

	}
}


void DxStaticMaterialMesh::UpdateLightMapUV( float fMultiX, float fMultiY )
{
	DxSingleTexMaterialMesh* pCur = m_pList;
	while( pCur )
	{
		pCur->UpdateLightMapUV( fMultiX, fMultiY );
		pCur = pCur->m_pNext;
	}
}

void DxStaticMaterialMesh::CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh )
{
	for ( unsigned int i=0; i<m_vecMeshNORMAL.size(); ++i )
	{
		m_vecMeshNORMAL[i]->CopyMesh( pd3dDevice, vecMesh );
	}

	for ( unsigned int i=0; i<m_vecMeshMATERIAL_EFFECT.size(); ++i )
	{
		m_vecMeshMATERIAL_EFFECT[i]->CopyMesh( pd3dDevice, vecMesh );
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------------
//										N	S		S	T	A	T	I	C		M	E	S	H
// -----------------------------------------------------------------------------------------------------------------------------------------
namespace NSSTATICMATERIALMESH
{
	void ConvertMAPtoLIST ( MAPSINGLETEXMATERIALMESH& mapData, PDXSINGLETEXMATERIALMESH& pList )
	{
		MAPSINGLETEXMATERIALMESH_ITER iter = mapData.begin();
		for ( ; iter!=mapData.end(); ++iter )
		{
			(*iter).second->m_pNext = pList;
			pList = (*iter).second;
		}
	}
};
