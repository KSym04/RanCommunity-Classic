#include "stdafx.h"
#include "./DxPieceMultiStaticMaterialMesh.h"

#include "../Common/SerialFile.h"

#include "../DxCommon/DxFogMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void DxPieceMultiStaticMaterialMesh::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();

	DWORD dwVer, dwSize;
	SFile >> dwVer;
	SFile >> dwSize;

	if ( dwVer == VERSION_MULTI_STATIC_MATERIAL_MESH_MAIN )
	{
		DWORD dwNum(0);
		SFile >> dwNum;

		for( DWORD i=0; i<dwNum; ++i )
		{
			DWORD _dwVer, _dwSize;
			SFile >> _dwVer;
			SFile >> _dwSize;

			if ( _dwVer == VERSION_MULTI_STATIC_MATERIAL_MESH_SUB )
			{
				DxStaticMaterialMesh* pstatic_mesh = new DxStaticMaterialMesh;
				pstatic_mesh->Load( pd3dDevice, SFile, FALSE, TRUE );
				m_vecStaticMesh.push_back( pstatic_mesh );
			}
			else
			{
				CDebugSet::ErrorVersion( "DxPieceMultiStaticMaterialMesh::Load_SUB", SFile.GetFileName(), _dwVer );
				DWORD dwCur = SFile.GetfTell();
				SFile.SetOffSet( dwCur+_dwSize );
			}
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxPieceMultiStaticMaterialMesh::Load_MAIN", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwSize );
	}

	CalculateAABB();
}

void DxPieceMultiStaticMaterialMesh::Save ( CSerialFile& SFile, BOOL bPiece )
{
	DWORD	dwSize = 0;

	SFile << (DWORD)VERSION_MULTI_STATIC_MATERIAL_MESH_MAIN;

	SFile.BeginBlock(EMBLOCK_04);
	{
		SFile << (DWORD)m_vecStaticMesh.size();

		for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
		{
			SFile << (DWORD)VERSION_MULTI_STATIC_MATERIAL_MESH_SUB;

			SFile.BeginBlock(EMBLOCK_05);
			{
				DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
				if ( !pmesh )	continue;
				pmesh->Save( SFile, bPiece );
			}
			SFile.EndBlock(EMBLOCK_05);
		}
	}
	SFile.EndBlock(EMBLOCK_04);
}