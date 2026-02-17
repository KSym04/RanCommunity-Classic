/*!
 * \file DxMultiStaticMaterialMesh.h
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

typedef std::vector<DxStaticMaterialMesh*>		VEC_STATIC_MATERIAL_MESH;

class DxMultiStaticMaterialMesh
{
protected:
	enum
	{
		VERSION_MULTI_STATIC_MATERIAL_MESH	= 0x0204,

		ENCODE_VER_BYTE_CRYPT_2021_WLD2_V1	= 0x0204,

		VERSION_ENCODE_4	= 0x0203,
		VERSION_ENCODE_3	= 0x0202,
		VERSION_ENCODE_2	= 0x0201,
		VERSION_ENCODE		= 0x0200,
	};

protected:
	CSerialFile m_SFile;

private:
	DxStaticMaterialMesh_THREAD	m_sThread;			
	LOADINGDATALIST		m_listLoadingData;
	CRITICAL_SECTION	m_CSLockLoading;

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

	void Render_THREAD_Normal( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_Blending( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_SpecularBlending( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void Render_THREAD_MaterialEffect( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	
public:
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, LPCSTR& szName, const BOOL bFrontColl );
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl );
	void CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh );

public:
	void BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vPos  );
	void BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin  );

	void Load_File( LPDIRECT3DDEVICEQ pd3dDevice, const char* szName );
	void Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_200( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_201( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_202( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_203( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_204( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

	void Save_File( const char* szName );
	void Save( CSerialFile& SFile, BOOL bPiece );
	
public:
	void StartThread( LPDIRECT3DDEVICEQ pd3dDevice );
	void EndThread();

public:
	DxMultiStaticMaterialMesh();
	~DxMultiStaticMaterialMesh();
};

