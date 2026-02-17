#include "stdafx.h"
#include "./GLChaosMachineDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SNATIVEID SCHAOS_MACHINE_DATA::getResultItem()
{
	wResultIndex ++;

	if ( wResultIndex >= vecResultItem.size() )
		wResultIndex = 0;

	if ( wResultIndex < vecResultItem.size() )
	{
		return vecResultItem[wResultIndex];
	}

	return SNATIVEID();
}