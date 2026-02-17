/*!
 * \file DxMultiStaticMesh.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma once

#include "./DxStaticMeshThread.h"
#include "./DxStaticMesh.h"
#include "../Meshs/DxFrameMesh.h"
#include "../Common/SerialFile.h"

typedef std::vector<DxStaticMesh*>		VECSTATICMESH;

class DxMultiStaticMesh
{
protected:
	enum
	{
		VERSION_MULTI_STATIC_MESH	= 0x0204,

		ENCODE_VER_BYTE_CRYPT_2021_WLD2_V1	= 0x0204,


		VERSION_ENCODE_4	= 0x0203,
		VERSION_ENCODE_3	= 0x0202,
		VERSION_ENCODE_2	= 0x0201,
		VERSION_ENCODE		= 0x0200,
	};

protected:
	CSerialFile m_SFile;

private:
	DxStaticMesh_THREAD	m_sThread;			
	LOADINGDATALIST		m_listLoadingData;
	CRITICAL_SECTION	m_CSLockLoading;

public:
	VECSTATICMESH		m_vecStaticMesh;

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

	void Render_THREAD_( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV );
	void Render_THREAD_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV );
	void Render_THREAD_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV );

public:
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, LPCSTR& szName, const BOOL bFrontColl );
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl );

	void MakeAABBOCTree( const LPDIRECT3DDEVICEQ pd3dDevice, DxFrameMesh* const pFrameMesh, const BOOL bProgress, BOOL bPiece );

public:
	void BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vPos  );
	void BaseLoad( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin  );

	//legacy loading start DxLandMan version 0x0109
	void Load_File_109( LPDIRECT3DDEVICEQ pd3dDevice, const char* szName );
	void Load_109( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

	void Load_File_120( LPDIRECT3DDEVICEQ pd3dDevice, const char* szName );
	void Load_120( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_120_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_120_200( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_120_201( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_120_202( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_120_203( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_120_204( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

	void Save_File( const char* szName );
	void Save( CSerialFile& SFile, BOOL bPiece );
	


public:
	void StartThread( LPDIRECT3DDEVICEQ pd3dDevice );
	void EndThread();

public:
	DxMultiStaticMesh();
	~DxMultiStaticMesh();
};

