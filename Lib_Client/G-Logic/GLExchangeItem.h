/*!
 * \file GLExchangeItem.h
 *
 * \author Juver
 * \date 2022/04/26
 *
 * 
 */

#ifndef GLEXCHANGEITEM_H_INCLUDED__
#define GLEXCHANGEITEM_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"

namespace nsexchange_item
{
	struct Require
	{
		SNATIVEID sItemID;

		Require()
			: sItemID(false)
		{

		}
	};

	typedef std::map<DWORD, Require> MAP_REQUIRE;
	typedef MAP_REQUIRE::iterator	MAP_REQUIRE_ITER;

	struct Result
	{
		SNATIVEID sItemID;
		WORD wPointCost;

		Result()
			: sItemID(false)
			, wPointCost(0)
		{
		}
	};

	typedef std::map<DWORD, Result> MAP_RESULT;
	typedef MAP_RESULT::iterator	MAP_RESULT_ITER;

	struct Data
	{
		std::string strName;

		MAP_REQUIRE mapRequire;
		MAP_RESULT	mapResult;

		Data()
			: strName("")
		{

		}

		Require* findRequire(DWORD dwID);
		Result* findResult(DWORD dwID);
	};

	typedef std::map<std::string, Data> MAP_DATA;
	typedef MAP_DATA::iterator			MAP_DATA_ITER;

	class DataManager
	{
	public:
		DataManager();
		~DataManager();

		static DataManager& GetInstance ();

	public:
		MAP_DATA m_mapData;

	public:
		void LoadData( std::string strName );
		Data* findData( std::string strName );
	};

};

#endif // GLEXCHANGEITEM_H_INCLUDED__
