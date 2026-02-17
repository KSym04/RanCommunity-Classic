#include "stdafx.h"
#include "./PIECETREE.h"

#include "./DxPiece.h"
#include "./DxPieceQuickSort.h"

#include "../DxCommon/Collision.h"
#include "../DxCommon/DxMethods.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------
//											N	S		P	I	E	C	E		T	R	E	E
// -----------------------------------------------------------------------------------------------------------------------------------------
namespace NSPIECETREE
{
	BOOL OptimizeDivide( DxPieceNode* pTree, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin )
	{
		float fLengthX = vMax.x - vMin.x;
		float fLengthY = vMax.y - vMin.y;
		float fLengthZ = vMax.z - vMin.z;
		fLengthX = fabsf(fLengthX);
		fLengthY = fabsf(fLengthY);
		fLengthZ = fabsf(fLengthZ);

		float fHalfX = (vMax.x + vMin.x)*0.5f;
		float fHalfY = (vMax.y + vMin.y)*0.5f;
		float fHalfZ = (vMax.z + vMin.z)*0.5f;

		// 가장 긴 선분을 분할 한다.
		int nDivide = 0;
		if( fLengthX>=fLengthY && fLengthX>=fLengthZ )	nDivide = 1;
		else if( fLengthY>=fLengthZ )					nDivide = 2;
		else											nDivide = 3;

		int nLeft = 0;
		int nRight = 0;
		D3DXVECTOR3 vMaxAABB, vMinAABB;
		DxPiece* pCur = pTree->m_pPiece;
		while( pCur )
		{
			vMaxAABB = pCur->m_vMax;
			vMinAABB = pCur->m_vMin;
			D3DXMatrixAABBTransform( vMaxAABB, vMinAABB, pCur->m_matWorld );

			switch( nDivide )
			{
			case 1:
				if( vMinAABB.x > fHalfX )	++nRight;
				else						++nLeft;
				break;
			case 2:
				if( vMinAABB.y > fHalfY )	++nRight;
				else						++nLeft;
				break;
			case 3:
				if( vMinAABB.z > fHalfZ )	++nRight;
				else						++nLeft;
				break;
			};

			pCur = pCur->m_pTreeNext;
		}

		// 균일하게 분할이 안되어서 그만 만든다.
		if( !nRight || !nLeft )		return FALSE;

		// Note : 작업
		pTree->m_pLeftChild = new DxPieceNode;
		pTree->m_pRightChild = new DxPieceNode;
		DxPiece* pLoop = pTree->m_pPiece;
		while( pLoop )
		{
			pCur = pLoop;				// 현재것 저장
			pLoop = pLoop->m_pTreeNext;	// 다음것 미리 셋팅

			vMaxAABB = pCur->m_vMax;
			vMinAABB = pCur->m_vMin;
			D3DXMatrixAABBTransform( vMaxAABB, vMinAABB, pCur->m_matWorld );

			switch( nDivide )
			{
			case 1:
				if( vMinAABB.x > fHalfX )
				{
					pCur->m_pTreeNext = pTree->m_pRightChild->m_pPiece;
					pTree->m_pRightChild->m_pPiece = pCur;
				}
				else
				{
					pCur->m_pTreeNext = pTree->m_pLeftChild->m_pPiece;
					pTree->m_pLeftChild->m_pPiece = pCur;
				}
				break;
			case 2:
				if( vMinAABB.y > fHalfY )
				{
					pCur->m_pTreeNext = pTree->m_pRightChild->m_pPiece;
					pTree->m_pRightChild->m_pPiece = pCur;
				}
				else
				{
					pCur->m_pTreeNext = pTree->m_pLeftChild->m_pPiece;
					pTree->m_pLeftChild->m_pPiece = pCur;
				}
				break;
			case 3:
				if( vMinAABB.z > fHalfZ )
				{
					pCur->m_pTreeNext = pTree->m_pRightChild->m_pPiece;
					pTree->m_pRightChild->m_pPiece = pCur;
				}
				else
				{
					pCur->m_pTreeNext = pTree->m_pLeftChild->m_pPiece;
					pTree->m_pLeftChild->m_pPiece = pCur;
				}
				break;
			};
		}

		// Note : 분할로써 내부의 값은 나뉘었다.
		pTree->m_pPiece = NULL;

		return TRUE;
	}

	void MakeTreeChild( DxPieceNode* pTree )
	{
		// FALSE 일 경우 더 이상 분할 불가
		if( !OptimizeDivide( pTree, pTree->m_vMax, pTree->m_vMin ) )	return;


		DxPiece* pCur = NULL;
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vMax;
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMaxAABB;
		D3DXVECTOR3 vMinAABB;

		// Left
		vMaxAABB = D3DXVECTOR3 ( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		vMinAABB = D3DXVECTOR3 ( FLT_MAX, FLT_MAX, FLT_MAX );
		pCur = pTree->m_pLeftChild->m_pPiece;
		while( pCur )
		{
			vMax = pCur->m_vMax;
			vMin = pCur->m_vMin;
			D3DXMatrixAABBTransform( vMax, vMin, pCur->m_matWorld );


			if( vMaxAABB.x < vMax.x )	vMaxAABB.x = vMax.x;
			if( vMaxAABB.y < vMax.y )	vMaxAABB.y = vMax.y;
			if( vMaxAABB.z < vMax.z )	vMaxAABB.z = vMax.z;

			if( vMinAABB.x > vMin.x )	vMinAABB.x = vMin.x;
			if( vMinAABB.y > vMin.y )	vMinAABB.y = vMin.y;
			if( vMinAABB.z > vMin.z )	vMinAABB.z = vMin.z;

			pCur = pCur->m_pTreeNext;
		}

		vMaxAABB += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
		vMinAABB -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

		pTree->m_pLeftChild->m_vMax = vMaxAABB;
		pTree->m_pLeftChild->m_vMin = vMinAABB;

		MakeTreeChild( pTree->m_pLeftChild );

		// Right
		vMaxAABB = D3DXVECTOR3 ( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		vMinAABB = D3DXVECTOR3 ( FLT_MAX, FLT_MAX, FLT_MAX );
		pCur = pTree->m_pRightChild->m_pPiece;
		while( pCur )
		{
			vMax = pCur->m_vMax;
			vMin = pCur->m_vMin;
			D3DXMatrixAABBTransform( vMax, vMin, pCur->m_matWorld );

			if( vMaxAABB.x < vMax.x )	vMaxAABB.x = vMax.x;
			if( vMaxAABB.y < vMax.y )	vMaxAABB.y = vMax.y;
			if( vMaxAABB.z < vMax.z )	vMaxAABB.z = vMax.z;

			if( vMinAABB.x > vMin.x )	vMinAABB.x = vMin.x;
			if( vMinAABB.y > vMin.y )	vMinAABB.y = vMin.y;
			if( vMinAABB.z > vMin.z )	vMinAABB.z = vMin.z;


			pCur = pCur->m_pTreeNext;
		}

		vMaxAABB += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
		vMinAABB -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

		pTree->m_pRightChild->m_vMax = vMaxAABB;
		pTree->m_pRightChild->m_vMin = vMinAABB;

		MakeTreeChild( pTree->m_pRightChild );
	}

	void MakeTree ( PDXPIECENODE& pTree, DxPiece* pPiece )
	{
		D3DXVECTOR3 vMax;
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMaxAABB = D3DXVECTOR3 ( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		D3DXVECTOR3 vMinAABB = D3DXVECTOR3 ( FLT_MAX, FLT_MAX, FLT_MAX );

		D3DXVECTOR3 vPos;
		DxPiece* pCur = pPiece;
		while( pCur )
		{
			vMax = pCur->m_vMax;
			vMin = pCur->m_vMin;
			D3DXMatrixAABBTransform( vMax, vMin, pCur->m_matWorld );

			if( vMaxAABB.x < vMax.x )	vMaxAABB.x = vMax.x;
			if( vMaxAABB.y < vMax.y )	vMaxAABB.y = vMax.y;
			if( vMaxAABB.z < vMax.z )	vMaxAABB.z = vMax.z;

			if( vMinAABB.x > vMin.x )	vMinAABB.x = vMin.x;
			if( vMinAABB.y > vMin.y )	vMinAABB.y = vMin.y;
			if( vMinAABB.z > vMin.z )	vMinAABB.z = vMin.z;

			pCur->m_pTreeNext = pCur->m_pNext;	// Tree에서 작업을 위하여..
			pCur = pCur->m_pTreeNext;
		}

		// Note : 충돌을 위해서
		vMaxAABB += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
		vMinAABB -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

		pTree = new DxPieceNode;
		pTree->m_vMax = vMaxAABB;
		pTree->m_vMin = vMinAABB;
		pTree->m_pPiece = pPiece;

		MakeTreeChild( pTree );

		pPiece = NULL;
	}


	void InsertQuickSort( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceNode* pTree, DxPieceQuickSort* pSort, DxPiece* pPiecePick,
		const CLIPVOLUME &sCV, const D3DXVECTOR3& vFromPt, const D3DXVECTOR3& vLookatPt, const float& fLength )
	{
		if ( !COLLISION::IsCollisionVolume( sCV, pTree->m_vMax, pTree->m_vMin ) )	return;

		if( pTree->m_pPiece )
		{
			pSort->InsertData( pTree->m_pPiece, pPiecePick, vFromPt, vLookatPt, fLength );
		}

		if( pTree->m_pLeftChild )	InsertQuickSort( pd3dDevice, pTree->m_pLeftChild, pSort, pPiecePick, sCV, vFromPt, vLookatPt, fLength );
		if( pTree->m_pRightChild )	InsertQuickSort( pd3dDevice, pTree->m_pRightChild, pSort, pPiecePick, sCV, vFromPt, vLookatPt, fLength );
	}

	void CollisionLine( DxPieceNode* pTree, const CLIPVOLUME &sCV, const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision, BOOL& bColl )
	{
		if ( !COLLISION::IsCollisionVolume( sCV, pTree->m_vMax, pTree->m_vMin ) )	return;

		DxPiece* pCur = pTree->m_pPiece;
		while( pCur )
		{
			if( pCur->IsCollisionLine( vStart, vEnd, vCollision ) )	bColl = TRUE;
			pCur = pCur->m_pTreeNext;
		}

		if( pTree->m_pLeftChild )	CollisionLine( pTree->m_pLeftChild, sCV, vStart, vEnd, vCollision, bColl );
		if( pTree->m_pRightChild )	CollisionLine( pTree->m_pRightChild, sCV, vStart, vEnd, vCollision, bColl );
	}

	void Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceNode* pTree, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
	{
		if ( !COLLISION::IsCollisionVolume( sCV, pTree->m_vMax, pTree->m_vMin ) )	return;

		if( pTree->m_pPiece )
		{
			pTree->m_pPiece->Render_Reflect( pd3dDevice, sCV, material );
		}

		if( pTree->m_pLeftChild )	Render_Reflect( pd3dDevice, pTree->m_pLeftChild, sCV, material );
		if( pTree->m_pRightChild )	Render_Reflect( pd3dDevice, pTree->m_pRightChild, sCV, material );
	}

	void Load_OLD( LPDIRECT3DDEVICEQ pd3dDevice, PDXPIECENODE& pNode, CSerialFile& SFile, DxPieceEdit* pSrc )
	{
		BOOL bExist = FALSE;
		SFile >> bExist;
		if( !bExist )	return;

		pNode = new DxPieceNode;
		pNode->Load_OLD( pd3dDevice, SFile, pSrc );
	}

	void Save( DxPieceNode* pNode, CSerialFile& SFile )
	{
		BOOL bExist = FALSE;
		if( pNode )
		{
			bExist = TRUE;
			SFile << bExist;
		}
		else
		{
			bExist = FALSE;
			SFile << bExist;

			return;
		}

		pNode->Save( SFile );
	}

	void Load_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, PDXPIECENODE& pNode, CSerialFile& SFile, DxPieceEdit* pSrc )
	{
		BOOL bExist = FALSE;
		SFile >> bExist;
		if( !bExist )	return;

		pNode = new DxPieceNode;
		pNode->Load_STABLE( pd3dDevice, SFile, pSrc );
	}
}
