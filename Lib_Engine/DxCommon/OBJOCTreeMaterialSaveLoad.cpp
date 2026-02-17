#include "stdafx.h"
#include "./OBJOCTreeMaterial.h"

#include "./DxLightMan.h"
#include "./Collision.h"
#include "./NsOCTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const DWORD OBJOCTreeMaterial::VERSION = 0x0200;

BOOL OBJOCTreeMaterial::IsLoad()
{
	if( !m_bLoad )			return FALSE;
	if( !m_pVB )			return FALSE;

	return TRUE; 
}

void OBJOCTreeMaterial::Save ( CSerialFile& SFile, BOOL bPiece )
{
	SFile << (DWORD)VERSION;

	SFile.BeginBlock(EMBLOCK_02);
	{
		SFile << vMax;
		SFile << vMin;

		BOOL bExist(FALSE);
		if( m_pVB && m_pIB )
		{
			bExist = TRUE;
		}

		SFile << bExist;
		if ( bExist )
		{
			SFile << m_dwFVF;
			SFile << m_dwVert;
			SFile << m_dwFace;

			m_nFVFSize = D3DXGetFVFVertexSize( m_dwFVF );

			EnterCriticalSection(&m_pCSLockVB);
			{
				BYTE* pVertices;
				m_pVB->Lock( 0, 0, (VOID**)&pVertices, 0L );
				SFile.WriteBuffer ( pVertices, m_nFVFSize*m_dwVert );
				m_pVB->Unlock();
			}
			LeaveCriticalSection(&m_pCSLockVB);

			WORD* pIndices;
			m_pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
			SFile.WriteBuffer ( pIndices, sizeof(WORD)*m_dwFace*3 );
			m_pIB->Unlock();
		}

		bExist = pCollsionTREE ? TRUE : FALSE;
		SFile << bExist;
		if ( pCollsionTREE )
		{
			pCollsionTREE->SaveFile( SFile );
		}
	}
	SFile.EndBlock(EMBLOCK_02);
}

void OBJOCTreeMaterial::Load( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece )
{
	CleanUpBase();
	CleanUpDerived();

	DWORD _dwVER;
	DWORD _dwSIZE;

	SFile >> _dwVER;
	SFile >> _dwSIZE;

	if ( _dwVER == VERSION )
	{
		SFile >> vMax;
		SFile >> vMin;

		vMax += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
		vMin -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			SFile >> m_dwFVF;
			SFile >> m_dwVert;
			SFile >> m_dwFace;

			m_nFVFSize = D3DXGetFVFVertexSize( m_dwFVF );

			BYTE*	pTempVB(NULL);
			pTempVB = new BYTE[m_dwVert*m_nFVFSize];

			EnterCriticalSection(&m_pCSLockVB);
			{		
				SAFE_RELEASE( m_pVB );
				pd3dDevice->CreateVertexBuffer( m_dwVert*m_nFVFSize, D3DUSAGE_WRITEONLY, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, NULL );
				if( !m_pVB )
				{
					SAFE_DELETE_ARRAY( pTempVB );
					LeaveCriticalSection(&m_pCSLockVB);
					return;
				}
			}
			LeaveCriticalSection(&m_pCSLockVB);

			SAFE_RELEASE( m_pIB );
			pd3dDevice->CreateIndexBuffer( m_dwFace*3*sizeof(WORD), 0L, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL );
			if( !m_pIB )
			{
				SAFE_DELETE_ARRAY( pTempVB );

				EnterCriticalSection(&m_pCSLockVB);
				SAFE_RELEASE( m_pVB );
				LeaveCriticalSection(&m_pCSLockVB);

				return;
			}

			SFile.ReadBuffer( pTempVB, m_dwVert*m_nFVFSize );

			WORD* pIndices;
			m_pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
			SFile.ReadBuffer( pIndices, sizeof(WORD)*m_dwFace*3 );
			m_pIB->Unlock();
 
			Sleep(0);

			EnterCriticalSection(&m_pCSLockVB);
			{
				BYTE* pVertices(NULL);
				m_pVB->Lock( 0, 0, (VOID**)&pVertices, 0L );
				memcpy( pVertices, pTempVB, m_dwVert*m_nFVFSize );
				m_pVB->Unlock();
			}
			LeaveCriticalSection(&m_pCSLockVB);

			Sleep(0);

			SAFE_DELETE_ARRAY( m_pVertSrc );
			m_pVertSrc = new VERTEXNORCOLORTEX_850[m_dwVert];
			memcpy( m_pVertSrc, pTempVB, sizeof(VERTEXNORCOLORTEX_850)*m_dwVert );

			Sleep(0);

			SAFE_DELETE_ARRAY( pTempVB );
		}

		SFile >> bExist;
		if ( bExist )
		{
			pCollsionTREE = new DxAABBNode;
			if ( pCollsionTREE )	pCollsionTREE->LoadFile( SFile );
		}
	}
	else if ( _dwVER == 0x0101 )
	{
		SFile >> vMax;
		SFile >> vMin;

		vMax += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
		vMin -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			DWORD _dwFVF;
			SFile >> _dwFVF;
			SFile >> m_dwVert;
			SFile >> m_dwFace;

			UINT nFVFSize = D3DXGetFVFVertexSize( _dwFVF );

			CreateBASE( pd3dDevice, m_dwVert, m_dwFace );
			if( !m_pVB )	return;
			if( !m_pIB )	return;

			BYTE*	pTempVB(NULL);
			pTempVB = new BYTE[m_dwVert*nFVFSize];
			SFile.ReadBuffer( pTempVB, m_dwVert*nFVFSize );

			WORD* pIndices;
			m_pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
			SFile.ReadBuffer( pIndices, sizeof(WORD)*m_dwFace*3 );
			m_pIB->Unlock();

			CreateVertSrc( pTempVB, _dwFVF, m_dwVert );
			CreateNormalColor( pTempVB, _dwFVF, m_dwVert );
			CopyVertSrcToVB( pTempVB, m_pVertSrc, m_pVB, m_dwVert );

			SAFE_DELETE_ARRAY( pTempVB );

			SFile >> bExist;
			if ( bExist )
			{
				pCollsionTREE = new DxAABBNode;
				if ( pCollsionTREE )	pCollsionTREE->LoadFile_BETA( SFile );
			}
		}
	}
	else if ( _dwVER == 0x0100 )
	{
		SFile >> vMax;
		SFile >> vMin;

		vMax += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
		vMin -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			DWORD _dwFVF;
			SFile >> _dwFVF;
			SFile >> m_dwVert;
			SFile >> m_dwFace;

			UINT nFVFSize = D3DXGetFVFVertexSize( _dwFVF );

			CreateBASE( pd3dDevice, m_dwVert, m_dwFace );
			if( !m_pVB )	return;
			if( !m_pIB )	return;

			BYTE*	pTempVB(NULL);
			pTempVB = new BYTE[m_dwVert*nFVFSize];
			SFile.ReadBuffer( pTempVB, m_dwVert*nFVFSize );

			WORD* pIndices;
			m_pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
			SFile.ReadBuffer( pIndices, sizeof(WORD)*m_dwFace*3 );
			m_pIB->Unlock();

			CreateVertSrc( pTempVB, _dwFVF, m_dwVert );
			CreateNormalColor( pTempVB, _dwFVF, m_dwVert );
			CopyVertSrcToVB( pTempVB, m_pVertSrc, m_pVB, m_dwVert );

			SAFE_DELETE_ARRAY( pTempVB );

			SFile >> bExist;
			if ( bExist )
			{
				pCollsionTREE = new DxAABBNode;
				if ( pCollsionTREE )	pCollsionTREE->LoadFile( SFile );
			}
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "OBJOCTreeMaterial::Load", SFile.GetFileName(), _dwVER );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+_dwSIZE );
	}

	m_bLoad = TRUE;
}

void OBJOCTreeMaterial::Load_OLD( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece )
{
}

void OBJOCTreeMaterial::ThreadLoad( LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading )
{
	if( m_bCheck )	return;
	m_bCheck = TRUE;

	DWORD dwData = (DWORD)(this);

	EnterCriticalSection(&CSLockLoading);
	{
		listLoadingData.push_back( dwData );
	}
	LeaveCriticalSection(&CSLockLoading);
}

BOOL OBJOCTreeMaterial::DynamicLoad( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bThread )
{
	return TRUE;
}
