/*!
 * \file GLPartyFinderDef.h
 *
 * \author Juver
 * \date 2020/01/03
 *
 * 
 */

#ifndef GLPARTYFINDERDEF_H_INCLUDED__
#define GLPARTYFINDERDEF_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "./GLMapNodeDefine.h"
#include "./GLCharDefine.h"

#define SPARTY_FINDER_SEARCH_INDEX_NULL 0xFFFFFFFF
#define SPARTY_FINDER_RESULT_NUM 16

struct SPARTY_FINDER_MAP_DATA
{
	DWORD dwIndex;
	SNATIVEID sMapID;
	std::string strMapName;

	SPARTY_FINDER_MAP_DATA()
		: dwIndex(0)
		, sMapID(false)
		, strMapName("")
	{
	}
};

struct SPARTY_FINDER_RESULT
{
	DWORD dwPartyID;
	DWORD dwPartyMembers;

	WORD wPartyMoneyOption;
	WORD wPartyItemOption;

	DWORD dwLeaderCharID;
	WORD wLeaderSchool;
	WORD wLeaderClass;

	char szLeaderName[CHAR_SZNAME];
	char szMapName[MAP_NAME_MAX];

	SPARTY_FINDER_RESULT()
		: dwPartyID(PARTY_NULL)
		, dwPartyMembers(1)
		, wPartyMoneyOption(0)
		, wPartyItemOption(0)
		, dwLeaderCharID(GAEAID_NULL)
		, wLeaderSchool(0)
		, wLeaderClass(0)
	{
		memset( szLeaderName, 0, sizeof(szLeaderName) );
		memset( szMapName, 0, sizeof(szMapName) );
	}
};

typedef std::vector<SPARTY_FINDER_RESULT>		SPARTY_FINDER_RESULT_VEC;
typedef std::map<DWORD,SPARTY_FINDER_RESULT>	SPARTY_FINDER_RESULT_MAP;
typedef SPARTY_FINDER_RESULT_MAP::iterator		SPARTY_FINDER_RESULT_MAP_ITER;

typedef std::map<DWORD,SPARTY_FINDER_MAP_DATA>	SPARTY_FINDER_MAP_DATA_MAP;
typedef SPARTY_FINDER_MAP_DATA_MAP::iterator	SPARTY_FINDER_MAP_DATA_MAP_ITER;


struct SPARTY_FINDER_QUESTION
{
	DWORD dwPartyID;

	DWORD dwCharID;
	WORD wSchool;
	WORD wClass;
	WORD wLevel;
	char szName[CHAR_SZNAME];

	SPARTY_FINDER_QUESTION ()
		: dwPartyID(PARTY_NULL)
		, dwCharID(GAEAID_NULL)
		, wSchool(0)
		, wClass(0)
		, wLevel(1)
	{
		memset( szName, 0, sizeof(szName) );
	}
};

#endif // GLPARTYFINDERDEF_H_INCLUDED__
