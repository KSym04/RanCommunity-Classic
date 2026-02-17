#include "stdafx.h"
#include "./DxPieceManager.h"

#include "./DxPiece.h"
#include "./DxPieceAABB.h"
#include "./DxMultiPieceAABB.h"
#include "./DxPieceContainer.h"
#include "../Meshs/DxFrameMesh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD DxPieceManager::VERSION_PSF				= 0x0101;
const DWORD DxPieceManager::VERSION_PIECE_MANAGER	= 0x0200;

void DxPieceManager::SavePSF( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxFrameMesh* pFrameMesh )
{
	D3DXMATRIX matIdentity;
	D3DXMATRIX* pmatPisEdit, *pmatFrame;
	D3DXMatrixIdentity( &matIdentity );

	DxFrame*		pFrame(NULL);
	DxPieceEdit*	pPieceEdit(NULL);

	SFile << VERSION_PSF;

	SFile.BeginBlock( EMBLOCK_00 );
	{
		DWORD dwSize = (DWORD)m_vecPointPis.size();
		SFile << dwSize;

		VECPOINTPIS_ITER iter = m_vecPointPis.begin();
		for( ; iter!=m_vecPointPis.end(); ++iter )
		{
			pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, (*iter)->m_pPisName );
			{
				if( pPieceEdit )	pmatPisEdit = pPieceEdit->GetMatrixLocal();
				else				pmatPisEdit = &matIdentity;

				(*iter)->Save( SFile, *pmatPisEdit, matIdentity );
			}
			DxPieceContainer::GetInstance().ReleasePiece( (*iter)->m_pPisName );
		}
	}
	{
		DWORD dwSize = (DWORD)m_mapChangePis.size();
		SFile << dwSize;

		MAPCHANGEPIS_ITER iter = m_mapChangePis.begin();
		for( ; iter!=m_mapChangePis.end(); ++iter )
		{
			SFile << (*iter).first;

			pFrame = pFrameMesh->FindFrame( (*iter).first.c_str() );
			pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, (*iter).second->m_pPisName );
			{
				if( pPieceEdit )	pmatPisEdit = pPieceEdit->GetMatrixLocal();
				else				pmatPisEdit = &matIdentity;

				if( pFrame )		pmatFrame = &pFrame->matCombined;
				else				pmatFrame = &matIdentity;

				(*iter).second->Save( SFile, *pmatPisEdit, *pmatFrame );
			}
			DxPieceContainer::GetInstance().ReleasePiece( (*iter).second->m_pPisName );
		}
	}
	SFile.EndBlock( EMBLOCK_00 );
}

void DxPieceManager::LoadPSF( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxFrameMesh* pFrameMesh )
{
	CleanUp();

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );

	DWORD dwVer, dwBufferSize;
	SFile >> dwVer;
	SFile >> dwBufferSize;

	DWORD dwSize = 0;
	{
		SFile >> dwSize;

		for( DWORD i=0; i<dwSize; ++i )
		{
			MATRIXPISNAME* pNew = new MATRIXPISNAME;
			pNew->Load( SFile );
			m_vecPointPis.push_back( pNew );

			DxPieceEdit* pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, pNew->m_pPisName );
			if( !pPieceEdit )
			{
				std::string strName = "조각파일 : ";
				strName += pNew->m_pPisName;
				strName += " 이 없습니다.";
				MessageBox( NULL, strName.c_str(), "ERROR", MB_OK );
				continue;
			}

			pNew->m_pPiece = new DxPiece;
			pNew->m_pPiece->Import( pd3dDevice, pPieceEdit, pNew->m_matWorld, matIdentity );
		}
	}
	{
		SFile >> dwSize;

		std::string szFrameName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> szFrameName;

			// Note : szFrameName이 있는지 없는지를 검사한다. 
			//			만약 없다면 로딩만 한 후 건너띤다.
			DxFrame* pFrame = pFrameMesh->FindFrame( szFrameName.c_str() );
			if( !pFrame )
			{
				MATRIXPISNAME sNew;
				sNew.Load( SFile );
				continue;
			}

			MATRIXPISNAME* pNew = new MATRIXPISNAME;
			pNew->Load( SFile );
			m_mapChangePis.insert( std::make_pair( szFrameName, pNew ) );

			DxPieceEdit* pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, pNew->m_pPisName );
			if( !pPieceEdit )
			{
				std::string strName = "조각파일 : ";
				strName += pNew->m_pPisName;
				strName += " 이 없습니다.";
				MessageBox( NULL, strName.c_str(), "ERROR", MB_OK );
				continue;
			}

			pNew->m_pPiece = new DxPiece;
			pNew->m_pPiece->Import( pd3dDevice, pPieceEdit, pNew->m_matWorld, pFrame->matCombined );
		}
	}

	MakeTree();

	// Note : 깔끔하게 지운다.
	pFrameMesh->SetPieceUseFALSE();

	// Note : Src Frame에 Piece를 사용하고 있다고 알린다.
	MAPCHANGEPIS_ITER iter = m_mapChangePis.begin();
	for( ; iter!=m_mapChangePis.end(); ++iter )
	{
		pFrameMesh->AddPiece2( (*iter).first.c_str() );
	}
}

void DxPieceManager::LoadPSF_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxFrameMesh* pFrameMesh )
{
	CleanUp();

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );

	DWORD dwSize = 0;
	{
		SFile >> dwSize;

		for( DWORD i=0; i<dwSize; ++i )
		{
			MATRIXPISNAME* pNew = new MATRIXPISNAME;
			pNew->Load( SFile );
			m_vecPointPis.push_back( pNew );

			DxPieceEdit* pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, pNew->m_pPisName );
			if( !pPieceEdit )
			{
				std::string strName = "조각파일 : ";
				strName += pNew->m_pPisName;
				strName += " 이 없습니다.";
				MessageBox( NULL, strName.c_str(), "ERROR", MB_OK );
				continue;
			}

			pNew->m_pPiece = new DxPiece;
			pNew->m_pPiece->Import( pd3dDevice, pPieceEdit, pNew->m_matWorld, matIdentity );
		}
	}
	{
		SFile >> dwSize;

		std::string szFrameName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> szFrameName;

			// Note : szFrameName이 있는지 없는지를 검사한다. 
			//			만약 없다면 로딩만 한 후 건너띤다.
			DxFrame* pFrame = pFrameMesh->FindFrame( szFrameName.c_str() );
			if( !pFrame )
			{
				MATRIXPISNAME sNew;
				sNew.Load( SFile );
				continue;
			}

			MATRIXPISNAME* pNew = new MATRIXPISNAME;
			pNew->Load( SFile );
			pNew->m_matWorld = matIdentity;
			m_mapChangePis.insert( std::make_pair( szFrameName, pNew ) );

			DxPieceEdit* pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, pNew->m_pPisName );
			if( !pPieceEdit )
			{
				std::string strName = "조각파일 : ";
				strName += pNew->m_pPisName;
				strName += " 이 없습니다.";
				MessageBox( NULL, strName.c_str(), "ERROR", MB_OK );
				continue;
			}

			pNew->m_pPiece = new DxPiece;
			pNew->m_pPiece->Import( pd3dDevice, pPieceEdit, pNew->m_matWorld, pFrame->matCombined );
		}
	}

	MakeTree();

	// Note : 깔끔하게 지운다.
	pFrameMesh->SetPieceUseFALSE();

	// Note : Src Frame에 Piece를 사용하고 있다고 알린다.
	MAPCHANGEPIS_ITER iter = m_mapChangePis.begin();
	for( ; iter!=m_mapChangePis.end(); ++iter )
	{
		pFrameMesh->AddPiece2( (*iter).first.c_str() );
	}
}


void DxPieceManager::LoadWLD_OLD( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();

	DWORD dwSize = 0;
	SFile >> dwSize;

	std::string szPieceName;
	for( DWORD i=0; i<dwSize; ++i )
	{
		SFile >> szPieceName;

		DxPieceAABB* pNew = new DxPieceAABB;
		pNew->Load_OLD( pd3dDevice, SFile, szPieceName.c_str() );

		m_mapNamePiece.insert( std::make_pair( szPieceName.c_str(), pNew ) );
	}
}

void DxPieceManager::SaveWLD( CSerialFile& SFile )
{
	SFile << (DWORD)VERSION_PIECE_MANAGER;

	SFile.BeginBlock(EMBLOCK_00);
	{
		SFile << unknown_var1;
		SFile << unknown_var2;

		DWORD dwSize = (DWORD)m_mapNamePiece.size();
		SFile << dwSize;

		MAPNAMEPIECE_ITER iter = m_mapNamePiece.begin();
		for( ; iter!=m_mapNamePiece.end(); ++iter )
		{
			SFile << (*iter).first;
			(*iter).second->Save( SFile );
		}

		if( m_pMultiPieceAABB )
		{
			SFile << (BOOL)TRUE;
			m_pMultiPieceAABB->Save( SFile );
		}
		else
		{
			SFile << (BOOL)FALSE;
		}
	}
	SFile.EndBlock(EMBLOCK_00);
}

void DxPieceManager::LoadWLD_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();

	DWORD dwVER(0);
	DWORD dwSIZE(0);

	SFile >> dwVER;
	SFile >> dwSIZE;

	if( dwVER == VERSION_PIECE_MANAGER )
	{
		SFile >> unknown_var1;
		SFile >> unknown_var2;

		DWORD dwNUM(0);
		SFile >> dwNUM;
		for( DWORD i=0; i<dwNUM; ++i )
		{
			std::string szPieceName;
			SFile >> szPieceName;

			DxPieceAABB* pNew = new DxPieceAABB;
			pNew->Load_STABLE( pd3dDevice, SFile, szPieceName.c_str() );
			m_mapNamePiece.insert( std::make_pair( szPieceName.c_str(), pNew ) );
		}

		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			m_pMultiPieceAABB = new DxMultiPieceAABB;
			m_pMultiPieceAABB->Load( pd3dDevice, SFile );
		}
	}
	else if( dwVER == 0x0104 )
	{
		SFile >> unknown_var1;

		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			m_pMultiPieceAABB = new DxMultiPieceAABB;
			m_pMultiPieceAABB->Load( pd3dDevice, SFile );
		}

		SFile >> unknown_var2;
	}
	else if( dwVER == 0x0102 || dwVER == 0x0103 )
	{
		SFile >> unknown_var1;

		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			m_pMultiPieceAABB = new DxMultiPieceAABB;
			m_pMultiPieceAABB->Load( pd3dDevice, SFile );
		}
	}
	else if( dwVER == 0x0101 )
	{
		SFile >> unknown_var1;

		DWORD dwNUM(0);
		SFile >> dwNUM;

		for( DWORD i=0; i<dwNUM; ++i )
		{
			std::string szPieceName;
			SFile >> szPieceName;

			DxPieceAABB* pNew = new DxPieceAABB;
			pNew->Load_STABLE( pd3dDevice, SFile, szPieceName.c_str() );
			m_mapNamePiece.insert( std::make_pair( szPieceName.c_str(), pNew ) );
		}
	}
	else if( dwVER == 0x0100 )
	{
		DWORD dwNUM(0);
		SFile >> dwNUM;

		for( DWORD i=0; i<dwNUM; ++i )
		{
			std::string szPieceName;
			SFile >> szPieceName;

			DxPieceAABB* pNew = new DxPieceAABB;
			pNew->Load_STABLE( pd3dDevice, SFile, szPieceName.c_str() );
			m_mapNamePiece.insert( std::make_pair( szPieceName.c_str(), pNew ) );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxPieceManager::LoadWLD_STABLE", SFile.GetFileName(), dwVER );
		DWORD dwCur = SFile.GetfTell ();
		SFile.SetOffSet ( dwCur+dwSIZE );
	}	
}
