/*!
 * \file OBJOCTree.h
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

struct DxAABBNode;

struct OBJOCTree
{
	union
	{
		struct { D3DXVECTOR3 vMax; };
		struct { float m_fMaxX, m_fMaxY, m_fMaxZ; };
	};
	union
	{
		struct { D3DXVECTOR3 vMin; };
		struct { float m_fMinX, m_fMinY, m_fMinZ; };
	};

	struct DIRECTPOINTCOLOR
	{
		WORD wAlpha;
		WORD wDayR;
		WORD wDayG;
		WORD wDayB;
		WORD wNightR;
		WORD wNightG;
		WORD wNightB;
		WORD wTemp;
	};

	struct NORMALCOLOR
	{
		D3DXVECTOR3 vNor;
		D3DXCOLOR	cColor;

		NORMALCOLOR() :
		vNor(0.f,0.f,0.f),
			cColor(1.f,1.f,1.f,1.f)
		{
		}
	};

	OBJOCTree *pLeftChild;
	OBJOCTree *pRightChild;

	BYTE*			pVertex;
	DWORD			dwFace;
	LPD3DXMESH		pOcMesh;
	DxAABBNode*		pCollsionTREE;
	NORMALCOLOR*	pNormalColor;	

	DWORD					m_dwFVF;
	UINT					m_nFVFSize;
	DWORD					m_dwVert;
	DWORD					m_dwFace;

	VERTEXNORCOLORTEX_850*	m_pVertSrc;
	DIRECTPOINTCOLOR*		m_pColor;
	
	LPDIRECT3DVERTEXBUFFERQ	m_pVB;
	LPDIRECT3DINDEXBUFFERQ	m_pIB;
	CRITICAL_SECTION		m_pCSLockVB;

	BOOL					m_bCheck;
	BOOL					m_bLoad;
	

	virtual DWORD GetTypeID() { return 0xffffffff; }

	void CleanUpBase();
	virtual void DrawEff( const LPDIRECT3DDEVICEQ pd3dDevice );

	virtual void CreateBASE( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwVert, const DWORD dwFaces ) {}
	virtual void CreateVertSrc( BYTE* pByte, const DWORD dwFVF, const DWORD dwVert ) {}
	virtual void CreateNormalColor( BYTE* pByte, const DWORD dwFVF, const DWORD dwVert ) {}
	virtual void CopyVertSrcToVB( BYTE* pByte, VERTEXNORCOLORTEX_850* pVertSrc, LPDIRECT3DVERTEXBUFFERQ	pVB, const DWORD dwVert ) {}
	virtual void CopyMeshToIB( WORD* pSrc, LPDIRECT3DINDEXBUFFERQ pIB, const DWORD dwFaces ) {}
	virtual void CovertPieceSetting( LPDIRECT3DDEVICEQ pd3dDevice ) {}
	        
	virtual void InsertColorList( LOADINGDATALIST &listColorData ) {}
	virtual void FrameMoveCOLOR() {}
	virtual void UpdateLightMapUV( float fMultiX, float fMultiY ) {}
	
	virtual BOOL IsLoad() { return FALSE; }

	virtual void Save( CSerialFile& SFile, BOOL bPiece ) {}
	virtual void Load( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece ) {}
	virtual void Load_OLD( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece ) {}
	virtual void ThreadLoad( LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading ){}
	virtual BOOL DynamicLoad( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bThread ){ return TRUE; }

	OBJOCTree();
	virtual ~OBJOCTree();
};
typedef OBJOCTree* POBJOCTREE;