#include "stdafx.h"
#include "./GLPersonalLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void SPERSONAL_LOCK::DeletePinInFo()
{
	memset (szPin, 0, sizeof(szPin));
}

