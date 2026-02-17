#include "stdafx.h"
#include "./DxPieceNode.h"

#include "./DxPiece.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// -----------------------------------------------------------------------------------------------------------------------------------------
//											D	x		P	i	e	c	e		T	r	e	e
// -----------------------------------------------------------------------------------------------------------------------------------------

const DWORD DxPieceNode::VERSION_PIECE_NODE	= 0x0100;

void DxPieceNode::CleanUp()
{
	//SAFE_DELETE( m_pPiece );

	DxPiece* pDel;
	DxPiece* pCur = m_pPiece;
	while( pCur )
	{
		pDel = pCur;
		pCur = pCur->m_pTreeNext;

		SAFE_DELETE( pDel );
	}

	if( m_pLeftChild )	m_pLeftChild->CleanUp();
	if( m_pRightChild )	m_pRightChild->CleanUp();
}

void DxPieceNode::Load_OLD( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxPieceEdit* pSrc )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	int nCount = 0;
	SFile >> nCount;
	for( int i=0; i<nCount; ++i )
	{
		DxPiece* pNew = new DxPiece;
		pNew->Load_OLD( pd3dDevice, SFile, pSrc );

		// Tree List 연결
		pNew->m_pTreeNext = m_pPiece;
		m_pPiece = pNew;
	}

	BOOL bExist = FALSE;
	SFile >> bExist;
	if( bExist )
	{
		m_pLeftChild = new DxPieceNode;
		m_pLeftChild->Load_OLD( pd3dDevice, SFile, pSrc );
	}

	SFile >> bExist;
	if( bExist )
	{
		m_pRightChild = new DxPieceNode;
		m_pRightChild->Load_OLD( pd3dDevice, SFile, pSrc );
	}
}

void DxPieceNode::Save( CSerialFile& SFile )
{
	SFile << (DWORD)VERSION_PIECE_NODE;

	SFile.BeginBlock(EMBLOCK_03);
	{
		SFile << m_vMax;
		SFile << m_vMin;

		// Note : 갯수 얻기
		int nCount = 0;
		DxPiece* pCur = m_pPiece;
		while( pCur )
		{
			++nCount;
			pCur = pCur->m_pTreeNext;
		}
		SFile << nCount;

		pCur = m_pPiece;
		while( pCur )
		{
			pCur->Save( SFile );
			pCur = pCur->m_pTreeNext;
		}

	}
	SFile.EndBlock(EMBLOCK_03);

	if( m_pLeftChild )
	{
		SFile << (BOOL)TRUE;
		m_pLeftChild->Save( SFile );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}

	if( m_pRightChild )	
	{
		SFile << (BOOL)TRUE;
		m_pRightChild->Save( SFile );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}
}

void DxPieceNode::Load_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxPieceEdit* pSrc )
{
	DWORD dwVER(0);
	DWORD dwSIZE(0);

	SFile >> dwVER;

	if ( dwVER == VERSION_PIECE_NODE )
		SFile >> dwSIZE;

	SFile >> m_vMax;
	SFile >> m_vMin;

	int nCount = 0;
	SFile >> nCount;
	for( int i=0; i<nCount; ++i )
	{
		DxPiece* pNew = new DxPiece;
		pNew->Load_STABLE( pd3dDevice, SFile, pSrc );

		// Tree List 연결
		pNew->m_pTreeNext = m_pPiece;
		m_pPiece = pNew;
	}

	BOOL bExist = FALSE;
	SFile >> bExist;
	if( bExist )
	{
		m_pLeftChild = new DxPieceNode;
		m_pLeftChild->Load_STABLE( pd3dDevice, SFile, pSrc );
	}

	SFile >> bExist;
	if( bExist )
	{
		m_pRightChild = new DxPieceNode;
		m_pRightChild->Load_STABLE( pd3dDevice, SFile, pSrc );
	}
}
