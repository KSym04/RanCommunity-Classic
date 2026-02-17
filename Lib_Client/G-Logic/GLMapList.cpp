#include "stdafx.h"
#include "./GLMapList.h"
#include "./GLGaeaServer.h"

#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/Common/StringUtils.h"
#include "../../Lib_Engine/Common/BaseString.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char* GLMapList::_FILEHEAD = "GLMAPS_LIST";

GLMapList::GLMapList(void)
{
}

GLMapList::~GLMapList(void)
{
}

BOOL GLMapList::IsRegMapNode ( SMAPNODE &MapCur )
{
	FIELDMAP_ITER iter = m_MapList.begin ();
	FIELDMAP_ITER iter_end = m_MapList.end ();

	for ( ; iter!=iter_end; ++iter )
	{
		const SMAPNODE &sMapNode = (*iter).second;

		if ( sMapNode.sNativeID==MapCur.sNativeID )		return TRUE;
		if ( MapCur.strFile==sMapNode.strFile )			return TRUE;
	}

	return FALSE;
}

//no longer used by server or client only for tool to import old ini
HRESULT GLMapList::LoadMapList_OLD ( const TCHAR* szMapListFile )
{
	GASSERT(szMapListFile);

	m_MapList.clear ();

	std::string strPath = szMapListFile;

	CIniLoader cFILE;

	std::string strSep( ",[]\t" );
	cFILE.reg_sep( strSep );

	if( !cFILE.open( strPath, true ) )
	{
		CDebugSet::ToLogFile ( "ERROR : GLMapList::LoadMapList_OLD(), File Open %s", szMapListFile );
		return E_FAIL;
	}

	INT nMapKeySize(0);
	cFILE.getflag( "MAP_LIST_INFO", "MapKeySize", 0, 1, nMapKeySize );

	DWORD dwNUM = cFILE.GetKeySize( "MAP_LIST", "MAP" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SMAPNODE sMapNode;
		cFILE.getflag( i, "MAP_LIST", "MAP", 0, nMapKeySize, sMapNode.bUsed );
		cFILE.getflag( i, "MAP_LIST", "MAP", 1, nMapKeySize, sMapNode.strFile );
		cFILE.getflag( i, "MAP_LIST", "MAP", 2, nMapKeySize, sMapNode.sNativeID.wMainID );
		cFILE.getflag( i, "MAP_LIST", "MAP", 3, nMapKeySize, sMapNode.sNativeID.wSubID );
		cFILE.getflag( i, "MAP_LIST", "MAP", 4, nMapKeySize, sMapNode.dwFieldSID );
		cFILE.getflag( i, "MAP_LIST", "MAP", 5, nMapKeySize, sMapNode.bPeaceZone );
		cFILE.getflag( i, "MAP_LIST", "MAP", 6, nMapKeySize, sMapNode.bCommission );
		cFILE.getflag( i, "MAP_LIST", "MAP", 7, nMapKeySize, sMapNode.bPKZone );
		cFILE.getflag( i, "MAP_LIST", "MAP", 8, nMapKeySize, sMapNode.bFreePK );
		cFILE.getflag( i, "MAP_LIST", "MAP", 9, nMapKeySize, sMapNode.bItemDrop );
		cFILE.getflag( i, "MAP_LIST", "MAP", 10, nMapKeySize, sMapNode.bMove );
		cFILE.getflag( i, "MAP_LIST", "MAP", 11, nMapKeySize, sMapNode.bRestart );
		cFILE.getflag( i, "MAP_LIST", "MAP", 12, nMapKeySize, sMapNode.bPetActivity );
		cFILE.getflag( i, "MAP_LIST", "MAP", 13, nMapKeySize, sMapNode.bDECEXP );
		cFILE.getflag( i, "MAP_LIST", "MAP", 14, nMapKeySize, sMapNode.bVehicleActivity );
		cFILE.getflag( i, "MAP_LIST", "MAP", 15, nMapKeySize, sMapNode.bClubBattleZone );				
		cFILE.getflag( i, "MAP_LIST", "MAP", 16, nMapKeySize, sMapNode.strMapName );
		cFILE.getflag( i, "MAP_LIST", "MAP", 17, nMapKeySize, sMapNode.strBGM );
		cFILE.getflag( i, "MAP_LIST", "MAP", 18, nMapKeySize, sMapNode.strLoadingImageName );
		cFILE.getflag( i, "MAP_LIST", "MAP", 19, nMapKeySize, sMapNode.bInstantMap );

		if ( !( sMapNode.sNativeID.wMainID<MAXLANDMID || sMapNode.sNativeID.wSubID<MAXLANDSID ) )
		{
			CDebugSet::MsgBox( "MapID Max Map(%d~%d) Map ID GuideLine is 0~%d / 0~%d.",
				sMapNode.sNativeID.wMainID, sMapNode.sNativeID.wSubID, MAXLANDMID, MAXLANDSID );
			continue;
		}

		if ( sMapNode.strMapName.length() > MAP_NAME_MAX )
		{
			CDebugSet::MsgBox( "MapName too long (%s) Map Name Limit is:%d", sMapNode.strMapName.c_str(), MAP_NAME_MAX );
			continue;
		}

		if ( IsRegMapNode(sMapNode) )
		{
			CDebugSet::MsgBox( "Map Duplicate(%d~%d~%s)", sMapNode.sNativeID.wMainID, sMapNode.sNativeID.wSubID, sMapNode.strMapName.c_str());
			continue;
		}


		m_MapList.insert ( std::make_pair(sMapNode.sNativeID.dwID,sMapNode) );
	}
	
	return S_OK;
}

HRESULT GLMapList::LoadMapsListFile ( const char* szFile, LPF_SERVER_INFO pServerInfo/*=NULL*/, int nServerNum/*=0*/, BOOL bTOOL /*= FALSE*/ )
{
	StringCchCopy(m_szFileName,MAX_PATH,szFile);

	char szFullPath[MAX_PATH] = {0};
	StringCchCopy ( szFullPath, MAX_PATH, GLOGIC::GetPath() );
	StringCchCat ( szFullPath, MAX_PATH, m_szFileName );

	std::auto_ptr<basestream> pBStream( GLOGIC::openfile_basestream(GLOGIC::bGLOGIC_ZIPFILE, 
										GLOGIC::strGLOGIC_ZIPFILE.c_str(),
										szFullPath, 
										szFile,
										EMRCRYPT_BYTE_TYPE_NONE,
										GLOGIC::bGLOGIC_PACKFILE ) );

	if ( !pBStream.get() )
	{
		if ( strlen( szFile ) > 0 )
			CDebugSet::ErrorFile( "GLMapList::LoadMapsListFile", szFile );

		return E_FAIL;
	}

	basestream &SFile = *pBStream;

	DWORD dwFILEVER;
	char szFILETYPE[_MAX_FNAME];
	SFile.GetFileType( szFILETYPE, _MAX_FNAME, dwFILEVER );

	if ( dwFILEVER>=VERSION_ENCODE_OLD )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_OLD );

	if ( dwFILEVER>=VERSION_ENCODE )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_MAPS_LIST_VER_1 );

	if ( dwFILEVER>=VERSION_ENCODE2 )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_MAPS_LIST_VER_2 );

	if ( dwFILEVER>=ENCODE_VER_BYTE_CRYPT_2021_MAPSLIST_V1 )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_2021_MAPS_LIST_V1 );

	m_MapList.clear ();

	DWORD dwNum(0);
	SFile >> dwNum;

	for ( DWORD i=0; i<dwNum; i++ )
	{
		SMAPNODE sMapNode;
		BOOL bOK = sMapNode.LOAD( SFile );
		if ( bOK )
		{
			if ( !bTOOL && !sMapNode.bUsed )	continue;

			if ( !( sMapNode.sNativeID.wMainID<MAXLANDMID || sMapNode.sNativeID.wSubID<MAXLANDSID ) )
			{
				CDebugSet::MsgBox( "MapID Max Map(%d~%d) Map ID GuideLine is 0~%d / 0~%d.",
					sMapNode.sNativeID.wMainID, sMapNode.sNativeID.wSubID, MAXLANDMID, MAXLANDSID );
				continue;
			}

			if ( sMapNode.strMapName.length() > MAP_NAME_MAX )
			{
				CDebugSet::MsgBox( "MapName too long (%s) Map Name Limit is:%d", sMapNode.strMapName.c_str(), MAP_NAME_MAX );
				continue;
			}

			if ( pServerInfo && nServerNum )
			{
				if ( sMapNode.dwFieldSID >= (DWORD)nServerNum )
				{
					CDebugSet::MsgBox( "FieldServer ID of Map (%s) is Overflow:%d / %d", sMapNode.strMapName.c_str(), sMapNode.dwFieldSID, nServerNum );
				}

				if ( !pServerInfo[0*MAX_CHANNEL_NUMBER+sMapNode.dwFieldSID].IsValid() )
				{
					CDebugSet::MsgBox( "Map (%s) Not valid FieldServer ID:%d", sMapNode.strMapName.c_str(), sMapNode.dwFieldSID );
				}
			}

			if ( IsRegMapNode(sMapNode) )
			{
				CDebugSet::MsgBox( "Map Duplicate(%d~%d~%s)", sMapNode.sNativeID.wMainID, sMapNode.sNativeID.wSubID, sMapNode.strMapName.c_str());
				continue;
			}

			if ( nServerNum )
			{
				GLLevelFile cLevelFile;
				BOOL bOK = cLevelFile.LoadFile ( sMapNode.strFile.c_str(), TRUE, NULL );
				if ( !bOK )
				{
					CONSOLEMSG_WRITE ( "cLevelFile.LoadFile(), %s fail", sMapNode.strFile.c_str() );
					continue;
				}

				sMapNode.sLEVEL_REQUIRE  = *cLevelFile.GetLevelRequire();
				sMapNode.sLEVEL_ETC_FUNC = *cLevelFile.GetLevelEtcFunc();
			}

			m_MapList.insert ( std::make_pair(sMapNode.sNativeID.dwID,sMapNode) );
		}
	}

	return S_OK;
}

HRESULT GLMapList::SaveMapsListFile ( const char* szFile )
{
	StringCchCopy(m_szFileName,MAX_PATH,szFile);

	char szFullPath[MAX_PATH];
	StringCchCopy ( szFullPath, MAX_PATH, GLOGIC::GetPath() );
	StringCchCat ( szFullPath, MAX_PATH, m_szFileName );

	CSerialFile SFile;
	SFile.SetFileType ( _FILEHEAD, VERSION );
	if ( !SFile.OpenFile ( FOT_WRITE, szFullPath ) )	return E_FAIL;
	SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_2021_MAPS_LIST_V1 );

	SFile << (DWORD)m_MapList.size();

	FIELDMAP_ITER iter = m_MapList.begin ();
	for ( ; iter!=m_MapList.end(); ++iter )
	{
		(*iter).second.SAVE( SFile ); 
	}

	return S_OK;
}

namespace SAVECSV
{
	typedef std::pair<DWORD,const SMAPNODE*> PAIR_FIELDMAP;
	typedef std::vector<PAIR_FIELDMAP>       VEC_SORT;
	typedef VEC_SORT::iterator               VEC_SORT_ITER;
	static VEC_SORT                          SortedVec;

	bool VEC_SORT_FUNCTOR ( const PAIR_FIELDMAP& lvalue, const PAIR_FIELDMAP& rvalue )
	{
		return lvalue.first < rvalue.first;
	}
};


bool GLMapList::SaveCsv ( std::string& strFileName )
{
	std::fstream streamFILE;
	streamFILE.open ( strFileName.c_str(), std::ios_base::out );

	using namespace SAVECSV;

	FIELDMAP_ITER iter     = m_MapList.begin();
	FIELDMAP_ITER iter_end = m_MapList.end();

	// Note : Vector에 넘긴뒤 정렬을 한다.
	SortedVec.clear();
	//SortedVec._Mylast = SortedVec._Myfirst;
	for ( ; iter != iter_end; ++iter ) 
	{ 
		SortedVec.push_back( PAIR_FIELDMAP((*iter).first, &(*iter).second) );
	}

	std::sort(SortedVec.begin(), SortedVec.end(), VEC_SORT_FUNCTOR);


	SMAPNODE::SaveCsvHead ( streamFILE );

	VEC_SORT_ITER veciter     = SortedVec.begin();
	VEC_SORT_ITER veciter_end = SortedVec.end();

	for ( ; veciter != veciter_end; ++veciter )
	{		
		const SMAPNODE* pMapNode = (*veciter).second;
		if ( !pMapNode ) continue;

		pMapNode->SaveCsv( streamFILE );
	}

	streamFILE.close();

	return true;
}

bool GLMapList::LoadCsv ( std::string& strFileName )
{
	CStringFile StrFile( 10240 );
	if( !StrFile.Open ( strFileName.c_str() ) )
		return false;

	m_MapList.clear();

	STRUTIL::ClearSeparator();
	STRUTIL::RegisterSeparator( "," );

	CString      strLine;
	CStringArray StrArray;

	//	Basic Head
	StrFile.GetNextLine( strLine );
	STRUTIL::StringSeparate( strLine, StrArray );
	int iHead = (int)StrArray.GetCount();

	while( StrFile.GetNextLine( strLine ) )
	{
		STRUTIL::StringSeparate( strLine, StrArray );

		int iCount = (int)StrArray.GetCount();
		if( iCount < iHead )
		{
			char szError[ 256 ];
			StringCchPrintf( szError, 256, "GLMapList::LoadCsv include blank column, next item load failed" );
			MessageBox( NULL, _T(szError), _T("Fail"), MB_OK );
			return false;
		}

		SMAPNODE sMapNode;
		sMapNode.LoadCsv( StrArray );

		FIELDMAP_ITER iterfind = m_MapList.find(sMapNode.sNativeID.dwID);
		if ( m_MapList.end() != iterfind )
		{
			iterfind->second = sMapNode;
		}
		else
		{
			m_MapList.insert(std::make_pair(sMapNode.sNativeID.dwID, sMapNode));
		}
	}

	return true;
}

SMAPNODE* GLMapList::FindMapNode ( const SNATIVEID &sNativeID )
{
	FIELDMAP_ITER iter = m_MapList.find ( sNativeID.dwID );
	if ( iter!=m_MapList.end() )	return &(*iter).second;

	return NULL;
}

const char* GLMapList::GetMapName ( const SNATIVEID &sNativeID )
{
	SMAPNODE* pMap = FindMapNode ( sNativeID );
	if ( !pMap )	return NULL;

	return pMap->strMapName.c_str();
}

const SLEVEL_REQUIRE* GLMapList::GetLevelRequire ( const SNATIVEID &sNativeID )
{
	SMAPNODE* pMap = FindMapNode ( sNativeID );
	if ( !pMap )	return NULL;

	return &pMap->sLEVEL_REQUIRE;
}

const SLEVEL_ETC_FUNC* GLMapList::GetLevelEtcFunc ( const SNATIVEID &sNativeID )
{
	SMAPNODE* pMap = FindMapNode ( sNativeID );
	if ( !pMap )	return NULL;

	return &pMap->sLEVEL_ETC_FUNC;
}

HRESULT GLMapList::CleanUp ()
{
	m_MapList.clear ();

	return S_OK;
}

BOOL GLMapList::InsertMapList( SMAPNODE &mapnew )
{
	FIELDMAP_ITER iter = m_MapList.begin ();
	FIELDMAP_ITER iter_end = m_MapList.end ();

	for ( ; iter!=iter_end; ++iter )
	{
		const SMAPNODE &sMapNode = (*iter).second;
		if ( sMapNode.sNativeID==mapnew.sNativeID )		return FALSE;
	}

	m_MapList.insert ( std::make_pair(mapnew.sNativeID.dwID,mapnew) );

	return TRUE;
}

BOOL GLMapList::EraseMapList( SNATIVEID sidNew)
{
	FIELDMAP_ITER iter = m_MapList.find (sidNew.dwID);
	if ( iter != m_MapList.end() )
	{
		m_MapList.erase( iter );
		return TRUE;
	}

	return FALSE;
}