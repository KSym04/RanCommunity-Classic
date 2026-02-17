#include "stdafx.h"
#include "./DxPieceManager.h"

#include <algorithm>
#include "../Common/StlFunctions.h"

#include "../DxCommon/DxInputDevice.h"
#include "../DxCommon/DxViewPort.h"
#include "../DxCommon/DxEnvironment.h"
#include "../Common/SerialFile.h"

#include "../Meshs/DxFrameMesh.h"
#include "./DxPiececontainer.h"

#include "./DxStaticMesh.h"
#include "./DxMultiStaticMesh.h"
#include "./DxMultiStaticMaterialMesh.h"

#include "DxPiece.h"
#include "DxPieceAABB.h"
#include "DxMultiPieceAABB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// -----------------------------------------------------------------------------------------------------------------------------------------
//											M	A	T	R	I	X		P	I	S	N	A	M	E
// -----------------------------------------------------------------------------------------------------------------------------------------
DxPieceManager::MATRIXPISNAME::MATRIXPISNAME() :
	m_pPisName(NULL),
	m_pPiece(NULL)
{
}

DxPieceManager::MATRIXPISNAME::~MATRIXPISNAME()
{
	CleanUp();
}

void DxPieceManager::MATRIXPISNAME::CleanUp()
{
	m_pPiece = NULL;	// m_mapNamePiece 에 넘겨주기 위해서 존재 했었다.
	SAFE_DELETE_ARRAY( m_pPisName );
}

void DxPieceManager::MATRIXPISNAME::Save( CSerialFile& SFile, const D3DXMATRIX& matLocal, const D3DXMATRIX& matFrame )
{
	// Note : PisEdit * 자신 * Frame = 값.
	//		
	D3DXMATRIX matInverse;
	D3DXMatrixInverse( &matInverse, NULL, &matFrame );
	D3DXMatrixMultiply( &m_matWorld, &m_pPiece->m_matWorld, &matInverse );

	D3DXMatrixInverse( &matInverse, NULL, &matLocal );
	D3DXMatrixMultiply( &m_matWorld, &matInverse, &m_matWorld );
	
	SFile.WriteBuffer( m_matWorld, sizeof(D3DXMATRIX) );

	DWORD dwSize = 0;
	if( m_pPisName )	dwSize = (DWORD)strlen(m_pPisName) + 1;
	else				dwSize = 0;
	SFile << dwSize;
	if( dwSize )
	{
		SFile.WriteBuffer( m_pPisName, sizeof(char)*dwSize );
	}
}

void DxPieceManager::MATRIXPISNAME::Load( CSerialFile& SFile )
{
	CleanUp();

	SFile.ReadBuffer( m_matWorld, sizeof(D3DXMATRIX) );

	DWORD dwSize;
	SFile >> dwSize;
	if( dwSize )
	{
		m_pPisName = new char[dwSize];
		SFile.ReadBuffer( m_pPisName, sizeof(char)*dwSize );
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------------
//										D	x		P	i	e	c	e		M	a	n	a	g	e	r
// -----------------------------------------------------------------------------------------------------------------------------------------

DxPieceManager::DxPieceManager() :
	m_fTime(0.f),
	m_fElapsedTime(0.f),
	m_pPiecePick(NULL),
	m_pMultiPieceAABB(NULL),
	unknown_var1(FALSE),
	unknown_var2(0.0f)
{
}

DxPieceManager::~DxPieceManager()
{
	CleanUp();
}

void DxPieceManager::CleanUp()
{
	m_fTime = 0.f;
	m_fElapsedTime = 0.f;

	std::for_each( m_mapChangePis.begin(), m_mapChangePis.end(), std_afunc::DeleteMapObject() );
	m_mapChangePis.clear();

	std::for_each( m_vecPointPis.begin(), m_vecPointPis.end(), std_afunc::DeleteObject() );
	m_vecPointPis.clear();

	MAPNAMEPIECE_ITER iter = m_mapNamePiece.begin();
	for( ; iter!=m_mapNamePiece.end(); ++iter )
	{
		(*iter).second->ClearTree();
	}

	std::for_each( m_mapNamePiece.begin(), m_mapNamePiece.end(), std_afunc::DeleteMapObject() );
	m_mapNamePiece.clear();

	SAFE_DELETE( m_pMultiPieceAABB );
}

void DxPieceManager::FrameMove( const float fTime, const float fElapsedTime )
{
	m_fTime = fTime;
	m_fElapsedTime = fElapsedTime;

	if( m_pMultiPieceAABB )	m_pMultiPieceAABB->FrameMove( fTime, fElapsedTime );
}

void DxPieceManager::Render( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	m_pPiecePick = NULL;

	pd3dDevice->LightEnable( 3, FALSE );
	pd3dDevice->LightEnable( 4, FALSE );
	pd3dDevice->LightEnable( 5, FALSE );
	pd3dDevice->LightEnable( 6, FALSE );
	pd3dDevice->LightEnable( 7, FALSE );

	DxPieceQuickSort::InitPickAlpha();		// Note : Init Static

	D3DMATERIALQ sMaterial;
	D3DUtil_InitMaterial( sMaterial, 1.f, 1.f, 1.0f, 1.f );
	switch( DxMeshes::g_emMeshMeterial )
	{
	case EM_MESH_MATERIAL_RED:
		sMaterial.Ambient.r = sMaterial.Diffuse.r = 1.f;
		sMaterial.Ambient.g = sMaterial.Diffuse.g = DxMeshes::g_fAlpha;
		sMaterial.Ambient.b = sMaterial.Diffuse.b = DxMeshes::g_fAlpha;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	};

	MAPNAMEPIECE_ITER iter = m_mapNamePiece.begin();
	for( ; iter!=m_mapNamePiece.end(); ++iter )
	{
		(*iter).second->Render( pd3dDevice, m_pPiecePick, sCV, m_fTime, m_fElapsedTime, material );
	}

	if( m_pMultiPieceAABB )	m_pMultiPieceAABB->Render( pd3dDevice, sCV, material );

	DxPieceQuickSort::FrameMoveONLY( m_fTime, m_fElapsedTime );	// Note : FrameMove Static

	switch( DxMeshes::g_emMeshMeterial )
	{
	case EM_MESH_MATERIAL_RED:
		sMaterial.Ambient.r = sMaterial.Diffuse.r = 1.f;
		sMaterial.Ambient.g = sMaterial.Diffuse.g = 1.f;
		sMaterial.Ambient.b = sMaterial.Diffuse.b = 1.f;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	};

	// Note : 나중 테스트 할때 써먹자
	//CLIPVOLUME	sCV;

	//MAPCHANGEPIS_ITER iter = m_mapChangePis.begin();
	//for( ; iter!=m_mapChangePis.end(); ++iter )
	//{
	//	DxPiece* pPiece = (*iter).second->m_pPiece;
	//	if( pPiece )	pPiece->Render( pd3dDevice, sCV, (*iter).second->m_matWorld );
	//}
}

void DxPieceManager::RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
								 DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material )
{
	pd3dDevice->LightEnable( 3, FALSE );
	pd3dDevice->LightEnable( 4, FALSE );
	pd3dDevice->LightEnable( 5, FALSE );
	pd3dDevice->LightEnable( 6, FALSE );
	pd3dDevice->LightEnable( 7, FALSE );

	D3DMATERIALQ sMaterial;
	D3DUtil_InitMaterial( sMaterial, 1.f, 1.f, 1.0f, 1.f );
	switch( DxMeshes::g_emMeshMeterial )
	{
	case EM_MESH_MATERIAL_RED:
		sMaterial.Ambient.r = sMaterial.Diffuse.r = 1.f;
		sMaterial.Ambient.g = sMaterial.Diffuse.g = DxMeshes::g_fAlpha;
		sMaterial.Ambient.b = sMaterial.Diffuse.b = DxMeshes::g_fAlpha;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	};

	MAPNAMEPIECE_ITER iter = m_mapNamePiece.begin();
	for( ; iter!=m_mapNamePiece.end(); ++iter )
	{
		(*iter).second->RenderAlpha( pd3dDevice, sCV, material );
	}

	iter = m_mapNamePiece.begin();
	for( ; iter!=m_mapNamePiece.end(); ++iter )
	{
		(*iter).second->RenderEff( pd3dDevice, sCV, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh, material );
	}

	if ( m_pMultiPieceAABB )
	{
		m_pMultiPieceAABB->RenderAlpha( pd3dDevice, sCV, material );
		m_pMultiPieceAABB->RenderEff( pd3dDevice, sCV, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh, material );
	}

	switch( DxMeshes::g_emMeshMeterial )
	{
	case EM_MESH_MATERIAL_RED:
		sMaterial.Ambient.r = sMaterial.Diffuse.r = 1.f;
		sMaterial.Ambient.g = sMaterial.Diffuse.g = 1.f;
		sMaterial.Ambient.b = sMaterial.Diffuse.b = 1.f;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	};
}

void DxPieceManager::RenderPickAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
									 DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material )
{
	DxPieceQuickSort::RenderPickAlpha( pd3dDevice, sCV, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh, material );
}

void DxPieceManager::Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material )
{
	D3DMATERIALQ sMaterial;
	D3DUtil_InitMaterial( sMaterial, 1.f, 1.f, 1.0f, 1.f );
	switch( DxMeshes::g_emMeshMeterial )
	{
	case EM_MESH_MATERIAL_RED:
		sMaterial.Ambient.r = sMaterial.Diffuse.r = 1.f;
		sMaterial.Ambient.g = sMaterial.Diffuse.g = DxMeshes::g_fAlpha;
		sMaterial.Ambient.b = sMaterial.Diffuse.b = DxMeshes::g_fAlpha;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	};

	// Note : View Volume을 변환 시킨다.
	D3DXVECTOR3&	vFromPt		= DxViewPort::GetInstance().GetFromPt();
	D3DXVECTOR3&	vLookatPt	= DxViewPort::GetInstance().GetLookatPt();

	D3DXVECTOR3	vUpVec ( 0.f, 1.f, 0.f );
	D3DXVECTOR3	vTempFromPt		= vFromPt;
	D3DXVECTOR3	vTempLookatPt	= vLookatPt;

	vTempFromPt.y = vTempFromPt.y - (DxEnvironment::GetInstance().GetCenter().y*2.f);
	vTempLookatPt.y = vTempLookatPt.y - (DxEnvironment::GetInstance().GetCenter().y*2.f);

	CLIPVOLUME sCV = DxViewPort::GetInstance().ComputeClipVolume( vTempLookatPt, vTempFromPt, vUpVec, 1.f, FALSE );

	MAPNAMEPIECE_ITER iter = m_mapNamePiece.begin();
	for( ; iter!=m_mapNamePiece.end(); ++iter )
	{
		(*iter).second->Render_Reflect( pd3dDevice, sCV, material );
	}

	if ( m_pMultiPieceAABB )	m_pMultiPieceAABB->Render_Reflect( pd3dDevice, sCV, material );

	switch( DxMeshes::g_emMeshMeterial )
	{
	case EM_MESH_MATERIAL_RED:
		sMaterial.Ambient.r = sMaterial.Diffuse.r = 1.f;
		sMaterial.Ambient.g = sMaterial.Diffuse.g = 1.f;
		sMaterial.Ambient.b = sMaterial.Diffuse.b = 1.f;
		pd3dDevice->SetMaterial( &sMaterial );
		break;
	};
}

BOOL DxPieceManager::IsGetName( DWORD nNUM, LPCSTR& pPieceName, LPCSTR& pFrameName )
{
	if( nNUM >= m_mapChangePis.size()+m_vecPointPis.size() )	return FALSE;

	if( m_vecPointPis.size() > nNUM )
	{
		VECPOINTPIS_ITER iter = m_vecPointPis.begin();
		for( DWORD i=0; i<nNUM; ++i )		++iter;

		pPieceName = (*iter)->m_pPisName;
		pFrameName = NULL;

		return TRUE;
	}
	else
	{
		MAPCHANGEPIS_ITER iter = m_mapChangePis.begin();
		for( DWORD i=(DWORD)m_vecPointPis.size(); i<nNUM; ++i )	++iter;

		pPieceName = (*iter).second->m_pPisName;
		pFrameName = (*iter).first.c_str();

		return TRUE;
	}
}

// m_mapChangePis 에 관련된 일만 한다.
void DxPieceManager::SetPiece( LPDIRECT3DDEVICEQ pd3dDevice, DxFrameMesh* pFrameMesh, const char* szFrameName, const char* szPisName )
{
	if( !szPisName )	return;

	DxFrame* pFrame = pFrameMesh->FindFrame( szFrameName );
	if( !pFrame )		return;

	// Note : Edit를 위한 구조체에 데이터 삽입
	MAPCHANGEPIS_ITER iter = m_mapChangePis.find( szFrameName );
	if( iter!=m_mapChangePis.end() )
	{
		// Note : 변환 Pis가 없거나 이름이 똑같다. ㅡㅡ;
		if( !strcmp( (*iter).second->m_pPisName, szPisName ) )	return;

		// Note : PieceContainer의 Count를 줄인다.
		DxPieceContainer::GetInstance().ReleasePiece( (*iter).second->m_pPisName );
		
		// Note : Piece File 이름을 바꾼다.
		size_t nCount = strlen( szPisName ) + 1;
		SAFE_DELETE_ARRAY( (*iter).second->m_pPisName );
		(*iter).second->m_pPisName = new char[ nCount ];
		StringCchCopy( (*iter).second->m_pPisName, nCount, szPisName );

		// PieceEdit 로드 후 Piece에 셋팅
		DxPieceEdit*	pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, szPisName );
		(*iter).second->m_pPiece->Import( pd3dDevice, pPieceEdit, (*iter).second->m_matWorld, pFrame->matCombined );
	}
	else
	{
		MATRIXPISNAME* pNew = new MATRIXPISNAME;
		D3DXMatrixIdentity( &pNew->m_matWorld );

		size_t nCount = strlen( szPisName ) + 1;
		SAFE_DELETE_ARRAY( pNew->m_pPisName );
		pNew->m_pPisName = new char[nCount];
		StringCchCopy( pNew->m_pPisName, nCount, szPisName );

		// PieceEdit 로드 후 Piece에 셋팅
		DxPieceEdit*	pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, szPisName );
		pNew->m_pPiece = new DxPiece;
		pNew->m_pPiece->Import( pd3dDevice, pPieceEdit, pNew->m_matWorld, pFrame->matCombined );

		m_mapChangePis.insert( std::make_pair( szFrameName, pNew ) );
	}

	MakeTree();

	// Note : Frame이 Piece로 사용한다고 전한다.
	pFrameMesh->AddPiece2( szFrameName );
}

void DxPieceManager::DeletePiece( DxFrameMesh* pFrameMesh, const char* szFrameName )
{
	// Note : 데이터 삭제
	MAPCHANGEPIS_ITER iter_DEL;
	MAPCHANGEPIS_ITER iter = m_mapChangePis.find( szFrameName );
	if( iter!=m_mapChangePis.end() )
	{
		// Release
		DxPieceContainer::GetInstance().ReleasePiece( (*iter).second->m_pPisName );

		SAFE_DELETE( (*iter).second );

		m_mapChangePis.erase( iter );
	}

	MakeTree();

	// Note : Frame에 Piece로 사용이 해제 되었다고 전한다.
	pFrameMesh->DelPiece2( szFrameName );
}

// m_vecPointPis에 관련된 일만 한다.
void DxPieceManager::SetPiece( LPDIRECT3DDEVICEQ pd3dDevice, const char* szPisName, const D3DXMATRIX& matWorld )
{
	if( !szPisName )	return;

	MATRIXPISNAME* pNew = new MATRIXPISNAME;
	pNew->m_matWorld = matWorld;

	size_t nCount = strlen( szPisName ) + 1;
	SAFE_DELETE_ARRAY( pNew->m_pPisName );
	pNew->m_pPisName = new char[nCount];
	StringCchCopy( pNew->m_pPisName, nCount, szPisName );

	// PieceEdit 로드 후 Piece에 셋팅
	DxPieceEdit*	pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, szPisName );
	pNew->m_pPiece = new DxPiece;
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );
	pNew->m_pPiece->Import( pd3dDevice, pPieceEdit, pNew->m_matWorld, matIdentity );

	// vector 에 삽입을 해 놓는다.
	m_vecPointPis.push_back( pNew );

	MakeTree();
}

void DxPieceManager::DeletePiece( DWORD dwNumber )
{
	VECPOINTPIS_ITER iter = m_vecPointPis.begin();
	for( DWORD i=0; i<dwNumber; ++i )	++iter;

	MATRIXPISNAME* pDelete = (*iter);
	SAFE_DELETE( pDelete );

	m_vecPointPis.erase( iter );

	MakeTree();
}

void DxPieceManager::MakeTree()
{
	// Note : Tree 를 삭제. List는 참조한 것이므로 삭제하면 안됨.
	MAPNAMEPIECE_ITER iter = m_mapNamePiece.begin();
	for( ; iter!=m_mapNamePiece.end(); ++iter )
	{
		(*iter).second->ClearOnlyTree();
	}
	m_mapNamePiece.clear();

	// Note : All Setting
	MAPCHANGEPIS_ITER iter_base = m_mapChangePis.begin();
	for( ; iter_base!=m_mapChangePis.end(); ++iter_base )
	{
		// Note : 없다면 작업 취소
		if( !(*iter_base).second->m_pPiece )	continue;

		iter = m_mapNamePiece.find( (*iter_base).second->m_pPisName );
		if( iter!=m_mapNamePiece.end() )
		{
			(*iter).second->SetPiece( (*iter_base).second->m_pPiece );
		}
		else
		{
			// Note : 새로운 넘이다.
			DxPieceAABB* pNew = new DxPieceAABB;

			pNew->SetPiece( (*iter_base).second->m_pPiece );

			m_mapNamePiece.insert( std::make_pair( (*iter_base).second->m_pPisName, pNew ) );
		}
	}
	VECPOINTPIS_ITER iter_point = m_vecPointPis.begin();
	for( ; iter_point!=m_vecPointPis.end(); ++iter_point )
	{
		// Note : 없다면 작업 취소
		if( !(*iter_point)->m_pPiece )	continue;

		iter = m_mapNamePiece.find( (*iter_point)->m_pPisName );
		if( iter!=m_mapNamePiece.end() )
		{
			(*iter).second->SetPiece( (*iter_point)->m_pPiece );
		}
		else
		{
			// Note : 새로운 넘이다.
			DxPieceAABB* pNew = new DxPieceAABB;

			pNew->SetPiece( (*iter_point)->m_pPiece );

			m_mapNamePiece.insert( std::make_pair( (*iter_point)->m_pPisName, pNew ) );
		}
	}

	// Note : Create AABB Tree
	iter = m_mapNamePiece.begin();
	for( ; iter!=m_mapNamePiece.end(); ++iter )
	{
		(*iter).second->MakeTree();
	}
}

D3DXMATRIX* DxPieceManager::MoveCamera( DxFrame* pFrameRoot, const char* szFrameName )
{
	MAPCHANGEPIS_ITER iter = m_mapChangePis.find( szFrameName );
	if( iter!=m_mapChangePis.end() )
	{
		D3DXVECTOR3 vPos;
		vPos.x = (*iter).second->m_pPiece->m_matWorld._41;
		vPos.y = (*iter).second->m_pPiece->m_matWorld._42;
		vPos.z = (*iter).second->m_pPiece->m_matWorld._43;

		// Note : 카메라를 이동해야지.
		DxViewPort::GetInstance().CameraJump( vPos );

		return &(*iter).second->m_pPiece->m_matWorld;
	}
	
	return NULL;
}

D3DXMATRIX* DxPieceManager::MoveCamera( DWORD dwNumber )
{
	VECPOINTPIS_ITER iter = m_vecPointPis.begin();
	for( DWORD i=0; i<dwNumber; ++i )	++iter;

	D3DXVECTOR3 vPos;
	vPos.x = (*iter)->m_pPiece->m_matWorld._41;
	vPos.y = (*iter)->m_pPiece->m_matWorld._42;
	vPos.z = (*iter)->m_pPiece->m_matWorld._43;

	// Note : 카메라를 이동해야지.
	DxViewPort::GetInstance().CameraJump( vPos );

	return &(*iter)->m_pPiece->m_matWorld;
}

BOOL DxPieceManager::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision )
{
	BOOL bCollision(FALSE);

	MAPNAMEPIECE_ITER iter = m_mapNamePiece.begin();
	for( ; iter!=m_mapNamePiece.end(); ++iter )
	{
		if( (*iter).second->IsCollisionLine( vStart, vEnd, vCollision ) )	bCollision = TRUE;
	}

	if ( m_pMultiPieceAABB )
	{
		if ( m_pMultiPieceAABB->IsCollisionLine( vStart, vEnd, vCollision ) )	bCollision = TRUE;
	}

	return bCollision;
}



void DxPieceManager::ClonePSFtoWLD( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceManager* pSrc )
{
	CleanUp();

	MAPNAMEPIECE_ITER iter = pSrc->m_mapNamePiece.begin();
	for( ; iter!=pSrc->m_mapNamePiece.end(); ++iter )
	{
		DxPieceAABB* pNew = new DxPieceAABB;
		pNew->CloneData( pd3dDevice, (*iter).second );

		m_mapNamePiece.insert( std::make_pair( (*iter).first.c_str(), pNew ) );
	}
}


