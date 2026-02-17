#pragma once

#include "../Common/SerialFile.h"
#include "../DxCommon/DxCustomTypes.h"
#include "./DxPieceNode.h"
#include "../DxCommon/MaterialShaderParam.h"

class DxPiece;
class DxPieceQuickSort;

// -----------------------------------------------------------------------------------------------------------------------------------------
//											N	S		P	I	E	C	E		T	R	E	E
// -----------------------------------------------------------------------------------------------------------------------------------------
namespace NSPIECETREE
{
	void MakeTree( PDXPIECENODE& pTree, DxPiece* pPiece );	// Tree 를 만든다. pPiece 는 Tree의 Leaf 속으로 흡수 된다. List 사용불가.
	void InsertQuickSort( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceNode* pTree, DxPieceQuickSort* pSort, DxPiece* pPiecePick, 
		const CLIPVOLUME &sCV, const D3DXVECTOR3& vFromPt, const D3DXVECTOR3& vLookatPt, const float& fLength );
	void CollisionLine( DxPieceNode* pTree, const CLIPVOLUME &sCV, const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision, BOOL& bColl );
	void Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceNode* pTree, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );

	void Save( DxPieceNode* pNode, CSerialFile& SFile );
	void Load_OLD( LPDIRECT3DDEVICEQ pd3dDevice, PDXPIECENODE& pNode, CSerialFile& SFile, DxPieceEdit* pSrc );
	void Load_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, PDXPIECENODE& pNode, CSerialFile& SFile, DxPieceEdit* pSrc );
};

