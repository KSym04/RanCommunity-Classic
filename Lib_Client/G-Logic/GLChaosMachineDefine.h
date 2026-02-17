/*!
 * \file GLChaosMachineDefine.h
 *
 * \author Juver
 * \date 2021/07/07
 *
 * 
 */

#ifndef GLCHAOSMACHINEDEFINE_H_INCLUDED__
#define GLCHAOSMACHINEDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>


#include "../Lib_Engine/G-Logic/GLDefine.h"

#define CHAOS_MACHINE_MATERIAL_NUM 3
#define CHAOS_MACHINE_ID_NULL 0

typedef std::vector<SNATIVEID>		SCHAOS_MACHINE_RESULT_ITEM_VEC;

struct SCHAOS_MACHINE_MATERIAL_DATA
{
	SNATIVEID	sItemID;
	WORD		wNum;

	SCHAOS_MACHINE_MATERIAL_DATA()
		: sItemID(NATIVEID_NULL())
		, wNum(1)
	{

	};
};

struct SCHAOS_MACHINE_DATA
{
	DWORD dwRequireGroupID;
	DWORD dwResultGroupID;

	LONGLONG llCost;
	float fChance;

	SCHAOS_MACHINE_MATERIAL_DATA sMaterial[CHAOS_MACHINE_MATERIAL_NUM];

	SCHAOS_MACHINE_RESULT_ITEM_VEC vecResultItem;
	WORD	wResultIndex;
	BOOL	bConsumeMainItem;

	SCHAOS_MACHINE_DATA()
		: dwRequireGroupID(CHAOS_MACHINE_ID_NULL)
		, dwResultGroupID(CHAOS_MACHINE_ID_NULL)
		, llCost(0)
		, fChance(1.0f)
		, wResultIndex(0)
		, bConsumeMainItem(FALSE)
	{
		for ( int i=0; i<CHAOS_MACHINE_MATERIAL_NUM; ++i )
		{
			sMaterial[i] = SCHAOS_MACHINE_MATERIAL_DATA();
		}
	};

	SNATIVEID	getResultItem();
	WORD		getCurrentResultIndex() { return wResultIndex; }
};

typedef std::map<DWORD, SCHAOS_MACHINE_DATA>		SCHAOS_MACHINE_DATA_MAP;
typedef SCHAOS_MACHINE_DATA_MAP::iterator			SCHAOS_MACHINE_DATA_MAP_ITER;



#endif // GLCHAOSMACHINEDEFINE_H_INCLUDED__
