#include "stdafx.h"
#include "./DxMultiStaticMaterialMesh.h"

#include "../Common/SerialFile.h"

#include "../DxCommon/DxFogMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void DxMultiStaticMaterialMesh::BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vPos )
{
	float fDis = DxFogMan::GetInstance().GetFogRangeFar();
	D3DXVECTOR3 vMax = vPos + D3DXVECTOR3( fDis, fDis, fDis );
	D3DXVECTOR3 vMin = vPos - D3DXVECTOR3( fDis, fDis, fDis );

	BaseLoad( pd3dDevice, vMax, vMin );
}

void DxMultiStaticMaterialMesh::BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin )
{
	for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->BaseLoad( pd3dDevice, vMax, vMin, &m_SFile );
	}
}

void DxMultiStaticMaterialMesh::Load_File ( LPDIRECT3DDEVICEQ pd3dDevice, const char* szName )
{
	std::string strName = szName;

	std::string::size_type idx;
	idx = strName.find( ".wld" );
	if( idx!=std::string::npos )	strName += '2';	

	m_SFile.CloseFile();
	if ( m_SFile.OpenFile ( FOT_READ, strName.c_str() ) )	
		Load( pd3dDevice, m_SFile );
}

void DxMultiStaticMaterialMesh::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();

	DWORD dwVer, dwSize;
	SFile >> dwVer;
	SFile >> dwSize;

	if ( dwVer >= VERSION_ENCODE )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_2_VER_1 );

	if ( dwVer >= VERSION_ENCODE_2 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_2_VER_2 );

	if ( dwVer >= VERSION_ENCODE_3 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_2_VER_3 );

	if ( dwVer >= VERSION_ENCODE_4 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_2_VER_4 );

	if ( dwVer >= ENCODE_VER_BYTE_CRYPT_2021_WLD2_V1 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_WLD2_V1 );

	if ( dwVer == VERSION_MULTI_STATIC_MATERIAL_MESH )
	{
		Load_204( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0203 )
	{
		Load_203( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0202 )
	{
		Load_202( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0201 )
	{
		Load_201( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0200 )
	{
		Load_200( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0100 )
	{
		Load_100( pd3dDevice, SFile );
	}
	else
	{
		CDebugSet::ErrorVersion( "DxMultiStaticMaterialMesh::Load_120", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwSize );
	}

	CalculateAABB();
}

void DxMultiStaticMaterialMesh::Load_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;

	for( DWORD i=0; i<dwNum; ++i )
	{
		DxStaticMaterialMesh* pstatic_mesh = new DxStaticMaterialMesh;
		pstatic_mesh->Load( pd3dDevice, SFile, TRUE, FALSE );
		m_vecStaticMesh.push_back( pstatic_mesh );
	}
}

void DxMultiStaticMaterialMesh::Load_200( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;

	for( DWORD i=0; i<dwNum; ++i )
	{
		DxStaticMaterialMesh* pstatic_mesh = new DxStaticMaterialMesh;
		pstatic_mesh->Load( pd3dDevice, SFile, TRUE, FALSE );
		m_vecStaticMesh.push_back( pstatic_mesh );
	}
}

void DxMultiStaticMaterialMesh::Load_201( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;

	for( DWORD i=0; i<dwNum; ++i )
	{
		DxStaticMaterialMesh* pstatic_mesh = new DxStaticMaterialMesh;
		pstatic_mesh->Load( pd3dDevice, SFile, TRUE, FALSE );
		m_vecStaticMesh.push_back( pstatic_mesh );
	}
}

void DxMultiStaticMaterialMesh::Load_202( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;

	for( DWORD i=0; i<dwNum; ++i )
	{
		DxStaticMaterialMesh* pstatic_mesh = new DxStaticMaterialMesh;
		pstatic_mesh->Load( pd3dDevice, SFile, TRUE, FALSE );
		m_vecStaticMesh.push_back( pstatic_mesh );
	}
}

void DxMultiStaticMaterialMesh::Load_203( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;

	for( DWORD i=0; i<dwNum; ++i )
	{
		DxStaticMaterialMesh* pstatic_mesh = new DxStaticMaterialMesh;
		pstatic_mesh->Load( pd3dDevice, SFile, TRUE, FALSE );
		m_vecStaticMesh.push_back( pstatic_mesh );
	}
}

void DxMultiStaticMaterialMesh::Load_204( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;

	for( DWORD i=0; i<dwNum; ++i )
	{
		DxStaticMaterialMesh* pstatic_mesh = new DxStaticMaterialMesh;
		pstatic_mesh->Load( pd3dDevice, SFile, TRUE, FALSE );
		m_vecStaticMesh.push_back( pstatic_mesh );
	}
}


void DxMultiStaticMaterialMesh::Save_File ( const char* szName )
{
	std::string strName = szName;

	std::string::size_type idx;
	idx = strName.find( ".wld" );
	if( idx!=std::string::npos )	strName += '2';

	if ( m_SFile.OpenFile ( FOT_WRITE, strName.c_str() ) )	
		Save( m_SFile, FALSE );

	m_SFile.CloseFile();
}

void DxMultiStaticMaterialMesh::Save ( CSerialFile& SFile, BOOL bPiece )
{
	DWORD	dwSize = 0;

	SFile << (DWORD)VERSION_MULTI_STATIC_MATERIAL_MESH;

	SFile.BeginBlock();
	{
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_WLD2_V1 );

		SFile << (DWORD)m_vecStaticMesh.size();
		for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
		{
			DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
			if ( !pmesh )	continue;

			pmesh->Save( SFile, bPiece );
		}

		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_NONE );
	}
	SFile.EndBlock();
}