// DxStaticMaterialMeshCreate.h: interface for the CCollision class.
//
//	class DxStaticMaterialMesh : FrameMesh 데이타를 넣으면 AABB 트리 적용하고 Octree로 세세하고 나눈다.
//
//	Name : Sung-Hwan Han
//	Begin :2004/10/21
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <set>
#include <map>

#include "../Common/SerialFile.h"

#include "../Meshs/DxFrameMesh.h"
#include "../DxCommon/NsOCTree.h"
#include "../DxCommon/DxSingleTexMaterialMesh.h"
#include "../DxCommon/DxShadowMeshData.h"

class DxTextureEffMan;

class DxStaticMaterialMesh
{
protected:
	enum
	{
		VERSION_STATIC_MATERIAL_MESH = 0x0200,
	};

public:
	MAPSINGLETEXMATERIALMESH	m_mapMesh;
	DxSingleTexMaterialMesh*	m_pList;
	OBJAABBNode*				m_pMeshTree;

	VECSINGLETEXMATERIALMESH	m_vecMeshNORMAL;
	VECSINGLETEXMATERIALMESH	m_vecMeshALPHA;
	VECSINGLETEXMATERIALMESH	m_vecMeshSOFTALPHA00;
	VECSINGLETEXMATERIALMESH	m_vecMeshSOFTALPHA01;
	VECSINGLETEXMATERIALMESH	m_vecMeshSOFTALPHA02;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND00;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND01;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND02;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND10;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND11;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND12;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND20;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND21;
	VECSINGLETEXMATERIALMESH	m_vecMeshBLEND22;
	VECSINGLETEXMATERIALMESH	m_vecMeshSPECULAR_BLEND;
	VECSINGLETEXMATERIALMESH	m_vecMeshMATERIAL_EFFECT;


protected:
	D3DXVECTOR3		m_vMin;				// 최소값
	D3DXVECTOR3		m_vMax;				// 최대값
	DWORD			unknown_var1;
	std::string		m_strLightMapDay;
	std::string		m_strLightMapDS1;
	std::string		m_strLightMapDS2;
	std::string		m_strLightMapShadow;

public:
	LPDIRECT3DTEXTUREQ	m_pTextureLightMapDay;

	D3DXMATRIX*		m_pmatPieceFrame;

private:
	enum EMRENDERTYPE
	{
		EMRT_OCTREE = 0,
		EMRT_PIECE = 1,
	};

	EMRENDERTYPE	m_emRenderType;

public:
	D3DXVECTOR3 GetAABBMax ()	{ return m_vMax; }
	D3DXVECTOR3 GetAABBMin ()	{ return m_vMin; }

protected:
	void SetAABBMaxMin ();

public:
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, 
						LPCSTR& szName, const BOOL bFrontColl );
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl );

public:
	void FrameMove( const float fTime, const float fElapsedTime );

	// Thread Loading Render
	void Render_THREAD_Normal( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_Blending( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_SpecularBlending( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_MaterialEffect( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	
	// Piece File Render
	void Render_PIECE_Normal( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_Blending( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_SpecularBlending( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	void Render_PIECE_MaterialEffect( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material, const D3DXMATRIX& matWorld );
	

protected:
	void CleanUp();

public:
	void BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vPos, CSerialFile* const SFile );
	void BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin, CSerialFile* const SFile );

	// Thread Loading 을 의미함.
	void Save_File( const char* szName );
	void Load_File( LPDIRECT3DDEVICEQ pd3dDevice, const char* szName );

	// Dynamic Loading	|| General <SFile==NULL> || Piece File			Loading 을 의미함.
	void Save( CSerialFile& SFile, BOOL bPiece );
	void Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, const BOOL bPiece );

protected:
	void SingleTexMesh_Save( MAPSINGLETEXMATERIALMESH& mapMesh, CSerialFile& SFile, BOOL bPiece );
	void SingleTexMesh_Load_Insert( LPDIRECT3DDEVICEQ pd3dDevice, MAPSINGLETEXMATERIALMESH& mapMesh, CSerialFile& SFile, BOOL bDynamicLoad, BOOL bPiece );

	void Load_200( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece );
	void Load_105( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece );
	void Load_104( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece );
	void Load_103( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece );
	void Load_102( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece );
	void Load_101( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece );
	void Load_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, BOOL bDynamicLoad, const BOOL bPiece );
	
public:
	DxSingleTexMaterialMesh*	FindMesh( std::string _strMeshName );
	void LoadLightMaps( LPDIRECT3DDEVICEQ pd3dDevice );
	void SortMaterialContainer( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bPiece );
	void UpdateLightMapUV( float fMultiX, float fMultiY );
	void CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh );

public:
	DxStaticMaterialMesh();
	~DxStaticMaterialMesh();
};

namespace NSSTATICMATERIALMESH
{
	void ConvertMAPtoLIST ( MAPSINGLETEXMATERIALMESH& mapData, PDXSINGLETEXMATERIALMESH& pList );
};