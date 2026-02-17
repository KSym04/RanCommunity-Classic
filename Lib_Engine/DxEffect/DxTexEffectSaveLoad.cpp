#include "stdafx.h"
#include "DxTexEffect.h"

#include "../Common/SerialFile.h"

#include "./DxTexEff.h"
#include "./DxTexEffMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL DxTexEffect::Load_File( LPDIRECT3DDEVICEQ pd3dDevice, const char* szName )
{
	std::string strName = szName;

	if ( strName.empty() )	return FALSE;

	std::string::size_type idx;
	idx = strName.find( ".wld" );
	if( idx!=std::string::npos )	strName += '1';	

	CSerialFile SFile;
	if ( SFile.OpenFile ( FOT_READ, strName.c_str() ) )	
	{
		Load( pd3dDevice, SFile );	
		return TRUE;
	}

	return FALSE;
}

void DxTexEffect::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();

	DWORD dwVer, dwSize;
	SFile >> dwVer;
	SFile >> dwSize;

	if ( dwVer >= VERSION_ENCODE )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_2_VER_1 );

	if ( dwVer >= VERSION_ENCODE_2 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_3_VER_1 );

	if ( dwVer >= VERSION_ENCODE_3 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_3_VER_2 );

	if ( dwVer >= VERSION_ENCODE_4 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_3_VER_3 );

	if ( dwVer >= ENCODE_VER_BYTE_CRYPT_2021_WLD3_V1 )
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_WLD3_V1 );

	if ( dwVer == VERSION_TEX_EFFECT )
	{
		Load_Ver_0204( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0203 )
	{
		Load_Ver_0203( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0202 )
	{
		Load_Ver_0202( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0201 )
	{
		Load_Ver_0201( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0200 )
	{
		Load_Ver_0200( pd3dDevice, SFile );
	}
	else if ( dwVer == 0x0100 )
	{
		Load_Ver_0100( pd3dDevice, SFile );
	}
	else
	{
		CDebugSet::ErrorVersion( "DxTexEffect::Load", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwSize );
	}
}

void DxTexEffect::Load_Ver_0100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;
	for ( DWORD i=0; i<dwNum; ++i )
	{
		std::string strEffectTarget("");
		DxTexEffBase*	pTexEffList = NULL;

		SFile >> strEffectTarget;

		DWORD dwEffectNum(0);
		SFile >> dwEffectNum;
		for ( DWORD j=0; j<dwEffectNum; ++j )
		{
			DWORD dwEffectType(0);
			SFile >> dwEffectType;

			DxTexEffBase* pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwEffectType, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = pTexEffList;
			pTexEffList = pTexEffBase;
		}

		if ( pTexEffList )
		{
			m_mapTextEff.insert( std::make_pair( strEffectTarget, pTexEffList ) );
		}
	}
}

void DxTexEffect::Load_Ver_0200( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;
	for ( DWORD i=0; i<dwNum; ++i )
	{
		std::string strEffectTarget("");
		DxTexEffBase*	pTexEffList = NULL;

		SFile >> strEffectTarget;

		DWORD dwEffectNum(0);
		SFile >> dwEffectNum;
		for ( DWORD j=0; j<dwEffectNum; ++j )
		{
			DWORD dwEffectType(0);
			SFile >> dwEffectType;

			DxTexEffBase* pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwEffectType, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = pTexEffList;
			pTexEffList = pTexEffBase;
		}

		if ( pTexEffList )
		{
			m_mapTextEff.insert( std::make_pair( strEffectTarget, pTexEffList ) );
		}
	}
}

void DxTexEffect::Load_Ver_0201( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;
	for ( DWORD i=0; i<dwNum; ++i )
	{
		std::string strEffectTarget("");
		DxTexEffBase*	pTexEffList = NULL;

		SFile >> strEffectTarget;

		DWORD dwEffectNum(0);
		SFile >> dwEffectNum;
		for ( DWORD j=0; j<dwEffectNum; ++j )
		{
			DWORD dwEffectType(0);
			SFile >> dwEffectType;

			DxTexEffBase* pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwEffectType, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = pTexEffList;
			pTexEffList = pTexEffBase;
		}

		if ( pTexEffList )
		{
			m_mapTextEff.insert( std::make_pair( strEffectTarget, pTexEffList ) );
		}
	}
}

void DxTexEffect::Load_Ver_0202( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;
	for ( DWORD i=0; i<dwNum; ++i )
	{
		std::string strEffectTarget("");
		DxTexEffBase*	pTexEffList = NULL;

		SFile >> strEffectTarget;

		DWORD dwEffectNum(0);
		SFile >> dwEffectNum;
		for ( DWORD j=0; j<dwEffectNum; ++j )
		{
			DWORD dwEffectType(0);
			SFile >> dwEffectType;

			DxTexEffBase* pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwEffectType, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = pTexEffList;
			pTexEffList = pTexEffBase;
		}

		if ( pTexEffList )
		{
			m_mapTextEff.insert( std::make_pair( strEffectTarget, pTexEffList ) );
		}
	}
}

void DxTexEffect::Load_Ver_0203( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;
	for ( DWORD i=0; i<dwNum; ++i )
	{
		std::string strEffectTarget("");
		DxTexEffBase*	pTexEffList = NULL;

		SFile >> strEffectTarget;

		DWORD dwEffectNum(0);
		SFile >> dwEffectNum;
		for ( DWORD j=0; j<dwEffectNum; ++j )
		{
			DWORD dwEffectType(0);
			SFile >> dwEffectType;

			DxTexEffBase* pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwEffectType, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = pTexEffList;
			pTexEffList = pTexEffBase;
		}

		if ( pTexEffList )
		{
			m_mapTextEff.insert( std::make_pair( strEffectTarget, pTexEffList ) );
		}
	}
}

void DxTexEffect::Load_Ver_0204( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwNum(0);
	SFile >> dwNum;
	for ( DWORD i=0; i<dwNum; ++i )
	{
		std::string strEffectTarget("");
		DxTexEffBase*	pTexEffList = NULL;

		SFile >> strEffectTarget;

		DWORD dwEffectNum(0);
		SFile >> dwEffectNum;
		for ( DWORD j=0; j<dwEffectNum; ++j )
		{
			DWORD dwEffectType(0);
			SFile >> dwEffectType;

			DxTexEffBase* pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwEffectType, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = pTexEffList;
			pTexEffList = pTexEffBase;
		}

		if ( pTexEffList )
		{
			m_mapTextEff.insert( std::make_pair( strEffectTarget, pTexEffList ) );
		}
	}
}

void DxTexEffect::Save_File ( const char* szName )
{
	std::string strName = szName;

	std::string::size_type idx;
	idx = strName.find( ".wld" );
	if( idx!=std::string::npos )	strName += '1';

	CSerialFile SFile;
	if ( SFile.OpenFile ( FOT_WRITE, strName.c_str() ) )	
		Save( SFile );

	SFile.CloseFile();
}

void DxTexEffect::Save ( CSerialFile& SFile )
{
	DWORD	dwSize = 0;

	SFile << (DWORD)VERSION_TEX_EFFECT;

	SFile.BeginBlock();
	{
		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_WLD3_V1 );

		SFile << (DWORD)m_mapTextEff.size();

		for( MAPDXTEXEFF_ITER iter = m_mapTextEff.begin(); iter != m_mapTextEff.end(); ++iter )
		{
			std::string strEffectTarget = (*iter).first;
			DxTexEffBase*	pTexEffList = (*iter).second;
			if ( !pTexEffList )	continue;

			SFile << strEffectTarget;

			DWORD dwSize=0;
			DxTexEffBase* pCur = pTexEffList;
			while( pCur )
			{
				++dwSize;
				pCur = pCur->m_pNext;
			}
			SFile << dwSize;

			pCur = pTexEffList;
			while( pCur )
			{
				SFile << pCur->GetTypeID();

				pCur->SavePSF( SFile );
				pCur = pCur->m_pNext;
			}

		}


		SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_NONE );
	}
	SFile.EndBlock();
}