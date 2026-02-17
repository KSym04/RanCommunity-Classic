#include "stdafx.h"

#include <algorithm>
#include "../Common/StlFunctions.h"

#include "../Common/SerialFile.h"
#include "../DxCommon/DxRenderStates.h"
#include "../DxCommon/DxViewPort.h"
#include "./TextureSetDXT.h"
#include "../DxOctree/DxLandMan.h"

#include "./DxStaticMesh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void DxStaticMesh::BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vPos, CSerialFile* const SFile )
{
	float fDis = DxFogMan::GetInstance().GetFogRangeFar();
	D3DXVECTOR3 vMax = vPos + D3DXVECTOR3( fDis, fDis, fDis );
	D3DXVECTOR3 vMin = vPos - D3DXVECTOR3( fDis, fDis, fDis );

	BaseLoad( pd3dDevice, vMax, vMin, SFile );
}

void DxStaticMesh::BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin, CSerialFile* const SFile )
{
	COLLISION::LoadAABBTreeBOX( pd3dDevice, vMax, vMin, m_pMeshTree, SFile );
	COLLISION::LoadAABBTreeBOX( pd3dDevice, vMax, vMin, m_pMeshAlphaTree, SFile );
	COLLISION::LoadAABBTreeBOX( pd3dDevice, vMax, vMin, m_pMeshSoftAlphaTree, SFile );
	COLLISION::LoadAABBTreeBOX( pd3dDevice, vMax, vMin, m_pMeshSoftAlphaTree01, SFile );
	COLLISION::LoadAABBTreeBOX( pd3dDevice, vMax, vMin, m_pMeshSoftAlphaTree02, SFile );
}

void DxStaticMesh::Save ( CSerialFile& SFile, BOOL bPiece )
{
	SFile << (DWORD)VERSION_STATIC_MESH;

	SFile.BeginBlock();
	{
		SFile << m_vMax;
		SFile << m_vMin;
		SFile << unknown_var1;
	}
	SFile.EndBlock();

	SingleTexMesh_Save( m_mapMesh,			SFile, bPiece );
	SingleTexMesh_Save( m_mapMeshSOFTALPHA, SFile, bPiece );
	SingleTexMesh_Save( m_mapMeshSOFTALPHA01, SFile, bPiece );
	SingleTexMesh_Save( m_mapMeshSOFTALPHA02, SFile, bPiece );
	SingleTexMesh_Save( m_mapMeshALPHA,		SFile, bPiece );
}

void DxStaticMesh::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, const BOOL bPiece )
{
	CleanUp();	// 모두 초기화

	DWORD dwVer, dwSize;
	SFile >> dwVer;
	SFile >> dwSize;

	if( dwVer==VERSION_STATIC_MESH )
	{
		Load_200( pd3dDevice, SFile, bDynamicLoad, bPiece );
	}
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
		Load_101( pd3dDevice, SFile, bDynamicLoad, bPiece );
		SetAABBMaxMin ();
	}
	else
	{
		CDebugSet::ErrorVersion( "DxStaticMesh::Load", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwSize );
		return;
	}

	// Tree를 만들기 위해 std::map 형식을 일반적인 list 형식으로 참조한다.
	NSSTATICMESH::ConvertMAPtoLIST( m_mapMesh,				m_pList );
	NSSTATICMESH::ConvertMAPtoLIST( m_mapMeshALPHA,			m_pAlphaList );
	NSSTATICMESH::ConvertMAPtoLIST( m_mapMeshSOFTALPHA,		m_pAlphaSoftList );
	NSSTATICMESH::ConvertMAPtoLIST( m_mapMeshSOFTALPHA01,	m_pAlphaSoftList01 );
	NSSTATICMESH::ConvertMAPtoLIST( m_mapMeshSOFTALPHA02,	m_pAlphaSoftList02 );

	// SingleMehs Tree를 생성한다.
	COLLISION::MakeAABBTree( m_pMeshTree,				m_pList );
	COLLISION::MakeAABBTree( m_pMeshAlphaTree,			m_pAlphaList );
	COLLISION::MakeAABBTree( m_pMeshSoftAlphaTree,		m_pAlphaSoftList );
	COLLISION::MakeAABBTree( m_pMeshSoftAlphaTree01,	m_pAlphaSoftList01 );
	COLLISION::MakeAABBTree( m_pMeshSoftAlphaTree02,	m_pAlphaSoftList02 );
}

void DxStaticMesh::Load_200( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;
	SFile >> unknown_var1;

	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA01, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA02, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshALPHA, SFile, bDynamicLoad, bPiece );
}

void DxStaticMesh::Load_105( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;
	SFile >> unknown_var1;

	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMeshSOFTALPHA, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMeshSOFTALPHA01, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMeshSOFTALPHA02, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMeshALPHA, SFile, bDynamicLoad, bPiece );
}

void DxStaticMesh::Load_104( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMeshSOFTALPHA, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMeshSOFTALPHA01, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMeshSOFTALPHA02, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_104( pd3dDevice, m_mapMeshALPHA, SFile, bDynamicLoad, bPiece );
}

void DxStaticMesh::Load_103( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA01, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA02, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshALPHA, SFile, bDynamicLoad, bPiece );
}

void DxStaticMesh::Load_102( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshALPHA, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA01, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA02, SFile, bDynamicLoad, bPiece );
}

void DxStaticMesh::Load_101( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMesh, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshALPHA, SFile, bDynamicLoad, bPiece );
	SingleTexMesh_Load_Insert_100( pd3dDevice, m_mapMeshSOFTALPHA, SFile, bDynamicLoad, bPiece );
}

void DxStaticMesh::SingleTexMesh_Save( MAPSINGLETEXMESH& mapMesh, CSerialFile& SFile, BOOL bPiece )
{
	DWORD	dwSize = 0;

	dwSize = (DWORD)mapMesh.size();
	SFile << dwSize;

	MAPSINGLETEXMESH_ITER iter;
	for ( iter=mapMesh.begin(); iter!=mapMesh.end(); ++iter )
	{
		SFile << (*iter).first;
		(*iter).second->Save( SFile, bPiece );
	}
}

void DxStaticMesh::SingleTexMesh_Load_Insert_100( LPDIRECT3DDEVICEQ pd3dDevice, MAPSINGLETEXMESH& mapMesh, CSerialFile& SFile, BOOL bDynamicLoad, BOOL bPiece )
{
	DWORD	dwSize = 0;

	if( bPiece )	m_emRenderType = EMRT_PIECE;
	else			m_emRenderType = EMRT_OCTREE;

	std::string szName;
	SFile >> dwSize;
	for ( DWORD i=0; i<dwSize; ++i )
	{
		SFile >> szName;
		DxSingleTexMesh* pNew = new DxSingleTexMesh;
		pNew->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );

		MAPSINGLETEXMESH_ITER iter = mapMesh.find( szName.c_str() );
		if ( iter != mapMesh.end() )
		{
			CDebugSet::ErrorCode( "DxStaticMesh::SingleTexMesh_Load_Insert_100", "DxSingleTexMesh exist in map %s", szName.c_str() );
			SAFE_DELETE(pNew);
		}else{
			mapMesh.insert( std::make_pair ( szName.c_str(), pNew ) );
		}
	}
}

void DxStaticMesh::SingleTexMesh_Load_Insert_104( LPDIRECT3DDEVICEQ pd3dDevice, MAPSINGLETEXMESH& mapMesh, CSerialFile& SFile, BOOL bDynamicLoad, BOOL bPiece )
{
	DWORD	dwSize = 0;

	if( bPiece )	m_emRenderType = EMRT_PIECE;
	else			m_emRenderType = EMRT_OCTREE;

	std::string szName;
	SFile >> dwSize;
	for ( DWORD i=0; i<dwSize; ++i )
	{
		SFile >> szName;

		DWORD dwKey(0); //flag for name formating by official
		SFile >> dwKey;

		//format string key using key and loop
		CString strKey;
		strKey.Format( "%u_%u_%s", dwKey, i, szName.c_str() );

		DxSingleTexMesh* pNew = new DxSingleTexMesh;
		pNew->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );

		MAPSINGLETEXMESH_ITER iter = mapMesh.find( strKey.GetString() );
		if ( iter != mapMesh.end() )
		{
			CDebugSet::ErrorCode( "DxStaticMesh::SingleTexMesh_Load_Insert_104", "DxSingleTexMesh exist in map %s", strKey.GetString() );
			SAFE_DELETE(pNew);
		}else{
			mapMesh.insert( std::make_pair ( strKey.GetString(), pNew ) );
		}
	}
}

