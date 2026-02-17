/*!
 * \file DxShadowMesh.h
 *
 * \author Juver
 * \date May 2018
 *
 * 
 */
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./DxVertexFVF.h"
#include "./DxShadowMeshData.h"
#include "../Common/SerialFile.h"

class DxShadowMeshTree
{
public:	
	D3DXVECTOR3		m_vMax;
	D3DXVECTOR3		m_vMin;
	DWORD			m_dwVert;
	DWORD			m_dwFace;
	DWORD			m_dwFVF;
	int				m_nFVFSize;

	VERTEX_SHADOW*	m_pVertex;
	WORD*			m_pIndex;

	LPDIRECT3DVERTEXBUFFERQ	m_pVB;
	LPDIRECT3DINDEXBUFFERQ	m_pIB;
	
	DxShadowMeshTree *m_pNext;

public:
	void CleanUp();
	BOOL Create( LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwVert, DWORD dwFace );
	BOOL CopyMesh( LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh );
	void Render( LPDIRECT3DDEVICEQ pd3dDevice );
	void RenderTest( LPDIRECT3DDEVICEQ pd3dDevice );
	void ShadowMakeUV ( const D3DXMATRIX& matDirect );

public:
	void Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Save ( CSerialFile& SFile );
	
public:
	DxShadowMeshTree();
	~DxShadowMeshTree();
};


class DxShadowMesh
{
protected:
	enum
	{
		VERSION_SHADOW_MESH	= 0x0204,

		ENCODE_VER_BYTE_CRYPT_2021_WLD3_V1	= 0x0204,

		VERSION_ENCODE_4	= 0x0203,
		VERSION_ENCODE_3	= 0x0202,
		VERSION_ENCODE_2	= 0x0201,
		VERSION_ENCODE		= 0x0200,
	};

public:
	DxShadowMeshTree*	m_pHead;

public:
	void CleanUp();
	BOOL CopyMesh( LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh );
	void Render( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld );
	void RenderTest( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld );

public:
	void Load_File ( const LPDIRECT3DDEVICEQ pd3dDevice, const char* szName );
	void Save_File ( const char* szName );
	
	void Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Save ( CSerialFile& SFile );
	

public:
	DxShadowMesh();
	~DxShadowMesh();

};