#pragma once

#include "../../Lib_Engine/G-Logic/GLDefine.h"


struct SMDF_ITEM
{
	SNATIVEID		sItemID;

	SMDF_ITEM()
		: sItemID(false)
	{
	}
};

typedef std::map<DWORD, SMDF_ITEM>		MDF_ITEM_MAP;
typedef MDF_ITEM_MAP::iterator			MDF_ITEM_MAP_ITER;
typedef MDF_ITEM_MAP::const_iterator	MDF_ITEM_MAP_CITER;

struct SMDF_MAP
{
	SNATIVEID		sMapID;
	MDF_ITEM_MAP	mapItem;

	SMDF_MAP()
		: sMapID(false)
	{
	}
};

typedef std::map<DWORD, SMDF_MAP>		MDF_MAP_MAP;
typedef MDF_MAP_MAP::iterator			MDF_MAP_MAP_ITER;
typedef MDF_MAP_MAP::const_iterator		MDF_MAP_MAP_CITER;

class CMapDropFilter
{
public:
	CMapDropFilter();
	~CMapDropFilter();

	static CMapDropFilter& GetInstance ();

public:
	MDF_MAP_MAP	m_mapData;

public:
	void CleanUp();
	void Load(const std::string& strFileName);
	bool Find(const SNATIVEID& mapID, const SNATIVEID& itemID);
};