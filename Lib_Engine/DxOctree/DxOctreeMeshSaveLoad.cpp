#include "stdafx.h"

#include "./DxOctreeMesh.h"
#include "../Common/SerialFile.h"
#include "../DxCommon/Collision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL	DxOctreeMesh::SaveFile ( CSerialFile &SFile )
{
	HRESULT hr(S_OK);

	SFile << m_dwFVF;

	//Note : VB
	OCTREEVERTEX	*ppbDataVB = NULL;
	SFile << m_dwNumVertices;	
	hr = m_pVB->Lock( 0, 0, (VOID**)&ppbDataVB, NULL );
	if( FAILED(hr) )	CDebugSet::ToLogFile( "DxOctreeMesh::SaveFile() -- m_pVB->Lock() -- Failed" );

	SFile.WriteBuffer ( ppbDataVB, sizeof ( OCTREEVERTEX ) * m_dwNumVertices );
	m_pVB->Unlock();

	//Note : IB
	WORD			*ppbDataIB = NULL;
	SFile << m_dwNumFaces;
	hr = m_pIB->Lock( 0, 0, (VOID**)&ppbDataIB, NULL );
	if( FAILED(hr) )	CDebugSet::ToLogFile( "DxOctreeMesh::SaveFile() -- m_pIB->Lock() -- Failed" );

	SFile.WriteBuffer ( ppbDataIB, sizeof ( WORD ) * m_dwNumFaces * 3 );
	m_pIB->Unlock();

	//Note : D3DXATTRIBUTERANGE
	SFile << m_dwAttribTableSize;
	SFile.WriteBuffer ( m_pAttribTable, sizeof ( D3DXATTRIBUTERANGE ) * m_dwAttribTableSize );

	if ( m_pAABBTreeRoot )
	{
		SFile << BOOL ( TRUE );
		m_pAABBTreeRoot->SaveFile ( SFile );
	}
	else
	{
		SFile << BOOL ( FALSE );
	}

	return TRUE;
}

BOOL	DxOctreeMesh::LoadFile ( CSerialFile &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr(S_OK);

	BOOL	bExist = FALSE;
	BOOL	bDataOK = TRUE;

	SFile >> m_dwFVF;	
	
	OCTREEVERTEX	*ppbDataVB = NULL;
	SFile >> m_dwNumVertices;
	if( m_dwNumVertices > 0 )
	{
		ppbDataVB = new OCTREEVERTEX [ m_dwNumVertices ];
		SFile.ReadBuffer ( ppbDataVB, sizeof ( OCTREEVERTEX ) * m_dwNumVertices );	
	}
	else
	{
		bDataOK = FALSE;
		CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- Data Error -- VB" );
	}
	
	WORD* ppbDataIB = NULL;	
	SFile >> m_dwNumFaces;
	if( m_dwNumFaces > 0 )
	{
		ppbDataIB = new WORD [ m_dwNumFaces * 3 ];
		SFile.ReadBuffer ( ppbDataIB, sizeof ( WORD ) * m_dwNumFaces * 3 );
	}
	else
	{
		bDataOK = FALSE;
		CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- Data Error -- IB" );
	}

	// Note : D3DXATTRIBUTERANGE
	SFile >> m_dwAttribTableSize;
	if( m_dwAttribTableSize > 0 )
	{
		CreateAttribute ( m_dwAttribTableSize );
		SFile.ReadBuffer ( m_pAttribTable, sizeof ( D3DXATTRIBUTERANGE ) * m_dwAttribTableSize );
	}
	else
	{
		bDataOK = FALSE;
		CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- Data Error -- D3DXATTRIBUTERANGE" );
	}

	if( bDataOK )
	{
		// Note : 
		CreateDxOctreeMesh ( pd3dDevice, m_dwNumFaces, m_dwNumVertices, m_dwFVF );

		// Note : VB
		OCTREEVERTEX	*ppbDataLockVB = NULL;
		hr = m_pVB->Lock( 0, 0, (VOID**)&ppbDataLockVB, NULL );
		if( FAILED(hr) )	CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- m_pVB->Lock() -- Failed" );

		memcpy ( ppbDataLockVB, ppbDataVB, sizeof ( OCTREEVERTEX ) * m_dwNumVertices );			
		m_pVB->Unlock();

		// Note : IB
		WORD			*ppbDataLockIB = NULL;
		hr = m_pIB->Lock( 0, 0, (VOID**)&ppbDataLockIB, NULL );
		if( FAILED(hr) )	CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- m_pIB->Lock() -- Failed" );

		memcpy ( ppbDataLockIB, ppbDataIB, sizeof ( WORD ) * m_dwNumFaces * 3 );
		m_pIB->Unlock();
	}
	else
	{
		SAFE_DELETE_ARRAY( m_pAttribTable );
	}

	SAFE_DELETE_ARRAY ( ppbDataVB );
	SAFE_DELETE_ARRAY ( ppbDataIB );

	SFile >> bExist;
	if ( bExist )
	{
		m_pAABBTreeRoot = new DxAABBNode;
		m_pAABBTreeRoot->LoadFile ( SFile );
	}

	return TRUE;
}

BOOL	DxOctreeMesh::LoadFile_BETA ( CSerialFile &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	DWORD dw;
	SFile >> dw; //version

	HRESULT hr(S_OK);

	BOOL	bExist = FALSE;
	BOOL	bDataOK = TRUE;

	DWORD dwFVF;
	SFile >> dwFVF;	

	UINT nFVFSize = D3DXGetFVFVertexSize( dwFVF );

	m_dwFVF = OCTREEVERTEX::FVF;

	SFile >> m_dwNumVertices;
	BYTE*	pTempVB(NULL);
	if( m_dwNumVertices > 0 )
	{
		pTempVB = new BYTE[m_dwNumVertices*nFVFSize];
		SFile.ReadBuffer( pTempVB, m_dwNumVertices*nFVFSize );
	}
	else
	{
		bDataOK = FALSE;
		CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- Data Error -- VB" );
	}

	OCTREEVERTEX	*ppbDataVB = NULL;
	ppbDataVB = new OCTREEVERTEX [ m_dwNumVertices ];



	UINT nSize = D3DXGetFVFVertexSize( dwFVF );

	D3DXVECTOR3* pVector(NULL);
	D3DXVECTOR3* pNormal(NULL);
	DWORD* pDiffuse(NULL);
	float*  pTex1x(NULL);
	float*  pTex1y(NULL);
	float*  pTex2x(NULL);
	float*  pTex2y(NULL);
	float*  pTex3x(NULL);
	float*  pTex3y(NULL);
	float*  pTex4x(NULL);
	float*  pTex4y(NULL);

	if ( dwFVF == (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX4) )
	{
		for( DWORD i=0; i<m_dwNumVertices; ++i )
		{
			pVector = (D3DXVECTOR3*)( pTempVB + (nSize*i) );
			pNormal = (D3DXVECTOR3*)( pTempVB + (nSize*i) + 4*3 );
			pDiffuse =(DWORD*)( pTempVB + (nSize*i) + 4*6 );
			pTex1x	= (float*)( pTempVB + (nSize*i) + 4*7 );
			pTex1y	= (float*)( pTempVB + (nSize*i) + 4*8 );
			pTex2x	= (float*)( pTempVB + (nSize*i) + 4*9 );
			pTex2y	= (float*)( pTempVB + (nSize*i) + 4*10 );

			pTex3x	= (float*)( pTempVB + (nSize*i) + 4*11 );
			pTex3y	= (float*)( pTempVB + (nSize*i) + 4*12 );
			pTex4x	= (float*)( pTempVB + (nSize*i) + 4*13 );
			pTex4y	= (float*)( pTempVB + (nSize*i) + 4*14 );


			ppbDataVB[i].vPos = *pVector;
			ppbDataVB[i].vNormal = *pNormal;
			ppbDataVB[i].tu = *pTex1x;
			ppbDataVB[i].tv = *pTex1y;
		}
	}
	else
	{
		DWORD dwFVFTemp = dwFVF & (~D3DFVF_TEXCOUNT_MASK);
		UINT nSize = D3DXGetFVFVertexSize( dwFVF );
		UINT nOffSet = D3DXGetFVFVertexSize( dwFVFTemp );

		dwFVFTemp = dwFVF & D3DFVF_POSITION_MASK;
		UINT nPositionMarkSize = D3DXGetFVFVertexSize( dwFVFTemp );

		D3DXVECTOR3* pVector(NULL);
		D3DXVECTOR3* pNormal(NULL);
		D3DXVECTOR2* pTex(NULL);
		for( DWORD i=0; i<m_dwNumVertices; ++i )
		{
			pVector = (D3DXVECTOR3*)( pTempVB + (nSize*i) );
			pNormal = (D3DXVECTOR3*)( pTempVB + (nSize*i) + nPositionMarkSize );
			pTex = (D3DXVECTOR2*)( pTempVB + (nSize*i) + nOffSet );

			pTex1x	= (float*)( pTempVB + (nSize*i) + nOffSet );
			pTex1y	= (float*)( pTempVB + (nSize*i) + nOffSet + 1 );

			ppbDataVB[i].vPos = *pVector;
			ppbDataVB[i].vNormal = *pNormal;
			ppbDataVB[i].tu = *pTex1x;
			ppbDataVB[i].tv = *pTex1y;
		}
	}

	SAFE_DELETE_ARRAY ( pTempVB );
	
	WORD* ppbDataIB = NULL;	
	SFile >> m_dwNumFaces;
	if( m_dwNumFaces > 0 )
	{
		ppbDataIB = new WORD [ m_dwNumFaces * 3 ];
		SFile.ReadBuffer ( ppbDataIB, sizeof ( WORD ) * m_dwNumFaces * 3 );
	}
	else
	{
		bDataOK = FALSE;
		CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- Data Error -- IB" );
	}

	// Note : D3DXATTRIBUTERANGE
	SFile >> m_dwAttribTableSize;
	if( m_dwAttribTableSize > 0 )
	{
		CreateAttribute ( m_dwAttribTableSize );
		SFile.ReadBuffer ( m_pAttribTable, sizeof ( D3DXATTRIBUTERANGE ) * m_dwAttribTableSize );
	}
	else
	{
		bDataOK = FALSE;
		CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- Data Error -- D3DXATTRIBUTERANGE" );
	}

	if( bDataOK )
	{
		// Note : 
		CreateDxOctreeMesh ( pd3dDevice, m_dwNumFaces, m_dwNumVertices, m_dwFVF );

		// Note : VB
		OCTREEVERTEX	*ppbDataLockVB = NULL;
		hr = m_pVB->Lock( 0, 0, (VOID**)&ppbDataLockVB, NULL );
		if( FAILED(hr) )	CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- m_pVB->Lock() -- Failed" );

		memcpy ( ppbDataLockVB, ppbDataVB, sizeof ( OCTREEVERTEX ) * m_dwNumVertices );			
		m_pVB->Unlock();

		// Note : IB
		WORD			*ppbDataLockIB = NULL;
		hr = m_pIB->Lock( 0, 0, (VOID**)&ppbDataLockIB, NULL );
		if( FAILED(hr) )	CDebugSet::ToLogFile( "DxOctreeMesh::LoadFile() -- m_pIB->Lock() -- Failed" );

		memcpy ( ppbDataLockIB, ppbDataIB, sizeof ( WORD ) * m_dwNumFaces * 3 );
		m_pIB->Unlock();
	}
	else
	{
		SAFE_DELETE_ARRAY( m_pAttribTable );
	}

	SAFE_DELETE_ARRAY ( ppbDataVB );
	SAFE_DELETE_ARRAY ( ppbDataIB );

	SFile >> bExist;
	if ( bExist )
	{
		m_pAABBTreeRoot = new DxAABBNode;
		m_pAABBTreeRoot->LoadFile ( SFile );
	}

	return TRUE;
}
