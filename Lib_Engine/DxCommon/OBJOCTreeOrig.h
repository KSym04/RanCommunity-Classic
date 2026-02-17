/*!
 * \file OBJOCTreeOrig.h
 *
 * \author Juver
 * \date April 2018
 *
 * 
 */
#pragma once


#include "./DxVertexFVF.h"
#include "./DxCustomTypes.h"
#include "./LandTypeDef.h"
#include "../Common/SerialFile.h"
#include "./OBJOCTree.h"

struct DxAABBNode;


struct OBJOCTreeOrig : public OBJOCTree
{
	const static DWORD	TYPEID;
	const static DWORD	VERSION;
	const static DWORD	VERSION_DYNAMIC;

	EM_LAND_TYPE	m_emLandType;
	

	DWORD		m_dwFileCur;

	virtual DWORD GetTypeID() { return TYPEID; }

	void CleanUpDerived();
	virtual void CreateBASE( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwVert, const DWORD dwFaces );
	virtual void CreateVertSrc( BYTE* pByte, const DWORD dwFVF, const DWORD dwVert );
	virtual void CreateNormalColor( BYTE* pByte, const DWORD dwFVF, const DWORD dwVert );
	virtual void CopyVertSrcToVB( BYTE* pByte, VERTEXNORCOLORTEX_850* pVertSrc, LPDIRECT3DVERTEXBUFFERQ	pVB, const DWORD dwVert );
	virtual void CopyMeshToIB( WORD* pSrc, LPDIRECT3DINDEXBUFFERQ pIB, const DWORD dwFaces );
	virtual void CovertPieceSetting( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual void UpdateLightMapUV( float fMultiX, float fMultiY );
	virtual void InsertColorList( LOADINGDATALIST &listColorData );
	virtual void FrameMoveCOLOR();

	void ComputeNormalColor( const DWORD dwVert );
	void ComputeNormalColorDAYNIGHT_OFF( const DWORD dwVert );
	void ComputeCOLOR( const DWORD& dwStart, const DWORD& dwEnd );
	
	virtual BOOL IsLoad();

	virtual void Save( CSerialFile& SFile, BOOL bPiece );
	virtual void Load( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece );
	virtual void Load_OLD( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece );
	virtual void ThreadLoad( LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading );
	virtual BOOL DynamicLoad( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bThread );

	BOOL OctreeLoad( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bThread );
	BOOL OctreeLoad_101( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bThread );
	BOOL OctreeLoad_100( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bThread );
	BOOL OctreeLoadOLD( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DWORD dwFVF, const BOOL bThread );

	void Load_VER100( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	

	OBJOCTreeOrig();
	virtual ~OBJOCTreeOrig();
};
typedef OBJOCTreeOrig* POBJOCTREE_ORIG;