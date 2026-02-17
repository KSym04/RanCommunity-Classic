#include "stdafx.h"
#include "./DxPiece.h"

#include "../DxCommon/Collision.h"
#include "../DxCommon/DxLightMan.h"
#include "../DxCommon/DxEnvironment.h"
//
#include "./DxStaticMesh.h"
#include "./DxMultiStaticMesh.h"
#include "./DxMultiStaticMaterialMesh.h"
#include "./DxPieceEff.h"
#include "./DxPieceStaticAni.h"
#include "./DxPieceContainer.h"
#include "./DxPieceTexEff.h"

#include "../DxCommon/TextureManager.h"


//
extern BOOL g_bPIECE_USE_COLLISION;
extern BOOL g_bPIECE_USE_DISTANCE;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// -----------------------------------------------------------------------------------------------------------------------------------------
//													D	x		P	i	e	c	e
//
//		보면 PDXEFFECTBASE 이	DxStaticAniFrame과 DxPieceEff에 둘다 들어 있다.. 하지만 2개의 동작은 틀리다.
//		DxStaticAniFrame는 같은 것을 써서 동작은 같지만 부하를 덜 먹고,
//		DxPieceEff는 틀린것을 써서 동작은 틀리지만 부하는 많이 먹는다.
// -----------------------------------------------------------------------------------------------------------------------------------------

DxPiece::DxPiece() :
	m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX),
	m_vMin(FLT_MAX,FLT_MAX,FLT_MAX),
	m_vStaticMax(FLT_MAX,FLT_MAX,FLT_MAX),
	m_vStaticMin(FLT_MAX,FLT_MAX,FLT_MAX),
	m_vMaxCol(FLT_MAX,FLT_MAX,FLT_MAX),
	m_vMinCol(FLT_MAX,FLT_MAX,FLT_MAX),
	m_vPos(0.f,0.f,0.f),
	m_vCenter(0.f,0.f,0.f),
	m_fLength(1.f),
	m_fLengthOrig(1.f),
	m_fAlpha(1.f),
	m_bAplhaBlend(FALSE),
	m_fTime(0.f),
	m_fAniTime(0.f),
	m_ElapsedTime(0.f),
	m_pStaticAniFrame(NULL),
	m_pPieceEff(NULL),
	m_pAniTexEff(NULL),
	m_pNext(NULL),
	m_pTreeNext(NULL),
	m_pSortNext(NULL),

	m_strPiece(""),
	m_dwLightMapOffset1(0),
	m_dwLightMapOffset2(0),
	m_strLightMapDay(""),
	m_strLightMapDS1(""),
	unknown_var5(""),
	unknown_var6(""),
	m_fLightMapUVX(1.0f),
	m_fLightMapUVY(1.0f),
	m_strLightMapDS2(""),
	m_strLightMapShadow(""),
	m_fDrawDistance1(500.f),
	m_fDrawDistance2(1000.f),
	unknown_var13(0),
	unknown_var14(0),
	unknown_var15(0),

	m_pTextureLightMapDay(NULL),
	
	m_fLightMapOffsetX(0.0f),
	m_fLightMapOffsetY(0.0f),
	m_dwIndex(0),
	m_bLightMapOffSet(FALSE)
{
	m_fAniTime = RANDOM_POS * 100.f * UNITANIKEY_PERSEC;

	D3DXMatrixIdentity( &m_matWorld );
}

DxPiece::~DxPiece()
{
	CleanUp();
}

void DxPiece::CleanUp()
{
	m_pStaticAniFrame = NULL;
	SAFE_DELETE( m_pPieceEff );
	SAFE_DELETE( m_pAniTexEff );

	if ( m_strLightMapDay.size() )
		TextureManager::ReleaseTexture( m_strLightMapDay.c_str(), m_pTextureLightMapDay );


	m_strLightMapDay = "";
	m_strLightMapDS1 = "";
	unknown_var5 = "";
	unknown_var6 = "";
	m_strPiece = "";
	m_bLightMapOffSet = FALSE;
}

void DxPiece::FrameMove( const float fTime, const float fElapsedTime )
{
	m_fTime += fElapsedTime;
	m_fAniTime += fElapsedTime * UNITANIKEY_PERSEC;
	if( m_fTime > 1.0e15f )		m_fTime = 0.f;
	if( m_fAniTime > 1.0e15f )	m_fAniTime = 0.f;



	if( m_pStaticAniFrame )	m_pStaticAniFrame->FrameMove( fTime, fElapsedTime );
	if( m_pPieceEff )		m_pPieceEff->FrameMove( fTime, fElapsedTime );
	if( m_pAniTexEff )		m_pAniTexEff->FrameMove( fElapsedTime );
}

void DxPiece::Render( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	if( !m_pStaticAniFrame )	return;

	material.m_pLightMapDay = m_pTextureLightMapDay;
	material.g_vLightMapUV_Offset.x = m_fLightMapOffsetX;
	material.g_vLightMapUV_Offset.y = m_fLightMapOffsetY;

	// Note : Point Light를 적용시킨다.
	RenderPointLight( pd3dDevice );

	// Note : Render
	m_pStaticAniFrame->Render( pd3dDevice, m_matWorld, m_fAniTime, m_pAniTexEff->GetMapTexEff(), material );

	// Note : Saved of Location Matirx. Because Animation is Change to All Time. Is Alpha ?
	//			위치 매트릭스를 저장 시켜 놓는다. Animation이 계속 바뀌기 때문이다. 알파는 .??
	if( !m_pPieceEff )		return;
	m_pPieceEff->SetMatrix();
}

void DxPiece::RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	if( !m_pStaticAniFrame )	return;

	// Note : Point Light를 적용시킨다.
	RenderPointLight( pd3dDevice );

	material.m_pLightMapDay = m_pTextureLightMapDay;
	material.g_vLightMapUV_Offset.x = m_fLightMapOffsetX;
	material.g_vLightMapUV_Offset.y = m_fLightMapOffsetY;

	// Note : Render
	m_pStaticAniFrame->Render_Alpha( pd3dDevice, m_matWorld, m_pAniTexEff->GetMapTexEff(), material );
}

void DxPiece::RenderEff( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
						DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material )
{
	if( !m_pPieceEff )	return;

	// Note : Point Light를 적용시킨다.
	RenderPointLight( pd3dDevice );

	// Note : Render
	m_pPieceEff->Render( pd3dDevice, m_matWorld, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh );
}

void DxPiece::Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	D3DXVECTOR3 vCenter = DxEnvironment::GetInstance().GetCenter();
	D3DXMATRIX matWorld = m_matWorld;
	matWorld._42 = matWorld._42 - (vCenter.y*2.f);
	D3DXMatrixMultiply_MIRROR( matWorld );

	// Note : Point Light를 적용시킨다.
	RenderPointLight( pd3dDevice );

	// Note : Render
	if( m_pStaticAniFrame )
	{
		material.m_pLightMapDay = m_pTextureLightMapDay;
		material.g_vLightMapUV_Offset.x = m_fLightMapOffsetX;
		material.g_vLightMapUV_Offset.y = m_fLightMapOffsetY;

		m_pStaticAniFrame->Render_Reflect( pd3dDevice, matWorld, m_fAniTime, m_pAniTexEff->GetMapTexEff(), material );
		m_pStaticAniFrame->Render_Reflect_Alpha( pd3dDevice, matWorld, m_pAniTexEff->GetMapTexEff(), material );
	}

	// Note : Render
	if( m_pPieceEff )
	{
		m_pPieceEff->Render( pd3dDevice, matWorld, NULL, NULL, NULL, NULL );
	}
}

void DxPiece::SetAABBBox()
{
	if ( m_pStaticAniFrame )
		m_pStaticAniFrame->GetAABBBox( m_vMax, m_vMin );

	m_vStaticMax = m_vMax;
	m_vStaticMin = m_vMin;
}

BOOL DxPiece::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision )
{
	D3DXMATRIX matInverse;
	D3DXMatrixInverse( &matInverse, NULL, &m_matWorld );

	D3DXVECTOR3 vNewStart = vStart;
	D3DXVECTOR3 vNewEnd = vEnd;
	D3DXVec3TransformCoord( &vNewStart, &vStart, &matInverse );
	D3DXVec3TransformCoord( &vNewEnd, &vEnd, &matInverse );

	BOOL bCollision(FALSE);
	bCollision = m_pStaticAniFrame->IsCollisionLine( vNewStart, vNewEnd, vCollision );

	if( bCollision )
	{
		D3DXVec3TransformCoord( &vEnd, &vNewEnd, &m_matWorld );
		D3DXVec3TransformCoord( &vCollision, &vCollision, &m_matWorld );
	}

	return bCollision;
}

void DxPiece::Import( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceEdit* pSrc, const D3DXMATRIX& matLocal, const D3DXMATRIX& matFrame )
{
	CleanUp();

	m_vMax		= pSrc->GetAABBMax();
	m_vMin		= pSrc->GetAABBMin();
	m_vStaticMax = pSrc->GetStaticMax();
	m_vStaticMin = pSrc->GetStaticMin();
	m_vCenter	= pSrc->GetCenter();
	m_fLength	= pSrc->GetLength();
	D3DXMatrixMultiply( &m_matWorld, &matLocal, &matFrame );
	D3DXMatrixMultiply( &m_matWorld, pSrc->GetMatrixLocal(), &m_matWorld );

	// Note : 자신에 맞도록 StaticAABB와 Center, Length를 바꾼다.
	D3DXMatrixAABBTransform( m_vStaticMax, m_vStaticMin, m_matWorld );
	m_vCenter = (m_vStaticMax+m_vStaticMin)*0.5f;
	m_fLengthOrig = m_fLength;
	m_fLength = m_fLength*m_fLength;

	m_pStaticAniFrame = pSrc->m_pStaticAniFrame;	// 그냥 주소만 주면 됨.

	m_pPieceEff = new DxPieceEff;
	m_pPieceEff->Clone( pd3dDevice, pSrc->m_pPieceEff );

	m_pAniTexEff = new DxPieceTexEff;
	m_pAniTexEff->Clone( pd3dDevice, pSrc->m_pAniTexEff );
}

void DxPiece::CloneData( LPDIRECT3DDEVICEQ pd3dDevice, DxPiece* pSrc )
{
	CleanUp();

	m_vMax		= pSrc->m_vMax;
	m_vMin		= pSrc->m_vMin;
	m_vStaticMax = pSrc->m_vStaticMax;
	m_vStaticMin = pSrc->m_vStaticMin;
	m_vCenter	= pSrc->m_vCenter;
	m_fLength	= pSrc->m_fLength;
	m_matWorld	= pSrc->m_matWorld;
	m_fLengthOrig = m_fLength * 0.5f;

	// Note : 자신과 맞닿아 있는 빛을 체크를 한다.
	CheckPointLight();

	m_pStaticAniFrame = pSrc->m_pStaticAniFrame;	// 그냥 주소만 주면 됨.

	m_pPieceEff = new DxPieceEff;
	m_pPieceEff->Clone( pd3dDevice, pSrc->m_pPieceEff );

	m_pAniTexEff = new DxPieceTexEff;
	m_pAniTexEff->Clone( pd3dDevice, pSrc->m_pAniTexEff );
}

void DxPiece::CheckPointLight()
{
	DxLightMan::GetInstance()->SetPiecePointIndex( m_vCenter, m_fLength, m_vecPointIndex );
}

void DxPiece::RenderPointLight( LPDIRECT3DDEVICEQ pd3dDevice )
{
	// Note : Light가 꺼져 있을 경우는 체크를 하지 않는다.

	// Note : 2개만 적용시키자.
	for( DWORD i=0; i<2; ++i )
	{
		if( m_vecPointIndex.size() > i )
		{
			D3DLIGHT9* pLight = DxLightMan::GetInstance()->GetPointLight( m_vecPointIndex[i] );
			if( pLight )
			{
				pd3dDevice->SetLight( i+1, pLight );
				pd3dDevice->LightEnable( i+1, TRUE );

				continue;
			}
		}

		// Note : 안 걸리면 무조건 조명은 끈다.
		pd3dDevice->LightEnable( i+1, FALSE );
	}
}



void DxPiece::LoadLightMaps( LPDIRECT3DDEVICEQ pd3dDevice )
{
	//lightmaps load
	if ( m_strLightMapDay.size() )
		TextureManager::LoadTexture( m_strLightMapDay.c_str(), pd3dDevice, m_pTextureLightMapDay, 0L, 0L, FALSE );


}

BOOL DxPiece::IsRender( const CLIPVOLUME &sCV, const D3DXVECTOR3& vFromPt, const D3DXVECTOR3& vLookatPt )
{
	if ( g_bPIECE_USE_COLLISION )
	{
		D3DXVECTOR3 vDir = m_vPos - vFromPt;
		float fDistance = D3DXVec3Length( &vDir );

		//check camera collision
		BOOL bCol = COLLISION::IsCollisionVolume( sCV, m_vMaxCol, m_vMinCol );
		if ( !bCol )
		{
			if ( fDistance > m_fLength )	return FALSE;
		}
	}
	
	if ( g_bPIECE_USE_DISTANCE )
	{
		D3DXVECTOR3 vDir = m_vPos - vLookatPt;
		float fDistance = D3DXVec3Length( &vDir );

		//check draw distance, data from official
		if ( fDistance > ( m_fLengthOrig + m_fDrawDistance1 + m_fDrawDistance2 )	) return FALSE;
	}


	//jdevtodo
	//add function not to render piece that is behind another piece

	return TRUE;
}

void DxPiece::UpdateLightMapUV()
{
	m_bLightMapOffSet = TRUE;

	if ( m_pStaticAniFrame )
		m_pStaticAniFrame->UpdateLightMapUV( m_fLightMapUVX, m_fLightMapUVY );
}

