#include "stdafx.h"
#include "./DxPieceQuickSort.h"

#include "./DxPiece.h"
#include "../Meshs/DxFrameMesh.h"

#include "./DxStaticMesh.h"
#include "./DxMultiStaticMesh.h"
#include "./DxMultiStaticMaterialMesh.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL	g_bPICK_ALPHA = TRUE;	// 픽킹알파.

//----------------------------------------------------------------------------------------------------------------------
//								D	x		P	i	e	c	e		Q	u	i	c	k		S	o	r	t
//----------------------------------------------------------------------------------------------------------------------
DWORD								DxPieceQuickSort::m_dwArraySizeONLY = 0L;
DWORD								DxPieceQuickSort::m_dwPieceCountONLY = 0L;
DxPieceQuickSort::PIECEDISTANCE*	DxPieceQuickSort::m_pArrayPieceONLY = NULL;

DxPieceQuickSort::DxPieceQuickSort() :
	m_fElapsedTime(0.f),
	m_pArrayPiece(NULL),
	m_dwArraySize(0),
	m_dwPieceCount(0)
{
}

DxPieceQuickSort::~DxPieceQuickSort()
{
	m_dwPieceCount = 0;
	m_dwArraySize = 0;
	SAFE_DELETE_ARRAY( m_pArrayPiece );

	m_dwPieceCountONLY = 0;
	m_dwArraySizeONLY = 0;
	SAFE_DELETE_ARRAY( m_pArrayPieceONLY );
}

void DxPieceQuickSort::FrameMove( const float fTime, const float fElapsedTime )
{
	m_fElapsedTime = fElapsedTime;

	for( DWORD i=0; i<m_dwPieceCount; ++i )
	{
		m_pArrayPiece[i].m_pPiece->FrameMove( fTime, fElapsedTime );
	}
}

void DxPieceQuickSort::Render( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	for( DWORD i=0; i<m_dwPieceCount; ++i )
	{
		m_pArrayPiece[i].m_pPiece->Render( pd3dDevice, sCV, material );
	}
}

void DxPieceQuickSort::RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	for( int i=(int)(m_dwPieceCount-1); i>=0; --i )
	{
		m_pArrayPiece[i].m_pPiece->RenderAlpha( pd3dDevice, sCV, material );
	}
}

void DxPieceQuickSort::RenderEff( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
								 DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material )
{
	for( int i=(int)(m_dwPieceCount-1); i>=0; --i )
	{
		m_pArrayPiece[i].m_pPiece->RenderEff( pd3dDevice, sCV, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh, material );
	}
}

void DxPieceQuickSort::FrameMoveONLY( const float fTime, const float fElapsedTime )
{
	for( DWORD i=0; i<m_dwPieceCountONLY; ++i )
	{
		m_pArrayPieceONLY[i].m_pPiece->FrameMove( fTime, fElapsedTime );
	}
}

void DxPieceQuickSort::RenderPickAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
									   DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material )
{
	// Note : SetRenderState
	DWORD dwAlphaBlendEnable;
	pd3dDevice->GetRenderState( D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable );

	// Note : SetRenderState
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	D3DMATERIALQ sMaterial;
	D3DUtil_InitMaterial( sMaterial, 1.f, 1.f, 1.0f, 1.f );
	switch( DxMeshes::g_emMeshMeterial )
	{
	case EM_MESH_MATERIAL:
		// Note : Change for Alpha 0.5f
		D3DMATERIALQ sMaterial;
		sMaterial.Diffuse.r = sMaterial.Diffuse.g = sMaterial.Diffuse.b = 1.f;
		sMaterial.Ambient.r = sMaterial.Ambient.g = sMaterial.Ambient.b = 1.f;
		sMaterial.Specular.a = sMaterial.Specular.r = sMaterial.Specular.g = sMaterial.Specular.b = 0.f;
		sMaterial.Emissive.a = sMaterial.Emissive.r = sMaterial.Emissive.g = sMaterial.Emissive.b = 0.f;
		break;
	case EM_MESH_MATERIAL_RED:
		sMaterial.Ambient.r = sMaterial.Diffuse.r = 1.f;
		sMaterial.Ambient.g = sMaterial.Diffuse.g = DxMeshes::g_fAlpha;
		sMaterial.Ambient.b = sMaterial.Diffuse.b = DxMeshes::g_fAlpha;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	};

	DxPiece* pPiece(NULL);
	for( int i=(int)(m_dwPieceCountONLY-1); i>=0; --i )
	{
		pPiece = m_pArrayPieceONLY[i].m_pPiece;

		if( pPiece->m_bAplhaBlend )
		{
			sMaterial.Diffuse.a = sMaterial.Ambient.a = pPiece->m_fAlpha;
			pd3dDevice->SetMaterial( &sMaterial );

			pPiece->Render( pd3dDevice, sCV, material );
			pPiece->RenderAlpha( pd3dDevice, sCV, material );
			pPiece->RenderEff( pd3dDevice, sCV, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh, material );
		}
	}

	switch( DxMeshes::g_emMeshMeterial )
	{
	case EM_MESH_MATERIAL:
		// Note : ReSet Material
		sMaterial.Diffuse.a = sMaterial.Ambient.a = 1.f;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	case EM_MESH_MATERIAL_RED:
		sMaterial.Ambient.r = sMaterial.Diffuse.r = 1.f;
		sMaterial.Ambient.g = sMaterial.Diffuse.g = 1.f;
		sMaterial.Ambient.b = sMaterial.Diffuse.b = 1.f;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	};

	// Note : SetRenderState
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable );
}

void DxPieceQuickSort::SetArrayMaxSize( DWORD dwCount )
{
	if( dwCount > m_dwArraySize )
	{
		SAFE_DELETE_ARRAY( m_pArrayPiece );

		m_dwArraySize = dwCount;
		m_pArrayPiece = new PIECEDISTANCE[m_dwArraySize];
	}

	if( dwCount > m_dwArraySizeONLY )
	{
		SAFE_DELETE_ARRAY( m_pArrayPieceONLY );

		m_dwArraySizeONLY = dwCount;
		m_pArrayPieceONLY = new PIECEDISTANCE[m_dwArraySizeONLY];
	}
}

void DxPieceQuickSort::Reset()
{
	m_dwPieceCount = 0;
}

void DxPieceQuickSort::InitPickAlpha()
{
	m_dwPieceCountONLY = 0;
}

void DxPieceQuickSort::InsertData( DxPiece* pPiece, DxPiece* pPiecePick, const D3DXVECTOR3& vFromPt, const D3DXVECTOR3& vLookatPt, const float& fLength )
{
	float fTempLength(0.f);
	D3DXVECTOR3 vDir(0.f,0.f,0.f);

	DxPiece* pCur = pPiece;
	while( pCur )
	{
		vDir = pCur->m_vCenter - vFromPt;
		fTempLength = vDir.x*vDir.x + vDir.y*vDir.y + vDir.z*vDir.z;

		if( g_bPICK_ALPHA )
		{
			if( fLength > (fTempLength - pCur->m_fLength) )
			{
				// Note : Pick Check를 한다.
				if ( COLLISION::IsCollisionLineToAABB( vFromPt, vLookatPt, pCur->m_vStaticMax, pCur->m_vStaticMin ) )
				{
					if( pCur->m_fAlpha > 0.4f )	pCur->m_fAlpha -= m_fElapsedTime*0.6f;	// 1초에 0.6f가 줄어든다.
					else						pCur->m_fAlpha = 0.4f;

					pCur->m_bAplhaBlend = TRUE;
				}
				else if( pCur->m_bAplhaBlend )
				{
					if( pCur->m_fAlpha < 1.f ) 
					{
						pCur->m_fAlpha += m_fElapsedTime*0.6f;	// 1초에 0.6f가 늘어난다.
					}
					if( pCur->m_fAlpha > 1.f )
					{
						pCur->m_bAplhaBlend = FALSE;
						pCur->m_fAlpha = 1.f;
					}
				}
			}
			else if( pCur->m_bAplhaBlend )
			{
				if( pCur->m_fAlpha < 1.f ) 
				{
					pCur->m_fAlpha += m_fElapsedTime*0.6f;	// 1초에 0.6f가 늘어난다.
				}
				if( pCur->m_fAlpha > 1.f )
				{
					pCur->m_bAplhaBlend = FALSE;
					pCur->m_fAlpha = 1.f;
				}
			}
		}

		// Note : Pick AlphaBlending      
		if( pCur->m_bAplhaBlend )
		{
			m_pArrayPieceONLY[m_dwPieceCountONLY].m_fDistance = fTempLength;
			m_pArrayPieceONLY[m_dwPieceCountONLY].m_pPiece = pCur;
			++m_dwPieceCountONLY;
		}
		else
		{
			m_pArrayPiece[m_dwPieceCount].m_fDistance = fTempLength;
			m_pArrayPiece[m_dwPieceCount].m_pPiece = pCur;
			++m_dwPieceCount;
		}

		pCur = pCur->m_pTreeNext;
	}
}

void DxPieceQuickSort::QuickSortProcess()
{
	DxQuickSort( m_pArrayPiece, 0, m_dwPieceCount-1 );
	DxQuickSort( m_pArrayPieceONLY, 0, m_dwPieceCountONLY-1 );
}

void DxPieceQuickSort::DxQuickSort( PIECEDISTANCE pArray[], int ilo, int ihi ) 
{
	float pivot;	// pivot value for partitioning array
	int ulo, uhi;	// indices at ends of unpartitioned region
	int ieq;		// least index of array entry with value equal to pivot
	//   int tempEntry;	// temporary entry used for swapping

	if (ilo >= ihi) return;

	// Select a pivot value.
	pivot = pArray[(ilo + ihi)/2].m_fDistance;
	// Initialize ends of unpartitioned region and least index of entry
	// with value equal to pivot.
	ieq = ulo = ilo;
	uhi = ihi;
	// While the unpartitioned region is not empty, try to reduce its size.
	while( ulo <= uhi ) 
	{
		if( pArray[uhi].m_fDistance > pivot ) 
		{
			// Here, we can reduce the size of the unpartitioned region and
			// try again.
			uhi--;
		} 
		else 
		{
			// Here, A[uhi] <= pivot, so swap entries at indices ulo and
			// uhi.
			//tempEntry = pArray[ulo];
			//pArray[ulo] = pArray[uhi];
			//pArray[uhi] = tempEntry;

			Swap( pArray[uhi], pArray[ulo] );

			// After the swap, A[ulo] <= pivot.
			if (pArray[ulo].m_fDistance < pivot) 
			{
				// Swap entries at indices ieq and ulo.
				//tempEntry = pArray[ieq];
				//pArray[ieq] = pArray[ulo];
				//pArray[ulo] = tempEntry;
				Swap( pArray[ieq], pArray[ulo] );

				// After the swap, A[ieq] < pivot, so we need to change
				// ieq.
				ieq++;
				// We also need to change ulo, but we also need to do
				// that when A[ulo] = pivot, so we do it after this if
				// statement.
			}
			// Once again, we can reduce the size of the unpartitioned
			// region and try again.
			ulo++;
		}
	}

	// Now, all entries from index ilo to ieq - 1 are less than the pivot
	// and all entries from index uhi to ihi + 1 are greater than the
	// pivot.  So we have two regions of the array that can be sorted
	// recursively to put all of the entries in order.
	DxQuickSort( pArray, ilo, ieq - 1 );
	DxQuickSort( pArray, uhi + 1, ihi );
}


void DxPieceQuickSort::Swap( PIECEDISTANCE& pData1, PIECEDISTANCE& pData2 )
{
	PIECEDISTANCE pTemp;
	pTemp.m_fDistance = pData1.m_fDistance;
	pTemp.m_pPiece = pData1.m_pPiece;

	pData1.m_fDistance = pData2.m_fDistance;
	pData1.m_pPiece = pData2.m_pPiece;

	pData2.m_fDistance = pTemp.m_fDistance;
	pData2.m_pPiece = pTemp.m_pPiece;
}