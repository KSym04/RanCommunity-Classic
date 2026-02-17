#ifndef GLMAPLIST_H_
#define GLMAPLIST_H_

#include <hash_map>
#include <string>

#include "./GLLevelFile.h"
#include "./GLogicData.h"

#include "../../Lib_Engine/Common/CList.h"
#include "../../Lib_Network/s_NetGlobal.h"

#include "./GLMapNode.h"


class GLMapList
{
public:
	enum
	{
		VERSION				= 0x0202,	

		ENCODE_VER_BYTE_CRYPT_2021_MAPSLIST_V1	= 0x0202,

		VERSION_ENCODE2		= 0x0201,
		VERSION_ENCODE		= 0x0200,
		VERSION_ENCODE_OLD	= 0x0100,

		MAPLIST_MAX			= 1000,
	};

public:
	static const char*	_FILEHEAD;
	char				m_szFileName[MAX_PATH];

public:
	typedef stdext::hash_map<DWORD,SMAPNODE>	FIELDMAP;
	typedef FIELDMAP::iterator					FIELDMAP_ITER;

	typedef std::vector<DWORD>				VEC_INSTANT_MAPID;
	typedef VEC_INSTANT_MAPID::iterator		VEC_INSTANT_MAPID_ITER;

protected:
	FIELDMAP	m_MapList;

public:
	BOOL IsRegMapNode ( SMAPNODE &MapNode );
	HRESULT LoadMapList_OLD ( const TCHAR* szMapListFile );
	HRESULT CleanUp ();

	HRESULT LoadMapsListFile ( const char* szFile, LPF_SERVER_INFO pServerInfo=NULL, int nServerNum=0, BOOL bTOOL = FALSE );
	HRESULT SaveMapsListFile ( const char* szFile );

	bool	SaveCsv    (std::string& strFileName);
	bool	LoadCsv    (std::string& strFileName);

public:
	SMAPNODE* FindMapNode ( const SNATIVEID &sNativeID );
	const char* GetMapName ( const SNATIVEID &sNativeID );
	const SLEVEL_REQUIRE*  GetLevelRequire ( const SNATIVEID &sNativeID );
	const SLEVEL_ETC_FUNC* GetLevelEtcFunc ( const SNATIVEID &sNativeID );

	FIELDMAP& GetMapList ()						{ return m_MapList; }

	BOOL InsertMapList( SMAPNODE &mapnew );
	BOOL EraseMapList( SNATIVEID sidNew );

public:
	GLMapList(void);
	~GLMapList(void);
};


#endif // GLMAPLIST_H_