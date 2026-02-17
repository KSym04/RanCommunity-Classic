#include "stdafx.h"
#include "./DxShadowMesh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void DxShadowMeshTree::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();

	SFile >> m_vMax;
	SFile >> m_vMin;

	m_vMax += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
	m_vMin -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{
		SFile >> m_dwFVF;
		SFile >> m_dwVert;
		SFile >> m_dwFace;

		m_nFVFSize = D3DXGetFVFVertexSize( m_dwFVF );

		SAFE_RELEASE( m_pVB );
		pd3dDevice->CreateVertexBuffer( m_dwVert*m_nFVFSize, D3DUSAGE_WRITEONLY, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, NULL );

		SAFE_RELEASE( m_pIB );
		pd3dDevice->CreateIndexBuffer( m_dwFace*3*sizeof(WORD), 0L, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL );

		if( !m_pVB || !m_pIB )
		{
			SAFE_RELEASE( m_pVB );
			SAFE_RELEASE( m_pIB );
			return;
		}

		BYTE*	pTempVB(NULL);
		pTempVB = new BYTE[m_dwVert*m_nFVFSize];
		SFile.ReadBuffer( pTempVB, m_dwVert*m_nFVFSize );

		WORD* pIndices;
		m_pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
		SFile.ReadBuffer( pIndices, sizeof(WORD)*m_dwFace*3 );
		m_pIB->Unlock();

		BYTE* pVertices(NULL);
		m_pVB->Lock( 0, 0, (VOID**)&pVertices, 0L );
		memcpy( pVertices, pTempVB, m_dwVert*m_nFVFSize );
		m_pVB->Unlock();

		SAFE_DELETE_ARRAY( m_pVertex );
		m_pVertex = new VERTEX_SHADOW[m_dwVert];
		memcpy( m_pVertex, pTempVB, sizeof(VERTEX_SHADOW)*m_dwVert );

		SAFE_DELETE_ARRAY( pTempVB );
	}

	SFile >> bExist;
	if ( bExist )
	{
		m_pNext = new DxShadowMeshTree;
		m_pNext->Load( pd3dDevice, SFile );
	}
}

void DxShadowMeshTree::Save ( CSerialFile& SFile )
{
	SFile << m_vMax;
	SFile << m_vMin;

	if ( m_pVB && m_pIB )
	{
		SFile << (BOOL)TRUE;

		SFile << m_dwFVF;
		SFile << m_dwVert;
		SFile << m_dwFace;

		m_nFVFSize = D3DXGetFVFVertexSize( m_dwFVF );

		BYTE* pVertices;
		m_pVB->Lock( 0, 0, (VOID**)&pVertices, 0L );
		SFile.WriteBuffer( pVertices, m_nFVFSize*m_dwVert );
		m_pVB->Unlock();

		WORD* pIndices;
		m_pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
		SFile.WriteBuffer( pIndices, sizeof(WORD)*m_dwFace*3 );
		m_pIB->Unlock();
	}
	else
	{
		SFile << (BOOL)FALSE;
	}
	
	if( m_pNext )
	{
		SFile << (BOOL)TRUE;
		m_pNext->Save( SFile );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}
}

void DxShadowMesh::Load_File ( LPDIRECT3DDEVICEQ pd3dDevice, const char* szName )
{
	std::string strName = szName;

	std::string::size_type idx;
	idx = strName.find( ".wld" );
	if( idx!=std::string::npos )	strName += '5';	

	CSerialFile m_SFile;
	if ( m_SFile.OpenFile ( FOT_READ, strName.c_str() ) )	
		Load( pd3dDevice, m_SFile );
}

void DxShadowMesh::Save_File ( const char* szName )
{
	std::string strName = szName;

	std::string::size_type idx;
	idx = strName.find( ".wld" );
	if( idx!=std::string::npos )	strName += '5';

	CSerialFile m_SFile;
	if ( m_SFile.OpenFile ( FOT_WRITE, strName.c_str() ) )	
		Save( m_SFile );

}

void DxShadowMesh::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();

	DWORD dwVer, dwSize;
	SFile >> dwVer;
	SFile >> dwSize;

	if ( dwVer >= VERSION_ENCODE )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_2_VER_1 );

	if ( dwVer >= VERSION_ENCODE_2 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_3_VER_1 );

	if ( dwVer >= VERSION_ENCODE_3 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_3_VER_2 );

	if ( dwVer >= VERSION_ENCODE_4 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_3_VER_3 );

	if ( dwVer >= ENCODE_VER_BYTE_CRYPT_2021_WLD3_V1 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_WLD3_V1 );

	if ( dwVer == VERSION_SHADOW_MESH )
	{
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			m_pHead = new DxShadowMeshTree;
			m_pHead->Load( pd3dDevice, SFile );
		}
	}
	else if ( dwVer == 0x0203 )
	{
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			m_pHead = new DxShadowMeshTree;
			m_pHead->Load( pd3dDevice, SFile );
		}
	}
	else if ( dwVer == 0x0202 )
	{
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			m_pHead = new DxShadowMeshTree;
			m_pHead->Load( pd3dDevice, SFile );
		}
	}
	else if ( dwVer == 0x0201 )
	{
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			m_pHead = new DxShadowMeshTree;
			m_pHead->Load( pd3dDevice, SFile );
		}
	}
	else if ( dwVer == 0x0200 )
	{
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			m_pHead = new DxShadowMeshTree;
			m_pHead->Load( pd3dDevice, SFile );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxShadowMesh::Load", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwSize );
	}
}

void DxShadowMesh::Save ( CSerialFile& SFile )
{
	SFile << (DWORD)VERSION_SHADOW_MESH;

	SFile.BeginBlock(EMBLOCK_00);
	{
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_WLD3_V1 );

		if( m_pHead )
		{
			SFile << (BOOL)TRUE;
			m_pHead->Save( SFile );
		}
		else
		{
			SFile << (BOOL)FALSE;
		}

		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_NONE );
	}
	SFile.EndBlock(EMBLOCK_00);
}