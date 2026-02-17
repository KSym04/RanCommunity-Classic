#pragma once

#include "../Common/SerialFile.h"

#include "./DxPieceQuickSort.h"
#include "../DxCommon/DxCustomTypes.h"

class DxPiece;
class DxFrameMesh;
class DxStaticMesh;
class DxMultiStaticMesh;
class DxMultiStaticMaterialMesh;
struct DxPieceNode;


// -----------------------------------------------------------------------------------------------------------------------------------------
//											D	x		P	i	e	c	e		A	A	B	B
// -----------------------------------------------------------------------------------------------------------------------------------------
class DxPieceAABB
{
private:
	static const DWORD VERSION_PIECE_AABB;

protected:
	DxPieceNode*		m_pPieceAABBTree;	// AABB Tree
	DxPiece*			m_pPieceHead;		// List
	DxPieceQuickSort	m_sQuickSort;		// Quick Sort

	DWORD			m_dwPieceNUM;		// Piece °¹¼ö..

public:
	void SetPiece( DxPiece* pPiece );
	void MakeTree();

public:
	void FrameMove( const float fTime, const float fElapsedTime );
	void Render( LPDIRECT3DDEVICEQ pd3dDevice, DxPiece* pPiecePick, const CLIPVOLUME &sCV, const float fTime, const float fElapsedTime, SMATERIAL_SETTING& material );
	void RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void RenderEff( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
		DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material );

	void RenderPickAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
		DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material );

	void Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );

	// Edit
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision );

public:
	void Save( CSerialFile& SFile );
	void Load_OLD( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const char* pPieceName );
	void Load_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const char* pPieceName );

public:
	void CloneData( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceAABB* pSrc );
	void ClearOnlyTree();
	void ClearTree();

public:
	DxPieceAABB();
	~DxPieceAABB();
};
