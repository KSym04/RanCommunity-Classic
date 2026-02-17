#include "stdafx.h"

#include "../DxEffect/DxFrameAniMan.h"
#include "./DxPieceEffMan.h"

#include "./DxPieceStaticAni.h"
#include "../Meshs/DxPieceAnimation.h"
#include "./DxPieceMultiStaticMaterialMesh.h"

#include "../DxCommon/MaterialShaderParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//----------------------------------------------------------------------------------------------------------------------
//								D	X		R	E	P	L	A	C	E	P	I	E	C	E		E	X
//----------------------------------------------------------------------------------------------------------------------
DWORD	DxStaticAniFrame::VERSION_STATIC_ANI = 0x0200;

DxStaticAniFrame::DxStaticAniFrame() :
	m_bFrameMoveLock(TRUE),
	m_dwEffectCount(0L),
	m_pEffectHead(NULL),
	m_pStaticMesh(NULL),
	m_pStaticMaterialMesh(NULL),
	m_pFrameAniMan(NULL),
	m_pPieceAnimation(NULL),
	m_pMultiStaticMaterialMesh(NULL),
	m_nRefCount(0),
	m_pNext(NULL)
{
}

DxStaticAniFrame::~DxStaticAniFrame()
{
	CleanUp();
}

void DxStaticAniFrame::CleanUp()
{
	m_dwEffectCount = 0L;
	SAFE_DELETE( m_pEffectHead );
	SAFE_DELETE( m_pStaticMesh );
	SAFE_DELETE( m_pStaticMaterialMesh );
	SAFE_DELETE( m_pFrameAniMan );
	SAFE_DELETE( m_pPieceAnimation );
	SAFE_DELETE( m_pMultiStaticMaterialMesh );

	SAFE_DELETE( m_pNext );	// 음 조심해야 한다.
}

void DxStaticAniFrame::OnResetDevice( LPDIRECT3DDEVICEQ pd3dDevice )
{
	PDXEFFECTBASE pCur = m_pEffectHead;
	while( pCur )
	{
		pCur->RestoreDeviceObjects( pd3dDevice );
		pCur = pCur->pEffectNext;
	}
}

void DxStaticAniFrame::OnLostDevice()
{
	PDXEFFECTBASE pCur = m_pEffectHead;
	while( pCur )
	{
		pCur->InvalidateDeviceObjects();
		pCur = pCur->pEffectNext;
	}
}

DxFrame* DxStaticAniFrame::GetAniFrame( const char* szName )
{
	if( !m_pFrameAniMan )	return NULL;
	return m_pFrameAniMan->GetAniFrame( szName );
}

DxAnimationMan*	DxStaticAniFrame::GetAniManHead()
{
	if( !m_pFrameAniMan )	return NULL;
	return m_pFrameAniMan->GetAniManHead();
}

void DxStaticAniFrame::MakeAniMan( LPDIRECT3DDEVICEQ pd3dDevice, DxFrameMesh* pFrameMesh )
{
	m_pFrameAniMan = new DxFrameAniMan;
	m_pFrameAniMan->MakeAniMan( pd3dDevice, pFrameMesh );
}

void DxStaticAniFrame::FrameMove( const float fTime, const float fElapsedTime )
{
	if( !m_bFrameMoveLock )	return;	// Note : Lock Off	.. No FrameMove

	if( m_pStaticMesh )					m_pStaticMesh->FrameMove( fTime, fElapsedTime );
	if( m_pStaticMaterialMesh )			m_pStaticMaterialMesh->FrameMove( fTime, fElapsedTime );
	if( m_pMultiStaticMaterialMesh )	m_pMultiStaticMaterialMesh->FrameMove( fTime, fElapsedTime );

	// 여기 놔두면 심하게 흔들린다.
	PDXEFFECTBASE pCur = m_pEffectHead;
	while( pCur )
	{
		pCur->FrameMove( 0.f, fElapsedTime );
		pCur = pCur->pEffectNext;
	}

	// Note : Lock Off
	m_bFrameMoveLock = FALSE;
}

void DxStaticAniFrame::Render( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, const float fAniTime, DxTextureEffMan::MAPTEXEFF* pmapTexEff, SMATERIAL_SETTING& material )
{
	// Note : FrameAnimation은 Alpha가 분리 되지 않는다....
	if( m_pFrameAniMan )
	{
		m_pFrameAniMan->Render( pd3dDevice, matWorld, fAniTime, pmapTexEff );
	}

	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if( m_pStaticMesh )
	{
		m_pStaticMesh->Render_PIECE_( pd3dDevice, pmapTexEff );
		m_pStaticMesh->Render_PIECE_Alpha( pd3dDevice, pmapTexEff );
	}

	material.Update( pd3dDevice, matWorld );
	if( m_pStaticMaterialMesh )
	{
		m_pStaticMaterialMesh->Render_PIECE_Normal( pd3dDevice, material, matWorld );
		m_pStaticMaterialMesh->Render_PIECE_MaterialEffect( pd3dDevice, material, matWorld );
		m_pStaticMaterialMesh->Render_PIECE_Blending( pd3dDevice, material, matWorld );
		m_pStaticMaterialMesh->Render_PIECE_Alpha( pd3dDevice, material, matWorld );
	}

	if( m_pMultiStaticMaterialMesh )
	{
		m_pMultiStaticMaterialMesh->Render_PIECE_Normal( pd3dDevice, material, matWorld );
		m_pMultiStaticMaterialMesh->Render_PIECE_MaterialEffect( pd3dDevice, material, matWorld );
		m_pMultiStaticMaterialMesh->Render_PIECE_Blending( pd3dDevice, material, matWorld );
		m_pMultiStaticMaterialMesh->Render_PIECE_Alpha( pd3dDevice, material, matWorld );
	}

	if ( m_pPieceAnimation )			m_pPieceAnimation->FrameMove( fAniTime, matWorld );

	PDXEFFECTBASE pCur = m_pEffectHead;
	while( pCur )
	{
		(*pCur->m_pmatLocal) = matWorld;	// Note : World Matrix 음.. 어렵네.

		pCur->Render( NULL, pd3dDevice );
		pCur = pCur->pEffectNext;
	}

	// Note : Lock ON
	m_bFrameMoveLock = TRUE;
}

void DxStaticAniFrame::Render_Alpha( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, DxTextureEffMan::MAPTEXEFF* pmapTexEff, SMATERIAL_SETTING& material )
{
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if( m_pStaticMesh )
	{
		m_pStaticMesh->Render_PIECE_SoftAlpha( pd3dDevice, pmapTexEff );
	}

	material.Update( pd3dDevice, matWorld );

	if( m_pStaticMaterialMesh )
	{
		m_pStaticMaterialMesh->Render_PIECE_SpecularBlending( pd3dDevice, material, matWorld );
		m_pStaticMaterialMesh->Render_PIECE_SoftAlpha( pd3dDevice, material, matWorld );
	}

	if( m_pMultiStaticMaterialMesh )
	{
		m_pMultiStaticMaterialMesh->Render_PIECE_SpecularBlending( pd3dDevice, material, matWorld );
		m_pMultiStaticMaterialMesh->Render_PIECE_SoftAlpha( pd3dDevice, material, matWorld );
	}
}

void DxStaticAniFrame::Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, const float fAniTime, DxTextureEffMan::MAPTEXEFF* pmapTexEff, SMATERIAL_SETTING& material )
{
	// Note : FrameAnimation은 Alpha가 분리 되지 않는다....
	if( m_pFrameAniMan )
	{
		m_pFrameAniMan->Render( pd3dDevice, matWorld, fAniTime, pmapTexEff );
	}

	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if( m_pStaticMesh )
	{
		m_pStaticMesh->Render_PIECE_( pd3dDevice, pmapTexEff );
		m_pStaticMesh->Render_PIECE_Alpha( pd3dDevice, pmapTexEff );
	}

	PDXEFFECTBASE pCur = m_pEffectHead;
	while( pCur )
	{
		(*pCur->m_pmatLocal) = matWorld;	// Note : World Matrix 음.. 어렵네.

		pCur->Render( NULL, pd3dDevice );
		pCur = pCur->pEffectNext;
	}

	// Note : Lock ON
	m_bFrameMoveLock = TRUE;
}


void DxStaticAniFrame::Render_Reflect_Alpha( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, DxTextureEffMan::MAPTEXEFF* pmapTexEff, SMATERIAL_SETTING& material )
{
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if( m_pStaticMesh )
	{
		m_pStaticMesh->Render_PIECE_SoftAlpha( pd3dDevice, pmapTexEff );
	}
}

void DxStaticAniFrame::Render_EDIT( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, SMATERIAL_SETTING& material )
{
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if( m_pFrameAniMan )
	{
		m_pFrameAniMan->Render_EDIT( pd3dDevice );
	}
}

void DxStaticAniFrame::Save( CSerialFile& SFile )
{
	SFile << (DWORD)VERSION_STATIC_ANI;
	SFile.BeginBlock( EMBLOCK_03 );

	if( m_pFrameAniMan )
	{
		SFile << (BOOL)TRUE;
		m_pFrameAniMan->Save( SFile );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}

	if( m_pStaticMesh )
	{
		SFile << (BOOL)TRUE;
		m_pStaticMesh->Save( SFile, TRUE );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}

	if( m_pStaticMaterialMesh )
	{
		SFile << (BOOL)TRUE;
		m_pStaticMaterialMesh->Save( SFile, TRUE );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}

	if( m_pMultiStaticMaterialMesh )
	{
		SFile << (BOOL)TRUE;
		m_pMultiStaticMaterialMesh->Save( SFile, TRUE );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}

	if( m_pPieceAnimation )
	{
		SFile << (BOOL)TRUE;
		m_pPieceAnimation->SaveFile( SFile );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}

	// Note : Replace Effect
	{
		DxEffectBase* pEffCur;
		DWORD dwEffCount = 0;
		
		pEffCur = m_pEffectHead;
		while ( pEffCur )
		{
			dwEffCount++;
			pEffCur = pEffCur->pEffectNext;
		}

		SFile << dwEffCount;

		pEffCur = m_pEffectHead;
		while ( pEffCur )
		{
			pEffCur->SaveFile( SFile );
			pEffCur = pEffCur->pEffectNext;
		}
	}

	SFile.EndBlock( EMBLOCK_03 );
}

void DxStaticAniFrame::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();		// 초기화

	DWORD dwVer;
	DWORD dwBufferSize;

	SFile >> dwVer;
	SFile >> dwBufferSize;

	if( dwVer==VERSION_STATIC_ANI )
	{

		BOOL bExist;
		// Note : Animation Frame
		SFile >> bExist;
		if ( bExist )
		{
			m_pFrameAniMan = new DxFrameAniMan;
			m_pFrameAniMan->Load( pd3dDevice, SFile );
		}

		// Note : Static Mesh
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMesh = new DxStaticMesh;
			m_pStaticMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		//static material mesh
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMaterialMesh = new DxStaticMaterialMesh;
			m_pStaticMaterialMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		SFile >> bExist;
		if ( bExist )
		{
			m_pMultiStaticMaterialMesh = new DxPieceMultiStaticMaterialMesh;
			m_pMultiStaticMaterialMesh->Load( pd3dDevice, SFile );
		}

		SFile >> bExist;
		if ( bExist )
		{
			m_pPieceAnimation = new DxPieceAnimation;
			m_pPieceAnimation->LoadFile( SFile, pd3dDevice, m_pMultiStaticMaterialMesh );
		}

		// Note : Replace Effect
		{
			DWORD dwMaxCount = 0;
			SFile >> dwMaxCount;
			for ( DWORD i=0; i<dwMaxCount; i++ )
			{
				HRESULT hr = S_OK;
				hr = EffectLoadToList( pd3dDevice, SFile, m_pEffectHead );
				if ( FAILED(hr) )
					MessageBox ( NULL, "이팩트를 로드중에 오류가 발생", "ERROR", MB_OK );

				m_dwEffectCount++;
			}
		}
	}
	else if( dwVer==0x0104 )
	{
		BOOL bExist;

		// Note : Static Mesh
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMesh = new DxStaticMesh;
			m_pStaticMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		//static material mesh
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMaterialMesh = new DxStaticMaterialMesh;
			m_pStaticMaterialMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		SFile >> bExist;
		if ( bExist )
		{
			m_pMultiStaticMaterialMesh = new DxPieceMultiStaticMaterialMesh;
			m_pMultiStaticMaterialMesh->Load( pd3dDevice, SFile );
		}

		SFile >> bExist;
		if ( bExist )
		{
			m_pPieceAnimation = new DxPieceAnimation;
			m_pPieceAnimation->LoadFile( SFile, pd3dDevice, m_pMultiStaticMaterialMesh );
		}
		
		// Note : Replace Effect
		{
			DWORD dwMaxCount = 0;
			SFile >> dwMaxCount;
			for ( DWORD i=0; i<dwMaxCount; i++ )
			{
				HRESULT hr = S_OK;
				hr = EffectLoadToList( pd3dDevice, SFile, m_pEffectHead );
				if ( FAILED(hr) )
					MessageBox ( NULL, "이팩트를 로드중에 오류가 발생", "ERROR", MB_OK );

				m_dwEffectCount++;
			}
		}
	}
	else if( dwVer==0x0103 )
	{
		BOOL bExist;
		// Note : Animation Frame
		SFile >> bExist;
		if ( bExist )
		{
			m_pFrameAniMan = new DxFrameAniMan;
			m_pFrameAniMan->Load( pd3dDevice, SFile );
		}

		// Note : Static Mesh
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMesh = new DxStaticMesh;
			m_pStaticMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		//static material mesh
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMaterialMesh = new DxStaticMaterialMesh;
			m_pStaticMaterialMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		// Note : Replace Effect
		{
			DWORD dwMaxCount = 0;
			SFile >> dwMaxCount;
			for ( DWORD i=0; i<dwMaxCount; i++ )
			{
				HRESULT hr = S_OK;
				hr = EffectLoadToList( pd3dDevice, SFile, m_pEffectHead );
				if ( FAILED(hr) )
					MessageBox ( NULL, "이팩트를 로드중에 오류가 발생", "ERROR", MB_OK );

				m_dwEffectCount++;
			}
		}
	}
	else if( dwVer==0x0102 )
	{

		BOOL bExist;
		// Note : Animation Frame
		SFile >> bExist;
		if ( bExist )
		{
			m_pFrameAniMan = new DxFrameAniMan;
			m_pFrameAniMan->Load( pd3dDevice, SFile );
		}

		// Note : Static Mesh
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMesh = new DxStaticMesh;
			m_pStaticMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		// Note : Replace Effect
		{
			DWORD dwMaxCount = 0;
			SFile >> dwMaxCount;
			for ( DWORD i=0; i<dwMaxCount; i++ )
			{
				HRESULT hr = S_OK;
				hr = EffectLoadToList( pd3dDevice, SFile, m_pEffectHead );
				if ( FAILED(hr) )
					MessageBox ( NULL, "이팩트를 로드중에 오류가 발생", "ERROR", MB_OK );

				m_dwEffectCount++;
			}
		}
	}
	else if( dwVer==0x0101 )
	{
		// Note : Static Mesh
		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMesh = new DxStaticMesh;
			m_pStaticMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		// Note : Animation Frame
		SFile >> bExist;
		if ( bExist )
		{
			m_pFrameAniMan = new DxFrameAniMan;
			m_pFrameAniMan->Load( pd3dDevice, SFile );
		}

		// Note : Replace Effect
		{
			DWORD dwMaxCount = 0;
			SFile >> dwMaxCount;
			for ( DWORD i=0; i<dwMaxCount; i++ )
			{
				HRESULT hr = S_OK;
				hr = EffectLoadToList( pd3dDevice, SFile, m_pEffectHead );
				if ( FAILED(hr) )
					MessageBox ( NULL, "이팩트를 로드중에 오류가 발생", "ERROR", MB_OK );

				m_dwEffectCount++;
			}
		}
	}
	else if( dwVer==0x0100 )
	{
		// Note : Static Mesh
		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			m_pStaticMesh = new DxStaticMesh;
			m_pStaticMesh->Load( pd3dDevice, SFile, FALSE, TRUE );
		}

		// Note : Animation Frame
		SFile >> bExist;
		if ( bExist )
		{
			m_pFrameAniMan = new DxFrameAniMan;
			m_pFrameAniMan->Load( pd3dDevice, SFile );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxStaticAniFrame::Load", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwBufferSize );
	}
}

void DxStaticAniFrame::Export( LPDIRECT3DDEVICEQ pd3dDevice, DxFrameMesh* pFrameMesh )
{
	//	Note : Optimum Octree..
	SAFE_DELETE( m_pStaticMesh );
	m_pStaticMesh = new DxStaticMesh;
	m_pStaticMesh->MakeAABBOCTree( pd3dDevice, pFrameMesh, FALSE, TRUE );	// Modify

	SAFE_DELETE( m_pFrameAniMan );
	m_pFrameAniMan = new DxFrameAniMan;
	m_pFrameAniMan->MakeAniMan( pd3dDevice, pFrameMesh );

	// Note : Replace ^^
	DxEffectBase* pEffCur = pFrameMesh->GetEffectList();
	while ( pEffCur )
	{
		if( !pEffCur->GetEachRender() )
		{
			DWORD TypeID = pEffCur->GetTypeID();
			char* szFrameName = pEffCur->GetAdaptFrameName();

			PBYTE pProperty;
			DWORD dwSize;
			DWORD dwVer;
			pEffCur->GetProperty ( pProperty, dwSize, dwVer );
			LPDXAFFINEPARTS pAffineParts = pEffCur->m_pAffineParts;

			DxFrame* pFrame = pFrameMesh->GetFrameRoot();
			EffectAdaptToList( pFrame, TypeID, szFrameName, pd3dDevice, pProperty, dwSize, dwVer, pAffineParts, pEffCur );
		}

		pEffCur = pEffCur->pEffectNext;
	}
}

HRESULT DxStaticAniFrame::EffectAdaptToList( DxFrame *pFrameRoot, DWORD TypeID, char* szFrame, LPDIRECT3DDEVICEQ pd3dDevice,
									PBYTE pProperty, DWORD dwSize, DWORD dwVer, LPDXAFFINEPARTS pAffineParts, DxEffectBase* pEffectSrc )
{
	HRESULT hr;

	DxFrame *pDxFrame = pFrameRoot->FindFrame( szFrame );

	DxEffectBase *pEffect = DxPieceEffMan::GetInstance().CreateEffInstance( TypeID );
	if ( !pEffect ) return NULL;

	hr = pEffect->AdaptToEffList( pDxFrame, pd3dDevice );

	if ( hr == S_FALSE )
	{
		char szMsg[256] = "";
		StringCchPrintf( szMsg, 256, "단독 효과(%s)의 Adapt 메소드가 존재하지 않습니다.", pEffect->GetName() );
		MessageBox ( NULL, szMsg, "ERROR", MB_OK );
		return E_FAIL;
	}

	if ( FAILED(hr) )
	{
		char szMsg[256] = "";
		StringCchPrintf( szMsg, 256, "단독 효과(%s)의 Adapt 도중 오류를 발생하였습니다.", pEffect->GetName() );
		MessageBox ( NULL, szMsg, "ERROR", MB_OK );
		return E_FAIL;
	}

	//	Note : Property, Affine  값을 대입.
	if ( pProperty )	pEffect->SetProperty ( pProperty, dwSize, dwVer );
	if ( pAffineParts && pEffect->IsUseAffineMatrix() )		pEffect->SetAffineValue ( pAffineParts );

	//	Note : 대량의 버퍼를 복제하기 위한 메소드.
	if ( pEffectSrc )	pEffect->CloneData ( pEffectSrc, pd3dDevice );

	hr = pEffect->Create ( pd3dDevice );
	if ( FAILED(hr) )
	{
		SAFE_DELETE(pEffect);
		return E_FAIL;
	}

	m_dwEffectCount++;
	pEffect->pEffectNext = m_pEffectHead;
	m_pEffectHead = pEffect;

	return S_OK;
}

HRESULT DxStaticAniFrame::EffectLoadToList( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile &SFile, PDXEFFECTBASE &pEffList )
{
	BOOL	bResult = FALSE;
	HRESULT	hr = E_FAIL;
	char	*szName = NULL;
	int		StrLength;

	SFile >> bResult;
	if ( bResult )
	{
		SFile >> StrLength;
		szName = new char [ StrLength ];
		SFile.ReadBuffer ( szName, StrLength );
	}

    DWORD	TypeID;
	SFile >> TypeID;
	
	DWORD	dwSize = 0;
	BYTE	*pProperty = NULL;
	DWORD	dwVer = 0;

	SFile >> dwVer;
	SFile >> dwSize;
	if ( dwSize )
	{
		pProperty = new BYTE [ dwSize ];
		SFile.ReadBuffer ( pProperty, sizeof ( BYTE ) * dwSize );
	}

	LPDXAFFINEPARTS	pAffineParts = NULL;
	SFile >> bResult;
	if ( bResult )
	{
		pAffineParts = new DXAFFINEPARTS;
		SFile.ReadBuffer ( pAffineParts, sizeof ( DXAFFINEPARTS ) );
	}

	DxEffectBase *pEffect = DxPieceEffMan::GetInstance().CreateEffInstance ( TypeID );
	if ( !pEffect ) return E_FAIL;

	//	Note : Property, Affine  값을 대입.
	//
	if ( pProperty )	pEffect->SetProperty ( pProperty, dwSize, dwVer );
	if ( pAffineParts && pEffect->IsUseAffineMatrix() )	pEffect->SetAffineValue ( pAffineParts );

	//	Note : LoadBuffer(), 특정 Effect 에서만 사용되는 메모리 버퍼를 읽는다.
	//
	pEffect->LoadBuffer ( SFile, dwVer, pd3dDevice );

	//	Note : Create Device.
	//
	hr = pEffect->Create ( pd3dDevice );
	if ( FAILED(hr) )
	{
		SAFE_DELETE(pEffect);
		return E_FAIL;
	}

    SAFE_DELETE_ARRAY ( pProperty );
	SAFE_DELETE ( pAffineParts );
	SAFE_DELETE_ARRAY ( szName );

	//	Note : Insert To List.
	//
	pEffect->pEffectNext = pEffList;
	pEffList = pEffect;

	// Note : 값 뒤집기	< List 순서를 유지하기 위해서 >
	DxEffectBase* pTemp = NULL;
	DxEffectBase* pCur	= pEffList;
	DxEffectBase* pThis = pEffList;
	while ( pCur )
	{
		pThis = pCur;
		pCur = pCur->pEffectNext;

		pThis->pEffectNext = pTemp;
		pTemp = pThis;
	}
	pEffList = pTemp;

	return S_OK;
}

void DxStaticAniFrame::GetAABBBox( D3DXVECTOR3& vMax, D3DXVECTOR3&  vMin )
{
	vMax = D3DXVECTOR3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
	vMin = D3DXVECTOR3(FLT_MAX,FLT_MAX,FLT_MAX);

	if( m_pStaticMesh )
	{
		D3DXVECTOR3	vMax2 = m_pStaticMesh->GetAABBMax();
		D3DXVECTOR3	vMin2 = m_pStaticMesh->GetAABBMin();

		if ( vMax.x < vMax2.x )	vMax.x = vMax2.x;
		if ( vMax.y < vMax2.y )	vMax.y = vMax2.y;
		if ( vMax.z < vMax2.z )	vMax.z = vMax2.z;

		if ( vMin.x > vMin2.x )	vMin.x = vMin2.x;
		if ( vMin.y > vMin2.y )	vMin.y = vMin2.y;
		if ( vMin.z > vMin2.z )	vMin.z = vMin2.z;
	}

	if( m_pStaticMaterialMesh )
	{
		D3DXVECTOR3	vMax2 = m_pStaticMaterialMesh->GetAABBMax();
		D3DXVECTOR3	vMin2 = m_pStaticMaterialMesh->GetAABBMin();

		if ( vMax.x < vMax2.x )	vMax.x = vMax2.x;
		if ( vMax.y < vMax2.y )	vMax.y = vMax2.y;
		if ( vMax.z < vMax2.z )	vMax.z = vMax2.z;

		if ( vMin.x > vMin2.x )	vMin.x = vMin2.x;
		if ( vMin.y > vMin2.y )	vMin.y = vMin2.y;
		if ( vMin.z > vMin2.z )	vMin.z = vMin2.z;
	}

	if( m_pMultiStaticMaterialMesh )
	{
		D3DXVECTOR3	vMax2 = m_pMultiStaticMaterialMesh->GetAABBMax();
		D3DXVECTOR3	vMin2 = m_pMultiStaticMaterialMesh->GetAABBMin();

		if ( vMax.x < vMax2.x )	vMax.x = vMax2.x;
		if ( vMax.y < vMax2.y )	vMax.y = vMax2.y;
		if ( vMax.z < vMax2.z )	vMax.z = vMax2.z;

		if ( vMin.x > vMin2.x )	vMin.x = vMin2.x;
		if ( vMin.y > vMin2.y )	vMin.y = vMin2.y;
		if ( vMin.z > vMin2.z )	vMin.z = vMin2.z;
	}
}

BOOL DxStaticAniFrame::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision )
{
	if( m_pStaticMesh )
	{
		const char* szName = NULL;
		D3DXVECTOR3 vNewNormal(0.f,1.f,0.f);
		if ( m_pStaticMesh->IsCollisionLine( vStart, vEnd, vCollision, vNewNormal, szName, TRUE ) ) return TRUE;
	}

	if( m_pStaticMaterialMesh )
	{
		const char* szName = NULL;
		D3DXVECTOR3 vNewNormal(0.f,1.f,0.f);
		if ( m_pStaticMaterialMesh->IsCollisionLine( vStart, vEnd, vCollision, vNewNormal, szName, TRUE ) ) return TRUE;
	}

	if( m_pMultiStaticMaterialMesh )
	{
		const char* szName = NULL;
		D3DXVECTOR3 vNewNormal(0.f,1.f,0.f);
		if ( m_pMultiStaticMaterialMesh->IsCollisionLine( vStart, vEnd, vCollision, vNewNormal, szName, TRUE ) ) return TRUE;
	}

	return FALSE;
}

void DxStaticAniFrame::UpdateLightMapUV( float fMultiX, float fMultiY )
{
	if ( m_pStaticMaterialMesh )
		m_pStaticMaterialMesh->UpdateLightMapUV( fMultiX, fMultiY );

	if ( m_pMultiStaticMaterialMesh )
		m_pMultiStaticMaterialMesh->UpdateLightMapUV( fMultiX, fMultiY );
}


