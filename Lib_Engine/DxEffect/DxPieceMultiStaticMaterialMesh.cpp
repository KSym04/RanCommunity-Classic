#include "stdafx.h"
#include "./DxPieceMultiStaticMaterialMesh.h"
#include "../Common/StlFunctions.h"

#include "../DxCommon/MaterialShaderParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxPieceMultiStaticMaterialMesh::DxPieceMultiStaticMaterialMesh() 
	: m_vMin(FLT_MAX,FLT_MAX,FLT_MAX)
	, m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX)
{
}

DxPieceMultiStaticMaterialMesh::~DxPieceMultiStaticMaterialMesh()
{
	CleanUp();
}

void DxPieceMultiStaticMaterialMesh::CleanUp()
{
	m_vMin = D3DXVECTOR3 (FLT_MAX,FLT_MAX,FLT_MAX);
	m_vMax = D3DXVECTOR3 (-FLT_MAX,-FLT_MAX,-FLT_MAX);

	std::for_each ( m_vecStaticMesh.begin(), m_vecStaticMesh.end(), std_afunc::DeleteObject() );
	m_vecStaticMesh.clear();
}

void DxPieceMultiStaticMaterialMesh::CalculateAABB()
{
	m_vMax = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	m_vMin = D3DXVECTOR3( FLT_MAX, FLT_MAX, FLT_MAX );

	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		D3DXVECTOR3	vMax = pmesh->GetAABBMax();
		D3DXVECTOR3	vMin = pmesh->GetAABBMin();

		if ( m_vMax.x < vMax.x )	m_vMax.x = vMax.x;
		if ( m_vMax.y < vMax.y )	m_vMax.y = vMax.y;
		if ( m_vMax.z < vMax.z )	m_vMax.z = vMax.z;

		if ( m_vMin.x > vMin.x )	m_vMin.x = vMin.x;
		if ( m_vMin.y > vMin.y )	m_vMin.y = vMin.y;
		if ( m_vMin.z > vMin.z )	m_vMin.z = vMin.z;
	}
}

void DxPieceMultiStaticMaterialMesh::FrameMove( const float fTime, const float fElapsedTime )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->FrameMove( fTime, fElapsedTime );
	}
}

void DxPieceMultiStaticMaterialMesh::Render_PIECE_Normal( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;
		pmesh->Render_PIECE_Normal( pd3dDevice, material, matWorld );
	}
}

void DxPieceMultiStaticMaterialMesh::Render_PIECE_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;
		pmesh->Render_PIECE_Alpha( pd3dDevice, material, matWorld );
	}
}

void DxPieceMultiStaticMaterialMesh::Render_PIECE_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;
		pmesh->Render_PIECE_SoftAlpha( pd3dDevice, material, matWorld );
	}
}

void DxPieceMultiStaticMaterialMesh::Render_PIECE_Blending( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;
		pmesh->Render_PIECE_Blending( pd3dDevice, material, matWorld );
	}
}

void DxPieceMultiStaticMaterialMesh::Render_PIECE_SpecularBlending( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;
		pmesh->Render_PIECE_SpecularBlending( pd3dDevice, material, matWorld );
	}
}

void DxPieceMultiStaticMaterialMesh::Render_PIECE_MaterialEffect( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;
		pmesh->Render_PIECE_MaterialEffect( pd3dDevice, material, matWorld );
	}
}

BOOL DxPieceMultiStaticMaterialMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, LPCSTR& szName, const BOOL bFrontColl )
{
	BOOL bUse = FALSE;

	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		if ( pmesh->IsCollisionLine( vStart, vEnd, vColl, vNor, szName, bFrontColl ) ) 
		{
			bUse = TRUE;
			break;
		}
	}
	
	return bUse;
}

BOOL DxPieceMultiStaticMaterialMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl )
{
	BOOL bUse = FALSE;

	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		if ( pmesh->IsCollisionLine( vStart, vEnd, vColl, vNor, bFrontColl ) ) 
		{
			bUse = TRUE;
			break;
		}
	}

	return bUse;
}

DxSingleTexMaterialMesh* DxPieceMultiStaticMaterialMesh::FindMesh( std::string _strMeshName )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;
		
		DxSingleTexMaterialMesh* psingletexmesh = pmesh->FindMesh( _strMeshName );
		if ( psingletexmesh )	return psingletexmesh;
	}

	return NULL;
}

void DxPieceMultiStaticMaterialMesh::UpdateLightMapUV( float fMultiX, float fMultiY )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->UpdateLightMapUV( fMultiX, fMultiY );

	}
}

void DxPieceMultiStaticMaterialMesh::CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->CopyMesh( pd3dDevice, vecMesh );

	}
}