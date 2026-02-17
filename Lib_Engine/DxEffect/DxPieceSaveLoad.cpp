#include "stdafx.h"
#include "./DxPiece.h"

#include "../Meshs/DxFrameMesh.h"
#include "./DxPieceContainer.h"
#include "./DxPieceEff.h"
#include "./DxPieceTexEff.h"
#include "./DxPieceNode.h"
#include "../DxCommon/TextureManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD DxPiece::VERSION_PIECE	= 0x0200;

void DxPiece::Load_OLD( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxPieceEdit* pSrc )
{
	SFile >> m_vMax;
	SFile >> m_vMin;

	SFile.ReadBuffer( &m_matWorld, sizeof(D3DXMATRIX) );

	// Note : 원본이 없을 경우 대비
	if( !pSrc )
	{
		m_vStaticMax = m_vMax;
		m_vStaticMin = m_vMin;

		// Note : 자신에 맞도록 StaticAABB와 Center, Length를 바꾼다.
		D3DXMatrixAABBTransform( m_vStaticMax, m_vStaticMin, m_matWorld );
		m_vCenter = (m_vStaticMax+m_vStaticMin)*0.5f;
		m_fLengthOrig = m_fLength;
		m_fLength = m_fLength*m_fLength;

		// Note : 자신과 맞닿아 있는 빛을 체크를 한다.
		CheckPointLight();

		return;
	}

	// Note : 얻어온다.
	m_vStaticMax = pSrc->GetStaticMax();
	m_vStaticMin = pSrc->GetStaticMin();
	m_fLength = pSrc->GetLength();

	// Note : 자신에 맞도록 StaticAABB와 Center, Length를 바꾼다.
	D3DXMatrixAABBTransform( m_vStaticMax, m_vStaticMin, m_matWorld );
	m_vCenter = (m_vStaticMax+m_vStaticMin)*0.5f;
	m_fLengthOrig = m_fLength;
	m_fLength = m_fLength*m_fLength;

	// Note : 자신과 맞닿아 있는 빛을 체크를 한다.
	CheckPointLight();

	// Note : 포인터를 얻어옴.
	m_pStaticAniFrame = pSrc->m_pStaticAniFrame;

	m_pPieceEff = new DxPieceEff;
	m_pPieceEff->Clone( pd3dDevice, pSrc->m_pPieceEff );

	m_pAniTexEff = new DxPieceTexEff;
	m_pAniTexEff->Clone( pd3dDevice, pSrc->m_pAniTexEff );
}

void DxPiece::Save( CSerialFile& SFile )
{
	SFile << (DWORD)VERSION_PIECE;

	SFile.BeginBlock(EMBLOCK_04);
	{
		SFile << m_vMax;		// Maybe It is unnecessary < 필요 없지 싶다. >
		SFile << m_vMin;		// 필요 없지 싶다. DxPieceEdit을 로드시 부르기 때문에..

		SFile.WriteBuffer( &m_matWorld, sizeof(D3DXMATRIX) );

		SFile << m_strPiece;
		SFile << m_dwLightMapOffset1;
		SFile << m_dwLightMapOffset2;
		SFile << m_strLightMapDay;
		SFile << m_strLightMapDS1;
		SFile << unknown_var5;
		SFile << unknown_var6;
		SFile << m_fLightMapUVX;
		SFile << m_fLightMapUVY;
		SFile << m_strLightMapDS2;
		SFile << m_strLightMapShadow;
		SFile << m_fDrawDistance1;
		SFile << m_fDrawDistance2;
		SFile << unknown_var13;
		SFile << unknown_var14;
		SFile << unknown_var15;
		SFile << m_bLightMapOffSet;

	}
	SFile.EndBlock(EMBLOCK_04);
}

void DxPiece::Load_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxPieceEdit* pSrc )
{
	DWORD dwVER(0);
	DWORD dwSIZE(0);

	SFile >> dwVER;
	SFile >> dwSIZE;

	if ( dwVER == VERSION_PIECE )
	{
		SFile >> m_vMax;
		SFile >> m_vMin;

		SFile.ReadBuffer( &m_matWorld, sizeof(D3DXMATRIX) );

		SFile >> m_strPiece;
		SFile >> m_dwLightMapOffset1;
		SFile >> m_dwLightMapOffset2;
		SFile >> m_strLightMapDay;
		SFile >> m_strLightMapDS1;
		SFile >> unknown_var5;
		SFile >> unknown_var6;
		SFile >> m_fLightMapUVX;
		SFile >> m_fLightMapUVY;
		SFile >> m_strLightMapDS2;
		SFile >> m_strLightMapShadow;
		SFile >> m_fDrawDistance1;
		SFile >> m_fDrawDistance2;
		SFile >> unknown_var13;
		SFile >> unknown_var14;
		SFile >> unknown_var15;
		SFile >> m_bLightMapOffSet;
	}
	else if ( dwVER == 0x0110 )
	{
		SFile >> m_strPiece;

		SFile >> m_vMax;
		SFile >> m_vMin;

		SFile.ReadBuffer( &m_matWorld, sizeof(D3DXMATRIX) );

		//lightmaps data
		SFile >> m_dwLightMapOffset1;
		SFile >> m_dwLightMapOffset2;
		SFile >> m_fLightMapUVX;
		SFile >> m_fLightMapUVY;
		SFile >> m_strLightMapDay;
		SFile >> m_strLightMapDS1;
		SFile >> m_strLightMapDS2;
		SFile >> m_strLightMapShadow;

		DWORD _dwVER(0);
		DWORD _dwSIZE(0);
		SFile >> _dwVER;
		SFile >> _dwSIZE;
		DWORD dwCur = SFile.GetfTell ();
		SFile.SetOffSet ( dwCur+_dwSIZE );

		SFile >> m_fDrawDistance1;
		SFile >> m_fDrawDistance2;
		SFile >> unknown_var13;
		SFile >> unknown_var14;
		SFile >> unknown_var15;
	}
	else if ( dwVER == 0x0109 )
	{
		SFile >> m_strPiece;

		SFile >> m_vMax;
		SFile >> m_vMin;

		SFile.ReadBuffer( &m_matWorld, sizeof(D3DXMATRIX) );

		//lightmaps data
		SFile >> m_dwLightMapOffset1;
		SFile >> m_dwLightMapOffset2;
		SFile >> m_fLightMapUVX;
		SFile >> m_fLightMapUVY;
		SFile >> m_strLightMapDay;
		SFile >> m_strLightMapDS1;
		SFile >> m_strLightMapDS2;
		SFile >> m_strLightMapShadow;

		DWORD _dwVER(0);
		DWORD _dwSIZE(0);
		SFile >> _dwVER;
		SFile >> _dwSIZE;
		DWORD dwCur = SFile.GetfTell ();
		SFile.SetOffSet ( dwCur+_dwSIZE );

		SFile >> m_fDrawDistance1;
		SFile >> m_fDrawDistance2;
		SFile >> unknown_var13;
		SFile >> unknown_var14; 
	}
	else if ( dwVER == 0x0105 )
	{
		SFile >> m_strPiece;

		SFile >> m_vMax;
		SFile >> m_vMin;

		SFile.ReadBuffer( &m_matWorld, sizeof(D3DXMATRIX) );

		//lightmaps data
		SFile >> m_dwLightMapOffset1;
		SFile >> m_dwLightMapOffset2;
		SFile >> m_fLightMapUVX;
		SFile >> m_fLightMapUVY;
		SFile >> m_strLightMapDay;
		SFile >> m_strLightMapDS1;

		DWORD _dwVER(0);
		DWORD _dwSIZE(0);
		SFile >> _dwVER;
		SFile >> _dwSIZE;
		DWORD dwCur = SFile.GetfTell ();
		SFile.SetOffSet ( dwCur+_dwSIZE );

		SFile >> m_fDrawDistance1;
		SFile >> m_fDrawDistance2;
	}
	else if ( dwVER == 0x0104 )
	{
		SFile >> m_strPiece;

		SFile >> m_vMax;
		SFile >> m_vMin;

		SFile.ReadBuffer( &m_matWorld, sizeof(D3DXMATRIX) );

		//lightmaps data
		SFile >> m_dwLightMapOffset1;
		SFile >> m_dwLightMapOffset2;
		SFile >> m_fLightMapUVX;
		SFile >> m_fLightMapUVY;
		SFile >> m_strLightMapDay;
		SFile >> m_strLightMapDS1;

		char sz[312];
		SFile.ReadBuffer( &sz, 312 );

		SFile >> m_fDrawDistance1;
		SFile >> m_fDrawDistance2;
	}
	else if ( dwVER == 0x0100 )
	{
		SFile >> m_vMax;
		SFile >> m_vMin;

		SFile.ReadBuffer( &m_matWorld, sizeof(D3DXMATRIX) );

		//lightmaps data
		SFile >> m_dwLightMapOffset1;
		SFile >> m_dwLightMapOffset2;
		SFile >> m_strLightMapDay;
		SFile >> m_strLightMapDS1;
		SFile >> unknown_var5;
		SFile >> unknown_var6;
	}
	else
	{
		CDebugSet::ErrorVersion( "DxPiece::Load_STABLE", SFile.GetFileName(), dwVER );
		DWORD dwCur = SFile.GetfTell ();
		SFile.SetOffSet ( dwCur+dwSIZE );
		return ;
	}

	m_fLightMapOffsetX = float(LOWORD(m_dwLightMapOffset1))/float(LOWORD(m_dwLightMapOffset2));
	m_fLightMapOffsetY = float(HIWORD(m_dwLightMapOffset1))/float(HIWORD(m_dwLightMapOffset2));

	if ( dwVER >= 0x0100 )
	{
		if ( !pSrc && m_strPiece.size() )
			pSrc = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, m_strPiece.c_str() );
	}
	
	// Note : 원본이 없을 경우 대비
	if( !pSrc )
	{
		m_vStaticMax = m_vMax;
		m_vStaticMin = m_vMin;

		// Note : 자신에 맞도록 StaticAABB와 Center, Length를 바꾼다.
		D3DXMatrixAABBTransform( m_vStaticMax, m_vStaticMin, m_matWorld );
		m_vCenter = (m_vStaticMax+m_vStaticMin)*0.5f;
		D3DXVECTOR3 vVector = m_vStaticMax - m_vCenter;
		float fLength = D3DXVec3Length( &vVector );
		m_fLengthOrig = fLength;
		m_fLength = fLength*fLength;

		// Note : 자신과 맞닿아 있는 빛을 체크를 한다.
		CheckPointLight();

		//create AABB for rendering collision
		m_vPos.x = m_matWorld._41;
		m_vPos.y = m_matWorld._42;
		m_vPos.z = m_matWorld._43;

		m_vMaxCol = m_vPos + m_vMax;
		m_vMinCol = m_vPos + m_vMin;

		return;
	}

	m_vMax = pSrc->GetAABBMax();
	m_vMin = pSrc->GetAABBMin();

	// Note : 얻어온다.
	m_vStaticMax = pSrc->GetStaticMax();
	m_vStaticMin = pSrc->GetStaticMin();
	//m_fLength = pSrc->GetLength();

	// Note : 자신에 맞도록 StaticAABB와 Center, Length를 바꾼다.
	D3DXMatrixAABBTransform( m_vStaticMax, m_vStaticMin, m_matWorld );
	m_vCenter = (m_vStaticMax+m_vStaticMin)*0.5f;
	D3DXVECTOR3 vVector = m_vStaticMax - m_vCenter;
	float fLength = D3DXVec3Length( &vVector );
	m_fLengthOrig = fLength;
	m_fLength = fLength*fLength;

	// Note : 자신과 맞닿아 있는 빛을 체크를 한다.
	CheckPointLight();

	// Note : 포인터를 얻어옴.
	m_pStaticAniFrame = pSrc->m_pStaticAniFrame;

	m_pPieceEff = new DxPieceEff;
	m_pPieceEff->Clone( pd3dDevice, pSrc->m_pPieceEff );

	m_pAniTexEff = new DxPieceTexEff;
	m_pAniTexEff->Clone( pd3dDevice, pSrc->m_pAniTexEff );

	//create AABB for rendering collision
	m_vPos.x = m_matWorld._41;
	m_vPos.y = m_matWorld._42;
	m_vPos.z = m_matWorld._43;

	m_vMaxCol = m_vPos + m_vMax;
	m_vMinCol = m_vPos + m_vMin;

	LoadLightMaps( pd3dDevice );
}

