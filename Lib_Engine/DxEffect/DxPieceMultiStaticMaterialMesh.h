/*!
 * \file DxPieceMultiStaticMaterialMesh.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma once

#include "./DxStaticMaterialMeshThread.h"
#include "./DxStaticMaterialMesh.h"
#include "../Meshs/DxFrameMesh.h"
#include "../Common/SerialFile.h"
#include "../DxCommon/MaterialShaderParam.h"
#include "../DxCommon/DxShadowMeshData.h"

class DxSingleTexMaterialMesh;

typedef std::vector<DxStaticMaterialMesh*>		VEC_STATIC_MATERIAL_MESH;

class DxPieceMultiStaticMaterialMesh
{
protected:
	enum
	{
		VERSION_MULTI_STATIC_MATERIAL_MESH_MAIN	= 0x0100,
		VERSION_MULTI_STATIC_MATERIAL_MESH_SUB	= 0x0100,
	};


public:
	VEC_STATIC_MATERIAL_MESH		m_vecStaticMesh;

protected:
	D3DXVECTOR3		m_vMin;
	D3DXVECTOR3		m_vMax;

protected:
	void CleanUp();

public:
	D3DXVECTOR3 GetAABBMax ()	{ return m_vMax; }
	D3DXVECTOR3 GetAABBMin ()	{ return m_vMin; }
	void	CalculateAABB();

public:
	void FrameMove( const float fTime, const float fElapsedTime );

	void Render_PIECE_Normal( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_Blending( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_SpecularBlending( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_MaterialEffect( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );

public:
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, LPCSTR& szName, const BOOL bFrontColl );
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl );
	void UpdateLightMapUV( float fMultiX, float fMultiY );
	void CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh );

public:
	void Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Save( CSerialFile& SFile, BOOL bPiece );

public:
	DxSingleTexMaterialMesh*	FindMesh( std::string _strMeshName );
	
public:
	DxPieceMultiStaticMaterialMesh();
	~DxPieceMultiStaticMaterialMesh();
};

