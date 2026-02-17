/*!
 * \file DxMultiPieceAABB.h
 *
 * \author Juver
 * \date April 2018
 *
 * 
 */

#pragma once

#include "../Common/SerialFile.h"

#include "./DxPieceQuickSort.h"
#include "../DxCommon/DxCustomTypes.h"
#include "../DxCommon/MaterialShaderParam.h"

class DxPiece;
class DxFrameMesh;
class DxStaticMesh;
class DxMultiStaticMesh;
class DxMultiStaticMaterialMesh;


class DxMultiPieceAABB
{
private:
	static const DWORD VERSION_MULTI_PIECE_AABB;

protected:
	DxPiece*		m_pPieceHead;
	DWORD			m_dwPieceNUM;
	std::vector<DxPiece*>	m_vecPiece;	//temp render

public:
	void SetPiece( DxPiece* pPiece );

public:
	void FrameMove( const float fTime, const float fElapsedTime );
	void Render( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void RenderEff( LPDIRECT3DDEVICEQ pd3dDevices, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
		DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material );

	void Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );

	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision );

public:
	void Save( CSerialFile& SFile );
	void Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

public:
	DxMultiPieceAABB();
	~DxMultiPieceAABB();
};
