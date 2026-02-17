#include "stdafx.h"

#include <algorithm>
#include "../Common/StlFunctions.h"

#include "../Common/SerialFile.h"
#include "../DxCommon/DxRenderStates.h"
#include "../DxCommon/DxViewPort.h"
#include "./TextureSetDXT.h"
#include "../DxOctree/DxLandMan.h"

#include "./DxStaticMaterialMesh.h"
#include "../DxCommon/TextureManager.h"
#include "../DxCommon/DxShaderManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void DxStaticMaterialMesh::BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vPos, CSerialFile* const SFile )
{
	float fDis = DxFogMan::GetInstance().GetFogRangeFar();
	D3DXVECTOR3 vMax = vPos + D3DXVECTOR3( fDis, fDis, fDis );
	D3DXVECTOR3 vMin = vPos - D3DXVECTOR3( fDis, fDis, fDis );

	BaseLoad( pd3dDevice, vMax, vMin, SFile );
}

void DxStaticMaterialMesh::BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin, CSerialFile* const SFile )
{
	COLLISION::LoadAABBTreeBOX( pd3dDevice, vMax, vMin, m_pMeshTree, SFile );
}

void DxStaticMaterialMesh::Save ( CSerialFile& SFile, BOOL bPiece )
{
	SFile << (DWORD)VERSION_STATIC_MATERIAL_MESH;

	SFile.BeginBlock();
	{
		SFile << m_vMax;
		SFile << m_vMin;

		SFile << unknown_var1;
		SFile << m_strLightMapDay;
		SFile << m_strLightMapDS1;
		SFile << m_strLightMapDS2;
		SFile << m_strLightMapShadow;
	}
	SFile.EndBlock();

	SingleTexMesh_Save( m_mapMesh,	SFile, bPiece );
}

void DxStaticMaterialMesh::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, const BOOL bPiece )
{
	CleanUp();	// 모두 초기화

	DWORD dwVer, dwSize;
	SFile >> dwVer;
	SFile >> dwSize;

	if( dwVer==VERSION_STATIC_MATERIAL_MESH )
	{
		Load_200( pd3dDevice, SFile, bDynamicLoad, bPiece );
	}
#ifdef _DEBUG
	else if( dwVer==0x0105 )
	{
		Load_105( pd3dDevice, SFile, bDynamicLoad, bPiece );
	}
	else if( dwVer==0x0104 )
	{
		Load_104( pd3dDevice, SFile, bDynamicLoad, bPiece );
	}
	else if( dwVer==0x0103 )
	{
		Load_103( pd3dDevice, SFile, bDynamicLoad, bPiece );
	}
	else if( dwVer==0x0102 )
	{
		Load_102( pd3dDevice, SFile, bDynamicLoad, bPiece );
	}
	else if( dwVer==0x0101 )
	{
		Load_101( pd3dDevice, SFile, bDynamicLoad, bPiece );
	}
	else if( dwVer==0x0100 )
	{
		Load_100( pd3dDevice, SFile, bDynamicLoad, bPiece );
	}
#endif
	else
	{
		CDebugSet::ErrorVersion( "DxStaticMaterialMesh::Load", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwSize );
		return;
	}

	SortMaterialContainer( pd3dDevice, bPiece );

	NSSTATICMATERIALMESH::ConvertMAPtoLIST( m_mapMesh, m_pList );
	COLLISION::MakeAABBTree( m_pMeshTree, m_pList );


	SetAABBMaxMin();
	LoadLightMaps( pd3dDevice );
}

void DxStaticMaterialMesh::Load_200( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SFile >> unknown_var1;
	SFile >> m_strLightMapDay;
	SFile >> m_strLightMapDS1;
	SFile >> m_strLightMapDS2;
	SFile >> m_strLightMapShadow;
	
	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
}

void DxStaticMaterialMesh::Load_105( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SFile >> m_strLightMapDay;
	SFile >> m_strLightMapDS1;
	SFile >> m_strLightMapDS2;
	SFile >> m_strLightMapShadow;
	SFile >> unknown_var1;

	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
}

void DxStaticMaterialMesh::Load_104( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SFile >> m_strLightMapDay;
	SFile >> m_strLightMapDS1;
	SFile >> m_strLightMapDS2;
	SFile >> unknown_var1;

	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
}

void DxStaticMaterialMesh::Load_103( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SFile >> m_strLightMapDay;
	SFile >> m_strLightMapDS1;
	SFile >> unknown_var1;

	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
}

void DxStaticMaterialMesh::Load_102( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SFile >> m_strLightMapDay;
	SFile >> m_strLightMapDS1;
	SFile >> unknown_var1;

	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
}

void DxStaticMaterialMesh::Load_101( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SFile >> unknown_var1;

	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
}

void DxStaticMaterialMesh::Load_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SFile >> unknown_var1;

	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
}

void DxStaticMaterialMesh::SingleTexMesh_Save( MAPSINGLETEXMATERIALMESH& mapMesh, CSerialFile& SFile, BOOL bPiece )
{
	DWORD	dwSize = 0;

	dwSize = (DWORD)mapMesh.size();
	SFile << dwSize;

	MAPSINGLETEXMATERIALMESH_ITER iter;
	for ( iter=mapMesh.begin(); iter!=mapMesh.end(); ++iter )
	{
		SFile << (*iter).first;
		(*iter).second->Save( SFile, bPiece );
	}
}

void DxStaticMaterialMesh::SingleTexMesh_Load_Insert( LPDIRECT3DDEVICEQ pd3dDevice, MAPSINGLETEXMATERIALMESH& mapMesh, CSerialFile& SFile, BOOL bDynamicLoad, BOOL bPiece )
{
	DWORD	dwSize = 0;

	if( bPiece )	m_emRenderType = EMRT_PIECE;
	else			m_emRenderType = EMRT_OCTREE;

	std::string szName;
	SFile >> dwSize;
	for ( DWORD i=0; i<dwSize; ++i )
	{
		SFile >> szName;
		DxSingleTexMaterialMesh* pNew = new DxSingleTexMaterialMesh;
		pNew->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );

		CString strName;
		strName.Format( "%d_%s", i, szName.c_str() );

		MAPSINGLETEXMATERIALMESH_ITER iter = mapMesh.find( strName.GetString() );
		if ( iter != mapMesh.end() )
		{
			CDebugSet::ErrorCode( "DxStaticMaterialMesh::SingleTexMesh_Load_Insert", "DxSingleTexMaterialMesh exist in map %s", strName.GetString() );
			SAFE_DELETE(pNew);
		}else{
			mapMesh.insert( std::make_pair ( strName.GetString(), pNew ) );
		}
	}
}
