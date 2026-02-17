/*!
 * \file DxPieceAnimation.h
 *
 * \author Juver
 * \date April 2018
 *
 * 
 */
#ifndef	__DX_PIECE_ANIMATION_INFO__
#define	__DX_PIECE_ANIMATION_INFO__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct DxAnimation;
class	CSerialFile;
class	DxPieceMultiStaticMaterialMesh;

class DxPieceAnimation
{
public:
	const static DWORD	DXPIECE_ANIMATION_VERSION;

public:
	D3DXVECTOR3		m_vMax;
	D3DXVECTOR3		m_vMin;
	D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_matResult;

	DxAnimation*	m_pDxAnimation;
	DWORD			m_dwVar;

public:
	DxPieceAnimation*	m_pLeft;	
	DxPieceAnimation*	m_pRight;	

public:
	void FrameMove ( float fCurTime, const D3DXMATRIX& mat );

public:
	void SaveFile ( CSerialFile &SFile );
	void LoadFile ( CSerialFile &SFile, LPDIRECT3DDEVICEQ pd3dDevice, DxPieceMultiStaticMaterialMesh* pMesh );

private:
	void Save( CSerialFile &SFile );
	void Load_0100 ( CSerialFile &SFile, LPDIRECT3DDEVICEQ pd3dDevice, DxPieceMultiStaticMaterialMesh* pMesh );

public:
	DxPieceAnimation ();
	~DxPieceAnimation();
};

typedef	DxPieceAnimation*	LPDxPieceAnimation;

#endif	//	__DX_PIECE_ANIMATION_INFO__